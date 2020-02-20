/*
 ******************************************************************************
 *   ###                               ##          ###### ###### ###
 *  ## ##                              ##            ##     ##   ###
 * ##   ## ## ###  ######  #####   ######  ######    ##     ##   ###
 * ##   ## ###    ##           ## ##   ## ##    ##   ##     ##   ##
 * ####### ##     ##       ###### ##   ## ########   ##     ##   ##
 * ##   ## ##     ##      ##   ## ##   ## ##         ##     ##
 * ##   ## ##      ######  ######  ######  ######  ######   ##   ##
 * -----------------------------------------------------------------
 *                       P E R I P H E R A L S
 * -----------------------------------------------------------------
 *           (C)2020 www.digimorf.com, www.arcadeit.net
 *
 * @author  Francesco De Simone
 * @file    ArcadeIT_I2C_Port.h
 * @version V0.13
 * @date    16-02-2020
 * @last    20-02-2020
 * @brief   This library is used to drive the I2C port at low level.
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HARDWARE USED

 1.3 - I2C port

  ArcadeIT! Motherboard CN15 - I2C Port

  Pin  GPIO Function    I2C interface
  ---------------------------------------
  3.3V VDD  3.3V        CN15-1
  046  PH5  SDA         CN15-2
  045  PH4  SCL         CN15-3
  GND  VSS  GND         CN15-4

 ArcadeIT! mainboard:

  I2C port / CN15
  .---.
  | o |  1 3.3V
  | o |  2 SDA
  | o |  3 SCL
  | o |  4 GND
  '---'

 ******************************************************************************
 HISTORY
 16-02-2020:
 - Creation date of the library structure.

 *******************************************************************************
 */

// /////////////////////////////////////////////////////////////////////////////
// Includes.
// /////////////////////////////////////////////////////////////////////////////

#ifndef _ARCADEIT_I2C_PORT_H_
#define _ARCADEIT_I2C_PORT_H_

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include "System/ArcadeIT_Common.h"
#include "System/ArcadeIT_Utilities.h"
#include "System/ArcadeIT_Firmware.h"

