/*
 * Copyright (c) 2015-2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!****************************************************************************
 *  @file       I2C.h
 *  @brief      Inter-Intergrated Circuit driver interface.
 *
 *  To use the I2C driver, ensure that the correct driver library for your
 *  device is linked in and include this header file as follows:
 *  @code
 *  #include <ti/drivers/I2C.h>
 *  @endcode
 *
 *  This module serves as the main interface for applications using an
 *  underlying I2C peripheral. Its purpose is to redirect the I2C APIs to
 *  device specific driver implementations which are specified using a pointer
 *  to a #I2C_FxnTable.
 *
 *  # Overview #
 *
 *  This section assumes that you have prior knowledge about the I2C protocol.
 *  For the full I2C-bus specification and user manual, view the \b UM10204
 *  document available online.
 *
 *  This I2C driver is designed to operate as an I2C master. This driver
 *  does not support I2C slave mode. This driver assumes it is the only I2C
 *  master on the I2C bus. Using the I2C APIs, you can transmit and recieve
 *  data over an I2C bus. The I2C-bus specification does not define how data
 *  is to be formatted or handled. This allows for flexible implementations
 *  across different peripheral vendors. As a result, this driver only
 *  performs the exchange of data between the master and slave(s). The
 *  application is responsible for manipulating and interpretting the data.
 *
 *  # Thread Safety #
 *
 *  This driver has been designed to operate in a Real-Time Operating System
 *  (RTOS) environment. This driver protects data transactions with operating
 *  system primitives supplied by the underlying RTOS.
 *
 *  # Usage #
 *
 *  The following code example opens an I2C instance.
 *
 *  @code
 *  I2C_Handle i2cHandle;
 *  I2C_Params i2cParams;
 *
 *  i2cHandle = I2C_open(Board_I2C0, NULL);
 *
 *  if (i2cHandle == NULL) {
 *      // Error opening I2C
 *      while (1);
 *  }
 *  @endcode
 *
 *  ## I2C Driver Configuration ##
 *
 *  In order to use the I2C APIs, the application is required to provide
 *  device-specific I2C configuration in the Board.c file. The I2C driver
 *  interface defines a configuration data structure, #I2C_Config.
 *
 *  The application must declare an array of #I2C_Config elements, named
 *  \p I2C_config[]. Each element of \p I2C_config[] is populated with
 *  pointers to a device specific I2C driver implementation's function
 *  table, driver object, and hardware attributes. The hardware attributes
 *  define properties such as the I2C peripheral's base address and
 *  pins. Each element in \p I2C_config[] corresponds to an I2C instance,
 *  and none of the elements should have \p NULL pointers.
 *
 *  The I2C configuration is device dependent. You will need to check the
 *  device specific I2C driver documentation. There you will find a
 *  description of the I2C hardware attributes.
 *
 *  ## Initializing the I2C Driver ##
 *
 *  I2C_init() must be called before any other I2C APIs. This function
 *  calls the device specific implementation's I2C initialization function
 *  for each element of \p NVS_config[].
 *
 *  ## Opening the I2C Driver ##
 *
 *  After initializing the I2C driver by calling I2C_init(), the application
 *  can open an I2C instance by calling I2C_open(). This function
 *  takes an index into the \p I2C_config[] array and an #I2C_Params structure.
 *  The #I2C_Handle returned from the I2C_open() is then associated with that
 *  index into the \p I2C_config[] array.
 *
 *  \note Each I2C index can only be opened exclusively. Calling I2C_open()
 *  multiple times with the same index will result in an error. The index can
 *  be re-used if I2C_close() is called first.
 *
 *  This example shows opening an I2C driver instance in callback mode
 *  with a bit rate of 400kbps.
 *
 *  @code
 *  I2C_Handle i2cHandle;
 *  I2C_Params i2cParams;
 *
 *  I2C_Params_init(&i2cParams);
 *
 *  i2cParams.transferMode = I2C_MODE_CALLBACK;
 *  i2cParams.transferCallbackFxn = myCallbackFunction;
 *  i2cParams.bitRate = I2C_400kHz;
 *
 *  handle = I2C_open(Board_I2C0, &i2cParams);
 *
 *  if (i2cHandle == NULL) {
 *      // Error opening I2C
 *      while (1);
 *  }
 *  @endcode
 *
 *  ## Transferring data ##
 *
 *  An I2C data transfer is performed using the I2C_transfer() function. Three
 *  types of transactions are supported: write, read, and write + read. The
 *  details of each transaction are specified with an #I2C_Transaction
 *  structure. Each transfer is completed before another transfer is initiated.
 *
 *  For write + read transactions, the specified data is first written to the
 *  peripheral, then a repeated start is sent by the driver, which initiates
 *  the read operation. This type of transfer is useful if an I2C peripheral
 *  has a pointer register that needs to be adjusted prior to reading from
 *  the referenced data register.
 *
 *  The below example shows sending three bytes of data to a slave peripheral
 *  at address 0x50, in blocking mode:
 *
 *  @code
 *  uint8_t writeBuffer[3];
 *  I2C_Transaction i2cTransaction;
 *
 *  i2cTransaction.slaveAddress = 0x50;
 *  i2cTransaction.writeBuf = writeBuffer;
 *  i2cTransaction.writeCount = 3;
 *  i2cTransaction.readBuf = NULL;
 *  i2cTransaction.readCount = 0;
 *
 *  status = I2C_transfer(i2cHandle, &i2cTransaction);
 *
 *  if (status == False) {
 *      // Unsuccessful I2C transfer
 *  }
 *  @endcode
 *
 *  The next example shows reading five bytes of data from the I2C
 *  peripheral, in blocking mode:
 *
 *  @code
 *  uint8_t readBuffer[5];
 *  I2C_Transaction i2cTransaction;
 *
 *  i2cTransaction.slaveAddress = 0x50;
 *  i2cTransaction.writeBuf = NULL;
 *  i2cTransaction.writeCount = 0;
 *  i2cTransaction.readBuf = readBuffer;
 *  i2cTransaction.readCount = 5;
 *
 *  status = I2C_transfer(i2cHandle, &i2cTransaction);
 *
 *  if (status == False) {
 *      // Unsuccessful I2C transfer
 *  }
 *  @endcode
 *
 *  This example shows writing two bytes and reading four bytes in a
 *  single transaction.
 *
 *  @code
 *  uint8_t readBuffer[4];
 *  uint8_t writeBuffer[2];
 *  I2C_Transaction i2cTransaction;
 *
 *  i2cTransaction.slaveAddress = 0x50;
 *  i2cTransaction.writeBuf = writeBuffer;
 *  i2cTransaction.writeCount = 2;
 *  i2cTransaction.readBuf = readBuffer;
 *  i2cTransaction.readCount = 4;
 *
 *  status = I2C_transfer(i2cHandle, &i2cTransaction);
 *
 *  if (status == False) {
 *      // Unsuccessful I2C transfer
 *  }
 *  @endcode
 *
 *  This final example shows usage of asynchronous callback mode, with queuing
 *  of multiple transactions. Because multiple transactions are simultaneously
 *  queued, separate #I2C_Transaction structures must be used.
 *
 *  \note #I2C_Transaction structures cannot be \b re-used until the previous
 *        transaction has completed.
 *
 *  First, the callback function specified by #I2C_Params.transferCallbackFxn
 *  is created. In this example, the #I2C_Transaction will contain a custom
 *  application argument. This argument will be a semaphore handle. The
 *  #I2C_Transaction.arg will point to the semaphore handle. When the callback
 *  function is called, the #I2C_Transaction.arg is checked for \p NULL. If
 *  this value is not \p NULL, then it can be assumed the \p arg is pointing
 *  to a valid semaphore handle. The semaphore handle is then used to call
 *  \p sem_post(). Hypothetically, this can be used to signal transaction
 *  completion to the task(s) that queued the transaction(s).
 *
 *  @code
 *  void callbackFxn(I2C_Handle handle, I2C_Transaction *msg, bool transfer)
 *  {
 *
 *      // Check for a semaphore handle
 *      if (msg->arg != NULL) {
 *
 *          // Perform a semaphore post
 *          sem_post((sem_t *) (msg->arg));
 *      }
 *  }
 *  @endcode
 *
 *  Snippets of the task code that initiates the transactions are shown below.
 *  Note the use of multiple #I2C_Transaction structures. The handle of the
 *  semaphore to be posted is specified via \p i2cTransaction2.arg.
 *  I2C_transfer() is called three times to initiate each transaction.
 *  Since callback mode is used, these functions return immediately. After
 *  the transactions have been queued, other work can be done. Eventually,
 *  \p sem_wait() is called causing the task to block until the transaction
 *  completes. When the transaction completes, the application's callback
 *  callback function, \p callbackFxn will be called. Once \p callbackFxn
 *  above posts the semaphore the task will be unblocked and can resume
 *  execution.
 *
 *  @code
 *  void taskfxn(arg0, arg1)
 *  {
 *
 *      I2C_Transaction i2cTransaction0;
 *      I2C_Transaction i2cTransaction1;
 *      I2C_Transaction i2cTransaction2;
 *
 *      // ...
 *
 *      i2cTransaction0.arg = NULL;
 *      i2cTransaction1.arg = NULL;
 *      i2cTransaction2.arg = semaphoreHandle;
 *
 *      // ...
 *
 *      I2C_transfer(i2c, &i2cTransaction0);
 *      I2C_transfer(i2c, &i2cTransaction1);
 *      I2C_transfer(i2c, &i2cTransaction2);
 *
 *      // ...
 *
 *      sem_wait(semaphoreHandle);
 *  }
 *  @endcode
 *
 *  # Implementation #
 *
 *  This top-level I2C module serves as the main interface for RTOS
 *  applications. Its purpose is to redirect the module's APIs to specific
 *  peripheral implementations which are specified using a pointer to an
 *  #I2C_FxnTable.
 *
 *  The I2C driver interface module is joined (at link time) to an
 *  array of #I2C_Config data structures named \p I2C_config.
 *  \p I2C_config is typically defined in the Board.c file used for the
 *  application. If there are multiple instances of I2C peripherals on the
 *  device, there will typically be multiple #I2C_Config structures defined in
 *  the board file in the form of an array. Each entry in \p I2C_config
 *  contains a:
 *  - #I2C_FxnTable pointer to a set of functions that implement an I2C
 *    peripheral.
 *  - (\p void *) data object that is associated with the #I2C_FxnTable
 *  - (\p void *) hardware attributes that are associated to the #I2C_FxnTable
 *
 *
 ******************************************************************************
 */

