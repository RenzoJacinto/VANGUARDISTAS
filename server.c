// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#define PORT 8080

typedef struct nodo {
	char data[50];
	char x[50];
} nodo_t;

int main(int argc, char const *argv[]){
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char *hello = "Hello from server";
	       
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	       
	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		perror("setsockopt");
	exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	       
	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	int i = 0;
	int sockets[2];   
	while(i<2){
		if (listen(server_fd, 2) < 0){
			perror("listen");
			exit(EXIT_FAILURE);
		    }
		if ((sockets[i] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
			perror("accept");
			exit(EXIT_FAILURE);
		} else{
			printf("Cliente %d aceptado\n", i);
			i++;
		}
		usleep(1000);
	}
	nodo_t* a = (nodo_t*)malloc(sizeof(nodo_t));
	nodo_t* b = (nodo_t*)malloc(sizeof(nodo_t));
	printf("alloc hecho\n");
	
	for(int j = 0 ; j<2 ; j++){
		if (j == 0 ){
			valread = recv( sockets[j] , a, sizeof(nodo_t), 0);
			if(a->data){
			printf("asd a\n");
			printf("%s del socket: %d\n",a->data, sockets[j]);
		    	send(sockets[j] , hello , strlen(hello) , 0 );
			printf("Hello message sent\n");
			}
		}
		
		else {
			valread = recv( sockets[j] , b, sizeof(nodo_t), 0);
			if(b->x) {
			printf("asd b\n");
			printf("%s del socket: %d\n",b->x, sockets[j]);
		    	send(sockets[j] , hello , strlen(hello) , 0 );
			printf("Hello message sent\n");
			}	
		}
	}
	free(a);
	free(b);
	return 0;
}

