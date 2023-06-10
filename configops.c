// header files
#include "configops.h"

/*
Function Name: clearConfigData
Algorithm: frees allocated memory for config data
Precondition: configData holds allocated data set
Postcondition: memory is freed, pointer is set to null
Exceptions: none
Notes: none
*/
ConfigDataType *clearConfigData( ConfigDataType *configData )
   {
    if( configData != NULL )
       {
        free( configData );

	configData = NULL;
       }

    return NULL;
   }

/*
Function Name: configCodeToString
Algorithm: utility function to support display of CPU scheduLing or Log To code strings
Precondition: code variable holds constant value from ConfigoataCodes for item (e.g., CPU SCHED SRTF P, LOGTO FILE CODE, etc.)
Postcondition: string parameter holds correct string associated with the given constant
Exceptions: none
Notes: none
*/
void configCodeToString( int code, char *outString )
   {
    
    char displayStrings[ 8 ][ 10 ] = { "SJF-N", "SRTF-P", "FCFS-P",
                                     "RR-P", "FCFS-N", "Monitor", 
                                     "File", "Both" }; 

    copyString( outString, displayStrings[ code ] );
   }



/*
Function Name: displayConfigData
Algorithm: disagnostic function to show config data output
Precondition: configData holds allocated data set
Postcondition: data is printed to screen
Exceptions: none
Notes: none
*/
void displayConfigData( ConfigDataType *configData )
   {
    char displayString[ STD_STR_LEN ];

    printf( "Config File Display\n" ); 
    printf( "-------------------\n" ); 
    printf( "Version                : %3.2f\n", configData->version ); 
    printf( "Program file name      : %s\n", configData->metaDataFileName ); 
    configCodeToString( configData->cpuSchedCode, displayString ); 
    printf( "CPU schedule selection : %s\n", displayString ); 
    printf( "Quantum time           : %d\n", configData->quantumCycles ); 
    printf( "Memory Available       : %d\n", configData->memAvailable ); 
    printf( "Process cycle rate     : %d\n", configData-> procCycleRate ); 
    printf( "I/O cycle rate         : %d\n", configData->ioCycleRate ); 
    configCodeToString( configData->cpuSchedCode, displayString );
    printf( "Log to selection       : %s\n", displayString );
    printf( "Log file name          : %s\n\n", configData->logToFileName );
   }

/*
Function Name: getConfigData 
Algorithm: opens file, acquires configuration data, returns pointer to data structure 
Precondition: for correct operation, file is available, is formatted correctly, 
              and has all configuration tines and data although 
              the configurction tines are not required to be 
              in a specifid order 
Postcondition: in correct operation, 
               returns pointer to correct configuration data structure 
Exceptions: correctly and appropriately (without program failure) 
            responds to and reports file access failure, 
            incorrectly formatted Lead or end descriptors, 
            incorrectly formatted prompt, data out of range, 
            and incomplete file conditions 
Notes: none  

*/
bool getConfigData( char *fileName,
                                ConfigDataType **configData, char *endStateMsg )
   {
    const int NUM_DATA_LINES = 9;
  
    const char READ_ONLY_FLAG[] = "r";

    ConfigDataType *tempData;

    FILE *fileAccessPtr;
    char dataBuffer[ MAX_STR_LEN ];
    int intData, dataLineCode, lineCtr = 0;
    double doubleData;   

    copyString( endStateMsg, "Configuration file upload successful" );

    *configData = NULL;

    fileAccessPtr = fopen( fileName, READ_ONLY_FLAG );

    if( fileAccessPtr == NULL )
       {
     	copyString( endStateMsg, "Configuration file access error" );

        return false;
       }

    if( getStringToDelimiter( fileAccessPtr, COLON, dataBuffer ) == false 
        || compareString( dataBuffer, "Start Simulator Configuration File" ) != STR_EQ )
       {
	fclose( fileAccessPtr );

    	copyString( endStateMsg, "Corrupt configuration leader line error" );
	
	return false;
       }

    tempData = (ConfigDataType *) malloc( sizeof( ConfigDataType ) );

    while( lineCtr < NUM_DATA_LINES )
       {
  	if( getStringToDelimiter( fileAccessPtr, COLON, dataBuffer ) == false )
           {
	    free( tempData );

     	    fclose( fileAccessPtr );

	    copyString( endStateMsg, "Configuration start line capture error" );

	    return false;
           }

	dataLineCode = getDataLineCode( dataBuffer );

   	if( dataLineCode != CFG_CORRUPT_PROMPT_ERR )
	   {
	    if( dataLineCode == CFG_VERSION_CODE )
	       {
	        fscanf( fileAccessPtr, "%lf", &doubleData );

	       }
	    else if( dataLineCode == CFG_MD_FILE_NAME_CODE
		     || dataLineCode == CFG_LOG_FILE_NAME_CODE
		        || dataLineCode == CFG_CPU_SCHED_CODE
			   || dataLineCode == CFG_LOG_TO_CODE )
	       {
	        fscanf( fileAccessPtr, "%s", dataBuffer );
	       }
	    else
	       {
                fscanf( fileAccessPtr, "%d", &intData ); 
	       }
	    if( valueInRange( dataLineCode, intData, doubleData, dataBuffer ) == true )
	       {
                switch( dataLineCode )
		   {
		    case CFG_VERSION_CODE: 
                       
                       tempData->version = doubleData; 
                       break; 

                    case CFG_MD_FILE_NAME_CODE: 

                       copyString( tempData->metaDataFileName, dataBuffer ); 
                       break; 
                    
                    case CFG_CPU_SCHED_CODE: 

                       tempData->cpuSchedCode = getCpuSchedCode( dataBuffer ); 
                       break; 

                    case CFG_QUANT_CYCLES_CODE: 

                       tempData->quantumCycles = intData; 
                       break; 

                    case CFG_MEM_AVAILABLE_CODE: 

                       tempData->memAvailable = intData; 
                       break; 

                    case CFG_PROC_CYCLES_CODE: 

                       tempData->procCycleRate = intData; 
                       break;

		    case CFG_IO_CYCLES_CODE: 
                     
                       tempData->ioCycleRate = intData; 
                       break; 

                    case CFG_LOG_TO_CODE: 

                       tempData->logToCode = getLogToCode( dataBuffer ); 
                       break; 

                    case CFG_LOG_FILE_NAME_CODE: 
                    
                       copyString( tempData->logToFileName, dataBuffer ); 
                       break;
 		   }

               }
            else
	       {
                free( tempData );

                fclose( fileAccessPtr );

                copyString( endStateMsg, "Configuration item out of range" );

                return false;
               }
       	   }
        else
	   {
            free( tempData );

            fclose( fileAccessPtr );

	    copyString( endStateMsg, "Corrupted configuration prompt" );

            return false;
	   }
 	lineCtr++;
       }
    if( getStringToDelimiter( fileAccessPtr, PERIOD, dataBuffer ) == false 
       || compareString( dataBuffer, "End Simulator Configuration File" ) != STR_EQ)
       {
        free( tempData );

 	fclose( fileAccessPtr );

   	copyString( endStateMsg, "Configuration end line capture error" );

	return false;
       }
    *configData = tempData;

    fclose( fileAccessPtr );

    return true; //check
   }

