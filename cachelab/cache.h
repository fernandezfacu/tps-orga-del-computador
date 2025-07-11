#ifndef _CACHE_HH_
#define _CACHE_HH_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "optparser.h"

typedef struct s_metrics {
    int total;
    int misses;
    int dirty_misses;
    int time;
} metrics_t;

typedef struct s_line {
    bool valid;
    bool dirty;
    int tag;
    int priority;
} line_t;

typedef struct s_cache {
	int size;
    int associativity;
    int sets;
    metrics_t loads;
    metrics_t stores;
    line_t **lines;
} cache_t;

typedef struct s_transaction {
    bool write; //True: escritura, False: lectura
    int address; //Direccion en decimal
    int bytes;
} transaction_t;

typedef struct s_verbose_params {
    int operation_index;
    char case_identifier[3];
    int line_tag;
    int valid_bit;
    int dirty_bit;
    int last_used;
} verbose_params_t;

cache_t *make_cache();

bool read_transaction(FILE *fp, transaction_t *t);

void process(cache_t *c, transaction_t *t, verbose_params_t *v);

void print_summary(cache_t *c);

void release_cache(cache_t *c);

#endif