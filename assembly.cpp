#include <iostream>
#include <list>
#include <stack>
#include <map>
#include <vector>
#include "quad.cpp"
#pragma once

using namespace std;

enum InstrKind{Add,Sub,Addim,Mult,Div,Mv,Loadi,Load,Store,Beq,Bne,Slt,Slti,And,Or,Not,Xor,Jump,Jumpr,Jal,Sr,Sl,mLO,mHI,Nop,Halt,In,Out,Writei,rstQnt,stpQnt};
enum Register{$zero,$t0,$t1,$t2,$t3,$t4,$t5,$t6,$t7,$t8,$t9,$t10,$t11,$t12,$t13,$t14,$t15,$a0,$a1,$a2,$a3,$a4,$a5,$a6,$a7,$a8,$aux,$pc,$v0,$sp,$gp,$ra};
enum Format{A,B,C,D};
enum LineKind{Label,Inst};

typedef struct InstructionRec{
    Format format;
    InstrKind instrKind;
    Register ra, rb, rc;
    LineKind lineKind;
    int immediate, lineNumber;
    string label;
}Instruction;

static list<Instruction> instructionList;
static list<string> busyRegisters;
stack<Register> parameters;
map<string,int> labelLine;

int lineCounter;
string currentFunction;
int memPosGlobal;
int memPosLocal;
int argCounter;


void insertInstructionA(InstrKind instrKind, Register ra, Register rb, Register rc, int lineNumber){
    Instruction newInstruction;
    newInstruction.instrKind = instrKind;
    newInstruction.format = A;
    newInstruction.ra = ra;
    newInstruction.rb = rb;
    newInstruction.rc = rc;
    newInstruction.lineKind = Inst;
    newInstruction.lineNumber = lineNumber;
    instructionList.push_back(newInstruction);
}

void insertInstructionB(InstrKind instrKind, Register ra, Register rb, int immed, int lineNumber){
    Instruction newInstruction;
    newInstruction.instrKind = instrKind;
    newInstruction.format = B;
    newInstruction.ra = ra;
    newInstruction.rb = rb;
    newInstruction.immediate = immed;
    newInstruction.lineKind = Inst;
    newInstruction.lineNumber = lineNumber;
    instructionList.push_back(newInstruction);
}

void insertInstructionC(InstrKind instrKind, Register ra, int immed, int lineNumber){
    Instruction newInstruction;
    newInstruction.instrKind = instrKind;
    newInstruction.format = C;
    newInstruction.ra = ra;
    newInstruction.immediate = immed;
    newInstruction.lineKind = Inst;
    newInstruction.lineNumber = lineNumber;
    instructionList.push_back(newInstruction);
}

void insertInstructionD(InstrKind instrKind, int immed, int lineNumber){
    Instruction newInstruction;
    newInstruction.instrKind = instrKind;
    newInstruction.format = C;
    newInstruction.immediate = immed;
    newInstruction.lineKind = Inst;
    newInstruction.lineNumber = lineNumber;
    instructionList.push_back(newInstruction);
}

void insertLabel(string label, int lineNumber){
    Instruction newInstruction;
    newInstruction.lineKind = Label;
    newInstruction.label = label;
    newInstruction.lineNumber = lineNumber;
    instructionList.push_back(newInstruction);
}

void insertJump(InstrKind instrKind, string label, int lineNumber){
    Instruction newInstruction;
    newInstruction.lineKind = Inst;
    newInstruction.instrKind = instrKind;
    newInstruction.label = label;
    newInstruction.lineNumber = lineNumber;
    instructionList.push_back(newInstruction);
}

void insertBeq_Bne(InstrKind instrKind, Register ra, Register rb, string label, int lineNumber){
    Instruction newInstruction;
    newInstruction.instrKind = instrKind;
    newInstruction.format = B;
    newInstruction.ra = ra;
    newInstruction.rb = rb;
    newInstruction.label = label;
    newInstruction.lineKind = Inst;
    newInstruction.lineNumber = lineNumber;
    instructionList.push_back(newInstruction);
}

