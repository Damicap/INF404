#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "couleur.h"
#include "analyse_lexicale.h"
#include "analyse_syntaxique.h"
#include "type_ast.h"
#include "ast_construction.h"
#include "ast_parcours.h"




void Erreur(char *c,long int ligne)
{
    rouge();
    printf("%s %ld \n",c,ligne);
    blanc();
}


Ast Rec_programme(long int numero_instr) // .data déclaration des données .text  puis les lignes de codes
{
    while(lexeme_courant().nature != DATA)
    {
        avancer();
        if(lexeme_courant().nature == FIN_SEQUENCE)
        {
            afficher_rouge_gras("Pas de .data dans le programme \n");
            exit(1);
        }
    }
    while(lexeme_courant().nature != TEXT)
    {
        avancer();
        if(lexeme_courant().nature == FIN_SEQUENCE)
        {
            afficher_rouge_gras("Pas de .text dans le programme \n");
            exit(1);
        }
    }
    avancer();
    return Rec_ligne(numero_instr);
}


Ast Rec_ligne(long int num_instr) /*Une ligne peut être une un commentaire avec une instruction vide*/
{   
    if(lexeme_courant().nature==COMMENTAIRE)
    {
        avancer();
    }
    if (lexeme_courant().nature==ETIQUETTE)
    {
        avancer();
        if (lexeme_courant().nature==DWORD) //Si ptr_x .dword x 
        {
            avancer();
            avancer();
        }
    }
    return Rec_instruction(num_instr);
}


Ast Rec_instruction(long int num_instr){
    num_instr++;
    //Ast A;
	switch(lexeme_courant().nature){
		case MOV:
			avancer();
			return Rec_fonction_MOV(num_instr);
			break;
		case ADD:
			avancer();
			return Rec_fonction_ADD(num_instr);
			break;
		case SUB:
			avancer();
			return Rec_fonction_SUB(num_instr);
			break;
        case LDR:
			avancer();
			return Rec_fonction_LDR(num_instr);
			break;
        case STR:
			avancer();
			return Rec_fonction_STR(num_instr);
			break;
        case BAL:
            avancer();
			return Rec_fonction_BRANCHEMENT(N_BAL,num_instr);
			break;
        case BEQ:
            avancer();
			return Rec_fonction_BRANCHEMENT(N_BEQ,num_instr);
			break;
        case BNE:
            avancer();
			return Rec_fonction_BRANCHEMENT(N_BNE,num_instr);
			break;
        case BGT:
            avancer();
			return Rec_fonction_BRANCHEMENT(N_BGT,num_instr);
			break;
        case BLT:
			avancer();
			return Rec_fonction_BRANCHEMENT(N_BLT,num_instr);
			break;
        case BL:
            avancer();
			return Rec_fonction_BRANCHEMENT(N_BL,num_instr);
			break;
        case CMP:
			avancer();
			return Rec_fonction_CMP(num_instr);
			break;
        case LSL:
			avancer();
			return Rec_fonction_LSL(num_instr);
			break;
        case LSR:
			avancer();
			return Rec_fonction_LSR(num_instr);
			break;
        case AND:
			avancer();
			return Rec_fonction_AND(num_instr);
			break;
        case ORR:
			avancer();
			return Rec_fonction_ORR(num_instr);
			break;
        case TST:
			avancer();
			return Rec_fonction_TST(num_instr);
			break;
        case AFFRN:
			avancer();
			return Rec_fonction_AFFRN(num_instr);
			break;
        case AFFRC:
			avancer();
			return Rec_fonction_AFFRC(num_instr);
			break;
        case AFFAC:
            avancer();
			return Rec_fonction_AFFAC(num_instr);
			break;
        case RETOUR_LIGNE:
            num_instr--;
            avancer();
            return Rec_ligne(num_instr);
            break;
		case FIN_SEQUENCE:
            return NULL;
			break;
		default:
			//Erreur();
            afficher_rouge_gras("Erreur: instruction inconnue \n");
            exit(1);
            break;
	}
	
}
Ast Rec_fonction_AFFRN(long int num_instr)
{
    Ast A2,A4,A5;
	A2=Rec_registre();						
	A4=Rec_retourLigne(num_instr);	// instruction suivante 
    A5=creer_fonction_base(N_AFFRN,A2,NULL,NULL,A4);
    A5->numero_instruction=num_instr;
    return A5; 
}

