#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int socket_descriptor, new_socket_descriptor, port_number;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;

     if (argc < 2) {fprintf(stderr,"ERROR, no port!!\n");exit(1);}

     socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

     if (socket_descriptor < 0){error("ERROR opening socket");exit(1);}

     bzero(&serv_addr,sizeof(serv_addr));

     port_number = atoi(argv[1]);

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(port_number);

     if (bind(socket_descriptor, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR on binding");

     listen(socket_descriptor,1);


     clilen = sizeof(cli_addr);

     new_socket_descriptor = accept(socket_descriptor, (struct sockaddr *) &cli_addr, &clilen);

     if (new_socket_descriptor < 0) error("ERROR on accept");

     bzero(buffer,256);

     n = read(new_socket_descriptor,buffer,255);

     if (n < 0) error("ERROR reading from socket");

     printf("Here is the message: %s\n",buffer);

     n = write(new_socket_descriptor,"I got your message",18);

     if (n < 0) error("ERROR writing to socket");

     close(new_socket_descriptor);
     close(socket_descriptor);
     return 0;
}
