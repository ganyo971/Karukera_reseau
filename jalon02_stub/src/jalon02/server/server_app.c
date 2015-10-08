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

    int max(int a, int b)   {

        int max = a>b ? a:b;
        return max;
    }


    //create the socket, check for validity!
    sockfd = do_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    //init the serv_add structure
    init_serv_addr(argv[1],serv_addr);

    //perform the binding
    //we bind on the tcp port specified
    do_bind(sockfd, serv_addr, sizeof(*serv_addr));

    //specify the socket to be a server socket and listen for at most 20 concurrent client
    if(listen(sockfd, 4) == -1) 
        error("ERROR Listen, trop de connexion");

    //Tableau avec les numeros de socket client
    int socks[20];
    memset(socks, 0, 20);

    //Nombre de client connecté
    int nb_client = 0;

    //init the fdset
    int nfds = 0;
    fd_set readfds;
    fd_set rd_select;
    fd_set writefds;
    fd_set exceptfds;
    struct timeval timeout;

    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    FD_ZERO(&readfds);
    FD_ZERO(&rd_select);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);

    // Surveiller la socket du server pour la lecture 
    FD_SET(sockfd, &readfds);
    nfds = max(nfds, sockfd);
    
    //in an infinite loop, listen to the stored sockets

    for (;;){
       
        //use select to listen to sockets

        //FD_COPY(&readfds, &rd_select);

        fd_set* p = &rd_select ;

        *p = readfds;

        int sel = select(nfds + 1, &rd_select, NULL, NULL, NULL);

        //printf("Valeur de select : %d", sel);

        if (sel == -1)     {
            perror("select()");
            exit(EXIT_FAILURE);
        }

        //implement the listening for incoming sockets

         if (FD_ISSET(sockfd, &rd_select)) {
            
            if (nb_client+1 > 2)    {
                int sock_tmp = do_accept(sockfd, (struct sockaddr *) addr, (socklen_t) sizeof(addr));

                
                char* tmp = "Can not accept further connexion: try later\n";
                
                do_write(sock_tmp, tmp);
                close(sock_tmp); 
                break;
            }
        //accept connection from client
            else    {
           
                socks[nb_client] = do_accept(sockfd, (struct sockaddr *) addr, (socklen_t) sizeof(addr));            
                FD_SET(socks[nb_client], &readfds);
                nfds = max(nfds, socks[nb_client]);
                nb_client ++;
            }
        }
        
        // then implement the listening for already connected socket that write data to the server
  
        /*int new_socks;
        //accept connection from client
            new_socks = do_accept(sockfd, (struct sockaddr *) addr, (socklen_t) sizeof(addr));*/
        int j;

        for (j = 0; j < nb_client; j++)     {

            if (FD_ISSET(socks[j], &rd_select))     {

                //read what the client has to say
                
                char buffer[256];
                memset(buffer, 0, 256);
                do_read(socks[j], buffer);        
                
                if (strcmp(buffer,"/quit\n") == 0)  {

                    fprintf(stdout, "[CLIENT n°%i] : %s", j, buffer);

                    do_write(socks[j], "You will be terminated\n");
                    break;
                }

                else    {
                    
                    fprintf(stdout, "[CLIENT] : %s", buffer);

                    //we write back to the client
                    do_write(socks[j], buffer);            
                }
            }
        }
       
    } 

    int j;

    for (j = 0; j < nb_client; j++)     {  
    //clean up client socket
        close(socks[j]);
    }

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
