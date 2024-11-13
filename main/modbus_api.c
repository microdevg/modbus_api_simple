

#include "modbus_api.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>


#define QUEUE_MAX_REQS       5
#define MODBUS_STACK        (4000)

QueueHandle_t _QUEUE_ELEMENTS = 0;
TickType_t    _QUEUE_TIMEOUT = 1000;

TaskHandle_t    modbus_task_handler = 0;
static bool     _modbus_ready = false;


#define UART_MODBUS_TX  22
#define UART_MODBUS_RX  23
#define UART_MODBUS_RTS 18
#define UART_MODBUS_CTS UART_PIN_NO_CHANGE



// Comentar este define si quiere usar la INTERFAZ RS485
#define UART_RS232      



static uint8_t* _buffer;

static mb_communication_info_t* _modbus_config = NULL;







static void modbus_task(void* params){
    modbus_request_callback callback =(modbus_request_callback) params;
    mb_param_request_t request = {0};
    esp_err_t err = ESP_OK;
    while(1){
    if( xQueueReceive( _QUEUE_ELEMENTS, &request, _QUEUE_TIMEOUT ) == pdPASS) {

        size_t len = request.reg_size * 2;
        _buffer = (uint8_t*)malloc(len);
        
        if(_buffer)
        {
        err =  mbc_master_send_request(&request,_buffer);   

       callback(    &(request),
                    _buffer,
                    len,
                    (err ==  ESP_OK)?true:false);
        // liberamos recursos
        err = ESP_FAIL;
        free(_buffer);
        }
        else printf("Error de memoria\n");
    }
       vTaskDelay(500/portTICK_PERIOD_MS);

    }
}



// Modbus master initialization
static esp_err_t _modbus_master_init(mb_communication_info_t* comm)
{
    void* master_handler = NULL;
    esp_err_t err = mbc_master_init(MB_PORT_SERIAL_MASTER, &master_handler);
    MB_RETURN_ON_FALSE((master_handler != NULL), ESP_ERR_INVALID_STATE, "Modbus_tag",
                                "mb controller initialization fail.");
    MB_RETURN_ON_FALSE((err == ESP_OK), ESP_ERR_INVALID_STATE, "Modbus_tag",
                            "mb controller initialization fail, returns(0x%x).", (int)err);
    err = mbc_master_setup((void*)comm);
    MB_RETURN_ON_FALSE((err == ESP_OK), ESP_ERR_INVALID_STATE, "Modbus_tag",
                            "mb controller setup fail, returns(0x%x).", (int)err);
    err = uart_set_pin(comm->port, UART_MODBUS_TX,UART_MODBUS_RX,UART_MODBUS_RTS, UART_MODBUS_CTS);
    MB_RETURN_ON_FALSE((err == ESP_OK), ESP_ERR_INVALID_STATE, "Modbus_tag",
        "mb serial set pin failure, uart_set_pin() returned (0x%x).", (int)err);
    err = mbc_master_start();
    MB_RETURN_ON_FALSE((err == ESP_OK), ESP_ERR_INVALID_STATE, "Modbus_tag",
                            "mb controller start fail, returned (0x%x).", (int)err);       
    #ifdef UART_RS232
        err = uart_set_mode(comm->port,     UART_MODE_UART );
    #else
        err = uart_set_mode(comm->port,     UART_MODE_RS485_HALF_DUPLEX );
    #endif
    MB_RETURN_ON_FALSE((err == ESP_OK), ESP_ERR_INVALID_STATE, "Modbus_tag",
            "mb serial set mode failure, uart_set_mode() returned (0x%x).", (int)err);
    vTaskDelay(5); 
    return err;
}




esp_err_t modbus_api_config(mb_communication_info_t* cfg,modbus_request_callback callback){
    esp_err_t err = ESP_OK;
    if(cfg == NULL) return ESP_FAIL;
    _modbus_config = cfg;
    err = _modbus_master_init(_modbus_config);
   _QUEUE_ELEMENTS = xQueueCreate( QUEUE_MAX_REQS, sizeof(mb_param_request_t ) );
    if(_QUEUE_ELEMENTS != NULL && err == ESP_OK)
    {
        _modbus_ready = true;
         xTaskCreatePinnedToCore(modbus_task,"Modbus task",MODBUS_STACK,callback, MODBUS_TASK_PRIORITY ,modbus_task_handler,1); 
    }  
    return err;
}



esp_err_t modbus_api_start(){
    return ESP_OK;
}
esp_err_t modbus_api_stop(){
    return ESP_OK;
}

esp_err_t modbus_api_send_request(mb_param_request_t* req){
    if(_modbus_ready){
        mb_param_request_t request  = 
        {
            .command = req->command,
            .reg_size = req->reg_size,
            .reg_start = req->reg_start,
            .slave_addr = _modbus_config->slave_addr
        };
        if( xQueueSend( _QUEUE_ELEMENTS, ( void * ) &request, _QUEUE_TIMEOUT ) == pdTRUE ) return ESP_OK;
         
    }
    return ESP_FAIL;
}


esp_err_t modbus_api_deinit(){
    _modbus_ready = false;
    return 0;
}