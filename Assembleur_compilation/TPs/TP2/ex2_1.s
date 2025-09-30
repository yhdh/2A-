# question 1 : Écrire en assembleur RISC-V la fonction max :
#function max(n, m : integer) : integer;
#begin
##if n <= m
#then max := m
##else max := n
#end;
.globl __start 
max: # n in a0, m in a1, max in a0 
    ble a0, a1, max_then 
    #mv a0, a0
    ret
max_then:
    ret 

__start: 
    li  a0, 32
    li a1, 56 
    call max 
    mv a1, a0 
    li a0, 1 