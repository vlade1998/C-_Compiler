#include <iostream>
#include <string.h>
#include "quad.cpp"
#pragma once

#define TABSAPCES 6
#define TABTREE 5

using namespace std;

int tempIndex = 0, labelIndex = 0;
enum NodeKind{ConstK,IdK,IdArrayK,TypeK,FnK, OpK, ReturnK, LoopK, CondK, CallK, AtrK};

struct treeNode{
    treeNode *child[3], *sibling;
    NodeKind nodeKind;
    int lineno;
    string name;
    int val;
};

treeNode *newNode(NodeKind kind){
    treeNode *node = (treeNode *) malloc(sizeof(treeNode));
    for(int i = 0; i < 3; i++) node->child[i] = NULL;
    node->sibling = NULL;
    node->nodeKind = kind;
    return node;
}

string tabTree(int treeLevel){
  string ret = "";
  for(int i=0;i<treeLevel*TABTREE;i++){
    ret = ret + " ";
  }
  return ret;
}

string showTree(treeNode *tree, bool is_brother, int treeLevel){
  if(tree == NULL) return "";
  if(treeLevel==0)
    return "{\n'tree:'" + showTree(tree, false, 1) + "\n}\n";
  string ret = "";
  if(tree->sibling!=NULL && is_brother==false)
    ret = "\n" + tabTree(treeLevel) + "[\n"+tabTree(treeLevel)+"{\n"+tabTree(treeLevel)+"'Node': ";
  else
    ret = "\n" + tabTree(treeLevel) + "{\n"+tabTree(treeLevel)+"'Node': ";
  switch(tree->nodeKind){
    case ConstK:
      ret = ret + to_string(tree->val);
    break;
    case FnK:
      ret = ret + "'FUNCTION', ";
    break;
    case ReturnK:
      ret = ret + "'return', ";
    break;
    case LoopK:
      ret = ret + "'while', ";
    break;
    case CondK:
      ret = ret + "'if', ";
    break;
    case CallK:
      ret = ret + "'CALL - " + tree->name + "',";
    break;
    default:
      ret = ret + "'" + tree->name + "',";
  }
  if(tree->child[0]!=NULL)
    ret = ret + "\n"+tabTree(treeLevel)+"'child[0]': " + showTree(tree->child[0], false, treeLevel+1);
  if(tree->child[1]!=NULL)
    ret = ret + "\n"+tabTree(treeLevel)+"'child[1]': " + showTree(tree->child[1], false, treeLevel+1);
  if(tree->child[2]!=NULL)
    ret = ret + "\n"+tabTree(treeLevel)+"'child[2]': " + showTree(tree->child[2], false, treeLevel+1);

  ret = ret + "\n"+tabTree(treeLevel)+"}";
  if(tree->sibling!=NULL && is_brother==false){
    ret = ret + ",";
    treeNode *aux = tree->sibling;
    while(aux!=NULL){
      ret = ret + showTree(aux, true, treeLevel);
      if(aux->sibling!=NULL)
        ret = ret + tabTree(treeLevel)+",";
      aux = aux->sibling;
    }
  }
  if(tree->sibling!=NULL && is_brother==false)
    string ret = "\n" + tabTree(treeLevel) + "]\n";
  return ret;
}

int countParams(treeNode *tree){
	int ret = 0;
	while(tree!=NULL){
		ret++;
		tree = tree->sibling;
	}
	return ret;
}

string getOppositeOperator(string a){
	if(a == "<") return ">=";
	if(a == "<=") return ">";
	if(a == ">") return "<=";
	if(a == ">=") return "<";
	if(a == "==") return "!=";
	if(a == "!=") return "==";
}

string tabGenerator(string first_word, bool is_label){
  string ret = "";
  int i;
  for(i=first_word.length(); i<=TABSAPCES;i++){
    ret = ret + " ";
  }
  if(is_label)
    return first_word+ret;
  else
    return ret+first_word;
}

string specialFunctions[6] = {
	"loadInstructions",
	"jumpAddr",
	"storeRegisters",
	"loadRegisters",
	"initializeRegisters",
	"storeCurrentProcessRegisters"
};

int isSpecialFunction(string fun){
    for(int i = 0; i < sizeof(specialFunctions)/sizeof(*specialFunctions); i++){
        if(specialFunctions[i] == fun) return 1;
    }
    return 0;
}
string codeGeneratorQuad(treeNode *tree, int flag);

