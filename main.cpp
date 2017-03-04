//
//  main.cpp
//  TAK AI
//
//  Created by Rohit Naik on 9/29/16.
//  Copyright © 2016 COL333. All rights reserved.
//

#include <iostream>
#include <stack>
#include <cstring>
#include <tuple>
#include <vector>
#include <time.h>
#include <climits>
#include <algorithm>
#include <memory>
using namespace std;

struct Stack{
    stack<int> stk;
};

int MAX_DEPTH = 2;
int size;
int playerID;
int timeLimit;

struct State{
    int noH;    // home - me
    int noA;    // away - opponent
    int noHCap; // home Cap Stones
    int noACap; // away Cap Stones
    Stack *board;
    vector<int> hWalls;
    vector<int> aWalls;
    State(int size){
        board = new Stack[size*size];
        switch(size){
            case 3:
                noH = noA = 10;
                noHCap = noACap = 0;
                break;
            case 4:
                noH = noA = 15;
                noHCap = noACap = 0;
                break;
            case 5:
                noH = noA = 21;
                noHCap = noACap = 1;
                break;
            case 6:
                noH = noA = 30;
                noHCap = noACap = 1;
                break;
            case 7:
                noH = noA = 40;
                noHCap = noACap = 1;
                break;
            case 8:
                noH = noA = 50;
                noHCap = noACap = 2;
                break;
        }
    }
};

int getCord(int row, int col){
    return (row*size+col);
}