void insertSpInstruction(int mem){
    Instruction newInstruction;
    newInstruction.format = B;
    newInstruction.instrKind = Addim;
    newInstruction.ra = $sp;
    newInstruction.rb = $gp;
    newInstruction.immediate = mem;
    newInstruction.lineKind = Inst;
    newInstruction.lineNumber = 2;
    instructionList.push_front(newInstruction);
}

Register stringToRegister(string reg){ 
    if(reg == "_t0") return $t0;
    if(reg == "_t1") return $t1;
    if(reg == "_t2") return $t2;
    if(reg == "_t3") return $t3;
    if(reg == "_t4") return $t4;
    if(reg == "_t5") return $t5;
    if(reg == "_t6") return $t6;
    if(reg == "_t7") return $t7;
    if(reg == "_t8") return $t8;
    if(reg == "_t9") return $t9;
    if(reg == "_t10") return $t10;
    if(reg == "_t11") return $t11;
    if(reg == "_t12") return $t12;
    if(reg == "_t13") return $t13;
    if(reg == "_t14") return $t14;
    if(reg == "_t15") return $t15;
}

Register getParamRegister(int n){
    switch(n){
        case 0: return $a0;
        case 1: return $a1;
        case 2: return $a2;
        case 3: return $a3;
        case 4: return $a4;
        case 5: return $a5;
        case 6: return $a6;
        case 7: return $a7;
        case 8: return $a8;
    }
}

string registerToString(Register reg){
    switch(reg){
        case $zero: return "$zero";
        case $t0: return "$t0";
        case $t1: return "$t1";
        case $t2: return "$t2";
        case $t3: return "$t3";
        case $t4: return "$t4";
        case $t5: return "$t5";
        case $t6: return "$t6";
        case $t7: return "$t7";
        case $t8: return "$t8";
        case $t9: return "$t9";
        case $t10: return "$t10";
        case $t11: return "$t11";
        case $t12: return "$t12";
        case $t13: return "$t13";
        case $t14: return "$t14";
        case $t15: return "$t15";
        case $a0: return "$a0";
        case $a1: return "$a1";
        case $a2: return "$a2";
        case $a3: return "$a3";
        case $a4: return "$a4";
        case $a5: return "$a5";
        case $a6: return "$a6";
        case $a7: return "$a7";
        case $a8: return "$a8";
        case $aux: return "$aux";
        case $pc: return "$pc";
        case $v0: return "$v0";
        case $sp: return "$sp";
        case $gp: return "$gp";
        case $ra: return "$ra";
    }
}

Register getNextTempRegister(Register reg){
    switch(reg){
        case $t0: return $t1;
        case $t1: return $t2;
        case $t2: return $t3;
        case $t3: return $t4;
        case $t4: return $t5;
        case $t5: return $t6;
        case $t6: return $t7;
        case $t7: return $t8;
        case $t8: return $t9;
        case $t9: return $t10;
        case $t10: return $t11;
        case $t11: return $t12;
        case $t12: return $t13;
        case $t13: return $t14;
        case $t14: return $t15;
        case $t15: return $t0;
    }
}

