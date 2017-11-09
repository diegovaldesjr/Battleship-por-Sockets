#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//Variables del socket a conectar con el servidor
int socket, portno, n;
char dirIP[18];
struct sockaddr_in servidor_addr;
struct hostent *server;

/**
*   En caso de error al conectar
*
*/
void error(const char *msg){
	perror(msg);
	exit(0);
}

/**
*   Manda mensaje al servidor (arreglo de caracteres serializado)
*
*/
void escribirServidor(char serializado[4]){
	int n;

	n = write(socket, serializado, sizeof(serializado));
	if(n<0)
		error("ERROR writing to socket");
	printf("Jugada enviada.\n");
}

/**
*   Recibe mensaje del servidor (arreglo de caracteres serializado)
*
*/
void leerServidor(char serializado[4]){
	int n;

	n = read(socket, serializado, sizeof(serializado));
	if(n<0)
		error("ERROR readign from socket");
	printf("Jugada recibida\n");
}

/**
*   Cierra conexion del socket
*
*/
void cerrarCliente(){
	close(socket);
}

/***Funciones del juego***/

/**
*   Estructura que contiene el mensaje, resultado o accion de cada jugada hecha por el jugador
*/
struct Mensaje {
  char msg;
  int fila;
  int columna;
  char simbolo;
};
typedef struct Mensaje mensaje;

/**
*   Deseariliza pasando el arreglo de caracteres a la estructura
*
*/
mensaje deserializar (mensaje estructura, char serializado[4]){
    estructura.msg = serializado[0];
    estructura.fila = (int)(serializado[1] - '0');
    estructura.columna = (int)(serializado[2] - '0');
    estructura.simbolo = serializado[3];
    return estructura;
}

/**
*   Serializa pasando la estructura un arreglo de caracteres 
*
*/
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

/**
*   Verifica que tu matriz de acertados tenga los 17 tiros acertados (necesarios para ganar la partida)
*
*/
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

/**
*   Verifica si el tiro/jugada pego a un barco del rival
*
*/
mensaje recibirJugada(mensaje estructura,char tablero[10][10]){
    if(tablero[estructura.fila][estructura.columna] != '*' && estructura.fila<=9 && estructura.columna<=9 &&estructura.fila>=0 && estructura.columna>=0){
        estructura.simbolo=tablero[estructura.fila][estructura.columna];
        estructura.msg = 'O';
        return estructura;
    }else 
    {
        estructura.msg = 'F';
        return estructura;
    }
    
}

/**
*   Solicita coordenadas en donde se realizara el disparo/jugada, 
*   y muestra graficamente los tiros acertados
*
*/
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

/**
*   Verifica que la no esta sobreponiendo un barco sobre otro 
*   y que no se coloca un barco fuera de los limites
*
*/
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

/**
*   Coloca el barco en el tablero
*
*/
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

/**
*   Gestiona la creacion del tablero
*   apoyandose con la funcion comprobador() y colocar()
*
*/
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

/**
*   Es la que gestiona todo el juego, tanto la creacion del tablero
*   verificar las jugadas hechas y finalizacion del juego
*   
*   Mensajes:
*   W:Ganaste, I:Inicia tu tablero, L:Listo, G: Golpe, O: Acertaste, F: Fallaste, T: Tu turno
*
*/
mensaje player2(mensaje estructura){
    static char tablero[10][10]; 
    static char acertados[10][10];
    int  jugada[2];
    int fila,columna,sentido,i;
  
    
    if(estructura.msg == 'L'){
        for(fila=0;fila<=9; fila++){
            for(columna=0; columna<=9;columna++){
                tablero[fila][columna] = '*';
                acertados[fila][columna]= '*';
        
            }
        }
       creador(tablero);
       estructura.msg = 'L';
       return estructura;
    }else if(estructura.msg == 'T'){
        printf("\n");
        printf("Tu turno jugador 2 \n");
        enviarJugada(acertados,jugada);
        estructura.fila=jugada[0];
        estructura.columna=jugada[1];
        estructura.msg='G';
        return estructura;
    }else if(estructura.msg == 'G'){
        estructura=recibirJugada(estructura,tablero); 
        return estructura;
    }else if(estructura.msg == 'O'){
        acertados[estructura.fila][estructura.columna] = estructura.simbolo;
        printf("¡Acertaste!");
        i=verificarGanador(acertados);
        if(i==-1){
            estructura.msg='W';
            printf("El jugador 2 gano la partida!");
            return estructura;
            
        }
    }else if(estructura.msg == 'F'){
        acertados[estructura.fila][estructura.columna] = 'X';
        printf("¡Fallaste!\n");
    }else if(estructura.msg == 'W'){
       estructura.msg='P';
       return estructura;
    }

    estructura.msg = 'T';
    return estructura;
}

/**
*   Funcion main, prende el servidor, espera al jugador e inicia el juego
*
*/
int main (int argc, char *argv[]){
	
    printf("Bienvenidos a Battleship (version cliente)\n");

    printf("Ingrese direccion ip del servidor.\n");
    scanf("%s", dirIP);

    printf("Ingrese numero de puerto.\n");
    scanf("%d", portno);

    //Crea socket para conectar con cliente y realiza la conexion
    socket = socket(AF_INET, SOCK_STREAM, 0);
	if(socket < 0)
		error("ERROR opening socket");
	server = gethostbyname(dirIP);
	if(server == NULL){
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &servidor_addr, sizeof(servidor_addr));
	servidor_addr.sin_family = AF_INET;
	
	bcopy((char *)server->h_addr, (char *)&servidor_addr.sin_addr.s_addr, server->h_length);
	
	servidor_addr.sin_port = htons(portno);
	if(connect(socket, (struct sockaddr *) &servidor_addr, sizeof(servidor_addr)) <0)
		error("ERROR connecting");

	printf("Conectado con player 1.\n");

    //Variables para empezar el juego
	int ganador=0; 
	char serial[4];
	mensaje msg;

    //Este ciclo dura mientras el juego se lleve a cabo
	while(ganador==0){
		leerServidor(serial);
		msg=deserializar(msg, serial);

		if(msg.msg=='W'){
	        ganador++;
	        break;
	    }
	    if(msg.msg == 'P'){
            ganador--;
            break;
        }

        msg=player2(msg);
        serializar(msg,serial);
        escribirServidor(serial);
	}

    //Verifica ganador y termina el juego
	if(ganador<0)
	    printf("el jugador 2 gano\n");
	if(ganador>0){
	    msg=player2(msg);
        serializar(msg,serial);
        escribirServidor(serial);
        printf("El jugador 1 gano\n");
    }

    //Cierra conexion con el servidor
    cerrarCliente();
	return 0;
}
