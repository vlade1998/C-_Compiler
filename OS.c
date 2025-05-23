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
    return remainder((processId+1),2);
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

    initializeRegisters(osMemorySectionSize + processId*processMemorySectionSize, currentProcessInsMemorySectionStart);
}

void executeProcess(int processId){
    int currentProcessInstructionsAddress;

    processState[processId] = EXECUTING;
    currentProcessInstructionsAddress = osMemorySectionSize + processId*processMemorySectionSize + 32;
    storeRegisters(0);
    loadInstructions(currentProcessInstructionsAddress, currentProcessInsMemorySectionStart, processNumberOfInstructions[processId]);
    loadRegisters(osMemorySectionSize + processId*processMemorySectionSize);

    if(1 == 0){
        storeCurrentProcessRegisters();
        kernelMode();
        processState[processId] = READY;
    }

    if(1 == 0){
        storeCurrentProcessRegisters();
        kernelMode();
        processState[processId] = 0;
        isProcessActive[processId] = 0;
    }
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
    currentProcessInsMemorySectionStart = 500;

    /* main loop */

    currentProcess = 7;

    initializeProcess(0);
    initializeProcess(1);

    processNumberOfInstructions[0] = 4;
    processNumberOfInstructions[1] = 4;

    while(1 == 1){
        currentProcess = getNextProcess(currentProcess);
        executeProcess(currentProcess);
    }
}

