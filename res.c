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

int maze[MAX][CAR], bfsVisited[MAX],dfsVisited[MAX];;
int inputMaze[MAXI][MAXI];

struct Path *bfsPath;

struct Path* pathsBFS[MAX];
struct Queue* queue = NULL;

struct Path *dfsPath;

struct Path* pathsDFS[MAX];
struct Node* stack = NULL;


struct Path {
    int lastNode;
    int len;
    int arrNodes[MAX];
};

struct Node {  
    struct Path* path;
    struct Node* next; 
}; 

  
struct Node* newNode(struct Path *newPath) 
{ 
    struct Node* stackNode = (struct Node*)malloc(sizeof(struct Node)); 
    stackNode->path = newPath; 
    stackNode->next = NULL; 
    return stackNode; 
}
  
int isEmpty(struct Node* stack) 
{
    return !stack; 
} 
  
void push(struct Node** stack, struct Path *newPath) 
{ 
    struct Node* stackNode = newNode(newPath); 
    stackNode->next = *stack; 
    *stack = stackNode; 
} 
  
struct Path* pop(struct Node** stack) 
{ 
    if (isEmpty(*stack)) 
        return NULL; 
    struct Node* temp = *stack; 
    *stack = (*stack)->next; 
    struct Path* popped = temp->path; 
    free(temp); 
  
    return popped; 
} 

// The queue, front stores the front node of LL and rear stores the 
// last node of LL 
struct Queue { 
    struct Node *front, *rear; 
}; 
  

struct Queue* createQueue() 
{ 
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue)); 
    q->front = q->rear = NULL; 
    return q; 
}
int isEmptyQ(struct Queue* queue) 
{ 
    return !queue; 
}  

struct Path* copyPathBFS(){
    struct Path* path = (struct Path*)malloc(sizeof(struct Path));
    int i;
    
    if (bfsPath == NULL){
        path->len = 0;
        path->lastNode = 0;

        for(i = 0; i < MAX; i++)
            path->arrNodes[i] = 0;

        return path;
    }
    
    path->len = bfsPath->len;
    path->lastNode = bfsPath->lastNode;

    for(i = 0; i < bfsPath->len; i++)
        path->arrNodes[i] = bfsPath->arrNodes[i];
    
    return path;
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
  

void enQueue(struct Queue* q, struct Path* k) 
{ 
    struct Node* temp = newNode(k); 
  
   
    if (q->rear == NULL) { 
        q->front = q->rear = temp; 
        return; 
    } 
  
    
    q->rear->next = temp; 
    q->rear = temp; 
} 
  

struct Path* deQueue(struct Queue* q) 
{ 
    
    if (q->front == NULL) 
        return NULL; 
  
    
    struct Node* temp = q->front; 
    struct Path* deQueue= temp->path;
    
  
    q->front = q->front->next; 
  
 
    if (q->front == NULL) 
        q->rear = NULL; 
  
    free(temp); 
    return deQueue;
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
void bfsToOutput(double time_taken){
    int i;
    int steps =  bfsPath->len;

    int path[steps];
    for(i=0;i<steps;i++)
        path[i]=0;

    struct Path *currentBFSPath = bfsPath;
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

        int actual = currentBFSPath->arrNodes[count];
        
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

    printf("Tiempo de bfs: %d min, %d segundos, %d ms\n", (int)(time_taken/60),(int)time_taken, (int)(time_taken*1000)); 

    printf("%d", steps);
    int stepsToPrint = (steps * 2) / 32;
    for(i = 0; i <= stepsToPrint; i++)
        printf(" %d ", path[i]);
    printf("\n");
    
}
void bfs(int node, int des)
{
    int j;
    int actual = 0;
    int tmpPosition = 0;
    int nonNodesLeft = 0;

    queue = createQueue();
    struct Path* tmp;

    tmp = copyPathBFS();
    tmp->lastNode = node;
    enQueue(queue,tmp);


    while(!isEmptyQ(queue)){

        bfsPath =  deQueue(queue);
        node = bfsPath->lastNode;

        bfsVisited[node] = 1;

        if(node == des){
            break;
        }

        for(j=0;j<CAR;j++){
            
            if(!bfsVisited[maze[node][j]] && maze[node][j] != -1){

                actual = maze[node][j];
                tmp = copyPathBFS();
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

                enQueue(queue, tmp);
            }
        }
        nonNodesLeft = 0;
    }

}

void setupBFS(int ori1, int ori2, int des1, int des2){
    int i;
    for(i=0;i<nodes;i++)
        bfsVisited[i] = 0;
    
    int oriNode = (ori1 * totalColumns) + ori2;
    int desNode = (des1 * totalColumns) + des2;

    clock_t t; 
    t = clock(); 

    bfs(oriNode, desNode);

    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC;

    bfsToOutput(time_taken);
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
    setupBFS(oriRow,oriColumn,desRow,desColumn);
}