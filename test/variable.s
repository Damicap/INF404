.data
x: .dword 12
.dword 13
y: .dword -12
z:.dword 5487
.text

ldr r1,ptr_y
ldr r2,[r1]

ldr r3,ptr_x
ldr r4,[r3]

add r3,#1
ldr r5,[r3]

bal exit
ptr_y: .dword y
ptr_x: .dword x

