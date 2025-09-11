    .data
detail: .asciz "*"      # caractère étoile
espace: .asciz "\n"     # retour à la ligne

    .text
    .globl __start
__start:
    # Exemple : initialiser a0 = 5 (nombre de lignes)
    li t0, 5           # tu peux aussi mettre une valeur directement dans a0 si tu veux
    beq t0, x0, end

loopdec:               # boucle externe (pour chaque ligne)
    mv t1, t0          # copie du compteur de la ligne
loop2:                 # boucle interne (impression d'étoiles)
    la a1, detail      # a1 = adresse de "*"
    li a0, 4           # code service : print string
    ecall

    addi t1, t1, -1    # décrémente t1
    bnez t1, loop2     # si t1 != 0, continuer la ligne

    # fin de la ligne → retour à la ligne
    la a1, espace
    li a0, 4
    ecall

    addi t0, t0, -1    # décrémente le compteur de lignes
    bnez t0, loopdec   # continuer si t0 > 0

end:
    li a0, 10          # code service : exit
    ecall
