/**************************************************************************//**
 * @brief Implementation specific functions for HRM code
 * @version 5.0.0
 ******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
#include <stdio.h>
#include "i2cspm.h"
#include "em_i2c.h"
#include "em_gpio.h"
#include "string.h"
#include "em_usb.h"
#include "rtcdriver.h"
#include "em_gpio.h"
#include "si114x_functions.h"
#include "si114x_sys_out.h"
#include "si114xhrm.h"

#ifdef USB_DEBUG
    #include "usb_debug.h"
#endif

/*  I2C port configuration */
static Si114xPortConfig_t _handle;
/*  Flag to indicate USB debug is enabled  */
static bool     usbDebugEnable;
/*  interrupt sequence counter  */
static uint16_t irqSequence = 0;
/*  interrupt queue size in bytes  */
#define	IRQ_QUEUE_SIZE	270
/*  interrupt queue data */
static u8 IrqQueue[IRQ_QUEUE_SIZE];
/*  interrupt queue current get index  */
static u16 irqQueueGetIndex = 0;
/*  interrupt queue current put index  */
static u16 irqQueuePutIndex = 0;

/*  Non-exported Function Prototypes  */
static s16 Si114x_i2c_smbus_write_byte_data(HANDLE si114x_handle, u8 address, u8 data, bool block);
static s16 Si114x_i2c_smbus_read_byte_data(HANDLE si114x_handle, u8 address, u8 *data, bool block);
static s16 Si114x_i2c_smbus_write_i2c_block_data(HANDLE si114x_handle, u8 address, u8 length, u8 const* values, bool block);
static s16 Si114x_i2c_smbus_read_i2c_block_data(HANDLE si114x_handle, u8 address, u8 length, u8* values, bool block);
static s16 Si114xIrqQueue_Put(SI114X_IRQ_SAMPLE *samples);

/**************************************************************************//**
 * @brief Enables/Disables USB debug output.
 *****************************************************************************/
int si114xSetupDebug(HANDLE si114x_handle, void *si114x_debug)
{
  int *enable_usb_debug = (int *)si114x_debug;
  (void) si114x_handle;
  if(*enable_usb_debug == 1)
    usbDebugEnable = true;
  else if(*enable_usb_debug == 0)
    usbDebugEnable = false;

  return SI114xHRM_SUCCESS;
}

/**************************************************************************//**
 * @brief Prints USB debug output.
 *****************************************************************************/
int si114xOutputDebugMessage(HANDLE si114x_handle, char *message)
{
  uint16_t i;
  uint8_t message_buffer[((99)+3)&~3] SL_ATTRIBUTE_ALIGN(4);
  uint8_t *Message_Buffer[1] = {message_buffer};
  (void) si114x_handle;
  message_buffer[0] = 0x10;
  for(i=0; i<strlen(message); i++)
    message_buffer[i+1] = (uint8_t)message[i];
  message_buffer[strlen(message)+1] = 0x10;
  message_buffer[strlen(message)+2] = 0x0D;
#ifdef USB_DEBUG
  USBD_Write(CDC_EP_DATA_IN, (void*) Message_Buffer[0], strlen(message)+3, NULL);
#endif
  return SI114xHRM_SUCCESS;
}

/**************************************************************************//**
 * @brief Write to Si114x register
 *****************************************************************************/
int16_t Si114xWriteToRegister(HANDLE si114x_handle, uint8_t address, uint8_t data)
{
  return Si114x_i2c_smbus_write_byte_data(si114x_handle, address, data, true);
}

/**************************************************************************//**
 * @brief Read from Si114x register.
 *****************************************************************************/
int16_t Si114xReadFromRegister(HANDLE si114x_handle, uint8_t address)
{
  u8 data;
  Si114x_i2c_smbus_read_byte_data(si114x_handle, address, &data, true);
  return data;
}

/**************************************************************************//**
 * @brief block write to si114x
 *****************************************************************************/
