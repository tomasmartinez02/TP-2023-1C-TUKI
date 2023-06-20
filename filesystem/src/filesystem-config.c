#include <filesystem-config.h>

// Funcion utilizada para la instanciacion de la configuracion de filesystem
static void __filesystem_config_initializer(void *moduleConfig, t_config *tempCfg) 
{
    t_filesystem_config *filesystemConfig = (t_filesystem_config *) moduleConfig;

    filesystemConfig->IP_MEMORIA = strdup(config_get_string_value(tempCfg, "IP_MEMORIA"));
    filesystemConfig->PUERTO_MEMORIA = strdup(config_get_string_value(tempCfg, "PUERTO_MEMORIA"));
    filesystemConfig->IP_ESCUCHA = strdup(config_get_string_value(tempCfg, "IP_ESCUCHA"));
    filesystemConfig->PUERTO_ESCUCHA = strdup(config_get_string_value(tempCfg, "PUERTO_ESCUCHA"));
    filesystemConfig->PATH_SUPERBLOQUE = strdup(config_get_string_value(tempCfg, "PATH_SUPERBLOQUE"));
    filesystemConfig->PATH_BITMAP = strdup(config_get_string_value(tempCfg, "PATH_BITMAP"));
    filesystemConfig->PATH_BLOQUES = strdup(config_get_string_value(tempCfg, "PATH_BLOQUES"));
    filesystemConfig->PATH_FCB = strdup(config_get_string_value(tempCfg, "PATH_FCB"));
    filesystemConfig->RETARDO_ACCESO_BLOQUE = (uint32_t) config_get_int_value(tempCfg, "RETARDO_ACCESO_BLOQUE");
    filesystemConfig->SOCKET_MEMORIA = -1;
    filesystemConfig->SOCKET_KERNEL = -1;

    return;
}

// Funciones publicas
t_filesystem_config *filesystem_config_create(char *filesystemConfigPath, t_log *filesystemLogger) 
{
    t_filesystem_config *self = malloc(sizeof(*self));
    
    //Chequeo que la inicializacion de la configuracion del filesystem sea correcta
    int inicializacionCorrecta = config_init(self, filesystemConfigPath, filesystemLogger, __filesystem_config_initializer);
    if(inicializacionCorrecta == -1){
        exit(EXIT_FAILURE);
    }
    
    return self;
}

void filesystem_config_destroy(t_filesystem_config *self) 
{
    free(self->IP_MEMORIA);
    free(self->PUERTO_MEMORIA);
    free(self->IP_ESCUCHA);
    free(self->PUERTO_ESCUCHA);
    free(self->PATH_SUPERBLOQUE);
    free(self->PATH_BITMAP);
    free(self->PATH_BLOQUES);
    free(self->PATH_FCB);
    free(self);

    return;
}

char *filesystem_config_get_memory_ip(t_filesystem_config *self)
{
    return self->IP_MEMORIA;
}

char *filesystem_config_get_puerto_memoria(t_filesystem_config *self)
{
    return self->PUERTO_MEMORIA;
}

char *filesystem_config_get_ip_escucha(t_filesystem_config *self)
{
    return self->IP_ESCUCHA;
}

char *filesystem_config_get_puerto_escucha(t_filesystem_config *self)
{
    return self->PUERTO_ESCUCHA;
}

char *filesystem_config_get_path_superbloque(t_filesystem_config *self)
{
    return self->PATH_SUPERBLOQUE;
}

char *filesystem_config_get_path_bitmap(t_filesystem_config *self)
{
    return self->PATH_BITMAP;
}

char *filesystem_config_get_path_bloques(t_filesystem_config *self)
{
    return self->PATH_BLOQUES;
}

char *filesystem_config_get_path_fcb(t_filesystem_config *self)
{
    return self->PATH_FCB;
}

uint32_t filesystem_config_get_retardo_acceso_bloque(t_filesystem_config *self)
{
    return self->RETARDO_ACCESO_BLOQUE;
}

int filesystem_config_get_socket_memoria(t_filesystem_config *self)
{
    return self->SOCKET_MEMORIA;
}

int filesystem_config_get_socket_kernel(t_filesystem_config *self)
{
    return self->SOCKET_KERNEL;
}

void filesystem_config_set_socket_memoria(t_filesystem_config *self, int socketMemoria)
{
    self->SOCKET_MEMORIA = socketMemoria;
}

void filesystem_config_set_socket_kernel(t_filesystem_config *self, int socketKernel)
{
    self->SOCKET_KERNEL = socketKernel;
}