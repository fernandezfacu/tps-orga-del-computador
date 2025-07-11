extern malloc
extern free
extern fprintf
extern intClone
extern intDelete
extern intPrint
extern intCmp
extern listNew
extern listDelete
extern listClone
extern listAddFirst
extern listPrint

extern getCloneFunction
extern getDeleteFunction
extern getPrintFunction

%define SIZE_CHAR 1
global strClone
global strPrint
global strCmp
global strLen
global strDelete

%define SIZE_POINTER 8
%define SIZE_OF_ARRAY 16
; %define SIZE_OF_ARRAY_DATA 24
%define OFFSET_TYPE 0 
%define OFFSET_SIZE 4 
%define OFFSET_CAPACITY 5 
%define OFFSET_ARRAY_DATA 8
global arrayNew
global arrayDelete
global arrayPrint
global arrayGetSize
global arrayAddLast
global arrayGet
global arrayRemove
global arraySwap

%define SIZE_OF_CARD 24
%define OFFSET_SUIT 0
%define OFFSET_NUMBER 8
%define OFFSET_STACKED 16
%define TYPE_CARD 3
global cardCmp
global cardClone
global cardAddStacked
global cardDelete
global cardGetSuit
global cardGetNumber
global cardGetStacked
global cardPrint
global cardNew

section .data
formato_escritura_int: db '%i', 0
formato_escritura_string: db '%s', 0
formato_escritura_apertura_carta: db '{', 0
formato_escritura_cierre_carta: db '}', 0
formato_escritura_apertura_array: db '[', 0
formato_escritura_cierre_array: db ']', 0
formato_escritura_variable_carta: db '-', 0
formato_escritura_variable_array: db ',', 0

null_str: db 'NULL', 0 ; define el string NULL en memoria

section .text

; ** String **

; char* strClone(char* a)
strClone:
    ; armo stack frame
    push rbp
    mov rbp, rsp

    ; pusheo no volátiles que voy a usar
    push r12
    push r13

    mov r12, rdi ; guardo puntero a a

    call strLen ; rax tiene el tamaño del string
    inc rax ; sumo uno por carcter nulo ('\0')
    mov r13, rax ; guardo el valor strlen+1

    ; guardo en rdi la memoria a reservar ((strlen+1) * size_char) para el malloc
    imul rax, SIZE_CHAR 
    mov rdi, rax

    call malloc ; rax tiene el puntero a string que quiero devolver

    xor rcx, rcx ; inicializo rcx en 0

    .ciclo: 
        cmp rcx, r13
        je .fin

        mov r8b, [r12 + rcx] ; guardo letra de a en posición rcx
        mov byte [rax + rcx], r8b ; la guardo en la memoria reservada
        inc rcx
        jmp .ciclo

    .fin:
        pop r13
        pop r12
        pop rbp
        ret

; void strPrint(char* a, FILE* pFile)
strPrint:
    ; armo stack frame
    push rbp
    mov rbp, rsp

    mov r8, rdi; guardamos el puntero al string 
    mov r9, rsi; guardamos el puntero al archivo

    .print:
        cmp byte [r8], 0 ; chequeo si es string vacío
        je .print_null

        mov rdi, r9 ; pFile en rdi
        mov rsi, formato_escritura_string ; "%s" en rsi
        mov rdx, r8 ; string en rdx
        call fprintf ; llamo a fprintf

        jmp .fin

    .print_null:
        mov rdi, r9 ; pFile en rdi
        mov rsi, formato_escritura_string ; "%s" en rsi
        mov rdx, null_str ; "NULL" en rdx
        call fprintf ; llamo a fprintf

        jmp .fin
        
    .fin:
        pop rbp
        ret

; int32_t strCmp(char* a, char* b)
strCmp:
    push rbp
    mov rbp, rsp

    xor rcx, rcx

    .ciclo: 
        
        mov r8b, [rcx + rdi]
        mov r9b, [rcx + rsi]

        inc rcx
        cmp r8b, 0
        je .menor_o_igual

        cmp r9b, 0
        je .mayor

        cmp r8b, r9b
        jg .mayor
        jl .menor 
        je .ciclo

    .menor:
        mov rax, 1
        jmp .fin

    .mayor: 
        mov rax, -1
        jmp .fin

    .menor_o_igual:
        cmp r9b, 0 
        jne .menor

        mov rax, 0
        jmp .fin

    .fin:
        pop rbp
        ret

