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
 * @file    ArcadeIT_LCD_Definitions.c
 * @version V0.13
 * @date    06-01-2018
 * @last    03-02-2020
 * @brief   Names of the registers of the supported display controllers.
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HISTORY
 06-01-2018:
 - Creation date of the library structure.

 03-02-2020:
 - Port to CMSIS and GitHub.

 ******************************************************************************
*/

#ifndef _ARCADEIT_LCD_DEFINITIONS_H
#define _ARCADEIT_LCD_DEFINITIONS_H

// /////////////////////////////////////////////////////////////////////////////
// Includes
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Definitions
// /////////////////////////////////////////////////////////////////////////////

// http://domoticx.com/arduino-library-tft-lcd-shield/
#define ILI932X_DRIV_OUT_CTRL      0x01
#define ILI932X_DRIV_WAV_CTRL      0x02
#define ILI932X_ENTRY_MOD          0x03
#define ILI932X_RESIZE_CTRL        0x04
#define ILI932X_DISP_CTRL1         0x07
#define ILI932X_DISP_CTRL2         0x08
#define ILI932X_DISP_CTRL3         0x09
#define ILI932X_DISP_CTRL4         0x0A
#define ILI932X_RGB_DISP_IF_CTRL1  0x0C
#define ILI932X_FRM_MARKER_POS     0x0D
#define ILI932X_RGB_DISP_IF_CTRL2  0x0F
#define ILI932X_POW_CTRL1          0x10
#define ILI932X_POW_CTRL2          0x11
#define ILI932X_POW_CTRL3          0x12
#define ILI932X_POW_CTRL4          0x13
#define ILI932X_GRAM_HOR_AD        0x20
#define ILI932X_GRAM_VER_AD        0x21
#define ILI932X_RW_GRAM            0x22
#define ILI932X_POW_CTRL7          0x29
#define ILI932X_FRM_RATE_COL_CTRL  0x2B
#define ILI932X_GAMMA_CTRL1        0x30
#define ILI932X_GAMMA_CTRL2        0x31
#define ILI932X_GAMMA_CTRL3        0x32
#define ILI932X_GAMMA_CTRL4        0x35
#define ILI932X_GAMMA_CTRL5        0x36
#define ILI932X_GAMMA_CTRL6        0x37
#define ILI932X_GAMMA_CTRL7        0x38
#define ILI932X_GAMMA_CTRL8        0x39
#define ILI932X_GAMMA_CTRL9        0x3C
#define ILI932X_GAMMA_CTRL10       0x3D
#define ILI932X_HOR_START_AD       0x50
#define ILI932X_HOR_END_AD         0x51
#define ILI932X_VER_START_AD       0x52
#define ILI932X_VER_END_AD         0x53
#define ILI932X_GATE_SCAN_CTRL1    0x60
#define ILI932X_GATE_SCAN_CTRL2    0x61
#define ILI932X_GATE_SCAN_CTRL3    0x6A
#define ILI932X_PART_IMG1_DISP_POS 0x80
#define ILI932X_PART_IMG1_START_AD 0x81
#define ILI932X_PART_IMG1_END_AD   0x82
#define ILI932X_PART_IMG2_DISP_POS 0x83
#define ILI932X_PART_IMG2_START_AD 0x84
#define ILI932X_PART_IMG2_END_AD   0x85
#define ILI932X_PANEL_IF_CTRL1     0x90
#define ILI932X_PANEL_IF_CTRL2     0x92
#define ILI932X_PANEL_IF_CTRL3     0x93
#define ILI932X_PANEL_IF_CTRL4     0x95
#define ILI932X_PANEL_IF_CTRL5     0x97
#define ILI932X_PANEL_IF_CTRL6     0x98

// /////////////////////////////////////////////////////////////////////////////
#define HX8347G_COLADDRSTART_HI    0x02
#define HX8347G_COLADDRSTART_LO    0x03
#define HX8347G_COLADDREND_HI      0x04
#define HX8347G_COLADDREND_LO      0x05
#define HX8347G_ROWADDRSTART_HI    0x06
#define HX8347G_ROWADDRSTART_LO    0x07
#define HX8347G_ROWADDREND_HI      0x08
#define HX8347G_ROWADDREND_LO      0x09
#define HX8347G_MEMACCESS          0x16

