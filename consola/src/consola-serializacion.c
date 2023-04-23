#include <consola-serializacion.h>

void consola_serializer_pack_no_args(t_buffer *buffer, t_tipo_instruccion tipoInstruccion) 
{
    buffer_pack(buffer, &tipoInstruccion, sizeof(tipoInstruccion));
    return;
}

void consola_serializer_pack_one_integer_args(t_buffer *buffer, t_tipo_instruccion tipoInstruccion, uint32_t op1) 
{
    consola_serializer_pack_no_args(buffer, tipoInstruccion);
    buffer_pack(buffer, &op1, sizeof(op1));
    return;
}

void consola_serializer_pack_one_register_args(t_buffer *buffer, t_tipo_instruccion tipoInstruccion, t_registro registro) 
{
    consola_serializer_pack_no_args(buffer, tipoInstruccion);
    buffer_pack(buffer, &registro, sizeof(registro));
    return;
}

void consola_serializer_pack_one_string_args(t_buffer *buffer, t_tipo_instruccion tipoInstruccion, char *dispositivoIoONombreArchivo) 
{
    consola_serializer_pack_no_args(buffer, tipoInstruccion);
    buffer_pack_string(buffer, dispositivoIoONombreArchivo);
    return;
}

void consola_serializer_pack_two_args(t_buffer *buffer, t_tipo_instruccion tipoInstruccion, void *arg1, void *arg2) 
{
    switch (tipoInstruccion)
    {
        case INSTRUCCION_set:
            consola_serializer_pack_one_register_args(buffer, tipoInstruccion, *((t_registro *) arg1));
            buffer_pack_string(buffer, (char *) arg2);
            break;
        
        case INSTRUCCION_movin:
            consola_serializer_pack_one_register_args(buffer, tipoInstruccion, *((t_registro *) arg1));
            buffer_pack(buffer, arg2, sizeof(uint32_t));
            break;

        case INSTRUCCION_movout:
            consola_serializer_pack_one_integer_args(buffer, tipoInstruccion, *((uint32_t *) arg1));
            buffer_pack(buffer, arg2, sizeof(t_registro));
            break;

        case INSTRUCCION_fseek:
        case INSTRUCCION_ftruncate:
            consola_serializer_pack_one_string_args(buffer, tipoInstruccion, (char *) arg1);
            buffer_pack(buffer, arg2, sizeof(uint32_t));
            break;
        
        case INSTRUCCION_create_segment:
            consola_serializer_pack_one_integer_args(buffer, tipoInstruccion, *((uint32_t *) arg1));
            buffer_pack(buffer, arg2, sizeof(uint32_t));
            break;
        
        default:
            log_error(consolaLogger, "No se pudo serializar la instruccion correctamente");
            log_error(consolaDebuggingLogger, "No se pudo serializar la instruccion correctamente");
            exit(EXIT_FAILURE);
            break;
    }
    return;
}

void consola_serializer_pack_three_args(t_buffer *buffer, t_tipo_instruccion tipoInstruccion, void *arg1, void *arg2, void *arg3)
{
    consola_serializer_pack_one_string_args(buffer, tipoInstruccion, (char *) arg1);
    buffer_pack(buffer, arg2, sizeof(uint32_t));
    buffer_pack(buffer, arg3, sizeof(uint32_t));
    return;
}