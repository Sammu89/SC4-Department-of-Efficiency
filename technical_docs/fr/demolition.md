[English](../en/demolition.md) | [Português](../pt/demolition.md) | **Français** | [Español](../es/demolition.md)

[Retour au sommaire](index.md#contents) | [Retour au README](../../README.fr.md)

<a id="demolition"></a>
## F. Démolition automatique

Le Ministère peut déblayer automatiquement certains bâtiments abandonnés et certaines ruines, ce qui permet au terrain zoné de votre ville de se développer à nouveau.

Il traite deux situations :

- **Les ruines :** les bâtiments détruits par des incendies ou des catastrophes qui restent sur leurs parcelles.
- **Les bâtiments abandonnés :** les bâtiments vides depuis suffisamment longtemps.

Le Ministère vérifie les parcelles éligibles **une fois par mois simulé**.

Vous contrôlez :

- Combien de temps un bâtiment doit rester abandonné ou détruit avant d’être démoli.
- Quels types de zones peuvent être inclus.
- Quels motifs d’abandon sont éligibles.

**La démolition n’est jamais gratuite.**

Le Ministère utilise la commande de bulldozer normale de SimCity 4 et paie le même prix que vous paieriez en démolissant la parcelle à la main.

Avant chaque démolition, il demande au jeu le coût réel et vérifie que la trésorerie de votre ville peut le couvrir.

**Les bâtiments municipaux et les bâtiments protégés au titre du patrimoine historique ne sont jamais démolis automatiquement.**

L’objectif est de déblayer les parcelles à l’abandon sans détruire de bâtiments importants ni dépenser plus d’argent que votre ville n’en a de disponible.

<a id="demolition-vanilla"></a>
### 1. Comment fonctionne la démolition automatique avec SimCity 4

#### Comment le jeu suit l’état des bâtiments

SimCity 4 suit un état pour chaque parcelle.

Il enregistre aussi **combien de jours simulés se sont écoulés depuis le dernier changement de cet état**.

Le Ministère utilise ces valeurs existantes pour décider si une parcelle est éligible à la démolition.

| État du jeu | Ce que vous voyez | Action du Ministère |
|---|---|---|
| **Occupé** | Un bâtiment en service | Jamais démoli |
| **En construction** | Un bâtiment en cours de construction | Jamais démoli |
| **Prêt / Créé** | Une parcelle en attente de développement ou un bâtiment sur le point d’être disponible | Jamais démoli |
| **Vacant** | Un bâtiment abandonné | Peut être démoli après le délai d’attente choisi |
| **En démolition** | Un bâtiment déjà en cours de démolition | Ignoré |
| **Détruit** | Des ruines laissées par un incendie ou une catastrophe | Peut être démoli après le délai d’attente choisi |

**Seules les parcelles Vacant et Détruit sont prises en compte pour la démolition automatique.**

Tous les autres états sont exclus.

#### Comment le délai d’attente est-il mesuré ?

Le Ministère utilise **le compteur de jours propre au jeu**, pas un minuteur distinct.

Par exemple, imaginons qu’un bâtiment soit abandonné depuis 200 jours simulés.

Si votre délai d’attente configuré est de 180 jours, il est déjà assez ancien pour être pris en compte lors de la prochaine évaluation mensuelle de démolition, à condition que toutes les autres conditions soient remplies.

Le compteur est enregistré avec la ville.

**Cela signifie que le Ministère reconnaît aussi les bâtiments qui étaient abandonnés avant l’installation du mod.**

Vous n’avez pas besoin d’attendre que la période d’abandon recommence.

#### Pourquoi un bâtiment a-t-il été abandonné ?

SimCity 4 enregistre la raison pour laquelle un bâtiment est devenu abandonné.

Le Ministère lit ce motif et utilise les options que vous avez choisies pour décider si la parcelle doit être éligible.

| Motif d’abandon du jeu | Ce que cela signifie | Option qui le contrôle |
|---|---|---|
| **Faible attractivité** | Le quartier est peu attrayant à cause de facteurs comme la pollution, la criminalité ou des services insuffisants. | **Motif : quartier peu attractif** |
| **Temps de trajet** | Les occupants ne peuvent pas rejoindre leur travail dans un temps de trajet acceptable. | **Motif : trajet trop long** |
| **Faible demande** | La demande est insuffisante pour ce type de bâtiment. | **Motif : pas de demande** |
| **Pas d’électricité** | Le bâtiment a été abandonné faute d’électricité. | **Motif : pas d’électricité** |
| **Pas d’eau** | Le bâtiment a été abandonné faute d’eau. | **Motif : pas d’eau** |
| **Occupants expulsés** | Le jeu indique que les occupants ont été évincés. | Toujours éligible selon le motif ; aucune option distincte |
| **Aucun / inconnu** | Le jeu n’indique aucun motif d’abandon reconnaissable. | Toujours éligible selon le motif ; aucune option distincte |

**L’éligibilité selon le motif ne signifie pas démolition immédiate.** Le délai d’attente, les restrictions de zone, la protection historique et les autres contrôles de sécurité s’appliquent toujours.

#### Pourquoi « Pas d’électricité » et « Pas d’eau » sont-ils désactivés par défaut ?

Un bâtiment abandonné faute d’électricité ou d’eau peut être de nouveau occupé une fois le service rétabli.

Par exemple :

- Un immeuble d’appartements perd le courant et est abandonné.
- Vous réparez le raccordement électrique.
- Le bâtiment peut être de nouveau occupé sans avoir été démoli.

Le démolir trop tôt serait inutile.

**C’est pourquoi l’abandon causé par le manque d’électricité ou d’eau est exclu par défaut.**

Vous pouvez activer ces motifs si vous voulez que le Ministère déblaie ces bâtiments après le délai d’attente choisi.

#### Quels types de zones sont éligibles ?

Le Ministère distingue le développement zoné ordinaire des parcelles municipales protégées ou à usage spécial.

| Groupe de zones | Types de zones inclus |
|---|---|
| **Résidentielle** | Zones résidentielles à faible, moyenne et haute densité |
| **Commerciale** | Zones commerciales à faible, moyenne et haute densité |
| **Industrielle** | Zones industrielles à moyenne et haute densité |
| **Agricole** | Fermes et champs agricoles |
| **Municipale / protégée** | Bâtiments posés à la main, parcelles à usage spécial et parcelles non zonées |

Le groupe municipal et protégé comprend :

- Les écoles et les universités.
- Les hôpitaux.
- Les commissariats de police et les casernes de pompiers.
- Les parcs et les monuments.
- Les bâtiments de récompense.
- Les services publics (utilités).
- Les installations militaires.
- Les aéroports, les ports maritimes et les ports spatiaux.
- Les décharges.
- Les parcelles sans zonage.

**Le Ministère ne démolit pas automatiquement les parcelles municipales ou protégées.**

Ces bâtiments représentent des décisions délibérées du joueur ou des infrastructures à usage spécial, et non un développement ordinaire à l’abandon.

#### Et les fermes ?

Les parcelles agricoles demandent une attention particulière.

Les champs agricoles peuvent sembler vacants dans le cadre de leur fonctionnement normal, même quand la ferme elle-même fonctionne.

**Une parcelle agricole d’apparence vacante ne prouve pas automatiquement qu’une ferme doit être supprimée.**

Le Ministère distingue donc le zonage agricole du développement résidentiel, commercial et industriel ordinaire, ce qui permet de le contrôler séparément.

#### Les bâtiments historiques sont toujours protégés

SimCity 4 vous permet de désigner certains bâtiments comme historiques.

Le jeu enregistre cette désignation au moyen d’un indicateur historique.

**Un bâtiment désigné comme historique est protégé contre la démolition automatique**, même s’il devient abandonné et a dépassé le délai d’attente configuré.

Le Ministère respecte cette décision.

#### Comment fonctionne le bulldozer ?

Lorsque vous sélectionnez manuellement l’outil bulldozer dans SimCity 4, le jeu calcule le prix de démolition de la parcelle.

Il vérifie aussi si la parcelle peut réellement être démolie dans son état actuel.

Le Ministère utilise les mêmes fonctions sous-jacentes du jeu.

**Avant de démolir une parcelle, il obtient le coût de bulldozer propre au jeu.**

Il ne suppose pas que tous les bâtiments ont le même prix de démolition.

Si le jeu indique qu’une parcelle ne peut pas être démolie pour le moment, le Ministère l’ignore.

**Que devient le terrain ensuite ?**

Démolir un bâtiment ne supprime pas son zonage.

Par exemple :

- Un bâtiment résidentiel démoli laisse derrière lui un zonage résidentiel.
- La ville peut ensuite développer un autre bâtiment résidentiel sur cette parcelle.

Il en va de même pour les bâtiments abandonnés éligibles et les ruines.

**Le Ministère déblaie le bâtiment, pas la zone.**

Les ruines laissées par les incendies ou les catastrophes restent sur leurs parcelles tant que quelque chose ne les démolit pas, que ce soit manuellement ou grâce à la démolition automatique du Ministère.

<a id="demolition-problem"></a>
### 2. Le problème de gestion

Dans les grandes villes, les bâtiments abandonnés et les ruines peuvent devenir difficiles à gérer à la main.

Plusieurs problèmes se posent.

**1. Les ruines empêchent un nouveau développement**

Les bâtiments détruits par des incendies ou des catastrophes peuvent rester sur le terrain zoné.

Tant qu’ils ne sont pas démolis, ce terrain ne peut pas simplement accueillir un bâtiment de remplacement.

Dans une grande ville, des ruines dispersées peuvent facilement passer inaperçues.

**2. L’abandon prolongé bloque le redéveloppement**

Un bâtiment abandonné à cause d’une faible demande, d’une mauvaise attractivité ou d’un temps de trajet excessif peut rester vide pendant longtemps.

Déblayer la parcelle peut permettre au jeu de développer un autre bâtiment lorsque les conditions s’améliorent.

**3. Tous les bâtiments en apparence vacants ne doivent pas être démolis**

Certains bâtiments peuvent se rétablir naturellement.

Un bâtiment abandonné faute d’électricité ou d’eau peut être de nouveau occupé lorsque le service revient.

Les champs agricoles peuvent normalement sembler vacants.

Les bâtiments municipaux et les bâtiments historiques doivent aussi être protégés.

**4. La démolition coûte de l’argent**

Chaque opération de bulldozer a un prix déterminé par le jeu.

Démolir de nombreux bâtiments au cours du même mois pourrait consommer une part importante de la trésorerie d’une petite ville.

**Le Ministère doit donc distinguer les parcelles réellement éligibles à la démolition de celles qu’il faut laisser tranquilles, tout en respectant vos réglages et l’argent disponible de la ville.**
<a id="demolition-algorithm"></a>
### 3. Comment le Ministère gère la démolition

Le Ministère vérifie votre ville **une fois par mois simulé** à la recherche de ruines et de bâtiments abandonnés qui répondent à vos réglages de démolition.

Il ne démolit rien immédiatement à l’ouverture d’une ville. La première évaluation de démolition a lieu au **premier changement de mois simulé après le chargement**.

#### Comment se déroule l’évaluation mensuelle de démolition ?

À chaque évaluation mensuelle, le Ministère suit ces étapes :

1. **Trouver les parcelles éligibles.** Il parcourt la ville à la recherche de bâtiments détruits ou abandonnés et vérifie leur ancienneté, leur zonage, leur motif d’abandon et leur statut de protection.
2. **Décider quelles parcelles sont retenues.** Il applique vos réglages de démolition et les règles d’éligibilité propres au jeu.
3. **Trier les candidates.** Les ruines viennent en premier, suivies des bâtiments abandonnés. Dans chaque groupe, les parcelles les plus anciennes passent d’abord.
4. **Vérifier les limites.** Il respecte le nombre maximal d’ordres de démolition, la trésorerie de la ville et tout budget mensuel de démolition que vous avez configuré.
5. **Revérifier chaque parcelle.** Avant d’envoyer un ordre, il vérifie que la parcelle n’a pas changé et demande au jeu son prix de bulldozer réel.
6. **Envoyer l’ordre de démolition.** Il utilise l’opération de bulldozer normale de SimCity 4, sans supprimer le zonage sous-jacent.
7. **Vérifier le résultat.** Lors d’un balayage ultérieur, il confirme que le bâtiment a disparu et que le terrain reste zoné.

**Le Ministère ne démolit jamais une parcelle simplement parce qu’elle paraissait éligible lors d’un balayage précédent.**

Il revérifie juste avant d’émettre l’ordre.

#### Quels bâtiments sont démolis en premier ?

L’ordre de priorité est le suivant :

1. **Les bâtiments détruits (ruines)**, les plus anciens d’abord.
2. **Les bâtiments abandonnés**, les plus anciens d’abord.

Cela signifie qu’une ruine éligible récente est traitée avant un bâtiment abandonné plus ancien.

Le Ministère peut normalement émettre jusqu’à **150 ordres de démolition lors d’une évaluation mensuelle**.

Toutefois, une précaution supplémentaire s’applique au début d’une session de ville.

**Tant que le jeu n’a pas confirmé au moins un ordre de démolition dans cette session, le Ministère envoie au plus trois ordres par évaluation mensuelle.**

Cela lui permet de vérifier que le mécanisme de démolition du jeu fonctionne correctement avant de traiter un plus grand nombre de bâtiments.

#### Que se passe-t-il entre les évaluations mensuelles ?

Tous les **10 jours simulés**, le Ministère effectue une vérification légère de la démolition.

Cette vérification :

- Confirme si les ordres de démolition précédents ont été exécutés.
- Enregistre les changements dans les parcelles surveillées.
- Recherche les résultats inattendus.

**La vérification tous les 10 jours n’émet jamais de nouveaux ordres de démolition.**

Les nouvelles démolitions ne sont réalisées que lors des évaluations mensuelles.

#### Comment le Ministère décide-t-il si une parcelle est retenue ?

Les règles suivantes sont vérifiées dans l’ordre.

| Condition | Décision |
|---|---|
| Le jeu est déjà en train de démolir la parcelle | L’ignorer |
| La parcelle n’est ni abandonnée ni détruite | Non éligible |
| La parcelle est historique et ne relève pas de l’exception spéciale des ruines | La conserver |
| La démolition automatique est désactivée pour ce type de parcelle | La conserver |
| La parcelle est municipale et ne relève pas de l’exception spéciale des ruines | La conserver |
| Un bâtiment abandonné se trouve dans un type de zone que vous avez exclu | Le conserver |
| Un bâtiment abandonné a un motif d’abandon exclu | Le conserver |
| La parcelle n’a pas atteint le délai d’attente configuré | La conserver |
| Le jeu indique que la parcelle ne peut pas être démolie | La conserver |
| Aucune des conditions ci-dessus ne s’applique | **Éligible à la démolition** |

**Une distinction importante :**

Les options de zone et de motif d’abandon s’appliquent uniquement aux **bâtiments abandonnés**.

Elles ne filtrent pas les ruines.

Par exemple, si **Démolir les bâtiments détruits** est activé, les ruines d’un bâtiment résidentiel, commercial, industriel ou agricole peuvent être déblayées après le délai d’attente, même lorsque ce type de zone est exclu de la démolition des bâtiments abandonnés.

Les ruines municipales et historiques ont leur propre réglage de protection.

<a id="demolition-destroyed"></a>
#### Bâtiments détruits : déblayer les ruines

Lorsque **Démolir les bâtiments détruits** est activé, le Ministère peut supprimer automatiquement les ruines laissées par les incendies et les catastrophes.

Par défaut, il attend **un mois simulé**, soit 30 jours.

Par exemple :

- Un bâtiment commercial est détruit par un incendie.
- Le jeu marque sa parcelle comme Détruit.
- Le Ministère attend qu’au moins 30 jours simulés se soient écoulés.
- Lors de l’évaluation mensuelle de démolition suivante, les ruines deviennent éligibles.
- Si le jeu autorise la démolition et que la ville peut se le permettre, le Ministère les supprime.

**Le zonage commercial sous-jacent reste en place**, ce qui permet un nouveau développement par la suite.

Les ruines situées sur des terrains résidentiels, commerciaux, industriels et agricoles ordinaires suivent cette règle.

**Les ruines municipales et historiques sont protégées par défaut.**

Elles ne peuvent être incluses que si vous activez explicitement **Démolir les bâtiments municipaux et historiques détruits**.

<a id="demolition-abandoned"></a>
#### Bâtiments abandonnés

Lorsque **Démolir les bâtiments abandonnés** est activé, le Ministère prend en compte les bâtiments qui sont restés abandonnés suffisamment longtemps.

Par défaut, il attend **12 mois simulés**, soit 365 jours.

Un bâtiment abandonné doit remplir toutes ces conditions :

- Il appartient à un type de zone activé.
- Son motif d’abandon est activé, ou il fait partie des motifs toujours éligibles.
- Il est abandonné depuis au moins le délai d’attente configuré.
- Il n’est ni municipal ni protégé au titre du patrimoine historique.
- SimCity 4 confirme que la parcelle peut être démolie.

**Comportement par défaut**

Le Ministère peut démolir les bâtiments résidentiels, commerciaux et industriels abandonnés lorsque leur abandon est causé par :

- Un quartier peu attractif.
- Un temps de trajet excessif.
- Une demande insuffisante.

L’abandon causé par le manque d’électricité ou d’eau est exclu par défaut.

C’est voulu : un bâtiment abandonné à cause d’un problème de service temporaire peut être de nouveau occupé lorsque ce service est rétabli.

<a id="demolition-delays"></a>
#### Comment fonctionnent les délais d’attente ?

La fenêtre d’options vous permet de choisir les délais d’attente en **mois simulés**.

En interne, cependant, SimCity 4 enregistre depuis combien de temps un bâtiment est dans son état actuel en **jours simulés**.

Le Ministère convertit donc le nombre de mois sélectionné en jours.

**Conversion du délai d’attente**

```math
D=\mathrm{round}\left(\frac{M\times365}{12}\right)
```

Où :

- **M** est le délai d’attente sélectionné, en mois simulés.
- **D** est le délai d’attente enregistré et utilisé par le Ministère, en jours simulés.

Le résultat est arrondi au jour entier le plus proche, les valeurs d’un demi-jour exact étant arrondies vers le haut.

**Exemples**

| Mois sélectionnés | Jours utilisés |
|---:|---:|
| 1 | 30 |
| 2 | 61 |
| 3 | 91 |
| 6 | 183 |
| 12 | 365 |
| 24 | 730 |
| 36 | 1 095 |
| 60 | 1 825 |

#### Délais d’attente disponibles

| Type de bâtiment | Plage dans le jeu | Par défaut |
|---|---|---|
| **Ruines** | 1 à 24 mois | 1 mois (30 jours) |
| **Bâtiments abandonnés** | 1 à 60 mois | 12 mois (365 jours) |

Si vous modifiez manuellement le fichier de configuration, les plages enregistrées autorisées sont plus larges :

- **Ruines :** 1 à 3 650 jours.
- **Bâtiments abandonnés :** 1 à 7 300 jours.

#### Comment le Ministère sait-il qu’une parcelle est assez ancienne ?

Il utilise le compteur propre au jeu.

**Condition d’ancienneté pour la démolition**

```math
A\ge D
```

Où :

- **A** est le nombre de jours simulés écoulés depuis que la parcelle est devenue abandonnée ou détruite.
- **D** est le délai d’attente configuré, en jours.

Par exemple, si le délai d’attente est de 91 jours :

- À 85 jours d’âge, la parcelle est trop récente.
- À 91 jours d’âge, elle devient éligible.
- Elle est effectivement traitée à la première évaluation mensuelle suivant l’atteinte de cet âge.

**La démolition peut donc intervenir jusqu’à environ un mois simulé après que le délai d’attente est atteint.**

#### Et si je modifie le délai d’attente directement dans le fichier de configuration ?

La fenêtre d’options reconvertit les jours enregistrés en un nombre approximatif de mois.

```math
M_{\mathrm{affiché}}=
\min\left(
M_{\max},
\max\left(
1,\mathrm{round}\left(\frac{D\times12}{365}\right)
\right)
\right)
```

Ici, **Mmax** vaut 24 mois pour les ruines et 60 mois pour les bâtiments abandonnés.

Par exemple, un délai configuré manuellement à **45 jours** apparaît comme environ **1 mois** dans la fenêtre d’options.

Cependant, la valeur sous-jacente de 45 jours est conservée tant que vous ne modifiez pas réellement le contrôle du délai d’attente et n’acceptez pas le changement.

**Le simple fait d’ouvrir la fenêtre d’options ne réécrit pas votre délai configuré manuellement.**

<a id="demolition-zones"></a>
#### Quelles zones le Ministère peut-il démolir ?

Le Ministère propose quatre options de zone :

| Option de zone | Par défaut |
|---|---|
| **Zone résidentielle** | Activé |
| **Zone commerciale** | Activé |
| **Zone industrielle** | Activé |
| **Zone agricole** | Désactivé |

Ces options déterminent quels **bâtiments abandonnés** sont éligibles.

Par exemple, désactiver Zone commerciale empêche le Ministère de démolir les bâtiments commerciaux abandonnés.

**Ces restrictions ne s’appliquent pas aux ruines.**

Si la démolition automatique des ruines est activée, les bâtiments détruits situés sur des zones constructibles peuvent tout de même être déblayés, quelles que soient les options de zone ci-dessus.

Les parcelles municipales restent protégées indépendamment de ces réglages.

<a id="demolition-reasons"></a>
#### Quels motifs d’abandon sont éligibles ?

Vous pouvez aussi choisir quels motifs d’abandon autorisent la démolition automatique.

| Motif d’abandon | Par défaut |
|---|---|
| **Motif : quartier peu attractif** | Activé |
| **Motif : trajet trop long** | Activé |
| **Motif : pas de demande** | Activé |
| **Motif : pas d’électricité** | Désactivé |
| **Motif : pas d’eau** | Désactivé |

**Pourquoi « Pas d’électricité » et « Pas d’eau » sont-ils désactivés ?**

Parce que les bâtiments abandonnés faute de services peuvent être de nouveau occupés.

Par exemple, si un bâtiment résidentiel perd l’électricité, le Ministère le laisse normalement tranquille pendant que vous rétablissez l’alimentation électrique.

Vous pouvez activer ces motifs si vous préférez démolir ces bâtiments après le délai d’attente.

#### Motifs toujours éligibles

Deux situations signalées par le jeu n’ont pas d’options distinctes :

- **Occupants expulsés.**
- **Aucun motif, ou motif d’abandon non reconnu.**

Ces motifs sont toujours considérés comme éligibles lorsque la démolition automatique des bâtiments abandonnés est activée.

Le délai d’attente normal, la sélection des zones, les règles de protection des bâtiments et les contrôles financiers continuent de s’appliquer.

<a id="demolition-protected"></a>
#### Bâtiments municipaux et historiques

Le Ministère protège les bâtiments qui représentent des choix délibérés du joueur.

**Bâtiments municipaux**

Ce groupe comprend :

- Les écoles, les hôpitaux, les commissariats de police et les casernes de pompiers.
- Les parcs, les monuments, les récompenses et les autres bâtiments posés à la main.
- Les services publics (utilités).
- Les installations militaires.
- Les aéroports, les ports maritimes et les ports spatiaux.
- Les décharges.
- Les parcelles sans zonage ordinaire.

**Bâtiments historiques**

Toute parcelle portant la désignation historique de SimCity 4 est également protégée.

**Les bâtiments municipaux et historiques, debout ou abandonnés, ne sont jamais démolis automatiquement.**

Cette protection s’applique quels que soient vos réglages d’abandon.

#### Et leurs ruines ?

Il existe une exception facultative.

**Démolir les bâtiments municipaux et historiques détruits** est **désactivé par défaut**.

Lorsqu’il est désactivé, les ruines des bâtiments municipaux et historiques restent intactes.

Lorsqu’il est activé, le Ministère peut déblayer ces ruines après le délai d’attente normal des ruines, à condition que :

- La parcelle soit réellement dans l’état Détruit.
- Le contrôle de démolition du jeu l’accepte.
- Les règles de trésorerie et de budget permettent la démolition.

**Cette option n’autorise jamais la démolition d’un bâtiment municipal ou historique encore debout.**

Elle ne s’applique qu’une fois que le bâtiment a déjà été détruit.

<a id="demolition-farms"></a>
#### Précautions particulières pour les parcelles agricoles

Les parcelles agricoles se comportent différemment des bâtiments résidentiels, commerciaux et industriels ordinaires.

Dans SimCity 4, certaines parcelles de champs agricoles peuvent apparaître dans l’état abandonné du jeu dans le cadre de leur comportement normal.

Elles peuvent aussi être enregistrées comme manquant d’eau.

**Cela ne signifie pas forcément que la ferme ne fonctionne plus.**

Pour cette raison, **Zone agricole est désactivée par défaut**.

Avec cette option désactivée, les fermes et les champs agricoles abandonnés sont préservés.

Si vous l’activez, le Ministère peut démolir les parcelles agricoles abandonnées éligibles, y compris des champs que vous pourriez vouloir conserver.

**Utilisez cette option avec précaution.**

Les ruines sur les terrains agricoles sont traitées séparément. Elles peuvent toujours être démolies lorsque **Démolir les bâtiments détruits** est activé, même si Zone agricole est désactivée.

#### Icônes d’avertissement d’eau agricoles

Le Ministère masque aussi l’icône d’avertissement « pas d’eau » du jeu au-dessus des parcelles agricoles sur la version du jeu prise en charge.

Cet ajustement visuel est indépendant de la démolition automatique et ne modifie pas le service d’eau réel.

Voir [Eau](water.md#water-farm-icon).

<a id="demolition-money"></a>
#### Comment le Ministère paie-t-il les démolitions ?

**Chaque démolition utilise le prix de bulldozer réel indiqué par SimCity 4.**

Le Ministère n’estime pas le prix d’après le type de bâtiment.

Avant chaque ordre de démolition, il demande au jeu le coût de cette parcelle précise.

Si le jeu ne peut pas fournir de prix valide, **la parcelle n’est pas démolie**.

Deux protections financières s’appliquent.

#### 1. La protection de la trésorerie

Le Ministère n’ordonne jamais de démolitions dont le coût cumulé dépasse la trésorerie disponible de la ville au début de l’évaluation mensuelle.

**Règle de trésorerie**

```math
c_i\le T-S_i
```

Où :

- **cᵢ** est le prix de bulldozer de la parcelle en cours.
- **T** est la trésorerie au début de l’évaluation de démolition.
- **Sᵢ** est le prix cumulé des ordres de démolition déjà envoyés pendant cette évaluation.

**Pourquoi le Ministère soustrait-il les ordres précédents ?**

Parce que SimCity 4 ne déduit pas nécessairement les frais de bulldozer immédiatement lorsqu’un ordre de démolition est soumis.

Sans cette protection, le Ministère pourrait émettre plusieurs ordres contre la même somme d’argent disponible.

Par exemple, si votre trésorerie contient 500 § et que le Ministère a déjà ordonné 300 § de démolitions, il ne considère que 200 § comme disponibles pour d’autres ordres.

**Que se passe-t-il lorsqu’une parcelle est trop chère ?**

Le Ministère ignore cette parcelle et passe à la candidate suivante.

Un bâtiment moins cher peut encore être abordable.

**Si la trésorerie est négative, aucune démolition n’est ordonnée, pas même une dont le prix est de 0 §.**

Cette protection de la trésorerie est toujours active, même si vous désactivez le budget mensuel de démolition facultatif.

#### 2. Le budget mensuel de démolition facultatif

Vous pouvez aussi limiter ce que le Ministère peut dépenser lors d’une seule évaluation mensuelle de démolition.

L’option est :

**Limiter le budget mensuel de démolition**

Elle est **désactivée par défaut**.

Lorsqu’elle est activée, le Ministère applique une règle supplémentaire.

**Règle du budget mensuel**

```math
c_i=0
\quad\text{ou}\quad
S_i+c_i\le B
```

Où **B** est le budget mensuel de démolition que vous avez configuré.

La valeur par défaut du budget est de **1 000 §**.

**Une démolition dont le prix est de 0 § ne compte pas dans ce budget facultatif.**

Il existe toutefois une différence importante entre cette règle et la protection de la trésorerie.

**Si une démolition payante dépassait le budget mensuel, toute l’évaluation de démolition s’arrête.**

Elle n’ignore pas ce bâtiment pour continuer à chercher des bâtiments moins chers.

Toutes les candidates restantes attendent le mois suivant, y compris les parcelles ultérieures qui auraient été gratuites à démolir.

#### 3. Nombre maximal d’ordres de démolition

Le Ministère limite aussi le nombre d’ordres de démolition lors d’une évaluation mensuelle.

```math
N\le
\begin{cases}
3, & \text{avant qu’un ordre ait été confirmé dans la session}\\
150, & \text{après confirmation}
\end{cases}
```

La limite normale de 150 ordres est une valeur technique par défaut.

Elle peut être ajustée grâce à la clé de configuration :

`MaxPerRun`

Plage autorisée : **1 à 1 000 ordres**.

Ce réglage n’est pas disponible dans la fenêtre d’options du jeu.

La trésorerie et le budget facultatif imposent souvent des limites pratiques plus basses.

<a id="demolition-never"></a>
#### Dans quels cas le Ministère ne démolit-il jamais une parcelle ?

Le Ministère laisse une parcelle intacte dès que l’un des cas suivants s’applique :

- Le Ministère est désactivé, la fonction de démolition concernée est désactivée, ou la configuration de démolition est absente ou invalide.
- La ville vient d’être chargée et la première évaluation mensuelle de démolition n’a pas encore eu lieu.
- Le bâtiment est occupé, en construction, en attente de développement ou déjà en cours de démolition.
- C’est un bâtiment municipal ou historique qui n’a pas été détruit.
- C’est une ruine municipale ou historique protégée et l’option spéciale des ruines est désactivée.
- Il est abandonné dans un type de zone que vous avez exclu.
- Il est abandonné pour un motif que vous avez exclu.
- Il n’a pas atteint le délai d’attente configuré.
- SimCity 4 indique que la parcelle ne peut pas être démolie.
- Le jeu ne peut pas fournir de prix de démolition valide.
- La trésorerie ne peut pas couvrir la démolition.
- Le budget mensuel de démolition facultatif empêche l’ordre.
- La parcelle a changé ou les limites de sa zone sur la carte ne peuvent pas être vérifiées.
- Une défaillance de sécurité de la démolition a arrêté la gestion pour la session en cours.
- La version du jeu n’est pas prise en charge.

**En cas de doute sur l’éligibilité d’une parcelle ou sur la sécurité de l’opération, le Ministère la laisse tranquille.**

<a id="demolition-safety"></a>
### 4. Conditions de sécurité

La démolition automatique est une opération potentiellement destructrice ; le Ministère effectue donc plusieurs vérifications avant et après chaque ordre.

#### Utilisation du système de bulldozer propre au jeu

Le Ministère utilise la commande de démolition normale de SimCity 4.

Il envoie le rectangle exact occupé par la parcelle éligible et n’invente pas de paramètres de démolition spéciaux.

**Il ne supprime pas directement les bâtiments des données internes du jeu.**

#### Vérification de la parcelle juste avant la démolition

Avant chaque ordre, le Ministère relit la parcelle.

Elle doit toujours :

- Exister à la position attendue.
- Être dans l’état attendu.
- Respecter les règles de démolition.
- Avoir les dimensions attendues.

Le rectangle sur la carte doit avoir exactement la superficie de la parcelle et contenir sa case d’origine.

Si ces conditions ne sont pas remplies, la parcelle est ignorée.

#### S’assurer que la vérification du prix ne modifie rien

Le Ministère demande au jeu le prix d’aperçu du bulldozer.

Cette demande ne doit pas modifier la parcelle.

Si la parcelle change de façon inattendue pendant la vérification du prix, **la démolition automatique s’arrête pour le reste de la session de la ville**.

#### Vérifier que la démolition a bien eu lieu

Les ordres de démolition ne sont pas forcément exécutés au moment exact où ils sont envoyés.

Le Ministère vérifie donc le résultat lors du prochain balayage de 10 jours ou de la prochaine évaluation mensuelle.

La parcelle doit avoir disparu.

Si **trois ordres de démolition consécutifs ne sont pas exécutés par le jeu**, la démolition automatique s’arrête pour le reste de la session de la ville.

#### Préserver le zonage

Après avoir confirmé que le bâtiment a disparu, le Ministère vérifie le zonage de la case d’origine de la parcelle.

**La zone d’origine doit toujours exister.**

Si la démolition supprime de façon inattendue le zonage, la démolition automatique s’arrête pour le reste de la session.

Cela protège l’agencement de ville voulu par le joueur.

#### Gestion des erreurs de configuration et des erreurs techniques

Si le fichier de configuration de la démolition est absent, illisible ou invalide, le Ministère n’effectue aucune démolition automatique.

Les erreurs survenant pendant une évaluation de démolition sont interceptées et consignées dans le journal. Elles n’arrêtent pas SimCity 4 lui-même.

**La démolition fonctionne indépendamment des autres fonctions du Ministère.**

Un arrêt de sécurité de la démolition ne désactive ni l’électricité, ni l’eau, ni la gestion des déchets, ni l’éducation, ni la santé, ni la gestion des accords régionaux.

De même, la défaillance d’un autre gestionnaire ne désactive pas automatiquement la démolition.

<a id="demolition-settings"></a>
### 5. Réglages

La démolition automatique se configure sur la page **Démolition** de la fenêtre d’options du Ministère.

Les réglages apparaissent dans l’ordre suivant :

| Réglage | Par défaut | Ce qu’il contrôle |
|---|---|---|
| [Démolir les bâtiments détruits](options.md#opt-demolishdestroyed) | Activé | Déblaie automatiquement les ruines éligibles. |
| [Attendre (ruines)](options.md#opt-destroyedmonths) | 1 mois | Combien de temps les ruines doivent rester avant d’être démolies. |
| [Démolir les bâtiments municipaux et historiques détruits](options.md#opt-demolishmunicipal) | Désactivé | Autorise la démolition des ruines des bâtiments protégés uniquement. |
| [Démolir les bâtiments abandonnés](options.md#opt-demolishabandoned) | Activé | Déblaie automatiquement les bâtiments abandonnés éligibles. |
| [Attendre (abandonnés)](options.md#opt-abandonedmonths) | 12 mois | Combien de temps les bâtiments doivent rester abandonnés. |
| [Zone résidentielle](options.md#opt-zoneresidential) | Activé | Inclut les bâtiments résidentiels abandonnés. |
| [Zone commerciale](options.md#opt-zonecommercial) | Activé | Inclut les bâtiments commerciaux abandonnés. |
| [Zone industrielle](options.md#opt-zoneindustrial) | Activé | Inclut les bâtiments industriels abandonnés. |
| [Zone agricole](options.md#opt-zoneagriculture) | Désactivé | Inclut les fermes et les champs abandonnés. |
| [Motif : quartier peu attractif](options.md#opt-reasonlowdesirability) | Activé | Inclut l’abandon causé par une faible attractivité. |
| [Motif : trajet trop long](options.md#opt-reasoncommute) | Activé | Inclut l’abandon causé par de longs trajets domicile-travail. |
| [Motif : pas de demande](options.md#opt-reasonlowdemand) | Activé | Inclut l’abandon causé par une demande insuffisante. |
| [Motif : pas d’électricité](options.md#opt-reasonnopower) | Désactivé | Inclut l’abandon causé par le manque d’électricité. |
| [Motif : pas d’eau](options.md#opt-reasonnowater) | Désactivé | Inclut l’abandon causé par le manque d’eau. |
| [Limiter le budget mensuel de démolition](options.md#opt-budgetlimit) | Désactivé | Limite les dépenses de démolition lors de chaque évaluation mensuelle. |
| [Budget mensuel](options.md#opt-demolitionbudget) | 1 000 § | Fixe la limite de dépenses lorsque l’option de budget est activée. |

Tous les réglages de démolition automatique dépendent de [Ministère actif](options.md#opt-master).

**Précautions recommandées**

Les réglages par défaut protègent les champs agricoles, les bâtiments abandonnés à cause de pannes temporaires de services, ainsi que les structures municipales ou historiques.

Si vous voulez une démolition plus agressive, passez en revue ces protections avant de modifier les options, surtout dans les villes qui comptent de précieux bâtiments historiques ou de vastes zones agricoles.

<a id="demolition-examples"></a>
### 6. Exemples détaillés

Ces exemples montrent comment le Ministère applique les délais d’attente et les limites financières.

**Tous les prix de démolition ci-dessous sont illustratifs.** Les prix réels sont obtenus auprès de SimCity 4 pour chaque parcelle.

#### Exemple 1 : une ruine atteint son délai d’attente

Vous réglez le délai d’attente des ruines sur **3 mois**.

Le Ministère l’enregistre ainsi :

**91 jours simulés.**

Un bâtiment est détruit par un incendie.

Lors d’une évaluation mensuelle, le jeu indique que les ruines ont **85 jours**.

```math
85<91
```

Le bâtiment est trop récent et il est laissé tranquille.

À l’évaluation mensuelle suivante, environ 30 jours plus tard, les ruines ont environ **115 jours**.

```math
115\ge91
```

La parcelle est désormais éligible.

**Le Ministère la démolit lors de cette évaluation si les contrôles d’éligibilité et les contrôles financiers du jeu le permettent.**

Le délai d’attente est un âge minimal, pas un rendez-vous de démolition exact.

#### Exemple 2 : le budget mensuel arrête une évaluation de démolition

Supposons que votre ville ait :

- **Trésorerie :** 20 000 §.
- **Budget mensuel de démolition :** activé.
- **Limite du budget :** 1 000 §.

Le Ministère a déjà confirmé une démolition réussie dans la session en cours, de sorte que la précaution initiale de trois ordres ne s’applique plus.

Les candidates sont triées avec les ruines en premier, suivies des bâtiments abandonnés.

| Ordre | Candidate | Prix | Déjà dépensé | Résultat budgétaire |
|---|---|---:|---:|---|
| 1 | Ruine A | 40 § | 0 § | Démolir |
| 2 | Ruine B | 0 § | 40 § | Démolir |
| 3 | Abandonné C | 300 § | 40 § | Démolir |
| 4 | Abandonné D | 450 § | 340 § | Démolir |
| 5 | Abandonné E | 700 § | 790 § | **Arrêt : dépasse le budget** |
| 6 | Abandonné F | 0 § | — | Attend le mois suivant |

Après les quatre premiers ordres, le Ministère s’est engagé à hauteur de :

```math
40+0+300+450=790
```

**790 § de dépenses de démolition.**

Le bâtiment suivant coûte 700 §.

```math
790+700=1490>1000
```

Cela dépasserait le budget mensuel configuré.

**Le Ministère arrête toute l’évaluation de démolition.**

Les bâtiments E et F attendent le mois suivant.

Remarquez que F ne coûterait rien à démolir, mais il est tout de même reporté, car la règle du budget arrête l’évaluation à la première candidate payante qui dépasse la limite.

#### Exemple 3 : la règle de trésorerie fait ignorer un bâtiment coûteux

Considérons maintenant une ville plus petite avec :

- **Trésorerie :** 250 §.
- **Budget mensuel de démolition :** désactivé.

Le Ministère évalue :

| Ordre | Candidate | Prix | Disponible avant | Décision |
|---|---|---:|---:|---|
| 1 | Ruine A | 40 § | 250 § | Démolir |
| 2 | Ruine B | 60 § | 210 § | Démolir |
| 3 | Abandonné C | 300 § | 150 § | **Ignorer** |
| 4 | Abandonné D | 100 § | 150 § | Démolir |

Après avoir ordonné A et B, le Ministère s’est engagé à hauteur de 100 §.

Le bâtiment C coûte 300 §, ce que la ville ne peut pas se permettre.

**Le Ministère ignore C mais continue d’examiner les autres candidates.**

Le bâtiment D ne coûte que 100 § et peut encore être démoli.

Les ordres obtenus coûtent :

```math
40+60+100=200
```

**Trois bâtiments sont ordonnés pour un total de 200 §.**

Le bâtiment C reste en place jusqu’à une évaluation ultérieure où l’argent disponible sera suffisant.

**La distinction est importante :**

- **Échec de trésorerie :** ignorer la parcelle trop chère et continuer.
- **Échec du budget mensuel :** arrêter complètement l’évaluation.

#### Exemple 4 : installer le Ministère dans une ville comptant de nombreux bâtiments abandonnés

Imaginons une ville contenant **400 bâtiments** déjà abandonnés depuis plus d’un an à cause d’une demande insuffisante.

Vous installez le Ministère avec ses réglages de démolition par défaut.

Ces bâtiments sont déjà assez anciens pour être retenus.

Toutefois, **rien n’est démoli immédiatement à l’ouverture de la ville**.

Au premier changement de mois simulé, le Ministère envoie au plus **trois ordres de démolition**.

Lors de la vérification suivante, au bout de 10 jours, il confirme que le jeu a exécuté ces ordres.

Dès qu’au moins une démolition a été confirmée, la limite normale devient disponible.

À l’évaluation mensuelle suivante, le Ministère peut ordonner jusqu’à **150 démolitions**, pourvu que la trésorerie et le budget mensuel facultatif les permettent.

Les plus anciens bâtiments éligibles sont traités en premier.

**Cette précaution initiale de trois ordres empêche le Ministère de tenter immédiatement des centaines de démolitions avant d’avoir confirmé que la commande de démolition du jeu fonctionne correctement.**

<a id="demolition-limitations"></a>
### 7. Limites connues

La démolition automatique réduit le travail manuel répétitif, mais plusieurs restrictions sont volontaires.

#### Calendrier et éligibilité

- **La démolition est mensuelle.** Un bâtiment est traité lors de la première revue mensuelle qui suit son délai d’attente, soit potentiellement jusqu’à environ un mois simulé plus tard.
- **L’ancienneté d’abandon existante est conservée.** Les bâtiments abandonnés avant l’installation du Ministère gardent leur ancienneté d’origine. Ils peuvent donc être éligibles dès la première revue mensuelle.
- **Chaque session de ville commence prudemment.** Tant qu’un ordre de démolition n’a pas été confirmé, chaque revue mensuelle envoie au plus trois ordres.
- **Le Ministère se fie au motif d’abandon du jeu.** Il ne prédit pas si un bâtiment pourrait être de nouveau occupé. Le délai d’attente et les options de motif fournissent cette protection.

#### Coûts de démolition

- **C’est le jeu qui détermine les prix de démolition.** Le Ministère lit le prix réel avant chaque ordre, mais n’affiche pas de notification de coût distincte.
- **Les dépenses de démolition apparaissent comme des coûts de bulldozer ordinaires.** Les ruines sont en général moins chères à déblayer que les bâtiments abandonnés, mais les prix réels dépendent de la parcelle.
- **Le budget mensuel s’applique à une seule revue de démolition.** S’il est activé, il arrête la revue au premier candidat chiffré qui dépasserait la limite.

#### Bâtiments protégés

- **Les bâtiments municipaux et historiques abandonnés ne sont jamais démolis.** Seules leurs ruines peuvent être supprimées, et uniquement lorsque l’option de protection distincte est activée.
- **Les champs agricoles peuvent être touchés.** Activer Zone agricole autorise la démolition des parcelles de champs abandonnées éligibles. Utilisez cette option avec prudence.
- **Le Ministère ne supprime jamais volontairement le zonage sous-jacent.** Le terrain reste disponible pour être reconstruit.

#### Limites techniques

- **Les arrêts de sécurité durent pour la session de ville en cours.** Rouvrir la ville permet à la gestion de la démolition de démarrer une nouvelle session.
- **Seule la ville actuellement ouverte est gérée.** Le Ministère n’inspecte ni ne démolit les bâtiments des villes voisines.
- **La démolition automatique exige la version du jeu prise en charge.** Les versions non prises en charge ne sont pas modifiées.

**Le but du Ministère est de déblayer les ruines éligibles et les bâtiments abandonnés depuis longtemps, pas de prendre à la place du joueur les décisions de reconstruction.**

---

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
