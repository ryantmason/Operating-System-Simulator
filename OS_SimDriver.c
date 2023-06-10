// header files
#include "StringUtils.h"
#include "metadataops.h"
#include "configops.h"
#include "simulator.h"
#include "OS_SimDriver.h"

int main( int argc, char **argv )
   {
    // initialize variables
    bool programRunFlag = false;
    bool configDisplayFlag = false;
    bool configUploadFlag = false;
    bool mdDisplayFlag = false;
    bool runSimFlag = false;
    bool infoFlag = false;
    int argIndex = 1;
    int lastFourLetters = 4;
    int fileStrLen, fileStrSubLoc;
    char fileName[ STD_STR_LEN ];
    char errorMessage[ MAX_STR_LEN ];
    ConfigDataType *configDataPtr = NULL;
    OpCodeType *metaDataPtr = NULL;

    printf( "\nSimulator Program\n" );
    printf( "=================\n\n" );

    if( argc < 2 )
       {

        showProgramFormat();

        // unset program run flag
        programRunFlag = false;

        // set info flag
        infoFlag = true;
       }

    fileName[ 0 ] = NULL_CHAR;

    while( programRunFlag == false && argIndex < argc )
       {
        fileStrLen = getStringLength( argv[ argIndex ] );
        fileStrSubLoc = findSubString( argv[ argIndex ], ".cnf" );

        if( compareString( argv[ argIndex ], "-dc" ) == STR_EQ )
           {
            configUploadFlag = true;

            configDisplayFlag = true;
           }
        else if( compareString( argv[ argIndex ], "-dm" ) == STR_EQ )
           {
            configUploadFlag = true;

            mdDisplayFlag = true;
           }
        else if( compareString( argv[ argIndex ], "-rs" ) == STR_EQ )
           {
 	    configUploadFlag = true;

            runSimFlag = true;
           }
        else if( fileStrSubLoc != SUBSTRING_NOT_FOUND
                  && fileStrSubLoc == fileStrLen - lastFourLetters )
           {
            copyString( fileName, argv[ argIndex ] );

            programRunFlag = true;
           }

  	argIndex++;
       }

    // check for command line failure
    if( programRunFlag== false && infoFlag == false )
       {
        printf( "Incorrect argument line format, program aborted\n\n" );

	showProgramFormat();
       }

    // check for program run and config upload flag
    if( programRunFlag == true && configUploadFlag == true )
       {
        // upload config file, check for success
        if( getStringLength( fileName ) > 0
            && getConfigData( fileName, &configDataPtr, errorMessage ) == true )
           {
            // check config display flag
            if( configDisplayFlag == true )
               {
                displayConfigData( configDataPtr );
               }
           }
        else
           {
            printf( "\nConfig Upload Error: %s, program aborted\n\n",
                                                                 errorMessage );
            programRunFlag = false;
           }
       }
    if( programRunFlag == true
                          && ( mdDisplayFlag == true || runSimFlag == true ) )
       {
        if( getMetaData( configDataPtr->metaDataFileName,
                                            &metaDataPtr, errorMessage ) == true )
           {
            if( mdDisplayFlag == true )
               {
                displayMetaData( metaDataPtr );
               }

            if( runSimFlag == true )
               {
                runSim( configDataPtr, metaDataPtr );
               }
           }
        else
           {
            printf( "\nMetadata Upload Error: %s, program aborted\n",
                                                                 errorMessage );
           }
       }
    // clean up config data as needed
    configDataPtr = clearConfigData( configDataPtr );

    // clean up metadata as needed
    metaDataPtr = clearMetaDataList ( metaDataPtr );

    // show program end
    printf( "\nSimulator Program End.\n\n" );

    // return success
    return 0;
   }
