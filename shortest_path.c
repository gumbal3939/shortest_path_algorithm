#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ARR 512

//struct dell'albero binario 
typedef struct nodo_{
    unsigned int dist;
    struct nodo_ *left, *right, *p;
    unsigned int *car;
    int n_car, dim_arr;
} nodo_t;

//funzione per creare un nodo dell'albero binario
nodo_t* new_node(int key){
    nodo_t *temp;

    temp = (nodo_t*) malloc(sizeof(nodo_t));
    temp->dist = key;
    temp->left = temp->right = temp->p = NULL;
    return temp;
}

//funzione per aggiungere un nodo all'albero binario
nodo_t* tree_insert(nodo_t **root, int key){
    nodo_t *x, *y, *z;

    z = new_node(key);
    y = NULL;
    x = *root;
    //scorro l'albero binario fino a quando non trovo una foglia
    while(x != NULL){
        y = x;
        if(z->dist < x->dist)
            x = x->left;
        else
            x = x->right;
    }
    //assegno il puntatore al padre di z a y
    z->p = y;
    //se l'albero è vuoto la radice diventa z
    if(y == NULL)
        *root = z;
    //assegno il puntatore al figlio sinistro/destro di y a z
    else if(z->dist < y->dist)
        y->left = z;
    else
        y->right = z;
    //tree_insert ritorna il nodo appena aggiunto
    return z;
}

//funzione per la ricerca di un nodo nell'albero binario
nodo_t* tree_search(nodo_t *x, int key){

    if(x == NULL || x->dist == key)
        return x;

    if(key < x->dist)
        return tree_search(x->left, key);
    return tree_search(x->right, key);
}
//tree_search ritorna il nodo cercato se trovato, NULL altrimenti

//funzione per trovare il minimo di un albero binario
nodo_t* tree_minimum(nodo_t* x){

    while(x->left != NULL)
        x = x->left;
    return x;
}

//funzione per trovare il successore di un elemento in un albero binario
nodo_t* tree_successor(nodo_t *x){
    nodo_t *y; 

    //se è presente l'albero destro il successore è il suo minimo
    if(x->right != NULL)
        return tree_minimum(x->right);
    //altrimenti, risalgo l'albero fino a trovare un elemento che è figlio sinistro di suo padre
    y = x->p;
    while(y != NULL && x == y->right){
        x = y;
        y = y->p;
    }
    return y;
}

//ERRORE NELLA TREE_DELETE 
//funzione per eliminare un nodo dall'albero binario
nodo_t* tree_delete(nodo_t **root, nodo_t* z){
    nodo_t *x, *y;

    //se z non ha più di un sottoalbero
    if(z->left == NULL || z->right == NULL)
        y = z;
    else
        y = tree_successor(z);
    //assegno ad x la radice del sottoalbero di y se ne ha uno, NULL altrimenti
    if(y->left != NULL)
        x = y->left;
    else
        x = y->right;
    //se y ha un sottoalbero
    if(x != NULL)
        x->p = y->p;
    //se y è la radice
    if(y->p == NULL)
        *root = x;
    //verifico se y è figlio sinistro/destro e associo i relativi puntatori ad x
    else if(y == y->p->left)
        y->p->left = x;
    else
        y->p->right = x;
    //se z ha due sottoalberi
    if(y != z){
        //oltre alla distanza è necessario aggiornare il puntatore all'array di auto e il loro numero
        free(z->car);
        z->dist = y->dist;
        y->dist = -1;
        z->car = y->car;
        z->n_car = y->n_car; //<---- HO AGGIUNTO UN -1, NON SO PERCHè ABBIA FUNZIONATO
        z->dim_arr = y->dim_arr;
    }
    return y;
}

//funzione per trovare il massimo di un albero binario
nodo_t* tree_maximum(nodo_t* x){

    while(x->right != NULL)
        x = x->right;
    return x;
}

