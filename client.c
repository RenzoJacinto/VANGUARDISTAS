
// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080

typedef struct nodo {
	char data[50];
	char x[50];
} nodo_t;
   
int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    nodo_t* nodo = (nodo_t*)malloc(sizeof(nodo_t));
    nodo->data[0] = 0;
    strncat(nodo->data, "Hola ", 49);
    nodo->x[0] = 0;
    strncat(nodo->x, "threadsss ", 49);
    send(sock , nodo , sizeof(nodo_t) , 0 );
    printf("Node sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    free(nodo);
    return 0;
}

127.0.0.1

xterm -T fase2 -e /usr/bin/cb_console_runner LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. /home/agustin/Desktop/taller/fase2/bin/Debug/TP 8080
