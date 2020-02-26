/*
 * test.c
 *
 *  Created on: Feb 24, 2020
 *      Author: Francesco
 */



  // --------------------------------------------------------------------------
  char lString[256];

#if 0
  // ===========================================================================
  // Status LEDs
  for (uint8_t lCycles = 0; lCycles < 4; lCycles++)
  {
    ArcadeIT_Status_LED1_Toggle();
    ArcadeIT_System_Delay(250);
    ArcadeIT_Status_LED2_Toggle();
    ArcadeIT_System_Delay(250);

  } // End if.
#endif
#if 1
  // --------------------------------------------------------------------------
  // We setup the scheduler to blink the Status LED 1 at 1Hz (every seconds).
  if (gUnits & ARCADEIT_UNIT_SCHEDULER)
  {
    if (gDevices & ARCADEIT_DEVICE_STATUSLED)
    {
          ArcadeIT_Scheduler_Task_Set (
              0,
              (void*) ArcadeIT_Status_LED1_Toggle,
              NULL,
              ARCADEIT_SCHEDULER_REPEAT_INFINITELY,
              ARCADEIT_PAUSE_HALF_SECOND
              );

     } // End if.

  } // End if.
#endif
#if 0
  // --------------------------------------------------------------------------
  // System MCU test. We use the internal clock cycles counter to measure time.
  uint32_t lCurrentTime = 0, lLastTime = 0;

  if (gDevices & ARCADEIT_DEVICE_SERIAL)
  {
    ArcadeIT_System_Delay(5000);
    ArcadeIT_Serial_Port_String_Send(RESET_DEVICE);

    ArcadeIT_Serial_Port_String_Send("Testing 500 ms of delay.\n\r");

  } // End if.

  ArcadeIT_Status_LED2_Toggle();

  lLastTime = SYS_MCU_CYCLES();
  ArcadeIT_System_Delay(500);
  lCurrentTime = SYS_MCU_CYCLES();

  if (gDevices & ARCADEIT_DEVICE_SERIAL)
  {
    sprintf(lString, "MCU time elapsed: %lu ms\r\n", SYS_MCU_TO_MS(lCurrentTime - lLastTime));
    ArcadeIT_Serial_Port_String_Send(lString);
    sprintf(lString, "MCU time elapsed: %lu us\r\n", SYS_MCU_TO_US(lCurrentTime - lLastTime));
    ArcadeIT_Serial_Port_String_Send(lString);
    sprintf(lString, "MCU time elapsed: %lu ns\r\n", SYS_MCU_TO_NS(lCurrentTime - lLastTime));
    ArcadeIT_Serial_Port_String_Send(lString);

  } // End if.

  ArcadeIT_Status_LED2_Toggle();
#endif
#if 0
  // --------------------------------------------------------------------------
  // Shows 256 colors over the serial terminal
  if (gDevices & ARCADEIT_DEVICE_SERIAL)
  {
    ArcadeIT_System_Delay(5000);
    ArcadeIT_Serial_Port_String_Send(RESET_DEVICE);
    ArcadeIT_Status_LED2_Toggle();

    ArcadeIT_Serial_Port_String_Send("ANSI colors table on serial terminal:\n\r");
    ArcadeIT_Serial_Port_String_Send(CURSOR_OFF);

    for (uint8_t lRow = 0; lRow < 16; lRow++)
    {
      for (uint8_t lCol = 0; lCol < 16; lCol++)
      {
        sprintf(lString, ATTR_COLOR_256_BG, lRow * 16 + lCol);
        ArcadeIT_Serial_Port_String_Send(lString);
        sprintf(lString, " %03d ", lRow * 16 + lCol);
        ArcadeIT_Serial_Port_String_Send(lString);

      } // end for

      ArcadeIT_Serial_Port_String_Send(CURSOR_NEWLINE);

    } // end for

    ArcadeIT_Serial_Port_String_Send(CURSOR_NEWLINE);
    ArcadeIT_Serial_Port_String_Send(CURSOR_ON);
    ArcadeIT_Status_LED2_Toggle();

  } // End if.
#endif
#if 0
  // --------------------------------------------------------------------------
  // Shows ANSI artwork over the serial terminal
  if (gDevices & ARCADEIT_DEVICE_SERIAL)
  {
    ArcadeIT_System_Delay(5000);
    ArcadeIT_Serial_Port_String_Send(RESET_DEVICE);
    ArcadeIT_Status_LED2_Toggle();

    ArcadeIT_Serial_Port_String_Send("ANSI art on serial terminal:\n\r");
    ArcadeIT_Serial_Port_String_Send(CURSOR_OFF);

    unsigned char *lANSIArtPtr = (unsigned char *)&gANSIDemo1[0];
    for (uint16_t lChar = 0; lChar < 7352; lChar++)
    {
      ArcadeIT_Serial_Port_Char_Send(*lANSIArtPtr++);

    } // end for

    ArcadeIT_Serial_Port_String_Send(CURSOR_NEWLINE);
    ArcadeIT_Serial_Port_String_Send(CURSOR_ON);
    ArcadeIT_Status_LED2_Toggle();

  } // End if.