//return true if there exists a path from one side of the board to another
bool DFS(int i, int j, State &state, int** visited, int size, int type, int BorW){
    visited[i][j] = 1;
    if(type==0 && i==size-1)
        return true;
    if(type==1 && j==size-1)
        return true;
    bool ret = false;
    if((i-1)>=0 && visited[i-1][j]==0 && !state.board[getCord(i-1, j)].stk.empty() && (state.board[getCord(i-1, j)].stk.top()==(1*BorW) || state.board[getCord(i-1, j)].stk.top()==(3*BorW)))
        ret = DFS(i-1, j, state, visited, size, type, BorW);
    if(ret)
        return true;
    if((j-1)>=0 && visited[i][j-1]==0 && !state.board[getCord(i, j-1)].stk.empty() && (state.board[getCord(i, j-1)].stk.top()==(1*BorW) || state.board[getCord(i, j-1)].stk.top()==(3*BorW)))
        ret = DFS(i, j-1, state, visited, size, type, BorW);
    if(ret)
        return true;
    if((i+1)<size && visited[i+1][j]==0 && !state.board[getCord(i+1, j)].stk.empty() && (state.board[getCord(i+1, j)].stk.top()==(1*BorW) || state.board[getCord(i+1, j)].stk.top()==(3*BorW)))
        ret = DFS(i+1, j, state, visited, size, type, BorW);
    if(ret)
        return true;
    if((j+1)<size && visited[i][j+1]==0 && !state.board[getCord(i, j+1)].stk.empty() && (state.board[getCord(i, j+1)].stk.top()==(1*BorW) || state.board[getCord(i, j+1)].stk.top()==(3*BorW)))
        ret = DFS(i, j+1, state, visited, size, type, BorW);
    return ret;
}
bool isPath(State &state, int size, int BorW){
    int** visited = new int*[size];
    for(int i=0; i<size; i++)
        visited[i] = new int[size];
    
    for(int j=0; j<size; j++)
        if(!state.board[getCord(0, j)].stk.empty() && (state.board[getCord(0, j)].stk.top()==(1*BorW) || state.board[getCord(0, j)].stk.top()==(3*BorW)) && DFS(0, j, state, visited, size, 0, BorW))
            return true;
    
    for (int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            visited[i][j] = 0;
    
    for(int i=0; i<size; i++)
        if(!state.board[getCord(i, 0)].stk.empty() && (state.board[getCord(i, 0)].stk.top()==(1*BorW) || state.board[getCord(i, 0)].stk.top()==(3*BorW)) && DFS(i, 0, state, visited, size, 1, BorW))
            return true;
    
    for(int i=0; i<size; i++)
        delete[] visited[i];
    delete visited;
    
    return false;
}
int isPathPresent2(State state){
    //checking for white road
    if(isPath(state, size, 1))
        return 3;
    if(isPath(state, size, -1))
        return 2;
    
    bool isFreeSquare = false;
    for (int i=0; i<size; i++)
        for (int j=0; j<size; j++)
            if(state.board[getCord(i, j)].stk.empty()){
                isFreeSquare = true;
                i = size;
                break;
            }
    
    if(state.noA==0 || state.noH==0 || !isFreeSquare)//flat win
        return 1;
    
    return 0;
}


void printSeq( int num , int a[] , int len , int s ,vector<vector<int>> &p,int holes){
    if( num <= 0 ){
        vector<int> p1;
        if(len<=holes){
            for( int j = 0 ; j < len ; j++ ){
                p1.push_back(a[j]);
            }
            vector<vector<int>> p1_perm;
            p1_perm.push_back(p1);
            while(next_permutation(p1.begin(),p1.end())){
                p1_perm.push_back(p1);
            }
            p.insert(p.begin(),p1_perm.begin(),p1_perm.end());
        }
        return;
    }
    for(int i = s ; i <= num ; i++){
        a[ len ] = i;
        printSeq( num - i , a , len + 1 , i,p,holes);
    }
}

vector<string> all_permut(int p,int h,int dir,int x,int y){
    vector<string> bucket;
    if (h==0)
        return bucket;
    vector<vector<int>> bucket1;
    int a[p];
    printSeq(p,a,0,1,bucket1,h);
    for(int i=0;i<bucket1.size();i++)
    {
        string num="";
        num +=(char)p+48;
        num +=(char)y+97;
        num +=(char)(size-x)+48;
        char dirc;
        if(dir==2){dirc='>';}else if(dir==-2){dirc='<';}else if(dir==1){dirc='+';}else{dirc='-';}
        num += dirc;
        vector<int> n=bucket1.at(i);
        for(int j=0;j<n.size();j++)
        {
            char d = (char)(n.at(j)+48);
            num += d;
        }
        bucket.push_back(num);
    }
    return bucket;
}

vector<string> stack_split(tuple<int,int,int,int> a,State state){
    int x=get<0>(a);
    int y=get<1>(a);
    int top=get<2>(a);
    int sizes=get<3>(a);
    if(sizes>size){sizes=size;}
    std::vector<string> all,right;
    int h=0;
    if(y<size-1){
        for(int a1=y+1;a1<size;a1++){//while I move right
            if((!state.board[getCord(x, a1)].stk.empty()) && (state.board[getCord(x, a1)].stk.top()==3||state.board[getCord(x, a1)].stk.top()==-3||state.board[getCord(x, a1)].stk.top()==2||state.board[getCord(x, a1)].stk.top()==-2))
                break;
            h++;
        }
    }
    right=all_permut(sizes,h,2,x,y);
    all.insert(all.end(), right.begin(), right.end());
    right.clear();
    
    h=0;
    if(y>0){
        for(int a2=y-1;a2>=0;a2--){//left{
            if((!state.board[getCord(x, a2)].stk.empty()) && (state.board[getCord(x, a2)].stk.top()==3||state.board[getCord(x, a2)].stk.top()==-3||state.board[getCord(x, a2)].stk.top()==2||state.board[getCord(x, a2)].stk.top()==-2))
                break;
            h++;
        }
    }
    right=all_permut(sizes,h,-2,x,y);
    all.insert(all.end(), right.begin(), right.end());
    right.clear();
    
    h=0;
    if(x<size-1){
        for(int b1=x+1;b1<size;b1++){//bottom
            if((!state.board[getCord(b1, y)].stk.empty()) && (state.board[getCord(b1, y)].stk.top()==3||state.board[getCord(b1, y)].stk.top()==-3||state.board[getCord(b1, y)].stk.top()==2||state.board[getCord(b1, y)].stk.top()==-2))
                break;
            h++;
        }
        
    }
    right=all_permut(sizes,h,-1,x,y);
    all.insert(all.end(), right.begin(), right.end());
    right.clear();
    h=0;
    if(x>0){
        for(int b2=x-1;b2>=0;b2--){//up
            if((!state.board[getCord(b2, y)].stk.empty()) && (state.board[getCord(b2, y)].stk.top()==3||state.board[getCord(b2, y)].stk.top()==-3||state.board[getCord(b2, y)].stk.top()==2||state.board[getCord(b2, y)].stk.top()==-2))
                break;
            h++;
        }
        
        
    }
    right=all_permut(sizes,h,1,x,y);
    all.insert(all.end(), right.begin(), right.end());
    right.clear();
    h=0;
    return all;
    
    
}


vector<string> generate_moves(State &state,int player){
    vector<string> allmoves,possiblemoves;
    string str;
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if((!state.board[getCord(i, j)].stk.empty()) && (state.board[getCord(i, j)].stk.top()*player>0)){
                possiblemoves=stack_split(tuple<int,int,int,int>(i,j,state.board[getCord(i, j)].stk.top(),state.board[getCord(i, j)].stk.size()),state);
                allmoves.insert(allmoves.end(), possiblemoves.begin(), possiblemoves.end());
                possiblemoves.clear();
            }
            if(state.board[getCord(i, j)].stk.empty()){
                str = "F";
                str += (j+97);
                str += (size-i+48);
                allmoves.push_back(str);
                str = "S";
                str += (j+97);
                str += (size-i+48);
                allmoves.push_back(str);
                if((player>0 && state.noHCap!=0)||(player<0 && state.noACap!=0)){
                    str = "C";
                    str += (j+97);
                    str += (size-i+48);
                    allmoves.push_back(str);
                }
            }
            
        }
    }
    return allmoves;
}