Ast Rec_fonction_AFFRC(long int num_instr)
{ 
    Ast A2,A4,A5;
	A2=Rec_registre();						
	A4=Rec_retourLigne(num_instr);	// instruction suivante 
    A5=creer_fonction_base(N_AFFRC,A2,NULL,NULL,A4);
    A5->numero_instruction=num_instr;
    return A5; 
}

Ast Rec_fonction_AFFAC(long int num_instr)
{ 
    Ast A2,A4,A5;
	A2=Rec_registre();						
	A4=Rec_retourLigne(num_instr);	// instruction suivante 
    A5=creer_fonction_base(N_AFFAC,A2,NULL,NULL,A4);
    A5->numero_instruction=num_instr;
    return A5; 
}



Ast Rec_fonction_MOV(long int num_instr){// MOV 
    Ast A2,A3,A4,A5;
	A2=Rec_registre();		// MOV reg		
	Rec_virgule();		// MOV reg,
	A3=Rec_operande();		// MOV reg, reg		OU	   MOV reg, entier
	A4=Rec_retourLigne(num_instr);	// instruction suivante 
    A5=creer_fonction_base(N_MOV,A2,A3,NULL,A4);
    A5->numero_instruction=num_instr;
    return A5; 
}

Ast Rec_fonction_ADD(long int num_instr)
{
    Ast A2,A3,A4,A5,A6;
	A2=Rec_registre();			
	Rec_virgule();
	switch(lexeme_courant().nature)
    {
		case ENTIER:
            A3=creer_valeur_int(lexeme_courant().valeur);
			avancer();
			if(lexeme_courant().nature==RETOUR_LIGNE || lexeme_courant().nature==COMMENTAIRE)
            {
				A5=Rec_retourLigne(num_instr);
                A6= creer_fonction_base(N_ADD,A2,A3,NULL,A5);
			}
			else
            {
				Erreur("Erreur ADD , attend un retour ligne à la ligne: ",lexeme_courant().ligne);
                exit(1);
                
			}
			break;
		default:
			Rec_registre();
            A3=creer_registre(lexeme_courant().numero_registre);
			if(lexeme_courant().nature==RETOUR_LIGNE ||  lexeme_courant().nature==COMMENTAIRE)
            {
				A5=Rec_retourLigne(num_instr);
                A6= creer_fonction_base(N_ADD,A2,A3,NULL,A5);
			}
			else if(lexeme_courant().nature==VIRGULE)
            {
                avancer();
				A4=Rec_operande();
				A5=Rec_retourLigne(num_instr);
                A6= creer_fonction_base(N_ADD,A2,A3,A4,A5);
			}
			else
            {
				Erreur("Erreur ADD , attend un retour ligne ou une virgule à la ligne: ",lexeme_courant().ligne);
                exit(1);
			}
			
	}
	A6->numero_instruction=num_instr;
	return A6;
}
				
Ast Rec_fonction_SUB(long int num_instr)
{	
    Ast A2,A3,A4,A5,A6;
	A2=Rec_registre();				
	Rec_virgule();
	switch(lexeme_courant().nature){
		case ENTIER:
            A3=creer_valeur_int(lexeme_courant().valeur);
			avancer();
			if(lexeme_courant().nature==RETOUR_LIGNE || lexeme_courant().nature==COMMENTAIRE)
            {
				A5=Rec_retourLigne(num_instr);
                A6= creer_fonction_base(N_SUB,A2,A3,NULL,A5);
			}
			else
            {
				Erreur("Erreur SUB , attend un retour ligne à la ligne: ",lexeme_courant().ligne);
                exit(1);
			}
			break;
		default:
			A3=Rec_registre();
			if(lexeme_courant().nature==RETOUR_LIGNE || lexeme_courant().nature==COMMENTAIRE)
            {
				A5=Rec_retourLigne(num_instr);
                A6= creer_fonction_base(N_SUB,A2,A3,NULL,A5);
			}
			else if(lexeme_courant().nature==VIRGULE)
            {
				avancer();
				A4=Rec_operande();
				A5=Rec_retourLigne(num_instr);
                A6= creer_fonction_base(N_SUB,A2,A3,A4,A5);
			}
			else
            {
 				Erreur("Erreur SUB , attend un retour ligne ou une virgule à la ligne: ",lexeme_courant().ligne);
                exit(1);
			}
	}
	A6->numero_instruction=num_instr;
	return A6; 
}

