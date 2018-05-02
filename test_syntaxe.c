#include <stdio.h>

#include "analyse_syntaxique.h"
#include "analyse_lexicale.h"

int main (int argc, char *argv[]) {
   if (argc >= 1) {
   	demarrer (argv[1]) ;
   } else {
	printf("Il faut donner un nom de fichier en argument !") ;
	return 1 ;
   } 
   analyse_syntaxique(argv[1]);
   return 0 ;
}