void generateSpecialFunction(treeNode* tree){
	if(tree->name == "loadInstructions"){ // loadInstruction(dataAddress,instructionAddress,Amount)
		if(countParams(tree->child[0]) != 3){
			cout << "Erro no uso da funcao loadInstructions";
			exit(-1);
		}
		codeGeneratorQuad(tree->child[0],1);
		string regDataAddress = "_t" + to_string((tempIndex-1)%16);
		codeGeneratorQuad(tree->child[0]->sibling,1);
		string regInstructionAddress = "_t" + to_string((tempIndex-1)%16);
		codeGeneratorQuad(tree->child[0]->sibling->sibling,1);
		string regAmount = "_t" + to_string((tempIndex-1)%16);
		string regCounter = "_t" + to_string((tempIndex)%16);
		tempIndex++;
		string regSet = "_t" + to_string((tempIndex)%16);
		tempIndex++;
		string regValue = "_t" + to_string((tempIndex)%16);
		tempIndex++;
		string regOne = "_t" + to_string((tempIndex)%16);
		tempIndex++;
		int label1 = labelIndex;
		int label2 = labelIndex + 1;
		labelIndex += 2;
		insertQuad("immed","0"," ",regCounter);
		insertQuad("immed","1"," ",regOne);
		insertQuad("label","_L" + to_string(label1)," "," ");
		insertQuad("<",regCounter,regAmount,regSet);
		insertQuad("if_f",regSet,"_L" + to_string(label2)," ");
		insertQuad("loadAddr",regDataAddress," ",regValue);
		insertQuad("writei",regValue," ",regInstructionAddress);
		insertQuad("+",regDataAddress,regOne,regDataAddress);
		insertQuad("+",regInstructionAddress,regOne,regInstructionAddress);
		insertQuad("+",regCounter,regOne,regCounter);
		insertQuad("goto","_L" + to_string(label1)," "," ");
		insertQuad("label","_L" + to_string(label2)," "," ");
	}else if(tree->name == "jumpAddr"){ // jumpAddr(instructionAddress)
		if(countParams(tree->child[0]) != 1){
			cout << "Erro no uso da funcao loadInstructions";
			exit(-1);
		}
		codeGeneratorQuad(tree->child[0],1);
		string regAddress = "_t" + to_string((tempIndex-1)%16);
		insertQuad("jumpAddr",regAddress," "," ");
	}else if(tree->name == "storeRegisters"){ // storeRegisters(dataAddress)
		if(countParams(tree->child[0]) != 1){
			cout << "Erro no uso da funcao storeRegisters";
			exit(-1);
		}
		codeGeneratorQuad(tree->child[0],1);
		string regDataAddress = "_t" + to_string((tempIndex-1)%16);

		insertQuad("storeReg", regDataAddress, " ", " ");

	}else if(tree->name == "loadRegisters"){ // loadRegisters(dataAddress)
		if(countParams(tree->child[0]) != 1){
			cout << "Erro no uso da funcao loadRegisters";
			exit(-1);
		}
		codeGeneratorQuad(tree->child[0],1);
		string regDataAddress = "_t" + to_string((tempIndex-1)%16);

		insertQuad("loadReg", regDataAddress, " ", " ");
	}else if(tree->name == "initializeRegisters"){ // initializeRegisters(processMemorySectionStart, instructionMemomrySectionStart)
		if(countParams(tree->child[0]) != 2){
			cout << "Erro no uso da funcao loadRegisters";
			exit(-1);
		}
		codeGeneratorQuad(tree->child[0],1);
		string regDataAddress = "_t" + to_string((tempIndex-1)%16);

		codeGeneratorQuad(tree->child[0]->sibling,1);
		string regInstMemSectionStart = "_t" + to_string((tempIndex-1)%16);

		codeGeneratorQuad(tree->child[0]->sibling->sibling,1);
		string regInitialPcValue = "_t" + to_string((tempIndex-1)%16);

		insertQuad("initReg", regDataAddress, regInstMemSectionStart, " ");
	}else if(tree->name == "storeCurrentProcessRegisters"){
		if(countParams(tree->child[0]) != 0){
			cout << "Erro no uso da funcao storeCurrentProcessRegisters";
			exit(-1);
		}
		insertQuad("storeCurReg", " ", " ", " ");
	}
}

