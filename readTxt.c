#include <stdio.h>
#include <string.h>
#include <stdbool.h> 
#define MAX 10000
#define MAXI 10000
int totalRows=0;
int totalColumns=0;
bool bandera=false;
int maze[MAX][MAX];


void read(){
    int i=0;
    char linea[1024];
    FILE *fich;
 
    fich = fopen("laberinto.txt", "r");
    while(fgets(linea, 1024, (FILE*) fich)) {
        totalColumns = strlen(linea);
        
        char *p;
        p = linea;
        while (*p != '\n' & *p!='\0') {

            maze[totalRows][i]= *p -'0';
            printf("%d ",maze[totalRows][i]);
            
            p++;
            i++;
        }
        totalRows++;
        i=0;
        printf("\n");


    }

    totalColumns=totalColumns-1;
    fclose(fich);


}


int main() {
 
   read();
   printf("Filas: %d\n",totalRows);
   printf("Columnas: %d\n",totalColumns);
   for (int i = 0; i < 4; i++)
   {
       for (int j = 0; j < 5; j++)
       {
          printf("%d ",maze[i][j]);
       }
       printf("\n");
       
   }
   



}
 