#include <bits/types/struct_timeval.h>
#include <sys/time.h>

#include <stdlib.h>
#include <stdio.h>


#define CTL_TYPE int
#include "vector.h"


int main (int argc, char** argv) {
    const unsigned long
        seed = strtoul (argv[1], NULL, 10),
        n = strtoul (argv[2], NULL, 10);

    srand (seed);

    struct int_v vec = make_int_vec (0);
    struct timeval startTime, endTime;


    gettimeofday (&startTime, NULL);
    
    vec_add_int (&vec, 0, 0);
    for (unsigned long i = 1; i < n; i++) {
        vec_add_int (&vec, rand() % i, i);
    }

    gettimeofday (&endTime, NULL);


    struct timeval length = { 
        .tv_sec = endTime.tv_sec - startTime.tv_sec,
        .tv_usec = endTime.tv_usec - startTime.tv_usec
    };

    printf ("%lu:%lu\n", length.tv_sec, length.tv_usec);


    free_int_vec (&vec);

    return 0;
}