// I play if player is positive, and the opponent plays if the player is negative
State getNewCopyOfState(State oldState, string move, int player){
    State state(size);
    //copying values from oldState to new State, state
    state.noA = oldState.noA;
    state.noH = oldState.noH;
    state.noACap = oldState.noACap;
    state.noHCap = oldState.noHCap;
    state.aWalls = oldState.aWalls;
    state.hWalls = oldState.hWalls;
    for(int i=0; i<size*size; i++)
        state.board[i].stk = oldState.board[i].stk;
    
    //finished making an exact duplicate of the old state
    
    if (move.find(">")!=string::npos) {
        int stoneCount = move[0]-48;
        stack<int> buffer;
        for (int i=0; i<stoneCount; i++) {
            buffer.push(state.board[getCord(size-(move[2]-48), move[1]-97)].stk.top());
            state.board[getCord(size-(move[2]-48), move[1]-97)].stk.pop();
        }
        int moveInd = 4;
        while (buffer.size()!=0) {
            int noMoving = move[moveInd] - 48;
            for (int i=0; i<noMoving; i++) {
                state.board[getCord(size-(move[2]-48), move[1]-97+(moveInd-3))].stk.push(buffer.top());
                buffer.pop();
            }
            moveInd++;
        }
    }else if (move.find("<")!=string::npos) {
        int stoneCount = move[0]-48;
        stack<int> buffer;
        for (int i=0; i<stoneCount; i++) {
            buffer.push(state.board[getCord(size-(move[2]-48), move[1]-97)].stk.top());
            state.board[getCord(size-(move[2]-48), move[1]-97)].stk.pop();
        }
        int moveInd = 4;
        while (buffer.size()!=0) {
            int noMoving = move[moveInd] - 48;
            for (int i=0; i<noMoving; i++) {
                state.board[getCord(size-(move[2]-48), move[1]-97-(moveInd-3))].stk.push(buffer.top());
                buffer.pop();
            }
            moveInd++;
        }
    }else if (move.find("-")!=string::npos) {
        int stoneCount = move[0]-48;
        stack<int> buffer;
        for (int i=0; i<stoneCount; i++) {
            buffer.push(state.board[getCord(size-(move[2]-48), move[1]-97)].stk.top());
            state.board[getCord(size-(move[2]-48), move[1]-97)].stk.pop();
        }
        int moveInd = 4;
        while (buffer.size()!=0) {
            int noMoving = move[moveInd] - 48;
            for (int i=0; i<noMoving; i++) {
                state.board[getCord(size-(move[2]-48)+(moveInd-3), move[1]-97)].stk.push(buffer.top());
                buffer.pop();
            }
            moveInd++;
        }
    }else if (move.find("+")!=string::npos) {
        int stoneCount = move[0]-48;
        stack<int> buffer;
        for (int i=0; i<stoneCount; i++) {
            buffer.push(state.board[getCord(size-(move[2]-48), move[1]-97)].stk.top());
            state.board[getCord(size-(move[2]-48), move[1]-97)].stk.pop();
        }
        int moveInd = 4;
        while (buffer.size()!=0) {
            int noMoving = move[moveInd] - 48;
            for (int i=0; i<noMoving; i++) {
                state.board[getCord(size-(move[2]-48)-(moveInd-3), move[1]-97)].stk.push(buffer.top());
                buffer.pop();
            }
            moveInd++;
        }
    }else{
        if(player<0){
            // The opponent plays a "Place" move if player is negative
            char type = move[0];
            if(type=='F'){
                state.board[getCord(size-(move[2]-48), move[1]-97)].stk.push(-1);
                state.noA--;
            }
            else if(type=='S'){
                state.board[getCord(size-(move[2]-48), move[1]-97)].stk.push(-2);
                state.aWalls.push_back((size-move[2])*size + (move[1]-97));
                state.noA--;
            }
            else{
                state.board[getCord(size-(move[2]-48), move[1]-97)].stk.push(-3);
                state.noACap--;
            }
        }else{
            // I play a move if player is positive
            char type = move[0];
            if(type=='F'){
                state.board[getCord(size-(move[2]-48), move[1]-97)].stk.push(1);
                state.noH--;
            }
            else if(type=='S'){
                state.board[getCord(size-(move[2]-48), move[1]-97)].stk.push(2);
                state.hWalls.push_back((size-move[2])*size + (move[1]-97));
                state.noH--;
            }
            else{
                state.board[getCord(size-(move[2]-48), move[1]-97)].stk.push(3);
                state.noHCap--;
            }
        }
    }
    return state;
}


