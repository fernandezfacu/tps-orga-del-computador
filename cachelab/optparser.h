#ifndef _OPTPARSER_HH_
#define _OPTPARSER_HH_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

typedef struct s_args {
	char *tracefile;
	int cache_size;
	int cache_associativity;
	int cache_sets;
	bool verbose;
	int verbose_n;
	int verbose_m;
} args_t;

bool argparse(int argc, char *argv[]);

#endif