#include <stdio.h>
#include <string.h>
#include "gestion_variable.h"

void init_seq(sequence *seq)
{
    seq->taille=0;
}

void ajouter_variable(sequence *seq,char *nom_var,long int val)
{
    if (chercher_etiq(seq,nom_var)!=-1)
    {
        printf("Erreur ajout étiquette impossible \n");
    }
    else
    {
        if (seq->taille<NB_ETIQ)
        {
            seq->tab_val[seq->taille]=val;
            strcpy(seq->tab_nom[seq->taille],nom_var);
            seq->taille=seq->taille+1;
        }
        else
        {
            printf("Erreur taille séquence");
        }
    }
}

long int chercher_etiq(sequence *seq,char *nom)
{
    for(long int i=0;i<seq->taille;i++)
    {
        if(strcmp (seq->tab_nom[i],nom)==0)
            return i;
    }
    return -1;
}

long int chercher_valeur(sequence *seq,char *nom)
{
    long int indice=0;
    for(long int i=0;i<seq->taille;i++)
    {
        if(strcmp (seq->tab_nom[i],nom)==0)
            indice=i;
    }
    return seq->tab_val[indice];
}

void ajouter_valeur(seq_int *seq,long int val)
{
    if(seq->taille>taille_memoire)
    {
        printf("Erreur taille mémoire");
    }
    seq->tab[seq->taille]=val;
    seq->taille=seq->taille+1;
}


void init_memoire(seq_int *memoire)
{
    memoire->taille=0;
    for(long int i=0;i<=taille_memoire;i++)
    {
        memoire->tab[i]=-1;
    }
}

 void afficher_memoire(seq_int memoire)
 {
    for(long int i = 0;i<taille_memoire;i++)
    {
        if(memoire.tab[i]!=-1)
            printf("%ld: %ld \n",i,memoire.tab[i]);
    }
 }
