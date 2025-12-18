# EnseaSH – Micro Shell ENSEA

## Présentation

Ce projet consiste à développer un **micro shell en C**, nommé **EnseaSH**, dans le cadre du **TP de Synthèse – Ensea in the Shell** (2ᵉ année, Majeure Informatique).

L’objectif est de reproduire les fonctionnalités de base d’un shell Unix :

* lecture de commandes utilisateur,
* exécution de programmes externes,
* affichage du code de retour et du temps d’exécution,
* gestion de la sortie du shell.

Le programme repose exclusivement sur les appels système (`read`, `write`, `fork`, `execvp`, `wait`, `clock_gettime`, etc.), conformément aux consignes du sujet.

---

## Fonctionnalités implémentées

### 1. Message d’accueil et prompt

Au lancement du programme, le shell affiche un message d’accueil :

```
Welcome to ENSEASH !
```

Puis un prompt simple invitant l’utilisateur à saisir une commande :

```
ESH >>
```

Cette fonctionnalité correspond à la **question 1** du sujet.

---

### 2. Boucle REPL (Read – Eval – Print Loop)

Le shell fonctionne en boucle infinie :

1. lecture de la commande utilisateur depuis l’entrée standard (READ)
2. analyse de la commande (EVAL)
3. exécution de la commande et sortie dans stdout (PRINT)

Cela correspond à la **question 2** du sujet.

---

### 3. Lecture de la commande

La fonction :

```c
int readline(char* buffer, int buffer_size, int fd);
```

* lit l’entrée caractère par caractère avec `read`,
* s’arrête à la réception d’un saut de ligne (`\n`),
* stocke la commande dans un buffer terminé par `\0`.

Cette implémentation respecte la contrainte d’utiliser `read` et non `scanf` ou `printf`.

---

### 4. Analyse des arguments

La fonction :

```c
int parse_arguments(char** command, char* buffer, int buffer_size);
```

* découpe la ligne de commande à l’aide de `strtok`,
* sépare la commande et ses arguments,
* remplit un tableau de chaînes de caractères qui sera ensuite transmis à `execvp`.

Cela permet l’exécution de **commandes avec arguments**, comme demandé à la **question 6**.

---

### 5. Exécution des commandes

Pour chaque commande saisie :

* un processus fils est créé avec `fork()`,
* le fils exécute la commande avec `execvp()`,
* le père attend la fin du fils avec `wait()`.

Le shell peut donc lancer n’importe quelle commande présente dans le `PATH`.

---

### 6. Commande `exit`

Lorsque l’utilisateur saisit :

```
exit
```

le shell se termine proprement avec `exit(0)`.

Cette fonctionnalité correspond à la **question 3** du sujet.

---

### 7. Code de retour et temps d’exécution

Le shell mesure le temps d’exécution d’une commande grâce à :

```c
clock_gettime(CLOCK_MONOTONIC, &tp);
```

* un timestamp est pris avant l’appel à `wait`,
* un second après la fin du processus,
* la différence donne le temps d’exécution.

Après l’exécution, le shell affiche :

```
exit:<status>|<time>ms
```

Cette fonctionnalité correspond aux **questions 4 et 5** du sujet.

---

## Limitations connues

Les fonctionnalités suivantes **ne sont pas encore implémentées** :

* redirections `>` et `<`,
* pipes `|`,
* exécution en arrière-plan avec `&`,
* gestion des signaux (`sign:x`),
* gestion avancée du code de retour (décodage avec `WIFEXITED`, `WEXITSTATUS`, etc.).

Ces points correspondent aux **questions 7 à 9** du sujet.

---

## Compilation

Le programme peut être compilé avec :

```bash
gcc -Wall -Wextra -o enseash enseash.c
```

---

## Exécution

Lancer le shell avec :

```bash
./enseash
```

Exemple d’utilisation :

```
ESH >> ls
exit:0|1ms
ESH >> uname -a
exit:0|2ms
ESH >> exit
```

---

## Conclusion

Ce projet constitue une première implémentation fonctionnelle d’un micro shell Unix.
Il respecte les contraintes principales du sujet :

* utilisation des appels système bas niveau,
* exécution de commandes externes,
* affichage du code de retour et du temps d’exécution.

Il forme une base solide pour l’ajout des fonctionnalités avancées demandées dans la suite du TP (redirections, pipes, arrière-plan).