int16_t Si114xBlockWrite(HANDLE si114x_handle,
                     uint8_t address, uint8_t length, uint8_t *values)
{
  return Si114x_i2c_smbus_write_i2c_block_data(si114x_handle,
                                               address,
                                               length,
                                               values,
                                               true);
}

/**************************************************************************//**
 * @brief Block read from Si114x.
 *****************************************************************************/
int16_t Si114xBlockRead(HANDLE si114x_handle,
                    uint8_t address, uint8_t length, uint8_t *values)
{
    return Si114x_i2c_smbus_read_i2c_block_data(si114x_handle,
                           address,    length,     values, true);
}

/**************************************************************************//**
 * @brief Disable GPIO interrupt for Si114x interrupt.
 *****************************************************************************/
void DisableSi114xInterrupt ()
{

	GPIO_IntDisable(1<<_handle.irqPin);
}

/**************************************************************************//**
 * @brief Enable GPIO interrupt for Si114x.
 *****************************************************************************/
void EnableSi114xInterrupt ()
{
	if (GPIO_PinInGet(_handle.irqPort, _handle.irqPin) == 0)
		GPIO_IntSet(1<<_handle.irqPin);
	GPIO_IntEnable(1<<_handle.irqPin);
}

/**************************************************************************//**
 * @brief Main interrupt processing routine for Si114x.
 *****************************************************************************/
s16 Si114xProcessIrq(HANDLE si114x_handle, u16 timestamp)
{
  u8 data_buffer[13];
  s16 error;
  SI114X_IRQ_SAMPLE sample;
  irqSequence++;
  Si114x_i2c_smbus_read_i2c_block_data(si114x_handle, 0x21, 13, data_buffer, false);		// Grab all data registers
  Si114x_i2c_smbus_write_byte_data(si114x_handle, 0x21, data_buffer[0], false);      // Clear interrupts

  sample.sequence = irqSequence;       // sequence number
  sample.timestamp = timestamp;      // 16-bit Timestamp to record
  sample.pad= 0;
  sample.irqstat = data_buffer[0];        // 8-bit irq status
  sample.vis = (((u16)(data_buffer[2]) << 8) & 0xff00) | data_buffer[1];            // VIS
  sample.ir = (((u16)(data_buffer[4]) << 8) & 0xff00) | data_buffer[3];             // IR
  sample.ps1 = (((u16)(data_buffer[6]) << 8) & 0xff00) | data_buffer[5];            // PS1
  sample.ps2 = (((u16)(data_buffer[8]) << 8) & 0xff00) | data_buffer[7];            // PS2
  sample.ps3 = (((u16)(data_buffer[10]) << 8) & 0xff00) | data_buffer[9];            // PS3
  sample.aux = (((u16)(data_buffer[12]) << 8) & 0xff00) | data_buffer[11];;            // AUX

  error = Si114xIrqQueue_Put(&sample);
  return error;
}


/**************************************************************************//**
 * @brief Query number of entries in the interrupt queue.
 *****************************************************************************/
s16 Si114xIrqQueueNumentries(HANDLE si114x_handle)
{
  (void) si114x_handle;
  u16 runnerIndex = irqQueueGetIndex;
  s16 count=0;
  while (runnerIndex != irqQueuePutIndex)
  {
    runnerIndex++;
    count++;
    if(runnerIndex == IRQ_QUEUE_SIZE)
      runnerIndex = 0;
  }
  return (count/sizeof(SI114X_IRQ_SAMPLE));

}

/**************************************************************************//**
 * @brief Get sample from the interrupt queue.
 *****************************************************************************/
s16 Si114xIrqQueue_Get(HANDLE si114x_handle, SI114X_IRQ_SAMPLE *samples)
{
  (void) si114x_handle;
  int16_t error = 0;
  uint16_t i;
  int8_t *data = (int8_t *)samples;
  DisableSi114xInterrupt ();
  if (irqQueueGetIndex == irqQueuePutIndex)
    error = -1;
  else
  {
    for(i=0; i<sizeof(SI114X_IRQ_SAMPLE); i++)
    {
      data[i] = IrqQueue[irqQueueGetIndex];
      irqQueueGetIndex++;
      if(irqQueueGetIndex == IRQ_QUEUE_SIZE)
        irqQueueGetIndex = 0;

    }

  }
  EnableSi114xInterrupt();
#ifdef USB_DEBUG
  if (usbDebugEnable == 1)                                    // Dump data to USB if usb_debug is enabled
    USBDebug_ProcessUSBOutput(data);
#endif // si114xHRM_USB_DEBUG
  return error;
}

