#include "hp_allocator.h"

#define MEM_SIZE (14UL*1024UL * 1024UL)
#define ALIGNEMENT(type) (offsetof(struct { char c; type t; }, t))

int __hp_fd;

union align
{
    long e;
    long double f;
    void *p;
};

union reserve
{
    union align align;
    //char mem[MEM_SIZE];
    char* mem;
};

static union reserve reserve;

static size_t calcule_multiple_align(size_t);
static void *__hp_malloc(size_t);


static size_t calcule_multiple_align(size_t a)
{
    /*
     *      * Calcule le plus petit multiple de l'alignement maximal égal ou supérieur à `a`.
     *           */
    static size_t align_max = ALIGNEMENT(union align);
    size_t multiple = a / align_max;

    return ((a % align_max == 0) ? multiple : multiple + 1) * align_max;
}

static void *__hp_malloc(size_t taille)
{
    /*
     *      * Alloue un bloc de mémoire au moins de taille `taille`.
     *           */

    static size_t alloue = 0UL;
    void *ret;

    assert(taille > 0);
    //La fonction calcule_multiple_align() détermine le plus petit
    //multiple de l’alignement maximal égal ou supérieur à a.
    //Celle-ci nous permet d’« arrondir » la taille demandée de
    //sorte que les blocs alloués soit toujours correctement alignés.
    
    taille = calcule_multiple_align(taille);

    if (alloue + taille > MEM_SIZE)
    {
        fprintf(stderr, "Il n'y a plus assez de mémoire disponible.\n");
        return NULL;
    }

    ret = (void *)(reserve.mem + alloue);
    alloue += taille;
    return ret;

}



//------------------------------------------------------------------------
//Init and finalize to allocate and deallocate huge pages
//
__attribute__((constructor))
static void hp_init(){
    //open the file into hugefs
    __hp_fd = open(FILE_NAME, O_CREAT | O_RDWR, 0755);
    if (__hp_fd < 0) {
        perror("Open failed");
        fprintf(stderr,"check you have done : \n echo 200 > \
                /proc/sys/vm/nr_hugepages\n mount -t hugetlbfs -o mode=0777 none \
                /mnt/huge\n");
        exit(1);
    }

    //mapping memory - utilise les huge pages via le file descriptor
    reserve.mem = mmap(ADDR, MEM_SIZE, PROTECTION, FLAGS, __hp_fd, 0);
    
    if (reserve.mem == MAP_FAILED) {
        perror("mmap");
        close(__hp_fd);
        unlink(FILE_NAME);
        exit(1);
    }
    
    printf("DEBUG: Huge pages allouées à l'adresse %p\n", reserve.mem);
}
__attribute__((destructor))
static void hp_finalize(){
	//free and close
	if (reserve.mem != MAP_FAILED) {
		munmap(reserve.mem, MEM_SIZE);
	}
	if (__hp_fd >= 0) {
		close(__hp_fd);
	}
	unlink(FILE_NAME);
}


//------------------------------------------------------------------------
//API Implementation
//
void *hp_malloc(size_t taille){
    return __hp_malloc(taille);
}

void hp_free(void *ptr){
    // Simple allocator - pas de libération individuelle
    // La mémoire sera libérée globalement à la fin du programme
    (void)ptr; // Pour éviter un warning de variable non utilisée
}
//------------------------------------------------------------------------


