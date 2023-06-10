// header files
#include "simulator.h"


/*
name: addMMUNode
Algorithm:
Precondition:
Postcondition:
Exceptions: none
Notes: none
*/
mmuType *addMMUNode( mmuType **mmuHeadRef, mmuType *newMMUNode )
   {
    mmuType *localPtr;

    localPtr = (mmuType * )malloc( sizeof( mmuType ) );

    localPtr->physStart = newMMUNode->physStart;
    localPtr->physEnd = newMMUNode->physEnd;
    localPtr->logicalStart = newMMUNode->logicalStart;
    localPtr->logicalEnd = newMMUNode->logicalEnd;
    localPtr->pid = newMMUNode->pid;
    copyString( localPtr->memState, newMMUNode->memState );

    localPtr->nextNode = *mmuHeadRef;

    *mmuHeadRef = localPtr;

    return *mmuHeadRef;
   }


/*
name: addPCBNode
Algorithm: creates empty PCB that will be filled in by inputted information. And
           links to previous node in LL
Precondition: given pcb pointer
Postcondition: returns pointer
Exceptions: none
Notes: none
*/
PCBType *addPCBNode( PCBType *localPtr, PCBType *newPCBNode )
   {
    if( localPtr == NULL )
       {
        localPtr = (PCBType * )malloc( sizeof( PCBType ) );

        localPtr->pid = newPCBNode->pid;
        localPtr->appStates = newPCBNode->appStates;
        localPtr->cpuSchedCode = newPCBNode->cpuSchedCode;
        localPtr->totalCycleTime = newPCBNode->totalCycleTime;
        localPtr->programCounter = newPCBNode->programCounter;

        localPtr->nextNode = NULL;

        return localPtr;
       }
    localPtr->nextNode = addPCBNode( localPtr->nextNode, newPCBNode );

    return localPtr;
   }


/*
name: addStringNode
Algorithm: adds a node and information to node in string linked list
Precondition: given pointer and string to be added to node
Postcondition: returns the pointer
Exceptions: none
Notes: none
*/
logLLType *addStringNode( logLLType *localPtr, char *newString )
  {
   if( localPtr == NULL )
      {
       localPtr = ( logLLType * )malloc( sizeof( logLLType ) );

       copyString( localPtr->formatString, newString );

       localPtr->nextNode = NULL;

       return localPtr;
      }

   localPtr->nextNode = addStringNode( localPtr->nextNode, newString );

   return localPtr;
  }


/*
name: addThreadNode
Algorithm: adds a node and information to node in thread linked list
Precondition:
Postcondition: returns the pointer
Exceptions: none
Notes: none
*/
threadType *addThreadNode( threadType *localPtr, threadType *newThreadNode )
   {
    if( localPtr == NULL )
       {
        localPtr = (threadType * )malloc( sizeof( threadType ) );

        localPtr->pid = newThreadNode->pid;
        localPtr->runTime = newThreadNode->runTime;

        localPtr->nextNode = NULL;

        return localPtr;
       }
    localPtr->nextNode = addThreadNode( localPtr->nextNode, newThreadNode );

    return localPtr;
   }


/*
name: checkforAccess
Algorithm:
Precondition:
Postcondition:
Exceptions: none
Notes: none
*/
bool checkforAccess( mmuType **mmuHeadRef, OpCodeType *localOpRef )
   {
    mmuType *localMMURef = *mmuHeadRef;

    while( localMMURef != NULL )
      {
       if( ( localOpRef->intArg2 <= localMMURef->logicalStart && localOpRef->intArg2 >= localMMURef->logicalEnd )
         || ( ( ( localOpRef->intArg2 + localOpRef->intArg3 ) - 1 ) >= localMMURef->logicalEnd
         && ( ( localOpRef->intArg2 + localOpRef->intArg3 ) - 1 ) <= localMMURef->logicalStart ) )
          {
           return true;
          }
       else
          {
           return false;
          }

       localMMURef = localMMURef->nextNode;
      }

    return false;
   }