//funzione per trovare il predecessore di un elemento in un albero binario
nodo_t* tree_predecessor(nodo_t *x){
    nodo_t *y; 

    //se è presente l'albero sinistro il predecessore è il suo minimo
    if(x->left != NULL)
        return tree_maximum(x->left);
    //altrimenti, risalgo l'albero fino a trovare un elemento che è figlio destro di suo padre
    y = x->p;
    while(y != NULL && x == y->left){
        x = y;
        y = y->p;
    }
    return y;
}

//funzione per deallocare i nodi dell'albero binario (potrei considerare una versione che non occupi memoria)
void free_bst(nodo_t *node){

    if(node == NULL)
        return;

    free_bst(node->right);
    free_bst(node->left);

    free(node->car);
    free(node);
}

/*

void inorder_tree_walk(nodo_t *node){
    if(node == NULL)
        return;
    inorder_tree_walk(node->left);
    printf("%d %d\n", node->dist, (node->car)[node->n_car - 1]);
    inorder_tree_walk(node->right);
}

*/

//funzione per creare un max-heap
void build_max_heap(unsigned int *arr, int n_auto){
    int i, j, temp;

    for(i = 1; i < n_auto; i++){

        //se il figlio è minore del padre
        if(arr[i] > arr[(i - 1) / 2]){
            j = i;
            //fino a quando il figlio è maggiore del padre
            while(arr[j] > arr[(j - 1) / 2]) {

                //scambia padre con figlio
                temp = arr[j];
                arr[j] = arr[(j - 1) / 2];
                arr[(j - 1) / 2] = temp;
                //continua la verifica con il prossimo padre
                j = (j - 1) / 2;
            }
        }
    }
}

void heap_sort(unsigned int *arr, int n_auto){
    int i, j, index;
    unsigned int temp;

    build_max_heap(arr, n_auto);

    for(i = n_auto - 1; i > 0; i--){

        //sostituisco il primo elemento dell'array con indici [0, ..., i] con l'ultimo 
        temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        j=0;
        //ricostituisco la proprietà di max-heap nel sotto-array rimasto
        do{

            index = (2 * j + 1);
            //se il figlio sinistro è minore di quello destro
            if(index < (i - 1) && arr[index] < arr[index + 1])
                index++;
            //se il padre è minore del figlio maggiore
            if(index < i && arr[j] < arr[index]){
                temp = arr[j];
                arr[j] = arr[index];
                arr[index] = temp;
            }
            j = index;

        } while(index < i);
    }
}

//funzione per aggiungere una stazione all'albero binario
int aggiungi_stazione(nodo_t **root, FILE *fp){
    nodo_t *temp;
    int n_auto;
    unsigned int dist, autonomia;
    int i;

    if(fscanf(fp, "%d", &dist)){}

    //controllo se la stazione è presente all'interno dell'albero binario
    if(tree_search(*root, dist) != NULL)
        return 0;

    if(fscanf(fp, "%d" "%d", &n_auto, &autonomia)){};

    //inserisco la stazione all'interno dell'albero binario
    temp = tree_insert(root, dist);

    //salvo il numero di auto presenti nella stazione e la dimensione effettiva dell'array dinamico
    temp->n_car = temp->dim_arr = n_auto;

    //considero il caso in cui la stazione non ha auto
    if(n_auto == 0)
        temp->dim_arr = 1;

    //alloco lo spazio necessario al parco auto della stazione
    temp->car = malloc(sizeof(int) * temp->dim_arr);
    if(temp->car == NULL)
        printf("Memoria non allocata\n");

    i = 0;
    while(n_auto > 0){

        temp->car[i] = autonomia;
        if(fscanf(fp, "%d", &autonomia)){}
        n_auto--;
        i++;
    }

    //heap_sort non fa niente se riceve un array di dimensione nulla
    heap_sort(temp->car, temp->n_car);

    return 1;
}

