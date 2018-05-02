
#ifndef __TYPE_AST__
#define __TYPE_AST__ 

typedef enum {FONCTION_BASE,REGISTRE,VALEUR} TypeAst ;
typedef enum {N_FIN,N_MOV, N_ADD, N_SUB, N_STR, N_LDR,N_BAL,N_BEQ,N_BNE,N_BLT,N_BGT,N_CMP,N_LSL,N_LSR,N_AND,N_ORR,N_TST,N_BL,N_AFFRN,N_AFFRC,N_AFFAC} TypeFonctionBase ;

typedef struct noeud { 
	TypeAst nature ;
	TypeFonctionBase instruction ;
	struct noeud *fils1, *fils2, *fils3, *instr_suiv ;
    long int syntaxe_str_ldr;
    long int valeur ;
    long int registre;
    long int numero_instruction;
    
} NoeudAst ;

typedef NoeudAst *Ast ;

#endif  
