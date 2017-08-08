/**
  ******************************************************************************
  * @file    I2C/Interrupt_MasterTransmitter&SlaveReceiver/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program body.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include <stdio.h>
#include "I2C_IE.h"

/* Private typedef -----------------------------------------------------------*/
#define BUFFER_SIZE        0x10  /*!< Write or read buffer size */

/* Private variables ---------------------------------------------------------*/
uint8_t I2C_Buf_Write[BUFFER_SIZE];
uint8_t I2C_Buf_Read[BUFFER_SIZE];

volatile uint8_t*    I2C_WriteBuffer;
volatile uint8_t*    I2C_ReadBuffer;
volatile uint16_t    I2C_nBytes;
volatile OP_Status   Status;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void LED_GPIO_Configuration(void);
void I2C_GPIO_Configuration(void);
void I2C_Configuration(void);
void I2C_NVIC_Configuraion(void);

/* Private function ----------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    int i;
    int equal_flag=1;/*!< Equal flag of the value of I2C_Buf_Write array and I2C_Buf_Read array */

    /* Clock enable */
    RCC_Configuration();

    /* Config LED GPIOs */
    LED_GPIO_Configuration();

    /* Config I2C1 and I2C2 GPIOs */
    I2C_GPIO_Configuration();

    /* Config I2C1 and I2C2 */
    I2C_Configuration();

    /* Config NVIC */
    I2C_NVIC_Configuraion();

    /* Fill the I2C_Buf_Write transmit data buffer */
    for(i=0;i<BUFFER_SIZE;i++)
    {
        I2C_Buf_Write[i]=i+0x80;
    }

    I2C_WriteBuffer=I2C_Buf_Write;
    I2C_ReadBuffer=I2C_Buf_Read;
    I2C_nBytes=BUFFER_SIZE;
    Status=start;

    /* Enable the I2C1 interrupt */
    I2C_INTConfig(I2C1, I2C_INT_EIE |I2C_INT_EE| I2C_INT_BIE, ENABLE);

    /* Enable the I2C2 interrupt */
    I2C_INTConfig(I2C2, I2C_INT_EIE |I2C_INT_EE| I2C_INT_BIE, ENABLE);

    /* The master waits until the I2C bus is idle */
    while(I2C_GetBitState(I2C1, I2C_FLAG_I2CBSY));

    /* The master sends a start condition to I2C bus */
    I2C_StartOnBus_Enable(I2C1, ENABLE);

    /* Wait all the data transmitted */
    while((I2C_nBytes>0))
    {
    }
    while(Status!=end)
    {
    }

    /* Verify whether the correct transmission */
    for(i=0;i<BUFFER_SIZE;i++)
    {
        if(I2C_Buf_Write[i]!=I2C_Buf_Read[i])
        {
            equal_flag=0;
            break;
        }
    }
    if(equal_flag==1) /*!< Transmit correct */
    {
        /* Turn on the LED2 and LED3 */
        GPIO_SetBits(GPIOC, GPIO_PIN_0);
        GPIO_SetBits(GPIOC, GPIO_PIN_2);
    }
    else   /*!< Transmit error */
    {
        /* Turn on all the LEDs */
        GPIO_SetBits(GPIOC, GPIO_PIN_0 | GPIO_PIN_2);
        GPIO_SetBits(GPIOE, GPIO_PIN_0 | GPIO_PIN_1);
    }
    while(1);
}

/**
  * @brief  Enable the peripheral clock.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    /* Enable GPIOB,GPIOC,GPIOE clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOB,ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC,ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOE,ENABLE);

    /* I2C1 APB1 clock */
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_I2C1,ENABLE);

    /* I2C2 APB1 clock */
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_I2C2,ENABLE);
}

/**
  * @brief  Configure the LED GPIO ports.
  * @param  None
  * @retval None
  */
void LED_GPIO_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0 | GPIO_PIN_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    /* Turn off all the LEDs */
    GPIO_ResetBits(GPIOC, GPIO_PIN_0 | GPIO_PIN_2);
    GPIO_ResetBits(GPIOE, GPIO_PIN_0 | GPIO_PIN_1);
}

/**
  * @brief  Cofigure the GPIO Ports.
  * @param  None
  * @retval None
  */
void I2C_GPIO_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;

    /* I2C1 and I2C2 GPIO ports */
    GPIO_InitStructure.GPIO_Pin =  GPIO_PIN_6 | GPIO_PIN_7|GPIO_PIN_10 | GPIO_PIN_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  Cofigure the I2C1 and I2C2 interfaces.
  * @param  None
  * @retval None
  */
void I2C_Configuration(void)
{
    /* I2C1 and I2C2 periphral configuration */
    I2C_InitPara I2C_InitStructure;
    I2C_InitStructure.I2C_Protocol = I2C_PROTOCOL_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DUTYCYCLE_2;
    I2C_InitStructure.I2C_BitRate = 100000;
    I2C_InitStructure.I2C_AddressingMode = I2C_ADDRESSING_MODE_7BIT;
    I2C_InitStructure.I2C_DeviceAddress = I2C1_SLAVE_ADDRESS7;
    
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Enable(I2C1,ENABLE);
    I2C_Acknowledge_Enable(I2C1,ENABLE);

    I2C_InitStructure.I2C_DeviceAddress = I2C2_SLAVE_ADDRESS7;
    I2C_Init(I2C2,&I2C_InitStructure);
    I2C_Enable(I2C2,ENABLE);
    I2C_Acknowledge_Enable(I2C2,ENABLE);
}

/**
  * @brief  Cofigure the NVIC peripheral.
  * @param  None
  * @retval None
  */
void I2C_NVIC_Configuraion(void)
{

    NVIC_InitPara NVIC_InitStructure;
    /* 1 bits for pre-emption priority and 3 bits for subpriority */
    NVIC_PRIGroup_Enable(NVIC_PRIGROUP_1);

    /* I2C1 event interrupt */
    NVIC_InitStructure.NVIC_IRQ = I2C1_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
    NVIC_InitStructure.NVIC_IRQSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* I2C1 error interrupt */
    NVIC_InitStructure.NVIC_IRQ = I2C1_ER_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
    NVIC_InitStructure.NVIC_IRQSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* I2C2 event interrupt */
    NVIC_InitStructure.NVIC_IRQ = I2C2_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
    NVIC_InitStructure.NVIC_IRQSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* I2C2 error interrupt */
    NVIC_InitStructure.NVIC_IRQ = I2C2_ER_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
    NVIC_InitStructure.NVIC_IRQSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
