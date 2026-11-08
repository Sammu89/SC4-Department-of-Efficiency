[English](../en/water.md) | [Português](../pt/water.md) | **Français** | [Español](../es/water.md)

[Retour au sommaire](index.md#contents) | [Retour au README](../../README.fr.md)

<a id="water"></a>
## C. Eau

Le Ministère gère automatiquement le **budget d’approvisionnement en eau** de votre ville, afin de garantir qu’il y a assez d’eau pour vos bâtiments tout en maintenant les coûts de fonctionnement aussi bas que possible.

Il tient compte de :

- La quantité d’eau dont votre ville a besoin.
- La réserve de sécurité que vous avez choisie (**10 % par défaut**).
- L’eau que votre ville achète aux villes voisines ou leur vend.
- La capacité de production de vos installations d’eau.
- L’état de votre réseau de canalisations.

Contrairement à l’électricité, où chaque centrale a son propre réglage de financement, **tout l’approvisionnement en eau partage un seul pourcentage de budget**.

Le Ministère ajuste ce pourcentage pour fournir assez d’eau sans dépense inutile.

Il peut aussi protéger vos canalisations contre la dégradation causée par un financement insuffisant.

Les contrats d’eau régionaux sont inclus dans les calculs. Leur gestion automatique est expliquée dans [Accords avec les voisins](deals.md#neighbor-deals).

<a id="water-vanilla"></a>
### 1. Comment l’eau fonctionne dans SimCity 4

#### Un seul budget pour tout l’approvisionnement en eau

SimCity 4 gère le financement de l’eau par un seul budget de département.

Votre ville peut posséder plusieurs installations d’eau, notamment des pompes, des châteaux d’eau et des stations de traitement. Chaque installation a son propre coût d’entretien, mais **toutes les installations reçoivent le même pourcentage de financement**.

Par exemple, si vous réglez le financement de l’eau à 80 %, ce pourcentage s’applique à tout le département de l’eau.

Vous ne pouvez pas financer une pompe à 100 % et une autre à 60 %, comme vous pouvez le faire avec les centrales électriques.

Le jeu autorise normalement un financement de l’eau jusqu’à **120 %**, même si un maximum plus bas peut s’appliquer s’il est signalé par le jeu.

Contrairement à l’électricité, l’eau n’a pas de pourcentage de financement minimal acceptable intégré, ni de mécanisme de grève.

#### Demande, production et consommation d’eau

Le jeu suit plusieurs valeurs liées à l’eau :

| Terme | Ce que cela signifie |
|---|---|
| **Demande** | La quantité d’eau dont les bâtiments de votre ville ont besoin. |
| **Capacité de production** | La quantité d’eau que vos propres installations peuvent fournir à leur financement actuel. |
| **Consommation** | L’eau réellement livrée aux bâtiments. |
| **Importations** | L’eau achetée aux villes voisines. |
| **Exportations** | L’eau vendue aux villes voisines. |
| **Bâtiments alimentés en eau** | Les bâtiments qui reçoivent actuellement de l’eau. |
| **Bâtiments sans eau** | Les bâtiments sans service d’eau. |
| **Canalisations fragilisées** | Les canalisations qui se sont dégradées. |
| **Canalisations rompues** | Les canalisations qui ont lâché. |

**Comment les contrats d’eau régionaux influencent-ils votre approvisionnement ?**

D’après nos observations, SimCity 4 utilise l’eau importée avant de puiser dans la production locale.

Cela signifie qu’acheter de l’eau à une ville voisine peut réduire ce que vos propres installations ont besoin de produire.

Vendre de l’eau a l’effet inverse : **votre ville doit avoir assez de capacité pour alimenter à la fois ses propres bâtiments et ses contrats d’exportation**.

Le Ministère tient compte de ces contrats lorsqu’il calcule le financement nécessaire.

**Un détail important à l’ouverture d’une ville**

Juste après le chargement d’une ville, le jeu peut signaler zéro bâtiment alimenté en eau et zéro bâtiment sans eau.

Cela ne signifie pas forcément que tous les bâtiments ont de l’eau ou que la ville n’a aucune demande en eau.

Cela signifie généralement que la simulation de l’eau n’a pas encore mis à jour ses informations.

Le Ministère doit donc distinguer les lectures initiales manquantes des valeurs réelles avant de prendre des décisions de financement.

<a id="water-efficiency-curve"></a>
#### Comment le financement influence la production d’eau

Dans SimCity 4, réduire le financement de l’eau réduit aussi la quantité d’eau que vos installations peuvent produire.

Cependant, **la production ne diminue pas au même rythme que le financement**.

Le jeu utilise la courbe de rendement suivante :

| Financement de l’eau | Rendement de production |
|---:|---:|
| 0 % | 0 % |
| 50 % | 30 % |
| 90 % | 80 % |
| 100 % | 100 % |
| 120 % | 110 % |

**Qu’est-ce que cela signifie ?**

- À **50 % de financement**, votre ville n’a que **30 % de sa capacité normale de production d’eau**.
- À **90 % de financement**, elle a **80 % de la capacité normale**.
- À **100 % de financement**, elle a la pleine capacité normale.
- À **120 % de financement**, elle a **110 % de la capacité normale**.

Un financement supérieur à 100 % fournit donc de l’eau supplémentaire, mais à un coût plus élevé par unité de capacité.

De même, réduire trop le financement peut supprimer beaucoup plus de capacité de production que vous ne l’imaginez.

**Comment les pourcentages intermédiaires sont-ils calculés ?**

Entre les points de contrôle du jeu, le rendement varie en ligne droite.

Par exemple, entre 90 % et 100 % de financement, le rendement passe de 80 % à 100 %.

À 95 % de financement, le rendement de production est donc de 90 %.

**Interpolation de la courbe de rendement**

```math
y(x) = y_i + \frac{x-x_i}{x_{i+1}-x_i}(y_{i+1}-y_i)
\qquad \text{for } x_i \le x \le x_{i+1}
```

Où :

- **x** est le pourcentage de financement choisi.
- **(xᵢ, yᵢ)** et **(xᵢ₊₁, yᵢ₊₁)** sont les deux points qui l’encadrent sur la courbe.
- **y(x)** est le rendement de production obtenu.

En dehors de la plage définie de la courbe, la valeur du point d’extrémité le plus proche est utilisée.

**Exemples de rendement de production d’eau**

| Financement (%) | 30 | 50 | 60 | 70 | 75 | 78 | 80 | 90 | 95 | 100 | 105 | 110 | 120 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| Rendement (%) | 18 | 30 | 42,5 | 55 | 61,25 | 65 | 67,5 | 80 | 90 | 100 | 102,5 | 105 | 110 |

**Pourquoi est-ce important pour le Ministère ?**

À 100 % de financement, la production d’eau est la plus efficace en termes de capacité obtenue par simoléon dépensé.

Mais cela ne veut pas dire que chaque ville a besoin de 100 % de financement.

Imaginez une ville qui produit bien plus d’eau qu’elle n’en consomme. Réduire le budget de l’eau peut quand même fournir assez d’eau tout en économisant de l’argent.

Par exemple :

| Financement | Capacité de production | Coût par rapport à 100 % |
|---|---:|---:|
| 50 % | 30 % | 50 % |
| 80 % | 67,5 % | 80 % |
| 100 % | 100 % | 100 % |
| 120 % | 110 % | 120 % |

**Le Ministère cherche le niveau de financement le plus bas qui puisse couvrir les besoins en eau de la ville et sa réserve de sécurité**, tout en respectant ses autres protections.

Contrairement à l’électricité, il n’a pas besoin de comparer différentes combinaisons de financement pour des installations individuelles, car il n’y a qu’un seul pourcentage de budget de l’eau.

Voir [Comment le Ministère choisit le financement](#water-algorithm) pour le calcul.

#### La production d’eau ne se met pas à jour immédiatement

Il y a un délai important dans SimCity 4.

**Quand le financement de l’eau change, la capacité de production signalée par le jeu se met à jour au mois simulé suivant, pas immédiatement.**

Par exemple, lors des tests, une ville avait une capacité de production de **241 520 unités** à 100 % de financement.

Après avoir réduit le financement à 95 %, la capacité signalée par le jeu est devenue **217 368 unités** à la mise à jour mensuelle suivante.

C’est exactement 90 % de la capacité d’origine, conformément à la courbe de rendement.

Ce délai est important, car le Ministère ne doit pas prendre une capacité inchangée, lue juste après un ajustement du financement, pour la preuve que l’ajustement n’a eu aucun effet.

#### Comment la facture mensuelle d’eau est-elle calculée ?

La dépense totale du département de l’eau dépend de deux choses :

1. Les coûts d’entretien cumulés de toutes les installations d’eau.
2. Le pourcentage de financement actuel de l’eau.

Le jeu facture ces coûts proportionnellement au financement.

**Dépense mensuelle d’eau (jeu de base)**

```math
X(f) = \mathrm{round}\left(I\times\frac{f}{100}\right)
```

Où :

- **I** est le coût d’entretien mensuel cumulé de toutes les installations d’eau à 100 % de financement.
- **f** est le pourcentage de financement du département de l’eau.
- **X(f)** est la dépense mensuelle d’eau estimée.

**Exemple : une ville dont le budget d’eau normal est de 2 000 § par mois**

| Financement | Dépense mensuelle | Économie mensuelle |
|---|---:|---:|
| 100 % | 2 000 § | — |
| 90 % | 1 800 § | 200 § |
| 78 % | 1 560 § | 440 § |
| 50 % | 1 000 § | 1 000 § |

À 78 % de financement, la ville économise **440 § par mois** par rapport à 100 %.

Cependant, ce financement ne fournit que **65 % de la capacité de production normale**.

Le Ministère doit donc vérifier si une capacité de 65 % suffit pour satisfaire la demande, la réserve configurée et les éventuelles exportations d’eau.

La formule arrondit les dépenses au simoléon entier. En raison des calculs internes du jeu, la dépense réellement affichée peut différer d’un ou deux simoléons.

<a id="water-pipe-curve"></a>
#### Comment le financement influence les canalisations d’eau

Le financement de l’eau ne contrôle pas seulement la production. Il influence aussi l’**état de votre réseau de canalisations**.

Dans SimCity 4, les canalisations peuvent se dégrader progressivement quand le département de l’eau reçoit un financement insuffisant.

Le jeu utilise une courbe distincte pour déterminer si les canalisations se dégradent, restent stables ou se réparent d’elles-mêmes.

**Courbe de dégradation et de réparation des canalisations (jeu de base)**

| Financement de l’eau | Effet sur les canalisations |
|---:|---|
| 0 % | Dégradation (−0,010) |
| 20 % | Dégradation (−0,005) |
| 50 % | Ni dégradation ni réparation (0) |
| 100 % | Réparation (+0,020) |
| 110 % | Réparation plus rapide (+0,050) |

La courbe suit la même règle d’interpolation linéaire que celle utilisée pour le rendement de production.

À 120 % de financement, la valeur de réparation reste de +0,050, comme à 110 %.

**Le seuil le plus important est 50 % de financement.**

- **En dessous de 50 % :** les canalisations se dégradent progressivement.
- **À 50 % :** leur état reste stable.
- **Au-dessus de 50 % :** le réseau de canalisations se répare progressivement de lui-même.

Cela ne signifie **pas** que les canalisations éclatent immédiatement quand vous réduisez le financement sous 50 %.

La dégradation se produit avec le temps.

À mesure que les canalisations s’abîment, le jeu peut les signaler comme *fragilisées*. Si leur état se dégrade suffisamment, elles peuvent devenir des *canalisations rompues*.

Rétablir le financement à 50 % arrête toute nouvelle dégradation causée par la courbe de financement. Le relever au-dessus de 50 % permet au réseau de se rétablir.

**À quelle vitesse les canalisations se dégradent-elles ou se rétablissent-elles ?**

Le tableau suivant montre la courbe du jeu pour plusieurs niveaux de financement :

| Financement (%) | 0 | 10 | 20 | 30 | 35 | 40 | 50 | 60 | 75 | 78 | 90 | 100 | 105 | 110 | 120 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| Courbe des canalisations | −0,0100 | −0,0075 | −0,0050 | −0,0033 | −0,0025 | −0,0017 | 0 | +0,0040 | +0,0100 | +0,0112 | +0,0160 | +0,0200 | +0,0350 | +0,0500 | +0,0500 |

Une valeur plus négative signifie un effet de dégradation plus fort. Une valeur positive plus grande signifie un effet de réparation plus fort.

Par exemple, à 20 % de financement, le taux de dégradation représenté par la courbe est d’environ le double de celui à 30 %.

À 110 % de financement, la valeur de réparation est 2,5 fois celle à 100 %.

**Comment le Ministère utilise-t-il ces informations ?**

Le Ministère reconnaît si un niveau de financement provoque :

- Une dégradation des canalisations.
- Un état stable des canalisations.
- Une réparation des canalisations.

Lorsque la protection des canalisations est activée, ces informations l’aident à éviter les réductions de financement qui mettraient le réseau en danger.

**Une limite importante :** le Ministère ne calcule pas le nombre exact de jours simulés qu’il faudrait à une canalisation donnée pour se dégrader ou éclater. Son modèle utilise la direction de la courbe, pas une date de défaillance prédite.

Cela lui permet de protéger le réseau sans s’appuyer sur une estimation non vérifiée de la vitesse d’usure de chaque canalisation.
<a id="water-problem"></a>
### 2. Le problème de gestion

Gérer le financement de l’eau à la main peut sembler simple, puisque toute la ville partage un seul pourcentage de budget. Pourtant, plusieurs facteurs compliquent les choses.

- **Dépenses inutiles :** une ville peut produire bien plus d’eau qu’elle n’en a besoin, notamment après la construction d’une nouvelle pompe ou station de traitement. Maintenir le financement à 100 % peut gaspiller des centaines de simoléons chaque mois.
- **Risque de pénurie d’eau :** réduire trop le financement peut laisser des bâtiments sans eau. Pire, la capacité de production du jeu ne se met à jour qu’au mois simulé suivant, si bien que les conséquences d’un changement de financement peuvent ne pas être visibles tout de suite.
- **Contrats régionaux :** acheter de l’eau réduit ce que votre ville doit produire localement, alors que vendre de l’eau l’augmente.
- **Dégradation des canalisations :** un financement inférieur à 50 % fait se dégrader progressivement les canalisations d’eau. Les coûts de réparation et les conséquences de canalisations endommagées ne se reflètent pas directement dans le budget mensuel de l’eau.
- **Problèmes de connexion :** des bâtiments peuvent être sans eau même quand la ville a assez de capacité de production. Dans ces cas, augmenter le financement n’aidera pas. Le joueur doit corriger les raccordements des canalisations.

**Le Ministère résout cela en calculant la capacité d’eau dont votre ville a réellement besoin, en trouvant le niveau de financement approprié le plus bas et en protégeant le réseau de canalisations lorsque cette option est activée.**

<a id="water-algorithm"></a>
### 3. Comment le Ministère choisit le financement de l’eau

Le Ministère ne se contente pas de baisser le budget de l’eau dès que la production dépasse la demande.

Il suit plutôt quatre étapes :

1. Calculer la quantité d’eau que la ville doit produire localement.
2. Estimer la quantité d’eau qui peut être produite à chaque pourcentage de financement.
3. Repérer les niveaux de financement sûrs et abordables.
4. Choisir le pourcentage approprié le moins cher et l’appliquer dans le bon ordre.

Chaque calcul utilise les informations les plus récentes de la ville.

<a id="water-required-capacity"></a>
#### Étape 1 : de combien d’eau la ville a-t-elle besoin ?

Le Ministère calcule d’abord la capacité minimale de production d’eau que votre ville doit maintenir.

Il tient compte de :

- La demande en eau actuelle de votre ville.
- La réserve d’eau que vous avez choisie (**10 % par défaut**).
- L’eau achetée aux villes voisines.
- L’eau vendue aux villes voisines.

**La réserve ne s’applique qu’à l’eau que votre ville doit produire elle-même.**

Par exemple, si votre ville a besoin de 20 000 unités d’eau et produit tout localement, une réserve de 10 % signifie qu’elle doit maintenir une capacité d’au moins **22 000 unités**.

Si la ville achète une partie de cette eau à un voisin, la réserve ne s’applique qu’à la demande locale restante.

**Capacité d’eau locale requise**

```math
L = \left\lceil R \times \max(0,D-B) \right\rceil + S
\qquad
R = 1+\frac{r}{100}
```

Où :

- **D** est la demande totale en eau de la ville.
- **B** est l’eau achetée aux villes voisines sur laquelle on peut compter de façon fiable.
- **S** est l’eau vendue aux villes voisines.
- **r** est la Réserve d’eau configurée, 10 % par défaut.
- **R** est le multiplicateur de réserve (1,10 pour une réserve de 10 %).
- **L** est la capacité locale de production d’eau requise.

Le résultat est arrondi à l’unité d’eau supérieure.

**Exemple : une ville ayant besoin de 20 000 unités**

| Situation | Capacité locale requise |
|---|---:|
| Ni importation ni exportation d’eau | 22 000 unités |
| Importation de 5 000 unités | 16 500 unités |
| Exportation de 5 000 unités | 27 000 unités |

Sans contrat régional :

```math
L = \lceil 20000 \times 1.10 \rceil = 22000
```

Avec un achat de 5 000 unités :

```math
L = \lceil (20000-5000)\times1.10\rceil = 16500
```

Avec une vente de 5 000 unités :

```math
L = \lceil 20000\times1.10\rceil+5000=27000
```

Remarquez que **la réserve n’est pas ajoutée à l’eau exportée**. Les exportations doivent être couvertes, mais elles ne reçoivent pas de marge de sécurité supplémentaire.

De même, le Ministère n’achète pas d’eau en plus uniquement pour créer une réserve sur l’approvisionnement importé.

**Quels achats d’eau sont fiables ?**

Seuls les achats confirmés comme actifs sont considérés comme fiables.

Les contrats en attente, suspendus ou défaillants ne sont pas comptés comme de l’eau disponible.

Si le Ministère gère aussi les contrats d’eau régionaux, il calcule le financement avec les achats et les ventes inclus dans son plan de contrats. Les deux décisions sont coordonnées plutôt que prises indépendamment.

Voir [Accords avec les voisins](deals.md#neighbor-deals).

#### Étape 2 : quelle quantité d’eau la ville peut-elle produire à chaque niveau de financement ?

Une fois la capacité requise connue, le Ministère examine la production d’eau actuelle de la ville.

Il estime ensuite la quantité d’eau que les installations existantes pourraient produire à différents pourcentages de financement.

Comme tous les producteurs d’eau partagent le même réglage de financement, ce calcul est beaucoup plus simple que l’optimisation de l’électricité.

**Capacité de production d’eau projetée**

```math
C(v)=\left\lfloor C_{\text{actuel}}\times\frac{\eta(v)}{\eta(f_{\text{actuel}})}\right\rfloor
```

Où :

- **Cactuel** est la capacité de production actuellement signalée par le jeu.
- **factuel** est le pourcentage de financement actuel de l’eau, lu tel que le jeu le stocke.
- **v** est un nouveau pourcentage de financement possible.
- **η** est la courbe de rendement de production d’eau du jeu.
- **C(v)** est la capacité estimée au financement proposé, arrondie à l’unité inférieure.

En clair, **le Ministère utilise la capacité de production actuelle et la courbe de rendement du jeu pour prédire la quantité d’eau qu’un autre niveau de financement fournirait**.

**Exemple : une ville produisant 34 000 unités**

Supposons que la ville produise actuellement 34 000 unités à 100 % de financement.

À 78 % de financement, la courbe de rendement du jeu fournit 65 % de la capacité de production normale.

```math
C(78)=\left\lfloor34000\times\frac{65}{100}\right\rfloor=22100
```

Le Ministère estime donc qu’**un financement de 78 % fournirait 22 100 unités**.

Si la ville a besoin de 22 000 unités, réserve comprise, cela serait suffisant.

**Une limite importante**

Ce calcul suppose que tous les producteurs d’eau réagissent au financement du département selon la courbe de rendement standard.

Si le financement actuel donne un rendement nul, comme à 0 %, le Ministère ne peut pas utiliser cette formule pour estimer de façon fiable d’autres capacités. Dans cette situation, il n’établit pas de plan automatique de financement de l’eau.

<a id="water-funding-choice"></a>
#### Étape 3 : quels niveaux de financement sont autorisés ?

Le Ministère ne considère pas chaque niveau de financement possible sans restriction.

Les choix disponibles dépendent de vos réglages et de l’état actuel du réseau d’eau.

**Financement minimal**

| Protéger les canalisations | Minimum pour les nouveaux choix de financement |
|---|---:|
| **Activé (par défaut)** | **50 %** |
| Désactivé | 0 % |

Lorsque la protection des canalisations est activée, le Ministère maintient normalement le financement au niveau du point où la courbe de dégradation des canalisations du jeu devient neutre, ou au-dessus.

À 50 % de financement, les canalisations ne se dégradent ni ne se réparent d’elles-mêmes à cause du financement du département.

Au-dessus de 50 %, le réseau se répare progressivement.

**Financement maximal**

Le maximum normal est de **120 %**, ou une valeur plus basse si le jeu signale une limite de département plus basse.

**Et si vous avez réglé manuellement le financement sous 50 % ?**

Le Ministère inclut toujours le financement actuel parmi les choix possibles, même quand il se situe en dehors de sa plage de recherche normale.

Par exemple, si vous réglez manuellement le financement de l’eau à 40 %, ce niveau existant n’est pas automatiquement écarté de l’examen.

Cependant, lorsque la protection des canalisations est activée, le Ministère ne choisira pas de **nouvelle réduction** qui provoque une dégradation des canalisations.

Cette distinction compte : **protéger les canalisations ne veut pas dire que le Ministère relève automatiquement tous les budgets existants inférieurs à 50 %.** Le choix du financement suit toujours ses besoins de capacité et ses règles de sécurité.

#### Quand le Ministère refuse-t-il de réduire le financement ?

Même si un pourcentage plus bas permettait d’économiser, le Ministère refuse une réduction lorsque l’une des conditions suivantes s’applique :

**1. La ville connaît une véritable pénurie d’eau**

Si des bâtiments sont sans eau et que la capacité de production locale est inférieure à la demande, le Ministère ne réduit pas davantage le financement.

**2. Les canalisations d’eau sont déjà endommagées**

Lorsque **Protéger les canalisations** est activé, le Ministère refuse les réductions si le jeu signale des canalisations fragilisées ou rompues.

**3. La réduction provoquerait une dégradation des canalisations**

Lorsque la protection est activée, une réduction de financement est rejetée si la courbe des canalisations du jeu devenait négative.

Ces restrictions s’appliquent aux **réductions de financement**.

Elles n’empêchent pas le Ministère d’augmenter le financement pour améliorer l’approvisionnement en eau.

#### Étape 4 : choisir le financement le moins cher

Après avoir écarté les options risquées, le Ministère compare les niveaux de financement restants.

Son objectif est de sélectionner **la dépense mensuelle la plus basse qui fournit encore la capacité d’eau requise**.

**Optimisation du financement**

```math
f^* = \arg\min_{v\in V,\ C(v)\ge L} X(v)
```

Où :

- **V** est l’ensemble des pourcentages de financement autorisés.
- **C(v)** est la capacité d’eau estimée au financement **v**.
- **L** est la capacité locale requise de l’étape 1.
- **X(v)** est la dépense de fonctionnement mensuelle au financement **v**.
- **f\*** est le pourcentage de financement choisi.

Comme la capacité d’eau et la dépense de fonctionnement augmentent toutes deux avec le financement, **la solution la moins chère est normalement le pourcentage autorisé le plus bas qui fournit assez d’eau**.

Si deux choix coûtent la même chose, le Ministère préfère conserver le financement actuel.

**Exemple : 34 000 unités à plein financement**

Reprenons l’exemple précédent :

- Capacité actuelle à 100 % : 34 000 unités.
- Capacité requise, réserve comprise : 22 000 unités.
- Protection des canalisations : activée.

| Financement | Capacité projetée | Résultat |
|---|---:|---|
| 77 % | 21 675 | Insuffisant |
| **78 %** | **22 100** | **Suffisant** |
| 80 % | 22 950 | Suffisant, mais plus cher |
| 100 % | 34 000 | Capacité excédentaire inutile |

**Le Ministère choisit un financement de 78 %**, à condition qu’aucune des conditions de sécurité n’empêche la réduction.

Si le département coûte normalement 2 000 § par mois, le nouveau coût est d’environ **1 560 §**, soit une économie de **440 § par mois**.

**Et si même le financement maximal est insuffisant ?**

Le Ministère sélectionne le niveau de financement autorisé qui fournit la plus grande capacité disponible, normalement 120 %.

Si plusieurs choix fournissent la même capacité maximale, il préfère le moins cher.

Il ne réduit pas le financement quand la ville manque déjà de capacité.

Lorsque la gestion des contrats d’eau régionaux est activée, le Ministère peut aussi tenter d’acheter de l’eau pour couvrir la pénurie restante.

Si le problème persiste, le joueur peut recevoir un avis de conseiller.

Voir [Accords avec les voisins](deals.md#neighbor-deals) et [Avis des conseillers](safety.md#notices).

#### Comment les changements de financement sont-ils appliqués ?

Le Ministère applique directement le pourcentage de financement qu’il a choisi, sans déplacer progressivement le curseur du budget.

Cependant, **l’ordre des changements de financement et de contrats compte**.

- **Les augmentations de financement ont lieu en premier**, avant de modifier les contrats d’eau.
- **Les réductions de financement n’ont lieu qu’après** l’application des changements de contrats régionaux nécessaires.

Par exemple, supposons que le Ministère veuille réduire la production d’eau locale parce qu’il a trouvé un achat moins cher auprès d’une ville voisine.

Il s’assure d’abord que les changements de contrats régionaux sont en place, puis il réduit le budget de l’eau.

Cela évite de couper la production locale avant que l’approvisionnement alternatif ne soit organisé.

Chaque examen effectue au plus une étape coordonnée pour une ressource. Les changements restants sont réexaminés lors d’une évaluation ultérieure, avec des lectures de jeu à jour.

**L’objectif est de sécuriser la transition, pas seulement le niveau de financement final.**

<a id="water-cadence"></a>
#### À quelle fréquence le Ministère vérifie-t-il le financement de l’eau ?

Le Ministère vérifie régulièrement votre approvisionnement en eau et peut réagir plus tôt quand des changements importants surviennent.

Tous les délais se rapportent au temps simulé du jeu.

| Quand | Ce qui se passe |
|---|---|
| **À l’ouverture d’une ville** | Effectue la première évaluation de l’eau dès que les lectures du jeu sont prêtes et cohérentes. |
| **Chaque jour simulé** | Recherche les pénuries graves et les changements importants de la demande en eau, de la production ou des contrats régionaux. |
| **Tous les 10 jours simulés** | Effectue un examen régulier du financement de l’eau, compté à partir du dernier examen complet. |
| **Au début de chaque mois simulé** | Effectue un examen de l’eau. |
| **Quand un changement important est détecté** | Recalcule le plan d’eau lors de la vérification quotidienne, sans attendre le prochain examen programmé. |

Chaque examen terminé réinitialise l’intervalle d’examen régulier. Si une évaluation vient d’avoir lieu, le Ministère ne la répète pas immédiatement simplement parce qu’une autre vérification programmée était due.

**Qu’est-ce qui déclenche une évaluation anticipée ?**

Les vérifications quotidiennes du Ministère recherchent deux situations.

**1. Une véritable pénurie d’approvisionnement en eau**

Il y a pénurie lorsque la ville consomme moins d’eau qu’il ne lui en faut **et** que l’approvisionnement utilisable total est insuffisant.

**Déficit d’eau régional**

```math
W < D \qquad\text{and}\qquad C+U-S<D
```

Où :

- **W** est l’eau réellement consommée.
- **D** est la demande en eau de la ville.
- **C** est la capacité de production locale.
- **U** est l’eau réellement livrée par les contrats d’achat.
- **S** est l’eau vendue aux villes voisines.

Par exemple, si une ville a besoin de 20 000 unités mais que son approvisionnement utilisable total n’est que de 15 000, le Ministère peut recalculer le financement sans attendre le prochain examen régulier.

Mais imaginez que la ville ait besoin de 20 000 unités et en ait 25 000 de disponibles, alors que certains bâtiments restent sans eau.

Dans cette situation, le problème se trouve probablement dans le réseau de canalisations de la ville plutôt que dans une production insuffisante.

**Augmenter le budget n’aiderait pas forcément**, donc le Ministère enregistre le problème de connexion au lieu de déclencher un examen du financement pour cette seule raison.

Une pénurie déjà évaluée ne déclenche pas de nouveaux examens à répétition, sauf si la situation de l’approvisionnement change.

**2. Un changement important de l’offre ou de la demande en eau**

Le Ministère recalcule aussi le financement lorsque l’un des éléments suivants varie d’au moins **1 %** par rapport à l’examen précédent :

- La demande en eau.
- La capacité de production locale.
- Les importations d’eau sous contrat.
- Les exportations d’eau sous contrat.

**Seuil de changement important**

```math
|x-x_0|\ge0.01\times\max(1,|x_0|)
```

Où :

- **x** est la dernière lecture.
- **x₀** est la valeur enregistrée lors de l’examen précédent.

Par exemple, si la demande passe de 20 000 à 20 300 unités, c’est une hausse de 1,5 %.

Le Ministère peut donc recalculer le financement lors de sa prochaine vérification quotidienne.

**Les lectures inchangées ne déclenchent pas d’évaluations inutiles.**

Cela garde le système réactif sans recalculer sans cesse la même situation.

<a id="water-regional"></a>
#### Comment les contrats d’eau régionaux influencent le financement

Les achats et ventes d’eau sont pris en compte pour calculer la quantité d’eau que vos propres installations doivent produire.

Les règles sont simples :

- **Les achats réduisent la demande locale.** Seuls les achats actifs de façon fiable comptent comme de l’eau disponible.
- **Les achats ne peuvent pas couvrir plus que la demande totale de la ville** dans le calcul de la capacité locale.
- **Les ventes augmentent le besoin de production locale.** La ville doit fournir assez d’eau à la fois pour ses propres besoins et pour ses obligations d’exportation.
- **Les ventes existantes sont traitées prudemment.** Quand le Ministère ne gère pas les contrats, il compte chaque vente listée comme une obligation, quel que soit son statut, ou les exportations signalées par le jeu si ce nombre est supérieur.
- **Les ventes ne sont pas maintenues pendant un véritable déficit d’eau.** Le planificateur de contrats donne la priorité à l’approvisionnement de la ville elle-même.

#### Comment le Ministère décide-t-il si un contrat est rentable ?

Un contrat d’eau n’est pas automatiquement intéressant du simple fait qu’une autre ville propose d’acheter ou de vendre de l’eau.

Le Ministère compare le prix du contrat avec le coût de fonctionnement supplémentaire, ou l’économie, qu’il créerait.

Par exemple :

- **Vendre de l’eau :** le Ministère calcule le financement supplémentaire qu’il faudrait pour produire l’eau vendue.
- **Acheter de l’eau :** il calcule le financement local qui pourrait être économisé en s’appuyant sur l’eau achetée.

**Coût marginal de l’eau**

```math
\Delta(q)=X^*(L+q)-X^*(L)
```

Où :

- **L** est la capacité d’eau locale requise actuelle.
- **q** est la quantité du contrat, positive pour une vente et négative pour un achat.
- **X\*(n)** est la dépense mensuelle d’eau la plus basse nécessaire pour fournir au moins **n** unités.
- **Δ(q)** est la variation des dépenses de fonctionnement mensuelles.

**Pour une vente d’eau**, le Ministère compare l’argent reçu avec la dépense de production locale supplémentaire.

Une vente n’est intéressante que si le voisin paie plus que le coût supplémentaire.

**Pour un achat d’eau**, le Ministère compare le prix du contrat avec les économies réalisées en réduisant la production locale.

Un achat n’est intéressant que s’il coûte moins que ces économies.

**Exemple**

Supposons que produire 5 000 unités d’eau supplémentaires exige d’augmenter le budget mensuel du département de 300 §.

Une ville voisine propose d’acheter ces 5 000 unités.

| Paiement mensuel du voisin | Résultat économique |
|---|---|
| 200 § | Pas intéressant : perte de 100 § |
| 300 § | Équilibre |
| 400 § | Intéressant : gain de 100 § |

C’est pourquoi le Ministère considère **la variation réelle des coûts de production**, au lieu de supposer que chaque contrat régional est bénéfique.

Les règles complètes de tarification et de gestion des contrats sont expliquées dans [Accords avec les voisins](deals.md#neighbor-deals).

#### Que se passe-t-il si la gestion automatique de l’eau ou des contrats est désactivée ?

Les deux fonctions peuvent fonctionner indépendamment.

| Réglages | Ce qui se passe |
|---|---|
| **Gérer l’eau : Activé** / **Gérer les accords d’eau : Activé** | Le Ministère peut optimiser à la fois le financement de l’eau et les contrats d’eau régionaux. |
| **Gérer l’eau : Activé** / **Gérer les accords d’eau : Désactivé** | Le financement de l’eau est optimisé en tenant compte de vos contrats existants, que le Ministère laisse inchangés. |
| **Gérer l’eau : Désactivé** / **Gérer les accords d’eau : Activé** | Le financement de l’eau reste à son niveau actuel, mais les contrats d’eau régionaux peuvent quand même être gérés. |
| **Les deux désactivés** | Le Ministère ne modifie automatiquement ni le financement de l’eau ni les contrats d’eau. |

La gestion automatique des contrats d’eau régionaux exige aussi que l’option principale **Gérer les accords avec les voisins** soit activée.

<a id="water-farm-icon"></a>
#### Une petite amélioration visuelle : les avertissements d’eau agricoles

Le Ministère apporte aussi un petit ajustement à l’interface du jeu.

**Il masque l’icône d’avertissement d’origine « pas d’eau » au-dessus des parcelles agricoles.**

Ce changement ne touche que l’icône d’avertissement affichée au-dessus des fermes.

Il ne fait **pas** ceci :

- Alimenter les fermes en eau.
- Modifier leur demande en eau.
- Modifier les décisions du jeu concernant les bâtiments agricoles.
- Modifier le système d’eau lui-même.
- Masquer les avertissements d’eau des bâtiments résidentiels, commerciaux ou industriels.

L’ajustement est appliqué au démarrage de SimCity 4 et ne dépend pas des réglages de gestion du Ministère.

Il ne s’applique qu’à la version du jeu prise en charge. Sur les autres versions, le Ministère laisse ce comportement de l’interface inchangé.

<a id="water-safety"></a>
### 4. Conditions de sécurité

Le Ministère suit plusieurs règles de sécurité pour éviter de laisser votre ville sans eau ou d’endommager son réseau de canalisations.

#### Avant de modifier le financement de l’eau

Le Ministère ne modifie le financement que lorsqu’il peut se fier aux informations du jeu.

Il vérifie que :

- Le département de l’eau et son financement actuel peuvent être lus correctement.
- La demande en eau et la capacité de production sont disponibles, avec une demande supérieure à zéro.
- Le jeu a mis à jour ses décomptes de bâtiments alimentés et non alimentés en eau.
- La consommation d’eau signalée est cohérente avec la demande.
- Chaque producteur d’eau utilise le système de financement standard du jeu.

**Si l’une de ces vérifications échoue, le Ministère laisse le financement de l’eau inchangé.**

C’est particulièrement important juste après l’ouverture d’une ville, quand certaines statistiques d’eau peuvent ne pas encore être disponibles.

Pour plus de précision technique, le Ministère accepte une petite différence entre la consommation et la demande :

```math
W \le 1.01 \times D + 1
```

Où **W** est la consommation d’eau signalée et **D** la demande.

Une consommation supérieure à cette limite est traitée comme une lecture incohérente.

Si un seul producteur d’eau utilise un mécanisme de financement non pris en charge, la gestion automatique du financement de l’eau est désactivée pour la session de cette ville plutôt que de risquer des calculs incorrects.

<a id="water-no-cut"></a>
#### Quand le Ministère refuse-t-il de réduire le financement ?

Même quand un budget plus bas pourrait économiser de l’argent, le Ministère refuse une réduction dans les situations suivantes :

| Situation | Pourquoi la réduction est refusée |
|---|---|
| **Production d’eau insuffisante** | Des bâtiments sont sans eau et la capacité de production est déjà inférieure à la demande. Couper le financement aggraverait la pénurie. |
| **Canalisations endommagées** | Avec **Protéger les canalisations** activé, toute canalisation fragilisée ou rompue empêche les réductions de financement. |
| **Risque de dégradation des canalisations** | Avec la protection des canalisations activée, le Ministère ne réduit pas le financement sous le seuil de 50 %. |
| **Capacité insuffisante après la réduction** | Le financement proposé ne fournirait pas assez d’eau pour la demande, la réserve et les exportations. |
| **Aucun niveau de financement approprié** | Si même le meilleur réglage disponible ne peut pas satisfaire le besoin, le Ministère augmente le financement quand c’est possible au lieu de le réduire. |
| **Informations peu fiables ou modifiées** | Les lectures sont incomplètes, incohérentes ou ont changé depuis le calcul du plan. |
| **Financement fixe** | Le financement de l’eau n’est pas sous gestion automatique. |

**Chaque réduction proposée est vérifiée deux fois :** une fois lors du calcul du plan, et de nouveau juste avant l’application du nouveau financement.

Cela évite que le Ministère utilise un calcul périmé si la situation de votre ville change.

**Et si certains bâtiments sont sans eau, mais que la production est suffisante ?**

C’est différent.

Par exemple, une ville peut produire 30 000 unités alors qu’elle n’en a besoin que de 20 000, et avoir pourtant des bâtiments sans eau parce qu’ils sont déconnectés du réseau de canalisations.

Dans ce cas, **augmenter le financement ne résoudra pas le problème de connexion**.

Le Ministère peut tout de même réduire le budget, tant que la capacité requise et les règles de protection des canalisations restent respectées.

#### Que se passe-t-il si un changement de financement échoue ?

Avant de modifier le budget, le Ministère vérifie que le financement actuel correspond toujours à la valeur utilisée dans son calcul.

Si vous avez déplacé le curseur à la main, le changement prévu est ignoré.

Après avoir appliqué un changement, il relit le financement pour vérifier que le jeu a accepté la nouvelle valeur.

Si la vérification échoue :

1. Le Ministère tente de rétablir le financement précédent.
2. Les changements automatiques du financement de l’eau s’arrêtent pour le reste de la session de la ville en cours.
3. La gestion du financement de l’eau peut redémarrer quand vous rouvrez la ville.

**Les contrats d’eau régionaux ne sont pas concernés par cet arrêt de sécurité.** Leur gestion peut continuer indépendamment.

<a id="water-settings"></a>
### 5. Réglages

La gestion de l’eau peut être configurée dans **Options de jeu → Ministère de l’Optimisation**, sur la page **Services**.

| Réglage | Par défaut | Plage | Ce qu’il fait |
|---|---|---|---|
| [Ministère actif](options.md#opt-master) | Activé | Activé / Désactivé | Active ou désactive l’ensemble du Ministère. |
| [Gérer l’eau](options.md#opt-water) | Activé | Activé / Désactivé | Ajuste automatiquement le financement du département de l’eau de la ville. |
| [Réserve d’eau](options.md#opt-waterreserve) | 10 % | 10–50 % | Définit la capacité de production d’eau locale excédentaire minimale. |
| [Protéger les canalisations](options.md#opt-waterpipes) | Activé | Activé / Désactivé | Empêche les réductions qui pourraient dégrader les canalisations ou gêner la remise en état des canalisations endommagées. |
| [Gérer les accords avec les voisins](options.md#opt-deals) | Activé | Activé / Désactivé | Active la gestion automatique des contrats régionaux. |
| [Gérer les accords d’eau](options.md#opt-tradewater) | Activé | Activé / Désactivé | Active les achats et ventes d’eau automatiques. |
| [Acheter à une ville pour vendre à une autre](options.md#opt-arbitrage) | Désactivé | Activé / Désactivé | Permet au Ministère d’acheter et de vendre de l’eau dans le même plan régional. |

#### Comment fonctionne la réserve d’eau ?

La réserve par défaut est de **10 %**.

Par exemple, si votre ville doit produire 20 000 unités localement, le Ministère vise à maintenir une capacité d’au moins **22 000 unités**.

La réserve est un minimum, pas une cible exacte.

Le Ministère peut conserver plus de capacité lorsque les pourcentages de financement disponibles ou le réglage de protection des canalisations l’exigent.

Augmenter la réserve fournit une capacité de production excédentaire plus importante, mais peut aussi augmenter vos dépenses d’eau mensuelles.

#### À quoi sert Protéger les canalisations ?

Lorsque cette option est activée, le Ministère :

- Utilise normalement **50 % comme minimum pour les nouveaux choix de financement**.
- Ne réduit jamais le financement à un niveau qui provoque une dégradation des canalisations.
- Refuse les réductions tant que le jeu signale des canalisations fragilisées ou rompues.

**Il n’augmente pas automatiquement le financement uniquement pour réparer les canalisations.**

Si vous réglez manuellement le financement sous 50 %, le Ministère peut le laisser ainsi tant que la production reste suffisante. Il relèvera le financement si nécessaire pour satisfaire le besoin d’approvisionnement en eau, dans sa plage de financement protégée normale.

Si vous désactivez la protection des canalisations, le Ministère peut choisir un financement aussi bas que **0 %**, à condition que les besoins d’approvisionnement en eau puissent toujours être satisfaits.

Cela peut économiser davantage d’argent, mais laisse le réseau de canalisations se dégrader avec le temps.

#### Réglage avancé (fichier INI uniquement)

| Réglage | Par défaut | Plage autorisée | Ce qu’il fait |
|---|---|---|---|
| `WaterCadenceDays` | 10 | 1–30 | Définit l’intervalle d’examen régulier du financement de l’eau, en jours simulés. |

Par exemple, définir `WaterCadenceDays=5` fait avoir lieu les examens réguliers tous les cinq jours simulés.

Les vérifications quotidiennes des changements importants et l’examen au début de chaque mois simulé restent distincts.

<a id="water-examples"></a>
### 6. Exemples détaillés

Les exemples suivants montrent comment le Ministère choisit le financement de l’eau et comment ses protections de sécurité influencent le résultat.

Tous les exemples utilisent la **réserve d’eau de 10 %** par défaut, sauf indication contraire.

Les capacités de production et les dépenses du service sont données à titre d’illustration.

#### Exemple 1 : réduire le financement de l’eau sans créer de pénurie

Prenons une ville avec :

- **Demande en eau :** 20 000 unités.
- **Capacité de production actuelle :** 34 000 unités.
- **Financement actuel :** 100 %.
- **Dépense mensuelle d’eau à 100 % :** 2 000 §.
- **Accords régionaux :** aucun.
- **Canalisations endommagées :** aucune.
- **Bâtiments sans eau :** aucun.

**Étape 1 : calculer la capacité nécessaire**

Avec une réserve de 10 % :

```math
L = \lceil 20000 \times 1.10 \rceil = 22000
```

La ville a besoin d’une capacité de **22 000 unités**.

**Étape 2 : trouver le financement suffisant le plus bas**

| Financement | Capacité attendue | Dépense mensuelle | Résultat |
|---|---:|---:|---|
| 77 % | 21 675 | 1 540 § | Insuffisant |
| **78 %** | **22 100** | **1 560 §** | **Suffisant** |
| 100 % | 34 000 | 2 000 § | Capacité excédentaire inutile |

**Le Ministère choisit un financement de 78 %.**

À ce niveau, la ville conserve assez d’eau pour sa demande et sa réserve.

La courbe de réparation des canalisations est elle aussi positive (+0,0112) : la réduction ne provoque donc pas de dégradation des canalisations.

**Économie mensuelle : 440 §.**

#### Exemple 2 : combien coûte la protection des canalisations ?

Imaginons une ville avec :

- **Demande en eau :** 10 000 unités.
- **Capacité de production à 100 % :** 60 000 unités.
- **Dépense mensuelle à 100 % :** 3 000 §.

Avec une réserve de 10 %, elle n’a besoin que de :

```math
L = 10000 \times 1.10 = 11000
```

La ville dispose d’une capacité de production d’eau bien supérieure à ses besoins.

Comparons le résultat avec la protection des canalisations activée puis désactivée.

**Avec Protéger les canalisations activé**

Le Ministère ne peut pas choisir un nouvel objectif de financement inférieur à 50 %.

À 50 % de financement, l’efficacité de production est de 30 % :

```math
C(50)=60000\times0.30=18000
```

C’est largement suffisant pour fournir les 11 000 unités requises.

**Le Ministère choisit un financement de 50 %**, pour un coût de **1 500 § par mois**.

Le réseau de canalisations reste stable.

**Avec Protéger les canalisations désactivé**

Le Ministère peut envisager des niveaux de financement plus bas.

| Financement | Capacité attendue | Résultat |
|---|---:|---|
| 30 % | 10 800 | Insuffisant |
| **31 %** | **11 160** | **Suffisant** |
| 50 % | 18 000 | Plus que nécessaire |

Sans la protection des canalisations, un **financement de 31 %** suffit.

La dépense mensuelle tombe à **930 §**.

Cependant, la courbe de dégradation des canalisations devient négative, d’environ −0,0032.

Le réseau s’usera progressivement.

**Le compromis financier**

| Réglage | Financement | Dépense mensuelle | Effet sur les canalisations |
|---|---:|---:|---|
| **Protection activée** | 50 % | 1 500 § | Stable |
| Protection désactivée | 31 % | 930 § | Dégradation progressive |

**Dans cet exemple, la protection des canalisations coûte 570 § de plus par mois.**

Cette dépense supplémentaire évite la dégradation causée par un sous-financement.

Savoir si l’économie vaut les dégâts à long terme est une décision que le joueur peut prendre en activant ou en désactivant la protection.

#### Exemple 3 : acheter et vendre de l’eau en même temps

Revenons à la ville de l’exemple 1 :

- **Demande en eau :** 20 000 unités.
- **Capacité de production normale :** 34 000 unités.
- **Coût mensuel à 100 % :** 2 000 §.

Cette fois, la ville a des accords régionaux :

- **Eau achetée :** 5 000 unités.
- **Eau vendue :** 8 000 unités.

L’achat est confirmé comme actif.

**Étape 1 : calculer la capacité locale nécessaire**

```math
L = \lceil(20000-5000)\times1.10\rceil+8000
```

```math
L=16500+8000=24500
```

La ville a besoin d’une capacité de production locale de **24 500 unités**.

**Étape 2 : choisir le financement**

| Financement | Capacité attendue | Résultat |
|---|---:|---|
| 83 % | 24 225 | Insuffisant |
| **84 %** | **24 650** | **Suffisant** |
| 100 % | 34 000 | Plus que nécessaire |

**Le Ministère choisit un financement de 84 %.**

La dépense mensuelle d’eau devient :

```math
X(84)=2000\times0.84=1680
```

**Dépense mensuelle du service des eaux : 1 680 §**, hors coûts et revenus des accords régionaux.

Cet exemple montre pourquoi le Ministère doit examiner ensemble les achats et les ventes plutôt que de se limiter à la demande propre de la ville.

#### Exemple 4 : vendre de l’eau est-il rentable ?

Reprenons la ville de l’exemple 1, sans accords régionaux.

Son financement le moins cher est actuellement :

- **Financement de 78 %.**
- **22 100 unités de capacité de production.**
- **1 560 § par mois.**

Une ville voisine propose maintenant d’acheter **5 000 unités d’eau**.

Pour approvisionner cette vente, la capacité locale nécessaire augmente :

```math
L=22000+5000=27000
```

Le Ministère vérifie quel financement est nécessaire :

| Financement | Capacité attendue | Dépense mensuelle |
|---|---:|---:|
| 78 % | 22 100 | 1 560 § |
| 89 % | 26 775 | 1 780 § |
| **90 %** | **27 200** | **1 800 §** |

**Vendre l’eau supplémentaire exige de faire passer le financement de 78 % à 90 %.**

La dépense supplémentaire est de :

```math
\Delta=1800-1560=240
```

**La vente coûte 240 § de plus par mois en production d’eau locale.**

Le Ministère peut maintenant comparer ce coût à l’argent proposé par la ville voisine :

| Paiement mensuel | Résultat avant les autres considérations liées à l’accord |
|---|---|
| 200 § | Perte de 40 § |
| 240 § | Équilibre |
| 300 § | Gain de 60 § |

**La vente n’est rentable que si le voisin paie plus de 240 § par mois**, sous réserve des autres règles de sécurité des accords.

C’est ainsi que le Ministère évalue les échanges régionaux d’après les coûts de production réels, au lieu d’accepter tous les accords proposés.

Voir [Accords avec les voisins](deals.md#neighbor-deals).

#### Exemple 5 : des canalisations endommagées empêchent une économie

Revenons à la ville de l’exemple 1.

Normalement, le Ministère réduirait le financement de l’eau de 100 % à 78 %, ce qui économiserait 440 § par mois.

Cependant, le jeu signale maintenant **trois canalisations en mauvais état**.

Avec **Protéger les canalisations** activé, le Ministère refuse la réduction.

| Situation | Financement | Dépense mensuelle |
|---|---:|---:|
| Aucune canalisation endommagée | 78 % | 1 560 § |
| **Trois canalisations en mauvais état** | **100 %** | **2 000 §** |

**Le Ministère maintient le financement à 100 % tant que les canalisations sont endommagées.**

À ce niveau de financement, la courbe des canalisations est positive (+0,020), ce qui permet au réseau de se rétablir.

Quand le jeu signale zéro canalisation en mauvais état et zéro canalisation éclatée, le Ministère peut ramener le financement à 78 % lors de sa prochaine évaluation.

**Le Ministère augmente-t-il le financement pour réparer les canalisations plus vite ?**

Non.

Il conserve le financement existant tant que les canalisations sont endommagées, mais ne l’augmente pas dans le seul but d’accélérer les réparations.

C’est une distinction délibérée entre **empêcher les coupes dangereuses** et **gérer activement les réparations des canalisations**.

#### Exemple 6 : des bâtiments sans eau ne signifient pas toujours une production insuffisante

Imaginons une ville avec :

- **Demande en eau :** 20 323 unités.
- **Capacité de production :** 34 081 unités.
- **Bâtiments sans eau :** 55.

À première vue, les 55 bâtiments sans eau laissent penser à une pénurie d’eau.

Cependant, la ville dispose déjà de bien plus de capacité de production que nécessaire pour couvrir sa demande.

Le problème vient probablement de bâtiments mal raccordés au réseau d’eau.

**Le Ministère n’augmente pas automatiquement le financement pour résoudre un problème de raccordement.**

Il peut même encore réduire le financement, à condition que la nouvelle capacité respecte la réserve requise et les règles de protection des canalisations.

Considérons maintenant une situation différente :

- **Demande en eau :** 20 323 unités.
- **Capacité de production :** 19 000 unités.
- **Bâtiments sans eau :** 55.

Ici, la capacité de production est réellement inférieure à la demande.

**Le Ministère refuse toute réduction de financement tant que la pénurie n’est pas résolue.**

La distinction est importante : **le manque d’eau causé par une production insuffisante peut être traité par le financement ; celui causé par des canalisations déconnectées ne le peut pas.**

#### Exemple 7 : même le financement maximal ne suffit pas

Prenons une ville avec :

- **Demande en eau :** 35 000 unités.
- **Capacité de production à 100 % :** 34 000 unités.
- **Réserve d’eau :** 10 %.
- **Accords régionaux :** aucun.

La capacité nécessaire est de :

```math
L=35000\times1.10=38500
```

La ville a besoin de **38 500 unités**.

Cependant, même à 120 % de financement, l’efficacité de production d’eau n’est que de 110 %.

```math
C(120)=34000\times1.10=37400
```

**Capacité maximale disponible : 37 400 unités.**

Cela fait **1 100 unités de moins que la capacité requise**, réserve comprise.

Le Ministère porte donc le financement de l’eau à **120 %**, le réglage le plus élevé possible.

Si la gestion des accords régionaux d’eau est activée, il peut aussi essayer d’acheter de l’eau supplémentaire à une ville voisine.

Sinon, le joueur doit augmenter la capacité de production locale, par exemple en construisant une autre pompe à eau.

**Le financement ne peut pas produire plus d’eau que la capacité maximale du jeu ne le permet.**

<a id="water-limitations"></a>
### 7. Limites connues

Le Ministère optimise automatiquement le financement de l’eau, mais plusieurs limites viennent de SimCity 4 lui-même.

#### Production d’eau et financement

- **La capacité de production se met à jour avec un délai.** Après un changement de financement, SimCity 4 met normalement à jour la capacité d’eau indiquée au mois simulé suivant. D’ici là, le Ministère peut temporairement calculer un nouveau plan à partir d’une capacité qui reflète encore l’ancien financement. La mise à jour ultérieure de la capacité déclenche une nouvelle évaluation, ce qui lui permet de corriger le plan.
- **Il n’existe qu’un seul pourcentage de financement de l’eau.** Le Ministère ne peut pas financer différemment chaque pompe, château d’eau ou station de traitement.
- **Le financement ne peut pas réparer tous les problèmes d’approvisionnement.** Une source d’eau polluée ou à l’arrêt peut produire moins d’eau. Augmenter le budget ne supprime pas le problème de fond.
- **Le financement maximal a des limites.** Si la production reste insuffisante à 120 %, la ville a besoin de plus de capacité de production d’eau ou d’achats régionaux fiables.

#### Protection des canalisations

- **La protection empêche les coupes ; elle ne gère pas activement les réparations.** Le Ministère n’augmente pas le financement simplement pour réparer plus vite les canalisations en mauvais état ou éclatées.
- **Les réseaux sous-financés manuellement ne sont pas automatiquement ramenés à 50 %.** Si vous fixez vous-même un financement inférieur à 50 % et que la production reste suffisante, le Ministère peut laisser ce financement tel quel. Il l’augmente quand il faut plus de production, en utilisant la plage protégée normale.
- **La dégradation des canalisations n’est pas prédite dans le temps.** Le Ministère utilise la courbe de financement du jeu pour déterminer si les canalisations se dégradent ou se rétablissent, mais ne calcule pas combien de mois une canalisation donnée mettra à céder.
- **Désactiver la protection permet la dégradation.** Avec **Protéger les canalisations** désactivé, le Ministère peut choisir un financement inférieur à 50 % si le besoin en eau peut tout de même être couvert.

#### Distribution de l’eau

- **Des bâtiments sans eau peuvent avoir des problèmes de raccordement.** Une production suffisante à l’échelle de la ville ne garantit pas que l’eau atteigne chaque bâtiment. Le Ministère ne peut pas réparer les raccordements manquants.
- **Le Ministère ne construit pas d’infrastructures.** Les nouvelles pompes, les châteaux d’eau et les réseaux de canalisations restent à la charge du joueur.

#### Compatibilité des bâtiments

**Chaque producteur d’eau doit utiliser le mécanisme de financement standard de SimCity 4.**

Si ne serait-ce qu’un seul bâtiment producteur d’eau utilise un système de financement personnalisé incompatible, le Ministère ne gère pas automatiquement le budget de l’eau de la ville.

Cette restriction empêche le Ministère d’effectuer des calculs fondés sur des hypothèses qui ne valent pas pour tous les producteurs.

**Le but du Ministère est de maintenir une production d’eau suffisante au moindre coût sûr, et non de remplacer la responsabilité du joueur envers les infrastructures d’eau.**

[Retour au sommaire](index.md#contents) | [Retour au README](../../README.fr.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