/*
name: checkforAllocation
Algorithm:
Precondition:
Postcondition:
Exceptions: none
Notes: none
*/
bool checkforAllocation(int memCounter, mmuType **mmuHeadRef, OpCodeType *localOpRef,
                                    ConfigDataType *localConfigRef )
   {
    // intialize variables

    // initialize tempMemNode
    mmuType *tempMemNode;

    mmuType *localMMURef = *mmuHeadRef;


    // if statement to check if this is the first mem allocation (for initialization)
    if( memCounter == 1 )
       {
        // create initial node with necessary information
        tempMemNode = ( mmuType * )malloc( sizeof( mmuType ) );

        tempMemNode->physStart = 0;
        tempMemNode->physEnd = localConfigRef->memAvailable - 1;
        tempMemNode->logicalStart = 0;
        tempMemNode->logicalEnd = 0;
        tempMemNode->pid = -69;
        copyString( tempMemNode->memState, "Open" );

        tempMemNode->nextNode = NULL;

        //add initial node to Linked List
        *mmuHeadRef = addMMUNode( mmuHeadRef, tempMemNode );

        free( tempMemNode );

        // return true because node was succesfully created
        return true;
       }
    else
       {
        // loop through created linked list
        while( localMMURef != NULL )
           {
            // if statement thatcheck new mem allocation variables with previous
            // variables in initialization node
            if( ( localOpRef->intArg2 >= localMMURef->logicalStart && localOpRef->intArg2 <= localMMURef->logicalEnd )
               || ( ( ( localOpRef->intArg2 + localOpRef->intArg3 ) - 1 ) <= localMMURef->logicalEnd
               && ( ( localOpRef->intArg2 + localOpRef->intArg3 ) - 1 ) >= localMMURef->logicalStart ) )
               {

                return false;
               }
            else
               {
                 tempMemNode = ( mmuType * )malloc( sizeof( mmuType ) );

                 tempMemNode->physStart = localOpRef->intArg2;
                 tempMemNode->physEnd = localOpRef->intArg2 - 1;
                 tempMemNode->logicalStart = localOpRef->intArg2;
                 tempMemNode->logicalEnd = ( localOpRef->intArg2 + localOpRef->intArg3 ) - 1;
                 tempMemNode->pid = localOpRef->pid;
                 copyString( tempMemNode->memState, "Used" );

                 tempMemNode->nextNode = NULL;

                 *mmuHeadRef = addMMUNode( mmuHeadRef, tempMemNode );

                 free( tempMemNode );


                 return true;
               }

            // itterate through previously created nodes
            localMMURef = localMMURef->nextNode;
           }

       }

    return false;
   }


/*
name: clearMMUList
Algorithm:
Precondition:
Postcondition:
Exceptions: none
Notes: none
*/
mmuType *clearMMUList( mmuType *localPtr )
  {
   if(localPtr != NULL)
      {
       clearMMUList( localPtr->nextNode );
       //free the local ptr
       free( localPtr );
       localPtr = NULL;
      }
   return NULL;
  }


/*
name: clearConfigData
Algorithm:
Precondition:
Postcondition:
Exceptions: none
Notes: none
*/
PCBType *clearPCBList( PCBType *localPtr )
   {
    if(localPtr != NULL)
       {
        clearPCBList( localPtr->nextNode );
        //free the local ptr
        free( localPtr );
        localPtr = NULL;
       }
    return NULL;
   }

/*
name: clearCPCBList
Algorithm:
Precondition:
Postcondition:
Exceptions: none
Notes: none
*/
logLLType *clearStringList( logLLType *localPtr )
  {
   if(localPtr != NULL)
      {
       clearStringList( localPtr->nextNode );
       //free the local ptr
       free( localPtr );
       localPtr = NULL;
      }
   return NULL;
  }


/*
name: clearThreadList
Algorithm:
Precondition:
Postcondition:
Exceptions: none
Notes: none
*/
threadType *clearThreadList( threadType *localPtr )
   {
    if(localPtr != NULL)
       {
        clearStringList( localPtr->nextNode );
        //free the local ptr
        free( localPtr );
        localPtr = NULL;
      }
    return NULL;
   }


/*
name: createPCB
Algorithm: creates all PCBs for app processes and fills in given information
Precondition: given meta data head ref, config data pointer, and double pointer
              to pcb head ref.
Postcondition: returns bool value whether or not creation was succesful
Exceptions: none
Notes: none
*/

