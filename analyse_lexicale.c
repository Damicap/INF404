#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "analyse_lexicale.h"
#include "lecture_caracteres.h"
#include "couleur.h"


//Variable locale   
lexeme lexeme_en_cours ;	/* le lexeme courant */


typedef enum {E_INIT, E_ENTIER, E_ENTIERMOINS, E_ENTIER_NEGATIF, E_FIN, E_ERREUR, E_M, E_MO, E_REGISTRE, E_REGISTRE2, E_A, E_AD,E_AN, E_S, E_SU, E_ST,E_POINTS,E_POINTSK,E_POINTSKI, E_L, E_LD ,E_LS,E_C,E_T,E_TS, E_CM, E_B, E_BA, E_BG, E_BE, E_BN, E_BL, E_O, E_OR, E_P, E_E, E_EX, E_F,E_AF,E_AFF,E_AFFA,E_AFFR, E_EXI, E_POINT, E_POINTD, E_POINTDA, E_POINTDAT, E_POINTT, E_POINTTE, E_POINTTEX, E_POINTDW, E_POINTDWO, E_POINTDWOR,E_POINTA,E_POINTAS,E_POINTASC,E_POINTASCI ,E_COMMENTAIRE, E_CHAINE ,E_ETIQUETTE, E_ETIQUETTE_2, E_VARIABLE, E_FIN_COMMANDE,E_FIN_INSTRUCTION,E_APO,E_APOCAR,} Etat_Automate ;

//Fonction locale

