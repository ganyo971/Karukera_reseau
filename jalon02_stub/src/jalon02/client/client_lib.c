#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include "network.h"
#include "client.h"


void get_addr_info(const char* address, const char* port, struct addrinfo** res) {

    int status;
    struct addrinfo hints, p;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(address, port, &hints, res);

    if (status != 0) {
        perror("Erreur client address info  ");
    }
}

void handle_client_message(int socket, char buffer[256]){        //handle_client_message(int sock, void * buffer, int maxlen) 
    memset(buffer, 0, 256);
    printf ("Tapez le message à envoyer (moins de 256 caractères) : \n");
    fgets((char *) buffer,256, stdin);
    
    int offset = 0;
    int maxlen2 = strlen((char *) buffer);

    while(offset < maxlen2){
        maxlen2 = maxlen2 - offset;
        offset = write(socket, buffer + offset, maxlen2);  //modification car il faut vérifier à chaque fois si write a bien envoyé
        if (offset == -1){
            fprintf(stderr,"Message non envoyé\n");
            exit(EXIT_FAILURE);            
        }
    } 
}


void handle_server_message(char buffer[256]) {
}

void handle_file_decline(const char* addr, const char* port) {

}
void handle_file_receive(const char *file_name, const char* addr,
		const char* port) {

}

void handle_send_file(const int socket, const char* file_path) {

}
