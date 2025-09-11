Ce document à pour objective l'étude et l'analyse du code de completion fournit par Torvalds dans kernel/sched/completion.c qui assure l'ordonnancement des processus et threads dans le système de linux.
##introduction: 
Le fichier completion.c est le fichier responsable de l'arret et de la repru d'un thread ou d'un processus son mode de travail est semblable à celui de pthread_barrier(). En fait si on a un tread qui a besoin de'attendre jusqu'à qu'une activité au kernel sera fait ou qu'un état spécifique sot attient, completion nous donne solution sans compétition avec les autres threads. 
La completion fournit une meilleure solution que les locks et les semaphores, en fait completion se focalise sur une tache bien précise et veille à l''executer avec un niveau avancé de précision en assurant en meme temps une strategie simple et minimaliste ce qui combine qualité et simplicité. 
Elle est construite sur le waitqueue et le wakeup infrastructure de l'ordonnanceur de linux. les thread qui sont sensé etre arretés pour attendre ce qu'in a mensionné précedement, vont étre reveillé à l'aide d'un simple signal "done" qui suit la structure de completion 'struct completion'. 
##Navigation profonde dans le code de completion.c
#1
En regardant le code on obsèrve qu'il commence par la précision de la license sous laquelle cette code est souscrite qui est SPDX-License-Identifier: GPL-2.0; une question posée à GPT-5 fournit l'explication suivante de ce license : 
// ...existing code...
En regardant le code on obsèrve qu'il commence par la précision de la license sous laquelle cette code est souscrite qui est SPDX-License-Identifier: GPL-2.0; une question posée à GPT-5 fournit l'explication suivante de ce license :
La GNU GPL v2 (GPL‑2.0) est une licence libre à copyleft fort: elle autorise l’utilisation, l’étude, la modification et la redistribution du logiciel. Dès qu’il y a distribution (binaire ou source), il faut fournir le code source correspondant complet, y compris les scripts de compilation/installation, conserver les mentions de copyright et signaler les modifications. Toute œuvre dérivée ou liée doit rester sous GPL‑2.0, sans imposer de restrictions supplémentaires; la licence décline toute garantie et la distribution inclut la licence de brevets nécessaire. “GPL‑2.0‑only” signifie v2 uniquement, tandis que “GPL‑2.0‑or‑later” permet d’utiliser la v2 ou une version ultérieure au choix du destinataire.
Au contraire de ce que je pensait, cette ligne de commentaire n'est pas seule écrite pour les lecteur du code, mais elle écrite aussi pour les outils de license c'est pour cela on voit : SPDX-License-Identifier: avant de préciser la license. 
#2 
Puis en suite on voit les ligne de commentaires : 
/*
 * Generic wait-for-completion handler;
 *
 * It differs from semaphores in that their default case is the opposite,
 * wait_for_completion default blocks whereas semaphore default non-block. The
 * interface also makes it easy to 'complete' multiple waiting threads,
 * something which isn't entirely natural for semaphores.
 *
 * But more importantly, the primitive documents the usage. Semaphores would
 * typically be used for exclusion which gives rise to priority inversion.
 * Waiting for completion is a typically sync point, but not an exclusion point.
 */
 Dans lesquelles l'auteur du code explique les similarité et les différences entre completion et les semaphores.Il signale que la différence majeur entre les deux est que le cas par defaut de semaphore est non-bloquant alors que le cas par defaut de completion est bloquante, ce qui signifie que le semaphore dit au processus ou thread de regarder si la resource est libre ou non, si elle est libre il lui donne l'acces à utiliserl la ressource et ce n'est pas libre, il lui demande d'attendre jusqu'à qu'elle se libère(non bloquante par défaut).Alors que le completion dit au processus ou thread de s'assoir et d'attendre son signal(bloquante par défaut). La deuxième majeur différence est que la completion permet de completer plusieures threads en attentes, ce qui n'est pas le cas pour les semphores.En d'autres termes la completion permet de réveiller plusieurs threads qui sont en attents d'un signal avec la fonction complete_all(x), alors que les semaphores sont plutot concis pour controler l'accès à des ressources ce qui n'inclus pas la fonctionnalité de réveil collective. En plus les fonctions appelé dans les completion sont explicite dans le sens ou leurs noms expliquent leurs fonctionalités ce qui n'est pas le cas pour les semaphores qui sont plus génériques et moins explicites. Pour resumer on dire que le role des semaphores est de proteger des ressources alors que la role des completions est de synchroniser les threads. 
 #3 
 #include <linux/linkage.h>
#include <linux/sched/debug.h>
#include <linux/completion.h>
#include "sched.h"
ce sont des includes rien de particulier à developper 
#4
static void complete_with_flags(struct completion *x, int wake_flags)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&x->wait.lock, flags);

	if (x->done != UINT_MAX)
		x->done++;
	swake_up_locked(&x->wait, wake_flags);
	raw_spin_unlock_irqrestore(&x->wait.lock, flags);
}

void complete_on_current_cpu(struct completion *x)
{
	return complete_with_flags(x, WF_CURRENT_CPU);
}



