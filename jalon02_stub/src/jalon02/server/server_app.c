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




void main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "plese specify local port to bind to\n");
        exit(-1);
    }

    int server_sock;
    int max_fd;
    struct sockaddr_in * serv_addr = malloc(sizeof(*serv_addr));
    struct sockaddr_in * addr = malloc(sizeof(*addr));

<<<<<<< HEAD
    char buffer[256];
    

    int users[15];
    int most_recent_user = 0;
=======
    int max(int a, int b)   {

        int max = a>b ? a:b;
        return max;
    }
>>>>>>> c906d537a44190c676414d14219c487c3ddd37b9

    fd_set rdfs;
    fd_set rdfs_utd;

<<<<<<< HEAD
    //init a fresh socket
    server_sock = do_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    max_fd = server_sock;
        
    //init server address structure
=======
    //create the socket, check for validity!
    sockfd = do_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    //init the serv_add structure
>>>>>>> c906d537a44190c676414d14219c487c3ddd37b9
    init_serv_addr(argv[1],serv_addr);

    //perform the binding
    do_bind(server_sock, serv_addr, sizeof(*serv_addr));

    //specify the socket to be a server socket and listen for at most 20 concurrent client
<<<<<<< HEAD
    if(listen(server_sock, 20) == -1)
        error("ERROR Listen");
    

    //init the fdset
    FD_ZERO(&rdfs);
    FD_SET(server_sock, &rdfs);
    rdfs_utd = rdfs;

    //in an infinite loop, listen to the stored sockets
    for (;;) {
        //use select to listen to sockets
        if(select(max_fd+1, &rdfs, NULL, NULL, NULL) == -1){
            error("ERROR Server Select");
        }

        int current_fd;
        for(current_fd = 3; current_fd <=max_fd; current_fd++){
            // Implement the listening for incoming sockets
            if(current_fd == server_sock){ // Activité sur socket Serveur = Nouveau client
                if(FD_ISSET(server_sock, &rdfs)){
                    int new_sock = do_accept(server_sock, (struct sockaddr *) addr, (socklen_t) sizeof(addr));
                    if (++most_recent_user > 2){
                        do_write(new_sock, "SATURATION : we can't accept further communication. Try later\n");
                        fprintf(stdout, "Attempt of connection but saturation\n");
                        close(new_sock);
                        //on ferme tout et on envoie un message de saturation du serveur
                    }
                    // on rajoute une nouvelle socket pour un nouvel utilisateurs
                    FD_SET(new_sock, &rdfs_utd);
                    users[most_recent_user] = new_sock;
                    //Mettre à jour le plus grand descripteur à considérer
                    max_fd = new_sock > max_fd ? new_sock : max_fd;
                    // Nettoyer le buffer
                    memset(buffer, 0, 256);
                    // Read what the client have to say
                    int lu = read(new_sock, buffer, (int) 256);
                    //Test the value of the message
                    if (strcmp(buffer,"/quit\n") == 0){
                        fprintf(stdout, "[CLIENT] : %s", buffer);
                        do_write(new_sock, "You will be terminated\n");
                    }
                    // Display on the server monitor who is saying what
                    fprintf(stdout, "[CLIENT n° %i] : %s", new_sock-3, buffer);
                    // we write back to the client
                    do_write(new_sock, buffer);
                }
            }
            // then implement the listening for already connected socket that write data to the server
            else{
                if(FD_ISSET(current_fd, &rdfs)){
                     // Nettoyer le buffer
                    memset(buffer, 0, 256);
                    // Read what the client have to say
                    int lu = read(current_fd, buffer, (int) 256);
                    // Display on the server monitor who is saying what
                    fprintf(stdout, "[CLIENT n° %i] : %s",(current_fd-3), buffer);
                    // we write back to the client
                    do_write(current_fd, buffer);
                }
            }
        }
        // Mettre à jour le fd d'activité
        rdfs = rdfs_utd;
    }

=======
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
       
        fd_set* p = &rd_select ;
        *p = readfds;
        int sel = select(nfds + 1, &rd_select, NULL, NULL, NULL);        

        if (sel == -1)     {
            perror("select()");
            exit(EXIT_FAILURE);
        }

    //implement the listening for incoming sockets

         if (FD_ISSET(sockfd, &rd_select)) {
            
            if (nb_client >= 2)    {                
                int sock_tmp = do_accept(sockfd, (struct sockaddr *) addr, (socklen_t) sizeof(addr));                
                char* tmp = "Server cannot accept incoming connections anymore. Try again later.\n";                
                do_write(sock_tmp, tmp);
                close(sock_tmp);                
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
  
        int j;
        for (j = 0; j < nb_client; j++)     {

            if (FD_ISSET(socks[j], &rd_select))     {
            //read what the client has to say                
                char buffer[256];
                memset(buffer, 0, 256);
                do_read(socks[j], buffer);
                fprintf(stdout, "[CLIENT n°%i] : %s", j+1, buffer);        
                
                if (strcmp(buffer,"/quit\n") == 0)  {                    
                    do_write(socks[j], "You will be terminated\n");                    
                    close(socks[j]);
                    nb_client--;
                                        
                }

                else    {
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
>>>>>>> c906d537a44190c676414d14219c487c3ddd37b9
}

//clean up server socket

