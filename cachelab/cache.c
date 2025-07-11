#include "cache.h"

#define ADDRESS_BITS 32
#define SUMMARY "%i-way, %i sets, size = %iKB\nloads %i stores %i total %i\nrmiss %i wmiss %i total %i\ndirty rmiss %i dirty wmiss %i\nbytes read %i bytes written %i\nread time %i write time %i\nmiss rate %f\n"
#define DIRECT_MAPPED_SUMMARY "%s, %i sets, size = %iKB\nloads %i stores %i total %i\nrmiss %i wmiss %i total %i\ndirty rmiss %i dirty wmiss %i\nbytes read %i bytes written %i\nread time %i write time %i\nmiss rate %f\n"
#define TRACE_MAX_SIZE 50
#define PENALTY 100
#define DIRECT_MAPPED 1
#define VERBOSE_ASSOCIATIVE "%i %s %x %x %i %x %i %i %i\n"
#define VERBOSE "%i %s %x %x %i %x %i %i\n"
#define VERBOSE_DECIMAL_TAG_ASSOCIATIVE "%i %s %x %x %i %i %i %i %i\n"
#define VERBOSE_DECIMAL_TAG "%i %s %x %x %i %i %i %i\n"
#define HIT "1"
#define CLEAN_MISS "2a"
#define DIRTY_MISS "2b"
#define MAX_OPERATION 100000

extern args_t args;

cache_t *make_cache(){
    cache_t *new_cache = calloc(1, sizeof(cache_t));

    new_cache->size = args.cache_size;
    new_cache->associativity = args.cache_associativity;
    new_cache->sets = args.cache_sets;

    line_t **new_lines = malloc(sizeof(line_t*) * new_cache->sets);
    for(int i = 0; i < new_cache->sets; i++) {
        new_lines[i] = malloc(sizeof(line_t) * new_cache->associativity);
        for(int j = 0; j < new_cache->associativity; j++) {
            new_lines[i][j].valid = false;
            new_lines[i][j].dirty = false;
            new_lines[i][j].tag = 0;
            new_lines[i][j].priority = 0;
        }
    }
    new_cache->lines = new_lines;
    return new_cache;
}

bool read_transaction(FILE *fp, transaction_t *t){
    char trace[TRACE_MAX_SIZE];
    if(!fgets(trace, TRACE_MAX_SIZE, fp))
        return false;

    int i = 0;
    char *trace_tokens[8];
    char *pch;
    pch = strtok(trace, " x:");
    while(pch != NULL) {
        trace_tokens[i] = pch;
        pch = strtok(NULL, " x:");
        i++;
    }

    t->write = (strcmp(trace_tokens[2], "W") == 0);
    t->address = strtol(trace_tokens[4], NULL, 16);
    t->bytes = atoi(trace_tokens[5]);

    return true;
}

int search_least_recently_used(cache_t *c, int set) {
    int line_number = 0;
    int lowest_priority = MAX_OPERATION;
    for(int i = 0; i < c->associativity; i++) {
        if(c->lines[set][i].priority < lowest_priority) {
            lowest_priority = c->lines[set][i].priority;
            line_number = i;
        }
    }
    return line_number;
}

int get_adress_tag(cache_t *c, transaction_t *t){
    int s = log2(c->sets);
    int B = c->size / (c->sets * c->associativity);
    int b = log2(B);
    int tag = 32 - s - b; 

    int tag_mask = 0xFFFFFFFF << (32 - tag);

    int aux_mask = (0xFFFFFFFF << tag) ^ 0xFFFFFFFF;
    return (((t->address & tag_mask) >> (b + s)) & aux_mask);
}

int get_adress_set(cache_t *c, transaction_t *t){
    int s = log2(c->sets);
    int B = c->size / (c->sets * c->associativity);
    int b = log2(B);
    int tag = ADDRESS_BITS - s - b; 

    int tag_mask = 0xFFFFFFFF << (ADDRESS_BITS - tag);
    int set_mask = (0xFFFFFFFF << b) ^ tag_mask;

    return ((t->address & set_mask) >> b);
}

void cache_hit(cache_t *c, verbose_params_t *v, transaction_t *t, int address_set, int address_tag, int cache_line) {
    if(c->lines[address_set][cache_line].dirty)
        v->dirty_bit = 1;
    else
        v->dirty_bit = 0;
    if(t->write) {
        c->lines[address_set][cache_line].dirty = true;
        c->stores.time++;
    } else {
        c->loads.time++;
    }
    strcpy(v->case_identifier, HIT);
    v->line_tag = address_tag;
    v->valid_bit = 1;
}
void cache_miss_free_line(cache_t *c, verbose_params_t *v, transaction_t *t, int address_set, int address_tag, int cache_line) {
    c->lines[address_set][cache_line].valid = true;
    c->lines[address_set][cache_line].tag = address_tag;
    if(t->write) {
        c->stores.misses++;
        c->stores.time += 1 + PENALTY;
        c->lines[address_set][cache_line].dirty = true;
    }
    else {
        c->loads.misses++;
        c->loads.time += 1 + PENALTY;
    }
    strcpy(v->case_identifier, CLEAN_MISS);
    v->line_tag = -1;
    v->valid_bit = 0;
    v->dirty_bit = 0;
}

