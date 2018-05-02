.data
.text

@Calucle le n ième terme de fibonacci et le stock dans le registre r0

mov r1,#0
mov r2,#0
mov r3,#1
test: cmp r1,#10 @Tant que r1<10
beq fin_boucle

mov r4,r3
add r3,r2
mov r2,r4

add r1,#1

bal test
fin_boucle:

mov r0,r4


bal exit


