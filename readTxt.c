#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define MAX 10000
int totalRows = 0;
int totalColumns = 0;
bool bandera = false;
int maze[MAX][MAX];
int nodes = 0;

int adjMaze[MAX][MAX];
int matriz[MAX][4];

void read()
{
    char c[10];
    char matriz[10][10];
    int i = 0;
    while (fgets(c, 100, stdin))
    {
        

        char *p;
        p = c;
        while (*p != '\n' & *p != '\0')
        {
            maze[totalRows][i] = *p - '0';
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

void matrizAdj()
{
    int i, j, tmp, tmpWest, tmpNorth;
    for (i = 0; i < totalRows; i++)
    {
        for (j = 0; j < totalColumns; j++)
        {
            tmp = (i * totalColumns) + j;
            switch (maze[i][j])
            {
            case 1:
                tmpWest = tmp - 1;
                matriz[tmp][2] = tmpWest;
                matriz[tmpWest][1] = tmp;
                break;
            case 2:
                tmpNorth = tmp - totalColumns;
                matriz[tmp][0] = tmpNorth;
                matriz[tmpNorth][3] = tmp;
                break;
            case 3:

                tmpWest = tmp - 1;
                matriz[tmp][2] = tmpWest;
                matriz[tmpWest][1] = tmp;

                tmpNorth = tmp - totalColumns;
                matriz[tmp][0] = tmpNorth;
                matriz[tmpNorth][3] = tmp;


                break;
            }
        }
    }

}

int main()
{

    read();
    nodes = totalColumns * totalRows;
    memset(matriz, -1, MAX);
    matrizAdj();
    for (int i = 0; i < nodes; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%d, ", matriz[i][j]);
        }
        printf("\n");
    }


    return 0;
}
