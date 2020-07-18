#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <GL/glut.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX 100000
#define MAXI 10000
#define NORTE 0
#define ESTE 1
#define OESTE 2
#define SUR 3

#define CAR 4

#define SET(a, b) ((a) |= (1 << (b)))

bool banderaBFS = false;
bool banderaDFS = false;
int totalRows, totalColumns, nodes, windowDFS, windowBFS;
int row, column;

void setupBFS(int ori1, int ori2, int des1, int des2);
void setupDFS(int ori1, int ori2, int des1, int des2);

int maze[MAX][CAR], bfsVisited[MAX], dfsVisited[MAX];
int inputMaze[MAXI][MAXI];

struct timespec tim, tim2;

struct Path *bfsPath;

struct Path *pathsBFS[MAX];
struct Queue *queue = NULL;

struct Path *dfsPath;

struct Path *pathsDFS[MAX];
struct Node *stack = NULL;

// representa un camnino ya sea del DFS o BFS
struct Path
{
    int lastNode;
    int len;
    int arrNodes[MAX];
};

// representa los parametros que entran por consola
typedef struct
{
    int oriRow;
    int oriColumn;
    int desRow;
    int desColumn;
    int timetoWait;
} param;

param *parametros;

// como su nombre lo dice representa un nodo para una lista enlazada
struct Node
{
    struct Path *path;
    struct Node *next;
};

void init()
{

    glClearColor(1, 1, 1, 1);

    glMatrixMode(GL_PROJECTION);

    gluOrtho2D(0, 800, 0, 600);
}

GLvoid check(GLint x1, GLint y1, GLint x2, GLint y2, GLint x3, GLint y3, GLint x4, GLint y4, int i, int j)
{
    if (inputMaze[i][j] == 0)
    {
        glBegin(GL_LINE_LOOP);
        glVertex3f(x1, y1, 0.0);
        glVertex3f(x2, y2, 0.0);
        glEnd();
        glutSwapBuffers();
        glBegin(GL_LINE_LOOP);
        glVertex3f(x1, y1, 0.0);
        glVertex3f(x4, y4, 0.0);
        glEnd();
    }
    else if (inputMaze[i][j] == 1)
    {
        glBegin(GL_LINE_LOOP);
        glVertex3f(x1, y1, 0.0);
        glVertex3f(x2, y2, 0.0);
        glEnd();
    }

    else if (inputMaze[i][j] == 2)
    {
        glBegin(GL_LINE_LOOP);
        glVertex3f(x1, y1, 0.0);
        glVertex3f(x4, y4, 0.0);
        glEnd();
    }
}

GLvoid drawSquare(GLint x1, GLint y1, GLint x2, GLint y2, GLint x3, GLint y3, GLint x4, GLint y4, int i, int j)
{
    glLineWidth(0.5);
    glColor3f(0, 0, 0);
    if (i == totalRows - 1)
    {
        if (j == totalColumns - 1)
        {
            glBegin(GL_LINE_LOOP);
            glVertex3f(x2, y2, 0.0);
            glVertex3f(x3, y3, 0.0);
            glEnd();
            glutSwapBuffers();
        }
        glBegin(GL_LINE_LOOP);
        glVertex3f(x3, y3, 0.0);
        glVertex3f(x4, y4, 0.0);
        glEnd();
        glutSwapBuffers();
        check(x1, y1, x2, y2, x3, y3, x4, y4, i, j);
    }
    else
    {
        if (j == totalColumns - 1)
        {
            glBegin(GL_LINE_LOOP);
            glVertex3f(x2, y2, 0.0);
            glVertex3f(x3, y3, 0.0);
            glEnd();
            glutSwapBuffers();
            check(x1, y1, x2, y2, x3, y3, x4, y4, i, j);
        }
        else
        {
            check(x1, y1, x2, y2, x3, y3, x4, y4, i, j);
        }
    }
}

