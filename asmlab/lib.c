#include "lib.h"

funcCmp_t* getCompareFunction(type_t t) {
    switch (t) {
        case TypeInt:      return (funcCmp_t*)&intCmp; break;
        case TypeString:   return (funcCmp_t*)&strCmp; break;
        case TypeCard:     return (funcCmp_t*)&cardCmp; break;
        default: break;
    }
    return 0;
}
funcClone_t* getCloneFunction(type_t t) {
    switch (t) {
        case TypeInt:      return (funcClone_t*)&intClone; break;
        case TypeString:   return (funcClone_t*)&strClone; break;
        case TypeCard:     return (funcClone_t*)&cardClone; break;
        default: break;
    }
    return 0;
}
funcDelete_t* getDeleteFunction(type_t t) {
    switch (t) {
        case TypeInt:      return (funcDelete_t*)&intDelete; break;
        case TypeString:   return (funcDelete_t*)&strDelete; break;
        case TypeCard:     return (funcDelete_t*)&cardDelete; break;
        default: break;
    }
    return 0;
}
funcPrint_t* getPrintFunction(type_t t) {
    switch (t) {
        case TypeInt:      return (funcPrint_t*)&intPrint; break;
        case TypeString:   return (funcPrint_t*)&strPrint; break;
        case TypeCard:     return (funcPrint_t*)&cardPrint; break;
        default: break;
    }
    return 0;
}


/** Int **/

int32_t intCmp(int32_t* a, int32_t* b){
    if(*a == *b)
        return 0;
    else if(*a < *b)
        return 1;
    else return -1;
}

void intDelete(int32_t* a){
    free(a);
}

void intPrint(int32_t* a, FILE* pFile){
    fprintf(pFile, "%i", *a);
}

int32_t* intClone(int32_t* a){
    int32_t* a_cloned = malloc(sizeof(int32_t));
    *a_cloned = *a;
    return a_cloned;
}

/** Lista **/

list_t* listNew(type_t t){  
    list_t* list = malloc(sizeof(list_t));
    list->type = t;
    list->size = 0;
    list->first = NULL; 
    list->last = NULL;
    return list;
}

uint8_t listGetSize(list_t* l){
    return l->size;
}

void* listGet(list_t* l, uint8_t i){
    if(i >= l->size) 
        return 0;
    listElem_t* actual = l->first;
    for(uint8_t j = 0; j < i; j++) 
        actual = actual->next;
    return actual->data;
}

void listAddFirst(list_t* l, void* data){
    listElem_t* listElem = malloc(sizeof(listElem_t));
    funcClone_t* fClone = getCloneFunction(l->type);
    listElem->data = (*fClone)(data);
    listElem->prev = NULL;
    listElem->next = l->first;
    if(!l->first)
        l->last = listElem;
    else 
        l->first->prev = listElem;
    l->first = listElem;
    l->size++;
}

void listAddLast(list_t* l, void* data){
    listElem_t* listElem = malloc(sizeof(listElem_t));
    funcClone_t* fClone = getCloneFunction(l->type);
    listElem->data = (*fClone)(data);
    listElem->prev = l->last;
    listElem->next = NULL;
    if(!l->last)
        l->first = listElem;
    else 
        l->last->next = listElem;
    l->last = listElem;
    l->size++;
}

void* listRemove(list_t* l, uint8_t i){
    if(i >= l->size)
        return 0;
    listElem_t* actual = l->first;
    for(uint8_t j = 0; j < i; j++) 
        actual = actual->next;
    void* data = actual->data;
    if(actual == l->first)
        l->first = actual->next;
    if(actual == l->last) 
        l->last = actual->prev;
    if(actual->prev)
        actual->prev->next = actual->next;
    if(actual->next)
        actual->next->prev = actual->prev;
    free(actual); 
    l->size--;
    return data;
}

