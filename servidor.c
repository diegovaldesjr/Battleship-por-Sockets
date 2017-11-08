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
	printf("Player 2 conectado.\n");
}

void escribirCliente(char serializado[4]){
	int n;
	n = write(newsockfd, serializado, sizeof(serializado));
	if(n < 0)
		error("ERROR writing to socket");
	printf("Jugada enviada.\n");
}

void leerCliente(char serializado[4]){
	int n;

	n = read(newsockfd, serializado, sizeof(serializado));
	//Error en la lectura
	if(n<0)
		error("ERROR reading from socket");
	printf("Jugada recibida\n");
}

void cerrarServidor(){
	close(newsockfd);
	close(sockfd);
}

/*Funciones del juego*/
struct Mensaje {
  char msg;
  int fila;
  int columna;
  char simbolo;
};

typedef struct Mensaje mensaje;

//W:Ganaste, I:Inicia tu tablero, L:Listo, G: Golpe, O: Acertaste, F: Fallaste, T: Tu turno
mensaje deserializar (mensaje estructura, char serializado[4]){
    estructura.msg = serializado[0];
    estructura.fila = (int)(serializado[1] - '0');
    estructura.columna = (int)(serializado[2] - '0');
    estructura.simbolo = serializado[3];
    return estructura;
}

void serializar (mensaje estructura , char serializado[4]){
  char s;
    serializado[0]=estructura.msg;
    s = '0' + estructura.fila;
    serializado[1] = s;
    s = '0' + estructura.columna;
    serializado[2] = s;
    serializado[3] = estructura.simbolo;
    return;
    
} 

int comprobador(int fila, int columna,int sentido,int fragata,char matriz [10][10]){
    int i,j;
    int barco;
    
    
    if(sentido>4 || fila>9 || columna>9 || fila<0 || columna<0)
        return -2;
    
    switch (fragata){
        case 1: barco = 5; break;
        case 2: barco = 4; break;
        case 3: barco = 3; break; 
        case 4: barco = 3; break; 
        case 5: barco = 2; break; 
    }
    switch (sentido){
        case 1:
            if (columna-barco < -1 ){
                return -1;
            }else{
                for(i=columna;i!=columna-barco;i--){
                    if(matriz[fila][i] != '*')
                        return -1;
                }
            }
            return 0;
            
        case 2:
            if (fila-barco < -1){
                int h=fila-barco;
                printf("Esto  es el resultado %d\n",h);
                return -1;
            }else{
                for(i=fila;i!=fila-barco;i--){
                    if(matriz[i][columna] != '*')
                        return -1;
                }
            }
            return 0;
        case 3: 
            if(columna+barco> 10 ){ 
                int g =fila+barco;
                printf("Este es el resultado de la derecha: %d\n",g);
                return -1;
            }else{
                for(i=columna;i!=columna+barco;i++){
                    if(matriz[fila][i] != '*')
                        return -1;
                }
            }
            return 0;
        case 4:
            if (fila+barco > 10){
                int f =fila+barco;
                printf("Este es el resultado de abajo: %d\n",f);
                return -1;
            }else{
                for(i=fila;i!=fila+barco;i++){
                    if(matriz[i][columna] != '*'){
                        return -1;
                    }        
                }
            }
            return 0;
    } 
   
    
}

int colocar (int fila, int columna,int sentido,int fragata,char matriz [10][10]){
    int i,j;
    int barco;
    char simbolo;
    switch (fragata){
        
        case 1: barco = 5; simbolo='P'; break;
        case 2: barco = 4; simbolo='A'; break;
        case 3: barco = 3; simbolo='C'; break; 
        case 4: barco = 3; simbolo='S'; break; 
        case 5: barco = 2; simbolo='D'; break;
       
        
    }
    switch (sentido){
        case 1:
                for(i=columna;i!=columna-barco;i--){
                    matriz[fila][i] = simbolo;
                }
                break;
        case 2:
                for(i=fila;i!=fila-barco;i--){
                    matriz[i][columna]= simbolo;
                }
                break;
        case 3: 
                for(i=columna;i!=columna+barco;i++){
                    matriz[fila][i] = simbolo;
                }
                break;
            
        case 4:
                for(i=fila;i!=fila+barco;i++){
                    matriz[i][columna] = simbolo;
                    
                }
                break;
    }
    
    for(i=0;i<10;i++){
        for(j=0;j<10;j++){
            printf("%c",matriz[i][j]);
            printf(" ");
        }
    printf("\n");
    }
    
}

