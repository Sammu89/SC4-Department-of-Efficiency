[English](../en/garbage.md) | [Português](../pt/garbage.md) | **Français** | [Español](../es/garbage.md)

[Retour au sommaire](index.md#contents) | [Retour au README](../../README.fr.md)

<a id="garbage-landfills"></a>
## D. Déchets et décharges

Le Ministère aide votre ville à gérer efficacement ses déchets, en gardant les coûts de la gestion des déchets aussi bas que possible sans laisser les ordures s’accumuler inutilement.

Il assure quatre fonctions distinctes :

1. **Financement automatique de la gestion des déchets :** ajuste le budget de la gestion des déchets pour traiter les déchets de la ville, y compris la réserve de sécurité que vous avez choisie.

2. **Accords régionaux sur les déchets :** gère les importations et exportations de déchets lorsque la gestion des accords avec les voisins est activée. Voir [Accords avec les voisins](deals.md#neighbor-deals).

3. **Conseil sur la taille de la décharge (facultatif) :** recommande d’agrandir ou de réduire les zones de décharge lorsque cela pourrait réduire sensiblement vos dépenses mensuelles. **Le Ministère ne modifie jamais automatiquement le zonage des décharges à cette fin.**

4. **Réparation des cases fantômes de décharge :** corrige automatiquement un bogue du jeu où d’anciennes cases de décharge continuent d’être comptées et facturées comme des décharges actives. Cela passe par le mécanisme de zonage du jeu lui-même et n’a pas d’option distincte.

Pour comprendre le fonctionnement de ces fonctions, il est important de distinguer **la production de déchets, la capacité de traitement, l’admission en décharge et le stockage à long terme**.

Toutes les quantités de déchets de ce chapitre utilisent les unités de déchets internes de SimCity 4. Les dépenses sont exprimées en simoleons (§) par mois simulé.

<a id="garbage-landfills-vanilla"></a>
### 1. Comment fonctionnent les déchets dans SimCity 4

#### Comment les déchets sont produits et recyclés

Chaque jour simulé, SimCity 4 passe en revue les bâtiments de la ville et calcule la quantité de déchets qu’ils génèrent.

Le jeu enregistre cette quantité sous le nom de **Déchets produits**.

Les centres de recyclage retirent une partie de ces déchets avant qu’ils n’entrent dans le système d’élimination.

La quantité recyclée dépend des installations de recyclage de la ville. **Modifier le financement de la gestion des déchets ne modifie pas le taux de recyclage.**

Les déchets restants, ainsi que ceux déjà laissés sans collecte, doivent être pris en charge par le système d’élimination de la ville.

#### Dans quel ordre le jeu élimine-t-il les déchets ?

SimCity 4 traite les déchets dans un ordre précis.

| Priorité | Méthode d’élimination | Ce qui se passe |
|---|---|---|
| **1** | Exportations de déchets | Les déchets sont envoyés aux villes voisines par des accords d’exportation actifs. |
| **2** | Incinérateurs et centrales de valorisation énergétique des déchets | Les installations de traitement traitent les déchets, dans la limite de leur capacité disponible. |
| **3** | Décharges | Les déchets restants sont envoyés aux décharges, dans la limite de leur admission et de leur stockage. |
| **4** | Élimination cachée des déchets | Le jeu supprime discrètement une petite quantité supplémentaire de déchets. |

**Le jeu essaie chaque méthode dans cet ordre avant de passer à la suivante.**

Par exemple, si votre ville exporte une partie de ses déchets et traite le reste dans des incinérateurs, ses décharges n’ont à accepter que ce qui reste ensuite.

**Qu’est-ce que l’élimination cachée des déchets ?**

SimCity 4 contient un mécanisme intégré parfois appelé *enlèvement de déchets* (« garbage abduction » en anglais).

Il peut supprimer discrètement jusqu’à **2 000 unités de déchets par mois simulé**, selon les réglages du jeu d’origine.

Ces déchets disparaissent sans être comptés dans les statistiques normales d’élimination des déchets.

Le Ministère doit tenir compte de ce comportement pour interpréter les chiffres de déchets du jeu.

**Que deviennent les déchets qui ne peuvent pas être éliminés ?**

Tous les déchets qui restent après les quatre étapes demeurent sur la grille de la ville sous forme de **déchets non collectés**.

Ce sont les déchets qui peuvent s’accumuler dans vos rues et vos bâtiments lorsque votre système d’élimination ne peut pas répondre aux besoins de la ville.

Le jeu tient un total, à l’échelle de la ville, de ces déchets non collectés, que le Ministère peut lire.

#### Comment fonctionnent les accords sur les déchets avec les villes voisines

Les accords sur les déchets fonctionnent différemment de ceux sur l’électricité et l’eau.

Il en existe deux types :

- **Importations de déchets :** une ville voisine vous envoie ses déchets et vous paie pour les éliminer.
- **Exportations de déchets :** votre ville paie une ville voisine pour qu’elle prenne ses déchets.

Bien que ces accords précisent une quantité mensuelle, **les déchets ne sont pas livrés de manière uniforme tout au long du mois**.

Le jeu déplace normalement environ 10 % de la quantité mensuelle de l’accord chaque jour simulé, jusqu’à ce que l’allocation du mois soit livrée.

**Livraison quotidienne d’un accord sur les déchets (jeu de base)**

```math
s = \mathrm{round}(0.1 \times A)
\qquad
\text{tant que } A-U>0
```

Après chaque livraison quotidienne :

```math
U \leftarrow U+s
```

Où :

- **A** est la quantité mensuelle de déchets de l’accord.
- **U** est la quantité déjà livrée pendant le mois en cours.
- **s** est la quantité livrée ce jour-là.

La quantité livrée est remise à zéro au début de chaque mois simulé.

**Exemple : exporter 38 990 unités par mois**

```math
s = 38990 \times 0.1 = 3899
```

Le jeu exporte **3 899 unités par jour**, et termine normalement la quantité mensuelle en une dizaine de jours.

Cela signifie que la plupart des livraisons d’accords sur les déchets ont lieu **pendant les dix premiers jours environ de chaque mois**, et non réparties uniformément sur les 30 ou 31 jours.

Il y a aussi un petit détail d’arrondi : la dernière livraison quotidienne n’est pas limitée à la quantité exacte restante. Certains accords peuvent donc livrer un peu plus que leur quantité mensuelle nominale.

**Pourquoi est-ce important ?**

Une ville peut recevoir une grande quantité de déchets importés au début du mois.

Le système de gestion des déchets doit pouvoir absorber cette concentration temporaire, même si la quantité mensuelle totale semble raisonnable.

**Qu’en est-il des statistiques de déchets importés du jeu ?**

Le compteur *Déchets importés le mois dernier* du jeu indique ce qui a réellement été livré **pendant le mois précédent**.

Il reste inchangé pendant tout le mois en cours.

Il ne faut pas le confondre avec la quantité actuellement livrée par les accords actifs.

#### Comment le financement de la gestion des déchets influence le traitement des déchets

Contrairement à l’électricité, la gestion des déchets a **un seul pourcentage de financement pour tout le service**.

Ce pourcentage influence la capacité des installations de traitement des déchets et la quantité de déchets que les décharges peuvent accepter.

Le jeu d’origine autorise normalement un financement de **0 % à 120 %**, sauf si le service a un maximum différent ou un financement fixe.

Cependant, **la capacité de traitement des déchets n’augmente pas proportionnellement au financement**.

SimCity 4 utilise la même courbe financement-efficacité que ses centrales électriques.

**Efficacité de la gestion des déchets (jeu de base)**

```math
e = \frac{f(p)}{100}
```

Où :

- **p** est le pourcentage de financement de la gestion des déchets.
- **f(p)** est l’efficacité donnée par la courbe du jeu.
- **e** est l’efficacité obtenue, exprimée sous forme de fraction.

La courbe d’origine a les points de contrôle suivants :

| Financement de la gestion des déchets | Efficacité de traitement |
|---:|---:|
| 0 % | 0 % |
| 50 % | 25 % |
| 90 % | 80 % |
| 100 % | 100 % |
| 120 % | 110 % |

Les valeurs intermédiaires sont calculées par interpolation linéaire, comme expliqué dans les chapitres précédents.

**Exemples d’efficacité de la gestion des déchets**

| Financement | Efficacité de traitement |
|---:|---:|
| 50 % | 25 % |
| 60 % | 38,75 % |
| 61 % | 40,125 % |
| 75 % | 59,375 % |
| 90 % | 80 % |
| 100 % | 100 % |
| 110 % | 105 % |
| 120 % | 110 % |

**Qu’est-ce que cela signifie pour votre ville ?**

À **50 % de financement**, votre service de gestion des déchets ne fournit que **25 % de sa capacité de traitement normale**.

À **90 % de financement**, il en fournit 80 %.

À **120 % de financement**, il en fournit 110 %, tout en coûtant 20 % de plus que la normale.

Le rapport est particulièrement important entre 50 % et 90 %, où chaque point de financement supplémentaire augmente l’efficacité de 1,375 point de pourcentage.

**Qu’est-ce que l’efficacité influence ?**

L’efficacité de la gestion des déchets modifie :

- La capacité de traitement des incinérateurs.
- La capacité de traitement des déchets des centrales de valorisation énergétique des déchets.
- La quantité de déchets que les décharges peuvent accepter pendant un mois.

Elle ne modifie **pas** :

- La quantité livrée par les accords régionaux sur les déchets.
- La quantité retirée par les centres de recyclage.

**La capacité change-t-elle immédiatement après un ajustement du financement ?**

Pas complètement.

Le pourcentage de financement change immédiatement, mais l’efficacité calculée par le jeu pour la gestion des déchets et les capacités de traitement se mettent à jour au cours des jours simulés suivants.

Le Ministère doit donc tenir compte de ces délais pour évaluer si un changement de financement a pris effet.

#### Incinérateurs et centrales de valorisation énergétique des déchets

SimCity 4 calcule chaque jour simulé la capacité de traitement disponible des incinérateurs et des centrales de valorisation énergétique des déchets.

Pour chaque installation, cette capacité tient déjà compte de :

- La capacité de traitement maximale du bâtiment.
- L’efficacité actuelle de la gestion des déchets.
- L’état propre du bâtiment et ses modificateurs de capacité.

Le jeu fournit ensuite la capacité de traitement combinée de chaque type d’installation.

**Le Ministère lit ces valeurs directement.**

Il ne les multiplie pas à nouveau par l’efficacité de la gestion des déchets, car le jeu a déjà appliqué cet ajustement.

Cela évite de compter deux fois l’effet du financement.

**Et l’électricité produite à partir des déchets ?**

Les centrales de valorisation énergétique des déchets produisent aussi de l’électricité.

Leur fonction de traitement des déchets relève de la gestion des déchets, tandis que leur production d’électricité est examinée séparément dans le chapitre [Électricité](power.md#power).

<a id="garbage-landfills-valve"></a>
#### Quelle quantité de déchets une décharge peut-elle accepter ?

C’est l’une des mécaniques les plus importantes de SimCity 4.

**Avoir de la place libre dans une décharge ne signifie pas qu’elle peut accepter une quantité illimitée de déchets chaque mois.**

Le jeu applique une limite d’admission mensuelle distincte, que le Ministère modélise sous la forme d’une **vanne de décharge**.

Imaginez deux restrictions différentes :

- **Capacité de stockage :** la quantité totale de déchets que la décharge peut contenir.
- **Capacité d’admission :** la quantité de nouveaux déchets qui peut y être envoyée pendant le mois en cours.

Une décharge peut avoir beaucoup d’espace de stockage vide et pourtant ne plus pouvoir accepter de déchets ce mois-là.

**Admission quotidienne en décharge (jeu de base)**

```math
\text{admis} =
\min\left(
G,\ F,\ \mathrm{round}(5000 \times L \times e)-S
\right)
```

La quantité réellement acceptée ne peut pas être négative.

Où :

- **G** est la quantité de déchets encore en attente d’élimination.
- **F** est l’espace de stockage libre restant.
- **L** est le nombre de cases de décharge comptées par le jeu.
- **e** est l’efficacité actuelle de la gestion des déchets.
- **S** est la quantité déjà envoyée par la vanne de la décharge pendant le mois en cours.
- **5 000** est la capacité d’admission mensuelle de base par case de décharge.

Chaque fois que des déchets entrent dans la décharge, ils sont ajoutés au total mensuel cumulé de la vanne.

**En termes simples :**

Le jeu limite l’admission en décharge selon le nombre de cases de décharge, le financement de la gestion des déchets et la quantité de déchets déjà acceptée ce mois-ci.

**Exemple : une décharge de 48 cases**

Supposons que la ville ait :

- **48 cases de décharge.**
- **50 % de financement de la gestion des déchets.**
- **25 % d’efficacité de la gestion des déchets.**

La limite d’admission mensuelle est d’environ :

```math
5000 \times 48 \times 0.25 = 60000
```

**La décharge peut accepter environ 60 000 unités de déchets ce mois-là**, en supposant qu’elle dispose d’assez de stockage libre et d’aucun report précédent de la vanne.

À 100 % de financement, la même décharge a une admission mensuelle théorique de :

```math
5000 \times 48 \times 1.00 = 240000
```

Soit **240 000 unités par mois**.

<text strong>Comparaison</text>

| Financement | Efficacité | Admission mensuelle approximative en décharge |
|---|---:|---:|
| 50 % | 25 % | 60 000 unités |
| 100 % | 100 % | 240 000 unités |

**La zone de décharge n’a pas changé. Seul le budget de la gestion des déchets a changé.**

C’est pourquoi réduire trop le financement de la gestion des déchets peut créer des déchets non collectés même quand vos décharges ont encore beaucoup de place libre.

<a id="garbage-landfills-rollover"></a>
#### Que deviennent les déchets de la décharge à la fin du mois ?

Il existe une autre distinction importante :

**Les déchets qui entrent dans une décharge pendant un mois ne sont pas la même chose que les déchets qui y sont stockés durablement.**

SimCity 4 traite le stockage des décharges à chaque changement de mois simulé.

Deux choses se produisent :

1. Une partie des déchets déjà stockés dans la décharge disparaît progressivement par le mécanisme de décomposition du jeu.
2. Les déchets accumulés dans la vanne d’admission mensuelle sont transférés vers le stockage de la décharge, dans les limites du jeu.

Cela signifie que le stockage de la décharge et l’admission mensuelle doivent être considérés séparément.

#### 1. Les déchets stockés se décomposent progressivement

SimCity 4 retire chaque mois simulé une petite quantité de déchets stockés dans chaque cellule de la décharge.

La quantité dépend du remplissage de la cellule.

**Décomposition mensuelle des déchets par cellule de décharge (jeu de base)**

```math
d =
\min\left(
\text{stock},
\max\left(
1,
\min\left(
1500,
\mathrm{round}(0.07 \times \text{stock})
\right)
\right)
\right)
```

Où :

- **stock** est la quantité de déchets actuellement stockée dans une cellule de décharge.
- **d** est la quantité qui disparaît lors de la mise à jour mensuelle.

Une cellule vide ne perd rien.

Pour les cellules occupées, le jeu retire normalement environ **7 % des déchets stockés**, avec un minimum d’une unité et un maximum de 1 500 unités par cellule et par mois.

**Exemple**

Imaginons qu’une cellule de décharge contienne 10 000 unités de déchets.

```math
10000 \times 0.07 = 700
```

Le jeu retire **700 unités** lors de la mise à jour mensuelle.

Si une cellule contient 30 000 unités :

```math
30000 \times 0.07 = 2100
```

Mais la décomposition maximale est de 1 500 unités.

Le jeu retire donc **1 500 unités**, et non 2 100.

Dans les villes testées, une cellule de décharge entièrement pleine contenait environ 40 000 unités et perdait exactement 1 500 unités chaque mois.

**Comment la décomposition est-elle calculée pour une décharge entière ?**

Le jeu stocke les déchets de manière compacte : d’abord les cellules pleines, puis une cellule partiellement remplie, puis les cellules vides.

Le Ministère peut donc calculer la décomposition totale à partir de la quantité de déchets stockée et de la capacité de chaque cellule.

**Décomposition mensuelle totale d’une décharge (jeu de base)**

```math
D(U) =
\left\lfloor\frac{U}{C}\right\rfloor d(C)
+
d(U \bmod C)
```

Où :

- **U** est la quantité totale de déchets stockée.
- **C** est la capacité d’une cellule de décharge.
- **d(C)** est la décomposition mensuelle d’une cellule pleine.
- **U mod C** est la quantité de déchets restant dans la dernière cellule partiellement remplie.

La première partie calcule la décomposition des cellules pleines. La seconde calcule celle de la cellule partiellement remplie, s’il y en a une.

Ce modèle a reproduit exactement la décomposition enregistrée par le jeu sur **11 mises à jour mensuelles consécutives** dans une ville de test.

**Pourquoi est-ce important ?**

Les décharges ne se remplissent pas simplement à un rythme constant pour toujours.

Une partie des déchets stockés disparaît chaque mois.

Le Ministère en tient compte pour estimer l’espace disponible dans les décharges et la vitesse à laquelle elles peuvent se remplir.

#### 2. L’admission mensuelle en décharge devient des déchets stockés

Lors de la mise à jour mensuelle, SimCity 4 transfère les déchets de la vanne d’admission vers les déchets stockés de la décharge.

Cependant, **la vanne n’est pas toujours entièrement vidée**.

La quantité absorbée dépend :

- Des déchets en attente dans la vanne.
- De l’espace de stockage disponible de la décharge.
- Du nombre de cases de décharge.
- Du financement de la gestion des déchets réellement fourni.

**Absorption mensuelle en décharge (jeu de base)**

```math
\begin{aligned}
H &= \min(S,\ 327\,680\,000) \\[4pt]
\text{si } A < B:\quad
H &= \left\lfloor\frac{H\times A}{B}\right\rfloor \\[4pt]
\text{absorbé} &=
\min\left(
H,\ 5000\times L,\ T_{\mathrm{cap}}-(U-D(U))
\right) \\[4pt]
U' &= U-D(U)+\text{absorbé} \\
S' &= S-\text{absorbé}
\end{aligned}
```

Où :

- **S** est la quantité de déchets en attente dans la vanne mensuelle de la décharge.
- **A** est le montant d’entretien de la décharge réellement financé.
- **B** est le coût d’entretien de la décharge à 100 % de financement.
- **L** est le nombre de cases de décharge.
- **Tcap** est la capacité de stockage totale de la décharge.
- **U** est la quantité déjà stockée avant la mise à jour mensuelle.
- **D(U)** est la quantité de déchets supprimée par la décomposition mensuelle.
- **absorbé** est la quantité de déchets transférée vers le stockage.
- **U′** est le nouveau total de déchets stockés.
- **S′** est la quantité laissée dans la vanne pour le mois suivant.

Le calcul du jeu fixe aussi une limite supérieure de **327 680 000 unités** pour la quantité de la vanne prise en compte pour l’absorption.

Vous n’avez pas à le calculer à la main. L’important est de voir comment le financement influence le résultat.

**À 100 % de financement de la gestion des déchets**, la réduction liée au financement ne s’applique pas. La vanne peut être entièrement absorbée lorsque le stockage suffisant et les autres limites le permettent.

**En dessous de 100 % de financement**, le jeu peut ne transférer qu’une partie de la vanne vers le stockage permanent.

Le reste demeure dans la vanne et est reporté au mois suivant.

#### Pourquoi les déchets restés dans la vanne posent-ils problème ?

Rappelez-vous que la décharge a une limite d’admission mensuelle.

Si une partie de la vanne du mois précédent reste occupée, la décharge commence le nouveau mois avec moins de capacité d’admission disponible.

Cela crée une seconde conséquence d’un faible financement de la gestion des déchets :

1. **Un financement plus faible réduit la quantité de déchets pouvant entrer dans la décharge pendant le mois.**
2. **Un financement plus faible peut aussi réduire la quantité de déchets absorbée dans le stockage au changement de mois**, laissant une partie de l’allocation d’admission occupée.

**Un faible financement de la gestion des déchets peut donc restreindre le débit de la décharge de deux manières distinctes.**

C’est pourquoi regarder uniquement l’espace de stockage disponible dans les décharges donne une image incomplète du système de gestion des déchets.

Une décharge peut être loin d’être pleine et avoir pourtant du mal à absorber les déchets de la ville.

#### Quand le jeu met-il à jour les statistiques de décharge ?

Le jeu publie la quantité de déchets absorbée dans le stockage de la décharge lors de la mise à jour mensuelle suivante, par son compteur *Déchets mis en décharge*.

Cette valeur est donc liée au processus de stockage mensuel, et non simplement aux déchets arrivant dans les décharges pendant la journée en cours.

**La distinction essentielle**

| Concept | Ce qu’il mesure | Principale limite |
|---|---|---|
| **Déchets produits** | Les déchets générés par les bâtiments de la ville | L’activité de la ville et le recyclage |
| **Capacité de traitement** | Les déchets que les incinérateurs et les centrales de valorisation énergétique des déchets peuvent traiter | Le financement de la gestion des déchets et l’état des bâtiments |
| **Admission en décharge** | Les nouveaux déchets que les décharges peuvent accepter pendant le mois | Les cases de décharge, l’efficacité et le report de la vanne |
| **Stockage en décharge** | Les déchets accumulés dans les cellules de décharge | L’espace de stockage disponible et la décomposition mensuelle |
| **Absorption mensuelle** | Les déchets transférés de la vanne vers le stockage | Le financement, la taille de la décharge et l’espace restant |

**Le Ministère doit tenir compte de tous ces mécanismes plutôt que de simplement comparer la production de déchets à l’espace libre des décharges.**
<a id="garbage-landfills-cells"></a>
#### Comment fonctionnent les zones de décharge : cases et cellules de stockage

Dans SimCity 4, une décharge est plus compliquée que la zone que vous tracez sur la carte.

Le jeu utilise deux éléments différents :

- **Cases de décharge :** les cases individuelles que vous zonez pour l’élimination des déchets.
- **Cellules de stockage :** des zones invisibles où le jeu stocke réellement les déchets accumulés.

Chaque cellule de stockage correspond à un **groupe de 2 × 2 cases de décharge**.

Une cellule ne peut stocker des déchets que si ses quatre cases appartiennent à la zone de décharge.

**Combien de cellules de stockage une décharge possède-t-elle ?**

Le jeu enregistre chaque décharge dans une zone rectangulaire, appelée *boîte englobante* (« bounding box » en anglais).

Ce rectangle contient aussi des informations qui identifient les cases appartenant réellement à la décharge.

Pour un rectangle de **w cases de large et h cases de haut**, la grille des cellules de stockage possibles contient :

```math
N_{\text{possibles}}=(w-1)(h-1)
```

Cependant, seules les cellules dont les quatre cases font partie de la décharge sont actives.

<box gap={2}>
**Exemple : une décharge de 3 × 3**

<svg viewBox="0 0 340 155" width="100%" xmlns="http://www.w3.org/2000/svg">
  {#each [0,1,2] as r}
    {#each [0,1,2] as c}
      <rect x={22+c*34} y={16+r*34} width="32" height="32" rx="2" fill="#8fba7b" stroke="#45683e" strokeWidth="1"/>
    {/each}
  {/each}
  {#each [0,1] as r}
    {#each [0,1] as c}
      <rect x={178+c*34} y={33+r*34} width="32" height="32" rx="2" fill="#d9b477" stroke="#956b35" strokeWidth="1"/>
    {/each}
  {/each}
  <text x="73" y="136" textAnchor="middle" fill="currentColor" fontSize="12">9 cases de décharge</text>
  <text x="228" y="136" textAnchor="middle" fill="currentColor" fontSize="12">4 cellules de stockage</text>
  <path d="M138 65 L162 65 M156 60 L162 65 L156 70" fill="none" stroke="currentColor" strokeWidth="1.5"/>
</svg>
<caption>Le schéma de droite montre les positions possibles des cellules de stockage, et non une seconde zone de décharge.</caption>
</box>

Une décharge de 3 × 3 entièrement zonée compte **9 cases mais 4 cellules de stockage qui se chevauchent**.

Une décharge de 2 × 2 n’a qu’une seule cellule de stockage.

Cette distinction est importante, car **l’entretien de la décharge se calcule à partir des cases, alors que le stockage des déchets est organisé en cellules**.

#### Quelle quantité de déchets une décharge peut-elle stocker ?

Le jeu attribue une capacité de stockage à chaque cellule active.

Cette capacité dépend :

- Du nombre de cases appartenant à la zone de décharge.
- Du nombre de cellules de stockage actives.

**Capacité de stockage d’une décharge (jeu de base)**

```math
C = \mathrm{round}\left(\frac{T\times30000}{N}\right)
\qquad
T_{\mathrm{cap}}=N\times C
```

Où :

- **T** est le nombre de cases enregistrées dans la zone de décharge, y compris les cases fantômes (expliquées plus bas).
- **N** est le nombre de cellules de stockage actives.
- **30 000** est la capacité de stockage du jeu par case de décharge.
- **C** est la capacité attribuée à chaque cellule active.
- **Tcap** est la capacité de stockage totale de la décharge.

Le jeu arrondit la capacité d’une cellule à l’unité entière la plus proche ; les valeurs exactement à mi-chemin sont arrondies à l’entier pair le plus proche.

**Exemples**

| Décharge | Cellules actives | Capacité par cellule | Capacité totale |
|---|---:|---:|---:|
| Décharge de 2 × 2 | 1 | 120 000 | 120 000 |
| Décharge de 129 cases | 70 | 55 286 | 3 870 020 |

En général, une décharge offre environ **30 000 unités de stockage par case zonée**.

Le total exact peut différer légèrement, car la capacité est arrondie séparément pour chaque cellule.

La capacité totale de décharge de la ville est la somme des capacités de toutes ses zones de décharge.

#### Que se passe-t-il quand vous agrandissez ou réduisez une décharge ?

C’est là que la structure interne en cellules du jeu devient importante.

**Agrandir une décharge**

Quand vous ajoutez de nouvelles cases de décharge, SimCity 4 reconstruit la grille de cellules de stockage de la zone.

Les cellules existantes conservent normalement leurs déchets stockés, tandis que les nouvelles cellules valides commencent vides.

Cependant, **le sens dans lequel vous agrandissez peut influencer les déchets stockés**.

Le jeu identifie les cellules de stockage par leur position relative au coin de la boîte englobante de la décharge.

- **Agrandir vers la droite ou vers le bas :** le coin d’origine reste en place, donc les cellules existantes conservent normalement leur position.
- **Agrandir vers la gauche ou vers le haut :** le coin se déplace et décale les positions relatives des cellules existantes. Les déchets stockés du jeu peuvent alors ne plus correspondre aux emplacements prévus.

C’est la cause d’un problème connu des décharges de SimCity 4, parfois rencontré après l’agrandissement d’une décharge.

**La méthode la plus sûre consiste à agrandir les décharges vers la droite ou vers le bas de leur zone existante, selon les coordonnées de la carte du jeu.**

L’agrandissement peut aussi modifier la quantité de déchets que chaque cellule peut contenir.

Si la nouvelle capacité d’une cellule devient inférieure à la quantité déjà stockée, **l’excédent de déchets est renvoyé dans la vanne d’admission de la décharge**.

Ces déchets doivent alors être de nouveau traités par le système de décharge.

Une opération de zonage fait aussi vérifier par le jeu les informations d’appartenance de la zone et retirer les cases qui ne sont plus réellement zonées comme décharge.

**Réduire une décharge**

Retirer des cases de décharge se comporte différemment.

Quand vous dézonez une case, le jeu :

1. La retire de la zone enregistrée de la décharge.
2. Désactive les cellules de stockage concernées.
3. Recalcule la capacité des cellules actives restantes.
4. Renvoie l’éventuel excédent de déchets stockés dans la vanne d’admission.

Contrairement à l’agrandissement, **le dézonage ne reconstruit pas toute la grille de cellules de stockage et ne concilie pas entièrement les informations d’appartenance de la zone**.

Le jeu empêche aussi certains changements :

- Vous ne pouvez pas dézoner une case contenant déjà un tas de déchets.
- Les zones de décharge doivent respecter les restrictions de taille du jeu, notamment la largeur minimale de deux cases et la largeur maximale de 48 cases.

**Pourquoi est-ce important ?**

Modifier la forme d’une décharge peut avoir plus d’effets que sur sa seule surface visible.

Cela peut aussi changer la quantité de déchets que chaque cellule peut contenir et la quantité de déchets renvoyée dans le système d’admission mensuelle.

Le Ministère tient compte de ces effets lorsqu’il évalue des changements de taille de décharge.

#### Combien coûte une décharge ?

L’entretien d’une décharge se calcule **case par case**.

Les cases de décharge vides ont un coût d’entretien fixe, tandis que les cases contenant des tas de déchets coûtent plus cher.

**Entretien d’une décharge à 100 % de financement (jeu de base)**

```math
B=\sum_{\text{cases de la zone}}c_i
\qquad
c_i=
\begin{cases}
\text{coût d’entretien du tas}, & \text{si un tas de déchets est présent}\\
5, & \text{sinon}
\end{cases}
```

Où :

- **B** est le coût d’entretien mensuel de la décharge à 100 % de financement.
- **cᵢ** est le coût d’entretien d’une case individuelle.

Dans les villes examinées, les coûts observés étaient :

| Type de case | Entretien mensuel à 100 % |
|---|---:|
| Case de décharge vide | 5 § |
| Case avec un tas de déchets | 20 §, 30 § ou 40 §, selon le tas |

**Une décharge devient plus chère à mesure que les déchets s’accumulent.**

Par exemple, une décharge de 100 cases vides coûte :

```math
100\times5=500
```

Soit **500 § par mois à 100 % de financement**, avant même l’apparition de tas de déchets.

Si 20 de ces cases développent des tas de déchets coûtant 20 § chacun, tandis que les 80 autres restent vides :

```math
(80\times5)+(20\times20)=800
```

L’entretien mensuel passe à **800 §**.

C’est pourquoi la taille de la décharge a une importance économique.

Conserver une grande surface de décharge inutilisée peut générer des dépenses d’entretien inutiles.

Le Ministère peut repérer les situations où réduire le zonage de décharge inutilisé ferait économiser de l’argent, à condition que le changement soit sans risque.

#### Cases fantômes : des zones de décharge qui ne devraient pas exister

SimCity 4 a un bogue qui peut laisser derrière lui des **cases fantômes de décharge**.

Une case fantôme est un emplacement que le jeu compte encore comme faisant partie d’une décharge alors qu’il n’est plus zoné comme décharge.

Par exemple, la case peut maintenant contenir :

- Une route ordinaire.
- Un morceau de terrain non zoné.
- Une autre zone qui n’est pas une décharge.

Visuellement, la décharge a disparu de cette case.

**En interne, cependant, le jeu la compte toujours comme faisant partie de la zone de décharge.**

Cela a deux conséquences :

1. **La case continue de coûter 5 § par mois** d’entretien de décharge à 100 % de financement.
2. **Elle est incluse dans le nombre de cases enregistré de la zone** lorsque la capacité de stockage de la décharge est recalculée.

Les cases fantômes ne comptent pas dans le total de cases de décharge actives utilisé pour calculer la limite d’admission mensuelle.

Autrement dit, **le jeu peut vous facturer des cases de décharge qui n’aident pas réellement à éliminer les déchets**.

#### Pourquoi les cases fantômes reviennent-elles à la réouverture d’une ville ?

Le problème vient de la façon dont SimCity 4 enregistre et recharge les zones de décharge.

Quand il enregistre une décharge, le jeu mémorise sa boîte englobante rectangulaire et les cases qui appartiennent à la zone.

Cependant, il n’enregistre pas correctement l’appartenance des cases situées sur la dernière ligne et la dernière colonne du rectangle.

À la réouverture de la ville, le jeu suppose d’abord que chaque case à l’intérieur du rectangle appartient à la décharge, puis restaure les informations d’appartenance enregistrées.

Comme les informations enregistrées pour la dernière ligne et la dernière colonne sont incomplètes, **les cases de ces bords peuvent redevenir à tort membres de la décharge**.

Cela crée deux types de cases fantômes :

| Emplacement de la case fantôme | Ce qui se passe après la réparation |
|---|---|
| **À l’intérieur du rectangle de la zone** | Une opération de zonage adaptée peut la supprimer définitivement, car son appartenance corrigée est enregistrée. |
| **Sur la dernière ligne ou colonne** | Elle peut revenir à la réouverture de la ville, car le jeu n’enregistre pas correctement l’appartenance de ce bord. |

Les outils de zonage du jeu d’origine ne peuvent pas supprimer définitivement chaque fantôme de bord, car la boîte englobante enregistrée de la zone ne rétrécit pas.

**Comment le Ministère aide-t-il ?**

Le Ministère comprend un mécanisme de réparation automatique des cases fantômes.

Quand il détecte des cases fantômes éligibles, il utilise le système de zonage du jeu lui-même pour les corriger, plutôt que de modifier directement le fichier de sauvegarde.

La réparation est conçue pour ne pas perturber les zones de décharge légitimes ni les déchets stockés.

**Il n’existe pas de réglage distinct pour la réparation des cases fantômes.** Elle fait partie de la gestion automatique des déchets.

Cependant, le bogue d’enregistrement et de chargement sous-jacent reste présent dans SimCity 4. Certains fantômes de bord peuvent donc revenir à la réouverture d’une ville et nécessiter une nouvelle correction.

<a id="garbage-landfills-problem"></a>
### 2. Le problème de gestion

Gérer les déchets dans SimCity 4 est plus compliqué que de simplement prévoir assez d’espace de décharge.

Le Ministère doit tenir compte de plusieurs problèmes.

- **Les déchets arrivent à des moments différents :** votre ville produit des déchets chaque jour, alors que les accords régionaux sur les déchets livrent généralement l’essentiel de leur quantité mensuelle pendant les dix premiers jours environ.
- **Le stockage en décharge n’est pas la capacité d’élimination :** une décharge peut avoir beaucoup de place libre et pourtant atteindre sa limite d’admission mensuelle.
- **Un faible financement affecte l’élimination des déchets de plusieurs façons :** il réduit la capacité de traitement et peut laisser une partie de la vanne d’admission de la décharge occupée au début du mois suivant.
- **Les statistiques du jeu peuvent être en retard :** les chiffres de déchets importés se rapportent au mois précédent, et certaines valeurs d’accords et de gestion des déchets mettent du temps à se mettre à jour.
- **La courbe de financement n’est pas proportionnelle :** réduire trop le financement de la gestion des déchets peut retirer bien plus de capacité de traitement que prévu.
- **Les zones de décharge inutilisées coûtent de l’argent :** les cases vides ont des frais d’entretien mensuels, et les cases fantômes peuvent créer des frais supplémentaires.
- **Changer la forme d’une décharge peut affecter les déchets stockés :** agrandir ou réduire une décharge peut redistribuer la capacité de stockage et renvoyer l’excédent de déchets dans le système d’admission.

**Le Ministère doit tenir compte de tous ces facteurs avant de décider quel financement de la gestion des déchets votre ville requiert.**

Il ne peut pas optimiser la gestion des déchets en toute sécurité en se fondant uniquement sur la production de déchets actuelle ou sur l’espace libre des décharges.

<a id="garbage-landfills-algorithm"></a>
### 3. Comment le Ministère gère les déchets

Le Ministère utilise les statistiques de déchets du jeu pour estimer la quantité de déchets que votre ville doit traiter.

Il prend ensuite en compte les installations de traitement des déchets, la capacité des décharges, l’accumulation actuelle de déchets et les accords régionaux pour choisir un budget de gestion des déchets adapté.

L’objectif est d’**éliminer les déchets que la ville doit évacuer, de maintenir une réserve de sécurité et d’éviter les dépenses mensuelles inutiles**.

<a id="garbage-landfills-need"></a>
#### 3.1 Quelle quantité de déchets la ville doit-elle traiter ?

Le Ministère commence par calculer la quantité de déchets que votre propre ville produit après recyclage.

**Étape 1 : calculer les déchets propres de la ville**

```math
G_{\mathrm{own}}=\max(0,P-R)
```

Où :

- **P** est la quantité de déchets produite pendant le mois simulé précédent.
- **R** est la quantité de déchets recyclée pendant ce mois.
- **Gown** est la quantité de déchets qui reste à éliminer.

Par exemple, supposons que votre ville ait produit 100 000 unités de déchets le mois dernier et en ait recyclé 20 000.

```math
G_{\mathrm{own}}=100000-20000=80000
```

Le Ministère estime donc que la ville génère **80 000 unités de déchets par mois à éliminer**.

**Les déchets importés ne sont pas inclus dans ce calcul.**

Les statistiques de déchets importés du jeu se rapportent aux livraisons du mois précédent.

Le Ministère traite les déchets étrangers séparément, à partir des accords régionaux réels, au lieu de mélanger les déchets importés à la production propre de la ville.

#### Étape 2 : ajouter la réserve de sécurité

La production de déchets peut augmenter à mesure que la ville grandit.

Pour éviter de choisir un budget tout juste suffisant pour la quantité exacte du mois précédent, le Ministère ajoute une réserve de sécurité.

La **réserve de traitement des déchets est de 2 %** par défaut, réglable de 2 % à 20 %.

**Production de déchets protégée**

```math
G_{\mathrm{prot}}=
\left\lceil
G_{\mathrm{own}}\left(1+\frac{r}{100}\right)
\right\rceil
```

Où :

- **Gown** est la quantité de déchets propre à la ville après recyclage.
- **r** est la réserve de traitement des déchets configurée.
- **Gprot** est la quantité de déchets protégée, arrondie à l’unité supérieure.

**Exemple : une ville produisant 75 464 unités de déchets après recyclage**

Avec la réserve par défaut de 2 % :

```math
G_{\mathrm{prot}}=
\lceil75464\times1.02\rceil
=76974
```

Le Ministère planifie donc pour **76 974 unités** plutôt que pour exactement 75 464.

La réserve est un minimum, pas un maximum. Le Ministère peut devoir traiter davantage de déchets à cause de l’accumulation existante ou des accords régionaux.

#### Étape 3 : inclure les déchets accumulés et les importations

La ville peut déjà avoir des déchets en attente de collecte.

Elle peut aussi recevoir des déchets de villes voisines.

Le Ministère ajoute ces quantités pour obtenir un besoin global de traitement mensuel.

**Besoin mensuel de traitement des déchets**

```math
H=G_{\mathrm{prot}}+Q+\sum\text{quantités importées}
```

Où :

- **Gprot** est la production mensuelle de déchets protégée de la ville.
- **Q** est la quantité de déchets non collectés actuellement accumulée sur la grille de la ville.
- **Quantités importées** sont les quantités de déchets précisées par les accords régionaux d’importation.
- **H** est le besoin global de traitement mensuel.

**Exemple**

Supposons que la ville ait :

| Source | Unités de déchets |
|---|---:|
| Déchets propres, réserve comprise | 76 974 |
| Déchets déjà non collectés | 5 000 |
| Déchets importés des voisins | 20 000 |
| **Besoin total de traitement** | **101 974** |

Le Ministère doit donc prendre en compte **101 974 unités** de déchets pour planifier la gestion des déchets.

Cependant, **ce total seul ne suffit pas pour décider si un niveau de financement conviendra**.

Les déchets arrivent et sont traités à des moments différents au cours du mois.

Une ville peut avoir assez de capacité de traitement mensuelle totale et pourtant connaître une accumulation temporaire de déchets, parce que les importations arrivent tôt ou que l’admission en décharge est temporairement limitée.

C’est pourquoi le Ministère utilise ce total pour son résumé mensuel et ses enregistrements, mais prend la décision finale à l’aide d’une **simulation jour par jour de l’élimination des déchets**.

Cette simulation prend en compte le moment d’arrivée des déchets, les installations qui les traitent, le comportement de la vanne de la décharge et la présence éventuelle de déchets non collectés.

Les sections suivantes expliquent comment fonctionne cette prévision.
<a id="garbage-landfills-uncollected"></a>
#### 3.2 Que deviennent les déchets non collectés ?

Il arrive que votre ville produise plus de déchets que son système d’élimination ne peut en traiter.

L’excédent reste sur la grille de la ville sous forme de **déchets non collectés**.

Le Ministère lit directement la quantité totale dans le jeu et la traite comme un **arriéré**, représenté par **Q**.

**L’objectif est de résorber cet arriéré avant la fin du mois simulé en cours**, en plus de traiter les déchets produits pendant ce mois.

Par exemple, si votre ville a déjà 10 000 unités de déchets non collectés, le Ministère inclut ces 10 000 unités dans ses calculs.

Il n’ignore pas les déchets existants simplement parce que la production normale de la ville est couverte.

**Et les déchets que le jeu supprime automatiquement ?**

Comme expliqué plus haut, SimCity 4 possède un mécanisme caché qui peut supprimer discrètement une quantité limitée de déchets excédentaires.

Le Ministère ne considère pas cette suppression comme un échec de la gestion des déchets et ne s’en sert pas comme raison de bloquer des réductions de financement.

Seuls les déchets qui restent réellement non collectés sur la grille de la ville contribuent à l’arriéré.

<a id="garbage-landfills-contracts"></a>
#### 3.3 Comment les accords sur les déchets sont pris en compte

Les accords sur les déchets sont plus compliqués qu’il n’y paraît, car **leurs quantités mensuelles sont livrées sur plusieurs jours**, et non d’un seul coup ni uniformément tout au long du mois.

Le Ministère calcule les livraisons de **chaque accord séparément**.

Il n’additionne pas simplement toutes les quantités des accords en supposant qu’elles arrivent de façon uniforme.

#### Importations de déchets

Quand une autre ville vous envoie des déchets, le Ministère suppose que le jeu livre environ **10 % de la quantité mensuelle de cet accord chaque jour**, jusqu’à épuisement de l’allocation mensuelle.

Par exemple, un accord d’importation de 20 000 unités livre environ 2 000 unités par jour pendant les dix premiers jours du mois.

Le Ministère tient aussi compte de l’arrondi. Comme la dernière livraison du jeu peut dépasser légèrement la quantité prévue au contrat, il ne sous-estime pas les déchets entrants.

**Les déchets importés sont toujours traités comme des déchets supplémentaires que votre ville doit éliminer.**

Une importation suspendue est quand même comptée de façon prudente lors de l’évaluation des déchets que la ville pourrait devoir traiter.

#### Exportations de déchets

Quand vous payez une autre ville pour qu’elle prenne vos déchets, l’accord réduit la quantité que vos propres installations de gestion des déchets doivent traiter.

Le Ministère calcule ce soulagement avec le même calendrier de livraison quotidien.

Cependant, **il ne suppose jamais que l’on peut exporter plus de déchets que l’accord ne le permet réellement**.

Par exemple, un accord d’exportation de 20 000 unités procure environ 2 000 unités de soulagement d’élimination chaque jour, jusqu’à épuisement de l’allocation mensuelle.

Contrairement aux importations, le dernier crédit d’exportation quotidien est limité à la quantité encore disponible.

**Et si un accord d’exportation est suspendu ?**

Le Ministère ne compte pas sur une exportation suspendue pour évacuer des déchets.

Il n’accorde pas non plus de crédit d’élimination lorsque l’utilisation enregistrée de l’accord par le jeu ne peut pas être lue de façon fiable.

L’accord reste une obligation existante, mais le Ministère ne suppose pas qu’il fournira une capacité d’élimination utilisable.

Cela évite que le plan de gestion des déchets dépende d’exportations qui pourraient ne pas avoir lieu.

#### Un problème avec les statistiques d’accords du jeu

Pendant les premiers jours simulés d’un nouveau mois, SimCity 4 peut afficher temporairement la quantité utilisée pendant le **mois précédent**.

Imaginons par exemple un accord d’exportation de 7 000 unités.

Le premier jour d’un nouveau mois, le jeu peut encore indiquer :

**Quantité utilisée : 7 000 unités**

Cela ne signifie pas nécessairement que l’accord du nouveau mois est déjà épuisé.

Le Ministère détecte cette situation pendant les **trois premiers jours simulés**.

Si l’utilisation indiquée est supérieure à ce que l’accord aurait pu livrer au maximum depuis le début du mois, le Ministère considère cette lecture comme périmée et repart de zéro pour le calcul du mois.

**Test de lecture périmée**

```math
U > (\text{jour du mois}-1)\times\mathrm{round}(0.1\times A)
```

Où :

- **A** est la quantité mensuelle de l’accord.
- **U** est la quantité que le jeu indique comme déjà utilisée ce mois-ci.

Cela évite que des statistiques périmées ne provoquent de mauvaises décisions de financement.

#### Quand l’importation de déchets est-elle rentable ?

Importer des déchets rapporte de l’argent de la ville voisine, mais cela crée aussi plus de travail pour votre système de gestion des déchets.

Le Ministère ne considère une importation comme acceptable que si le financement choisi pour la gestion des déchets peut absorber les déchets supplémentaires **sans laisser d’arriéré à la fin du mois**.

Parfois, le revenu supplémentaire peut justifier d’augmenter le financement de la gestion des déchets.

Par exemple, accepter des déchets importés peut exiger un budget de gestion des déchets plus élevé tout en produisant un bénéfice net si le voisin paie assez.

Le gestionnaire d’accords compare ces coûts et ces revenus avant de prendre ses décisions.

Voir [Accords avec les voisins](deals.md#neighbor-deals) pour les règles complètes des accords.

<a id="garbage-landfills-projection"></a>
#### 3.4 Simuler les jours restants du mois

C’est la partie la plus importante des calculs de gestion des déchets du Ministère.

**Au lieu de comparer uniquement des totaux mensuels, le Ministère simule la façon dont les déchets seront traités chacun des jours restants du mois.**

Pourquoi ?

Parce que les déchets arrivent et sont éliminés à des moments différents.

- Votre ville produit des déchets chaque jour.
- Les importations arrivent généralement pendant les dix premiers jours environ.
- Les exportations soulagent l’élimination selon leur propre calendrier quotidien.
- Les incinérateurs et les centrales de valorisation énergétique des déchets ont une capacité de traitement limitée.
- Les décharges ont une limite d’admission mensuelle qui s’épuise progressivement.

Une ville peut disposer d’une capacité mensuelle théorique suffisante et pourtant connaître une accumulation temporaire de déchets.

Le Ministère doit déterminer si ces déchets accumulés peuvent être évacués avant la fin du mois.

#### Comment fonctionne la simulation quotidienne

Pour chaque pourcentage de financement des déchets possible, le Ministère effectue le calcul suivant pour chaque jour restant.

<box gap={2}>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="trash-2" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **1. Ajouter les déchets entrants**

      On part des déchets restés de la veille, puis on ajoute la production quotidienne estimée de la ville et les importations du jour.
    </box>
  </row>
  <divider color="subtle"/>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="truck" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **2. Appliquer les exportations de déchets**

      On retire autant de déchets que les contrats d’exportation disponibles peuvent en absorber. Le crédit d’exportation inutilisé peut être reporté au jour suivant.
    </box>
  </row>
  <divider color="subtle"/>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="factory" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **3. Utiliser les installations de traitement**

      Les incinérateurs et les centrales de valorisation énergétique des déchets traitent les déchets jusqu’à leur capacité de traitement quotidienne disponible.
    </box>
  </row>
  <divider color="subtle"/>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="layers" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **4. Utiliser la capacité des décharges**

      Les déchets restants sont envoyés aux décharges, tant que leur vanne d’admission mensuelle dispose encore de capacité.
    </box>
  </row>
  <divider color="subtle"/>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="calendar-check" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **5. Reporter les déchets non collectés**

      Les déchets qui ne peuvent pas être traités aujourd’hui restent dans l’arriéré du lendemain.
    </box>
  </row>
</box>

Le Ministère répète ce processus jusqu’au dernier jour du mois.

**Un niveau de financement est jugé suffisant lorsque l’arriéré prévu en fin de mois est nul.**

Une accumulation temporaire pendant le mois est permise, à condition qu’elle puisse être résorbée avant la fin du mois.

Le Ministère n’impose aucune limite artificielle supplémentaire à l’arriéré temporaire maximal.

**Projection quotidienne des déchets**

```math
\begin{aligned}
a_t &= Q_{t-1}+\frac{G_{\mathrm{prot}}}{n}+I_t\\
X_t &= R_{t-1}+c_t\\
x_t &= \min(a_t,X_t)\\
R_t &= X_t-x_t\\
b_t &= \min\left(a_t-x_t,\frac{K(p)}{n}\right)\\
l_t &= \min(a_t-x_t-b_t,V_{t-1})\\
V_t &= V_{t-1}-l_t\\
Q_t &= a_t-x_t-b_t-l_t
\end{aligned}
```

Où :

- **n** est le nombre de jours du mois simulé.
- **Gprot** est la production mensuelle de déchets protégée de la ville.
- **Q₀** est l’arriéré actuel de déchets non collectés.
- **Iₜ** est la quantité de déchets importés le jour *t*.
- **cₜ** est le crédit d’exportation généré ce jour-là.
- **Rₜ** est le crédit d’exportation inutilisé reporté.
- **aₜ** est la quantité de déchets à éliminer ce jour-là.
- **xₜ** est la quantité de déchets évacuée par les exportations.
- **bₜ** est la quantité de déchets traitée par les installations de traitement.
- **lₜ** est la quantité de déchets envoyée aux décharges.
- **K(p)** est la capacité de traitement mensuelle au financement **p**.
- **Vₜ** est la capacité d’admission restante des décharges.
- **Qₜ** est la quantité de déchets encore non collectés à la fin du jour *t*.

Le Ministère approxime la production de déchets propre à la ville comme répartie uniformément sur le mois.

Les importations et les exportations, en revanche, suivent les calendriers de livraison de chacun des contrats.

Le calcul est effectué pour chaque niveau de financement candidat.

#### Comment la capacité de traitement est-elle ajustée selon le niveau de financement ?

Le jeu indique déjà la capacité de traitement mensuelle combinée des incinérateurs et des centrales de valorisation énergétique des déchets.

Le Ministère utilise cette valeur pour estimer la capacité à un autre pourcentage de financement.

**Capacité de traitement projetée**

```math
K(p)=\mathrm{round}\left(K_{\mathrm{act}}\times\frac{e(p)}{e_{\mathrm{act}}}\right)
```

Où :

- **Kact** est la capacité de traitement combinée actuellement indiquée par le jeu.
- **e(p)** est l’efficacité de la gestion des déchets au financement proposé.
- **eact** est l’efficacité actuelle.
- **K(p)** est la capacité de traitement projetée.

**Exemple**

Lors des tests, le passage du financement des déchets de 61 % à 50 % a fait passer l’efficacité de 40,125 % à 25 %.

La capacité des centrales de valorisation énergétique des déchets est passée d’environ **34 088 à 21 238 unités**, ce qui correspond au modèle.

Si l’efficacité actuelle est nulle alors que des bâtiments de traitement existent, le Ministère ne peut pas calculer de façon fiable les capacités alternatives et interrompt le plan de financement plutôt que de deviner.

#### Comment la capacité d’admission disponible des décharges est-elle calculée ?

Le Ministère estime aussi la quantité de déchets que les décharges peuvent encore accepter pendant le mois en cours.

**Admission restante des décharges**

```math
V_0(p)=
\max\left(
0,\min\left(
F,\mathrm{round}(5000\times L\times e(p))-S
\right)
\right)
```

Où :

- **F** est l’espace de stockage libre restant des décharges.
- **L** est le nombre de cases de décharge.
- **e(p)** est l’efficacité de la gestion des déchets au financement proposé.
- **S** est la quantité de déchets déjà accumulée dans la vanne mensuelle des décharges.
- **V₀(p)** est la capacité d’admission restante à ce financement.

Par exemple, une décharge peut avoir 100 000 unités de stockage libre mais seulement 20 000 unités de capacité d’admission mensuelle restante.

**Le Ministère ne compte que ces 20 000 unités comme disponibles pour l’élimination pendant le reste du mois.**

S’il n’existe aucune décharge, la capacité d’admission disponible est nulle.

Si les informations nécessaires sur la décharge ne peuvent pas être lues, le Ministère ne suppose pas que la décharge peut accepter davantage de déchets.

Lorsque les exportations et les installations de traitement peuvent absorber tous les déchets restants, aucune capacité de décharge n’est nécessaire.

<a id="garbage-landfills-nextmonth"></a>
#### 3.5 Pourquoi le Ministère vérifie-t-il parfois aussi le mois suivant ?

Comme expliqué plus haut, un financement trop bas des déchets peut laisser des déchets dans la vanne d’admission de la décharge à la fin d’un mois.

Ces déchets restants réduisent la capacité d’admission disponible de la décharge le mois suivant.

**Un niveau de financement qui résorbe le mois en cours peut donc créer un problème le mois d’après.**

Pour l’éviter, le Ministère peut aussi simuler **tout le mois suivant**.

Il ne fait toutefois cette vérification supplémentaire que lorsqu’il dispose d’assez d’informations pour prédire de façon fiable le comportement du stockage interne de la décharge.

#### Comment apprend-il la structure de stockage d’une décharge ?

SimCity 4 ne fournit pas directement tous les détails de l’agencement interne des cellules de la décharge nécessaires à ce calcul.

Le Ministère apprend donc la structure en observant les variations mensuelles normales.

Il compare :

- Les déchets stockés avant le changement de mois.
- Les déchets stockés après.
- La quantité nouvellement absorbée dans le stockage de la décharge.

**Dégradation mensuelle observée de la décharge**

```math
D_{\mathrm{obs}}=
U_{\mathrm{avant}}-U_{\mathrm{après}}+\text{absorbé}
```

Où :

- **Uavant** est la quantité stockée avant la mise à jour mensuelle.
- **Uaprès** est la quantité stockée après.
- **absorbé** est la quantité transférée dans le stockage pendant cette mise à jour.
- **Dobs** est la quantité de déchets éliminée par dégradation.

Le Ministère compare ce résultat avec la formule de dégradation des décharges du jeu.

Il teste différents nombres possibles de cellules de stockage et vérifie s’ils reproduisent **exactement** la dégradation observée.

Le nombre de cellules possible doit aussi diviser la capacité de stockage totale de la décharge en unités entières.

**Lorsqu’un seul agencement correspond aux observations, la géométrie de la décharge est considérée comme calibrée.**

Si plusieurs agencements restent possibles, le Ministère attend d’autres observations mensuelles.

Si aucun ne correspond, ou si la structure de la décharge de la ville ne peut pas être représentée de façon fiable par un seul agencement, il n’utilise pas la prédiction du mois suivant.

Par exemple, une ville comptant plusieurs décharges aux structures de cellules internes différentes peut ne pas produire d’observations compatibles avec un modèle de géométrie unique.

**Combien de temps dure la calibration ?**

Le Ministère apprend à partir des mises à jour mensuelles normales, sans que vous ayez à faire quoi que ce soit de particulier.

Dans une ville testée, un seul changement de mois ordinaire a fourni assez d’informations pour identifier la géométrie de la décharge.

La vérification du mois suivant est donc devenue disponible au deuxième événement mensuel de la session.

La calibration peut toutefois demander davantage d’observations, voire être impossible dans certaines villes.

**La géométrie apprise n’est conservée que pour la session de ville en cours.**

Elle n’est pas sauvegardée d’une session à l’autre.

Si la capacité de la décharge change parce que vous zonez ou dézonez des cases, le Ministère recommence à collecter des indices.

#### Comment fonctionne la prédiction du mois suivant ?

Une fois la géométrie de la décharge calibrée, le Ministère peut estimer :

1. Quelle quantité de déchets stockés se dégradera au prochain changement de mois.
2. Quelle quantité de déchets de la vanne d’admission sera absorbée dans le stockage.
3. Quelle quantité de déchets pourra rester dans la vanne.
4. Quelle capacité d’admission de la décharge sera disponible pendant le mois suivant.

Il utilise les formules mensuelles d’absorption du jeu décrites plus haut.

Comme la valeur d’entretien financée par le jeu peut occasionnellement différer d’un simoléon du calcul attendu, le Ministère teste les deux valeurs :

```math
A=\left\lfloor B\times\frac{p}{100}\right\rfloor
```

et la même valeur augmentée de un.

Il remet ensuite à zéro l’utilisation mensuelle simulée de chaque contrat de déchets et répète la projection quotidienne des déchets pour tout le mois suivant, en repartant sans arriéré de déchets non collectés.

**Le niveau de financement n’est accepté que si les deux mois se terminent sans déchets non collectés.**

Si la géométrie de la décharge ne peut pas être calibrée, le Ministère n’utilise que le calcul du mois en cours.

Dans cette situation, le gestionnaire des contrats régionaux se montre aussi plus prudent lorsqu’il envisage de modifier les contrats de déchets existants.

Voir [Accords avec les voisins](deals.md#neighbor-deals).

<a id="garbage-landfills-funding"></a>
#### 3.6 Comment le Ministère choisit-il le financement des déchets ?

Après avoir calculé la quantité de déchets traitable à chaque niveau de financement, le Ministère cherche **le pourcentage le moins cher qui satisfait les besoins d’élimination**.

Les dépenses de gestion des déchets augmentent proportionnellement au financement.

**Dépense mensuelle de gestion des déchets**

```math
E(p)=\mathrm{round}\left(I\times\frac{p}{100}\right)
```

Où :

- **I** est le coût mensuel du service des déchets à 100 % de financement, tel qu’indiqué par le jeu.
- **p** est le pourcentage de financement proposé.
- **E(p)** est la dépense mensuelle estimée.

Ce coût comprend l’entretien des décharges et les autres lignes budgétaires du service.

Par exemple, si le service des déchets coûte 2 000 § par mois à 100 % de financement :

| Financement | Dépense mensuelle estimée |
|---|---:|
| 50 % | 1 000 § |
| 75 % | 1 500 § |
| 100 % | 2 000 § |
| 120 % | 2 400 § |

Mais le pourcentage le moins cher n’est pas forcément sûr.

Un budget trop bas peut réduire la capacité de traitement, restreindre l’admission des décharges ou laisser des déchets non collectés à la fin du mois.

**Optimisation du financement**

```math
p^*=
\arg\min_{p\in[0,p_{\max}]}E(p)
```

Sous réserve que :

- Le mois en cours se termine sans déchets non collectés.
- Le mois suivant se termine aussi sans déchets non collectés, lorsque la géométrie de la décharge est calibrée.

Ici, **pmax** est le financement maximal des déchets autorisé par le jeu.

**Comment trouve-t-il le pourcentage adapté le plus bas ?**

Comme la capacité d’élimination modélisée augmente normalement avec le financement, le Ministère peut rechercher efficacement le pourcentage le plus bas qui satisfait les exigences.

Il utilise une recherche dichotomique lorsque la relation est monotone. Si cette hypothèse ne se vérifie pas, il examine les pourcentages disponibles un par un.

Si plusieurs pourcentages donnent la même dépense mensuelle arrondie, il préfère celui qui est le plus proche du financement actuel.

Cela évite des changements inutiles qui ne font pas économiser d’argent.

#### Et si même le financement maximal ne peut pas résorber les déchets ?

Il arrive que le système de gestion des déchets n’ait tout simplement pas assez de capacité d’élimination.

Par exemple, une décharge peut être presque pleine.

Dans ce cas, augmenter le financement des déchets ne crée pas forcément plus d’espace de stockage utilisable.

Le Ministère évite donc de choisir automatiquement 120 % lorsque cela ne servirait à rien.

À la place, il :

1. Calcule le plus petit arriéré atteignable au financement maximal.
2. Cherche le **niveau de financement le moins cher qui produit ce même arriéré**.
3. Choisit ce niveau plutôt que de payer davantage sans amélioration.

**Le Ministère ne dépense pas plus lorsqu’un financement supplémentaire ne peut pas améliorer le résultat.**

Si un financement plus élevé réduit réellement l’arriéré, il peut tout de même choisir le financement maximal.

Les contrats régionaux de déchets peuvent aussi aider à résoudre une pénurie, selon les villes voisines disponibles et les règles des contrats.

<a id="garbage-landfills-writing"></a>
#### 3.7 Comment les changements de financement sont-ils appliqués ?

Contrairement à l’électricité et à l’eau, le financement des déchets **n’est pas toujours modifié directement jusqu’à sa cible finale**.

La raison est que SimCity 4 met plusieurs jours simulés à mettre à jour l’efficacité de la gestion des déchets et les capacités de traitement après une modification du budget.

Effectuer plusieurs grands ajustements avant que le jeu ait fini de se mettre à jour pourrait mener à des décisions instables.

**Le Ministère modifie donc le financement des déchets progressivement lorsqu’il le réduit.**

| Changement | Comment il est appliqué |
|---|---|
| **Réduction du financement** | Au plus **25 points de pourcentage** à la fois, avec au moins **7 jours simulés** entre deux changements de financement des déchets. |
| **Augmentation du financement** | L’augmentation complète nécessaire peut être appliquée en une seule étape, avec au moins **3 jours simulés** depuis le précédent changement de financement des déchets. |

Il s’agit de points de pourcentage, et non de pourcentages du budget actuel.

**Exemple : réduire le financement de 120 % à 31 %**

Au lieu de passer immédiatement de 120 % à 31 %, le Ministère suit cette séquence :

<box gap={2}>
  <row align="center" justify="between">
    <text weight="medium">Financement de départ</text>
    <text weight="medium" tabularNums>120 %</text>
  </row>
  <box background="surface-tertiary" height="8px" radius="full" clip>
    <box background="rgba(100,116,139,0.85)" width="100%" height="100%" radius="full"/>
  </box>
  {#each [{p:95,w:"79.2%",d:"Au moins 7 jours plus tard"},{p:70,w:"58.3%",d:"Au moins 7 jours de plus"},{p:45,w:"37.5%",d:"Au moins 7 jours de plus"},{p:31,w:"25.8%",d:"Au moins 7 jours de plus"}] as s}
    <row align="center" justify="between">
      <text color="secondary" size="sm">{s.d}</text>
      <text weight="medium" tabularNums>{s.p} %</text>
    </row>
    <box background="surface-tertiary" height="8px" radius="full" clip>
      <box background="rgba(100,116,139,0.85)" width={s.w} height="100%" radius="full"/>
    </box>
  {/each}
</box>

Atteindre la cible finale prend donc **au moins plusieurs semaines simulées**, potentiellement environ un mois.

Cela laisse au jeu le temps de mettre à jour son efficacité et sa capacité d’élimination entre les changements.

**Les augmentations de financement sont plus rapides**, car rétablir la capacité d’élimination peut être urgent.

Même les augmentations respectent toutefois l’intervalle minimal de trois jours simulés depuis le précédent ajustement du financement des déchets.

#### Comment le Ministère vérifie-t-il chaque changement ?

Chaque ajustement du financement utilise le propre contrôle budgétaire des déchets de SimCity 4.

Avant d’appliquer un changement, le Ministère relit le financement actuel.

Si vous avez déplacé le curseur à la main depuis le calcul du plan, il annule ce changement en attente et recalcule à partir de votre nouveau réglage.

**Après avoir appliqué le financement, le Ministère effectue deux vérifications.**

**1. Vérification immédiate du financement**

Il confirme que le jeu indique bien le nouveau pourcentage de financement.

Si la valeur n’a pas été acceptée, **la gestion automatique du financement des déchets s’arrête pour le reste de la session de ville en cours**.

Les contrats régionaux de déchets peuvent continuer à être gérés, avec le financement des déchets désormais figé.

**2. Vérification de l’efficacité et de la capacité**

Comme la capacité de gestion des déchets ne se met pas à jour immédiatement, le Ministère attend aussi que les calculs du jeu se mettent à niveau.

Il vérifie que :

- L’efficacité indiquée correspond à la valeur attendue d’après la courbe de financement, à **0,001** près.
- Les capacités des bâtiments de traitement correspondent aux valeurs attendues, à **3 % ou 8 unités de déchets** près, selon la tolérance la plus large.

**Aucun nouveau changement de financement des déchets ni ajustement de contrat de déchets n’est effectué tant que ces vérifications ne sont pas confirmées.**

Cela évite que le Ministère prenne de nouvelles décisions à partir de données de jeu partiellement mises à jour.

**L’objectif n’est pas seulement de trouver le budget de gestion des déchets le moins cher, mais d’y parvenir sans déstabiliser le système d’élimination des déchets de la ville.**
<a id="garbage-landfills-when"></a>
#### 3.8 Quand le Ministère réexamine-t-il le financement des déchets ?

Le Ministère réexamine le financement des déchets **au début de chaque mois simulé**.

Il peut aussi recalculer le budget plus tôt lorsque la situation des déchets de la ville change de façon significative.

Une évaluation anticipée est déclenchée lorsque :

- La **production de déchets** change d’au moins 1 %.
- La **capacité de traitement des incinérateurs ou des centrales de valorisation énergétique des déchets** change d’au moins 1 %.
- L’**efficacité de la gestion des déchets** change d’au moins 1 %.
- Les **importations ou exportations de déchets** changent d’au moins 1 %.
- Des **déchets non collectés apparaissent ou disparaissent** de la ville.

Ces changements sont comparés aux valeurs enregistrées lors de l’évaluation précédente.

**Le Ministère ne répète pas le même calcul lorsque rien n’a changé.**

Par exemple, si votre ville construit un nouvel incinérateur, l’augmentation de la capacité de traitement peut déclencher une nouvelle évaluation sans attendre le mois suivant.

De même, si des déchets commencent à s’accumuler dans les rues, le Ministère peut vérifier si un financement supplémentaire est nécessaire.

Chaque évaluation utilise les dernières mesures disponibles et respecte les ajustements progressifs du financement décrits à la [section 3.7](#garbage-landfills-writing).

<a id="garbage-landfills-advice"></a>
#### 3.9 Conseil sur la taille de la décharge : quand agrandir ou réduire une décharge peut faire économiser de l’argent

Il arrive que modifier la taille d’une décharge réduise les dépenses de gestion des déchets de votre ville.

Cela peut sembler surprenant. Ajouter des cases de décharge augmente les coûts d’entretien, mais peut aussi permettre au Ministère de faire fonctionner le système de gestion des déchets à un pourcentage de financement plus bas.

À l’inverse, retirer des cases inutilisées peut réduire l’entretien sans affecter la capacité de la ville à éliminer ses déchets.

Le Ministère peut analyser ces deux possibilités et recommander un changement lorsque l’économie attendue est significative.

**Cette fonction est désactivée par défaut.**

Vous pouvez l’activer avec l’option :

**M’avertir si revoir la décharge permet d’économiser**

Le Ministère recherche alors deux types de recommandations :

| Recommandation | Ce que cela signifie |
|---|---|
| **AGRANDIR** | Ajouter une zone précise à votre décharge pourrait réduire la facture mensuelle totale de gestion des déchets. |
| **RÉDUIRE** | Retirer une zone inutilisée précise pourrait réduire les coûts d’entretien sans compromettre l’élimination des déchets. |

**Le Ministère ne modifie jamais automatiquement le zonage de la décharge pour ces recommandations.**

Il affiche uniquement un conseil. C’est à vous de décider de le suivre ou non, et c’est vous qui effectuez le zonage ou le dézonage.

Le conseil ne modifie pas non plus votre financement des déchets ni vos contrats régionaux.

#### Quelles décharges peuvent être analysées ?

Le Ministère ne donne de conseil sur la taille que lorsqu’il peut comprendre avec certitude la structure de la décharge.

Il exige :

- **Exactement une zone de décharge** dans la ville.
- Une version du jeu prise en charge (**SimCity 4 Deluxe 1.1.641.0**).
- Un accès fiable aux cases de la décharge, à ses cellules de stockage, à ses tas de déchets et à ses coûts d’entretien.
- Une concordance totale entre la structure interne de la décharge et les totaux indiqués par le jeu.

Il vérifie que la capacité, les déchets stockés, l’entretien, le nombre de cases et le contenu des cellules sont cohérents.

Il vérifie aussi que chaque case appartient bien à la zone attendue.

**Si une information est incohérente, le Ministère ne fait aucune recommandation.**

Cela évite des suggestions fondées sur des données de décharge incomplètes ou erronées.

#### Comment le Ministère trouve-t-il des zones d’agrandissement sûres ?

Pour une recommandation **AGRANDIR**, le Ministère cherche des zones rectangulaires de terrain inutilisé à côté de la décharge existante.

Il n’envisage que des agrandissements vers la **droite ou le bas de la décharge, dans les coordonnées internes de la carte du jeu**.

Cette restriction est importante, car agrandir vers le haut ou vers la gauche peut décaler les cellules de stockage internes de la décharge et perturber les déchets qui y sont stockés.

La zone proposée doit remplir plusieurs conditions :

- Elle doit toucher la décharge existante le long de son bord droit ou bas.
- Elle doit partir d’un coin du bord choisi.
- Elle doit faire **entre 2 et 8 cases de profondeur**.
- Elle doit faire **au moins 2 cases de long**, sans dépasser le bord adjacent ni la limite de 48 cases du jeu.
- Elle ne doit pas contenir plus de **64 cases**.
- Toutes les cases doivent être disponibles, dans la carte, et ne pas être déjà occupées ni zonées.
- Elle ne doit pas fusionner avec une autre zone de décharge.
- Elle ne doit pas provoquer le refoulement de déchets stockés vers la vanne d’admission de la décharge.

Le Ministère calcule comment la nouvelle forme affecterait les cellules de stockage et la capacité de la décharge.

Il effectue aussi un **essai à blanc avec les propres vérifications de zonage du jeu**, pour confirmer que SimCity 4 accepterait réellement la zone proposée.

Si le jeu rejetait l’opération ou omettait silencieusement une case, ce candidat est écarté.

**Seuls les agrandissements vérifiés comme sûrs sont retenus.**

Lorsque plusieurs emplacements sûrs ont le même nombre de cases, le Ministère garde celui qui donne la plus grande capacité de stockage résultante.

#### Comment le Ministère trouve-t-il des zones à retirer sans risque ?

Pour une recommandation **RÉDUIRE**, le Ministère cherche des cases inutilisées le long des bords extérieurs de la décharge.

Il envisage de retirer **de 1 à 8 rangées ou colonnes extérieures complètes**, formant une zone rectangulaire de 64 cases au plus.

La réduction proposée doit satisfaire toutes ces conditions :

- Aucune case retirée ne contient de tas de déchets.
- Aucune des cellules de stockage concernées ne contient de déchets.
- Aucun déchet stocké ne serait renvoyé vers la vanne d’admission.
- La décharge restante doit rester connectée en une seule zone.
- Au moins un **bloc de décharge 2 × 2** complet doit subsister.
- La vérification de dézonage propre au jeu doit accepter l’opération.

**Le Ministère ne recommande jamais de retirer des cases qui perturberaient les déchets stockés.**

Par exemple, une grande décharge peut avoir plusieurs rangées entièrement inutilisées le long d’un bord.

Retirer ces rangées pourrait réduire l’entretien sans affecter les déchets déjà stockés ni les besoins d’élimination de la ville.

Mais si leur retrait endommageait la structure de stockage interne de la décharge, la suggestion est rejetée.

#### Comment le Ministère calcule-t-il les économies potentielles ?

Le Ministère prend en compte deux sources d’économies :

1. Les variations du coût d’entretien de la décharge.
2. Les variations du financement des déchets nécessaire après le redimensionnement de la décharge.

**Variations de l’entretien de la décharge**

```math
\begin{aligned}
\text{AGRANDIR :}\quad
\Delta B &= 5n_{\text{ajoutées}}-5g_{\text{corrigées}}
\\
L' &= L+n
\\[6pt]
\text{RÉDUIRE :}\quad
\Delta B &= -5n
\\
L' &= L-n
\end{aligned}
```

Où :

- **n** est le nombre de cases de décharge ajoutées ou retirées.
- **g<sub>corrigées</sub>** est le nombre de cases fantômes supprimées lors de l’agrandissement.
- **ΔB** est la variation de l’entretien mensuel de la décharge à 100 % de financement.
- **L** et **L′** sont le nombre de cases de la décharge avant et après la modification.

Les nouvelles cases de décharge vides coûtent **5 § chacune par mois** à 100 % de financement.

Retirer une case inutilisée économise cette dépense d’entretien.

Un agrandissement peut aussi supprimer des cases fantômes qui coûtent déjà de l’argent, ce qui peut compenser en partie l’entretien de la zone nouvellement ajoutée.

Le Ministère recalcule aussi la capacité de stockage de la décharge :

```math
C'=\mathrm{round}\left(\frac{T'\times30000}{N'}\right)
\qquad
T'_{\mathrm{cap}}=N'\times C'
```

Où **T′** est le nombre de cases appartenant à la zone résultante et **N′** le nombre de cellules de stockage actives résultant.

Le nouveau coût mensuel nominal du service des déchets est :

```math
I'=I+\Delta B
```

**Ce qui compte, c’est ce qui se passe ensuite.**

Le Ministère ne se contente pas de multiplier par 5 § le nombre de cases ajoutées ou retirées.

Il recalcule aussi **le financement des déchets le moins cher requis après la modification proposée**, avec les mêmes prévisions de déchets, la même réserve, les mêmes contrats et le même calendrier.

**Économie mensuelle estimée**

```math
\begin{aligned}
E_0 &= \mathrm{round}\left(I\times\frac{p_0}{100}\right)\\
E_1 &= \mathrm{round}\left(I'\times\frac{p_1}{100}\right)\\
\text{Économie} &= E_0-E_1
\end{aligned}
```

Où :

- **E₀** est la dépense mensuelle de la décharge dans sa configuration d’origine.
- **E₁** est la dépense mensuelle après la modification proposée.
- **p₀** est le financement que le Ministère choisirait pour la décharge actuelle.
- **p₁** est le financement qu’il choisirait après la modification proposée.

Les deux niveaux de financement sont calculés par l’optimiseur de gestion des déchets.

**La comparaison utilise le financement que le Ministère choisirait aujourd’hui, pas nécessairement celui qu’affiche actuellement votre curseur de budget.**

C’est important, car les réductions du financement des déchets se font progressivement. Une réduction inachevée ne doit pas être confondue avec une économie due au redimensionnement de la décharge.

Une modification proposée n’est acceptée comme conseil que si le plan de gestion des déchets qui en résulte peut toujours résorber les déchets requis, y compris le mois suivant lorsque cette vérification supplémentaire est disponible.

Le calcul utilise les coûts d’entretien au moment de l’opération de zonage proposée. Les hausses futures dues à de nouveaux tas de déchets ne sont pas incluses, et la dépense ponctuelle de zonage n’est pas comptée dans les économies mensuelles.

#### Combien une modification doit-elle faire économiser pour que le Ministère la recommande ?

Vous pouvez configurer le **Seuil d’économie des alertes de décharge**.

La valeur par défaut est de **25 %**, réglable de **20 % à 50 %**.

Le Ministère ne recommande une modification que lorsque son économie mensuelle attendue est **strictement supérieure à votre seuil**.

**Seuil de conseil**

```math
\text{Économie}\times100>\theta\times E_0
```

Où **θ** est votre seuil d’économie configuré.

Le pourcentage affiché dans l’avis est arrondi à l’entier inférieur :

```math
\text{Économie affichée (\%)}=
\left\lfloor\frac{100\times\text{Économie}}{E_0}\right\rfloor
```

**Exemple : un budget de gestion des déchets de 1 000 § par mois**

Avec le seuil par défaut de 25 % :

| Économie mensuelle attendue | Pourcentage d’économie | Recommandation |
|---|---:|---|
| 200 § | 20 % | Non |
| 250 § | 25 % | Non |
| **251 §** | **25,1 %** | **Oui** |
| 350 § | 35 % | Oui |

**Une économie d’exactement 25 % ne déclenche pas de recommandation.**

L’économie doit dépasser 25 %, et non simplement l’atteindre.

#### À quelle fréquence le Ministère analyse-t-il la taille de la décharge ?

Le conseil sur la taille de la décharge est volontairement prudent.

Le Ministère évite de remettre en cause à répétition vos décisions de zonage ou de recommander des changements pendant qu’une décharge se stabilise dans sa nouvelle configuration.

Trois règles de temporisation s’appliquent :

| Règle | Ce que cela signifie |
|---|---|
| **Au plus une fois par mois simulé** | Le Ministère n’analyse pas les tailles de décharge à répétition au cours d’un même mois. |
| **Période de protection de 24 mois** | Il n’analyse pas une décharge pendant 24 mois simulés après certains changements, ni lorsque son historique est incertain. |
| **Intervalle de 12 mois entre deux conseils** | Après avoir émis une recommandation, il attend au moins 12 mois simulés avant d’analyser la ville pour en émettre une autre. |

**La période de protection de 24 mois**

Le Ministère attend 24 mois simulés après :

- Un agrandissement ou une réduction de décharge de votre part.
- La première fois qu’il rencontre une ville qui contient déjà une décharge, car il ignore quand cette décharge a été créée ou modifiée pour la dernière fois.
- Le chargement d’une ancienne sauvegarde qui fait reculer le calendrier du jeu.

Cela évite des conseils prématurés tant que l’entretien de la décharge, la répartition des déchets et les calculs de stockage interne évoluent encore.

**L’intervalle de 12 mois entre deux conseils**

Après avoir fourni une recommandation, le Ministère attend 12 mois simulés avant de vérifier à nouveau.

Si la même opportunité dépasse toujours votre seuil, elle peut être recommandée de nouveau.

Désactiver puis réactiver l’option, ou modifier le seuil d’économie, **ne réinitialise pas ces périodes d’attente**.

Le Ministère enregistre séparément pour chaque ville les dates et les informations de décharge nécessaires, si bien que les périodes d’attente survivent à la fermeture et à la réouverture du jeu.

Les réparations automatiques de cases fantômes ne relancent pas la période de protection de 24 mois.

#### Comment le Ministère garde-t-il l’analyse rapide ?

Analyser des modifications de zonage potentielles implique d’examiner de nombreuses formes de décharge possibles.

Pour éviter de ralentir SimCity 4, le Ministère limite le travail effectué à chaque analyse.

- Il évalue **au plus 64 candidats** par analyse.
- Il vise un budget de calcul de **10 millisecondes**.
- Il répartit d’abord les évaluations de candidats entre AGRANDIR et RÉDUIRE.
- Si une direction compte moins de candidats, l’allocation d’évaluation restante peut être utilisée par l’autre.
- Si des analyses répétées dépassent le budget de temps, le Ministère réduit progressivement le nombre de candidats examinés.

Le nombre maximal de candidats peut descendre temporairement par paliers : **48, 32, 16, 8, 4 puis 2**, selon les performances.

Après six analyses consécutives dans le budget de temps, la limite peut remonter d’un palier.

Le Ministère examine aussi les petites tailles de candidats avant de recommander les plus grandes, car l’économie n’augmente pas forcément avec la taille de la décharge.

**Une conséquence importante :** il ne recommande une taille qu’après avoir évalué toutes les tailles autorisées plus petites. Cela évite de passer à côté d’une modification plus petite et meilleure.

Parmi les candidats sûrs qu’il évalue, le Ministère sélectionne :

1. Le candidat qui offre la plus grande économie mensuelle.
2. À économie égale, le candidat qui utilise le moins de cases.
3. En cas de nouvelle égalité, AGRANDIR avant RÉDUIRE.

L’analyse s’exécute dans le cadre d’une évaluation normale de la gestion des déchets. Elle ne nécessite aucun processus d’arrière-plan distinct.

#### À quoi ressemble la recommandation ?

Le Ministère envoie un message par le **conseiller des services publics** de SimCity 4, classé comme bonne nouvelle.

Deux types d’avis sont possibles :

**Pour un agrandissement :**

> **Plus de décharge, moins de dépenses ?**

**Pour une réduction :**

> **Notre décharge prend trop de place !**

L’avis explique :

- S’il faut agrandir ou réduire la décharge.
- Combien de cases doivent être ajoutées ou retirées.
- La longueur et la profondeur du rectangle proposé.
- L’économie mensuelle estimée en simoléons.
- Le pourcentage du budget de gestion des déchets qui pourrait être économisé.

Il contient aussi un lien cliquable **voir où**.

Sélectionner ce lien déplace la caméra au centre de la zone suggérée.

Le message évite de dire « gauche », « droite », « haut » ou « bas », car la direction que vous voyez à l’écran dépend de la rotation de la vue de votre ville.

**Le Ministère n’enregistre qu’il a émis un conseil que lorsque le jeu accepte avec succès l’avis du conseiller.**

N’oubliez pas : **la recommandation est informative. C’est à vous de décider de modifier ou non votre décharge.**

<a id="ghost-tiles"></a>
#### 3.10 Cases fantômes : réparation automatique

Les cases de décharge fantômes sont différentes d’une décharge surdimensionnée ou inefficace.

Elles relèvent **d’un bug de SimCity 4**, et non d’une décision de zonage.

Comme expliqué plus haut, une case fantôme est un emplacement que le jeu compte encore comme faisant partie d’une décharge, alors qu’il n’est plus réellement zoné en décharge.

Cela peut créer des dépenses d’entretien inutiles.

**Le Ministère peut réparer automatiquement ces cases fantômes lorsqu’il gère la gestion des déchets.**

Contrairement au conseil sur la taille de la décharge, cette réparation :

- Ne nécessite pas de réglage distinct.
- Ne dépend pas d’un seuil d’économie.
- Ne génère aucun avis de conseiller.
- N’ajoute ni ne retire aucune case de décharge légitime.
- Ne démolit aucun bâtiment et ne modifie pas le zonage visible.
- Ne réduit jamais délibérément la capacité de stockage de la décharge.

L’activité de réparation est consignée dans le journal.

#### Comment le Ministère peut-il réparer une case fantôme ?

La réparation s’appuie sur un comportement particulier du système de zonage de SimCity 4.

**Réappliquer le zonage de décharge à des cases déjà légitimement zonées en décharge amène le jeu à reconstruire la structure interne de la zone.**

Cette opération ne coûte pas d’argent et ne modifie pas le type de zone de ces cases, mais la reconstruction supprime aussi l’appartenance obsolète des cases fantômes.

En pratique, le Ministère demande au jeu de reconstruire une décharge selon ses propres règles de zonage, sans modifier la zone de décharge légitime.

#### Comment fonctionne la réparation automatique ?

Le Ministère suit quatre étapes.

**1. Détecter les cases fantômes**

Lors de la première évaluation de la gestion des déchets de chaque mois simulé, il examine les zones de décharge disponibles.

Après l’ouverture d’une ville, cette vérification a lieu au plus tard lors de la première évaluation mensuelle de la gestion des déchets, à condition que les informations nécessaires soient disponibles.

Il utilise les mêmes informations de décharge validées, en lecture seule, que celles requises pour le conseil sur la taille.

Si les totaux indiqués par le jeu et les données internes de la décharge ne concordent pas, le Ministère ne tente pas de réparation.

**2. Vérifier que la réparation est sûre**

Le Ministère choisit une région contenant des cases de décharge légitimes sans tas de déchets.

Il utilise les dimensions de zonage prises en charge par le jeu et vérifie le résultat attendu avant d’effectuer le moindre changement.

La réparation est refusée si elle risquait de :

- Ajouter de nouvelles cases de décharge.
- Renvoyer des déchets stockés vers la vanne d’admission de la décharge.
- Réduire la capacité de stockage totale de la décharge.
- Fusionner deux zones de décharge distinctes.
- Produire un résultat de capacité incertain à cause des arrondis.
- Affecter une zone que les vérifications de zonage du jeu rejetteraient.

Elle est aussi refusée si aucune case de décharge adaptée, sans tas de déchets, n’est disponible.

La région de réparation sélectionnée doit tenir dans la **limite de zonage de 48 × 48** du jeu.

**3. Effectuer la réparation**

Une fois les vérifications réussies, le Ministère utilise l’opération de zonage propre à SimCity 4 pour réappliquer le zonage de décharge à la région sélectionnée.

L’opération ne doit omettre aucune case en silence.

Si l’appel de zonage échoue ou si le jeu rejette une partie de la région, la tentative est considérée comme un échec.

**4. Vérifier le résultat**

Le jeu traite l’opération de zonage après avoir reçu la demande.

Le Ministère contrôle donc le résultat lors des évaluations suivantes, avec jusqu’à **quatre vérifications de confirmation**.

Il vérifie que :

- Le rectangle englobant de la décharge n’a pas changé.
- Les cases fantômes ont disparu.
- L’agencement des cellules de stockage et la capacité correspondent au résultat prévu.
- Le nombre de cases de décharge légitimes reste inchangé.
- La capacité de décharge de la ville correspond à la valeur attendue.
- Les informations internes de la décharge concordent toujours avec les totaux du jeu.

La quantité de déchets stockés évolue naturellement au fil du jeu ; elle est donc enregistrée, sans qu’on exige qu’elle reste identique.

**La réparation n’est acceptée que si la structure de décharge obtenue correspond au résultat sûr attendu.**

#### Que se passe-t-il si une réparation échoue ?

Le Ministère ne retente pas à répétition des réparations risquées.

Chaque état distinct de cases fantômes est tenté **au plus une fois par session de ville**.

Si l’opération de zonage échoue ou si la structure obtenue ne correspond pas à la prévision, **la réparation automatique des cases fantômes est désactivée jusqu’à la fermeture de SimCity 4**.

Les autres fonctions du Ministère peuvent continuer à fonctionner.

Le Ministère ne répare aussi **qu’une seule zone de décharge par mois simulé**.

Si plusieurs zones contiennent des cases fantômes, les zones restantes sont examinées lors des mois suivants.

Pendant qu’une réparation attend sa confirmation, le conseil sur la taille de la décharge est reporté, pour ne pas analyser une zone dont la structure interne est encore en train de changer.

#### Pourquoi les cases fantômes pourraient-elles revenir ?

Malheureusement, le comportement de sauvegarde et de chargement du jeu d’origine n’est pas corrigé de façon permanente.

Lorsque vous rouvrez une ville, SimCity 4 peut recréer l’appartenance fantôme le long de la dernière rangée ou colonne du rectangle englobant interne d’une décharge.

**Le Ministère peut donc devoir réparer à nouveau les mêmes cases fantômes après la réouverture de la ville.**

Cela ne signifie pas que la réparation précédente a échoué. Cela signifie que le jeu a recréé le bug pendant le chargement.

Chaque case fantôme supprimée fait économiser **5 § par mois simulé à 100 % de financement des déchets**.

L’économie réelle à un pourcentage de financement plus bas est proportionnellement plus faible.

**La réparation automatique des cases fantômes est une correction prudente d’un bug du jeu, et non une refonte automatique de la décharge.**

<a id="garbage-landfills-safety"></a>
### 4. Conditions de sécurité

Le Ministère applique des règles de sécurité distinctes au financement des déchets, au conseil sur la taille de la décharge et à la réparation des cases fantômes.

#### Financement automatique des déchets

- **La gestion doit être activée.** **Ministère actif** et **Gérer les déchets** doivent tous deux être activés.
- **Seuls des niveaux de financement valides sont utilisés.** Le financement reste dans les limites acceptées par le système budgétaire des déchets du jeu.
- **Un financement fixe est respecté.** Le Ministère ne modifie pas un budget de gestion des déchets que le jeu signale comme fixe.
- **La réserve configurée est toujours incluse.** Le plan de gestion des déchets doit couvrir le besoin de déchets protégé.
- **Le mois projeté doit se terminer sans déchets non collectés.** Lorsque les prédictions du mois suivant sont disponibles, le mois suivant doit lui aussi être réalisable.
- **Une information peu fiable n’est jamais traitée comme de la capacité disponible.** Une vanne de décharge illisible n’apporte aucune admission supposée, et une exportation de déchets illisible n’apporte aucun soulagement d’élimination supposé.
- **Les changements de financement sont progressifs et vérifiés.** Les réductions suivent les règles de rythme, et aucun nouveau changement n’est tenté avant que le précédent ait été confirmé.
- **Les défaillances techniques arrêtent les changements de financement pour la session.** Le Ministère évite de poursuivre avec un contrôle de financement peu fiable.

Si **Gérer les déchets** est désactivé, le Ministère laisse le budget des déchets inchangé.

Les contrats régionaux de déchets peuvent toujours être gérés indépendamment, en utilisant le financement actuel pour calculer la capacité d’élimination disponible.

#### Conseil sur la taille de la décharge

**Le conseil sur la taille de la décharge ne modifie jamais le zonage ni le financement.**

Il n’est disponible que lorsque le financement des déchets est géré et que les informations internes de la décharge peuvent être vérifiées exactement.

Le Ministère exige aussi :

- Exactement une zone de décharge.
- Une modification proposée sûre, qui préserve les déchets stockés.
- Aucun risque de fusion de zones.
- Un agrandissement qui ne décale pas la position des cellules de stockage de la décharge.
- L’approbation de la validation de zonage ou de dézonage propre au jeu.
- Une économie strictement supérieure au seuil configuré.

Si ces conditions ne sont pas remplies, **aucun conseil n’est émis**.

#### Réparation automatique des cases fantômes

La réparation automatique des cases fantômes n’est disponible que sur la version prise en charge, **SimCity 4 Deluxe 1.1.641.0**, tant que la gestion des déchets est active.

Chaque réparation doit :

- Utiliser des données de décharge validées.
- Préserver toutes les cases de décharge légitimes.
- Éviter de modifier des types de zone ou de démolir quoi que ce soit.
- Préserver ou augmenter la capacité de stockage totale de la décharge.
- Éviter de perturber les déchets stockés ou de les renvoyer vers la vanne d’admission.
- Éviter de fusionner des zones ou de reposer sur un arrondi incertain.
- Réussir la validation de zonage propre au jeu.
- Produire exactement le résultat attendu lorsqu’on le vérifie ensuite.

Si la vérification échoue, les réparations automatiques s’arrêtent pour le reste de la session de jeu.

**Le Ministère préfère toujours laisser une décharge inchangée plutôt que d’effectuer une modification qu’il ne peut pas vérifier comme sûre.**

<a id="garbage-landfills-settings"></a>
### 5. Réglages

La gestion des déchets et des décharges se configure dans **Options de jeu → Ministère de l’Optimisation**, sur la page **Services**.

| Réglage | Par défaut | Plage | Rôle |
|---|---|---|---|
| [Ministère actif](options.md#opt-master) | Activé | Activé / Désactivé | Active ou désactive l’ensemble du Ministère. |
| [Gérer les déchets](options.md#opt-sanitation) | Activé | Activé / Désactivé | Gère automatiquement le financement des déchets. Également requis pour le conseil sur la taille de la décharge et la réparation des cases fantômes. |
| [Réserve de traitement des déchets](options.md#opt-sanitationreserve) | 2 % | 2–20 % | Ajoute de la capacité d’élimination de réserve pour absorber les hausses de production de déchets. |
| [Gérer les accords avec les voisins](options.md#opt-deals) | Activé | Activé / Désactivé | Active la gestion automatique des contrats régionaux. |
| [Gérer les accords de déchets](options.md#opt-tradegarbage) | Activé | Activé / Désactivé | Active les importations et exportations automatiques de déchets lorsque la gestion des accords avec les voisins est active. |
| [M’avertir si revoir la décharge permet d’économiser](options.md#opt-landfilladvice) | Désactivé | Activé / Désactivé | Active les recommandations d’agrandir ou de réduire les zones de décharge lorsque des économies importantes sont possibles. |
| [Seuil d’économie des alertes de décharge](options.md#opt-landfillthreshold) | 25 % | 20–50 % | Définit l’économie minimale, en pourcentage, requise avant que le Ministère recommande de modifier la taille de la décharge. |

#### Comment ces options fonctionnent-elles ensemble ?

**Gérer les déchets**

Lorsque cette option est activée, le Ministère calcule et ajuste automatiquement le budget de gestion des déchets.

Si elle est désactivée, le financement des déchets reste inchangé. Les contrats régionaux de déchets peuvent toujours être gérés indépendamment, avec le financement existant.

Le conseil sur la taille de la décharge et la réparation automatique des cases fantômes exigent que la gestion des déchets soit active.

**Réserve de traitement des déchets**

La réserve par défaut est de **2 %**.

Par exemple, si votre ville produit 100 000 unités de déchets par mois après recyclage, le Ministère prévoit de traiter au moins **102 000 unités**, avant de compter les importations ou les déchets non collectés existants.

Augmenter cette réserve offre une marge supplémentaire face à la hausse de la production de déchets, mais peut exiger un financement plus élevé.

**Conseil sur la taille de la décharge**

Cette fonction est **désactivée par défaut**.

Lorsqu’elle est activée, le Ministère peut recommander :

- **AGRANDIR :** ajouter des cases de décharge si la capacité d’élimination supplémentaire permettait une réduction suffisamment importante des dépenses de gestion des déchets.
- **RÉDUIRE :** retirer des cases de décharge inutilisées si cela réduisait les coûts d’entretien sans compromettre la capacité d’élimination.

**Ces recommandations ne modifient jamais automatiquement le zonage de la décharge.**

L’option de conseil est indisponible dans la fenêtre des réglages tant que le Ministère ou la gestion des déchets est désactivé, mais sa valeur enregistrée est conservée.

**Seuil d’économie des alertes de décharge**

Le seuil par défaut est de **25 %**.

Une modification de décharge proposée doit faire économiser **plus de 25 %** de la dépense mensuelle optimisée de gestion des déchets avant qu’un avis de conseiller soit émis.

Le réglage du seuil est indisponible tant que le conseil sur la taille de la décharge est désactivé, mais sa valeur est conservée.

#### Réglages avancés (fichier INI)

Ces options sont aussi enregistrées dans le fichier de configuration des services du Ministère :

| Clé INI | Rôle |
|---|---|
| `AutoGarbage` | Financement automatique des déchets |
| `GarbageReservePercent` | Réserve de sécurité des déchets |
| `AutoTradeGarbage` | Contrats de déchets automatiques |
| `WarnLandfillSizeSavings` | Conseil d’agrandissement et de réduction de la décharge |
| `LandfillSavingsAlertThreshold` | Économie minimale requise pour un conseil |

**La réparation automatique des cases fantômes n’a pas de réglage distinct.** Elle fait partie de la gestion des déchets, sur la version du jeu prise en charge, chaque fois que les vérifications de sécurité requises réussissent.

<a id="garbage-landfills-examples"></a>
### 6. Exemples détaillés

Les exemples suivants montrent comment le financement de la gestion des déchets, le stockage des décharges et les règles de sécurité du Ministère fonctionnent en pratique.

Plusieurs exemples utilisent une ville de test où le comportement des décharges du jeu a été mesuré.

#### Exemple 1 : Une décharge avec beaucoup d’espace de stockage mais une admission mensuelle limitée

Prenons une décharge avec :

- **48 cases de décharge.**
- **Financement de la gestion des déchets :** 50 %.
- **Efficacité de la gestion des déchets :** 25 %.
- **Espace de stockage disponible :** 900 000 unités de déchets.
- **Déchets déjà acceptés ce mois-ci :** 41 000 unités.
- **Date actuelle :** jour 20 du mois simulé.

**Combien de déchets peut-elle encore accepter ?**

D’abord, le Ministère calcule la limite d’admission mensuelle :

```math
5000 \times 48 \times 0.25 = 60000
```

La décharge peut accepter environ **60 000 unités pendant tout le mois**.

Cependant, 41 000 unités sont déjà entrées par la vanne de la décharge.

```math
60000-41000=19000
```

**Il ne reste que 19 000 unités de capacité d’admission pour le mois**, alors que la décharge dispose de 900 000 unités de stockage libre.

Imaginons maintenant que l’on augmente le financement de la gestion des déchets à **61 %**.

Avec ce financement, l’efficacité passe à 40,125 %.

```math
5000\times48\times0.40125=96300
```

L’admission mensuelle restante devient :

```math
96300-41000=55300
```

| Financement | Limite d’admission mensuelle | Admission restante |
|---|---:|---:|
| 50 % | 60 000 | 19 000 |
| **61 %** | **96 300** | **55 300** |

Une fois l’efficacité du jeu mise à jour, le financement plus élevé procure **36 300 unités d’admission mensuelle supplémentaires**.

**La distinction importante :** une décharge peut avoir une énorme capacité de stockage et pourtant ne plus pouvoir accepter de déchets parce que sa limite d’admission mensuelle est atteinte.

C’est pourquoi le Ministère tient compte à la fois du stockage et de l’admission lorsqu’il choisit le financement de la gestion des déchets.

#### Exemple 2 : Les contrats de déchets juste après un changement de mois

Imaginons que votre ville ait un contrat d’exportation de **7 000 unités de déchets par mois**.

Le jeu exporte normalement :

```math
7000\times0.10=700
```

Cela représente **700 unités par jour simulé**, pendant environ dix jours.

Cependant, SimCity 4 peut afficher temporairement l’utilisation du contrat du mois précédent après le changement de calendrier.

**Le premier jour d’un nouveau mois**, le jeu peut encore afficher :

*Quantité utilisée : 7 000*

Si le Ministère acceptait cette valeur, il supposerait à tort que toute l’allocation d’exportation du nouveau mois est déjà épuisée.

À la place, il vérifie si la quantité indiquée aurait pu être réellement livrée depuis le début du mois.

Le jour 1 :

```math
7000>(1-1)\times700
```

L’utilisation indiquée est impossible pour le nouveau mois, donc **le Ministère la traite comme nulle**.

Il planifie ensuite l’exportation de 7 000 unités sur les jours à venir.

**Que se passe-t-il le jour 4 ?**

Supposons que le jeu indique 2 100 unités utilisées.

```math
2100\le(4-1)\times700
```

Cette valeur est cohérente avec trois livraisons quotidiennes terminées.

**Le Ministère accepte la lecture de 2 100 unités.**

Cette protection évite que des statistiques de contrat périmées ne faussent les calculs de gestion des déchets pendant les trois premiers jours d’un mois.

#### Exemple 3 : Comment les déchets stockés disparaissent avec le temps

Cet exemple provient de la **ville de test**.

La décharge présente :

- **Capacité par cellule de stockage :** 42 000 unités.
- **Total des déchets stockés :** 985 887 unités.

SimCity 4 stocke les déchets de manière compacte, en remplissant les cellules avant d’en entamer de nouvelles.

Les déchets stockés occupent donc :

- **23 cellules entièrement pleines :** 966 000 unités.
- **Une cellule partiellement remplie :** 19 887 unités.

Au début de chaque mois simulé, le jeu retire une partie des déchets stockés par son mécanisme de décomposition.

**Décomposition des cellules pleines**

Chaque cellule pleine perd au maximum 1 500 unités par mois.

```math
23\times1500=34500
```

**Décomposition de la cellule partiellement remplie**

Le jeu retire environ 7 % de son contenu :

```math
19887\times0.07=1392.09
```

Arrondi à l’unité la plus proche, cela donne **1 392 unités**.

**Décomposition mensuelle totale**

```math
D(U)=34500+1392=35892
```

<text strong>Résultat : 35 892 unités de déchets disparaissent du stockage lors de la mise à jour mensuelle.</text>

Cela se produit par le fonctionnement normal des décharges du jeu, sans aucune intervention du Ministère.

**Pourquoi est-ce utile ?**

Le stockage d’une décharge n’est pas entièrement permanent. Une partie des déchets stockés disparaît chaque mois, ce qui libère progressivement de la capacité.

Le Ministère utilise ce comportement lorsqu’il prédit la quantité de déchets qu’une décharge pourra contenir au fil du temps.

#### Exemple 4 : Réparer les cases fantômes dans une ville de test

Dans la ville de test, la décharge contenait **sept cases fantômes**.

Il s’agissait de cases que le jeu comptait encore comme faisant partie de la décharge alors qu’elles n’étaient plus réellement zonées en décharge.

La structure de décharge enregistrée était la suivante :

| Propriété | Avant la réparation |
|---|---:|
| Rectangle englobant | 6 × 9 cases |
| Positions possibles de cellules de stockage | 40 |
| Cellules de stockage actives | 30 |
| Appartenance enregistrée à la décharge | 49 cases |
| Cases de décharge réelles | 42 |
| **Cases fantômes** | **7** |
| Capacité de stockage par cellule active | 42 000 |
| Capacité de stockage totale | 1 260 000 |
| Entretien mensuel à 100 % de financement | 1 030 § |

Les sept fantômes se trouvaient tous le long de la dernière ligne ou colonne du rectangle interne de la décharge.

**Qu’a calculé le Ministère ?**

Supprimer les sept fantômes laisserait :

- 42 cases de décharge légitimes.
- 30 cellules de stockage actives.

La capacité par cellule qui en résulterait serait :

```math
C'=\frac{42\times30000}{30}=42000
```

Capacité totale :

```math
30\times42000=1260000
```

**La capacité de stockage resterait exactement la même.**

Rien ne serait refoulé dans la vanne d’admission de la décharge.

Chaque case fantôme coûtait aussi 5 § par mois à 100 % de financement.

```math
7\times5=35
```

**Économie d’entretien mensuelle attendue : 35 §.**

Le Ministère a effectué la réparation automatique en passant par le système de zonage du jeu.

**Résultats observés**

| Propriété | Avant la réparation | Après la réparation |
|---|---:|---:|
| Cases fantômes | 7 | **0** |
| Entretien mensuel | 1 030 § | **995 §** |
| Capacité de stockage | 1 260 000 | **1 260 000** |

Tous les résultats correspondaient à la prédiction du Ministère.

**Mais les fantômes sont revenus après la réouverture de la ville.**

Une copie de la ville de test sauvegardée après la réparation a recréé les mêmes sept cases fantômes lorsqu’elle a été rechargée.

Cela a confirmé le bogue de sauvegarde et de chargement du jeu décrit plus haut.

Le Ministère a pu les réparer à nouveau pendant la nouvelle session de la ville.

**La réparation fonctionne, mais le jeu original peut recréer le problème à chaque chargement de la ville.**

#### Exemple 5 : Un agrandissement qui fait économiser de l’argent, mais pas assez pour être recommandé

Le Ministère a aussi testé l’agrandissement de la décharge de la ville de test.

Au départ, une rue longeait son bord droit.

Les vérifications de zonage du jeu ont rejeté tous les candidats à l’agrandissement de ce côté, parce que le terrain proposé était occupé.

**Le Ministère n’a ni recommandé ni effectué d’agrandissement.**

Une fois le tronçon de rue supprimé, un **agrandissement de 2 × 2** est devenu possible.

Cela ajouterait quatre cases de décharge.

L’opération de zonage supprimerait aussi les sept cases fantômes existantes.

**Nouvelle structure de la décharge**

```math
T'=49+4-7=46
```

La décharge agrandie aurait 32 cellules de stockage actives.

```math
C'=\frac{46\times30000}{32}=43125
```

Capacité de stockage totale :

```math
32\times43125=1380000
```

La capacité de la décharge passerait de **1 260 000 à 1 380 000 unités** sans refouler de déchets stockés dans la vanne.

**Et l’entretien ?**

Les quatre nouvelles cases vides coûteraient :

```math
4\times5=20
```

La suppression des sept fantômes ferait économiser :

```math
7\times5=35
```

Variation nette de l’entretien :

```math
\Delta B=20-35=-15
```

L’entretien mensuel nominal de la décharge passerait de **1 030 § à 1 015 §**, alors même que la décharge est devenue plus grande.

L’agrandissement a été réalisé pendant les tests, et les chiffres observés dans le jeu correspondaient aux prédictions.

**Le Ministère le recommanderait-il ?**

Non.

La réduction estimée de la facture totale de gestion des déchets n’était que d’environ **4 à 5 %**.

Le seuil d’alerte minimal configurable est de 20 %, et la valeur par défaut est de 25 %.

**L’agrandissement était sans risque et pouvait faire économiser de l’argent, mais l’économie était trop faible pour justifier un avis du conseiller.**

Cela illustre pourquoi le Ministère ne recommande pas tous les changements de zonage techniquement avantageux.

#### Exemple 6 : Pourquoi le Ministère refuse parfois de réparer les fantômes

Une autre expérience dans la ville de test a montré pourquoi la réparation des cases fantômes doit rester prudente.

La décharge d’origine comptait :

- **49 cases enregistrées**, dont sept fantômes.
- **30 cellules de stockage actives.**
- **Capacité totale :** 1 260 000 unités.

Une rangée extérieure vide contenant cinq cases de décharge légitimes a été supprimée alors que les fantômes étaient toujours présents.

Après cette opération :

- L’appartenance enregistrée est tombée à **44 cases**.
- Les cellules de stockage actives sont tombées à **25**.

La nouvelle capacité par cellule est devenue :

```math
C'=\frac{44\times30000}{25}=52800
```

Capacité de stockage totale :

```math
25\times52800=1320000
```

**Étonnamment, la capacité totale de la décharge a augmenté jusqu’à 1 320 000 unités**, alors que cinq cases avaient été supprimées.

Cela s’explique par le fait que les cases fantômes comptaient toujours dans l’appartenance enregistrée utilisée par le calcul de la capacité.

**Que se passerait-il si le Ministère supprimait ensuite les sept fantômes ?**

L’appartenance enregistrée passerait de 44 à 37 cases.

Avec les mêmes 25 cellules actives :

```math
C'=\frac{37\times30000}{25}=44400
```

La capacité de chaque cellule passerait de 52 800 à 44 400 unités.

Le modèle du Ministère prévoyait que **151 200 unités de déchets stockés seraient refoulées dans la vanne d’admission de la décharge**.

Cela perturberait le système de gestion des déchets.

**Le Ministère a donc refusé la réparation des fantômes et laissé la décharge inchangée.**

C’est un principe de sécurité important :

**Supprimer des cases fantômes n’est autorisé que lorsque le Ministère peut prouver que l’opération ne réduira pas la capacité de stockage et ne perturbera pas les déchets existants.**

Économiser quelques simoleons ne vaut pas le risque de mettre en danger les déchets stockés d’une décharge.

#### Exemple 7 : Quand un avis d’économie sur la décharge apparaît-il ?

Supposons que le Ministère calcule que le budget de gestion des déchets actuel le moins cher est de :

**1 015 § par mois.**

Il trouve ensuite une modification de la taille de la décharge, sans risque, qui ramènerait la dépense optimisée à :

**760 § par mois.**

L’économie attendue est de :

```math
1015-760=255
```

Cela représente **255 § économisés chaque mois simulé**.

Avec le seuil d’alerte par défaut de 25 %, le Ministère vérifie :

```math
255\times100>25\times1015
```

```math
25500>25375
```

La condition est remplie.

**Le Ministère émet une recommandation.**

L’économie réelle est d’environ **25,12 %**, bien que l’avis affiche **25 %**, car le pourcentage est arrondi à l’inférieur.

| Seuil d’alerte | Résultat |
|---|---|
| 20 % | Recommandation émise |
| **25 % (par défaut)** | **Recommandation émise** |
| 30 % | Aucune recommandation |

**Un détail important :** la comparaison utilise l’économie réelle avant l’arrondi du pourcentage affiché.

C’est pourquoi un avis peut afficher 25 % alors que la règle exige une économie strictement supérieure à 25 %.

<a id="garbage-landfills-limitations"></a>
### 7. Limites connues

Le modèle de gestion des déchets du Ministère repose sur des observations détaillées de SimCity 4, mais plusieurs limites subsistent.

#### Production de déchets et financement

- **La production future de déchets est estimée.** Le Ministère utilise la production du mois précédent moins le recyclage et suppose que la quantité obtenue est produite uniformément sur les jours restants du mois en cours et du mois suivant. Il ne prédit pas la croissance future de la ville. Les changements sont pris en compte dès que de nouvelles informations sont disponibles.
- **Le financement peut changer pendant le mois.** Les importations, les exportations et l’admission en décharge ne suivent pas un calendrier parfaitement uniforme. Le niveau de financement le moins cher peut donc évoluer au fil du mois. Dans la ville de test, les décisions observées comprenaient **47 %, 63 % et 53 %** à différents moments. C’est un comportement attendu, pas nécessairement une anomalie.
- **Le Ministère utilise les courbes de gestion des déchets du jeu original.** Ses prédictions supposent la courbe d’efficacité standard, une admission en décharge de 5 000 unités par case et par mois, un stockage d’environ 30 000 unités par case, les règles de décomposition standard et un entretien de 5 § pour les cases vides. Les mods qui modifient ces mécanismes peuvent rendre les calculs inexacts. Si l’efficacité du jeu ne correspond pas à la valeur attendue, le Ministère cesse de modifier le financement plutôt que de poursuivre avec des données peu fiables.

#### Prédire le mois suivant

- **La vérification du mois suivant exige une géométrie de décharge calibrée.** Elle nécessite normalement une seule zone de décharge et une observation mensuelle avec au moins une cellule de stockage pleine.
- **La calibration n’est pas permanente.** Le Ministère réapprend la géométrie après la réouverture de la ville et relance la calibration lorsque la capacité de la décharge change.
- **La prédiction suppose que le financement choisi reste en place.** Elle n’anticipe pas tous les changements futurs de production de déchets ni de contrats régionaux.
- **Le modèle de la vanne de la décharge a une limite.** Lorsque la vanne est déjà pleine, une partie des déchets en excès peut ne pas être incluse dans l’estimation du mois suivant. Dans ces situations, le Ministère peut sous-estimer la quantité de déchets qui reste dans la vanne au changement de mois.

Si aucune calibration fiable n’est disponible, le Ministère utilise uniquement sa projection du mois en cours.

#### Conseil sur la taille de la décharge

- **Seules les villes qui ont exactement une zone de décharge sont éligibles.** Le Ministère ne fournit pas de recommandations AGRANDIR ou RÉDUIRE lorsqu’il existe plusieurs zones de décharge.
- **Les zones d’agrandissement sont restreintes.** Seules des zones sûres le long des bords droit ou inférieur des coordonnées internes de la décharge sont envisagées, avec une profondeur maximale de huit cases et une surface maximale de 64 cases.
- **Les réductions sont limitées aux rangées ou colonnes extérieures inutilisées.** Les réductions proposées doivent préserver la structure de la décharge restante et les déchets stockés.
- **Les économies sont des estimations.** Les nouvelles cases de décharge sont d’abord considérées comme vides et coûtent 5 § chacune à 100 % de financement. Leur entretien peut augmenter par la suite lorsque des tas de déchets apparaissent.
- **Les frais de zonage ponctuels ne sont pas inclus.** Le Ministère compare les coûts mensuels récurrents de gestion des déchets, pas le coût initial du changement de zone.
- **Les grosses économies sont rares dans les villes testées.** Les opportunités observées sont généralement restées bien en dessous du seuil d’alerte minimal de 20 %, si bien que le conseil sur la taille de la décharge peut apparaître rarement.

#### Réparation des cases fantômes

- **Certaines cases fantômes reviennent à chaque chargement de la ville.** Le bogue de sauvegarde et de chargement du jeu original peut les recréer le long de la dernière ligne ou colonne du rectangle interne d’une décharge.
- **La réparation ne peut pas corriger durablement le format de sauvegarde du jeu.** Le Ministère peut devoir effectuer la même correction sûre à chaque nouvelle session.
- **Les réparations de fantômes risquées sont refusées.** Une case fantôme reste intacte si la supprimer pouvait réduire la capacité de la décharge, perturber les déchets stockés ou refouler des déchets dans la vanne d’admission.
- **Laisser un fantôme peut quand même coûter de l’argent.** Chaque fantôme non réparé continue de coûter 5 § par mois à 100 % de financement de la gestion des déchets.
- **Les zones de décharge multiples sont traitées séparément.** Le Ministère peut tenter des réparations de fantômes sûres une zone à la fois, mais ne peut normalement pas utiliser la prédiction du mois suivant à géométrie unique ni fournir de conseil sur la taille de la décharge.

#### L’élimination cachée de déchets par le jeu

SimCity 4 peut retirer discrètement une quantité limitée de déchets grâce à son mécanisme intégré d’*enlèvement*.

**Le Ministère ne considère pas cette élimination comme un échec de la gestion des déchets et ne cherche pas à l’empêcher.**

Son objectif est d’empêcher les accumulations réelles et persistantes de déchets non collectés, pas de modifier chaque règle d’élimination cachée du jeu original.

**Le Ministère privilégie l’élimination sûre des déchets et des décisions de financement fiables. Lorsqu’il ne peut pas prédire une opération sur une décharge ou une modification de la gestion des déchets avec une confiance suffisante, il laisse la situation existante inchangée.**

[Retour au sommaire](index.md#contents) | [Retour au README](../../README.fr.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
