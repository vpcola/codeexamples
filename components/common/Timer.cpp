#include "Timer.h"

void Timer::init()
{
    timer_config_t config;

    config.divider = m_divider;
    config.counter_dir = TIMER_COUNT_UP;
    config.counter_en = TIMER_PAUSE; // Start paused
    config.alarm_en = m_alarmenable;
    config.auto_reload = m_autoreload;
    config.intr_type = TIMER_INTR_LEVEL;
    ::timer_init(m_group, m_index, &config);

    /* timer will start from a value below, also
     * the value loaded whtn auto_reload is set.
     */
    ::timer_set_counter_value(m_group, m_index, 0x00000000ULL);

    /* TODO: Configure alarm if enabled */
}

void Timer::Start()
{
    ::timer_start(m_group, m_index);
}

void Timer::Pause()
{
    ::timer_pause(m_group, m_index);
}

esp_err_t Timer::getCounterValue(uint64_t * val)
{
    return ::timer_get_counter_value(m_group, m_index, val);
}

esp_err_t Timer::getCounterValSec(double * time)
{
    return ::timer_get_counter_time_sec(m_group, m_index, time);
}

esp_err_t Timer::setCounterValue(uint64_t val)
{
    return ::timer_set_counter_value(m_group, m_index, val);
}