/* 
Function Name: getCpuSchedCode 
Algorithm: converts string data (e.g., "SJF-N", "SRTF-P") to constant code number 
           to be stored as integer 
Precondition: codeStr is a C-StyLe string with one of the specified cpu 
              scheduling operations 
Post condition: returns code representing scheduLing actions 
Exceptions: defaults to fcfs code 
Notes: none
*/ 
ConfigDataCodes getCpuSchedCode( char *codeStr )
   {
    int strLen = getStringLength( codeStr );
    char *tempStr = (char *)malloc( strLen + 1 );

    int returnVal = CPU_SCHED_FCFS_N_CODE;

    setStrToLowerCase( tempStr, codeStr );

    if( compareString( tempStr, "sjf-n" ) == STR_EQ )
       {
        returnVal = CPU_SCHED_SJF_N_CODE;        
       }
    if( compareString( tempStr, "srtf-p" ) == STR_EQ )
       {
        returnVal = CPU_SCHED_SRTF_P_CODE;
       }
     if( compareString( tempStr, "fcfs-p" ) == STR_EQ )
       {
        returnVal = CPU_SCHED_FCFS_P_CODE;
       }
     if( compareString( tempStr, "rr-p" ) == STR_EQ )
       {
        returnVal = CPU_SCHED_RR_P_CODE;
       }
    free( tempStr );


    return returnVal;
   }

/* 
Function Name: getDataLineCode 
Algorithm: tests string for one of known Leader strings, 
           returns Line number if string is correct, 
           returns CFG CORRUPT DATA ERR if string is 
           not found 
Precondition: datasuffer is valid C-StyLe string 
Postcondition: returns Line number of data item in terms 
               of a constant (e.g., CFG VERSION CODE, 
               CFG CPU SCHED CODE, etc.) 
Exceptions: returns CFG CORRUPT FILE ERR if string is
            not identified 
Notes: none 
*/ 