/**************************************************************************//**
 * @brief Put new sample in the interrupt queue.
 *****************************************************************************/
static s16 Si114xIrqQueue_Put(SI114X_IRQ_SAMPLE *samples)
{
  uint16_t i;
  u8 *data = (u8 *)samples;
  for(i=0; i<sizeof(SI114X_IRQ_SAMPLE); i++)
  {
    IrqQueue[irqQueuePutIndex] = data[i];
    irqQueuePutIndex++;
    if(irqQueuePutIndex == IRQ_QUEUE_SIZE)
      irqQueuePutIndex = 0;
  }
  if (irqQueueGetIndex == irqQueuePutIndex)
  {
    irqQueueGetIndex += sizeof(SI114X_IRQ_SAMPLE);			// if we have wrapped around then we must delete one sample
    return -1; //indicate to caller something bad happened
  }
  return 0;
}

/**************************************************************************//**
 * @brief Empty the interrupt queue.
 *****************************************************************************/
s16 Si114xIrqQueue_Clear(HANDLE si114x_handle)
{
  (void) si114x_handle;
  irqQueueGetIndex = 0;
  irqQueuePutIndex = 0;
  return 0;
}

/**************************************************************************//**
 * @brief Initialize low level handle and clear irq queue.
 *****************************************************************************/
s16 Si114xInit(void *port, int options, HANDLE *si114x_handle)
{
  s16 error = 0;
  u8 data;

  (void) options;

  *si114x_handle = (HANDLE)&_handle;
  _handle.i2cPort = ((Si114xPortConfig_t*)port)->i2cPort;
  _handle.i2cAddress = ((Si114xPortConfig_t*)port)->i2cAddress << 1;
  _handle.irqPort = ((Si114xPortConfig_t*)port)->irqPort;
  _handle.irqPin = ((Si114xPortConfig_t*)port)->irqPin;

  data = Si114xReadFromRegister(*si114x_handle, REG_PART_ID);

  if ((_handle.i2cAddress == (0x60 << 1)) && (data != 0x46) && (data != 0x47))
    error = -1;
  if ((_handle.i2cAddress == (0x5A << 1)) && (data != 0x43))
    error = -1;

  Si114xIrqQueue_Clear(*si114x_handle);

  return error;
}

/**************************************************************************//**
 * @brief Close Si114x.
 *****************************************************************************/
s16 Si114xClose(HANDLE si114x_handle)
{
  (void) si114x_handle;
  _handle.i2cAddress = 0xff;
  return 0;
}

/**************************************************************************//**
 * @brief Reset not implemented.
 *****************************************************************************/
s16 Si114xSysReset(HANDLE si114x_handle)
{
  (void) si114x_handle;
  return 0;
}

/**************************************************************************//**
 * @brief 10ms delay required by Si114x reset sequence.
 *****************************************************************************/
void delay_10ms(void)
{
  RTCDRV_Delay(10);
  return;
}

/**************************************************************************//**
 * @brief 1ms delay required by Si114x polling sequence.
 *****************************************************************************/
void delay_1ms(void)
{
  RTCDRV_Delay(1);
  return;
}


/**************************************************************************//**
 * @brief Write to Si114x i2c.
 *****************************************************************************/
