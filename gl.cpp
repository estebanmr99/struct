#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
#include <string>
#include <queue>


#define SET(a, b) ((a) |= (1UL << (b)))
#define CLEAR(a, b) ((a) & ~(1UL << (b)))
#define FLIP(a, b) ((a) ^ (1UL << (b)))
#define dprint(v) cerr << #v "=" << v << endl //;)
#define forr(i, a, b) for (int i = (a); i < (b); i++)
#define forn(i, n) forr(i, 0, n)
#define dforsn(i, a, b) for (int i = (b)-1; i >= a; i--)
#define forall(it, v) for (typeof(v.begin()) it = v.begin(); it != v.end(); ++it)
#define sz(c) ((int)c.size())
#define zero(v) memset(v, 0, sizeof(v))

using namespace std;

typedef long long ll;
typedef pair<int, int> ii;

const int MAXN = 100000;

vector<ii> G[MAXN];
bool taken[MAXN];

struct comparator
{
    bool operator()(pair<int, int> const &a, pair<int, int> const &b) const
    {
        return a.first > b.first;
    }
};

typedef priority_queue<pair<int, int>, vector<pair<int, int>>, comparator> my_priority_queue;
my_priority_queue fillPriorityQueue(vector<pair<int, int>> &matrix)
{
    my_priority_queue mpq;

    for (int i = 0; i < matrix.size(); i++)
    {
        mpq.push(matrix[i]);
    }

    return mpq;
}

int getRandomInt()
{
        ifstream randomStream;
        randomStream.open("/dev/urandom");
        if(randomStream.fail())
        {
                cout << "Can't open /dev/urandom\n";
                exit (1);
        }
        char randomChar;
        randomStream.get(randomChar);

        int randomInt;
        randomInt = int(randomChar);

        randomStream.close();

return randomInt;
}

void createFile(vector<vector<int>> &matrix)
{

    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
            cout << matrix[i][j];
        if(!(i == matrix.size() - 1))
            cout << endl;
    }

}

vector<vector<int>> fillBinaryMatrix(my_priority_queue &priority, int row, int column)
{
    vector<vector<int>> binaryMatrix(row, vector<int>(column, 0));

    int major = column;

    while (!priority.empty())
    {
        pair<int, int> tmp;
        tmp = priority.top();

        int i = tmp.first / major;
        int j = tmp.first % major;

        int k = tmp.second / major;
        int l = tmp.second % major;

        if (k - 1 == i)
        {
            SET(binaryMatrix[k][l], 1);
        }
        else if (l - 1 == j)
        {
            SET(binaryMatrix[k][l], 0);
        }
        else
        {
            exit(0);
        }
        priority.pop();
    }
    return binaryMatrix;
}

typedef pair<int, pair<int, int>> ip;
priority_queue<ip, vector<ip>, greater<ip>> pq; //min heap
void process(int v)
{
    taken[v] = true;
    forall(it, G[v]) if (!taken[it->second]) pq.push(make_pair(it->first, make_pair(v, it->second)));
}

vector<ii> prim()
{
    vector<ii> edges;
    zero(taken);
    process(0);
    ll cost = 0;
    while (sz(pq))
    {
        ip e = pq.top();
        pq.pop();
        if (!taken[e.second.second])
        {
            cost += e.first;
            if(e.second.second < e.second.first){
              edges.push_back(make_pair(e.second.second,e.second.first));  

            }else
            {
                edges.push_back(e.second);
            }
            process(e.second.second);
        }
    }
    cout << "Costo: " << cost << endl;

    return edges;
}

struct UnionFind
{
    vector<int> f; //the array contains the parent of each node
    void init(int n)
    {
        f.clear();
        f.insert(f.begin(), n, -1);
    }
    int comp(int x)
    {
        return (f[x] == -1 ? x : f[x] = comp(f[x]));
    } //O(1)
    bool join(int i, int j)
    {
        bool con = comp(i) == comp(j);
        if (!con)
        {
            f[comp(i)] = comp(j);
        }
        return con;
    }
} uf;

int n;

