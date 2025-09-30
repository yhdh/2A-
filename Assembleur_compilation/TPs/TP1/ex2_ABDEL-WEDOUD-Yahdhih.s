.data
# No data needed as we use the gp register relative memory

.text
.globl __start

__start:
    # Initialise notre pointeur de pile personnalisé (s1) à l'adresse gp + 32.
    # s1 pointera vers le prochain emplacement libre de la pile.
    addi s1, gp, 32

loop:
    # Appel système pour lire une chaîne de caractères entrée par l'utilisateur.
    li a0, 8          # Code de l'appel système 8 (Read String) pour Jupiter.
    mv a1, gp         # L'adresse du tampon de lecture est dans gp.
    li a2, 32         # Taille maximale de la chaîne à lire.
    ecall

    # Charge le premier caractère de la commande depuis le tampon.
    lb t0, 0(gp)

    # Si la ligne est vide (le premier caractère est nul), on recommence la boucle.
    beq t0, x0, loop

    # Aiguillage en fonction de la commande.
    li t1, 'q'
    beq t0, t1, exit      # Si c'est 'q', on quitte.

    li t1, '+'
    beq t0, t1, handle_add # Si c'est '+', on va à l'addition.

    li t1, 'p'
    beq t0, t1, handle_print # Si c'est 'p', on va à l'affichage.

    # Vérifie si le caractère est un chiffre.
    li t1, '0'
    blt t0, t1, loop      # Si ce n'est pas un chiffre, on ignore et on boucle.
    li t1, '9'
    bgt t0, t1, loop      # Si ce n'est pas un chiffre, on ignore et on boucle.

    # Si on arrive ici, c'est un chiffre. On saute à la section de traitement des nombres.
    j handle_push_number

handle_push_number:
    # Bloc pour analyser un entier depuis le tampon d'entrée et l'empiler.
    mv t1, gp         # t1 est notre pointeur pour parcourir la chaîne.
    li t2, 0          # t2 est l'accumulateur pour construire le nombre.

parse_loop:
    # Charge le caractère courant de la chaîne.
    lb t3, 0(t1)

    # Vérifie si le caractère est un chiffre.
    li t4, '0'
    blt t3, t4, end_parse # Si le caractère < '0', fin du nombre.
    li t4, '9'
    bgt t3, t4, end_parse # Si le caractère > '9', fin du nombre.

    # Convertit le caractère ASCII en sa valeur entière (ex: '1' -> 1).
    addi t3, t3, -48

    # Met à jour l'accumulateur : resultat = resultat * 10 + chiffre.
    li t4, 10
    mul t2, t2, t4
    add t2, t2, t3

    # Passe au caractère suivant.
    addi t1, t1, 1
    j parse_loop

end_parse:
    # Empile le nombre analysé (contenu dans t2).
    sw t2, 0(s1)      # Stocke la valeur à l'adresse pointée par s1.
    addi s1, s1, 4    # Incrémente le pointeur de pile pour le prochain élément.
    j loop

handle_add:
    # Dépile deux nombres, les additionne, et empile le résultat.
    # Vérifie qu'il y a au moins 2 éléments sur la pile.
    addi t0, gp, 32   # Adresse de début de la pile.
    addi t0, t0, 8    # Il faut au moins 2 entiers (8 octets).
    blt s1, t0, loop  # Pas assez d'éléments, on ignore l'opération.

    # Dépile le premier opérande (B).
    addi s1, s1, -4   # Décrémente le pointeur de pile.
    lw t1, 0(s1)      # Charge la valeur.

    # Dépile le second opérande (A).
    addi s1, s1, -4
    lw t2, 0(s1)

    # Additionne A et B.
    add t3, t2, t1

    # Empile le résultat.
    sw t3, 0(s1)
    addi s1, s1, 4
    j loop

handle_print:
    # Dépile un nombre et l'affiche.
    # Vérifie qu'il y a au moins 1 élément sur la pile.
    addi t0, gp, 32   # Adresse de début de la pile.
    addi t0, t0, 4    # Il faut au moins 1 entier (4 octets).
    blt s1, t0, loop  # Pile vide, on ignore l'opération.

    # Dépile la valeur.
    addi s1, s1, -4
    lw a1, 0(s1)      # Charge la valeur dans a1 pour l'appel système.

    # Appel système pour afficher un entier.
    li a0, 1          # Code 1 (Print Integer).
    ecall

    # Appel système pour afficher un caractère (saut de ligne).
    li a0, 11         # Code 11 (Print Char).
    li a1, '\n'       # Le caractère à afficher est '\n'.
    ecall

    j loop

exit:
    # Appel système pour terminer le programme.
    li a0, 10         # Code 10 (Exit).
    ecall
