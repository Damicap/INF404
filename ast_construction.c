#include <stdlib.h>
#include <stdio.h>

#include "type_ast.h"


Ast creer_valeur_int(long int val) 
{
      Ast expr ;
      expr = (Ast) malloc (sizeof(NoeudAst));
      expr->nature = VALEUR;
      expr->valeur = val;
      return expr ;
}


Ast creer_registre(long int val) 
{
      Ast expr ;
      expr = (Ast) malloc (sizeof(NoeudAst));
      expr->nature = REGISTRE;
      expr->registre = val;
      return expr ;
}



Ast creer_fonction_base(TypeFonctionBase fun_base , Ast fils_1 , Ast fils_2, Ast fils_3, Ast fils_instr_suiv)
{
    Ast expr ;
    expr = (Ast) malloc (sizeof(NoeudAst));
    expr->nature = FONCTION_BASE;
    expr->instruction = fun_base;
    
    expr->fils1 = fils_1 ;
    expr->fils2 = fils_2 ;
    expr->fils3 = fils_3 ;
    expr->instr_suiv = fils_instr_suiv ;
       
    return expr ;
}