// /////////////////////////////////////////////////////////////////////////////
// Definitions.
// /////////////////////////////////////////////////////////////////////////////

  #define SYS_I2C_RCC_REG                 RCC_APB1ENR
  #define SYS_I2C_RCC_CMD                 RCC_APB1PeriphClockCmd
  #define SYS_I2C_RCC_PER                 RCC_APB1Periph_I2C2
  #define SYS_I2C_PORT                    I2C2
  #define SYS_I2C_AF                      ((uint8_t)0x04)

  #define SYS_I2C_SCL_RCC_REG             RCC_AHB1ENR
  #define SYS_I2C_SCL_RCC_CMD             RCC_AHB1PeriphClockCmd
  #define SYS_I2C_SCL_RCC_PER             RCC_AHB1Periph_GPIOH
  #define SYS_I2C_SCL_PER                 GPIOH
  #define SYS_I2C_SCL_PIN                 GPIO_Pin_4
  #define SYS_I2C_SCL_PIN_NO              4
  #define SYS_I2C_SCL_SRC                 GPIO_PinSource4

  #define SYS_I2C_SDA_RCC_REG             RCC_AHB1ENR
  #define SYS_I2C_SDA_RCC_CMD             RCC_AHB1PeriphClockCmd
  #define SYS_I2C_SDA_RCC_PER             RCC_AHB1Periph_GPIOH
  #define SYS_I2C_SDA_PER                 GPIOH
  #define SYS_I2C_SDA_PIN                 GPIO_Pin_5
  #define SYS_I2C_SDA_PIN_NO              5
  #define SYS_I2C_SDA_SRC                 GPIO_PinSource5

  #define I2C_Mode_I2C                    ((uint16_t)0x0000)
  #define I2C_Mode_SMBusDevice            ((uint16_t)0x0002)
  #define I2C_Mode_SMBusHost              ((uint16_t)0x000A)
  #define I2C_DutyCycle_16_9              ((uint16_t)0x4000)
  #define I2C_DutyCycle_2                 ((uint16_t)0xBFFF)
  #define I2C_AcknowledgedAddress_7bit    ((uint16_t)0x4000)
  #define I2C_AcknowledgedAddress_10bit   ((uint16_t)0xC000)
  #define I2C_Ack_Enable                  ((uint16_t)0x0400)
  #define I2C_Ack_Disable                 ((uint16_t)0x0000)

  #define TEXT_I2C_INITED                 "I2C port enabled.\n\r"

  #define FLAG_MASK                       ((uint32_t)0x00FFFFFF)
  #define ITEN_MASK                       ((uint32_t)0x07000000)

  #define I2C_Direction_Transmitter       ((uint8_t)0x00)
  #define I2C_Direction_Receiver          ((uint8_t)0x01)

  #define I2C_ACK                         1
  #define I2C_NOACK                       0

  #define I2C_TIMEOUT_TIME                180000000
  #define I2C_DUTY_2                      0
  #define I2C_DUTY_16_9                   1

  // SR2 register flags
  #define I2C_FLAG_DUALF                  ((uint32_t)0x00800000)
  #define I2C_FLAG_SMBHOST                ((uint32_t)0x00400000)
  #define I2C_FLAG_SMBDEFAULT             ((uint32_t)0x00200000)
  #define I2C_FLAG_GENCALL                ((uint32_t)0x00100000)
  #define I2C_FLAG_TRA                    ((uint32_t)0x00040000)
  #define I2C_FLAG_BUSY                   ((uint32_t)0x00020000)
  #define I2C_FLAG_MSL                    ((uint32_t)0x00010000)

  // SR1 register flags
  #define I2C_FLAG_SMBALERT               ((uint32_t)0x10008000)
  #define I2C_FLAG_TIMEOUT                ((uint32_t)0x10004000)
  #define I2C_FLAG_PECERR                 ((uint32_t)0x10001000)
  #define I2C_FLAG_OVR                    ((uint32_t)0x10000800)
  #define I2C_FLAG_AF                     ((uint32_t)0x10000400)
  #define I2C_FLAG_ARLO                   ((uint32_t)0x10000200)
  #define I2C_FLAG_BERR                   ((uint32_t)0x10000100)
  #define I2C_FLAG_TXE                    ((uint32_t)0x10000080)
  #define I2C_FLAG_RXNE                   ((uint32_t)0x10000040)
  #define I2C_FLAG_STOPF                  ((uint32_t)0x10000010)
  #define I2C_FLAG_ADD10                  ((uint32_t)0x10000008)
  #define I2C_FLAG_BTF                    ((uint32_t)0x10000004)
  #define I2C_FLAG_ADDR                   ((uint32_t)0x10000002)
  #define I2C_FLAG_SB                     ((uint32_t)0x10000001)

// I2C Master Events (Events grouped in order of communication)
/*
  Communication start

  After sending the START condition (I2C_GenerateSTART() function) the master
  has to wait for this event. It means that the Start condition has been correctly
  released on the I2C bus (the bus is free, no other devices is communicating).
*/
  // --EV5
  #define  I2C_EVENT_MASTER_MODE_SELECT   ((uint32_t)0x00030001)

