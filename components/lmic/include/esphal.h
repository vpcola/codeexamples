#ifndef _hal_hpp_
#define _hal_hpp_

//#define CFG_eu868 1
#define CFG_us915 1
#define CFG_sx1276_radio 1


#ifdef __cplusplus
extern "C" {
#endif

#define TIMER_DIVIDER   1600
// ESP32's APB Clock (TIMER_BASE_CLK) is running at 80Mhz
#define TIMER_SCALE     (TIMER_BASE_CLK / TIMER_DIVIDER)  // convert counter value to seconds
#define OSTICKS_PER_SEC 50000 // TIMER_SCALE = 50000 (50Khz)

typedef struct {
    u1_t nss;
    u1_t rst;
    u1_t dio[3];  // DIO0, DIO1, DIO2
    u1_t spi[3];  // MISO, MOSI, SCK
} lmic_pinmap_t;

/*
 * initialize hardware (IO, SPI, TIMER, IRQ).
 */
void hal_init (void);

/*
 * drive radio NSS pin (0=low, 1=high).
 */
void hal_pin_nss (u1_t val);

/*
 * drive radio RX/TX pins (0=rx, 1=tx).
 */
void hal_pin_rxtx (u1_t val);

/*
 * control radio RST pin (0=low, 1=high, 2=floating)
 */
void hal_pin_rst (u1_t val);

/*
 * perform 8-bit SPI transaction with radio.
 *   - write given byte 'outval'
 *   - read byte and return value
 */
u1_t hal_spi (u1_t outval);

/*
 * disable all CPU interrupts.
 *   - might be invoked nested
 *   - will be followed by matching call to hal_enableIRQs()
 */
void hal_disableIRQs (void);

/*
 * enable CPU interrupts.
 */
void hal_enableIRQs (void);

/*
 * put system and CPU in low-power mode, sleep until interrupt.
 */
void hal_sleep (void);

/*
 * return 32-bit system time in ticks.
 */
ll_u8_t hal_ticks (void);

/*
 * busy-wait until specified timestamp (in ticks) is reached.
 */
void hal_waitUntil (ll_u8_t time);

/*
 * check and rewind timer for target time.
 *   - return 1 if target time is close
 *   - otherwise rewind timer for target time or full period and return 0
 */
u1_t hal_checkTimer (ll_u8_t targettime);

/*
 * check if any interrupts has sent data to the queue and handles them
 *
 */
void hal_io_check(void);

/*
 * perform fatal failure action.
 *   - called by assertions
 *   - action could be HALT or reboot
 */
void hal_failed (void);

#ifdef __cplusplus
}
#endif

#endif // _hal_hpp_
