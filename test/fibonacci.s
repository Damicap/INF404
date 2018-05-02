.data
tab: .skip 100
.text

ldr r1,adr_tab
mov r2,#0
str r2,[r1 ]
add r1,#1
add r2,#1
str r2,[r1 ]
add r1,#1
mov r6,#2
mov r7,#1

mov r9, '\n' 

mov r8,#2 @Indice
deb_boucle: cmp r8, #94 @Afficher le n premier nombre de fibonacci
    beq fin_boucle
    ldr r3, [r1,#-1]
    ldr r4, [r1,#-2]
    add r4,r3
    str r4, [r1]
    
    mov r9, ':' 
    affrn r8
    affrc r9
    affrn r4
    mov r9, '\n' 
    affrc r9
    
    add r1,#1
    add r8,#1
    bal deb_boucle
fin_boucle:
bal exit

adr_tab: .dword tab

