#ifndef DATATYPES
#define DATATYPES

#include <stdio.h>
#include <stdbool.h>
#include "StandardConstants.h"

typedef struct ConfigDataType
   {
     double version;
     char metaDataFileName[100];
     int cpuSchedCode;
     int quantumCycles;
     int memAvailable;
     int procCycleRate;
     int ioCycleRate;
     int logToCode;
     char logToFileName[100];
   } ConfigDataType;

typedef struct OpCodeType
   {
     int pid;
     char command[5];
     char inOutArg[5];
     char strArg1[15];
     int intArg2;
     int intArg3;
     double opEndTime;
     struct OpCodeType *nextNode;
   } OpCodeType;

typedef struct PCBType
   {
     int pid;
     int appStates;
     int cpuSchedCode;
     int totalCycleTime;
     struct OpCodeType *programCounter;
     struct PCBType *nextNode;
   } PCBType;

typedef struct logLLType
   {
    char formatString[MAX_STR_LEN];
    struct logLLType *nextNode;

  } logLLType;

typedef struct mmuType
   {
    int physStart;
    int physEnd;
    int logicalStart;
    int logicalEnd;

    int pid;
    char memState[MAX_STR_LEN];


    struct mmuType *nextNode;

  } mmuType;

typedef struct threadType
   {
    int pid;
    int runTime;

    struct threadType *nextNode;
  } threadType;


#endif
