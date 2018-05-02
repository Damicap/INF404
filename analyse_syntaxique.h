#include "type_ast.h"

void Erreur(char *c,long int ligne);

Ast Rec_programme(long int numero_instr);
Ast Rec_ligne(long int num_instr);
Ast Rec_instruction(long int num_instr);
Ast Rec_fonction_MOV(long int num_instr);
Ast Rec_fonction_ADD(long int num_instr);
Ast Rec_fonction_SUB(long int num_instr);
Ast Rec_registre();
Ast Rec_operande();
Ast Rec_fonction_LDR(long int num_instr);
Ast Rec_fonction_STR(long int num_instr);
Ast Rec_entier();
void Rec_virgule();
Ast Rec_retourLigne();
Ast Rec_fonction_CMP(long int num_instr);
Ast Rec_fonction_BRANCHEMENT(TypeFonctionBase fonction,long int num_instr);
Ast Rec_fonction_LSL(long int num_instr);
Ast Rec_fonction_LSR(long int num_instr);
Ast Rec_fonction_AND(long int num_instr);
Ast Rec_fonction_ORR(long int num_instr);
Ast Rec_fonction_TST(long int num_instr);
Ast Rec_fonction_AFFRC(long int num_instr);
Ast Rec_fonction_AFFRN(long int num_instr);
Ast Rec_fonction_AFFAC(long int num_instr);
// Renvoie l'AST contenant la valeur de l'étiquette
Ast Rec_fonction_VARIABLE();
/*Permet de récupére toutes les étiquettes*/
void Recuperer_etitquette(long int i);

void analyse_syntaxique(char *fichier);


void evaluer(char *fichier,int debug);
