#include <bits/types/struct_timeval.h>
#include <sys/time.h>

#include <stdlib.h>
#include <stdio.h>


#define CTL_TYPE int
#include "deque.h"


int main (int argc, char** argv) {
    const unsigned long
        seed = strtoul (argv[1], NULL, 10),
        n    = strtoul (argv[2], NULL, 10);

    srand (seed);

    struct int_d deq = make_int_deq (0);
    struct timeval startTime, endTime;


    gettimeofday (&startTime, NULL);
    
    deq_add_int (&deq, 0, 0);
    for (unsigned long i = 1; i < n; i++) {
        deq_add_int (&deq, rand() % i, i);
    }

    gettimeofday (&endTime, NULL);


    struct timeval length = { 
        .tv_sec = endTime.tv_sec - startTime.tv_sec,
        .tv_usec = endTime.tv_usec - startTime.tv_usec
    };

    printf ("%lu:%lu\n", length.tv_sec, length.tv_usec);


    free_int_deq (&deq);

    return 0;
}


