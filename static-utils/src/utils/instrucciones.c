#include <utils/instrucciones.h>

// Funciones privadas

// Retorna el string correspondiente al identificador del registro
static char *__t_registro_to_char(t_registro registro)
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

// Transforma una instruccion en un string que se puede imprimir
static char *__instruccion_to_string(t_instruccion *self) 
{
    t_tipo_instruccion tipoInstruccion = instruccion_get_tipo_instruccion(self);
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
    instruccionToString = tipoInstruccion == INSTRUCCION_set   ? string_from_format("SET %s %s", __t_registro_to_char(registro1), valorSet)
                        : tipoInstruccion == INSTRUCCION_movin    ? string_from_format("MOV_IN %s %d", __t_registro_to_char(registro1), operando2)
                        : tipoInstruccion == INSTRUCCION_movout  ? string_from_format("MOV_OUT %d %s", operando1, __t_registro_to_char(registro2))
                        // Dispositivos io
                        : tipoInstruccion == INSTRUCCION_io ? string_from_format("I/O %d", operando1)
                        // Archivos
                        : tipoInstruccion == INSTRUCCION_fopen ? string_from_format("F_OPEN %s", nombreArchivo)
                        : tipoInstruccion == INSTRUCCION_fclose ? string_from_format("F_CLOSE %s", nombreArchivo)
                        : tipoInstruccion == INSTRUCCION_fseek ? string_from_format("F_SEEK %s %d", nombreArchivo, operando2)
                        : tipoInstruccion == INSTRUCCION_fread ? string_from_format("F_READ %s %d %d", nombreArchivo, operando2, operando3)
                        : tipoInstruccion == INSTRUCCION_fwrite ? string_from_format("F_WRITE %s %d %d", nombreArchivo, operando2, operando3)
                        : tipoInstruccion == INSTRUCCION_ftruncate ? string_from_format("F_TRUNCATE %s %d", nombreArchivo, operando2)
                        // Semaforos
                        : tipoInstruccion == INSTRUCCION_wait ? string_from_format("WAIT %s", dispositivoIo)
                        : tipoInstruccion == INSTRUCCION_signal ? string_from_format("SIGNAL %s", dispositivoIo)
                        // Memoria
                        : tipoInstruccion == INSTRUCCION_create_segment ? string_from_format("CREATE_SEGMENT %d %d", operando1, operando2)
                        : tipoInstruccion == INSTRUCCION_delete_segment ? string_from_format("DELETE_SEGMENT %d", operando1)
                        // Desalojo voluntario y exit
                        : tipoInstruccion == INSTRUCCION_yield ? string_from_format("YIELD")
                        : tipoInstruccion == INSTRUCCION_exit ? string_from_format("EXIT")
                        : string_from_format("UNKNOWN");

    if (valorSet != NULL) free(valorSet);
    if (dispositivoIo != NULL) free(dispositivoIo);
    if (nombreArchivo != NULL) free(nombreArchivo);
    
    return instruccionToString;
}

// Destruye una instruccion
static void __instruccion_destroy(void *selfVoid) 
{
    t_instruccion* self = (t_instruccion*)selfVoid;
    
    if (self->valorSet != NULL) free(self->valorSet);
    if (self->dispositivoIo != NULL) free(self->dispositivoIo);
    if (self->nombreArchivo != NULL) free(self->nombreArchivo);

    free(self->toString);
    free(self);

    return;
}

// Funciones publicas

// Creates y destroys de estructuras

t_instruccion *instruccion_create(t_tipo_instruccion tipoInstruccion, t_info_instruccion *infoInstruccion) 
{
    t_instruccion *self = malloc(sizeof(*self));
    
    // Inicializamos los valores a partir de la estructura auxiliar infoInstruccion
    self->tipoInstruccion = tipoInstruccion;
    self->operando1 = info_instruccion_get_operando1(infoInstruccion);
    self->operando2 = info_instruccion_get_operando2(infoInstruccion);
    self->operando3 = info_instruccion_get_operando3(infoInstruccion);
    self->registro1 = info_instruccion_get_registro1(infoInstruccion);
    self->registro2 = info_instruccion_get_registro2(infoInstruccion);
    self->valorSet = info_instruccion_get_valor_set(infoInstruccion);
    self->dispositivoIo = info_instruccion_get_dispositivo_io(infoInstruccion);
    self->nombreArchivo = info_instruccion_get_nombre_archivo(infoInstruccion);
    self->toString = __instruccion_to_string(self);
    
    return self;
}

