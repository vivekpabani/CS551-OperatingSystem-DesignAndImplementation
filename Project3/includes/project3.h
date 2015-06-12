#include <lib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>


int inodewalker(int * r){
    message m;
    m.m1_i1=r;
    int x=_syscall(VFS_PROC_NR,108,&m);
    return x;
}


int zonemapwalker(int * r){
    message m;
    m.m1_i1=r;
    m.m1_i2=0;
    int x=_syscall(VFS_PROC_NR,109,&m);
    return x;
}


int inodefixer(int n){
    message m;
    m.m1_i1=NULL;
    int x=_syscall(VFS_PROC_NR,106,&m);
    return x;
}

int inodedamage(int * r, int inodeNb){
    message m;
    m.m1_i1=r;
    m.m1_i2=inodeNb;

    int x=_syscall(VFS_PROC_NR,110,&m);

    return x;
}



