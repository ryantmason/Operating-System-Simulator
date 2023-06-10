#ifndef METADATAOPS_H
#define METADATAOPS_H
// header files
#include "datatypes.h"
#include "StringUtils.h"


typedef enum { MD_FILE_ACCESS_ERR = 3,
		MD_CORRUPT_DESCRIPTOR_ERR,
		OPCMD_ACCESS_ERR,
		CORRUPT_OPCMD_ERR,
		CORRUPT_OPCMD_ARG_ERR,
		UNBALANCED_START_END_ERR,
		COMPLETE_OPCMD_FOUND_MSG,
		CORRUPT_OPCMD_FOUND_MSG,
		LAST_OPCMD_FOUND_MSG} OpCodeMessages;

extern const int BAD_ARG_VAL;


// prototypes
OpCodeType *addNode (OpCodeType *localPtr, OpCodeType *newNode);
OpCodeType *clearMetaDataList(OpCodeType *localPtr);
void displayMetaData( OpCodeType *localPtr);
int getCommand( char *cmd, char *inputStr, int index);
bool getMetaData( char *filename, OpCodeType ** opCodeDataHead, char *endStateMsg);
int getOpCommand(FILE *filePtr, OpCodeType *inData);
int getNumberArg(int *number, char *inputStr, int index);
int getStringArg( char *strArg, char*inputStr, int index);
bool isDigit( char testChar);
int updateStartCount( int count, char *opString);
int updateEndCount(int count, char *opString);
bool verifyFirstStringArg(char *strArg);
bool verifyValidCommand( char *testCmd);
#endif