t_info_instruccion *info_instruccion_create(void) 
{
    t_info_instruccion *self = malloc(sizeof(*self));

    // Inicializamos las variables con valores nulos
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

t_registros_cpu *registros_cpu_create(void)
{
    t_registros_cpu *tempRegistrosCpu = malloc(sizeof(*tempRegistrosCpu));
    memset((void *) tempRegistrosCpu, '\0', sizeof(*tempRegistrosCpu));

    // Reservamos memoria para cada registro
    // Registros 4 bytes
    tempRegistrosCpu->registroAx = NULL;
    tempRegistrosCpu->registroBx = NULL;
    tempRegistrosCpu->registroCx = NULL;
    tempRegistrosCpu->registroDx = NULL;
    // Registros 8 bytes
    tempRegistrosCpu->registroEax = NULL;
    tempRegistrosCpu->registroEbx = NULL;
    tempRegistrosCpu->registroEcx = NULL;
    tempRegistrosCpu->registroEdx = NULL;
    // Registros 16 bytes
    tempRegistrosCpu->registroRax = NULL;
    tempRegistrosCpu->registroRbx = NULL;
    tempRegistrosCpu->registroRcx = NULL;
    tempRegistrosCpu->registroRdx = NULL;

    return tempRegistrosCpu;
}

void registros_cpu_destroy(t_registros_cpu *self)
{
    // Liberamos memoria para registros
    // Registros 4 bytes
    if (self->registroAx != NULL) free(self->registroAx); 
    if (self->registroBx != NULL) free(self->registroBx); 
    if (self->registroCx != NULL) free(self->registroCx); 
    if (self->registroDx != NULL) free(self->registroDx); 
    // Registros 8 bytes
    if (self->registroEax != NULL) free(self->registroEax);
    if (self->registroEbx != NULL) free(self->registroEbx);
    if (self->registroEcx != NULL) free(self->registroEcx);
    if (self->registroEdx != NULL) free(self->registroEdx);
    // Registros 16 bytes
    if (self->registroRax != NULL) free(self->registroRax);
    if (self->registroRbx != NULL) free(self->registroRbx);
    if (self->registroRcx != NULL) free(self->registroRcx);
    if (self->registroRdx != NULL) free(self->registroRdx);

    free(self);

    return;
}

// Create y destroy de la lista de instrucciones

t_list* instruccion_list_create_from_buffer(t_buffer *bufferConInstrucciones, t_log *loggerModulo) 
{
    t_list *listaInstrucciones = list_create();
    
    bool isExit = false;
    
    while (!isExit) {
        
        t_tipo_instruccion identificadorInstruccion;
        buffer_unpack(bufferConInstrucciones, &identificadorInstruccion, sizeof(identificadorInstruccion));
        
        t_info_instruccion *infoInstruccion = info_instruccion_create();

        // Desempaqueta una instruccion dependiendo de su identificador
        switch (identificadorInstruccion) {
            
            case INSTRUCCION_set:
            {
                t_registro registro1;
                char *valorSet;
                
                // Desempaquetamos y seteamos los parametros registro y valor
                buffer_unpack(bufferConInstrucciones, (void*) &registro1, sizeof(registro1));
                valorSet = buffer_unpack_string(bufferConInstrucciones);

                info_instruccion_set_registro1(infoInstruccion, registro1);
                info_instruccion_set_valor_set(infoInstruccion, valorSet);

                free(valorSet);
                break;
            }

            case INSTRUCCION_movin:
            {
                t_registro registro1;
                uint32_t operando2;

                // Desempaquetamos y seteamos los parametros registro y direccion logica
                buffer_unpack(bufferConInstrucciones, (void*) &registro1, sizeof(registro1));
                buffer_unpack(bufferConInstrucciones, (void*) &operando2, sizeof(operando2));

                info_instruccion_set_registro1(infoInstruccion, registro1);
                info_instruccion_set_operando2(infoInstruccion, operando2);
                break;
            }

            case INSTRUCCION_movout:
            {
                uint32_t operando1;
                t_registro registro2;

                // Desempaquetamos y seteamos los parametros direccion logica y registro
                buffer_unpack(bufferConInstrucciones, (void*) &operando1, sizeof(operando1));
                buffer_unpack(bufferConInstrucciones, (void*) &registro2, sizeof(registro2));

                info_instruccion_set_operando1(infoInstruccion, operando1);
                info_instruccion_set_registro2(infoInstruccion, registro2);
                break;
            }

            case INSTRUCCION_io:
            {
                uint32_t operando1;
                
                // Desempaquetamos y seteamos el parametro tiempo
                buffer_unpack(bufferConInstrucciones, (void*) &operando1, sizeof(operando1));

                info_instruccion_set_operando1(infoInstruccion, operando1);
                break;
            }

            case INSTRUCCION_fopen:
            case INSTRUCCION_fclose:
            {
                char *nombreArchivo;

                // Desempaquetamos y seteamos el parametro nombre archivo
                nombreArchivo = buffer_unpack_string(bufferConInstrucciones);

                info_instruccion_set_nombre_archivo(infoInstruccion, nombreArchivo);

                free(nombreArchivo);
                break;
            }

            case INSTRUCCION_fseek:
            case INSTRUCCION_ftruncate:
            {
                char *nombreArchivo;
                uint32_t operando2;

                // Desempaquetamos y seteamos los parametros nombre archivo y posicion o tamanio
                nombreArchivo = buffer_unpack_string(bufferConInstrucciones);
                buffer_unpack(bufferConInstrucciones, &operando2, sizeof(operando2));

                info_instruccion_set_nombre_archivo(infoInstruccion, nombreArchivo);
                info_instruccion_set_operando2(infoInstruccion, operando2);

                free(nombreArchivo);
                break;
            }

            case INSTRUCCION_fread:
            case INSTRUCCION_fwrite:
            {
                char *nombreArchivo;
                uint32_t operando2;
                uint32_t operando3;

                // Desempaquetamos y seteamos los parametros nombre archivo, direccion logica y cantidad de bytes
                nombreArchivo = buffer_unpack_string(bufferConInstrucciones);
                buffer_unpack(bufferConInstrucciones, &operando2, sizeof(operando2));
                buffer_unpack(bufferConInstrucciones, &operando3, sizeof(operando3));

                info_instruccion_set_nombre_archivo(infoInstruccion, nombreArchivo);
                info_instruccion_set_operando2(infoInstruccion, operando2);
                info_instruccion_set_operando3(infoInstruccion, operando3);

                free(nombreArchivo);
                break;
            }

            case INSTRUCCION_wait:
            case INSTRUCCION_signal:
            {
                char *dispositivoIo;

                // Desempaquetamos y seteamos el parametro recurso o dispositivo io
                dispositivoIo = buffer_unpack_string(bufferConInstrucciones);

                info_instruccion_set_dispositivo_io(infoInstruccion, dispositivoIo);

                free(dispositivoIo);
                break;
            }

            case INSTRUCCION_create_segment:
            {
                uint32_t operando1;
                uint32_t operando2;

                // Desempaquetamos y seteamos los parametros id del segmento y tamanio
                buffer_unpack(bufferConInstrucciones, (void*) &operando1, sizeof(operando1));
                buffer_unpack(bufferConInstrucciones, (void*) &operando2, sizeof(operando2));

                info_instruccion_set_operando1(infoInstruccion, operando1);
                info_instruccion_set_operando2(infoInstruccion, operando2);
                break;
            }

            case INSTRUCCION_delete_segment:
            {
                uint32_t operando1;
                
                // Desempaquetamos y seteamos el parametro id del segmento
                buffer_unpack(bufferConInstrucciones, (void*) &operando1, sizeof(operando1));

                info_instruccion_set_operando1(infoInstruccion, operando1);
                break;
            }
            
            case INSTRUCCION_yield:
                break;
            
            case INSTRUCCION_exit:
                isExit = true;
                break;
            
            default:
                log_error(loggerModulo, "Error al intentar desempaquetar una instrucción");
                exit(EXIT_FAILURE);
        }
        
        // Cargo instruccion en la lista de instrucciones
        t_instruccion* instruccionActual = instruccion_create(identificadorInstruccion, infoInstruccion);
        list_add(listaInstrucciones, (void*) instruccionActual);
        
        char *stringInstruccion = instruccion_get_to_string(instruccionActual);
        log_info(loggerModulo, "Se desempaqueta la instruccion %s", stringInstruccion);
        free(stringInstruccion);

        info_instruccion_destroy(infoInstruccion);
    }
    
    log_info(loggerModulo, "Se desempaquetan %d instrucciones", list_size(listaInstrucciones));
    
    return listaInstrucciones;
}

void destroy_lista_instrucciones(t_list* listaInstrucciones)
{
    list_destroy_and_destroy_elements(listaInstrucciones, __instruccion_destroy);

    return;
}

// Getters instruccion

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

char *instruccion_get_to_string(t_instruccion *self) 
{
    return string_duplicate(self->toString);
}

// Getters info instruccion

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

// Setters info instruccion

void info_instruccion_set_operando1(t_info_instruccion* self, uint32_t operando1) 
{
    self->operando1 = operando1;
}

void info_instruccion_set_operando2(t_info_instruccion* self, uint32_t operando2) 
{
    self->operando2 = operando2;
}

void info_instruccion_set_operando3(t_info_instruccion *self, uint32_t operando3)
{
    self->operando3 = operando3;
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
    return;
}

void info_instruccion_set_nombre_archivo(t_info_instruccion *self, char *nombreArchivo)
{
    self->nombreArchivo = nombreArchivo != NULL ? string_duplicate(nombreArchivo) : nombreArchivo;
    return;
}

// Getters registros cpu

char *registros_cpu_get_registro_ax(t_registros_cpu *self)
{
    char *registroAx = self->registroAx;
    return registroAx != NULL ? string_duplicate(registroAx) : registroAx;
}

char *registros_cpu_get_registro_bx(t_registros_cpu *self)
{
    char *registroBx = self->registroBx;
    return registroBx != NULL ? string_duplicate(registroBx) : registroBx;
}

char *registros_cpu_get_registro_cx(t_registros_cpu *self)
{
    char *registroCx = self->registroCx;
    return registroCx != NULL ? string_duplicate(registroCx) : registroCx;
}

char *registros_cpu_get_registro_dx(t_registros_cpu *self)
{
    char *registroDx = self->registroDx;
    return registroDx != NULL ? string_duplicate(registroDx) : registroDx;
}

char *registros_cpu_get_registro_eax(t_registros_cpu *self)
{
    char *registroEax = self->registroEax;
    return registroEax != NULL ? string_duplicate(registroEax) : registroEax;
}

char *registros_cpu_get_registro_ebx(t_registros_cpu *self)
{
    char *registroEbx = self->registroEbx;
    return registroEbx != NULL ? string_duplicate(registroEbx) : registroEbx;
}

char *registros_cpu_get_registro_ecx(t_registros_cpu *self)
{
    char *registroEcx = self->registroEcx;
    return registroEcx != NULL ? string_duplicate(registroEcx) : registroEcx;
}

char *registros_cpu_get_registro_edx(t_registros_cpu *self)
{
    char *registroEdx = self->registroEdx;
    return registroEdx != NULL ? string_duplicate(registroEdx) : registroEdx;
}

char *registros_cpu_get_registro_rax(t_registros_cpu *self)
{
    char *registroRax = self->registroRax;
    return registroRax != NULL ? string_duplicate(registroRax) : registroRax;
}

char *registros_cpu_get_registro_rbx(t_registros_cpu *self)
{
    char *registroRbx = self->registroRbx;
    return registroRbx != NULL ? string_duplicate(registroRbx) : registroRbx;
}

char *registros_cpu_get_registro_rcx(t_registros_cpu *self)
{
    char *registroRcx = self->registroRcx;
    return registroRcx != NULL ? string_duplicate(registroRcx) : registroRcx;
}

char *registros_cpu_get_registro_rdx(t_registros_cpu *self)
{
    char *registroRdx = self->registroRdx;
    return registroRdx != NULL ? string_duplicate(registroRdx) : registroRdx;
}

// Setters registros cpu

void registros_cpu_set_registro_ax(t_registros_cpu *self, char *valor)
{
    if (self->registroAx == NULL) {
        self->registroAx = malloc(sizeof(uint32_t));
    }
    memcpy(self->registroAx, valor, sizeof(uint32_t));

    return;
}

void registros_cpu_set_registro_bx(t_registros_cpu *self, char *valor)
{
    if (self->registroBx == NULL) {
        self->registroBx = malloc(sizeof(uint32_t));
    }
    memcpy(self->registroBx, valor, sizeof(uint32_t));

    return;
}

void registros_cpu_set_registro_cx(t_registros_cpu *self, char *valor)
{
    if (self->registroCx == NULL) {
        self->registroCx = malloc(sizeof(uint32_t));
    }
    memcpy(self->registroCx, valor, sizeof(uint32_t));

    return;
}

void registros_cpu_set_registro_dx(t_registros_cpu *self, char *valor)
{
    if (self->registroDx == NULL) {
        self->registroDx = malloc(sizeof(uint32_t));
    }
    memcpy(self->registroDx, valor, sizeof(uint32_t));

    return;
}

void registros_cpu_set_registro_eax(t_registros_cpu *self, char *valor)
{
    if (self->registroEax == NULL) {
        self->registroEax = malloc(sizeof(uint64_t));
    }
    memcpy(self->registroEax, valor, sizeof(uint64_t));

    return;
}

void registros_cpu_set_registro_ebx(t_registros_cpu *self, char *valor)
{
    if (self->registroEbx == NULL) {
        self->registroEbx = malloc(sizeof(uint64_t));
    }
    memcpy(self->registroEbx, valor, sizeof(uint64_t));

    return;
}

void registros_cpu_set_registro_ecx(t_registros_cpu *self, char *valor)
{
    if (self->registroEcx == NULL) {
        self->registroEcx = malloc(sizeof(uint64_t));
    }
    memcpy(self->registroEcx, valor, sizeof(uint64_t));

    return;
}

void registros_cpu_set_registro_edx(t_registros_cpu *self, char *valor)
{
    if (self->registroEdx == NULL) {
        self->registroEdx = malloc(sizeof(uint64_t));
    }
    memcpy(self->registroEdx, valor, sizeof(uint64_t));

    return;
}

void registros_cpu_set_registro_rax(t_registros_cpu *self, char *valor)
{
    if (self->registroRax == NULL) {
        self->registroRax = malloc(2 * sizeof(uint64_t));
    }
    memcpy(self->registroRax, valor, 2 * sizeof(uint64_t));

    return;
}

void registros_cpu_set_registro_rbx(t_registros_cpu *self, char *valor)
{
    if (self->registroRbx == NULL) {
        self->registroRbx = malloc(2 * sizeof(uint64_t));
    }
    memcpy(self->registroRbx, valor, 2 * sizeof(uint64_t));

    return;
}

void registros_cpu_set_registro_rcx(t_registros_cpu *self, char *valor)
{
    if (self->registroRcx == NULL) {
        self->registroRcx = malloc(2 * sizeof(uint64_t));
    }
    memcpy(self->registroRcx, valor, 2 * sizeof(uint64_t));

    return;
}

void registros_cpu_set_registro_rdx(t_registros_cpu *self, char *valor)
{
    if (self->registroRdx == NULL) {
        self->registroRdx = malloc(2 * sizeof(uint64_t));
    }
    memcpy(self->registroRdx, valor, 2 * sizeof(uint64_t));

    return;
}