#endif
#if 0
  // --------------------------------------------------------------------------
  // Shows terminal TUI ANSI widgets demo over the serial terminal
  if (gDevices & ARCADEIT_DEVICE_SERIAL)
  {
    ArcadeIT_System_Delay(5000);
    ArcadeIT_Serial_Port_String_Send(RESET_DEVICE);
    ArcadeIT_Status_LED2_Toggle();

    ArcadeIT_Serial_Port_String_Send("TUI widgets for serial terminal:\n\r");
    ArcadeIT_Serial_Port_String_Send(CURSOR_OFF);

    label_t  title1   = {1, 0, 220, 18, "OUTPUT1", LABEL_SKIN4_ANSI};
    frame_t  window1 = {1, 1, 40, 10, 27, 18, title1, FRAME_SKIN_ANSI};
    slider_h_t sliderh1 = {2, 39, 9, false, 0.0, 10.0, 2.0, 0.0, 0, 0, SLIDER_SKIN_ANSI};
    slider_v_t sliderv1 = {38, 2, 9, false, 0.0, 10.0, 2.0, 0.0, 0, 0, SLIDER_SKIN_ANSI};

    ArcadeIT_System_Delay(2000);
    ArcadeIT_Status_LED2_Toggle();

    frame_draw(&window1);

    slider_h_refresh(&sliderh1);
    slider_v_refresh(&sliderv1);

    char lBufferfg[100], lBufferbg[100];
    uint8_t lFlags = WINDOW_SLIDER_H | WINDOW_SLIDER_V | WINDOW_TITLE;
    window_t window2 = {"Window", 1, 6, 20, 10, 0, 0, WINDOW_SKIN_ANSI, lBufferfg, lBufferbg, lFlags};
    window_draw(&window2);

    ArcadeIT_Serial_Port_String_Send(CURSOR_NEWLINE);
    ArcadeIT_Serial_Port_String_Send(CURSOR_ON);
    ArcadeIT_Status_LED2_Toggle();

  } // End if.
