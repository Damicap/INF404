
//Faire un REG

typedef enum{
    MOV,
    ADD,
    SUB,
    CMP,
    REG,
    STR,
    LDR,
    BAL,
    BEQ,
    BNE,
    BGT,
    BLT,
    BL,
    LSL,
    LSR,
    AND,
    ORR,
    TST,
    SKIP,
    DATA,
    TEXT,
    EXIT,
    DWORD,
    ASCII,
    CHAINE_CARACTERE,
    COMMENTAIRE,
    ETIQUETTE, //Etiquette debut_boucle : mv ...
    VARIABLE, //Exemple bl deb_boucle  
    ENTIER,
    RETOUR_LIGNE,
    VIRGULE,
    FIN_SEQUENCE,     // pseudo lexeme ajoute en fin de sequence
    ERREUR,
    CROCHET_OUVRANT,
    CROCHET_FERMANT,
    AFFRN, //Afficher la valeur du registre sous forme de nombre
    AFFRC, //Afficher la valeur du registre sous forme de caractère
    AFFAC, //Afficher la chaine de caractère stockée à l'adresse
} nature_lexeme;


typedef struct{
    
    nature_lexeme nature ; //Nature du lexème
    char chaine[64];   
    char chaine_etiquette[32];
    long int valeur;   //Valeur immédiate 
    long int numero_registre; //Valeur du registre
    long int ligne;
    long int colonne;
    
} lexeme;

   void afficher(lexeme l);

   void afficher_erreur_lexeme();
   
   void demarrer(char *nom_fichier);
   // e.i. : indifferent
   // e.f. : la sequence de lexemes est lue dans le fichier designe par
   //        nom_fichier
   //        fin_de_sequence <=> lexeme_courant.nature = FIN_SEQUENCE
   //        (non fin_de_sequence) => lexeme courant est le premier
   //        lexeme de la sequence
   //        l'exception Erreur_Lexicale est levee en cas d'erreur

   void avancer();
   // pre-condition : la machine sequentielle est demarree
   // e.i. : on pose i = rang du lexeme_courant :
   //        (non fin_de_sequence)
   //        et (non lexeme_courant.nature = FIN_SEQUENCE)
   // e.f. : fin_de_sequence <=> lexeme_courant.nature = FIN_SEQUENCE
   //        (non fin_de_sequence) => lexeme_courant est le lexeme i+1
   //        l'exception Erreur_Lexicale est levee en cas d'erreur

   lexeme lexeme_courant();
   // pre-condition : la machine sequentielle est demarree
   // lexeme_courant est :
   // . le pseudo lexeme FIN_SEQUENCE si fin_de_sequence
   // . le pseudo lexeme ERREUR si une erreur a ete detectee
   // . le lexeme de rang i sinon

   long int fin_de_sequence();
   // pre-condition : la machine sequentielle est demarree
   // fin_de_sequence vaut :
   // . vrai si tous les lexemes de la sequence ont ete reconnus
   // . faux sinon

   void arreter();
   // e.i. : la machine sequentielle est demarree
   // e.f. : la machine sequentielle est arretee

   char *Nature_vers_Chaine (nature_lexeme nature);
