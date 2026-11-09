[English](../en/phantom_slider.md) | [Português](../pt/phantom_slider.md) | **Français** | [Español](../es/phantom_slider.md)

[Retour à la référence technique](index.md) · [Retour au README](../../README.fr.md)

# Phantom Funding Slider Bug (dérive « fantôme » des curseurs de financement)

Cette page explique un ancien problème de SimCity 4, comment il a été reproduit pour ce projet, ce que fait le Ministère de l’Optimisation lorsqu’il le rencontre et, tout aussi important, ce qu’il ne fait **pas**.

## 1. Ce qu’est le bug

Après qu’on a modifié le **coût d’entretien** d’un type de bâtiment (par exemple par un mod qui édite l’exemplar du bâtiment), SimCity 4 peut se mettre à déplacer les **curseurs de financement** des bâtiments de ce type qui étaient **déjà placés** dans une ville sauvegardée. Ce mouvement n’est pas un bruit aléatoire : il peut se répéter aux recalculs mensuels, durer plusieurs mois et pousser le curseur toujours dans le même sens, jusqu’à ce que quelque chose l’arrête.

La communauté le connaît sous le nom de **Phantom Funding Slider Bug** (« curseur fantôme »). Il a été décrit et reproduit par plusieurs membres de la communauté SimCity 4, notamment CorinaMarie, Kel9509 et Null 45 (voir [Crédits et références](#7-crédits-et-références)).

**Un mouvement inattendu d’un curseur ne prouve pas ce bug.** Une modification du joueur, un recalcul légitime, un autre mod ou un autre problème du jeu peuvent avoir le même aspect. Le Ministère ne conclut jamais que « le bug » est présent.

## 2. Comment il a été reproduit

Le Ministère a été testé avec le matériel de reproduction partagé par CorinaMarie : de petites villes jetables et des fichiers DAT qui modifient le coût d’entretien d’une clinique. La séquence suivante reproduit le problème :

1. Un curseur de financement d’un bâtiment est **réglé et confirmé** (dans les tests, par une écriture vérifiée du Ministère lui-même ; la communauté le rapporte aussi avec un réglage manuel du curseur).
2. La ville est sauvegardée et le jeu fermé.
3. On installe un DAT qui modifie le **coût d’entretien de ce type de bâtiment**.
4. La ville est rouverte et simulée mois par mois.

Dans les villes utilisées, faire passer le coût de la ligne de personnel du bâtiment de **300 à 200** a fait baisser le curseur mois après mois (7 % → 4,5 % → 2,5 % → 1,5 %), ce qui a réduit la capacité du bâtiment jusqu’à la grève. Faire passer le coût de **300 à 450** a fait monter le curseur mois après mois (7 % → 10 % → 15 % → 33 % → 49 % → 109 % → 120 %, la limite supérieure). Sans curseur confirmé, le même changement de coût n’a rien déplacé.

Cela s’est produit **sans la gestion automatique du Ministère** (il ne faisait qu’observer) : c’est un comportement du jeu, pas un défaut introduit par le plugin.

**Ce qui a été mesuré (ce n’est pas une explication du fonctionnement interne du jeu).** Après le changement de coût, le coût de la ligne du bâtiment et son « financement plein » mémorisé ont suivi le nouveau coût, mais la valeur que le département budgétaire mémorise pour son financement mensuel idéal a conservé l’ancien total. À chaque recalcul mensuel, le pourcentage a été multiplié par le rapport entre les deux coûts, encore et encore, jusqu’à s’arrêter : après trois recalculs quand le coût baissait, à la limite supérieure quand il montait. La raison pour laquelle le jeu garde l’ancienne valeur n’a pas été établie.

## 3. Ce que fait le Ministère

La protection utilise des mécanismes que le Ministère possédait déjà. Elle n’ajoute aucun réglage au fichier INI, aucun avis pour le joueur, aucune DLL externe ni aucun nouveau calendrier global.

1. **Il enregistre chaque changement de financement qu’il fait et a vérifié** : après chaque écriture, il relit la valeur et la mémorise pour ce bâtiment.
2. **Il surveille les bâtiments qu’il gère.** La surveillance civique observe déjà les écoles et les hôpitaux tous les deux jours simulés.
3. **Il reconnaît un changement suspect** : le financement d’un bâtiment n’est plus ce que le Ministère a vérifié en dernier, et le changement apparaît dans les premiers jours **après un recalcul mensuel**. Dans les villes utilisées, tous les changements que le jeu a faits de lui-même sont apparus à moins d’un jour du recalcul mensuel, et aucun en milieu de mois.
4. **Il réévalue ce bâtiment** avec le même calcul qu’à l’ordinaire, à partir de l’utilisation et de la capacité réelles.
5. **Il corrige le financement plus tôt, dans les deux sens** : il relève ce que le jeu a trop baissé et abaisse ce que le jeu a poussé trop haut.
6. **Il applique le financement minimal qui donne encore la capacité et la réserve exigées par la politique**, et pas simplement l’ancienne valeur.
7. **Il vérifie le résultat** : les contrôles de sécurité habituels s’appliquent, la valeur est donc relue après le changement et comparée à celle visée.
8. **Il continue avec les autres bâtiments** et avec sa revue mensuelle normale.

Règles qui préservent la sécurité :

- Un changement **en milieu de mois**, ou dans un bâtiment où le Ministère n’a jamais écrit, **n’est pas** traité comme ce problème. Il attend la revue mensuelle normale, comme avant. Un changement manuel du joueur n’est donc pas « corrigé tôt », sauf dans les quelques jours qui suivent un recalcul mensuel, où on ne peut pas le distinguer d’un changement du jeu (la revue mensuelle l’aurait remplacé de toute façon).
- Une **réduction** anticipée n’est permise que pour un bâtiment dont le changement a été reconnu ainsi. Les réductions normales dues à une demande en baisse attendent toujours la revue mensuelle.
- **Au plus une correction anticipée par bâtiment et par recalcul mensuel.** Le Ministère ne lutte jamais contre le jeu en boucle.
- Un bâtiment n’est **pas** mis hors service à cause d’une dérive. Si une écriture ne peut pas être vérifiée, les règles de sécurité existantes s’appliquent : ce bâtiment est laissé tranquille pour la session et, après trois bâtiments différents, l’écrivain civique s’arrête.

Dans les tests, le plus long délai entre un changement fait par le jeu et sa correction **est passé d’environ 30 jours simulés à un jour simulé**.

## 4. Ce que la protection ne fait pas

- Elle ne corrige **pas** les données internes du jeu à l’origine du problème.
- Elle n’empêche **pas** le jeu de redéplacer les curseurs au recalcul suivant.
- Elle ne répare **pas** nécessairement des coûts internes périmés.
- Elle ne supprime **pas** les entrées budgétaires fantômes.
- Elle ne garantit **pas** que l’argent réellement prélevé par le jeu corresponde au coût calculé par le Ministère.
- Elle ne protège **pas** les bâtiments que le Ministère ne peut pas gérer (par exemple casernes de pompiers ou commissariats, ou bâtiments sans ligne de financement standard).
- Elle ne garantit **pas** qu’un bâtiment n’ait jamais de grève ni de manque de capacité **entre** deux observations.

Ce qui a été démontré est plus étroit : **le Ministère continue d’optimiser la capacité et le financement des bâtiments éligibles qu’il gère automatiquement, au lieu que les curseurs restent là où le jeu les a laissés.** Le bug de SimCity 4 n’est pas corrigé, mais ses effets observés sur les bâtiments gérés sont sensiblement atténués.

## 5. Limites et sécurité

La détection repose sur des signaux observables, surtout un changement inattendu proche du recalcul mensuel. Elle ne peut pas identifier avec certitude la cause de chaque changement, et n’essaie pas de le faire. Les contrôles qui s’appliquent à toute écriture (compare-and-set avant d’écrire, relecture de confirmation ensuite, exclusion d’un bâtiment dont l’écriture ne peut pas être vérifiée, arrêt de l’écrivain après trois bâtiments différents) s’appliquent ici aussi, et le Ministère refuse d’agir sur des lectures auxquelles il ne peut pas se fier.

## 6. Preuves

Quatre exécutions des scénarios ci-dessus (deux avec le coût qui baisse, deux avec le coût qui monte, 10 mois simulés chacune), le Ministère gérant la clinique automatiquement, comparées à quatre exécutions antérieures des mêmes scénarios faites avant cette protection :

| | Avant | Après |
|---|---|---|
| Plus long délai entre un changement du jeu et sa correction | jusqu’à 30 jours simulés | 1 jour simulé |
| Changements du jeu / corrections vérifiées (par exécution) | 4 à 10 changements, 3 à 8 corrections | 10 et 10 dans chaque exécution : une correction vérifiée par changement, aucune sans changement |
| Financement et capacité finaux | 7 % et 35 | 7 % et 35 (capacité exigée : 32 à 33) |
| Échecs d’écriture, bâtiments mis hors service, grèves | aucun | aucun |
| Jours consécutifs sous la capacité exigée (ou avec un financement supérieur au minimum) | jusqu’à 31 | 2 au plus |

Une exécution témoin de 8 mois simulés **sans aucun changement de coût** n’a fait **aucune** écriture supplémentaire. Le temps total du Ministère était d’environ 2 ms par mois avant et après, sous la résolution de 1 ms de son propre journal.

Ce sont des **résultats observés dans quelques villes de test jetables**, pas une garantie pour toutes les villes.

## 7. Crédits et références

- **CorinaMarie** a expliqué le phénomène et partagé les villes et les fichiers DAT de reproduction qui ont rendu ce travail possible. Son tutoriel sur la mise à jour du coût mensuel d’un lot en évitant les problèmes de budget fantôme décrit comment éviter le problème en **créant** des mods (en donnant au bâtiment modifié une nouvelle identité d’exemplar), ce qui est de la prévention et non la réparation d’une ville déjà touchée : [tutoriel sur Simtropolis](https://community.simtropolis.com/forums/topic/761229-tutorial-updating-lot-monthly-cost-while-avoiding-phantom-budget-predicaments/).
- **Kel9509** a décrit une manière simple de provoquer le problème (modifier la capacité d’un bâtiment avec [SC4PIM-X](https://community.simtropolis.com/files/file/37485-sc4pim-x-2026/), sauvegarder, rouvrir, déplacer le curseur). Cette méthode n’a pas été utilisée dans les tests décrits ici.
- **Null 45 (Nicholas Hayes)** a écrit [Remove Phantom Budget Items](https://community.simtropolis.com/files/file/36437-remove-phantom-budget-items-dll-plugin/) ([code source](https://github.com/0xC0000054/sc4-remove-phantom-budget-items)), une commande de console qui supprime les **entrées budgétaires orphelines** (éléments de bâtiments dont l’exemplar n’existe plus). Elle ne lit ni ne modifie les curseurs de financement et ne corrige **pas** le comportement décrit ici. Elle ne fait pas partie du Ministère et il n’en a pas besoin.
- La discussion a eu lieu dans les [commentaires de la page du Ministère sur Simtropolis](https://community.simtropolis.com/files/file/37700-department-of-efficiency-automatic-city-management-dll/?tab=comments).

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