// I play if player is positive, and the opponent plays if player is negative
vector<State> getSuccessors(State oldState, int player){
    vector<string> actions = generate_moves(oldState, player);
    vector<State> successors;
    for(int i=0; i<actions.size(); i++){
        string move = actions.at(i);
        State state = getNewCopyOfState(oldState, move, player);
        successors.push_back(state);
    }
    return successors;
}

/*
 * This method returns the utility of a given terminal state.
 */
int utility(State &state){
    int winType = isPathPresent2(state); //winType will definitely be greater than 0
    if(winType==1){
        // Flat Win
        int myCount = 0;
        int oppCount = 0;
        for (int i=0; i<size; i++)
            for (int j=0; j<size; j++)
                if(!state.board[getCord(i, j)].stk.empty() && state.board[getCord(i, j)].stk.top()==1)
                    myCount++;
                else if(!state.board[getCord(i, j)].stk.empty() && state.board[getCord(i, j)].stk.top()==-1)
                    oppCount++;
        int myScore = 0;
        int oppScore = 0;
        if(myCount>oppCount){
            myScore = state.noH + myCount;
            oppScore = oppCount;
        }else if(myCount<oppCount){
            myScore = myCount;
            oppScore = state.noA + oppCount;
        }else{
            myScore = state.noH;
            oppScore = state.noA;
        }
        int utility = myScore - oppScore;
        return utility;
    }else if(winType==2){
        // Opponent's Road Win
        int oppScore = size*size + state.noA;
        int myScore = 0;
        for (int i=0; i<size; i++)
            for (int j=0; j<size; j++)
                if(!state.board[getCord(i, j)].stk.empty() && state.board[getCord(i, j)].stk.top()==1)
                    myScore++;
        int utility = myScore - oppScore;
        return utility;
    }else{
        // Our Road Win
        int myScore = size*size + state.noH;
        int oppScore = 0;
        for (int i=0; i<size; i++)
            for (int j=0; j<size; j++)
                if(!state.board[getCord(i, j)].stk.empty() && state.board[getCord(i, j)].stk.top()==-1)
                    oppScore++;
        int utility = myScore - oppScore;
        return utility;
    }
}

