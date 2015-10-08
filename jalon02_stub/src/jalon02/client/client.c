/////////////////////////////////////////////////////////////////////////////////////////
//BORDEAUX INP ENSEIRB-MATMECA
//DEPARTEMENT TELECOM
//RE216 PROGRAMMATION RESEAUX
//{daniel.negru,joachim.bruneau_-_queyreix,nicolas.herbaut}@ipb.fr
////////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/select.h>
#include <resolv.h>
#include <string.h>
#include <sys/select.h>
#include <stdlib.h>
#include "../commons/network.h"
#include "server.h"
#include "client.h"
#include "../commons/network.h"
#include "../commons/contrib.h"


/*
void error(const char *msg)
{
    perror(msg);
    exit(1);
}*/



/*char * readline(size_t maxlen)      {
    int offset = 0;
    do {
        offset = read(fd, str + offset, maxlen - offset);
            if (offset == -1){
                printf("Message non reçu");
                exit(EXIT_FAILURE);
            }
        } 
    while (offset != maxlen);

    return offset;
}*/

int main(int argc,char** argv)
{


    if (argc != 3)
    {
        fprintf(stderr,"usage: RE216_CLIENT hostname port\n");
        return 1;
    }

// File descriptor set for the select
    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;
    struct timeval timeout;

//get address info from the server
    struct addrinfo addr;
    struct addrinfo * add = &addr;
    struct addrinfo ** res = &add;
    int sockfd;

    get_addr_info(argv[2], argv[1], res);
    add = *res;
    addr= *add;

    //get the socket
    sockfd = do_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //connect to remote socket
    do_connect(sockfd,addr.ai_addr, addr.ai_addrlen);

    for(;;){


        FD_SET(sockfd, &writefds);

        char buffer[256];
        memset(&buffer, 0, 256);
        fprintf (stdout,"Tapez le message à envoyer (moins de 256 caractères) : \n");
        fgets((char *) buffer, 256, stdin);
        write(sockfd, buffer, strlen((char *) buffer));

        //handle_client_message(sockfd, &buffer, (int) 256);

        FD_SET(sockfd, &readfds);

        memset(&buffer, 0, (int) 256);
        do_read(sockfd, buffer);
        fprintf(stdout, "[SERVER] : %s", buffer);
        if(strcmp(buffer, "You will be terminated\n") == 0)     {
            fprintf(stdout, "Connection terminated\n");
            break;
        }
        if (strcmp(buffer, "Server cannot accept incoming connections anymore. Try again later.\n")==0)
            break;

   
    }

    close(sockfd);


    return 0;


}

/*
	//in an infinite loop, use select to listen to open sockets
	for (;;) {

	}

}           */

