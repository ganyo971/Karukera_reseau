/////////////////////////////////////////////////////////////////////////////////////////
//BORDEAUX INP ENSEIRB-MATMECA
//DEPARTEMENT TELECOM
//RE216 PROGRAMMATION RESEAUX
//{daniel.negru,joachim.bruneau_-_queyreix,nicolas.herbaut}@ipb.fr
////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <regex.h>
#include <resolv.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdbool.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>
#include "user.h"
#include "callbacks.h"
#include "colors.h"
#include "network.h"

void error(const char* msg) {
	perror(msg);
	exit(-1);
}

/**************************************************
 * In this file, you should implement every code that
 * can be used by both client and server
 */

int do_socket(int family, int type, int protocol){

    int sockfd;
    sockfd = socket(family, type, protocol);
    if (sockfd == -1){
        error("ERROR socket");
    }
    return sockfd;
}

void do_connect(int sock, const struct sockaddr* addr, socklen_t addr_len){
    int c;
    c = connect(sock, addr, addr_len);
    if (c != 0) {
        perror("Erreur client connect : ");
    }
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

int do_read(const int sock, char buffer[256]){

    int lu = read(sock, buffer, (int) 256);    
    return lu;
}

int do_write(const int sock, const char buffer[256]){
   
    int offset = 0;      
    int i;

    for (i = 0; i < 256; i++) {

       if (buffer [i] == '\n')
        break;                                          
    }

    int maxlen2 = i+1;

    while(offset < maxlen2){
    	maxlen2 = maxlen2 - offset;
    	buffer = buffer + offset;

        offset = write(sock, buffer, maxlen2);

        if (offset == -1)   {
            fprintf(stderr,"Message non envoyé\n");
            exit(EXIT_FAILURE);            
        } 
              
    }

    return offset;
}