//ERRORE NELLA DEMOLISCI_STAZIONE ---> ERRORE NELLA TREE_DELETE
//funzione per eliminare una stazione dall'albero binario
int demolisci_stazione(nodo_t **root, FILE *fp){
    nodo_t *temp;
    int dist;

    if(fscanf(fp, "%d", &dist)){}

    //controllo se la stazione è presente all'interno dell'albero binario
    temp = tree_search(*root, dist);
    if(temp == NULL)
        return 0;

    temp = tree_delete(root, temp);

    //libero lo spazio allocato per il nodo dell'albero binario e per l'array di auto
    //caso in cui il nodo da eliminare ha due sottoalberi
    if(temp->dist != -1)
        free(temp->car);
    free(temp);

    return 1;
}


//funzione per sistemare l'ultimo elemento aggiunto in un array ordinato
void mod_insertion_sort(unsigned int *arr, int n_auto){
    int i;
    unsigned int key;

    //non è necessario un ciclo perchè sistemo sempre e solo l'ultimo elemento dell'array
    key = arr[n_auto - 1];
    i = n_auto - 2;

    while(i >= 0 && arr[i] > key){
        arr[i + 1] = arr[i];
        i = i - 1;
    }

    arr[i + 1] = key;
}

//funzione per aggiungere un'auto al parco auto di una stazione
int aggiungi_auto(nodo_t *root, FILE *fp){
    nodo_t *temp;
    unsigned int dist, autonomia;

    if(fscanf(fp, "%d", &dist)){}

    //controllo se la stazione è presente all'interno dell'albero binario
    temp = tree_search(root, dist);
    if(temp == NULL)
        return 0;

    //aggiorno il numero di auto presenti nella stazione
    (temp->n_car)++;

    if(fscanf(fp, "%d", &autonomia)){}

    //controllo se è necessario allocare nuovo spazio per il parco auto della stazione
    if(temp->n_car > temp->dim_arr){

        //se la dimensione non eccede i limiti massimi alloco lo spazio richiesto, altrimenti alloco tutto quello possibile
        temp->dim_arr = temp->dim_arr * 2;
        if(temp->dim_arr > MAX_ARR)
            temp->dim_arr = MAX_ARR;

        temp->car = realloc(temp->car, sizeof(int) * temp->dim_arr);
        if(temp->car == NULL)
            return 0;
    }

    //memorizzo l'autonomia dell'auto appena aggiunta
    (temp->car)[temp->n_car - 1] = autonomia;
    mod_insertion_sort(temp->car, temp->n_car);

    return 1;
}

int bin_search(unsigned int *arr, int p, int r, int v){
    int q;

    if(p > r)
        return -1;
    //se v è più piccolo del più piccolo elemento o più grande del più grande
    if(v < arr[p] || v > arr[r])
        return -1;
    q = (p + r) / 2;
    if(arr[q] == v)
        return q;
    //devo cercare nella metà sinistra dell'array
    if(arr[q] > v)
        return bin_search(arr, p, q - 1, v);
    //devo cercare nella metà destra dell'array
    else
        return bin_search(arr, q + 1, r, v);
}

//funzione per eliminare un'auto dal parco auto di una stazione
int rottama_auto(nodo_t* root, FILE *fp){
    nodo_t *temp;
    int q;
    unsigned int dist, autonomia;

    if(fscanf(fp, "%d", &dist)){}

    //controllo se la stazione è presente all'interno dell'albero binario
    temp = tree_search(root, dist);
    if(temp == NULL)
        return 0;

    if(fscanf(fp, "%d", &autonomia)){}

    q = bin_search(temp->car, 0, (temp->n_car) - 1, autonomia);
    //controllo se l'auto è presente nel parco auto della stazione
    if(q == -1)
        return 0;

    //sposto gli elementi dell'array dopo quello da rimuovere di una posizione
    while(q < (temp->n_car) - 1){
        (temp->car)[q] = (temp->car)[q + 1];
        q++;
    }
    (temp->n_car)--;
    //non è necessario deallocare le singole celle dell'array (perchè le potrei riutilizzare)
    return 1;
}