/*
  Address Acknowledge

  After checking on EV5 (start condition correctly released on the bus), the
  master sends the address of the slave(s) with which it will communicate
  (I2C_Send7bitAddress() function, it also determines the direction of the communication:
  Master transmitter or Receiver). Then the master has to wait that a slave acknowledges
  his address. If an acknowledge is sent on the bus, one of the following events will
  be set:

   1) In case of Master Receiver (7-bit addressing): the I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED
      event is set.

   2) In case of Master Transmitter (7-bit addressing): the I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED
      is set

   3) In case of 10-Bit addressing mode, the master (just after generating the START
   and checking on EV5) has to send the header of 10-bit addressing mode (I2C_SendData()
   function). Then master should wait on EV9. It means that the 10-bit addressing
   header has been correctly sent on the bus. Then master should send the second part of
   the 10-bit address (LSB) using the function I2C_Send7bitAddress(). Then master
   should wait for event EV6.
*/

  // --EV6
  #define  I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED  ((uint32_t)0x00070082)
  #define  I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED     ((uint32_t)0x00030002)

  // --EV9
  #define  I2C_EVENT_MASTER_MODE_ADDRESS10             ((uint32_t)0x00030008)

/*
  Communication events

  If a communication is established (START condition generated and slave address
  acknowledged) then the master has to check on one of the following events for
  communication procedures:

  1) Master Receiver mode: The master has to wait on the event EV7 then to read
     the data received from the slave (I2C_ReceiveData() function).

  2) Master Transmitter mode: The master has to send data (I2C_SendData()
     function) then to wait on event EV8 or EV8_2.
     These two events are similar:
      - EV8 means that the data has been written in the data register and is
        being shifted out.
      - EV8_2 means that the data has been physically shifted out and output
        on the bus.
      In most cases, using EV8 is sufficient for the application.
      Using EV8_2 leads to a slower communication but ensure more reliable test.
      EV8_2 is also more suitable than EV8 for testing on the last data transmission
      (before Stop condition generation).

   note In case the  user software does not guarantee that this event EV7 is
        managed before the current byte end of transfer, then user may check on EV7
        and BTF flag at the same time (ie. (I2C_EVENT_MASTER_BYTE_RECEIVED | I2C_FLAG_BTF)).
        In this case the communication may be slower.
*/

  // Master RECEIVER mode
  // --EV7
  #define  I2C_EVENT_MASTER_BYTE_RECEIVED      ((uint32_t)0x00030040)

  // Master TRANSMITTER mode
  // --EV8
  #define I2C_EVENT_MASTER_BYTE_TRANSMITTING   ((uint32_t)0x00070080)

  // --EV8_2
  #define  I2C_EVENT_MASTER_BYTE_TRANSMITTED   ((uint32_t)0x00070084)


// I2C Slave Events (Events grouped in order of communication)
/*
  Communication start events

  Wait on one of these events at the start of the communication. It means that
  the I2C peripheral detected a Start condition on the bus (generated by master
  device) followed by the peripheral address. The peripheral generates an ACK
  condition on the bus (if the acknowledge feature is enabled through function
  I2C_AcknowledgeConfig()) and the events listed above are set :

  1) In normal case (only one address managed by the slave), when the address
    sent by the master matches the own address of the peripheral (configured by
    I2C_OwnAddress1 field) the I2C_EVENT_SLAVE_XXX_ADDRESS_MATCHED event is set
    (where XXX could be TRANSMITTER or RECEIVER).

  2) In case the address sent by the master matches the second address of the
    peripheral (configured by the function I2C_OwnAddress2Config() and enabled
    by the function I2C_DualAddressCmd()) the events I2C_EVENT_SLAVE_XXX_SECONDADDRESS_MATCHED
    (where XXX could be TRANSMITTER or RECEIVER) are set.

  3) In case the address sent by the master is General Call (address 0x00) and
    if the General Call is enabled for the peripheral (using function I2C_GeneralCallCmd())
    the following event is set I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED.
*/

  // --EV1  (all the events below are variants of EV1)
  // 1) Case of One Single Address managed by the slave
  #define  I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED          ((uint32_t)0x00020002)
  #define  I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED       ((uint32_t)0x00060082)

  // 2) Case of Dual address managed by the slave
  #define  I2C_EVENT_SLAVE_RECEIVER_SECONDADDRESS_MATCHED    ((uint32_t)0x00820000)
  #define  I2C_EVENT_SLAVE_TRANSMITTER_SECONDADDRESS_MATCHED ((uint32_t)0x00860080)

  // 3) Case of General Call enabled for the slave
  #define  I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED        ((uint32_t)0x00120000)

