#include <stdio.h>
#include <unistd.h>
#include <modbus_api.h>



#define API_NAME        "Modbus Simple"

void app_main(void)
{

    printf("%s\n",API_NAME);

    esp_err_t err = modbus_api_init();
    while(1){
        sleep(1);
    }

}