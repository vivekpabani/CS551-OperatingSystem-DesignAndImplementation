#define BLOCK_SIZE 32

typedef struct SH_FileHandle {
    char *fileName;
    int totalNumVars;
    int curVarPos;
    void *mgmtInfo;
}SH_FileHandle;

typedef char* SH_VarHandle;

int createVarFile (char *fileName);
int openVarFile (char *fileName, SH_FileHandle *fHandle);
int closeVarFile(SH_FileHandle *fHandle);

int readVar (int varNum, SH_FileHandle *fHandle, SH_VarHandle memVar);

int writeVar (int varNum, SH_FileHandle *fHandle, SH_VarHandle memVar);