void listSwap(list_t* l, uint8_t i, uint8_t j) {
    if(i >= l->size || j >= l->size || i == j)
        return;
    if(i > j) {
        uint8_t aux = i;
        i = j;
        j = aux;
    }

    listElem_t* element_i = NULL;
    listElem_t* element_j = NULL;
    listElem_t* actual = l->first;
    
    int k = 0;
    while(!element_i || !element_j) {
        if(k == i) {
            element_i = actual;
        } else if (k == j) {
            element_j = actual;
        }
        actual = actual->next;
        k++;
    }

    if(l->first == element_i)
        l->first = element_j;
    if(l->last == element_j)
        l->last = element_i;

    listElem_t* prevJ = element_j->prev;
    listElem_t* prevI = element_i->prev;
    listElem_t* nextJ = element_j->next;
    listElem_t* nextI = element_i->next;

    if(((prevJ == element_i) && (nextI == element_j)) || ((nextJ == element_i) && (prevI == element_j))) {
        if(element_i->prev)
            element_i->prev->next = element_j;
        if(element_j->next)
            element_j->next->prev = element_i;
        element_i->next = nextJ;
        element_j->prev = prevI;
        element_i->prev = nextI;
        element_j->next = prevJ;
    } else {
        if(element_i->prev)
            element_i->prev->next = element_j;
        if(element_j->next)
            element_j->next->prev = element_i;
        element_i->next->prev = element_j;
        element_j->prev->next = element_i;
        element_i->next = nextJ;
        element_j->prev = prevI;
        element_i->prev = prevJ;
        element_j->next = nextI;
    }
}

list_t* listClone(list_t* l) {
    list_t* listClone = listNew(l->type);
    listElem_t* actual = l->first;
    while(actual) {
        listAddLast(listClone, actual->data);        
        actual = actual->next;
    }
    return listClone;
}

void listDelete(list_t* l){
    funcDelete_t* fDelete = getDeleteFunction(l->type);
    listElem_t* actual = l->first;
    while(actual) {
        (*fDelete)(actual->data);
        listElem_t* aux = actual;
        actual = actual->next;
        free(aux);
    }
    free(l);
}

void listPrint(list_t* l, FILE* pFile) {
    funcPrint_t* fPrint = getPrintFunction(l->type);
    listElem_t* actual = l->first;
    fprintf(pFile, "%s", "[");
    while(actual) {
        (*fPrint)(actual->data, pFile);
        if(actual->next)
            fprintf(pFile, "%s", ",");
        actual = actual->next;
    }
    fprintf(pFile, "%s", "]");
}

/** Game **/

game_t* gameNew(void* cardDeck, funcGet_t* funcGet, funcRemove_t* funcRemove, funcSize_t* funcSize, funcPrint_t* funcPrint, funcDelete_t* funcDelete) {
    game_t* game = (game_t*)malloc(sizeof(game_t));
    game->cardDeck = cardDeck;
    game->funcGet = funcGet;
    game->funcRemove = funcRemove;
    game->funcSize = funcSize;
    game->funcPrint = funcPrint;
    game->funcDelete = funcDelete;
    return game;
}
int gamePlayStep(game_t* g) {
    int applied = 0;
    uint8_t i = 0;
    while(applied == 0 && i+2 < g->funcSize(g->cardDeck)) {
        card_t* a = g->funcGet(g->cardDeck,i);
        card_t* b = g->funcGet(g->cardDeck,i+1);
        card_t* c = g->funcGet(g->cardDeck,i+2);
        if( strCmp(cardGetSuit(a), cardGetSuit(c)) == 0 || intCmp(cardGetNumber(a), cardGetNumber(c)) == 0 ) {
            card_t* removed = g->funcRemove(g->cardDeck,i);
            cardAddStacked(b,removed);
            cardDelete(removed);
            applied = 1;
        }
        i++;
    }
    return applied;
}
uint8_t gameGetCardDeckSize(game_t* g) {
    return g->funcSize(g->cardDeck);
}
void gameDelete(game_t* g) {
    g->funcDelete(g->cardDeck);
    free(g);
}
void gamePrint(game_t* g, FILE* pFile) {
    g->funcPrint(g->cardDeck, pFile);
}