/*
  Communication events

  Wait on one of these events when EV1 has already been checked and:

  - Slave RECEIVER mode:
      - EV2: When the application is expecting a data byte to be received.
      - EV4: When the application is expecting the end of the communication: master
        sends a stop condition and data transmission is stopped.

  - Slave Transmitter mode:
     - EV3: When a byte has been transmitted by the slave and the application is expecting
       the end of the byte transmission. The two events I2C_EVENT_SLAVE_BYTE_TRANSMITTED and
       I2C_EVENT_SLAVE_BYTE_TRANSMITTING are similar. The second one can optionally be
       used when the user software doesn't guarantee the EV3 is managed before the
       current byte end of transfer.
     - EV3_2: When the master sends a NACK in order to tell slave that data transmission
       shall end (before sending the STOP condition). In this case slave has to stop sending
       data bytes and expect a Stop condition on the bus.

    note In case the  user software does not guarantee that the event EV2 is
         managed before the current byte end of transfer, then user may check on EV2
         and BTF flag at the same time (ie. (I2C_EVENT_SLAVE_BYTE_RECEIVED | I2C_FLAG_BTF)).
         In this case the communication may be slower.
*/

  // Slave RECEIVER mode
  // --EV2
  #define  I2C_EVENT_SLAVE_BYTE_RECEIVED     ((uint32_t)0x00020040)

  // --EV4
  #define  I2C_EVENT_SLAVE_STOP_DETECTED     ((uint32_t)0x00000010)

  // Slave TRANSMITTER mode
  // --EV3
  #define  I2C_EVENT_SLAVE_BYTE_TRANSMITTED  ((uint32_t)0x00060084)
  #define  I2C_EVENT_SLAVE_BYTE_TRANSMITTING ((uint32_t)0x00060080)

  // --EV3_2
  #define  I2C_EVENT_SLAVE_ACK_FAILURE       ((uint32_t)0x00000400)

// /////////////////////////////////////////////////////////////////////////////
// Functions.
// /////////////////////////////////////////////////////////////////////////////
int ArcadeIT_I2C_CheckEvent(
    uint32_t pEvent // the event flags
    );
// -----------------------------------------------------------------------------
int8_t ArcadeIT_I2C_WaitEvent(
    uint32_t pEvent // the event flags
    );
// -----------------------------------------------------------------------------
int8_t ArcadeIT_I2C_StartCommunication(void);
// -----------------------------------------------------------------------------
void ArcadeIT_I2C_EndCommunication(void);
// -----------------------------------------------------------------------------
int8_t ArcadeIT_I2C_Send7bitAddress(
    uint8_t pAddress,   // The address of the I2C device
    uint8_t pDirection  // The direction of the communication
    );
// -----------------------------------------------------------------------------
void ArcadeIT_I2C_Port_Init(
    uint32_t pClock     // The frequency clock of the I2C device
    );
// -----------------------------------------------------------------------------
int8_t ArcadeIT_I2C_Send(
    uint8_t pData       // byte data to send
    );
// -----------------------------------------------------------------------------
int8_t ArcadeIT_I2C_Receive(
    uint8_t *pData,       // ponter to byte where to store the received data
    uint8_t pAcknowledge  // flag to enable or disable the acknowledge event
    );
// -----------------------------------------------------------------------------

// /////////////////////////////////////////////////////////////////////////////

#endif // _ARCADEIT_I2C_PORT_H_

