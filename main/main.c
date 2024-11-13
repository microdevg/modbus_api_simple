#include <stdio.h>
#include <modbus_api.h>
#include "freertos/FreeRTOS.h"
#include <freertos/task.h>

#define ADDRESS_SLAVE       1
#define BAUDRATE            115200
mb_communication_info_t modbus_cfg = {
                            .baudrate = BAUDRATE,
                            .mode = MB_MODE_RTU,                    //  MB_MODE_ASCII DISPONIBLE TAMBIEN 
                            .port = UART_NUM_1,                     // UART_NUM 0 ,1 , 2
                            .parity = UART_PARITY_DISABLE,          // UART_PARITY_DISABLE , UART_PARITY_EVEN , UART_PARITY_ODD    
                            .slave_addr = ADDRESS_SLAVE,
};



/**
 * @brief Funcion dummy, simula ser la funcion que recibe el buffer 
 *        respuesta modbus y lo gestiona (imprime).
 * 
 * @param buffer 
 * @param len 
 */
void iot_publish_dummy(void* buffer,size_t len){
    uint16_t* b = (uint16_t*) buffer;
    for (int counter = 0; counter < len; counter++)  printf("\t\treg[%d] = %d\n", counter, b[counter]);
}

/***
 * Funcion callback, recibe la respuesta modbus y la publica.
 */
void server_iot(    mb_param_request_t* req, void* buff,size_t len, bool ok){
    if(ok)  iot_publish_dummy(buff,req->reg_size);
}






 void iot_task(void* params){
   mb_param_request_t req = {   .command = 0x03,
                                .reg_size = 20,
                                .reg_start = 0,
                            };
    esp_err_t err = ESP_FAIL;
    while(1){
        err = modbus_api_send_request(&req);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
 }




void app_main(void)
{
    esp_err_t err = modbus_api_config(&modbus_cfg,server_iot);
    TaskHandle_t iot_task = NULL;
    if(err != ESP_OK){
        printf("Error al inicial modbus api\n");
    }
    else{
        BaseType_t ret =  xTaskCreatePinnedToCore(iot_task,"iot task",4000,NULL, tskIDLE_PRIORITY + 2 ,iot_task,1); 
        if(ret == pdFAIL)printf("Error al crear tarea\n");
    }
}