; void strDelete(char* a)
strDelete:
    push rbp
    mov rbp, rsp

    call free

    pop rbp
    ret

; uint32_t strLen(char* a)
strLen:

    push rbp
    mov rbp, rsp
    xor rax, rax

    .ciclo: 
        
        cmp byte [rax + rdi], 0
        jne .inc

        pop rbp
        ret

    .inc:
        inc rax
        jmp .ciclo
; ** Array **

; uint8_t arrayGetSize(array_t* a)
arrayGetSize:
    ; armo stack frame
    push rbp
    mov  rbp, rsp

    ;rdi tiene *a(array)
    mov al, byte [rdi + OFFSET_SIZE]

    pop  rbp
    ret

; void arrayAddLast(array_t* a, void* data)
arrayAddLast:
    ; armo stack frame
    push rbp
    mov  rbp, rsp 

    push r12
    push r13

    mov r12, rdi; guardamos puntero a array en r12
    mov r13, rsi; guardamos puntero a data en r13

    mov  cl, [r12 + OFFSET_CAPACITY] 
    cmp  byte [r12 + OFFSET_SIZE], cl
    jge  .fin ; si a->size >= a->capacity, no hace nada

    mov edi, [r12 + OFFSET_TYPE]
    call getCloneFunction ; en rax el puntero a la función de clonación

    mov rdi, r13
    call rax ; en rax la data clonada

    mov  r8, [r12 + OFFSET_ARRAY_DATA] ; guardo en r8 puntero a primera posicion de data
    xor  r9, r9 ; inicializo r9 en 0
    mov  r9b, byte [r12 + OFFSET_SIZE] ; guardo en r9 el size de array

    mov  [r8 + r9*SIZE_POINTER], rax ; guardo en posición size del array el clon de data (rax)

    inc byte [r12 + OFFSET_SIZE]; incremento en uno el valor de size

    .fin:
        pop r13
        pop r12
        pop  rbp
        ret

; void* arrayGet(array_t* a, uint8_t i)
arrayGet:
    ; armo stack frame
    push rbp
    mov  rbp, rsp

    mov  rax, 0 ; inicializo rax en 0

    cmp  sil, [rdi + OFFSET_SIZE]
    jge  .fin ; si i >= size, devuelvo 0

    mov  r8, [rdi + OFFSET_ARRAY_DATA] ; guardo en r8 puntero a primera posicion de data
    mov  rax, [r8 + rsi*SIZE_POINTER] ; guardo en rax valor de posicion i de data
    
    .fin: 
        pop  rbp
        ret

; array_t* arrayNew(type_t t, uint8_t capacity)
arrayNew:
    ; armo stack frame
    push rbp
    mov  rbp, rsp
    ; pusheo no volátiles que voy a usar
    push r12 
    push r13
    push rbx
    sub  rsp, 8 ;alineo la pila a 16 bytes

    mov  r12d, edi   ; guardo type en r12
    mov  r13b, sil   ; guardo capacity en r13

    mov  rdi, SIZE_OF_ARRAY
    call malloc      ; rax tiene puntero a struct
    mov  rbx, rax    ; rbx mantiene el puntero a struct

    mov  [rbx + OFFSET_TYPE], r12d ; copio type en el struct  
    mov  r8b, 0
    mov  [rbx + OFFSET_SIZE], r8b ; inicializo size en 0
    mov  [rbx + OFFSET_CAPACITY], r13b  ; copio capacity en el struct  
    
    mov  r8, r13
    imul r8, SIZE_POINTER
    mov  rdi, r8 
    call malloc      ; rax tiene puntero a data
    mov  [rbx + OFFSET_ARRAY_DATA], rax

    mov  rax, rbx

    add  rsp, 8
    pop  rbx
    pop  r13
    pop  r12
    pop  rbp
    ret