#endif
#if 0
  // ---------------------------------------------------------------------------
  // I2C Tests
  if (gDevices & ARCADEIT_DEVICE_I2C)
  {
    /*
       This test reads and writes a block of data from/to an I2C 32KB EEPROM
       24LC256

       Write Protect = 0
       Address = 0

            24LC256                                             ArcadeIT!
            I2C EEPROM            Pull-up resistors             I2C port / CN15
            .-------------.      .------o-------o--[1 3.3V>     .---.
          .-|             |-.    |      |       |               | o |  1 3.3V
     .----|1|A0        VCC|8|----'      |       |               | o |  2 SDA
     |    '-|             |-'          .-.     .-.              | o |  3 SCL
     |    .-|             |-.        R1| |   R2| |              | o |  4 GND
     o----|2|A1         WP|7|----. 4.7K| | 4.7K| |              '---'
     |    '-|             |-'    |     '-'     '-'
     |    .-|             |-.    |      |       |
     o----|3|A2        SCL|6|----)------o-------)--[3 SCL>
     |    '-|             |-'    |              |
     |    .-|             |-.    |              |
     o----|4|GND       SDA|5|----)--------------o--[2 SDA>
     |    '-|             |-'    |
     |      '-------------'      |
     |                           |           .-----[4 GND>
     |           C1              |           |
     |           ,,              |           |
     |     .-----||---[1 3.3V>   |           |
     |     |     ''              |           |
    ===   ===  0.1 uF           ===         ===
    GND   GND                   GND         GND

    */


    uint8_t lDataRead = 0x0, lDataToWrite = 0xA5;
    uint16_t lAddress = 0x0000;

    if (gDevices & ARCADEIT_DEVICE_SERIAL)
    {
      ArcadeIT_System_Delay(5000);
      ArcadeIT_Serial_Port_String_Send(RESET_DEVICE);
      ArcadeIT_Status_LED2_Toggle();

      ArcadeIT_Serial_Port_String_Send("I2C test for EEPROM:\n\r");

      sprintf(lString, "Writing '0x%02X' to memory @ 0x%04X ", lDataToWrite, lAddress);
      ArcadeIT_Serial_Port_String_Send(lString);

    } // end if

    // Write byte to the memory starting from address 0
    // Start condition
    ArcadeIT_I2C_StartCommunication();
    ArcadeIT_I2C_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT);
    // Send byte: Control byte
    ArcadeIT_I2C_Send7bitAddress(0xA0, I2C_Direction_Transmitter);
    ArcadeIT_I2C_WaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    // Send bytes: Set internal 16-bit address
    ArcadeIT_I2C_Send(lAddress >> 8);
    ArcadeIT_I2C_Send(lAddress & 0xFF);
    // Send byte: Data byte
    ArcadeIT_I2C_Send(lDataToWrite);
    // End condition
    ArcadeIT_I2C_EndCommunication();
    // Wait for internal write operation completed
    ArcadeIT_System_Delay(5);

    if (gDevices & ARCADEIT_DEVICE_SERIAL)
    {
      ArcadeIT_Serial_Port_String_Send("-> done!\r\n");

      sprintf(lString, "Reading memory @ 0x%04X ", lAddress);
      ArcadeIT_Serial_Port_String_Send(lString);

    } // end if

    // Read byte to the memory starting from address 0
    // Start condition
    ArcadeIT_I2C_StartCommunication();
    ArcadeIT_I2C_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT);
    // Send byte: Control byte
    ArcadeIT_I2C_Send7bitAddress(0xA0, I2C_Direction_Transmitter);
    ArcadeIT_I2C_WaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    // Send bytes: Set internal 16-bit address
    ArcadeIT_I2C_Send(lAddress >> 8);
    ArcadeIT_I2C_Send(lAddress & 0xFF);
    // Address set, restart communication to switch master in receive mode
    ArcadeIT_I2C_StartCommunication();
    ArcadeIT_I2C_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT);
    // Send byte: Control byte
    ArcadeIT_I2C_Send7bitAddress(0xA0, I2C_Direction_Receiver);
    // receive a byte without sending an Acknowledge
    ArcadeIT_I2C_Receive(&lDataRead, I2C_NOACK);
    // End condition
    ArcadeIT_I2C_EndCommunication();

    if (gDevices & ARCADEIT_DEVICE_SERIAL)
    {
      sprintf(lString, "-> 0x%02X, ", lDataRead);
      ArcadeIT_Serial_Port_String_Send(lString);

      ArcadeIT_Serial_Port_String_Send(lDataRead == lDataToWrite ? "correct\r\n" : "wrong!\r\n");

      ArcadeIT_Serial_Port_String_Send(CURSOR_NEWLINE);
      ArcadeIT_Serial_Port_String_Send(CURSOR_ON);
    } // end if

    ArcadeIT_Status_LED2_Toggle();

  } // end if
