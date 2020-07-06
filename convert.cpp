#include <iostream>
#include <queue>
#include <vector>
#include <fstream>

#define SET(a,b) ((a) |= (1UL<<(b)))
#define CLEAR(a,b) ((a) & ~(1UL<<(b)))
#define FLIP(a,b) ((a) ^ (1UL<<(b)))

using namespace std;


// How to compare elements
struct my_comparator
{
    // queue elements are vectors so we need to compare those
    bool operator()(pair<int,int> const& a, pair<int,int> const& b) const
    {
        // reverse sort puts the lowest value at the top    
        return a.first > b.first;
    }
};

typedef priority_queue<pair<int,int>, vector<pair<int,int>>, my_comparator> my_priority_queue;
my_priority_queue fillPriorityQueue(vector<pair<int,int>>& matrix){
    my_priority_queue mpq;

    for(int i = 0; i < matrix.size(); i++){
        mpq.push(matrix[i]);
    }

    return mpq;
}

void createFile(vector<vector<int>>& matrix){
    ofstream myfile ("laberinto.txt");
    if (myfile.is_open()){
        for (int i = 0; i < matrix.size(); i++) { 
            for (int j = 0; j < matrix[i].size(); j++) 
                myfile << matrix[i][j]; 
            myfile << endl; 
        }
        myfile.close();
    }
    else cout << "Unable to open file";
}


vector<vector<int>> fillBinaryMatrix(my_priority_queue& priority, int row, int column){
    vector<vector<int>> binaryMatrix(row, vector<int> (column, 0));

    int major = row > column ? row : column;

    while(!priority.empty()) {
        pair<int,int> tmp;
        tmp = priority.top();

        int i = tmp.first / major;
        int j = tmp.first % major;

        int k = tmp.second / major;
        int l = tmp.second % major;

        if(k-1 == i){
            SET(binaryMatrix[k][l],1);

        } else if(l-1 == j){
            SET(binaryMatrix[k][l],0);
        } 
        else{
            exit(0);
        }
        priority.pop();
    }
    return binaryMatrix; 

}

int main() 
{ 
    my_priority_queue mpq;

    vector<pair<int,int> > matrix {
        {0,4},
        {0,1},
        {1,5},
        {2,6},
        {2,3},
        {3,7},
        {4,8},
        {5,9},
        {6,10},
        {7,11},
        {9,10}        
    };

    mpq = fillPriorityQueue(matrix);

    int row = 3;
    int column = 4;

    vector<vector<int> > binaryMatrix = fillBinaryMatrix(mpq, row, column);

    createFile(binaryMatrix);
  
    return 0;
} 