#include "hp_allocator.h"

#define MEM_SIZE (14UL*1024UL * 1024UL)
#define ALIGNEMENT(type) (offsetof(struct { char c; type t; }, t))
#define TAILLE_EN_TETE (offsetof(struct { struct bloc b; union align u; }, u))

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

struct bloc
{
    size_t taille;
    struct bloc *suivant;
};

static union reserve reserve;
static struct bloc *libre;

static void bloc_init(struct bloc *, size_t);
static size_t calcule_multiple_align(size_t);
static struct bloc *recherche_bloc_libre(size_t);

static void *__hp_malloc(size_t);
static void __hp_free(void*);


static void bloc_init(struct bloc *b, size_t taille)
{
    /*
     *      * Initialise les membres d'une structure `bloc`.
     *           */

    b->taille = taille;
    b->suivant = NULL;
}


static size_t calcule_multiple_align(size_t a)
{
    /*
     *      * Calcule le plus petit multiple de l'alignement maximal égal ou supérieur à `a`.
     *           */

    static size_t align_max = ALIGNEMENT(union align);
    size_t multiple = a / align_max;

    return ((a % align_max == 0) ? multiple : multiple + 1) * align_max;
}

//       En-tête                                          En-tête
//       <------------------->                            <------------------->
//       +---------+---------+-----------------+          +---------+---------+-----------------+
//       | Taille  | Suivant |   Bloc alloué   |          | Taille  | Suivant |   Bloc alloué   |
//       +---------+---------+-----------------+          +---------+---------+-----------------+
//                      +                                 ^
//                      |                                 |
//                      +---------------------------------+

static struct bloc *recherche_bloc_libre(size_t taille)
{
    /*
     *      * Recherche un bloc libre ou une suite de bloc libres dont la taille
     *           * est au moins égale à `taille`.
     *                */

    struct bloc *bloc = libre;
    struct bloc *precedent = NULL;
    struct bloc *ret = NULL;

    while (bloc != NULL)
    {
        if (bloc->taille >= taille)
        {
            if (precedent != NULL)
                precedent->suivant = bloc->suivant;
            else
                libre = bloc->suivant;

            ret = bloc;
            break;
        }

        precedent = bloc;
        bloc = bloc->suivant;
    }

    return ret;
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

    struct bloc* bloc_trouve = recherche_bloc_libre(taille); //recherche d'un bloc libre

    if (bloc_trouve == NULL)
    {
        if (MEM_SIZE - alloue < TAILLE_EN_TETE + taille)
        {
            fprintf(stderr, "Il n'y a plus assez de mémoire disponible.\n");
            return NULL;
        }

        bloc_trouve = (struct bloc *)(reserve.mem + alloue);
        alloue += TAILLE_EN_TETE + taille;
        bloc_init(bloc_trouve, taille);
    }

    return (void *)((char *)bloc_trouve + TAILLE_EN_TETE);
}


static void __hp_free(void *ptr)
{
    /*
     *      * Ajoute le bloc fourni à la liste des blocs libres.
     *           */

    struct bloc *bloc = libre;
    struct bloc *nouveau;

    if (ptr == NULL)
        return;

    nouveau = (struct bloc *)((char *)ptr - TAILLE_EN_TETE);

    // Insertion en tête de la liste des blocs libres
    nouveau->suivant = libre;
    libre = nouveau;
}



//------------------------------------------------------------------------
//Init and finalize to allocate and deallocate huge pages
//
__attribute__((constructor))
static void hp_init(){
    static int is_initialized=0;
   // if(! is_initialized){
   //     is_initialized=1;
        //open the file into hugefs
        __hp_fd = open(FILE_NAME, O_CREAT | O_RDWR, 0755);
        if (__hp_fd < 0) {
            perror("Open failed");
            fprintf(stderr,"check you have done : \n echo 200 > \
                    /proc/sys/vm/nr_hugepages\n mount -t hugetlbfs -o mode=0777 none \
                    /mnt/huge");
            exit(1);
        }

        //mapping memory
        //buffer = mmap(ADDR, MEM_SIZE, PROTECTION, FLAGS, __hp_fd, 0);
        reserve.mem = mmap(ADDR, MEM_SIZE, PROTECTION, FLAGS, __hp_fd, 0);
        //if (buffer == MAP_FAILED) {
        if (reserve.mem == MAP_FAILED) {
            perror("mmap");
            unlink(FILE_NAME);
            exit(1);
        }
    //}
}
__attribute__((destructor))
static void hp_finalize(){
	//free and close
	//munmap(buffer, MEM_SIZE);
	munmap(reserve.mem, MEM_SIZE);
	close(__hp_fd);
	unlink(FILE_NAME);
}


//------------------------------------------------------------------------
//API Implementation
//
void *malloc(size_t taille){
    return __hp_malloc(taille);
}

void free(void *ptr){
    __hp_free(ptr);
}
//------------------------------------------------------------------------