bool createPCB( OpCodeType *opHeadRef, ConfigDataType *configData,
                                           PCBType **pcbHeadRef )
   {

    // check that list isnt empty
    if( opHeadRef == NULL)
       {
        // if empty return false
        return false;
       }

    // int. local ref
    OpCodeType *localOpRef = NULL;

    // set local ref to local headref(or given headref)
    localOpRef = opHeadRef;

    // temp PCB
    PCBType *tempPCB;

    // local PCB headRef
    PCBType *tempPCBHeadRef = NULL;

    // set pid to 0
    int pidCounter = 0;

    // loops as long as local ref is not null
    while( localOpRef != NULL )
       {
        // check for app start
        if( compareString( localOpRef->strArg1, "start" ) == STR_EQ &&
                           compareString( localOpRef->command, "app" ) == STR_EQ )
           {
            // create pcb
            tempPCB = ( PCBType * )malloc( sizeof( PCBType ) );

            tempPCB->pid = pidCounter;
            pidCounter++;
            tempPCB->appStates = NEW;
            tempPCB->cpuSchedCode = configData->cpuSchedCode;
            tempPCB->totalCycleTime = 0;
            tempPCB->programCounter = localOpRef;

            tempPCB->nextNode = NULL;

           }
        // check for not app end
        if( compareString( localOpRef->strArg1, "end" ) != STR_EQ &&
                           compareString(localOpRef->command, "app" ) != STR_EQ )
           {
            // check for cpu
            if( compareString( localOpRef->command, "cpu" ) == STR_EQ )
               {
                tempPCB->totalCycleTime =  tempPCB->totalCycleTime +
                               configData->procCycleRate * localOpRef->intArg2;

               }
            // checks for dev in/out
            else if( compareString(localOpRef->command, "dev" ) == STR_EQ )
               {
                tempPCB->totalCycleTime = tempPCB->totalCycleTime +
                                  configData->ioCycleRate * localOpRef->intArg2;
               }


           }
        // check for app end
        else if( compareString( localOpRef->strArg1, "end" ) == STR_EQ &&
                                compareString(localOpRef->command, "app" ) == STR_EQ )
           {
            tempPCBHeadRef = addPCBNode( tempPCBHeadRef, tempPCB );

            free( tempPCB );
           }
        // move to next node
        localOpRef = localOpRef->nextNode;
       }
    *pcbHeadRef = tempPCBHeadRef;
    return true;
   }



threadType *createThreadNode( OpCodeType *opWkgRef, ConfigDataType *configDataPtr,
                       PCBType *pcbWkgRef )
   {
    threadType *tempThread;

    tempThread = ( threadType * )malloc( sizeof( threadType ) );

    tempThread->pid = pcbWkgRef->pid;
    tempThread->runTime = opWkgRef->intArg2 * configDataPtr->ioCycleRate;

    tempThread->nextNode = NULL;

    return tempThread;
   }


/*
name: displayAndLog
Algorithm: checks for logToCode to monitor, file, or both. Then prints
    to monitor when needed and adds strings to linked list when needed
Precondition: given logToCode, double pointer to string list and string to be
       added
Postcondition: none
Exceptions: none
Notes: none
*/
void displayAndLog( int logToCode, logLLType **stringList, char *addString )
   {
    // check for log to monitor or Both
    if( logToCode == LOGTO_MONITOR_CODE || logToCode == LOGTO_BOTH_CODE )
       {
        //print inputted string
        printf( "%s", addString );

       }

    // check for log to file or Both
    if( logToCode == LOGTO_FILE_CODE || logToCode == LOGTO_BOTH_CODE )
       {
        // log to file using add node
        *stringList = addStringNode( *stringList, addString );

       }

   }


/*
name: displayMMU
Algorithm:
Precondition:
      added
Postcondition: none
Exceptions: none
Notes: none
*/
void displayMMU( mmuType *mmuHeadRef )
   {
    mmuType *localWkgRef = mmuHeadRef;

    while( localWkgRef != NULL)
       {

        printf("%d [ %s, P# %d, %d-%d ] %d\n", localWkgRef->physStart,
              localWkgRef->memState, localWkgRef->pid, localWkgRef->logicalStart,
              localWkgRef->logicalEnd, localWkgRef->physEnd );

        localWkgRef = localWkgRef->nextNode;
       }

   }


