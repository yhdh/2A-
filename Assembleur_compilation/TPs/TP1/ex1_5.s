# question 5 : Transformer le programme précédent en une fonction.
# La fonction `afficher_etoiles` reçoit le nombre d'étoiles `n` dans a0.
# Elle préserve les registres qu'elle modifie (t0, t1, ra).

.data
etoile: .asciz "*"
newline: .asciz "\n"

.text
.globl __start

# Programme principal pour tester la fonction
__start:
    # Appeler la fonction pour afficher 9 étoiles
    li a0, 9
    jal ra, afficher_etoiles

    # Terminer le programme
    li a0, 10           # Code de l'appel système "exit" pour Jupiter
    ecall

# -----------------------------------------------------------
# Fonction: afficher_etoiles
# Affiche n étoiles suivies d'un retour à la ligne.
# Argument: a0 = nombre d'étoiles (n)
# Préserve les registres : t0, t1, ra
# -----------------------------------------------------------
afficher_etoiles:
    # 1. Prologue : Sauvegarder les registres sur la pile
    addi sp, sp, -12      # Allouer de l'espace sur la pile pour 3 registres
    sw   ra, 8(sp)        # Sauvegarder l'adresse de retour (ra)
    sw   t0, 4(sp)        # Sauvegarder le registre temporaire t0 (compteur)
    sw   t1, 0(sp)        # Sauvegarder le registre temporaire t1 (utilisé pour l'appel système)

    # Corps de la fonction
    mv   t0, a0           # t0 = n (notre compteur)
    beq  t0, x0, fin_boucle # Si n=0, sauter la boucle d'affichage

boucle_etoiles:
    # Afficher une étoile
    la   a1, etoile       # Charger l'adresse de "*" dans a1
    li   a0, 4            # Code "print string" pour Jupiter
    ecall

    addi t0, t0, -1       # Décrémenter le compteur
    bnez t0, boucle_etoiles # Continuer si le compteur n'est pas zéro

fin_boucle:
    # Afficher le retour à la ligne
    la   a1, newline      # Charger l'adresse de "\n" dans a1
    li   a0, 4            # Code "print string"
    ecall

    # 2. Épilogue : Restaurer les registres depuis la pile
    lw   t1, 0(sp)        # Restaurer t1
    lw   t0, 4(sp)        # Restaurer t0
    lw   ra, 8(sp)        # Restaurer l'adresse de retour
    addi sp, sp, 12       # Libérer l'espace sur la pile

    # 3. Retourner à l'appelant
    ret                   # Pseudo-instruction pour jr ra
