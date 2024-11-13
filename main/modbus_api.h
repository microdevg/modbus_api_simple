/**
 * @file modbus_api.h
 * @author German Velardez (germanvelardez16@gmail.com)
 * @brief  Api para gestiones peticiones Modbus en una tarea externa mediante Queue.
 * @version 0.1
 * @date 2024-11-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef MODBUS_API_H
#define MODBUS_API_H


#include <esp_err.h>
#include "modbus_params.h"  // for modbus parameters structures
#include "mbcontroller.h"

#define MODBUS_TASK_PRIORITY    (tskIDLE_PRIORITY + 1)



typedef void (*modbus_request_callback)(    mb_param_request_t* req,
                                            void* buffer,
                                            size_t buffer_len,
                                            bool succeeded);




/**
 * @brief Configuracion de interfaz modbus
 * 
 * @param cfg  estructura de configuracion
 * @param callback  funcion de callback, se llama cuando una peticion modbus a sido realizada
 * @return esp_err_t 
 */
esp_err_t modbus_api_config(mb_communication_info_t* cfg,modbus_request_callback callback);


/**
 * @brief Funcion para enviar una peticion modbus. Se envia una estructura request.
 * 
 * @param req estructura request.
 * @return esp_err_t 
 */
esp_err_t modbus_api_send_request(mb_param_request_t* req);


/**
 * @brief Inicia el canal de peticiones. Aun sin implementar
 * 
 * @return esp_err_t 
 */
esp_err_t modbus_api_start();


/**
 * @brief Detiene el canal de peticiones. Aun sin implementar.
 * 
 * @return esp_err_t 
 */
esp_err_t modbus_api_stop();






#endif