bool DFSN(int i, int j, State &state, int** visited, int size, int type, int BorW,int max){
    visited[i][j] = 1;
    if(type==0 && i>((size-1)/2))
        return true;
    if(type==1 && j>((size-1)/2))
        return true;
    
    bool ret = false;
    
    if((i-1)>=0 && visited[i-1][j]==0 && !state.board[getCord(i-1,j)].stk.empty() && (state.board[getCord(i-1,j)].stk.top()==(1*BorW) || state.board[getCord(i-1,j)].stk.top()==(3*BorW)))
        ret = DFSN(i-1, j, state, visited, size, type, BorW,max+1);
    if(ret)
        return true;
    
    if((j-1)>=0 && visited[i][j-1]==0 && !state.board[getCord(i,j-1)].stk.empty() && (state.board[getCord(i,j-1)].stk.top()==(1*BorW) || state.board[getCord(i,j-1)].stk.top()==(3*BorW)))
        ret = DFSN(i, j-1, state, visited, size, type, BorW,max+1);
    if(ret)
        return true;
    
    if((i+1)<size && visited[i+1][j]==0 && !state.board[getCord(i+1,j)].stk.empty() && (state.board[getCord(i+1,j)].stk.top()==(1*BorW) || state.board[getCord(i+1,j)].stk.top()==(3*BorW)))
        ret = DFSN(i+1, j, state, visited, size, type, BorW,max+1);
    if(ret)
        return true;
    
    if((j+1)<size && visited[i][j+1]==0 && !state.board[getCord(i,j+1)].stk.empty() && (state.board[getCord(i,j+1)].stk.top()==(1*BorW) || state.board[getCord(i,j+1)].stk.top()==(3*BorW)))
        ret = DFSN(i, j+1, state, visited, size, type, BorW,max+1);
    return ret;
}
int isPathN(State &state, int size, int BorW){
    
    int max=0;
    int max_value=INT_MIN;
    
    int** visited = new int*[size];
    for(int i=0; i<size; i++)
        visited[i] = new int[size];
    for(int j=0; j<size; j++)
        if(!state.board[getCord(0,j)].stk.empty() && state.board[getCord(0,j)].stk.top()==1 && DFSN(0, j, state, visited, size, 0, BorW, max))
            if(max_value<max)
                max_value=max;max=0;
    
    for (int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            visited[i][j] = 0;
    for(int i=0; i<size; i++)
        if(!state.board[getCord(i,0)].stk.empty() && state.board[getCord(i,0)].stk.top()==1 && DFSN(i, 0, state, visited, size, 1, BorW, max))
            if(max_value<max)
                max_value=max;max=0;
    
    for(int i=0; i<size; i++)
        delete[] visited[i];
    delete visited;
    return 1+max_value;
}

float DFSTB(State state, int player, bool** visited, int row, int col){
    visited[row][col] = true;
    if(row==size-1)
        return 2*size;
    float length = 1.0;
    if((col-1)>=0 && !visited[row][col-1] && !state.board[getCord(row, col-1)].stk.empty() && (state.board[getCord(row, col-1)].stk.top()*player==1 || state.board[getCord(row, col-1)].stk.top()*player==3))
        length = max(length, 0.9f+DFSTB(state, player, visited, row, col-1));
    if((col+1)<size && !visited[row][col+1] && !state.board[getCord(row, col+1)].stk.empty() && (state.board[getCord(row, col+1)].stk.top()*player==1 || state.board[getCord(row, col+1)].stk.top()*player==3))
        length = max(length, 0.9f+DFSTB(state, player, visited, row, col+1));
    if((row+1)<size && !visited[row+1][col] && !state.board[getCord(row+1, col)].stk.empty() && (state.board[getCord(row+1, col)].stk.top()*player==1 || state.board[getCord(row+1, col)].stk.top()*player==3))
        length = max(length, 1.0f+DFSTB(state, player, visited, row+1, col));
    return length;
}

float DFSLR(State state, int player, bool** visited, int row, int col){
    visited[row][col] = true;
    if(col==size-1)
        return 2*size;
    float length = 1.0;
    if((row-1)>=0 && !state.board[getCord(row-1, col)].stk.empty() && (state.board[getCord(row-1, col)].stk.top()*player==1 || state.board[getCord(row-1, col)].stk.top()*player==3))
        length = max(length, 0.9f+DFSTB(state, player, visited, row-1, col));
    if((row+1)<size && !state.board[getCord(row+1, col)].stk.empty() && (state.board[getCord(row+1, col)].stk.top()*player==1 || state.board[getCord(row+1, col)].stk.top()*player==3))
        length = max(length, 0.9f+DFSTB(state, player, visited, row+1, col));
    if((col+1)<size && !state.board[getCord(row, col+1)].stk.empty() && (state.board[getCord(row, col+1)].stk.top()*player==1 || state.board[getCord(row, col+1)].stk.top()*player==3))
        length = max(length, 1.0f+DFSTB(state, player, visited, row, col+1));
    return length;
}
// UpDown_LeftRight = 0 implies Top to Bottom
// UpDown_LeftRight = 1 implies Left to Right
void pathAcross(State state,  int &white, int &black){
    bool** visited = new bool*[size];
    for(int i=0; i<size; i++){
        visited[i] = new bool[size];
        for(int j=0; j<size; j++)
            visited[i][j] = false;
    }
    
    float maxLW = 0.0f;
    float maxLB = 0.0f;
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if(!visited[i][j] && !state.board[getCord(i, j)].stk.empty() && (state.board[getCord(i, j)].stk.top()==1 || state.board[getCord(i, j)].stk.top()==3)){
                maxLW = max(maxLW, DFSTB(state, 1, visited, i, j));
                maxLW = max(maxLW, DFSLR(state, 1, visited, i, j));
            }else if(!visited[i][j] && !state.board[getCord(i, j)].stk.empty() && (state.board[getCord(i, j)].stk.top()==-1 || state.board[getCord(i, j)].stk.top()==-3)){
                maxLB = max(maxLB, DFSTB(state, -1, visited, i, j));
                maxLB = max(maxLB, DFSLR(state, -1, visited, i, j));
            }
        }
    }
    
    for(int i=0; i<size; i++)
        delete[] visited[i];
    delete [] visited;
    
    white = maxLW;
    black = maxLB;
}