//funzione per calcolare la somma dell'autonomia più alta della stazione e della sua distanza
unsigned int netto_r(nodo_t *node){
    return ((node->car)[node->n_car - 1] + node->dist);
}


int pianifica_right(nodo_t* root, FILE *fp_out, unsigned int *arr_basi, int *n_basi, unsigned int i_dist, unsigned int f_dist){
    nodo_t *base, *prec, *succ;
    int dist_rimasta = 0;
    int in;

    *n_basi = 0;

    base = tree_search(root, i_dist);
    prec = succ = tree_successor(base);

    arr_basi[*n_basi] = i_dist;
    *n_basi = *n_basi + 1;

    while(succ != NULL){
        in = 0;
        dist_rimasta = netto_r(base) - succ->dist;
        while(dist_rimasta >= 0){
            in = 1;
            if(succ->dist == f_dist){
                arr_basi[*n_basi] = f_dist;
                *n_basi = *n_basi + 1;
            }
            if(netto_r(prec) < netto_r(succ))
                prec = succ;
            succ = tree_successor(succ);
            if(succ != NULL)
                dist_rimasta = netto_r(base) - succ->dist;
            else
                dist_rimasta = -1;
        }
        if(!in){
            *n_basi = 0;
            return 0;
        }
        if(arr_basi[*n_basi - 1] != f_dist){
            arr_basi[*n_basi] = prec->dist;
            *n_basi = *n_basi + 1;
            base = prec;
            prec = succ;
            continue;
        }
        return 1;
    }
    printf("Non dovrebbe arrivare qua\n");
    return 0;
}


void aggiusta_right(nodo_t *root, unsigned int *arr_basi, int *n_basi){
    nodo_t *new_base;
    int i, found;
    unsigned int goal_base, old_base;

    i = *n_basi - 1;

    while(i >= 2){
        found = 0;
        goal_base = arr_basi[i];
        old_base = arr_basi[i - 1];
        new_base = tree_successor(tree_search(root, arr_basi[i - 2]));

        while(!found && new_base->dist != old_base){
            if(netto_r(new_base) >= goal_base){
                arr_basi[i - 1] = new_base->dist;
                found = 1;
            }
            new_base = tree_successor(new_base);
        }
        i--;
    }
}


unsigned int netto_l(nodo_t *node){
    int res;

    res = node->dist - (node->car)[node->n_car - 1];
    if(res < 0)
        return 0;
    return res;
}

int pianifica_left(nodo_t* root, FILE *fp_out, unsigned int *arr_basi, int *n_basi, unsigned int i_dist, unsigned int f_dist){
    nodo_t *base, *prec, *succ;
    int dist_rimasta = 0;
    int in;

    *n_basi = 0;

    base = tree_search(root, i_dist);
    prec = succ = tree_predecessor(base);

    arr_basi[*n_basi] = i_dist;
    *n_basi = *n_basi + 1;

    while(succ != NULL){
        in = 0;

        dist_rimasta = succ->dist - netto_l(base);
        while(dist_rimasta >= 0){
            in = 1;
            if(succ->dist == f_dist){
                arr_basi[*n_basi] = f_dist;
                *n_basi = *n_basi + 1;
            }

            if(netto_l(succ) <= netto_l(prec))
                prec = succ;
            succ = tree_predecessor(succ);

            if(succ != NULL)
                dist_rimasta = succ->dist - netto_l(base);
            else
                dist_rimasta = -1;
        }

        if(!in){
            *n_basi = 0;
            return 0;
        }
        if(arr_basi[*n_basi - 1] != f_dist){
            arr_basi[*n_basi] = prec->dist;
            *n_basi = *n_basi + 1;
            base = prec;
            prec = succ;
            continue;
        }
        return 1;
    }
    printf("Non dovrebbe arrivare qua\n");
    return 0;
}

