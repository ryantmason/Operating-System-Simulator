// header files
#include "StringUtils.h"

// global constants

// function prototypes

// Programs

/*
Name: compareString
Process: compares two strings with the following results:
         if left string less than right string, returns less than zero
         if left string greater than right string, returns greater than zero
         if left string equals right string, returns zero
         - equals test includes length
Function Input/Paramters: c-style left and right strings (char *)
Function Output/Parameters: none
Function Output/Returned: results as specified (int)
Device Input/Keyboard: none
Device Output/Monitor: none
Dependenices: getStringLength
*/

int compareString( const char *oneStr, const char *otherStr )
   {
    // intialize function/vars
    int diff, index = 0;

    //loop to the end of shortest string,
    // with overrun protection
    while( oneStr[ index ] != NULL_CHAR
            && otherStr[ index ] != NULL_CHAR
                         && index < MAX_STR_LEN ) 
       {
        // get dif in chars
        diff = oneStr[ index ] - otherStr[ index ];

	// check for dif between chars
        if( diff != 0 )
           {
            // return dif
            return diff;
           }
        // increment index
        index++;
       }
    // end loop

    // return dif in lengths, if any
       // function: getStringLength
    return getStringLength( oneStr ) - getStringLength( otherStr );
   }

/*
Name: concatenateString
Process: appends one string onto another 
Function Input/Paramters: c-style source string (char *)
Function Output/Parameters: c-style destination string (char *)
Function Output/Returned: none
Device Input/Keyboard: none
Device Output/Monitor: none
Dependenices: getStringLength
*/
void concatenateString( char *destStr, const char *sourceStr )
   {
    int destIndex = getStringLength( destStr );
    
    int sourceStrLen = getStringLength( sourceStr );

    char *tempStr;

    int sourceIndex = 0;

    tempStr = (char *)malloc( sizeof(sourceStrLen + 1 ) );
    copyString( tempStr, sourceStr );

    while( tempStr[ sourceIndex ] != NULL_CHAR && destIndex < MAX_STR_LEN )
       {
        destStr[ destIndex ] = tempStr[ sourceIndex];
        
	destIndex++; sourceIndex++;
    	
	destStr[ destIndex ] = NULL_CHAR;
       }

    free( tempStr );
   }
    






/*
Name: copyString
Process: copies one string into another,
         overwriting data in the destination string
Function Input/Paramters: c-style source string (char *)
                          c-style test string (char *)
Function Output/Parameters: c-style destination string (char *)
Function Output/Returned: none
Device Input/Keyboard: none
Device Output/Monitor: none
Dependenices: getStringLength
*/
void copyString( char *destStr, const char *sourceStr )
   {
    int index = 0;

    if( destStr != sourceStr )
       {
 	while( sourceStr[ index] != NULL_CHAR && index < MAX_STR_LEN)
           {
 	    destStr[ index ] = sourceStr[ index ];

            index++;

            destStr[ index ] = NULL_CHAR;
	   }

       }

   }


/*
Name: findSubString
Process: linear search for given substring within another string
Function Input/Paramters: c-style source string (char *)
Function Output/Parameters: c-style destination string (char *)
Function Output/Returned: none
Device Input/Keyboard: none
Device Output/Monitor: none
Dependenices: getStringLength
*/
int findSubString( const char *testStr, const char *searchSubStr )
   {
    int testStrLen = getStringLength( testStr );

    int masterIndex = 0;

    int searchIndex, internalIndex;

    while( masterIndex < testStrLen )
       {
  	internalIndex = masterIndex;

   	searchIndex = 0;

	while( internalIndex <= testStrLen
                     && testStr[ internalIndex ] == searchSubStr[ searchIndex ] )
     	   {
  	    internalIndex++; searchIndex++;

	    if( searchSubStr[ searchIndex ] == NULL_CHAR )
   	       {
		return masterIndex;
	       }
	   }
	masterIndex++;
       }    
    return SUBSTRING_NOT_FOUND;
   }

/*
Name: getStringConstrained
Process: 
Function Input/Paramters: 
Function Output/Parameters: 
Function Output/Returned: none
Device Input/Keyboard: none
Device Output/Monitor: none
Dependenices: getStringLength
*/
bool getStringConstrained(
			  FILE *inStream,
			  bool clearLeadingNonPrintable,
			  bool clearLeadingSpace,
			  bool stopAtNonPrintable,
			  char delimiter,
			  char *capturedString
			 )
   {
    int intChar = EOF, index = 0;

    capturedString[ index ] = NULL_CHAR;

    intChar = fgetc( inStream );

    // loop to clear non printable or space, if indicated
    while( ( intChar != EOF )
  	     && ( ( clearLeadingNonPrintable && intChar < (int)SPACE )
			    || ( clearLeadingSpace && intChar == (int)SPACE ) )
	 )
       {
        intChar = fgetc( inStream );
       }
    // end clear non printable/space loop
    
    if( intChar == EOF )
       {
        return false;
       }

    while( ( intChar != EOF && index < MAX_STR_LEN -1 ) && 
             ( ( stopAtNonPrintable && intChar >= (int)SPACE ) ||
                 ( !stopAtNonPrintable ) ) && ( intChar != (char)delimiter )
         )
       {
        capturedString[ index ] = (char)intChar;
        
        index ++;

        capturedString[ index ] = NULL_CHAR;

        intChar = fgetc( inStream );
       }
    return true;
   }