static s16 Si114x_i2c_smbus_write_byte_data(HANDLE si114x_handle, u8 address, u8 data, bool block)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  Si114xPortConfig_t* handle;
  uint8_t i2c_write_data[2];
  uint8_t i2c_read_data[1];

  if (block)
    DisableSi114xInterrupt();
  seq.addr  = _handle.i2cAddress;
  seq.flags = I2C_FLAG_WRITE;
  /* Select register and data to write */
  i2c_write_data[0] = address;
  i2c_write_data[1] = data;
  seq.buf[0].data = i2c_write_data;
  seq.buf[0].len  = 2;
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 0;

  handle = (Si114xPortConfig_t *)si114x_handle;

  ret = I2CSPM_Transfer(handle->i2cPort, &seq);

  if (block)
    EnableSi114xInterrupt();
  if (ret != i2cTransferDone)
  {
    return (s16)ret;
  }
  return (s16)0;
}

/**************************************************************************//**
 * @brief Read from Si114x i2c.
 *****************************************************************************/
static s16 Si114x_i2c_smbus_read_byte_data(HANDLE si114x_handle, u8 address, u8 *data, bool block)
{
  //  si114x_handle is not used in the EFM32.   We use a global instead
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[1];
  Si114xPortConfig_t* i2cDrvHandle;

  if (block)
    DisableSi114xInterrupt ();
  seq.addr  = _handle.i2cAddress;
  seq.flags = I2C_FLAG_WRITE_READ;
  /* Select register to start reading from */
  i2c_write_data[0] = address;
  seq.buf[0].data = i2c_write_data;
  seq.buf[0].len  = 1;
  /* Select length of data to be read */
  seq.buf[1].data = data;
  seq.buf[1].len  = 1;

  i2cDrvHandle = (Si114xPortConfig_t *)si114x_handle;

  ret = I2CSPM_Transfer(i2cDrvHandle->i2cPort, &seq);

  if (block)
    EnableSi114xInterrupt();
  if (ret != i2cTransferDone)
  {
    *data = 0xff;
    return((int) ret);
  }
  return((int) 1);
}

/**************************************************************************//**
 * @brief Write block of data to Si114x i2c.
 *****************************************************************************/
static s16 Si114x_i2c_smbus_write_i2c_block_data(HANDLE si114x_handle, u8 address, u8 length, u8 const* data, bool block)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[10];
  uint8_t i2c_read_data[1];
  Si114xPortConfig_t* handle;
  int i;

  if (block)
    DisableSi114xInterrupt ();
  seq.addr  = _handle.i2cAddress;
  seq.flags = I2C_FLAG_WRITE;
  /* Select register to start writing to*/
  i2c_write_data[0] = address;
  for (i=0; i<length;i++)
  {
    i2c_write_data[i+1] = data[i];
  }
  seq.buf[0].data = i2c_write_data;
  seq.buf[0].len  = 1+length;
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 0;

  handle = (Si114xPortConfig_t *)si114x_handle;

  ret = I2CSPM_Transfer(handle->i2cPort, &seq);

  if (block)
    EnableSi114xInterrupt ();
  if (ret != i2cTransferDone)
  {
    return((int) ret);
  }

  return((int) 0);
}

/**************************************************************************//**
 * @brief Read block of data from Si114x i2c.
 *****************************************************************************/
static s16 Si114x_i2c_smbus_read_i2c_block_data(HANDLE si114x_handle, u8 address, u8 length, u8* data, bool block)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[1];
  Si114xPortConfig_t* handle;

  seq.addr  = _handle.i2cAddress;
  seq.flags = I2C_FLAG_WRITE_READ;
  if (block)
    DisableSi114xInterrupt ();
  /* Select register to start reading from */
  i2c_write_data[0] = address;
  seq.buf[0].data = i2c_write_data;
  seq.buf[0].len  = 1;

  /* Select length of data to be read */
  seq.buf[1].data = data;
  seq.buf[1].len  = length;

  handle = (Si114xPortConfig_t *)si114x_handle;

  ret = I2CSPM_Transfer(handle->i2cPort, &seq);

  if (block)
    EnableSi114xInterrupt ();
  if (ret != i2cTransferDone)
  {
    return((int) ret);
  }

  return((int) 0);
}

int si114xFindEvb(char *port_description, char *last_port, int num_ports_found)
{
	(void) port_description;
	(void) last_port;
	(void) num_ports_found;
	return 0;
}

