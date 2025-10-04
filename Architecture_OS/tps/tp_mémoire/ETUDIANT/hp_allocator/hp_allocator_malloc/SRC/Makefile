# Compilation de la bibliothèque dynamique
CC = gcc
CFLAGS = -Wall -fPIC
LDFLAGS = -shared

# Cibles
LIB = libhp_allocator.so
WRAPPER = libmalloc_wrapper.so
EXEC = a.out

# Règle par défaut : compiler tout
all: $(LIB) $(WRAPPER) $(EXEC)

# Compilation de la bibliothèque dynamique hp_allocator
$(LIB): hp_allocator.c hp_allocator.h
	$(CC) $(CFLAGS) $(LDFLAGS) hp_allocator.c -o $(LIB)

# Compilation du wrapper malloc/free
$(WRAPPER): malloc_wrapper.c hp_allocator.h $(LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) malloc_wrapper.c -L. -lhp_allocator -ldl -Wl,-rpath=. -o $(WRAPPER)

# Compilation du programme principal en utilisant la bibliothèque
$(EXEC): main.c $(LIB)
	$(CC) -Wall main.c -L. -lhp_allocator -Wl,-rpath=. -o $(EXEC)

# Compilation de l'ancien exécutable (sans bibliothèque)
static: main.c hp_allocator.c hp_allocator.h
	$(CC) main.c hp_allocator.c -o $(EXEC)

# Installer les bibliothèques
install: $(LIB) $(WRAPPER)
	mkdir -p ../lib ../include
	cp $(LIB) $(WRAPPER) ../lib/
	cp hp_allocator.h ../include/

# Nettoyage
clean:
	rm -f $(EXEC) $(LIB) $(WRAPPER) *.o

.PHONY: all clean static install
