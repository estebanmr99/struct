#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h> 

#define MAX 10000
#define SET(a,b) ((a) |= (1<<(b)))

#define NORTE 0
#define SUR 3
#define OESTE 2
#define ESTE 1

void dfs(int, int);

int totalRows, totalColumns, nodes;
int maze[MAX][MAX], adjMaze[MAX][MAX], visited[MAX];
bool bandera=false;

struct Node* dfsList;
struct Path* dfsPath;

struct Node { 
    int data;
    struct Node* next; 
};

struct Path { 
    int cardinality;
    struct Path* next; 
};

void dfsToOutput(double time_taken){
    int i;
    struct Node *currentDFSList = dfsList;

    int steps = 0;

    while (currentDFSList != NULL) {
        printf("%d",currentDFSList->data);
        steps++;
        currentDFSList = currentDFSList->next;
    }
    steps--;

    int path[steps];
    for(i=0;i<steps;i++)
        path[i]=0;

    struct Path *currentDFSPath = dfsPath;
    int count = 0;
    int count2 = 0;
    int bits = (steps * 2) - 1;
    while (currentDFSPath != NULL) {
        if(count % 16 == 0 && count != 0){
            bits = (steps * 2) - 1;
            count2++;
        }

        int actual = currentDFSPath->cardinality;
        
        if (actual == 1){ //este
            SET(path[count2], (bits - 1));
        } else if(actual == 2){ //oeste
            SET(path[count2], (bits));
        } else if (actual == 3){ //sur
            SET(path[count2], (bits));
            SET(path[count2], (bits - 1));
        } // para norte como es 00 no se hace nada
        
        bits = bits - 2;
        count++; 
        currentDFSPath = currentDFSPath->next;
    }
    printf("Tiempo: %d min, %d segundos, %d ms\n", (int)(time_taken/60),(int)time_taken, (int)(time_taken*1000)); 

    printf("%d", steps);
    int stepsToPrint = (steps * 2) / 32;
    for(i = 0; i <= stepsToPrint; i++)
        printf(" %d ", path[i]);
    printf("\n");
    
}

void push(int val) {
    struct Node *current = dfsList;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = (struct Node*)malloc(sizeof(struct Node));
    current->next->data = val;
    current->next->next = NULL;
}

void pushPath(int car) {
    struct Path * current = dfsPath;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = (struct Path*)malloc(sizeof(struct Path));
    current->next->cardinality = car;
    current->next->next = NULL;
}

int getLastNode() {
    struct Node *current = dfsList;
    while (current->next != NULL) {
        current = current->next;
    }

    return current->data;
}


void dfs(int i, int des)
{
    int j;
    int lastNode = 0;
    visited[i]=1;
    printf("%d\n", i);

    if(i == des){
        dfsPath = dfsPath->next;
        return;
    }else{
        for(j=0;j<nodes;j++)
            if(!visited[j] && adjMaze[i][j]==1){
                lastNode = getLastNode();
                if(adjMaze[lastNode][j] == 1 && adjMaze[j][lastNode] == 1){
                    if(j == (i - totalColumns)){ //norte
                        pushPath(0);
                    } else if (j == (i - 1)){ //oeste
                        pushPath(2);
                    } else if(j == (i + totalColumns)){ //sur
                        pushPath(3);
                    } else if (j == (i + 1)){ //este
                        pushPath(1);
                    }
                    push(j);
                }
                dfs(j, des);
       }
    }
}

void setupDFS(int ori1, int ori2, int des1, int des2){
    int i;
    for(i=0;i<nodes;i++)
        visited[i]=0;
    
    int oriNode = (ori1 * totalColumns) + ori2;
    int desNode = (des1 * totalColumns) + des2;

    dfsList = (struct Node*)malloc(sizeof(struct Node));
    dfsList-> data = oriNode;

    dfsPath = (struct Path*)malloc(sizeof(struct Path));

    clock_t t; 
    t = clock(); 

    dfs(oriNode, desNode);

    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC;

    dfsToOutput(time_taken);
}

void mazeToAdj(){
    int i, j, tmp, tmpNorth, tmpWest;
    for(i=0;i<totalRows;i++){
        for(j=0;j<totalColumns;j++){
            tmp = (i * totalColumns) + j;

            switch(maze[i][j]){
                case 1:
                    tmpWest = tmp - 1;
                    adjMaze[tmp][tmpWest] = 1;
                    adjMaze[tmpWest][tmp] = 1;
                    break;

                case 2:
                    tmpNorth = tmp - totalColumns;
                    adjMaze[tmp][tmpNorth] = 1;
                    adjMaze[tmpNorth][tmp] = 1;
                    break;

                case 3:
                    tmpWest = tmp - 1;
                    adjMaze[tmp][tmpWest] = 1;
                    adjMaze[tmpWest][tmp] = 1;

                    tmpNorth = tmp - totalColumns;
                    adjMaze[tmp][tmpNorth] = 1;
                    adjMaze[tmpNorth][tmp] = 1;
                    
                    break;
            }
        }
    }
}

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
            
            p++;
            i++;
        }
        totalRows++;
        i=0;
    }
    fclose(fich);
}
 
void main()
{
    read();
    printf("Filas: %d\n",totalRows);
    printf("Columnas: %d\n",totalColumns);

    nodes = totalRows * totalColumns;
 
    mazeToAdj();

    setupDFS(2,0,2,2);
}