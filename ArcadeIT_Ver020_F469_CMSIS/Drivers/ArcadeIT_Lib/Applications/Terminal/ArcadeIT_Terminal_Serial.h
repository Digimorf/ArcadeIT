/*
 * ArcadeIT_Terminal_Serial.h
 *
 *  Created on: Feb 13, 2020
 *      Author: Francesco
 */

#ifndef ARCADEIT_LIB_APPLICATIONS_TERMINAL_ARCADEIT_TERMINAL_SERIAL_H_
#define ARCADEIT_LIB_APPLICATIONS_TERMINAL_ARCADEIT_TERMINAL_SERIAL_H_

// /////////////////////////////////////////////////////////////////////////////
// Definitions.
// /////////////////////////////////////////////////////////////////////////////
#define WINDOW_FRAME_SKIN_ANSI_START  0
#define FRAME_SPACE                   0
#define FRAME_CORNER_LOWER_RIGHT      1
#define FRAME_CORNER_UPPER_RIGHT      2
#define FRAME_CORNER_UPPER_LEFT       3
#define FRAME_CORNER_LOWER_LEFT       4
#define FRAME_EDGE_LOWER_HORIZONTAL   5
#define FRAME_EDGE_LEFT_VERTICAL      6
#define FRAME_EDGE_UPPER_HORIZONTAL   7
#define FRAME_EDGE_RIGHT_VERTICAL     8
#define FRAME_SKIN_ANSI               "\x20\x6a\x6b\x6c\x6d\x71\x78\x71\x78"

#define WINDOW_LABEL_SKIN_ANSI_START  WINDOW_FRAME_SKIN_ANSI_START + 9
#define LABEL_SPACE                   0
#define LABEL_LEFT                    1
#define LABEL_RIGHT                   2

#define LABEL_SKIN0_ANSI              "\x20\x0\x0"   // no edges
#define LABEL_SKIN1_ANSI              "\x20\x3c\x3e" //  <label>
#define LABEL_SKIN2_ANSI              "\x20\x5b\x5d" //  [label]
#define LABEL_SKIN3_ANSI              "\x20\x28\x29" //  (label)
#define LABEL_SKIN4_ANSI              "\x20\x75\x74" // -|label|-

#define WINDOW_SLIDER_SKIN_ANSI_START WINDOW_LABEL_SKIN_ANSI_START + 3
#define SLIDER_H_ARROW_LEFT           0
#define SLIDER_H_ARROW_LEFT_FG        1
#define SLIDER_H_ARROW_LEFT_BG        2
#define SLIDER_H_ARROW_RIGHT          3
#define SLIDER_H_ARROW_RIGHT_FG       4
#define SLIDER_H_ARROW_RIGHT_BG       5
#define SLIDER_H_BLOCK                6
#define SLIDER_H_BLOCK_FG             7
#define SLIDER_H_BLOCK_BG             8
#define SLIDER_V_ARROW_UP             9
#define SLIDER_V_ARROW_UP_FG          10
#define SLIDER_V_ARROW_UP_BG          11
#define SLIDER_V_ARROW_DOWN           12
#define SLIDER_V_ARROW_DOWN_FG        13
#define SLIDER_V_ARROW_DOWN_BG        14
#define SLIDER_V_BLOCK                15
#define SLIDER_V_BLOCK_FG             16
#define SLIDER_V_BLOCK_BG             17
#define SLIDER_MARKER                 18
#define SLIDER_MARKER_FG              19
#define SLIDER_MARKER_BG              20

#define SLIDER_SKIN_ANSI              "\x3c\x1B\x14\x3e\x1b\x14\x71\x1B\x14\x5e\x1b\x14\x56\x1b\x14\x78\x1b\x14\x61\x1B\x14"

#define WINDOW_SLIDER_H               0x01
#define WINDOW_SLIDER_V               0x02
#define WINDOW_TITLE                  0x04

#define WINDOW_SKIN_ANSI              FRAME_SKIN_ANSI LABEL_SKIN2_ANSI SLIDER_SKIN_ANSI

// /////////////////////////////////////////////////////////////////////////////
// Macros
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Types
// /////////////////////////////////////////////////////////////////////////////ù
typedef struct
{
    int8_t  x, y;
    uint8_t cf, cb;
    char    *text;

    char    *skin;

} label_t;

// -----------------------------------------------------------------------------
typedef struct
{
    uint8_t x1, y1;
    uint8_t x2, y2;
    uint8_t cf, cb;
    label_t title;

    char    *skin;

} frame_t;

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
typedef struct
{
    // drawing attributes
    int8_t   x1, x2, y;
    bool     selected;

    // value
    float    value_min;
    float    value_max;
    float    value;
    float    stepsize;

    uint8_t  x_marker;
    uint8_t  x_marker_previously;

    char    *skin;

} slider_h_t;

// -----------------------------------------------------------------------------
typedef struct
{
    // drawing attributes
    int8_t   x, y1, y2;
    bool     selected;

    // value
    float    value_min;
    float    value_max;
    float    value;
    float    stepsize;

    uint8_t  y_marker;
    uint8_t  y_marker_previously;

    char    *skin;

} slider_v_t;

// -----------------------------------------------------------------------------
typedef struct
{
  char    *title;

  uint8_t x,y;
  uint8_t width, height;
  uint8_t cursor_x, cursor_y;

  char    *skin;

  char    *buffer_foreground;
  char    *buffer_background;

  uint8_t flags;

} window_t;

// -----------------------------------------------------------------------------
typedef struct
{
  char    *title;
  uint8_t width, height;
  char    *buffer;
  uint8_t flags;

} screen_t;

// /////////////////////////////////////////////////////////////////////////////
// Functions.
// /////////////////////////////////////////////////////////////////////////////
void frame_draw(
    frame_t *pFrame // the pointer to a structure of a frame
    );

// -----------------------------------------------------------------------------
void label_draw(
    label_t *pLabel,
    frame_t *pFrame
    );

// -----------------------------------------------------------------------------
void slider_h_draw(
    slider_h_t *pSlider // the pointer to a structure of a slider
    );

// -----------------------------------------------------------------------------
void slider_h_refresh(
    slider_h_t *pSlider
    );

// -----------------------------------------------------------------------------
void slider_v_draw(
    slider_v_t *pSlider // the pointer to a structure of a slider
    );

// -----------------------------------------------------------------------------
void slider_v_refresh(
    slider_v_t *pSlider
    );

// -----------------------------------------------------------------------------
void window_draw(
    window_t *pWindow // the pointer to a structure of a window
    );

// /////////////////////////////////////////////////////////////////////////////

#endif /* ARCADEIT_LIB_APPLICATIONS_TERMINAL_ARCADEIT_TERMINAL_SERIAL_H_ */
