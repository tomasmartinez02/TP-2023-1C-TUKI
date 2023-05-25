#include <serializacion/stream.h>

// Funciones estaticas o privadas de la biblioteca
// Envia un stream con un tamanio de buffer
static void __stream_send(int toSocket, void *streamToSend, uint32_t bufferSize) 
{
    // Variables creadas para el sizeof
    uint8_t header = 0;
    uint32_t size = 0;
    
    ssize_t bytesSent = send(toSocket, streamToSend, sizeof(header) + sizeof(size) + bufferSize, 0);
    
    // Chequeo que se haya enviado bien el stream
    if (bytesSent == -1) {
        printf("\e[0;31m__stream_send: Error en el envío del buffer [%s]\e[0m\n", strerror(errno));
    }

    return;
}

// Crea un paquete con un header y un buffer
static void *__stream_create(uint8_t header, t_buffer *buffer) 
{
    void *streamToSend = malloc(sizeof(header) + sizeof(buffer->size) + buffer->size);
    
    // Creamos el stream
    int offset = 0;
    memcpy(streamToSend + offset, &header, sizeof(header));
    offset += sizeof(header);
    memcpy(streamToSend + offset, &(buffer->size), sizeof(buffer->size));
    offset += sizeof(buffer->size);
    memcpy(streamToSend + offset, buffer->stream, buffer->size);
    
    return streamToSend;
}

// Funciones publicas
void stream_send_buffer(int toSocket, uint8_t header, t_buffer *buffer) 
{
    void *stream = __stream_create(header, buffer);
    __stream_send(toSocket, stream, buffer->size);
    free(stream);

    return;
}

// Enviamos un buffer vacio cuando solo queremos enviar un codigo de operacion en el header
void stream_send_empty_buffer(int toSocket, uint8_t header) 
{
    t_buffer *emptyBuffer = buffer_create();
    stream_send_buffer(toSocket, header, emptyBuffer);
    buffer_destroy(emptyBuffer);

    return;
}

void stream_recv_buffer(int fromSocket, t_buffer *destBuffer)
{
    // Recibo el size del buffer
    ssize_t msgBytes = recv(fromSocket, &(destBuffer->size), sizeof(destBuffer->size), 0);
    
    // Chequeo que el size del buffer se haya recibido correctamente
    if (msgBytes == -1) {
        printf("\e[0;31mstream_recv_buffer: Error en la recepción del buffer [%s]\e[0m\n", strerror(errno));
    } 
    else if (destBuffer->size > 0) {
        // Recibo el stream del buffer
        destBuffer->stream = malloc(destBuffer->size);
        recv(fromSocket, destBuffer->stream, destBuffer->size, 0);
    }

    return;
}

// Utilizada para recibir un mensaje con solo codigo de operacion
// Y limpiar el buffer de recv
void stream_recv_empty_buffer(int fromSocket) 
{
    t_buffer *buffer = buffer_create();
    stream_recv_buffer(fromSocket, buffer);
    buffer_destroy(buffer);

    return;
}

uint8_t stream_recv_header(int fromSocket) 
{
    uint8_t header;
    ssize_t msgBytes = recv(fromSocket, &header, sizeof(header), 0);
    
    // Chequeo que se haya recibido el header o codigo de operacion correctamente
    if (msgBytes == -1) {
        printf("\e[0;31mstream_recv_buffer: Error en la recepción del header [%s]\e[0m\n", strerror(errno));
    }
    
    return header;
}