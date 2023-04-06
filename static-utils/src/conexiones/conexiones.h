// Biblioteca para crear un servidor y conectarse a un servidor como cliente
#ifndef CONEXION_H_
#define CONEXION_H_

// Bibliotecas estandar
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

// Tipos
// Headers de mensajes para ser enviados en los paquetes
enum header {
    
    HEADER_error,
    //HEADER_pid,
    //HEADER_lista_instrucciones,
    //HEADER_interrumpir_ejecucion,
    //HEADER_marco,
    //HEADER_memoria_insuficiente,
    //HEADER_pcb_a_ejecutar,
    //HEADER_proceso_bloqueado,
    //HEADER_proceso_desalojado,
    //HEADER_proceso_terminado,
    //HEADER_movin,
    //HEADER_movout,
    //HEADER_solicitud_tabla_paginas_segmentos,
    //HEADER_page_fault,
    //HEADER_page_fault_resuelto,
    //HEADER_instruccion_io_teclado,
    //HEADER_instruccion_io_pantalla,
    //HEADER_segmentation_fault
};
typedef enum header t_header;

// Handshakes de los distintos servidores y clientes
enum handshake {

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
 * @brief Inicia un servidor en la ip y puerto correspondientes
 * 
 * @param ip: ip del servidor que estamos creando
 * @param puerto: puerto del servidor que estamos creando
 * @return int: ID del file descriptor del socket del servidor
 * 
 * @example int socketServidor = iniciar_servidor("192.168.0.1", 2340);
 */
int iniciar_servidor(char *ip, char *puerto);

/**
 * @brief Inicia un cliente conectandose a un servidor en la ip y puerto correspondientes
 * 
 * @param ip: ip del servidor al que nos conectamos
 * @param puerto: puerto del servidor al cual nos conectamos
 * @return int: ID del file descriptor del socket creado con el servidor
 * 
 * @example int socketCliente = conectar_a_servidor("192.168.0.1", 2340);
 */
int conectar_a_servidor(char *ip, char *puerto);

#endif