string codeGeneratorQuad(treeNode *tree, int flag){
	if(tree==NULL) return " ";
	switch(tree->nodeKind){
		case OpK:{
			string aux = "*/-+";
			string a = codeGeneratorQuad(tree->child[0],1);
			string at = "_t" + to_string((tempIndex-1)%16);
			string b = codeGeneratorQuad(tree->child[1],1);
			string bt = "_t" + to_string((tempIndex-1)%16);
			if(tree->name != "="){
				insertQuad(tree->name,at,bt,"_t" + to_string(tempIndex%16));
				tempIndex++;
				if(flag == 2 && aux.find(tree->name) != std::string::npos){ 
					insertQuad("param","_t" + to_string((tempIndex-1)%16)," "," ");
					if(tree->sibling != NULL) codeGeneratorQuad(tree->sibling,2);
				}
				return "_t" + to_string((tempIndex-1)%16);
			}else{
				insertQuad("asn",bt," ",at);
				if(tree->child[0]->nodeKind != IdArrayK){
					insertQuad("store",at," ",a);
				}else{
					string aux = codeGeneratorQuad(tree->child[0]->child[0],1);
					insertQuad("store",at,"_t" + to_string((tempIndex-1)%16),tree->child[0]->name);
				}
				if(tree->sibling != NULL) codeGeneratorQuad(tree->sibling,0);
				return " ";
			}
			break;

		}
		case IdK:
			if(flag == 2){
				insertQuad("load",tree->name," ","_t" + to_string(tempIndex%16));
				insertQuad("param","_t" + to_string(tempIndex%16)," "," ");
				tempIndex++;
				if(tree->sibling != NULL)codeGeneratorQuad(tree->sibling,2);
				return " ";
			}else if(flag == 3){
				insertQuad("alloc",tree->name,"1"," ");
			}else if(flag == 5){
				insertQuad("arg",tree->name," "," ");
			}else{
				insertQuad("load",tree->name," ","_t" + to_string(tempIndex%16));
				tempIndex++;
				return tree->name;
				break;
			}
		case ConstK:
			if(flag == 2){
				insertQuad("immed",to_string(tree->val)," ","_t" + to_string(tempIndex%16));
				insertQuad("param","_t" + to_string(tempIndex%16)," "," ");
				tempIndex++;
				if(tree->sibling != NULL)codeGeneratorQuad(tree->sibling,2);
				return " ";
			}else if (flag == 1){
				insertQuad("immed",to_string(tree->val)," ","_t" + to_string(tempIndex%16));
				tempIndex++;
				return to_string(tree->val);
			}else{
				return to_string(tree->val);
			}
			break;
		case FnK:
			tempIndex = 0;
			insertQuad("fun",tree->name," "," ");
			codeGeneratorQuad(tree->child[0],5);
			codeGeneratorQuad(tree->child[1],0);
			insertQuad("endfun"," "," "," ");
			if(tree->sibling != NULL)  codeGeneratorQuad(tree->sibling,0);
			return " ";
			break;
		case TypeK:
			if(flag == 5){
				if(tree->child[0] != NULL) codeGeneratorQuad(tree->child[0],5);
				if(tree->sibling != NULL) codeGeneratorQuad(tree->sibling,5);
			}else{
				if(tree->child[0]->nodeKind == FnK) codeGeneratorQuad(tree->child[0],0);
				else if(tree->child[0]->nodeKind == IdK || tree->child[0]->nodeKind == IdArrayK) codeGeneratorQuad(tree->child[0],3);
				if(tree->sibling != NULL) codeGeneratorQuad(tree->sibling,0);
				return " ";
			}
			break;
		case CallK:{
			if(flag == 0){
				int n = 0;
				if(isSpecialFunction(tree->name)){
					generateSpecialFunction(tree);
				}else if(tree->name != "output"){
					if(tree->child[0] != NULL){
						n = countParams(tree->child[0]);
						codeGeneratorQuad(tree->child[0],2);
					}
					insertQuad("call",tree->name,to_string(n),"_t" + to_string(tempIndex%16));
					tempIndex++;
				}else{
					if(tree->child[0] == NULL || (tree->child[0]->nodeKind != IdK && tree->child[0]->nodeKind != OpK && tree->child[0]->nodeKind != ConstK && tree->child[0]->nodeKind != CallK&& tree->child[0]->nodeKind != IdArrayK)){
						cout << "Erro no uso da funcao output";
						exit(-1);
					}
					if(tree->child[0]->sibling == NULL || tree->child[0]->sibling->nodeKind != ConstK){
						cout << "Erro no uso da funcao output";
						exit(-1);
					}
					codeGeneratorQuad(tree->child[0],1);
					insertQuad("output","_t" + to_string((tempIndex - 1)%16),codeGeneratorQuad(tree->child[0]->sibling,0)," ");
				}
				if(tree->sibling != NULL) codeGeneratorQuad(tree->sibling,0);
				return " ";
			}else if(flag==1){
				int n = 0;
				if(tree->child[0] != NULL){
					n = countParams(tree->child[0]);
					codeGeneratorQuad(tree->child[0],2);
				}
				insertQuad("call",tree->name,to_string(n),"_t" + to_string(tempIndex%16));
				tempIndex++;
				return "_t" + to_string((tempIndex-1)%16);
			}else{
				int n = 0;
				if(tree->child[0] != NULL){
					n = countParams(tree->child[0]);
					codeGeneratorQuad(tree->child[0],2);
				}
				insertQuad("call",tree->name,to_string(n),"_t" + to_string(tempIndex%16));
				insertQuad("param","_t" + to_string(tempIndex%16)," "," ");
				tempIndex++;
				if(tree->sibling != NULL) codeGeneratorQuad(tree->sibling,2);
				return " ";
				break;
			}
		}
		case CondK:{
			string a;
			int label1, label2;
			label1 = labelIndex;
			label2 = labelIndex + 1;
			labelIndex += 2;
			a = codeGeneratorQuad(tree->child[0],2);
			insertQuad("if_t",a,"_L" + to_string(label1)," ");
			if(tree->child[2] != NULL) codeGeneratorQuad(tree->child[2],0);
			insertQuad("goto","_L" + to_string(label2)," "," ");
			insertQuad("label","_L" + to_string(label1)," "," ");
			codeGeneratorQuad(tree->child[1],0);
			insertQuad("label","_L" + to_string(label2)," "," ");
			if(tree->sibling != NULL) codeGeneratorQuad(tree->sibling,0);
			return " ";
			break;
		}
		case LoopK:{
			string a;
			int label1, label2;
			label1 = labelIndex;
			label2 = labelIndex + 1;
			labelIndex += 2;
			insertQuad("label","_L" + to_string(label1)," "," ");
			a = codeGeneratorQuad(tree->child[0],3);
			insertQuad("if_f",a,"_L" + to_string(label2)," ");
			codeGeneratorQuad(tree->child[1],0);
			insertQuad("goto","_L" + to_string(label1)," "," ");
			insertQuad("label","_L" + to_string(label2)," "," ");
			if(tree->sibling != NULL) codeGeneratorQuad(tree->sibling,0);
			return " ";
			break;
		}
		case ReturnK:{
			if(tree->child[0] != NULL){
				string a = codeGeneratorQuad(tree->child[0],1);
				insertQuad("ret","_t" + to_string((tempIndex-1)%16) ," "," ");
			}else{
				insertQuad("ret"," ", " ", " ");
			}
			return " ";
			break;
		}
		case IdArrayK:
			string a;
			if(flag==3){
				a = codeGeneratorQuad(tree->child[0],0);
				insertQuad("alloc",tree->name,a," ");
			}else if(flag == 2){
				a = codeGeneratorQuad(tree->child[0],1);
				insertQuad("load",tree->name,"_t" + to_string((tempIndex-1)%16),"_t" + to_string(tempIndex%16));
				insertQuad("param","_t" + to_string(tempIndex%16)," "," ");
				tempIndex++;
				if(tree->sibling != NULL)codeGeneratorQuad(tree->sibling,2);
				return tree->name + "[" + a + "]";
			}else{
				a = codeGeneratorQuad(tree->child[0],1);
				insertQuad("load",tree->name,"_t" + to_string((tempIndex-1)%16),"_t" + to_string(tempIndex%16));
				tempIndex++;
				return tree->name + "[" + a + "]";
			}
			break;
	}
}
