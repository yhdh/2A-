# question 4 : Proposer un programme qui affiche n étoiles puis un retour à la ligne, où n est le
# contenu du registre a0.
.data
etoile: .asciz "*"      # caractère étoile
espace: .asciz "\n"     # retour à la ligne

.text
.globl __start
__start:
    # Utiliser t0 comme compteur n. Initialiser avec la valeur de a0.
    # Pour l'exemple, on peut mettre 5 dans a0 au début.
    #li a0, 5
    mv t0, a0           # t0 = n (le compteur)

    # Si n=0, ne rien faire et aller directement à la fin
    beq t0, x0, print_newline

loop:
    # Afficher une étoile
    la a1, etoile       # a1 = adresse de la chaîne "*"
    li a0, 4            # a0 = code service "print string" (pour Jupiter)
    ecall

    addi t0, t0, -1     # Décrémenter le compteur n
    bnez t0, loop       # Si n != 0, continuer la boucle

print_newline:
    # Afficher le retour à la ligne
    la a1, espace       # a1 = adresse de "\n"
    li a0, 4            # a0 = code service "print string"
    ecall

end:
    # Terminer le programme
    li a0, 10           # a0 = code service "exit"
    ecall
