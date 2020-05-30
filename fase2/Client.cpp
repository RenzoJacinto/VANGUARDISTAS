#include "Client.h"

Client::Client(){}

int Client::inicializar(int* args){
    int client_socket;
    struct sockaddr_in server;
    int status = 0;
    struct Command client_command;
    struct View client_view;

    char ip[10];
    char puerto[10];

    sprintf(ip, "%d", args[1]);
    sprintf(puerto, "%d", args[2]);

    printf("Arguments: 1) ip: %s ,2) port: %s \n",ip,puerto);

    // Create Socket
    // int socket(int domain, int type, int protocol);
    // Domain: AF_INET (IPv4 Internet protocols)
    // Type: SOCK_STREAM (Provides  sequenced,  reliable, two-way connection-based byte streams.)
    // Protocol: 0 (chosen automatically)
    client_socket = socket(AF_INET , SOCK_STREAM , 0);
    if (client_socket == -1)
    {
        perror("Could not create socket");
        return 1;
    }
    printf("Socket created\n");
    //------------------------

    // Prepare the sockaddr_in structure
    server.sin_addr.s_addr = inet_addr((const char*)ip);
    server.sin_family = AF_INET;
    server.sin_port = htons( args[2] );
    //------------------------

    //Connect to remote server
    // int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    // sockfd -> file descriptor that refers to a socket
    // addr -> pointer to sockaddr_in structure for the SERVER.
    // addrlen -> size of sockaddr_in structure for the SERVER.
    // The connect() system call connects the socket referred to by the file descriptor sockfd to the address specified by addr.
    if (connect(client_socket , (struct sockaddr *)&server , sizeof(struct sockaddr_in)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    printf("Connected\n\n");
    //------------------------

    // data to send
    int commands [20]={1,4,2,3,1,4,2,3,2,4,1,4,2,3,2,4,1,3,2,4};

    //keep communicating with server
    for(int i=0; i<20; i++)
    {
        // Set data to send
        client_command.action = commands[i];
        //--------------------
        printf("Commands count: %d\n", i + 1);

        // Send data (command)
        if (sendData(&client_socket, &client_command) < 0){
            perror("Send Data Error");
            status = -1;
        }
        printf("Send data: action = %d\n", client_command.action);
        //--------------------

        // Receive data (view)
        if (receiveData(&client_socket, &client_view) < 0){
            perror("Receive Data Error");
            status = -1;
        }
        printf("Incomming data: pos(X,Y) = (%d,%d)\n\n", client_view.positionX, client_view.positionY);
        //--------------------
    }

    close(client_socket);
    printf("Socket number %d closed\n", client_socket);
    return 0;
}

int Client::sendData(int* client_socket, struct Command* client_command){

    int total_bytes_written = 0;
    int bytes_written = 0;
    int send_data_size = sizeof(struct Command);
    bool client_socket_still_open = true;

    while ((send_data_size > total_bytes_written) && client_socket_still_open){
        bytes_written = send(*client_socket, (client_command + total_bytes_written), (send_data_size-total_bytes_written), MSG_NOSIGNAL);

        if (bytes_written < 0) { // Error
            return bytes_written;
        }
        else if (bytes_written == 0) { // Socket closed
            client_socket_still_open = false;
        }
        else {
            total_bytes_written += bytes_written;
        }
    }

    return 0;
}

int Client::receiveData(int* client_socket, struct View* client_view){

    int total_bytes_receive = 0;
    int bytes_receive = 0;
    int receive_data_size = sizeof(struct View);
    bool client_socket_still_open = true;

    while ((receive_data_size > bytes_receive) && client_socket_still_open) {
        bytes_receive = recv(*client_socket, (client_view + total_bytes_receive), (receive_data_size - total_bytes_receive), MSG_NOSIGNAL);

        if (bytes_receive < 0) { // Error
            return bytes_receive;
        }
        else if (bytes_receive == 0) { // Socket closed
            client_socket_still_open = false;
        }
        else {
            total_bytes_receive += bytes_receive;
        }
    }

    return 0;
}