; void* arrayRemove(array_t* a, uint8_t i)
arrayRemove:
    ; armo stack frame
    push rbp
    mov  rbp, rsp

    xor  rax, rax ; inicializo rax en 0
    cmp  sil, [rdi + OFFSET_SIZE]
    jge  .fin ; si i >= size, devuelvo 0

    mov  r8, [rdi + OFFSET_ARRAY_DATA] ; guardo en r8 puntero a primera posicion de data
    mov  rax, [r8 + rsi*SIZE_POINTER] ; guardo en rax valor de posicion i de data
    xor  r10, r10
    mov  r10b, [rdi + OFFSET_SIZE]
    dec  r10 ; guardo en r10 el valor de size-1

    mov rcx, rsi

    .ciclo:
        inc rcx
        mov r9, [r8 + rcx*SIZE_POINTER]
        dec rcx
        mov [r8 + rcx*SIZE_POINTER], r9

        inc rcx
        cmp rcx, r10
        jl .ciclo

        dec byte [rdi + OFFSET_SIZE]

    .fin:
        pop rbp
        ret

; void arraySwap(array_t* a, uint8_t i, uint8_t j)
arraySwap:
    ; armo stack frame
    push rbp
    mov  rbp, rsp

    cmp  sil, [rdi + OFFSET_SIZE]
    jge  .fin ; si i >= size, no hago nada

    cmp  dl, [rdi + OFFSET_SIZE]
    jge  .fin ; si j >= size, no hago nada

    cmp  dl, sil 
    je  .fin ; si i == j, no hago nada

    mov  r8, [rdi + OFFSET_ARRAY_DATA] ; guardo en r8 puntero a primera posicion de data
   
    mov  r9, [r8 + rsi*SIZE_POINTER] ; guardo en r9 valor de posicion i de data
    mov  rcx, [r8 + rdx*SIZE_POINTER] ; guardo en rcx valor de posicion j de data
    mov  [r8 + rsi*SIZE_POINTER], rcx
    mov  [r8 + rdx*SIZE_POINTER], r9

    .fin:
        pop rbp
        ret

; void arrayDelete(array_t* a)
arrayDelete:
    ; armo stack frame
    push rbp
    mov  rbp, rsp 

    push r12
    push r13
    push rbx
    push r14
    push r15
    sub rsp, 8

    mov r12, rdi; guardamos puntero a array en r12

    mov edi, [r12 + OFFSET_TYPE]
    call getDeleteFunction 
    mov r13, rax ; en r13 el puntero a la función de borrado

    mov r14, [r12 + OFFSET_ARRAY_DATA] ; guardo en r14 puntero a primera posicion de data
    xor r15, r15
    mov r15b, [r12 + OFFSET_SIZE] ; guardo en r15 size de array

    cmp r15, 0
    je .fin

    xor rbx, rbx

    .ciclo:
        mov rdi, [r14 + rbx*SIZE_POINTER] ; guardo data en posición rbx de array
        call r13 ; se libera memoria de data
        inc rbx
        cmp rbx, r15
        jl .ciclo ; si rbx < size, se sigue borrando memoria

    .fin: 
        mov rdi, r14
        call free ; se libera memoria de a->data

        mov rdi, r12
        call free ; se libera memoria de a

        add rsp, 8
        pop r15
        pop r14
        pop rbx
        pop r13
        pop r12
        pop rbp
        ret

