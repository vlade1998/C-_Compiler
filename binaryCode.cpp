#include <iostream>
#include <list>
#include <bitset>
#include "assembly.cpp"
#pragma once

using namespace std;

typedef struct BinaryInstRec{
    int opCode, ra, rb, rc, imed;
    Format format;
}BinaryInst;

static list<BinaryInst> binaryCode;

void insertInstructionA(int opCode, int ra, int rb, int rc){
    BinaryInst newInstruction;
    newInstruction.opCode = opCode;
    newInstruction.ra = ra;
    newInstruction.rb = rb;
    newInstruction.rc = rc;
    newInstruction.format = A;
    binaryCode.push_back(newInstruction);
}

void insertInstructionB(int opCode, int ra, int rb, int imed){
    BinaryInst newInstruction;
    newInstruction.opCode = opCode;
    newInstruction.ra = ra;
    newInstruction.rb = rb;
    newInstruction.imed = imed;
    newInstruction.format = B;
    binaryCode.push_back(newInstruction);
}

void insertInstructionC(int opCode, int ra, int imed){
    BinaryInst newInstruction;
    newInstruction.opCode = opCode;
    newInstruction.ra = ra;
    newInstruction.imed = imed;
    newInstruction.format = C;
    binaryCode.push_back(newInstruction);
}

void insertInstructionD(int opCode, int imed){
    BinaryInst newInstruction;
    newInstruction.opCode = opCode;
    newInstruction.imed = imed;
    newInstruction.format = D;
    binaryCode.push_back(newInstruction);
}

int registerToInt(Register reg){
    switch(reg){
        case $zero: return 0;
        case $t0: return 1;
        case $t1: return 2;
        case $t2: return 3;
        case $t3: return 4;
        case $t4: return 5;
        case $t5: return 6;
        case $t6: return 7;
        case $t7: return 8;
        case $t8: return 9;
        case $t9: return 10;
        case $t10: return 11;
        case $t11: return 12;
        case $t12: return 13;
        case $t13: return 14;
        case $t14: return 15;
        case $t15: return 16;
        case $a0: return 17;
        case $a1: return 18;
        case $a2: return 19;
        case $a3: return 20;
        case $a4: return 21;
        case $a5: return 22;
        case $a6: return 23;
        case $a7: return 24;
        case $a8: return 25;
        case $aux: return 26;
        case $pc: return 27;
        case $v0: return 28;
        case $sp: return 29;
        case $gp: return 30;
        case $ra: return 31;
    }
}


