## Aperçu  
[![Aperçu de la vidéo](https://img.youtube.com/vi/Ai7FLY1otNE/maxresdefault.jpg)](https://www.youtube.com/watch?v=Ai7FLY1otNE)

---

# The Fallen One

**The Fallen One** est un jeu solo de type **Boss Rush**, composé de **deux boss**, chacun ayant **deux phases**, ainsi qu'une **brève phase de plateforme**.
Ce projet a été développé en **8 semaines** (pré-production et production comprises), en **C++** avec **SFML** et **Box2D**.

---

## Pitch

Inspiré par les Cavaliers de l’Apocalypse, le joueur affronte deux incarnations de ces derniers : **la Pandémie** et **la Famine**.
Armé de son fidèle **grappin** et de **différents pouvoirs**, il devra triompher de ces entités dévastatrices dans des combats intenses et exigeants.

---

## Présentation du jeu

Le jeu commence avec un **mannequin d'entraînement** qui initie le joueur aux mécaniques de gameplay.
Ce même mannequin devient ensuite le premier **adversaire**.

### Boss 1 : La Pandémie

#### Phase 1

Le boss utilise 4 patterns d'attaque :

* Des **pics** surgissent du sol
* Une **attaque sautée** créant une onde de choc
* Il se **transforme en flèche** pour charger et se **téléporter**
* **Lance des rats** en arc de cercle

#### Phase 2 (à l'intérieur de son estomac)

* Le boss est **vulnérable uniquement si les plateformes sont équilibrées**
* Des **petits monstres** nous en empêchent
* Des **mâchoires** tentent de nous bloquer
* Même une fois l'équilibre atteint, le boss **projette des attaques** entre lesquelles il faut **frapper rapidement**

### Phase de plateforme

Une fois la Pandémie vaincue, une **courte séquence de plateforme** mène au **dernier boss**.

---

## Ma participation au projet

Dans ce projet, j’ai travaillé sur :

### Le joueur

* Mouvements et attaques
* Grappin (accroche et déplacement)
* Intégration des animations

### Le tutoriel

### Le premier boss (deux phases)

### Son & Musique

* Intégration des sons et musiques (tutoriel + premier boss)

### Outils développés

* Fonction unique pour **créer des bodies Box2D**
* Utilisation d’un `void*` dans les bodies pour stocker des **informations contextuelles**
* Une fonction permettant des **placements rapides** pour les éléments de la map (murs, grappins, plateformes)
* **Animator** gérant les spritesheets fournies par les graphistes
* **Shaders simples** pour l'apparition des chaînes

---

## Équipe de développement

### Développeurs

* Swann Carle
* Eliot Ledan
* Léo-Paul Vray
* **Hedi Ben Hafsa** (moi)

### Graphistes

* Steven Bacouel
* Tristan Tasset
* Alan Boekwa
* Amance Galaup
* Valentine Lopez-de Mortillet
* Sacha Zamora

---


