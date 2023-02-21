#pragma once

#include <lcom/lcf.h>

#define DELAY 20000

#define KBD_ESQ_BC 0x81 //ESQ breakcode
#define KBD_TWOBYTE_CODE 0xE0 //first byte of a two-byte scancode
#define KBD_MSB BIT(7) //MSB of a byte

#define KBD_IRQ 1 //keyboard irq line number

#define KBC_ST_REG 0x64 //keyboard status register
#define KBC_OBF BIT(0) //status register output buffer full bit
#define KBC_AUX BIT(5) //status register AUX bit
#define KBC_PARITY_ERR BIT(7) //status register parity error bit
#define KBC_TIMEOUT_ERR BIT(6) //status register timeout error bit

#define KBC_OUT_BUF 0x60 //keyboard output buffer

#define KBC_CMD_REG 0x64 //keyboard command register

#define KBC_CMD_RCB 0x20 //keyboard read command byte
#define KBC_CMD_WCB 0x60 //keyboard write command byte

#define KBC_INT BIT(0) //command byte enable interrupt on keyboard 
