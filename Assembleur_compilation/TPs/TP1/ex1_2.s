    .data
msg_ok:   .asciz "OK\n"
msg_not:  .asciz "NO\n"

    .text
    .globl __start
__start:
    # (Optionnel pour tester localement)
    # li a0, 3
    # li a1, 4
    # li a2, 5

    # t0 = a0^2 + a1^2
    mul t0, a0, a0
    mul t1, a1, a1
    add t0, t0, t1

    # t1 = a2^2
    mul t1, a2, a2

    # a0 <- 1 si égalité, 0 sinon
    xor t0, t0, t1
    seqz a0, t0

    # choisir le message selon a0
    la  a1, msg_not      # par défaut: "NO"
    beqz a0, do_print    # si a0 == 0 -> garder "NO"
    la  a1, msg_ok       # sinon -> "OK"

do_print:
    li  a0, 4            # Jupiter: print string
    ecall                # a1 = adresse de la chaîne

    li  a0, 10           # Jupiter: exit
    ecall