void drawSquare2(GLint x1, GLint x2, GLint y1, GLint y2, int color)
{
    //nanosleep(&ts,NULL);
    if (color == 0)
        glColor3f(1, 0, 0); // black color value is 0 0 0
    else
        glColor3f(1, 1, 1);
    glutSwapBuffers();
    if (totalRows >= 50 || totalColumns >= 50)
    {
        glPointSize(2.5);
    }
    else if (totalRows <= 5  && totalColumns <= 5)
    {
        glPointSize(40);
    }
    else if (totalRows <= 10  && totalColumns <= 10)
    {
        glPointSize(25);
    }
    else if (totalRows < 50 && totalColumns < 50)
    {
        glPointSize(5);
    }

    glBegin(GL_POINTS);
    glVertex3f(((x2 - x1) / 2) + x1, ((y2 - y1) / 2) + y1, 0.0);
    glEnd();
}

GLvoid showPath(int type, int color)
{
    struct Path *camino;
    if (type == 0)
    {
        camino = bfsPath;
    }
    else
        camino = dfsPath;
    int nodoInicio = ((totalRows * row) + column) + row;
    int cont = 0;
    int i = totalRows - 1;
    while (nodoInicio >= cont + totalColumns)
    {
        cont = cont + totalColumns;
        i--;
    }
    int j = nodoInicio - cont;
    int n = camino->len;
    GLfloat x;
    GLfloat y;
    x = ((double)800 / (double)totalColumns) * (j);
    y = ((double)600 / (double)totalRows) * (i);
    drawSquare2(x, x + (double)800 / (double)totalColumns, y + (double)600 / (double)totalRows, y, color);
    for (int k = 0; k < n; k++)
    {
        glutSwapBuffers();
        glColor3f(0.0, 1.0, 0.0);
        if (camino->arrNodes[k] == ESTE)
        {
            j++;
            GLfloat x;
            GLfloat y;
            x = ((double)800 / (double)totalColumns) * (j);
            y = ((double)600 / (double)totalRows) * (i);
            drawSquare2(x, x + (double)800 / (double)totalColumns, y + (double)600 / (double)totalRows, y, color);
        }
        else if (camino->arrNodes[k] == OESTE)
        {
            j--;
            GLfloat x;
            GLfloat y;
            x = ((double)800 / (double)totalColumns) * (j);
            y = ((double)600 / (double)totalRows) * (i);
            drawSquare2(x, x + (double)800 / (double)totalColumns, y + (double)600 / (double)totalRows, y, color);
        }
        else if (camino->arrNodes[k] == SUR)
        {
            i--;
            GLfloat x;
            GLfloat y;
            x = ((double)800 / (double)totalColumns) * (j);
            y = ((double)600 / (double)totalRows) * (i);
            drawSquare2(x, x + (double)800 / (double)totalColumns, y + (double)600 / (double)totalRows, y, color);
        }
        else if (camino->arrNodes[k] == NORTE)
        {
            i++;
            GLfloat x;
            GLfloat y;
            x = ((double)800 / (double)totalColumns) * (j);
            y = ((double)600 / (double)totalRows) * (i);
            drawSquare2(x, x + (double)800 / (double)totalColumns, y + (double)600 / (double)totalRows, y, color);
        }
    }
}

void drawMaze()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear display window
    GLfloat x = 0;
    GLfloat y = 0;
    int i = totalRows - 1;
    int j = 0;
    while (i >= 0)
    {
        while (j < totalColumns)
        {
            drawSquare(x, y + (double)600 / (double)totalRows, x + (double)800 / (double)totalColumns, y + (double)600 / (double)totalRows, x + (double)800 / (double)totalColumns, y, x, y, i, j);
            x += ((double)800 / (double)totalColumns);
            j++;
        }
        x = 0;
        y += ((double)600 / (double)totalRows);
        j = 0;
        i--;
    }
    // Process all OpenGL routine s as quickly as possible
    glFlush();
}

// Entrada: No tiene parametros
// Salida: No tiene retorno
void displayBFS()
{
    drawMaze();
    setupBFS(parametros->oriRow, parametros->oriColumn, parametros->desRow, parametros->desColumn);
    glutSwapBuffers();
}

// Entrada: No tiene parametros
// Salida: No tiene retorno
void displayDFS()
{
    drawMaze();
    setupDFS(parametros->oriRow, parametros->oriColumn, parametros->desRow, parametros->desColumn);
    glutSwapBuffers();
}

