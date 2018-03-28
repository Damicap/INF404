#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "analyse_lexicale.h"
#include "lecture_caracteres.h"
#include "couleur.h"


//Variable locale   
lexeme lexeme_en_cours ;	/* le lexeme courant */


typedef enum {E_INIT, E_ENTIER, E_FIN,E_ERREUR,E_M,E_MO,E_MOV,E_REGISTRE,E_REGISTRE2,E_A,E_AD,E_ADD,E_S,E_SU,E_SUB,E_ST,E_STR,E_L,E_LD,E_LDR,E_FIN_INSTRUCTION} Etat_Automate ;

//Fonction locale

int est_retour_ligne(char c);
int est_chiffre(char c ) ;
void ajouter_caractere (char *s, char c);
int est_espace(char c);
int est_caractere(char c);
void reconnaitre_lexeme();


   /* --------------------------------------------------------------------- */

   void demarrer(char *nom_fichier) {
      demarrer_car(nom_fichier);
      avancer();
   }

   /* --------------------------------------------------------------------- */

   void avancer() {
      reconnaitre_lexeme();
   }

   /* --------------------------------------------------------------------- */

   lexeme lexeme_courant() {
      return lexeme_en_cours;
   }

   /* --------------------------------------------------------------------- */

   int fin_de_sequence() {
      return lexeme_en_cours.nature == FIN_SEQUENCE;
   }

   /* --------------------------------------------------------------------- */

   void arreter() {
      arreter_car() ;
   } 
   
   /* --------------------------------------------------------------------- */
   void fon_erreur(Etat_Automate *etat)
   {
        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
        lexeme_en_cours.nature=ERREUR;
        *etat = E_ERREUR;
        avancer_car ();
   }



