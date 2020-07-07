#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;
typedef pair<int,int> ii;
const int MAXN = 100000;
vector<ii> G[MAXN];


int n,m;


void print(int cantNodes){
  for (int i = 0; i < cantNodes; i++)
    {
    	cout<<i<<endl;
        for (int j = 0; j < G[i].size(); j++)
        {
            int a=G[i][j].second;
            int b=G[i][j].first;
            cout<<"["<<b<<","<<a<<"]"<<endl;

        }

    }

}

void createGraph(int rows , int columns){
	int nodo = 0;
	int i = 0;
	while(i<rows){
		int j = 0;
		while(j<columns){
			if(i == rows-1){
				if(j==columns-1)
					j++;
				else{
					srand((unsigned int )time(NULL));
					int pesoNDerecho = rand() % 100 + 1;
					G[nodo].push_back(make_pair(nodo+1,pesoNDerecho));
					G[nodo+1].push_back(make_pair(nodo,pesoNDerecho));
					nodo++;
					j++;
				}
			} else if(j == columns-1){
				srand((unsigned int )time(NULL));
				int pesoNAbajo = rand() % 100 + 1;
				G[nodo].push_back(make_pair(nodo+columns,pesoNAbajo));
				G[nodo+columns].push_back(make_pair(nodo,pesoNAbajo));
				nodo++;
				j++;
			} else{
				srand((unsigned int )time(NULL));
				int pesoNDerecho = rand() % 100 + 1;
				G[nodo].push_back(make_pair(nodo+1,pesoNDerecho));
				G[nodo+1].push_back(make_pair(nodo,pesoNDerecho));
				srand((unsigned int )time(NULL));
				int pesoNAbajo = rand() % 100 + 1;
				G[nodo].push_back(make_pair(nodo+columns,pesoNAbajo));
				G[nodo+columns].push_back(make_pair(nodo,pesoNAbajo));
				nodo++;
				j++;
			}

		}
		i++;
	}
	print(rows*columns);
}




int main(){

  cin>>n>>m;
  createGraph(n,m);
  return 0;
}


/*
0  1  2  3  4  5  6
7  8  9  10 11 12 13
14 15 16 17 18 19 20
*/
