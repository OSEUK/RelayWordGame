#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "error.h"
#include "sendBySocket.h"

void sendBySocket(int client_socket, const char *message){
        if(send(client_socket, message, strlen(message) + 1, 0) == -1)
                err_handling("send");
}
