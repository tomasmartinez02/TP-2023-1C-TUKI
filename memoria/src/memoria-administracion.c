#include <memoria-administracion.h>

// Variables estaticas
void* memoriaPrincipal; 
t_info_segmentos* tablaSegmentos;

// Variables globales
t_huecos_libres *listaHuecosLibres; 
t_info_segmentos *segmentoCero;
lista_tablas *tablasDeSegmentos;
pthread_mutex_t mutexSocketKernel;
// Funciones privadas

// Funciones inicializacion estructuras de la memoria

static t_algoritmo __algoritmo_seleccionado(void)
{
    t_algoritmo algoritmo;

    if (strcmp(memoria_config_get_algoritmo_asignacion(memoriaConfig), "FIRST") == 0) {
        algoritmo = FIRST_FIT;
    }

    if (strcmp(memoria_config_get_algoritmo_asignacion(memoriaConfig), "BEST") == 0) {
        algoritmo = BEST_FIT;
    }

    if (strcmp(memoria_config_get_algoritmo_asignacion(memoriaConfig), "WORST") == 0) {
        algoritmo = WORST_FIT;
    }

    return algoritmo;
}

static void __inicializar_memoria_principal(void)
{
    uint32_t tamanioMemoria;
    tamanioMemoria = memoria_config_get_tamanio_memoria(memoriaConfig);

    memoriaPrincipal = malloc(tamanioMemoria); // Esto nunca se libera no?

    log_info(memoriaDebuggingLogger, "Se inicializa a la memoria principal con un tamanio de %d bytes", tamanioMemoria);

    return;
}

static t_huecos_libres* __crear_lista_huecos_libres (uint32_t direccionBase, uint32_t tamanio) // lo crea pero no lo agrega a la lista
{   
    t_huecos_libres* nuevoHueco = malloc(sizeof(t_huecos_libres));
    nuevoHueco->hueco = malloc(sizeof(t_info_segmentos));
    nuevoHueco->siguiente = NULL;
    
    nuevoHueco->hueco->direccionBase = direccionBase;
    nuevoHueco->hueco->tamanio = tamanio;    

    return nuevoHueco;
}

static t_info_segmentos* __crear_hueco (uint32_t base, uint32_t tamanio)
{
    t_info_segmentos* nuevoHueco = malloc(sizeof(t_info_segmentos));
    nuevoHueco->direccionBase = base;
    nuevoHueco->tamanio = tamanio;

    return nuevoHueco;
}

static void __eliminar_hueco (t_huecos_libres *huecoAEliminar) 
{
    t_huecos_libres *auxiliarLista = listaHuecosLibres;

    while(auxiliarLista->hueco->direccionBase < huecoAEliminar->hueco->direccionBase) {
        auxiliarLista = auxiliarLista->siguiente;
    } // avanza hasta que encuentra el nodo anterior al que quiere eliminar

    auxiliarLista->siguiente = huecoAEliminar->siguiente;

    free(huecoAEliminar);

    return ;
}

static void __partir_hueco (t_huecos_libres *auxiliarLista, t_info_segmentos *segmento) 
{
    auxiliarLista->hueco->direccionBase = segmento->direccionBase + segmento->tamanio;
    auxiliarLista->hueco->tamanio = auxiliarLista->hueco->tamanio - segmento->tamanio;

    return ;
}

void actualizar_lista_huecos_libres (t_info_segmentos *segmento) // el nombre de esta funcion habria que cambiarlo
{
    t_huecos_libres *auxiliarLista = listaHuecosLibres;
    uint32_t posicionSegmento = segmento->direccionBase;
    uint32_t tamanioSegmento = segmento->tamanio;
    uint32_t direccionBaseHueco = auxiliarLista->hueco->direccionBase;
    uint32_t tamanioHueco = auxiliarLista->hueco->tamanio;
    
    while(direccionBaseHueco != posicionSegmento) {
        auxiliarLista = auxiliarLista->siguiente;
        direccionBaseHueco = auxiliarLista->hueco->direccionBase;
    } // aca busca que hueco habria que modificar 

    tamanioHueco = auxiliarLista->hueco->tamanio;

    if (tamanioHueco == tamanioSegmento){
        __eliminar_hueco(auxiliarLista); // ver bien despues esta funcion
    } else {
        __partir_hueco(auxiliarLista, segmento); 
    }
    
    return ;
}