int creador (char matriz[10][10]){
  int i,fila,columna,sentido;
  

  for (i=1;i<=6;i++){
    switch(i){
        case 1: printf("Inserta el portaaviones (5 casillas de largo)\n"); break;
        case 2: printf("Inserta el acorazado (4 casillas de largo)\n"); break;
        case 3: printf("Inserta el crucero (3 casillas de largo)\n"); break;
        case 4: printf("Inserta el submarino (3 casillas de largo)\n"); break;
        case 5: printf("Inserta el destructor (2 casillas de largo)\n"); break;
    }
   
    if(i==6)
        return 0;
    
    printf("fila:");
    scanf("%d",&fila); 
    printf("columnas:");
    scanf("%d",&columna);
    printf("Fija la direccion\n"); 
    printf("1. Izquierda\n");
    printf("2. Arriba\n");
    printf("3.Derecha\n");
    printf("4.Abajo\n"); 
    scanf("%d",&sentido);
   
    
    if (comprobador(fila-1,columna-1,sentido,i,matriz)==-1){
        printf("Tu barco sobrepone a otro o se sale de los limites del tablero vuelve a intentarlo\n");
        i--;
    }
    if (comprobador(fila-1,columna-1,sentido,i,matriz)==-2){
        printf("Tuviste un error de tipeo en alguna fila, columna o sentido ¡vuelve a intentarlo!\n");
        i--;
    }
    if(comprobador(fila-1,columna-1,sentido,i,matriz)==0){
        colocar(fila-1,columna-1,sentido,i,matriz);
    }
  }
}

mensaje player1(mensaje estructura){
    static char tablero[10][10]; 
    static char acertados[10][10];
    static int  partida = 0;
    int jugada[2];
    int fila,columna,i;
    
    
    if(estructura.msg == 'I'){
    /*iniciadores de matrices*/
        for(fila=0;fila<=9; fila++){
            for(columna=0; columna<=9;columna++){
                tablero[fila][columna] = '*';
                acertados[fila][columna] = '*';
            }
        }
        creador(tablero);
        estructura.msg = 'L';
        return estructura;
     
        
    }
    if(estructura.msg == 'L'){
        estructura.msg = 'T';
        return estructura;
    }
    if(estructura.msg == 'G'){
        estructura=recibirJugada(estructura,tablero); 
        return estructura;
    }
    if(estructura.msg == 'T'){
        printf("\n");
        printf("Tu turno jugador 1 \n");
        enviarJugada(acertados,jugada);
        estructura.fila=jugada[0];
        estructura.columna=jugada[1];
        estructura.msg='G';
        return estructura;
    }
    
    if(estructura.msg == 'O'){
        acertados[estructura.fila][estructura.columna] = estructura.simbolo;
        printf("¡Acertaste!\n");
        i=verificarGanador(acertados);
        if(i==-1){
            estructura.msg='W';
            printf("El jugador 1 gano la partida!");
            return estructura;
        }    
    }
    if(estructura.msg == 'F'){
        acertados[estructura.fila][estructura.columna] = 'X';
        printf("¡Fallaste!\n");
    }
    estructura.msg = 'T';    
    return estructura;
}

int verificarGanador(char jugadas[10][10]){
    int i,j,total;
    for(i=0;i<=9;i++){
        for(j=0;j<=9;j++){
            if(jugadas[i][j] != 'X' && jugadas[i][j] != '*' )
            total++;
        }
    }
    if(total==17){
        return -1;
    }
    
return 0;
}

mensaje recibirJugada(mensaje estructura,char tablero[10][10]){
    if(tablero[estructura.fila][estructura.columna] != '*' && estructura.fila<=9 && estructura.columna<=9 &&estructura.fila>=0 && estructura.columna>0){
        estructura.simbolo=tablero[estructura.fila][estructura.columna];
        estructura.msg = 'O';
        return estructura;
    }else 
    {
        estructura.msg = 'F';
        return estructura;
    }
    
}


void enviarJugada(char acertados[10][10], int jugada[2]){
    int i,j,fila,columna;
 
    printf("Tiros acertados\n");
    for(fila=0;fila<=9; fila++){
            for(columna=0; columna<=9;columna++){
                printf("%c",acertados[fila][columna]);
            }    
    printf("\n");
    }
    printf("\n");
    printf("Inserte la fila a disparar: ");
    scanf("%d",&i);
    jugada[0]=i-1;
    printf("Inserte la columna a disparar: ");
    scanf("%d",&j);
    jugada[1]=j-1;
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

	conectarCliente();

	int ganador=0; 
	char serial[4];
	
	mensaje msg = {'I',0,0,'I'};

	while(ganador==0){
		msg=player1(msg);
		serializar(msg,serial);
		escribirCliente(serial);

		leerCliente(serial);
		msg=deserializar(msg, serial);


		if(msg.msg=='W'){
	        ganador++;
	        break;
	    }
	}

	if(ganador<0)
	    printf("el jugador 2 gano\n");
	if(ganador>0)
	    printf("El jugador 1 gano\n");
	
	return 0;
}
