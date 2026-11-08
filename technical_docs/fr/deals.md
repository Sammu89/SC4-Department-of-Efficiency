[English](../en/deals.md) | [Português](../pt/deals.md) | **Français** | [Español](../es/deals.md)

[Retour au sommaire](index.md#contents) | [Retour au README](../../README.fr.md)

<a id="neighbor-deals"></a>
## E. Accords avec les voisins

Les adresses de l’exécutable liées à ce chapitre figurent dans la [référence des adresses du jeu](../en/game_addresses.md) (en anglais).

Le Ministère peut gérer automatiquement les **contrats d’électricité, d’eau et de déchets entre votre ville et ses voisines**.

Il décide quand :

- **Créer un contrat** lorsque échanger avec une voisine profiterait à votre ville.
- **Augmenter ou réduire un contrat existant** à mesure que les besoins de votre ville évoluent.
- **Annuler un contrat** lorsqu’il n’est plus utile ou sûr.

L’objectif est de **réduire les dépenses globales de votre ville, de gagner de l’argent grâce aux exportations rentables et de maintenir des services essentiels fiables**.

Le Ministère considère d’abord les besoins propres de votre ville. Il compare ensuite le coût de production ou d’élimination d’une ressource sur place au prix proposé par les villes voisines.

Par exemple, acheter de l’électricité peut coûter moins cher qu’augmenter le financement d’une centrale électrique. À l’inverse, vendre de l’électricité excédentaire peut être rentable si les revenus dépassent le coût de production supplémentaire.

Le même principe s’applique à l’eau et à l’élimination des déchets.

Ce chapitre explique **comment fonctionnent les contrats régionaux et comment le Ministère les gère**.

Pour le détail de la production et du financement locaux, voir [Électricité](power.md#power), [Eau](water.md#water) et [Déchets et décharges](garbage.md#garbage-landfills).

<a id="neighbor-deals-mechanism"></a>
### 1. Comment fonctionnent les accords avec les voisins dans SimCity 4

#### Les six types de contrats régionaux

SimCity 4 prend en charge six types de contrats avec chaque ville voisine : l’achat et la vente d’électricité, d’eau et de services d’élimination des déchets.

Cependant, **les contrats de déchets fonctionnent dans le sens inverse de ceux d’électricité et d’eau**.

Lorsque vous envoyez des déchets à une voisine, vous payez cette ville pour qu’elle les élimine. Lorsque vous acceptez les déchets d’une voisine, c’est cette ville qui vous paie.

| Ressource | Votre ville paie | Votre ville est payée |
|---|---|---|
| **Électricité** | Acheter de l’électricité à une voisine | Vendre de l’électricité à une voisine |
| **Eau** | Acheter de l’eau à une voisine | Vendre de l’eau à une voisine |
| **Déchets** | Envoyer vos déchets à une voisine pour qu’elle les élimine | Accepter les déchets d’une voisine pour les éliminer |

**Qu’est-ce que cela signifie pour votre ville ?**

- **Acheter de l’électricité ou de l’eau** réduit la quantité que votre ville doit produire sur place.
- **Vendre de l’électricité ou de l’eau** exige une capacité de production suffisante pour alimenter votre ville et la ville voisine.
- **Exporter des déchets** réduit la quantité que votre système de gestion des déchets doit traiter, mais coûte de l’argent.
- **Importer des déchets** rapporte de l’argent, mais augmente la quantité que votre système de gestion des déchets doit traiter.

Le jeu autorise **un contrat de chaque type avec chaque ville voisine**.

Les quantités des contrats utilisent les mêmes unités mensuelles que celles affichées dans la fenêtre Accords avec les voisins du jeu. Les prix et les paiements sont exprimés en simoleons (§) par mois simulé.

#### Quelles connexions sont nécessaires ?

Les contrats régionaux exigent des connexions adaptées entre les villes voisines.

La connexion dépend de la ressource :

| Ressource | Connexion requise |
|---|---|
| **Électricité** | Une ligne électrique qui traverse la frontière de la ville |
| **Eau** | Une canalisation d’eau qui traverse la frontière de la ville |
| **Déchets** | Une route ou une liaison de transport adaptée, reconnue par le système de contrats du jeu |

Pour l’électricité et l’eau, la règle est simple : le réseau concerné doit traverser la frontière régionale.

Les connexions pour les déchets sont plus compliquées, car SimCity 4 effectue différentes vérifications internes.

**Qu’a-t-on confirmé au sujet des connexions pour les déchets ?**

La vérification de présence de connexion régionale du jeu reconnaît :

- Les routes.
- Les voies ferrées.

Cette vérification précise ne reconnaît pas les autoroutes comme des connexions pour les déchets.

Cependant, le jeu utilise aussi une recherche distincte lorsqu’il vérifie chaque ville voisine et chaque contrat. Cette recherche examine plusieurs types de connexion, notamment les autoroutes, les avenues, les routes et les chemins de terre.

**Ces deux vérifications ne sont pas identiques.**

La présence d’un type de route particulier dans la seconde recherche ne prouve pas, à elle seule, qu’il peut établir un contrat de déchets éligible.

Le Ministère s’appuie donc sur les vérifications réelles de connexion et d’éligibilité des contrats du jeu, plutôt que de supposer que toute liaison de transport est valide.

**Que se passe-t-il si une connexion est perdue ?**

Pour une vente, la connexion de la ville locale doit rester active.

Pour un achat, la connexion correspondante du côté de la ville voisine doit exister et être active.

Si un contrat perd la connexion dont il a besoin, SimCity 4 peut signaler le problème lors de sa maintenance mensuelle des contrats.

**Un contrat ne peut pas être considéré comme fiable simplement parce qu’il figure encore dans la liste.**

#### Que sait le jeu sur les villes voisines ?

SimCity 4 conserve un petit enregistrement régional pour chaque ville.

Pour chaque ressource, cet enregistrement comprend :

| Information | Ce qu’elle représente |
|---|---|
| **Produit** | La ressource produite localement. |
| **Importé** | La quantité achetée ou reçue des villes voisines. |
| **Demandé** | Les besoins propres de la ville pour cette ressource. |
| **Exporté** | La quantité engagée auprès des villes voisines. |
| **Coût unitaire** | Le coût de référence du jeu pour cette ressource. |
| **Trésorerie** | L’argent dont dispose la ville. |

**Une limite importante : les villes voisines ne sont pas simulées pendant que vous jouez votre ville actuelle.**

SimCity 4 utilise les dernières informations sauvegardées pour ces voisines.

Par exemple, imaginez que votre ville voisine ait eu autrefois un gros excédent d’électricité. Si vous modifiez ensuite ses centrales électriques sans sauvegarder cette ville, les informations régionales peuvent ne pas refléter vos derniers changements.

Le Ministère doit travailler avec les enregistrements des villes voisines que SimCity 4 met à disposition.

**Quand ces enregistrements sont-ils mis à jour ?**

Le jeu actualise les informations régionales de la ville actuelle :

- Lors des mises à jour mensuelles.
- Lorsque des propositions de contrats régionaux sont demandées, par exemple quand vous ouvrez la fenêtre Accords avec les voisins.

Pour l’électricité, *Produit* représente la capacité de production des centrales au financement actuel, tandis que *Demandé* utilise les besoins mensuels en électricité du jeu, qui peuvent être légèrement supérieurs à la consommation réelle.

**Importé et Exporté représentent-ils des livraisons mesurées ?**

Pas nécessairement.

Dans les enregistrements régionaux, ces chiffres reposent sur les **quantités mensuelles contractées**, et non sur une mesure de ce qui a physiquement circulé ce jour-là.

Par exemple, un contrat de vente de 5 000 unités d’électricité peut compter pour 5 000 unités exportées dans les chiffres régionaux dès que le contrat existe.

Le jeu compte normalement la quantité totale du contrat des deux côtés, sauf si le contrat est annulé, invalide ou pas encore inclus dans les enregistrements.

Cette distinction compte lorsque le Ministère calcule si un autre contrat peut être proposé.

<a id="neighbor-deals-max"></a>
#### Quelle quantité les villes voisines peuvent-elles échanger ?

SimCity 4 n’autorise pas les échanges illimités.

La quantité maximale dépend de deux choses :

1. **L’excédent dont dispose la ville vendeuse.**
2. **La quantité d’approvisionnement supplémentaire que la ville acheteuse est autorisée à recevoir.**

Le maximum réel est la plus basse de ces deux limites.

**Quantité maximale d’un contrat (jeu de base)**

```math
Q_{\max} =
\max\left(
0,\min\left(S_{\mathrm{surplus}},B_{\mathrm{need}}\right)
\right)
```

Où :

- **Qmax** est la quantité maximale que le jeu autorise pour ce contrat.
- **Ssurplus** est l’excédent disponible de la ville vendeuse.
- **Bneed** est l’approvisionnement supplémentaire autorisé pour la ville acheteuse.

Le jeu calcule ces valeurs à partir des enregistrements régionaux des villes.

**Marge d’approvisionnement supplémentaire de la ville acheteuse**

```math
B_{\mathrm{need}} =
F \times
\max\left(
B_D+B_E,\frac{M}{F}+1
\right)
-(B_P+B_I)+q_{\mathrm{cur}}
```

**Excédent disponible de la ville vendeuse**

```math
S_{\mathrm{surplus}} =
(S_P+S_I)-(S_D+S_E)+q_{\mathrm{cur}}
```

Où :

- **P** signifie Produit.
- **I** signifie Importé.
- **D** signifie Demandé.
- **E** signifie Exporté.
- **S** désigne la ville vendeuse.
- **B** désigne la ville acheteuse.
- **qcur** est la quantité d’un contrat existant de ce type entre les deux villes, ou zéro s’il n’y en a pas.
- **F** est le facteur de sur-demande de l’acheteur du jeu, **2,5** par défaut.
- **M** est le montant minimal de contrat du jeu, **1 000 unités** par défaut.

Toutes les quantités de ressources sont des unités mensuelles.

**Que signifie le facteur 2,5 ?**

SimCity 4 permet à un acheteur de disposer d’un approvisionnement contracté et local pouvant atteindre **2,5 fois sa propre demande plus ses engagements d’exportation**, sous réserve des autres limites du jeu.

C’est une marge maximale, pas une obligation d’acheter autant.

Le vendeur, lui, ne peut proposer que l’excédent qui reste après avoir pris en compte ses propres besoins et ses engagements existants.

**Exemple : vendre de l’électricité à une ville voisine**

Imaginez que votre ville ait :

- Production d’électricité : **14 000 unités**.
- Besoin en électricité : **10 300 unités**.
- Aucun contrat d’électricité existant.

La ville voisine a :

- Production d’électricité : **3 000 unités**.
- Besoin en électricité : **6 000 unités**.
- Aucun contrat existant.

**Étape 1 : calculer votre excédent**

```math
S_{\mathrm{surplus}}=14000-10300=3700
```

Votre ville a **3 700 unités** à vendre.

**Étape 2 : calculer la marge d’achat de la voisine**

```math
B_{\mathrm{need}}=
2.5\times6000-3000=12000
```

La voisine est autorisée à recevoir jusqu’à **12 000 unités supplémentaires** selon la formule du jeu.

**Étape 3 : déterminer le contrat maximal**

```math
Q_{\max}=\min(3700,12000)=3700
```

**SimCity 4 autorise un contrat allant jusqu’à 3 700 unités par mois.**

Même si la voisine pourrait en accepter davantage, votre ville n’a pas actuellement plus d’excédent à proposer.

| Limite | Quantité |
|---|---:|
| Excédent de votre ville | 3 700 unités |
| Marge d’achat de la voisine | 12 000 unités |
| **Contrat maximal autorisé** | **3 700 unités** |

Si votre ville augmente sa capacité de production, la quantité de contrat autorisée peut elle aussi augmenter.

Cependant, la formule du jeu ne garantit pas à elle seule qu’un contrat sera proposé. D’autres conditions doivent être remplies.

**Que se passe-t-il lorsqu’un contrat existe déjà ?**

Pour calculer dans quelle mesure un contrat existant peut être renégocié, le jeu rajoute sa quantité actuelle des deux côtés du calcul.

Cela évite que le contrat existant ne réduise artificiellement son propre maximum.

Par exemple, si vous vendez déjà 2 000 unités, le jeu peut toujours évaluer si le contrat peut être augmenté ou ajusté, au lieu de considérer ces 2 000 unités comme définitivement indisponibles pour une renégociation.

Le jeu n’arrondit pas la quantité maximale de contrat calculée.

De plus, **le minimum de 1 000 unités sert à décider si une proposition peut être faite**. Il n’est pas appliqué automatiquement comme étape d’arrondi à la quantité maximale.

<a id="neighbor-deals-offer"></a>
#### Quand SimCity 4 propose-t-il un contrat régional ?

Même lorsque deux villes ont des connexions compatibles et assez de ressources pour échanger, le jeu peut ne pas proposer de nouveau contrat.

SimCity 4 vérifie aussi si **la ville qui paie a assez d’argent pour soutenir l’accord**.

La quantité minimale de contrat du jeu original est de **1 000 unités par mois**.

Le jeu exige que la ville qui paie ait :

- Une trésorerie supérieure à **5 000 §**.
- Assez d’argent pour payer le prix mensuel du contrat minimal.
- Assez d’argent pour couvrir **plus de 60 paiements mensuels** à ce prix.

La dernière condition correspond à **cinq ans de paiements en temps de jeu simulé**.

**Conditions de proposition d’un contrat (jeu de base)**

```math
\text{offered}\iff
Q_{\max}\ge M
\;\land\;
T>T_{\min}
\;\land\;
T>P(M)
\;\land\;
T>60\times P(M)
```

Où :

- **Qmax** est la quantité maximale de contrat autorisée.
- **M** est la quantité minimale de contrat, normalement 1 000 unités.
- **T** est la trésorerie de la ville qui paie.
- **Tmin** est la trésorerie minimale requise, normalement 5 000 §.
- **P(M)** est le prix mensuel d’un contrat de taille minimale.
- **60** est le nombre de paiements mensuels que le jeu exige que l’acheteur soit en mesure d’assumer.

La comparaison est stricte pour les valeurs de trésorerie : avoir exactement le montant requis ne suffit pas.

En revanche, une quantité maximale de contrat d’exactement 1 000 unités satisfait la condition de quantité minimale.

**Qui a besoin de l’argent ?**

Cela dépend du contrat.

| Contrat | Ville dont la trésorerie est vérifiée |
|---|---|
| Votre ville achète de l’électricité | Votre ville |
| Votre ville vend de l’électricité | La voisine |
| Votre ville achète de l’eau | Votre ville |
| Votre ville vend de l’eau | La voisine |
| Votre ville exporte des déchets | Votre ville |
| Votre ville importe des déchets | La voisine |

Rappelez-vous que l’exportation de déchets est un service que vous achetez, tandis que l’importation de déchets est un service que vous fournissez.

**Exemple : pourquoi un contrat peut ne pas apparaître**

Supposons que le prix mensuel du contrat minimal soit de **200 §**.

Le jeu exige que la ville qui paie détienne plus de :

```math
60\times200=12000
```

**12 000 §**, en plus de satisfaire les autres conditions.

| Trésorerie de la ville qui paie | Proposition de contrat |
|---|---|
| 5 000 § | Non proposé |
| 10 000 § | Non proposé |
| 12 000 § | Non proposé |
| **12 001 §** | **Peut être proposé** |

À 12 001 §, la condition de trésorerie est satisfaite. Le contrat a encore besoin d’une connexion valide, d’une quantité d’échange suffisante et des autres conditions d’éligibilité du jeu.

**Pourquoi un accord potentiellement rentable peut-il rester indisponible ?**

Parce que **la rentabilité économique et l’éligibilité ne sont pas la même chose**.

Une ville peut être disposée à acheter de l’électricité, par exemple, mais SimCity 4 ne proposera pas le contrat tant que le vendeur n’a pas au moins 1 000 unités d’excédent disponible.

Un vendeur qui pourrait augmenter sa production grâce à un financement plus élevé ne reçoit pas automatiquement de proposition fondée sur cette capacité future hypothétique.

La proposition ne devient possible que lorsque les chiffres régionaux du jeu montrent réellement un excédent suffisant.

De même, une ville peut avoir assez de ressources mais manquer de la trésorerie ou de la connexion réseau requises.

**Le Ministère doit respecter ces restrictions du jeu original.** Il ne peut pas supposer qu’un contrat financièrement attrayant est disponible avant que SimCity 4 ne le juge éligible.

<a id="neighbor-deals-pricing"></a>
#### Comment les prix des contrats régionaux sont-ils calculés ?

SimCity 4 calcule les prix des contrats à partir du **coût moyen de fourniture de la ressource par le vendeur**.

Il y ajoute ensuite une petite marge bénéficiaire.

Cela s’applique à l’électricité, à l’eau et aux services d’élimination des déchets.

La distinction importante est que **le prix des contrats du jeu repose sur le coût moyen, et non sur le coût supplémentaire de fourniture de ce contrat en particulier**.

Cette différence est au cœur de la façon dont le Ministère décide quels contrats valent la peine d’être signés.

#### Étape 1 : le coût moyen du vendeur

Le jeu calcule d’abord combien la ville vendeuse dépense par unité d’approvisionnement disponible.

**Coût unitaire du vendeur (jeu de base)**

```math
u=\frac{E_{\mathrm{dept}}+C_{\mathrm{buy}}}{I+P+1}
```

Où :

- **Edept** est la dépense mensuelle du vendeur pour le service concerné.
- **Cbuy** est ce que le vendeur paie pour acheter la même ressource à d’autres villes.
- **I** est la ressource importée par le vendeur.
- **P** est la ressource produite localement par le vendeur.
- **u** est le coût moyen par unité qui en résulte.

Le **1** supplémentaire au dénominateur fait partie de la formule du jeu.

**Exemple : une ville qui vend de l’électricité**

Supposons que votre ville produise 14 000 unités d’électricité et dépense 1 400 § par mois pour faire fonctionner ses centrales électriques.

Sans importations :

```math
u=\frac{1400}{14000+1}\approx0.09999
```

Le coût moyen de production calculé par le jeu est d’environ **0,10 § par unité**.

Le jeu utilise cette valeur comme point de départ pour fixer le prix des contrats d’électricité.

Il recalcule le coût unitaire de la ville actuelle lorsqu’il met à jour les enregistrements régionaux, notamment lors des mises à jour mensuelles et des demandes de nouvelles propositions de contrat.

#### Étape 2 : la marge bénéficiaire du jeu

Le prix du contrat comprend une majoration au-dessus du coût moyen du vendeur.

Avec les réglages du jeu original, la majoration va d’environ **10 % à 13 %**.

La valeur exacte dépend de l’approvisionnement disponible de l’acheteur et de l’excédent qu’il restera au vendeur après le contrat.

**Prix mensuel du contrat (jeu de base)**

```math
P(q)=q\times\mathrm{clamp}\left(
u(1+m_{\min})+
\frac{g(V/q)+g(R/S_{\mathrm{sup}})}{2}
\,u(m_{\max}-m_{\min}),
\ u(1+m_{\min}),
\ u(1+m_{\max})
\right)
```

Avec :

```math
\begin{aligned}
V &= 1+(B_P+B_I-q_{\mathrm{cur}})
-\max\left(B_D+B_E,\frac{M}{F}+1\right)\\
S_{\mathrm{sup}} &= \max(S_P+S_I,10^{-8})\\
R &= S_{\mathrm{sup}}-q-(S_D+S_E-q_{\mathrm{cur}})
\end{aligned}
```

Où :

- **q** est la quantité mensuelle de contrat proposée.
- **u** est le coût unitaire moyen du vendeur.
- **mmin = 0,10** est le facteur de profit minimal du jeu.
- **mmax = 0,50** est le facteur de profit maximal du jeu.
- **V** représente l’approvisionnement disponible de l’acheteur au-delà de ce dont il a besoin.
- **R** représente l’approvisionnement restant du vendeur après le contrat et les autres engagements.
- **Ssup** représente la production et les importations totales disponibles du vendeur.
- **qcur** est la quantité actuelle du contrat évalué, ou zéro pour un nouveau contrat.
- **g** est la courbe de tarification du jeu.

Pour une quantité de contrat effectivement égale à zéro (inférieure à \(10^{-8}\)), le prix calculé est nul.

**La courbe de tarification utilisée par le jeu**

| Entrée | Valeur de la courbe |
|---:|---:|
| 0 | 0,00001 |
| 0,0075 | 0,0075 |
| 1 | 0,075 |

Le jeu interpole entre ces points. En dessous du premier point, la valeur reste à 0,00001 ; au-dessus du dernier, elle reste à 0,075.

Bien que les réglages généraux de majoration du jeu autorisent une plage bien plus large, les petites valeurs produites par cette courbe limitent la majoration effective des contrats.

**Avec le réglage du jeu original, un contrat est facturé entre environ 1,10 et 1,13 fois le coût unitaire moyen du vendeur.**

La majoration tend à être :

- **Plus basse** lorsque l’acheteur a besoin de plus d’approvisionnement ou que le contrat utilise la majeure partie de l’excédent restant du vendeur.
- **Plus haute** lorsque l’acheteur dispose déjà d’un approvisionnement en réserve et que le vendeur conserve beaucoup de capacité.

En pratique, la différence est relativement faible.

#### Étape 3 : le montant réellement facturé

Une fois que le jeu a calculé le prix mensuel du contrat, il le convertit en un nombre entier de simoleons.

**Coût mensuel enregistré du contrat (jeu de base)**

```math
\text{cost}=\max\left(1,\lfloor P(q)\rfloor\right)
```

La valeur obtenue est enregistrée sur le contrat et apparaît dans le budget de la ville.

**Un contrat peut donc coûter au moins 1 § par mois, même lorsque sa quantité est nulle.**

C’est l’une des raisons pour lesquelles le Ministère supprime les contrats inutiles de quantité nulle.

#### Exemple : tarification des ventes d’électricité

Revenons à l’exemple d’électricité de la section précédente.

Le vendeur a un coût moyen d’électricité d’environ **0,09999 § par unité**.

L’acheteur voisin n’a pas assez d’approvisionnement local, si bien que sa contribution à la tarification est au minimum.

La formule de tarification du jeu produit les valeurs suivantes :

| Quantité du contrat | Ratio d’approvisionnement restant du vendeur | Valeur de la courbe de tarification | Prix par rapport au coût unitaire | Prix mensuel du contrat |
|---:|---:|---:|---:|---:|
| 1 200 | 0,1786 | 0,01913 | 1,1038 × | 132 § |
| 2 400 | 0,0929 | 0,01331 | 1,1027 × | 264 § |
| 3 600 | 0,0071 | 0,00714 | 1,1014 × | 396 § |
| 4 800 | Inférieur à zéro | 0,00001 | 1,1000 × | 527 § |

<text color="secondary">La ligne de 4 800 unités illustre la formule de tarification au-delà de la limite de contrat de 3 700 unités calculée précédemment. Cela ne signifie pas que le jeu autoriserait ce contrat dans l’état initial de l’exemple.</text>

Remarquez que le prix par unité change légèrement lorsque la quantité du contrat change.

**Cependant, ce prix ne nous dit pas si la vente d’électricité est rentable pour la ville.**

Par exemple, gagner 396 § par mois peut sembler intéressant, mais que se passe-t-il si fournir ces 3 600 unités supplémentaires exige d’augmenter les dépenses des centrales électriques de 450 § ?

Le contrat ferait perdre de l’argent.

À l’inverse, la ville peut avoir assez de capacité de production inutilisée pour fournir le contrat pour seulement 100 § de dépenses supplémentaires.

Dans ce cas, il serait rentable.

**Le Ministère considère le coût de fonctionnement supplémentaire réel, et pas seulement le prix de vente moyen du jeu.**

<a id="neighbor-deals-ladder"></a>
#### Quelles quantités de contrat le jeu autorise-t-il ?

SimCity 4 calcule une quantité maximale possible de contrat, mais la fenêtre Accords avec les voisins ne vous permet pas de choisir tous les nombres entre zéro et ce maximum.

Elle propose à la place une **liste de quantités autorisées**.

Par exemple, si le contrat maximal est de 3 700 unités, la fenêtre peut proposer seulement :

**1 000, 2 000 ou 3 000 unités.**

Elle ne propose pas nécessairement 3 700.

Les montants disponibles dépendent de :

- La quantité maximale actuelle.
- Le montant minimal d’accord du jeu.
- Si le contrat est nouveau ou déjà actif.
- L’incrément enregistré sur un contrat existant.
- Si le contrat est suspendu.

#### Comment la liste des quantités est-elle construite ?

Pour un nouveau contrat, le jeu divise normalement la quantité maximale en environ dix paliers, tout en respectant la taille minimale de contrat.

**Incrément d’un nouveau contrat (jeu de base)**

```text
maxQ = truncate(Qmax)
d10  = maxQ / 10       (integer division)
step = max(d10, M)
```

Où **M** est le montant minimal d’accord, normalement 1 000 unités.

Les quantités proposées sont des multiples de ce palier, jusqu’au maximum autorisé.

**Exemple 1 : quantité maximale de 15 442 unités**

```text
step = max(1544, 1000)
     = 1544
```

La fenêtre propose :

1 544 ; 3 088 ; 4 632 ; 6 176 ; 7 720 ; 9 264 ; 10 808 ; 12 352 ; 13 896 et 15 440 unités.

**Exemple 2 : quantité maximale de 3 700 unités**

```text
step = max(370, 1000)
     = 1000
```

La fenêtre propose :

1 000 ; 2 000 et 3 000 unités.

Le maximum exact de 3 700 n’est pas disponible comme montant sélectionnable.

#### Les contrats existants ont des règles supplémentaires

Lorsqu’un contrat existe déjà, SimCity 4 essaie de conserver sa quantité actuelle parmi les options disponibles.

Il utilise aussi l’incrément enregistré sur le contrat.

Si la plage disponible devient beaucoup plus grande, le jeu peut construire une liste à deux niveaux, avec des paliers plus petits près de la quantité actuelle et des paliers plus grands au-dessus.

**Exemple**

Prenons un contrat actif avec :

- Montant actuel : 2 000 unités.
- Incrément enregistré : 1 000 unités.
- Nouveau maximum : 30 000 unités.

La fenêtre propose les plus petites quantités de **1 000 et 2 000**, suivies de paliers plus grands :

**3 000 ; 6 000 ; 9 000 ; 12 000 ; 15 000 ; 18 000 ; 21 000 ; 24 000 ; 27 000 et 30 000.**

Cela permet d’ajuster un contrat existant sans obliger le joueur à passer par des dizaines de petits paliers.

**Règle complète de la liste des quantités (jeu de base)**

```text
maxQ = trunc(Qmax)
       (if maxQ <= 0: use the current amount)

d10  = maxQ / 10
step = max(d10, M)

New proposal:
    Inc = step

Active contract:
    if (Inc == Amount and Inc > M) or Inc == 0:
        Inc = d10

    step     = Inc
    selected = Amount / Inc

Suspended contract:
    only the current amount

Q' = max(maxQ, step * selected, step)

If active, Amount > 0, and Q' / Inc > 15:
    Inc2 = Q' / 10

    entries =
        multiples of Inc up to Amount
        plus multiples of Inc2 above Amount, up to Q'
Otherwise:
    entries = multiples of step up to Q'
```

Toutes les divisions sont tronquées à l’entier.

La sélection **zéro ou « pas d’accord » du jeu annule un contrat**.

Un contrat suspendu ne peut pas être redimensionné par la liste de quantités normale.

**Le Ministère respecte les choix de quantité légaux du jeu.** Il n’invente pas de montants de contrat arbitraires simplement parce qu’un autre nombre serait plus rentable.

<a id="neighbor-deals-accounting"></a>
#### États d’un contrat : actif, en attente et suspendu

Un contrat régional peut se trouver dans plusieurs états différents.

| État | Ce qu’il signifie |
|---|---|
| **En attente** | Une proposition existe, mais le contrat n’est pas encore actif. |
| **Actif** | Le contrat est actuellement en vigueur. |
| **Suspendu** | Le jeu a temporairement interrompu le contrat à cause d’un problème. |
| **Annulé** | Le contrat a pris fin. |
| **Invalide** | Le contrat ne remplit plus une condition d’éligibilité, comme une connexion manquante ou une capacité insuffisante. |
| **Suspension écoulée** | Le contrat a atteint la fin de sa période de suspension. |

SimCity 4 vérifie les contrats lors de sa maintenance mensuelle.

Par exemple, le jeu peut signaler un contrat lorsque :

- La trésorerie de la ville qui paie n’est plus supérieure au coût mensuel du contrat.
- La connexion régionale requise a disparu.
- La ville ne peut plus satisfaire l’exigence de capacité correspondante.

**Un contrat défaillant est normalement suspendu avant que le jeu n’y mette définitivement fin.**

La suspension joue le rôle de délai de grâce.

Pendant la suspension, le contrat ne peut être ni redimensionné ni annulé par les opérations de contrat normales. Le jeu gère sa résolution ultérieure.

#### Comment les contrats sont-ils comptés dans l’approvisionnement régional ?

Les chiffres régionaux et le budget financier ne traitent pas toujours les contrats de la même façon.

**Quantités de ressources régionales**

Le jeu compte normalement la quantité totale d’un contrat dans le total Importé de l’acheteur et dans le total Exporté du vendeur, y compris les propositions en attente et les contrats suspendus.

Les contrats annulés et invalides sont exclus.

**Revenus et dépenses mensuels des contrats**

Les calculs financiers du jeu incluent les contrats actifs et suspendus, mais excluent les contrats en attente, annulés et invalides.

| État du contrat | Compté dans les quantités régionales | Compté dans les revenus ou dépenses mensuels des contrats |
|---|---|---|
| En attente | Oui | Non |
| Actif | Oui | Oui |
| Suspendu | Oui | Oui |
| Annulé | Non | Non |
| Invalide | Non | Non |

**Pourquoi est-ce important ?**

Une proposition en attente peut influencer les chiffres d’approvisionnement régional du jeu avant de créer le moindre paiement financier.

De même, un contrat suspendu peut rester représenté dans la comptabilité régionale alors que son service n’est peut-être pas fiable.

**Le Ministère distingue un contrat enregistré de son approvisionnement réellement utilisable.**

Cela l’empêche de compter sur des achats d’électricité ou d’eau suspendus simplement parce que leurs quantités figurent encore dans les totaux de la région.

<a id="neighbor-deals-delivery"></a>
#### Quand les ressources contractées sont-elles livrées ?

L’électricité, l’eau et les déchets ne suivent pas les mêmes règles de livraison.

**Électricité et eau**

Ces contrats sont traités comme des engagements d’approvisionnement mensuels.

La quantité contractée compte comme approvisionnement pour l’acheteur et comme obligation pour le vendeur tant que le contrat reste en vigueur, quelle que soit la quantité réellement consommée.

**Déchets**

Les contrats de déchets fonctionnent par livraisons quotidiennes.

**Livraison quotidienne d’un contrat de déchets (jeu de base)**

```math
\text{daily step}=
\max\left(1,\mathrm{round}(0.1\times A)\right)
```

La livraison quotidienne se poursuit tant que :

```math
A-U>0
```

Où :

- **A** est la quantité mensuelle contractée.
- **U** est la quantité déjà livrée pendant le mois.

Le jeu remet l’utilisation mensuelle à zéro au changement de mois.

Par conséquent, **la plupart des contrats de déchets livrent toute leur quantité mensuelle pendant les dix premiers jours simulés environ**.

**Exemple : un contrat de déchets de 25 004 unités**

La quantité quotidienne est de :

```math
\mathrm{round}(25004\times0.1)=2500
```

Au bout de dix jours, le jeu a livré :

```math
10\times2500=25000
```

Il ne reste que quatre unités, mais le jeu ne réduit pas la dernière livraison à quatre.

Il effectue au contraire une autre livraison de 2 500 unités le onzième jour.

**Le total devient 27 500 unités**, soit plus que la quantité mensuelle nominale du contrat.

C’est particulièrement important lorsque votre ville reçoit des déchets d’une voisine.

**Les calculs de gestion des déchets du Ministère ne doivent pas sous-estimer les déchets importés simplement parce que la quantité contractuelle est légèrement inférieure à ce que le jeu livre réellement.**

#### Quelle quantité de déchets votre ville peut-elle recevoir ?

SimCity 4 limite aussi le total des déchets qu’une ville peut accepter de ses voisines.

Cette limite dépend principalement :

- Du stockage de décharge disponible.
- De la production de déchets propre à la ville.
- Des déchets déjà importés.
- Du réglage de durée des contrats du jeu.

**Déchets reçus maximaux (jeu de base)**

```math
\sum A_{\mathrm{received}}
\le
L_{\mathrm{free}}
\left(1-\frac{1}{60+0.000107}\right)
+G_P+G_I
```

Où :

- **Lfree** est le stockage de décharge disponible de la ville.
- **GP** est la production de déchets enregistrée de la ville.
- **GI** est ses importations de déchets enregistrées.
- **60** représente la durée de contrat de cinq ans du jeu, mesurée en mois.

**Exemple**

Supposons qu’une ville ait :

- Stockage de décharge libre : 500 000 unités.
- Déchets produits : 40 000 unités.
- Déchets importés : 10 000 unités.

La marge maximale de déchets reçus du jeu est d’environ :

```math
500000
\left(1-\frac{1}{60+0.000107}\right)
+40000+10000
\approx541667
```

**Déchets reçus maximaux autorisés : environ 541 667 unités.**

Si le total reçu par contrats dépasse cette limite, le jeu signale les contrats en excès comme ayant une capacité insuffisante.

Ces contrats sont alors suspendus, dans l’ordre de la liste des contrats du jeu.

**Le Ministère doit respecter cette limite même lorsqu’accepter des déchets supplémentaires paraîtrait autrement rentable.**

<a id="neighbor-deals-problem"></a>
### 2. Le problème de gestion

Gérer les contrats régionaux à la main est difficile, car plusieurs décisions s’influencent mutuellement.

#### Un contrat qui semble rentable peut en réalité faire perdre de l’argent

SimCity 4 fonde ses prix de vente sur le **coût moyen**.

Mais la valeur économique d’un contrat dépend du **coût marginal** : ce que la ville doit dépenser en plus, ou ce qu’elle peut économiser, à cause de ce contrat précis.

Par exemple, vendre de l’électricité 400 § par mois n’est pas intéressant si cela exige 500 § de dépenses supplémentaires pour les centrales électriques.

Le jeu n’affiche pas directement cette comparaison.

#### Les propositions de contrat changent constamment

Un contrat disponible aujourd’hui peut disparaître lorsque :

- La demande de votre ville change.
- L’approvisionnement enregistré d’une ville voisine change.
- Une centrale perd de la capacité de production.
- Une ville n’a plus assez d’argent.
- Un autre contrat utilise l’excédent disponible.

Les choix de quantité légaux changent eux aussi avec la capacité disponible.

#### Plusieurs voisines se disputent les mêmes ressources

Imaginez que votre ville puisse vendre 5 000 unités d’électricité.

Une voisine propose d’en acheter 3 000, tandis qu’une autre propose d’en acheter 4 000.

**Votre ville ne peut pas forcément accepter les deux contrats**, car le total dépasserait son approvisionnement disponible.

Le Ministère doit comparer les combinaisons possibles plutôt que de choisir chaque contrat séparément.

#### Les achats influencent les besoins de production locale

Acheter de l’électricité ou de l’eau peut réduire la quantité que vos propres installations doivent produire.

Avec les règles de réserve du Ministère, cela peut générer des économies supérieures à ce que la seule quantité achetée pourrait laisser penser.

#### Le calcul d’excédent du jeu ne protège pas votre réserve

Les limites de contrat propres à SimCity 4 n’incluent pas la réserve de sécurité supplémentaire du Ministère.

Le jeu peut autoriser une vente qui laisse trop peu de capacité d’électricité ou d’eau de réserve pour les besoins de votre ville.

**Le Ministère doit appliquer ses propres exigences de réserve et de service en plus des limites légales du jeu.**

#### Les contrats de déchets dépendent du calendrier quotidien

Deux combinaisons de contrats de déchets peuvent avoir la même quantité mensuelle totale mais des calendriers de livraison différents.

Cela peut changer le fait que le système de gestion des déchets évacue ou non les déchets du mois.

**Le Ministère évalue donc des combinaisons complètes de contrats de déchets à l’aide de la simulation quotidienne de la gestion des déchets, et pas seulement des totaux mensuels.**

<a id="neighbor-deals-algorithm"></a>
### 3. Comment le Ministère gère les contrats régionaux

Le Ministère combine la sélection des contrats avec les optimiseurs de ressources locaux.

Au lieu d’examiner chaque ville voisine indépendamment, il recherche **la combinaison de contrats qui procure le plus grand bénéfice économique global tout en protégeant les services de la ville elle-même**.

Il effectue une optimisation distincte pour l’électricité, l’eau et les déchets.

#### Le Ministère signe-t-il les contrats automatiquement ?

**Oui.**

Lorsque la gestion automatique des accords avec les voisins est activée, le Ministère peut :

- Créer de nouveaux contrats.
- Augmenter ou réduire les quantités des contrats existants.
- Annuler les contrats qui ne sont plus utiles.

Il effectue ces opérations directement par les fonctions de contrat normales du jeu.

Pour un nouveau contrat, il demande à SimCity 4 les propositions disponibles, active la proposition choisie et fixe sa quantité.

Pour un contrat existant, il utilise les opérations normales du jeu pour modifier la quantité ou annuler le contrat.

**Vous n’avez pas besoin d’ouvrir la fenêtre des accords avec les voisins ni d’accepter les contrats manuellement.**

Le Ministère ne modifie pas les villes voisines, ne réécrit pas les fichiers de sauvegarde, ne change pas les dates des contrats et ne force pas le jeu à accepter des contrats qu’il ne proposerait pas normalement.

**Et les contrats que vous avez créés vous-même ?**

Lorsque le Ministère gère une ressource, vos contrats créés manuellement sont inclus dans son optimisation.

Il peut les redimensionner ou les annuler si une autre combinaison légale est meilleure.

Si vous voulez garder le contrôle manuel total des contrats d’une ressource donnée, désactivez l’option de gestion automatique des accords de cette ressource.

#### À quelle fréquence les contrats sont-ils examinés ?

Les contrats sont normalement examinés en même temps que la ressource correspondante.

| Ressource | Examen régulier |
|---|---|
| **Électricité** | Tous les 10 jours simulés, avec des examens plus précoces en cas de changements importants ou de pénuries d’approvisionnement confirmées. |
| **Eau** | Tous les 10 jours simulés depuis le dernier examen de l’eau, à chaque changement de mois, et plus tôt si nécessaire. |
| **Déchets** | À chaque changement de mois et lorsque les valeurs surveillées de la gestion des déchets changent de façon importante. |

Une vérification quotidienne légère recherche aussi les vraies pénuries et les changements importants de la demande, de la production ou des contrats régionaux.

Chaque évaluation part de l’état actuel de la ville.

**Le Ministère n’impose normalement ni délai d’attente supplémentaire ni temps de repos du seul fait qu’il a récemment modifié un contrat.**

Des protections spécifiques contre les changements de contrat inutiles existent ; elles sont expliquées plus bas et dans la section sur la réévaluation des contrats.

#### Étape 1 : trouver toutes les options de contrat disponibles

Le Ministère commence par examiner les villes voisines dotées de connexions adaptées et de relevés régionaux complets.

Pour chaque ressource et chaque sens d’échange, il identifie les quantités de contrat légales.

**Contrats existants**

La quantité actuelle est incluse, ainsi que les autres quantités permises par le jeu.

**Nouveaux achats d’électricité et d’eau**

Le Ministère utilise les quantités actuellement permises par la liste de propositions du jeu.

Il exclut les achats dont le prix mensuel ne resterait pas inférieur à la trésorerie disponible de la ville locale.

**Nouvelles ventes d’électricité et d’eau**

La ville voisine doit déjà figurer dans les offres disponibles du jeu.

Cependant, certaines quantités de vente plus importantes ne deviennent possibles qu’**après que le Ministère a augmenté la capacité de production locale**.

Le Ministère peut les considérer comme des *candidats de vente latents*.

Par exemple, imaginons que votre ville n’ait actuellement que 2 000 unités d’électricité en excédent, mais pourrait produire 3 000 unités de plus en augmentant le financement des centrales.

Un contrat plus important pourrait devenir possible une fois cette capacité réellement disponible.

Le Ministère peut évaluer si le coût de production supplémentaire serait justifié.

**Un contrat latent n’est pas signé tant que les règles d’éligibilité réelles du jeu ne le permettent pas.**

**Déchets reçus des voisins**

Le Ministère considère les quantités permises par la liste de propositions actuelle du jeu.

Il doit aussi vérifier que la ville peut traiter les déchets supplémentaires.

#### Pourquoi le Ministère utilise-t-il une base de prix commune ?

Pour évaluer les contrats possibles, le Ministère retire d’abord ses propres contrats ajustables des calculs de prix.

Il évalue ensuite les quantités candidates par rapport à cette base commune.

Cela évite une situation indésirable où la modification d’un contrat altère les relevés régionaux, ce qui change le prix calculé, ce qui conduit le Ministère à préférer immédiatement de nouveau l’arrangement précédent.

**La même quantité candidate ne devrait pas paraître plus ou moins intéressante simplement parce que le Ministère vient d’écrire un contrat équivalent.**

Lors de l’évaluation de portefeuilles complets, les prix sont calculés à partir de la combinaison de contrats sélectionnée.

Les candidats qui n’ont pas assez d’informations fiables pour calculer leur prix sont exclus. Les valeurs manquantes ne sont jamais traitées silencieusement comme des zéros.

<a id="neighbor-deals-objective"></a>
#### Étape 2 : trouver la combinaison la plus rentable

Le Ministère ne se contente pas de maximiser les revenus des contrats.

Il considère **ensemble les revenus, les dépenses des contrats et le coût d’approvisionnement des propres services de la ville**.

**Objectif du portefeuille de contrats régionaux**

```math
\max_x\ \mathrm{Net}(x)=
\sum_{i\in\mathrm{sell\text{-}like}}P_i(x_i\mid x)
-
\sum_{j\in\mathrm{buy\text{-}like}}P_j(x_j\mid x)
-
C_{\mathrm{local}}(x)
```

Où :

- **x** représente les quantités choisies pour tous les contrats candidats d’une ressource.
- **Pi** est le revenu d’un contrat de type vente.
- **Pj** est la dépense d’un contrat de type achat.
- **Clocal(x)** est le coût d’exploitation local le plus bas qui permet de soutenir les contrats choisis en toute sécurité.
- **Net(x)** est la valeur économique mensuelle qui en résulte.

Le prix de chaque contrat est calculé dans le contexte du portefeuille complet sélectionné.

**Qu’est-ce qui est inclus dans le coût local ?**

Cela dépend de la ressource :

| Ressource | Coût local considéré |
|---|---|
| **Électricité** | Financement des centrales nécessaire pour satisfaire les besoins locaux, la réserve et les exportations, après les importations. |
| **Eau** | Financement du service des eaux nécessaire pour satisfaire les besoins locaux, la réserve et les exportations, après les importations. |
| **Déchets** | Financement de la gestion des déchets nécessaire pour traiter les déchets de la ville, les importations et exportations choisies, et la réserve configurée. |

Le Ministère utilise les mêmes calculs que ceux décrits dans les chapitres précédents.

Lorsque **Tenir compte de l’usure des centrales** est activé, le coût économique local de l’électricité inclut aussi l’usure estimée des centrales.

Cette usure sert à comparer les options mais **n’est pas une dépense supplémentaire pour la trésorerie**.

#### Exemple : comparer des portefeuilles de contrats

Prenons une ville fictive avec trois arrangements possibles :

| Arrangement | Revenus des contrats | Dépenses des contrats | Coût d’exploitation local | Net |
|---|---:|---:|---:|---:|
| Production locale uniquement | 0 § | 0 § | 1 000 § | −1 000 § |
| Acheter à un voisin | 0 § | 300 § | 600 § | −900 § |
| Vendre à un autre voisin | 400 § | 0 § | 1 200 § | −800 § |

Dans cet exemple simplifié, la vente donne le meilleur résultat économique, même si les coûts d’exploitation locaux augmentent.

Le Ministère préférerait cet arrangement **à condition que toutes les conditions légales et de sécurité soient satisfaites**.

Les chiffres sont illustratifs ; le véritable planificateur utilise les prix des contrats du jeu et les calculs de financement propres à chaque ressource.

**Comment les égalités sont-elles traitées ?**

Si deux portefeuilles légaux ont la même valeur économique, le Ministère préfère :

1. Le portefeuille comptant **moins de contrats**.
2. En cas de nouvelle égalité, le portefeuille nécessitant **moins de changements**.

Cela évite des contrats et des ajustements inutiles qui n’améliorent pas le résultat.

#### Les règles de sécurité que tout portefeuille doit respecter

Le Ministère ne considère que les combinaisons de contrats qui respectent les restrictions suivantes.

**1. Les services de votre ville passent en premier**

Le plan local de production ou de gestion des déchets doit rester réalisable avec les contrats choisis.

Un portefeuille que les services propres de la ville ne peuvent pas soutenir en toute sécurité est rejeté.

**2. Les achats ne peuvent pas dépasser la demande propre de votre ville**

Le Ministère n’achète pas d’électricité ni d’eau uniquement pour la revendre.

Il n’achète pas non plus d’électricité ou d’eau supplémentaire uniquement pour constituer la réserve configurée.

**3. Normalement, une ressource est soit achetée, soit vendue**

Par défaut, le Ministère choisit l’une de trois positions pour chaque ressource :

- **LOCAL :** aucun achat ni vente.
- **ACHAT :** acheter la ressource.
- **VENTE :** vendre la ressource.

Acheter et vendre la même ressource simultanément n’est pas autorisé dans la configuration par défaut.

Le réglage facultatif **Acheter à une ville pour vendre à une autre** est expliqué dans les réglages des contrats.

**4. Les importations de déchets doivent rester dans des limites sûres**

Le total des déchets reçus ne peut pas dépasser la limite de déchets entrants du jeu ni la quantité supplémentaire que le système de gestion des déchets de la ville peut traiter.

**5. Les contrats suspendus sont figés**

Un contrat suspendu ne peut pas être modifié par les fonctions normales du jeu.

Le Ministère inclut sa quantité existante dans les calculs mais ne tente pas de le redimensionner ni de l’annuler.

**6. Un déficit confirmé d’électricité ou d’eau empêche les ventes**

Si les compteurs du jeu confirment une véritable pénurie régionale, le Ministère ne maintient pas les ventes de cette ressource.

**L’approvisionnement essentiel de sa propre ville a priorité sur les revenus d’exportation.**

#### Comment les réserves d’électricité et d’eau sont-elles incluses ?

Le Ministère calcule quelle capacité locale reste nécessaire après les importations et les exportations.

**Capacité locale requise**

```math
N(B,S)=
\max\left(
\left\lceil R\times\max(0,D-B)\right\rceil,
R_q-B
\right)+S
```

Où :

- **D** est la demande propre de la ville.
- **B** est la quantité totale achetée.
- **S** est la quantité totale vendue.
- **R** est le multiplicateur de réserve.
- **Rq** est le besoin mensuel d’électricité propre au jeu.

Pour l’électricité, la réserve par défaut est de 20 %, ce qui donne **R = 1,20**.

Pour l’eau, la réserve par défaut est de 10 %, ce qui donne **R = 1,10**.

**Pour l’eau, le terme Rq est omis**, car le calcul de l’eau n’utilise pas le compteur de besoin d’électricité distinct.

**Exemple : électricité avec une réserve de 20 %**

Supposons que la ville ait :

- Demande : 10 000 unités.
- Besoin du jeu : 10 300 unités.
- Réserve : 20 %.

Sans contrats :

```math
N(0,0)=12000
```

Si elle achète 2 000 unités :

```math
N(2000,0)=
\max(9600,8300)=9600
```

Si elle vend plutôt 2 400 unités :

```math
N(0,2400)=14400
```

| Situation | Capacité de production locale requise |
|---|---:|
| Aucun contrat | 12 000 |
| Acheter 2 000 unités | 9 600 |
| Vendre 2 400 unités | 14 400 |

**Acheter 2 000 unités réduit de 2 400 unités le besoin de capacité locale** dans cet exemple, car la réserve ne s’applique qu’à ce que la ville doit produire elle-même.

Vendre 2 400 unités augmente le besoin d’exactement 2 400.

Le Ministère compare donc les prix des contrats à la variation réelle des coûts locaux causée par ces besoins de capacité.

#### Pourquoi le Ministère ne peut-il pas acheter plus que ce dont la ville a besoin ?

Le Ministère applique une limite d’achat supplémentaire.

**Plafond d’achat**

```math
\sum_j x_j\le
\min\left(
D,\;
F\times\max\left(L_D+L_E,\frac{M}{F}+1\right)
-(L_P+L_I)
\right)
```

Où :

- **D** est la demande propre de la ville.
- **F** est le facteur de surdemande de l’acheteur du jeu.
- **M** est la quantité minimale d’un contrat.
- Les valeurs **L** sont les relevés régionaux de la ville locale, hors ses propres achats ajustables.

Pour les déchets, la limite de demande repose sur **la production de déchets de la ville moins le recyclage**.

Cela empêche le Ministère de :

- Acheter plus que ses propres besoins.
- Acheter des ressources uniquement pour les revendre.
- Acheter des ressources supplémentaires pour constituer une réserve sur les importations.
- Utiliser un approvisionnement acheté comme base de nouveaux engagements d’exportation.

Les limites d’achat légales propres au jeu sont également respectées.

#### Comment le Ministère évalue-t-il un contrat isolé ?

Le calcul du portefeuille peut aussi se comprendre en regardant un seul contrat.

**Pour une vente**

Le Ministère compare le paiement reçu au coût d’exploitation local supplémentaire.

```math
\mathrm{Profit}(q)=
P(q)-
\left[
C_{\mathrm{local}}(N+q)
-
C_{\mathrm{local}}(N)
\right]
```

Par exemple, si une vente rapporte 300 § mais demande 200 § de dépenses de production en plus :

**Bénéfice net : 100 § par mois.**

La vente est rentable.

**Pour un achat**

Le Ministère compare le prix du contrat à la dépense locale qu’il évite.

```math
\mathrm{Saving}(q)=
\left[
C_{\mathrm{local}}(N(0))
-
C_{\mathrm{local}}(N(q))
\right]
-
P(q)
```

Par exemple, si l’achat d’électricité coûte 250 § mais permet à votre ville d’économiser 400 § de financement des centrales :

**Économie nette : 150 § par mois.**

L’achat est rentable.

**C’est la rentabilité marginale :** le Ministère évalue l’effet économique réel de l’ajout ou de la modification d’un contrat.

Il ne suppose pas qu’une vente vaut la peine simplement parce que le jeu applique une marge au coût moyen du vendeur.

#### Étape 3 : rechercher la meilleure combinaison de contrats

Une fois les quantités disponibles et les exigences de sécurité connues, le Ministère compare les portefeuilles légaux.

La recherche fonctionne différemment pour l’électricité, l’eau et les déchets.

**Électricité et eau**

Le Ministère utilise un algorithme d’optimisation pour comparer des combinaisons d’achats et de ventes totaux.

Pour que le calcul reste efficace, les quantités sont d’abord regroupées en **blocs de 100 unités**.

Pour de très grandes plages d’échange, la taille des blocs augmente afin de limiter le calcul.

Après avoir choisi le meilleur candidat, le Ministère vérifie de nouveau la combinaison complète avec les **quantités exactes**, sans le regroupement approximatif.

**Déchets**

Les déchets demandent une méthode différente.

Comme chaque contrat a son propre calendrier de livraison quotidien, le Ministère ne peut pas décider en toute sécurité à partir des seuls totaux mensuels.

Il évalue donc des combinaisons de contrats complètes et exécute le calcul de financement de la gestion des déchets pour chacune.

**Chaque portefeuille candidat doit être testé par rapport au processus d’élimination des déchets projeté.**

Cette recherche peut devenir coûteuse quand de nombreux voisins et de nombreuses quantités sont disponibles.

Si le nombre de combinaisons dépasse **5 millions d’un côté de la recherche**, le Ministère considère l’optimum comme non prouvé.

**Il ne fait pas de changement de contrat spéculatif lorsqu’il ne peut pas établir un résultat fiable.**

#### Étape 4 : supprimer les contrats et les changements inutiles

Après avoir trouvé le portefeuille préféré, le Ministère effectue plusieurs vérifications finales.

**1. Supprimer les contrats qui n’apportent aucun avantage**

Si la suppression d’un contrat ne rend pas le portefeuille économiquement moins bon, le Ministère abandonne ce contrat.

Chaque contrat facultatif doit apporter un avantage strictement positif.

**2. Ne jamais remplacer un meilleur arrangement existant**

Si conserver tous les contrats actuels est légal, réalisable et strictement plus avantageux que le résultat proposé, le Ministère laisse les contrats inchangés.

**3. Ignorer les changements facultatifs insignifiants**

Un réglage INI avancé peut définir un avantage minimal requis pour les changements de contrat facultatifs.

Le seuil par défaut est de **0 §**, de sorte que tout avantage strictement positif peut convenir.

Lorsqu’un seuil plus élevé est configuré, le Ministère évite les créations de contrat ou changements de quantité facultatifs dont l’avantage est inférieur à ce montant.

**Les changements nécessaires pour maintenir les services essentiels sont exemptés de ce seuil économique.**

**4. Supprimer les contrats dormants de quantité zéro**

SimCity 4 peut parfois laisser un contrat actif avec une quantité de zéro.

Un tel contrat ne fournit aucune ressource et coûte quand même 1 § par mois.

Le Ministère annule ces contrats inutiles.

#### Comment mesure-t-on l’avantage d’une décision sur un contrat ?

Quand plusieurs contrats interagissent, regarder uniquement le revenu ou la dépense d’un seul contrat peut donner une mauvaise réponse.

À la place, le Ministère se demande :

**De combien le meilleur plan légal se détériorerait-il si ce contrat particulier devait rester à sa quantité actuelle ?**

**Avantage marginal d’une décision sur un contrat**

```math
\Delta_k=
\mathrm{Net}(x^*)
-
\max_{x:\,x_k=x_k^{\mathrm{now}}}
\mathrm{Net}(x)
```

Où :

- **x\*** est le portefeuille optimal sélectionné.
- **xk** est la quantité du contrat évalué.
- **xk actuel** est sa quantité actuelle.
- **Net** est la valeur économique totale du portefeuille.
- **Δk** est l’avantage économique attribuable au fait de laisser ce contrat particulier changer.

Le Ministère compare le portefeuille sélectionné à la **meilleure alternative qui laisse ce contrat inchangé**, et pas seulement à l’état global actuel de la ville.

Cela compte parce qu’un autre voisin peut offrir un contrat tout aussi bon.

Par exemple, supposons que deux voisins puissent fournir de l’électricité pour le même coût économique total.

Créer un contrat avec l’un ou l’autre peut être rentable par rapport à une production entièrement locale.

Mais si l’autre voisin peut apporter le même avantage, **aucun de ces contrats n’est indispensable**.

Le Ministère en tient compte pour décider si un changement individuel est justifié.

**La création ou l’augmentation d’un contrat n’est normalement écrite que si son avantage marginal est strictement positif.**

Il existe une exception : un changement requis pour maintenir le service lorsqu’aucun portefeuille légal ne peut garder ce contrat à sa quantité actuelle.

Dans ce cas, le Ministère n’invente pas d’avantage monétaire. Il reconnaît que le changement est nécessaire pour satisfaire les exigences du service.

#### Économies de trésorerie et économies économiques

Pour l’électricité, le Ministère conserve deux versions de l’avantage d’une décision sur un contrat :

| Mesure | Ce qu’elle inclut |
|---|---|
| **Avantage de trésorerie** | Variations des dépenses d’exploitation réelles et des revenus ou paiements des contrats. |
| **Avantage économique** | Avantage de trésorerie plus la valeur estimée de l’usure des centrales, lorsque la prise en compte de l’usure est activée. |

**La décision utilise l’avantage économique.**

L’avantage de trésorerie décrit l’effet sur le budget réel de la ville.

L’usure estimée des centrales n’est jamais débitée directement de la trésorerie.

Cette distinction permet au Ministère de tenir compte de la détérioration à long terme des centrales sans présenter une dépense mensuelle imaginaire comme de l’argent réellement dépensé.

**L’objectif global reste le même : choisir la combinaison de contrats légaux la plus avantageuse sur le plan économique, sans compromettre les services essentiels de votre ville, ses réserves ni les protections de sécurité existantes.**
<a id="neighbor-deals-exclusive"></a>
#### Acheter ou vendre : un seul sens par défaut

Par défaut, le Ministère n’achète pas et ne vend pas la même ressource en même temps.

Pour chaque ressource, il choisit l’un de trois arrangements possibles :

| Arrangement | Ce que fait le Ministère |
|---|---|
| **LOCAL** | Utilise la production ou l’élimination locale sans acheter ni vendre cette ressource. |
| **ACHAT** | Achète la ressource à une ou plusieurs villes voisines. |
| **VENTE** | Vend la ressource ou le service d’élimination à une ou plusieurs villes voisines. |

Pour l’électricité et l’eau, acheter et vendre ont leur sens habituel.

Pour les déchets, les sens sont inversés :

- **ACHAT :** votre ville paie un voisin pour éliminer ses déchets.
- **VENTE :** votre ville reçoit les déchets d’un voisin et est payée pour les traiter.

Ce comportement est contrôlé par **Acheter à une ville pour vendre à une autre**, qui est **désactivé par défaut**.

#### Et si la ville a déjà des contrats dans les deux sens ?

Le Ministère compare les arrangements disponibles et se dirige vers celui qui présente le plus grand avantage économique global.

Par exemple, si votre ville achète et vend de l’électricité à la fois, le Ministère choisit normalement de garder soit les achats, soit les ventes, puis annule les contrats inutiles.

**Il y a une exception importante : les contrats suspendus.**

SimCity 4 ne permet pas de modifier les contrats suspendus par ses fonctions de contrat normales.

Supposons que votre ville ait une vente d’électricité suspendue mais ait maintenant besoin d’acheter de l’électricité pour éviter une pénurie.

Le Ministère ne peut pas supprimer immédiatement la vente suspendue.

Dans ce cas, **l’approvisionnement en électricité de la ville a priorité sur la restriction habituelle achat ou vente**.

Le Ministère peut acheter l’électricité nécessaire pour maintenir le service, évite de créer des ventes supplémentaires et consigne le conflit de politique dans le journal.

Il ne laisse pas la règle d’exclusivité priver la ville d’une ressource essentielle.

<a id="neighbor-deals-arbitrage"></a>
#### Facultatif : acheter et vendre en même temps

Vous pouvez activer **Acheter à une ville pour vendre à une autre** pour permettre au Ministère d’envisager des contrats dans les deux sens pour la même ressource.

Cela nécessite que **Gérer les accords avec les voisins** soit activé.

Le Ministère ne choisit un arrangement mixte que s’il produit un meilleur résultat économique que chacun des arrangements à sens unique disponibles.

**En quoi cela peut-il être utile ?**

Prenons une ville avec deux voisins.

Le voisin A vend l’électricité à un prix relativement bas, tandis que le voisin B est prêt à la payer plus cher.

Votre ville peut avoir intérêt à acheter de l’électricité à A et à en vendre à B.

Cependant, **le Ministère n’achète pas simplement de l’électricité pour revendre ces mêmes unités importées**.

L’achat réduit plutôt la quantité d’électricité que votre ville doit produire pour ses propres bâtiments.

Cela peut libérer de la capacité de production locale pour une vente rentable.

**Exemple : l’électricité**

Imaginons l’arrangement suivant :

- Votre ville a besoin de 10 000 unités d’électricité.
- Elle achète 3 000 unités au voisin A.
- Ses propres centrales couvrent le reste de la demande locale et de la réserve.
- La capacité de production locale disponible permet une vente au voisin B.

Le Ministère évalue si le coût d’achat, le revenu de la vente et les dépenses de centrales qui en résultent produisent un meilleur résultat global.

La réserve d’électricité normale et toutes les autres règles de sécurité s’appliquent toujours.

**Et les déchets ?**

Les déchets peuvent aussi tirer parti d’un arrangement mixte.

Par exemple :

- Le voisin A paie votre ville pour qu’elle accepte ses déchets.
- Votre ville paie le voisin B pour éliminer une partie de ses propres déchets.

Si le revenu de la réception des déchets dépasse les coûts supplémentaires de gestion des déchets et d’exportation, la combinaison peut valoir la peine.

Le Ministère évalue le calendrier d’élimination complet pour s’assurer que la ville peut traiter les déchets qui en résultent.

#### Quelles restrictions s’appliquent encore ?

Activer cette option ne supprime **pas** les protections normales.

- Les achats ne peuvent pas dépasser la demande propre de votre ville.
- L’électricité ou l’eau importée ne sert jamais de base à une revente.
- Les réserves d’électricité et d’eau configurées restent obligatoires.
- Les importations de déchets doivent rester dans les limites d’élimination permises.
- Chaque contrat doit respecter les règles du jeu en matière d’éligibilité, de quantité et de trésorerie.
- Les services locaux doivent rester réalisables.
- Une ressource dont la gestion automatique des accords est désactivée n’est jamais échangée par le Ministère.

**Une ville voisine ne peut pas être à la fois votre fournisseur et votre client pour la même ressource.**

Par exemple, le voisin A peut vendre de l’électricité à votre ville ou lui en acheter, mais pas les deux dans le même plan.

Des voisins différents peuvent jouer des rôles différents.

#### Comment le Ministère choisit-il le rôle de chaque voisin ?

Lorsqu’une ville voisine pourrait potentiellement échanger dans les deux sens, le Ministère évalue les affectations possibles.

Jusqu’à **quatre voisins** de ce type, il peut évaluer chaque affectation de façon exacte.

Quatre voisins créent au plus :

```math
2^4=16
```

affectations achat ou vente possibles.

Chaque affectation est ensuite évaluée avec les règles d’optimisation des contrats normales.

Pour éviter un calcul excessif, le Ministère revient à l’échange à sens unique lorsque :

- Plus de **quatre voisins** nécessitent une affectation achat ou vente.
- La recherche mixte sur les déchets dépasserait **4 000 combinaisons achat par vente**.

Dans ces situations, le Ministère utilise l’optimisation ordinaire à sens unique pour cet examen.

**L’option élargit les choix économiques offerts au Ministère sans permettre un échange illimité ni contourner les règles de sécurité.**

<a id="neighbor-deals-execution"></a>
#### Étape 4 : appliquer les changements de contrats et de financement en toute sécurité

Trouver la meilleure combinaison de contrats n’est qu’une partie du travail.

Le Ministère doit aussi appliquer les changements dans un ordre qui n’interrompt pas les services de votre ville.

Par exemple, imaginons qu’il veuille acheter de l’électricité à un voisin et réduire le financement des centrales.

**Réduire la production avant que l’achat soit disponible pourrait provoquer une panne de courant.**

De même, vendre de l’eau supplémentaire avant d’augmenter la production locale pourrait laisser vos propres bâtiments sans approvisionnement suffisant.

Le Ministère coordonne le financement local et les contrats régionaux en six phases d’exécution.

#### Les six phases d’exécution

| Phase | Action | Pourquoi cet ordre |
|---|---|---|
| **1** | Augmenter la capacité de production locale | Rendre disponible un approvisionnement local supplémentaire avant qu’il soit nécessaire. |
| **2** | Réduire ou annuler les ventes | Libérer la capacité actuellement engagée auprès des voisins. |
| **3** | Créer ou augmenter les achats | Établir l’approvisionnement de remplacement avant de réduire la production locale. |
| **4** | Créer ou augmenter les ventes | Ne commencer de nouvelles exportations qu’une fois l’approvisionnement nécessaire disponible. |
| **5** | Réduire la capacité de production locale | Baisser le financement une fois le nouvel arrangement soutenu. |
| **6** | Réduire ou annuler les achats | Retirer l’approvisionnement importé seulement quand l’arrangement restant peut soutenir la ville. |

**Le principe directeur est que la transition doit être sûre, pas seulement l’arrangement final.**

Pour l’électricité et l’eau, le besoin d’approvisionnement local dépend de la demande propre de la ville, des ventes et des achats.

Sous forme simplifiée :

```math
\text{Besoin local}=\text{Besoin propre}+\text{Ventes}-\text{Achats}
```

Le calcul complet inclut aussi la réserve configurée et, pour l’électricité, le compteur de besoin distinct du jeu.

L’ordre d’exécution garantit que les changements intermédiaires n’exigent pas plus de capacité locale que l’arrangement de départ ou final.

#### Exemple : remplacer l’électricité locale par un achat

Supposons que les centrales de votre ville fournissent actuellement tout en local.

Le Ministère constate qu’acheter de l’électricité à un voisin réduirait les dépenses totales.

Il procède dans cet ordre :

1. Établir le nouvel achat d’électricité.
2. Vérifier que le jeu a accepté le contrat.
3. Recalculer le besoin de capacité locale.
4. Réduire le financement des centrales quand cela est sûr.

**Il ne coupe pas vos centrales avant d’avoir confirmé l’approvisionnement de remplacement.**

#### Exemple : remplacer un achat d’électricité par une production locale

Supposons maintenant que l’inverse devienne plus économique.

Votre ville veut cesser d’acheter de l’électricité et la produire localement à la place.

Le Ministère :

1. Augmente le financement nécessaire des centrales.
2. Vérifie que la capacité de production locale est disponible.
3. Réduit ou annule l’achat d’électricité.

**Il n’annule pas l’approvisionnement importé avant que la capacité locale de remplacement existe.**

#### Y a-t-il des exceptions à l’ordre des phases ?

Oui. Deux changements peuvent être avancés à la phase 2 lorsque le Ministère a confirmé qu’ils ne peuvent pas compromettre le service local :

- **Réduire un achat** lorsque la capacité locale confirmée de la ville est déjà suffisante sans lui.
- **Créer ou augmenter une vente** que le jeu propose actuellement, lorsque la capacité locale confirmée existante suffit déjà à la soutenir.

Dans ces cas, la capacité est déjà disponible. Il n’y a pas besoin d’attendre une hausse de financement inutile.

#### Le Ministère applique-t-il les six phases d’un coup ?

Non.

**Chaque examen n’exécute que la première phase en attente pour chaque ressource, avec un seul changement de contrat à la fois.**

Le Ministère relit ensuite l’état mis à jour de la ville avant de poursuivre lors d’un examen ultérieur.

Cela évite que plusieurs changements non vérifiés soient appliqués ensemble.

#### Que se passe-t-il pour les ventes qui exigent d’abord une production supplémentaire ?

Comme expliqué plus haut, le Ministère peut repérer une vente d’électricité ou d’eau potentiellement rentable qui n’est pas encore disponible à la quantité souhaitée.

On l’appelle une *vente latente*.

Par exemple, une vente peut devenir légale après une augmentation du financement des centrales.

Le Ministère doit d’abord augmenter la capacité de production.

Ce n’est qu’une fois que le jeu reconnaît cette capacité et autorise le contrat voulu que la vente peut être créée.

**Une vente latente n’est jamais signée avant que le jeu la permette.**

Il existe aussi une protection contre un financement inutile.

Si le Ministère a prévu une vente latente mais qu’il n’y a aucune hausse de capacité ni réduction de vente correspondante pour la rendre possible, il recalcule le plan sans cette quantité latente.

**Votre ville n’est pas délibérément laissée à payer une capacité de production supplémentaire pour soutenir une vente qui ne peut pas avoir lieu.**

<a id="neighbor-deals-garbage"></a>
#### Règles particulières pour les contrats de déchets

Les contrats de déchets demandent des précautions supplémentaires, car les déchets sont livrés selon un calendrier quotidien plutôt que comme un approvisionnement mensuel continu.

#### Envoyer vos déchets à un voisin

Exporter des déchets est traité comme l’achat d’une capacité d’élimination.

Le Ministère limite la quantité envoyée aux voisins à :

- La production de déchets propre à la ville après recyclage.
- La quantité maximale permise par les règles d’achat du jeu.

Il n’exporte pas plus de déchets que la ville n’en produit elle-même après recyclage.

#### Recevoir des déchets d’un voisin

Importer des déchets génère des revenus, mais ajoute des déchets que votre système de gestion des déchets doit éliminer.

Le Ministère inclut les déchets entrants **en plus de la production propre de votre ville et de sa réserve de traitement des déchets**.

Il n’accepte un contrat que si :

- Le financement de la gestion des déchets choisi peut traiter les déchets supplémentaires.
- Le mois projeté se termine **sans aucun déchet non collecté**.
- Le revenu justifie toute dépense supplémentaire de gestion des déchets.
- Les limites de réception de déchets du jeu sont respectées.
- La capacité d’élimination restante de la ville est suffisante.

**Le Ministère n’accepte pas des déchets simplement parce qu’un voisin est prêt à payer pour cela.**

La ville doit pouvoir traiter ces déchets en toute sécurité et de façon économique.

#### Pourquoi le Ministère évalue-t-il chaque contrat de déchets séparément ?

Deux contrats peuvent avoir le même montant mensuel combiné mais des calendriers de livraison quotidiens différents.

Par exemple, un gros contrat peut livrer un peu différemment de deux contrats plus petits, parce que le jeu arrondit indépendamment la quantité quotidienne de chaque contrat.

Le Ministère simule donc chaque contrat avec :

- Son montant mensuel.
- Sa quantité de livraison quotidienne.
- Le montant déjà livré pendant le mois.
- Les livraisons restantes.

**Il ne remplace jamais les calendriers de livraison individuels par un seul total mensuel combiné.**

C’est essentiel pour prédire si les décharges et les installations de traitement peuvent évacuer les déchets avant la fin du mois.

#### Pourquoi certains contrats de déchets sont-ils conservés jusqu’au mois suivant ?

Les contrats de déchets terminent généralement leurs livraisons mensuelles pendant la dizaine de premiers jours.

Supposons qu’un contrat ait déjà livré toute son allocation.

Le remplacer par un nouveau contrat pendant le même mois peut introduire un nouveau calendrier de livraison que la simulation du mois en cours n’avait pas anticipé.

Lorsque le Ministère ne peut pas prédire de façon fiable le comportement des décharges le mois suivant, il **conserve donc un contrat déjà épuisé jusqu’au changement de mois**, plutôt que de le remplacer inutilement.

Cette restriction peut être assouplie lorsque conserver le contrat empêcherait la ville de maintenir son service de gestion des déchets.

#### Les déchets importés le mois dernier comptent-ils comme production locale ?

Non.

Le compteur du jeu *Déchets importés le mois dernier* indique les livraisons passées.

**Le Ministère ne compte jamais ces importations comme des déchets produits par votre propre ville.**

La production locale est calculée à partir des valeurs Produits et Recyclés de la ville.

Les déchets étrangers entrent dans le plan par leurs contrats individuels.

<a id="neighbor-deals-verification"></a>
#### Étape 5 : vérifier chaque contrat avant et après sa modification

Le Ministère n’applique pas aveuglément un plan de contrats calculé plus tôt.

**Chaque changement de contrat individuel doit être revérifié immédiatement avant d’être écrit.**

Cela protège contre les changements de demande, de capacité, de trésorerie, d’éligibilité des contrats et des relevés régionaux propres au jeu.

#### Avant de modifier un contrat

**1. Actualiser les informations régionales**

Le Ministère demande à SimCity 4 d’actualiser ses relevés de ressources régionaux, comme le jeu le fait lors de ses mises à jour mensuelles normales.

Il lit ensuite l’état actuel de la ville et calcule un nouveau plan de contrats.

Le changement prévu doit toujours correspondre au nouveau plan :

- Le même type de contrat et la même ville voisine.
- La même quantité de départ.
- La même quantité cible.
- La même première phase d’exécution en attente.
- La justification économique requise.

**Si le changement proposé ne passe plus ces vérifications, rien n’est écrit.**

Le Ministère ne force pas une décision de contrat périmée sur une ville qui a changé.

**2. Vérifier le prix réel du jeu**

Pour un nouveau contrat ou un changement de quantité, le Ministère demande à SimCity 4 son prix de référence.

Il le compare au montant prédit par sa propre formule de prix.

**Tolérance du prix d’un contrat**

```math
\left|P_{\mathrm{game}}-P_{\mathrm{model}}\right|
\le
\max\left(
2,\left\lfloor\frac{P_{\mathrm{model}}}{100}\right\rfloor
\right)
```

Où :

- **Pgame** est le prix mensuel renvoyé par SimCity 4.
- **Pmodel** est le prix calculé par le Ministère.

La différence permise est d’au moins **2 §**, et passe à environ **1 %** pour les contrats plus importants.

**Exemple**

Supposons que le Ministère prédise un prix mensuel de **1 874 §**.

La tolérance est :

```math
\max(2,\lfloor1874/100\rfloor)=18
```

Le jeu doit renvoyer un prix compris entre **1 856 § et 1 892 §**, bornes incluses.

| Prix indiqué par le jeu | Résultat |
|---|---|
| 1 855 § | Rejeté |
| 1 856 § | Accepté |
| 1 874 § | Accepté |
| 1 892 § | Accepté |
| 1 893 § | Rejeté |

**Et si la différence est trop grande ?**

Le Ministère considère que son modèle de prix n’est pas fiable pour la ville actuelle.

Il arrête la gestion automatique des contrats avec les voisins pour le reste de cette session de ville.

**La gestion locale du financement de l’électricité, de l’eau et de la gestion des déchets peut continuer indépendamment.**

Cela empêche le Ministère de prendre des décisions financières à partir de prix que le jeu ne confirme pas.

**3. Vérifier le contrat exact**

Le Ministère retrouve le contrat en cours grâce à son **type et à sa ville voisine**, au lieu de se fier à une position mémorisée plus tôt dans la liste des contrats.

Il refuse le changement lorsque :

- Plus d’un contrat correspond à la même identité.
- Le contrat est suspendu.
- La quantité actuelle ne correspond plus à celle utilisée dans le plan.
- La quantité cible demandée n’est plus légale.

Une cible doit être l’une des quantités permises par le jeu, ou un multiple autorisé de l’incrément de proposition dans le maximum actuel du jeu.

#### Une règle particulière pour les déchets reçus

Il existe un ajustement prudent pour les déchets reçus des voisins.

Supposons que le Ministère ait prévu d’accepter 10 000 unités, mais que le jeu n’en autorise plus que 8 000.

Si 8 000 est une quantité légale, le Ministère peut réduire la quantité de déchets reçus prévue à cette valeur inférieure.

Il choisit la **plus grande quantité légale ne dépassant pas la cible d’origine**.

C’est sans danger, car accepter moins de déchets étrangers ne peut pas alourdir la charge d’élimination de la ville.

**Le même ajustement n’est pas appliqué aux achats.**

Par exemple, réduire silencieusement un achat d’électricité prévu pourrait laisser la ville sans approvisionnement suffisant.

Le Ministère ne substitue donc pas une quantité d’achat plus petite sans recalculer le plan.

#### Après la modification d’un contrat

Une opération de contrat réussie ne signifie pas que tous les compteurs du jeu sont déjà à jour.

Le Ministère attend une confirmation avant de considérer le changement comme terminé.

Il vérifie que :

1. **Le contrat a le bon état :** actif après une création ou un redimensionnement, ou annulé à la quantité zéro ou supprimé après annulation.
2. **La quantité est correcte :** le montant correspond à la valeur demandée.
3. **Le prix mensuel enregistré est mis à jour.**
4. **Les quantités régionales sont cohérentes :** les totaux Importé et Exporté concordent avec la liste des contrats.
5. **Les totaux financiers sont cohérents :** les revenus et dépenses mensuels des accords avec les voisins concordent avec les contrats actifs et suspendus, selon les règles comptables du jeu.

**Le jeu peut mettre à jour ces valeurs légèrement après l’opération de contrat.**

Si les nouvelles valeurs ne sont pas encore apparues, le Ministère attend.

Une mise à jour retardée n’est pas immédiatement traitée comme une transaction échouée.

Pendant qu’un changement de contrat attend sa confirmation :

- Aucun autre changement de contrat n’est fait pour cette ressource.
- La transaction en attente est revérifiée chaque jour simulé.
- Les autres ressources peuvent poursuivre leur gestion indépendante.

**Le Ministère ne continue qu’une fois le changement de contrat précédent confirmé.**

<a id="neighbor-deals-revenue"></a>
#### Comment le Ministère mesure-t-il les économies des contrats ?

Le Ministère distingue trois étapes d’une décision de contrat.

| Étape | Ce que cela signifie |
|---|---|
| **Projeté** | L’avantage mensuel attendu d’un plan de contrats, calculé avant d’appliquer les changements. |
| **En attente** | L’avantage de trésorerie attendu d’un changement de contrat qui a été écrit mais attend encore sa vérification. |
| **Réalisé** | La variation des revenus et dépenses mensuels des accords avec les voisins confirmée par les compteurs budgétaires propres au jeu. |

Ces distinctions empêchent le Ministère de présenter un contrat prévu ou non vérifié comme de l’argent déjà gagné.

#### Économies projetées

Avant de modifier les contrats, le Ministère estime de combien l’arrangement proposé serait meilleur que de conserver les contrats existants.

Il calcule deux valeurs :

- **Avantage de trésorerie :** la variation attendue des revenus et dépenses réels.
- **Avantage économique :** l’avantage de trésorerie incluant l’usure estimée des centrales, lorsque cette option est activée.

Cette comparaison est faite lorsque conserver les contrats existants est une alternative réalisable.

Un contrat prévu ou une vente latente n’est **pas compté comme un revenu réalisé** simplement parce qu’il figure dans un portefeuille optimisé.

#### Économies en attente

Une fois qu’un changement de contrat a été écrit, mais avant que le jeu le confirme, le Ministère suit sa marge de trésorerie attendue comme **en attente**.

Cela reste une estimation.

Elle ne devient un résultat réalisé qu’une fois l’état du contrat et les compteurs financiers du jeu vérifiés.

#### Revenus de contrats réalisés

Après la confirmation, le Ministère compare les revenus et dépenses mensuels des accords avec les voisins du jeu avant et après le changement.

**Résultat réalisé d’un contrat**

```math
\Delta_{\mathrm{realized}}=
(I_{\mathrm{after}}-X_{\mathrm{after}})
-
(I_{\mathrm{before}}-X_{\mathrm{before}})
```

Où :

- **Iavant** et **Iaprès** sont les revenus mensuels totaux des accords avec les voisins du jeu avant et après le changement.
- **Xavant** et **Xaprès** sont les dépenses mensuelles correspondantes des accords avec les voisins.
- **Δréalisé** est la variation vérifiée du revenu net mensuel des contrats.

**Exemple : une nouvelle vente d’électricité**

Supposons que les chiffres mensuels des accords avec les voisins de la ville aient été :

| | Avant | Après |
|---|---:|---:|
| Revenus des contrats | 200 § | 500 § |
| Dépenses des contrats | 100 § | 100 § |
| **Revenu net des contrats** | **100 §** | **400 §** |

La variation réalisée est :

```math
\Delta_{\mathrm{realized}}=
(500-100)-(200-100)=300
```

**Amélioration vérifiée du revenu mensuel des contrats : 300 §.**

C’est la variation du seul revenu des contrats.

Elle ne soustrait pas encore la dépense supplémentaire de centrales nécessaire pour soutenir la vente.

Ces changements sont enregistrés séparément lorsque le Ministère ajuste le financement local.

#### Et si les compteurs financiers ne peuvent pas être lus ?

Si SimCity 4 ne fournit pas de totaux fiables des revenus et dépenses des contrats avant et après, le Ministère utilise la marge de trésorerie prévue en solution de repli.

**Le journal consigne explicitement qu’une estimation a été utilisée à la place d’une mesure financière vérifiée.**

Les deux ne sont pas présentées silencieusement comme équivalentes.

#### Et les économies dues à la réduction du financement local ?

Un achat régional peut créer deux effets financiers distincts.

Par exemple, acheter de l’électricité peut :

1. Ajouter une dépense de contrat mensuelle.
2. Permettre au Ministère de réduire le financement des centrales.

Le premier effet relève de la comptabilité des accords avec les voisins.

Le second relève de la gestion du financement de l’électricité.

**Le Ministère les enregistre séparément pour éviter de compter deux fois la même économie.**

De même, un contrat de déchets peut augmenter les revenus des contrats tout en exigeant un financement supplémentaire de la gestion des déchets. Ce sont des changements budgétaires distincts.

**L’objectif du Ministère est de sélectionner les contrats d’après leur effet économique complet, de les appliquer sans perturber les services essentiels, et de distinguer les avantages attendus des changements réellement confirmés par le jeu.**
<a id="neighbor-deals-reevaluation"></a>
#### Quand le Ministère reconsidère-t-il les contrats existants ?

Le Ministère ne suppose pas qu’un contrat existant doit se poursuivre indéfiniment.

**À chaque examen, il recalcule la valeur économique de tous les contrats avec les dernières informations de la ville.**

Par exemple :

- Une vente d’électricité peut devenir moins rentable à mesure que la demande de votre ville augmente.
- Un achat d’eau peut ne plus être nécessaire après la construction d’une autre installation de pompage.
- Une importation de déchets peut devenir trop coûteuse à traiter si vos décharges approchent de leurs limites.

Quand la situation change, le Ministère peut augmenter, réduire ou annuler des contrats pour trouver un meilleur arrangement.

**Les besoins propres de votre ville et ses réserves de sécurité ont toujours priorité.**

Si la demande d’électricité augmente, par exemple, le Ministère réduit ou annule les ventes d’électricité avant de les laisser consommer la capacité nécessaire à votre ville et à sa réserve.

#### Le Ministère se souvient-il des échecs précédents ?

Normalement, chaque examen repart de zéro. Il n’y a aucun délai de maintien ni temps de repos général pour les contrats.

Cependant, deux exceptions évitent de répéter des actions que le jeu a déjà refusées.

**1. Un nouveau contrat a été refusé par le jeu**

Si SimCity 4 refuse de créer un contrat, le Ministère ne retente pas cette même création avant le **troisième changement de mois simulé après le refus**.

Cela laisse aux conditions d’éligibilité du jeu, comme la trésorerie, la demande et les connexions, le temps de changer.

**2. Le jeu refuse d’annuler un contrat dormant**

Un contrat dormant est un contrat actif avec une quantité de zéro.

Si le jeu refuse son annulation, le Ministère le laisse tranquille pour le reste de la session de ville en cours plutôt que de retenter sans cesse la même opération.

<a id="neighbor-deals-emergency"></a>
#### Que se passe-t-il lors d’une grave pénurie d’approvisionnement ?

Parfois, une ville ne peut tout simplement pas obtenir assez d’électricité ou d’eau pour satisfaire tous ses besoins.

Par exemple :

- Une grande centrale électrique est démolie.
- Plusieurs installations d’eau cessent de produire.
- Une connexion régionale est perdue.
- Une ville voisine ne peut plus fournir une ressource sous contrat.

Normalement, le Ministère recherche des contrats qui satisfont à la fois votre demande et la réserve de sécurité configurée.

**Mais que se passe-t-il si aucune combinaison légale de production locale et d’achats régionaux ne peut y parvenir ?**

Dans ce cas, le Ministère change de priorités.

Au lieu de rejeter tous les arrangements possibles parce qu’aucun ne satisfait la totalité du besoin, **il essaie de couvrir autant que possible la demande réelle de la ville**.

#### Quelle part de la demande peut encore être couverte ?

**Demande couverte en situation d’urgence**

```math
\mathrm{served}(B,S)=
D-\max\left(
0,\max(0,D-B)+S-C_{\max}
\right)
```

Où :

- **D** est la demande réelle de la ville.
- **B** est la quantité achetée aux villes voisines.
- **S** est la quantité vendue aux villes voisines.
- **Cmax** est la plus grande capacité locale de production ou de pompage que les installations de la ville peuvent légalement fournir.

La formule détermine quelle part de la demande reste couverte après prise en compte des importations, des exportations et de la capacité locale maximale disponible.

**Exemple : une ville confrontée à une pénurie d’électricité**

Supposons que votre ville ait besoin de **10 000 unités**, mais que ses centrales ne puissent en produire que **6 000**.

Sans importations, la ville ne peut pas couvrir 4 000 unités de sa demande.

Si une ville voisine peut légalement en fournir 3 000, les acheter porte la demande couverte à **9 000 unités**.

La ville manque toujours d’électricité, mais le Ministère a réduit le déficit.

**En situation d’urgence, le Ministère suit ces priorités :**

1. **Couvrir autant de demande réelle que possible.**
2. **Rétablir la réserve de sécurité dans la mesure du possible.**
3. **Choisir l’arrangement le moins coûteux parmi des solutions également efficaces.**

La réserve normale est donc toujours prise en compte, mais le service réel passe en premier lorsqu’il n’y a pas assez de capacité pour tout satisfaire.

#### Qu’advient-il des ventes d’électricité et d’eau ?

Le Ministère annule toutes les ventes qu’il a légalement le droit de modifier.

Cela libère de la capacité pour votre propre ville.

Il considère ensuite les quantités d’achat légales disponibles auprès des villes voisines.

Normalement, les achats ne peuvent pas dépasser la demande propre de la ville.

**En situation d’urgence, un palier de quantité supplémentaire au-delà de la demande peut être autorisé**, mais seulement si ce palier augmente de plus de 1 % la demande réellement couverte.

C’est une exception limitée, destinée à gérer les choix de quantités de contrat discrets du jeu.

Les contrats suspendus sont toujours inclus dans les calculs, mais le Ministère ne tente pas de les modifier.

#### Et en cas d’urgence d’élimination des déchets ?

Pour les déchets, le Ministère suit le principe équivalent.

Il tente de :

- Annuler les contrats qui amènent des déchets supplémentaires dans votre ville.
- Augmenter les exportations de déchets vers les villes voisines.
- Utiliser d’abord les arrangements d’élimination disponibles les moins coûteux.

Les exportations de déchets restent soumises aux quantités légales du jeu et à la quantité de déchets que votre ville produit après recyclage.

**Le Ministère ne peut pas garantir que toutes les pénuries seront résolues.**

Si les installations et les contrats disponibles sont insuffisants, le joueur doit fournir des infrastructures supplémentaires ou régler le problème sous-jacent.

Le Ministère n’émet un avis de pénurie que lorsque le problème est **confirmé et persistant**, au lieu de vous avertir à chaque fluctuation temporaire.

Voir [Avis](safety.md#notices).

<a id="neighbor-deals-safety"></a>
### 4. Conditions de sécurité

Le Ministère suit des règles strictes lorsqu’il crée, modifie ou annule des contrats régionaux.

#### Protéger les services propres de votre ville

**Les besoins de votre ville passent en premier.**

Les ventes d’électricité et d’eau ne doivent pas consommer la capacité nécessaire pour servir la ville et maintenir sa réserve configurée.

La définition de l’excédent disponible propre à SimCity 4 n’inclut pas les marges de sécurité du Ministère.

Le Ministère effectue donc ses propres vérifications de capacité avant d’accepter une vente.

#### Ne s’appuyer que sur des achats fiables

Un achat n’est traité comme un approvisionnement fiable que tant qu’il est **correctement actif**.

Le Ministère ne s’appuie pas sur des contrats qui sont :

- En attente.
- Suspendus.
- Défaillants à cause d’une connexion manquante.
- Défaillants parce que la ville payeuse ne peut pas se les permettre.

**La production locale n’est jamais réduite avant que l’achat de remplacement soit en vigueur.**

Aucune règle artificielle n’exige qu’un achat actif ait un nombre de jours restants particulier.

Le Ministère le considère comme utilisable jusqu’à ce que le jeu change son état.

#### Honorer les ventes existantes

Une vente d’électricité ou d’eau active est traitée comme une obligation.

Le Ministère inclut sa quantité complète pour calculer la capacité dont votre ville a besoin.

Le même principe s’applique aux contrats de déchets et à la capacité de gestion des déchets nécessaire pour les soutenir.

Une vente peut être réduite ou annulée lors d’un examen ultérieur, mais **tant qu’elle reste en vigueur, le plan de service local doit en tenir compte**.

#### Respecter les règles de contrat du jeu

Le Ministère n’utilise que :

- Les offres de contrat que SimCity 4 met réellement à disposition.
- Les quantités permises par le système de contrats du jeu.
- Les fonctions propres au jeu pour créer, redimensionner et annuler des contrats.

Il ne modifie jamais directement les villes voisines et ne force jamais un contrat que le jeu rejetterait.

**Les contrats suspendus ne sont jamais modifiés.**

#### S’arrêter en toute sécurité en cas de problème

Si une erreur technique survient pendant une opération de contrat, le Ministère arrête la gestion automatique des contrats pour le reste de la session de ville en cours.

Cela peut arriver lorsque :

- Le jeu refuse de façon inattendue une opération de contrat.
- Un contrat passe dans un état inattendu après avoir été modifié.
- Le prix indiqué par le jeu diffère trop du calcul du Ministère.
- Les informations du contrat obtenu ne peuvent pas être vérifiées de façon fiable.

**Les gestionnaires de financement de l’électricité, de l’eau et de la gestion des déchets peuvent continuer à fonctionner.**

Ils utilisent simplement les contrats régionaux existants comme des engagements fixes.

Si vous rechargez la ville ou désactivez la gestion des contrats alors qu’une transaction n’est pas terminée, le Ministère abandonne toute action ultérieure pour cette transaction.

Une transaction qui n’attend plus que les mises à jour comptables du jeu peut encore être observée jusqu’à ce que ces valeurs se stabilisent, sans effectuer d’appels de contrat supplémentaires.

#### Un seul changement de contrat à la fois

Pour chaque ressource, le Ministère applique **un seul changement de contrat à la fois**.

Il vérifie ensuite l’état du contrat obtenu et les totaux régionaux et financiers du jeu avant de faire un autre changement.

Cela évite que plusieurs opérations de contrat non vérifiées interfèrent entre elles.

#### Qu’advient-il des messages des conseillers du jeu d’origine ?

Tant que le Ministère gère les contrats régionaux, les messages des conseillers du jeu d’origine sur les accords avec les voisins sont supprimés par un fichier de ressources inclus.

Cela comprend :

- Les offres de nouveaux contrats.
- Les messages de renouvellement de contrat.
- Les avertissements de défaut de contrat.

Le Ministère traite les situations correspondantes par son propre système de gestion.

Lorsqu’une véritable pénurie ne peut pas être résolue automatiquement, il peut émettre son propre avis.

Voir [Avis](safety.md#notices).

**Important :** les messages du jeu d’origine sont supprimés par un fichier de ressources chargé au démarrage de SimCity 4. Désactiver la gestion automatique des contrats dans les options ne rétablit pas immédiatement ces messages. La procédure pour les rétablir est expliquée dans [Limites connues](#neighbor-deals-limitations).

<a id="neighbor-deals-settings"></a>
### 5. Réglages

La gestion des accords régionaux se configure dans **Options de jeu → Ministère de l’Optimisation**, sur la page **Services**.

| Réglage | Par défaut | Ce qu’il fait |
|---|---|---|
| [Gérer les accords avec les voisins](options.md#opt-deals) | Activé | Active la gestion automatique des accords régionaux. |
| [Gérer les accords d’électricité](options.md#opt-tradepower) | Activé | Autorise le Ministère à acheter et à vendre de l’électricité. |
| [Gérer les accords d’eau](options.md#opt-tradewater) | Activé | Autorise le Ministère à acheter et à vendre de l’eau. |
| [Gérer les accords de déchets](options.md#opt-tradegarbage) | Activé | Autorise le Ministère à envoyer des déchets aux voisins ou à recevoir les leurs. |
| [Acheter à une ville pour vendre à une autre](options.md#opt-arbitrage) | **Désactivé** | Permet d’acheter et de vendre la même ressource dans un même plan lorsque c’est économiquement avantageux. |
| [Réserve d’électricité](options.md#opt-powerreserve) | 20 % | Protège l’approvisionnement local en électricité. |
| [Réserve d’eau](options.md#opt-waterreserve) | 10 % | Protège l’approvisionnement local en eau. |
| [Réserve de traitement des déchets](options.md#opt-sanitationreserve) | 2 % | Fournit une capacité supplémentaire d’élimination des déchets. |

#### Comment les interrupteurs interagissent-ils ?

**Gérer les accords avec les voisins** est l’interrupteur principal des accords régionaux.

Lorsqu’il est désactivé :

- Le Ministère ne demande pas de nouvelles offres d’accord.
- Il ne crée, ne redimensionne et n’annule aucun accord.
- Les accords existants restent en vigueur et sont pris en compte dans les calculs de financement local.

Les trois interrupteurs par ressource permettent de gérer indépendamment l’électricité, l’eau et les déchets.

**Vous pouvez aussi gérer les accords sans gérer le financement local.**

Par exemple :

| Réglages | Résultat |
|---|---|
| **Gérer les accords d’électricité : Activé** / **Gérer l’électricité : Activé** | Le Ministère peut coordonner les accords d’électricité et le financement des centrales. |
| **Gérer les accords d’électricité : Activé** / **Gérer l’électricité : Désactivé** | Les accords peuvent être gérés, mais le financement des centrales électriques reste inchangé. |
| **Gérer les accords d’électricité : Désactivé** / **Gérer l’électricité : Activé** | Le financement des centrales peut être optimisé, mais les accords d’électricité existants restent inchangés. |
| **Les deux désactivés** | Ni les accords d’électricité ni le financement des centrales ne sont modifiés automatiquement. |

La même indépendance s’applique à l’eau et à la gestion des déchets.

**Si la gestion des accords est activée mais que la gestion du financement local est désactivée**, le Ministère ne peut utiliser que la capacité de production ou d’élimination déjà disponible au financement actuel.

Il n’augmente pas le financement pour soutenir un nouvel accord.

**Si la gestion du financement local est activée mais que la gestion des accords est désactivée**, les achats et les ventes existants sont traités comme des engagements fixes.

Le Ministère peut tout de même surveiller la ressource et vous avertir des pénuries persistantes.

Désactiver **Gérer les accords avec les voisins** conserve les interrupteurs individuels par ressource et le réglage d’arbitrage. Leurs valeurs s’appliquent de nouveau lorsque l’interrupteur principal est réactivé.

#### Réglage avancé : avantage minimal d’un accord

Vous pouvez configurer un avantage mensuel minimal pour les décisions d’accord facultatives.

Ce réglage est disponible dans le fichier INI, pas dans la fenêtre d’options du jeu.

| Réglage INI | Par défaut | Plage autorisée |
|---|---:|---:|
| `MinContractBenefit` | 0 §/mois | 0 à 100 000 §/mois |

Fichier : `DepartmentEfficiencyUtility.ini`

Lorsqu’il est à zéro, **tout avantage économique strictement positif suffit**.

Par exemple, un accord qui améliore de 1 § le résultat économique mensuel peut être retenu.

Si vous indiquez :

```ini
MinContractBenefit=20
```

Les créations d’accords facultatifs et les changements de quantité doivent produire au moins **20 § par mois** d’avantage supplémentaire.

Ce réglage peut être utile si vous préférez moins de changements d’accords, quitte à ignorer de petites économies potentielles.

**Le seuil ne bloque jamais un changement nécessaire au maintien des services essentiels.**

SimCity 4 ne facture pas de frais distincts pour le simple fait de créer, de modifier ou d’annuler un accord régional.

<a id="neighbor-deals-examples"></a>
### 6. Exemples détaillés

Les exemples suivants montrent comment le Ministère évalue les accords régionaux.

Les **formules et les prix des accords du jeu sont calculés selon ses règles réelles**.

En revanche, les coûts d’exploitation locaux indiqués dans ces exemples sont des valeurs illustratives, qui représentent ce que l’optimiseur de financement approprié pourrait calculer pour une ville donnée.

#### Exemple 1 : vendre de l’électricité sans sacrifier la réserve

Prenons deux villes voisines.

**Votre ville (ville A)**

- Demande en électricité : **10 000 unités**.
- Besoin en électricité du jeu : **10 300 unités**.
- Réserve d’électricité : **20 %**.
- Production d’électricité actuelle : **14 000 unités**.
- Dépenses actuelles des centrales électriques : **1 400 § par mois**.

**Ville voisine (ville B)**

- Production d’électricité : **3 000 unités**.
- Besoin en électricité : **6 000 unités**.
- Trésorerie : **250 000 §**.

#### Étape 1 : qu’autorise SimCity 4 ?

La ville A dispose d’un excédent de :

```math
14000-10300=3700
```

La capacité d’achat de la ville B est de :

```math
2.5\times6000-3000=12000
```

La quantité maximale d’accord du jeu est donc :

```math
Q_{\max}=\min(3700,12000)=3700
```

La ville B satisfait aux exigences de proposition du jeu.

Le prix minimal de l’accord est d’environ **110 § par mois** pour 1 000 unités, et la trésorerie de la ville B dépasse largement les cinq années de paiements exigées.

**SimCity 4 accepte de proposer un achat d’électricité à la ville A.**

#### Étape 2 : qu’exige le Ministère ?

Le Ministère applique sa propre réserve d’électricité de 20 %.

Sans vente :

```math
N(0,0)=\max(12000,10300)=12000
```

La ville A doit conserver au moins **12 000 unités de capacité de production locale**.

Chaque unité vendue augmente ce besoin d’une unité.

Le Ministère évalue les quantités de vente candidates avec l’incrément de proposition dérivé de la limite du voisin :

```math
\text{pas}=\max(12000/10,1000)=1200
```

Il examine donc des quantités comme 1 200, 2 400 et 3 600 unités.

Des quantités plus importantes peuvent aussi être envisagées comme *ventes latentes*, c’est-à-dire qu’elles exigeraient une capacité locale supplémentaire avant que le jeu puisse légalement les accepter.

#### Étape 3 : quelle quantité est la plus rentable ?

| Électricité vendue | Revenu mensuel | Coût local illustratif | Coût local supplémentaire | Profit net |
|---:|---:|---:|---:|---:|
| 0 | 0 § | 1 250 § | — | 0 § |
| 1 200 | 132 § | 1 330 § | 80 § | +52 § |
| **2 400** | **264 §** | **1 420 §** | **170 §** | **+94 §** |
| 3 600 | 396 § | 1 560 § | 310 § | +86 § |
| 4 800 (latente) | 527 § | 1 720 § | 470 § | +57 § |

**Le choix le plus rentable est de vendre 2 400 unités.**

Cela apporte une amélioration estimée à **94 § par mois**.

Toutefois, la vente exige :

```math
12000+2400=14400
```

**14 400 unités de capacité de production locale.**

La ville A ne produit actuellement que 14 000 unités.

#### Étape 4 : comment le plan est-il appliqué ?

Le Ministère ne signe pas la vente immédiatement.

D’abord, il augmente le financement des centrales électriques pour atteindre la capacité requise de 14 400 unités.

Une fois que le jeu a pris en compte la production supplémentaire, l’accord est réexaminé lors d’une évaluation ultérieure.

Avec 14 400 unités de production, l’excédent disponible dans le jeu devient :

```math
14400-10300=4100
```

La vente prévue de 2 400 unités se situe désormais dans le maximum autorisé, et elle peut être conclue si le plan actualisé la retient toujours.

**Le Ministère augmente la production avant de signer la vente, ce qui protège la réserve de la ville pendant toute la transition.**

Sans la protection de réserve du Ministère, le joueur aurait pu vendre les 3 700 unités autorisées par les calculs d’origine du jeu, sans laisser aucune marge de réserve.

#### Exemple 2 : acheter de l’électricité et économiser davantage grâce à la réserve

Prenons une autre ville, la **ville C**.

- Demande en électricité : **10 000 unités**.
- Besoin du jeu : **10 300 unités**.
- Production actuelle : **12 500 unités**.
- Réserve d’électricité : **20 %**.

Son fournisseur voisin dispose de :

- Production : **30 000 unités**.
- Besoin : **20 000 unités**.
- Coût unitaire moyen : **0,08 §**.

Le fournisseur a 10 000 unités d’excédent.

La capacité d’achat de l’acheteur est de :

```math
2.5\times10300-12500=13250
```

Le maximum du jeu est donc de **10 000 unités**, et les quantités proposées augmentent par paliers de 1 000.

La limite d’achat propre au Ministère est elle aussi de 10 000 unités.

#### Étape 1 : de quelle capacité locale a-t-on besoin ?

Sans achats, la réserve de 20 % de la ville exige :

```math
N(0,0)=12000
```

Acheter de l’électricité réduit la quantité qui doit être produite localement.

**Chaque unité achetée peut réduire le besoin de capacité locale de 1,2 unité**, tant que l’exigence liée à la réserve reste contraignante.

#### Étape 2 : comparer les achats possibles

| Unités achetées | Capacité locale requise | Coût local illustratif | Économie locale | Prix de l’accord | Économie nette |
|---:|---:|---:|---:|---:|---:|
| 0 | 12 000 | 1 250 § | — | — | 0 § |
| 1 000 | 10 800 | 1 160 § | 90 § | 89 § | +1 § |
| **2 000** | **9 600** | **1 060 §** | **190 §** | **179 §** | **+11 §** |
| 3 000 | 8 400 | 975 § | 275 § | 267 § | +8 § |
| 4 000 | 7 200 | 900 § | 350 § | 356 § | −6 § |

**L’arrangement globalement le moins cher est d’acheter 2 000 unités.**

L’achat coûte **179 § par mois**, mais permet de réduire de **190 §** les dépenses locales des centrales électriques.

Économie nette estimée :

```math
190-179=11
```

**11 § par mois.**

#### Étape 3 : appliquer les changements en toute sécurité

Le Ministère crée et vérifie d’abord l’achat.

Il réduit ensuite le financement des centrales électriques lors de la phase d’exécution ultérieure appropriée.

**Et si l’avantage minimal est fixé à 20 § ?**

Avec :

```ini
MinContractBenefit=20
```

L’accord facultatif n’est pas créé.

Son avantage mensuel de 11 § est inférieur au seuil configuré.

Ce réglage vous permet d’éviter les petits ajustements économiques si vous préférez un ensemble d’accords plus stable.

#### Exemple 3 : pourquoi un accord ne peut pas toujours être redimensionné à n’importe quelle quantité

Supposons que votre ville ait déjà une vente d’électricité avec :

- Quantité actuelle : **3 088 unités**.
- Incrément de l’accord : **1 544 unités**.
- Maximum du jeu : **15 442 unités**.

Les quantités sélectionnables dans le jeu suivent l’incrément :

1 544 ; 3 088 ; 4 632 ; 6 176 ; et ainsi de suite, jusqu’à 15 440.

**Une quantité d’accord de 6 000 unités n’est pas disponible.**

La quantité autorisée la plus proche au-dessus est **6 176 unités**.

Le Ministère respecte ces choix de quantités légaux plutôt que de soumettre un nombre arbitraire au jeu.

#### Exemple 4 : accepter les déchets d’une autre ville

Une ville voisine propose de vous payer pour éliminer **20 000 unités de déchets par mois**.

Le jeu livre normalement :

```math
20000\times0.10=2000
```

**2 000 unités par jour simulé**, pendant environ dix jours.

Le Ministère évalue si vos installations de traitement des déchets peuvent absorber ces 20 000 unités supplémentaires.

Il prend en compte :

- La production de déchets propre à votre ville après recyclage.
- Votre réserve de traitement des déchets configurée.
- Les déchets déjà non collectés.
- Les importations et exportations existantes.
- La capacité de traitement des incinérateurs et des centrales de valorisation énergétique des déchets.
- La capacité d’accueil disponible des décharges.
- Le calendrier de livraison quotidien de chaque accord.

**L’accord n’est accepté que si le mois projeté se termine sans déchets non collectés.**

Les revenus doivent aussi justifier tout financement supplémentaire de la gestion des déchets nécessaire pour traiter les déchets entrants.

Enfin, le total des déchets reçus doit rester inférieur à la limite autorisée par le jeu.

Dans l’exemple de la section 1, cette limite était d’environ **541 667 unités**.

**Un accord qui paie bien est tout de même rejeté si votre ville ne peut pas traiter les déchets supplémentaires en toute sécurité.**

<a id="neighbor-deals-limitations"></a>
### 7. Limites connues

Le Ministère peut gérer automatiquement les accords régionaux, mais il reste soumis aux règles d’origine de SimCity 4 et aux informations disponibles.

#### Villes voisines et informations régionales

- **Les données des voisins sont des instantanés.** SimCity 4 ne simule pas les villes voisines pendant que vous jouez la ville actuelle. Le Ministère utilise les chiffres régionaux enregistrés en dernier pour ces voisins, qui peuvent être périmés.
- **Le Ministère ne modifie jamais les villes voisines.** Il ne les charge pas, ne modifie pas leurs bâtiments et ne met pas à jour leurs informations enregistrées.
- **Les dates des accords restent contrôlées par SimCity 4.** Le jeu gère les dates de début, l’expiration et le renouvellement des accords. Le Ministère n’essaie ni de les prévoir ni de les modifier. Lorsqu’un accord se termine ou est suspendu, son nouvel état est pris en compte à l’évaluation suivante.
- **Les nouvelles ventes exigent une offre existante.** Le Ministère peut augmenter la production de votre ville pour soutenir une vente plus importante à un voisin qui propose déjà cette ressource. Il ne peut pas faire apparaître un tout nouvel acheteur lorsque le jeu n’a aucune offre de ce voisin.

#### Application des changements d’accords

- **Une seule étape est appliquée par évaluation.** Chaque ressource effectue au plus un changement d’accord ou une étape de financement local par évaluation. Une réorganisation importante peut donc demander plusieurs évaluations.
- **Chaque prix d’accord est vérifié.** Si le prix de référence du jeu s’écarte du modèle du Ministère au-delà de la tolérance admise, la gestion des accords s’arrête pour le reste de la session de la ville.
- **Les accords suspendus ne peuvent pas être modifiés.** Le jeu contrôle leur résolution, et le Ministère doit composer avec eux.
- **Les recherches sur de grands accords de déchets ont des limites.** Si le Ministère ne peut pas évaluer de façon exhaustive les combinaisons légales parce qu’il y en a plus de cinq millions d’un côté, il ne fait aucun changement d’accord spéculatif. Les recherches mixtes d’achat et de vente ont aussi leurs propres limites, décrites plus haut.

#### Messages d’origine des conseillers sur les accords avec les voisins

Le Ministère inclut un fichier de ressources qui supprime les messages d’origine des conseillers de SimCity 4 sur les accords avec les voisins.

Ce fichier est :

`DepartmentEfficiencyDealAdvice.dat`

**SimCity 4 charge les fichiers de ressources au démarrage du jeu.**

Par conséquent, désactiver les options de gestion des accords du Ministère ne rétablit pas immédiatement les messages d’origine des conseillers.

Si vous voulez gérer les accords manuellement et rétablir les messages d’origine :

1. Fermez SimCity 4.
2. Retirez `DepartmentEfficiencyDealAdvice.dat` du dossier Plugins du jeu.
3. Relancez le jeu.

Cela rétablit aussi les messages d’origine tels que *Accord non honoré*.

Le Ministère consigne un avertissement dans son journal lorsque le fichier de ressources et les réglages de gestion actuels ne correspondent pas.

#### Propositions en attente

Lorsque le Ministère demande des offres d’accord, il utilise la même opération du jeu que l’ouverture de la fenêtre des accords avec les voisins.

Cela peut faire apparaître dans les listes d’accords du jeu des propositions en attente pour d’autres villes voisines.

Comme dans le jeu d’origine, **les propositions en attente ne génèrent ni revenus ni dépenses** tant qu’elles ne sont pas devenues des accords actifs.

**Le Ministère travaille dans le système d’accords du jeu. Il peut optimiser les choix disponibles, mais il ne peut pas créer de ressources, de connexions, d’éligibilité ni d’informations fiables que SimCity 4 ne fournit pas.**

[Retour au sommaire](index.md#contents) | [Retour au README](../../README.fr.md)

---

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
