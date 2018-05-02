.data
.text

mov r1,#10
cmp r1,#20
bgt vrai @ r1>20 ?
faux:
mov r2,#1
bal fin_si
vrai:
mov r3,#1
fin_si:

bal exit


