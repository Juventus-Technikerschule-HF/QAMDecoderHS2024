/********************************************************************************************* */
//    Eduboard2 ESP32-S3 Template with BSP
//    Author: Martin Burger
//    Juventus Technikerschule
//    Version: 1.0.0
//    
//    This is the starting point for the QAM-Decoder Project. ADC-Streaming for Channel AN1
//    and AN2 is set up and working. 
//    Caution: On the ESP32-S3 UP-Board two Resistors have to be soldered differently.
//    LED2 and LED3 have to be resoldered to GPIOX0 and GPIOX1
//    If one would want to setup DAC-Streaming in the same Program, the LCD must be disabled
//    For ADC-Streaming the LCD can remain active.
/********************************************************************************************* */
#include "eduboard2.h"
#include "memon.h"

#include "math.h"

#define TAG "QAMDecoder"

#define UPDATETIME_MS 100
uint16_t rxbuffer[CONFIG_ADC_STREAMING_BUFFERSIZE];

void adcCallbackFunction() {
    adc_get_buffer(AN1, rxbuffer);
    uint32_t x = 0;
    for(int i = 0; i < CONFIG_ADC_STREAMING_BUFFERSIZE; i++) {
        x+= rxbuffer[i];
    }
    ESP_LOGI(TAG, "Rx ADC: value:%i", (int)(x));
}
void adc_receiver_Task(void* param) {
    //Init stuff here
    adc_set_stream_callback(&adcCallbackFunction);
    vTaskDelay(100);
    for(;;) {
        vTaskDelay(UPDATETIME_MS/portTICK_PERIOD_MS);
    }
}

void app_main()
{
    //Initialize Eduboard2 BSP
    eduboard2_init();
    xTaskCreate(adc_receiver_Task,"adcreceiverTask",4*2048,NULL,10,NULL);
    for(;;) {
        vTaskDelay(2000/portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "QAM Decoder");
    }
}