.data
.text
    mov r1,#15
    mov r4,#0
    mov r5,'\n'
deb_boucle: cmp r1, #1
    beq fin_boucle
    add r4 , #1
    str r1,[r4]
    affrn r1 @Affichage de la valeur
    affrc r5 @Affichage saut de ligne
    TST r1,#1
    bne impair
pair:
    lsr r1,#1 @Divise par 2
    bal deb_boucle
    
impair: @ 3*x+1
    mov r2,r1 
    lsl r1,#1 @Multiplie par 2
    add r1,r1,r2 @Ajoute r1
    add r1,r1,#1 @ + 1
        
    bal deb_boucle
fin_boucle:
affrn r1
affrc r5
bal exit

