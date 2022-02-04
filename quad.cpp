#include <iostream>
#include <list>

#pragma once

using namespace std;

typedef struct QuadRec{
    string op, arg1, arg2, arg3;
}Quad;

static list <Quad> quadList;

void insertQuad(string op, string arg1, string arg2, string arg3){
    Quad newQuad;
    if(quadList.size() == 0){
        newQuad.op = "goto";
        newQuad.arg1 = "main";
        newQuad.arg2 = " ";
        newQuad.arg3 = " ";
        quadList.push_back(newQuad);
    }
    newQuad.op = op;
    newQuad.arg1 = arg1;
    newQuad.arg2 = arg2;
    newQuad.arg3 = arg3;
    quadList.push_back(newQuad);
}

void showQuadList(){
    list<Quad>::iterator it;
    string quad;
    for(it = quadList.begin(); it != quadList.end(); it++){
       quad += "(" + it->op + "," + it->arg1 + "," + it->arg2 + "," + it->arg3 + ")" + "\n";
    }
    ofstream quadCodeFile;
    quadCodeFile.open("./outputs/quadCode");
    quadCodeFile << quad;
    quadCodeFile.close();
}




