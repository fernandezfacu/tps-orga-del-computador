#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "lib.h"

int main(int argc, char* argv[]){     
    
    FILE* archivo = fopen("mazos.txt", "w"); 
    int number1 = 1;
    int number2 = 2;
    int number3 = 3;
    int number4 = 4;
    int number5 = 5;
    char oro[] = "oro";
    char espada[] = "espada";
    char basto[] = "basto";
    char copa[] = "copa";
    card_t* card1 = cardNew(espada, &number1);
    card_t* card2 = cardNew(espada, &number2);
    card_t* card3 = cardNew(basto, &number3);
    card_t* card4 = cardNew(copa, &number4);
    card_t* card5 = cardNew(oro, &number5);

    type_t tipo = TypeCard;

    // MAZO CON ARREGLO

    uint8_t capacidad = 5;
    array_t* mazo_arr = arrayNew(tipo, capacidad); 

    arrayAddLast(mazo_arr, card1);
    arrayAddLast(mazo_arr, card2);
    arrayAddLast(mazo_arr, card3);
    arrayAddLast(mazo_arr, card4);
    arrayAddLast(mazo_arr, card5);

    arrayPrint(mazo_arr, archivo);

    card_t* card = arrayGet(mazo_arr, 2);
    card_t* card_aux = arrayGet(mazo_arr, 0);
    cardAddStacked(card, card_aux);

    arrayPrint(mazo_arr, archivo);

    arrayDelete(mazo_arr);


    fprintf(archivo, "\n");

    // MAZO CON LISTA

    list_t* mazo_list = listNew(tipo); 

    listAddLast(mazo_list, card1);
    listAddLast(mazo_list, card2);
    listAddLast(mazo_list, card3);
    listAddFirst(mazo_list, card4);
    listAddFirst(mazo_list, card5);

    listPrint(mazo_list, archivo);

    card = listGet(mazo_list, 2);
    card_aux = listGet(mazo_list, 0);
    cardAddStacked(card, card_aux);

    listPrint(mazo_list, archivo);

    listDelete(mazo_list);

    cardDelete(card1);
    cardDelete(card2);
    cardDelete(card3);
    cardDelete(card4);
    cardDelete(card5);
    fclose(archivo); 

    return 0;
}