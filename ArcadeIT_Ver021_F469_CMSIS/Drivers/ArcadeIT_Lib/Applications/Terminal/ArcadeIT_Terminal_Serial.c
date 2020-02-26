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
 * @file    ArcadeIT_Serial_Terminal.c
 * @version V0.13
 * @date    13-02-2020
 * @last
 * @brief   This library is used to implement a terminal over a serial communication.
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HARDWARE USED

 1.3 - Serial port

  ArcadeIT! Motherboard CN7 - Serial Port

  Pin  GPIO Function    Serial interface
  ---------------------------------------
  3.3V VDD  3.3V        CN7-1
  042  PA2  USART2 TX   CN7-2
  047  PA3  USART2 RX   CN7-3
  GND  VSS  GND         CN7-4

 ArcadeIT! mainboard:

  Serial port  / CN7
  .----.
  | o  |  1 3.3V
  | o |   2 TX
  | o |   3 RX
  | o  |  4 GND
  '----'

 ******************************************************************************
 HISTORY
 13-02-2020:
 - Creation date of the library structure.

 *******************************************************************************
 */

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include <System/ArcadeIT_Common.h>
#include <System/ArcadeIT_Utilities.h>
#include <System/ArcadeIT_Firmware.h>

// ArcadeIT! Applications
#include <Applications/Terminal/ArcadeIT_Terminal_Serial.h>

// ArcadeIT! Peripherals and buses.
#include <System/Devices/ArcadeIT_Serial_Port.h>

// /////////////////////////////////////////////////////////////////////////////
// Global elements.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Functions.
// /////////////////////////////////////////////////////////////////////////////
void color_set(uint8_t pColorForeground, uint8_t pColorBackground)
{
  char s[256];
  sprintf(s, ANSI_COLOR_256_FGBG, pColorForeground, pColorBackground);
  ArcadeIT_Serial_Port_String_Send(s);

} // end color_set

// -----------------------------------------------------------------------------
void cursor_at(uint8_t x, uint8_t y)
{
  char s[256];
  sprintf(s, CURSOR_SET, y, x);
  ArcadeIT_Serial_Port_String_Send(s);

} // end cursor_at

// -----------------------------------------------------------------------------
void label_draw(
    label_t *pLabel,
    frame_t *pFrame
        )
{
  //  __0 label 1_________
  // |                    |
  cursor_at(pLabel->x, pLabel->y);

  if (pFrame != NULL)
    color_set(pFrame->cf, pFrame->cb);
  else
    color_set(pLabel->cf, pLabel->cb);

  ArcadeIT_Serial_Port_String_Send(FONT_GFX_ON);
  ArcadeIT_Serial_Port_Char_Send(pLabel->skin[LABEL_LEFT]);

  if (pFrame != NULL)
    color_set(pLabel->cf, pLabel->cb);

  ArcadeIT_Serial_Port_String_Send(FONT_GFX_OFF);
  ArcadeIT_Serial_Port_String_Send(pLabel->text);
  ArcadeIT_Serial_Port_String_Send(FONT_GFX_ON);

  if (pFrame != NULL)
    color_set(pFrame->cf, pFrame->cb);

  ArcadeIT_Serial_Port_Char_Send(pLabel->skin[LABEL_RIGHT]);

} // end label_draw

