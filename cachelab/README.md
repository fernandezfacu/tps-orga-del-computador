# Simulador de Caché (CacheSim)

El objetivo de este trabajo práctico es desarrollar un simulador de caché en lenguaje C capaz de analizar el comportamiento de distintos algoritmos de reemplazo y configuraciones de memoria caché frente a trazas de acceso simuladas. El simulador permite estudiar el rendimiento de la caché y sus estadísticas en distintos escenarios de tamaño, asociatividad y cantidad de conjuntos.

## Funcionalidades 

### Parámetros de configuración:

- Tamaño total de la caché (C)
- Grado de asociatividad (E)
- Cantidad de conjuntos (S)
- Modo verboso opcional (-v n m) para imprimir operaciones específicas

### Procesamiento de transacciones:

- Lectura de archivos de trazas
- Soporte para operaciones de lectura y escritura en memoria
- Cálculo de índices y etiquetas a partir de la dirección
- Implementación de reemplazo por política Least Recently Used (LRU)
- Manejadores para hit, clean miss y dirty miss

### Estadísticas que genera el simulador:

- Cantidad de cargas y escrituras
- Total de misses y dirty misses
- Tiempo total de lectura y escritura
- Tasa de fallos de caché (miss rate)
- Cantidad de bytes leídos/escritos
- Modo verboso configurable para imprimir información detallada de hits y misses

## Compilación y Ejecución

Compilar:
```bash
make
```

Ejecutar:
```bash
./cachesim <archivo_traza> <C> <E> <S> [-v <n> <m>]
```

Ejecutar tests:
```bash
./runtester.sh
```

El repositorio incluye una suite de pruebas automatizadas en pytest proporcionada por la cátedra.
