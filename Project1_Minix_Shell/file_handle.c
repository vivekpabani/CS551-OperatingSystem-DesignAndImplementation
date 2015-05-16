#include <stdio.h>
#include <stdlib.h>
#include "file_handle.h"


int createVarFile (char *fileName){

    FILE *pf=NULL;
    char* buff = (char *)calloc(BLOCK_SIZE,sizeof(char));

    //create the file.
    pf = fopen(fileName,"w+");
    
    int returnV = -1;
    
    if(fprintf(pf, "%s",buff)==0) {
        returnV = 0;
    } else{
        returnV = 1;
    }
    //release the space. and close the file.
    free(buff);
    fclose(pf);
    
    return returnV;
}

int openVarFile (char *fileName, SH_FileHandle *fHandle){
    //Open the file, and store onformation to the handle variable.
    FILE *pf=NULL;
    long len;
    long tailPointer = -1;
    pf=fopen(fileName, "r+");
    int returnV;
    if(pf==NULL)
        returnV = 0;
    else
    {
        fseek(pf,0L,SEEK_END);
        tailPointer = ftell(pf);
        if(tailPointer == -1)
            returnV = 0;
        else{
            len=tailPointer+1;
            fHandle ->fileName = fileName;
            fHandle ->totalNumVars =(int) (len/(BLOCK_SIZE))+1;
            fHandle ->curVarPos =0;
            fHandle ->mgmtInfo = pf;
            returnV = 0;
        }
    }
    return returnV;
}

int closeVarFile (SH_FileHandle *fHandle){
    if(fclose(fHandle->mgmtInfo)==EOF)          //if could not find the file ,return NOT FOUND.
        return 0;
    else
        return 1;
}

int readVar (int varNum, SH_FileHandle *fHandle, SH_VarHandle memVar){

    int seekSuccess;
    size_t readVarSize;
    FILE *readFile;

    if (varNum > fHandle->totalNumVars || varNum < 0){
        return 0;
    }

    readFile = fHandle->mgmtInfo ;

    seekSuccess = fseek(readFile, varNum*BLOCK_SIZE*sizeof(char), SEEK_SET);

    if (seekSuccess == 0){
        readVarSize = fread(memVar, sizeof(char), BLOCK_SIZE, readFile);
        fHandle->curVarPos = varNum;
        return 1;
    }
    else{
        return 0;
    }
}

int writeVar (int varNum, SH_FileHandle *fHandle, SH_VarHandle memVar){
    // write the variable to correct position.
    if(fHandle->totalNumVars<=varNum||varNum<0) {
        return 0;
    }
    if(fHandle->mgmtInfo==NULL){
        return 0;
    }
    if(fseek(fHandle->mgmtInfo,BLOCK_SIZE*varNum*sizeof(char),SEEK_SET)!=0){
        return 0;
    }

    if (!fprintf(fHandle->mgmtInfo, "%s",  memVar)) {
        return 0;
    }

    fHandle->curVarPos=varNum;
    return 1;
}


