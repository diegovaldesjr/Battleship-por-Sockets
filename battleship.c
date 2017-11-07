#include <stdio.h>
#include <stdlib.h>


struct Mensaje {
  char msg;
  int fila;
  int columna;
  char simbolo;
};

typedef struct Mensaje mensaje;

//W:Ganaste, I:Inicia tu tablero, L:Listo, G: Golpe, O: Acertaste, F: Fallaste, T: Tu turno

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

/*------------- JUGADOR 1 ------------*/
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
/*------------- JUGADOR 2 --------------*/
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
    }
   
    if(estructura.msg == 'T'){
        printf("\n");
        printf("Tu turno jugador 2 \n");
        enviarJugada(acertados,jugada);
        estructura.fila=jugada[0];
        estructura.columna=jugada[1];
        estructura.msg='G';
        return estructura;
    }
    if(estructura.msg == 'G'){
        estructura=recibirJugada(estructura,tablero); 
        return estructura;
    }
    if(estructura.msg == 'O'){
        acertados[estructura.fila][estructura.columna] = estructura.simbolo;
        printf("¡Acertaste!");
        if(i==-1){
            estructura.msg='W';
            printf("El jugador 2 gano la partida!");
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

int main() {
int i=0; 
mensaje msg = {'I',0,0,'I'};
   while(i==0){
        msg=player1(msg);
        if(msg.msg=='W'){
            i++;
            break;
        }
        msg=player2(msg);
        if(msg.msg=='W'){
            i--;
            break;
        }
   }
   if(i<0)
        printf("el jugador 2 gano\n");
   if(i>0)
        printf("El jugador 1 gano\n");
   return 0; 
}