Ast Rec_registre(){
	switch(lexeme_courant().nature){
        case REG:
			avancer();
            return creer_registre(lexeme_courant().numero_registre);	
		default:
 			Erreur("Erreur ,attend un registre à la ligne: ",lexeme_courant().ligne);
            exit(1);
	}

}

//Soit un entier soit un registre
Ast Rec_operande()
{ 
	switch(lexeme_courant().nature)
    {
		case ENTIER:
            avancer();
            return creer_valeur_int(lexeme_courant().valeur);
        case REG:
			avancer();
            return creer_registre(lexeme_courant().numero_registre);
		default:
            Erreur("Erreur attend un entier ou un registre à la ligne: ",lexeme_courant().ligne);
            exit(1);
	}
}

void Rec_virgule(){
	switch(lexeme_courant().nature){
		case VIRGULE:
			avancer();
			break;
		default:
            Erreur("Erreur attendu une virgule à la ligne: ",lexeme_courant().ligne);
            exit(1);
	}
}

Ast Rec_retourLigne(long int num_instr)
{
    if(lexeme_courant().nature==COMMENTAIRE)
        avancer();
	switch(lexeme_courant().nature)
    {
		case RETOUR_LIGNE:
			avancer();
			return Rec_ligne(num_instr); 
			break;
		case FIN_SEQUENCE:
			printf("Syntaxe correcte");
            return NULL;
			break;
		default:
            Erreur("Erreur attend un retour à la ligne ou une fin de séquence à la ligne: ",lexeme_courant().ligne);
            exit(1);
	}
}


Ast Rec_fonction_STR(long int num_instr){	
	Ast A2,A3,A4,A5,A6;
	A2=Rec_registre();				
	Rec_virgule();
	if(lexeme_courant().nature!=CROCHET_OUVRANT)
    {
        Erreur("Erreur attend un crochet ouvrant à la ligne: ",lexeme_courant().ligne);
	}
	avancer();
	A3=Rec_registre();
	switch(lexeme_courant().nature){
		case CROCHET_FERMANT:
			avancer();
			if(lexeme_courant().nature==VIRGULE){ 
				avancer();
				A4=Rec_entier();
				A5=Rec_retourLigne(num_instr);
                A6=creer_fonction_base(N_STR,A2,A3,A4,A5); //STR reg, [reg],int
                A6->syntaxe_str_ldr=0;
                A6->numero_instruction=num_instr;
			}else{				
				A5=Rec_retourLigne(num_instr);
                A6=creer_fonction_base(N_STR,A2,A3,NULL,A5); //STR reg, [reg]
                A6->numero_instruction=num_instr;
			}
			break;
		case VIRGULE:
			avancer();
			A4=Rec_entier();
			if(lexeme_courant().nature!=CROCHET_FERMANT)
            {
 				Erreur("Erreur attend un crochet fermant à la ligne: ",lexeme_courant().ligne);
			}
			avancer();
			A5=Rec_retourLigne(num_instr);
            A6=creer_fonction_base(N_STR,A2,A3,A4,A5); //STR reg, [reg, entier]
            A6->syntaxe_str_ldr=1;
            A6->numero_instruction=num_instr; 
			break;
		default:
 			Erreur("Erreur STR, attend crochet fermant ou virgule à la ligne: ",lexeme_courant().ligne);
			exit(1);
	}
	return A6;
}