/*
 * This is the evaluation function which returns the approximate desirability of the given state
 */
int evaluation(State &state){
    int w1 = 7000, w2 = 60, w3 = 20, w4=40, w5=100, eval = 0;
    int myRoad, oppRoad;
    int myStack=0;
    int oppStack=0;
    int isRoad = isPathPresent2(state);
    if(isRoad==2){
        myRoad = 0;
        oppRoad = 1;
    }else if(isRoad==3){
        myRoad = 1;
        oppRoad = 0;
    }else{
        myRoad = 0;
        oppRoad = 0;
    }
    
    int myCover = 0, oppCover = 0;
    for(int i=0; i<size; i++)
        for (int j=0; j<size; j++){
            // **NOTE: Pressence of ANY white piece at the top of a square own that square
            if(!state.board[getCord(i, j)].stk.empty() && (state.board[getCord(i, j)].stk.top()==1 || state.board[getCord(i, j)].stk.top()==3))
                myCover++;
            else if(!state.board[getCord(i, j)].stk.empty() && (state.board[getCord(i, j)].stk.top()==-1 || state.board[getCord(i, j)].stk.top()==-3))
                oppCover++;
            if(state.board[getCord(i, j)].stk.size()>=2 && state.board[getCord(i, j)].stk.top()>0)
                myStack=myStack+state.board[getCord(i, j)].stk.size();
            else if(state.board[getCord(i, j)].stk.size()>=2 && state.board[getCord(i, j)].stk.top()<0)
                oppStack=oppStack+state.board[getCord(i, j)].stk.size();
        }
    //    int WPath=isPathN(state,size,1);
    //    int BPath=isPathN(state,size,-1);
    int WPath = 0;
    int BPath = 0;
    pathAcross(state, WPath, BPath);
    eval = w1*(myRoad - oppRoad) + w2*(myCover - oppCover) + w3*(state.noH - state.noA) + w4*(myStack-oppStack) + w5*(WPath - BPath);
    
    return eval;
}

/*
 * This method takes the move played by the opponent and updates the local State
 */
