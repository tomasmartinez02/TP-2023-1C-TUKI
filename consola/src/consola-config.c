#include <consola-config.h>

// Funciones privadas

// Funcion utilizada para la instanciacion de la configuracion de la consola
static void __consola_config_initializer(void *moduleConfig, t_config *tempCfg) 
{
    t_consola_config *consolaConfig = (t_consola_config *) moduleConfig;

    consolaConfig->IP_KERNEL = strdup(config_get_string_value(tempCfg, "IP_KERNEL"));
    consolaConfig->PUERTO_KERNEL = strdup(config_get_string_value(tempCfg, "PUERTO_KERNEL"));
    consolaConfig->SOCKET_KERNEL = -1;
    consolaConfig->PID = -1;

    return;
}

// Funciones publicas
t_consola_config *consola_config_create(char *consolaConfigPath, t_log *consolaLogger) 
{
    t_consola_config *self = malloc(sizeof(*self));
    
    //Chequeo que la inicializacion de la configuracion de la consola sea correcta
    int inicializacionCorrecta = config_init(self, consolaConfigPath, consolaLogger, __consola_config_initializer);
    if(inicializacionCorrecta == -1){
        exit(EXIT_FAILURE);
    }
    
    return self;
}

void consola_config_destroy(t_consola_config *self) 
{
    free(self->IP_KERNEL);
    free(self->PUERTO_KERNEL);
    free(self);

    return;
}

char *consola_config_get_ip_kernel(t_consola_config *self) 
{
    return self->IP_KERNEL;
}

char *consola_config_get_puerto_kernel(t_consola_config *self) 
{
    return self->PUERTO_KERNEL;
}

int consola_config_get_socket_kernel(t_consola_config *self)
{
    return self->SOCKET_KERNEL;
}

int consola_config_get_pid(t_consola_config *self)
{
    return self->PID;
}

void consola_config_set_socket_kernel(t_consola_config *self, int socketKernel)
{
    self->SOCKET_KERNEL = socketKernel;
}

void consola_config_set_pid(t_consola_config *self, uint32_t pid)
{
    self->PID = pid;
}