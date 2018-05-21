#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <getopt.h>

int main(int argc, const char *argv[]) {
    int uFlag = 0;
    int NVal = 0;
    int MVal = 1;
    int pFlag = 0;
    int sVal = 1;
    int sFlag = 0;
    int c;
    int index;
    opterr = 0;
    
    /* ----------------------------------------------------ARGUMENT PARSING------------------------------------------------------------ */
    
    while ((c = getopt (argc, argv, "uN:M:ps:")) != -1)
        switch (c) {
            case 'u':
                uFlag = 1;
                printf ("prog2tree [-u] [-N <num-levels>] [-M <num-children>] [-p] [-s <sleep-time]\n");
                exit(0);
            case 'N':
                NVal = atoi(optarg);
                if (NVal > 4) {fprintf(stderr, "The maximum value for <num-levels> is 4\n"); exit(1);}
                break;
            case 'M':
                MVal = atoi(optarg);
                if (MVal > 3) { fprintf (stderr, "The maximum value for <num-children> is 3\n"); exit(1); }
                break;
            case 'p':
                if (sFlag == 1) {fprintf (stderr, "You cannot specify both -p and -s\nprog2tree [-u] [-N <num-levels>] [-M <num-children>] [-p] [-s <sleep-time]\n"); exit(1); }
                pFlag = 1;
                break;
            case 's':
                if (pFlag == 1) {fprintf (stderr, "You cannot specify both -p and -s\nprog2tree [-u] [-N <num-levels>] [-M <num-children>] [-p] [-s <sleep-time]\n"); exit(1); }
                sVal = atoi(optarg);
                sFlag = 1;
                break;
            case '?':
                if (optopt == 'N' || optopt == 'M'|| optopt == 's')
                    fprintf (stderr, "Option -%c requires an argument.\nprog2tree [-u] [-N <num-levels>] [-M <num-children>] [-p] [-s <sleep-time]\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\nprog2tree [-u] [-N <num-levels>] [-M <num-children>] [-p] [-s <sleep-time]\n", optopt);
                else
                    fprintf (stderr, "Unknown option character `\\x%x'.\nprog2tree [-u] [-N <num-levels>] [-M <num-children>] [-p] [-s <sleep-time]\n", optopt);
                return 1;
            default:
                abort ();
        }
    
    for (index = optind; index < argc; index++)
        printf ("Non-option argument %s\n", argv[index]);
    
    /* ----------------------------------------------------LINUX TREE-------------------------------------------------------------- */
    if (NVal == 0)
        printf ("ALIVE: Level %d process with pid = %d, child of ppid = %d.\n", NVal,(int) getpid(),(int) getppid());
    else
        printf ("ALIVE: Level %d process with pid = %d, child of ppid = %d.\n", NVal - 1,(int) getpid(),(int) getppid());
    if (NVal != 1 && NVal != 0) { /* not leaf node */
        for (int i = 0; i < MVal; i++) {
            char newNVal[15];
            sprintf(newNVal, "%d",NVal - 1);
            char newMVal[15];
            sprintf(newMVal, "%d",MVal);
            char newSVal[15];
            sprintf(newSVal, "%d",sVal);
            pid_t pid; /* for a child process */
            pid = fork();
            if (pid < 0) { /* error */
                fprintf(stderr, "Fork failed.");
                return 1;
            }
            if (pid == 00) { /* child process */
                if (pFlag == 1) execlp("./prog2tree", "./prog2tree", "-N", newNVal, "-M", newMVal, "-p",NULL);
                else execlp("./prog2tree", "./prog2tree", "-N", newNVal, "-M", newMVal, "-s", newSVal,NULL);
            }
        }
        for (int i = 0; i < MVal; i++) {
            wait(NULL);
        }
    }
    else if (NVal == 1 || NVal == 0){ /* leaf node */
        if (pFlag == 1)
            pause();
        else
            sleep(sVal);
    }
    if (NVal == 0)
        printf ("EXITING: Level %d process with pid = %d, child of ppid = %d.\n",NVal, (int) getpid(),(int) getppid());
    else
        printf ("EXITING: Level %d process with pid = %d, child of ppid = %d.\n",NVal - 1, (int) getpid(),(int) getppid());
    return 0;
}
