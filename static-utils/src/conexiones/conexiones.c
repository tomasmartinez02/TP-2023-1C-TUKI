#include <conexiones/conexiones.h>

//Funciones publicas
int iniciar_servidor(char *ip, char *puerto) 
{
    int optVal = 1;
    struct addrinfo hints;
    struct addrinfo *serverInfo;
    // Seteo las configuraciones del servidor junto con la ip y el puerto
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Cargo la informacion en serverInfo y chequeo que se haya hecho correctamente
    int rv = getaddrinfo(ip, puerto, &hints, &serverInfo);
    if (rv != 0) {
        printf("getaddrinfo error: %s\n", gai_strerror(rv));
        return EXIT_FAILURE;
    }

    // Creo el socket del servidor y chequeo que se haya hecho correctamente
    int serverSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
    if (serverSocket == -1) {
        printf("Socket creation error\n%s", strerror(errno));
        return EXIT_FAILURE;
    }

    // Seteo configuraciones del servidor y chequeo que se haya hecho correctamente
    int sockOpt = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    if (sockOpt == -1) {
        printf("Sockopt error\n%s", strerror(errno));
        return EXIT_FAILURE;
    }

    // Bindeo el puerto a mi servidor para que el SO no se lo asigne a nadie mas
    // Y chequeo que se haya hecho correctamente
    int bindVal = bind(serverSocket, serverInfo->ai_addr, serverInfo->ai_addrlen);
    if (bindVal == -1) {
        printf("Mismatched bind\n%s", strerror(errno));
        return EXIT_FAILURE;
    }

    // Seteo al servidor para que pueda quedarse escuchando conexiones de los clientes
    // Y chequeo que se haya hecho correctamente
    int listenVal = listen(serverSocket, SOMAXCONN);
    if (listenVal == -1) {
        printf("Listen error\n%s", strerror(errno));
        return EXIT_FAILURE;
    }

    freeaddrinfo(serverInfo);

    return serverSocket;
}

int conectar_a_servidor(char *ip, char *puerto) 
{
    int conn;
    struct addrinfo hints;
    struct addrinfo *serverInfo;
    struct addrinfo *p;
    // Seteo las configuraciones junto con la ip y el puerto del servidor a conectarse
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Cargo la informacion en serverInfo y chequeo que se haya hecho correctamente
    int rv = getaddrinfo(ip, puerto, &hints, &serverInfo);
    if (rv != 0) {
        fprintf(stderr, "getaddrinfo error: %s", gai_strerror(rv));
        return EXIT_FAILURE;
    }

    // Intento repetidas veces conectarme al servidor hasta que se conecte correctamente
    for (p = serverInfo; p != NULL; p = p->ai_next) {
        // Intento establecer conexion inicial
        conn = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        // Si no se conecto correctamente, continuo intentando
        if (conn == -1) {
            continue;
        }
        // Si se conecto correctamente, salgo del bucle
        if (connect(conn, p->ai_addr, p->ai_addrlen) != -1) {
            break;
        }
        close(conn);
    }

    freeaddrinfo(serverInfo);
    // Chequeo si la conexion fue exitosa
    if (conn != -1 && p != NULL) {
        return conn;
    }

    return -1;
}