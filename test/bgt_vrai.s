.data
.text
    mov r1,#10
    cmp r1,#2
    bgt vrai
faux:
    mov r2,#1
    bal fin_si
vrai:
    mov r3,#1
fin_si:


bal exit