void reconnaitre_lexeme()
{
    Etat_Automate etat=E_INIT;
    
    while (est_espace(caractere_courant()))  //Lire les espaces, tabulations
    {
        avancer_car() ;
    } 
    
    lexeme_en_cours.chaine[0] = '\0' ;
    
    while (etat != E_FIN) 
    {
	    switch (etat) 
        {
            case E_INIT:
                
                if (fin_de_sequence_car() != 0)  //Si on a une fin de séquence
                {
                    lexeme_en_cours.nature = FIN_SEQUENCE;
                    etat = E_FIN;
                }
                
                else
                {
                    if (caractere_courant()=='#') //Pour les valeur immédiates (entier)
                    {
                        lexeme_en_cours.nature = ENTIER;
                        lexeme_en_cours.ligne = numero_ligne();
                        lexeme_en_cours.colonne = numero_colonne();
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        lexeme_en_cours.valeur = 0;
                        etat = E_ENTIER;
 
                    }
                    
                    else if( est_caractere(caractere_courant()) )
                    {
                        lexeme_en_cours.ligne = numero_ligne();
                        lexeme_en_cours.colonne = numero_colonne();
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        
                        switch(caractere_courant())
                        {
                            case 'A':
                            case 'a':
                                lexeme_en_cours.nature=ADD;
                                etat=E_A;
                                break;
                                
                            case 'M':
                            case 'm':
                                lexeme_en_cours.nature=MOV;
                                etat=E_M;
                                break;
                    
                            case 'S':
                            case 's':
                                lexeme_en_cours.nature=SUB;
                                etat=E_S;
                                break;
                               
                            case ',':
                                lexeme_en_cours.nature=VIRGULE;
                                etat=E_FIN;
                                break;
                                
                            case 'R':
                            case 'r':
                                lexeme_en_cours.nature=R1;
                                etat=E_REGISTRE;
                                break;
                                
                            case 'L':
                            case 'l':
                                lexeme_en_cours.nature=LDR;
                                etat=E_L;
                                break;
                                
                            case '\n':
                                lexeme_en_cours.nature=RETOUR_LIGNE;
                                etat=E_FIN;
                                break;
                                
                            default:
                                lexeme_en_cours.nature=ERREUR;
                                etat=E_ERREUR;
                                break;
                                
                        }
                    }
                }
                                       
                avancer_car ();
                break;
                
            case E_ENTIER:
                if (est_chiffre(caractere_courant())) 
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    lexeme_en_cours.valeur = lexeme_en_cours.valeur * 10 + caractere_courant() - '0';
                    avancer_car ();
                } 
                else
                {
                    etat=E_FIN;
                }
                break;
                
    
               
            /*LECTURE DE MOV*/
               
            case E_M:
                if (caractere_courant()=='o' || caractere_courant()=='O')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_MO;
                    avancer_car ();
                }
                else
                {
                    fon_erreur(&etat);
                }
                break;
                
            case E_MO:
                if (caractere_courant()=='v' || caractere_courant()=='V')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_MOV;
                    avancer_car ();
                }
                else
                {
                    fon_erreur(&etat);
                }
                break;
                
            case E_MOV:
                if ( est_espace(caractere_courant()) || est_retour_ligne(caractere_courant())  )
                {
                    etat = E_FIN;
                }
                else
                {
                   fon_erreur(&etat);
                }
                break;
                
                
            /*LECTURE DE ADD*/
            
            case E_A:
                if (caractere_courant()=='d' || caractere_courant()=='D')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_AD;
                    avancer_car ();
                }
                else
                {
                    fon_erreur(&etat);
                }
                break;
                
            case E_AD:
                if (caractere_courant()=='d' || caractere_courant()=='D')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_ADD;
                    avancer_car ();
                }
                else
                {
                    fon_erreur(&etat);
                }
                break;
                
            case E_ADD:
                if ( est_espace(caractere_courant()) || est_retour_ligne(caractere_courant())  )
                {
                    etat = E_FIN;
                }
                else
                {
                    fon_erreur(&etat);
                }
                break;
            
                /*LECTURE DE SUB*/
            
            case E_S:
                if (caractere_courant()=='u' || caractere_courant()=='U')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_SU;
                    avancer_car ();
                }
                else if(caractere_courant()=='t' || caractere_courant()=='T')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_ST;
                    avancer_car ();
                }
                else
                {
                    fon_erreur(&etat);
                }
                break;
                
            case E_SU:
                if (caractere_courant()=='b' || caractere_courant()=='B')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_SUB;
                    avancer_car ();
                }
                else
                {
                    fon_erreur(&etat);
                }
                break;
                
            case E_SUB:
                if ( est_espace(caractere_courant()) || est_retour_ligne(caractere_courant())  )
                {
                    etat = E_FIN;
                }
                else
                {
                    fon_erreur(&etat);
                }
                break;
            
                /*LECTURE DE STR*/
                
                case E_ST:
                if (caractere_courant()=='r' || caractere_courant()=='R')
                {
                    lexeme_en_cours.nature=STR;
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_STR;
                    avancer_car ();
                }
                else
                {
                    fon_erreur(&etat);
                }
                break;
                
                case E_STR:
                if ( est_espace(caractere_courant()) || est_retour_ligne(caractere_courant())  )
                {
                    etat = E_FIN;
                }
                else
                {
                    fon_erreur(&etat);
                }
                break;
                
                
                /*LECTURE DE LDR*/
                
                case E_L:
                if (caractere_courant()=='d' || caractere_courant()=='D')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_LD;
                    avancer_car ();
                }
                else
                {
                    fon_erreur(&etat);
                }
                break;
                
                case E_LD:
                if (caractere_courant()=='r' || caractere_courant()=='R')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_LDR;
                    avancer_car ();
                }
                else
                {
                    fon_erreur(&etat);
                }
                break;
                
                case E_LDR:
                if ( est_espace(caractere_courant()) || est_retour_ligne(caractere_courant())  )
                {
                    etat = E_FIN;
                }
                else
                {
                    fon_erreur(&etat);
                }
                break;
                
                
                
                
                /*LECTURE DES REGISTRES*/
                
            case E_REGISTRE:
                if (est_chiffre(caractere_courant()))
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    switch (caractere_courant())
                    {
                        case '0':
                            lexeme_en_cours.nature=R0;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '1':
                            lexeme_en_cours.nature=R1;
                            etat = E_REGISTRE2;
                            avancer_car();
                            break;
                        
                        case '2':
                            lexeme_en_cours.nature=R2;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '3':
                            lexeme_en_cours.nature=R3;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '4':
                            lexeme_en_cours.nature=R4;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '5':
                            lexeme_en_cours.nature=R5;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                        
                        case '6':
                            lexeme_en_cours.nature=R6;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '7':
                            lexeme_en_cours.nature=R7;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '8':
                            lexeme_en_cours.nature=R8;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                    
                        case '9':
                            lexeme_en_cours.nature=R9;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                    }
                }
                else
                {
                    etat = E_ERREUR;
                }
    
            //Registre à 2 chiffres
            case E_REGISTRE2:
                if (est_chiffre(caractere_courant()) && caractere_courant()<='6')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    switch (caractere_courant())
                    {
                        case '0':
                            lexeme_en_cours.nature=R10;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '1':
                            lexeme_en_cours.nature=R11;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                        
                        case '2':
                            lexeme_en_cours.nature=R12;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '3':
                            lexeme_en_cours.nature=R13;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '4':
                            lexeme_en_cours.nature=R14;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '5':
                            lexeme_en_cours.nature=R15;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                        
                        case '6':
                            lexeme_en_cours.nature=R16;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                    }
                }
                else if (est_chiffre(caractere_courant()) && caractere_courant()>'6')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_ERREUR;
                    lexeme_en_cours.nature=ERREUR;
                }
                else 
                {
                    etat = E_FIN_INSTRUCTION;
                }
                
            
            case E_FIN_INSTRUCTION:
                if( caractere_courant()==',' || est_espace(caractere_courant())  )
                {
                    etat=E_FIN;
                }
                else
                {
                    etat=E_ERREUR;
                }
                break;
            
            case E_ERREUR:
            if ( !est_espace(caractere_courant()) && !est_retour_ligne(caractere_courant()) && !fin_de_sequence_car()  )
            {
                etat=E_ERREUR;
                ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                avancer_car ();
            }
            else
                etat = E_FIN;
            break;
              
            case E_FIN:
                break ;
            
        }
        
    }
    
}




