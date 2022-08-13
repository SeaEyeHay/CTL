#include <ostream>
#include <deque>
#include <string>
#include <iostream>

#include <bits/types/struct_timeval.h>
#include <sys/time.h>



using namespace std;


int main (int argc, char** argv) {
    const unsigned long
        seed = stoul(argv[1], NULL, 10),
        n = stoul(argv[2], NULL, 10);

    srand (seed);
    deque<int> vec;
    struct timeval startTime, endTime;


    gettimeofday (&startTime, NULL);
    
    vec.insert(vec.begin(), 0);
    for (unsigned long i = 1; i < n; i++) {
        vec.insert(vec.begin() + rand() % i , i);
    }

    gettimeofday (&endTime, NULL);


    struct timeval length = { 
        .tv_sec = endTime.tv_sec - startTime.tv_sec,
        .tv_usec = endTime.tv_usec - startTime.tv_usec
    };


    cout << length.tv_sec << ":" << length.tv_usec << endl;

    return 0;
}