Ast Rec_fonction_LDR(long int num_instr)
{	
	Ast A2,A3,A4,A5,A6=NULL;
	A2=Rec_registre();				
	Rec_virgule();
    if(lexeme_courant().nature == VARIABLE) //ldr r1,ptr_x
    {
        if(chercher_etiq(&seq_adresse,lexeme_courant().chaine_etiquette)==-1  )
        {
            Erreur("Erreur variable, variable introuvable à la ligne: ",lexeme_courant().ligne);
            exit(1);
        }
        else
        {
            A3=creer_valeur_int( chercher_valeur(&seq_adresse,lexeme_courant().chaine_etiquette) );
            avancer();
            A5=Rec_retourLigne(num_instr);
            A6=creer_fonction_base(N_LDR,A2,NULL,A3,A5); //Permet d'identifier qu'on charge une adresse: NLDR Registre NULL Adresse Suite_prog
            A6->numero_instruction=num_instr;
        }
        
    }
	else if(lexeme_courant().nature!=CROCHET_OUVRANT)
    {
        Erreur("Erreur attend un crochet ouvrant à la ligne: ",lexeme_courant().ligne);
        exit(1);
	}
	else if(lexeme_courant().nature==CROCHET_OUVRANT)
    {
        avancer();
        A3=Rec_registre();
        switch(lexeme_courant().nature){
            case CROCHET_FERMANT:
                avancer();
                if(lexeme_courant().nature==VIRGULE)
                {  
                    avancer();
                    A4=Rec_entier();
                    A5=Rec_retourLigne(num_instr);
                    A6 = creer_fonction_base(N_LDR,A2,A3,A4,A5); //LDR reg, [reg],int
                    A6->syntaxe_str_ldr=0;
                    A6->numero_instruction=num_instr;
                }
                else
                {				
                    A5=Rec_retourLigne(num_instr); //LDR reg, [reg]
                    A6=creer_fonction_base(N_LDR,A2,A3,NULL,A5);
                    A6->numero_instruction=num_instr;
                }
                break;
            case VIRGULE:	
                avancer();
                A4=Rec_entier();
                if(lexeme_courant().nature!=CROCHET_FERMANT)
                {
                    Erreur("Erreur attend un crochet fermant à la ligne: ",lexeme_courant().ligne);
                    exit(1);
                }
                avancer();
                A5=Rec_retourLigne(num_instr);
                A6 = creer_fonction_base(N_LDR,A2,A3,A4,A5); //LDR reg, [reg, entier]
                A6->syntaxe_str_ldr=1;
                A6->numero_instruction=num_instr;
                break;
            default:
                Erreur("Erreur LDR, attend crochet fermant ou virgule à la ligne: ",lexeme_courant().ligne);
                exit(1);
        }
    }
	return A6;
}

Ast Rec_entier(){
	switch(lexeme_courant().nature){
		case ENTIER:
            		avancer();
            		return creer_valeur_int(lexeme_courant().valeur);
		default:
 			Erreur("Erreur attend un entier à la ligne: ",lexeme_courant().ligne);
			exit(1);
	}
}

Ast Rec_fonction_CMP(long int num_instr)
{
    Ast A2,A3,A4,A5;
	A2=Rec_registre();		// CMP reg		
	Rec_virgule();		// CMP reg,
	A3=Rec_operande();		// CMP reg, reg		OU	   CMP reg, entier
	A4=Rec_retourLigne(num_instr);	// instruction suivante 
    A5=creer_fonction_base(N_CMP,A2,A3,NULL,A4);
    A5->numero_instruction=num_instr;
    return A5;
}

Ast Rec_fonction_BRANCHEMENT(TypeFonctionBase fonction,long int num_instr)
{
    if(lexeme_courant().nature==EXIT)
    {
        Ast A1,A2;
        avancer();
        A2=Rec_retourLigne(num_instr);	// instruction suivante 
        A1=creer_fonction_base(N_FIN,NULL,NULL,NULL,A2) ;
        A1->numero_instruction=num_instr;
        return A1 ;
    }
    else
    {
        Ast A1,A2,A3;
        A1=Rec_fonction_VARIABLE();
        A2=Rec_retourLigne(num_instr);	// instruction suivante 
        A3=creer_fonction_base(fonction,A1,NULL,NULL,A2);
        A3->numero_instruction=num_instr;
        return A3;
    }
}

Ast Rec_fonction_VARIABLE()
{
    if (lexeme_courant().nature==VARIABLE)
    {
        if (chercher_etiq(&seq_etiq,lexeme_courant().chaine_etiquette)==-1)
        {
            Erreur("Erreur étiquette, étiquette introuvable à la ligne: ",lexeme_courant().ligne);
            exit(1);
        }
        else
        {
            long int val=chercher_valeur(&seq_etiq,lexeme_courant().chaine_etiquette);
            avancer();
            return creer_valeur_int(val);
        }
    }
    else
    {
        Erreur ("Erreur: attend une étiquette à la ligne: ",lexeme_courant().ligne);
        exit(1);
    }
}