// /////////////////////////////////////////////////////////////////////////////
#define NO_OPERATION                                0x00
#define SOFTWARE_RESET                              0x01
#define READ_DISPLAY_IDENTIFICATION_INFORMATION     0x04
#define READ_DISPLAY_STATUS                         0x09
#define READ_DISPLAY_POWER_MODE                     0x0A
#define READ_DISPLAY_MADCTL                         0x0B
#define READ_DISPLAY_PIXEL_FORMAT                   0x0C
#define READ_DISPLAY_IMAGE_FORMAT                   0x0D
#define READ_DISPLAY_SIGNAL_MODE                    0x0E
#define READ_DISPLAY_SELFDIAGNOSTIC_RESULT          0x0F
#define ENTER_SLEEP_MODE                            0x10
#define SLEEP_OUT                                   0x11
#define PARTIAL_MODE_ON                             0x12
#define NORMAL_DISPLAY_MODE_ON                      0x13
#define DISPLAY_INVERSION_OFF                       0x20
#define DISPLAY_INVERSION_ON                        0x21
#define GAMMA_SET                                   0x26
#define DISPLAY_OFF                                 0x28
#define DISPLAY_ON                                  0x29
#define COLUMN_ADDRESS_SET                          0x2A
#define PAGE_ADDRESS_SET                            0x2B
#define MEMORY_WRITE                                0x2C
#define COLOR_SET                                   0x2D
#define MEMORY_READ                                 0x2E
#define PARTIAL_AREA                                0x30
#define VERTICAL_SCROLLING_DEFINITION               0x33
#define TEARING_EFFECT_LINE_OFF                     0x34
#define TEARING_EFFECT_LINE_ON                      0x35
#define MEMORY_ACCESS_CONTROL                       0x36
#define VERTICAL_SCROLLING_START_ADDRESS            0x37
#define IDLE_MODE_OFF                               0x38
#define IDLE_MODE_ON                                0x39
#define PIXEL_FORMAT_SET                            0x3A
#define PIXEL_FORMAT_RGB565BIT                      0x55
#define PIXEL_FORMAT_RGB666BIT                      0x66
#define WRITE_MEMORY_CONTINUE                       0x3C
#define READ_MEMORY_CONTINUE                        0x3E
#define SET_TEAR_SCANLINE                           0x44
#define GET_SCANLINE                                0x45
#define WRITE_DISPLAY_BRIGHTNESS                    0x51
#define READ_DISPLAY_BRIGHTNESS                     0x52
#define WRITE_CTRL_DISPLAY                          0x53
#define READ_CTRL_DISPLAY                           0x54
#define WRITE_CONTENT_ADAPTIVE_BRIGHTNESS_CONTROL   0x55
#define READ_CONTENT_ADAPTIVE_BRIGHTNESS_CONTROL    0x56
#define WRITE_CABC_MINIMUM_BRIGHTNESS               0x5E
#define READ_CABC_MINIMUM_BRIGHTNESS                0x5F
#define READ_ID1                                    0xDA
#define READ_ID2                                    0xDB
#define READ_ID3                                    0xDC
#define RGB_INTERFACE                               0xB0
#define SIGNAL_CONTROL                              0x40
#define FRAME_CONTROL_IN_NORMAL_MODE                0xB1
#define FRAME_CONTROL_IN_IDLE_MODE                  0xB2
#define FRAME_CONTROL_IN_PARTIAL_MODE               0xB3
#define DISPLAY_INVERSION_CONTROL                   0xB4
#define BLANKING_PORCH_CONTROL                      0xB5
#define DISPLAY_FUNCTION_CONTROL                    0xB6
#define ENTRY_MODE_SET                              0xB7
#define BACKLIGHT_CONTROL_1                         0xB8
#define BACKLIGHT_CONTROL_2                         0xB9
#define BACKLIGHT_CONTROL_3                         0xBA
#define BACKLIGHT_CONTROL_4                         0xBB
#define BACKLIGHT_CONTROL_5                         0xBC
#define BACKLIGHT_CONTROL_7                         0xBE
#define BACKLIGHT_CONTROL_8                         0xBF
#define POWER_CONTROL_1                             0xC0
#define POWER_CONTROL_2                             0xC1
#define VCOM_CONTROL_1                              0xC5
#define VCOM_CONTROL_2                              0xC7
#define POWER_CONTROL_A                             0xCB
#define POWER_CONTROL_B                             0xCF
#define NV_MEMORY_WRITE                             0xD0
#define NV_MEMORY_PROTECTION_KEY                    0xD1
#define NV_MEMORY_STATUS_READ                       0xD2
#define READ_ID4                                    0xD3
#define POSITIVE_GAMMA_CORRECTION                   0xE0
#define NEGATIVE_GAMMA_CORRECTION                   0xE1
#define DIGITAL_GAMMA_CONTROL_1                     0xE2
#define DIGITAL_GAMMA_CONTROL_2                     0xE3
#define DRIVER_TIMING_CONTROL_A                     0xE8
#define DRIVER_TIMING_CONTROL_B                     0xEA
#define POWER_ON_SEQUENCE_CONTROL                   0xED
#define ENABLE_3G                                   0xF2
#define INTERFACE_CONTROL                           0xF6
#define PUMP_RATIO_CONTROL                          0xF7