/*
Name: getStringLength
Process: finds the length of a string 
         by counting characters up to the NULL_CHAR character
Function Input/Paramters: c-style string (char *)
Function Output/Parameters: none
Function Output/Returned: length of string (int)
Device Input/Keyboard: none
Device Output/Monitor: none
Dependenices: none
*/
int getStringLength( const char *inpString )
   {
    // initialize function/variables
    int index = 0;

    // loop to end of string, protect from overflow
    while( index < STD_STR_LEN && inpString[ index ] != NULL_CHAR )
       {
        // update index
        index++;
       }
    // end loop
 
    // return index/length    
    return index;
   }

/*
Name: getStringToDelimiter
Process: captures a string from the input stream
         to a specified delimiter;
         Note: consumes delimiter  
Function Input/Paramters: input stream (FILE *)
		          stops at specified delimiter (char),
Function Output/Parameters: string returned (char *)
Function Output/Returned: success of operation (bool)
Device Input/Keyboard: none
Device Output/Monitor: none
Dependenices: getStringConstrained
*/
bool getStringToDelimiter(
			  FILE *inStream,
			  char delimiter,
			  char *capturedString
			 )
  {
   return getStringConstrained(
		  inStream,
		  true,
		  true,
		  true,
		  delimiter,
 		  capturedString
			      );   

  }

/*
Name: getStringToLineEnd
Process: captures a string from the input stream
         to the end of the line 
Function Input/Paramters: input Stream (FILE *)
Function Output/Parameters: string returned (char *)
Function Output/Returned: success of operation (bool)
Device Input/Keyboard: none
Device Output/Monitor: none
Dependenices: getStringConstrained
*/
bool getStringToLineEnd(
			FILE *inStream,
			char *capturedString
		       )
   {
    return getStringConstrained(
		  inStream,
		  true,
		  true,
		  true,
		  NON_PRINTABLE_CHAR,
 		  capturedString
			      );    

   }

/*
Name: getSubString
Process: captures sub string within larger string
 	 between two inclusive indices.
 	 returns empty string if either index is out of range,
	 assumes enough memory in destination string 
Function Input/Paramters: c-style source string (char *),
                          start and end indices (int)
Function Output/Parameters: c-style destination string (char *)
Function Output/Returned: none
Device Input/Keyboard: none
Device Output/Monitor: none
Dependenices: getStringLength, malloc, copyString, free
*/

void getSubString( char *destStr, const char *sourceStr,
                        int startIndex, int endIndex )
   {
    int sourceStrLen = getStringLength( sourceStr );

    int destIndex = 0;

    int sourceIndex = startIndex;

    char *tempStr;

    if( startIndex >= 0 && startIndex <= endIndex
                                   && endIndex < sourceStrLen )
       {
        tempStr = (char *)malloc(sizeof( sourceStrLen + 1 ) );
        copyString( tempStr, sourceStr );

        while( sourceIndex <= endIndex )
  	   {
 	    destStr[ destIndex ] = tempStr[ sourceIndex ];

            destIndex++; sourceIndex++;

  	    destStr[ destIndex ] = NULL_CHAR;
	   }
	free( tempStr );
       }
   }

/*
Name: setStrToLowerCase
Process: 
Function Input/Paramters: 
Function Output/Parameters: 
Function Output/Returned: none
Device Input/Keyboard: none
Device Output/Monitor: none
Dependenices: toLowerCase
*/
void setStrToLowerCase( char *destStr, const char *sourceStr )
   {
    int sourceStrLen = getStringLength( sourceStr );
    
    char *tempStr;

    int index = 0;

    tempStr = (char *)malloc( sizeof( sourceStrLen + 1 ) );
    copyString( tempStr, sourceStr );

    while( tempStr[ index ] != NULL_CHAR && index < MAX_STR_LEN )
       {
	destStr[ index ] = toLowerCase( tempStr[ index ] );
        
	index++;

	destStr[ index ] = NULL_CHAR;
       }
    free( tempStr );
   }

/*
Name: toLowerCase
Process: if character is upper case, sets it to lower case;
         otherwise returns character unchanged 
Function Input/Paramters: test character (char)
Function Output/Parameters: none
Function Output/Returned: character to set to lower case, if appropriate
Device Input/Keyboard: none
Device Output/Monitor: none
Dependenices: toLowerCase
*/
char toLowerCase( char testChar )
   {
    if( testChar >= 'A' && testChar <= 'Z' )
       {
  	return testChar - 'A' + 'a';
       }
    return testChar;
   }


