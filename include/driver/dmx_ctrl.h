#pragma once

#include "driver/dmx.h"
#include "hal/uart_hal.h"
#include "soc/uart_caps.h"
#include "freertos/semphr.h"

#define DMX_CONTEX_INIT_DEF(dmx_num)                               \
  {                                                                \
    .hal.dev = UART_LL_GET_HW(dmx_num),                            \
    .spinlock = portMUX_INITIALIZER_UNLOCKED, .hw_enabled = false, \
  }

typedef struct {
  dmx_port_t dmx_num;
  QueueHandle_t queue;
  dmx_isr_handle_t intr_handle;

  uint16_t rx_buffer_size;
  uint8_t *rx_buffer;
  uint16_t rx_slot_idx;  // The index of the current slot that is being rx'd.

  int64_t rx_last_byte_ts;  // Timestamp of the last rx'd byte.
  int64_t rx_last_brk_ts;  // Timestamp of the last rx'd break.
  int64_t rx_valid_len;  // The valid frame length received.

  uint16_t tx_buffer_size;
  uint8_t *tx_buffer;
  uint16_t tx_slot_idx;  // The index of the current slot that is being tx'd.
  SemaphoreHandle_t tx_done_sem;  // Signals the frame has finished being sent.

  int64_t tx_last_brk_ts;  // Timestamp of the last tx'd break. 

} dmx_obj_t;

typedef struct {
  uart_hal_context_t hal;
  portMUX_TYPE spinlock;
  bool hw_enabled;
} dmx_context_t;

dmx_obj_t *p_dmx_obj[DMX_NUM_MAX] = {0};

dmx_context_t dmx_context[DMX_NUM_MAX] = {
    DMX_CONTEX_INIT_DEF(DMX_NUM_0),
    DMX_CONTEX_INIT_DEF(DMX_NUM_1),
#if DMX_NUM_MAX > 2
    DMX_CONTEX_INIT_DEF(DMX_NUM_2),
#endif
};