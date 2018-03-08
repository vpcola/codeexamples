#include "SPILMIC.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "lmic.h"

#define ESP_INTR_FLAG_DEFAULT 0

static const char* TAG = "SPILMIC";

// Static event queue handle
xQueueHandle SPILMIC::m_evtqueue = NULL;

void  IRAM_ATTR SPILMIC::dio_isr_handler(void *arg)
{
    uint32_t dionum = (uint32_t) arg;
    xQueueSendFromISR(SPILMIC::m_evtqueue, &dionum, NULL);
}

void SPILMIC::init()
{
    // Call base initialization
    SPIDevice::init();
    // Initialize gpios
    //

    ESP_LOGI(TAG, "Initializing GPIO's ...");
    // RST Pin
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = 1ULL<< m_rst;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    // Setup all DIO pins to use interrupts
    // DIOs are activated on the rising edge
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.pin_bit_mask = ((1ULL<< m_dio0) | (1ULL<< m_dio1) | (1ULL<< m_dio2));
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);

    ESP_LOGI(TAG, "Initializing event queues ...");
    // Create a queue to handle the event from ISR
    m_evtqueue = xQueueCreate(10, sizeof(uint32_t));

    ESP_LOGI(TAG, "Initializing interrupt handlers ...");
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    // hook isr handler to the specific GPIO pins
    gpio_isr_handler_add(m_dio0, &SPILMIC::dio_isr_handler, (void *) 0);
    gpio_isr_handler_add(m_dio1, &SPILMIC::dio_isr_handler, (void *) 1);
    gpio_isr_handler_add(m_dio2, &SPILMIC::dio_isr_handler, (void *) 2);

    // Create a timer to provide ticks to the lora module
    ESP_LOGI(TAG, "Initializing timers ...");
    m_timer.init();

    ESP_LOGI(TAG, "Starting timer ...");
    /*Start timer counter*/
    m_timer.Start();

    ESP_LOGI(TAG, "Initializing LMIC State machine ...");
    os_init();

    ESP_LOGI(TAG, "Initializing LMIC Hal subsystem ...");
    hal_init();
    ESP_LOGI(TAG, "Initializing LMIC Radion ...");
    radio_init();

    // initialize LMIC subsystem
    ESP_LOGI(TAG, "Initializing LMIC ...");
    LMIC_init();
    ESP_LOGI(TAG, "Resetting LMIC ...");
    LMIC_reset();
}

