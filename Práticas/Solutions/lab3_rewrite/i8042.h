#pragma once

#define ESQ_BREAK_CODE 0x81
#define TWO_BYTE_CODE 0xe0
#define KBD_IRQ 1

#define KBD_ST_REG 0x64
#define KBD_OUT_BUF 0x60

#define KBD_RCB 0x20
#define KBD_WCB 0x60
#define KBD_CMD_REG 0x64
#define KBD_ARG_REG 0x60

#define KBD_ENABLE_INT BIT(0)

#define KBD_PARITY_ERROR BIT(7)
#define KBD_TIMEOUT_ERROR BIT(6)
#define KBD_OBF BIT(0)

#define SCANCODE_MSB BIT(7)

#define DELAY 20000