int getDataLineCode( char *dataBuffer ) 
   {  
    if( compareString( dataBuffer, "Version/Phase" ) == STR_EQ )
       { 
        return CFG_VERSION_CODE; 
       }
    if( compareString( dataBuffer, "File Path" ) == STR_EQ ) 
       { 
        return CFG_MD_FILE_NAME_CODE; 
       } 
    if( compareString( dataBuffer, "CPU Scheduling Code" ) == STR_EQ ) 
       { 
        return CFG_CPU_SCHED_CODE;
       } 
    if( compareString( dataBuffer, "Quantum Time (cycles)" ) == STR_EQ )
       { 
        return CFG_QUANT_CYCLES_CODE;
       }
    if( compareString( dataBuffer, "Memory Available (KB)" ) == STR_EQ )
       { 
        return CFG_MEM_AVAILABLE_CODE;
       } 
    if( compareString( dataBuffer, "Processor Cycle Time (msec)" )
                                                                  == STR_EQ )
       { 
	return CFG_PROC_CYCLES_CODE;
       }
    if( compareString( dataBuffer, "I/O Cycle Time (msec)" ) == STR_EQ )
       { 
        return CFG_IO_CYCLES_CODE; 
       } 
    if( compareString( dataBuffer, "Log To" ) == STR_EQ )
       { 
        return CFG_LOG_TO_CODE;
       } 
    if( compareString( dataBuffer, "Log File Path" ) == STR_EQ )
       {
        return CFG_LOG_FILE_NAME_CODE;
       }
    return CFG_CORRUPT_PROMPT_ERR;
   }

/* 
Function Name: getLogToCode 
Algorithm: converts string data (e.g., "File", "Monitor") to constant 
           code number to be stored as integer 
Precondition: codeStr is a C-StyLe string with one of the 
              specified Log to operations 
Post condition: returns code representing Log to actions 
Exceptions: defaults to monitor code 
Notes: none 
*/
ConfigDataCodes getLogToCode( char *logToStr )
   {
    int strLen = getStringLength( logToStr );
    char *tempStr = (char *)malloc(strLen + 1 );
    
    int returnVal = LOGTO_MONITOR_CODE;

    setStrToLowerCase( tempStr, logToStr );

    if( compareString( tempStr, "both" ) == STR_EQ )
       {
        returnVal = LOGTO_BOTH_CODE;
       }
    
    if( compareString( tempStr, "file" ) == STR_EQ )
       {
        returnVal = LOGTO_FILE_CODE;
       }

    free( tempStr );

    return returnVal;
   }

/* 
Function Name: valueInRange 
Algorithm: tests one of three values (int, double, string) for being in 
           specified range, depending on data code (i.e., specified config value) 
Precondition: one of the three data values is valid 
Postcondition: returns True if data is within specified parameters, False otherwise 
Exceptions: metadata or LogfiLe names are ignored and return True 
Notes: none 
*/
bool valueInRange( int lineCode, int intVal,
                                             double doubleVal, char *stringVal )
   {
    bool result = true;
    char *tempStr;
    int strLen;

    switch( lineCode )
       {
        case CFG_VERSION_CODE:
           
           if( doubleVal < 0.00 || doubleVal > 10.00 )
              {
 	       result = false;
 	      }
           break;

        case CFG_CPU_SCHED_CODE:
 
           strLen = getStringLength( stringVal ); 
           tempStr = (char *)malloc( strLen + 1 ); 
           setStrToLowerCase( tempStr, stringVal ); 
           
           if( compareString( tempStr, "fcfs-n" ) != STR_EQ 
               && compareString( tempStr,"sjf-n" ) != STR_EQ 
               && compareString( tempStr, "srtf-p" ) != STR_EQ 
               && compareString( tempStr, "fcfs-p" ) != STR_EQ 
               && compareString( tempStr, "rr-p" ) != STR_EQ ) 
              { 
               result = false;
              } 
           free( tempStr );
           
           break;
        
        case CFG_QUANT_CYCLES_CODE:
	
	   if( intVal < 0 || intVal > 100 )
              {
   	       result = false;
              }
           break;

        case CFG_MEM_AVAILABLE_CODE:

           if( intVal < 1024 || intVal > 102400 )
              {
               result = false;
              }
           break;

        case CFG_PROC_CYCLES_CODE:
  
           if( intVal < 1 || intVal > 100 )
              {
               result = false;
              }
           break;
        
        case CFG_IO_CYCLES_CODE:

           if( intVal < 1 || intVal > 1000 )
              {
               result = false;
              } 
           break;

        case CFG_LOG_TO_CODE:
           
           strLen = getStringLength( stringVal ); 
           tempStr = (char *)malloc( strLen + 1 ); 
           setStrToLowerCase( tempStr, stringVal ); 
          
           if( compareString( tempStr, "both" ) != STR_EQ 
               && compareString( tempStr, "monitor" ) != STR_EQ 
               && compareString( tempStr, "file" ) != STR_EQ )
             {
              result = false;
             }
 
           free( tempStr ); 
           break; 

       }
    return result;
   }

/*
name: showProgramFormat
process: displays command line argument requirements for this program
method input/parameters: none
method output/paramteres: none
method output/returned: none
device input/keyboard: none
device output/monitor: instructions provided as specified
dependencies: none
*/
void showProgramFormat()
   {
    printf( "Program Format:\n" );
    printf( "     sim_0x [-dc] [-dc] [-rs] <config file name>\n" );
    printf( "     -dc [optional] displays configuration data\n" );
    printf( "     -dm [optional] displays meta data\n" );
    printf( "     -rs [optional] runs simulator\n" );
    printf( "     config file name is required\n" );
   }