void __crear_estructura_espacios_libres (void) 
{   
    listaHuecosLibres = __crear_lista_huecos_libres(0, memoria_config_get_tamanio_memoria(memoriaConfig));
    return ;
}

void __crear_segmentos_cero(void)
{
    uint32_t tamanioSegmentoCero = memoria_config_get_tamanio_segmento_cero(memoriaConfig);
    segmentoCero = crear_info_segmentos(0,0, tamanioSegmentoCero); // despues ver esto
    actualizar_lista_huecos_libres(segmentoCero);

    tablasDeSegmentos = NULL;

    return;
}

static uint32_t __obtener_base_segmento_first_fit(t_info_segmentos* segmento)
{
    t_huecos_libres* auxiliarLista = listaHuecosLibres;
    uint32_t baseSegmento;

    while (auxiliarLista->hueco->tamanio < segmento->tamanio)
    {
        auxiliarLista = auxiliarLista->siguiente;
    }

    baseSegmento = auxiliarLista->hueco->direccionBase;

    return baseSegmento;
}

static uint32_t __obtener_base_segmento_best_fit(t_info_segmentos* segmento)
{
    t_huecos_libres* auxiliarLista = listaHuecosLibres;
    uint32_t baseSegmento = auxiliarLista->hueco->direccionBase;
    uint32_t tamanioHueco = auxiliarLista->hueco->tamanio;

    while (auxiliarLista->siguiente != NULL)
    {
        auxiliarLista = auxiliarLista->siguiente;
        if (auxiliarLista->hueco->tamanio < tamanioHueco && auxiliarLista->hueco->tamanio >= segmento->tamanio)
        {
            tamanioHueco = auxiliarLista->hueco->tamanio;
            baseSegmento = auxiliarLista->hueco->direccionBase;
        }
    }

    return baseSegmento;
}

static uint32_t __obtener_base_segmento_worst_fit(t_info_segmentos* segmento)
{
    t_huecos_libres* auxiliarLista = listaHuecosLibres;
    uint32_t baseSegmento = auxiliarLista->hueco->direccionBase;
    uint32_t tamanioHueco = auxiliarLista->hueco->tamanio;

    while (auxiliarLista->siguiente != NULL)
    {
        auxiliarLista = auxiliarLista->siguiente;
        if (auxiliarLista->hueco->tamanio > tamanioHueco && auxiliarLista->hueco->tamanio >= segmento->tamanio)
        {
            tamanioHueco = auxiliarLista->hueco->tamanio;
            baseSegmento = auxiliarLista->hueco->direccionBase;
        }
    }

    return baseSegmento;
}

static uint32_t __obtener_base_segmento_segun_algoritmo(t_algoritmo algoritmo, t_info_segmentos* segmento)
{
    uint32_t baseSegmento;

    switch (algoritmo)
    {
    case BEST_FIT:
        baseSegmento = __obtener_base_segmento_best_fit(segmento);
        break;

    case WORST_FIT:
        baseSegmento = __obtener_base_segmento_worst_fit(segmento);
        break;

    case FIRST_FIT:
        baseSegmento = __obtener_base_segmento_first_fit(segmento);
        break;

    default:
        break;
    }

    return baseSegmento;
}

static void __agregar_segmento_a_tabla(t_info_segmentos* segmento, uint32_t pid, uint32_t baseSegmento){
    //TODO
    lista_tablas *aux = tablasDeSegmentos;
    t_info_segmentos** tablaSeleccionada;
    uint32_t indice = 0;
    
    while (aux->pidProceso != pid) {
        aux = aux->siguiente;
    }

    tablaSeleccionada = aux->tablaSegmentos;

    // si es necesario, habria que chequear que la tabla no este llena

    while (tablaSeleccionada[indice]->idSegmento != -1) {
        indice = indice + 1;
    }

    tablaSeleccionada[indice]->direccionBase = baseSegmento;
    tablaSeleccionada[indice]->idSegmento = segmento->idSegmento;
    tablaSeleccionada[indice]->tamanio = segmento->tamanio;

    actualizar_lista_huecos_libres(tablaSeleccionada[indice]);

    free(segmento);   
    
    return;
}

