#include <stdio.h>
#include <string.h>
#include "optparser.h"
#include "cache.h"

args_t args;

int main (int argc, char *argv[]){    
    if(!argparse(argc, argv)) return -1;

    cache_t *c = make_cache();
    FILE *fp = fopen(args.tracefile, "r");
    verbose_params_t v;
    v.operation_index = 0;

    transaction_t t;
    while(read_transaction(fp, &t)) {
        process(c, &t, &v);
    }

    fclose(fp);
    print_summary(c);
    release_cache(c);

    return 0;
}