/*
name: getNextProcess
Algorithm:
Precondition:
Postcondition: none
Exceptions: none
Notes: none
*/
PCBType *getNextProcess(PCBType *headRef, ConfigDataType *configPtr )
   {
     // set wkgRef to headnode
     PCBType *wkgRef = headRef;

     PCBType *minPCB;


    // check if schedCode FCFS
    if( configPtr->cpuSchedCode == CPU_SCHED_FCFS_N_CODE ||
        configPtr->cpuSchedCode == CPU_SCHED_FCFS_P_CODE )
       {

        // while currentNode not null and appStates is exiting
        while( wkgRef != NULL && ( wkgRef->appStates == EXITING  ||
                wkgRef->appStates == BLOCKED ) )
           {
            // move to next nextNode
            wkgRef = wkgRef->nextNode;
           }

        // return currentNode
        return wkgRef;
       }
    // check if schedCode SJF
    else if( configPtr->cpuSchedCode == CPU_SCHED_SJF_N_CODE )
       {
        // set a minNode to the HeadRef
        // create pcb
        minPCB = ( PCBType * )malloc( sizeof( PCBType ) );

        // temp minimum pcb created
        minPCB->pid = -1;
        minPCB->appStates = NEW;
        minPCB->cpuSchedCode = configPtr->cpuSchedCode;
        minPCB->totalCycleTime = 100000;
        minPCB->programCounter = NULL;
        minPCB->nextNode = NULL;

        // set wkgRef to HeadRef
        wkgRef = headRef;


        while( wkgRef != NULL )
           {
            if( wkgRef->totalCycleTime < minPCB->totalCycleTime && wkgRef->appStates != EXITING )
               {
                minPCB = wkgRef;
               }

            wkgRef = wkgRef->nextNode;

           }
        // checks pid of minPCB, after all processes are completed the loop hits
        // the default temp pcb which holds a pid of -1. Return null in this case
        if( minPCB->pid < 0 )
           {
            minPCB = NULL;
           }

        return minPCB;
       }
    return NULL;
   }


/*
name: interruptManager
Algorithm:
Precondition:
Postcondition: none
Exceptions: none
Notes: none
*/
threadType *interruptManager( threadType **threadHead, threadType *threadInterrupt,
                              double activeSimTime, int enumFlags )
   {
    double interruptTime = 0;

    threadType *tmpInterruptRef = *threadHead;

    switch( enumFlags )
       {
        case ADD :
           *threadHead = addThreadNode( *threadHead, threadInterrupt );

           free( threadInterrupt );

           return *threadHead;
           break;
        case INTERRUPT :
           while( tmpInterruptRef != NULL )
              {
               interruptTime = activeSimTime - ((tmpInterruptRef->runTime)/1000);

               if( interruptTime < 0.0003 )
                  {
                   return tmpInterruptRef;
                  }
               else
                  {
                   tmpInterruptRef = tmpInterruptRef->nextNode;
                  }
              }
           break;
        case RESOLVE :
           printf("resolve interrupt");
           break;
        default :
           return NULL;
       }

   }


/*
name: threadTimer
Algorithm:
Precondition:
Postcondition: none
Exceptions: none
Notes: none
*/
void *threadTimer( void *miliSecs )
   {
    int *miliSecTotal = (int *) (miliSecs);

    runTimer( *miliSecTotal );

    return NULL;
   }


