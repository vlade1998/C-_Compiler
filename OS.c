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

void initializeProcess(int processId){
    isProcessActive[processId] = 1;
    processState[processId] = READY;

}

void executeProcess(int processId){
    int currentProcessInstructionsAddress;

    currentProcessInstructionsAddress = osMemorySectionSize + processId*processMemorySectionSize + 32;
    loadInstructions(currentProcessInstructionsAddress, currentProcessInsMemorySectionStart, processNumberOfInstructions[processId]);
    loadRegisters(osMemorySectionSize + processId*processMemorySectionSize);

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
    currentProcessInsMemorySectionStart = 400;

    /* main loop */

    currentProcess = 7;

    initializeProcess(0);

    processNumberOfInstructions[0] = 3;

    while(1 == 1){
        currentProcess = getNextProcess(currentProcess);
        executeProcess(currentProcess);
    }
}