;void arrayPrint(array_t* a, FILE* pFile)
arrayPrint:
    ; armo stack frame
    push rbp
    mov  rbp, rsp 

    push r12
    push r13
    push rbx
    push r14
    push r15
    sub rsp, 8

    mov r12, rsi; guardamos puntero a pFile en r12

    xor r13, r13
    mov r13b, [rdi + OFFSET_SIZE] ; guardo size de array en r13
    mov r14, [rdi + OFFSET_ARRAY_DATA] ; guardo en r14 puntero a primera posicion de data

    mov edi, [rdi + OFFSET_TYPE]
    call getPrintFunction 
    mov r15, rax ; en r15 el puntero a la función de impresión

    mov  rdi, r12 ; pFile en rdi
    mov  rsi, formato_escritura_string ; "%s" en rsi
    mov  rdx, formato_escritura_apertura_array ; "[" en rdx
    call fprintf ; llamo a fprintf

    cmp r13, 0
    je .fin
    dec r13 ; size-1 de array en r13

    xor rbx, rbx ; inicializo en 0 rbx para utilizar como contador

    .ciclo:
        mov rdi, [r14 + rbx*SIZE_POINTER] ; guardo data en posición rbx de array
        mov rsi, r12 ; guardo pFile en rsi
        call r15 ; imprimo en el archivo lo que hay en data
        
        cmp rbx, r13
        je .fin
        
        mov  rdi, r12 ; pFile en rdi
        mov  rsi, formato_escritura_string ; "%s" en rsi
        mov  rdx, formato_escritura_variable_array ; "," en rdx
        call fprintf ; llamo a fprintf

        inc rbx
        jmp .ciclo

    .fin: 

        mov  rdi, r12 ; pFile en rdi
        mov  rsi, formato_escritura_string ; "%s" en rsi
        mov  rdx, formato_escritura_cierre_array ; "]" en rdx
        call fprintf ; llamo a fprintf

        add rsp, 8
        pop r15
        pop r14
        pop rbx
        pop r13
        pop r12
        pop rbp
        ret

    

; ** Card **

; card_t* cardNew(char* suit, int32_t* number)
cardNew:
    ; armo stack frame
    push rbp
    mov  rbp, rsp
    ; pusheo no volátiles que voy a usar
    push r12 
    push r13
    push rbx
    sub  rsp, 8 ;alineo la pila a 16 bytes

    mov  r13, rsi   ; guardo *number
    mov  r12, rdi   ; guardo *suit en r12

    mov  rdi, SIZE_OF_CARD   
    call malloc      ; rax tiene puntero a struct

    mov  rbx, rax    ; rbx mantiene el puntero a struct

    mov  rdi, r12
    call strClone  ; rax tiene el suit(string) clonado
    mov  [rbx + OFFSET_SUIT], rax  ; copio el puntero al suit en el struct  

    mov  rdi, r13
    call intClone  ; rax tiene el number(int32) clonado
    mov  [rbx + OFFSET_NUMBER], rax  ; copio el puntero al number en el struct  

    mov  rdi, TYPE_CARD
    call listNew  ; rax tiene el *stacked(nueva lista vacía)
    mov  [rbx + OFFSET_STACKED], rax  ; copio el puntero a stacked en el struct  

    mov rax, rbx

    add  rsp, 8
    pop  rbx
    pop  r13
    pop  r12
    pop  rbp
    ret

;char* cardGetSuit(card_t* c)
cardGetSuit:
    ; armo stack frame
    push rbp
    mov  rbp, rsp

    ;rdi tiene *c(Carta)
    mov  rax, [rdi + OFFSET_SUIT]

    pop  rbp
    ret

;int32_t* cardGetNumber(card_t* c)
cardGetNumber:
    ; armo stack frame
    push rbp
    mov  rbp, rsp

    ;rdi tiene *c(Carta)
    mov  rax, [rdi + OFFSET_NUMBER]

    pop  rbp
    ret

;list_t* cardGetStacked(card_t* c)
cardGetStacked:
    ; armo stack frame
    push rbp
    mov  rbp, rsp

    ;rdi tiene *c(Carta)
    mov  rax, [rdi + OFFSET_STACKED]

    pop  rbp
    ret

