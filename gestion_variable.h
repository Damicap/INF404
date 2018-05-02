
#define NB_ETIQ 200 //Nombre max d'étiquettes
#define taille_memoire 1000 

typedef struct sequence
{
    long int taille;
    long int tab_val[NB_ETIQ];
    char tab_nom[NB_ETIQ][32];
}sequence;

typedef struct seq_int //Séquence pour la mémoire
{
    long int taille;
    long int tab[taille_memoire];
}seq_int;

/*Pour sequence*/
void init_seq(sequence *seq);

// Ajoute une variable dans la séquence seq
void ajouter_variable(sequence *seq,char *nom_var,long int val);

// Retourne -1 si la val nom n'est pas une variable sinon renvoie l'indice de son emplacement dans le tableau
long int chercher_etiq(sequence *seq,char *nom);

//Retourne la valeur de la variable nom
long int chercher_valeur(sequence *seq,char *nom);

//Ajouter une valeur dans la mémoire
void ajouter_valeur(seq_int *seq,long int val);

void init_memoire(seq_int *);

 void afficher_memoire(seq_int);
