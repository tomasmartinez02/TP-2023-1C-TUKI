#include <utils/instrucciones.h>

// Funciones privadas

// Retorna el string correspondiente al identificador del registro
static char *t_registro_to_char(t_registro registro)
{
    switch (registro)
    {
        case REGISTRO_ax:
            return "AX";
            break;
        case REGISTRO_bx:
            return "BX";
            break;
        case REGISTRO_cx:
            return "CX";
            break;
        case REGISTRO_dx:
            return "DX";
            break;
        case REGISTRO_eax:
            return "EAX";
            break;
        case REGISTRO_ebx:
            return "EBX";
            break;
        case REGISTRO_ecx:
            return "ECX";
            break;
        case REGISTRO_edx:
            return "EDX";
            break;
        case REGISTRO_rax:
            return "RAX";
            break;
        case REGISTRO_rbx:
            return "RBX";
            break;
        case REGISTRO_rcx:
            return "RCX";
            break;
        case REGISTRO_rdx:
            return "RDX";
            break;
        default:
            return "NULL";
            break;
    }
}

t_registros_cpu *registros_cpu_create(void)
{
    t_registros_cpu *tempRegistrosCpu = malloc(sizeof(*tempRegistrosCpu));
    memset((void *) tempRegistrosCpu, '\0', sizeof(*tempRegistrosCpu));

    // Registros 4 bytes
    tempRegistrosCpu->registroAx = malloc(sizeof(uint32_t));
    tempRegistrosCpu->registroBx = malloc(sizeof(uint32_t));
    tempRegistrosCpu->registroCx = malloc(sizeof(uint32_t));
    tempRegistrosCpu->registroDx = malloc(sizeof(uint32_t));
    // Registros 8 bytes
    tempRegistrosCpu->registroEax = malloc(sizeof(uint64_t));
    tempRegistrosCpu->registroEbx = malloc(sizeof(uint64_t));
    tempRegistrosCpu->registroEcx = malloc(sizeof(uint64_t));
    tempRegistrosCpu->registroEdx = malloc(sizeof(uint64_t));
    // Registros 16 bytes
    tempRegistrosCpu->registroRax = malloc(2 * sizeof(uint64_t));
    tempRegistrosCpu->registroRbx = malloc(2 * sizeof(uint64_t));
    tempRegistrosCpu->registroRcx = malloc(2 * sizeof(uint64_t));
    tempRegistrosCpu->registroRdx = malloc(2 * sizeof(uint64_t));

    return tempRegistrosCpu;
}

void registros_cpu_destroy(t_registros_cpu *registrosCpu)
{
    // Registros 4 bytes
    free(registrosCpu->registroAx); 
    free(registrosCpu->registroBx); 
    free(registrosCpu->registroDx); 
    free(registrosCpu->registroCx); 
    // Registros 8 bytes
    free(registrosCpu->registroEax);
    free(registrosCpu->registroEbx);
    free(registrosCpu->registroEcx);
    free(registrosCpu->registroEdx);
    // Registros 16 bytes
    free(registrosCpu->registroRax);
    free(registrosCpu->registroRbx);
    free(registrosCpu->registroRcx);
    free(registrosCpu->registroRdx);

    free(registrosCpu);

    return;
}


