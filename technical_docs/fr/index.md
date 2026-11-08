[English](../en/index.md) | [Português](../pt/index.md) | **Français** | [Español](../es/index.md)

[Retour au README](../../README.fr.md)


# Ministère de l’Optimisation : Référence technique

Ce guide explique comment SimCity 4 fonctionne en coulisses et comment le Ministère de l’Optimisation prend ses décisions, service par service. Il s’adresse aux joueurs qui veulent comprendre le raisonnement derrière les actions du Ministère, ainsi qu’aux moddeurs intéressés par la mécanique sous-jacente du jeu.

## Référence rapide

| Service | Réserve ou réglage principal | Par défaut | Protection principale | Chapitre |
|---|---|---|---|---|
| Éducation | Marge de sécurité de capacité | 5 % | Conserve de la capacité en réserve pour absorber les fluctuations de fréquentation des élèves. Le financement des bus scolaires reste inchangé. | [Éducation](education_healthcare.md#education-healthcare) |
| Santé | Marge de sécurité de capacité | 5 % | Conserve de la capacité en réserve pour absorber les fluctuations du nombre de patients. Le financement des ambulances reste inchangé. | [Santé](education_healthcare.md#education-healthcare) |
| Électricité | Réserve d’électricité | 20 % | Conserve de la capacité de production en réserve pour accompagner la croissance de la ville et réduire le risque de pannes de courant. Les lignes électriques sont aussi protégées contre la dégradation. | [Électricité](power.md#power) |
| Eau | Réserve d’eau | 10 % | Conserve de la capacité de production d’eau en réserve, avec la protection des canalisations activée par défaut. | [Eau](water.md#water) |
| Gestion des déchets | Réserve de traitement des déchets | 2 % | Garantit une capacité suffisante de traitement et d’élimination des déchets, dans le but d’évacuer les ordures accumulées avant la fin du mois simulé. | [Déchets et décharges](garbage.md#garbage-landfills) |
| Conseil sur la décharge | Seuil d’économie des alertes | Conseil désactivé ; seuil de 25 % | Recommande d’agrandir ou de réduire la décharge lorsque cela peut faire baisser sensiblement les coûts de la gestion des déchets. | [Conseil sur la taille de la décharge](garbage.md#garbage-landfills-advice) |
| Accords avec les voisins | Gestion par ressource | Activée ; achat-revente entre voisins désactivé | Gère les contrats d’électricité, d’eau et de déchets en donnant la priorité aux besoins propres de la ville et à ses réserves de sécurité. | [Accords avec les voisins](deals.md#neighbor-deals) |
| Démolition | Délais d’attente | Ruines : 1 mois ; bâtiments abandonnés : 12 mois | Protège par défaut les bâtiments municipaux et historiques, avec un plafond mensuel optionnel des dépenses de démolition. | [Démolition automatique](demolition.md#demolition) |

<a id="contents"></a>

## Sommaire

* [A. Éducation et santé](education_healthcare.md#education-healthcare)
* [B. Électricité](power.md#power)
* [C. Eau](water.md#water)
* [D. Déchets et décharges](garbage.md#garbage-landfills)
  * [Conseil sur la taille de la décharge](garbage.md#garbage-landfills-advice)
  * [Cases fantômes](garbage.md#ghost-tiles)
* [E. Accords avec les voisins](deals.md#neighbor-deals)
* [F. Démolition automatique](demolition.md#demolition)
* [G. Sécurité, automatisation et prise de décision](safety.md#safety)
  * [Avis des conseillers](safety.md#notices)
* [H. Référence des options](options.md#options)

* [I. Référence des adresses de l’exécutable](../en/game_addresses.md) (en anglais)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