void generateAssembly(){
    list<Quad>::iterator it;
    currentFunction = " ";
    memPosGlobal = 0;
    memPosLocal;
    argCounter = 0;
    lineCounter = 3;
    for(it = quadList.begin(); it != quadList.end(); it ++){
        if(it->op == "goto"){
            insertJump(Jump,it->arg1,lineCounter);
            lineCounter++;
        }
        else if(it->op == "fun"){
            memPosLocal = 0;
            argCounter = 0;
            currentFunction = it->arg1;
            insertLabel(currentFunction,lineCounter);
            labelLine.insert(pair<string,int>(it->arg1,lineCounter));
        }else if(it->op == "label"){
            insertLabel(it->arg1,lineCounter);
            labelLine.insert(pair<string,int>(it->arg1,lineCounter));
        }else if(it->op == "endfun"){
            if(getDataType(currentFunction," ") == Void && currentFunction != "main"){
                insertInstructionC(Jumpr,$ra,0,lineCounter);
                lineCounter++;
            }
            currentFunction = " ";
        }else if(it->op == "alloc"){
            if(currentFunction == " "){
                defMemPos(it->arg1," ",memPosGlobal);
                memPosGlobal += stoi(it->arg2);
            }else{
                defMemPos(it->arg1,currentFunction,memPosLocal);
                memPosLocal += stoi(it->arg2);
            }
        }
        else if(it->op == "load"){
            if(isGlobal(it->arg1)){
                if(it->arg2 == " "){
                    if(getMemLoc(it->arg1," ") == 1){
                        insertInstructionB(Load,stringToRegister(it->arg3),$gp,getMemPos(it->arg1," "),lineCounter);
                        lineCounter++;
                    }else{
                        insertInstructionB(Mv,stringToRegister(it->arg3),$gp,0,lineCounter);
                        lineCounter++;
                        insertInstructionB(Addim,stringToRegister(it->arg3),stringToRegister(it->arg3),getMemPos(it->arg1," "),lineCounter);
                        lineCounter++;
                    }
                }else{
                    insertInstructionA(Add,stringToRegister(it->arg2),$gp,stringToRegister(it->arg2),lineCounter);
                    lineCounter++;
                    insertInstructionB(Load,stringToRegister(it->arg3),stringToRegister(it->arg2),getMemPos(it->arg1," "),lineCounter);
                    lineCounter++;
                }
            }else{
                if(it->arg2 == " "){
                    if(getMemLoc(it->arg1,currentFunction) == 1){
                        insertInstructionB(Load,stringToRegister(it->arg3),$sp,getMemPos(it->arg1,currentFunction),lineCounter);
                        lineCounter++;
                    }else{
                        if(!isArg(it->arg1,currentFunction)){
                            insertInstructionB(Mv,stringToRegister(it->arg3),$sp,0,lineCounter);
                            lineCounter++;
                            insertInstructionB(Addim,stringToRegister(it->arg3),stringToRegister(it->arg3),getMemPos(it->arg1,currentFunction),lineCounter);
                            lineCounter++;
                        }else{
                            insertInstructionB(Load,stringToRegister(it->arg3),$sp,getMemPos(it->arg1,currentFunction),lineCounter);
                            lineCounter++;
                        }
                    }

                }else{
                    if(!isArg(it->arg1,currentFunction)){
                        insertInstructionA(Add,stringToRegister(it->arg2),$sp,stringToRegister(it->arg2),lineCounter);
                        lineCounter++;
                        insertInstructionB(Load,stringToRegister(it->arg3),stringToRegister(it->arg2),getMemPos(it->arg1,currentFunction),lineCounter);
                        lineCounter++;
                    }else{
                        insertInstructionB(Load,stringToRegister(it->arg3),$sp,getMemPos(it->arg1,currentFunction),lineCounter);
                        lineCounter++;
                        insertInstructionA(Add,stringToRegister(it->arg2),stringToRegister(it->arg3),stringToRegister(it->arg2),lineCounter);
                        lineCounter++;
                        insertInstructionB(Load,stringToRegister(it->arg3),stringToRegister(it->arg2),0,lineCounter);
                        lineCounter++;
                    }
                }
            }
            busyRegisters.push_back(it->arg3);
            if(it->arg2 != " ") busyRegisters.remove(it->arg2);
        }else if(it->op == "immed"){
            insertInstructionC(Loadi,stringToRegister(it->arg3),stoi(it->arg1),lineCounter);
            lineCounter++;
            busyRegisters.push_back(it->arg3);
        }else if(it->op == "asn"){
            insertInstructionB(Mv,stringToRegister(it->arg3),stringToRegister(it->arg1),0,lineCounter);
            lineCounter++;
            busyRegisters.push_back(it->arg3);
            busyRegisters.remove(it->arg1);
        }else if(it->op == "store"){
            if(isGlobal(it->arg3)){
                if(it->arg2 == " "){
                    insertInstructionB(Store,stringToRegister(it->arg1),$gp,getMemPos(it->arg3," "),lineCounter);
                    lineCounter++;
                }else{
                    insertInstructionA(Add,stringToRegister(it->arg2),$gp,stringToRegister(it->arg2),lineCounter);
                    lineCounter++;
                    insertInstructionB(Store,stringToRegister(it->arg1),stringToRegister(it->arg2),getMemPos(it->arg3," "),lineCounter);
                    lineCounter++;
                }
            }else{
                if(it->arg2 == " "){
                    insertInstructionB(Store,stringToRegister(it->arg1),$sp,getMemPos(it->arg3,currentFunction),lineCounter);
                    lineCounter++;
                }else{
                    if(!isArg(it->arg3,currentFunction)){
                        insertInstructionA(Add,stringToRegister(it->arg2),$sp,stringToRegister(it->arg2),lineCounter);
                        lineCounter++;
                        insertInstructionB(Store,stringToRegister(it->arg1),stringToRegister(it->arg2),getMemPos(it->arg3,currentFunction),lineCounter);
                        lineCounter++;
                    }else{
                        insertInstructionB(Load,getNextTempRegister(stringToRegister(it->arg2)),$sp,getMemPos(it->arg3,currentFunction),lineCounter);
                        lineCounter++;
                        insertInstructionA(Add,stringToRegister(it->arg2),getNextTempRegister(stringToRegister(it->arg2)),stringToRegister(it->arg2),lineCounter);
                        lineCounter++;
                        insertInstructionB(Store,stringToRegister(it->arg1),stringToRegister(it->arg2),0,lineCounter);
                        lineCounter++;
                    }
                }
            }
            busyRegisters.remove(it->arg1);
            if(it->arg2 != " ") busyRegisters.remove(it->arg2);
        }else if(it->op == "param"){
            parameters.push(stringToRegister(it->arg1));
        }else if(it->op == "call"){
            if(it->arg1 == "input"){
                insertInstructionC(In,stringToRegister(it->arg3),0,lineCounter);
                lineCounter++;
                busyRegisters.push_back(it->arg3);
            }else{
                int n = stoi(it->arg2);
                Register rTemp;
                for(int i = n-1; i >= 0; i--){
                    if(parameters.empty()){
                        cout << "Passagem errada de parâmetros na funcao " << it->arg1;
                        exit(-1);
                    }
                    rTemp = parameters.top();
                    busyRegisters.remove(registerToString(rTemp).replace(0,1,"_"));
                    parameters.pop();
                    insertInstructionB(Mv,getParamRegister(i),rTemp,0,lineCounter);
                    lineCounter++;   
                }
                insertInstructionB(Store,$ra,$sp,memPosLocal,lineCounter);
                lineCounter++;
                list<string>::iterator tempIt;
                int tempCounter = 0;
                for(tempIt = busyRegisters.begin(); tempIt != busyRegisters.end(); tempIt++){
                    tempCounter++;
                    insertInstructionB(Store,stringToRegister(*tempIt),$sp,memPosLocal+tempCounter,lineCounter);
                    lineCounter++;
                }
                insertInstructionB(Addim,$sp,$sp,memPosLocal+1 + tempCounter,lineCounter);
                lineCounter++;
                insertJump(Jal,it->arg1,lineCounter);
                lineCounter++;
                insertInstructionB(Addim,$sp,$sp,-(memPosLocal+1+tempCounter),lineCounter);
                lineCounter++;
                for(auto tempItR = busyRegisters.rbegin(); tempItR != busyRegisters.rend(); tempItR++){
                    insertInstructionB(Load,stringToRegister(*tempItR),$sp,memPosLocal+tempCounter,lineCounter);
                    lineCounter++;
                    tempCounter--;
                }
                insertInstructionB(Load,$ra,$sp,memPosLocal,lineCounter);
                lineCounter++;
                if(it->arg3 != " "){ 
                    busyRegisters.push_back(it->arg3);
                    insertInstructionB(Mv,stringToRegister(it->arg3),$v0,0,lineCounter); lineCounter++;
                }
            }
        }else if(it->op == "+"){
            insertInstructionA(Add,stringToRegister(it->arg1),stringToRegister(it->arg2),stringToRegister(it->arg3),lineCounter);
            lineCounter++;
            busyRegisters.push_back(it->arg3);
            busyRegisters.remove(it->arg1);
            busyRegisters.remove(it->arg2);
        }else if(it->op == "-"){
            insertInstructionA(Sub,stringToRegister(it->arg1),stringToRegister(it->arg2),stringToRegister(it->arg3),lineCounter);
            lineCounter++;
            busyRegisters.push_back(it->arg3);
            busyRegisters.remove(it->arg1);
            busyRegisters.remove(it->arg2);
        }else if(it->op == "*"){
            insertInstructionB(Mult,stringToRegister(it->arg1),stringToRegister(it->arg2),0,lineCounter);
            busyRegisters.push_back(it->arg3);
            busyRegisters.remove(it->arg1);
            busyRegisters.remove(it->arg2);
            lineCounter++;
            insertInstructionC(mLO,stringToRegister(it->arg3),0,lineCounter);
            lineCounter++;
        }else if(it->op == "/"){
            insertInstructionB(Div,stringToRegister(it->arg1),stringToRegister(it->arg2),0,lineCounter);
            lineCounter++;
            insertInstructionC(mHI,stringToRegister(it->arg3),0,lineCounter);
            lineCounter++;
            busyRegisters.push_back(it->arg3);
            busyRegisters.remove(it->arg1);
            busyRegisters.remove(it->arg2);
        }else if(it->op == "=="){
            Register r1, r2;
            busyRegisters.remove(it->arg1);
            busyRegisters.remove(it->arg2);
            r1 = stringToRegister(it->arg1);
            r2 = stringToRegister(it->arg2);
            it++;
            if(it->op == "if_t"){insertBeq_Bne(Beq,r1,r2,it->arg2,lineCounter); lineCounter++;}
            else{insertBeq_Bne(Bne,r1,r2,it->arg2,lineCounter); lineCounter++;}
        }else if(it->op == "!="){
            Register r1, r2;
            r1 = stringToRegister(it->arg1);
            r2 = stringToRegister(it->arg2);
            it++;
            busyRegisters.remove(it->arg1);
            busyRegisters.remove(it->arg2);
            if(it->op == "if_t"){insertBeq_Bne(Bne,r1,r2,it->arg2,lineCounter); lineCounter++;}
            else{insertBeq_Bne(Beq,r1,r2,it->arg2,lineCounter); lineCounter++;}            
        }else if(it->op == "<"){
            insertInstructionA(Slt,stringToRegister(it->arg1),stringToRegister(it->arg2),stringToRegister(it->arg3),lineCounter);
            lineCounter++;
            busyRegisters.push_back(it->arg3);
            busyRegisters.remove(it->arg1);
            busyRegisters.remove(it->arg2);
        }else if(it->op == ">"){
            insertInstructionA(Slt,stringToRegister(it->arg2),stringToRegister(it->arg1),stringToRegister(it->arg3),lineCounter);
            lineCounter++;
            busyRegisters.push_back(it->arg3);
            busyRegisters.remove(it->arg1);
            busyRegisters.remove(it->arg2);
        }else if(it->op == "<="){
            insertInstructionB(Addim,stringToRegister(it->arg2),stringToRegister(it->arg2),1,lineCounter);
            lineCounter++;
            insertInstructionA(Slt,stringToRegister(it->arg1),stringToRegister(it->arg2),stringToRegister(it->arg3),lineCounter);
            lineCounter++;
            busyRegisters.push_back(it->arg3);
            busyRegisters.remove(it->arg1);
            busyRegisters.remove(it->arg2);
        }else if(it->op == ">="){
            insertInstructionB(Addim,stringToRegister(it->arg1),stringToRegister(it->arg1),1,lineCounter);
            lineCounter++;
            insertInstructionA(Slt,stringToRegister(it->arg2),stringToRegister(it->arg1),stringToRegister(it->arg3),lineCounter);
            lineCounter++;
            busyRegisters.push_back(it->arg3);
            busyRegisters.remove(it->arg1);
            busyRegisters.remove(it->arg2);
        }else if(it->op == "if_t"){
            insertBeq_Bne(Bne,stringToRegister(it->arg1),$zero,it->arg2,lineCounter);
            lineCounter++;
            busyRegisters.remove(it->arg1);
        }else if(it->op == "if_f"){
            insertBeq_Bne(Beq,stringToRegister(it->arg1),$zero,it->arg2,lineCounter);
            lineCounter++;
            busyRegisters.remove(it->arg1);
        }else if(it->op == "ret"){
            insertInstructionB(Mv,$v0,stringToRegister(it->arg1),0,lineCounter);
            lineCounter++;
            insertInstructionC(Jumpr,$ra,0,lineCounter);
            lineCounter++;
            busyRegisters.remove(it->arg1);
        }else if(it->op == "arg"){
            defMemPos(it->arg1,currentFunction,memPosLocal);
            memPosLocal += 1;
            insertInstructionB(Store,getParamRegister(argCounter),$sp,argCounter,lineCounter);
            lineCounter++;
            argCounter++;
        }else if(it->op == "output"){
            insertInstructionC(Out,stringToRegister(it->arg1),stoi(it->arg2),lineCounter);
            lineCounter++;
            busyRegisters.remove(it->arg1);
        }else if(it->op == "loadAddr"){
            insertInstructionB(Load,stringToRegister(it->arg3),stringToRegister(it->arg1),0,lineCounter);
            lineCounter++;
        }else if(it->op == "jumpAddr"){
            insertInstructionC(Jumpr,stringToRegister(it->arg1),0,lineCounter);
            lineCounter++;
        }else if(it->op == "writei"){
            insertInstructionB(Writei,stringToRegister(it->arg1),stringToRegister(it->arg3),0,lineCounter);
            lineCounter++;
        }else if(it->op == "storeReg"){
            insertInstructionD(stpQnt, 0, lineCounter);
            lineCounter++;
            // insertInstructionB(Mv, $aux, stringToRegister(it->arg1), 0, lineCounter);
            // lineCounter++;
            for(int i = 0; i < 32; i++){
                insertInstructionB(Store, static_cast<Register>(i), stringToRegister(it->arg1), i, lineCounter);
                lineCounter++;
            }
        }else if(it->op == "loadReg"){
            insertInstructionB(Mv, $aux, stringToRegister(it->arg1), 0, lineCounter);
            lineCounter++;
            for(int i = 0; i < 32; i++){
                if(i != 26){
                    insertInstructionB(Load, static_cast<Register>(i), $aux, i, lineCounter);
                    lineCounter++;
                }
            }
            insertInstructionD(rstQnt, 0, lineCounter);
            lineCounter++;
            insertInstructionC(Jumpr, $pc, 0, lineCounter);
            lineCounter++;
        }else if(it->op == "loadRegKernel"){
            insertInstructionB(Mv, $aux, stringToRegister(it->arg1), 0, lineCounter);
            lineCounter++;
            for(int i = 0; i < 32; i++){
                if(i != 26){
                    insertInstructionB(Load, static_cast<Register>(i), $aux, i, lineCounter);
                    lineCounter++;
                }
            }
            // insertInstructionC(Jumpr, $pc, 0, lineCounter);
            // lineCounter++;
        }else if(it->op == "initReg"){
            insertInstructionB(Store, stringToRegister(it->arg2), stringToRegister(it->arg1), 27, lineCounter);
            lineCounter++;
            insertInstructionB(Store, stringToRegister(it->arg2), stringToRegister(it->arg1), 30, lineCounter);
            lineCounter++;
        }else if(it->op == "storeCurReg"){
            for(int i = 0; i < 32; i++){
                insertInstructionB(Store, static_cast<Register>(i), $aux, i, lineCounter);
                lineCounter++;
            }
        }else if(it->op == "stpQnt"){
            insertInstructionD(stpQnt, 0, lineCounter);
            lineCounter++;
        }else if(it->op == "rstQnt"){
            insertInstructionD(rstQnt, 0, lineCounter);
            lineCounter++;
        }
    }
    insertSpInstruction(memPosGlobal);
    insertInstructionD(Halt,0,lineCounter);
}