Ast Rec_fonction_LSL(long int num_instr){
	Ast A2,A3,A4,A5,A6;
	A2=Rec_registre();			
	Rec_virgule();
	switch(lexeme_courant().nature){
		case ENTIER:
            		A3=creer_valeur_int(lexeme_courant().valeur);
			avancer();
			if(lexeme_courant().nature==RETOUR_LIGNE || lexeme_courant().nature==COMMENTAIRE){
				A5=Rec_retourLigne(num_instr);
                		A6= creer_fonction_base(N_LSL,A2,A3,NULL,A5);
			}else{
                Erreur("Erreur LSL, attend un retour à la ligne à la ligne: ",lexeme_courant().ligne);
                exit(1);
			}
			break;
		default:
			Rec_registre();
            		A3=creer_registre(lexeme_courant().numero_registre);
			if(lexeme_courant().nature==RETOUR_LIGNE || lexeme_courant().nature==COMMENTAIRE){
				A5=Rec_retourLigne(num_instr);
                		A6= creer_fonction_base(N_LSL,A2,A3,NULL,A5);
			}else if(lexeme_courant().nature==VIRGULE){
                		avancer();
				A4=Rec_operande();
				A5=Rec_retourLigne(num_instr);
                		A6= creer_fonction_base(N_LSL,A2,A3,A4,A5);
			}else
            {
                Erreur("Erreur LSL, attend un retour ligne ou une virgule à la ligne: ",lexeme_courant().ligne);
                exit(1);
			}		
	}
	A6->numero_instruction=num_instr;
	return A6;
}

Ast Rec_fonction_LSR(long int num_instr){
	Ast A2,A3,A4,A5,A6;
	A2=Rec_registre();			
	Rec_virgule();
	switch(lexeme_courant().nature){
		case ENTIER:
            		A3=creer_valeur_int(lexeme_courant().valeur);
			avancer();
			if(lexeme_courant().nature==RETOUR_LIGNE || lexeme_courant().nature==COMMENTAIRE){
				A5=Rec_retourLigne(num_instr);
                		A6= creer_fonction_base(N_LSR,A2,A3,NULL,A5);
			}else{
                Erreur("Erreur STR, attend un retour à la ligne à la ligne: ",lexeme_courant().ligne);
                exit(1);
			}
			break;
		default:
			Rec_registre();
            		A3=creer_registre(lexeme_courant().numero_registre);
			if(lexeme_courant().nature==RETOUR_LIGNE || lexeme_courant().nature==COMMENTAIRE){
				A5=Rec_retourLigne(num_instr);
                		A6= creer_fonction_base(N_LSR,A2,A3,NULL,A5);
			}else if(lexeme_courant().nature==VIRGULE){
                		avancer();
				A4=Rec_operande();
				A5=Rec_retourLigne(num_instr);
                		A6= creer_fonction_base(N_LSR,A2,A3,A4,A5);
			}else
            {
                Erreur("Erreur LSR, attend crochet un retour à la ligne ou une virgule à la ligne: ",lexeme_courant().ligne);
                exit(1);
			}		
	}
	A6->numero_instruction=num_instr;
	return A6;
}

Ast Rec_fonction_AND(long int num_instr){
	Ast A2,A3,A4,A5,A6;
	A2=Rec_registre();			
	Rec_virgule();
	switch(lexeme_courant().nature){
		case ENTIER:
            		A3=creer_valeur_int(lexeme_courant().valeur);
			avancer();
			if(lexeme_courant().nature==RETOUR_LIGNE || lexeme_courant().nature==COMMENTAIRE){
				A5=Rec_retourLigne(num_instr);
                		A6= creer_fonction_base(N_AND,A2,A3,NULL,A5);
			}else
            {
                Erreur("Erreur AND, attend un retour à la ligne à la ligne: ",lexeme_courant().ligne);
                exit(1);
			}
			break;
		default:
			Rec_registre();
            		A3=creer_registre(lexeme_courant().numero_registre);
			if(lexeme_courant().nature==RETOUR_LIGNE || lexeme_courant().nature==COMMENTAIRE){
				A5=Rec_retourLigne(num_instr);
                		A6= creer_fonction_base(N_AND,A2,A3,NULL,A5);
			}else if(lexeme_courant().nature==VIRGULE){
                		avancer();
				A4=Rec_operande();
				A5=Rec_retourLigne(num_instr);
                		A6= creer_fonction_base(N_AND,A2,A3,A4,A5);
			}else{
 				Erreur("Erreur AND, attend crochet un retour à la ligne ou une virgule à la ligne: ",lexeme_courant().ligne);
                exit(1);
			}		
	}
	A6->numero_instruction=num_instr;
	return A6;
}

