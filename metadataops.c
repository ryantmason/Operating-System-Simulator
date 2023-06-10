//
//
//
#include <stdio.h>
#include "metadataops.h"

const int BAD_ARG_VAL = -1;

/*
Function Name: addNode
Algorithm: adds op command structure with data to a linked list
Precondition: Linked List pointer assigned to null or to one op command link,
              struct pointer assigned to op command struct data
Postcondition: assigns new structure node to beginning of linked list or and
               of estabLished linked list
Exceptions: none
Notes: assumes memory access/availability
*/
OpCodeType *addNode (OpCodeType *localPtr, OpCodeType *newNode)
   {
   if(localPtr == NULL)
      {


      localPtr = (OpCodeType *) malloc(sizeof(OpCodeType));

      localPtr->pid = newNode->pid;

      copyString(localPtr ->command, newNode -> command);
      copyString(localPtr->inOutArg, newNode->inOutArg);
      copyString(localPtr->strArg1, newNode->strArg1);
      localPtr->intArg2 = newNode-> intArg2;
      localPtr->intArg3 = newNode->intArg3;
      localPtr->opEndTime = newNode->opEndTime;

      localPtr->nextNode = NULL;

      return localPtr;
      }

      localPtr->nextNode = addNode(localPtr->nextNode, newNode);

      return localPtr;
   }


/*
Function Name: clearMetaDataList
Algorithm: recursively iterates through op code linked list, returns memory
           to OS from the bottom of the list upward
Precondition: linked list, with or without data
Postcondition: all node memory, if any, is returned to OS, return pointer
               (head) is set to null
Exceptions: none
Notes: none
*/
OpCodeType *clearMetaDataList(OpCodeType *localPtr)
   {
   if(localPtr != NULL)
      {
      clearMetaDataList (localPtr->nextNode);

      free( localPtr);

      localPtr = NULL;
      }

      return NULL;
   }


/*
Function Name: displayMetaData
Algorithm: iterates through op code Linked List,
          displays op code data individually
Precondition: Linked List, with or without data
             (should not be called if no data)
Postcondition: displays all op codes in list
Exceptions: none
Notes: none
*/
void displayMetaData( OpCodeType *localPtr)
   {
   printf("Meta-Data File Display\n");
   printf("--------------------\n\n");

   while( localPtr != NULL)
      {
      printf("Op Code: ");

      printf("/pid: %d", localPtr->pid);

      printf("/cmd: %s", localPtr-> command);

      if(compareString(localPtr->command, "dev") == STR_EQ)
         {
         printf("/io: %s", localPtr-> inOutArg);
         }

      else
      {
	printf("/io: NA");
      }

      printf("\n\t /arg 1: %s", localPtr->strArg1);
      printf("/arg 2: %d", localPtr->intArg2);
      printf("/arg 3: %d", localPtr->intArg3);
      printf("/op end time: %8.6f", localPtr->opEndTime);

      printf("\n\n");

      localPtr = localPtr->nextNode;

      }
   }


/*
Function Name: getCommand
Algorithm: gets first three letters of input string to get command
Precondition: provided starting index
Postcondition: returns command (ref param) and updated index (return)
Exceptions: none Notes: none
*/
int getCommand( char *cmd, char *inputStr, int index)
   {
   int lengthOfCommand = 3;

   while( index < lengthOfCommand)
      {
      cmd[ index ] = inputStr[ index ];

      index++;

      cmd[ index ] = NULL_CHAR;
      }
    return index;
   }