/*
name: runSim
Algorithm: master driver for simulator operations;
           conducts OS simulation with varying scheduling strategies
	   and varying numbers of processes
Precondition: given head pointer to config data and meta data
Postcondition: simulation is provided, file output is provided as configured
Exceptions: none
Notes: none
*/
void runSim( ConfigDataType *configPtr, OpCodeType *metaDataMstrPtr )
   {
    int logToCodePtr = configPtr->logToCode;

    logLLType *stringList = NULL;

    logLLType *wkgStringRef;

    char holderString[ 250 ];

    // create pcbHeadRef
    PCBType *pcbHeadRef = NULL;

    threadType *threadHeadRef = NULL;

    threadType *threadPtr = NULL;

    threadType *threadWkgRef = NULL;

    // run createPCB function
    createPCB( metaDataMstrPtr, configPtr, &pcbHeadRef );

    PCBType *pcbWkgRef = ( PCBType * )malloc( sizeof( PCBType ) );

    pcbWkgRef = pcbHeadRef;

    char time[ MAX_STR_LEN ];

    pthread_t thread1;

    int total;

    mmuType *mmuHeadRef = NULL;

    bool allocateFlag;

    bool accessFlag;

    int memCounter = 0;

    if( logToCodePtr == LOGTO_FILE_CODE )
       {
        printf("Simulator currently writing to file.\n");
       }

    sprintf(holderString, "Simulator Run\n" );
    displayAndLog( logToCodePtr, &stringList, holderString );
    sprintf(holderString, "-------------\n");
    displayAndLog( logToCodePtr, &stringList, holderString );
    // create local references

    accessTimer( ZERO_TIMER, time );

    // print Sim start
    sprintf( holderString, "\n%s, OS: Simulator start\n", time );
    displayAndLog( logToCodePtr, &stringList, holderString );

    // while loop that prints state changes
    while( pcbWkgRef != NULL )
       {
        //set PCBs appstate to READY
        pcbWkgRef->appStates = READY;

        accessTimer( LAP_TIMER, time );

        // print each process being set to NEW state from READY
        sprintf( holderString, "%s, OS: Process %d set to READY state from NEW state\n",
                 time, pcbWkgRef->pid );
        displayAndLog( logToCodePtr, &stringList, holderString );
        // itterate to next node
        pcbWkgRef = pcbWkgRef->nextNode;
       }

     OpCodeType *localOpWkgRef;

     pcbWkgRef = getNextProcess( pcbHeadRef, configPtr );

     bool failFlag;

     double activeTime = 0;

     // Master Loop
     // while loop that loops within each process
     while( pcbWkgRef != NULL )
        {
         failFlag = false;
         accessTimer( LAP_TIMER, time );

         // print process selected with remaining milliSeconds
         sprintf( holderString, "%s, OS: Process %d selected with %d ms remaning\n", time,
                                     pcbWkgRef->pid, pcbWkgRef->totalCycleTime );
         displayAndLog( logToCodePtr, &stringList, holderString );

         // set app state to RUNNING
         pcbWkgRef->appStates = RUNNING;

         accessTimer( LAP_TIMER, time );

         // print proccess being set to running
         sprintf( holderString, "%s, OS: Process %d set from READY to RUNNING\n\n", time, pcbWkgRef->pid );
         displayAndLog( logToCodePtr, &stringList, holderString );
         localOpWkgRef = pcbWkgRef->programCounter->nextNode;

         // while loop that loops through metadata
         while( compareString( localOpWkgRef->strArg1, "end" ) != STR_EQ &&
                    compareString( localOpWkgRef->command, "app" ) != STR_EQ &&
                        failFlag != true )
            {
             // check for CPU process
             if( compareString( localOpWkgRef->command, "cpu" ) == STR_EQ )
                {
                 //access timer
                 accessTimer( LAP_TIMER, time );

                 // print CPU Process start
                 sprintf( holderString, "%s, Process: %d, cpu process operation start\n", time, pcbWkgRef->pid );
                 displayAndLog( logToCodePtr, &stringList, holderString );

                 total = configPtr->procCycleRate*localOpWkgRef->intArg2;

                 pthread_create(&thread1, NULL, threadTimer, (void*) &total );

                 pthread_join(thread1, NULL);

                 accessTimer( LAP_TIMER, time );
                 // print CPU Process end
                 sprintf( holderString, "%s, Process: %d, cpu process operation end\n", time, pcbWkgRef->pid );
                 displayAndLog( logToCodePtr, &stringList, holderString );
                }
             else if(compareString( localOpWkgRef->command, "dev" ) == STR_EQ)
                {
                 // if statement that checks for premptive
                 if( configPtr->cpuSchedCode != CPU_SCHED_FCFS_N_CODE &&
                     configPtr->cpuSchedCode != CPU_SCHED_SJF_N_CODE )
                    {
                     // if preemptive utilize interruptManager
                     threadPtr = createThreadNode( localOpWkgRef, configPtr, pcbWkgRef );

                     threadHeadRef = interruptManager( &threadHeadRef , threadPtr, activeTime, ADD );

                     accessTimer( LAP_TIMER, time );

                     sprintf( holderString, "%s, Process: %d, %s %sput operation start\n\n", time, pcbWkgRef->pid,
                                       localOpWkgRef->strArg1, localOpWkgRef->inOutArg );
                     displayAndLog( logToCodePtr, &stringList, holderString );

                     pcbWkgRef->appStates = BLOCKED;

                     sprintf( holderString, "%s, Process: %d, blocked for %sput operation\n", time, pcbWkgRef->pid,
                              localOpWkgRef->inOutArg );
                     displayAndLog( logToCodePtr, &stringList, holderString );

                     threadWkgRef = interruptManager( &threadHeadRef , threadPtr, activeTime, INTERRUPT );

                     total = configPtr->ioCycleRate*localOpWkgRef->intArg2;

                     pthread_create(&thread1, NULL, threadTimer, (void*) &total );

                     pthread_join(thread1, NULL);

                    }
                 // else checks for non-premptive
                 else
                    {
                     accessTimer( LAP_TIMER, time );

                     sprintf( holderString, "%s, Process: %d, %s %sput operation start\n", time, pcbWkgRef->pid,
                                       localOpWkgRef->strArg1, localOpWkgRef->inOutArg );
                     displayAndLog( logToCodePtr, &stringList, holderString );

                     total = configPtr->ioCycleRate*localOpWkgRef->intArg2;

                     pthread_create(&thread1, NULL, threadTimer, (void*) &total );

                     pthread_join(thread1, NULL);

                     accessTimer( LAP_TIMER, time );

                     sprintf( holderString, "%s, Process: %d, %s %sput operation end\n", time, pcbWkgRef->pid,
                                         localOpWkgRef->strArg1, localOpWkgRef->inOutArg );
                     displayAndLog( logToCodePtr, &stringList, holderString );
                    }

                }
             // check for memory process
             else if(compareString( localOpWkgRef->command, "mem" ) == STR_EQ &&
                      compareString( localOpWkgRef->strArg1, "allocate" ) == STR_EQ)
                {
                 // lap timer
                 accessTimer( LAP_TIMER, time );

                 memCounter++;

                 sprintf( holderString, "%s, Process: %d, %s %s request (%d, %d)\n",
                         time, pcbWkgRef->pid, localOpWkgRef->command,
                         localOpWkgRef->strArg1, localOpWkgRef->intArg2,
                         localOpWkgRef-> intArg3 );

                 displayAndLog( logToCodePtr, &stringList, holderString );

                 accessTimer( LAP_TIMER, time );


                 allocateFlag = checkforAllocation( memCounter, &mmuHeadRef, localOpWkgRef, configPtr );

                 if( memCounter == 1 )
                    {
                     printf( "--------------------------------------------------\n" );
                     printf( "After memory initialization\n" );
                     displayMMU( mmuHeadRef );
                     printf( "--------------------------------------------------\n" );
                    }

                 // check if memory was allocated
                 if( allocateFlag )
                    {
                     printf( "--------------------------------------------------\n" );
                     printf("After allocate success\n");
                     displayMMU( mmuHeadRef );
                     printf( "--------------------------------------------------\n" );
                     sprintf( holderString, "%s, Process: %d, successful %s %s request\n", time, pcbWkgRef->pid,
                                                                localOpWkgRef->command, localOpWkgRef->strArg1 );
                     displayAndLog( logToCodePtr, &stringList, holderString );

                    }
                 else
                    {
                     printf( "--------------------------------------------------\n" );
                     printf("After allocate overlap failure\n");
                     // display MMU information
                     displayMMU( mmuHeadRef );
                     printf( "--------------------------------------------------\n" );

                     sprintf( holderString, "%s, Process: %d, failed %s %s request\n", time, pcbWkgRef->pid,
                                                             localOpWkgRef->command, localOpWkgRef->strArg1 );
                     displayAndLog( logToCodePtr, &stringList, holderString );

                     failFlag = true;

                    }

                 }
              else if(compareString( localOpWkgRef->command, "mem" ) == STR_EQ &&
                      compareString( localOpWkgRef->strArg1, "access" ) == STR_EQ)
                 {
                  accessFlag = checkforAccess( &mmuHeadRef, localOpWkgRef );

                  if( accessFlag )
                     {
                      printf( "--------------------------------------------------\n" );
                      printf("After access success\n");
                      displayMMU( mmuHeadRef );
                      printf( "--------------------------------------------------\n" );
                      sprintf( holderString, "%s, Process: %d, successful %s %s request\n", time, pcbWkgRef->pid,
                                                                localOpWkgRef->command, localOpWkgRef->strArg1 );
                      displayAndLog( logToCodePtr, &stringList, holderString );
                     }
                  else
                     {
                      printf( "--------------------------------------------------\n" );
                      printf("After access failure\n");
                      // display MMU information
                      displayMMU( mmuHeadRef );
                      printf( "--------------------------------------------------\n" );

                      sprintf( holderString, "%s, Process: %d, failed %s %s request\n", time, pcbWkgRef->pid,
                                                               localOpWkgRef->command, localOpWkgRef->strArg1 );
                      displayAndLog( logToCodePtr, &stringList, holderString );

                      failFlag = true;
                     }
                 }

             localOpWkgRef = localOpWkgRef->nextNode;
            }

         accessTimer( LAP_TIMER, time );

         if( failFlag )
            {
             sprintf( holderString, "\n%s, OS: Segmentation fault, Process %d ended\n", time, pcbWkgRef->pid );
             displayAndLog( logToCodePtr, &stringList, holderString );
            }
         else
            {
             sprintf( holderString, "\n%s, OS: Process %d ended\n", time, pcbWkgRef->pid );
             displayAndLog( logToCodePtr, &stringList, holderString );
            }


         clearMMUList( mmuHeadRef );

         mmuHeadRef = NULL;

         checkforAllocation( 1, &mmuHeadRef, localOpWkgRef, configPtr );

         printf( "--------------------------------------------------\n" );
         printf("After clear one process success\n");
         displayMMU( mmuHeadRef );
         printf( "--------------------------------------------------\n" );



         pcbWkgRef->appStates = EXITING;

         accessTimer( LAP_TIMER, time );



         sprintf( holderString, "%s, OS: Process %d set to EXIT\n", time, pcbWkgRef->pid );
         displayAndLog( logToCodePtr, &stringList, holderString );


         pcbWkgRef = getNextProcess( pcbHeadRef, configPtr );

        }

     accessTimer( LAP_TIMER, time );

     // Print System stop
     sprintf( holderString, "%s, OS: System Stop\n", time );
     displayAndLog( logToCodePtr, &stringList, holderString );


     printf( "--------------------------------------------------\n" );
     printf("After clear all process success\n");
     printf("No memory configured\n");
     printf( "--------------------------------------------------\n" );


     accessTimer( LAP_TIMER, time );

     // print Sim end
     sprintf( holderString, "%s, OS: Simulation end\n", time );
     displayAndLog( logToCodePtr, &stringList, holderString );

     // log to fiLe
     if( logToCodePtr == LOGTO_FILE_CODE || logToCodePtr == LOGTO_BOTH_CODE )
        {
         FILE *filePtr;

         filePtr = fopen( configPtr->logToFileName, "w+" );



         fprintf( filePtr, "==================================================\n");
         fprintf( filePtr, "Simulator Log File Header\n");

         fprintf( filePtr, "File Name                     : %s\n", configPtr->metaDataFileName );
         fprintf( filePtr, "CPU Scheduling                  : %d\n", configPtr->cpuSchedCode );
         fprintf( filePtr, "Quantum Cycles                  : %d\n", configPtr->quantumCycles );
         fprintf( filePtr, "Memory Available (KB)           : %d\n", configPtr->memAvailable );
         fprintf( filePtr, "Processor Cycle Rate (ms/cycle) : %d\n", configPtr->procCycleRate );
         fprintf( filePtr, "I/O Cycle Rate (ms/cycle)       : %d\n", configPtr->ioCycleRate );

         wkgStringRef = stringList;

         while( wkgStringRef != NULL )
            {
             // print each element of LL
             fprintf( filePtr, "%s", wkgStringRef->formatString );


             // itterate
             wkgStringRef = wkgStringRef->nextNode;
            }

         fclose( filePtr );
        }
    free( pcbWkgRef );
    clearPCBList( pcbHeadRef );
    clearStringList( stringList );
   }