#endif
#if 0
  // ---------------------------------------------------------------------------
  // SPI Tests
  if (gDevices & ARCADEIT_DEVICE_SPI1)
  {
    /*
       This test reads and writes a byte from/to an SPI 32KB SRAM 23K256

                    24K256                         ArcadeIT!
                    SPI SRAM                       SPI port, SD Card / CN10
                    .-------------.                .--------.
                  .-|             |-.              |  o  o  |  2 GND  | 1 3.3V
      <8 CS]------|1|CS        VCC|8|-o-[1 3.3V>   |  o  o |   4 NC   | 3 SCK
                  '-|             |-' |            |  o  o |   6 MISO | 5 MOSI
                  .-|             |-. |            |  o  o  |  8 CS   | 7 SD DET.
    <6 MISO]-o----|2|SO        HLD|7|-'            '--------'
             |    '-|             |-'
            .-.   .-|             |-.
  pull-up R1| |   |3|NC        CLK|6|---[3 SCK>
        4.7K| |   '-|             |-'
            '-'   .-|             |-.
             |  .-|4|GND        SI|5|---[5 MOSI>
    <1 3.3V]-'  | '-|             |-'
                |   '-------------'
                |         C1
                |         ,,
                |   .-----||------------[1 3.3V>
                |   |     ''
               === ===    0.1 uF
               GND GND
     */

    ArcadeIT_SPI_Port_Init (SPI_FREQ_22_50_MHz);

    uint8_t lDataRead = 0x0, lDataToWrite = 0xA5;
    uint16_t lAddress = 0x0000;

    if (gDevices & ARCADEIT_DEVICE_SERIAL)
    {
      ArcadeIT_System_Delay(5000);
      ArcadeIT_Serial_Port_String_Send(RESET_DEVICE);
      ArcadeIT_Status_LED2_Toggle();

      ArcadeIT_Serial_Port_String_Send("SPI test for SRAM:\n\r");

    } // end if

    // set SRAM in single byte transfer mode
    SYS_SPI_CS_LOW();
    ArcadeIT_SPI_Port_RW_Byte(0x1);
    ArcadeIT_SPI_Port_RW_Byte(0x0);
    SYS_SPI_CS_HIGH();

    if (gDevices & ARCADEIT_DEVICE_SERIAL)
      ArcadeIT_Serial_Port_String_Send("SRAM status register set to: ");

    // set SRAM in single byte transfer mode
    SYS_SPI_CS_LOW();
    ArcadeIT_SPI_Port_RW_Byte(0x5);
    lDataRead = ArcadeIT_SPI_Port_RW_Byte(0x0);
    SYS_SPI_CS_HIGH();

    if (gDevices & ARCADEIT_DEVICE_SERIAL)
    {
      sprintf(lString, "0x%02X\n\r", lDataRead);
      ArcadeIT_Serial_Port_String_Send(lString);

      sprintf(lString, "Writing '0x%02X' to memory @ 0x%04X ", lDataToWrite, lAddress);
      ArcadeIT_Serial_Port_String_Send(lString);

    } // end if

    // write a byte into SRAM
    SYS_SPI_CS_LOW();
    ArcadeIT_SPI_Port_RW_Byte(0x02);

    // Send bytes: Set internal 16-bit address
    ArcadeIT_SPI_Port_RW_Byte(lAddress >> 8);
    ArcadeIT_SPI_Port_RW_Byte(lAddress & 0xFF);

    ArcadeIT_SPI_Port_RW_Byte(lDataToWrite);
    SYS_SPI_CS_HIGH();

    if (gDevices & ARCADEIT_DEVICE_SERIAL)
    {
      ArcadeIT_Serial_Port_String_Send("-> done!\r\n");

      sprintf(lString, "Reading memory @ 0x%04X ", lAddress);
      ArcadeIT_Serial_Port_String_Send(lString);

    } // end if

    // read a byte from SRAM
    SYS_SPI_CS_LOW();
    ArcadeIT_SPI_Port_RW_Byte(0x03);

    // Send bytes: Set internal 16-bit address
    ArcadeIT_SPI_Port_RW_Byte(lAddress >> 8);
    ArcadeIT_SPI_Port_RW_Byte(lAddress & 0xFF);

    lDataRead = ArcadeIT_SPI_Port_RW_Byte(0x0);
    SYS_SPI_CS_HIGH();

    if (gDevices & ARCADEIT_DEVICE_SERIAL)
    {
      sprintf(lString, "-> 0x%02X, ", lDataRead);
      ArcadeIT_Serial_Port_String_Send(lString);

      ArcadeIT_Serial_Port_String_Send(lDataRead == lDataToWrite ? "correct\r\n" : "wrong!\r\n");

      ArcadeIT_Serial_Port_String_Send(CURSOR_NEWLINE);
      ArcadeIT_Serial_Port_String_Send(CURSOR_ON);

    } // end if

    ArcadeIT_Status_LED2_Toggle();

  } // end if
  // ===========================================================================
