// Bibliotecas usada para enviar buffers y mensajes entre modulos
#ifndef STREAM_H
#define STREAM_H

// Bibliotecas estandar
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
// Bibliotecas internas
#include <serializacion/buffer.h>

// Tipos
// Headers de mensajes para ser enviados en los paquetes
enum header
{   
    HEADER_archivo_creado,
    HEADER_archivo_existe_en_filesystem,
    HEADER_archivo_no_existe_en_filesystem,
    HEADER_consulta_existencia_archivo,
    HEADER_compactar,
    HEADER_crear_segmento,
    HEADER_eliminar_segmento,
    HEADER_error,
    HEADER_escritura_finalizada,
    HEADER_instruccion_movin,
    HEADER_instruccion_movout,
    HEADER_instruccion_io,
    HEADER_instruccion_fopen,
    HEADER_instruccion_fclose,
    HEADER_instruccion_fseek,
    HEADER_instruccion_fread,
    HEADER_instruccion_fwrite,
    HEADER_instruccion_ftruncate,
    HEADER_instruccion_wait,
    HEADER_instruccion_signal,
    HEADER_instruccion_create_segment,
    HEADER_instruccion_delete_segment,
    HEADER_instruccion_yield,
    HEADER_instruccion_exit,
    HEADER_tiempo_io,
    HEADER_dispositivo_io,
    HEADER_lectura_finalizada,
    HEADER_lista_instrucciones,
    HEADER_memoria_compactada,
    HEADER_memoria_insuficiente,
    HEADER_necesita_compactacion,
    HEADER_out_of_memory,
    HEADER_pcb_a_ejecutar,
    HEADER_pid,
    HEADER_proceso_bloqueado,
    HEADER_proceso_desalojado,
    HEADER_proceso_terminado,
    HEADER_segmento_creado,
    HEADER_segmento_destruido,
    HEADER_segmentation_fault,
    HEADER_solicitud_creacion_archivo,
    HEADER_solicitud_escribir_archivo,
    HEADER_solicitud_finalizacion_proceso,
    HEADER_solicitud_inicializacion_proceso,
    HEADER_solicitud_leer_archivo,
    HEADER_solicitud_modificar_tamanio_archivo,
    HEADER_tabla_segmentos,
    HEADER_tamanio_archivo_modificado,
   
    HEADER_solicitud_memoria_lectura,
    HEADER_memoria_confirmacion_lectura,
    HEADER_solicitud_memoria_escritura,
    HEADER_memoria_confirmacion_escritura
};
typedef enum header t_header;

// Handshakes de los distintos servidores y clientes
enum handshake 
{
    HANDSHAKE_consola,
    HANDSHAKE_kernel,
    HANDSHAKE_memoria,
    HANDSHAKE_cpu,
    HANDSHAKE_interrupt,
    HANDSHAKE_dispatch,
    HANDSHAKE_filesystem,
    HANDSHAKE_ok_continue
};
typedef enum handshake t_handshake;

// Prototipos
/**
 * @brief Envía un buffer previamente serializado, asignándole un header en su envío
 *
 * @example stream_send_buffer(socketModuloDestinatario, header, bufferSerializado);
 */
void stream_send_buffer(int toSocket, uint8_t header, t_buffer *buffer);

/**
 * @brief Envía solamente el header del buffer (sin payload)
 *
 * @example stream_send_empty_buffer(socketModuloDestinatario, header);
 */
void stream_send_empty_buffer(int toSocket, uint8_t header);

/**
 * @brief Recibe solamente el payload (stream de bytes) del buffer enviado
 *
 * @example stream_recv_buffer(socketModuloRemitente, destBuffer);
 */
void stream_recv_buffer(int fromSocket, t_buffer *destBuffer);

/**
 * @brief En combinación con stream_recv_header/1, recibe un buffer que se
 * sabe que vendrá con un payload vacío. En otras palabras, útil para casos en
 * que se quiera recibir solamente un header (sin payload)
 *
 * @example stream_recv_header(socketModuloRemitente);
 *          stream_recv_empty_buffer(socketModuloRemitente);
 */
void stream_recv_empty_buffer(int fromSocket);

/**
 * @brief Recibe solamente el header del buffer enviado
 *
 * @example stream_recv_header(socketModuloRemitente);
 */
uint8_t stream_recv_header(int fromSocket);

#endif
