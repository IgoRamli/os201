/*
// Sun Jun 21 14:27:37 WIB 2020
// Tue Jun  9 17:46:47 WIB 2020
 */

// adjust DELAY and akunGitHub
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define  DELAY 12
#define  akunGitHub "skycruiser8"
#include "p00.h"

char*    progs[]={P01, P02, P03, P04, P05};
myshare* mymap;
int entry;

int init(void) {
    int ssize = sizeof(myshare);
    int fd    = open(SHAREMEM, MYFLAGS, CHMOD);
    fchmod   (fd, CHMOD);
    ftruncate(fd, ssize);
    mymap = mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);
    close(fd);
    sem_t *sem = sem_open("/pas2-k88", O_CREAT, SEM_PERMS, 1);
    mymap->mutex = *sem;
    mymap->entry = 0;
    mymap->state = OPEN;
    return BOSS;
}

char tmpStr[256]={};  // a temporary string.
void myprint(char* str1, char* str2) {
    printf("%s[%s]\n", str1, str2);
    sprintf(tmpStr, "");
}

int getEntry(void) {
    int entry;
    sem_wait(&mymap->mutex);
    mymap->mutexctr++;
    entry=mymap->entry++;
    mymap->progs[entry].stamp++;
    strcpy(mymap->progs[entry].akun, "");
    sem_post(&mymap->mutex);
    return entry;
}

void display(int entry) {
    sem_wait(&mymap->mutex);
    mymap->mutexctr++;
    mymap->progs[entry].stamp++;
    int jumlah_entri=mymap->entry;

    char contents[256] = {};
    for(int jj=0; jj<jumlah_entri; jj++) {
        char* akun=mymap->progs[jj].akun;
        sprintf(contents, "%s[%s]", contents, akun);
    }

    sprintf(tmpStr, "progs[%02d] TIME[%02d] MUTEX[%02d] MMAP[OPEN] %s", entry, mymap->mutexctr, mymap->progs[entry].stamp, contents);
    myprint(akunGitHub, tmpStr);
    sem_post(&mymap->mutex);
    //display an entry of MMAP. Eg.
    // akunGH2[progs[03] TIME[18] MUTEX[05] MMAP[OPEN] [akunGH1][akunGH3][akunGH0][akunGH2]]
}

void putInfo(char* akun, int entry) {
    sem_wait(&mymap->mutex);
    mymap->mutexctr++;
    mymap->progs[entry].stamp++;
    strcpy(mymap->progs[entry].akun, akun);
    sem_post(&mymap->mutex);
    // put "akunGitHub" into akun[] array (MMAP)
}

void checkOpen(void) {
    sem_wait(&mymap->mutex);
    mymap->mutexctr++;
    mymap->progs[entry].stamp++;
    int status=mymap->state;
    sem_post(&mymap->mutex);
    if(status==CLOSED) {
        printf("CLOSED, BYE BYE ==== ====\n");
        exit(0);
    }
}

int main(void) {
    sprintf(tmpStr, "START PID[%d] PPID[%d]", getpid(), getppid());
    myprint(akunGitHub, tmpStr);
    int boss=init();
    int entry=getEntry();
    checkOpen();
    for(int ii=0; ii<sizeof(progs); ii++) {
      if(fork()==0) { execlp(progs[ii], NULL); }
    }
    display(entry);
    sleep (DELAY);
    display(entry);
    putInfo(akunGitHub, entry);
    display(entry);
    display(entry);
    // blah... blah... blah...
    // blah... blah... blah...
    mymap->state=CLOSED;
    sem_close(&mymap->mutex);
    wait(NULL);
    myprint(akunGitHub, "BYEBYE =====  ===== =====");
}