typedef struct Ar
{
    int a, b, w;
} Edge;
bool operator<(const Ar &a, const Ar &b) { return a.w < b.w; }
vector<Ar> E;

int cmp (Edge a1, Edge a2){
    return a1.w <= a2.w;
}

vector<ii> kruskal()
{
    vector<ii> edges;
    ll cost = 0;
    sort(E.begin(), E.end(), cmp); //ordenar aristas de menor a mayor
    uf.init(n);

    forall(it, E)
    {
        if (uf.comp(it->a) != uf.comp(it->b))
        { //si no estan conectados

            uf.join(it->a, it->b); //conectar
            cost = cost + it->w;
            if (it->b < it->a)
            {
                edges.push_back(make_pair(it->b, it->a));
            }
            else
            {
                edges.push_back(make_pair(it->a, it->b));
            }
        }
    }
    return edges;
}

void convert(int filas, int columnas)
{
    for (int i = 0; i < filas * columnas; i++)
    {
        for (int j = 0; j < G[i].size(); j++)
        {
            int a = G[i][j].second;
            int b = G[i][j].first;
            Ar p = {i, a, b};
            E.push_back(p);
        }
    }
}

bool member(vector<ii> g, int element)
{
    for (int i = 0; i < g.size(); i++)
    {
        int a = g[i].second;
        if (a == element)
            return true;
    }
    return false;
}

void print(int cantNodes)
{
    for (int i = 0; i < cantNodes; i++)
    {
        cout << i << endl;
        for (int j = 0; j < G[i].size(); j++)
        {
            int a = G[i][j].second;
            int b = G[i][j].first;
            cout << "[" << b << "," << a << "]" << endl;
        }
    }
}

void createGraph(int rows, int columns)
{
    int nodo = 0;
    int i = 0;
    while (i < rows)
    {
        int j = 0;
        while (j < columns)
        {
            if (i == rows - 1)
            {
                if (j == columns - 1)
                    j++;
                else
                {
                    srand(getRandomInt());
                    int pesoNDerecho = rand() % 100 + 1;
                    G[nodo].push_back(make_pair(pesoNDerecho, nodo + 1));
                    G[nodo + 1].push_back(make_pair(pesoNDerecho, nodo));
                    nodo++;
                    j++;
                }
            }
            else if (j == columns - 1)
            {
                srand(getRandomInt());
                int pesoNAbajo = rand() % 100 + 1;
                G[nodo].push_back(make_pair(pesoNAbajo, nodo + columns));
                G[nodo + columns].push_back(make_pair(pesoNAbajo, nodo));
                nodo++;
                j++;
            }
            else
            {
                srand(getRandomInt());
                int pesoNDerecho = rand() % 100 + 1;
                G[nodo].push_back(make_pair(pesoNDerecho, nodo + 1));
                G[nodo + 1].push_back(make_pair(pesoNDerecho, nodo));
                srand(getRandomInt());
                int pesoNAbajo = rand() % 100 + 1;
                G[nodo].push_back(make_pair(pesoNAbajo, nodo + columns));
                G[nodo + columns].push_back(make_pair(pesoNAbajo, nodo));
                nodo++;
                j++;
            }
        }
        i++;
    }
    
}

int main(int argc, char **argv)
{
    int rows = atoi(argv[1]);
    int columns = atoi(argv[2]);
    string funcion = argv[3];

    createGraph(rows, columns);

    if (funcion == "P")
    {
        vector<ii> cost = prim();
        my_priority_queue mpq;
        mpq = fillPriorityQueue(cost);
        vector<vector<int>> binaryMatrix = fillBinaryMatrix(mpq, rows, columns);
        createFile(binaryMatrix);
    }
    else
    {
        n = sizeof(G);
        convert(rows, columns);
        vector<ii> edges = kruskal();

        my_priority_queue mpq;
        mpq = fillPriorityQueue(edges);
        vector<vector<int>> binaryMatrix = fillBinaryMatrix(mpq, rows, columns);
        createFile(binaryMatrix);
    }

    return 0;
}