void aggiusta_left(nodo_t *root, unsigned int *arr_basi, int *n_basi){
    nodo_t *new_base, *ref_base;
    int i;
    unsigned int goal_base, old_base;

    i = *n_basi - 1;

    while(i >= 2){

        goal_base = arr_basi[i];
        old_base = arr_basi[i - 1];
        ref_base = tree_search(root, arr_basi[i - 2]);
        new_base = tree_predecessor(tree_search(root, old_base));

        while(netto_l(ref_base) <= new_base->dist){
            if(netto_l(new_base) <= goal_base)
                arr_basi[i - 1] = new_base->dist;

            new_base = tree_predecessor(new_base);
        }
        i--;
    }
}


//funzione per trovare il percorso minimo tra due stazioni dell'autostrada
int pianifica_percorso(nodo_t *root, FILE *fp_in, FILE *fp_out, unsigned int *arr_basi, int *n_basi){
    unsigned int i_dist, f_dist;
    int res = 0;

    if(fscanf(fp_in, "%d" "%d", &i_dist, &f_dist)){}

    //se stazione iniziale e finale coincidono
    if(i_dist == f_dist){
        fprintf(fp_out, "%d\n", i_dist);
        return 1;
    }

    if(i_dist < f_dist){
        res = pianifica_right(root, fp_out, arr_basi, n_basi, i_dist, f_dist);
        if(res && *n_basi != 0)
            aggiusta_right(root, arr_basi, n_basi);

    } else{
        res = pianifica_left(root, fp_out, arr_basi, n_basi, i_dist, f_dist);
        if(res && *n_basi != 0)
            aggiusta_left(root, arr_basi, n_basi);
    }

    return res;
}


int main(int argc, char *argv[]){
    FILE *fp_in, *fp_out;
    nodo_t *root = NULL;
    char temp[100];
    int n_basi, dim = 512;
    int i;
    unsigned int *arr_basi = malloc(sizeof(unsigned int) * dim);

    fp_in = stdin;
        if(fp_in){
            fp_out = stdout;
            if(fp_out){
                if(fscanf(fp_in, "%s", temp)){}
                while(!feof(fp_in)){
                    if(strcmp(temp, "aggiungi-stazione") == 0){
                        if(aggiungi_stazione(&root, fp_in))
                            fprintf(fp_out, "%s", "aggiunta\n");
                        else
                            fprintf(fp_out, "%s", "non aggiunta\n");
                    }
                    else if(strcmp(temp, "aggiungi-auto") == 0){
                        if(aggiungi_auto(root, fp_in))
                            fprintf(fp_out, "%s", "aggiunta\n");
                        else
                            fprintf(fp_out, "%s", "non aggiunta\n");
                    }
                    else if(strcmp(temp, "demolisci-stazione") == 0){
                        if(demolisci_stazione(&root, fp_in))
                            fprintf(fp_out, "%s", "demolita\n");
                        else
                            fprintf(fp_out, "%s", "non demolita\n");
                    }
                    else if(strcmp(temp, "rottama-auto") == 0){
                        if(rottama_auto(root, fp_in))
                            fprintf(fp_out, "%s", "rottamata\n");
                        else
                            fprintf(fp_out, "%s", "non rottamata\n");
                    }
                    else if(strcmp(temp, "pianifica-percorso") == 0){
                        if(pianifica_percorso(root, fp_in, fp_out, arr_basi, &n_basi)){
                            if(n_basi != 0){
                                i = 0;
                                while(i < n_basi - 1){
                                    fprintf(fp_out, "%d ", arr_basi[i]);
                                    i = i + 1;
                                }
                                fprintf(fp_out, "%d\n", arr_basi[i]);
                            }
                        } else
                            fprintf(fp_out, "%s", "nessun percorso\n");
                    }
                    if(fscanf(fp_in, "%s", temp)){}
                }
                fclose(fp_in);
                fclose(fp_out);

                free(arr_basi);
                free_bst(root);
            } else
                printf("Errore di apertura\n");
        } else
            printf("Errore di apertura\n");

    return 0;
}
