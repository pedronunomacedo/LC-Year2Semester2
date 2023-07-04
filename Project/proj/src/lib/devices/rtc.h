#pragma once

#include <lcom/lcf.h>

/** @defgroup rtc rtc
 * @{
 *
 * Constants and functions for programming RTC
 */

//rtc utils
#define RTC_DELAY 20000
#define RTC_IRQ 8
#define RTC_BASE_YEAR 2000

// rtc registers
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

// rtc address
#define RTC_SECONDS 0
#define RTC_MINUTES 2
#define RTC_HOURS 4
#define RTC_DAY_MONTH 7
#define RTC_MONTH 8
#define RTC_YEAR 9
#define RTC_REG_A 10
#define RTC_REG_B 11
#define RTC_REG_C 12
#define RTC_REG_D 13

// rtc control
#define RTC_UIP BIT(7)
#define RTC_UF BIT(4)
#define RTC_UIE BIT(4)
#define RTC_AIE BIT(5)
#define RTC_PIE BIT(6)
#define RTC_DM BIT(2)

/**
 * @brief Subscribes and enables RTC interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int (rtc_subscribe_int)(uint8_t * bit_no);

/**
 * @brief Unsubscribes RTC interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (rtc_unsubscribe_int)();

/**
 * @brief read byte from a register of the RTC
 * 
 * @param reg register where the byte will be read
 * @param input byte read from the register
 * @return int Return 0 upon success and non-zero otherwise
 */
int (rtc_read_register)(uint8_t reg, uint8_t * input);

/**
 * @brief write byte in a register of the RTC
 * 
 * @param reg register where the byte will be write
 * @param output byte to write in the register
 * @return int Return 0 upon success and non-zero otherwise
 */
int (rtc_write_register)(uint8_t reg, uint8_t output);

/**
 * @brief Wait for a valid RTC to read correctly the registers
 * 
 */
void (wait_for_a_valid_rtc)(void);

/**
 * @brief RTC interrupt handler
 * 
 * Read register and set the current date
 */
void (rtc_ih)(void);

/**
 * @brief RTC update-ended interrupt handler
 * 
 */
void (rtc_handle_update_int)(void);

/**
 * @brief Disable RTC interrupts
 * 
 * @return int Return 0 upon success and non-zero otherwise
 */
int (rtc_disable_int)(void);

/**
 * @brief Enable RTC update interrupts
 * 
 * @return int Return 0 upon success and non-zero otherwise
 */
int (rtc_enable_update_int)(void);

/**
 * @brief Get the current date from the RTC
 * 
 * @return char* Returns the current date
 */
char * (rtc_get_current_date)(void);

/**@} */
/* EOF */
