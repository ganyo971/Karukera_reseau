/////////////////////////////////////////////////////////////////////////////////////////
//BORDEAUX INP ENSEIRB-MATMECA
//DEPARTEMENT TELECOM
//RE216 PROGRAMMATION RESEAUX
//{daniel.negru,joachim.bruneau_-_queyreix,nicolas.herbaut}@ipb.fr
////////////////////////////////////////////////////////////////////////////////////////

#include <resolv.h>
#include <string.h>
#include <sys/select.h>
#include <stdlib.h>
#include "../commons/network.h"
#include "server.h"

int server_socket_fd;




/*void main(int argc, char** argv) {

	if (argc != 2) {
		fprintf(stderr, "plese specify local port to bind to\n");
		exit(-1);
	}*/

	


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
    sockfd = do_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


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
        int lu = do_read(new_socks, buffer);            

        
        if (strcmp(buffer,"/quit\n") == 0)  {

            fprintf(stdout, "[CLIENT] : %s", buffer);
            do_write(new_socks, "You will be terminated\n");
            break;
        }

        else    {
            
            fprintf(stdout, "[CLIENT] : %s", buffer);

            //we write back to the client
            int e = do_write(new_socks, buffer);            
        }
        
    }


    //clean up client socket
    close(new_socks);

    //clean up server socket
    close(sockfd);

    return 0;
}


/*

    //specify the socket to be a server socket and listen for at most 20 concurrent client
	

	//init the fdset

	//in an infinite loop, listen to the stored sockets
	for (;;) {

		//use select to listen to sockets

		//implement the listening for incoming sockets

		// then implement the listening for already connected socket that write data to the server

	}

//clean up server socket

}

           */
