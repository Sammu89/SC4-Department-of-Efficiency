[English](../en/options.md) | [Português](../pt/options.md) | **Français** | [Español](../es/options.md)

[Retour au sommaire](index.md#contents) | [Retour au README](../../README.fr.md)

<a id="options"></a>
## H. Référence des options

Ce chapitre explique comment ouvrir les réglages du Ministère et comment les changements de configuration sont enregistrés.

Les descriptions détaillées de chaque option figurent dans les sections suivantes.

<a id="options-open"></a>
### Ouvrir la fenêtre des options

Pour accéder aux réglages du Ministère :

1. Ouvrez **Options de jeu** dans SimCity 4, soit pendant que vous jouez une ville, soit depuis la vue de la région.
2. Cliquez sur **Ministère de l’Optimisation**, le bouton ajouté sous le bouton d’origine **Crédits**.
3. La fenêtre Options de jeu se ferme et la fenêtre de réglages propre au Ministère s’ouvre au centre de l’écran.

La fenêtre de réglages comporte deux pages :

| Page | Ce qu’elle contient |
|---|---|
| **Services** | Financement automatique, marges de réserve, contrats régionaux et conseils sur la taille de la décharge. |
| **Démolition** | Règles de démolition automatique, délais d’attente, types de parcelles protégés et limites de dépenses de démolition. |

Chaque réglage apparaît sous la forme d’une ligne étiquetée avec une case à cocher ou un contrôle numérique.

**Survolez l’étiquette ou la case à cocher d’un réglage pour afficher son infobulle.**

#### Pourquoi certains réglages sont-ils grisés ?

Certains réglages dépendent de l’activation d’une autre option.

Par exemple, le **Seuil d’économie des alertes de décharge** ne peut pas être modifié tant que le conseil sur la taille de la décharge est désactivé.

Lorsqu’une option parente est désactivée :

- Ses réglages dépendants sont grisés.
- Vous ne pouvez pas modifier ces réglages.
- Leurs valeurs existantes sont conservées.
- Ils redeviennent disponibles quand l’option parente est activée.

**Désactiver une fonction n’efface pas ses réglages précédents.**

Chaque option a actuellement au plus une dépendance parente.

<a id="options-accept"></a>
### Accepter et Annuler

La fenêtre de réglages propose deux actions.

#### Accepter

Cliquer sur **Accepter** enregistre vos modifications.

Le Ministère :

1. Identifie les réglages que vous avez modifiés.
2. Met à jour uniquement ces réglages dans les fichiers de configuration concernés.
3. Préserve les réglages sans rapport et les commentaires existants.
4. Vérifie que les fichiers mis à jour peuvent être relus avec exactement les valeurs choisies.
5. Remplace chaque fichier de configuration en une seule opération complète, seulement une fois la validation réussie.
6. Applique immédiatement les nouveaux réglages.

**Si la validation échoue, aucune modification n’est écrite et la fenêtre de réglages reste ouverte.**

Cela évite d’enregistrer partiellement une configuration invalide.

#### Annuler

Cliquer sur **Annuler** ferme la fenêtre de réglages sans enregistrer aucune modification.

Fermer la ville ou quitter SimCity 4 ferme aussi la fenêtre du Ministère sans écrire les réglages non enregistrés.

#### L’ordonnance du Ministère

Le jeu contient aussi l’ordonnance :

**Instituer un Ministère de l’Optimisation**

Elle apparaît dans la fenêtre normale des ordonnances de SimCity 4 et son coût mensuel est de **0 §**.

Cette ordonnance contrôle le même réglage que **Ministère actif**.

- **Promulguer l’ordonnance** active le Ministère.
- **Abroger l’ordonnance** le désactive.

Les deux méthodes modifient le même réglage ; elles restent donc cohérentes.

**Vous pouvez activer ou désactiver le Ministère depuis l’un ou l’autre endroit sans modifier les préférences individuelles de fonctions que vous avez déjà configurées.**
<a id="options-files"></a>
### Où sont enregistrés les réglages ?

Le Ministère enregistre ses réglages dans **trois fichiers de configuration texte**, situés dans le dossier `Department of Efficiency` du répertoire Plugins de SimCity 4.

Vous pouvez modifier la plupart des réglages depuis la fenêtre d’options du Ministère dans le jeu, ou éditer les fichiers à la main.

| Fichier de configuration | Ce qu’il contrôle |
|---|---|
| `DepartmentEfficiency.ini` | Financement de l’éducation et de la santé, marges de sécurité et interrupteur général du Ministère. |
| `DepartmentEfficiencyUtility.ini` | Électricité, eau, gestion des déchets, conseil sur la décharge, contrats régionaux et leurs options associées. |
| `DepartmentEfficiencyDemolition.ini` | Démolition automatique, délais d’attente, zones éligibles, motifs d’abandon et dépenses de démolition. |

Chaque fichier a sa propre section de configuration.

#### Éducation et santé

Fichier : `DepartmentEfficiency.ini`

Section : `[DepartmentEfficiency]`

| Réglage | Clé INI |
|---|---|
| Gérer l’éducation | `Education` |
| Gérer la santé | `Health` |
| Marge de sécurité (éducation) | `EducationReservePercent` |
| Marge de sécurité (santé) | `HealthReservePercent` |
| Ministère actif | `Mode` |

La configuration fournie utilise `ReservePercent=5` pour les marges de sécurité de l’éducation et de la santé.

#### Services publics et contrats régionaux

Fichier : `DepartmentEfficiencyUtility.ini`

Section : `[DepartmentEfficiencyUtility]`

| Réglage | Clé INI |
|---|---|
| Gérer l’électricité | `AutoPower` |
| Réserve d’électricité | `PowerReservePercent` |
| Tenir compte de l’usure des centrales | `ConsiderPowerAging` |
| Gérer l’eau | `AutoWater` |
| Réserve d’eau | `WaterReservePercent` |
| Protéger les canalisations | `ProtectWaterPipes` |
| Gérer les déchets | `AutoGarbage` |
| Réserve de traitement des déchets | `GarbageReservePercent` |
| Conseil sur la taille de la décharge | `WarnLandfillSizeSavings` |
| Seuil d’économie des alertes de décharge | `LandfillSavingsAlertThreshold` |
| Gérer les accords avec les voisins | `AutoDeals` |
| Acheter à une ville pour vendre à une autre | `AllowUtilityArbitrage` |
| Gérer les accords d’électricité | `AutoTradePower` |
| Gérer les accords d’eau | `AutoTradeWater` |
| Gérer les accords de déchets | `AutoTradeGarbage` |
| Ministère actif | `Mode` |

#### Démolition automatique

Fichier : `DepartmentEfficiencyDemolition.ini`

Section : `[LotDemolition]`

| Réglage | Clé INI |
|---|---|
| Démolir les bâtiments détruits | `DemolishDestroyed` |
| Attendre (ruines) | `DestroyedAfterDays` |
| Démolir les bâtiments municipaux et historiques détruits | `DemolishMunicipalAndHistoricRuins` |
| Démolir les bâtiments abandonnés | `DemolishAbandoned` |
| Attendre (bâtiments abandonnés) | `AbandonedAfterDays` |
| Types de zone éligibles | `AbandonedZones` |
| Motifs d’abandon éligibles | `AbandonedReasons` |
| Limiter le budget mensuel de démolition | `LimitMonthlyBudget` |
| Budget mensuel de démolition | `MonthlyBudget` |
| Ministère actif | `Mode` |

#### Puis-je modifier ces fichiers à la main ?

Oui, mais **les valeurs de configuration doivent respecter le format pris en charge et les plages autorisées**.

Le Ministère vérifie chaque fichier avant de l’utiliser.

Un réglage inconnu ou une valeur invalide rend invalide le fichier de configuration concerné : cette partie du Ministère est alors désactivée plutôt que de s’appuyer sur des réglages incertains.

Les réglages absents des fichiers fournis utilisent leurs valeurs par défaut.

Par exemple, la configuration de démolition par défaut active la suppression des ruines et des bâtiments abandonnés éligibles, tout en protégeant les parcelles agricoles et les bâtiments abandonnés à cause de l’absence d’électricité ou d’eau.

**Les modifications manuelles sont normalement détectées et appliquées en l’espace d’un mois simulé.**

Pour les modifications courantes, la fenêtre d’options du jeu est plus sûre, car elle vérifie les valeurs autorisées avant de les enregistrer.

---

<a id="options-services"></a>
### Page Services

La page **Services** regroupe le financement automatique, les réserves de sécurité, les contrats régionaux et les conseils sur la décharge.

La première option contrôle l’ensemble du Ministère. Les autres options vous permettent de décider quels services il doit gérer.

<a id="opt-master"></a>
### Ministère actif

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `Mode`</text>

**C’est l’interrupteur général de tout le Ministère.**

Lorsqu’il est activé, chaque fonction automatique suit son propre réglage individuel.

Par exemple, vous pouvez laisser le Ministère actif tout en désactivant la gestion de l’eau ou la démolition automatique.

Lorsqu’il est désactivé, le Ministère cesse de :

- Ajuster le financement de l’éducation, de la santé et des services publics.
- Créer, modifier ou annuler des contrats régionaux.
- Démolir des bâtiments et des ruines.
- Réparer les cases fantômes de décharge.
- Calculer les recommandations sur la taille de la décharge et les avis de pénurie.

**Désactiver le Ministère n’annule pas les changements qu’il a déjà effectués.**

Les niveaux de financement et les contrats régionaux restent tels quels.

Vos préférences individuelles sont aussi conservées : elles redeviennent actives quand vous réactivez le Ministère.

Tous les autres réglages des pages Services et Démolition deviennent indisponibles tant que l’interrupteur général est désactivé.

**Important :** les fichiers de ressources de conseillers inclus et la modification qui masque les icônes « pas d’eau » des fermes restent actifs tant qu’ils sont installés, même lorsque le Ministère est désactivé.

Vous pouvez aussi contrôler ce même interrupteur avec **Instituer un Ministère de l’Optimisation** dans la fenêtre des ordonnances de SimCity 4.

Le réglage est enregistré sous la forme `Mode=Auto` ou `Mode=Off` dans chacun des trois fichiers de configuration.

L’éducation et la santé fonctionnent dès qu’au moins une de leurs options de gestion est activée. La démolition fonctionne dès qu’au moins une catégorie de démolition est activée. Le contrôleur des services publics reste disponible tant que le Ministère est actif, même si aucun service public n’est sélectionné individuellement.

Voir [Sécurité, automatisation et prise de décision](safety.md#safety-player-control).

<a id="opt-education"></a>
### Gérer l’éducation

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `Education`</text>

Ajuste automatiquement le financement des établissements d’éducation éligibles, notamment les écoles, collèges, universités, bibliothèques et musées.

**Le Ministère choisit le pourcentage de financement entier le plus bas qui permet de servir les usagers de l’établissement tout en maintenant la marge de sécurité requise.**

Lorsque l’option est activée :

- Le financement peut augmenter pendant le premier jour simulé après l’ouverture d’une ville.
- Le Ministère peut augmenter le financement lorsqu’il détecte un risque de capacité.
- Les réductions de financement sont évaluées lors de la révision mensuelle.
- Une saturation persistante que le financement ne peut pas résoudre peut générer un avis de conseiller.

Lorsqu’elle est désactivée, le financement de l’éducation reste aux valeurs que vous avez définies à la main et les avis de capacité de l’éducation sont désactivés.

**Important :** les bâtiments qui ne prennent pas en charge le mécanisme de financement standard du jeu ne sont pas modifiés. Les conseils d’origine du jeu sur le financement de l’éducation restent supprimés tant que le fichier de ressources de conseils civiques est installé, même si la gestion automatique de l’éducation est désactivée.

**Nécessite :** Ministère actif.

**Contrôle :** la [Marge de sécurité](#opt-educationreserve) de l’éducation.

Voir [Éducation et santé](education_healthcare.md#education-healthcare).

<a id="opt-educationreserve"></a>
### Marge de sécurité (éducation)

<text color="secondary">Par défaut : 5 % | Plage : 0–50 %, par pas de 1 % | INI : `EducationReservePercent`</text>

Détermine la capacité supplémentaire que le Ministère maintient au-dessus de l’utilisation actuelle de chaque bâtiment d’éducation.

Par exemple, une école qui accueille 1 000 élèves avec une marge de 5 % a un besoin protégé de base de :

```math
1000\times1.05=1050
```

**Au moins 1 050 places.**

Une marge plus élevée laisse plus de place à la croissance et aux hausses imprévues de fréquentation, mais peut exiger plus de financement.

Une marge plus basse réduit les dépenses en laissant la capacité plus proche de l’utilisation actuelle.

**La marge de sécurité est un minimum, pas une cible exacte.**

Le Ministère peut conserver une capacité supplémentaire pour les petits bâtiments ou les établissements dont l’utilisation fluctue fortement, afin d’éviter les grèves.

**Nécessite :** Gérer l’éducation et Ministère actif.

Voir [Éducation et santé](education_healthcare.md#education-healthcare-required).

<a id="opt-health"></a>
### Gérer la santé

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `Health`</text>

Gère automatiquement le financement des cliniques et des hôpitaux éligibles.

Le Ministère sélectionne le pourcentage de financement entier le plus bas qui offre une capacité suffisante pour les patients actuels et la marge de sécurité de santé configurée.

Lorsque l’option est activée :

- Les hausses de financement peuvent intervenir rapidement quand un risque de capacité est détecté.
- Les réductions sont envisagées lors des révisions mensuelles.
- Des pénuries de capacité persistantes que le financement maximal ne peut pas résoudre peuvent générer des avis de conseiller.

Lorsqu’elle est désactivée, le financement de la santé reste sous votre contrôle manuel et les avis de capacité de la santé ne sont pas générés.

**Le Ministère ne modifie jamais la ligne budgétaire distincte des ambulances.**

Les bâtiments qui ne prennent pas en charge le mécanisme de financement standard sont exclus.

Les conseils d’origine du jeu sur le financement ordinaire de la santé restent supprimés tant que le fichier de ressources de conseils civiques est installé, quel que soit ce réglage.

**Nécessite :** Ministère actif.

**Contrôle :** la [Marge de sécurité](#opt-healthreserve) de la santé.

Voir [Éducation et santé](education_healthcare.md#education-healthcare).

<a id="opt-healthreserve"></a>
### Marge de sécurité (santé)

<text color="secondary">Par défaut : 5 % | Plage : 0–50 %, par pas de 1 % | INI : `HealthReservePercent`</text>

Détermine la capacité supplémentaire de patients que maintient chaque établissement de santé géré.

Par exemple, un hôpital qui soigne actuellement 2 000 patients avec une marge de sécurité de 5 % exige au moins :

```math
2000\times1.05=2100
```

**2 100 places de capacité.**

Augmenter la marge laisse plus de place à la croissance et aux fluctuations du nombre de patients, mais peut accroître les dépenses de financement.

La réduire permet un financement plus bas lorsque l’utilisation réelle est stable.

**Comme pour l’éducation, c’est une exigence minimale.** Le Ministère peut maintenir plus de capacité lorsque ses calculs de sécurité l’exigent.

**Nécessite :** Gérer la santé et Ministère actif.

Voir [Éducation et santé](education_healthcare.md#education-healthcare-required).

<a id="opt-power"></a>
### Gérer l’électricité

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `AutoPower`</text>

Ajuste automatiquement le financement de chaque centrale électrique.

Au lieu de financer toutes les centrales au même pourcentage, le Ministère cherche **la combinaison la moins chère de niveaux de financement des centrales** capable de fournir :

- Les besoins en électricité de votre ville.
- La réserve d’électricité configurée.
- Toutes les ventes d’électricité en cours aux villes voisines.

Il tient aussi compte des achats régionaux d’électricité disponibles.

Lorsque l’option est activée, une révision complète a normalement lieu tous les **10 jours simulés**, avec des évaluations anticipées en cas de déficit d’approvisionnement confirmé ou de changement important de la demande ou de la capacité.

Lorsqu’elle est désactivée, le financement des centrales reste à ses valeurs actuelles.

**Les contrats d’électricité sont contrôlés séparément.** Ils peuvent toujours être gérés si Gérer les accords d’électricité est activé.

Le Ministère ne modifie que le financement. Il ne construit jamais de centrales supplémentaires.

Si les besoins en électricité de la ville ne peuvent pas être couverts avec les installations et les contrats disponibles, une pénurie persistante peut produire un avis de conseiller.

**Nécessite :** Ministère actif.

**Contrôle :** [Réserve d’électricité](#opt-powerreserve) et [Tenir compte de l’usure des centrales](#opt-poweraging).

Voir [Électricité](power.md#power).

<a id="opt-powerreserve"></a>
### Réserve d’électricité

<text color="secondary">Par défaut : 20 % | Plage : 10–50 %, par pas de 1 % | INI : `PowerReservePercent`</text>

Détermine la capacité de production d’électricité supplémentaire que le Ministère protège contre les variations de la demande.

Sans achats ni ventes, le besoin local protégé est au moins égal au plus grand des deux éléments suivants :

- Le besoin en électricité propre au jeu.
- La demande d’électricité multipliée par **1 + réserve**.

Par exemple, avec une réserve de 20 %, une ville dont la demande est de 10 000 unités a normalement besoin d’au moins 12 000 unités de capacité de production, sauf si le besoin propre au jeu est plus élevé.

Une réserve plus élevée offre une meilleure protection contre les hausses soudaines de la demande d’électricité, mais peut augmenter les frais de fonctionnement.

Une réserve plus basse réduit les coûts, mais laisse moins de capacité excédentaire avant qu’une pénurie ne devienne possible.

**La réserve est un minimum, pas une limite de production.**

Le Ministère peut maintenir une capacité supplémentaire lorsque des ventes d’électricité rentables le justifient.

**Nécessite :** Gérer l’électricité et Ministère actif.

Voir [Électricité](power.md#power).

<a id="opt-poweraging"></a>
### Tenir compte de l’usure des centrales

<text color="secondary">Par défaut : désactivé | Valeurs : activé / désactivé | INI : `ConsiderPowerAging`</text>

Permet au Ministère de prendre en compte **l’usure et le vieillissement des centrales électriques** lorsqu’il compare les configurations de financement de l’électricité.

Lorsqu’elle est désactivée, l’optimiseur se concentre sur les dépenses de fonctionnement mensuelles.

Lorsqu’elle est activée, le calcul tient aussi compte du coût économique de l’usure des centrales.

Il peut alors être intéressant de dépenser un peu plus chaque mois pour réduire l’usure et prolonger la durée de vie utile d’une centrale.

**Cette usure estimée est un coût économique, pas une charge mensuelle supplémentaire pour votre trésorerie.**

Les besoins en électricité de la ville et la réserve configurée restent protégés dans les deux modes.

Si Gérer l’électricité est désactivé, la préférence d’usure est conservée mais n’a aucun effet tant que la gestion de l’électricité n’est pas réactivée.

**Nécessite :** Gérer l’électricité et Ministère actif.

Voir [Électricité](power.md#power).

<a id="opt-water"></a>
### Gérer l’eau

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `AutoWater`</text>

Gère automatiquement le financement du service des eaux.

Contrairement à l’électricité, l’eau utilise **un seul curseur de financement de service** pour ses pompes et ses installations de traitement.

Le Ministère choisit un niveau de financement qui offre une capacité d’eau suffisante pour la demande de la ville et la réserve configurée, en tenant compte des contrats régionaux.

Lorsque l’option est activée, le financement de l’eau est révisé :

- Tous les 10 jours simulés depuis l’évaluation précédente.
- À chaque changement de mois simulé.
- Plus tôt lorsqu’une pénurie régionale ou un changement important est détecté.

Lorsqu’elle est désactivée, le financement de l’eau reste inchangé.

Les contrats régionaux d’eau peuvent toujours être gérés indépendamment avec Gérer les accords d’eau.

**Le Ministère ne réduit pas le financement de l’eau tant que des bâtiments sont privés d’eau, selon les conditions de sécurité applicables du gestionnaire de l’eau.**

**Nécessite :** Ministère actif.

**Contrôle :** [Réserve d’eau](#opt-waterreserve) et [Protéger les canalisations](#opt-waterpipes).

Voir [Eau](water.md#water).

<a id="opt-waterreserve"></a>
### Réserve d’eau

<text color="secondary">Par défaut : 10 % | Plage : 10–50 %, par pas de 1 % | INI : `WaterReservePercent`</text>

Détermine la capacité d’eau supplémentaire que le Ministère maintient au-dessus de la demande de la ville.

Par exemple, avec une réserve de 10 %, une ville dont la demande est de 50 000 unités d’eau exige au moins :

```math
50000\times1.10=55000
```

**55 000 unités de capacité protégée**, avant de tenir compte des effets détaillés des contrats.

Une réserve plus élevée protège mieux contre la hausse de la demande en eau, mais peut augmenter les dépenses.

Une réserve plus basse permet un fonctionnement moins cher avec moins de capacité excédentaire.

**La réserve est une exigence minimale.** Le Ministère peut maintenir plus de capacité si nécessaire.

**Nécessite :** Gérer l’eau et Ministère actif.

Voir [Eau](water.md#water-required-capacity).

<a id="opt-waterpipes"></a>
### Protéger les canalisations

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `ProtectWaterPipes`</text>

Protège le réseau de distribution d’eau contre la détérioration causée par un financement insuffisant.

Dans SimCity 4, maintenir le service des eaux sous certains niveaux de financement peut contribuer à endommager les canalisations.

Lorsque cette option est activée, le Ministère :

- Ne réduit jamais le financement de l’eau sous **50 %**.
- Ne réduit pas le financement tant que le réseau de canalisations est endommagé.
- Évite les niveaux de financement pour lesquels la courbe d’entretien des canalisations du jeu permet la détérioration du réseau.

**L’objectif est d’économiser de l’argent sans laisser le réseau d’eau se dégrader par manque de financement.**

Lorsqu’elle est désactivée, les restrictions spéciales de protection des canalisations sont levées.

Le financement de l’eau reste soumis aux exigences normales de service et de réserve, mais le Ministère peut choisir des niveaux inférieurs à 50 % lorsque cela est autorisé par ailleurs.

Cela peut faire économiser de l’argent, **au risque que les canalisations se détériorent ou éclatent**.

L’option n’augmente pas à elle seule le financement dans le seul but de réparer des dégâts existants sur les canalisations ; sa protection consiste surtout à empêcher les réductions dangereuses.

Si Gérer l’eau est désactivé, cette préférence est enregistrée mais inactive.

**Nécessite :** Gérer l’eau et Ministère actif.

Voir [Eau](water.md#water-pipe-curve).

<a id="opt-sanitation"></a>
### Gérer les déchets

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `AutoGarbage`</text>

Ajuste automatiquement le financement du service de gestion des déchets.

La gestion des déchets couvre le système d’élimination des ordures de la ville, y compris les décharges et les installations de traitement, au moyen d’un seul curseur de financement de service.

Le Ministère prend en compte :

- Les déchets produits après recyclage.
- La réserve de traitement des déchets configurée.
- Les déchets déjà non collectés.
- Les importations et exportations régionales de déchets.
- La capacité de traitement des incinérateurs et des centrales de valorisation énergétique des déchets.
- Le stockage disponible des décharges et leur admission mensuelle.

Il cherche ensuite le niveau de financement le moins cher capable de traiter les déchets prévus.

**Contrairement à l’électricité et à l’eau, les réductions de financement de la gestion des déchets sont progressives.**

Les réductions sont limitées à **25 points de pourcentage par écriture**, avec au moins **7 jours simulés** entre deux baisses.

Les hausses de financement exigent au moins **3 jours simulés** depuis la dernière écriture de financement de la gestion des déchets.

Le Ministère révise la gestion des déchets à chaque changement de mois et lorsque les statistiques de déchets changent de façon importante.

Lorsque l’option est activée, il peut aussi effectuer des réparations automatiques sûres des cases fantômes de décharge.

Lorsqu’elle est désactivée :

- Le financement de la gestion des déchets reste inchangé.
- La réparation des cases fantômes s’arrête.
- Le conseil sur la taille de la décharge n’est plus disponible.
- Les contrats de déchets peuvent toujours être gérés indépendamment si Gérer les accords de déchets est activé.

**Le Ministère ne construit ni ne zone jamais automatiquement de zones de décharge supplémentaires.** L’agrandissement et la réduction de la décharge relèvent uniquement du conseil.

**Nécessite :** Ministère actif.

**Contrôle :** [Réserve de traitement des déchets](#opt-sanitationreserve) et [Conseil sur la taille de la décharge](#opt-landfilladvice).

Voir [Déchets et décharges](garbage.md#garbage-landfills).

<a id="opt-sanitationreserve"></a>
### Réserve de traitement des déchets

<text color="secondary">Par défaut : 2 % | Plage : 2–20 %, par pas de 1 % | INI : `GarbageReservePercent`</text>

Ajoute une marge de sécurité à la quantité de déchets que votre ville produit après recyclage.

La quantité de déchets protégée est calculée à partir de :

```math
\text{Déchets protégés}=
(\text{Produits}-\text{Recyclés})
\times(1+\text{réserve})
```

Le calcul de planification réel inclut aussi l’éventuel arriéré de déchets non collectés et les effets des contrats régionaux.

Par exemple, une ville qui produit 80 000 unités de déchets après recyclage a un besoin protégé de base de **81 600 unités** avec la réserve par défaut de 2 %.

Une marge plus élevée offre une protection supplémentaire contre les hausses de production de déchets, mais peut exiger un financement plus élevé de la gestion des déchets.

Une marge plus basse réduit les dépenses potentielles.

**La réserve est un minimum, pas une cible maximale d’élimination.**

**Nécessite :** Gérer les déchets et Ministère actif.

Voir [Déchets et décharges](garbage.md#garbage-landfills-need).

<a id="opt-landfilladvice"></a>
### M’avertir si revoir la décharge permet d’économiser

<text color="secondary">Par défaut : désactivé | Valeurs : activé / désactivé | INI : `WarnLandfillSizeSavings`</text>

Active des recommandations facultatives sur la modification de la taille de votre décharge.

Le Ministère recherche les situations où :

- **Agrandir une décharge** apporterait assez de capacité d’élimination supplémentaire pour réduire le financement de gestion des déchets nécessaire et la dépense mensuelle totale.
- **Réduire une décharge** supprimerait des cases inutilisées et abaisserait les frais d’entretien sans compromettre l’élimination des déchets.

Si un changement sûr produit des économies supérieures à votre seuil configuré, le Ministère publie un avis de conseiller.

L’avis indique la zone proposée, estime l’économie mensuelle et comprend un lien qui déplace la caméra vers l’endroit concerné.

**Cette fonction ne donne que des conseils.**

Elle ne :

- Zone ni ne dézone aucun terrain.
- Construit ni ne démolit rien.
- Modifie pas le financement à cause du conseil.
- Modifie pas les contrats régionaux à cause du conseil.

L’analyse s’exécute **au plus une fois par mois simulé**, sous réserve de périodes de protection supplémentaires décrites dans le chapitre sur les décharges.

Elle ne fonctionne que dans les villes contenant **exactement une zone de décharge**.

Lorsqu’elle est désactivée, aucune analyse de taille de décharge ni aucun avis n’est produit.

L’historique de délais existant est conservé : désactiver puis réactiver l’option ne réinitialise donc pas ses périodes d’attente.

**Nécessite :** Gérer les déchets et Ministère actif.

**Contrôle :** [Seuil d’économie des alertes de décharge](#opt-landfillthreshold).

Voir [Déchets et décharges](garbage.md#garbage-landfills-advice).

<a id="opt-landfillthreshold"></a>
### Seuil d’économie des alertes de décharge

<text color="secondary">Par défaut : 25 % | Plage : 20–50 %, par pas de 1 % | INI : `LandfillSavingsAlertThreshold`</text>

Détermine l’économie minimale qu’une modification proposée de la taille de la décharge doit produire pour que le Ministère la recommande.

La comparaison utilise la dépense mensuelle optimisée de gestion des déchets, entretien compris.

**Seuil de conseil**

```math
\text{Économie}\times100>
\text{Seuil}\times\text{Dépense mensuelle}
```

**La comparaison est stricte.**

Par exemple, avec une dépense optimisée actuelle de gestion des déchets de 1 000 § et un seuil de 25 % :

- Économie de 200 § : pas d’avis.
- Économie de 250 § : pas d’avis.
- Économie de 251 § : un avis peut être émis.

Un seuil plus élevé signifie moins de recommandations, limitées aux économies potentielles plus importantes.

Un seuil plus bas permet de signaler des occasions d’économies plus modestes.

**L’avis affiche l’économie réellement calculée, et non le seuil que vous avez choisi.**

Modifier ce réglage ne réinitialise pas les périodes d’attente du conseil.

Lorsque le conseil sur la taille de la décharge est désactivé, le contrôle du seuil devient indisponible, mais sa valeur enregistrée est conservée.

**Nécessite :** Conseil sur la taille de la décharge, Gérer les déchets et Ministère actif.

Voir [Déchets et décharges](garbage.md#garbage-landfills-advice).

<a id="opt-deals"></a>
### Gérer les accords avec les voisins

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `AutoDeals`</text>

C’est l’**interrupteur général de la gestion automatique des contrats régionaux**.

Lorsqu’il est activé, le Ministère peut examiner les offres des villes voisines et gérer les contrats d’électricité, d’eau et de déchets.

Chaque ressource est contrôlée par sa propre option.

Le Ministère peut :

- Acheter des ressources lorsqu’elles sont moins chères que la production locale.
- Vendre la capacité réellement excédentaire lorsque c’est rentable.
- Ajuster les quantités des contrats existants.
- Annuler les contrats qui n’apportent plus d’avantage.

**Les services propres à votre ville et ses réserves de sécurité passent toujours en premier.**

Lorsqu’il est désactivé :

- Les contrats régionaux existants restent en place.
- Ces contrats continuent d’être pris en compte dans les calculs de financement.
- Aucune nouvelle offre de contrat n’est demandée.
- Aucun contrat n’est créé, redimensionné ou annulé.
- Aucune capacité locale n’est réservée pour d’hypothétiques ventes futures.
- Les avis de pénurie de services publics ne peuvent pas être entièrement prouvés, car les offres régionales ne sont pas examinées.

**Important :** les messages de conseillers d’origine de SimCity 4 sur les accords avec les voisins restent supprimés tant que le fichier de ressources de conseils sur les accords du Ministère est installé.

Pour rétablir ces messages d’origine après avoir désactivé la gestion automatique des contrats, retirez le fichier de ressources concerné, le jeu étant fermé.

**Nécessite :** Ministère actif.

**Contrôle :** [Acheter à une ville pour vendre à une autre](#opt-arbitrage), [Gérer les accords d’électricité](#opt-tradepower), [Gérer les accords d’eau](#opt-tradewater) et [Gérer les accords de déchets](#opt-tradegarbage).

Voir [Accords avec les voisins](deals.md#neighbor-deals).

<a id="opt-arbitrage"></a>
### Acheter à une ville pour vendre à une autre

<text color="secondary">Par défaut : désactivé | Valeurs : activé / désactivé | INI : `AllowUtilityArbitrage`</text>

Permet au Ministère d’**acheter et de vendre la même ressource dans un même plan de contrats** lorsque cette combinaison donne un meilleur résultat économique.

Lorsque cette option est désactivée, chaque ressource suit l’un de ces trois schémas :

- **LOCAL :** ni achat ni vente régionaux.
- **ACHAT :** achat de la ressource aux villes voisines.
- **VENTE :** vente de la ressource aux villes voisines.

Lorsque l’option est activée, le Ministère peut combiner achats et ventes.

Par exemple, il peut acheter de l’électricité à un voisin pour couvrir une partie de la demande de la ville, ce qui libère de l’électricité produite localement pour la vendre avec profit à un autre voisin.

**Le Ministère ne revend jamais directement les ressources importées.**

Les achats restent limités par la demande propre de la ville, et les règles normales de service et de réserve continuent de s’appliquer.

Pour les déchets, un schéma mixte pourrait consister à recevoir des déchets payés par un voisin tout en payant un autre voisin pour éliminer une partie de vos propres déchets.

Une ressource dont l’option de gestion individuelle des accords est désactivée n’est jamais échangée par le Ministère, quel que soit ce réglage.

**Nécessite :** Gérer les accords avec les voisins et Ministère actif.

Voir [Accords avec les voisins](deals.md#neighbor-deals-arbitrage).

<a id="opt-tradepower"></a>
### Gérer les accords d’électricité

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `AutoTradePower`</text>

Permet au Ministère de créer, redimensionner et annuler des contrats d’électricité avec les villes voisines.

Lorsque l’option est activée, il peut :

- Acheter de l’électricité lorsque c’est économiquement intéressant.
- Vendre l’électricité excédentaire lorsque c’est rentable.
- Réduire ou annuler les ventes qui menacent l’approvisionnement propre de la ville.
- Ajuster les contrats existants au gré de l’évolution de la demande, de la production et des prix.

Lorsqu’elle est désactivée, les contrats d’électricité existants restent sous votre contrôle manuel.

Le Ministère continue de les lire pour calculer les besoins locaux en électricité, mais il ne les modifie pas et ne réserve pas de capacité pour de futures ventes automatiques.

**Ce réglage contrôle les contrats, pas le financement des centrales.**

Il fonctionne indépendamment de **Gérer l’électricité**.

Par exemple, vous pouvez gérer automatiquement les contrats d’électricité tout en contrôlant vous-même le financement des centrales. Dans ce cas, le Ministère doit composer avec la capacité de production déjà disponible.

**Nécessite :** Gérer les accords avec les voisins et Ministère actif.

Voir [Accords avec les voisins](deals.md#neighbor-deals).

<a id="opt-tradewater"></a>
### Gérer les accords d’eau

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `AutoTradeWater`</text>

Permet au Ministère de gérer les achats et les ventes d’eau avec les villes voisines.

Lorsque l’option est activée, il compare les prix des contrats au coût de la production locale d’eau.

Il peut créer, redimensionner ou annuler des contrats lorsque cela améliore la situation économique de la ville sans compromettre le service de l’eau ni la réserve configurée.

Lorsqu’elle est désactivée, les contrats d’eau existants restent sous votre contrôle manuel.

**Cette option ne contrôle pas le financement du service des eaux.**

Elle est indépendante de **Gérer l’eau**.

Par exemple, vous pouvez autoriser le Ministère à optimiser le financement de l’eau tout en gardant inchangés vos propres contrats régionaux d’eau.

**Nécessite :** Gérer les accords avec les voisins et Ministère actif.

Voir [Accords avec les voisins](deals.md#neighbor-deals).

<a id="opt-tradegarbage"></a>
### Gérer les accords de déchets

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `AutoTradeGarbage`</text>

Permet au Ministère de gérer les contrats de déchets dans les deux sens.

Rappelez-vous que les contrats de déchets fonctionnent différemment de ceux de l’électricité et de l’eau :

- **Envoyer des déchets à un voisin :** votre ville paie pour leur élimination.
- **Recevoir des déchets d’un voisin :** le voisin paie votre ville pour traiter ses déchets.

Lorsque l’option est activée, le Ministère peut créer, redimensionner et annuler ces contrats selon leur valeur économique et la capacité d’élimination de la ville.

**Les besoins d’élimination des déchets propres à votre ville passent toujours en premier.**

Le Ministère n’accepte pas de déchets supplémentaires simplement parce qu’une autre ville offre de l’argent.

Il doit pouvoir traiter ces déchets en toute sécurité avec les installations de gestion des déchets et le financement disponibles.

Lorsqu’elle est désactivée, les contrats de déchets existants restent sous votre contrôle manuel.

**Cette option est indépendante de Gérer les déchets.**

Vous pouvez gérer les contrats de déchets sans modifier automatiquement le financement de la gestion des déchets, ou gérer ce financement tout en laissant les contrats inchangés.

**Nécessite :** Gérer les accords avec les voisins et Ministère actif.

Voir [Accords avec les voisins](deals.md#neighbor-deals-garbage).

---

<a id="options-demolition"></a>
### Page Démolition

La page **Démolition** contrôle la façon dont le Ministère déblaie les bâtiments détruits et les parcelles abandonnées de longue date.

Vous pouvez décider :

- Si les ruines doivent être supprimées automatiquement.
- Pendant combien de temps les bâtiments doivent rester détruits ou abandonnés.
- Quels types de bâtiments abandonnés peuvent être démolis.
- Quels motifs d’abandon sont éligibles.
- Si les ruines municipales et historiques peuvent être supprimées.
- Combien d’argent peut être dépensé lors de chaque révision mensuelle de démolition.

**La démolition automatique utilise la commande de démolition normale du jeu (le bulldozer) et coûte le même prix que la démolition manuelle.**

Elle ne supprime pas le zonage sous-jacent.

<a id="opt-demolishdestroyed"></a>
### Démolir les bâtiments détruits

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `DemolishDestroyed`</text>

Supprime automatiquement les ruines laissées par les incendies et les catastrophes.

Lorsque l’option est activée, le Ministère peut déblayer les bâtiments détruits sur les terrains résidentiels, commerciaux, industriels et agricoles une fois le délai d’attente configuré écoulé.

**Les ruines sont traitées avant les bâtiments abandonnés lors de chaque révision mensuelle de démolition.**

Lorsqu’elle est désactivée, aucune démolition automatique de ruines n’a lieu.

L’option distincte pour les ruines municipales et historiques devient elle aussi sans effet.

Chaque démolition reste soumise au prix de démolition du jeu, à la trésorerie disponible et au budget mensuel facultatif.

**Nécessite :** Ministère actif.

**Contrôle :** [Attendre (ruines)](#opt-destroyedmonths) et [Démolir les bâtiments municipaux et historiques détruits](#opt-demolishmunicipal).

Voir [Démolition automatique](demolition.md#demolition-destroyed).

<a id="opt-destroyedmonths"></a>
### Attendre (ruines)

<text color="secondary">Par défaut : 1 mois (30 jours) | Plage : 1–24 mois, par pas de 1 | INI : `DestroyedAfterDays`</text>

Détermine pendant combien de temps un bâtiment détruit doit rester en ruines avant que le Ministère puisse le déblayer.

La fenêtre d’options affiche des mois simulés, mais le Ministère enregistre le délai d’attente en jours simulés.

La conversion utilise :

```math
\text{Jours}=
\operatorname{round}\left(\frac{\text{Mois}\times365}{12}\right)
```

Exemples :

| Délai choisi | Jours enregistrés |
|---|---:|
| 1 mois | 30 |
| 3 mois | 91 |
| 6 mois | 183 |
| 12 mois | 365 |
| 24 mois | 730 |

Un délai plus long vous laisse plus de temps pour inspecter les ruines et décider d’agir manuellement.

Un délai plus court permet aux terrains zonés de redevenir disponibles plus tôt pour un nouvel aménagement.

**Le Ministère vérifie l’éligibilité à la démolition une fois par mois simulé.**

Une ruine est donc déblayée lors de la première révision mensuelle après qu’elle est devenue assez ancienne, et pas forcément le jour exact où le délai d’attente expire.

Le même délai s’applique aux ruines municipales et historiques si vous activez leur option de démolition distincte.

**Plage INI avancée :** 1–3 650 jours.

**Nécessite :** Démolir les bâtiments détruits et Ministère actif.

Voir [Démolition automatique](demolition.md#demolition-delays).

<a id="opt-demolishmunicipal"></a>
### Démolir les bâtiments municipaux et historiques détruits

<text color="secondary">Par défaut : désactivé | Valeurs : activé / désactivé | INI : `DemolishMunicipalAndHistoricRuins`</text>

Détermine si le Ministère peut déblayer automatiquement les ruines des bâtiments protégés.

Cela comprend les bâtiments municipaux et à usage spécial, tels que :

- Les écoles et les hôpitaux.
- Les commissariats et les casernes de pompiers.
- Les monuments et les récompenses.
- Les autres bâtiments posés à la main.
- Les parcelles situées dans des zones spéciales.
- Les bâtiments classés historiques.

**Lorsqu’elle est désactivée, ces ruines restent en place pour que vous vous en occupiez manuellement.**

Lorsqu’elle est activée, leurs ruines suivent le même délai d’attente que les bâtiments détruits ordinaires.

**Cette option n’autorise jamais la démolition de bâtiments municipaux ou historiques encore debout ou abandonnés.**

Elle ne s’applique qu’une fois que le jeu a déjà marqué la parcelle comme Détruite.

Par exemple, si un monument est détruit par une catastrophe, activer cette option permet au Ministère d’en supprimer les ruines après le délai d’attente.

Vous préférerez peut-être la laisser désactivée si vous voulez inspecter les monuments détruits ou d’autres bâtiments spéciaux avant que leurs vestiges ne disparaissent.

**Nécessite :** Démolir les bâtiments détruits et Ministère actif.

Voir [Démolition automatique](demolition.md#demolition-protected).

<a id="opt-demolishabandoned"></a>
### Démolir les bâtiments abandonnés

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `DemolishAbandoned`</text>

Déblaie automatiquement les bâtiments qui sont restés abandonnés pendant le délai d’attente configuré.

Le Ministère ne considère que les bâtiments abandonnés qui correspondent à vos choix de :

- Types de zone.
- Motifs d’abandon.
- Délai d’attente.

Les bâtiments abandonnés éligibles sont traités lors des révisions mensuelles, **après les ruines**, en commençant par les parcelles abandonnées depuis le plus longtemps.

Deux motifs d’abandon sont toujours considérés comme éligibles lorsque cette fonction est activée :

- Occupants expulsés.
- Aucun motif ou motif non reconnu.

Toutes les autres conditions de sécurité continuent de s’appliquer.

**Les bâtiments municipaux et historiques protégés abandonnés ne sont jamais démolis automatiquement.**

Lorsqu’elle est désactivée, aucun bâtiment abandonné n’est déblayé.

**Important :** les bâtiments abandonnés avant l’installation du Ministère conservent leur ancienneté d’origine. Certains peuvent déjà être éligibles dès la première révision mensuelle de démolition.

**Nécessite :** Ministère actif.

**Contrôle :** [Attendre (abandonnés)](#opt-abandonedmonths), les quatre options de zone et les cinq options de motif d’abandon.

Voir [Démolition automatique](demolition.md#demolition-abandoned).

<a id="opt-abandonedmonths"></a>
### Attendre (abandonnés)

<text color="secondary">Par défaut : 12 mois (365 jours) | Plage : 1–60 mois, par pas de 1 | INI : `AbandonedAfterDays`</text>

Détermine pendant combien de temps un bâtiment doit rester abandonné avant de devenir éligible à la démolition.

Par exemple, avec le délai par défaut de 12 mois, un bâtiment doit avoir été abandonné pendant au moins **365 jours simulés**.

Un délai plus long laisse plus de temps aux bâtiments pour être de nouveau occupés.

Un délai plus court permet de déblayer plus rapidement les terrains abandonnés.

**Le Ministère utilise le compteur d’ancienneté d’abandon propre à SimCity 4**, qui est enregistré dans la sauvegarde de la ville.

Le délai d’attente ne repart donc pas de zéro quand vous installez le plugin ou rouvrez la ville.

La fenêtre d’options affiche des mois, tandis que le fichier de configuration enregistre des jours selon la même conversion sur 365 jours décrite plus haut.

**Plage INI avancée :** 1–7 300 jours.

**Nécessite :** Démolir les bâtiments abandonnés et Ministère actif.

Voir [Démolition automatique](demolition.md#demolition-delays).

<a id="opt-zoneresidential"></a>
### Zone résidentielle

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `AbandonedZones` (`Residential`)</text>

Inclut les bâtiments résidentiels abandonnés dans la démolition automatique.

Cela couvre les constructions résidentielles de faible, moyenne et haute densité, y compris les maisons et les immeubles d’habitation.

Lorsque l’option est activée, les logements abandonnés éligibles peuvent être déblayés une fois le délai d’attente écoulé.

Lorsqu’elle est désactivée, les bâtiments résidentiels abandonnés sont préservés.

**Ce réglage ne concerne que les bâtiments abandonnés.**

Les ruines résidentielles peuvent toujours être déblayées avec **Démolir les bâtiments détruits**.

**Nécessite :** Démolir les bâtiments abandonnés et Ministère actif.

Voir [Démolition automatique](demolition.md#demolition-zones).

<a id="opt-zonecommercial"></a>
### Zone commerciale

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `AbandonedZones` (`Commercial`)</text>

Inclut les bâtiments commerciaux abandonnés dans la démolition automatique.

Cela couvre les constructions commerciales de faible, moyenne et haute densité, y compris les boutiques et les bureaux.

Lorsque l’option est activée, les bâtiments commerciaux abandonnés éligibles peuvent être déblayés, ce qui permet au terrain zoné de se développer de nouveau.

Lorsqu’elle est désactivée, les bâtiments commerciaux abandonnés ne sont pas touchés.

**Les ruines commerciales sont contrôlées séparément** par le réglage des bâtiments détruits.

**Nécessite :** Démolir les bâtiments abandonnés et Ministère actif.

Voir [Démolition automatique](demolition.md#demolition-zones).

<a id="opt-zoneindustrial"></a>
### Zone industrielle

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `AbandonedZones` (`Industrial`)</text>

Inclut les bâtiments industriels abandonnés dans la démolition automatique.

Cela couvre les constructions industrielles de moyenne et haute densité, comme les usines et les entrepôts.

Lorsque l’option est activée, les bâtiments industriels abandonnés éligibles peuvent être démolis une fois le délai d’attente écoulé.

Lorsqu’elle est désactivée, ils sont préservés.

**Les parcelles agricoles ne sont pas incluses dans ce réglage.** Elles ont leur propre option distincte.

Les ruines industrielles restent soumises au réglage des bâtiments détruits, quel que soit ce choix.

**Nécessite :** Démolir les bâtiments abandonnés et Ministère actif.

Voir [Démolition automatique](demolition.md#demolition-zones).

<a id="opt-zoneagriculture"></a>
### Zone agricole

<text color="secondary">Par défaut : désactivé | Valeurs : activé / désactivé | INI : `AbandonedZones` (`Agriculture`)</text>

Contrôle la démolition automatique des fermes et des champs agricoles abandonnés.

**Ce réglage est désactivé par défaut pour une raison importante.**

Dans SimCity 4, les champs agricoles peuvent sembler abandonnés dans le cadre de leur fonctionnement normal.

Cela ne signifie pas forcément que la ferme ne fonctionne plus.

Lorsqu’elle est désactivée, **toutes les parcelles agricoles abandonnées sont préservées**.

Lorsqu’elle est activée, le Ministère peut démolir les fermes et les champs abandonnés éligibles une fois le délai d’attente écoulé.

**Activer cette option peut supprimer des champs agricoles que vous vouliez conserver.**

Utilisez-la avec prudence, surtout dans les villes agricoles.

Les ruines sur des terres agricoles constituent un cas distinct. Elles peuvent toujours être déblayées avec **Démolir les bâtiments détruits**, même lorsque Zone agricole est désactivée.

**Nécessite :** Démolir les bâtiments abandonnés et Ministère actif.

Voir [Démolition automatique](demolition.md#demolition-farms).

<a id="opt-reasonlowdesirability"></a>
### Motif : quartier peu attractif

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `AbandonedReasons` (`LowDesirability`)</text>

Inclut les bâtiments abandonnés parce que leur emplacement est devenu peu attractif.

Les causes possibles comprennent :

- La pollution.
- La criminalité.
- Des services inadéquats.
- D’autres facteurs qui réduisent l’attractivité locale.

Lorsque l’option est activée, ces bâtiments peuvent être démolis une fois le délai d’attente configuré écoulé.

Lorsqu’elle est désactivée, ils sont préservés.

**Démolir un bâtiment abandonné ne corrige pas le problème d’attractivité sous-jacent.**

Par exemple, si la pollution reste excessive, le bâtiment de remplacement peut rencontrer des difficultés similaires.

Améliorer les environs peut parfois permettre au bâtiment d’origine d’être de nouveau occupé.

**Nécessite :** Démolir les bâtiments abandonnés et Ministère actif.

Voir [Démolition automatique](demolition.md#demolition-reasons).

<a id="opt-reasoncommute"></a>
### Motif : trajet trop long

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `AbandonedReasons` (`CommuteTime`)</text>

Inclut les bâtiments abandonnés parce que leurs occupants ne pouvaient pas rejoindre leur lieu de travail dans un temps de trajet acceptable.

Lorsque l’option est activée, les bâtiments éligibles peuvent être démolis une fois le délai d’attente écoulé.

Lorsqu’elle est désactivée, ils sont préservés.

**La démolition ne résout pas directement le problème de trajet.**

Par exemple, améliorer les transports publics ou l’accès routier pourrait permettre à un bâtiment résidentiel abandonné d’être de nouveau occupé.

Vous préférerez peut-être un délai d’attente plus long si vous améliorez régulièrement le réseau de transport de votre ville.

**Nécessite :** Démolir les bâtiments abandonnés et Ministère actif.

Voir [Démolition automatique](demolition.md#demolition-reasons).

<a id="opt-reasonlowdemand"></a>
### Motif : pas de demande

<text color="secondary">Par défaut : activé | Valeurs : activé / désactivé | INI : `AbandonedReasons` (`LowDemand`)</text>

Inclut les bâtiments abandonnés parce que la ville n’a plus assez de demande pour ce type de construction.

Lorsque l’option est activée, le Ministère peut déblayer ces bâtiments une fois le délai d’attente écoulé, ce qui laisse leur zonage disponible pour une croissance future.

Lorsqu’elle est désactivée, ils ne sont pas touchés.

**La demande peut évoluer à mesure que la ville se développe.**

Un bâtiment abandonné aujourd’hui faute de demande suffisante peut redevenir utile plus tard.

Le délai d’attente détermine le temps que vous accordez à cette possibilité avant que le Ministère ne déblaie la parcelle.

**Nécessite :** Démolir les bâtiments abandonnés et Ministère actif.

Voir [Démolition automatique](demolition.md#demolition-reasons).

<a id="opt-reasonnopower"></a>
### Motif : pas d’électricité

<text color="secondary">Par défaut : désactivé | Valeurs : activé / désactivé | INI : `AbandonedReasons` (`NoPower`)</text>

Contrôle la démolition des bâtiments abandonnés faute d’électricité.

**Cette option est désactivée par défaut.**

Un bâtiment abandonné à cause d’une panne d’électricité peut être de nouveau occupé une fois le courant rétabli.

Lorsqu’elle est désactivée, le Ministère préserve ces bâtiments.

Lorsqu’elle est activée, il peut les démolir une fois le délai d’attente configuré écoulé.

**Vérifiez le réseau électrique avant d’activer cette option.**

Un bâtiment abandonné peut avoir seulement besoin d’une connexion réparée ou d’une alimentation rétablie, plutôt que d’une démolition.

**Nécessite :** Démolir les bâtiments abandonnés et Ministère actif.

Voir [Démolition automatique](demolition.md#demolition-reasons).

<a id="opt-reasonnowater"></a>
### Motif : pas d’eau

<text color="secondary">Par défaut : désactivé | Valeurs : activé / désactivé | INI : `AbandonedReasons` (`NoWater`)</text>

Contrôle la démolition des bâtiments abandonnés faute d’eau.

Comme pour l’électricité, **cette option est désactivée par défaut**.

Les bâtiments abandonnés à cause du manque d’eau peuvent être de nouveau occupés lorsque l’approvisionnement en eau ou la connexion aux canalisations est rétabli.

Lorsqu’elle est désactivée, ces bâtiments sont préservés.

Lorsqu’elle est activée, le Ministère peut les déblayer une fois le délai d’attente écoulé.

**Vérifiez le réseau d’eau avant d’activer cette option.**

La démolition peut être inutile si le rétablissement du service permettrait au bâtiment de se rétablir.

**Nécessite :** Démolir les bâtiments abandonnés et Ministère actif.

Voir [Démolition automatique](demolition.md#demolition-reasons).

<a id="opt-budgetlimit"></a>
### Limiter le budget mensuel de démolition

<text color="secondary">Par défaut : désactivé | Valeurs : activé / désactivé | INI : `LimitMonthlyBudget`</text>

Ajoute une limite de dépenses facultative à la démolition automatique.

La limite s’applique aux **ruines et aux bâtiments abandonnés ensemble** lors de chaque révision mensuelle de démolition.

Lorsque l’option est activée, le Ministère suit les coûts de démolition des ordres déjà soumis pendant cette révision.

Une démolition n’est autorisée que si son coût ne dépasse pas le budget mensuel configuré.

**Si le prochain bâtiment chiffré dépasse la limite, toute la révision mensuelle de démolition s’arrête.**

Les bâtiments restants attendent le mois suivant.

Lorsqu’elle est désactivée, il n’y a pas de plafond de dépenses mensuel supplémentaire.

Toutefois, **la protection de la trésorerie reste toujours active**.

Le Ministère n’ordonne jamais plus de frais de démolition que la ville ne peut en payer avec la trésorerie disponible au début de la révision.

Les démolitions dont le prix est de 0 § ne consomment pas le budget mensuel facultatif.

**Nécessite :** Ministère actif.

**Contrôle :** [Budget mensuel](#opt-demolitionbudget).

Voir [Démolition automatique](demolition.md#demolition-money).

<a id="opt-demolitionbudget"></a>
### Budget mensuel

<text color="secondary">Par défaut : 1 000 § | Plage dans le jeu : 0–200 000 §, par pas de 1 000 § | INI : `MonthlyBudget`</text>

Fixe le montant maximal que le Ministère peut dépenser en démolition automatique lors d’une révision mensuelle, lorsque la limite budgétaire est activée.

Par exemple, avec un budget de **1 000 §**, le Ministère peut ordonner des démolitions éligibles tant que leur coût cumulé reste dans cette limite.

Si une démolition proposée dépasse la limite, la révision s’arrête et les candidats restants attendent le mois suivant.

**Que se passe-t-il selon les niveaux de budget ?**

| Budget mensuel | Effet |
|---|---|
| **0 §** | Seules les démolitions coûtant 0 § sont autorisées. |
| **1 000 §** | Jusqu’à 1 000 § peuvent être engagés par révision. |
| **5 000 §** | Jusqu’à 5 000 § peuvent être engagés par révision. |
| **20 000 §** | Jusqu’à 20 000 § peuvent être engagés par révision. |

Un budget plus important permet davantage de frais de démolition en un seul mois.

Un budget plus petit étale sur plus de mois un gros arriéré de bâtiments éligibles.

**Le budget mensuel ne l’emporte jamais sur la règle de la trésorerie.**

Par exemple, fixer un budget de démolition de 20 000 § ne permet pas au Ministère de dépenser 20 000 § si la ville ne dispose que de 5 000 §.

#### Et si la limite budgétaire est désactivée ?

Le contrôle du Budget mensuel devient indisponible, mais sa valeur enregistrée est conservée.

Si vous activez la limite plus tard, le budget précédent redevient actif.

Par exemple, la configuration fournie enregistre **1 000 §** même si la limite est désactivée par défaut.

L’activer ne réinitialise pas ce montant.

#### Puis-je fixer un budget plus élevé à la main ?

Oui.

La fenêtre d’options du jeu autorise un maximum de **200 000 §**.

Le fichier de configuration accepte des valeurs allant jusqu’à 2 000 000 000, bien que les valeurs plus grandes ne puissent pas être représentées entièrement dans la fenêtre d’options.

**Nécessite :** Limiter le budget mensuel de démolition et Ministère actif.

Voir [Démolition automatique](demolition.md#demolition-money).

---

[Retour au sommaire](index.md#contents) | [Retour au README](../../README.fr.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
