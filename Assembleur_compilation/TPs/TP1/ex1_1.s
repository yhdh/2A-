    .data
msg:        .asciz "bonjour \n"     # chaîne terminée par 0

    .text
    .globl __start
__start:
    # Afficher la chaîne pointée par msg
    li  a0, 4            # code service : print string (convention Jupiter)
    la  a1, msg          # a1 = adresse de "msg"
    ecall

    # Quitter proprement
    li  a0, 10           # code service : exit
    ecall