char *instruccion_to_string(t_instruccion *self) 
{
    uint32_t operando1 = instruccion_get_operando1(self);
    uint32_t operando2 = instruccion_get_operando2(self);
    uint32_t operando3 = instruccion_get_operando3(self);
    t_registro registro1 = instruccion_get_registro1(self);
    t_registro registro2 = instruccion_get_registro2(self);
    char *valorSet = instruccion_get_valor_set(self);
    char *dispositivoIo = instruccion_get_dispositivo_io(self);
    char *nombreArchivo = instruccion_get_nombre_archivo(self);
    
    char *instruccionToString;
                        // Registros/memoria
    instruccionToString = self->tipoInstruccion == INSTRUCCION_set   ? string_from_format("SET %s %s", t_registro_to_char(registro1), valorSet)
                        : self->tipoInstruccion == INSTRUCCION_movin    ? string_from_format("MOV_IN %s %d", t_registro_to_char(registro1), operando2)
                        : self->tipoInstruccion == INSTRUCCION_movout  ? string_from_format("MOV_OUT %d %s", operando1, t_registro_to_char(registro2))
                        // Dispositivos io
                        : self->tipoInstruccion == INSTRUCCION_io ? string_from_format("I/O %s %d", dispositivoIo, operando2)
                        // Archivos
                        : self->tipoInstruccion == INSTRUCCION_fopen ? string_from_format("F_OPEN %s", nombreArchivo)
                        : self->tipoInstruccion == INSTRUCCION_fclose ? string_from_format("F_CLOSE %s", nombreArchivo)
                        : self->tipoInstruccion == INSTRUCCION_fseek ? string_from_format("F_SEEK %s %d", nombreArchivo, operando2)
                        : self->tipoInstruccion == INSTRUCCION_fread ? string_from_format("F_READ %s %d %d", nombreArchivo, operando2, operando3)
                        : self->tipoInstruccion == INSTRUCCION_fwrite ? string_from_format("F_WRITE %s %d %d", nombreArchivo, operando2, operando3)
                        : self->tipoInstruccion == INSTRUCCION_ftruncate ? string_from_format("F_TRUNCATE %s %d", nombreArchivo, operando2)
                        // Semaforos
                        : self->tipoInstruccion == INSTRUCCION_wait ? string_from_format("WAIT %s", dispositivoIo)
                        : self->tipoInstruccion == INSTRUCCION_signal ? string_from_format("SIGNAL %s", dispositivoIo)
                        // Memoria
                        : self->tipoInstruccion == INSTRUCCION_create_segment ? string_from_format("CREATE_SEGMENT %d %d", operando1, operando2)
                        : self->tipoInstruccion == INSTRUCCION_delete_segment ? string_from_format("DELETE_SEGMENT %d", operando1)
                        // Desalojo voluntario y exit
                        : self->tipoInstruccion == INSTRUCCION_yield ? string_from_format("YIELD")
                        : self->tipoInstruccion == INSTRUCCION_exit ? string_from_format("EXIT")
                        : string_from_format("UNKNOWN");

    if (valorSet != NULL) free(valorSet);
    if (dispositivoIo != NULL) free(dispositivoIo);
    if (nombreArchivo != NULL) free(nombreArchivo);
    
    return instruccionToString;
}

t_instruccion *instruccion_create(t_tipo_instruccion tipoInstruccion, t_info_instruccion *infoInstruccion) 
{
    t_instruccion *self = malloc(sizeof(*self));
    
    self->tipoInstruccion = tipoInstruccion;
    self->operando1 = info_instruccion_get_operando1(infoInstruccion);
    self->operando2 = info_instruccion_get_operando2(infoInstruccion);
    self->operando3 = info_instruccion_get_operando3(infoInstruccion);
    self->registro1 = info_instruccion_get_registro1(infoInstruccion);
    self->registro2 = info_instruccion_get_registro2(infoInstruccion);
    self->valorSet = info_instruccion_get_valor_set(infoInstruccion);
    self->dispositivoIo = info_instruccion_get_dispositivo_io(infoInstruccion);
    self->nombreArchivo = info_instruccion_get_nombre_archivo(infoInstruccion);
    self->toString = instruccion_to_string(self);
    
    return self;
}

void instruccion_destroy(void* selfVoid) 
{
    t_instruccion* self = (t_instruccion*)selfVoid;
    
    if (self->valorSet != NULL) free(self->valorSet);
    if (self->dispositivoIo != NULL) free(self->dispositivoIo);
    if (self->nombreArchivo != NULL) free(self->nombreArchivo);

    free(self->toString);
    free(self);

    return;
}

