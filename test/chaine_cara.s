.data
chaine: .ascii "Vive le cours d'INF 404!  \n"
chaine2: .ascii "é à ç  \n"
.text

@Affichage par adresse
ldr r2,ptr_chaine
affac r2

@Affichage en récupérant chaque caractère
ldr r0,ptr_chaine
ldr r1,[r0]
deb: cmp r1,#0 @Tant qu'on est pas à la fin de la chaine  '\0'
    beq fin
    affrc r1
    add r0,r0,#1 @Avance d'un caractère dans la chaine
    ldr r1,[r0]
    
    bal deb
fin:

ldr r2,ptr_chaine2
affac r2

mov r3,'a'

bal exit

ptr_chaine: .dword chaine
ptr_chaine2: .dword chaine2
