#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "table_hash.h"
#define CAPACITY_HASH 10000
#define CAPACITY_WORD 50

// Structure de liste chainé de mot pour les stopwords
typedef struct Stopword {
    char *word;
    struct Stopword *next;
} stopword;

// lit les stopwords du fichier et rempli la structure 
stopword *file_as_list_stop_words(FILE* file){
    stopword *stop_words = NULL;
    stopword *current = NULL;
    char *word = calloc(CAPACITY_WORD,sizeof(char));
    char c;
    int i = 0;
    while ((c = fgetc(file)) != EOF) {
        if(c>64 && c<91 || c>96 && c<123){
            word[i]=c;
            i++;
        }
        else if(i>0){
            word[i]='\0';
            stopword *new_node = malloc(sizeof(stopword));
            new_node->word = malloc(strlen(word) + 1);
            strcpy(new_node->word, word); 
            new_node->next = NULL;

            if(stop_words==NULL)
                stop_words = new_node;
            else 
                current->next = new_node;
            current = new_node;
            word[0]='\0';
            i=0; 
        }
    } 
    fclose(file);
    return stop_words;
}

// lit le fichier en param et rempli la table de hashage avec les mots et leurs occurences
hashtbl *file_as_hashed_words(FILE* file, stopword *stopwords){
    stopword *head_stop_word = stopwords;
    hashtbl *hash_table = hashtbl_create(CAPACITY_HASH);
    char c;
    char *word = calloc(CAPACITY_WORD,sizeof(char));
    int i = 0;
    do {
        c = fgetc(file);
        if(c>='a' && c<='z' || c>'A' && c<'Z'){
            if(c>'A' && c<'Z'){
                c=c+'a'-'A';
            }
            word[i]=c;
            i++;
        }
        else {
            if(i>1){
                word[i]='\0';
                // on ne met pas le mot s'il fait parti des stopwords
                bool isStopWord = false;
                while (stopwords->next != NULL) {
                    if(strcmp(stopwords->word,word)==0){
                        isStopWord = true;
                        break;
                    }
                    else 
                        stopwords = stopwords->next;
                }
                stopwords = head_stop_word;
                if(!isStopWord){
                    char *copy = malloc(strlen(word) + 1);
                    strcpy(copy, word);
                    hashtbl_add_one(hash_table, copy);
                }
            }
            i=0;
            word[0]='\0';
        }
    } while (c != EOF);
    fclose(file);
    return hash_table;
}

void frequency(char *path_book, char *path_stop_words, int n) {
    FILE* file = fopen(path_book, "r");
    FILE* file_stop_words = fopen(path_stop_words, "r");

    if (file == NULL || file_stop_words == NULL) {
        printf("Impossible d'ouvrir le fichier %s ou %s", path_book, path_stop_words);
        return;
    }

    stopword *stopwords = file_as_list_stop_words(file_stop_words);
    hashtbl *hash_table = file_as_hashed_words(file, stopwords);
    print_top_n_words(hash_table, n);

}

int main(int argc, char *argv[]){
    if(argc != 3)
        printf("Il faut deux paramètres, le chemin du fichier txt et le nombre de mots");
    else
        frequency(argv[1], "stop_words.txt", atoi(argv[2]));
}