// agrega un nodo a la pila
struct Node *newNode(struct Path *newPath)
{
    struct Node *stackNode = (struct Node *)malloc(sizeof(struct Node));
    stackNode->path = newPath;
    stackNode->next = NULL;
    return stackNode;
}

void display();

// Entrada: Recibe puntero a la pila
// Salida: Retorna 0 o 1 dependiendo si la pila esta vacia
int isEmpty(struct Node *stack)
{
    return !stack;
}

// Entrada: Recibe puntero a un puntero de la pila, el camino que va a agregar a la pila
// Salida: No tiene retorno
void push(struct Node **stack, struct Path *newPath)
{
    struct Node *stackNode = newNode(newPath);
    stackNode->next = *stack;
    *stack = stackNode;
}

// Entrada: Recibe puntero a un puntero de la pila
// Salida: Retorna  un camino elinminado de la pila
struct Path *pop(struct Node **stack)
{
    if (isEmpty(*stack))
        return NULL;
    struct Node *temp = *stack;
    *stack = (*stack)->next;
    struct Path *popped = temp->path;
    free(temp);

    return popped;
}

// representa una cola
struct Queue
{
    struct Node *front, *rear;
};

// Entrada: No recibe parametros
// Salida:  Retorna una estructura que representa la cola
struct Queue *createQueue()
{
    struct Queue *q = (struct Queue *)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

// Entrada:  Recibe una cola
// Salida: Retorna un bool, indicando si esta vacía o no 
int isEmptyQ(struct Queue *queue)
{
    return !queue;
}
// Entrada: No recibe parametros
// Salida: Devuelve un struct de tipo path, que contiene la ruta copiada
struct Path *copyPathBFS()
{
    struct Path *path = (struct Path *)malloc(sizeof(struct Path));
    int i;

    if (bfsPath == NULL)
    {
        path->len = 0;
        path->lastNode = 0;

        for (i = 0; i < MAX; i++)
            path->arrNodes[i] = 0;

        return path;
    }

    path->len = bfsPath->len;
    path->lastNode = bfsPath->lastNode;

    for (i = 0; i < bfsPath->len; i++)
        path->arrNodes[i] = bfsPath->arrNodes[i];

    return path;
}

// Entrada: No recibe nada
// Salida: Devuelve una copia de la ultima ruta del DFS
struct Path *copyPath()
{
    struct Path *path = (struct Path *)malloc(sizeof(struct Path));
    int i;

    if (dfsPath == NULL)
    {
        path->len = 0;
        path->lastNode = 0;

        for (i = 0; i < MAX; i++)
            path->arrNodes[i] = 0;

        return path;
    }

    path->len = dfsPath->len;
    path->lastNode = dfsPath->lastNode;

    for (i = 0; i < dfsPath->len; i++)
        path->arrNodes[i] = dfsPath->arrNodes[i];

    return path;
}

// Entrada: Recibe una cola y una estructura tipo path
// Salida: Devuelve la cola recibida, con el nuevo nodo insertado 

void enQueue(struct Queue *q, struct Path *k)
{
    struct Node *temp = newNode(k);

    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
        return;
    }

    q->rear->next = temp;
    q->rear = temp;
}

// Entrada: Recibe una cola
// Salida: Una estructura tipo path con el nodo eliminado

struct Path *deQueue(struct Queue *q)
{

    if (q->front == NULL)
        return NULL;

    struct Node *temp = q->front;
    struct Path *deQueue = temp->path;

    q->front = q->front->next;

    if (q->front == NULL)
        q->rear = NULL;

    free(temp);
    return deQueue;
}

