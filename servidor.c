#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

//Variables del servidor
int sockfd, portno;	
struct sockaddr_in serv_addr;

//Variables de cliente a conectar
int newsockfd;
socklen_t clilen;
struct sockaddr_in cli_addr;

void error(const char *msg){
	perror(msg);
	exit(1);
}

void conectarCliente(){
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if(newsockfd < 0)
		error("ERROR on accept");
}

void escribirCliente(){
	char buffer[256];
	int n;

	bzero(buffer, 256);
	n = write(newsockfd, "Igot your message", 18);
	if(n < 0)
		error("ERROR writing to socket");
}

void leerCliente(){
	char buffer[256];
	int n;

	bzero(buffer, 256);
	n = read(newsockfd, buffer, 255);
	//Error en la lectura
	if(n<0)
		error("ERROR reading from socket");
	printf("Here is the message: %s\n", buffer);
}

void cerrarServidor(){
	close(newsockfd);
	close(sockfd);
}

int main (int argc, char *argv[]){
	
	if(argc<2){
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd <0)	//Si 
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);	
	
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	
	listen(sockfd, 5);

	printf("Habla claro");

	conectarCliente();

	while(1){
		leerCliente();
		escribirCliente();
	}
	
	return 0;
}
