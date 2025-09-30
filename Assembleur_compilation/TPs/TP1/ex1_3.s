# question 3 : Proposer un programme qui, étant donnés des entiers contenus dans les registres
# a0, a1 et a2, teste s’ils peuvent représenter les longueurs des côtés d’un triangle
# rectangle.
#
# Logique optimisée :
# Si a^2 + b^2 = c^2, alors a^2 + b^2 + c^2 = 2 * c^2.
# On calcule la somme des trois carrés. Ensuite, on vérifie si le double du
# carré de l'un des côtés est égal à cette somme totale.
#
# Entrées : a0, a1, a2 (longueurs)
# Sortie : a0 (1 si rectangle, 0 sinon)
.data
oui:   .asciz "OUI\n"
non:   .asciz "NON\n"

.text
.globl __start
__start:
    # Calculer les carrés des trois côtés
    mul t0, a0, a0      # t0 = a0*a0
    mul t1, a1, a1      # t1 = a1*a1
    mul t2, a2, a2      # t2 = a2*a2

    # Calculer la somme des trois carrés
    add t3, t0, t1      # t3 = a0^2 + a1^2
    add t3, t3, t2      # t3 = a0^2 + a1^2 + a2^2

    # Vérifier si 2 * (un des carrés) == somme des carrés
    # Cas 1: 2 * a0^2 == somme ?
    slli t4, t0, 1      # t4 = 2 * t0
    beq t4, t3, is_right_triangle

    # Cas 2: 2 * a1^2 == somme ?
    slli t4, t1, 1      # t4 = 2 * t1
    beq t4, t3, is_right_triangle

    # Cas 3: 2 * a2^2 == somme ?
    slli t4, t2, 1      # t4 = 2 * t2
    beq t4, t3, is_right_triangle

    # Si aucune condition n'est remplie
not_right_triangle:
    la a1, non          # Charger l'adresse de la chaîne "NON" dans a1
    li a0, 4            # Code de l'appel système "print string"
    ecall               # Afficher le message
    j end_program       # Aller à la fin

is_right_triangle:
    la a1, oui          # Charger l'adresse de la chaîne "OUI" dans a1
    li a0, 4            # Code de l'appel système "print string"
    ecall               # Afficher le message
    j end_program       # Aller à la fin pour quitter proprement

end_program:
    # Appel système pour terminer proprement le programme
    li a0, 10           # Code de l'appel système "exit"
    ecall