t_info_instruccion *info_instruccion_create(void) 
{
    t_info_instruccion *self = malloc(sizeof(*self));

    self->operando1 = 0;
    self->operando2 = 0;
    self->operando3 = 0;
    self->registro1 = REGISTRO_null;
    self->registro2 = REGISTRO_null;
    self->valorSet = NULL;
    self->dispositivoIo = NULL;
    self->nombreArchivo = NULL;
    
    return self;
}

void info_instruccion_destroy(t_info_instruccion *self) 
{
    if (self->valorSet != NULL) free(self->valorSet);
    if (self->dispositivoIo != NULL) free(self->dispositivoIo);
    if (self->nombreArchivo != NULL) free(self->nombreArchivo);
    free(self);

    return;
}

t_list* instruccion_list_create_from_buffer(t_buffer *bufferConInstrucciones, t_log *logger) 
{
    t_list *instrucciones = list_create();
    
    bool isExit = false;
    
    while (!isExit) {
        
        t_tipo_instruccion identificadorInstruccion;
        buffer_unpack(bufferConInstrucciones, &identificadorInstruccion, sizeof(identificadorInstruccion));
        
        t_info_instruccion *infoInstruccion = info_instruccion_create();

        switch (identificadorInstruccion) {
            
            case INSTRUCCION_set:
                t_registro registro1;
                char *valorSet;
                
                buffer_unpack(bufferConInstrucciones, (void*) &registro1, sizeof(registro1));
                valorSet = buffer_unpack_string(bufferConInstrucciones);

                info_instruccion_set_valor_set(infoInstruccion, valorSet);
                info_instruccion_set_registro1(infoInstruccion, registro1);

                free(valorSet);
                break;
            
            case INSTRUCCION_movin:
                buffer_unpack(bufferConInstrucciones, (void*) &infoInstruccion->registro1, sizeof(infoInstruccion->registro1));
                buffer_unpack(bufferConInstrucciones, (void*) &infoInstruccion->operando2, sizeof(infoInstruccion->operando2));
                break;

            case INSTRUCCION_movout:
                buffer_unpack(bufferConInstrucciones, (void*) &infoInstruccion->operando1, sizeof(infoInstruccion->operando1));
                buffer_unpack(bufferConInstrucciones, (void*) &infoInstruccion->registro2, sizeof(infoInstruccion->registro2));
                break;

            case INSTRUCCION_io:
                infoInstruccion->dispositivoIo = buffer_unpack_string(bufferConInstrucciones);

                if(!strcmp(infoInstruccion->dispositivoIo, "PANTALLA") || !strcmp(infoInstruccion->dispositivoIo, "TECLADO")) {

                    buffer_unpack(bufferConInstrucciones, (void*) &infoInstruccion->registro2, sizeof(infoInstruccion->registro2));
                }
                else {

                    buffer_unpack(bufferConInstrucciones, (void*) &infoInstruccion->operando2, sizeof(infoInstruccion->operando2));
                }
                break;
                
            case INSTRUCCION_exit:
                isExit = true;
                break;
            
            default:
                log_error(logger, "Error al intentar desempaquetar una instrucción");
                exit(EXIT_FAILURE);
        }
        
        t_instruccion* instruccionActual = instruccion_create(identificadorInstruccion, infoInstruccion);
        list_add(instrucciones, (void*) instruccionActual);
        log_info(logger, "Se desempaqueta la instruccion %s", instruccion_get_to_string(instruccionActual));
        
        info_instruccion_destroy(infoInstruccion);
    }
    
    log_info(logger, "Se desempaquetan %d instrucciones", list_size(instrucciones));
    
    return instrucciones;
}

void destroy_instructions_list(t_list* instructionsList)
{
    list_destroy_and_destroy_elements(instructionsList, instruccion_destroy);
}