void generateBinaryCode(){
    list<Instruction>::iterator it;
    for(it = instructionList.begin(); it!= instructionList.end(); it++){
        if(it->lineKind == Inst){
            switch(it->instrKind){
                case Nop:
                    insertInstructionD(0,0);
                    break;
                case Addim:
                    insertInstructionB(1,registerToInt(it->ra),registerToInt(it->rb),it->immediate);
                    break;
                case Sub:
                    insertInstructionA(2,registerToInt(it->ra),registerToInt(it->rb),registerToInt(it->rc));
                    break;
                case Mult:
                    insertInstructionB(3,registerToInt(it->ra),registerToInt(it->rb),it->immediate);
                    break;
                case Div:
                    insertInstructionB(4,registerToInt(it->ra),registerToInt(it->rb),it->immediate);
                    break;
                case And:
                    insertInstructionA(5,registerToInt(it->ra),registerToInt(it->rb),registerToInt(it->rc));
                    break;
                case Or:
                    insertInstructionA(6,registerToInt(it->ra),registerToInt(it->rb),registerToInt(it->rc));
                    break;
                case Xor:
                    insertInstructionA(7,registerToInt(it->ra),registerToInt(it->rb),registerToInt(it->rc));
                    break;
                case Not:
                    insertInstructionB(8,registerToInt(it->ra),registerToInt(it->rb),it->immediate);
                    break;
                case Sr:
                    insertInstructionB(9,registerToInt(it->ra),registerToInt(it->rb),it->immediate);
                    break;
                case Sl:
                    insertInstructionB(10,registerToInt(it->ra),registerToInt(it->rb),it->immediate);
                    break;
                case Mv:
                    insertInstructionB(11,registerToInt(it->ra),registerToInt(it->rb),it->immediate);
                    break;
                case mLO:
                    insertInstructionC(12,registerToInt(it->ra),it->immediate);
                    break;
                case mHI:
                    insertInstructionC(13,registerToInt(it->ra),it->immediate);
                    break;
                case Load:
                    insertInstructionB(14,registerToInt(it->ra),registerToInt(it->rb),it->immediate);
                    break;
                case Loadi:
                    insertInstructionC(15,registerToInt(it->ra),it->immediate);
                    break;
                case Store:
                    insertInstructionB(16,registerToInt(it->ra),registerToInt(it->rb),it->immediate);
                    break;
                case Beq:
                    insertInstructionB(17,registerToInt(it->ra),registerToInt(it->rb),labelLine.find(it->label)->second - it->lineNumber - 1);
                    break;
                case Bne:
                    insertInstructionB(18,registerToInt(it->ra),registerToInt(it->rb),labelLine.find(it->label)->second - it->lineNumber - 1);
                    break;
                case Slt:
                    insertInstructionA(19,registerToInt(it->ra),registerToInt(it->rb),registerToInt(it->rc));
                    break;
                case Slti:
                    insertInstructionB(20,registerToInt(it->ra),registerToInt(it->rb),it->immediate);
                    break;
                case Jump:
                    insertInstructionD(21,labelLine.find(it->label)->second);
                    break;
                case Jumpr:
                    insertInstructionC(22,registerToInt(it->ra),it->immediate);
                    break;
                case Jal:
                    insertInstructionD(23,labelLine.find(it->label)->second);
                    break;
                case In:
                    insertInstructionC(24,registerToInt(it->ra),it->immediate);
                    break;
                case Out:
                    insertInstructionC(25,registerToInt(it->ra),it->immediate);
                    break;
                case Add:
                    insertInstructionA(26,registerToInt(it->ra),registerToInt(it->rb),registerToInt(it->rc));
                    break;
                case Halt:
                    insertInstructionD(27,0);
                    break;
                case Writei:
                    insertInstructionB(28,registerToInt(it->ra),registerToInt(it->rb),it->immediate);
                    break;
                default:
                    break;
            }
        }
    }
}

void printBinaryCode(){
    list<BinaryInst>::iterator it;
    string binaryCodeString;
    int line = 1;
    for(it = binaryCode.begin(); it!= binaryCode.end(); it++){
        switch(it->format){
            case A:
                binaryCodeString += "ram[" + to_string(line) + "] = {6'b" + bitset<6>(it->opCode).to_string() + ",5'b" + bitset<5>(it->ra).to_string() + ",5'b" + bitset<5>(it->rb).to_string() + ",5'b" + bitset<5>(it->rc).to_string() + ",11'b" + bitset<11>(0).to_string() + "};\n";
                line++;
                break;
            case B: 
                binaryCodeString += "ram[" + to_string(line) + "] = {6'b" + bitset<6>(it->opCode).to_string() + ",5'b" + bitset<5>(it->ra).to_string() + ",5'b" + bitset<5>(it->rb).to_string() + ",16'b" + bitset<16>(it->imed).to_string() + "};\n";
                line++;
                break;
            case C:
                binaryCodeString += "ram[" + to_string(line) + "] = {6'b" + bitset<6>(it->opCode).to_string() + ",5'b" + bitset<5>(it->ra).to_string() + ",21'b" + bitset<21>(it->imed).to_string() + "};\n";
                line++;
                break;
            case D:
                binaryCodeString += "ram[" + to_string(line) + "] = {6'b" + bitset<6>(it->opCode).to_string() + ",26'b" + bitset<26>(it->imed).to_string() + "};\n";
                line++;
                break;
        }
    }
    ofstream binaryCodeFile;
    binaryCodeFile.open("./outputs/binaryCode");
    binaryCodeFile << binaryCodeString;
    binaryCodeFile.close();
}