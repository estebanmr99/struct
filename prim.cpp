#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <queue>
#include <vector>
#include <string>
using namespace std;
#define oo 1<<30
#define dprint(v) cerr << #v"=" << v << endl //;)
#define forr(i,a,b) for(int i=(a); i<(b); i++)
#define forn(i,n) forr(i,0,n)
#define dforsn(i,a,b) for(int i=(b)-1; i>=a; i--)
#define forall(it,v) for(typeof(v.begin()) it=v.begin();it!=v.end();++it)
#define sz(c) ((int)c.size())
#define zero(v) memset(v, 0, sizeof(v))
typedef long long ll;
typedef pair<int,int> ii;
const int MAXN=100000;

int n;
vector<ii> G[MAXN];
bool taken[MAXN];
typedef pair<int,pair<int,int>> ip;
priority_queue<ip, vector<ip>, greater<ip>> pq;//min heap
void process(int v){
    taken[v]=true;
    forall(it, G[v])
        if(!taken[it->second]) pq.push(make_pair(it->first,make_pair(v,it->second)));
}

vector<ii> prim(){
    vector<ii> edges;
    zero(taken);
    process(0);
    ll cost=0;
    while(sz(pq)){
        ip e=pq.top(); pq.pop();
        if(!taken[e.second.second]){ 
            cost+=e.first; 
            edges.push_back(e.second);
            process(e.second.second);
        }

    }
    cout<<"Costo: "<<cost<<endl;

    return edges;
}

int main(){


//grafo de ejemplo
    G[0].push_back(make_pair(oo,0));
    G[0].push_back(make_pair(1,1));
    G[0].push_back(make_pair(2,2));
	G[0].push_back(make_pair(oo,3));
    G[0].push_back(make_pair(oo,4));
	G[1].push_back(make_pair(1,0));
	G[1].push_back(make_pair(oo,1));
	G[1].push_back(make_pair(2,2));
	G[1].push_back(make_pair(oo,3));
	G[1].push_back(make_pair(oo,4));
	G[2].push_back(make_pair(2,0));
	G[2].push_back(make_pair(2,1));
	G[2].push_back(make_pair(oo,2));
	G[2].push_back(make_pair(24,3));
	G[2].push_back(make_pair(67,4));
	G[3].push_back(make_pair(oo,0));
	G[3].push_back(make_pair(oo,1));
	G[3].push_back(make_pair(67,2));
	G[3].push_back(make_pair(oo,3));
	G[3].push_back(make_pair(100,4));
	G[4].push_back(make_pair(oo,0));
	G[4].push_back(make_pair(oo,1));
	G[4].push_back(make_pair(67,2));
	G[4].push_back(make_pair(100,3));
	G[4].push_back(make_pair(oo,4));



    vector<ii> cost = prim();

     for (ii i : cost){
		 cout<<"Arista:"<< i.first <<","<< i.second << "\n";
	 }
        return 0;
}