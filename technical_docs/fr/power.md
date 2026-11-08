[English](../en/power.md) | [Português](../pt/power.md) | **Français** | [Español](../es/power.md)

[Retour au sommaire](index.md#contents) | [Retour au README](../../README.fr.md)

<a id="power"></a>
## B. Électricité

Le Ministère gère automatiquement **chaque centrale électrique individuellement**, en ajustant son financement pour fournir assez d’électricité à la ville tout en gardant les coûts de fonctionnement aussi bas que possible.

Il prend en compte :

- La quantité d’électricité dont votre ville a besoin.
- La réserve de sécurité que vous avez choisie (**20 % par défaut**).
- L’électricité achetée aux villes voisines ou vendue à celles-ci.
- La capacité de production, l’état et le coût de fonctionnement de chaque centrale.

Le Ministère examine le financement de l’électricité **tous les 10 jours simulés**. Il effectue aussi une vérification rapide chaque jour simulé et peut ajuster le financement plus tôt lorsque la demande, la capacité de production ou les contrats régionaux changent de façon significative.

Plutôt que d’appliquer le même pourcentage de financement à toutes les centrales, il cherche **la combinaison de niveaux de financement la moins chère** qui répond aux besoins de la ville.

Le Ministère ne construit, ne démolit et ne remplace jamais de centrales électriques. Il ne fait qu’ajuster leur financement, dans sa propre plage autorisée et avec ses propres contrôles de sécurité, pour protéger l’approvisionnement en électricité et le réseau de lignes électriques.

<a id="power-vanilla"></a>
### 1. Comment fonctionne l’électricité dans SimCity 4

#### Demande, production et consommation d’électricité

SimCity 4 suit plusieurs valeurs d’électricité pour votre ville :

| Terme | Ce que cela signifie |
|---|---|
| **Demande** | L’électricité réclamée par les bâtiments actuellement alimentés. |
| **Besoin** | L’estimation par le jeu de l’électricité dont la ville a besoin, y compris pour les bâtiments actuellement sans courant. Il est généralement un peu supérieur à la demande. |
| **Consommation** | L’électricité réellement distribuée aux bâtiments. |
| **Capacité de production** | L’électricité que vos centrales peuvent actuellement produire, selon leur financement et leur état. |
| **Importations** | Électricité achetée aux villes voisines. |
| **Exportations** | Électricité vendue aux villes voisines. |

Toutes ces valeurs sont mesurées dans les unités d’électricité du jeu par mois simulé.

**Toutes les pénuries d’électricité ne sont pas dues à une production insuffisante.**

Il y a deux situations possibles :

- **Pas assez d’électricité :** vos centrales et les importations disponibles ne peuvent pas satisfaire la demande de la ville. Le Ministère peut réagir en relevant le financement ou en ajustant les contrats régionaux.
- **L’électricité n’atteint pas certains bâtiments :** la ville a assez de capacité de production, mais certains bâtiments ne sont pas correctement raccordés au réseau électrique. Augmenter le financement n’y changera rien. C’est à vous de corriger les raccordements.

Le Ministère distingue ces situations pour ne pas augmenter inutilement les dépenses d’électricité.

**Comment l’électricité importée influe-t-elle sur la production locale ?**

D’après nos observations, SimCity 4 utilise l’électricité importée avant de puiser dans les centrales locales. Une centrale peut donc parfois afficher une utilisation très faible, voire nulle, pendant qu’une ville voisine fournit l’électricité.

Le Ministère tient compte des contrats régionaux pour décider de la capacité de production locale nécessaire.

#### Comment le financement influe sur la production d’une centrale

Chaque centrale électrique possède trois caractéristiques importantes :

- **Production maximale :** la quantité d’électricité que la centrale peut produire à sa capacité nominale.
- **Financement :** le pourcentage de son budget de fonctionnement alloué à la centrale.
- **État :** l’état actuel de la centrale, de 0 (complètement dégradée) à 1 (neuve).

La production réelle d’une centrale dépend à la fois de **son financement et de son état**.

Par exemple, une centrale neuve financée à 100 % peut produire toute sa capacité nominale. À mesure que son état se dégrade, sa production disponible diminue.

<a id="power-plant-efficiency"></a>
**Rendement d’une centrale (jeu de base)**

```math
\text{Eff} = \frac{\text{Curve}(f)}{100} \times \text{Cond}
```

Où :

- **f** est le pourcentage de financement de la centrale.
- **Curve(f)** est le rendement donné par la courbe de financement du jeu.
- **Cond** est l’état actuel de la centrale, entre 0 et 1.
- **Eff** est le rendement obtenu, exprimé sous forme de fraction.

**Production effective (jeu de base)**

```math
\text{Out} = \text{MaxOut} \times \text{Eff}
```

Où :

- **MaxOut** est la production maximale de la centrale.
- **Out** est sa capacité de production effective.

**Exemple : une centrale d’une production maximale de 7 500 unités**

Supposons que la centrale soit dans un état presque parfait (0,9998).

| Financement | Rendement après état | Production effective |
|---|---:|---:|
| 100 % | Environ 100 % | Environ 7 498 unités |
| 95 % | Environ 90 % | Environ 6 748 unités |

Réduire le financement de seulement **5 points de pourcentage** réduit la capacité de production d’environ **10 %**.

C’est parce que la relation entre financement et production n’est pas proportionnelle dans le jeu.

<a id="power-efficiency-curve"></a>
#### La relation entre le financement et le rendement

SimCity 4 utilise une courbe intégrée pour déterminer la quantité d’électricité qu’une centrale peut produire à chaque niveau de financement.

Voici les points de contrôle du jeu d’origine :

| Financement de la centrale | Rendement de production |
|---:|---:|
| 0 % | 0 % |
| 50 % | 25 % |
| 90 % | 80 % |
| 100 % | 100 % |
| 120 % | 110 % |

**Le détail important est que réduire le financement réduit aussi le rendement.**

Par exemple :

- À **50 % de financement**, une centrale ne fournit que **25 % de sa production nominale**.
- À **90 % de financement**, elle fournit **80 % de sa production nominale**.
- À **100 % de financement**, elle fournit **100 % de sa production nominale**.
- À **120 % de financement**, elle fournit **110 % de sa production nominale**.

Un financement supérieur à 100 % est donc moins rentable : vous payez 20 % de plus pour n’obtenir que 10 % de capacité de production supplémentaire.

**Comment les valeurs intermédiaires sont-elles calculées ?**

Le jeu utilise une interpolation linéaire entre les points de contrôle. Le Ministère applique la même règle.

**Interpolation de la courbe**

```math
y(x) = y_i + \frac{x-x_i}{x_{i+1}-x_i}(y_{i+1}-y_i)
\qquad \text{pour } x_i \le x \le x_{i+1}
```

Cela signifie simplement qu’entre deux points connus, le rendement varie à un rythme constant.

Par exemple, entre 90 % et 100 % de financement, le rendement de production passe de 80 % à 100 %. À 95 % de financement, le rendement est donc de 90 %.

En dehors de la plage définie, la courbe conserve sa première ou sa dernière valeur.

**Rendement aux niveaux de financement utilisés par le Ministère**

Le Ministère considère normalement des niveaux de financement de 60 % à 120 %.

| Financement (%) | 60 | 65 | 70 | 75 | 80 | 85 | 90 | 95 | 100 | 105 | 110 | 115 | 120 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| Rendement (%) | 38,75 | 45,625 | 52,5 | 59,375 | 66,25 | 73,125 | 80 | 90 | 100 | 102,5 | 105 | 107,5 | 110 |
| Production par § dépensé (relative à 100 %) | 0,65 | 0,70 | 0,75 | 0,79 | 0,83 | 0,86 | 0,89 | 0,95 | 1,00 | 0,98 | 0,95 | 0,93 | 0,92 |

La dernière ligne compare la capacité de production fournie par chaque niveau de financement pour l’argent dépensé, par rapport à un financement de 100 %.

**Qu’est-ce que cela signifie pour le budget d’électricité de votre ville ?**

À **100 % de financement**, une centrale électrique fournit le plus de capacité de production par simoléon dépensé.

Cela ne signifie pourtant **pas** que toutes les centrales doivent toujours être financées à 100 %.

Imaginez que votre ville possède deux centrales mais n’a pas besoin de toute leur production cumulée.

Financer les deux centrales de la même façon peut coûter plus cher que de faire tourner l’une près de 100 % et de garder l’autre à un niveau de financement plus bas.

**C’est exactement ce que le Ministère optimise :** il compare les combinaisons disponibles et choisit la moins chère qui fournit encore l’électricité nécessaire et la réserve de sécurité.

Voir l’[Exemple 3](#power-example-mixed) pour un calcul portant sur deux centrales différentes.

#### Financement individuel des centrales et coûts de fonctionnement

Contrairement au service de l’eau, **le financement de l’électricité est géré séparément pour chaque centrale**.

Chaque centrale a son propre réglage de financement et son propre coût de fonctionnement mensuel.

Cela permet au Ministère d’augmenter le financement d’une centrale tout en réduisant celui d’une autre, selon la combinaison la plus économique.

**Quels niveaux de financement le Ministère peut-il utiliser ?**

La configuration d’électricité du jeu indique **60 %** comme son *financement minimal acceptable*. Cette valeur ne doit toutefois pas être confondue avec une limite stricte avérée du contrôle de financement du jeu.

La fonction de réglage du budget local sous-jacente accepte des valeurs à partir de 0 %, dans la limite du maximum du service.

L’optimiseur du Ministère considère délibérément des niveaux de financement de **60 % à 120 %**, en pourcentages entiers, et applique des vérifications supplémentaires avant de modifier le budget d’une centrale.

Ces vérifications garantissent que les changements prévus répondent aux besoins en électricité de la ville et n’entraînent pas de dégradation des lignes électriques.

**Comment le coût de fonctionnement mensuel est-il calculé ?**

Le coût de fonctionnement varie proportionnellement au pourcentage de financement.

**Dépense mensuelle d’une centrale (jeu de base)**

```math
E(f) = \mathrm{round}\left(C \times \frac{f}{100}\right)
```

Où :

- **C** est le coût de fonctionnement mensuel de la centrale à 100 % de financement.
- **f** est le pourcentage de financement choisi.
- **E(f)** est la dépense mensuelle estimée à ce financement.

Par exemple, une centrale qui coûte 253 § par mois à 100 % de financement coûterait environ :

| Financement | Coût de fonctionnement mensuel |
|---|---:|
| 60 % | 152 § |
| 84 % | 213 § |
| 100 % | 253 § |
| 120 % | 304 § |

À **84 % de financement**, la ville économise environ **40 § par mois** par rapport à un financement de cette centrale à 100 %.

Le Ministère ne choisit toutefois 84 % que si la capacité de production obtenue suffit au plan d’électricité global.

Le jeu indique séparément le coût de production nominal d’une centrale et ce qui est effectivement facturé à son financement actuel. Le Ministère utilise cette dernière relation pour estimer les dépenses, en arrondissant aux simoléons entiers, et relit le dernier coût nominal de la centrale à chaque nouveau calcul du plan.

#### Pourquoi le service d’électricité affiche-t-il un autre pourcentage de financement ?

Bien que les centrales soient financées individuellement, SimCity 4 affiche aussi dans le budget de la ville un pourcentage de financement global du service d’électricité.

**Ce n’est pas un curseur de financement supplémentaire.**

Il est calculé comme une **moyenne pondérée par les coûts** des niveaux de financement individuels des centrales.

Cela signifie qu’une grande centrale coûteuse pèse plus lourd sur le pourcentage du service qu’une petite centrale bon marché.

**Financement global de l’électricité (jeu de base)**

```math
F_{\text{serv}} = \frac{\sum_i C_i f_i}{\sum_i C_i}
```

Où :

- **Cᵢ** est le coût de fonctionnement mensuel nominal de la centrale *i*.
- **fᵢ** est le pourcentage de financement de cette centrale.
- **Fserv** est le pourcentage de financement global du service d’électricité.

**Exemple**

Imaginez que les centrales de votre ville ont un coût de fonctionnement nominal cumulé de **11 660 § par mois**.

Une centrale coûte 300 § par mois et est financée à 95 %, tandis que toutes les autres restent à 100 %.

Cette centrale économise 15 § par mois.

Le financement global du service d’électricité devient alors :

```math
F_{\text{serv}} = \frac{11660-15}{11660}\times100
\approx 99.87\%
```

Même si une centrale est financée à 95 %, le service dans son ensemble affiche toujours environ **99,87 % de financement**.

C’est normal.

**Le Ministère gère chaque centrale individuellement. Le budget global de l’électricité reflète le résultat combiné de ces décisions.**
#### Lignes électriques : comment le financement affecte le réseau

Dans SimCity 4, le financement de l’électricité n’affecte pas que les centrales. Il influence aussi l’état du **réseau de lignes électriques** de la ville.

Le jeu utilise le financement global du service d’électricité pour déterminer si les lignes électriques se dégradent ou sont réparées au fil du temps.

| Financement du service | Effet sur les lignes électriques |
|---:|---|
| 0 % | Dégradation rapide (−0,020) |
| 25 % | Dégradation (−0,005) |
| 50 % | Ni dégradation ni réparation (0) |
| 100 % | Réparation (+0,025) |
| 110 % | Réparation plus rapide (+0,050) |

Les valeurs entre ces points sont calculées avec l’interpolation linéaire standard du jeu.

**Le seuil important est 50 % de financement du service :**

- **En dessous de 50 % :** les lignes électriques se dégradent progressivement.
- **À 50 % :** leur état reste stable.
- **Au-dessus de 50 % :** le réseau se répare progressivement.

Le Ministère considère normalement des niveaux de financement entre **60 % et 120 % pour chaque centrale gérée**. Cela aide à maintenir le financement global de l’électricité au-dessus du niveau auquel les lignes électriques se dégradent.

Il vérifie aussi l’effet des changements de financement sur le réseau de lignes électriques avant de les appliquer.

**Le Ministère ne se contente pas de réduire les budgets des centrales sans tenir compte de l’état du réseau électrique.**

<a id="power-aging"></a>
#### État et vieillissement des centrales

Les centrales ne durent pas éternellement. Au fil de leur fonctionnement, leur état se dégrade progressivement.

Dans SimCity 4, **la vitesse de vieillissement d’une centrale dépend principalement de son taux d’utilisation**, même si son financement joue aussi un rôle.

Le Ministère lit plusieurs informations sur chaque centrale, notamment :

- **Taux de vieillissement de base :** le paramètre de vieillissement sous-jacent de la centrale.
- **Seuil de vieillissement :** l’état à partir duquel le jeu considère la centrale comme vieille. Il valait 0,9 (90 %) pour les centrales examinées.
- **Coût de remplacement :** le montant nécessaire pour construire une autre centrale du même type.

Ces valeurs aident le Ministère à estimer le coût économique du fonctionnement d’une centrale lorsque le calcul optionnel du vieillissement est activé.

**Financement et utilisation ne sont pas la même chose**

Cette distinction est essentielle.

- Le **financement** détermine la quantité d’électricité qu’une centrale *peut produire*.
- L’**utilisation** mesure la part de cette capacité disponible que la ville utilise réellement.

Par exemple :

| Centrale | Financement | Utilisation réelle |
|---|---:|---:|
| Centrale A | 100 % | 30 % |
| Centrale B | 70 % | 95 % |

La centrale A est entièrement financée mais produit beaucoup moins d’électricité qu’elle ne le pourrait.

La centrale B reçoit moins de financement, mais la ville utilise presque toute l’électricité qu’elle peut produire à ce niveau de financement.

**Utilisation d’une centrale (jeu de base)**

```math
u = \frac{\text{Utilisé}}{\text{Sortie}}
```

Où :

- **Utilisé** est l’électricité effectivement prélevée sur la centrale.
- **Sortie** est la capacité de production disponible de la centrale.
- **u** est la proportion de cette capacité qui est utilisée.

Par exemple, si une centrale peut produire 10 000 unités et que la ville en prélève 8 000, son utilisation est de **80 %**.

Quand une centrale ne produit rien, le Ministère considère son utilisation comme nulle.

### À quelle vitesse les centrales vieillissent-elles ?

Le calcul du vieillissement du jeu dépend de trois facteurs :

1. Le taux de vieillissement de base de la centrale.
2. Son taux d’utilisation.
3. Son niveau de financement.

**Vieillissement estimé par mois simulé (SimCity 4, modélisé par le Ministère)**

```math
A = \frac{B \times U(\min(\max(u,0),2)) \times G\left(\frac{f}{100}\right)}{16}
```

Où :

- **B** est le paramètre de vieillissement de base de la centrale.
- **u** est son utilisation actuelle, exprimée en fraction.
- **U(u)** est le multiplicateur de vieillissement associé à l’utilisation.
- **f** est son pourcentage de financement.
- **G(f/100)** est le multiplicateur de vieillissement associé au financement.
- **A** est la perte d’état estimée par mois simulé.

En termes simples :

**Vieillissement de la centrale = Vieillissement de base × Effet de l’utilisation × Effet du financement × Facteur temps**

Le taux de base et les deux courbes de vieillissement proviennent de la mécanique du jeu.

Le **facteur temps de 1/16** a été établi à partir de mesures sur sept centrales pendant 13 mois simulés. C’est une valeur mesurée expérimentalement, et non une constante lue directement dans le jeu.

Les estimations de vieillissement obtenues doivent donc être traitées comme des approximations et non comme des prévisions exactes de la durée de vie restante d’une centrale.

#### 1. Comment le financement affecte le vieillissement

La courbe de vieillissement en fonction du financement du jeu comporte trois points de contrôle :

| Financement | Multiplicateur de vieillissement |
|---:|---:|
| 0 % | 1,10 |
| 100 % | 1,00 |
| 120 % | 0,90 |

Les valeurs entre ces points sont interpolées.

Dans la plage de financement normale du Ministère :

| Financement | Multiplicateur de vieillissement |
|---:|---:|
| 60 % | 1,04 |
| 80 % | 1,02 |
| 90 % | 1,01 |
| 100 % | 1,00 |
| 110 % | 0,95 |
| 120 % | 0,90 |

**Le financement a un effet direct relativement faible sur le vieillissement.**

Par exemple, réduire le financement d’une centrale de 100 % à 60 % fait passer son multiplicateur de vieillissement direct de 1,00 à 1,04, soit une augmentation de **4 %** seulement.

Cependant, réduire le financement réduit aussi la quantité d’électricité que la centrale peut produire. Si la ville continue de prélever la même quantité, l’utilisation de la centrale augmente, ce qui peut avoir un effet bien plus important sur le vieillissement.

#### 2. Comment l’utilisation affecte le vieillissement

La courbe de vieillissement en fonction de l’utilisation du jeu est beaucoup plus significative :

| Utilisation de la centrale | Multiplicateur de vieillissement |
|---:|---:|
| 0 % | 0,10 |
| 25 % | 0,20 |
| 50 % | 0,45 |
| 75 % | 0,85 |
| 90 % | 1,25 |
| 100 % | 1,55 |
| 110 % ou plus | 2,00 |

Comme pour les autres courbes, les valeurs intermédiaires sont calculées par interpolation linéaire.

**Plus une centrale travaille dur, plus elle vieillit vite.**

Par exemple :

- À **50 % d’utilisation**, le multiplicateur de vieillissement est de 0,45.
- À **90 % d’utilisation**, il atteint 1,25.

Une centrale qui fonctionne à 90 % d’utilisation vieillit donc environ **2,8 fois plus vite** que la même centrale à 50 % d’utilisation, toutes choses égales par ailleurs.

C’est pourquoi réduire simplement le financement des centrales n’est pas toujours idéal sur le plan économique.

**Exemple : réduire le financement peut presque doubler le vieillissement**

Imaginez une centrale initialement financée à 100 %, dont la ville utilise environ 60 % de la production disponible.

Le Ministère réduit le financement pour économiser sur les coûts de fonctionnement. La centrale peut maintenant produire moins d’électricité, mais la ville continue d’en prélever à peu près la même quantité.

Son utilisation peut grimper à environ 83 %.

Le multiplicateur de vieillissement lié à l’utilisation passe d’environ 0,60 à 1,06, tandis que le multiplicateur direct lié au financement ne change que légèrement.

**Le taux de vieillissement estimé de la centrale devient presque deux fois plus élevé.**

Cela ne veut pas forcément dire que la réduction du financement est une mauvaise décision. L’économie mensuelle peut valoir plus que l’usure supplémentaire.

Le Ministère peut comparer ces coûts lorsque **Tenir compte de l’usure des centrales** est activé.

**Que fait « Tenir compte de l’usure des centrales » ?**

Cette option est **désactivée par défaut**.

- **Désactivée :** le Ministère minimise les dépenses de fonctionnement mensuelles d’électricité tout en maintenant l’approvisionnement et la réserve de sécurité nécessaires.
- **Activée :** le Ministère inclut aussi le coût économique estimé de l’usure des centrales lorsqu’il compare les combinaisons de financement.

L’option peut favoriser une répartition du financement différente lorsque la réduction de l’usure compense la dépense de fonctionnement supplémentaire.

**Elle n’impose pas de pourcentage de financement minimal distinct, n’empêche pas le vieillissement et ne remplace pas automatiquement les vieilles centrales.**

<a id="power-usage-myth"></a>
#### Une utilisation de 80 % est-elle une limite de sécurité ?

**Non. SimCity 4 n’a pas de seuil brutal de vieillissement ou de panne à 80 % d’utilisation.**

La confusion vient de la courbe de vieillissement du jeu.

À environ **80,6 % d’utilisation**, le multiplicateur de vieillissement atteint 1,0. En dessous de ce point, le multiplicateur est plus bas ; au-dessus, il augmente.

Mais la courbe est continue.

Une centrale qui fonctionne à 81 % d’utilisation ne devient pas soudainement dangereuse. De même, une centrale à 95 % d’utilisation ne provoque pas automatiquement une panne de courant.

**Les pannes surviennent quand l’offre d’électricité est insuffisante ou ne peut pas atteindre les bâtiments, et non parce qu’une centrale dépasse 80 % d’utilisation.**

Le Ministère n’impose donc pas de plafond d’utilisation de 80 %.

Il utilise plutôt le réglage **Réserve d’électricité** pour conserver une capacité de production de réserve.

#### Comment la réserve d’électricité affecte-t-elle l’utilisation ?

La réserve d’électricité par défaut est de **20 %**.

Pour une ville simple alimentée par une seule centrale locale, sans échange régional d’électricité, la relation est approximativement :

```math
u \approx \frac{1}{1+r}
```

Où **r** est la réserve configurée exprimée en fraction (0,20 pour une réserve de 20 %).

Cela suppose que la centrale fournit exactement la capacité requise et ignore les petits écarts dus aux arrondis et aux exigences supplémentaires du jeu.

| Réserve d’électricité | Utilisation approximative de la centrale | Multiplicateur de vieillissement lié à l’utilisation |
|---|---:|---:|
| 10 % | 90,9 % | 1,28 |
| **20 % (par défaut)** | **83,3 %** | **1,07** |
| 25 % | 80,0 % | 0,98 |
| 30 % | 76,9 % | 0,90 |
| 50 % | 66,7 % | 0,72 |

Une réserve plus élevée laisse en général plus de capacité de production disponible et peut réduire l’intensité de fonctionnement d’une centrale.

Cependant, **une réserve plus importante peut aussi nécessiter plus de financement et donc augmenter les dépenses mensuelles**.

Avec plusieurs centrales, la relation devient plus compliquée, car SimCity 4 décide de la façon dont la demande d’électricité est répartie entre elles.

Le Ministère ne contrôle pas directement cette répartition.

Son but est de maintenir une capacité totale suffisante, avec la réserve que vous avez choisie, tout en minimisant les coûts inclus dans son optimisation.

#### Échanges régionaux d’électricité

Le Ministère tient aussi compte de l’électricité achetée aux villes voisines ou vendue à celles-ci.

- **Acheter de l’électricité** réduit la capacité de production locale dont la ville a besoin.
- **Vendre de l’électricité** augmente la capacité que les centrales de la ville doivent fournir.

Par exemple, si votre ville a besoin de 10 000 unités et importe de façon fiable 3 000 unités, ses centrales n’ont pas besoin de produire localement la totalité des 10 000.

En revanche, vendre 2 000 unités à un voisin signifie que ces unités doivent être disponibles en plus des besoins propres de votre ville.

Le Ministère inclut ces contrats dans le calcul de la capacité requise et le choix du financement des centrales.

La façon dont il crée, modifie, tarifie et annule les contrats est expliquée dans [Accords avec les voisins](deals.md#neighbor-deals).

<a id="power-problem"></a>
### 2. Le problème de gestion

Gérer l’électricité à la main est plus compliqué que d’ajuster un simple curseur de budget.

Le Ministère doit équilibrer plusieurs facteurs :

- **Éviter les dépenses inutiles :** une centrale financée à 100 % peut produire bien plus d’électricité que la ville n’en a besoin.
- **Maintenir un approvisionnement suffisant :** réduire trop le financement peut laisser des bâtiments sans électricité.
- **Protéger le réseau électrique :** un financement global de l’électricité très bas peut dégrader les lignes électriques.
- **Tenir compte du rendement des centrales :** le financement et la production ne sont pas proportionnels. Ramener toutes les centrales au même pourcentage n’est pas forcément la solution la moins chère.
- **Tenir compte de l’état et du vieillissement :** les centrales plus anciennes produisent moins d’électricité, et celles qui travaillent près de leur pleine capacité vieillissent en général plus vite.
- **Prendre en compte les échanges régionaux :** acheter de l’électricité peut réduire les besoins de production locale, tandis qu’en vendre les augmente.
- **Gérer plusieurs centrales ensemble :** la combinaison la moins chère peut consister à financer une centrale près de 100 % et une autre bien plus bas, plutôt que d’appliquer le même pourcentage aux deux.

SimCity 4 n’indique pas directement quelle combinaison de niveaux de financement des centrales fournira l’électricité nécessaire au moindre coût global.

**Le Ministère calcule cette combinaison automatiquement.**

Il prend en compte la capacité, l’état et les dépenses de fonctionnement de chaque centrale, ainsi que la réserve que vous avez choisie et les contrats régionaux.

Lorsque le réglage optionnel du vieillissement est activé, il inclut aussi le coût économique estimé de l’usure des centrales.

**Le résultat est un budget d’électricité conçu pour répondre aux besoins de la ville sans dépenser plus que ce que le modèle du Ministère juge nécessaire.**
<a id="power-algorithm"></a>
### 3. Comment le Ministère optimise le financement de l’électricité

Le Ministère ne se contente pas de réduire le financement de chaque centrale du même pourcentage.

Il calcule plutôt la quantité d’électricité dont la ville a besoin et compare différentes combinaisons de financement pour trouver **la solution la moins chère qui maintient une capacité de production suffisante**.

Le processus comporte trois grandes étapes :

1. Calculer la quantité d’électricité que les centrales de la ville doivent fournir.
2. Calculer la quantité que chaque centrale peut produire à différents niveaux de financement.
3. Trouver la combinaison la moins chère qui satisfait l’exigence.

Si **Tenir compte de l’usure des centrales** est activé, le Ministère inclut aussi le coût économique estimé de l’usure des centrales lorsqu’il compare ces combinaisons.

<a id="power-required-capacity"></a>
#### Étape 1 : de quelle quantité d’électricité la ville a-t-elle besoin ?

Le Ministère calcule d’abord la capacité de production qui doit être disponible localement.

Il prend en compte :

- La demande d’électricité actuelle de la ville.
- Les bâtiments qui pourraient encore avoir besoin d’électricité.
- Votre **Réserve d’électricité** configurée, 20 % par défaut.
- L’électricité achetée aux villes voisines.
- L’électricité vendue aux villes voisines.

**La réserve ne s’applique qu’à l’électricité que la ville doit produire elle-même.**

Par exemple, si votre ville a besoin de 5 000 unités et en importe 2 000 d’un voisin, ses centrales n’ont qu’à couvrir les 3 000 unités restantes, plus la réserve configurée.

L’électricité vendue aux voisins doit aussi être produite : les exportations augmentent donc le besoin local.

**Capacité de production locale requise**

```math
N = \max\left(\left\lceil R \times \max(0,D-B)\right\rceil,\ Q-B\right)+S
```

Où :

- **D** est la demande d’électricité de la ville.
- **Q** est le besoin total d’électricité du jeu, y compris les bâtiments actuellement sans courant.
- **B** est l’électricité achetée aux voisins sur laquelle le plan peut compter.
- **S** est l’électricité vendue aux voisins.
- **R** est le multiplicateur de réserve (1,20 pour la réserve par défaut de 20 %).
- **N** est la capacité de production exigée des centrales locales.

Le Ministère retient la plus grande de ces deux valeurs : la demande locale avec réserve ou le besoin d’électricité restant du jeu. Les exportations sont ensuite ajoutées.

**Exemple : une ville avec 5 000 unités de demande**

Supposons que le besoin total du jeu soit de 5 050 unités et que la réserve soit de 20 %.

| Situation | Capacité locale requise |
|---|---:|
| Aucune importation ni exportation d’électricité | 6 000 unités |
| Importation de 2 000 unités | 3 600 unités |

Sans importations :

```math
N = \max(5000 \times 1.20,\ 5050)=6000
```

Avec 2 000 unités importées :

```math
N = \max(3000 \times 1.20,\ 3050)=3600
```

**Le Ministère n’achète pas de réserve supplémentaire en plus de l’électricité importée.** Sa réserve protège la part de la demande que les centrales locales doivent fournir.

Si le Ministère gère les contrats d’électricité, il planifie ensemble les contrats et le financement des centrales.

Sinon, il utilise les contrats existants, en ne comptant les achats que lorsqu’ils sont actifs de façon fiable. Les achats en attente, suspendus ou défaillants ne sont pas considérés comme un approvisionnement garanti. Les ventes existantes sont comptées comme des obligations.

Voir [Accords avec les voisins](deals.md#neighbor-deals) pour plus d’informations.

#### Étape 2 : quelle quantité chaque centrale peut-elle produire ?

Une fois la capacité requise connue, le Ministère examine chaque centrale qu’il peut gérer en toute sécurité.

Pour chaque centrale, il considère des **pourcentages de financement entiers de 60 % à 120 %**.

Il considère aussi le financement actuel de la centrale, même si vous l’avez réglé manuellement en dehors de cette plage.

Pour chaque niveau de financement possible, le Ministère calcule :

- La capacité de production disponible de la centrale.
- Sa dépense de fonctionnement mensuelle.
- Son coût d’usure estimé, lorsque l’option de vieillissement est activée.

**Capacité de production prévue**

```math
\text{Cap}(f) = \left\lfloor \text{SortieMax} \times \frac{\text{Courbe}(f)}{100} \times \text{État} \right\rfloor
```

Où :

- **SortieMax** est la production maximale de la centrale.
- **Courbe(f)** est le rendement du jeu au niveau de financement **f**.
- **État** est l’état actuel de la centrale, de 0 à 1.
- **Cap(f)** est la capacité de production obtenue, arrondie à l’inférieur.

Le Ministère utilise l’**état réel** de la centrale, et non la capacité qu’elle avait à l’état neuf.

**Exemple : une centrale en bon état**

Supposons qu’une centrale ait :

- Production maximale : 6 000 unités.
- État actuel : 98 %.
- Financement proposé : 84 %.

À 84 % de financement, la courbe de rendement du jeu donne 71,75 %.

```math
\text{Cap}(84) = \left\lfloor 6000 \times 0.7175 \times 0.98 \right\rfloor = 4218
```

Le Ministère compte donc **4 218 unités** de capacité de production pour cette centrale.

Il répète ce calcul pour les autres niveaux de financement possibles et pour chaque centrale éligible.

**Et les centrales que le Ministère ne peut pas gérer ?**

Certaines centrales ne peuvent pas être ajustées en toute sécurité. Par exemple :

- Les centrales dont les informations de financement ne sont pas prises en charge ou sont illisibles.
- Les centrales dont les coûts de fonctionnement ou les caractéristiques de production ne peuvent pas être déterminés de façon fiable.
- Les centrales de valorisation énergétique des déchets qui brûlent actuellement des déchets, car modifier leur financement pourrait perturber le système de gestion des déchets de la ville.

Ces centrales ne sont **pas ignorées**.

Le Ministère inclut leur capacité de production actuelle dans le calcul mais laisse leur financement inchangé. Leurs coûts sont pris en compte sans compter deux fois la même dépense.

Si **Gérer l’électricité** est désactivé, toutes les centrales conservent leur financement actuel. Les contrats d’électricité peuvent quand même être gérés séparément si leurs options restent activées.

<a id="power-optimizer"></a>
#### Étape 3 : trouver la combinaison la moins chère

C’est ici que le Ministère fait la différence.

Imaginez une ville avec plusieurs centrales. Chaque centrale a son propre coût de fonctionnement, sa propre courbe de rendement et son propre état.

Il peut exister de nombreuses combinaisons de financement qui fournissent assez d’électricité.

**Le Ministère compare ces combinaisons et choisit celle dont le coût total modélisé est le plus bas.**

Il ne réduit pas simplement d’abord la centrale la plus chère et ne finance pas toutes les centrales au même pourcentage.

**Formule d’optimisation de l’électricité**

```math
\min_{f_1,\ldots,f_n}\sum_{i=1}^{n}\left(E_i(f_i)+W_i(f_i)\right)
\quad\text{sous la contrainte}\quad
K+\sum_{i=1}^{n}\text{Cap}_i(f_i)\ge N
```

Où :

- **Eᵢ** est la dépense de fonctionnement mensuelle de la centrale *i*.
- **Wᵢ** est son coût d’usure estimé, ou zéro lorsque l’option de vieillissement est désactivée.
- **Capᵢ** est sa capacité disponible au financement sélectionné.
- **K** est la capacité cumulée des centrales dont le financement ne peut pas être modifié.
- **N** est la capacité locale requise calculée à l’étape 1.

En clair :

**Trouver la combinaison de niveaux de financement des centrales la moins chère qui produit assez d’électricité pour la ville et sa réserve.**

Par exemple, la solution la plus économique pourrait être :

| Centrale | Financement choisi |
|---|---:|
| Centrale à charbon | 91 % |
| Centrale à gaz naturel | 61 % |

Une autre ville avec une demande, des centrales ou des coûts de fonctionnement différents pourrait bénéficier d’une combinaison complètement différente.

Voir l’[Exemple 3](#power-example-mixed) pour un calcul détaillé portant sur deux centrales.

**Comment le Ministère trouve-t-il la meilleure combinaison ?**

Il utilise un algorithme d’optimisation qui compare ensemble les options de financement des centrales, au lieu de prendre des décisions indépendantes pour chacune.

Pour garder des calculs rapides dans les grandes villes, il organise la capacité de production en petites unités.

**Unité de capacité d’optimisation**

```math
\text{unité} = \max\left(1,\left\lceil\frac{N}{20000}\right\rceil\right)
```

Pour les villes qui exigent jusqu’à **20 000 unités**, la capacité de production est comparée une unité à la fois.

Pour les villes plus grandes, le Ministère utilise des unités un peu plus grandes afin de réduire la quantité de calcul.

Cela peut donner un plan avec un peu plus de capacité de réserve que le minimum mathématique, mais l’arrondi est prudent : **le plan retenu doit quand même atteindre la capacité de production requise**.

Lorsqu’il compare de nombreux contrats régionaux possibles, le planificateur de contrats utilise un calcul plus grossier pour gagner en rapidité. Le plan final de financement des centrales utilise le calcul plus détaillé.

**Et si plusieurs combinaisons coûtent le même prix ?**

Le Ministère préfère la combinaison qui modifie le moins de centrales.

Par exemple, si conserver le financement actuel coûte exactement le même prix que de modifier trois centrales, il les laisse telles quelles.

Cela évite des ajustements de budget inutiles.

**Et s’il n’y a pas assez d’électricité ?**

Il arrive que même financer chaque centrale disponible à son réglage de capacité maximale ne suffise pas.

Dans ce cas, le Ministère :

1. Choisit la capacité de production maximale disponible parmi les centrales gérées, normalement en les montant à 120 %.
2. Tente de couvrir le déficit restant par des achats régionaux, si la gestion des contrats d’électricité est activée.
3. Émet un avis de conseiller si la pénurie est confirmée et persiste.

**Le Ministère ne construit ni ne remplace jamais de centrales.** Si le financement et les achats disponibles ne peuvent pas résoudre le problème, c’est à vous d’apporter de la capacité de production supplémentaire.

<a id="power-wear"></a>
#### Option : tenir compte du vieillissement des centrales

Par défaut, le Ministère se concentre sur la **minimisation de la facture mensuelle d’électricité**.

Cependant, le budget de fonctionnement le moins cher n’est pas toujours le choix le moins cher sur toute la durée de vie d’une centrale.

Comme expliqué plus haut, réduire le financement peut augmenter l’intensité d’utilisation d’une centrale et donc accélérer potentiellement sa détérioration.

Le réglage optionnel **Tenir compte de l’usure des centrales** permet au Ministère d’inclure un coût estimé pour cette usure.

| Réglage | Comment le financement est optimisé |
|---|---|
| **Désactivé (par défaut)** | Minimise les dépenses de fonctionnement mensuelles réelles tout en maintenant la capacité et la réserve requises. |
| **Activé** | Minimise les dépenses de fonctionnement plus le coût économique estimé de l’usure des centrales. |

**Important :** le coût d’usure estimé n’est pas une charge supplémentaire pour le trésor de votre ville. Il sert uniquement à comparer les choix de financement.

Les centrales vieillissent normalement que cette option soit activée ou non.

#### Comment le coût du vieillissement est-il calculé ?

Chaque centrale a un coût de construction et un seuil de vieillissement.

Le Ministère estime quelle part de l’état utile restant de la centrale est consommée pendant un mois simulé, puis attribue une valeur monétaire à cette usure.

**Coût d’usure mensuel estimé**

```math
W(f) = \mathrm{round}\left(P \times \frac{\min\left(A(f),\ \text{Cond}-T\right)}{1-T}\right)
```

Cette formule s’applique tant que l’état de la centrale est au-dessus de son seuil de vieillissement. Sinon, le coût d’usure modélisé est nul.

Où :

- **P** est le coût de construction ou de remplacement de la centrale.
- **A(f)** est son vieillissement mensuel estimé au niveau de financement considéré.
- **Cond** est son état actuel.
- **T** est le seuil d’état à partir duquel le jeu la considère comme vieille.
- **W(f)** est le coût d’usure économique estimé par mois simulé.

Par exemple, si une centrale coûte 10 000 § à construire et que la plage d’état utile va de 100 % à 90 %, le Ministère attribue à cette plage d’état de 10 % une valeur économique de 10 000 §.

Une petite part de cette plage consommée en un mois représente une fraction correspondante du coût de remplacement.

Le résultat est arrondi aux simoléons entiers.

**Coûts de construction des centrales du jeu d’origine**

| Centrale | Coût de remplacement |
|---|---:|
| Éolienne | 500 § |
| Gaz naturel | 9 000 § |
| Charbon | 10 000 § |
| Pétrole | 17 000 § |
| Valorisation énergétique des déchets | 25 000 § |
| Solaire | 30 000 § |
| Nucléaire | 40 000 § |
| Hydrogène | 100 000 § |

Pour les centrales personnalisées compatibles, le Ministère lit le coût de construction fourni par le bâtiment.

**Et si les informations nécessaires manquent ?**

Si le coût de remplacement, le taux de vieillissement ou le seuil de vieillissement de la centrale ne peut pas être déterminé de façon fiable, le Ministère laisse simplement le coût d’usure en dehors du calcul **pour cette seule centrale**.

La centrale peut quand même être gérée normalement à partir de sa dépense de fonctionnement.

Les autres centrales dont les informations de vieillissement sont valides continuent d’inclure leurs coûts d’usure estimés.

#### Quelle différence cette option apporte-t-elle ?

L’effet est souvent plus faible que les joueurs ne pourraient s’y attendre.

Dans la plage de financement du Ministère, de 60 % à 120 %, le multiplicateur direct de vieillissement lié au financement ne varie que de **1,04 à 0,90**.

Le Ministère calcule le coût d’usure de chaque candidat à partir de l’**utilisation actuellement observée** de la centrale. Il ne prédit pas exactement comment SimCity 4 redistribuera la charge d’électricité après chaque changement de financement possible.

Par conséquent, le coût d’usure estimé ne change en général que légèrement d’un niveau de financement à l’autre.

Pour beaucoup de centrales du jeu d’origine, cela ne représente que quelques simoléons par mois.

**Exemple**

Supposons que le Ministère compare deux choix de financement :

| Option | Dépense de fonctionnement | Usure estimée | Coût total modélisé |
|---|---:|---:|---:|
| A | 200 § | 8 § | 208 § |
| B | 205 § | 2 § | 207 § |

<italic>Chiffres illustratifs, et non des mesures d’une centrale précise.</italic>

Avec le vieillissement désactivé, l’**option A** l’emporte car sa dépense de fonctionnement mensuelle est plus basse.

Avec le vieillissement activé, l’**option B** l’emporte car son coût combiné de fonctionnement et d’usure est légèrement plus bas.

La dépense mensuelle réelle d’électricité de la ville serait de 205 §, et non de 207 §. Les 2 § supplémentaires n’existent que dans la comparaison économique.

En pratique, les deux modes d’optimisation choisissent souvent la même combinaison de financement, surtout lorsque l’écart d’usure est faible.

Voir l’[Exemple 5](#power-example-wear) pour un calcul utilisant les données de centrales du jeu.

**Ce que cette option ne fait pas**

Activer **Tenir compte de l’usure des centrales** ne permet pas de :

- Empêcher les centrales de se détériorer.
- Obliger les centrales à fonctionner sous 80 % d’utilisation.
- Introduire un niveau de financement minimal supplémentaire.
- Remplacer automatiquement les centrales vieillissantes.
- Assouplir la réserve d’électricité de la ville ou les autres exigences de sécurité.

Elle permet simplement au Ministère de prendre en compte **le coût économique estimé à long terme de l’usure en plus de la facture de fonctionnement mensuelle**.

L’objectif reste le même : **fournir assez d’électricité à votre ville au moindre coût modélisé, selon les options et les limites de sécurité que vous avez choisies.**
<a id="power-cadence"></a>
#### Étape 4 : quand le Ministère ajuste-t-il le financement de l’électricité ?

Le Ministère surveille régulièrement l’approvisionnement en électricité de votre ville et ajuste le financement des centrales quand c’est nécessaire.

**Il examine normalement le financement tous les 10 jours simulés, mais il peut réagir plus tôt si la demande ou l’offre d’électricité change de façon significative.**

Tous les délais se réfèrent au temps de jeu simulé.

| Quand | Ce que fait le Ministère |
|---|---|
| **À l’ouverture d’une ville** | Effectue sa première évaluation dès que les informations d’électricité du jeu sont disponibles et cohérentes. Il n’y a pas de délai d’attente fixe. |
| **Chaque jour simulé** | Vérifie rapidement la demande, l’offre, la consommation, la capacité de production et les contrats régionaux d’électricité pour y déceler des changements significatifs. |
| **Tous les 10 jours simulés** | Effectue une évaluation régulière des centrales et de leur financement. |
| **Quand un changement significatif est détecté** | Recalcule le plan d’électricité sans attendre le prochain examen régulier. |

**Qu’est-ce qui déclenche une évaluation anticipée ?**

Lors de ses vérifications quotidiennes, le Ministère recherche :

- **Une véritable pénurie d’électricité :** la ville n’a pas assez d’électricité provenant de ses centrales et des importations utilisables pour satisfaire la demande.
- **Une variation d’au moins 1 % de la demande d’électricité**, par rapport à la dernière évaluation.
- **Une variation d’au moins 1 % de la capacité de production locale.**
- **Une variation d’au moins 1 % des importations ou exportations d’électricité sous contrat.**

Une lecture inchangée ne déclenche pas de nouvelle évaluation.

Le Ministère distingue aussi une véritable pénurie d’électricité d’un problème de raccordement électrique de la ville.

Par exemple, si l’électricité disponible est suffisante mais que certains bâtiments sont déconnectés du réseau, le Ministère n’augmente pas inutilement le financement des centrales. Le problème de raccordement est plutôt consigné dans le journal.

**Comment les changements de financement sont-ils appliqués ?**

Lorsque le Ministère choisit un nouveau niveau de financement, il l’applique directement.

Par exemple, une centrale peut passer de 60 % à 95 % de financement en un seul ajustement. Il n’y a ni augmentation progressive ni délai d’attente artificiel.

Cependant, **les augmentations passent toujours avant les réductions**.

Le Ministère augmente le financement là où c’est nécessaire et s’assure que les autres parties du plan, comme les nouveaux achats d’électricité, sont en place avant d’effectuer des réductions lors d’une évaluation ultérieure.

Cela évite des pénuries temporaires de capacité pendant l’ajustement de plusieurs centrales ou contrats régionaux.

Chaque évaluation utilise les informations actuelles de la ville. Si vous modifiez manuellement le financement d’une centrale, ou si vous sauvegardez et rouvrez la ville, le Ministère tient compte de la nouvelle situation.

<a id="power-safety"></a>
### 4. Conditions de sécurité

Le Ministère suit plusieurs règles de sécurité pour empêcher que ses ajustements de financement ne perturbent l’approvisionnement en électricité.

#### Avant de calculer un plan

Le Ministère ne prend de décisions de financement que lorsque les informations du jeu sont fiables.

- La demande d’électricité et les autres lectures nécessaires doivent être disponibles et cohérentes.
- Une demande lue à zéro est traitée comme potentiellement non initialisée, et non automatiquement comme la preuve que la ville n’a pas besoin d’électricité.
- Au moins une centrale doit avoir des informations de financement et de production lisibles avant que le financement des centrales puisse être géré.
- Des lectures contradictoires, par exemple une consommation d’électricité déclarée supérieure à la demande de plus de 1 % plus une unité, empêchent les changements de financement automatiques.

Une ville **sans centrale** peut quand même être évaluée pour des achats d’électricité auprès des villes voisines, si la gestion des contrats régionaux est activée.

#### Protéger l’approvisionnement en électricité

**Le Ministère ne réduit jamais délibérément le financement en dessous de la capacité exigée par ses règles de sécurité.**

Avant chaque réduction de financement, il :

1. Relit les informations d’électricité de la ville.
2. Vérifie la capacité de production mise à jour de chaque centrale, d’après son état actuel et son financement prévu.
3. Ne compte que les achats d’électricité déjà actifs et utilisables.
4. Vérifie que la capacité restante couvrira la demande locale, la réserve configurée et les exportations.

Si la réduction laissait une capacité insuffisante, **elle est annulée**.

Un achat d’électricité en attente, suspendu ou défaillant n’est jamais traité comme un approvisionnement garanti.

Les augmentations de financement ne sont pas rejetées simplement parce que la ville reste en manque d’électricité. Même si la production maximale ne peut pas résoudre toute la pénurie, augmenter la capacité disponible peut quand même aider.

#### Protéger les centrales et les lignes électriques

- Les nouvelles cibles de financement vont normalement de **60 % à 120 %**, en pourcentages entiers.
- Le Ministère respecte les limites de financement acceptées par le système de budget du jeu.
- Il rejette les changements de financement qui entraîneraient une dégradation du réseau de lignes électriques.
- Lorsque la ville connaît un véritable déficit d’électricité, le planificateur de contrats régionaux ne maintient pas les ventes d’électricité.

Le Ministère ne construit, ne démolit, ne remplace ni n’éteint jamais une centrale. Ses actions se limitent à la modification des réglages de financement éligibles.

#### Vérification de chaque changement de financement

Avant de modifier le financement d’une centrale, le Ministère vérifie que sa valeur actuelle correspond encore à celle utilisée pour calculer le plan.

Si vous avez modifié cette valeur manuellement, le Ministère saute l’ajustement et recalcule lors d’une évaluation ultérieure.

Il identifie aussi les centrales par leur position sur la carte de la ville. Si deux centrales ne peuvent pas être distinguées de façon fiable à la même position, il ne devine pas laquelle modifier.

**Après chaque changement, le Ministère vérifie que le jeu l’a accepté.**

La valeur de financement de la centrale et sa ligne de budget doivent toutes deux refléter le nouveau réglage.

Si la vérification échoue :

- Le Ministère tente d’annuler les changements effectués lors de cet examen.
- La gestion automatique du financement des centrales s’arrête pour le reste de la session de la ville en cours.
- Rouvrir la ville permet de relancer la gestion.
- Les autres services et les contrats régionaux d’électricité continuent de fonctionner indépendamment.

**La priorité est de préserver un approvisionnement fiable en électricité, quitte à renoncer à une économie potentielle.**

Voir [Sécurité générale](safety.md#safety) pour les garde-fous communs à tous les services.

<a id="power-settings"></a>
### 5. Réglages

La gestion de l’électricité se configure dans **Options de jeu → Ministère de l’Optimisation**, sur la page **Services**.

| Réglage | Par défaut | Plage | Ce que cela fait |
|---|---|---|---|
| [Ministère actif](options.md#opt-master) | Activé | Activé / Désactivé | Active ou désactive l’ensemble du Ministère. |
| [Gérer l’électricité](options.md#opt-power) | Activé | Activé / Désactivé | Optimise automatiquement le financement de chaque centrale. |
| [Réserve d’électricité](options.md#opt-powerreserve) | 20 % | 10–50 % | Définit la capacité de production locale de réserve minimale que le Ministère vise à maintenir. |
| [Tenir compte de l’usure des centrales](options.md#opt-poweraging) | Désactivé | Activé / Désactivé | Inclut l’usure estimée des centrales lors de la comparaison des coûts de financement. |
| [Gérer les accords avec les voisins](options.md#opt-deals) | Activé | Activé / Désactivé | Active la gestion automatique des contrats régionaux. |
| [Gérer les accords d’électricité](options.md#opt-tradepower) | Activé | Activé / Désactivé | Active la gestion automatique des achats et des ventes d’électricité. |

L’option [Acheter à une ville pour vendre à une autre](options.md#opt-arbitrage) détermine si le Ministère peut acheter de l’électricité à une ville voisine pour la revendre à une autre. Voir [Accords avec les voisins](deals.md#neighbor-deals).

#### Comment fonctionne la réserve d’électricité ?

Avec une réserve de 20 %, le Ministère vise normalement à maintenir 20 % de capacité de production locale de plus que l’électricité que la ville doit produire elle-même.

Par exemple, si la ville doit produire 5 000 unités localement, le Ministère vise au moins **6 000 unités de capacité**.

C’est un **minimum**, pas un maximum. Le Ministère peut maintenir plus de capacité lorsque les réglages disponibles des centrales ou des ventes d’électricité rentables le justifient.

Augmenter la réserve peut offrir une protection supplémentaire contre la croissance et les variations de la demande, mais peut aussi augmenter les coûts de fonctionnement.

Si vous désactivez **Gérer l’électricité**, le Ministère laisse le financement des centrales inchangé. Il peut quand même gérer les achats et les ventes d’électricité si les options correspondantes restent activées.

#### Réglages avancés (fichier INI uniquement)

| Réglage | Par défaut | Plage autorisée | Ce que cela fait |
|---|---|---|---|
| `PowerReservePercent` | 20 | 10–50 | Définit le pourcentage de réserve d’électricité. |
| `PowerCadenceDays` | 10 | 1–30 | Définit la fréquence des examens réguliers du financement de l’électricité, en jours simulés. |

Par exemple, avec `PowerCadenceDays=5`, le Ministère effectue un examen régulier tous les cinq jours simulés au lieu de dix.

**Les vérifications quotidiennes des changements urgents d’approvisionnement restent indépendantes de cet intervalle.**

Une valeur invalide de `PowerReservePercent` rend invalide le fichier de configuration des services. Dans ce cas, le Ministère ne gère aucun service tant que la configuration n’est pas corrigée.

Pour la plupart des joueurs, les réglages par défaut constituent un bon point de départ.

<a id="power-examples"></a>
### 6. Exemples détaillés

Les exemples suivants montrent comment le Ministère choisit le financement pour différentes centrales et différents besoins en électricité.

Toutes les capacités, dépenses de fonctionnement et états des centrales sont illustratifs. Les calculs utilisent les courbes de rendement du jeu d’origine.

Sauf indication contraire, la réserve d’électricité est de **20 %** et il n’y a aucun contrat régional d’électricité.

Les chiffres de vieillissement de la centrale à charbon utilisent des valeurs mesurées pendant les tests : un taux de vieillissement de base de 0,001111 par mois et un seuil de vieillissement de 0,9. Les mêmes paramètres de vieillissement sont utilisés pour la centrale à gaz naturel lorsque c’est nécessaire pour l’illustration.

<a id="power-example-coal"></a>
#### Exemple 1 : une centrale à charbon avec de la capacité inutilisée

Prenons une centrale à charbon avec :

- **Production maximale :** 6 000 unités.
- **Coût de fonctionnement mensuel à 100 % :** 253 §.
- **État actuel :** 98 %.
- **Demande de la ville :** 3 500 unités.
- **Besoin du jeu :** 3 550 unités.
- **Réserve d’électricité :** 20 %.

**Étape 1 : calculer la capacité requise**

```math
N = \max(3500 \times 1.20,\ 3550)=4200
```

La ville a besoin de **4 200 unités** de capacité de production locale.

**Étape 2 : comparer les niveaux de financement**

| Financement | Capacité disponible | Coût mensuel | Résultat |
|---|---:|---:|---|
| 83 % | 4 138 | 210 § | Insuffisant |
| **84 %** | **4 218** | **213 §** | **Suffisant** |
| 85 % | 4 299 | 215 § | Suffisant, mais plus cher |
| 100 % | 5 880 | 253 § | Bien plus de capacité que nécessaire |

**Le Ministère choisit 84 % de financement.**

La centrale peut produire 4 218 unités, un peu plus que les 4 200 requises.

Son coût de fonctionnement mensuel passe de 253 § à 213 §.

<box gap={1}>
**Économie mensuelle**
<title size="xl" color="success">40 §</title>
</box>

**Que se passe-t-il pour le vieillissement de la centrale ?**

À 100 % de financement, la centrale peut produire 5 880 unités, alors que la ville en prélève 3 500. Son utilisation est d’environ **59,5 %**.

À 84 % de financement, la production disponible tombe à 4 218 unités. Si la ville continue de prélever 3 500 unités, l’utilisation monte à environ **83 %**.

La perte d’état mensuelle modélisée change :

| Financement | Utilisation de la centrale | Perte d’état mensuelle estimée |
|---|---:|---:|
| 100 % | 59,5 % | 0,0000418 |
| 84 % | 83,0 % | 0,0000750 |

**Le taux de vieillissement estimé devient presque 1,8 fois plus élevé.**

L’essentiel de cet écart provient de l’utilisation plus élevée, et non directement du financement plus bas.

Cela illustre pourquoi réduire les coûts de fonctionnement peut accélérer le vieillissement des centrales, et pourquoi le calcul optionnel du vieillissement peut être utile.

<a id="power-example-gas"></a>
#### Exemple 2 : une centrale à gaz naturel

Prenons une centrale à gaz naturel avec :

- **Production maximale :** 3 000 unités.
- **Coût mensuel à 100 % :** 230 §.
- **État :** 100 %.
- **Demande de la ville :** 2 000 unités.
- **Besoin du jeu :** 2 020 unités.

Avec une réserve de 20 %, la ville a besoin de :

```math
N = \max(2000 \times 1.20,\ 2020)=2400
```

Le Ministère compare les niveaux de financement disponibles :

| Financement | Capacité disponible | Résultat |
|---|---:|---|
| 89 % | 2 358 | Insuffisant |
| **90 %** | **2 400** | **Suffisant** |
| 100 % | 3 000 | Plus que nécessaire |

**Le Ministère choisit 90 % de financement.**

Le coût de fonctionnement mensuel devient :

```math
E = 230 \times 0.90 = 207
```

La centrale continue de fournir l’électricité et la réserve requises, tout en économisant **23 § par mois**.

<a id="power-example-mixed"></a>
#### Exemple 3 : deux centrales qui travaillent ensemble

Cet exemple montre pourquoi le Ministère ne finance pas toutes les centrales au même pourcentage.

Imaginez une ville avec les deux centrales des exemples précédents :

| Centrale | Production maximale | État | Coût à 100 % |
|---|---:|---:|---:|
| Charbon | 6 000 | 98 % | 253 § |
| Gaz naturel | 3 000 | 100 % | 230 § |
| **Total** | **9 000** | | **483 §** |

La ville a :

- **Demande d’électricité :** 5 000 unités.
- **Besoin du jeu :** 5 050 unités.
- **Réserve d’électricité :** 20 %.

Le Ministère calcule une capacité locale requise de :

```math
N = \max(5000 \times 1.20,\ 5050)=6000
```

**La ville a besoin de 6 000 unités de capacité de production.**

Le Ministère compare différentes stratégies de financement :

| Stratégie de financement | Capacité combinée | Coût mensuel |
|---|---:|---:|
| Les deux centrales à 100 % | 8 880 | 483 § |
| Les deux centrales à 81 % | 6 004 | 391 § |
| **Charbon à 91 %, gaz à 61 %** | **6 024** | **370 §** |

**Le Ministère choisit 91 % pour la centrale à charbon et 61 % pour la centrale à gaz naturel.**

La capacité obtenue est :

```math
\text{Charbon} = \lfloor6000\times0.82\times0.98\rfloor=4821
```

```math
\text{Gaz} = \lfloor3000\times0.40125\rfloor=1203
```

Ensemble, elles fournissent **6 024 unités**, ce qui suffit pour satisfaire l’exigence.

Les dépenses mensuelles sont :

```math
\text{Charbon} = \mathrm{round}(253\times0.91)=230
```

```math
\text{Gaz} = \mathrm{round}(230\times0.61)=140
```

<grid columns={2} gap={3}>
  <grid-item>
    <box gap={1}>
      <text color="secondary" size="xs">Par rapport à un financement à 100 %</text>
      <title size="xl" color="success">113 §/mois économisés</title>
    </box>
  </grid-item>
  <grid-item>
    <box gap={1}>
      <text color="secondary" size="xs">Par rapport à des réductions égales</text>
      <title size="xl" color="success">21 §/mois économisés</title>
    </box>
  </grid-item>
</grid>

C’est possible parce que **le rendement d’une centrale n’est pas proportionnel au financement**.

Dans ce cas, garder la centrale à charbon près du plein rendement et la centrale à gaz près du financement minimal coûte moins cher que de réduire les deux de la même façon.

**Et si la ville vend de l’électricité ?**

Supposons qu’elle signe un contrat pour vendre **1 000 unités** à une ville voisine.

La capacité locale requise passe de 6 000 à **7 000 unités**.

La combinaison la moins chère du Ministère devient :

| Centrale | Financement | Capacité disponible |
|---|---:|---:|
| Charbon | 100 % | 5 880 |
| Gaz naturel | 60 % | 1 162 |
| **Total** | | **7 042** |

Le coût de fonctionnement mensuel est de **391 §**.

Le Ministère augmente donc la production pour couvrir le contrat d’exportation sans sacrifier l’approvisionnement de la ville elle-même.

**Et si la demande devient trop élevée ?**

À 120 % de financement, ces deux centrales peuvent fournir au maximum **9 768 unités** combinées.

Si la capacité locale requise dépasse ce montant, le Ministère monte les centrales à leurs réglages de capacité maximale, pour un coût mensuel combiné de **580 §**.

Si la gestion des contrats régionaux est activée, il tente aussi d’acheter assez d’électricité pour couvrir le déficit restant.

Sinon, le joueur doit fournir de la capacité de production supplémentaire.

<a id="power-example-trade"></a>
#### Exemple 4 : acheter de l’électricité à une ville voisine

Prenons la même ville à deux centrales que dans l’exemple 3.

Sa demande d’électricité est de 5 000 unités, mais elle a maintenant un contrat actif pour acheter **2 000 unités** à une ville voisine.

Les centrales locales n’ont plus qu’à fournir les 3 000 unités restantes, plus la réserve de 20 %.

```math
N = \max(3000\times1.20,\ 5050-2000)=3600
```

**Capacité locale requise : 3 600 unités.**

Le Ministère choisit :

| Centrale | Financement | Capacité | Coût mensuel |
|---|---:|---:|---:|
| Charbon | 62 % | 2 440 | 157 § |
| Gaz naturel | 60 % | 1 162 | 138 § |
| **Total** | | **3 602** | **295 §** |

Sans l’achat, les centrales coûtent 370 § par mois pour fournir l’électricité et la réserve de la ville.

Avec l’achat, leur coût de fonctionnement tombe à **295 § par mois**.

C’est une réduction de **75 § par mois sur les dépenses des centrales locales**.

Cependant, la ville doit aussi payer l’électricité importée.

**Que l’achat permette réellement d’économiser dépend du prix du contrat.**

Le planificateur de contrats régionaux du Ministère compare le coût de l’achat aux économies de production locale avant de décider si le contrat vaut la peine.

Voir [Accords avec les voisins](deals.md#neighbor-deals).

**Pourquoi ne pas réduire encore les centrales ?**

Le Ministère ne choisit normalement pas de financement inférieur à 60 %.

Même si les deux centrales sont financées à 60 %, elles fournissent encore environ 3 440 unités et coûtent 290 § par mois.

Ce financement minimal empêche le Ministère de ramener des centrales superflues à zéro dans le cadre de son optimisation normale.

<a id="power-example-wear"></a>
#### Exemple 5 : tenir compte de l’usure des centrales change-t-il le résultat ?

Revenons à la centrale à charbon de l’exemple 1.

Ses caractéristiques sont les suivantes :

- **Coût de remplacement :** 10 000 §.
- **État actuel :** 98 %.
- **Seuil d’usure :** 90 %.
- **Taux de vieillissement de base :** 0,001111 par mois.
- **Utilisation observée avant optimisation :** 59,5 %.

Activons maintenant **Tenir compte de l’usure des centrales**.

Le Ministère estime le coût économique de l’usure pour chaque niveau de financement possible, en se basant sur l’utilisation actuellement observée de la centrale.

| Financement | Coût de fonctionnement mensuel | Coût d’usure estimé | Coût total modélisé |
|---|---:|---:|---:|
| **84 %** | **213 §** | **4 §** | **217 §** |
| 85 % | 215 § | 4 § | 219 § |
| 100 % | 253 § | 4 § | 257 § |
| 120 % | 304 § | 4 § | 308 § |

**Le Ministère choisit toujours un financement de 84 %.**

Pourquoi ?

Parce que la différence de coût d’usure estimé est trop faible pour justifier de dépenser davantage dans le budget mensuel de la centrale.

Le coût d’usure de 4 § n’est pas prélevé sur votre trésorerie. C’est une estimation économique utilisée uniquement pour comparer les choix.

**Et la ville à deux centrales ?**

Dans l’exemple 3, avec une utilisation observée de 50 % pour la centrale à charbon et de 60 % pour la centrale à gaz, activer l’usure donne toujours le même résultat :

- Centrale à charbon : 91 %.
- Centrale à gaz naturel : 61 %.
- Coût de fonctionnement mensuel : 370 §.
- Usure estimée : 7 §.
- Coût total modélisé : 377 §.

Là encore, la combinaison économiquement la moins chère est aussi la combinaison de fonctionnement la moins chère.

**L’usure peut-elle jamais faire une différence ?**

Oui, en particulier pour les centrales coûteuses.

Prenons par exemple une centrale avec :

- Un coût de remplacement de 40 000 §.
- Un taux de vieillissement de base de 0,001389 par mois.
- Une utilisation observée de 90 %.

Son coût d’usure mensuel estimé est :

| Financement | Coût d’usure estimé |
|---|---:|
| 60 % | 45 § |
| 100 % | 43 § |
| 120 % | 39 § |

La différence entre un financement de 60 % et de 120 % est de **6 § par mois**.

Cela peut influencer le choix lorsque deux combinaisons de financement ont des dépenses de fonctionnement presque identiques.

Cependant, cela ne rend pas automatiquement préférable un financement plus élevé.

**Combien de temps une centrale peut-elle durer ?**

Avec le même modèle de vieillissement, la centrale à charbon de l’exemple 1 mettrait approximativement le nombre de mois simulés suivant pour passer d’un état de 98 % à son seuil d’usure de 90 %, en supposant une utilisation constante et un financement de 100 % :

| Utilisation de la centrale | Durée estimée jusqu’au seuil d’usure |
|---|---:|
| 50 % | 2 560 mois |
| 80 % | 1 170 mois |
| 90 % | 920 mois |
| 100 % | 740 mois |

Ce sont des **estimations théoriques**, pas des durées de vie garanties.

Le jeu réel peut se comporter différemment, car l’utilisation, le financement et l’état changent.

**Une limite importante de l’option d’usure**

Le Ministère utilise l’**utilisation actuellement observée** de chaque centrale pour estimer l’usure aux différents niveaux de financement possibles.

Il ne prédit pas exactement comment le jeu redistribuera la demande d’électricité après ces changements de financement.

Comme le montre l’exemple 1, réduire le financement peut augmenter sensiblement l’utilisation réelle, ce qui peut accélérer le vieillissement plus que ne le laisse penser l’estimation économique initiale.

Le Ministère observe la nouvelle utilisation lors des évaluations suivantes.

**Tenir compte de l’usure des centrales est donc une estimation économique, pas une garantie de durée de vie plus longue des centrales.**

<a id="power-limitations"></a>
### 7. Limites connues

Le Ministère gère automatiquement le financement de l’électricité, mais plusieurs aspects restent contrôlés par SimCity 4 ou par le joueur.

#### Compatibilité des centrales électriques

- **Les courbes de rendement et de vieillissement du jeu d’origine sont intégrées au Ministère.** Si un autre mod modifie ces courbes, le Ministère ne détecte pas automatiquement les nouvelles valeurs.
- **Les propriétés de chaque centrale sont lues dans le jeu.** La production maximale, l’état, le coût de fonctionnement, les paramètres de vieillissement et le coût de construction sont obtenus auprès de chaque centrale, ce qui permet de gérer les centrales personnalisées compatibles.
- **Certaines centrales ne peuvent pas être gérées en toute sécurité.** Un système de financement non pris en charge ou des informations illisibles font exclure une centrale des changements de financement automatiques.
- **Les centrales de valorisation énergétique des déchets qui brûlent actuellement des ordures ne sont pas ajustées.** Leur financement reste inchangé, car le même bâtiment intervient dans la gestion des déchets. Leur capacité de production disponible compte tout de même dans le plan d’électricité.

#### Financement et capacité de production

- **Le Ministère ne construit ni ne remplace de centrales.** Une véritable pénurie exige une capacité de production supplémentaire ou des achats d’électricité.
- **Le financement minimal normal est de 60 %.** Une ville qui possède beaucoup plus de centrales que nécessaire peut quand même payer au moins 60 % du coût de fonctionnement nominal de chaque centrale gérée.
- **Les centrales ne sont jamais éteintes délibérément par le financement.** Le Ministère ne choisit pas de nouvelles cibles sous son minimum normal.
- **Les très grandes villes utilisent un arrondi de capacité.** Lorsque la capacité requise dépasse 20 000 unités, l’optimiseur peut conserver une capacité de production légèrement supérieure au strict nécessaire. Il n’arrondit jamais délibérément un plan en dessous du besoin.

#### Vieillissement des centrales

- **Le calcul du vieillissement utilise l’utilisation actuellement observée.** Il ne prédit pas exactement comment un changement de financement redistribuera la production d’électricité entre les centrales.
- **Le facteur de temps du vieillissement repose sur des mesures.** Le facteur de 1/16 a été observé lors de tests plutôt que lu directement dans les données internes du jeu.
- **Les coûts d’usure et les estimations de durée de vie sont approximatifs.** Ils servent à étayer des décisions économiques, pas à prédire une date de remplacement exacte.
- **Le Ministère ne remplace jamais automatiquement les centrales usées.** Le joueur reste responsable de construire des remplacements au besoin.

#### Distribution d’électricité et échanges régionaux

- **Le besoin en électricité du jeu est utilisé tel quel.** Le Ministère n’essaie pas de le remplacer par sa propre estimation des besoins totaux de la ville.
- **Les problèmes de connexion ne sont pas des problèmes de production.** Si l’électricité disponible est suffisante mais ne peut pas atteindre certains bâtiments, modifier le financement des centrales ne résoudra pas le problème.
- **Les contrats d’électricité influencent le plan de financement.** Les importations et exportations sont incluses dans le calcul des besoins de capacité locale, mais seuls les achats utilisables sont traités comme une offre disponible.

**Le rôle du Ministère est de gérer l’électricité que votre ville peut produire, pas de redessiner le réseau ni de construire de nouvelles centrales.**

[Retour au sommaire](index.md#contents) | [Retour au README](../../README.fr.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
