/** $lic$
 * Copyright (C) 2016-2017 by The Board of Trustees of Cornell University
 * Copyright (C) 2013-2016 by The Board of Trustees of Stanford University
 *
 * This file is part of iBench.
 *
 * iBench is free software; you can redistribute it and/or modify it under the
 * terms of the Modified BSD-3 License as published by the Open Source Initiative.
 *
 * If you use this software in your research, we request that you reference
 * the iBench paper ("iBench: Quantifying Interference for Datacenter Applications",
 * Delimitrou and Kozyrakis, IISWC'13, September 2013) as the source of the benchmark
 * suite in any publications that use this software, and that
 * you send us a citation of your work.
 *
 * iBench is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the BSD-3 License for more details.
 *
 * You should have received a copy of the Modified BSD-3 License along with
 * this program. If not, see <https://opensource.org/licenses/BSD-3-Clause>.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <sched.h>

#ifndef L3_SIZE_KB
#   define L3_SIZE_KB       36608
#endif

#define KB (1024)
#define L3_SIZE_BYTES  L3_SIZE_KB * KB


#ifndef OFFSET
#   define OFFSET       0
#endif
#define NS_PER_S (1000000000L)


static double  a[L3_SIZE_BYTES + OFFSET],
b[L3_SIZE_BYTES + OFFSET],
c[L3_SIZE_BYTES + OFFSET];

//#ifdef _OPENMP
extern int omp_get_num_threads();
//#endif

unsigned long int getNs() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec*NS_PER_S + ts.tv_nsec;
}

int main (int argc, char **argv) {
    //#ifdef _OPENMP
    //#pragma omp parallel
    //	{
    //#pragma omp master
    //	{
    //		register int k = omp_get_num_threads();
    //	    	printf ("Number of Threads requested = %i\n",k);
    //	}
    //	}
    //#endif

    double scalar = 3.0;
    /*Usage: ./memBw <duration in sec>*/
    if (argc < 2) {
        printf("Usage: ./memory_BW <loop_num>\n");
        exit(0);
    }
    unsigned int loop_num = atoi(argv[1]);

    // init array
    for (int i=0; i< L3_SIZE_BYTES+OFFSET; i ++){
        a[i] = 1;
        b[i] = 2;
        c[i] = 0;
    }
    sched_yield();
    printf("L3 cache: %d, loop %ld times\n", L3_SIZE_BYTES+OFFSET, loop_num);
    unsigned int i = 0;
    uint64_t start_ns = getNs();
    while (i<loop_num) {
		#pragma omp parallel for
        for (int j=0; j< L3_SIZE_BYTES+OFFSET; j ++){
            c[j] = scalar * a[j] + b[j];
        }
        i++;
    }
    uint64_t end_ns = getNs();

    uint64_t dur = end_ns - start_ns;
    printf("cost %ld ns\n", (long)dur);
    return 0;
}
