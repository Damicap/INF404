#include <stdio.h>
#include "type_ast.h"

void afficher_arbre(Ast expr);
void init_registre();
void afficher_registre();



Ast affectation(Ast expr);
void addition(Ast expr);
void soustraction(Ast expr);
void charger(Ast expr);
void stocker(Ast expr);
void decaler_droite(Ast expr);
void decaler_gauche(Ast expr);
void et_bit_a_bit(Ast expr);
void ou_bit_a_bit(Ast expr);
void et_bit_a_bit_bool(Ast expr);
void affichage_cara(Ast expr);
void affichage_nombre (Ast expr);
void affichage_chaine_caractere(Ast expr);

void init_tab_arbre(Ast expr);

void evaluation(Ast expr);

