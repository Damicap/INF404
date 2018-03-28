clang=clang -std=c99 -Wall -Wextra -Wfatal-errors -fdiagnostics-color=auto

executable= test_lexeme

all:$(executable)

clean:
	rm -f  test_lexeme *.o

#****Fichiers objets****#

couleur.o: couleur.c couleur.h
	$(clang) -g  -c couleur.c
	
analyse_lexicale.o: analyse_lexicale.c analyse_lexicale.h lecture_caracteres.h couleur.h
	$(clang) -g  -c analyse_lexicale.c
	
lecture_caracteres.o: lecture_caracteres.h lecture_caracteres.c
	$(clang) -g  -c lecture_caracteres.c
	
test_lexeme.o: test_lexeme.c analyse_lexicale.h
	$(clang) -g  -c test_lexeme.c




#****Exécutable****#

test_lexeme: analyse_lexicale.o  lecture_caracteres.o  test_lexeme.o couleur.o
	$(clang) -g -o test_lexeme analyse_lexicale.o  lecture_caracteres.o  test_lexeme.o couleur.o
