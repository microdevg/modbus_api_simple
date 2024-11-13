# Modbus API Simple

Este proyecto proporciona una interfaz sencilla para trabajar con Modbus, basada en la API `esp-modbus`.

## ¿Cómo funciona?

Para utilizar la interfaz Modbus con _Modbus API Simple_, sigue los pasos a continuación:

### Paso 1: Configurar la API

Llama a la API de configuración:

```c
esp_err_t err = modbus_api_config(&modbus_cfg, server_iot);
```

#### Donde los parámetros utilizados son:

 - __modbus_cfg__: una estructura de configuración para Modbus.
- __server_iot__: una función de callback que se llama cuando se realiza una petición Modbus y se obtiene una respuesta.

### Paso 2: Realizar peticiones Modbus
Para realizar una petición Modbus, puedes usar el siguiente ejemplo:
```c
mb_param_request_t req = { 
    .command = 0x03,
    .reg_size = 20,
    .reg_start = 0,
};
esp_err_t err = ESP_FAIL;
err = modbus_api_send_request(&req);
```

### Recomendación
Se recomienda usar una tarea productora de solicitudes que tenga una prioridad mayor que la prioridad de la tarea Modbus para optimizar el rendimiento.