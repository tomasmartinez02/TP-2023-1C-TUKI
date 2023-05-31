#ifndef CONSOLA_PARSER_H_
#define CONSOLA_PARSER_H_

//Standard libraries
#include <errno.h>
#include <stdlib.h>
#include <string.h>
//Commons libaries
#include <commons/log.h>
#include <commons/string.h>
//Static libraries
#include <serializacion/buffer.h>
#include <conexiones/conexiones.h>
#include <utils/instrucciones.h>
#include <utils/common-utils.h>
//Internal libraries consola module
#include <consola-serializacion.h>
#include <consola-estructuras.h>

#define MAX_LENGTH_INSTRUCTION 50

/**
 * @brief Parsea el archivo de instrucciones y serializa la informacion en un buffer
 * 
 * @param buffer: Buffer a serializar con instrucciones 
 * @param pathInstrucciones: Path al archivo de instrucciones 
 * @return true: Parse success 
 * @return false: Parse error
 */
bool consola_parser_parse_instrucciones(t_buffer *buffer, const char *pathInstrucciones);

#endif