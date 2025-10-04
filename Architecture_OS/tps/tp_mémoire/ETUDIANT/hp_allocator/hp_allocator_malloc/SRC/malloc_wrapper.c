/*
 * Wrapper pour intercepter les appels à malloc/free
 * et les rediriger vers hp_malloc/hp_free
 * 
 * Usage: LD_PRELOAD=./libmalloc_wrapper.so ./votre_programme
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <stddef.h>
#include "hp_allocator.h"

// Pointeurs vers les vraies fonctions malloc/free (pour la phase d'initialisation)
static void *(*real_malloc)(size_t) = NULL;
static void (*real_free)(void *) = NULL;
static int initializing = 0;
static int initialized = 0;

// Buffer temporaire pour l'initialisation
#define TEMP_BUFFER_SIZE 1024
static char temp_buffer[TEMP_BUFFER_SIZE];
static size_t temp_buffer_offset = 0;

/*
 * Initialisation : récupère les vrais malloc/free
 */
static void init(void) {
    if (initializing || initialized) return;
    initializing = 1;
    
    // Récupérer les vraies fonctions
    real_malloc = dlsym(RTLD_NEXT, "malloc");
    real_free = dlsym(RTLD_NEXT, "free");
    
    if (!real_malloc || !real_free) {
        fprintf(stderr, "Error in dlsym: %s\n", dlerror());
    }
    
    fprintf(stderr, "[WRAPPER] malloc/free interceptés et redirigés vers hp_malloc/hp_free\n");
    initialized = 1;
    initializing = 0;
}

/*
 * Wrapper pour malloc
 */
void *malloc(size_t size) {
    // Initialiser si nécessaire
    if (!initialized && !initializing) {
        init();
    }
    
    // Pendant l'initialisation, utiliser le buffer temporaire
    if (initializing || !initialized) {
        if (temp_buffer_offset + size < TEMP_BUFFER_SIZE) {
            void *ptr = temp_buffer + temp_buffer_offset;
            temp_buffer_offset += size;
            return ptr;
        }
        // Fallback si le buffer temp est plein
        if (real_malloc) {
            return real_malloc(size);
        }
        return NULL;
    }
    
    // Utiliser hp_malloc au lieu de malloc
    void *ptr = hp_malloc(size);
    
    // Si hp_malloc échoue, utiliser le vrai malloc comme fallback
    if (!ptr && real_malloc) {
        ptr = real_malloc(size);
    }
    
    return ptr;
}

/*
 * Wrapper pour calloc (alloue et initialise à zéro)
 */
void *calloc(size_t nmemb, size_t size) {
    size_t total = nmemb * size;
    void *ptr = malloc(total);
    
    if (ptr) {
        // Initialiser à zéro
        char *p = (char *)ptr;
        for (size_t i = 0; i < total; i++) {
            p[i] = 0;
        }
    }
    
    return ptr;
}

/*
 * Wrapper pour realloc
 */
void *realloc(void *ptr, size_t size) {
    // Si ptr est NULL, équivalent à malloc
    if (ptr == NULL) {
        return malloc(size);
    }
    
    // Si size est 0, équivalent à free
    if (size == 0) {
        free(ptr);
        return NULL;
    }
    
    // Pour simplifier, on alloue un nouveau bloc et on copie
    // (dans un vrai allocateur, on optimiserait ceci)
    void *new_ptr = malloc(size);
    if (new_ptr && ptr) {
        // Copier les données (on ne connaît pas la taille originale,
        // on suppose que c'est <= size pour cette démo)
        char *src = (char *)ptr;
        char *dst = (char *)new_ptr;
        for (size_t i = 0; i < size; i++) {
            dst[i] = src[i];
        }
    }
    
    free(ptr);
    return new_ptr;
}

/*
 * Wrapper pour free
 */
void free(void *ptr) {
    // Ignorer les free du buffer temporaire
    if (ptr >= (void *)temp_buffer && 
        ptr < (void *)(temp_buffer + TEMP_BUFFER_SIZE)) {
        return;
    }
    
    // Ignorer les pointeurs NULL
    if (ptr == NULL) {
        return;
    }
    
    // Utiliser hp_free au lieu de free
    hp_free(ptr);
}

/*
 * Wrapper pour posix_memalign (allocation alignée)
 */
int posix_memalign(void **memptr, size_t alignment, size_t size) {
    // Pour simplifier, on utilise malloc (hp_malloc est déjà aligné)
    *memptr = malloc(size);
    return (*memptr == NULL) ? -1 : 0;
}

/*
 * Wrapper pour aligned_alloc
 */
void *aligned_alloc(size_t alignment, size_t size) {
    // Pour simplifier, on utilise malloc (hp_malloc est déjà aligné)
    return malloc(size);
}

/*
 * Wrapper pour memalign
 */
void *memalign(size_t alignment, size_t size) {
    return malloc(size);
}

/*
 * Wrapper pour valloc (allocation alignée sur page)
 */
void *valloc(size_t size) {
    return malloc(size);
}

/*
 * Wrapper pour pvalloc
 */
void *pvalloc(size_t size) {
    return malloc(size);
}