t_info_segmentos **__buscar_tabla_segun_pid(uint32_t pid)
{
    lista_tablas *aux = tablasDeSegmentos;
    t_info_segmentos** tablaSeleccionada;
    
    while (aux->pidProceso != pid) {
        aux = aux->siguiente;
    }
    
    tablaSeleccionada = aux->tablaSegmentos;

    return tablaSeleccionada;
}

static t_info_segmentos *__eliminar_segmento_de_tabla (t_info_segmentos** tablaDeSegmentos, uint32_t idSegmento, uint32_t pid)
{
    uint32_t indice = 0;
    t_info_segmentos* huecoLiberado;

    while (tablaDeSegmentos[indice]->idSegmento != idSegmento) {
        indice = indice + 1;
    }

    log_info(memoriaLogger,  "PID: <%d> - Eliminar Segmento: <%d> - Base: <%d> - TAMAÑO: <%d>", pid, idSegmento, tablaDeSegmentos[indice]->direccionBase, tablaDeSegmentos[indice]->tamanio);

    huecoLiberado = __crear_hueco(tablaDeSegmentos[indice]->direccionBase, tablaDeSegmentos[indice]->tamanio);

    tablaDeSegmentos[indice]->direccionBase = 0;
    tablaDeSegmentos[indice]->idSegmento = -1;
    tablaDeSegmentos[indice]->tamanio = 0;

    return huecoLiberado;
}

static void __unir_3_huecos (t_huecos_libres* huecoAnterior, t_huecos_libres* huecoSiguiente, t_info_segmentos* huecoAInsertar) 
{
    uint32_t nuevoTamanio = huecoAnterior->hueco->tamanio + huecoAInsertar->tamanio + huecoSiguiente->hueco->tamanio;
    
    huecoAnterior->hueco->tamanio = nuevoTamanio;
    huecoAnterior->siguiente = huecoSiguiente->siguiente;
    
    free(huecoSiguiente->hueco);
    free(huecoSiguiente);
    free(huecoAInsertar);

    return;
}

static void __unir_2_huecos_inferior (t_huecos_libres* huecoAnterior, t_info_segmentos* huecoAInsertar)
{
    uint32_t nuevoTamanio = huecoAnterior->hueco->tamanio + huecoAInsertar->tamanio;

    huecoAnterior->hueco->tamanio = nuevoTamanio;

    free(huecoAInsertar);

    return;
}

static void __unir_2_huecos_superior (t_info_segmentos* huecoAInsertar, t_huecos_libres* huecoSiguiente)
{
    uint32_t nuevaBase = huecoAInsertar->direccionBase;
    uint32_t nuevoTamanio = huecoAInsertar->tamanio + huecoSiguiente->hueco->tamanio;

    huecoSiguiente->hueco->direccionBase = nuevaBase;
    huecoSiguiente->hueco->tamanio = nuevoTamanio;

    free(huecoAInsertar);

    return;
}

static void __insertar_hueco (t_huecos_libres* huecoAnterior, t_huecos_libres* huecoSiguiente, t_info_segmentos* huecoAInsertar) 
{
    t_huecos_libres* nuevoHueco = __crear_lista_huecos_libres(huecoAInsertar->direccionBase, huecoAInsertar->tamanio);

    nuevoHueco->siguiente = huecoSiguiente;
    huecoAnterior->siguiente = nuevoHueco;

    return;
}

static void __insertar_hueco_antes (t_huecos_libres* huecoExistente, t_info_segmentos* huecoAInsertar) 
{
    t_huecos_libres *nuevoHueco = __crear_lista_huecos_libres(huecoAInsertar->direccionBase, huecoAInsertar->tamanio);

    nuevoHueco->siguiente = huecoExistente;
    listaHuecosLibres = nuevoHueco;

    return; 
}