// -----------------------------------------------------------------------------
void frame_draw(
        frame_t* pFrame
        )
{
  color_set(pFrame->cf, pFrame->cb);
  cursor_at(pFrame->x1, pFrame->y1);

  ArcadeIT_Serial_Port_String_Send(FONT_GFX_ON);
  // ----

  //  ____________________
  // |                    |
  ArcadeIT_Serial_Port_Char_Send(pFrame->skin[FRAME_CORNER_UPPER_LEFT]);
  for (uint8_t x = pFrame->x1 + 1; x < pFrame->x2; x++)
  {
    ArcadeIT_Serial_Port_Char_Send(pFrame->skin[FRAME_EDGE_UPPER_HORIZONTAL]);

  } // end for
  ArcadeIT_Serial_Port_Char_Send(pFrame->skin[FRAME_CORNER_UPPER_RIGHT]);

  // |                    |
  for (uint8_t y = pFrame->y1 + 1; y < pFrame->y2; y++)
  {
    cursor_at(pFrame->x1, y);
    ArcadeIT_Serial_Port_Char_Send(pFrame->skin[FRAME_EDGE_LEFT_VERTICAL]);

    for (uint8_t x = pFrame->x1 + 1; x < pFrame->x2; x++)
    {
      ArcadeIT_Serial_Port_Char_Send(pFrame->skin[FRAME_SPACE]);

    } // end for
    ArcadeIT_Serial_Port_Char_Send(pFrame->skin[FRAME_EDGE_RIGHT_VERTICAL]);

  } // end for

  // |_____________________|
  cursor_at(pFrame->x1, pFrame->y2);
  ArcadeIT_Serial_Port_Char_Send(pFrame->skin[FRAME_CORNER_LOWER_LEFT]);
  for (uint8_t x = pFrame->x1 + 1; x < pFrame->x2; x++)
  {
    ArcadeIT_Serial_Port_Char_Send(pFrame->skin[FRAME_EDGE_LOWER_HORIZONTAL]);

  } // end for
  ArcadeIT_Serial_Port_Char_Send(pFrame->skin[FRAME_CORNER_LOWER_RIGHT]);

  // ----
  ArcadeIT_Serial_Port_String_Send(FONT_GFX_OFF);

} // end frame_draw

// -----------------------------------------------------------------------------
void slider_h_calculate_marker_position(slider_h_t * pSlider)
{
  pSlider->x_marker_previously = pSlider->x_marker;
  pSlider->x_marker = (uint8_t) (pSlider->x1 + 1 + ((pSlider->value - pSlider->value_min + 1) / (float) (pSlider->value_max - pSlider->value_min + 1) * (pSlider->x2 - 1 - pSlider->x1 - 1)));

} // end slider_h_calculate_marker_position

// -----------------------------------------------------------------------------
void slider_h_draw(
        slider_h_t *pSlider // the pointer to a structure of a slider
        )
{
    cursor_at(pSlider->x1, pSlider->y);

    ArcadeIT_Serial_Port_String_Send(FONT_GFX_ON);
    // ----

    color_set(pSlider->skin[SLIDER_H_ARROW_LEFT_FG], pSlider->skin[SLIDER_H_ARROW_LEFT_BG]);
    ArcadeIT_Serial_Port_Char_Send(pSlider->skin[SLIDER_H_ARROW_LEFT]);

    color_set(pSlider->skin[SLIDER_H_BLOCK_FG], pSlider->skin[SLIDER_H_BLOCK_BG]);
    for (uint8_t x = pSlider->x1 + 1; x < pSlider->x2; x++)
    {
        if (x == pSlider->x_marker)
          {
          color_set(pSlider->skin[SLIDER_MARKER_FG], pSlider->skin[SLIDER_MARKER_BG]);
          ArcadeIT_Serial_Port_Char_Send(pSlider->skin[SLIDER_MARKER]);
          color_set(pSlider->skin[SLIDER_H_BLOCK_FG], pSlider->skin[SLIDER_H_BLOCK_BG]);
          }
        else
          ArcadeIT_Serial_Port_Char_Send(pSlider->skin[SLIDER_H_BLOCK]);

    } // end for
    color_set(pSlider->skin[SLIDER_H_ARROW_RIGHT_FG], pSlider->skin[SLIDER_H_ARROW_RIGHT_BG]);
    ArcadeIT_Serial_Port_Char_Send(pSlider->skin[SLIDER_H_ARROW_RIGHT]);

    // ----
    ArcadeIT_Serial_Port_String_Send(FONT_GFX_OFF);

} // end slider_draw

// -----------------------------------------------------------------------------
void slider_h_refresh(
    slider_h_t *pSlider
    )
{
    slider_h_calculate_marker_position(pSlider);

    if (pSlider->x_marker != pSlider->x_marker_previously) slider_h_draw(pSlider);

} // end slider_h_refresh

// -----------------------------------------------------------------------------
void slider_v_calculate_marker_position(slider_v_t * pSlider)
{
  pSlider->y_marker_previously = pSlider->y_marker;
  pSlider->y_marker = (uint8_t) (pSlider->y1 + 1 + ((pSlider->value - pSlider->value_min + 1) / (float) (pSlider->value_max - pSlider->value_min + 1) * (pSlider->y2 - 1 - pSlider->y1 - 1)));

} // end slider_v_calculate_marker_position

