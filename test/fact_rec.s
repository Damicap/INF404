
    @fact(n):
    @si n=1 retourne 1
    @sinon retourne n*fact(n-1)

.data
val_fact: .dword 0
rep: .ascii "La factorielle de "
rep2: .ascii " est: "
.text

main:
    mov r0,#9 @Factorielle
 
    sub sp,#1
    str r0,[sp] @Le paramètre
    sub sp,#1 @La case retour
    bl fact

    ldr r1,adr_rep @Affichage du résultat
    affac r1
    affrn r0
    ldr r1,adr_rep2
    affac r1
    ldr r0,[sp]
    affrn r0
    mov r8,'\n'
    affrc r8

bal exit

fact: 
    
    sub sp,#1
    str lr,[sp]
    sub sp,#1
    str fp,[sp]
    mov fp,sp
    
    
    @Stocker les variables
    sub sp,#1
    str r0,[sp]
    sub sp,#1
    str r1,[sp]
    sub sp,#1
    str r2,[sp]
    sub sp,#1
    str r3,[sp]
    sub sp,#1
    str r4,[sp]
    sub sp,#1
    str r5,[sp]
    
    
    ldr r0,[fp,#3] @Récupère le nombre passé en paramètre

    
si: cmp r0,#1 @Si égale à 1 on retourne 1
    bne diff_1

    str r0,[fp,#2] @On stocke 1 dans l'adresse prévue retourne 1
    bal fin_si
    
diff_1:
    sub sp,#1 @On stocke les paramètres
    sub r1,r0,#1 @ n-1
    str r1,[sp]
    sub sp,#1 @On laisse une case vide pour le résultat
    bl fact @fact(n-1)
        
    ldr r2,[sp]
    add sp,#2
    mov r3,#0
    mov r4,#0
    multiplication:cmp r3, r0   @mul n*fact(n-1)  
    beq fin_multiplication @Si r3=r2 alors on a fini la multiplication
    add r4,r2
    add r3,r3,#1 @Incrémente le compteur
    bal multiplication
    fin_multiplication:
    mov r0,r4
    str r0,[fp,#2] @n*fact(n-1)
    

    
  fin_si:
    ldr r5,[sp]
    add sp,#1
    ldr r4,[sp]
    add sp,#1
    ldr r3,[sp]
    add sp,#1
    ldr r2,[sp]
    add sp,#1
    ldr r1,[sp]
    add sp,#1
    ldr r0,[sp]
    add sp,#1
    
    
    ldr fp,[sp]
    add sp,#1
    ldr lr,[sp]
    add sp,#1
    mov pc,lr


adr_val_fact: .dword val_fact
adr_rep: .dword rep
adr_rep2: .dword rep2