/*
Function Name: getMetaData
Algorithm: acquires the metadata items from file, stores them in a linked list
Precondition: provided file name
Postcondition: returns success (Boolean), returns linked list
              head via parameter, returns endstate/error message via parameter
Exceptions: function halted and error message returned if bad input data
Notes: none
*/
bool getMetaData( char *fileName, OpCodeType ** opCodeDataHead, char *endStateMsg)
   {
   const char READ_ONLY_FLAG[] = "r";

   int accessResult, startCount = 0, endCount =0;
   char dataBuffer [MAX_STR_LEN];
   bool returnState = true;
   OpCodeType *newNodePtr;
   OpCodeType *localHeadPtr = NULL;
   FILE *fileAccessPtr;

   *opCodeDataHead = NULL;

   copyString(endStateMsg, "MetaData file upload successful");

   fileAccessPtr = fopen(fileName, READ_ONLY_FLAG);

   if(fileAccessPtr == NULL)
      {

      copyString( endStateMsg, "Metadata file access error");

      return false;
      }

   if( getStringToDelimiter(fileAccessPtr, COLON,
		dataBuffer) ==false
   	   || compareString(dataBuffer, "Start Program Meta-Data Code") !=STR_EQ)
   	   {

   	   fclose(fileAccessPtr);

   	   copyString(endStateMsg, "Currupt metadata leader line error");
   	   return false;
   	   }

   newNodePtr = (OpCodeType *) malloc(sizeof(OpCodeType) );

   // get first op command

   accessResult = getOpCommand(fileAccessPtr, newNodePtr);

   startCount = updateStartCount( startCount, newNodePtr->strArg1);
   endCount = updateEndCount( endCount, newNodePtr->strArg1);

   if(accessResult != 	COMPLETE_OPCMD_FOUND_MSG)
      {
      fclose(fileAccessPtr);

      *opCodeDataHead = clearMetaDataList( localHeadPtr);

      free(newNodePtr);

      copyString(endStateMsg, "Metadata incomplete first op command found");

      return false;
      }

   while( accessResult == COMPLETE_OPCMD_FOUND_MSG)
      {
         localHeadPtr = addNode( localHeadPtr, newNodePtr);

         accessResult = getOpCommand(fileAccessPtr, newNodePtr);


         startCount = updateStartCount( startCount, newNodePtr->strArg1);
         endCount = updateEndCount( endCount, newNodePtr->strArg1);
      }

      if(accessResult == LAST_OPCMD_FOUND_MSG)
         {
            if(startCount == endCount)
               {
               localHeadPtr = addNode( localHeadPtr, newNodePtr);

               accessResult = NO_ERR;

               if(getStringToDelimiter(fileAccessPtr, PERIOD,
		dataBuffer) ==false
                  || compareString( dataBuffer, "End Program Meta-Data Code") !=STR_EQ)

                  {
                     accessResult = MD_CORRUPT_DESCRIPTOR_ERR;


                     copyString( endStateMsg, "MetaData corrupted descriptor error");
                  }
               }
         }

      else
      {
      copyString( endStateMsg, "Corrupted metadata op code");

      returnState = false;
      }

      if(accessResult != NO_ERR)
         {
         localHeadPtr = clearMetaDataList(localHeadPtr);
         }

      //close file
      fclose(fileAccessPtr);
      //release mem
      free(newNodePtr);

      *opCodeDataHead = localHeadPtr;


   return returnState;
   }


/*
Function Name: getOpCommand
Algorithm: acquires one op command, verifies all parts of it,
           returns as parameter
Precondition: file is open and file cursor is at beginning of an op code
Postcondition: in correct operation, finds, tests, and returns op command
              as parameter, and returns status as integer - either complete
              op command found, or last op command found
Exceptions: correctLy and appropriateLy (without program failure) responds to
           and reports file access faiLare, incorrectLy formatted op command
           letter, incorrectLy formatted op command name, incorrect or out of
           range op command value
Notes: none
*/
int getOpCommand(FILE *filePtr, OpCodeType *inData)
   {
   const int MAX_CMD_LENGTH = 5;
   const int MAX_ARG_STR_LENGTH = 15;


   int accessResult, numBuffer =0;
   char strBuffer[ STD_STR_LEN ];
   char cmdBuffer[ MAX_CMD_LENGTH ];
   char argStrBuffer[ MAX_ARG_STR_LENGTH ];
   int runningStringIndex =0;

   bool arg2FailureFlag = false;
   bool arg3FailureFlag = false;


   // get whole op command
   accessResult = getStringToDelimiter(filePtr, SEMICOLON,
		                                   strBuffer);

   if(accessResult == NO_ERR)
      {
       runningStringIndex = getCommand( cmdBuffer, strBuffer, runningStringIndex);
       copyString( inData->command, cmdBuffer);
      }

   else
      {
       inData=NULL;

       return OPCMD_ACCESS_ERR;
      }

    if(verifyValidCommand(cmdBuffer) == false)
       {
       return CORRUPT_OPCMD_ERR;
       }

    inData->pid=0;
    inData->inOutArg[ 0 ] = NULL_CHAR;
    inData->intArg2 =0;
    inData->intArg3=0;
    inData->opEndTime=0.0;
    inData->nextNode=NULL;

    if(compareString(cmdBuffer, "dev") == STR_EQ)
       {
          runningStringIndex = getStringArg( argStrBuffer, strBuffer, runningStringIndex);

          copyString(inData->inOutArg, argStrBuffer);

          if(compareString(argStrBuffer, "in")!=STR_EQ && compareString(argStrBuffer, "out")!=STR_EQ)
          {
             return CORRUPT_OPCMD_ARG_ERR;
          }


       }

       runningStringIndex = getStringArg(argStrBuffer, strBuffer, runningStringIndex);

       copyString(inData->strArg1, argStrBuffer);


       //check for legitimate first string arg
       if(verifyFirstStringArg( argStrBuffer ) == false)
       {
          return CORRUPT_OPCMD_ARG_ERR;
       }

       if(compareString(inData->command, "sys") == STR_EQ && compareString(inData->strArg1,"end") == STR_EQ)
       {
          return LAST_OPCMD_FOUND_MSG;
       }

       // CHECK FOR APP START SECONDS
       if(compareString(inData->command, "app") == STR_EQ && compareString(inData->strArg1, "start") == STR_EQ)
       {
       runningStringIndex = getNumberArg(&numBuffer, strBuffer, runningStringIndex);

          if(numBuffer <= BAD_ARG_VAL)
           {
              arg2FailureFlag = true;
           }

        inData->intArg2 = numBuffer;
       }
       // CHECK FOR CPU CYCLE TIME
       else if(compareString(inData->command, "cpu")==STR_EQ)
          {
             runningStringIndex = getNumberArg( &numBuffer, strBuffer, runningStringIndex);

             if(numBuffer <= BAD_ARG_VAL)
              {
                 arg2FailureFlag = true;
              }

              inData->intArg2 = numBuffer;
          }

       // CHECK FOR DEVICE CYCLE TIME
       else if(compareString(inData->command, "dev")==STR_EQ)
          {
             runningStringIndex = getNumberArg( &numBuffer, strBuffer, runningStringIndex);

             if(numBuffer <= BAD_ARG_VAL)
              {
                 arg2FailureFlag = true;
              }

              inData->intArg2 = numBuffer;
          }
       // CHECK FOR MEMORY
       else if(compareString(inData->command, "mem")==STR_EQ)
          {
             runningStringIndex = getNumberArg( &numBuffer, strBuffer, runningStringIndex);

             if(numBuffer <= BAD_ARG_VAL)
              {
                 arg2FailureFlag = true;
              }

              inData->intArg2 = numBuffer;

              runningStringIndex = getNumberArg( &numBuffer, strBuffer, runningStringIndex);

             if(numBuffer <= BAD_ARG_VAL)
              {
                 arg3FailureFlag = true;
              }

              inData->intArg3 = numBuffer;

          }

          if(arg2FailureFlag == true || arg3FailureFlag == true)
             {
             return CORRUPT_OPCMD_FOUND_MSG;
             }

          return COMPLETE_OPCMD_FOUND_MSG;
   }