/*
 Inter-integrated circuit interface (I2C)

 The I²C bus interface can operate in multimaster and slave modes. It can support
 the standard (up to 100 KHz), and fast (up to 400 KHz) modes. It supports the 7/10-bit
 addressing mode and the 7-bit dual addressing mode (as slave). A hardware CRC
 generation/verification is embedded.

 The I2C port used from the ones available from the STM32F469 in the ArcadeIT!
 is the I2C2.

 -

 From: RM0386, Reference manual, page 1001

 29 Inter-integrated circuit (I2C) interface

 This section describes the I2C port functions, but this driver has been written
 to provide a common and standard driver. We will explain what and how registers
 are set to start the I2C port, and how transmit or receive data over it.

 -

 From: RM0386, Reference manual, page 1001

 Figure 321. I2C block diagram

 This diagram shows the architecture of the I2C peripheral inside the STM32F469
 microcontroller.

 -

 We have to enable the clock of the peripheral that drives the two signals SCL
 and SDA, and then we need to configure them.

 I2C SCL, SDA = PH4, PH5

 From: RM0386, Reference manual, page 206-207

 These registers are 32 bit wide, values for each pin go from 0 to 3
  GPIO port mode register (GPIOx_MODER) (x = A to K)
  GPIO port speed register (GPIOx_OSPEEDR) (x = A to K)
  GPIO port pull-up register (GPIOx_PUPDR) (x = A to K)

 |  0    0    0    0 .  0    0    0    0 |  0    0    0    0 .  0    0    0    0 |
 +---------+---------+---------+---------+---------+---------+---------+---------+
 | PIN 15  | PIN 14  | PIN 13  | PIN 12  | PIN 11  | PIN 10  | PIN 09  | PIN 08  |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | MODER   | MODER   | MODER   | MODER   | MODER   | MODER   | MODER   | MODER   |
 | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR |
 | PUDR    | PUDR    | PUDR    | PUDR    | PUDR    | PUDR    | PUDR    | PUDR    |
 |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+

 |  0    0    0    0 .  1    1    1    1 |  0    0    0    0 .  0    0    0    0 |
 +---------+---------#####################----+----+---------+---------+---------+
 | PIN 07  | PIN 06  # PIN 05  | PIN 04  # PIN 03  | PIN 02  | PIN 01  | PIN 00  |
 +----+----+----+----#----+----+----+----#----+----+----+----|----+----+----+----+
 | 15 | 14 | 13 | 12 # 11 | 10 | 09 | 08 # 07 | 06 | 05 | 04 | 03 | 02 | 01 | 00 |
 +----+----+----+----#----+----+----+----#----+----+----+----|----+----+----+----+
 | MODER   | MODER   # MODER   | MODER   # MODER   | MODER   | MODER   | MODER   |
 | OSPEEDR | OSPEEDR # OSPEEDR | OSPEEDR # OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR |
 | PUDR    | PUDR    # PUDR    | PUDR    # PUDR    | PUDR    | PUDR    | PUDR    |
 |  [1:0]  |  [1:0]  #  [1:0]  |  [1:0]  #  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |
 +----+----+----+----#----+----+----+----#----+----+----+----|----+----+----+----+
 | rw | rw | rw | rw # rw | rw | rw | rw # rw | rw | rw | rw | rw | rw | rw | rw |
 +----+----+----+----#####################----+----+---------+----+----+----+----+

 This register is 32 bit wide but only 16 are used since values for each pin
 require only one bit.
  GPIO port type register (GPIOx_OTYPER) (x = A to K)

 |  0    0    0    0 .  0    0    0    0 |  0    0    0    0 .  0    0    0    0 |
 +---------+---------+---------+---------+---------+---------+---------+---------+
 |         |         |         |         |         |         |         |         |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | OT | OT | OT | OT | OT | OT | OT | OT | OT | OT | OT | OT | OT | OT | OT | OT |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+

 |  0    0    0    0 .  0    0    0    0 |  0    0    1    1 .  0    0    0    0 |
 +----+----+----+----+----+----+----+----+----+----###########----+----+----+----+
 |P15 |P14 |P13 |P12 |P11 |P10 |P09 |P08 |P07 |P06 #P05 |P04 #P03 |P02 |P01 |P00 |
 +----+----+----+----+----+----+----+----+----+----#----+----#----+----+----+----+
 | 15 | 14 | 13 | 12 | 11 | 10 | 09 | 08 | 07 | 06 # 05 | 04 # 03 | 02 | 01 | 00 |
 +----+----+----+----+----+----+----+----+----+----#----+----#----+----+----+----+
 | OT | OT | OT | OT | OT | OT | OT | OT | OT | OT # OT | OT # OT | OT | OT | OT |
 +----+----+----+----+----+----+----+----+----+----#----+----#----+----+----+----+
 | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw # rw | rw # rw | rw | rw | rw |
 +----+----+----+----+----+----+----+----+----+----###########----+----+----+----+

 -

 From: Datasheet, page 81

 Table 12. Alternate functions

 To activate the I2C function we need to apply to pins PH4 and, PH5 the alternate
 function 4.

 Since we have sixteen different alternate functions, 4 bits are needed for each
 pin to be configured. So two registers are used for pins 7..0 and pins 15..8,
 respectively registers GPIOx_AFRL and GPIOx_AFRH.

 For I2C we use pins 4 and 5, so the low register GPIOx_AFRL is used.

                                             alternate fn 4      alternate fn 4

 |  0    0    0    0 .  0    0    0    0 |  0    1    0    0 .  0    1    0    0 |
 +---------+---------+---------+---------#########################################
 |      PIN 07       |      PIN 06       #      PIN 05       #      PIN 04       #
 +----+----+----+----+----+----+----+----#----+----+----+----#----+----+----+----#
 | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 # 23 | 22 | 21 | 20 # 19 | 18 | 17 | 16 #
 +----+----+----+----+----+----+----+----#----+----+----+----#----+----+----+----#
 |    AFRH07[3:0]    |    AFRH06[3:0]    #    AFRH05[3:0]    #    AFRH04[3:0]    #
 +----+----+----+----+----+----+----+----#----+----+----+----#----+----+----+----#
 | rw | rw | rw | rw | rw | rw | rw | rw # rw | rw | rw | rw # rw | rw | rw | rw #
 +----+----+----+----+----+----+----+----#########################################


 |  0    0    0    0 .  0    0    0    0 |  0    0    0    0 .  0    0    0    0 |
 +---------+---------+---------+---------+---------+---------+---------+---------+
 |      PIN 03       |      PIN 02       |      PIN 01       |      PIN 00       |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | 15 | 14 | 13 | 12 | 11 | 10 | 09 | 08 | 07 | 06 | 05 | 04 | 03 | 02 | 01 | 00 |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 |    AFRH03[3:0]    |    AFRH02[3:0]    |    AFRH01[3:0]    |    AFRH00[3:0]    |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+

 -

 From: RM0386, Reference manual, page 168

 6.3.13 RCC APB1 peripheral clock enable register (RCC_APB1ENR)

 We first need to enable the clock of the I2C2 by enabling the corresponding
 flag in register RCC_APB1ENR: Bit 22 is called I2C1EN (I2C clock enable).
 Then we can configure the I2C peripheral (frequency, data etc.)

 -

 -

 Typical transmission

               +-----+-----+-----+-----+-----+-----+-----+-----+---+
               |                      byte                     |Ack|
               +-----+-----+-----+-----+-----+-----+-----+-----+---+
  Start        |     |     |     |     |     |     |     |     |   |   End
  condition    |0    |1    |2    |3    |4    |5    |6    |7    |   |   condition
  |------.     .--.        .--------.        .-----------------#####    .------|
  |SDA   |     |1 |   0    |1     1 |   0    |1     1     1    #####    |   SDA|
         '--- -'  '--------'        '--------'                 ##### ---' .----|
  |----.       .--.  .--.  .--.  .--.  .--.  .--.  .--.  .--.  .--.
  |SCL |       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |       | SCL|
  |    '----- -'  '--'  '--'  '--'  '--'  '--'  '--'  '--'  '--'  '- -----'    |

 */
