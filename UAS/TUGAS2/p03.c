/*
// Sun Jun 21 14:27:37 WIB 2020
// Tue Jun  9 17:46:47 WIB 2020
 */

// adjust DELAY and akunGitHub
#define  DELAY 6
#define  akunGitHub "IgoRamli"
#include "p00.h"

char*    progs[]={P01, P02, P03, P04, P05};
myshare* mymap;

int init(void) {
    //sleep (DELAY);

    int ssize = sizeof(mymap);
    int fd = open(SHAREMEM, O_RDWR, CHMOD);

    if (fd < 0) {
        // p00 belum dijalankan
        printf ("No \"%s\" file.\n", SHAREMEM);
        exit (0);
    }

    mymap = mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);

    return NOTBOSS;
}

char tmpStr[256]={};  // a temporary string.
void myprint(char* str1, char* str2) {
    printf("%s[%s]\n", str1, str2);
    // blah blah blah
    // blah blah blah
    // blah blah blah
}

int getEntry(void) {
    int entry;
    // get an entry number
    sem_wait (&mymap->mutex);

    mymap->mutexctr++;
    entry = mymap->entry++;
    mymap->progs[entry].stamp++;
    strcpy (mymap->progs[entry].akun, "");

    sem_post (&mymap->mutex);
    return entry;
}

void display(int entry) {
    sem_wait(&mymap->mutex);

    mymap->mutexctr++;
    mymap->progs[entry].stamp++;

    int entry_cnt = mymap->entry;

    char content[256] = {};
    for (int i = 0; i < entry_cnt; i++) {
        char* akun = mymap->progs[i].akun;
        sprintf (content, "%s[%s]", content, akun);
    }

    // display an entry of MMAP.
    // eg. akunGH2[progs[03] TIME[18] MUTEX[05] MMAP[OPEN] [akunGH1][akunGH3][akunGH0][akunGH2]]
    sprintf (tmpStr, "progs[%02d] TIME[%02d] MUTEX[%02d] MMAP[OPEN] %s", entry, mymap->mutexctr, mymap->progs[entry].stamp, content);
    myprint (akunGitHub, tmpStr);

    sem_post(&mymap->mutex);
}

void putInfo(char* akun, int entry) {
    // put "akunGitHub" into akun[] array (MMAP)
    sem_wait(&mymap->mutex);

    mymap->mutexctr++;
    mymap->progs[entry].stamp++;
    strcpy(mymap->progs[entry].akun, akun);

    sem_post(&mymap->mutex);
}

void checkOpen(void) {
    // exit if MMAP is closed.
    if (mymap->state == CLOSED) {
        printf ("CLOSED, BYE BYE ==== ====\n");
        exit (0);
    }
}

int main(void) {
    sprintf(tmpStr, "START PID[%d] PPID[%d]", getpid(), getppid());
    myprint(akunGitHub, tmpStr);
    int boss=init();
    checkOpen();

    int entry = getEntry();

    sleep (DELAY);
    display (entry);
    putInfo (akunGitHub, entry);
    display (entry);
    display (entry);

    myprint(akunGitHub, "BYEBYE =====  ===== =====");
}