//I play if player is not 1, and the opponent plays if the plater is 1
void updateState(State &state, string move, int player){
    if (move.find(">")!=string::npos) {
        int stoneCount = move[0]-48;
        stack<int> buffer;
        for (int i=0; i<stoneCount; i++) {
            buffer.push(state.board[getCord(size-(move[2]-48), move[1]-97)].stk.top());
            state.board[getCord(size-(move[2]-48), move[1]-97)].stk.pop();
        }
        int moveInd = 4;
        while (buffer.size()!=0) {
            int noMoving = move[moveInd] - 48;
            for (int i=0; i<noMoving; i++) {
                state.board[getCord(size-(move[2]-48),move[1]-97+(moveInd-3))].stk.push(buffer.top());
                buffer.pop();
            }
            moveInd++;
        }
    }else if (move.find("<")!=string::npos) {
        int stoneCount = move[0]-48;
        stack<int> buffer;
        for (int i=0; i<stoneCount; i++) {
            buffer.push(state.board[getCord(size-(move[2]-48), move[1]-97)].stk.top());
            state.board[getCord(size-(move[2]-48), move[1]-97)].stk.pop();
        }
        int moveInd = 4;
        while (buffer.size()!=0) {
            int noMoving = move[moveInd] - 48;
            for (int i=0; i<noMoving; i++) {
                state.board[getCord(size-(move[2]-48), move[1]-97-(moveInd-3))].stk.push(buffer.top());
                buffer.pop();
            }
            moveInd++;
        }
    }else if (move.find("-")!=string::npos) {
        int stoneCount = move[0]-48;
        stack<int> buffer;
        for (int i=0; i<stoneCount; i++) {
            buffer.push(state.board[getCord(size-(move[2]-48), move[1]-97)].stk.top());
            state.board[getCord(size-(move[2]-48), move[1]-97)].stk.pop();
        }
        int moveInd = 4;
        while (buffer.size()!=0) {
            int noMoving = move[moveInd] - 48;
            for (int i=0; i<noMoving; i++) {
                state.board[getCord(size-(move[2]-48)+(moveInd-3),move[1]-97)].stk.push(buffer.top());
                buffer.pop();
            }
            moveInd++;
        }
    }else if (move.find("+")!=string::npos) {
        int stoneCount = move[0]-48;
        stack<int> buffer;
        for (int i=0; i<stoneCount; i++) {
            buffer.push(state.board[getCord(size-(move[2]-48), move[1]-97)].stk.top());
            state.board[getCord(size-(move[2]-48), move[1]-97)].stk.pop();
        }
        int moveInd = 4;
        while (buffer.size()!=0) {
            int noMoving = move[moveInd] - 48;
            for (int i=0; i<noMoving; i++) {
                state.board[getCord(size-(move[2]-48)-(moveInd-3),move[1]-97)].stk.push(buffer.top());
                buffer.pop();
            }
            moveInd++;
        }
    }else{
        if(player==1){
            // The opponent plays a "Place" move
            char type = move[0];
            if(type=='F'){
                state.board[getCord(size-(move[2]-48), move[1]-97)].stk.push(-1);
                state.noA--;
            }
            else if(type=='S'){
                state.board[getCord(size-(move[2]-48), move[1]-97)].stk.push(-2);
                state.aWalls.push_back((size-move[2])*size + (move[1]-97));
                state.noA--;
            }
            else{
                state.board[getCord(size-(move[2]-48), move[1]-97)].stk.push(-3);
                state.noACap--;
            }
        }else{
            // I play a move
            char type = move[0];
            if(type=='F'){
                state.board[getCord(size-(move[2]-48), move[1]-97)].stk.push(1);
                state.noH--;
            }
            else if(type=='S'){
                state.board[getCord(size-(move[2]-48), move[1]-97)].stk.push(2);
                state.hWalls.push_back((size-move[2])*size + (move[1]-97));
                state.noH--;
            }
            else{
                state.board[getCord(size-(move[2]-48), move[1]-97)].stk.push(3);
                state.noHCap--;
            }
        }
    }
}

bool cutoff_test(State state, int depth){
    if(depth>MAX_DEPTH)
        return true;
    return isPathPresent2(state)!=0;
}

int min_value(State &state, int alpha, int beta, int depth);