// /////////////////////////////////////////////////////////////////////////////
#define HX8357_NOP                 0x00
#define HX8357_SWRESET             0x01
#define HX8357_RDDID               0x04
#define HX8357_RDDST               0x09
#define HX8357_SLPIN               0x10
#define HX8357_SLPOUT              0x11
#define HX8357_INVOFF              0x20
#define HX8357_INVON               0x21
#define HX8357_DISPOFF             0x28
#define HX8357_DISPON              0x29
#define HX8357_CASET               0x2A
#define HX8357_PASET               0x2B
#define HX8357_RAMWR               0x2C
#define HX8357_RAMRD               0x2E
#define HX8357_TEON                0x35
#define HX8357_TEARLINE            0x44
#define HX8357_MADCTL              0x36
#define HX8357_COLMOD              0x3A
#define HX8357_SETOSC              0xB0
#define HX8357_SETPWR1             0xB1
#define HX8357_SETRGB              0xB3
#define HX8357_SETPANEL            0xCC

// /////////////////////////////////////////////////////////////////////////////
#define HX8357B_RDPOWMODE          0x0A
#define HX8357B_RDMADCTL           0x0B
#define HX8357B_RDCOLMOD           0x0C
#define HX8357B_RDDIM              0x0D
#define HX8357B_RDDSDR             0x0F
#define HX8357B_PTLON              0x12
#define HX8357B_NORON              0x13
#define HX8357B_PTLAR              0x30
#define HX8357B_SETDISPLAY         0xB2
#define HX8357B_SETDISPMODE        0xB4
#define HX8357B_SETOTP             0xB7
#define HX8357B_SET_PANEL_DRIVING  0xC0
#define HX8357B_SETDGC             0xC1
#define HX8357B_SETID              0xC3
#define HX8357B_SETDDB             0xC4
#define HX8357B_SETDISPLAYFRAME    0xC5
#define HX8357B_GAMMASET           0xC8
#define HX8357B_SETCABC            0xC9
#define HX8357B_SETPOWER           0xD0
#define HX8357B_SETVCOM            0xD1
#define HX8357B_SETPWRNORMAL       0xD2
#define HX8357B_RDID1              0xDA
#define HX8357B_RDID2              0xDB
#define HX8357B_RDID3              0xDC
#define HX8357B_RDID4              0xDD
#define HX8357B_SETGAMMA           0xC8
#define HX8357B_SETPANELRELATED    0xE9
#define HX8357B_MADCTL_MY          0x80
#define HX8357B_MADCTL_MX          0x40
#define HX8357B_MADCTL_MV          0x20
#define HX8357B_MADCTL_ML          0x10
#define HX8357B_MADCTL_RGB         0x00
#define HX8357B_MADCTL_BGR         0x08
#define HX8357B_MADCTL_MH          0x04

// /////////////////////////////////////////////////////////////////////////////
#define HX8357D_SETCOM             0xB6
#define HX8357D_SETCYC             0xB4
#define HX8357D_SETC               0xB9
#define HX8357D_SETSTBA            0xC0
#define HX8357D_SETGAMMA           0xE0

// /////////////////////////////////////////////////////////////////////////////
#define ST7735_MADCTL_BGR          0x08
#define ST7735_MADCTL_MH           0x04
#define ST7735_FRMCTR1             0xB1
#define ST7735_FRMCTR2             0xB2
#define ST7735_FRMCTR3             0xB3
#define ST7735_INVCTR              0xB4
#define ST7735_DISSET5             0xB6
#define ST7735_PWCTR1              0xC0
#define ST7735_PWCTR2              0xC1
#define ST7735_PWCTR3              0xC2
#define ST7735_PWCTR4              0xC3
#define ST7735_PWCTR5              0xC4
#define ST7735_VMCTR1              0xC5
#define ST7735_PWCTR6              0xFC
#define ST7735_GMCTRP1             0xE0
#define ST7735_GMCTRN1             0xE1

#endif // _ARCADEIT_LCD_DEFINITIONS_H