void __insertar_en_un_solo_hueco(t_info_segmentos* huecoAInsertar)
{
    t_huecos_libres* huecoExistente = listaHuecosLibres;

    uint32_t baseHuecoExistente = huecoExistente->hueco->direccionBase;
    uint32_t limiteHuecoExistente = huecoExistente->hueco->direccionBase + huecoExistente->hueco->tamanio;                                                                                                                                                                                                                                                                                                                                                                                                      
    uint32_t baseHuecoAInsertar = huecoAInsertar->direccionBase;
    uint32_t limiteHuecoAInsertar = huecoAInsertar->direccionBase + huecoAInsertar->tamanio;

    if (limiteHuecoAInsertar == baseHuecoExistente) {
        __unir_2_huecos_inferior(huecoExistente, huecoAInsertar); 
        // los tiene que unir dejando la base del hueco a insertar y cambiando el tamanio
    } else if (limiteHuecoExistente == baseHuecoAInsertar) {
        __unir_2_huecos_superior(huecoAInsertar, huecoExistente); 
        // los tiene que unir dejando la base del hueco existente y cambiando el tamanio
    } else if (baseHuecoExistente < baseHuecoAInsertar) {
        __insertar_hueco(huecoExistente, NULL, huecoAInsertar);
        // el hueco existente tiene que apuntar al hueco a insertar en su siguiente
    } else {
        __insertar_hueco_antes(huecoExistente, huecoAInsertar);
        // el hueco a insertar tiene que apuntar al hueco existente en su siguiente
    }

    return; 
}

void __insertar_hueco_en_posicion (t_info_segmentos* huecoAInsertar, t_huecos_libres* huecoSiguiente)
{
    t_huecos_libres* huecoAnterior = listaHuecosLibres;
    uint32_t baseHuecoSiguiente;

    if (huecoSiguiente != NULL) {
        while ((huecoAnterior->siguiente->hueco->direccionBase != huecoSiguiente->hueco->direccionBase)) {
        huecoAnterior = huecoAnterior->siguiente;
        }
        baseHuecoSiguiente = huecoSiguiente->hueco->direccionBase;
    } else {
        baseHuecoSiguiente = 0;
    }
    uint32_t limiteHuecoAnterior = huecoAnterior->hueco->direccionBase + huecoAnterior->hueco->tamanio;
    uint32_t baseHuecoAInsertar = huecoAInsertar->direccionBase;
    uint32_t limiteHuecoAInsertar = huecoAInsertar->direccionBase + huecoAInsertar->tamanio;
    if (limiteHuecoAnterior == baseHuecoAInsertar && limiteHuecoAInsertar == baseHuecoSiguiente) {
        __unir_3_huecos(huecoAnterior, huecoSiguiente, huecoAInsertar);
    } else if (limiteHuecoAnterior == baseHuecoAInsertar) {
        __unir_2_huecos_inferior(huecoAnterior, huecoAInsertar); 
    } else if (limiteHuecoAInsertar == baseHuecoSiguiente) {
        __unir_2_huecos_superior(huecoAInsertar, huecoSiguiente); 
    } else {
        __insertar_hueco(huecoAnterior, huecoSiguiente, huecoAInsertar);
    }

    return;
}

static void __insertar_nuevo_hueco(t_info_segmentos* huecoLiberado)
{
    t_huecos_libres* aux = listaHuecosLibres;

    if (aux == NULL) {
        listaHuecosLibres = __crear_lista_huecos_libres(huecoLiberado -> direccionBase, huecoLiberado -> tamanio);
    } else {
        while ((aux->hueco->direccionBase + aux->hueco->tamanio) <= huecoLiberado->direccionBase) {
            aux = aux->siguiente;
        }
        if(aux->hueco->direccionBase == listaHuecosLibres->hueco->direccionBase) { // hay un solo hueco
            __insertar_en_un_solo_hueco(huecoLiberado);
        } else { //hay mas de un hueco
            __insertar_hueco_en_posicion(huecoLiberado, aux);
        }
    }
    
    return;
}

static void __inicializar_mutex_socket()
{
    pthread_mutex_init(&mutexSocketKernel, NULL);
    return;
}

static void __inicializar_hilos() {

    void* resultadoAtencionKernel;
    pthread_t atencionKernelth;
    pthread_create(&atencionKernelth, NULL, atender_peticiones_kernel, NULL);

    void* resultadoAtencionCPU;
    parametrosHilo parametrosCPU;
    parametrosCPU.socketModulo = memoria_config_get_socket_cpu(memoriaConfig);
    parametrosCPU.nombreModulo = "CPU";
    pthread_t atencionCPUth;
    pthread_create(&atencionCPUth, NULL, atender_modulo, (void*)&parametrosCPU);

    /*void* resultadoAtencionFileSystem;
    parametrosHilo parametrosFS;
    parametrosFS.socketModulo = memoria_config_get_socket_filesystem(memoriaConfig);
    parametrosFS.nombreModulo = "FS";
    pthread_t atencionFileSystemth;
    pthread_create(&atencionFileSystemth, NULL, atender_modulo, (void*)&parametrosFS);*/

    pthread_join(atencionKernelth, &resultadoAtencionKernel);
    //pthread_join(atencionFileSystemth, &resultadoAtencionFileSystem);
    //pthread_join(atencionCPUth, &resultadoAtencionCPU);

    return;
}