Ast Rec_fonction_ORR(long int num_instr){
	Ast A2,A3,A4,A5,A6;
	A2=Rec_registre();			
	Rec_virgule();
	switch(lexeme_courant().nature){
		case ENTIER:
            		A3=creer_valeur_int(lexeme_courant().valeur);
			avancer();
			if(lexeme_courant().nature==RETOUR_LIGNE || lexeme_courant().nature==COMMENTAIRE){
				A5=Rec_retourLigne(num_instr);
                		A6= creer_fonction_base(N_ORR,A2,A3,NULL,A5);
			}else
            {
				Erreur("Erreur ORR, attend un retour à la ligne à la ligne: ",lexeme_courant().ligne);
                exit(1);
			}
			break;
		default:
			Rec_registre();
            		A3=creer_registre(lexeme_courant().numero_registre);
			if(lexeme_courant().nature==RETOUR_LIGNE || lexeme_courant().nature==COMMENTAIRE){
				A5=Rec_retourLigne(num_instr);
                		A6= creer_fonction_base(N_ORR,A2,A3,NULL,A5);
			}else if(lexeme_courant().nature==VIRGULE){
                		avancer();
				A4=Rec_operande();
				A5=Rec_retourLigne(num_instr);
                		A6= creer_fonction_base(N_ORR,A2,A3,A4,A5);
			}else
            {
 				Erreur("Erreur ORR, attend crochet un retour à la ligne ou une virgule à la ligne: ",lexeme_courant().ligne);
                exit(1);
			}		
	}
	A6->numero_instruction=num_instr;
	return A6;
}

Ast Rec_fonction_TST(long int num_instr){
	Ast A2,A3,A5,A6;
	A2=Rec_registre();			
	Rec_virgule();
	switch(lexeme_courant().nature){
		case ENTIER: //TST r1, #1
            		A3=creer_valeur_int(lexeme_courant().valeur);
			avancer();
			if(lexeme_courant().nature==RETOUR_LIGNE || lexeme_courant().nature==COMMENTAIRE){
				A5=Rec_retourLigne(num_instr);
                		A6= creer_fonction_base(N_TST,A2,A3,NULL,A5);
			}else
            {
 				Erreur("Erreur TST, attend un retour à la ligne à la ligne: ",lexeme_courant().ligne);
                exit(1);
			}
			break;
		default: //TST r1, r2
			Rec_registre();
            		A3=creer_registre(lexeme_courant().numero_registre);
			if(lexeme_courant().nature==RETOUR_LIGNE || lexeme_courant().nature==COMMENTAIRE){
				A5=Rec_retourLigne(num_instr);
                A6= creer_fonction_base(N_TST,A2,A3,NULL,A5);
			}else
            {
                Erreur("Erreur TST, attend crochet un retour à la ligne ou une virgule à la ligne: ",lexeme_courant().ligne);
                exit(1);
			}		
	}
	A6->numero_instruction=num_instr;
	return A6;
}

