#ifndef CONSOLA_SERIALIZACION_H_
#define CONSOLA_SERIALIZACION_H_

// Bibliotecas static utils
#include <serializacion/buffer.h>
#include <utils/instrucciones.h>
// Bibliotecas internas
#include <consola-utils.h>
#include <consola-estructuras.h>

void consola_serializer_pack_no_args(t_buffer *buffer, t_tipo_instruccion tipoInstruccion);
void consola_serializer_pack_one_integer_args(t_buffer *buffer, t_tipo_instruccion tipoInstruccion, uint32_t op1);
void consola_serializer_pack_one_register_args(t_buffer *buffer, t_tipo_instruccion tipoInstruccion, t_registro registro);
void consola_serializer_pack_one_string_args(t_buffer *buffer, t_tipo_instruccion tipoInstruccion, char *dispositivoIoONombreArchivo);
void consola_serializer_pack_two_args(t_buffer *buffer, t_tipo_instruccion tipoInstruccion, void *arg1, void *arg2);
void consola_serializer_pack_three_args(t_buffer *buffer, t_tipo_instruccion tipoInstruccion, void *arg1, void *arg2, void *arg3);

#endif