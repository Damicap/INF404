
@Multiplication de 2 nombres par additions 
.data
.text

main:

    mov r0,#40321000  @ Mettre le plus petit nombre ici
    mov r1,#15
    bl mul
    affrn r0
    mov r0,'\n'
    affrc r0
    bal exit


@Prend le registre r0,r1 en paramètre et retourne la valeur dans r0
mul:
    mov r2,#0
    mov r3,#0
    multiplication:cmp r2, r0   
    beq fin_multiplication 
    add r3,r1
    add r2,#1 @Incrémente le compteur
    bal multiplication
    fin_multiplication:
    mov r0,r3
    mov pc, lr
