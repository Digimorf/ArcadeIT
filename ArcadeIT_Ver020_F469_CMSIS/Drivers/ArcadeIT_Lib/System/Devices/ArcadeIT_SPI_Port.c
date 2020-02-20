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
 * @file    ArcadeIT_SPI_Port.h
 * @version V0.13
 * @date    26-07-2017
 * @last    20-02-2020
 * @brief   This library is used to drive the SPI port at low level.
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HARDWARE USED

  1.2 - SD Card

  ArcadeIT! Motherboard CN10 - PortB - SPI

  Pin  GPIO Function                     SD Card interface
  ---------------------------------------------------------------------
  3.3V VDD  SD 3.3V                      CN10-1
  GND  VSS  SD GND                       CN10-2
  051  PA5  SD CLK    (SPI1_SCK)         CN10-3
  -    -    -                            CN10-4
  053  PA7  SD MOSI   (SPI1_MOSI)        CN10-5
  052  PA6  SD MISO   (SPI1_MISO)        CN10-6
  055  PC5  SD DETECT (GPIO SD Inserted) CN10-7
  054  PC4  SD CS     (GPIO Chip Select) CN10-8


  SPI port, SD Card / CN10
               .--------.
   2 GND       |  o  o  |   1 3.3V
   4 NC        |  o  o |    3 SPI1_SCK
   6 SPI1_MISO |  o  o |    5 SPI1_MOSI
   8 CS        |  o  o  |   7 SD DETECT
               '--------'

 *******************************************************************************
 HISTORY
 26-07-2017:
 - Creation date of the library structure.

 21-09-2017:
 - Cleaned the code from the specific memory location positioning of the
   functions.

 16-10-2018:
 - Cleaned code and optimized it for portability.

 20-02-2020:
 - converted to CMSIS and ported to GitHUB

 ******************************************************************************
*/

// /////////////////////////////////////////////////////////////////////////////
// Includes.
// /////////////////////////////////////////////////////////////////////////////

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include <System/ArcadeIT_Common.h>
#include <System/ArcadeIT_Utilities.h>
#include <System/ArcadeIT_Firmware.h>


// ArcadeIT! Peripherals and buses.
#include <System/Devices/ArcadeIT_SPI_Port.h>
#include <System/Devices/ArcadeIT_Serial_Port.h>

// /////////////////////////////////////////////////////////////////////////////
// Macros.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// External elements.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Global elements.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Shared functions.
// /////////////////////////////////////////////////////////////////////////////
uint8_t ArcadeIT_SPI_Port_RW_Byte
(
  uint8_t pData // The data byte to send
)
{
  /*
  * DESCRIPTION: This function is used to send a byte through the SPI SD port.
  * PARAMETERS:  The byte to send.
  * RETURNS:     A byte returned from the SPI port.
  */

  // The read byte.
  uint8_t lParameter;
  uint32_t lTimeout;

  //Wait until the transmit buffer is empty.
  lTimeout = SYS_SD_SPI_TIMEOUT;

  while ((SYS_SD_SPI_PORT->SR & SPI_I2S_FLAG_TXE) == RESET)
  {
    if (lTimeout-- == 0) break;
  };

  // Send the byte.
  SYS_SD_SPI_PORT->DR = pData;

  // Wait to receive a byte.
  lTimeout = SYS_SD_SPI_TIMEOUT;
  while ((SYS_SD_SPI_PORT->SR & SPI_I2S_FLAG_RXNE) == RESET)
  {
    if (lTimeout-- == 0) break;
  };

  // Return the byte read from the SPI bus.
  lParameter = SYS_SD_SPI_PORT->DR;

  return lParameter;

} // End ArcadeIT_SPI_Port_RW_Byte.

#if SYS_SD_SPI_METHOD == NORMAL
// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_SPI_Port_Read_Buffer
(
  uint8_t *pBuffer,       // Pointer to data buffer.
  uint32_t pNumberOfBytes // Number of bytes to receive.
)
{
  uint8_t lData;

  for (uint32_t lBytes = 0; lBytes < pNumberOfBytes; lBytes++)
  {
    // Receive the data block into buffer.
    lData = ArcadeIT_SPI_Port_RW_Byte(ARCADEIT_DUMMY_BYTE);
    pBuffer[lBytes] = lData;

  } // End for.

} // End ArcadeIT_SPI_Port_Read_Buffer.

// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_SPI_Port_Write_Buffer
(
  uint8_t *pBuffer,       // Pointer to data buffer.
  uint32_t pNumberOfBytes // Number of bytes to send.
)
{

  for (uint32_t lBytes = 0; lBytes < pNumberOfBytes; lBytes++)
  {
    // Sends the data block to the SPI.
    ArcadeIT_SPI_Port_RW_Byte(pBuffer[lBytes]);

  } // End for.

} // End ArcadeIT_SPI_Port_Write_Buffer.

#elif SYS_SD_SPI_METHOD == DMA

// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_SPI_Port_Read_Buffer
(
  uint8_t *pBuffer,       // Pointer to data buffer.
  uint32_t pNumberOfBytes // Number of bytes to receive.
)
{
  uint32_t rw_workbyte = DUMMY_BYTE;

  // Rx buffer.
  SYS_SD_SPI_DMA_MISO_STREAM->CR &= ((uint32_t)~(DMA_SxCR_MINC | DMA_SxCR_PINC | DMA_SxCR_DIR ));
  SYS_SD_SPI_DMA_MISO_STREAM->CR |= ( DMA_DIR_PeripheralToMemory | DMA_MemoryInc_Enable | DMA_PeripheralInc_Disable );
  SYS_SD_SPI_DMA_MISO_STREAM->NDTR = (uint16_t)pNumberOfBytes;
  SYS_SD_SPI_DMA_MISO_STREAM->PAR = (uint32_t)&SYS_SD_SPI_PORT->DR;
  SYS_SD_SPI_DMA_MISO_STREAM->M0AR = (uint32_t)pBuffer;

  // Tx dummy byte.
  SYS_SD_SPI_DMA_MOSI_STREAM->CR &= ((uint32_t)~(DMA_SxCR_MINC | DMA_SxCR_PINC | DMA_SxCR_DIR ));
  SYS_SD_SPI_DMA_MOSI_STREAM->CR |= ( DMA_DIR_MemoryToPeripheral | DMA_MemoryInc_Disable | DMA_PeripheralInc_Disable );
  SYS_SD_SPI_DMA_MOSI_STREAM->NDTR = (uint16_t)pNumberOfBytes;
  SYS_SD_SPI_DMA_MOSI_STREAM->PAR = (uint32_t)&SYS_SD_SPI_PORT->DR;
  SYS_SD_SPI_DMA_MOSI_STREAM->M0AR = (uint32_t)&rw_workbyte;

  SYS_SD_SPI_PORT->CR2 |= SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx;
  SYS_SD_SPI_DMA_MISO_STREAM->CR |= (uint32_t)DMA_SxCR_EN;
  SYS_SD_SPI_DMA_MOSI_STREAM->CR |= (uint32_t)DMA_SxCR_EN;

  while (DMA_GetFlagStatus(SYS_SD_SPI_DMA_MISO_STREAM, DMA_FLAG_TCIF2)==RESET);
  while (DMA_GetFlagStatus(SYS_SD_SPI_DMA_MOSI_STREAM, DMA_FLAG_TCIF3)==RESET);

  DMA_ClearFlag(SYS_SD_SPI_DMA_MISO_STREAM, DMA_FLAG_TCIF2);
  DMA_ClearFlag(SYS_SD_SPI_DMA_MOSI_STREAM, DMA_FLAG_TCIF3);

  SYS_SD_SPI_PORT->CR2 &= (uint16_t)~SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx;
  SYS_SD_SPI_DMA_MISO_STREAM->CR &= ~(uint32_t)DMA_SxCR_EN;
  SYS_SD_SPI_DMA_MOSI_STREAM->CR &= ~(uint32_t)DMA_SxCR_EN;

} // End ArcadeIT_SPI_Port_Read_Buffer.

// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_SPI_Port_Write_Buffer
(
  uint8_t *pBuffer,       // Pointer to data buffer.
  uint32_t pNumberOfBytes // Number of bytes to send.
)
{
  uint32_t rw_workbyte = DUMMY_BYTE;

  // Rx buffer.
  SYS_SD_SPI_DMA_MISO_STREAM->CR &= ((uint32_t)~(DMA_SxCR_MINC | DMA_SxCR_PINC | DMA_SxCR_DIR ));
  SYS_SD_SPI_DMA_MISO_STREAM->CR |= ( DMA_DIR_PeripheralToMemory | DMA_MemoryInc_Disable | DMA_PeripheralInc_Disable );
  SYS_SD_SPI_DMA_MISO_STREAM->NDTR = (uint16_t)pNumberOfBytes;
  SYS_SD_SPI_DMA_MISO_STREAM->PAR = (uint32_t)&SYS_SD_SPI_PORT->DR;
  SYS_SD_SPI_DMA_MISO_STREAM->M0AR = (uint32_t)&rw_workbyte;

  // Tx dummy byte.
  SYS_SD_SPI_DMA_MOSI_STREAM->CR &= ((uint32_t)~(DMA_SxCR_MINC | DMA_SxCR_PINC | DMA_SxCR_DIR ));
  SYS_SD_SPI_DMA_MOSI_STREAM->CR |= ( DMA_DIR_MemoryToPeripheral | DMA_MemoryInc_Enable | DMA_PeripheralInc_Disable );
  SYS_SD_SPI_DMA_MOSI_STREAM->NDTR = (uint16_t)pNumberOfBytes;
  SYS_SD_SPI_DMA_MOSI_STREAM->PAR = (uint32_t)&SYS_SD_SPI_PORT->DR;
  SYS_SD_SPI_DMA_MOSI_STREAM->M0AR = (uint32_t)pBuffer;

  SYS_SD_SPI_PORT->CR2 |= SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx;
  SYS_SD_SPI_DMA_MISO_STREAM->CR |= (uint32_t)DMA_SxCR_EN;
  SYS_SD_SPI_DMA_MOSI_STREAM->CR |= (uint32_t)DMA_SxCR_EN;

  while (DMA_GetFlagStatus(SYS_SD_SPI_DMA_MISO_STREAM, DMA_FLAG_TCIF2)==RESET);
  while (DMA_GetFlagStatus(SYS_SD_SPI_DMA_MOSI_STREAM, DMA_FLAG_TCIF3)==RESET);

  DMA_ClearFlag(SYS_SD_SPI_DMA_MISO_STREAM, DMA_FLAG_TCIF2);
  DMA_ClearFlag(SYS_SD_SPI_DMA_MOSI_STREAM, DMA_FLAG_TCIF3);

  SYS_SD_SPI_PORT->CR2 &= (uint16_t)~SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx;
  SYS_SD_SPI_DMA_MISO_STREAM->CR &= ~(uint32_t)DMA_SxCR_EN;
  SYS_SD_SPI_DMA_MOSI_STREAM->CR &= ~(uint32_t)DMA_SxCR_EN;

} // End ArcadeIT_SPI_Port_Write_Buffer.
#endif
// /////////////////////////////////////////////////////////////////////////////
int ArcadeIT_SPI_Port_Init (void)
{
  /*
   * DESCRIPTION: Enables and configures the SPI port of the ArcadeIT! system.
   * PARAMETERS:  None.
   * RETURNS:     Nothing.
   */

  // Enable GPIOs used by the SPI.
  if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOA) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOA;
  if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOC) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOC;
  if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOD) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOD;
  if ((RCC->APB2ENR & RCC_APB2Periph_SPI1) == FALSE) RCC->APB2ENR |= RCC_APB2Periph_SPI1;

  uint32_t lPinPosition;

  // Configure the output line SCK as alternate function.
  lPinPosition = (SYS_SD_SPI_SCK_PIN_NO * 2);

  SYS_SD_SPI_SCK_PER->MODER   &= ~GPIO_MODER_MODER5;
  SYS_SD_SPI_SCK_PER->MODER   |= (((uint32_t)GPIO_Mode_AF) << lPinPosition);
  SYS_SD_SPI_SCK_PER->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR5;
  SYS_SD_SPI_SCK_PER->OSPEEDR |= ((uint32_t)(GPIO_Speed_100MHz) << lPinPosition);
  SYS_SD_SPI_SCK_PER->OTYPER  &= ~GPIO_OTYPER_OT_5;
  SYS_SD_SPI_SCK_PER->OTYPER  |= (uint16_t)(GPIO_OType_OD << SYS_SD_SPI_SCK_PIN_NO);
  SYS_SD_SPI_SCK_PER->PUPDR   &= ~GPIO_PUPDR_PUPDR5;
  SYS_SD_SPI_SCK_PER->PUPDR   |= (((uint32_t)GPIO_PuPd_DOWN) << lPinPosition);
  SYS_SD_SPI_SCK_PER->AFR[0]  &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)SYS_SD_SPI_SCK_PIN_NO & (uint32_t)0x07) * 4));
  SYS_SD_SPI_SCK_PER->AFR[0]  |= ((uint32_t)(SYS_SD_SPI_AF) << ((uint32_t)((uint32_t)SYS_SD_SPI_SCK_PIN_NO & (uint32_t)0x07) * 4));

  // Configure the output line MOSI as alternate function.
  lPinPosition = (SYS_SD_SPI_MOSI_PIN_NO * 2);

  SYS_SD_SPI_MOSI_PER->MODER   &= ~GPIO_MODER_MODER7;
  SYS_SD_SPI_MOSI_PER->MODER   |= (((uint32_t)GPIO_Mode_AF) << lPinPosition);
  SYS_SD_SPI_MOSI_PER->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR7;
  SYS_SD_SPI_MOSI_PER->OSPEEDR |= ((uint32_t)(GPIO_Speed_100MHz) << lPinPosition);
  SYS_SD_SPI_MOSI_PER->OTYPER  &= ~GPIO_OTYPER_OT_7;
  SYS_SD_SPI_MOSI_PER->OTYPER  |= (uint16_t)(GPIO_OType_OD << SYS_SD_SPI_MOSI_PIN_NO);
  SYS_SD_SPI_MOSI_PER->PUPDR   &= ~GPIO_PUPDR_PUPDR7;
  SYS_SD_SPI_MOSI_PER->PUPDR   |= (((uint32_t)GPIO_PuPd_DOWN) << lPinPosition);
  SYS_SD_SPI_MOSI_PER->AFR[0]  &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)SYS_SD_SPI_MOSI_PIN_NO & (uint32_t)0x07) * 4));
  SYS_SD_SPI_MOSI_PER->AFR[0]  |= ((uint32_t)(SYS_SD_SPI_AF) << ((uint32_t)((uint32_t)SYS_SD_SPI_MOSI_PIN_NO & (uint32_t)0x07) * 4));

  // Configure the output line MOSI as alternate function.
  lPinPosition = (SYS_SD_SPI_MOSI_PIN_NO * 2);

  SYS_SD_SPI_MOSI_PER->MODER   &= ~GPIO_MODER_MODER7;
  SYS_SD_SPI_MOSI_PER->MODER   |= (((uint32_t)GPIO_Mode_AF) << lPinPosition);
  SYS_SD_SPI_MOSI_PER->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR7;
  SYS_SD_SPI_MOSI_PER->OSPEEDR |= ((uint32_t)(GPIO_Speed_100MHz) << lPinPosition);
  SYS_SD_SPI_MOSI_PER->OTYPER  &= ~GPIO_OTYPER_OT_7;
  SYS_SD_SPI_MOSI_PER->OTYPER  |= (uint16_t)(GPIO_OType_OD << SYS_SD_SPI_MOSI_PIN_NO);
  SYS_SD_SPI_MOSI_PER->PUPDR   &= ~GPIO_PUPDR_PUPDR7;
  SYS_SD_SPI_MOSI_PER->PUPDR   |= (((uint32_t)GPIO_PuPd_DOWN) << lPinPosition);
  SYS_SD_SPI_MOSI_PER->AFR[0]  &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)SYS_SD_SPI_MOSI_PIN_NO & (uint32_t)0x07) * 4));
  SYS_SD_SPI_MOSI_PER->AFR[0]  |= ((uint32_t)(SYS_SD_SPI_AF) << ((uint32_t)((uint32_t)SYS_SD_SPI_MOSI_PIN_NO & (uint32_t)0x07) * 4));

  // Configure the output line CS as a normal output GPIO.
  lPinPosition = (SYS_SD_SPI_CS_PIN_NO * 2);

  SYS_SD_SPI_CS_PER->MODER   &= ~GPIO_MODER_MODER4;
  SYS_SD_SPI_CS_PER->MODER   |= (((uint32_t)GPIO_Mode_OUT) << lPinPosition);
  SYS_SD_SPI_CS_PER->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR4;
  SYS_SD_SPI_CS_PER->OSPEEDR |= ((uint32_t)(GPIO_Speed_100MHz) << lPinPosition);
  SYS_SD_SPI_CS_PER->OTYPER  &= ~GPIO_OTYPER_OT_4;
  SYS_SD_SPI_CS_PER->OTYPER  |= (uint16_t)(GPIO_OType_PP << SYS_SD_SPI_CS_PIN_NO);
  SYS_SD_SPI_CS_PER->PUPDR   &= ~GPIO_PUPDR_PUPDR4;
  SYS_SD_SPI_CS_PER->PUPDR   |= (((uint32_t)GPIO_PuPd_UP) << lPinPosition);

  // Initially the SPI port has 4 multiplexed lines to use as CS.
  // CS1 0 1 0 1
  SYS_SPI_CS_HIGH();

  #if (USE_DETECT_PIN == TRUE)
  // Configure the input line SD Detect as a normal input GPIO, but this will be
  // connected to an EXTI interrupt.
  lGPIO_InitStructure.GPIO_Pin   = SYS_SD_DETECT_PIN;
  lGPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  lGPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  lGPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  lGPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init (SYS_SD_DETECT_PER, &lGPIO_InitStructure);

  // Configure the interrupt line on the SD detect.
  // Connect EXTI8 Line to SD Detect pin
  SYSCFG_EXTILineConfig(SYS_SD_DETECT_EXT_PER, SYS_SD_DETECT_SRC);

  //  Configure EXTI8 line
  lEXTI_InitStructure.EXTI_Line    = SYS_SD_DETECT_SRC;
  lEXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
  lEXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // either edge issues an interrupt
  lEXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&lEXTI_InitStructure);

  // Enable and set EXTI4_15 Interrupt
  lNVIC_InitStructure.NVIC_IRQChannel                   = SYS_SD_SPI_PORT_IRQ;
  lNVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  lNVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&lNVIC_InitStructure);
  #endif

  // SPI configuration.
  // Reset SPI
  RCC->APB2RSTR |= RCC_APB2Periph_SPI1;
  RCC->APB2RSTR &= ~RCC_APB2Periph_SPI1;

  SYS_SD_SPI_PORT->CR1 |=
      (uint16_t)((uint32_t)SPI_Direction_2Lines_FullDuplex
                         | SPI_Mode_Master
                         | SPI_DataSize_8b
                         | SPI_CPOL_Low
                         | SPI_CPHA_1Edge
                         | SPI_NSS_Soft
                         | SYS_SD_SPI_SPEED
                         | SPI_FirstBit_MSB);

  SYS_SD_SPI_PORT->CR1 |= SPI_CR1_SPE;