;void cardPrint(card_t* c, FILE* pFile)
cardPrint:
    ; armo stack frame
    push rbp
    mov  rbp, rsp

    push r12
    push r13

    mov  r12, rdi; guardamos el puntero a c 
    mov  r13, rsi; guardamos el puntero al archivo

    mov  rdi, r13 ; pFile en rdi
    mov  rsi, formato_escritura_string ; "%s" en rsi
    mov  rdx, formato_escritura_apertura_carta ; "{" en rdx
    call fprintf ; llamo a fprintf

    mov  rdi, [r12 + OFFSET_SUIT] ; suit de la card en rdi
    mov  rsi, r13 ; pfile
    call strPrint

    mov  rdi, r13 ; pFile en rdi
    mov  rsi, formato_escritura_string ; "%s" en rsi
    mov  rdx, formato_escritura_variable_carta ; "-" en rdx
    call fprintf ; llamo a fprintf

    mov  rdi, [r12 + OFFSET_NUMBER] ; number de la card en rdi
    mov  rsi, r13 ; pfile
    call intPrint

    mov  rdi, r13 ; pFile en rdi
    mov  rsi, formato_escritura_string ; "%s" en rsi
    mov  rdx, formato_escritura_variable_carta ; "-" en rdx
    call fprintf ; llamo a fprintf

    mov  rdi, [r12 + OFFSET_STACKED] ; stacked de la card en rdi
    mov  rsi, r13 ; pfile
    call listPrint

    mov  rdi, r13 ; pFile en rdi
    mov  rsi, formato_escritura_string ; "%s" en rsi
    mov  rdx, formato_escritura_cierre_carta ; "}" en rdx
    call fprintf ; llamo a fprintf
 
    pop  r13
    pop  r12
    pop  rbp
    ret

;int32_t cardCmp(card_t* a, card_t* b)
cardCmp:
    ; armo stack frame
    push rbp
    mov  rbp, rsp

    push r12
    push r13

    mov  r12, rdi ;r12 tiene *a(Carta)
    mov  r13, rsi ;r13 tiene *b(Carta)

    mov  rdi, [r12 + OFFSET_SUIT]
    mov  rsi, [r13 + OFFSET_SUIT]
    call strCmp ;rax tiene el resultado de la comparacion

    cmp  rax, 0
    jne  .fin

    mov  rdi, [r12 + OFFSET_NUMBER]
    mov  rsi, [r13 + OFFSET_NUMBER]
    call intCmp ;rax tiene el resultado de la comparacion

    .fin:
        pop  r13
        pop  r12
        pop  rbp
        ret

;card_t* cardClone(card_t* c)
cardClone:
    ; armo stack frame
    push rbp
    mov  rbp, rsp
    ; pusheo no volátiles que voy a usar
    push r12 
    push r13

    mov  r12, rdi   ; r12 tiene *c(card)

    mov  rdi, SIZE_OF_CARD   
    call malloc      ; rax tiene puntero a struct

    mov  r13, rax    ; r13 mantiene el puntero a struct

    mov  rdi, [r12 + OFFSET_SUIT]
    call strClone  ; rax tiene el suit(string) clonado
    mov  [r13 + OFFSET_SUIT], rax  ; copio el puntero al suit en el struct  

    mov  rdi, [r12 + OFFSET_NUMBER]
    call intClone  ; rax tiene el number(int32) clonado
    mov  [r13 + OFFSET_NUMBER], rax  ; copio el puntero al number en el struct  

    mov  rdi, [r12 + OFFSET_STACKED]
    call listClone  ; rax tiene el *stacked(lista)
    mov  [r13 + OFFSET_STACKED], rax  ; copio el puntero a stacked en el struct  

    mov rax, r13

    pop  r13
    pop  r12
    pop  rbp
    ret

;void cardAddStacked(card_t* c, card_t* card)
cardAddStacked:
    ; armo stack frame
    push rbp
    mov  rbp, rsp

    mov  rdi, [rdi + OFFSET_STACKED] 
    call listAddFirst

    pop  rbp
    ret

;void cardDelete(card_t* c)
cardDelete:
    ; armo stack frame
    push rbp
    mov  rbp, rsp

    push r12 
    sub  rsp, 8 ;alineo la pila a 16 bytes

    ;r12 tiene *c(Carta)
    mov  r12, rdi

    mov  rdi, [r12 + OFFSET_SUIT]
    call strDelete

    mov  rdi, [r12 + OFFSET_NUMBER]
    call intDelete

    mov  rdi, [r12 + OFFSET_STACKED]
    call listDelete

    mov  rdi, r12
    call free

    add  rsp, 8

    pop  r12
    pop  rbp
    ret