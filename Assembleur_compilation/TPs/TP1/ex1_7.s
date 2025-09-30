#Écrire un programme qui implémente la fonction atoi de C : en supposant que
#a0 contient l’adresse d’une chaîne de caractère s, met dans a0 la conversion en
#entier du plus grand préfixe de s qui ne contient que des chiffres. Par exemple sur
#"234blabla123" le registre a0 contiendra 234.
.data 
.text.globl __start
__start:
    # la méthodologie qu'on va suivre cconsiste à lire le contenude a0 et le lire caractère par caractère
    # et boucler jousgu'à trouver un caractère qui semble etre un caractère et pas un chiffre

    lb t0, 0(a0)      # lire le premier caractère de la chaîne pointée par a0
    beq t0, x0, end   # si c'est le caractère nul (fin de chaîne), terminer
    loop: 
        addi a0, a0, 1  # avancer au caractère suivant
        lb t0, 0(a0)
        
