#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ntedb.h>

void error(const char *msg){
	perror(msg);
	exit(1);
}

//Servidor
void activarServidor(int *sockfd, int portno, struct sockaddr_in *serv_addr){
	
	*sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(*sockfd <0)	//Si 
		error("ERROR opening socket");
	bzero((char *) serv_addr, sizeof(*serv_addr));
	
	*serv_addr.sin_family = AF_INET;
	*serv_addr.sin_addr.s_addr = INADDR_ANY;
	*serv_addr.sin_port = htons(portno);
	
	if(bind(*sockfd, (struct sockaddr *) serv_addr, sizeof(*serv_addr)) < 0)
		error("ERROR on binding");
	
	listen(*sockfd, 2);
	
}

void recibirCliente(int *sockfd, int *newsockfd, socklen_t *clilen, struct sockaddr_in *cli_addr){
	
	clilen = sizeof(cli_addr);
	
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if(newsockfd < 0)
		error("ERROR on accept");
}

void imprimirMensaje(int *newsockfd){
	char buffer[256];
	int n;

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
}

void cerrarServidor(int *newsockfd, int *sockfd){
	close(*newsockfd);
	close(*sockfd);
}

char *ip_local(){
	struct sockaddr_in host;
	char nombre[255], *ip;

	gethostbyname(nombre, 255);
	host.sin_addr = *(struct in_addr*) gethostbyname(nombre)->h_addr;
	ip = inet_ntoa(host.sin_addr);

	return ip;
}

//Cliente
void conectarCliente(int portno){
	int sockfd, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	
	char buffer[256];
	
	//portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		error("ERROR opening socket");
	server = gethostbyname(ip_local());
	if(server == NULL){
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	

	serv_addr.sin_port = htons(portno);
	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) <0)
		error("ERROR connecting");
	printf("Please enter the message: ");
	bzero(buffer, 256);
	fgets(buffer, 255, stdin);
	n = write(sockfd, buffer, strlen(buffer));
	if(n<0)
		error("ERROR writing to socket");
	bzero(buffer, 256);
	n = read(sockfd, buffer, 255);
	if(n<0)
		error("ERROR readign from socket");
	printf("%s\n", buffer);
	close(sockfd);
}

void cerrarCliente(int *sockfd){
	close(*sockfd);
}

int main (int argc, char *argv[]){

	// if(argc<2){
	// 	fprintf(stderr,"ERROR, no port provided\n");
	// 	exit(1);
	// }

	//Variables del servidor
	int sockfd, portno=8000;	
	struct sockaddr_in serv_addr;

	activarServidor(&sockfd, portno, &serv_addr);

	printf("Active el beta\n");

	int newsockfd;	
	socklen_t clilen;
	struct sockaddr_in cli_addr;

	recibirCliente(&sockfd, &newsockfd, &clilen, &cli_addr);	

	//Variables del cliente


	conectarCliente(portno);

	imprimirMensaje(&newsockfd);


	cerrarServidor(&newsockfd, &sockfd);
	// while(1){
	// 	imprimirMensaje(&newsockfd);
	// }
	
	return 0;
}