#endif
#if 0
  // ---------------------------------------------------------------------------
  // SD-Card access test
  if (gStorage & ARCADEIT_STORAGE_SD_CARD_SPI1)
  {
    if (gDevices & ARCADEIT_DEVICE_SERIAL)
    {
      ArcadeIT_System_Delay(5000);
      ArcadeIT_Serial_Port_String_Send(RESET_DEVICE);
      ArcadeIT_Status_LED2_Toggle();

      ArcadeIT_Serial_Port_String_Send("File test for SD-Card:\n\r");

    } // end if

    // Create a new file
    // the global file system that ArcadeIT! opens is held by the global variable
    // FATFS gFileSystemSD;
    FIL lFile;                /* File objects */
    BYTE lFileBuffer[4096];   /* File copy buffer */
    FRESULT lFileStatus = 0;  /* FatFs function common result code */
    UINT lBytesRead = 0;
    uint8_t lChar = 32;

    // create a file
    lFileStatus += f_open(&lFile, "SDCARD:file-test.txt", FA_WRITE | FA_CREATE_ALWAYS);
    if (gDevices & ARCADEIT_DEVICE_SERIAL)
    {
      sprintf(lString, "%s", lFileStatus == FR_OK ? "File created.\r\n" : "Problems in creating the file.\r\n");
      ArcadeIT_Serial_Port_String_Send(lString);

    } // end if

    // store some text
    lFileStatus += f_write(&lFile, "ASCII Charset:\r\n", strlen("ASCII Charset:\r\n"), &lBytesRead);
    for (lChar = 32; lChar < 128; lChar++)
    {
      lFileBuffer[lChar - 32] = lChar;

    } // end for
    lFileBuffer[lChar - 32] = 0;

    lFileStatus += f_write(&lFile, lFileBuffer, strlen((char*)lFileBuffer), &lBytesRead);
    if (gDevices & ARCADEIT_DEVICE_SERIAL)
    {
      sprintf(lString, "%s", lFileStatus == FR_OK ? "File populated.\r\n" : "Problems in populating the file.\r\n");
      ArcadeIT_Serial_Port_String_Send(lString);

    } // end if

    // close file
    f_close(&lFile);

    // Now show file
    lFileStatus = f_open(&lFile, "SDCARD:file-test.txt", FA_READ);
    if (gDevices & ARCADEIT_DEVICE_SERIAL)
    {
      sprintf(lString, "%s", lFileStatus == FR_OK ? "File opened.\r\n" : "Problems in opening the file.\r\n");
      ArcadeIT_Serial_Port_String_Send(lString);

      sprintf(lString, "File size: %d bytes\r\n", f_size(&lFile));
      ArcadeIT_Serial_Port_String_Send(lString);

    } // end if

    lFileStatus += f_read(&lFile, lFileBuffer, f_size(&lFile), &lBytesRead);  /* Read a chunk of source file */
    if (gDevices & ARCADEIT_DEVICE_SERIAL)
    {
      sprintf(lString, "%s", (lBytesRead == f_size(&lFile)) ? "File read.\r\n" : "Problems in reading the file.\r\n");
      ArcadeIT_Serial_Port_String_Send(lString);

    } // end if

    if (lBytesRead == f_size(&lFile))
      ArcadeIT_Utility_Show_Bytes(0, lFileBuffer, lBytesRead);

    ArcadeIT_Status_LED2_Toggle();

    // close file
    f_close(&lFile);

  } // end if
#endif
#if 01
  // ---------------------------------------------------------------------------
  // SD-Card access test
  if (gStorage & ARCADEIT_STORAGE_SD_CARD_SPI1)
  {
    if (gDevices & ARCADEIT_DEVICE_SERIAL)
    {
      ArcadeIT_System_Delay(5000);
      ArcadeIT_Serial_Port_String_Send(RESET_DEVICE);
      ArcadeIT_Status_LED2_Toggle();

      ArcadeIT_Serial_Port_String_Send("Speed test for SD-Card:\n\r");

    } // end if

    // Create a new file
    // the global file system that ArcadeIT! opens is held by the global variable
    // FATFS gFileSystemSD;
    FIL lFile;                /* File objects */
    FRESULT lFileStatus = 0;  /* FatFs function common result code */
    UINT lBytesToSave = 1024*1024*2, lBytesRead = 0;
    BYTE *lMemoryFrom = 0x8000000;
    uint32_t lCurrentTime = 0, lLastTime = 0, lTimeElapsed = 0;

    // create a file
    lFileStatus += f_open(&lFile, "SDCARD:ArcadeIT_firmware.bin", FA_WRITE | FA_CREATE_ALWAYS);
    if (gDevices & ARCADEIT_DEVICE_SERIAL)
    {
      sprintf(lString, "%s", lFileStatus == FR_OK ? "File created.\r\n" : "Problems in creating the file.\r\n");
      ArcadeIT_Serial_Port_String_Send(lString);

    } // end if



    // store the content of the FLASH int the file
    while (lBytesToSave)
    {
      lLastTime = SYS_MCU_CYCLES();
      lFileStatus += f_write(&lFile, lMemoryFrom, 4096, &lBytesRead);
      lCurrentTime = SYS_MCU_CYCLES();
      lTimeElapsed += (lCurrentTime - lLastTime);

      if (lBytesRead != 4096) break;
      lMemoryFrom += 4096;
      lBytesToSave -= 4096;

    } // end while

    if (gDevices & ARCADEIT_DEVICE_SERIAL)
    {
      sprintf(lString, "Transfer rate: %0.2f kB/s\r\n", 2048/(lTimeElapsed*0.00000000555));
      ArcadeIT_Serial_Port_String_Send(lString);

    } // End if.

    if (gDevices & ARCADEIT_DEVICE_SERIAL)
    {
      sprintf(lString, "%s", lFileStatus == FR_OK ? "Firmware saved.\r\n" : "Problems in saving the file.\r\n");
      ArcadeIT_Serial_Port_String_Send(lString);

    } // end if

    // close file
    f_close(&lFile);

    ArcadeIT_Status_LED2_Toggle();

  } // end if
#endif