// Entrada: Recibe un doble que representa el tiempo 
// Salida: No tiene retorno
void dfsToOutput(double time_taken)
{
    int i;
    int steps = dfsPath->len;

    int path[steps];
    for (i = 0; i < steps; i++)
        path[i] = 0;

    struct Path *currentDFSPath = dfsPath;
    int count = 0;
    int count2 = 0;
    int saved = (steps * 2) - 1;
    int bits = (saved < 32) ? saved : 32;
    while (count < steps)
    {
        if (count % 16 == 0 && count != 0)
        {
            saved = saved - 32;
            bits = (saved < 32) ? saved : 32;
            count2++;
        }

        int actual = currentDFSPath->arrNodes[count];

        if (actual == 1)
        { //este
            SET(path[count2], (bits - 1));
        }
        else if (actual == 2)
        { //oeste
            SET(path[count2], (bits));
        }
        else if (actual == 3)
        { //sur
            SET(path[count2], (bits));
            SET(path[count2], (bits - 1));
        } // para norte como es 00 no se hace nada

        bits = bits - 2;
        count++;
    }

    printf("Tiempo: %d min, %d segundos, %d ms\n", (int)(time_taken / 60), (int)time_taken, (int)(time_taken * 1000));

    printf("%d", steps);
    int stepsToPrint = (steps * 2) / 32;
    for (i = 0; i <= stepsToPrint; i++)
        printf(" %d ", path[i]);
    printf("\n");
}

// Entrada: Recibe un doble que representa el tiempo 
// Salida: No tiene retorno
void bfsToOutput(double time_taken)
{
    int i;
    int steps = bfsPath->len;

    int path[steps];
    for (i = 0; i < steps; i++)
        path[i] = 0;

    struct Path *currentBFSPath = bfsPath;
    int count = 0;
    int count2 = 0;
    int saved = (steps * 2) - 1;
    int bits = (saved < 32) ? saved : 32;
    while (count < steps)
    {
        if (count % 16 == 0 && count != 0)
        {
            saved = saved - 32;
            bits = (saved < 32) ? saved : 32;
            count2++;
        }

        int actual = currentBFSPath->arrNodes[count];

        if (actual == 1)
        { //este
            SET(path[count2], (bits - 1));
        }
        else if (actual == 2)
        { //oeste
            SET(path[count2], (bits));
        }
        else if (actual == 3)
        { //sur
            SET(path[count2], (bits));
            SET(path[count2], (bits - 1));
        } // para norte como es 00 no se hace nada

        bits = bits - 2;
        count++;
    }

    printf("Tiempo de bfs: %d min, %d segundos, %d ms\n", (int)(time_taken / 60), (int)time_taken, (int)(time_taken * 1000));

    printf("%d", steps);
    int stepsToPrint = (steps * 2) / 32;
    for (i = 0; i <= stepsToPrint; i++)
        printf(" %d ", path[i]);
    printf("\n");
}

// Entrada: Recibe dos enteros, uno que representa origen y el otro destino
// Salida: No tiene retorno
void bfs(int node, int des)
{
    int j;
    int actual = 0;
    int tmpPosition = 0;
    int nonNodesLeft = 0;

    queue = createQueue();
    struct Path *tmp;

    tmp = copyPathBFS();
    tmp->lastNode = node;
    enQueue(queue, tmp);

    while (!isEmptyQ(queue))
    {
        bfsPath = deQueue(queue);
        showPath(0, 0);
        nanosleep(&tim, NULL);
        showPath(0, 1);
        node = bfsPath->lastNode;
        bfsVisited[node] = 1;

        if (node == des)
        {
            showPath(0, 0);
            break;
        }

        for (j = 0; j < CAR; j++)
        {

            if (!bfsVisited[maze[node][j]] && maze[node][j] != -1)
            {

                actual = maze[node][j];
                tmp = copyPathBFS();
                tmp->len++;
                tmp->lastNode = actual;

                tmpPosition = tmp->len - 1;

                if (actual == (node - totalColumns))
                { //norte
                    tmp->arrNodes[tmpPosition] = NORTE;
                }
                else if (actual == (node - 1))
                { //oeste
                    tmp->arrNodes[tmpPosition] = OESTE;
                }
                else if (actual == (node + totalColumns))
                { //sur
                    tmp->arrNodes[tmpPosition] = SUR;
                }
                else if (actual == (node + 1))
                { //este
                    tmp->arrNodes[tmpPosition] = ESTE;
                }

                enQueue(queue, tmp);
            }
        }
        nonNodesLeft = 0;
    }
}

// Entrada: recibe cuatro enteros que representan los puntos de origen y detsino
// Salida: No tiene retorno
void setupBFS(int ori1, int ori2, int des1, int des2)
{
    int i;
    for (i = 0; i < nodes; i++)
        bfsVisited[i] = 0;

    int oriNode = (ori1 * totalColumns) + ori2;
    int desNode = (des1 * totalColumns) + des2;

    clock_t t;
    t = clock();

    bfs(oriNode, desNode);

    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC;

    bfsToOutput(time_taken);
}

