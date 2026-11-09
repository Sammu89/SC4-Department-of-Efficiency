[English](../en/education_healthcare.md) | [Português](../pt/education_healthcare.md) | **Français** | [Español](../es/education_healthcare.md)

[Retour au sommaire](index.md#contents) | [Retour au README](../../README.fr.md)

<a id="education-healthcare"></a>
## A. Éducation et santé

Les adresses de l’exécutable liées à ce chapitre figurent dans la [référence des adresses du jeu](../en/game_addresses.md) (en anglais).

Le Ministère ajuste automatiquement le financement de chaque école, faculté, université, bibliothèque, musée, clinique et hôpital qu’il peut gérer. Son objectif est simple : **offrir assez de capacité à tous ceux qui ont besoin du service, avec une marge de sécurité, en dépensant le moins possible.**

Chaque bâtiment est géré individuellement. Une école de 200 élèves n’a pas besoin du même budget qu’une école de 2 000 élèves.

Le Ministère n’ajuste que le budget de fonctionnement du bâtiment. **Il ne modifie jamais le financement des bus scolaires ou des ambulances, les zones de couverture des bâtiments, ni le nombre d’équipements de votre ville.** Ces décisions restent les vôtres.

### Comment le financement influe sur la capacité

Dans SimCity 4, chaque bâtiment d’éducation et de santé dispose d’un budget de fonctionnement, affiché en simoleons (§) par mois. En coulisses, le jeu représente ce financement sous forme de pourcentage.

Le financement détermine à la fois ce que coûte le bâtiment et le nombre d’élèves, de patients, de visiteurs ou de livres qu’il peut accueillir.

La relation n’est pas entièrement proportionnelle :

| Financement | Capacité disponible | Coût de fonctionnement mensuel |
|---|---:|---:|
| 50 % | 50 % | 50 % |
| 75 % | 75 % | 75 % |
| 100 % | 100 % | 100 % |
| 120 % | 110 % | 120 % |

Les pourcentages sont exprimés par rapport à la capacité et au coût de fonctionnement du bâtiment à 100 % de financement.

**Jusqu’à 100 % de financement**, la capacité augmente au même rythme que les dépenses. **Au-delà de 100 %**, chaque point de pourcentage de financement supplémentaire n’apporte que la moitié de capacité en plus.

Par exemple, une école de 1 000 places à 100 % de financement peut accueillir environ 500 élèves à 50 %, ou 1 100 élèves à 120 %.

<a id="education-healthcare-capacity-curve"></a>
**Formule de capacité (jeu de base)**

```math
\phi(f) = \min(f,1) + \frac{\max(0,f-1)}{2}
```

```math
C = \left\lfloor B \times \phi(f) \right\rfloor
```

Où :

- **B** est la capacité du bâtiment à 100 % de financement.
- **f** est le niveau de financement (1,0 = 100 %).
- **C** est la capacité obtenue.
- **φ(f)** est le multiplicateur de capacité du jeu.

Le jeu utilise des arrondis internes et peut appliquer des modificateurs de capacité supplémentaires. La capacité affichée peut donc parfois différer légèrement d’un calcul direct. Le Ministère tient compte de ces écarts dans ses décisions.

**Coût de fonctionnement mensuel (jeu de base)**


```math
K = \mathrm{round}(K_0 \times f)
```

Où **K₀** est le coût de fonctionnement mensuel du bâtiment à 100 % de financement.

Par exemple, une école qui coûte 200 § par mois à 100 % de financement coûte environ 100 § à 50 %.

### Comment le Ministère choisit le financement

Le Ministère vérifie combien de personnes utilisent chaque bâtiment et calcule la capacité qu’il doit maintenir.

Par défaut, il vise à conserver **au moins 5 % de capacité en réserve**, mais il peut maintenir une marge plus large si nécessaire.

C’est particulièrement important pour les petits équipements, où même une légère hausse de fréquentation peut représenter une forte variation en pourcentage.

Par exemple, une clinique qui passe de 40 à 60 patients a connu une hausse de la demande de 50 %. Le Ministère tient compte de ce type de fluctuation au lieu d’appliquer aveuglément la même marge à tous les bâtiments.

Il choisit ensuite le **niveau de financement le plus bas qui permet de répondre en toute sécurité à la capacité requise**.

Si la demande augmente, le financement peut être relevé. Si la demande baisse, le Ministère peut réduire le financement pour éviter des dépenses inutiles.

### À quelle fréquence vérifie-t-il et ajuste-t-il le financement ?

Tous les délais ci-dessous se rapportent au **temps de jeu simulé**, pas au temps réel.

| Quand | Ce que fait le Ministère |
|---|---|
| **À l’ouverture d’une ville** | Effectue sa première évaluation complète le premier jour simulé. Si certains bâtiments n’ont pas encore de données exploitables, il peut réessayer les jours suivants. |
| **Tous les 2 jours simulés** | Recherche les bâtiments qui pourraient avoir besoin d’une hausse de financement urgente. |
| **Chaque jour simulé pendant une grève** | Recherche les problèmes urgents afin que le financement puisse être relevé au plus vite. |
| **Une fois par mois simulé** | Examine en détail chaque bâtiment et ajuste le financement à la hausse ou à la baisse selon les besoins du moment. |

**Les hausses de financement n’attendent pas toujours le mois suivant.** Si un bâtiment devient gravement surchargé, ou s’il est en grève, le Ministère peut relever le financement lors de ses vérifications plus fréquentes.

**Les baisses de financement n’ont lieu que lors de l’examen mensuel.** Cela évite de réduire trop vite les budgets après une baisse temporaire de fréquentation.

Le Ministère ne baisse jamais le financement lorsque les chiffres du jeu lui-même indiquent un risque de service insuffisant.

<a id="education-healthcare-strike"></a>
### Comment le Ministère aide à prévenir les grèves

Dans SimCity 4, les écoles et les établissements de santé peuvent se mettre en grève lorsque leur capacité est trop faible par rapport à la demande.

Le jeu utilise un **taux de couverture** pour déterminer ce risque.

```math
V = \min\left(1,\frac{C}{U}\right)
```

Où :

- **C** est la capacité disponible du bâtiment.
- **U** est sa demande.
- **V** est le taux de couverture.

Un taux de couverture de **1,0 (100 %)** signifie que le bâtiment a assez de capacité pour sa demande.

Un taux inférieur à **0,8 (80 %)** crée un risque de grève. Plus le taux est bas, plus le risque est élevé.

**Probabilité de grève (jeu de base)**

```math
S(V) = 100 \times \max\left(0,1-\frac{V}{0.8}\right)
```

| Couverture | Probabilité de grève |
|---|---:|
| 100 % | 0 % |
| 80 % | 0 % |
| 60 % | 25 % |
| 40 % | 50 % |
| 0 % | 100 % |

Une grève prend fin lorsque le taux de couverture du jeu atteint 90 %.

Le Ministère conserve normalement plus de capacité que le minimum nécessaire pour éviter les grèves. Il surveille aussi les bâtiments pour détecter des hausses de demande imprévues.

Si une grève est déjà en cours, **le Ministère relève le financement du bâtiment à son maximum configuré**, à condition de pouvoir contrôler ce bâtiment.

Le calcul de couverture interne du jeu peut parfois différer de la demande affichée dans la fenêtre d’information du bâtiment. Pour cette raison, le Ministère s’appuie aussi sur les informations de couverture du jeu lui-même afin d’éviter des baisses de financement risquées.

### Que se passe-t-il quand le financement ne suffit plus ?

Le financement ne peut pas résoudre tous les problèmes de capacité.

À 120 % de financement, un bâtiment ne fournit qu’environ 110 % de sa capacité normale. Si la demande dépasse ce que le bâtiment peut absorber en toute sécurité à son financement maximal, le Ministère ne peut pas créer de places supplémentaires.

**Vous devez construire un autre équipement.**

Lorsque le Ministère confirme qu’une pénurie ne peut pas être résolue par le financement et qu’elle persiste pendant **30 jours simulés**, il envoie un avis de conseiller dans le jeu.

Quand c’est possible, l’avis contient un lien qui vous mène directement au bâtiment concerné.

Le Ministère ne construit pas de bâtiments et ne modifie pas leurs zones de couverture. C’est à vous de décider où placer les nouvelles écoles, hôpitaux et autres équipements.

Pour d’autres garde-fous et des cas particuliers, consultez les [Conditions de sécurité](#education-healthcare-safety).
<a id="education-healthcare-problem"></a>
### 2. Le problème de gestion

Gérer à la main les budgets d’éducation et de santé est plus compliqué qu’il n’y paraît.

- **Argent gaspillé :** une école à moitié vide n’a pas besoin du même financement qu’une école pleine. Garder chaque bâtiment à 100 % peut gaspiller des milliers de simoleons au fil du temps.
- **Risque de grève :** réduire trop le financement diminue la capacité et peut provoquer des grèves. Le Ministère doit économiser de l’argent sans mettre les services en danger.
- **Demande changeante :** le nombre d’élèves et de patients fluctue avec la croissance de la ville. Certains changements peuvent être soudains, notamment lorsque les élèves passent d’un niveau scolaire à l’autre.
- **Petits bâtiments :** une clinique qui passe de 40 à 60 patients connaît une hausse de la demande de 50 %. Les petits équipements ont besoin d’une protection supplémentaire contre ces fluctuations.
- **Nouveaux bâtiments :** le jeu peut mettre quelques jours simulés à indiquer combien de personnes utilisent un équipement tout juste placé.
- **Capacité limitée :** même à 120 % de financement, un bâtiment ne peut fournir qu’environ 110 % de sa capacité normale. Au-delà, le joueur doit construire un autre équipement.
- **Trop de bâtiments à surveiller :** dans une grande ville, examiner à la main chaque mois toutes les écoles, bibliothèques, musées, cliniques et hôpitaux devient fastidieux.

**Le Ministère résout ce problème en surveillant chaque bâtiment individuellement, en maintenant assez de capacité pour que les services continuent de fonctionner et en réduisant les dépenses inutiles dès que c’est sans risque.**

<a id="education-healthcare-algorithm"></a>
### 3. Comment le Ministère prend ses décisions

<a id="education-healthcare-eligibility"></a>
#### 3.1 Quels bâtiments peuvent être gérés ?

Le Ministère gère automatiquement les bâtiments d’éducation et de santé qui utilisent le système de financement standard du jeu.

Cela comprend :

| Éducation | Santé |
|---|---|
| Écoles primaires | Cliniques |
| Lycées | Hôpitaux |
| Facultés et universités | Centres de recherche médicale |
| Bibliothèques | |
| Musées | |

Les bâtiments personnalisés compatibles peuvent aussi être gérés, à condition qu’ils utilisent les mêmes mécanismes de financement que le jeu d’origine.

Avant de gérer un bâtiment, le Ministère vérifie que :

1. Le jeu l’identifie comme un équipement d’éducation ou de santé.
2. Il dispose de son propre budget de fonctionnement, qui peut réellement être ajusté.
3. Sa fenêtre d’information fournit des données de demande et de capacité reconnaissables.
4. Ses chiffres de financement et de budget sont cohérents.

**Si un bâtiment ne remplit pas ces conditions, le Ministère n’y touche pas.**

Par exemple, les écoles privées du jeu d’origine n’ont pas de budget de fonctionnement propre ajustable : le Ministère n’a donc rien à gérer. Certains bâtiments personnalisés utilisent aussi des systèmes de financement ou des fenêtres d’information spéciaux que le Ministère ne peut pas contrôler en toute sécurité.

Les bâtiments sont identifiés par leur fonctionnement, **pas par leur nom**. Une école personnalisée compatible peut donc être gérée exactement comme une école d’origine de SimCity 4.

Les commissariats, les casernes de pompiers et les prisons ne sont pas gérés par ces services.

<a id="education-healthcare-required"></a>
#### 3.2 De quelle capacité un bâtiment a-t-il besoin ?

Le Ministère ne se contente pas d’aligner le financement sur la fréquentation actuelle. Il conserve une **capacité supplémentaire pour faire face aux hausses de demande imprévues**.

La marge de sécurité par défaut est de **5 %**, mais le Ministère peut automatiquement en maintenir davantage lorsqu’un bâtiment a besoin d’une protection supplémentaire.

Il prend en compte trois facteurs :

**1. Votre marge de sécurité**

Par défaut, un bâtiment qui dessert 1 000 personnes doit avoir une capacité d’au moins 1 050.

Vous pouvez modifier cette marge dans les réglages du Ministère.

**2. Les hausses de demande passées**

Le Ministère mémorise de combien la fréquentation d’un bâtiment a augmenté d’un mois à l’autre.

Il utilise jusqu’à **36 observations mensuelles de la session de jeu en cours** et retient la plus forte hausse. Seules les comparaisons partant d’au moins 50 usagers sont prises en compte, car de très petits nombres peuvent produire des pourcentages trompeurs.

Par sécurité, la hausse utilisée dans le calcul est plafonnée à **50 %**.

Les lycées, facultés et universités bénéficient d’une protection spéciale. Les transferts d’étudiants entre niveaux d’enseignement peuvent provoquer de fortes hausses annuelles ; le Ministère suppose donc une hausse possible de 50 % tant qu’il n’a pas recueilli 13 observations mensuelles.

**Plus forte hausse mensuelle**

```math
v = \min\left(0.5,\ \max\left(0,\ \max_i\left(\frac{U_i}{U_{i-1}}-1\right)\right)\right)
```

Où :

- **Uᵢ** est le nombre d’usagers enregistré au mois *i*.
- **v** est la plus forte hausse mensuelle valide, exprimée sous forme de fraction et plafonnée à 0,5.

**3. Protection supplémentaire pour les petits bâtiments**

Les petits équipements peuvent connaître de fortes variations en pourcentage même lorsque seules quelques personnes arrivent.

Le Ministère conserve donc une capacité supplémentaire :

| Fréquentation actuelle | Marge de capacité minimale |
|---|---|
| Moins de 100 usagers | 50 % de capacité en plus |
| 100 à 199 usagers | 25 % de capacité en plus |
| 200 usagers ou plus | Votre marge choisie, ajustée au besoin selon les fluctuations de la demande |

Par exemple, une clinique de 60 patients conserve une capacité d’au moins 90 patients, même si votre marge de sécurité configurée n’est que de 5 %.

Cette protection supplémentaire est particulièrement utile pour les petits bâtiments, où le surcoût de fonctionnement est généralement modeste.

**Comment ces protections se combinent**

Le Ministère compare les trois exigences et retient celle qui demande le plus de capacité :

- Votre marge de sécurité configurée.
- La protection contre les hausses de demande passées et contre les grèves.
- La protection supplémentaire pour les petits bâtiments.

**Formule de la marge de sécurité**

```math
M = \max\left(1+\frac{r}{100},\ 0.85(1+v),\ s(U)\right)
```

Où :

- **r** est votre marge de sécurité configurée, 5 % par défaut.
- **v** est la plus forte hausse mensuelle décrite plus haut.
- **U** est la fréquentation actuelle du bâtiment.
- **s(U)** vaut 1,5 en dessous de 100 usagers, 1,25 de 100 à 199 usagers, et 0 sinon.
- **M** est le multiplicateur de capacité obtenu.

Le facteur 0,85 de la deuxième règle vise à maintenir la couverture au-dessus du seuil de risque de grève de 80 % du jeu, même après une hausse de demande observée précédemment.

**Capacité requise**

```math
R = \left\lceil U \times M \right\rceil
```

Où **R** est la capacité minimale que le Ministère veut que le bâtiment fournisse.

Par exemple, une école de 610 élèves, sans hausse de demande inhabituellement forte et avec une marge de sécurité de 5 %, a besoin de :

```math
R = \lceil 610 \times 1.05 \rceil = 641
```

Le Ministère cherche donc le niveau de financement le moins cher qui fournit au moins 641 places, avec une petite tolérance pour le comportement d’arrondi du jeu.

<a id="education-healthcare-choice"></a>
#### 3.3 Comment le Ministère choisit-il le financement ?

Une fois la capacité requise connue, le Ministère vérifie quel niveau de financement peut la fournir.

Il choisit **le pourcentage entier le plus bas qui satisfait l’exigence en toute sécurité**.

Par défaut, il considère des niveaux de financement de **1 % à 120 %**.

**Choix du financement**

```math
p^* = \min\left\{p\in[p_{\min},p_{\max}]: C(p)-1\ge R\right\}
```

Où :

- **p** est un pourcentage de financement possible.
- **C(p)** est la capacité attendue à ce financement.
- **R** est la capacité requise calculée plus haut.
- La soustraction d’une unité offre une petite protection contre l’arrondi de capacité du jeu.

Par exemple, si une école a besoin de 641 places et que 54 % de financement en fournissent environ 647, le Ministère choisit **54 %**, en supposant que 53 % ne suffisent pas.

Cela évite de payer pour des centaines de places inutilisées.

**Règles de sécurité supplémentaires**

Le Ministère suit aussi plusieurs règles importantes :

- **Pas de changements inutiles :** si le financement est déjà au bon niveau, il n’y touche pas.
- **Grèves :** si un bâtiment est en grève, son financement est directement relevé au maximum configuré, normalement 120 %.
- **Nouveaux bâtiments :** si le jeu n’a pas encore indiqué la fréquentation, le Ministère ne suppose pas que le bâtiment est vide. Il maintient le financement à 100 % au moins, ou au maximum configuré si celui-ci est inférieur, jusqu’à ce que des données exploitables soient disponibles.
- **Pas de baisse risquée :** le financement n’est jamais réduit tant que la couverture indiquée par le jeu est inférieure à 85 % ou qu’une grève est en cours.
- **Pas de financement à zéro :** avec la configuration par défaut, le Ministère ne règle jamais le financement d’un bâtiment à 0 %.
- **Les baisses sont mensuelles :** le financement peut être relevé lors des vérifications urgentes, mais il n’est réduit que lors de l’examen mensuel.

Si même le financement maximal ne peut pas fournir la capacité souhaitée, le Ministère maintient le bâtiment à son maximum et vérifie s’il s’agit d’une véritable pénurie.

Un bâtiment légèrement en dessous de sa marge de sécurité n’a pas forcément besoin d’un autre équipement. Le Ministère ne vous avertit que lorsque la capacité disponible est réellement insuffisante et que le problème persiste.

Consultez les [Pénuries](#education-healthcare-shortage) pour connaître les conditions qui déclenchent un avis de conseiller.

<a id="education-healthcare-capacity-evidence"></a>
#### 3.4 Et si la capacité réelle du jeu diffère du calcul ?

SimCity 4 ne rapporte pas toujours exactement la capacité prévue par sa formule de financement standard.

De petites différences d’arrondi sont normales. Certaines villes ou certains bâtiments personnalisés peuvent aussi appliquer des modificateurs de capacité supplémentaires.

Le Ministère **vérifie donc la capacité réelle indiquée par le jeu avant de modifier le financement d’un bâtiment**.

Il gère trois situations :

| Situation | Ce que fait le Ministère |
|---|---|
| **La capacité correspond au calcul** | Utilise la formule standard et gère le bâtiment normalement. Un écart d’une place est accepté comme un arrondi normal. |
| **La capacité suit un modificateur constant** | Apprend l’écart et ajuste ses calculs, en prenant des précautions tant que le modificateur n’est pas confirmé. |
| **La capacité se comporte de façon imprévisible** | Évite les baisses de financement automatiques plutôt que de risquer une perte de capacité inattendue. |

**Apprendre la capacité réelle d’un bâtiment**

Par exemple, une école personnalisée peut fournir de façon constante 15 % de capacité de plus que ne le prévoit la formule standard.

Plutôt que d’ignorer cette capacité supplémentaire ou de s’y fier immédiatement, le Ministère vérifie si l’écart est constant.

Le cas échéant, il peut effectuer une petite hausse de financement sans risque lors d’un examen mensuel pour confirmer comment la capacité réagit. Il utilise ensuite la relation vérifiée pour choisir le financement plus précisément.

Cet étalonnage est prudent : il n’est tenté que lorsque le bâtiment a déjà assez de capacité, et il ne suppose aucun bonus non confirmé.

Si la capacité ne peut pas être prédite de façon fiable, le Ministère laisse le financement du bâtiment inchangé ou se limite à des hausses sûres lorsque la capacité observée est insuffisante.

**La priorité est toujours d’éviter les baisses de financement risquées.** Économiser quelques simoleons ne vaut pas le risque de fragiliser une école ou un hôpital dont le jeu rapporte la capacité de façon incohérente.

<a id="education-healthcare-timing"></a>
#### 3.5 Quand le Ministère vérifie-t-il et ajuste-t-il le financement ?

Le Ministère surveille les bâtiments d’éducation et de santé tout au long de la simulation. **Il peut relever le financement dès qu’un problème apparaît, mais ne le réduit que lors de son examen mensuel.**

Tous les délais se rapportent au temps de jeu simulé.

| Quand | Ce qui se passe | Hausses de financement | Baisses de financement |
|---|---|---|---|
| **À l’ouverture d’une ville** | Vérifie chaque bâtiment géré le premier jour simulé. Si certains bâtiments n’ont pas encore de données de fréquentation, il peut répéter la vérification les jours suivants, jusqu’à six fois au total. | Oui | Non |
| **Tous les 2 jours simulés** | Recherche les bâtiments qui nécessitent une attention urgente en raison d’une surcharge ou d’un financement insuffisant. | Oui | Non |
| **Chaque jour pendant une grève** | Recherche les problèmes de financement urgents tant qu’une grève de l’éducation ou de la santé est signalée. | Oui | Non |
| **Chaque mois simulé** | Examine chaque bâtiment, met à jour l’historique de fréquentation et ajuste le financement aux besoins actuels. | Oui | Oui |

**Qu’est-ce qu’un problème urgent ?**

Le Ministère intervient entre deux examens mensuels lorsqu’un bâtiment :

- N’est pas déjà à son financement maximal.
- A besoin d’une hausse de financement selon ses règles de sécurité.
- Est en grève, ou a une capacité actuelle inférieure de plus de 3 % à sa demande.

La dernière condition s’exprime ainsi :

```math
C < 0.97 \times U
```

Où :

- **C** est la capacité disponible du bâtiment.
- **U** est sa fréquentation actuelle.

Par exemple, une école dont la capacité est de 900 élèves pour une fréquentation de 1 000 est nettement surchargée. Le Ministère peut relever son financement sans attendre le mois suivant.

Un problème nouveau ou qui évolue est évalué lors des vérifications régulières. Si le même problème persiste sans changer, le Ministère peut répéter son évaluation tous les **7 jours simulés**.

Une hausse supplémentaire n’aide pas un bâtiment déjà à son financement maximal. Le Ministère traite alors cette situation comme une possible pénurie de capacité.

**La priorité est toujours de rétablir les services d’abord et d’optimiser les dépenses ensuite.**

<a id="education-healthcare-buses"></a>
#### 3.6 Que modifie le Ministère, et comment vérifie-t-il son travail ?

Le Ministère ne modifie que **le budget de fonctionnement de chaque bâtiment éligible**, en utilisant le même système de financement que le curseur de budget du jeu.

Il ne modifie jamais :

- Le financement des bus scolaires.
- Le financement des ambulances.
- Les zones de couverture des bâtiments.
- L’emplacement des bâtiments ni le nombre d’équipements.

**Chaque changement de financement est vérifié avant et après son application.**

Avant de modifier un budget, le Ministère relit les réglages actuels du bâtiment. Si vous avez déplacé son curseur vous-même depuis que le calcul a été fait, le Ministère ignore ce changement et recalcule plus tard.

Après avoir effectué un changement, il vérifie que :

1. Le nouveau financement est correctement rapporté par le jeu.
2. Le coût de fonctionnement mensuel du bâtiment est cohérent avec le nouveau financement.
3. La capacité disponible réagit comme prévu.
4. Le financement des bus scolaires ou des ambulances, le cas échéant, reste inchangé.

**Que se passe-t-il si quelque chose ne va pas ?**

Si un changement ne peut pas être vérifié, le Ministère cesse de gérer ce bâtiment pour le reste de la session de ville en cours.

Si trois bâtiments différents échouent à ces vérifications, il cesse complètement de modifier le financement de l’éducation et de la santé jusqu’à la réouverture de la ville.

Cela évite de tenter à répétition de modifier des bâtiments dont le système de financement se comporte de façon inattendue.

**Pouvez-vous encore modifier un budget à la main ?**

Oui. Vous pouvez ajuster vous-même le financement de n’importe quel bâtiment. Le Ministère détecte le changement et le prend en compte lors de la prochaine évaluation, tant que ce bâtiment reste sous gestion automatique.

Si vous voulez garder un contrôle manuel complet de ces budgets, désactivez **Gérer l’éducation** ou **Gérer la santé** dans les réglages du Ministère.

<a id="education-healthcare-shortage"></a>
#### 3.7 Que se passe-t-il quand le financement ne suffit pas ?

Certains bâtiments deviennent tellement surchargés que même le financement maximal ne peut pas fournir assez de capacité.

Dans ce cas, **le Ministère ne peut pas résoudre le problème. Il vous faut un autre bâtiment.**

Cependant, le Ministère ne considère pas immédiatement chaque bâtiment surchargé comme une pénurie.

Il vérifie d’abord si un financement plus élevé pourrait régler le problème.

| Situation | Ce que fait le Ministère |
|---|---|
| **Le financement est inférieur au maximum** | Relève le financement si nécessaire. Il ne considère pas encore la pénurie comme inévitable. |
| **Le financement est au maximum, avec assez de capacité** | Maintient le financement du bâtiment et n’émet pas d’avis de pénurie. |
| **Le financement est au maximum, mais la couverture est inférieure à 85 %** | Confirme que le bâtiment ne peut pas répondre correctement à sa demande actuelle. |
| **Le bâtiment est en grève mais pourrait normalement fournir une capacité suffisante** | Traite la grève comme un problème temporaire plutôt que comme la preuve qu’un nouveau bâtiment est nécessaire. |

**Couverture maximale atteignable**

```math
V_{\max} = \min\left(1,\frac{C_{\max}}{U}\right)
```

Où :

- **Cₘₐₓ** est la capacité disponible au financement maximal.
- **U** est la demande actuelle du bâtiment.
- **Vₘₐₓ** est la couverture la plus élevée que le bâtiment puisse atteindre.

Si le bâtiment est en grève et que sa capacité rapportée est temporairement affectée, le Ministère estime quelle serait sa capacité sans la grève.

**Une pénurie est confirmée lorsque la couverture maximale atteignable est inférieure à 85 %.**

Par exemple, un hôpital qui peut accueillir 1 500 patients au financement maximal mais dont la demande est de 2 000 patients ne peut couvrir que 75 % de cette demande.

Aucune hausse de financement ne peut y remédier. Un autre établissement de santé est nécessaire.

Le Ministère attend encore avant de vous prévenir, pour éviter de signaler des problèmes temporaires qui pourraient se résoudre d’eux-mêmes.

<a id="education-healthcare-notices"></a>
#### 3.8 Quand et comment le Ministère vous prévient-il ?

Le Ministère essaie de traiter discrètement les problèmes de financement courants.

**Vous ne recevez pas de message chaque fois qu’il économise de l’argent, relève un financement ou évite une grève possible.**

Il vous prévient lorsqu’un bâtiment a réellement besoin de votre intervention.

**Quand un avis est-il envoyé ?**

Une pénurie doit rester confirmée à chaque évaluation pendant **30 jours simulés** avant que le Ministère n’émette un avis.

Si le bâtiment se rétablit pendant cette période, le décompte repart de zéro si la pénurie revient.

Les bâtiments actuellement en grève ne sont pas inclus dans les avis de pénurie, car SimCity 4 fournit déjà ses propres alertes de grève.

**Que montre l’avis ?**

L’avis apparaît dans la liste normale des nouvelles des conseillers du jeu.

Il indique quel type de bâtiment a besoin de plus de capacité et fournit des liens cliquables qui vous mènent directement aux lieux concernés.

Par exemple, un avis pour une école surchargée pourrait dire :

> **Trop d’élèves, pas assez de place !**
>
> Monsieur le Maire *(votre nom)*, cette école déborde ! Le financement est au maximum, le Ministère a épuisé tous ses recours, mais les élèves continuent d’arriver. Il faudrait construire une autre école, avant que la cour de récréation ne devienne une salle de classe ! Pour y regarder de plus près, rendez-vous à **l’école** où les pupitres ont demandé des renforts.

Dans le jeu, **l’école** est un lien cliquable qui déplace la caméra vers le bâtiment concerné.

**Comment les pénuries multiples sont-elles gérées ?**

- Le Ministère regroupe les pénuries par type de bâtiment, par exemple écoles primaires, hôpitaux ou bibliothèques.
- Chaque avis peut lister jusqu’à **10 bâtiments**, les plus surchargés en premier.
- Les autres bâtiments concernés continuent d’être surveillés et peuvent apparaître lorsque des pénuries antérieures sont résolues.
- Si les bâtiments concernés changent, le Ministère met à jour l’avis existant chaque fois que c’est possible plutôt que de créer inutilement de nouveaux messages.
- Si le nombre de bâtiments listés change, l’ancien avis est remplacé. Des changements répétés peuvent déclencher une **pause de 30 jours** entre deux remplacements afin de ne pas inonder la liste des conseillers.
- Lorsque tous les bâtiments couverts par un avis de pénurie se sont rétablis, l’avis est supprimé.

**Le même avertissement réapparaîtra-t-il chaque fois que vous rouvrez votre ville ?**

Non. Le Ministère mémorise quelles pénuries il a déjà signalées pour chaque ville identifiable.

Si vous fermez puis rouvrez une ville avec le même problème non résolu, il ne répète normalement pas l’avertissement. Fermer un avis empêche aussi qu’une même pénurie inchangée soit annoncée de nouveau.

Ces informations sont conservées séparément de vos sauvegardes de ville.

**Où les avis apparaissent-ils ?**

Les avis de pénurie d’éducation et de santé sont transmis par les conseillers du jeu. Ils apparaissent dans la liste des nouvelles des conseillers sans ouvrir de fenêtre intrusive.

Pour le système de notification partagé avec les autres services, consultez les [Avis des conseillers](safety.md#notices).

<a id="education-healthcare-safety"></a>
### 4. Conditions de sécurité

Le Ministère applique plusieurs garde-fous pour éviter de réduire les services à tort.

**Avant de modifier le financement**

- Il ne gère que les bâtiments dont le système de financement standard peut être identifié et vérifié.
- Il ne suppose jamais que l’absence de données de fréquentation signifie qu’un bâtiment est vide.
- Il revérifie le financement et la capacité du bâtiment avant d’effectuer un changement.
- Il laisse tranquilles les bâtiments dont les relevés sont peu fiables ou incohérents.

**Lors d’une baisse de financement**

- Les baisses n’ont lieu que lors de l’examen mensuel.
- Le financement n’est jamais réduit tant qu’un bâtiment est en grève.
- Le financement n’est jamais réduit lorsque la couverture indiquée par le jeu est inférieure à 85 %.
- Les bâtiments sans données de fréquentation exploitables ne sont jamais sous-financés.
- Les bâtiments dont la capacité ne peut pas être prédite de façon fiable sont protégés contre les baisses automatiques.
- Avec la configuration par défaut, le financement ne descend jamais sous 1 %.

**Lors d’une hausse de financement**

- Un bâtiment en grève est relevé à son financement maximal configuré.
- Les bâtiments qui ont des problèmes de capacité urgents peuvent recevoir des hausses entre deux examens mensuels.
- Si le financement maximal ne peut pas résoudre une véritable pénurie, le Ministère laisse le bâtiment à son financement maximal et, si le problème persiste, vous prévient.

**Après un changement**

- Le Ministère vérifie que le nouveau budget et la capacité sont cohérents.
- Le financement des bus scolaires et des ambulances doit rester inchangé.
- Un bâtiment qui échoue à la vérification est exclu de toute modification automatique pendant la session.
- Trois échecs de vérification distincts arrêtent les changements de financement de l’éducation et de la santé pour la session.

**Lorsque la gestion automatique est désactivée**

Désactiver le Ministère ne rétablit pas les budgets précédents. Chaque bâtiment conserve son financement actuel jusqu’à ce que vous le modifiiez à la main ou que vous réactiviez la gestion.

#### Les messages d’origine des conseillers du jeu

SimCity 4 affiche normalement des avertissements lorsque le financement de l’éducation ou de la santé tombe sous certains pourcentages.

Par exemple, le jeu peut se plaindre de budgets d’éducation inférieurs à 90 %, même lorsqu’une école a largement assez de capacité avec un financement bien plus faible.

Ces avertissements ne sont pas toujours pertinents lorsque le Ministère gère les bâtiments individuellement.

Tant que le Ministère gère l’éducation, ou la santé, le plugin ajuste donc certains conseils d’origine du jeu pour ce service :

- Les avertissements courants fondés uniquement sur de faibles pourcentages de financement sont supprimés.
- Certains avertissements de surfinancement ou de niveaux de personnel sont aussi supprimés.
- Les avertissements de pré-grève concernés sont limités aux situations où une grève a réellement lieu.
- **Les vraies alertes de grève restent actives.**
- Les messages habituels de félicitations et d’état du jeu sont conservés.

Cela suit l’état du Ministère dans chaque ville, sans redémarrer le jeu. Si vous désactivez la gestion de l’éducation ou de la santé (ou tout le Ministère), ou si le Ministère ne peut pas agir dessus, les conseils d’origine du jeu pour ce service reviennent aussitôt.

Consultez la [Sécurité générale](safety.md#safety) pour en savoir plus.

#### Si le jeu déplace un curseur de lui-même

Quand le jeu modifie le financement d’une école ou d’un hôpital que le Ministère avait réglé, juste après un recalcul mensuel, le Ministère réévalue ce bâtiment à sa prochaine surveillance, dans les deux sens, et applique le financement le plus bas qui donne encore la capacité exigée. Voir [Phantom Funding Slider Bug](phantom_slider.md).

<a id="education-healthcare-settings"></a>
### 5. Réglages

La gestion de l’éducation et de la santé se configure directement dans **Options de jeu → Ministère de l’Optimisation**, sur la page **Services**.

| Réglage | Par défaut | Plage | Ce qu’il fait |
|---|---|---|---|
| [Ministère actif](options.md#opt-master) | Activé | Activé / Désactivé | Active ou désactive l’ensemble du Ministère. |
| [Gérer l’éducation](options.md#opt-education) | Activé | Activé / Désactivé | Gère automatiquement le financement des écoles, facultés, universités, bibliothèques et musées éligibles. |
| [Marge de sécurité (éducation)](options.md#opt-educationreserve) | 5 % | 0–50 % | Définit la capacité minimale en réserve que le Ministère vise à maintenir dans les bâtiments d’éducation. |
| [Gérer la santé](options.md#opt-health) | Activé | Activé / Désactivé | Gère automatiquement le financement des cliniques, hôpitaux et centres de recherche médicale éligibles. |
| [Marge de sécurité (santé)](options.md#opt-healthreserve) | 5 % | 0–50 % | Définit la capacité minimale en réserve que le Ministère vise à maintenir dans les bâtiments de santé. |

**Comment fonctionne la marge de sécurité ?**

Une marge de sécurité de 5 % signifie qu’un bâtiment de 1 000 usagers doit avoir une capacité d’au moins 1 050.

Cependant, **5 % est un minimum, pas une cible fixe**. Le Ministère peut conserver plus de capacité lorsqu’un bâtiment est petit ou a connu de fortes hausses de demande.

Augmenter la marge offre un tampon supplémentaire contre une croissance soudaine, mais peut aussi augmenter les coûts de fonctionnement.

Les changements effectués dans la fenêtre d’options du jeu s’appliquent immédiatement. Si vous modifiez les fichiers de configuration à la main, ces changements sont lus au début du mois simulé suivant.

Les réglages invalides ne sont pas corrigés silencieusement. Une valeur hors plage ou un réglage inconnu peut laisser la gestion de l’éducation et de la santé désactivée jusqu’à la correction de la configuration.

#### Réglages avancés (fichier INI uniquement)

Deux réglages supplémentaires sont disponibles dans le fichier de configuration. Ils n’apparaissent pas dans la fenêtre d’options du jeu.

| Réglage | Par défaut | Plage autorisée | Ce qu’il fait |
|---|---|---|---|
| `MinFundingPercent` | 1 % | 0–100 % | Définit le pourcentage de financement le plus bas que le Ministère peut choisir. |
| `MaxFundingPercent` | 120 % | 1–120 % | Définit le pourcentage de financement le plus élevé que le Ministère peut choisir, y compris pendant les grèves. |

**Financement minimal :** la valeur par défaut de 1 % empêche le Ministère de supprimer complètement le budget de fonctionnement d’un bâtiment, même lorsqu’il n’a aucun usager signalé.

**Financement maximal :** la valeur par défaut de 120 % permet aux bâtiments de fonctionner au niveau de financement le plus élevé disponible dans le jeu d’origine. Abaisser cette limite restreint les dépenses, mais réduit aussi la capacité que le Ministère peut fournir en cas de surcharge ou de grève.

Le minimum ne doit pas dépasser le maximum.

Pour la plupart des joueurs, **les valeurs par défaut sont recommandées**.

<a id="education-healthcare-examples"></a>
### 6. Exemples détaillés

Les exemples suivants montrent comment le Ministère ajuste le financement dans différentes situations.

Les capacités et les coûts de fonctionnement des bâtiments sont donnés à titre d’illustration. Vos chiffres peuvent différer selon le bâtiment et les mods que vous utilisez.

Le jeu utilise aussi des arrondis internes : les capacités affichées peuvent donc parfois différer d’une unité d’un simple calcul en pourcentage.

#### Exemple 1 : une école avec plus de capacité que nécessaire

Une école primaire présente :

- **Capacité normale maximale :** 1 200 élèves.
- **Fréquentation actuelle :** 610 élèves.
- **Coût de fonctionnement à 100 % de financement :** 187 §/mois.
- **Marge de sécurité :** 5 %.

Le Ministère calcule la capacité requise :

```math
R = \lceil 610 \times 1.05 \rceil = 641
```

Il compare ensuite les niveaux de financement :

| Financement | Capacité disponible | Résultat |
|---|---:|---|
| 53 % | 635 | Insuffisant |
| 54 % | 647 | Suffisant |
| 100 % | 1 200 | Plus que nécessaire |

**Le Ministère choisit un financement de 54 %.**

Le coût de fonctionnement mensuel passe de 187 § à environ **101 §**, soit une économie de **86 § par mois**.

Le financement des bus scolaires reste inchangé.

#### Exemple 2 : une petite clinique a besoin d’une marge de sécurité plus large

Une clinique a une capacité normale de 1 400 patients mais n’en accueille actuellement que 60.

Bien que la marge de sécurité configurée soit de 5 %, les petits bâtiments bénéficient d’une protection supplémentaire contre les changements soudains de fréquentation.

Comme cette clinique a moins de 100 patients, le Ministère conserve **50 % de capacité en plus** :

```math
R = \lceil 60 \times 1.5 \rceil = 90
```

À 6 % de financement, la clinique ne peut accueillir que 84 patients. À 7 %, elle peut en accueillir environ 98.

**Le Ministère choisit un financement de 7 %.**

C’est un peu plus que ce qu’exigerait la marge habituelle de 5 %, mais cela offre une protection utile contre les hausses soudaines pour un surcoût très faible.

#### Exemple 3 : un lycée aux variations saisonnières de fréquentation

Un lycée présente :

- **Capacité normale :** 5 000 élèves.
- **Fréquentation actuelle :** 900 élèves.
- **Moins de 13 mois d’historique de fréquentation dans la session en cours.**

Les lycées peuvent recevoir un grand nombre d’élèves lorsque ceux de l’école primaire passent au niveau supérieur.

Tant que le nombre d’observations mensuelles n’est pas suffisant, le Ministère suppose une possible **hausse de fréquentation de 50 %**.

Cela donne une capacité requise de :

```math
R = \lceil 900 \times 0.85 \times 1.5 \rceil = 1148
```

**Le Ministère choisit un financement de 23 %**, ce qui fournit environ 1 149 places.

Après 13 observations mensuelles, supposons que la plus forte hausse réellement enregistrée n’ait été que de 8 %.

Le Ministère peut alors s’appuyer sur le profil de demande observé et revenir à la marge de sécurité normale de 5 %.

La capacité requise devient 945 élèves.

**Le financement peut descendre à 19 %**, ce qui fournit environ 949 places.

Le Ministère évite les dépenses inutiles tout en tenant compte de la demande saisonnière.

#### Exemple 4 : une bibliothèque avec de la capacité inutilisée

Une bibliothèque a une capacité de collection de 40 000 livres, mais seuls 9 000 sont actuellement empruntés.

Avec une marge de 5 %, le Ministère a besoin d’une capacité de :

```math
R = \lceil 9000 \times 1.05 \rceil = 9450
```

À 23 % de financement, la capacité est d’environ 9 199 livres, ce qui est insuffisant.

À 24 %, la capacité atteint environ 9 599.

**Le Ministère choisit un financement de 24 %.**

La bibliothèque continue de servir ses lecteurs sans qu’on paie toute sa capacité de collection inutilisée.

#### Exemple 5 : un musée avec peu de visiteurs

Un musée peut accueillir 2 800 visiteurs mais n’en reçoit actuellement que 150.

Comme il accueille moins de 200 visiteurs, le Ministère conserve une **marge de sécurité de 25 %**.

```math
R = \lceil 150 \times 1.25 \rceil = 188
```

À 7 % de financement, le musée a une capacité d’environ 196 visiteurs.

**Le Ministère choisit un financement de 7 %.**

Le musée reste opérationnel, avec assez de capacité en réserve pour les fluctuations de fréquentation.

#### Exemple 6 : une hausse soudaine entre deux examens mensuels

Reprenons l’école primaire de l’exemple 1.

Elle présente actuellement :

- **Financement :** 54 %.
- **Capacité :** 647 élèves.
- **Fréquentation :** 610 élèves.

Au milieu du mois, la fréquentation passe soudainement à **700 élèves**.

L’école est maintenant gravement surchargée.

Le Ministère recherche les problèmes urgents tous les deux jours simulés. Comme la capacité disponible est inférieure de plus de 3 % à la demande, il n’attend pas l’examen mensuel suivant.

La nouvelle capacité requise est :

```math
R = \lceil 700 \times 1.05 \rceil = 735
```

**Le financement passe de 54 % à 62 %**, ce qui fournit environ 744 places.

Si la fréquentation baisse ensuite, le Ministère attend l’examen mensuel suivant avant de réduire le budget.

C’est voulu : **les hausses de financement protègent immédiatement les services, tandis que les baisses sont traitées avec plus de prudence**.

#### Exemple 7 : un financement maximal ne signifie pas toujours une pénurie

Une clinique a une capacité normale de 1 400 patients.

À 120 % de financement, elle peut accueillir environ **1 539 patients**.

Prenons deux situations :

| Patients actuels | Capacité maximale | Couverture | Décision du Ministère |
|---|---:|---:|---|
| 1 500 | 1 539 | 100 % | Maintenir le financement maximal ; pas d’avis de pénurie |
| 1 900 | 1 539 | 81 % | Confirmer une pénurie de capacité |

Avec 1 500 patients, la clinique a assez de capacité pour servir tout le monde, même si elle ne peut pas maintenir la marge de sécurité préférée.

**Aucun nouveau bâtiment n’est encore nécessaire.**

Avec 1 900 patients, même le financement maximal ne peut pas assurer une couverture adéquate.

Le Ministère maintient le financement à 120 %. Si la pénurie reste confirmée pendant 30 jours simulés et que la clinique n’est pas en grève, un avis de conseiller vous indique qu’une autre clinique est nécessaire.

Il y a aussi un arbitrage financier : un financement de 120 % coûte **20 % de plus**, mais ne fournit que **10 % de capacité en plus** par rapport à 100 %.

#### Exemple 8 : un hôpital qui a besoin de capacité supplémentaire

Un hôpital présente :

- **Capacité normale :** 3 000 patients.
- **Demande actuelle :** 4 376 patients.
- **Financement maximal :** 120 %.

Même au financement maximal, l’hôpital ne peut accueillir qu’environ **3 299 patients**.

Sa couverture maximale est :

```math
V_{\max} = \frac{3299}{4376} \approx 0.754
```

Cela signifie que l’hôpital ne peut répondre qu’à environ **75,4 % de sa demande**.

C’est inférieur à la fois au seuil de pénurie de 85 % du Ministère et au seuil de risque de grève de 80 % du jeu.

**Augmenter le financement ne peut pas résoudre le problème.**

Le Ministère maintient le financement à 120 %, mais l’hôpital reste exposé à un risque de grève.

Si la pénurie se poursuit pendant 30 jours simulés, le Ministère peut émettre un avis vous demandant de construire un autre hôpital. Une grève en cours est plutôt gérée par les alertes de grève du jeu lui-même.

<a id="education-healthcare-limits"></a>
### 7. Limites connues

Le Ministère automatise les décisions de financement, mais il ne peut pas contrôler tous les aspects de SimCity 4.

**Capacité et couverture des bâtiments**

- **Il ne peut pas construire de nouveaux équipements.** Si une école ou un hôpital est surchargé même au financement maximal, vous devez construire un autre bâtiment.
- **Il ne peut pas modifier les zones de couverture.** Le financement des bus scolaires et des ambulances, et donc les décisions de couverture qui y sont liées, reste sous votre contrôle.
- **Il utilise la fréquentation indiquée pour chaque bâtiment.** Le jeu peut parfois calculer le risque de grève à partir de la demande de toute la zone de couverture, qui peut être supérieure au nombre d’usagers affiché dans la fenêtre d’information du bâtiment. Le Ministère utilise les informations de couverture du jeu pour empêcher les baisses risquées, mais il peut ne pas détecter immédiatement toutes les hausses de demande cachées.

**Surveillance et délais**

- **L’historique de fréquentation est conservé pour la session de ville en cours.** Quand vous fermez la ville, le Ministère oublie l’historique mensuel de fréquentation et les ajustements de capacité qu’il avait appris. Les lycées et facultés retrouvent donc leur protection saisonnière initiale après la réouverture.
- **Les informations de couverture du jeu peuvent être retardées.** Certains chiffres de couverture ne sont mis à jour qu’environ une fois par mois simulé. Le Ministère utilise la fréquentation et la capacité actuelles pour planifier le financement, tout en vérifiant aussi les informations de couverture du jeu avant d’autoriser des baisses.
- **Les vérifications urgentes ne sont pas continues.** Le Ministère recherche normalement les problèmes urgents tous les deux jours simulés, ou chaque jour pendant les grèves. Une hausse soudaine de la demande peut donc encore provoquer une grève avant la vérification suivante.
- **Les économies sont examinées chaque mois.** Après une hausse temporaire de fréquentation, le financement reste au niveau supérieur jusqu’à l’examen mensuel suivant.
- **De petits changements de financement peuvent survenir d’un mois à l’autre.** Si la fréquentation fluctue près d’un seuil de financement, le Ministère peut augmenter ou diminuer le budget d’un point de pourcentage lors d’examens mensuels successifs.

**Compatibilité des bâtiments**

Le Ministère gère les bâtiments qui utilisent des mécanismes de financement reconnaissables et cohérents.

Il ne gère pas automatiquement :

- Les écoles privées sans budget de fonctionnement propre ajustable.
- Les bâtiments personnalisés qui utilisent des fenêtres d’information ou des systèmes de financement non pris en charge.
- Les bâtiments dont les relevés de financement et de budget sont en désaccord.
- Les bâtiments dont la capacité ne peut pas être prédite de façon fiable à partir de leur financement.

Les bâtiments qui ne peuvent pas être gérés en toute sécurité sont laissés tels quels.

**Avis des conseillers**

- Les avis de pénurie couvrent huit types de bâtiments d’éducation et de santé.
- Chaque avis peut lister jusqu’à **10 bâtiments concernés**.
- Les bâtiments actuellement en grève sont gérés par les alertes de grève d’origine du jeu plutôt que par les avis de pénurie du Ministère.
- Le Ministère mémorise les avis pour chaque ville identifiable. Si une ville ne peut pas être identifiée de façon fiable, le même avertissement peut réapparaître après la réouverture.

**Conseils d’origine du jeu**

Le plugin inclut un fichier de données qui supprime certains avertissements de financement trompeurs du jeu d’origine.

Cet ajustement est indépendant des interrupteurs de gestion de l’éducation et de la santé. Tant que le fichier est installé, ces avertissements d’origine restent supprimés même si vous désactivez la gestion automatique du Ministère.

Consultez les [Conditions de sécurité](#education-healthcare-safety) pour plus de détails.

[Retour au sommaire](index.md#contents) | [Retour au README](../../README.fr.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
