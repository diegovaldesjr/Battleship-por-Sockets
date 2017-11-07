#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg){
	perror(msg);
	exit(1);
}

void servidor(){
	int sockfd, newsockfd, portno=8000;	
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	
	//el primer parametro es la familia del protocolo. el segundo indica que es un socket tcp
	//Crear sockets, el 0 es parametro de control
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd <0)	//Si 
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));	//Poner los bits en 0 hasta el tamaño de serv_addr
	//portno = atoi(argv[1]);	//Convertimos el parametro que esta en string y lo convertimos en int
	
	serv_addr.sin_family = AF_INET;	//familia
	serv_addr.sin_addr.s_addr = INADDR_ANY;	//Escucha cualquier direccion
	serv_addr.sin_port = htons(portno);	//puerto standar, las redes se comunican en big endain, htons transforma el puerto a big endain
	
	//Se hace bind del puerto
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	
	//Bloquea el servidor en espera de mensaje
	//El 2 representa la cantidad de conexiones simultaneas
	listen(sockfd, 2);
	
	//
	clilen = sizeof(cli_addr);
	//El resultado del accept es el nuevo socket creadp àra comunicar con el cliente
	//cli_addr guarda los datos del cliente
	//Este socket que se usara para lecturas/escrituras sera este
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if(newsockfd < 0)
		error("ERROR on accept");
	//
	bzero(buffer, 256);
	//Leer 
	n = read(newsockfd, buffer, 255);
	//Error en la lectura
	if(n<0)
		error("ERROR reading from socket");
	printf("Here is the message: %s\n", buffer);
	n = write(newsockfd, "Igot your message", 18);
	if(n < 0)
		error("ERROR writing to socket");
	close(newsockfd);
	close(sockfd);
}

int main (int argc, char *argv[]){

	// if(argc<2){
	// 	fprintf(stderr,"ERROR, no port provided\n");
	// 	exit(1);
	// }

	servidor();	
	return 0;
}
