# Input Iterator Concept

Concepts hérités : DefaultConstructible, Assignable (copiable), EqualityComparable

Un input iterator permet une lecture séquentielle (sens unique) d'une plage de données :

Opérations minimales :
- *it : accès en lecture (valeur ou référence const)
- it->m  : accès membre
- ++it   : pré-incrément (renvoie référence à l'itérateur avancé)
- it++   : post-incrément (renvoie une ancienne copie puis avance)
- it1 == it2 / it1 != it2 : comparaison de positions (même séquence et même endroit)

Caractéristiques :
- Monopasse (single-pass) : après avoir avancé, les anciennes copies d'itérateur peuvent être invalidées pour la lecture répétée (ex: stream iterator). On ne garantit pas qu'on puisse relire le même élément plusieurs fois via différentes copies.
- Pas d'arithmétique (pas de it + n, ni --it). Pas de random access.
- La valeur obtenue par *it est lue une fois (pas forcément stable si on réévalue *it plusieurs fois sur certains types d'input iterators, ex: istream_iterator).

Différences avec d'autres catégories :
- Forward iterator : multi-passe (on peut refaire plusieurs parcours), toujours au moins les mêmes opérations qu'un input iterator + stabilité des lectures.
- Bidirectional iterator : ajoute --it.
- Random access iterator : ajoute l'arithmétique (it + n, it - n, it[n], comparaisons < <= > >=, différence it2 - it1).

Utilisation typique :
- Lecture de flux (std::istream_iterator)
- Parcours d'un algorithme qui consomme les données au fur et à mesure.

Résumé : Input iterator = lecture séquentielle minimale, monopasse, uniquement ++ et comparaisons d'égalité.
