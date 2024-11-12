# Añadir archivos de mb_example_common a los archivos fuente
idf_component_register(
    SRCS "modbus_api.c" "main.c"
         "${IDF_PATH}/examples/protocols/modbus/mb_example_common/modbus_params.c"

    INCLUDE_DIRS "." 
                 "${IDF_PATH}/examples/protocols/modbus/mb_example_common/include"
)