// -----------------------------------------------------------------------------
void slider_v_draw(
        slider_v_t *pSlider // the pointer to a structure of a slider
        )
{
    cursor_at(pSlider->x, pSlider->y1);

    ArcadeIT_Serial_Port_String_Send(FONT_GFX_ON);
    // ----

    color_set(pSlider->skin[SLIDER_V_ARROW_UP_FG], pSlider->skin[SLIDER_V_ARROW_UP_BG]);
    ArcadeIT_Serial_Port_Char_Send(pSlider->skin[SLIDER_V_ARROW_UP]);

    color_set(pSlider->skin[SLIDER_V_BLOCK_FG], pSlider->skin[SLIDER_V_BLOCK_BG]);
    uint8_t y;
    for (y = pSlider->y1 + 1; y < pSlider->y2; y++)
    {
      cursor_at(pSlider->x, y);

      if (y == pSlider->y_marker)
        {
        color_set(pSlider->skin[SLIDER_MARKER_FG], pSlider->skin[SLIDER_MARKER_BG]);
        ArcadeIT_Serial_Port_Char_Send(pSlider->skin[SLIDER_MARKER]);
        color_set(pSlider->skin[SLIDER_V_BLOCK_FG], pSlider->skin[SLIDER_V_BLOCK_BG]);
        }
      else
        ArcadeIT_Serial_Port_Char_Send(pSlider->skin[SLIDER_V_BLOCK]);

    } // end for

    cursor_at(pSlider->x, y);

    color_set(pSlider->skin[SLIDER_V_ARROW_DOWN_FG], pSlider->skin[SLIDER_V_ARROW_DOWN_BG]);
    ArcadeIT_Serial_Port_Char_Send(pSlider->skin[SLIDER_V_ARROW_DOWN]);

    // ----
    ArcadeIT_Serial_Port_String_Send(FONT_GFX_OFF);

} // end slider_draw

// -----------------------------------------------------------------------------
void slider_v_refresh(
    slider_v_t *pSlider
    )
{
    slider_v_calculate_marker_position(pSlider);

    if (pSlider->y_marker != pSlider->y_marker_previously) slider_v_draw(pSlider);

} // end slider_v_refresh

// -----------------------------------------------------------------------------
void window_draw(
    window_t *pWindow // the pointer to a structure of a window
    )
{
  label_t  title     = {pWindow->x + 1, pWindow->y, 220, 18, pWindow->title, LABEL_SKIN4_ANSI};

  frame_t  frame     = {
      pWindow->x, pWindow->y,
      pWindow->x + pWindow->width, pWindow->y + pWindow->height,
      27, 18,
      title,
      FRAME_SKIN_ANSI
  };

  slider_h_t sliderh = {pWindow->x + 1, pWindow->x + pWindow->width - 1 - (pWindow->flags & WINDOW_SLIDER_H ? 1 : 0), pWindow->y + pWindow->height - 1, false, 0.0, 10.0, 2.0, 0.0, 0, 0, SLIDER_SKIN_ANSI};
  slider_v_t sliderv = {pWindow->x + pWindow->width - 1, pWindow->y + 1, pWindow->y + pWindow->height - 1 - (pWindow->flags & WINDOW_SLIDER_V ? 1 : 0), false, 0.0, 10.0, 2.0, 0.0, 0, 0, SLIDER_SKIN_ANSI};

  frame_draw(&frame);

  if (pWindow->flags & WINDOW_TITLE)
    label_draw(&title, &frame);

  if (pWindow->flags & WINDOW_SLIDER_H)
    slider_h_refresh(&sliderh);

  if (pWindow->flags & WINDOW_SLIDER_V)
    slider_v_refresh(&sliderv);

} // end window_draw

// -----------------------------------------------------------------------------
void window_move(
    window_t *pWindow, // the pointer to a structure of a window
    int8_t dx,
    int8_t dy
    )
{
  pWindow->x += dx;
  pWindow->y += dy;

  // restore buffer under the window
  //copy_buffer(from_buffer, to_buffer, w, h);

  // save the buffer of the new position
  //copy_buffer(from_buffer, to_buffer, w, h);

  // draw window in the new position

}  //  end window_move

// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////