void proccess_replace_miss(metrics_t *m, verbose_params_t *v, bool dirty) {
    m->misses++;
    if(dirty) {
        m->dirty_misses++;
        m->time += 1 + 2*PENALTY;
        strcpy(v->case_identifier, DIRTY_MISS);
        v->dirty_bit = 1;
    } else {
        m->time += 1 + PENALTY;
        strcpy(v->case_identifier, CLEAN_MISS);
        v->dirty_bit = 0;
    }
}

void cache_miss_replace_lru(cache_t *c, verbose_params_t *v, transaction_t *t, int address_set, int address_tag, int cache_line) {
    v->line_tag = c->lines[address_set][cache_line].tag;
    c->lines[address_set][cache_line].tag = address_tag;
    if(t->write) {
        proccess_replace_miss(&(c->stores), v, c->lines[address_set][cache_line].dirty);
        c->lines[address_set][cache_line].dirty = true;
    }
    else {
        proccess_replace_miss(&(c->loads), v, c->lines[address_set][cache_line].dirty);
        c->lines[address_set][cache_line].dirty = false;
    }
    v->valid_bit = 1;
}

void print_verbose(cache_t *c, verbose_params_t *v, int address_set, int address_tag, int cache_line) {
    if ((v->operation_index >= args.verbose_n) && (v->operation_index <= args.verbose_m)) {
        if (c->associativity > 1) {
            if (v->line_tag == -1)
                printf(VERBOSE_DECIMAL_TAG_ASSOCIATIVE, v->operation_index, v->case_identifier, address_set, address_tag, cache_line, v->line_tag, v->valid_bit, v->dirty_bit, v->last_used);
            else
                printf(VERBOSE_ASSOCIATIVE, v->operation_index, v->case_identifier, address_set, address_tag, cache_line, v->line_tag, v->valid_bit, v->dirty_bit, v->last_used);
        } 
        else {
            if (v->line_tag == -1)
                printf(VERBOSE_DECIMAL_TAG, v->operation_index, v->case_identifier, address_set, address_tag, cache_line, v->line_tag, v->valid_bit, v->dirty_bit);
            else
                printf(VERBOSE, v->operation_index, v->case_identifier, address_set, address_tag, cache_line, v->line_tag, v->valid_bit, v->dirty_bit);
        }
    }
}

void process(cache_t *c, transaction_t *t,  verbose_params_t *v){
    if(t->write)
        c->stores.total++;
    else
        c->loads.total++;

    int address_tag = get_adress_tag(c, t);
    int address_set = get_adress_set(c, t);

    int cache_line = 0;
    bool cached = false;
    while(!cached) {
        if(c->lines[address_set][cache_line].valid) {
            if(c->lines[address_set][cache_line].tag == address_tag) {
                cache_hit(c, v, t, address_set, address_tag, cache_line);  
                cached = true;   
            }    
        }
        else {
            cache_miss_free_line(c, v, t, address_set, address_tag, cache_line);
            cached = true;
        }
        if((cache_line == (c->associativity - 1)) && !cached) {
            cache_line = search_least_recently_used(c, address_set);
            cache_miss_replace_lru(c, v, t, address_set, address_tag, cache_line);
            cached = true;
        }
        cache_line++;
    }
    
    v->last_used = c->lines[address_set][cache_line-1].priority;
    c->lines[address_set][cache_line-1].priority = v->operation_index;

    print_verbose(c, v, address_set, address_tag, cache_line-1);
    v->operation_index++;
}


void print_summary(cache_t *c){
    
    int block_size = c->size / (c->sets * c->associativity);
    int operations = c->loads.total + c->stores.total;
    int misses = c->loads.misses + c->stores.misses;
    int dirty_misses = c->loads.dirty_misses + c->stores.dirty_misses;
    float missrate = (float) misses/operations;
    
    if(c->associativity == DIRECT_MAPPED)
        printf(DIRECT_MAPPED_SUMMARY, "direct-mapped", c->sets, (c->size/1024),
                    c->loads.total, c->stores.total, operations,
                    c->loads.misses, c->stores.misses, misses,
                    c->loads.dirty_misses, c->stores.dirty_misses,
                    misses*block_size, dirty_misses*block_size,
                    c->loads.time, c->stores.time,
                    missrate); 
    else 
        printf(SUMMARY, c->associativity, c->sets, (c->size/1024),
                    c->loads.total, c->stores.total, operations,
                    c->loads.misses, c->stores.misses, misses,
                    c->loads.dirty_misses, c->stores.dirty_misses,
                    misses*block_size, dirty_misses*block_size,
                    c->loads.time, c->stores.time,
                    missrate);
}

void release_cache(cache_t *c){
    for(int i = 0; i < c->sets; i++)
        free(c->lines[i]);
    free(c->lines);
    free(c);
}

