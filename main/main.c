#include <stdio.h>
#include <unistd.h>


#define API_NAME        "Modbus Simple"

void app_main(void)
{

    printf("%s\n",API_NAME);

    while(1){
        sleep(1);
    }

}