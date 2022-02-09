/*#include <stdio.h>*/
/*#include <unistd.h>*/


/* Constants */

int EXECUTING;
int READY;
int BLOCKED;

/* Settings variables */

int osMemorySectionSize;
int currentProcessInsMemorySectionStart;
int processMemorySectionSize;


/* Process variables */

int isProcessActive[8];
int processState[8];
int processMemorySection[8];
int processProgramCounter[8];
int processNumberOfInstructions[8];

int remainder(int num, int divisor){
    return (num - divisor * (num / divisor));
}

int roundRobin(int processId){
    return remainder((processId+1),8);
}

int getNextProcess(int currentProcesss){
    int nextProcessCandidate;
    nextProcessCandidate = roundRobin(currentProcesss);
    while(processState[nextProcessCandidate] != 1){
        nextProcessCandidate = roundRobin(nextProcessCandidate);
    }
    return nextProcessCandidate;
}

void executeProcess(int processId){
    int currentProcessInstructionsAddress;

    currentProcessInstructionsAddress = osMemorySectionSize + processId*processMemorySectionSize + 32;
    loadInstructions(currentProcessInstructionsAddress, 300, 3);
    jumpAddr(300);

    output(processId, 0);
    output(currentProcessInstructionsAddress, 1);
    /*printf("%d\n", processId);*/
    /*sleep(1);*/
}

void main(void){

    /* Main variable initialization */

    int currentProcess;

    /* Constants definiton */

    EXECUTING = 0;
    READY = 1;
    BLOCKED = 2;

    /* settings variables definiton */

    osMemorySectionSize = 100;
    processMemorySectionSize = 100;
    currentProcessInsMemorySectionStart = 300;

    /* main loop */

    currentProcess = 7;

    processState[0] = 1;
    processState[1] = 1;
    processState[2] = 1;
    processState[3] = 1;
    processState[4] = 1;
    processState[5] = 1;
    processState[6] = 0;
    processState[7] = 1;

    processNumberOfInstructions[0] = 2;

    while(1 == 1){
        currentProcess = getNextProcess(currentProcess);
        executeProcess(currentProcess);
    }
}

