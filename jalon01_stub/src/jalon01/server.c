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

int do_socket(){
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == -1)
        error("ERROR socket");
    return sockfd;
}

void init_serv_addr(const char* port, struct sockaddr_in *serv_addr){
    int portno;

    memset(serv_addr, 0, sizeof(*serv_addr));

    portno = atoi(port);

    serv_addr->sin_family = AF_INET;
    serv_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr->sin_port = htons(portno);
}

void do_bind(int sock, const struct sockaddr_in *serv_addr, socklen_t serv_addr_len){
    if (bind(sock, (struct sockaddr *) serv_addr, serv_addr_len) == -1)
        error("ERROR Bind");
}

int do_accept(int sock, struct sockaddr * addr, socklen_t addr_len){
    int new_sock;
    new_sock = accept(sock, addr, &addr_len);
    if(new_sock == -1)
        error("ERROR accept");
    return new_sock;
}

void do_read(int sock, char * buffer){
    int lu = read(socket, buffer, (int) 256);
}

void do_write(const int sock, const char * buffer){
   // ssize_t rep = write(sock, buffer, strlen(buffer));

    int offset = 0;
    int maxlen2 = strlen((char *) buffer);

    while(offset < maxlen2){
        offset += write(sock, buffer + offset, maxlen2 - offset);
        if (offset == -1)   {
            fprintf(stderr,"Message non envoyé\n");
            exit(EXIT_FAILURE);            
        }    
    }
}

int main(int argc, char** argv)
{

    if (argc != 2)
    {
        fprintf(stderr, "usage: RE216_SERVER port\n");
        return 1;
    }

    int sockfd;
    struct sockaddr_in * serv_addr = malloc(sizeof(*serv_addr));
    struct sockaddr_in * addr = malloc(sizeof(*addr));


    //create the socket, check for validity!
    sockfd = do_socket();


    //init the serv_add structure
    init_serv_addr(argv[1],serv_addr);

    //perform the binding
    //we bind on the tcp port specified
    do_bind(sockfd, serv_addr, sizeof(*serv_addr));

    //specify the socket to be a server socket and listen for at most 20 concurrent client
    if(listen(sockfd, 20) == -1)
        error("ERROR Listen");

    int new_socks;
    //accept connection from client
    new_socks = do_accept(sockfd, (struct sockaddr *) addr, (socklen_t) sizeof(addr));

    for (;;){
        //read what the client has to say
        char buffer[256];
        memset(buffer, 0, 256);

        //do_read(new_socks, buffer);
        int lu = read(new_socks, buffer, (int) 256);
        if (strcmp(buffer,"quit\n") == 0){
            fprintf(stdout, "CLIENT --> %s : connexion terminée\n", buffer);
            do_write(new_socks, "Connexion terminée\n");
            break;
        }
        else{
            fprintf(stdout, "CLIENT --> %s", buffer);

            //we write back to the client
            do_write(new_socks, buffer);   
        }
        
    }


    //clean up client socket
    close(new_socks);

    //clean up server socket
    close(sockfd);

    return 0;
}