static void __actualizar_huecos_eliminacion_proceso(t_info_segmentos **tablaSegmentos)
{
    log_info(memoriaLogger,"Entra a la funcion");
    for(int i = 0; i < memoria_config_get_cantidad_segmentos(memoriaConfig); i++){
        if(tablaSegmentos[i]->idSegmento != -1 && tablaSegmentos[i]->idSegmento != 0){
            log_info(memoriaLogger,"Entra al if");
            t_info_segmentos *hueco = __crear_hueco(tablaSegmentos[i]->direccionBase, tablaSegmentos[i]->tamanio);
            __insertar_nuevo_hueco(hueco);
        }
    }

    return;
}

// Funciones publicas

uint32_t crear_segmento(t_info_segmentos* segmento, uint32_t pid)
{
    t_algoritmo algoritmoActual;
    algoritmoActual = __algoritmo_seleccionado();
    uint32_t baseSegmento = __obtener_base_segmento_segun_algoritmo(algoritmoActual, segmento);

    __agregar_segmento_a_tabla(segmento, pid, baseSegmento);

    return baseSegmento; 
}

void inicializar_memoria (void) 
{

    __crear_estructura_espacios_libres();
    __crear_segmentos_cero();
    __inicializar_memoria_principal();
    __inicializar_mutex_socket(); // ojo capaz hay que hacer un mutex para la tabla de segmentos
    __inicializar_hilos();

    return;
}

bool verificar_memoria_suficiente (uint32_t tamanioSolicitado)
{
    uint32_t capacidad = 0;
    t_huecos_libres* aux = listaHuecosLibres;

    if (aux == NULL) {
    } else {
        capacidad = capacidad + aux->hueco->tamanio;
        while(aux->siguiente != NULL) {
            capacidad = capacidad + aux->hueco->tamanio;
            aux = aux->siguiente;
        }
    } 
    
    return capacidad >= tamanioSolicitado;
}

bool verificar_memoria_contigua (uint32_t tamanioSolicitado)
{
    t_huecos_libres* aux = listaHuecosLibres;

    while (aux->siguiente != NULL && tamanioSolicitado < aux->hueco->tamanio) {
        aux = aux->siguiente;
    }
    
    return tamanioSolicitado <= aux->hueco->tamanio;
}

void eliminar_segmento (uint32_t idSegmento, uint32_t pid) 
{
    t_info_segmentos** tablaDeSegmentos = __buscar_tabla_segun_pid(pid);
    t_info_segmentos* huecoLiberado = __eliminar_segmento_de_tabla(tablaDeSegmentos, idSegmento, pid);

    __insertar_nuevo_hueco(huecoLiberado);     

    return;
}

void eliminar_estructuras_proceso (uint32_t pid)
{
    lista_tablas *nodoAnterior = tablasDeSegmentos;
    lista_tablas *nodoAEliminar = NULL;
    
    while (nodoAnterior->siguiente != NULL  && nodoAnterior->siguiente->pidProceso != pid) {
        /*if (nodoAnterior->siguiente->pidProceso == pid) {
            nodoAEliminar = nodoAnterior->siguiente;
            break;
        }*/
        nodoAnterior = nodoAnterior->siguiente;
    }
    if(nodoAnterior->pidProceso == pid){
        nodoAEliminar = nodoAnterior;
    }else{
        nodoAEliminar = nodoAnterior->siguiente;
    }

    if (nodoAnterior->siguiente != NULL && nodoAnterior->siguiente->siguiente != NULL) {
    nodoAnterior->siguiente = nodoAnterior->siguiente->siguiente;
    } else {
        nodoAnterior->siguiente = NULL;
    }

    __actualizar_huecos_eliminacion_proceso(nodoAEliminar->tablaSegmentos);

    destruir_tabla_segmentos(nodoAEliminar->tablaSegmentos, memoria_config_get_cantidad_segmentos(memoriaConfig));
    free(nodoAEliminar);

    return;
}

