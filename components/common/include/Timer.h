#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdio.h>
#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "soc/timer_group_struct.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"

class Timer
{
    public:
        Timer(timer_group_t group, 
                timer_idx_t index, 
                uint32_t divider,
                bool autoreload = false,
                bool alarmenable = false,
                timer_intr_mode_t mode = TIMER_INTR_LEVEL)
            :m_group(group),
            m_index(index),
            m_divider(divider),
            m_autoreload(autoreload),
            m_alarmenable(alarmenable),
            m_mode(mode)
        {
        }
        void init();
        void Start();
        void Pause();

        esp_err_t getCounterValue(uint64_t * val);
        esp_err_t getCounterValSec(double * time);
        esp_err_t setCounterValue(uint64_t val);

    private:
        timer_group_t m_group;
        timer_idx_t m_index;
        uint32_t m_divider;
        bool m_autoreload;
        bool m_alarmenable;
        timer_intr_mode_t m_mode;

};
#endif

