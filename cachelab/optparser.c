#include "optparser.h"
#define F_OK 0
extern args_t args;

bool isPowerOfTwo(int n) {
    if(n == 0)
        return false;
    return (ceil(log2(n)) == floor(log2(n)));
}

bool fileExist(char *filename) {
    return(access(filename, F_OK) == 0);
}

bool argparse(int argc, char *argv[]) {
    if((argc != 5) && (argc != 8)) {
        fprintf(stderr, "El número de argumentos no es 4 o 7\n");
        return false;
    }

    int opt;
    while((opt = getopt(argc, argv, "v")) != -1) 
        args.verbose = (opt == 'v');
    
    // Archivo de traza
    if(!fileExist(argv[optind])) {
        fprintf(stderr, "El archivo de trazas especificado no existe\n");
        return false;
    } else {
        args.tracefile = argv[optind++];
    }

    // Tamaño de la caché
    int aux = atoi(argv[optind]);
    if(isPowerOfTwo(aux))
        args.cache_size = aux;
    else {
        fprintf(stderr, "C no es una potencia de 2\n");
        return false;
    }

    // Asociatividad de la caché
    aux = atoi(argv[++optind]);
    if(isPowerOfTwo(aux))
        args.cache_associativity = aux;
    else {
        fprintf(stderr, "E no es una potencia de 2\n");
        return false;
    }

    // Número de sets de la caché
    aux = atoi(argv[++optind]);
    if(isPowerOfTwo(aux))
        args.cache_sets = aux;
    else {
        fprintf(stderr, "S no es una potencia de 2\n");
        return false;
    }

    if(!(args.cache_size > args.cache_associativity * args.cache_sets)) {
        fprintf(stderr, "La combinación de parámetros de C, E y S es inválida\n");
        return false;
    }

    // Verboso: n y m
    if(args.verbose) {
        args.verbose_n = atoi(argv[++optind]);
        args.verbose_m = atoi(argv[++optind]);
        if(!((args.verbose_n >= 0) && (args.verbose_n <= args.verbose_m))) {
            fprintf(stderr, "Los argumentos n y m del modo verboso no son números enteros que cumplan 0 ≤ n ≤ m\n");
            return false;   
        }
    } else {
        args.verbose_n = -1;
        args.verbose_m = -1;
    }

    return true;
}