/*
Function Name: getNumberArg
Algorithm: skips Leading white space, acquires next integer from op
          command input string ending at comma or end of string
Precondition: input string has some remaining string argument
Postcondition: in correct operation, captures next integer argument, returns
              index Location after process finished
Exceptions: none
Notes: none
*/
int getNumberArg(int *number, char *inputStr, int index)
   {
      bool foundDigit = false;
      *number = 0;
      int multiplier =1;

      while(inputStr[ index ] <= SPACE || inputStr[index] == COMMA)
         {

            index++;
         }

      while( isDigit( inputStr[ index ] ) == true &&
      					inputStr[ index ] != NULL_CHAR)
      	{
      	   foundDigit = true;

      	   (*number) = (*number) * multiplier + inputStr[ index ] - '0';

      	   index++; multiplier =10;
      	}

      	if(foundDigit == false)
      	   {
      	   *number = BAD_ARG_VAL;
      	   }
      	return index;
   }

int getStringArg( char *strArg, char*inputStr, int index)
   {
      int localIndex =0;

      while(inputStr[index] <= SPACE || inputStr[index] == COMMA)
         {
         index++;
         }

      while( inputStr[ index] != COMMA && inputStr[index] != NULL_CHAR)
         {
         strArg[localIndex] = inputStr[ index];

         index++;
         localIndex++;

         strArg[ localIndex] = NULL_CHAR;
         }
      return index;
   }

bool isDigit( char testChar)
   {
      if(testChar >= '0' && testChar <= '9')
         {
            return true;
         }

     return false;
   }

int updateEndCount(int count, char *opString)
   {
      if(compareString(opString, "end") == STR_EQ)
         {
            return count +1;
         }

     return count;

   }

int updateStartCount( int count, char *opString)
   {
      if(compareString(opString, "start") == STR_EQ)
         {
            return count +1;
         }

      return count;
   }

bool verifyFirstStringArg(char *strArg)
   {

   if(compareString(strArg, "access") == STR_EQ
      || compareString(strArg, "allocate") == STR_EQ
      || compareString(strArg, "end") == STR_EQ
      || compareString(strArg, "ethernet") == STR_EQ
      || compareString(strArg, "hard drive") == STR_EQ
      || compareString(strArg, "keyboard") == STR_EQ
      || compareString(strArg, "monitor") == STR_EQ
      || compareString(strArg, "printer") == STR_EQ
      || compareString(strArg, "process") == STR_EQ
      || compareString(strArg, "serial") == STR_EQ
      || compareString(strArg, "sound signal") == STR_EQ
      || compareString(strArg, "start") == STR_EQ
      || compareString(strArg, "usb") == STR_EQ
      || compareString(strArg, "video signal") == STR_EQ)
      {
      return true;
      }
      return false;
   }

bool verifyValidCommand( char *testCmd)
   {
     if(compareString(testCmd, "sys") == STR_EQ
       ||compareString(testCmd, "app") == STR_EQ
       ||compareString(testCmd, "cpu") == STR_EQ
       ||compareString(testCmd, "mem") == STR_EQ
       ||compareString(testCmd, "dev") == STR_EQ)
       {
       return true;
       }
      return false;
   }
