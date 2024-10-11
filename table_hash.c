#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

typedef struct Entry {
    char *key;
    int value;
    struct Entry *next;
} entry;

typedef struct Hashtbl 
{
    int capacity; // 0 < capacity
    int size; // nombre d'entrées dans la table
    entry **data; // tableau de taille capacity
} hashtbl;

hashtbl *hashtbl_create(int capacity) {
    hashtbl *h = malloc(sizeof(hashtbl));
    h->capacity = capacity;
    h->data = calloc(capacity, sizeof(entry*));
    h->size = 0;
    return h;
}

int hashtbl_bucket(hashtbl *hash, char *k) {
    int h=0;
    char c;
    while ((c = *k++) != 0) {
        h=31*h+c;
    }
    int i = h & INT_MAX; // opération bit à bit pour garder le bit de poids fort à 0 et ne pas avoir de neg
    return i % hash->capacity; 
}

entry *hashtbl_find_entry(entry *e, char *k) {
    while (e != NULL) {
        if (strcmp(k, e->key) == 0) {
            return e;
        }
        e = e->next;
    }
    return NULL;
}

entry *create_entry(char *k, int v, entry *n) {
    entry *e = malloc(sizeof(entry));
    e->key = k;
    e->value = v;
    e->next = n;
    return e;
}

void hashtbl_add_one(hashtbl *h, char *k) {
    int b = hashtbl_bucket(h, k);
    entry *e = hashtbl_find_entry(h->data[b], k);
    if (e == NULL) {
        h->data[b] = create_entry(k, 1, h->data[b]);
        h->size++;
    }
    else {
        e->value++;
    }
}

typedef struct Pair {
    char *key;
    int value;
} pair;

pair *copy(pair *p1){
    pair *tmp = malloc(sizeof(pair));
    tmp->key = p1->key;
    tmp->value = p1->value;
    return tmp;
}

// converti la table de hashage en list
void print_top_n_words(hashtbl *h, int n){
    pair **res = calloc(n, sizeof(pair*));
    for(int k=0; k<n;k++){
        res[k]=malloc(sizeof(pair));
        res[k]->value = 0;
    }
    for(int i=0;i<h->capacity;i++){
        if(h->data[i]!=NULL){
            entry *e = h->data[i];
            while (e != NULL) {
                //on ajoute si la valeur est dans le top n
                if(e->value > res[n-1]->value){
                    for(int j=0; j<n; j++){
                        if(e->value > res[j]->value){
                            pair *tmp = copy(res[j]);
                            res[j]->key = e->key;
                            res[j]->value = e->value;
                            for(int k=j+1;k<n;k++){
                                pair *tmp2 = copy(res[k]);
                                res[k]->key = tmp->key;
                                res[k]->value = tmp->value;
                                tmp = tmp2;
                            }
                            break;
                        }
                    }
                }
                e=e->next;
            }
            free(h->data[i]);
        }
    }
    for(int i=0;i<n;i++){
        printf("(%s,%d)\n", res[i]->key, res[i]->value);
    }
}