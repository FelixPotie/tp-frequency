typedef struct Hashtbl hashtbl;
typedef struct Pair pair;

hashtbl *hashtbl_create(int capacity);

void hashtbl_add_one(hashtbl *h, char *k);
void print_top_n_words(hashtbl *h, int n);