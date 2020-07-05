#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;
#define oo 1<<30
//#define dprint(v) cerr << #v"=" << v << endl //;)
//#define forr(i,a,b) for(int i=(a); i<(b); i++)
//#define forn(i,n) forr(i,0,n)
//#define dforsn(i,a,b) for(int i=(b)-1; i>=a; i--)
#define forall(it,v) for(typeof(v.begin()) it=v.begin();it!=v.end();++it)
#define sz(c) ((int)c.size())
#define zero(v) memset(v, 0, sizeof(v))
typedef long long ll;
typedef pair<int,int> ii;
const int MAXN=100000;

struct UnionFind{
	vector<int> f;//the array contains the parent of each node
	void init(int n){f.clear(); f.insert(f.begin(), n, -1);}
	int comp(int x){
        return (f[x]==-1?x:f[x]=comp(f[x]));
    }//O(1)
	bool join(int i, int j) {
		bool con=comp(i)==comp(j);
		if(!con){ 
            f[comp(i)] = comp(j);}
		return con;
	}}uf;
vector<ii> G[MAXN];
int n;

struct Ar{int a,b,w;};
bool operator<(const Ar& a, const Ar &b){return a.w<b.w;}
vector<Ar> E;

vector<ii> kruskal(){
    vector<ii> edges;
    ll cost=0;
    sort(E.begin(), E.end());//ordenar aristas de menor a mayor
    uf.init(n);


    forall(it, E){
        if(uf.comp(it->a)!=uf.comp(it->b)){//si no estan conectados
            
            uf.join(it->a, it->b);//conectar
            cost=cost+it->w;
            edges.push_back(make_pair(it->a,it->b));
        }
    }
    return edges;
}

void convert(int filas, int columnas){
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            int a=G[i][j].second;
            int b=G[i][j].first;
            Ar p={i,a,b};
            E.push_back(p);
  
        }
        
    }

}


int main(){

 
    G[0].push_back(make_pair(oo,0));
    G[0].push_back(make_pair(oo,1));
    G[0].push_back(make_pair(10,2));
	G[0].push_back(make_pair(7,3));
    G[0].push_back(make_pair(oo,4));
	G[1].push_back(make_pair(oo,0));
	G[1].push_back(make_pair(oo,1));
	G[1].push_back(make_pair(oo,2));
	G[1].push_back(make_pair(32,3));
	G[1].push_back(make_pair(oo,4));
	G[2].push_back(make_pair(10,0));
	G[2].push_back(make_pair(oo,1));
	G[2].push_back(make_pair(oo,2));
	G[2].push_back(make_pair(9,3));
	G[2].push_back(make_pair(oo,4));
	G[3].push_back(make_pair(7,0));
	G[3].push_back(make_pair(32,1));
	G[3].push_back(make_pair(9,2));
	G[3].push_back(make_pair(oo,3));
	G[3].push_back(make_pair(23,4));
	G[4].push_back(make_pair(oo,0));
	G[4].push_back(make_pair(oo,1));
	G[4].push_back(make_pair(oo,2));
	G[4].push_back(make_pair(23,3));
	G[4].push_back(make_pair(oo,4));



    
    n=sizeof(G);  
    convert(5,5);
    vector<ii> edges = kruskal();
    for(ii i: edges){
        cout<<"Arista: "<<i.first<<", "<<i.second<<endl;
    }

    

    return 0;
}
