#ifndef FRAME_H
#define FRAME_H

#include <winsock2.h>
#include <windows.h> /* UINT */ 
#include "sound.h" /* sound* etc */
#include "myddraw.h" /* surf */

/* #defines */
#define FRM_ONOFF_TYPE 1
#define FRM_IMAGE_TYPE 2
#define FRM_TXT_TYPE 3
#define FRM_INPUT_TYPE 4
#define FRM_LIST_TYPE 5

/* structure definitions */
typedef struct _FRAME {
  short offset_x,offset_y; //offset of top, left corner
  short size_x,size_y; //width and height
  bool display; //TRUE, displayes panel on screen (FALSE also deactivates the panel)
  bool active; //TRUE, processes mouse/keyboard input
  bool no_focus; //TRUE, panel can't be brought to the front
  bool move; //TRUE, panel can be dragged/moved with the second mouse button
  void* previous; //pointer to the previous panel (NULL if first panel)
  void* next; //pointer to the next panel (NULL if last panel)
  void* firstobject; //pointer to first object
  sound *sound_move; //sound when frame is moved
  sound *sound_select; //sound when frame is selected
  surf *graphic; //graphic to use for frame background
  bool graphic_use0pixel; //draws index 0 pixels of graphic
  surf *graphic_mask; //if graphic defines frame boundries, _mask defines it's shape
  bool mouse_pressed; //TRUE if mouse button is pressed, user reads value then sets to FALSE
  bool mouse_click; //TRUE if mouse has been pushed, user reads value then sets to FALSE
  bool mouse_over; //TRUE if mouse is over the frame, user reads value then sets to FALSE
  short mouse_x,mouse_y; //position of mouse at last press position
  unsigned char mouse_over_transparent;//default is FALSE
  short default_size_x, default_size_y;
  bool scaling;
  float scale_x, scale_y;
} FRAME;

typedef struct _FRM_TYPE {
  long type;
  void* next; //pointer to next object
} FRM_TYPE;

typedef struct _FRM_ONOFF { //buttons that return an ON/OFF status
  long type; //this value must be set to FRM_ONOFF_TYPE
  void* next; //pointer to next object
  unsigned char value; //0="off" 1="on" ...value can be used as a multiplier
  surf *graphic_off; //graphic of button when off
  surf *graphic_on; //graphic of button when on
  short offset_x,offset_y; //offset inside frame
  short offset2_x,offset2_y; //offset of "on" graphic button from "off" graphic
  bool hold; //TRUE if button must be held down to keep active
  sound *sound_off; //sound when button is turned off
  sound *sound_on; //sound when button is turned on
} FRM_ONOFF;
typedef struct _FRM_IMAGE {
  long type; //this value must be set to FRM_ONOFF_TYPE
  void* next; //pointer to next object
  surf *graphic; //graphic to display
  bool graphic_use0pixel; //draws index 0 pixels of graphic
  short offset_x,offset_y; //offset inside frame
} FRM_IMAGE;
typedef struct _FRM_TXT { //simple text output
  long type; //this value must be set to FRM_TXT_TYPE
  void* next; //pointer to next object
  short offset_x,offset_y; //offset inside frame
  txt *text; //text to display
  HFONT font; //windows font
  long col; //font colour
  float timer; //when timer(sec) finishes, text is not displayed
  unsigned char special; //flags 1|shadow
} FRM_TXT;
typedef struct _FRM_INPUT { //keyboard input with display and feedback
  long type; //this value must be set to FRM_INPUT_TYPE
  void* next; //pointer to next object
  short offset_x,offset_y; //offset inside parent frame
  short size_x,size_y; //width and height
  surf *graphic; //graphic defines width and height if size_x=0 and size_y=0
  txt *input; //buffer for keyboard input (manually clear as necessary)
  unsigned short length_limit; //0=unlimited
  unsigned char enterpressed; //TRUE if enter has been pressed (manually set to FALSE)
  txt *prefix;
  txt *suffix;
  unsigned char cursor; //0=none, ASCII character to use for cursor
  float cursor_blinkrate; //0=no blink, 1=blink once a second (higher is faster)
  HFONT font; //windows font
  long col; //font colour
  unsigned char special; //flags 1|shadow
} FRM_INPUT;
typedef struct _FRM_LIST_ITEM { //keyboard input with display and feedback
  void* next; //pointer to next list item
  txt* text; //text to display
  surf* graphic; //picture to display (text appears to the right)
  unsigned long flags; //list item flags (eg.1=selected)
} FRM_LIST_ITEM;
typedef struct _FRM_LIST { //keyboard input with display and feedback
  long type; //this value must be set to FRM_INPUT_TYPE
  void* next; //pointer to next object

  short offset_x,offset_y; //offset inside parent frame

  short size_x,size_y; //width and height
  surf *graphic; //graphic defines width and height if size_x=0 and size_y=0

  surf *graphic_select; //selection of items

  void* FRM_LIST_ITEM_first;
  unsigned long list_first_item;
  unsigned long list_length; //number of items to appear on screen
  unsigned long list_spacing; //number of v.space for list items

  HFONT font; //windows font
  long col; //font colour

} FRM_LIST;

/* external global.h variables */
extern FRAME* firstpanel;
extern FRM_ONOFF *ONOFF_hold;
//frame: information
extern FRAME *FRAME_mblf[2];
extern bool FRAME_mbl[2];
extern long* objtype;
extern FRAME *drg;
extern bool FRAME_drg_begin;
extern FRM_TYPE* FRM_type;
extern FRAME *pn,*lpn,*pn2,*pn3;
extern FRM_ONOFF *tonoff;
extern FRM_IMAGE *timage;
extern FRM_INPUT *tinp;
extern long FRAME_mb;
extern HFONT thfont;
extern FRM_TXT *ttxt;

/* function prototypes */
void frame_init(void);
FRAME* FRMnew(); //creates a new frame on top of previous frames
FRM_IMAGE* FRM_IMAGEnew(FRAME* frame,surf* graphic);
FRM_INPUT* FRM_INPUTnew(FRAME* frame,txt* input);
FRM_TXT* FRM_TXTnew(FRAME* frame,txt* text);
FRM_ONOFF* FRM_ONOFFnew(FRAME* frame,surf* graphic_on, surf* graphic_off);

#endif /* FRAME_H */
