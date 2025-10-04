# Compilation de la bibliothèque dynamique
CC = gcc
CFLAGS = -Wall -fPIC
LDFLAGS = -shared

# Cibles
LIB = libhp_allocator.so
EXEC = a.out

# Règle par défaut : compiler tout
all: $(LIB) $(EXEC)

# Compilation de la bibliothèque dynamique
$(LIB): hp_allocator.c hp_allocator.h
	$(CC) $(CFLAGS) $(LDFLAGS) hp_allocator.c -o $(LIB)

# Compilation du programme principal en utilisant la bibliothèque
$(EXEC): main.c $(LIB)
	$(CC) -Wall main.c -L. -lhp_allocator -Wl,-rpath=. -o $(EXEC)

# Compilation de l'ancien exécutable (sans bibliothèque)
static: main.c hp_allocator.c hp_allocator.h
	$(CC) main.c hp_allocator.c -o $(EXEC)

# Nettoyage
clean:
	rm -f $(EXEC) $(LIB) *.o

.PHONY: all clean static