#if (SYS_SD_SPI_METHOD == DMA)
  // DMA Streams
  DMA_InitTypeDef         lDMA_InitStructure;

  if ((SYS_SD_SPI_DMA_RCC_REG & SYS_SD_SPI_DMA_RCC_PER) == FALSE) SYS_SD_SPI_DMA_RCC_CMD(SYS_SD_SPI_DMA_RCC_PER, ENABLE);

  DMA_DeInit(SYS_SD_SPI_DMA_MOSI_STREAM); //SPI1_TX_DMA_STREAM
  DMA_DeInit(SYS_SD_SPI_DMA_MISO_STREAM); //SPI1_RX_DMA_STREAM
  DMA_StructInit(&lDMA_InitStructure);

  lDMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  lDMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  lDMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  lDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  lDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  lDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  lDMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)NULL;
  lDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SYS_SD_SPI_PORT->DR;
  lDMA_InitStructure.DMA_BufferSize = (uint16_t)1;
  lDMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  lDMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  lDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  lDMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  lDMA_InitStructure.DMA_Priority = DMA_Priority_High;

  // Configure Tx DMA.
  lDMA_InitStructure.DMA_Channel = SYS_SD_SPI_DMA_MOSI_STREAM_CH;
  lDMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_Init(SYS_SD_SPI_DMA_MOSI_STREAM, &lDMA_InitStructure);

  // Configure Rx DMA.
  lDMA_InitStructure.DMA_Channel = SYS_SD_SPI_DMA_MISO_STREAM_CH;
  lDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_Init(SYS_SD_SPI_DMA_MISO_STREAM, &lDMA_InitStructure);
#endif

  if (gDevices & ARCADEIT_DEVICE_SERIAL)
  {
    ArcadeIT_Serial_Port_String_Send(TEXT_SPI_PORT_INITED);

  } // End if.

  return TRUE;

} // End ArcadeIT_SPI_Port_Init.

// /////////////////////////////////////////////////////////////////////////////
