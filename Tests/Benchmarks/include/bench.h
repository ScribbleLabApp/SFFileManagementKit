//===-- include/bench.h - Benchmarking ----------------------------*- C -*-===//
//                                                                            //
// This source file is part of the Scribble Foundation open source project    //
//                                                                            //
// Copyright (c) 2024 ScribbleLabApp. and the ScribbleLab project authors     //
// Licensed under Apache License v2.0 with Runtime Library Exception          //
//                                                                            //
// Copyright (c) 2022 Olaf Berstein - All rights reserved                     //
//                                                                            //
// You may not use this file except in compliance with the License.           //
// You may obtain a copy of the License at                                    //
//                                                                            //
//      http://www.apache.org/licenses/LICENSE-2.0                            //
//                                                                            //
// Unless required by applicable law or agreed to in writing, software        //
// distributed under the License is distributed on an "AS IS" BASIS,          //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   //
// See the License for the specific language governing permissions and        //
// limitations under the License.                                             //
//                                                                            //
//===----------------------------------------------------------------------===//
///
/// \file
///
//===----------------------------------------------------------------------===//

#ifndef BENCH_H
#define BENCH_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include <math.h>

#if !defined(__pnacl__) && !defined(__EMSCRIPTEN__) && \
    (defined(__clang__) || defined(__GNUC__) || defined(__INTEL_COMPILER))

/* artificial use of all of memory */
#define BENCH_CLOBBER() asm volatile("":::"memory")

/* artificial dependency of x on all of memory and all of memory on x */
#define BENCH_VOLATILE(x) asm volatile("" : "+g"(x) : "g"(x) : "memory")
#define BENCH_VOLATILE_REG(x) asm volatile("" : "+r"(x) : "r"(x) : "memory")
#define BENCH_VOLATILE_MEM(x) asm volatile("" : "+m"(x) : "m"(x) : "memory")

#else

#if defined(_MSC_VER)
# pragma optimize("", off)
static inline void bench__use_ptr(void const volatile *x) {}
#define BENCH_CLOBBER() _ReadWriteBarrier()
# pragma optimize("", on)

#else

static void bench_use_ptr(char const volatile *x) {}
#define BENCH_CLOBBER()

#endif /* _MSC_VERS */

/* artificial use of all of memory dependent on x */
# define BENCH_CLOBBER_WITH(x) (bench__use_ptr(&(x)), BENCH_CLOBBER())
# define BENCH_CLOBBER_WITH_REG(x) (bench__use_ptr(&(x)), BENCH_CLOBBER())
# define BENCH_CLOBBER_WITH_MEM(x) (bench__use_ptr(&(x)), BENCH_CLOBBER())

#endif /* defined(__pancl__) ... */

typedef struct {
    size_t count;
    double min, mean, M2;
    char const *title;
} BenchRecord;

typedef struct {
    size_t count, cap;
    BenchRecord *records;
    /* temporaries */
    size_t i;
    double ns;
} Bench;

static Bench benchInternal;

#define BENCH(title, warmup, samples)                                  \
        for (bench_append(title),                                      \
             benchInternal.i = (warmup) + (samples);                   \
             (benchInternal.ns = bench_gettime()), benchInternal.i--;  \
             benchInternal.i < (samples) ?                             \
             bench_update(bench_gettime()-benchInternal.ns),0 : 0)

static inline double
bench_gettime(void) {
#if defined(CLOCK_PROCESS_CPUTIME_ID)
    struct timespec t;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
    return t.tv_nsec * 1.0/1000000000 + t.tv_sec;
#elif defined(CLOCK_MONOTONIC_RAW)
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC_RAW, &t);
    return t.tv_nsec * 1.0/1000000000 + t.tv_sec;
#else
    return clock() * 1.0 / CLOCKS_PER_SEC;
#endif
}

static inline void
bench_append(char const *title) {
    Bench *b = &benchInternal;
    BenchRecord *r;
    if (b->count >= b->cap) {
        b->cap = (b->cap << 1) + 1;
        b->records = (BenchRecord *)
                      realloc(b->records, b->cap * sizeof *b->records);
    }
    r = &b->records[b->count++];
    r->mean = r->M2 = r->count = 0;
    r->min = DBL_MAX;
    r->title = title;
}

static inline void
bench_update(double time) {
    BenchRecord *r = &benchInternal.records[benchInternal.count-1];
    double const delta = time - r->mean;
    r->mean += delta / ++r->count;
    r->M2 += delta * (time - r->mean);
    if (time < r->min)
        r->min = time;
}

static inline int
bench_record_cmp(void const *lhs, void const *rhs) {
    BenchRecord const *l = (BenchRecord const *)lhs;
    BenchRecord const *r = (BenchRecord const *)rhs;
    return l->mean > r->mean ? 1 : -1;
}

static inline void
bench_done(void) {
    size_t i, j, maxlen;
    double minmean = DBL_MAX;
    Bench *b = &benchInternal;
    qsort(b->records, b->count, sizeof *b->records, bench_record_cmp);

    for (maxlen = i = 0; i < b->count; ++i) {
        size_t l = strlen(b->records[i].title);
        if (l > maxlen)
            maxlen = l;
        if (b->records[i].mean < minmean)
            minmean = b->records[i].mean;
    }

    for (i = 0; i < b->count; ++i) {
        int l = printf("%s:", b->records[i].title) - 4;

        for (j = 0; j < maxlen-l; ++j)
            putchar(' ');

        printf("mean: %.9e,   stddev: %.2e,   min: %.9e \n",
               b->records[i].mean / minmean,
               sqrt(b->records[i].M2 / b->records[i].count) / minmean,
               b->records[i].min);
    }
    b->count = 0;
}

static inline void
bench_free(void) {
    free(benchInternal.records);
}


static inline uint64_t
bench_hash64(uint64_t x) {
    x ^= x >> 30;
    x *= 0xBF58476D1CE4E5B9u;
    x ^= x >> 27;
    x *= 0x94D049BB133111EBu;
    x ^= x >> 31;
    return x;
}

#endif /* BENCH_H */