int est_caractere(char c)
{
    
    return c>=0 && c<=127;
}

int est_espace(char c)
{
    return c=='\t' || c==' ';
}

int est_retour_ligne(char c)
{
    return c=='\n';
}

int est_chiffre(char c) 
{
      return (c >= '0') && (c <= '9') ;
}

void ajouter_caractere (char *s, char c) 
{
	int l ;

	l = strlen(s) ;
	s[l] = c ;
	s[l+1] = '\0' ;
} 




   // renvoie la chaine de caracteres correspondant a la nature du lexeme
char *Nature_vers_Chaine (nature_lexeme nature) 
{
    switch (nature)
    {
		case MOV: return "MOV" ;
		case ADD: return "ADD" ;
        case SUB: return "SUB" ;
        case STR: return "STR" ;
        case LDR: return "LDR" ;
        case R0: return "R0" ;
        case R1: return "R1" ;
        case R2: return "R2" ;
        case R3: return "R3" ;
        case R4: return "R4" ;
        case R5: return "R5" ;
        case R6: return "R6" ;
        case R7: return "R7" ;
        case R8: return "R8" ;
        case R9: return "R9" ;
        case R10: return "R10" ;
        case R11: return "R11" ;
        case R12: return "R12" ;
        case R13: return "R13" ;
        case R14: return "R14" ;
        case R15: return "R15" ;
        case R16: return "R16" ;
        case ENTIER: return "ENTIER" ;
        case VIRGULE: return "VIRGULE" ;
        case AFFICHER_REGISTRE: return "AFFICHER_REGISTRE" ;
        case RETOUR_LIGNE: return "RETOUR_LIGNE" ;
        case FIN_SEQUENCE: return "FIN_SEQUENCE" ;
        default: return "ERREUR" ;
    }
}

   // affiche a l'ecran le lexeme l
void afficher(lexeme l) 
{
    switch (l.nature) 
    {
        case FIN_SEQUENCE:
            break;
        default:
            if(l.nature==ERREUR)
                rouge_gras();
            printf("(ligne %d, ", l.ligne);
            printf("colonne %d) : ",l.colonne);
            printf("[") ;
            printf("nature = %s", Nature_vers_Chaine(l.nature)) ;
            printf(", chaine = %s, ", l.chaine) ;
            if(l.nature==ERREUR)
                blanc();
            switch(l.nature) 
            {
                case ENTIER:
                    printf("valeur = %d", l.valeur);
                    break;
                default:
                    break;
            } 
        if(l.nature==ERREUR)
            afficher_rouge_gras("]");
        else
            printf("]") ;
	} 
}