int max_value(State &state, int alpha, int beta, int depth){
    if(cutoff_test(state, depth)) // if the current state is terminal
        return evaluation(state);
    vector<State> successors = getSuccessors(state, 1);
    for (State s:successors){
        int child = min_value(s, alpha, beta, depth+1);
        alpha = max(alpha, child);
        if(alpha>=beta){
            for (vector<State>::iterator it= successors.begin(); it!=successors.end(); ++it)
                delete[] (*it).board;
            return child;
        }
    }
    for (vector<State>::iterator it= successors.begin(); it!=successors.end(); ++it)
        delete[] (*it).board;
    return alpha;
}

int min_value(State &state, int alpha, int beta, int depth){
    if(cutoff_test(state, depth)) // if the current state is terminal
        return evaluation(state);
    vector<State> successors = getSuccessors(state, -1);
    for (State s:successors){
        int child = max_value(s, alpha, beta, depth+1);
        beta = min(beta, child);
        if(alpha>=beta){
            for (vector<State>::iterator it= successors.begin(); it!=successors.end(); ++it)
                delete[] (*it).board;
            return child;
        }
    }
    for (vector<State>::iterator it= successors.begin(); it!=successors.end(); ++it)
        delete[] (*it).board;
    return beta;
}

tuple<int, string> max_valueWithAction(State &state, int alpha, int beta){
    if(cutoff_test(state, 0)) // if the current state is terminal
        return make_tuple(evaluation(state), "");
    string action;
    vector<string> actions = generate_moves(state, 1);
    srand(time(NULL));
    random_shuffle(actions.begin(), actions.end());
    for (string move:actions){
        State nextState = getNewCopyOfState(state, move, 1);
        int child = min_value(nextState, alpha, beta, 1);
        if(alpha<child){
            alpha = child;
            action = move;
        }
        delete [] nextState.board;
        if(alpha>=beta)
            return make_tuple(child, action);
    }
    return make_tuple(alpha, action);
}

/*
 * This method returns the action leading to the most optimal state from a given state
 */
string minimax_decision(State &state){
    return get<1>(max_valueWithAction(state, INT_MIN, INT_MAX));
}

int main(int argc, const char * argv[]) {
    cin >> playerID >> size >> timeLimit;
    
    State curState(size);
    int turn;
    vector<string> initMoves = {};//"Fc3" ,"Fc2", "Fc4"};//,"Fb3", "Fd3", "Fc1", "Fc5", "Fa3", "Fe3", "Fd1"};
    //    srand(time(NULL));
    //    random_shuffle(initMoves.begin(), initMoves.end());
    
    if (playerID==1) {
        cout << "Fb2" << endl;
        updateState(curState, "Fb2", 1);
        string move; //move played by opponent to set my piece
        cin >> move;
        updateState(curState, move, 0);
        turn = 1;
    }else{
        string move;
        cin >> move;
        updateState(curState, move, 0);
        string myMove;
        if(move!="Fa1"){
            cout << "Fa1" << endl;  // move to place a white piece on board
            updateState(curState, "Fa1", 1);
        }else{
            cout << "Fa5" << endl;  // move to place a white piece on board
            updateState(curState, "Fa5", 1);
        }
        turn = 4;
    }
    
    while (isPathPresent2(curState)==0) {
        if (turn%2==0) {    // takes input for even turns
            string move;
            cin >> move;
            updateState(curState, move, 1);
            turn++;
            continue;
        }
        time_t startTime = time(NULL);
        
         //play hardcoded moves for the first 5 turns
                if(!initMoves.empty() && turn<=3){
                    string move;
                    while(!initMoves.empty()){
                        move = initMoves[initMoves.size()-1];
                        initMoves.pop_back();
                        if(curState.board[getCord(size-(move[2]-48), move[1]-97)].stk.empty()){
                            cout << move << endl;
                            updateState(curState, move, 0);
                            break;
                        }
                    }
                    turn++;
                    continue;
                }
        
        // makes move for odd turns;
        string myMove = minimax_decision(curState);
        //        cout << "About to update my state with the move: " << myMove << endl;
        cout << myMove << endl;
        updateState(curState, myMove, 0);
        turn++;
        
        time_t timeElapsed = time(NULL) - startTime;
        timeLimit -= timeElapsed;
        
        if(timeLimit < 17)
            MAX_DEPTH = 1;
    }
    return 0;
}