long int est_retour_ligne(char c);
long int est_chiffre(char c ) ;
void ajouter_caractere (char *s, char c);
long int est_espace(char c);
long int est_caractere(char c);
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

   long int fin_de_sequence() {
      return lexeme_en_cours.nature == FIN_SEQUENCE;
   }

   /* --------------------------------------------------------------------- */

   void arreter() {
      arreter_car() ;
   } 
   
   /* --------------------------------------------------------------------- */
   void passer_espace()
   {
        while(est_espace(caractere_courant()))
            avancer_car();
   }
   
   /* --------------------------------------------------------------------- */
   void fon_etiq(Etat_Automate *etat)
   {
        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
        strcpy( lexeme_en_cours.chaine_etiquette,lexeme_en_cours.chaine);
        lexeme_en_cours.nature=ETIQUETTE;
        if(caractere_courant()==':')
            *etat = E_FIN; //Si on a ':' on a déjà lu toute l'étiquette
        else
            *etat = E_ETIQUETTE; //Sinon il reste à en lire
        avancer_car ();
        
   }
   void fon_erreur(Etat_Automate *etat)
   {
        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
        lexeme_en_cours.nature=VARIABLE;
        *etat = E_VARIABLE;
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
    lexeme_en_cours.chaine_etiquette[0]='\0';
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
                        etat = E_ENTIERMOINS;
                    }
                    /*Nombre sans # pour la zone data*/
                    else if(est_chiffre(caractere_courant()))
                    {
                        lexeme_en_cours.nature = ENTIER;
                        lexeme_en_cours.ligne = numero_ligne();
                        lexeme_en_cours.colonne = numero_colonne();
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        lexeme_en_cours.valeur = caractere_courant() - '0';
                        etat = E_ENTIER;
                    }
                    else if(caractere_courant()=='-')
                    {
                        lexeme_en_cours.nature = ENTIER;
                        lexeme_en_cours.ligne = numero_ligne();
                        lexeme_en_cours.colonne = numero_colonne();
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        lexeme_en_cours.valeur = 0;
                        etat = E_ENTIER_NEGATIF;
                    }
                    
                    else if( est_caractere(caractere_courant()) )
                    {
                        lexeme_en_cours.ligne = numero_ligne();
                        lexeme_en_cours.colonne = numero_colonne();
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        
                        switch(caractere_courant())
                        {
                            
                            case '@':
                                lexeme_en_cours.nature=COMMENTAIRE;
                                etat=E_COMMENTAIRE;
                                break;
                                
                            case '.':
                                etat=E_POINT;
                                break;
                            
                            case '\n':
                                lexeme_en_cours.nature=RETOUR_LIGNE;
                                etat=E_FIN;
                                break;
                                
                            case '[':
                                lexeme_en_cours.nature=CROCHET_OUVRANT;
                                etat=E_FIN;
                                break;
                                
                            case ']':
                                lexeme_en_cours.nature=CROCHET_FERMANT;
                                etat=E_FIN;
                                break;
                            
                            case 'A':
                            case 'a':
                                lexeme_en_cours.nature=ADD;
                                etat=E_A;
                                break;
                                
                            case 'F':
                            case 'f':
                                lexeme_en_cours.nature=REG;
                                etat=E_F;
                                break;
                                
                            case 'B':
                            case 'b':
                                lexeme_en_cours.nature=BAL;
                                etat=E_B;
                                break;
                                
                            case 'E':
                            case 'e':
                                lexeme_en_cours.nature=EXIT;
                                etat=E_E;
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
                                lexeme_en_cours.nature=REG;
                                etat=E_REGISTRE;
                                break;
                                
                            case 'C':
                            case 'c':
                                lexeme_en_cours.nature=CMP;
                                etat=E_C;
                                break;
                                
                            case 'L':
                            case 'l':
                                lexeme_en_cours.nature=LDR;
                                etat=E_L;
                                break;
                                
                            case 'o':
                            case 'O':
                                lexeme_en_cours.nature=ORR;
                                etat=E_O;
                                break;
                                
                            case 't':
                            case 'T':
                                lexeme_en_cours.nature=TST;
                                etat=E_T;
                                break;
                                
                            case 'p':
                            case 'P':
                                lexeme_en_cours.nature=REG;
                                etat=E_P;
                                break;
                                
                            case '"':
                                lexeme_en_cours.nature=CHAINE_CARACTERE;
                                etat=E_CHAINE;
                                break;
                                
                            case '\'':
                                lexeme_en_cours.nature=ENTIER;
                                etat=E_APO;
                                break;
    
                            default:
                                ajouter_caractere (lexeme_en_cours.chaine_etiquette, caractere_courant()) ;
                                lexeme_en_cours.nature=ETIQUETTE;
                                etat=E_ETIQUETTE;
                                break;
                        }
                    }
                }
                                       
                avancer_car ();
                break;
                
            case E_APO:
                if (caractere_courant()=='\\') //Lecture de 
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat=E_APOCAR;
                    lexeme_en_cours.nature = ENTIER;
                    avancer_car ();
                    
                    if (caractere_courant()=='n')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        lexeme_en_cours.valeur = '\n';
                        avancer_car ();
                    }
                    else if(caractere_courant()=='\\')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        lexeme_en_cours.valeur = '\\';
                        avancer_car ();
                    }
                    else if(caractere_courant()=='\'')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        lexeme_en_cours.valeur = '\'';
                        avancer_car ();
                    }
                    else if(caractere_courant()=='\t')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        lexeme_en_cours.valeur = '\t';
                        avancer_car ();
                    }
                    else if(caractere_courant()=='\0')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        lexeme_en_cours.valeur = '\0';
                        avancer_car ();
                    }
                }
               // else if (caractere_courant()<127 && caractere_courant()>0)
               else if(1<2)
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat=E_APOCAR;
                    lexeme_en_cours.nature = ENTIER;
                    lexeme_en_cours.valeur = caractere_courant();
                    avancer_car ();
                }
                
                else
                {
                    etat=E_ERREUR;
                }
                break;
                
            case E_APOCAR:
                if (caractere_courant()=='\'')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat=E_FIN_COMMANDE;
                    avancer_car ();
                }
                else
                {
                    etat=E_ERREUR;
                }
                break;
                
                
            case E_ENTIERMOINS:
                if (est_chiffre(caractere_courant())) 
                {
                    etat=E_ENTIER;
                }
                else if (caractere_courant()=='-')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat=E_ENTIER_NEGATIF;
                    avancer_car ();
                }
                else
                {
                    etat=E_ERREUR;
                }
                break;
                
             case E_ENTIER_NEGATIF:
                if (est_chiffre(caractere_courant())) 
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    lexeme_en_cours.valeur = lexeme_en_cours.valeur * 10 - (caractere_courant() - '0');
                    avancer_car ();
                } 
                else
                {
                    etat=E_FIN;
                }
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
                
    
                
            case E_FIN_COMMANDE:
                if ( est_espace(caractere_courant()) || est_retour_ligne(caractere_courant()) || caractere_courant()==','   )
                {
                    etat = E_FIN;
                    passer_espace();
                    if(caractere_courant()==':')
                    {
                        etat=E_ERREUR;
                        lexeme_en_cours.nature=ERREUR;
                    }
                        
                }
                else
                {
                    if(caractere_courant()==':')
                    {
                        lexeme_en_cours.nature=ERREUR;
                        etat=E_ERREUR;
                    }
                    else
                        fon_etiq(&etat);
                        
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
                    fon_etiq(&etat);
                }
                break;
                
            case E_MO:
                if (caractere_courant()=='v' || caractere_courant()=='V')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_FIN_COMMANDE;
                    avancer_car ();
                }
                else
                {
                    fon_etiq(&etat);
                }
                break;
                
            /*LECTURE DE AND*/
                
            case E_AN:
                if (caractere_courant()=='d' || caractere_courant()=='D')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_FIN_COMMANDE;
                    lexeme_en_cours.nature=AND;
                    avancer_car ();
                }
                else
                {
                    fon_etiq(&etat);
                }
                break;
            
            /*LECTURE DE ORR*/
        
            case E_O:
                if (caractere_courant()=='r' || caractere_courant()=='R')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_OR;
                    avancer_car ();
                }
                else
                {
                    fon_etiq(&etat);
                }
                break;
                
                
            case E_OR:
                if (caractere_courant()=='r' || caractere_courant()=='R')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_FIN_COMMANDE;
                    avancer_car ();
                }
                else
                {
                    fon_etiq(&etat);
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
                else if (caractere_courant()=='n' || caractere_courant()=='N')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_AN;
                    avancer_car ();
                }
                else if (caractere_courant()=='f' || caractere_courant()=='F')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_AF;
                    avancer_car ();
                }
                else
                {
                    fon_etiq(&etat);
                }
                break;
                
            case E_AD:
                if (caractere_courant()=='d' || caractere_courant()=='D')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_FIN_COMMANDE;
                    avancer_car ();
                }
                else
                {
                    fon_etiq(&etat);
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
                else if(caractere_courant()=='p' || caractere_courant()=='P')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_FIN_INSTRUCTION;
                    lexeme_en_cours.nature=REG;
                    lexeme_en_cours.numero_registre=13;
                    avancer_car ();
                }
                else
                {
                    fon_etiq(&etat);
                }
                break;
                
            case E_SU:
                if (caractere_courant()=='b' || caractere_courant()=='B')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_FIN_COMMANDE;
                    avancer_car ();
                }
                else
                {
                    fon_etiq(&etat);
                }
                break;
                
                
                
                
                /*LECTURE DE TST*/
            
            case E_T:
                if (caractere_courant()=='s' || caractere_courant()=='S')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_TS;
                    avancer_car ();
                }
                else
                {
                    fon_etiq(&etat);
                }
                break;
                
            case E_TS:
                if (caractere_courant()=='t' || caractere_courant()=='T')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_FIN_COMMANDE;
                    avancer_car ();
                }
                else
                {
                    fon_etiq(&etat);
                }
                break;

            
                /*LECTURE DE STR*/
                
                case E_ST:
                    if (caractere_courant()=='r' || caractere_courant()=='R')
                    {
                        lexeme_en_cours.nature=STR;
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN_COMMANDE;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
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
                    else if (caractere_courant()=='r' || caractere_courant()=='R')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        lexeme_en_cours.nature=REG;
                        lexeme_en_cours.numero_registre=14;
                        etat = E_FIN_COMMANDE;
                        avancer_car ();
                    }
                    else if (caractere_courant()=='s' || caractere_courant()=='S')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_LS;
                        avancer_car ();
                    }
                    
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                
                case E_LD:
                    if (caractere_courant()=='r' || caractere_courant()=='R')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN_COMMANDE;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                
                
                /*LECTURE DE LSL LSR*/
                
                case E_LS:
                    if (caractere_courant()=='l' || caractere_courant()=='L')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN_COMMANDE;
                        lexeme_en_cours.nature=LSL;
                        avancer_car ();
                    }
                    else if (caractere_courant()=='r' || caractere_courant()=='R')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN_COMMANDE;
                        lexeme_en_cours.nature=LSR;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                
                
                    
                    

                /*LECTURE DE CMP*/
                
                case E_C:
                    if (caractere_courant()=='m' || caractere_courant()=='M')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_CM;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                    
                    case E_CM:
                    if (caractere_courant()=='p' || caractere_courant()=='P')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN_COMMANDE;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                

                
                
                /*LECTURE DE BAL*/
                
                case E_B:
                    if (caractere_courant()=='a' || caractere_courant()=='A')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_BA;
                        avancer_car ();
                    }
                    else if (caractere_courant()=='e' || caractere_courant()=='E')
                    {
                        lexeme_en_cours.nature=BEQ;
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_BE;
                        avancer_car ();   
                    }
                    else if (caractere_courant()=='n' || caractere_courant()=='N')
                    {
                        lexeme_en_cours.nature=BNE;
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_BN;
                        avancer_car ();   
                    }
                    else if (caractere_courant()=='g' || caractere_courant()=='G')
                    {
                        lexeme_en_cours.nature=BGT;
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_BG;
                        avancer_car ();   
                    }
                    else if (caractere_courant()=='l' || caractere_courant()=='L')
                    {
                        lexeme_en_cours.nature=BLT;
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_BL;
                        avancer_car ();   
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                
                case E_BA:
                    if (caractere_courant()=='l' || caractere_courant()=='L')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN_COMMANDE;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;

                
                /*LECTURE DE BEQ*/
                
                case E_BE:
                    if (caractere_courant()=='q' || caractere_courant()=='Q')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN_COMMANDE;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                

                
                
                /*LECTURE DE BNE*/
                
                case E_BN:
                    if (caractere_courant()=='e' || caractere_courant()=='E')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN_COMMANDE;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                

                
                /*LECTURE DE BGT*/
                
                case E_BG:
                    if (caractere_courant()=='t' || caractere_courant()=='T')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN_COMMANDE;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                

                
                
                /*LECTURE DE BLT et  BL*/
                
                case E_BL:
                    if (caractere_courant()=='t' || caractere_courant()=='T')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN_COMMANDE;
                        avancer_car ();
                    }
                    else if(est_espace(caractere_courant()))
                    {
                        etat = E_FIN;
                        lexeme_en_cours.nature=BL;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                   
                    /*LECTURE DE AFFRN et  AFFRC*/
                    
                case E_AF:
                    if (caractere_courant()=='f' || caractere_courant()=='F')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_AFF;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                
                case E_AFF:
                    if (caractere_courant()=='r' || caractere_courant()=='R')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_AFFR;
                        avancer_car ();
                    }
                    else if (caractere_courant()=='a' || caractere_courant()=='A')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_AFFA;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                    
                    
                case E_AFFA:
                    if (caractere_courant()=='c' || caractere_courant()=='C')
                    {
                        lexeme_en_cours.nature=AFFAC;
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN_COMMANDE;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                    
                    
                case E_AFFR:
                    if (caractere_courant()=='n' || caractere_courant()=='N')
                    {
                        lexeme_en_cours.nature=AFFRN;
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN_COMMANDE;
                        avancer_car ();
                    }
                    else if (caractere_courant()=='c' || caractere_courant()=='C')
                    {
                        lexeme_en_cours.nature=AFFRC;
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN_COMMANDE;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                    
                    
                /*LECTURE DE EXIT*/
                
                case E_E:
                    if (caractere_courant()=='x' || caractere_courant()=='X')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_EX;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                
                case E_EX:
                    if (caractere_courant()=='i' || caractere_courant()=='I')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_EXI;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                    
                case E_EXI:
                    if (caractere_courant()=='t' || caractere_courant()=='T')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN_COMMANDE;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                /*LECTURE DE .DATA*/
                
                case E_POINT:
                    if (caractere_courant()=='d' || caractere_courant()=='D')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_POINTD;
                        avancer_car ();
                    }
                    else if (caractere_courant()=='t' || caractere_courant()=='T')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_POINTT;
                        avancer_car ();
                    }
                    else if (caractere_courant()=='s' || caractere_courant()=='S')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_POINTS;
                        avancer_car ();
                    }
                    else if (caractere_courant()=='a' || caractere_courant()=='A')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_POINTA;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                    
                case E_POINTD:
                    if (caractere_courant()=='a' || caractere_courant()=='A')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_POINTDA;
                        avancer_car ();
                    }
                    else if (caractere_courant()=='w' || caractere_courant()=='W')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_POINTDW;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                    
                case E_POINTDA:
                    if (caractere_courant()=='t' || caractere_courant()=='T')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_POINTDAT;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                    
                case E_POINTDAT:
                    if (caractere_courant()=='a' || caractere_courant()=='A')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN_COMMANDE;
                        avancer_car ();
                        lexeme_en_cours.nature=DATA;
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                /*LECTURE DE .TEXT*/
                case E_POINTT:
                    if (caractere_courant()=='e' || caractere_courant()=='E')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_POINTTE;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                
                case E_POINTTE:
                    if (caractere_courant()=='x' || caractere_courant()=='X')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_POINTTEX;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                    
                case E_POINTTEX:
                    if (caractere_courant()=='t' || caractere_courant()=='T')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN_COMMANDE;
                        avancer_car ();
                        lexeme_en_cours.nature=TEXT;
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                    
                /*LECTURE DE .DWORD*/
                case E_POINTDW:
                    if (caractere_courant()=='o' || caractere_courant()=='O')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_POINTDWO;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                
                case E_POINTDWO:
                    if (caractere_courant()=='r' || caractere_courant()=='R')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_POINTDWOR;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                    
                case E_POINTDWOR:
                    if (caractere_courant()=='d' || caractere_courant()=='D')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN_COMMANDE;
                        avancer_car ();
                        lexeme_en_cours.nature=DWORD;
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;

                
                /*LECTURE DE .SKIP*/
                case E_POINTS:
                    if (caractere_courant()=='k' || caractere_courant()=='K')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_POINTSK;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                    
                case E_POINTSK:
                    if (caractere_courant()=='i' || caractere_courant()=='I')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_POINTSKI;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                    
                case E_POINTSKI:
                    if (caractere_courant()=='p' || caractere_courant()=='P')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        lexeme_en_cours.nature=SKIP;
                        etat = E_FIN_COMMANDE;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                    
                case E_POINTA:
                    if (caractere_courant()=='s' || caractere_courant()=='S')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_POINTAS;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                    
                case E_POINTAS:
                    if (caractere_courant()=='c' || caractere_courant()=='C')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_POINTASC;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                    
                case E_POINTASC:
                    if (caractere_courant()=='i' || caractere_courant()=='I')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_POINTASCI;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
                    }
                    break;
                    
                case E_POINTASCI:
                    if (caractere_courant()=='i' || caractere_courant()=='I')
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN_COMMANDE;
                        lexeme_en_cours.nature=ASCII;
                        avancer_car ();
                    }
                    else
                    {
                        fon_etiq(&etat);
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
                            lexeme_en_cours.numero_registre=0;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '1':
                            lexeme_en_cours.numero_registre=1;
                            etat = E_REGISTRE2;
                            avancer_car();
                            break;
                        
                        case '2':
                            lexeme_en_cours.numero_registre=2;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '3':
                            lexeme_en_cours.numero_registre=3;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '4':
                            lexeme_en_cours.numero_registre=4;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '5':
                            lexeme_en_cours.numero_registre=5;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                        
                        case '6':
                            lexeme_en_cours.numero_registre=6;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '7':
                            lexeme_en_cours.numero_registre=7;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '8':
                            lexeme_en_cours.numero_registre=8;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                    
                        case '9':
                            lexeme_en_cours.numero_registre=9;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                    }
                }
                else
                {
                    fon_etiq(&etat);
                }
                break;
            //Registre à 2 chiffres
            case E_REGISTRE2:
                if (est_chiffre(caractere_courant())  &&  caractere_courant()<='6')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    switch (caractere_courant())
                    {
                        case '0':
                            etat = E_FIN_INSTRUCTION;
                            lexeme_en_cours.numero_registre=10;
                            avancer_car();
                            break;
                            
                        case '1':
                            lexeme_en_cours.numero_registre=11;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                        
                        case '2':
                            lexeme_en_cours.numero_registre=12;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '3':
                            lexeme_en_cours.numero_registre=13;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '4':
                            lexeme_en_cours.numero_registre=14;
                            etat = E_FIN_INSTRUCTION;
                            avancer_car();
                            break;
                            
                        case '5':
                            lexeme_en_cours.numero_registre=15;
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
                break;
                
            /*LECTURE DE PC*/
            case E_P:
                if (caractere_courant()=='c' || caractere_courant()=='C')
                {
                    lexeme_en_cours.numero_registre=15;
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_FIN_INSTRUCTION;
                    avancer_car ();
                }
                else
                {
                    fon_etiq(&etat);
                }
                break;
                    
                    
            /*LECTURE DE FP*/
            case E_F:
                if (caractere_courant()=='p' || caractere_courant()=='P')
                {
                    lexeme_en_cours.nature=REG;
                    lexeme_en_cours.numero_registre=11;
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_FIN_INSTRUCTION;
                    avancer_car ();
                }
                else
                {
                    fon_etiq(&etat);
                }
                break;
            
            case E_FIN_INSTRUCTION:
                if( caractere_courant()==',' || caractere_courant()==']' || est_espace(caractere_courant()) || est_retour_ligne(caractere_courant())   )
                {
                    etat=E_FIN;
                }
                else
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat = E_ERREUR;
                    lexeme_en_cours.nature=ERREUR;
                }
                break;
            
                /*ETIQUETTE*/
                
            case E_ETIQUETTE:
                if (caractere_courant()==':')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat=E_FIN;
                    avancer_car();
                }
                else if(  est_retour_ligne(caractere_courant())  )
                {
                    etat = E_FIN;
                    lexeme_en_cours.nature=VARIABLE;
                }
                else if(  est_caractere(caractere_courant())  )
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    ajouter_caractere (lexeme_en_cours.chaine_etiquette, caractere_courant()) ;
                    avancer_car();
                }
                else if (est_espace(caractere_courant()))
                {
                    etat=E_ETIQUETTE_2;
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    avancer_car();
                }
                else 
                {
                    etat = E_ERREUR;
                    lexeme_en_cours.nature=ERREUR;
                }
                
                break;
                
            case E_ETIQUETTE_2:
                if (caractere_courant()==':')
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    etat=E_FIN;
                    avancer_car();
                }
                else if (est_espace(caractere_courant()))
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    avancer_car();
                }
                else 
                {
                    etat = E_FIN;
                    lexeme_en_cours.nature=VARIABLE;
                }
                break;
                
            case E_COMMENTAIRE:
                while(!est_retour_ligne(caractere_courant()))
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    avancer_car();
                }
                etat=E_FIN;
                break;
                
            case E_CHAINE:
                while((caractere_courant()!='"'))
                {
                    if (caractere_courant()=='\\')
                    {
                        avancer_car();
                        if (caractere_courant()=='n')
                        {
                            ajouter_caractere (lexeme_en_cours.chaine, '\n') ;
                            avancer_car ();
                        }
                        else if(caractere_courant()=='\\')
                        {
                            ajouter_caractere (lexeme_en_cours.chaine, '\\') ;
                            avancer_car ();
                        }
                        else if(caractere_courant()=='\'')
                        {
                            ajouter_caractere (lexeme_en_cours.chaine, '\'') ;
                            avancer_car ();
                        }
                        else if(caractere_courant()=='t')
                        {
                            ajouter_caractere (lexeme_en_cours.chaine, '\t') ;
                            avancer_car ();
                        }
                        else if(caractere_courant()=='\"')
                        {
                            ajouter_caractere (lexeme_en_cours.chaine, '\"') ;
                            avancer_car ();
                        }
                        else if(caractere_courant()=='\0')
                        {
                            ajouter_caractere (lexeme_en_cours.chaine, '\0') ;
                            avancer_car ();
                        }
                    }
                    else
                    {
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        avancer_car();
                    }
                    
                }
                ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                avancer_car();
                etat=E_FIN;
                break;
                
            case E_VARIABLE:
                if(  est_caractere(caractere_courant())  )
                {
                    ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                    ajouter_caractere (lexeme_en_cours.chaine_etiquette, caractere_courant()) ;
                    avancer_car();
                }
                else if (est_espace(caractere_courant()) || est_retour_ligne(caractere_courant()) || fin_de_sequence_car() )
                {
                    etat=E_FIN;
                }
                
            case E_ERREUR:
                if ( !est_espace(caractere_courant()) && !est_retour_ligne(caractere_courant()) && !fin_de_sequence_car()  )
                {
                    lexeme_en_cours.nature=ERREUR;
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




long int est_caractere(char c)
{
    
    return c>=0 && c<=127 && c!=' ' && c!='\t';
}

long int est_espace(char c)
{
    return c=='\t' || c==' ';
}

long int est_retour_ligne(char c)
{
    return c=='\n';
}

long int est_chiffre(char c) 
{
      return (c >= '0') && (c <= '9') ;
}

void ajouter_caractere (char *s, char c) 
{
	long int l ;

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
        case CMP: return "CMP" ;
        case REG: return "REG" ;
        case BAL: return "BAL" ;
        case BEQ: return "BEQ" ;
        case BNE: return "BNE" ;
        case BLT: return "BLT" ;
        case BGT: return "BGT" ;
        case LSL: return "LSL" ;
        case LSR: return "LSR" ;
        case ORR: return "ORR" ;
        case AND: return "AND" ;
        case TST: return "TST" ;
        case BL: return "BL" ;
        case SKIP: return ".SKIP" ;
        case DATA: return ".DATA";
        case TEXT: return ".TEXT";
        case ASCII: return ".ASCII";
        case EXIT: return "EXIT";
        case DWORD: return "DWORD";
        case COMMENTAIRE: return "COMMENTAIRE" ;
        case CHAINE_CARACTERE: return "CHAINE_CARACTERE";
        case ETIQUETTE: return "ETIQUETTE" ;
        case VARIABLE: return "VARIABLE";
        case ENTIER: return "ENTIER" ;
        case VIRGULE: return "VIRGULE" ;
        case CROCHET_OUVRANT: return "CROCHET_OUVRANT";
        case CROCHET_FERMANT: return "CROCHET_FERMANT";
        case AFFRN: return "AFFRN" ;
        case AFFRC: return "AFFRC" ;
        case AFFAC: return "AFFAC" ;
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
            printf("(ligne %ld, ", l.ligne);
            printf("colonne %ld) : ",l.colonne);
            printf("[") ;
            printf("nature = %s", Nature_vers_Chaine(l.nature)) ;
            printf(", chaine = %s, ", l.chaine) ;
            if(l.nature==ERREUR)
                blanc();
            switch(l.nature) 
            {
                case ENTIER:
                    printf("valeur = %ld", l.valeur);
                    break;
                case REG:
                    printf("numéro registre = %ld",l.numero_registre);
                    break;
                case ETIQUETTE:
                    printf("nom étiquette = %s",l.chaine_etiquette);
                    break;
                case VARIABLE:
                    printf("nom variable = %s",l.chaine_etiquette);
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
void afficher_erreur_lexeme()
{
    lexeme l;
    while (! fin_de_sequence())
    {
        printf("re");
        l=lexeme_courant();
        switch(l.nature)
        {
            case ERREUR:
                rouge_gras();
                printf("(ligne %ld, ", l.ligne);
                printf("colonne %ld) : ",l.colonne);
                printf("[") ;
                printf("nature = %s", Nature_vers_Chaine(l.nature)) ;
                printf(", chaine = %s, ", l.chaine) ;
                printf("]") ;
                break;
            default:
                break;
        }
        avancer();
    }
}