/*Récupération des étiquettes,variables*/
void Recuperer_etitquette(long int instr)
{
    char nom_etiq[32]; long int bal_exit=0;long int text=0;; long int val=0;
    while( lexeme_courant().nature != FIN_SEQUENCE)
    {
        switch(lexeme_courant().nature)
        {
            case ETIQUETTE:
                strcpy(nom_etiq,lexeme_courant().chaine_etiquette);
                avancer();
                if(lexeme_courant().nature==DWORD && bal_exit==0 && text ==0) //Déclaration d'une variable dans .data
                {
                    avancer();
                    ajouter_variable( &seq_donnee,nom_etiq,memoire.taille); //Ajout de la variable 
                    ajouter_valeur(&memoire,lexeme_courant().valeur); //Ajout dans la mémoire
                    avancer();
                }
                else if(lexeme_courant().nature==DWORD && bal_exit==1) //Déclaration des pointeurs après le BAL EXIT:  adr_x .dword x
                {
                    avancer();
                    if (chercher_etiq(&seq_donnee,lexeme_courant().chaine_etiquette) < 0)
                    {
                        rouge();
                        printf("Pas de variable %s pour la création du pointeur à la ligne: %ld \n",lexeme_courant().chaine_etiquette,lexeme_courant().ligne);
                        blanc();
                        exit(1);
                    }
                    val=chercher_valeur(&seq_donnee,lexeme_courant().chaine_etiquette);
                    ajouter_variable( &seq_adresse,nom_etiq,val );
                    avancer(); 
                }
                else if(lexeme_courant().nature==SKIP  && text ==0) //Saut de mémoire
                {
                   
                    avancer();
                    ajouter_variable( &seq_donnee,nom_etiq,memoire.taille);
                    memoire.taille=memoire.taille + lexeme_courant().valeur;
                    avancer();
                }

                else if(lexeme_courant().nature==ASCII && bal_exit==0 && text ==0) //Déclaration de chaine de caractère
                {
                    
                    avancer();
                    
                    ajouter_variable( &seq_donnee,nom_etiq,memoire.taille); //Ajout de la variable 
                    for (unsigned int i=1; i<strlen(lexeme_courant().chaine )-1 ;i++) // "abc" -> On ne prend pas les " 
                    {
                        ajouter_valeur(&memoire,lexeme_courant().chaine[i]);
                    }
                    ajouter_valeur(&memoire,'\0');
                }
                else if(text==1) //Etiquette normale
                {
                    ajouter_variable( &seq_etiq,nom_etiq,instr);
                }
                else
                {
                    avancer();
                }
                break;
                
            /*case ASCII: //On met la chaine de caractère dans la mémoire
                avancer();
                ajouter_variable( &seq_donnee,nom_etiq,memoire.taille); //Ajout de la variable 
            */
            case DWORD: // .dword nombre
                avancer();
                ajouter_valeur(&memoire,lexeme_courant().valeur);
                avancer();
                break;
                
            case SKIP: //.SKIP nombre
                avancer();
                memoire.taille=memoire.taille + lexeme_courant().valeur;
                avancer();
                break;
                
            case RETOUR_LIGNE:
                avancer();
                break;
            case MOV:case ADD:case SUB:case LDR:case STR:case CMP:case BL:case BAL:case BEQ:case BNE:case BGT:case BLT:case LSL:case LSR:case TST: case AFFRN: case AFFRC: case AFFAC:
                avancer();
                instr++;
                break;
            case TEXT:
                avancer();
                text=1;
                break;
            case EXIT:
                avancer();
                bal_exit=1;
                break;
            default:
                avancer();
                break;
        }
    }
} 


void analyse_syntaxique(char *fichier)
{   
    init_memoire(&memoire);
    long int num_instr=0;
    init_seq(&seq_etiq);
    Recuperer_etitquette(1); //On récupère toutes les étiquettes
    demarrer(fichier);
    Ast A;
    A=Rec_programme(num_instr);
    afficher_arbre(A);
    afficher_vert_gras("Analyse réussie \n");
}

void evaluer(char * fichier,int debug)
{
    init_memoire(&memoire);
    long int num_instr=0;
    init_seq(&seq_etiq);
    Recuperer_etitquette(1); //On récupère toutes les étiquettes
    demarrer(fichier);
    Ast A,A_cpy;
    A=Rec_programme(num_instr);
    A_cpy=A;
    init_registre();
    init_tab_arbre(A_cpy);
    evaluation(A);
    

    if (debug==1)
    {   afficher_jaune_gras("\n  ***********************************  ") ;
        afficher_jaune_gras("\n  *         ZONE DEBOGGUAGE         *  ") ;
        afficher_jaune_gras("\n  *********************************** \n\n ") ;
        afficher_jaune_gras("\n  ***********************************  ") ;
        afficher_jaune_gras("\n  *            L'ARBRE              *  ") ;
        afficher_jaune_gras("\n  *********************************** \n\n ") ;
        afficher_arbre(A);
        afficher_jaune_gras("\n  ***********************************  ") ;
        afficher_jaune_gras("\n  *          LES REGISTRES          *  ") ;
        afficher_jaune_gras("\n  *********************************** \n\n ") ;        
        afficher_registre();
        afficher_jaune_gras("\n  ***********************************  ") ;
        afficher_jaune_gras("\n  *            LA MEMOIRE           *  ") ;
        afficher_jaune_gras("\n  *********************************** \n\n ") ;  
        afficher_memoire(memoire);
    }
}
