#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

#define MAX 100000
#define MAXI 10000

#define NORTE 0
#define ESTE 1
#define OESTE 2
#define SUR 3

#define CAR 4

#define SET(a,b) ((a) |= (1<<(b)))

int totalRows, totalColumns, nodes;

int maze[MAX][CAR], dfsVisited[MAX];
int inputMaze[MAXI][MAXI];

struct Path *dfsPath;

struct Path* pathsDFS[MAX];
struct StackNode* stack = NULL;

struct Path {
    int lastNode;
    int len;
    int arrNodes[MAX];
};

struct StackNode { 
    struct Path* path;
    struct StackNode* next; 
}; 
  
struct StackNode* newNode(struct Path *newPath) 
{ 
    struct StackNode* stackNode = (struct StackNode*)malloc(sizeof(struct StackNode)); 
    stackNode->path = newPath; 
    stackNode->next = NULL; 
    return stackNode; 
}
  
int isEmpty(struct StackNode* stack) 
{
    return !stack; 
} 
  
void push(struct StackNode** stack, struct Path *newPath) 
{ 
    struct StackNode* stackNode = newNode(newPath); 
    stackNode->next = *stack; 
    *stack = stackNode; 
} 
  
struct Path* pop(struct StackNode** stack) 
{ 
    if (isEmpty(*stack)) 
        return NULL; 
    struct StackNode* temp = *stack; 
    *stack = (*stack)->next; 
    struct Path* popped = temp->path; 
    free(temp); 
  
    return popped; 
} 

struct Path* copyPath(){
    struct Path* path = (struct Path*)malloc(sizeof(struct Path));
    int i;
    
    if (dfsPath == NULL){
        path->len = 0;
        path->lastNode = 0;

        for(i = 0; i < MAX; i++)
            path->arrNodes[i] = 0;

        return path;
    }
    
    path->len = dfsPath->len;
    path->lastNode = dfsPath->lastNode;

    for(i = 0; i < dfsPath->len; i++)
        path->arrNodes[i] = dfsPath->arrNodes[i];
    
    return path;
}
  
struct Path* peek(struct StackNode* stack) 
{ 
    if (isEmpty(stack)) 
        return NULL; 
    return stack->path;
}

void dfsToOutput(double time_taken){
    int i;
    int steps =  dfsPath->len;

    int path[steps];
    for(i=0;i<steps;i++)
        path[i]=0;

    struct Path *currentDFSPath = dfsPath;
    int count = 0;
    int count2 = 0;
    int saved = (steps * 2) - 1;
    int bits = (saved < 32) ? saved : 32;
    while (count < steps) {
        if(count % 16 == 0 && count != 0){
            saved = saved - 32;
            bits = (saved < 32) ? saved : 32;
            count2++;
        }

        int actual = currentDFSPath->arrNodes[count];
        
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
    }

    printf("Tiempo: %d min, %d segundos, %d ms\n", (int)(time_taken/60),(int)time_taken, (int)(time_taken*1000)); 

    printf("%d", steps);
    int stepsToPrint = (steps * 2) / 32;
    for(i = 0; i <= stepsToPrint; i++)
        printf(" %d ", path[i]);
    printf("\n");
    
}

void dfs(int node, int des)
{
    int j;
    int actual = 0;
    int tmpPosition = 0;
    int nonNodesLeft = 0;

    struct Path* tmp;

    tmp = copyPath();
    tmp->lastNode = node;
    push(&stack,tmp);

    while(!isEmpty(stack)){
        dfsPath =  pop(&stack);
        node = dfsPath->lastNode;

        dfsVisited[node] = 1;

        if(node == des){
            break;
        }

        for(j=0;j<CAR;j++){
            
            if(!dfsVisited[maze[node][j]] && maze[node][j] != -1){

                actual = maze[node][j];
                tmp = copyPath();
                tmp->len++;
                tmp->lastNode = actual;

                tmpPosition = tmp->len - 1;

                if(actual == (node - totalColumns)){ //norte
                    tmp->arrNodes[tmpPosition] = NORTE;
                } else if (actual == (node - 1)){ //oeste
                    tmp->arrNodes[tmpPosition] = OESTE;
                } else if(actual == (node + totalColumns)){ //sur
                    tmp->arrNodes[tmpPosition] = SUR;
                } else if (actual == (node + 1)){ //este
                    tmp->arrNodes[tmpPosition] = ESTE;
                }
                push(&stack, tmp);
            }
        }
        nonNodesLeft = 0;
    }
}

void setupDFS(int ori1, int ori2, int des1, int des2){
    int i;
    for(i=0;i<nodes;i++)
        dfsVisited[i] = 0;
    
    int oriNode = (ori1 * totalColumns) + ori2;
    int desNode = (des1 * totalColumns) + des2;

    clock_t t; 
    t = clock(); 

    dfs(oriNode, desNode);

    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC;

    dfsToOutput(time_taken);
}

void read()
{
    char c[MAX];
    int i = 0;
    while (fgets(c, MAX, stdin))
    {
        char *p;
        p = c;
        while (*p != '\n' & *p != '\0')
        {
            inputMaze[totalRows][i] = *p - '0';
            p++;
            i++;
        }
        if (totalRows == 0)
        {
            totalColumns = strlen(c);
            totalColumns--;
        }

        totalRows++;
        i = 0;
    }
}

void formatInputMaze()
{
    int i, j, tmp, tmpWest, tmpNorth;
    for (i = 0; i < totalRows; i++)
    {
        for (j = 0; j < totalColumns; j++)
        {
            tmp = (i * totalColumns) + j;
            switch (inputMaze[i][j])
            {
                case 1:
                    tmpWest = tmp - 1;
                    maze[tmp][2] = tmpWest;
                    maze[tmpWest][1] = tmp;
                    break;
                case 2:
                    tmpNorth = tmp - totalColumns;
                    maze[tmp][0] = tmpNorth;
                    maze[tmpNorth][3] = tmp;
                    break;
                case 3:

                    tmpWest = tmp - 1;
                    maze[tmp][2] = tmpWest;
                    maze[tmpWest][1] = tmp;

                    tmpNorth = tmp - totalColumns;
                    maze[tmp][0] = tmpNorth;
                    maze[tmpNorth][3] = tmp;


                    break;
            }
        }
    }

}

void main(int argc,char* argv[])
{
    int oriRow = atoi(argv[1]);
    int oriColumn = atoi(argv[2]);
    int desRow = atoi(argv[3]);
    int desColumn = atoi(argv[4]);
    int timetoWait = atoi(argv[5]);

    read();
    nodes = totalColumns * totalRows;
    memset(maze, -1, MAX);
    formatInputMaze();

    nodes = totalRows * totalColumns;

    setupDFS(oriRow,oriColumn,desRow,desColumn);
}