void printAssembly(){
    list<Instruction>::iterator it;
    string assemblyCode;
    for(it = instructionList.begin(); it != instructionList.end(); it++){
        if(it->lineKind == Label){
            assemblyCode += "." + it->label + "\n";
        }else{
            assemblyCode += to_string(it->lineNumber) + ":";
            switch(it->instrKind){
                case Jump:
                    assemblyCode += "   Jump " + it->label + "\n";
                    break;
                case Load:
                    assemblyCode += "   Load " + registerToString(it->ra) + "," + registerToString(it->rb) + "," + to_string(it->immediate) + "\n";
                    break;
                case Store:
                    assemblyCode += "   Store " + registerToString(it->ra) + "," + registerToString(it->rb) + "," + to_string(it->immediate) + "\n";
                    break;
                case Loadi:
                    assemblyCode += "   Loadi " + registerToString(it->ra) + "," + to_string(it->immediate) + "\n";
                    break;
                case Out:
                    assemblyCode += "   Out " + registerToString(it->ra) + "," + to_string(it->immediate) + "\n";
                    break;
                case Mv:
                    assemblyCode += "   Mv " + registerToString(it->ra) + "," + registerToString(it->rb) + "\n";
                    break;
                case In:
                    assemblyCode += "   In " + registerToString(it->ra) + "\n";
                    break;
                case Add:
                    assemblyCode += "   Add " + registerToString(it->ra) + "," + registerToString(it->rb) + "," + registerToString(it->rc) + "\n";
                    break;
                case Sub:
                    assemblyCode += "   Sub " + registerToString(it->ra) + "," + registerToString(it->rb) + "," + registerToString(it->rc) + "\n";
                    break;
                case Mult:
                    assemblyCode += "   Mult " + registerToString(it->ra) + "," + registerToString(it->rb) + "\n";
                    break;
                case mLO:
                    assemblyCode += "   mLo " + registerToString(it->ra) + "\n";
                    break;
                case Div:
                    assemblyCode += "   Div " + registerToString(it->ra) + "," + registerToString(it->rb) + "\n";
                    break;
                case mHI:
                    assemblyCode += "   mHi " + registerToString(it->ra) + "\n";
                    break;
                case Beq:
                    assemblyCode += "   Beq " + registerToString(it->ra) + "," + registerToString(it->rb) + "," + it->label + "\n";
                    break;
                case Bne:
                    assemblyCode += "   Bne " + registerToString(it->ra) + "," + registerToString(it->rb) + "," + it->label + "\n";
                    break;
                case Slt:
                    assemblyCode += "   Slt " + registerToString(it->ra) + "," + registerToString(it->rb) + "," + registerToString(it->rc) + "\n";
                    break;
                case Addim:
                     assemblyCode += "   Addim " + registerToString(it->ra) + "," + registerToString(it->rb) + "," + to_string(it->immediate) +"\n";
                    break;
                case Jal:
                    assemblyCode += "   Jal " + it->label + "\n";
                    break;
                case Jumpr:
                    assemblyCode += "   Jumpr " + registerToString(it->ra) + "\n";
                    break;
                case Halt:
                    assemblyCode += "   Halt\n";
                    break;
                case Writei:
                    assemblyCode += "   Writei " + registerToString(it->ra) + "," + registerToString(it->rb) + "," + to_string(it->immediate) + "\n";
                    break;
                case rstQnt:
                    assemblyCode += "   rstQnt\n";
                    break;
                case stpQnt:
                    assemblyCode += "   stpQnt\n";
                    break;
            }
        }
    }
    ofstream assemblyCodeFile;
    assemblyCodeFile.open("./outputs/assemblyCode");
    assemblyCodeFile << assemblyCode;
    assemblyCodeFile.close();
}