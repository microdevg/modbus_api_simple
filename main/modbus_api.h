#ifndef MODBUS_API_H
#define MODBUS_API_H


#include <esp_err.h>
#include "modbus_params.h"  // for modbus parameters structures
#include "mbcontroller.h"

esp_err_t modbus_api_init();

esp_err_t modbus_api_start();

esp_err_t modbus_api_send_request();

esp_err_t modbus_api_stop();






#endif