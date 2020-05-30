#include "Server.h"

Server::Server(){}

int Server::inicializar(int* args){
    int server_socket;
    int client_socket;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int client_addrlen;

    printf("Arguments: 1) port: %d \n", args[1]);

    // Create Socket
    // int socket(int domain, int type, int protocol);
    // Domain: AF_INET (IPv4 Internet protocols)
    // Type: SOCK_STREAM (Provides  sequenced,  reliable, two-way connection-based byte streams.)
    // Protocol: 0 (chosen automatically)
    server_socket = socket(AF_INET , SOCK_STREAM , 0);
    if (server_socket == -1)
    {
        perror("Could not create socket");
        return 1;
    }
    printf("Socket created\n");
    //------------------------

    // Prepare the sockaddr_in structure
    /* Structures for handling internet addresses
    struct sockaddr_in {
        short            sin_family;   // e.g. AF_INET
        unsigned short   sin_port;     // e.g. htons(3490)
        struct in_addr   sin_addr;     // see struct in_addr, below
        char             sin_zero[8];  // zero this if you want to
    };
    struct in_addr {
        unsigned long s_addr;  // load with inet_aton()
    }; */
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; //Address to accept any incoming messages. INADDR_ANY accepts any.
    server_addr.sin_port = htons(args[1]); // The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.
    //------------------------

    // Bind
    // int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    // sockfd -> file descriptor that refers to a socket
    // addr -> pointer to sockaddr_in structure of the SERVER
    // addrlen -> size of the sockaddr_in structure
    // bind() assigns the address specified by addr to the socket referred to by the file descriptor sockfd.
    if( bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed. Error");
        return 1;
    }
    printf("Bind done\n");
    //------------------------

    // Listen
    // int listen(int sockfd, int backlog);
    // sockfd -> file descriptor that refers to a socket,in this case of type SOCK_STREAM
    // backlog-> The backlog argument defines the maximum length to which the queue of pending connections for sockfd may grow.
    // listen() marks the socket referred to by sockfd as a passive socket, that is, as a socket that will be used to accept incoming connection requests using accept();
    if (listen(server_socket , 3) < 0)
    {
        perror("Listen failed. Error");
        return 1;
    }
    printf("Listening on port: %d Waiting for incoming connections...\n", args[1]);
    //------------------------

    // Accept incoming connection from a client
    // int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    // sockfd -> socket that has been created with socket(), bound to a local address with bind(), and is listening for connections after a listen()
    // addr -> pointer to a sockaddr structure for the CLIENT.
    // addrlen -> size of sockaddr structure for the CLIENT.
    client_socket = accept(server_socket, (struct sockaddr *) &client_addr, (socklen_t*) &client_addrlen);
    if (client_socket < 0)
    {
        perror("Accept failed");
        return 1;
    }
    printf("Connection accepted\n\n");


    struct Command client_command;
    struct View client_view;

    int commands_count = 0;
    int status = 0;
    initializeData(&client_view);

    //keep communicating with client
    while (commands_count < 20)
    {
        printf("Commands count: %d\n", commands_count + 1);

        // Receive data (command)
        if (receiveData(&client_socket, &client_command) < 0){
            perror("Receive Data Error");
            status = -1;
        }
        printf("Incomming command action: %d\n", client_command.action);
        //--------------------

        // Process model
        processData(client_command.action, &client_view);
        //--------------------

        // Send data (view)
        if (sendData(&client_socket, &client_view) < 0){
            perror("Send Data Error");
            status = -1;
        }
        printf("Send data: pos(X,Y) = (%d,%d)\n\n", client_view.positionX, client_view.positionY);
        //--------------------

        commands_count++;
    }

    close(client_socket);
    printf("Client socket number %d closed\n",client_socket);
    close(server_socket);
    printf("Server socket number %d closed\n",server_socket);

    return status;
}

int Server::sendData(int* client_socket, struct View* client_view){

    int total_bytes_written = 0;
    int bytes_written = 0;
    int send_data_size = sizeof(struct View);
    bool client_socket_still_open = true;

    // Send
    // ssize_t send(int sockfd, const void *buf, size_t len, int flags);
    // sockfd -> file descriptor that refers to a socket
    // buf ->  the message is found in buf.
    // len -> the message and has length len
    // flags
    // The system call send() is used to transmit a message to another socket.
    // The send() call may be used only when the socket is in a connected state (so that the intended recipient is known).

    while ((send_data_size > total_bytes_written) && client_socket_still_open){
        bytes_written = send(*client_socket, (client_view + total_bytes_written), (send_data_size - total_bytes_written), MSG_NOSIGNAL);
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

int Server::receiveData(int* client_socket, struct Command* client_command){

    int total_bytes_receive = 0;
    int bytes_receive = 0;
    int receive_data_size = sizeof(struct Command);
    bool client_socket_still_open = true;

    // Receive
    // ssize_t recv(int sockfd, void *buf, size_t len, int flags);
    // sockfd -> file descriptor that refers to a socket
    // buf -> where the received message into the buffer buf.
    // len -> The caller must specify the size of the buffer in len.
    // flags
    // The recv() call are used to receive messages from a socket.
    // If no messages are available at the socket, the receive call wait for a message to arrive. (Blocking)

    while ((receive_data_size > bytes_receive) && client_socket_still_open) {
        bytes_receive = recv(*client_socket, (client_command + total_bytes_receive), (receive_data_size - total_bytes_receive), MSG_NOSIGNAL);
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

void Server::processData(int action, struct View* view){
    switch ( action )
    {
        case 1:
            (*view).positionY += 1;
            break;
        case 2:
            (*view).positionY -= 1;
            break;
        case 3:
            (*view).positionX += 1;
            break;
        case 4:
            (*view).positionX -= 1;
            break;
    }
}

void Server::initializeData(struct View* client_view){
    (*client_view).positionX = 0;
    (*client_view).positionY = 0;
}