t_tipo_instruccion instruccion_get_tipo_instruccion(t_instruccion *self) 
{
    return self->tipoInstruccion;
}

uint32_t instruccion_get_operando1(t_instruccion *self) 
{
    return self->operando1;
}

uint32_t instruccion_get_operando2(t_instruccion *self) 
{
    return self->operando2;
}

uint32_t instruccion_get_operando3(t_instruccion *self)
{
    return self->operando3;
}

t_registro instruccion_get_registro1(t_instruccion *self) 
{
    return self->registro1;
}

t_registro instruccion_get_registro2(t_instruccion *self) 
{
    return self->registro2;
}

char *instruccion_get_valor_set(t_instruccion *self)
{
    char *valorSet = self->valorSet;
    return valorSet != NULL ? string_duplicate(valorSet) : valorSet;
}

char *instruccion_get_dispositivo_io(t_instruccion *self) 
{
    char *dispositivoIo = self->dispositivoIo;
    return dispositivoIo != NULL ? string_duplicate(dispositivoIo) : dispositivoIo;
}

char *instruccion_get_nombre_archivo(t_instruccion *self)
{
    char *nombreArchivo = self->nombreArchivo;
    return nombreArchivo != NULL ? string_duplicate(nombreArchivo) : nombreArchivo;
}

char const* instruccion_get_to_string(t_instruccion *self) 
{
    return self->toString;
}

uint32_t info_instruccion_get_operando1(t_info_instruccion *self) 
{
    return self->operando1;
}

uint32_t info_instruccion_get_operando2(t_info_instruccion *self) 
{
    return self->operando2;
}

uint32_t info_instruccion_get_operando3(t_info_instruccion *self)
{
    return self->operando3;
}

t_registro info_instruccion_get_registro1(t_info_instruccion *self) 
{
    return self->registro1;
}

t_registro info_instruccion_get_registro2(t_info_instruccion *self) 
{
    return self->registro2;
}

char *info_instruccion_get_valor_set(t_info_instruccion *self)
{
    char *valorSet = self->valorSet;
    return valorSet != NULL ? string_duplicate(valorSet) : valorSet;
}

char *info_instruccion_get_dispositivo_io(t_info_instruccion *self) 
{
    char *dispositivoIo = self->dispositivoIo;
    return dispositivoIo != NULL ? string_duplicate(dispositivoIo) : dispositivoIo;
}

char *info_instruccion_get_nombre_archivo(t_info_instruccion *self)
{
    char *nombreArchivo = self->nombreArchivo;
    return nombreArchivo != NULL ? string_duplicate(nombreArchivo) : nombreArchivo;
}

void info_instruccion_set_operando1(t_info_instruccion* self, uint32_t operando1) 
{
    self->operando1 = operando1;
}

void info_instruccion_set_operando2(t_info_instruccion* self, uint32_t operando2) 
{
    self->operando1 = operando2;
}

void info_instruccion_set_operando3(t_info_instruccion *self, uint32_t operando3)
{
    self->operando1 = operando3;
}

void info_instruccion_set_registro1(t_info_instruccion* self, t_registro registro1) 
{
   self->registro1 = registro1;
}

void info_instruccion_set_registro2(t_info_instruccion* self, t_registro registro2) 
{
    self->registro2 = registro2;
}

void info_instruccion_set_valor_set(t_info_instruccion *self, char *valorSet)
{
    self->valorSet = valorSet != NULL ? string_duplicate(valorSet) : valorSet;
    return;
}

void info_instruccion_set_dispositivo_io(t_info_instruccion *self, char *dispositivoIo) 
{
    self->dispositivoIo = dispositivoIo != NULL ? string_duplicate(dispositivoIo) : dispositivoIo;
}

void info_instruccion_set_nombre_archivo(t_info_instruccion *self, char *nombreArchivo)
{
    self->nombreArchivo = nombreArchivo != NULL ? string_duplicate(nombreArchivo) : nombreArchivo;
}