#ifndef ti_drivers_I2C__include
#define ti_drivers_I2C__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 *  @defgroup I2C_CONTROL I2C_control command and status codes
 *  These I2C macros are reservations for I2C.h
 *  @{
 */

/*!
 * Common I2C_control command code reservation offset.
 * I2C driver implementations should offset command codes with
 * #I2C_CMD_RESERVED growing positively
 *
 * Example implementation specific command codes:
 * @code
 * #define I2CXYZ_CMD_COMMAND0      I2C_CMD_RESERVED + 0
 * #define I2CXYZ_CMD_COMMAND1      I2C_CMD_RESERVED + 1
 * @endcode
 */
#define I2C_CMD_RESERVED           (32)

/*!
 * Common I2C_control status code reservation offset.
 * I2C driver implementations should offset status codes with
 * #I2C_STATUS_RESERVED growing negatively.
 *
 * Example implementation specific status codes:
 * @code
 * #define I2CXYZ_STATUS_ERROR0     I2C_STATUS_RESERVED - 0
 * #define I2CXYZ_STATUS_ERROR1     I2C_STATUS_RESERVED - 1
 * #define I2CXYZ_STATUS_ERROR2     I2C_STATUS_RESERVED - 2
 * @endcode
 */
#define I2C_STATUS_RESERVED        (-32)

/**
 *  @defgroup I2C_STATUS Status Codes
 *  I2C_STATUS_* macros are general status codes returned by I2C_control()
 *  @{
 *  @ingroup I2C_CONTROL
 */

/*!
 * @brief   Successful status code returned by I2C_control().
 *
 * I2C_control() returns #I2C_STATUS_SUCCESS if the control code was executed
 * successfully.
 */
#define I2C_STATUS_SUCCESS         (0)

/*!
 * @brief   Generic error status code returned by I2C_control().
 *
 * I2C_control() returns #I2C_STATUS_ERROR if the control code was not executed
 * successfully.
 */
#define I2C_STATUS_ERROR           (-1)

/*!
 * @brief   An error status code returned by I2C_control() for undefined
 * command codes.
 *
 * I2C_control() returns #I2C_STATUS_UNDEFINEDCMD if the control code is not
 * recognized by the driver implementation.
 */
#define I2C_STATUS_UNDEFINEDCMD    (-2)
/** @}*/

/**
 *  @defgroup I2C_CMD Command Codes
 *  I2C_CMD_* macros are general command codes for I2C_control(). Not all I2C
 *  driver implementations support these command codes.
 *  @{
 *  @ingroup I2C_CONTROL
 */

/* Add I2C_CMD_<commands> here */

/** @} end I2C commands */

/** @} end I2C_CONTROL group */

/*!
 *  @brief      A handle that is returned from an I2C_open() call.
 */
typedef struct I2C_Config_ *I2C_Handle;

/*!
 *  @brief     This structure defines the I2C slave address, pointers to write
 *             and read buffers, and their associated byte counts. If no data
 *             needs to be written, the write byte count should be zero.
 *             Similarly, if no data needs to be read, the read byte count
 *             should be set to zero.
 *
 *  @sa I2C_transfer()
 */
typedef struct I2C_Transaction_ {
    void         *writeBuf;    /*!< Pointer to a buffer containing data to be
                                    written. */
    size_t        writeCount;  /*!< Number of bytes to write from the
                                    \p writeBuf. */
    void         *readBuf;     /*!< Pointer to a buffer to store data read. */
    size_t        readCount;   /*!< Number of bytes to be read from the slave */

    uint_least8_t slaveAddress; /*!< Address of the I2C slave peripheral. */

    void         *arg;         /*!< Optional application argument. This argument
                                    will be passed to the callback function
                                    specified by #I2C_Params.transferCallbackFxn
                                    when using #I2C_MODE_CALLBACK. */
    void         *nextPtr;     /*!< This value is used internally by the driver
                                    and must never be modified by the
                                    application. */
} I2C_Transaction;

/*!
 *  @brief    This I2C driver supports two transfer modes of operation:
 *            blocking and callback. The transfer mode is specified using
 *            the #I2C_Params structure. The transfer mode defines how the
 *            I2C_transfer() function behaves for a driver instance.
 *
 *  \note Once an I2C driver instance is opened, the transfer mode can NOT be
 *        changed. The driver instance can be closed and re-open with a new
 *        transfer mode. See I2C_open() and I2C_close().
 */
typedef enum I2C_TransferMode_ {
    I2C_MODE_BLOCKING,  /*!< In blocking mode, a task calling I2C_transfer()
                             is blocked until the transaction completes. Other
                             tasks requesting I2C transactions while a
                             transaction is currently taking place, are also
                             placed into a blocked state. */
    I2C_MODE_CALLBACK   /*!< In callback mode, a task calling I2C_transfer()
                             is not blocked. The application's callback
                             function, #I2C_Params.transferCallbackFxn, is
                             called when the transaction is complete. The
                             callback function will be called from either a
                             hardware or software interrupt context. This
                             depends on the device specific driver
                             implementation. Sequential calls to I2C_transfer()
                             will put #I2C_Transaction structures onto an
                             internal queue that automatically starts queued
                             transactions after the previous transaction has
                             completed. This queuing occurs regardless of any
                             error state from previous transactions. The
                             application callback function will be called as
                             each transaction is completed. */
} I2C_TransferMode;

/*!
 *  @brief    I2C callback function prototype. The application is responsible
 *            for declaring a callback function when using #I2C_MODE_CALLBACK.
 *
 *  @param  handle              Handle to the I2C instance that called the
 *                              I2C_transfer().
 *
 *  @param  transaction         Pointer to the #I2C_Transaction that just
 *                              completed.
 *
 *  @param  transferStatus      Result of the I2C transfer.
 */
typedef void (*I2C_CallbackFxn)(I2C_Handle handle, I2C_Transaction *transaction,
    bool transferStatus);

/*!
 *  @brief  Specifies one of the standard I2C bus bit rates for I2C
 *          communication. You must check that the device specific
 *          implementation supports the desired #I2C_BitRate_.
 */
typedef enum I2C_BitRate_ {

    I2C_100kHz     = 0,     /*!< 100kbps  */
    I2C_400kHz     = 1,     /*!< 400kbps  */
    I2C_1000kHz    = 2,     /*!< 1Mbps    */
    I2C_3330kHz    = 3,     /*!< 3.33Mbps */
} I2C_BitRate;

/*!
 *  @brief  I2C Parameters
 *
 *  I2C parameters are used with the I2C_open() call. Default values for
 *  these parameters are set using I2C_Params_init().
 *
 *  @sa     I2C_Params_init()
 */
typedef struct I2C_Params_ {
    I2C_TransferMode transferMode;        /*!<Specifies the #I2C_TransferMode.
                                              Default is blocking. */
    I2C_CallbackFxn  transferCallbackFxn; /*!< Callback function pointer used
                                               when #I2C_TransferMode is
                                               #I2C_MODE_CALLBACK. */
    I2C_BitRate      bitRate;             /*!< #I2C_BitRate_. The default is
                                               #I2C_100kHz.*/
    void            *custom;              /*!< Custom argument used by driver
                                               implementation */
} I2C_Params;

/*!
 *  @brief      A function pointer to a driver-specific implementation of
 *              I2C_cancel().
 */
typedef void (*I2C_CancelFxn) (I2C_Handle handle);

/*!
 *  @brief      A function pointer to a driver-specific implementation of
 *              I2C_close().
 */
typedef void (*I2C_CloseFxn) (I2C_Handle handle);

/*!
 *  @brief      A function pointer to a driver-specific implementation of
 *              I2C_control().
 */
typedef int_fast16_t (*I2C_ControlFxn) (I2C_Handle handle, uint_fast16_t cmd,
    void *controlArg);

/*!
 *  @brief      A function pointer to a driver-specific implementation of
 *              I2C_init().
 */
typedef void (*I2C_InitFxn) (I2C_Handle handle);

/*!
 *  @brief      A function pointer to a driver-specific implementation of
 *              I2C_open().
 */
typedef I2C_Handle (*I2C_OpenFxn) (I2C_Handle handle, I2C_Params *params);

/*!
 *  @brief      A function pointer to a driver-specific implementation of
 *              I2C_transfer().
 */
typedef bool (*I2C_TransferFxn) (I2C_Handle handle,
    I2C_Transaction *transaction);

/*!
 *  @brief      The definition of an I2C function table that contains the
 *              required set of functions to control a specific I2C driver
 *              implementation.
 */
typedef struct I2C_FxnTable_ {
    /*! Cancel all I2C data transfers */
    I2C_CancelFxn   cancelFxn;

    /*! Close the specified peripheral */
    I2C_CloseFxn    closeFxn;

    /*! Implementation-specific control function */
    I2C_ControlFxn  controlFxn;

    /*! Initialize the given data object */
    I2C_InitFxn     initFxn;

    /*! Open the specified peripheral */
    I2C_OpenFxn     openFxn;

    /*! Initiate an I2C data transfer */
    I2C_TransferFxn transferFxn;
} I2C_FxnTable;

/*!
 *  @brief  I2C global configuration
 *
 *  The #I2C_Config structure contains a set of pointers used to characterize
 *  the I2C driver implementation.
 *
 *  This structure needs to be defined before calling I2C_init() and it must
 *  not be changed thereafter.
 *
 *  @sa     I2C_init()
 */
typedef struct I2C_Config_ {
    /*! Pointer to a table of driver-specific implementations of I2C APIs */
    I2C_FxnTable const *fxnTablePtr;

    /*! Pointer to a driver-specific data object */
    void               *object;

    /*! Pointer to a driver-specific hardware attributes structure */
    void         const *hwAttrs;
} I2C_Config;

/*!
 *  @brief  Cancel all I2C transfers
 *
 *  This function will cancel asynchronous I2C_transfer() operations, and is
 *  applicable only for #I2C_MODE_CALLBACK.  An in progress transfer, as well
 *  as any queued transfers will be canceled. The individual callback functions
 *  for each transfer will be called from the context that I2C_cancel() is
 *  called.
 *
 *  @pre    I2C_Transfer() has been called.
 *
 *  @param  handle  An #I2C_Handle returned from I2C_open()
 *
 *  @note   Different I2C slave devices will behave differently when an
 *          in-progress transfer fails and needs to be canceled.  The slave
 *          may need to be reset, or there may be other slave-specific
 *          steps that can be used to successfully resume communication.
 *
 *  @sa     I2C_transfer()
 */
extern void I2C_cancel(I2C_Handle handle);

/*!
 *  @brief  Close an I2C peripheral specified by an #I2C_Handle
 *
 *  @pre    I2C_open() has been called.
 *
 *  @param  handle  An #I2C_Handle returned from I2C_open()
 *
 *  @sa     I2C_open()
 */
extern void I2C_close(I2C_Handle handle);

/*!
 *  @brief  Perform implementation-specific features on a given
 *          #I2C_Handle.
 *
 *  Commands for I2C_control() can originate from I2C.h or from implementation
 *  specific I2C.h (I2CCC26XX.h_, I2CMSP432.h_, etc.) files.
 *  While commands from I2C.h are API portable across driver implementations,
 *  not all implementations may support all these commands.
 *  Conversely, commands from driver implementation specific I2C*.h files add
 *  unique driver capabilities but are not API portable across all I2C driver
 *  implementations.
 *
 *  Commands supported by I2C.h follow a I2C_CMD_\<cmd\> naming
 *  convention.<br>
 *  Commands supported by I2C.h follow a I2C_CMD_\<cmd\> naming
 *  convention.<br>
 *  Each control command defines @b arg differently. The types of @b arg are
 *  documented with each command.
 *
 *  See @ref I2C_CMD "I2C_control command codes" for command codes.
 *
 *  See @ref I2C_STATUS "I2C_control return status codes" for status codes.
 *
 *  @pre    I2C_open() has to be called first.
 *
 *  @param  handle      An #I2C_Handle returned from I2C_open()
 *
 *  @param  cmd         I2C.h or I2C*.h command.
 *
 *  @param  controlArg  An optional R/W (read/write) command argument
 *                      accompanied with cmd
 *
 *  @return Implementation-specific return codes. Negative values indicate
 *          unsuccessful operations.
 *
 *  @sa     I2C_open()
 */
extern int_fast16_t I2C_control(I2C_Handle handle, uint_fast16_t cmd,
    void *controlArg);

/*!
 *  @brief  Initializes the I2C module
 *
 *  @pre    The \p I2C_config structure must exist and be persistent before this
 *          function can be called. This function must also be called before
 *          any other I2C driver APIs. This function call does not modify any
 *          peripheral registers.
 */
extern void I2C_init(void);

/*!
 *  @brief  Initialize a given I2C peripheral as identified by an index value.
 *          The #I2C_Params structure defines the operating mode, and any
 *          related settings.
 *
 *  @pre    The I2C controller has been initialized, via a previous call to
 *          I2C_init()
 *
 *  @param  index         Logical peripheral number for the I2C indexed into
 *                        the I2C_config table
 *
 *  @param  params        Pointer to a parameter block. Default values will be
 *                        used if \p NULL is specified for \p params. All the
 *                        fields in this structure are are considered read-only.
 *
 *  @return An #I2C_Handle on success, or \p NULL on an error, or if the
 *          peripheral is already opened.
 *
 *  @sa     I2C_init()
 *  @sa     I2C_close()
 */
extern I2C_Handle I2C_open(uint_least8_t index, I2C_Params *params);

/*!
 *  @brief  Initialize an #I2C_Params structure to its default values.
 *
 *  @param  params      A pointer to #I2C_Params structure for
 *                      initialization.
 *
 *  Defaults values are:
 *    - #I2C_Params.transferMode = #I2C_MODE_BLOCKING
 *    - #I2C_Params.transferCallbackFxn = \p NULL
 *    - #I2C_Params.bitRate = #I2C_100kHz
 *    - #I2C_Params.custom = \p NULL
 */
extern void I2C_Params_init(I2C_Params *params);

/*!
 *  @brief  Perform an I2C transaction with an I2C slave peripheral.
 *
 *  This function will perform an I2C transfer, as specified by an
 *  #I2C_Transaction structure.
 *
 *  An I2C transaction may write data to a peripheral, or read data from a
 *  peripheral, or both write and read data, in a single transaction.  If there
 *  is any data to be written, it will always be sent before any data is read
 *  from the peripheral.
 *
 *  The data written to the peripheral is preceded with the peripheral's 7-bit
 *  I2C slave address (with the Write bit set).
 *  After all the data has been transmitted, the driver will evaluate if any
 *  data needs to be read from the device.
 *  If yes, another START bit is sent, along with the same 7-bit I2C slave
 *  address (with the Read bit). After the specified number of bytes have been
 *  read, the transfer is ended with a NACK and a STOP bit.  Otherwise, if
 *  no data is to be read, the transfer is concluded with a STOP bit.
 *
 *  In #I2C_MODE_BLOCKING, I2C_transfer() will block thread execution until the
 *  transaction completes. Therefore, this function must only be called from an
 *  appropriate thread context (e.g., Task context for the TI-RTOS kernel).
 *
 *  In #I2C_MODE_CALLBACK, the I2C_transfer() call does not block thread
 *  execution. Instead, a callback function (specified during I2C_open(), via
 *  the transferCallbackFxn field in the #I2C_Params structure) is called when
 *  the transfer completes. Success or failure of the transaction is reported
 *  via the #I2C_CallbackFxn \b bool argument. If a transfer is already in
 *  progress, the new transaction is put on an internal queue. The driver
 *  services the queue in a first come first served basis.
 *
 *  @param  handle      An #I2C_Handle
 *
 *  @param  transaction A pointer to an #I2C_Transaction. All of the fields
 *                      within the transaction structure should be considered
 *                      write only, unless otherwise noted in the driver
 *                      implementation.
 *
 *  @note The #I2C_Transaction structure must persist unmodified until the
 *  corresponding call to I2C_transfer() has completed.
 *
 *  @return In #I2C_MODE_BLOCKING: true for a successful transfer; false for an
 *          error (for example, an I2C bus fault (NACK)).
 *
 *          In #I2C_MODE_CALLBACK: always true. The #I2C_CallbackFxn \p bool
 *          argument will be true to indicate success, and false to indicate
 *          an error.
 *
 *  @sa     I2C_open
 */
extern bool I2C_transfer(I2C_Handle handle, I2C_Transaction *transaction);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_I2C__include */
