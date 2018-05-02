#include <stdlib.h>
#include <stdio.h>

#include "analyse_syntaxique.h"
#include "analyse_lexicale.h"


int main (int argc, char *argv[]) 
{
    if (argc >= 2) 
    {
        demarrer (argv[1]) ;
    } 
    else 
    {
        printf("Il faut donner un nom de fichier en argument !") ;
        return 1 ;
    } 
    
    if (argc ==3 && argv[2][0]=='d') //Debug mode 
    {
        
        evaluer(argv[1],1);
    }
    
    else
    {
       
        evaluer(argv[1],0);
    }
   return 0 ;
}
