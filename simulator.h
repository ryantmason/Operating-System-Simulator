#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdbool.h>
#include <pthread.h>
#include "datatypes.h"
#include "StringUtils.h"
#include "simtimer.h"
#include "configops.h"
#include "metadataops.h"


typedef enum { NEW,
              READY,
              RUNNING,
              BLOCKED,
              EXITING } appStates;

typedef enum { ADD,
               INTERRUPT,
               RESOLVE } enumFlags;


mmuType *addMMUNode( mmuType **mmuHeadRef, mmuType *newMMUNode );
PCBType *addPCBNode( PCBType *localPtr, PCBType *newPCBNode );
logLLType *addStringNode( logLLType *localPtr, char *newString );
threadType *addThreadNode( threadType *localPtr, threadType *newThreadNode );
bool checkforAccess( mmuType **mmuHeadRef, OpCodeType *localOpRef );
bool checkforAllocation(int memCounter, mmuType **mmuHeadRef,
      OpCodeType *localOpRef, ConfigDataType *localConfigRef );
mmuType *clearMMUList( mmuType *localPtr );
PCBType *clearPCBList( PCBType *localPtr );
logLLType *clearStringList( logLLType *localPtr );
threadType *clearThreadList( threadType *localPtr );
bool createPCB( OpCodeType *opHeadRef, ConfigDataType *configData,
                                            PCBType **pcbHeadRef );
void displayAndLog( int logToCode, logLLType **stringList, char *addString );
threadType *createThreadNode( OpCodeType *opWkgRef, ConfigDataType *configDataPtr,
                       PCBType *pcbWkgRef );
void displayMMU( mmuType *mmuHeadRef );
PCBType *getNextProcess( PCBType *headRef, ConfigDataType *configPtr );
threadType *interruptManager( threadType **threadHead, threadType *threadInterrupt,
                              double activeSimTime, int enumFlags );
void *threadTimer( void *miliSecs );
void runSim( ConfigDataType *configPtr, OpCodeType *metaDataMstrPtr );

#endif
