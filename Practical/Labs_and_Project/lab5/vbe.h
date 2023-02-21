#pragma once

#include <lcom/lcf.h>

#define VBE_AH 0x4f
#define VBE_INTERRUPT_VECTOR 0x10
#define LINEAR_FRAME_BUFFER BIT(14)

//vbe functions
#define RETURN_VBE_INFO 0x00
#define RETURN_VBE_MODE 0x01
#define SET_VBE_MODE 0x02
#define SET_TEXT_MODE 0x03
#define AL_SET_VBE_START 0x07
#define BL_SET_VBE_START 0x0
#define CL_SET_VBE_START 0x0

//vbe errors
#define FUNCTION_CALL_FAILED 0x01
#define FUNCTION_NOT_SUPPORTED 0x02
#define FUNCTION_IS_INVALID 0x03

//vbe modes
#define MODE_RES_1024x768_BITS_8 0x105 //8
#define MODE_RES_640x480_BITS_15 0x110 //((1:)5:5:5)
#define MODE_RES_800x600_BITS_24 0x115 //(8:8:8)
#define MODE_RES_1280x1024_BITS_16 0x11A //(5:6:5)
#define MODE_RES_1152x864_BITS_32 0x14C //((8:)8:8:8)
