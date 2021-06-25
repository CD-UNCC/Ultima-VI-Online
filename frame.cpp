#include "frame.h"

/* function definitions */
void frame_init(void){
  FRAME* firstpanel=NULL; //init using NULL (active/closest panel)
  FRM_ONOFF *ONOFF_hold=NULL;
  FRAME *drg=NULL;
}
FRAME* FRMnew(){ //creates a new frame on top of previous frames
  static FRAME* tf;
  tf=(FRAME*)malloc(sizeof(FRAME));
  ZeroMemory(tf,sizeof(FRAME));
  if (firstpanel!=NULL){
    tf->next=firstpanel;
    firstpanel->previous=tf;
  }
  firstpanel=tf;
  tf->active=TRUE;
  tf->display=TRUE;
  tf->move=TRUE;
  return tf;
}

FRM_IMAGE* FRM_IMAGEnew(FRAME* frame,surf* graphic){
  static FRM_IMAGE* FRM_IMAGEp;
  static FRM_TYPE* FRM_TYPEp;
  FRM_IMAGEp=(FRM_IMAGE*)malloc(sizeof(FRM_IMAGE));
  ZeroMemory(FRM_IMAGEp,sizeof(FRM_IMAGE));
  FRM_IMAGEp->type=FRM_IMAGE_TYPE;
  FRM_IMAGEp->graphic=graphic;
  if (frame->firstobject==NULL){
    frame->firstobject=(void*)FRM_IMAGEp;
    return FRM_IMAGEp;
  }
  FRM_TYPEp=(FRM_TYPE*)frame->firstobject;
  while(FRM_TYPEp->next!=NULL){
    FRM_TYPEp=(FRM_TYPE*)FRM_TYPEp->next;
  }
  FRM_TYPEp->next=(void*)FRM_IMAGEp;
  return FRM_IMAGEp;
}

FRM_INPUT* FRM_INPUTnew(FRAME* frame,txt* input){
  static FRM_INPUT* FRM_INPUTp;
  static FRM_TYPE* FRM_TYPEp;
  FRM_INPUTp=(FRM_INPUT*)malloc(sizeof(FRM_INPUT));
  ZeroMemory(FRM_INPUTp,sizeof(FRM_INPUT));
  FRM_INPUTp->type=FRM_INPUT_TYPE;
  FRM_INPUTp->input=input;
  FRM_INPUTp->col=rgb(255,255,255);
  FRM_INPUTp->cursor=95;
  FRM_INPUTp->cursor_blinkrate=4;
  if (frame->firstobject==NULL){
    frame->firstobject=(void*)FRM_INPUTp;
    return FRM_INPUTp;
  }
  FRM_TYPEp=(FRM_TYPE*)frame->firstobject;
  while(FRM_TYPEp->next!=NULL){
    FRM_TYPEp=(FRM_TYPE*)FRM_TYPEp->next;
  }
  FRM_TYPEp->next=(void*)FRM_INPUTp;
  return FRM_INPUTp;
}

FRM_TXT* FRM_TXTnew(FRAME* frame,txt* text){
  static FRM_TXT* FRM_TXTp;
  static FRM_TYPE* FRM_TYPEp;
  FRM_TXTp=(FRM_TXT*)malloc(sizeof(FRM_TXT));
  ZeroMemory(FRM_TXTp,sizeof(FRM_TXT));
  FRM_TXTp->type=FRM_TXT_TYPE;
  FRM_TXTp->text=text;
  FRM_TXTp->col=rgb(255,255,255);
  if (frame->firstobject==NULL){
    frame->firstobject=(void*)FRM_TXTp;
    return FRM_TXTp;
  }
  FRM_TYPEp=(FRM_TYPE*)frame->firstobject;
  while(FRM_TYPEp->next!=NULL){
    FRM_TYPEp=(FRM_TYPE*)FRM_TYPEp->next;
  }
  FRM_TYPEp->next=(void*)FRM_TXTp;
  return FRM_TXTp;
}

FRM_ONOFF* FRM_ONOFFnew(FRAME* frame,surf* graphic_on, surf* graphic_off){
  static FRM_ONOFF* FRM_ONOFFp;
  static FRM_TYPE* FRM_TYPEp;
  FRM_ONOFFp=(FRM_ONOFF*)malloc(sizeof(FRM_ONOFF));
  ZeroMemory(FRM_ONOFFp,sizeof(FRM_ONOFF));
  FRM_ONOFFp->type=FRM_ONOFF_TYPE;
  FRM_ONOFFp->graphic_on=graphic_on;
  FRM_ONOFFp->graphic_off=graphic_off;
  if (frame->firstobject==NULL){
    frame->firstobject=(void*)FRM_ONOFFp;
    return FRM_ONOFFp;
  }
  FRM_TYPEp=(FRM_TYPE*)frame->firstobject;
  while(FRM_TYPEp->next!=NULL){
    FRM_TYPEp=(FRM_TYPE*)FRM_TYPEp->next;
  }
  FRM_TYPEp->next=(void*)FRM_ONOFFp;
  return FRM_ONOFFp;
}