// Entrada: Recibe dos enteros, uno que representa origen y el otro destino
// Salida: No tiene retorno
void dfs(int node, int des)
{
    int j;
    int actual = 0;
    int tmpPosition = 0;
    int nonNodesLeft = 0;

    struct Path *tmp;

    tmp = copyPath();
    tmp->lastNode = node;
    push(&stack, tmp);

    while (!isEmpty(stack))
    {

        dfsPath = pop(&stack);
        showPath(1, 0);
        nanosleep(&tim, NULL);
        showPath(1, 1);
        node = dfsPath->lastNode;

        dfsVisited[node] = 1;

        if (node == des)
        {
            showPath(1, 0);
            break;
        }

        for (j = 0; j < CAR; j++)
        {

            if (!dfsVisited[maze[node][j]] && maze[node][j] != -1)
            {

                actual = maze[node][j];
                tmp = copyPath();
                tmp->len++;
                tmp->lastNode = actual;

                tmpPosition = tmp->len - 1;

                if (actual == (node - totalColumns))
                { //norte
                    tmp->arrNodes[tmpPosition] = NORTE;
                }
                else if (actual == (node - 1))
                { //oeste
                    tmp->arrNodes[tmpPosition] = OESTE;
                }
                else if (actual == (node + totalColumns))
                { //sur
                    tmp->arrNodes[tmpPosition] = SUR;
                }
                else if (actual == (node + 1))
                { //este
                    tmp->arrNodes[tmpPosition] = ESTE;
                }
                push(&stack, tmp);
            }
        }
        nonNodesLeft = 0;
    }
}

// Entrada: recibe cuatro enteros que representan los puntos de origen y detsino
// Salida: No tiene retorno
void setupDFS(int ori1, int ori2, int des1, int des2)
{
    int i;
    for (i = 0; i < nodes; i++)
        dfsVisited[i] = 0;

    int oriNode = (ori1 * totalColumns) + ori2;
    int desNode = (des1 * totalColumns) + des2;

    clock_t t;
    t = clock();

    dfs(oriNode, desNode);

    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC;

    dfsToOutput(time_taken);
}

// Entrada: No recibe parametros
// Salida: No tiene retorno
//Se encarga de leer la matriz desde la consola
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

// Entrada: No recibe parametros
// Salida: No tiene retorno
// Se encarga de convertir los datos del archivo a una matriz representativa
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

void *threadBfs(void *p)
{
    param *parametros = (param *)p;

    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Maze BFS");
    init();
    glutDisplayFunc(displayBFS);

    return NULL;
}

void *threadDfs(void *p)
{

    param *parametros = (param *)p;
    glutInitWindowPosition(1000000, 0);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Maze DFS");
    init();
    glutDisplayFunc(displayDFS);

    return NULL;
}

void main(int argc, char *argv[])
{
    int oriRow = atoi(argv[1]);
    int oriColumn = atoi(argv[2]);
    int desRow = atoi(argv[3]);
    int desColumn = atoi(argv[4]);
    int timetoWait = atoi(argv[5]);
    tim.tv_sec = timetoWait;
    tim.tv_nsec = 0;
    row = oriRow;
    column = oriColumn;
    read();
    nodes = totalColumns * totalRows;
    memset(maze, -1, MAX);
    formatInputMaze();
    parametros = (param *)malloc(sizeof(param));
    parametros->oriRow = oriRow;
    parametros->oriColumn = oriColumn;
    parametros->desRow = desRow;
    parametros->desColumn = desColumn;
    parametros->timetoWait = timetoWait;
    pthread_t thread_bfs;
    pthread_t thread_dfs;
    int argc1 = 1;
    char *argv1[] = {"res"};
    glutInit(&argc1, argv1);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    pthread_create(&thread_bfs, NULL, threadBfs, NULL);
    pthread_join(thread_bfs, NULL);
    pthread_create(&thread_dfs, NULL, threadDfs, NULL);

    pthread_join(thread_dfs, NULL);
    glutMainLoop();
}
