[English](../en/safety.md) | [Português](../pt/safety.md) | **Français** | [Español](../es/safety.md)

[Retour au sommaire](index.md#contents) | [Retour au README](../../README.fr.md)

<a id="safety"></a>
## G. Sécurité, automatisation et prise de décision

Le Ministère peut modifier des budgets, gérer des contrats régionaux, déblayer des bâtiments abandonnés et corriger certains problèmes de décharge. Comme ces actions touchent directement votre ville, **la protection des services essentiels et le respect de vos décisions doivent toujours passer avant les économies**.

Ce chapitre explique les règles communes aux fonctions automatiques du Ministère :

- Comment il décide quels objectifs sont prioritaires.
- Quand il lit la ville et recalcule ses plans.
- Comment il vérifie les changements avant de continuer.
- Ce qui se passe quand les informations du jeu sont incomplètes ou incohérentes.
- Comment les défaillances techniques sont contenues.
- Quelles décisions restent sous votre contrôle.
- Comment fonctionnent ses actions automatiques et ses messages de conseillers.

<a id="safety-vanilla"></a>
### 1. Comment SimCity 4 fonctionne sans le Ministère

Dans le jeu d’origine, la plupart des décisions de financement des services et d’échanges régionaux sont manuelles.

#### Budgets des services

Les écoles, les hôpitaux et les centrales électriques ont chacun leurs commandes de financement.

L’eau et le traitement des déchets ont chacun un budget unique de service.

SimCity 4 accepte les changements de financement via ces commandes, mais **ne les optimise pas automatiquement quand la demande évolue**.

#### Statistiques du jeu

La simulation met à jour la demande, la production, la consommation, les capacités et les contrats régionaux selon son propre calendrier interne.

Ces valeurs ne changent pas forcément immédiatement après un ajustement de budget.

Par exemple :

- La capacité de production d’eau peut être mise à jour au prochain mois simulé.
- L’efficacité du traitement des déchets et les capacités de traitement peuvent mettre plusieurs jours simulés à refléter un nouveau financement.
- Certaines statistiques peuvent être à zéro au départ après l’ouverture d’une ville, parce que la simulation ne les a pas encore renseignées.

**Le Ministère doit distinguer un vrai zéro d’une information qui n’est pas prête.**

#### Messages des conseillers

SimCity 4 possède un système de conseillers qui affiche des nouvelles et des avertissements.

Chaque message a un conseiller, une humeur et une priorité.

Selon la difficulté du jeu ou les réglages des conseillers, les messages de haute priorité peuvent s’afficher sous forme de fenêtres contextuelles.

Les seuils de priorité du jeu pour ces fenêtres sont **60, 80 ou 100**, selon le niveau de difficulté choisi.

Les messages sous le seuil apparaissent normalement dans la liste des nouvelles des conseillers, sans ouvrir de fenêtre contextuelle.

Chaque message reste disponible pendant sa durée configurée.

#### Contrats régionaux

Le jeu d’origine propose, renouvelle et annule aussi des contrats régionaux, avec ses propres messages de conseillers.

Le Ministère peut automatiser ces opérations lorsqu’il est activé.

<a id="safety-problem"></a>
### 2. Le problème de la gestion

La gestion automatique peut économiser de l’argent et réduire le travail répétitif, mais elle peut aussi créer des problèmes si les décisions sont prises sans garde-fous suffisants.

Par exemple, un système automatisé pourrait :

- Réduire le financement en dessous du niveau nécessaire pour maintenir un service.
- Vendre de l’électricité ou de l’eau dont la ville a réellement besoin.
- Prendre des décisions à partir de statistiques incomplètes juste après le chargement.
- Appliquer un autre changement avant que le précédent ait pris effet.
- Répéter une opération de contrat ou de financement que le jeu a déjà rejetée.
- Démolir des bâtiments qui devraient être protégés.
- Inonder le joueur de notifications sur des actions de routine.

**Le Ministère est conçu pour éviter d’abord ces problèmes, et optimiser les dépenses ensuite.**

<a id="safety-principles"></a>
### 3. Les règles générales du Ministère

Quand plusieurs objectifs entrent en conflit, le Ministère suit cet ordre de priorités :

1. **Maintenir les services essentiels.** Éviter de laisser la ville sans électricité, eau, traitement des déchets, éducation ou santé.
2. **Protéger les réserves configurées.** Garder une capacité suffisante au-dessus des besoins prévus de la ville.
3. **Respecter les limites de sécurité de la réserve.** Ne pas contourner les règles destinées à protéger la continuité des services.
4. **Respecter les restrictions de SimCity 4.** N’utiliser que les niveaux de financement, les capacités et les quantités de contrat permis par le jeu.
5. **Minimiser les dépenses mensuelles.** Choisir l’arrangement de fonctionnement sûr le moins cher.
6. **Vendre le véritable surplus quand c’est rentable.** Le revenu supplémentaire doit justifier le coût de le produire.
7. **Éviter par défaut d’acheter et de vendre en même temps.** Les échanges mixtes exigent le réglage optionnel.
8. **Ne jamais inventer d’information manquante.** Les valeurs illisibles ou incohérentes ne sont pas traitées comme des mesures valides.
9. **Éviter les délais inutiles.** Une décision sûre et prête ne reçoit pas de délai d’attente artificiel.
10. **Vérifier chaque changement.** Lire le nouvel état, confirmer le résultat et recalculer avant de continuer.

Ces principes s’appliquent aux gestionnaires individuels du Ministère, avec les garde-fous propres à chaque ressource décrits dans les chapitres précédents.

<a id="safety-service-first"></a>
#### Les services essentiels passent toujours en premier

Le Ministère coordonne le financement et les contrats régionaux pour éviter d’interrompre l’approvisionnement pendant une transition.

#### Les changements sont appliqués dans un ordre sûr

Pour l’électricité, l’eau et les déchets, le Ministère utilise six phases d’exécution :

| Phase | Action |
|---|---|
| **1** | Augmenter la capacité locale |
| **2** | Réduire ou annuler les ventes |
| **3** | Créer ou augmenter les achats |
| **4** | Créer ou augmenter les ventes |
| **5** | Réduire la capacité locale |
| **6** | Réduire ou annuler les achats |

**Chaque ressource n’exécute que sa plus ancienne phase en attente lors d’une revue.**

Par exemple, le Ministère ne réduit pas le financement d’une centrale électrique avant que l’achat d’électricité destiné à remplacer cette production soit actif.

Si une augmentation de capacité n’a pas encore pris effet, les phases suivantes de cette ressource attendent.

Il y a une distinction importante pour le traitement des déchets : une réduction de financement peut être reportée, car les écritures du traitement des déchets sont volontairement progressives.

**Une réduction inachevée du traitement des déchets n’empêche pas forcément une réduction ultérieure des achats de traitement des déchets**, car conserver une capacité locale de traitement plus élevée ne peut pas nuire à la continuité du service.

#### Le financement n’est pas réduit pendant une véritable pénurie

Le Ministère distingue une offre insuffisante des problèmes internes au réseau de distribution de la ville.

Pour l’eau, par exemple, il refuse une réduction quand des bâtiments manquent d’eau **et que la capacité de production est inférieure à la demande**.

Si l’eau disponible est suffisante mais que certains bâtiments sont déconnectés du réseau de canalisations, une hausse du financement ne résoudrait pas le problème. Un tel problème de connexion n’empêche pas automatiquement une réduction par ailleurs sûre.

Voir [Conditions de sécurité de l’eau](water.md#water-no-cut).

#### Les ventes s’arrêtent pendant un déficit confirmé d’électricité ou d’eau

Si les compteurs du jeu confirment que la ville n’a pas assez d’électricité ou d’eau, **le Ministère ne maintient pas les ventes de cette ressource**.

La demande locale passe avant les revenus d’exportation.

#### L’éducation et la santé augmentent le financement rapidement

L’éducation et la santé utilisent une autre stratégie de calendrier.

Le Ministère peut augmenter le financement :

- Pendant le premier jour simulé après l’ouverture de la ville.
- Quand un risque sur la capacité d’un service est détecté.
- Lors de contrôles supplémentaires tant qu’une grève ou un risque persistant est présent.

**Les réductions de financement attendent l’évaluation mensuelle.**

Cela permet au Ministère de réagir vite aux écoles ou hôpitaux surchargés tout en évitant des réductions inutiles entre les revues mensuelles.

Voir [Éducation et santé](education_healthcare.md#education-healthcare-timing).

<a id="safety-reserves"></a>
#### Réserves de sécurité : capacité minimale, pas capacité maximale

Chaque service géré a une réserve configurable.

**Une réserve est une exigence minimale, jamais un maximum.**

Le Ministère peut maintenir davantage de capacité quand c’est nécessaire en raison des réglages de financement disponibles, des fluctuations du service ou de ventes régionales rentables.

Il ne doit pas choisir délibérément un plan qui tombe sous l’exigence protégée applicable.

| Service | Réserve par défaut | Plage autorisée | Fonctionnement |
|---|---:|---:|---|
| **Éducation et santé** | 5 % | 0–50 % | Ajoute de la capacité disponible au-dessus de l’utilisation, avec une protection supplémentaire pour les petits établissements ou ceux dont la fréquentation fluctue. |
| **Électricité** | 20 % | 10–50 % | Protège la part de la demande fournie par les centrales électriques locales, tout en respectant aussi l’exigence d’électricité du jeu. |
| **Eau** | 10 % | 10–50 % | Protège la part de la demande fournie par les installations d’eau locales. |
| **Traitement des déchets** | 2 % | 2–20 % | Ajoute une marge aux déchets produits après recyclage, en plus des déchets non collectés existants. |

Voir [Éducation et santé](education_healthcare.md#education-healthcare-required), [Électricité](power.md#power), [Eau](water.md#water-required-capacity) et [Déchets et décharges](garbage.md#garbage-landfills-need).

**Que se passe-t-il si une valeur de réserve est invalide ?**

Si vous modifiez un fichier de configuration à la main et saisissez une réserve hors de sa plage autorisée, le Ministère n’ajuste pas silencieusement la valeur.

**La configuration invalide est rejetée, et le composant de gestion concerné reste désactivé jusqu’à sa correction.**

Cela évite d’appliquer des marges de sécurité inattendues que vous n’avez pas réellement configurées.

<a id="safety-economy"></a>
#### Comment le Ministère minimise-t-il les dépenses ?

Une fois les exigences de service et de sécurité satisfaites, le Ministère cherche l’arrangement adapté le moins cher.

Les services utilisent des méthodes différentes, car SimCity 4 gère leur financement différemment.

| Service | Méthode d’optimisation |
|---|---|
| **Éducation et santé** | Choisir le pourcentage de financement entier le plus bas qui fournit la capacité requise. |
| **Électricité** | Trouver la combinaison la moins chère de niveaux de financement individuels des centrales électriques. |
| **Eau** | Choisir le pourcentage de financement du service le plus bas qui soit sûr. |
| **Traitement des déchets** | Simuler le traitement des déchets sur les jours restants du mois et trouver le financement le moins cher capable de gérer les déchets. |
| **Contrats régionaux** | Comparer les revenus et les dépenses du contrat au coût réel de la production ou du traitement locaux. |

#### À quelle vitesse les changements de financement sont-ils appliqués ?

**Électricité et eau**

Le Ministère applique directement le pourcentage de financement choisi, sans limite de pas artificielle.

Les contrôles de sécurité applicables et la coordination avec les contrats régionaux restent en vigueur.

**Traitement des déchets**

Les réductions de financement sont volontairement progressives :

- Au plus **25 points de pourcentage** par réduction.
- Au moins **7 jours simulés** entre deux réductions du financement du traitement des déchets.
- Au moins **3 jours simulés** depuis la dernière écriture du financement du traitement des déchets avant une augmentation.

Ces délais laissent à l’efficacité du traitement des déchets et à la capacité de traitement du jeu le temps de rattraper les changements de financement.

**Contrats régionaux**

Le Ministère crée, modifie ou annule des contrats uniquement lorsque le plan obtenu est économiquement justifié et satisfait les exigences de service pertinentes.

Voir [Accords avec les voisins](deals.md#neighbor-deals-objective).

<a id="safety-physical"></a>
#### Respecter les limites physiques du jeu

Le Ministère ne contourne pas les restrictions de fonctionnement normales de SimCity 4.

Il utilise :

- Les pourcentages de financement acceptés par le jeu.
- Des choix de financement en nombres entiers lorsque c’est exigé.
- La plage configurée pour l’éducation et la santé, normalement **1–120 %**.
- Le plancher de protection des canalisations d’eau lorsque cette option est activée.
- Les limites réelles de stockage et d’admission des décharges.
- Les quantités de contrat que le jeu autorise.
- Les prix renvoyés par les propres fonctions de contrat et de démolition du jeu.

**Le Ministère ne construit jamais automatiquement de bâtiments ni d’infrastructures.**

Il ne fait pas non plus :

- Créer de zonage ordinaire.
- Modifier les taux d’imposition.
- Modifier les ordonnances de la ville.
- Ajuster des budgets sans rapport.
- Modifier les villes voisines.

La seule exception liée au zonage est la [réparation des cases fantômes](garbage.md#ghost-tiles), strictement encadrée, qui utilise le propre système de zonage du jeu pour corriger l’appartenance erronée à une décharge sans modifier volontairement le zonage légitime des décharges.

<a id="safety-reevaluation"></a>
#### Quand le Ministère recalcule-t-il ses décisions ?

Le Ministère combine des revues régulières et des contrôles plus légers pour les changements significatifs.

**Il n’attend pas un délai fixe arbitraire après l’ouverture d’une ville.**

Chaque service commence à évaluer dès que les informations dont il a besoin sont disponibles, à jour et cohérentes.

La démolition fait exception : elle démarre au premier changement de mois simulé après le chargement.

#### Vérifier que les statistiques du jeu sont fiables

Avant de prendre des décisions sur l’électricité ou l’eau, le Ministère compare la demande déclarée à la consommation réelle.

Une petite différence peut venir des arrondis du jeu ou du moment de ses mises à jour.

En revanche, des lectures nettement contradictoires empêchent les décisions de financement automatiques.

**Lectures de service incohérentes**

```math
R>D\times1.01+1
```

Où :

- **D** est la demande déclarée par le jeu.
- **R** est la quantité du service déclarée comme livrée ou consommée.

Si la consommation dépasse la demande de plus de **1 % plus une unité**, les lectures sont considérées comme incohérentes.

Le Ministère attend des données fraîches plutôt que d’agir sur un état peu fiable.

Parmi les autres signes que la simulation n’est pas prête :

- Une demande encore à zéro juste après le chargement.
- Des nombres de bâtiments alimentés et non alimentés en eau pas encore renseignés.
- Des informations de financement ou de production des centrales électriques illisibles ou absentes lorsque la gestion du financement des centrales les exige.

#### Contrôles quotidiens des services

Chaque jour simulé, le Ministère lit un petit nombre de compteurs d’électricité et d’eau.

Ces contrôles sont volontairement légers.

Ils n’exigent ni d’analyser chaque bâtiment ni de reconstruire tout le marché des contrats régionaux.

**Une lecture identique n’est pas traitée comme une nouvelle observation.**

Le Ministère ne déclenche une évaluation anticipée que si quelque chose de pertinent a réellement changé.

#### Détecter une véritable pénurie d’approvisionnement régional

Une ville peut déclarer une consommation insuffisante pour deux raisons très différentes :

1. Elle ne dispose pas d’assez d’électricité ou d’eau.
2. Elle dispose d’assez d’approvisionnement, mais le service n’atteint pas certains bâtiments.

Le Ministère utilise le test suivant pour les distinguer.

**Déficit d’approvisionnement régional**

```math
S<D
\qquad\text{et}\qquad
C+I-E<D
```

Où :

- **D** est la demande de la ville.
- **S** est le service réellement livré.
- **C** est la capacité de production locale.
- **I** est l’approvisionnement importé utilisable.
- **E** est l’approvisionnement exporté.

Toutes les quantités utilisent les unités mensuelles de la ressource concernée.

**Une véritable pénurie existe lorsque la consommation est inférieure à la demande et que l’approvisionnement utilisable total est lui aussi insuffisant.**

Par exemple, si une ville a besoin de 20 000 unités mais n’en a que 16 000 de disponibles après importations et exportations, le Ministère peut immédiatement reconsidérer le financement et les contrats.

En revanche, supposons que la ville ait besoin de 20 000 unités et dispose de 25 000 unités de capacité, mais que certains bâtiments restent non desservis.

Le problème se situe probablement dans la ville elle-même, par exemple des lignes électriques ou des canalisations d’eau déconnectées.

**Le Ministère consigne le problème mais ne replanifie pas inutilement la production simplement parce que des bâtiments n’ont pas de connexion.**

#### Détecter les changements significatifs avant la prochaine revue régulière

Le Ministère peut aussi réagir quand la demande, la capacité locale, les importations ou les exportations changent d’au moins **1 %**.

**Seuil de changement significatif**

```math
|x-x_0|\ge0.01\times\max(1,|x_0|)
```

Où :

- **x** est la dernière lecture.
- **x₀** est la valeur enregistrée lors de la dernière évaluation.

**Exemple**

Supposons que la demande d’électricité était de 50 000 unités lors de la revue précédente.

| Nouvelle demande | Variation | Résultat |
|---|---:|---|
| 50 400 | 0,8 % | Attendre la revue normale |
| **50 600** | **1,2 %** | **Déclencher une évaluation anticipée** |

Une nouvelle lecture significative peut avancer la prochaine évaluation sans attendre l’intervalle régulier.

**Il n’y a pas de délai d’attente supplémentaire pour une décision prête et sûre à appliquer.**

#### Intervalles d’évaluation réguliers

| Service | Quand il est évalué |
|---|---|
| **Électricité** | Tous les 10 jours simulés, plus des revues anticipées en cas de pénurie confirmée ou de changement significatif. |
| **Eau** | Tous les 10 jours simulés depuis sa dernière revue, à chaque changement de mois, et plus tôt si nécessaire. |
| **Traitement des déchets** | À chaque changement de mois et quand les chiffres de déchets surveillés changent de façon significative. |
| **Éducation et santé** | Hausses initiales après l’ouverture, contrôles de risque tous les deux jours (chaque jour pendant les grèves), nouveaux contrôles tous les sept jours tant que les risques persistent, et revues mensuelles pour les hausses et les réductions. |
| **Démolition automatique** | À chaque changement de mois simulé. |
| **Conseil sur la taille de la décharge** | Au plus une fois par mois simulé, lors d’une évaluation ordinaire du traitement des déchets. |

**Que se passe-t-il pendant qu’un changement précédent est encore en cours de vérification ?**

Le Ministère ne fait pas de changements qui se chevauchent sur une même ressource.

Par exemple :

- Un changement de contrat régional doit être confirmé avant l’opération de contrat suivante.
- Un ajustement du financement du traitement des déchets doit atteindre l’efficacité et la capacité de traitement attendues avant qu’un autre ajustement soit fait.

Pendant l’attente, le Ministère continue de contrôler les valeurs du jeu concernées.

**Il n’effectue pas une nouvelle optimisation complète simplement parce qu’un contrôle de vérification est dû.**

Cela empêche des opérations partiellement achevées de se gêner mutuellement, tout en gardant le système réactif aux véritables changements.
<a id="safety-verification"></a>
#### Comment le Ministère vérifie-t-il ses changements ?

Le Ministère ne présume pas qu’une opération a réussi simplement parce qu’il a envoyé une commande à SimCity 4.

**Chaque changement automatique est contrôlé avant et après son application.**

La vérification dépend du type d’opération.

| Opération | Ce que le Ministère vérifie |
|---|---|
| **Financement des écoles et des hôpitaux** | Le nouveau financement correspond à la valeur demandée, et le budget des bus scolaires ou des ambulances reste inchangé. |
| **Financement des centrales électriques et de l’eau** | Le jeu accepte la valeur de financement demandée. |
| **Financement du traitement des déchets** | Le nouveau financement est accepté, puis l’efficacité et les capacités de traitement du jeu se mettent à jour comme prévu. |
| **Contrats régionaux** | L’état du contrat, le montant, le prix et la comptabilité régionale concordent avec le changement demandé. |
| **Démolition automatique** | Le bâtiment a disparu et son zonage sous-jacent est intact. |

#### Avant d’appliquer un changement

Le Ministère relit le bâtiment, le service ou le contrat.

La valeur doit encore correspondre à celle utilisée lors du calcul du plan.

Par exemple, supposons que le Ministère prévoie de réduire le financement de l’eau de 100 % à 78 %.

Avant d’appliquer le changement, il vérifie que le financement est toujours de 100 %.

Si vous avez vous-même déplacé le curseur sur 90 %, **l’ancien plan est annulé au lieu d’être appliqué à votre nouveau réglage**.

Le Ministère peut alors calculer un nouveau plan à partir des informations actuelles.

#### Après avoir appliqué un changement

Le Ministère lit la valeur obtenue.

Pour les changements de financement, le pourcentage demandé doit être confirmé.

Pour les écoles et les hôpitaux, il vérifie aussi que le budget distinct des bus ou des ambulances n’a pas changé.

Les contrats régionaux exigent une confirmation supplémentaire, car les enregistrements de contrats et les totaux financiers du jeu peuvent se mettre à jour après l’opération.

Le traitement des déchets exige aussi un contrôle ultérieur, car son efficacité et ses capacités de traitement ne réagissent pas forcément tout de suite à un changement de financement.

Les démolitions sont vérifiées lors d’une analyse ultérieure, quand le jeu a eu le temps d’exécuter l’ordre.

**Un changement n’est pas considéré comme réussi simplement parce que la commande a été envoyée.**

Le Ministère distingue :

- **Planifié :** un changement a été calculé mais pas appliqué.
- **En attente :** un changement a été soumis et attend confirmation.
- **Vérifié :** le jeu signale le résultat attendu.

Les économies ne sont comptées comme confirmées que lorsque le résultat pertinent a été vérifié.

<a id="safety-missing-data"></a>
#### Que se passe-t-il quand le jeu ne fournit pas assez d’informations ?

Le Ministère suit une règle simple :

**Une information inconnue n’est pas la même chose que zéro.**

Par exemple :

- Une liste valide ne contenant aucune centrale électrique signifie qu’il n’y a aucune centrale électrique dans cette liste.
- Une tentative échouée de lecture de la liste des centrales électriques ne prouve pas que la ville n’a pas de centrales électriques.
- Une décharge dont la capacité d’admission confirmée est de zéro ne peut pas accepter davantage de déchets.
- Une décharge dont la capacité d’admission est illisible a une capacité d’admission inconnue, pas un zéro confirmé.

Le Ministère traite ces situations différemment.

#### Lectures incomplètes ou invalides

Si une partie des informations nécessaires n’est pas disponible, le Ministère n’utilise que des valeurs réellement lues et vérifiées.

Quand une valeur manquante ou invalide rend le calcul peu fiable, **le service concerné n’effectue aucun changement de financement ni de contrat pendant cette évaluation**.

Il ne remplace pas l’information manquante par une ancienne valeur en cache ni n’invente une estimation plausible.

Les autres services peuvent continuer à fonctionner indépendamment.

#### Villes voisines ou offres inconnues

Si les offres régionales ou les enregistrements des villes voisines sont illisibles, le Ministère ne suppose pas qu’aucun approvisionnement alternatif n’existe.

Par exemple, il ne peut pas conclure qu’une pénurie d’électricité est impossible à résoudre s’il n’a pas pu inspecter les achats régionaux disponibles.

**Un marché de contrats incomplet ne peut pas justifier un avertissement de pénurie définitif.**

#### Géométrie de la décharge inconnue

Le Ministère peut calculer le financement du traitement des déchets sans connaître chaque détail de la structure interne des cases de la décharge.

Tant que cette structure n’a pas été calibrée, il utilise les calculs prudents décrits dans [Déchets et décharges](garbage.md#garbage-landfills-algorithm).

Il n’invente pas de capacité de stockage et ne fait pas de prévisions pour le mois suivant qui exigeraient des informations indisponibles.

#### Prix de démolition inconnus

Si SimCity 4 ne peut pas fournir le prix de démolition d’une parcelle, **le Ministère ne la démolit pas**.

L’opération peut être reconsidérée lors d’une revue mensuelle ultérieure.

<a id="safety-player-control"></a>
#### Vous gardez la main

Le Ministère automatise des décisions de gestion répétitives, mais vous pouvez choisir les fonctions qu’il contrôle.

| Ce que vous voulez faire | Comment faire |
|---|---|
| **Désactiver tout le Ministère** | Désactivez **Ministère actif**, ou abrogez **Instituer un Ministère de l’Optimisation** dans la fenêtre des ordonnances du jeu. |
| **Gérer un service manuellement** | Désactivez l’option de gestion de ce service, par exemple **Gérer l’électricité** ou **Gérer l’éducation**. |
| **Gérer les contrats régionaux manuellement** | Désactivez **Gérer les accords d’électricité**, **Gérer les accords d’eau** ou **Gérer les accords de déchets** pour la ressource concernée. |
| **Modifier vous-même un curseur de financement** | Déplacez-le normalement. Le Ministère utilisera votre nouvelle valeur lors de sa prochaine évaluation si la gestion reste activée. |
| **Créer ou annuler un contrat vous-même** | Utilisez la fenêtre normale des accords avec les voisins. Le Ministère prendra en compte les contrats obtenus lors de sa prochaine revue. |
| **Arrêter la démolition automatique** | Désactivez à la fois **Démolir les bâtiments détruits** et **Démolir les bâtiments abandonnés**, ou désactivez tout le Ministère. |
| **Retirer le plugin** | Fermez SimCity 4 et supprimez les fichiers du plugin. Les sauvegardes de vos villes restent utilisables sans lui. |

#### Que se passe-t-il quand vous désactivez le Ministère ?

**Désactiver le Ministère n’annule pas les changements précédents.**

Par exemple, s’il avait auparavant réduit le financement de l’électricité à 72 %, le désactiver ne rétablit pas le financement précédent.

De même, les contrats régionaux restent dans leur état actuel.

Le Ministère cesse simplement de faire d’autres changements automatiques.

#### Que se passe-t-il quand vous déplacez un curseur manuellement ?

Vous pouvez toujours déplacer un curseur de financement.

Toutefois, **une valeur manuelle n’est pas verrouillée tant que son gestionnaire est activé**.

Le Ministère traite votre nouvelle valeur comme point de départ de son prochain calcul.

Selon le service :

- **Électricité :** le financement peut être recalculé en quelques jours, au plus tard lors de la revue régulière de 10 jours.
- **Eau :** le financement peut être recalculé en 10 jours simulés.
- **Éducation et santé :** une réduction manuelle qui crée un risque de capacité peut être corrigée en quelques jours. Une hausse manuelle est examinée lors du passage mensuel.

Par exemple, si vous augmentez le financement de l’eau à 100 % mais laissez **Gérer l’eau** activé, le Ministère peut le rabaisser quand ses calculs montrent qu’un financement plus faible suffit.

**Pour garder la maîtrise d’une valeur de financement précise, désactivez le gestionnaire de ce service.**

**Si le jeu déplace lui-même un curseur.** SimCity 4 peut déplacer de lui-même les curseurs de bâtiments déjà placés après la modification du coût d’entretien d’un bâtiment (le Phantom Funding Slider Bug). Si le changement apparaît juste après un recalcul mensuel, dans un bâtiment dont le Ministère avait réglé le financement, le Ministère réévalue ce bâtiment à sa prochaine surveillance et remet son financement au minimum qui donne la capacité exigée, au lieu d’attendre la revue mensuelle. Un changement fait à la main en milieu de mois n’est pas traité ainsi. Voir [Phantom Funding Slider Bug](phantom_slider.md).

#### Y a-t-il des effets qui ne peuvent pas être désactivés individuellement ?

Oui.

Sur la version du jeu prise en charge, le plugin masque l’icône d’avertissement d’origine *pas d’eau* au-dessus des parcelles agricoles dès que le plugin est chargé.

Cela ne modifie pas le service de l’eau lui-même.

Voir [Eau](water.md#water-farm-icon).

Le Ministère masque aussi certains messages de conseillers d’origine de SimCity 4, mais uniquement tant qu’il gère réellement le service correspondant.

Quand il ne gère pas (ou ne peut pas gérer) le service, les messages d’origine sont exactement ceux que fournit le jeu. Voir les [Avis des conseillers](#safety-notices-game).

<a id="safety-automation"></a>
#### Quelles fonctions agissent automatiquement, et lesquelles ne font que conseiller ?

Certaines fonctions du Ministère modifient directement la ville.

D’autres se contentent d’afficher des informations pour que vous décidiez quoi faire.

| Fonction | Action automatique | Avis des conseillers |
|---|---|---|
| **Éducation et santé** | Ajuste le financement | Avertit quand un financement supplémentaire ne peut pas résoudre un problème de capacité persistant |
| **Électricité** | Ajuste le financement des centrales électriques | Avertit d’une pénurie persistante qui ne peut pas être résolue automatiquement |
| **Eau** | Ajuste le financement du service | Avertit d’une pénurie persistante qui ne peut pas être résolue automatiquement |
| **Traitement des déchets** | Ajuste le financement du service | Avertit d’une pénurie persistante de traitement des déchets |
| **Contrats avec les voisins** | Crée, modifie et annule des contrats | Aucun avis de contrat de routine distinct |
| **Démolition automatique** | Supprime les ruines et les bâtiments abandonnés éligibles | Aucun avis |
| **Cases fantômes de décharge** | Répare automatiquement les cases fantômes éligibles | Aucun avis |
| **Conseil sur la taille de la décharge** | **Ne modifie jamais la taille de la décharge** | Recommande en option une réduction précise |

**Le conseil sur la taille de la décharge est la principale fonction qui recommande un changement sans l’effectuer.**

Il peut vous indiquer que réduire une décharge pourrait faire économiser de l’argent, mais la décision de zonage reste la vôtre.

<a id="safety-performance"></a>
#### Comment le Ministère évite-t-il de ralentir le jeu ?

Le Ministère est conçu pour n’effectuer que le travail nécessaire à chaque moment de la simulation.

**Tout le travail de simulation s’exécute sur le propre thread du jeu, entre les images, à l’aide de ses événements quotidiens et mensuels normaux.**

Il ne crée pas de simulation distincte en arrière-plan.

La charge de travail est répartie ainsi :

| Opération | Fréquence ou approche |
|---|---|
| **Surveillance quotidienne des services** | Lit un petit nombre de compteurs du jeu. |
| **Optimisation complète des ressources** | S’exécute uniquement quand le service doit être évalué ou qu’un changement notable survient. |
| **Analyse des parcelles pour la démolition** | Lit toute la grille des parcelles une fois par mois simulé. |
| **Vérification de la démolition** | Effectue une analyse légère tous les 10 jours simulés, en lisant une valeur d’état par parcelle et en enregistrant les changements. |
| **Détection du bouton d’options** | Un minuteur de 250 millisecondes vérifie seulement si la fenêtre Options de jeu est disponible. |
| **Contrôles des avis de conseillers** | Une fois par jour simulé. |

**Les contrôles de surveillance quotidiens ne répètent pas inutilement les calculs complets coûteux.**

Par exemple, une lecture inchangée de la demande d’électricité ne déclenche pas une nouvelle optimisation complète des centrales électriques.

De même, le conseil sur la taille de la décharge s’exécute dans une évaluation normale du traitement des déchets, et non par une recherche continue distincte.

<a id="notices"></a>
### Avis des conseillers

Le Ministère évite volontairement d’annoncer son travail de routine.

Il n’affiche pas de messages chaque fois qu’il :

- Ajuste un financement.
- Achète ou vend une ressource.
- Modifie ou annule un contrat.
- Démolit un bâtiment abandonné.
- Répare une case fantôme de décharge.

**Il n’y a pas non plus de message d’accueil ni d’activation.**

Le Ministère n’utilise les avis des conseillers que lorsque le joueur a peut-être besoin d’intervenir, ou lorsqu’une recommandation facultative sur la décharge pourrait offrir des économies importantes.

<a id="safety-notices-utility"></a>
#### Avis de pénurie d’électricité, d’eau et de déchets

Le Ministère peut vous prévenir lorsqu’une pénurie persiste et que **les solutions automatiques disponibles sont épuisées**.

Ces avis viennent du **conseiller des Services publics**. Leur texte s’adresse au maire et à la ville par leur nom.

Un avis de pénurie n’existe **que tant que le Ministère gère ce service localement** (électricité, eau ou déchets, Ministère actif). Si vous désactivez cette gestion, l’avis est retiré. Les contrats régionaux sont une autre affaire : ils ne provoquent jamais d’avis, et un avis n’annule jamais un contrat.

| Ressource | Titre de l’avis |
|---|---|
| **Électricité** | **Panne de courant ! Qui a acheté les bougies ?** |
| **Eau** | **Même les robinets ont soif !** |
| **Déchets : pénurie grave** | **Des déchets jusqu’au cou !** |
| **Déchets : vieux tas qui ne diminue pas** | **Ces déchets paient déjà un loyer !** |

Les deux premiers avis d’un épisode sont des messages urgents qui peuvent s’ouvrir en fenêtre ; les suivants sont des entrées discrètes de la liste des nouvelles. L’avis du vieux tas est informatif : neutre, toujours discret, jamais en fenêtre. Aucun ne comporte de lien vers la caméra.

#### Quand le Ministère considère-t-il une pénurie comme prouvée ?

Un avis n’est pas généré simplement parce que la demande dépasse temporairement l’offre. Le Ministère doit établir, avec des relevés actuels, que **la pénurie ne peut pas être résolue avec ce qu’il a le droit de faire**.

Pour l’électricité et l’eau, il vérifie la capacité utilisable des installations locales, les achats régionaux disponibles, les ventes qui occupent de la capacité, la demande réelle de la ville et le service livré, et qu’aucune modification de sa part n’attend de confirmation. **Un bâtiment sans eau uniquement parce qu’il est déconnecté du réseau ne prouve pas une pénurie régionale.**

Pour les déchets, il compare son modèle de traitement au retard de déchets non collectés du jeu, relevé mois par mois. Un retard allant jusqu’à **1 % de la production mensuelle** est considéré comme résiduel.

| Situation | Ce que voit le Ministère | Avis |
|---|---|---|
| **Grave** | le retard a augmenté de plus que cette marge chacun des deux derniers mois | avis urgent |
| **Vieux tas** | il y a un retard au-dessus de la marge, mais il ne grandit plus : presque tous les nouveaux déchets sont traités, le vieux tas reste | avis neutre et discret |
| **En baisse ou résolu** | le retard diminue, ou a disparu ou est résiduel | aucun nouvel avis ; la résolution retire l’avis |
| **Incertain** | les données ne suffisent pas | rien ne change |

Les deux avis de déchets n’existent jamais en même temps. Quand les données ne montrent plus de pénurie sans confirmer qu’elle est finie, le Ministère ne prévient pas et ne retire pas un avis déjà affiché, et les jours d’attente restent à l’arrêt.

#### Quand les avis et les rappels apparaissent-ils ?

| Ressource | Premier avis (jours de pénurie prouvée) | Rappel tous les |
|---|---|---|
| **Électricité** | 15 jours simulés | 120 jours simulés |
| **Eau** | 30 jours simulés | 150 jours simulés |
| **Déchets** | 60 jours simulés | 240 jours simulés |

Une pénurie résolue avant ces jours ne produit aucun avis. Seuls comptent les jours où la pénurie est prouvée. Les deux premiers avis d’un épisode peuvent s’ouvrir en fenêtre ; à partir du troisième, ils sont discrets. Il n’y a pas de limite de rappels tant que la pénurie dure.

Le Ministère ne peut pas voir si vous avez fermé un avis, donc il ne le suppose jamais : les rappels suivent simplement ces intervalles fixes.

#### Quand un épisode de pénurie se termine-t-il ?

Pour l’électricité et l’eau, quand des relevés récents confirment, deux jours de suite, que le service livré est au moins égal à la demande. Pour les déchets, quand le modèle de traitement redevient viable et que le retard a disparu ou est résiduel.

L’avis est alors retiré du conseiller et les rappels s’arrêtent. **Une pénurie ultérieure est un nouvel épisode**, avec ses propres jours d’attente. Désactiver la gestion du service termine aussi l’épisode et retire l’avis ; en la réactivant, la pénurie doit être prouvée à nouveau.

#### Que se passe-t-il si vous fermez et rouvrez la ville ?

**Rien n’est mémorisé d’une session à l’autre** pour ces avis. À l’ouverture d’une ville, le Ministère retire tout avis de pénurie laissé par une session précédente. Si la pénurie est toujours là, elle doit être prouvée à nouveau et le premier avis arrive après les jours d’attente normaux (15, 30 ou 60). Les avis d’éducation et de santé, plus bas, conservent leur mémoire.

#### Quels réglages sont nécessaires pour les avis de pénurie ?

Seulement le Ministère actif et la **gestion locale de ce service** (électricité, eau ou déchets). Les réglages des contrats régionaux n’interviennent pas.

<a id="safety-notices-civic"></a>
#### Avis de capacité pour l’éducation et la santé

Le Ministère peut aussi vous avertir quand des écoles ou des établissements de santé restent gravement surchargés malgré le financement maximal disponible.

Ces avis sont émis par le **conseiller de la santé et de l’éducation**.

Ils apparaissent comme des **entrées rouges dans la liste des nouvelles**, sans ouvrir de fenêtre contextuelle.

Il y a un avis par type d’établissement :

- Écoles primaires.
- Lycées.
- Universités et établissements d’enseignement supérieur.
- Bibliothèques.
- Musées.
- Cliniques.
- Hôpitaux.
- Centres de recherche sur les maladies.

#### Quand un bâtiment est-il inclus ?

Un bâtiment doit remplir toutes les conditions suivantes :

1. Son financement maximal possible ne peut toujours pas fournir une capacité suffisante.
2. Sa capacité maximale couvre **moins de 85 % de son utilisation**.
3. Il **n’est pas actuellement en grève**.
4. Le problème dure depuis **30 jours simulés sans interruption**.

**Une couverture inférieure à 85 % signifie que, même au financement maximal, l’établissement ne peut pas servir 85 % de son utilisation actuelle.**

Par exemple, si un hôpital a besoin d’une capacité de 1 000 patients mais ne peut en servir que 800 au financement maximal, sa couverture est de 80 %.

Si cette situation persiste pendant 30 jours simulés sans grève, l’hôpital peut figurer dans un avis de capacité.

Voir [Éducation et santé](education_healthcare.md#education-healthcare-notices) pour le calcul complet.

#### Quelles informations apparaissent dans l’avis ?

Un avis liste jusqu’à **10 bâtiments concernés**, les problèmes les plus graves en premier.

Chaque bâtiment listé a un lien qui déplace la caméra vers son emplacement.

**Le Ministère ne crée pas d’avis distinct pour chaque bâtiment surchargé.**

Il regroupe plutôt les bâtiments par type d’établissement pour que la liste des conseillers reste gérable.

#### Que se passe-t-il quand les bâtiments concernés changent ?

Le Ministère ne maintient qu’un seul avis actif par type d’établissement.

Si le nombre de bâtiments concernés reste le même mais que leur identité change, il met à jour les liens vers les bâtiments de l’avis existant.

Si le nombre change, l’ancien avis est retiré avant qu’un remplaçant soit affiché.

**Il n’y a jamais deux avis actifs pour le même type d’établissement.**

Si deux remplacements surviennent en moins de 30 jours simulés, le Ministère impose une **pause de 30 jours** avant d’autres remplacements pour ce type.

Les mises à jour des liens d’un avis existant peuvent se poursuivre pendant la pause.

#### Quand l’avis est-il supprimé ?

Quand tous les bâtiments concernés sont rétablis, l’avis est retiré.

Un avis que vous fermez manuellement n’est plus affiché tant que les mêmes bâtiments restent concernés.

Cet état de fermeture est conservé quand on rouvre la ville.

À la première évaluation après l’ouverture, le Ministère retire aussi tout avis résiduel d’une session précédente.

Un avis de capacité peut rester dans la liste des nouvelles jusqu’à **720 jours simulés** tant que le problème persiste.

<a id="safety-notices-landfill"></a>
#### Recommandations sur la taille de la décharge

Le Ministère peut émettre un avis facultatif quand modifier la taille de la décharge réduirait nettement les dépenses de traitement des déchets.

Il provient du **conseiller des services publics**, avec une humeur de bonne nouvelle et une priorité urgente.

| Recommandation | Titre de l’avis |
|---|---|
| **RÉDUIRE** | **Notre décharge prend trop de place !** |

Chaque avis décrit :

- Les dimensions de la zone proposée, en cases.
- L’économie mensuelle estimée.
- La réduction en pourcentage des dépenses de traitement des déchets.
- Un lien **voir où** qui déplace la caméra vers la zone recommandée.

Ces avis ne sont disponibles que lorsque :

- **Gérer les déchets** est activé.
- **M’avertir si revoir la décharge permet d’économiser** est activé.
- L’économie estimée est strictement supérieure à votre seuil configuré.
- La ville contient exactement une zone de décharge.
- Les conditions de calendrier et de sécurité décrites dans le chapitre sur les décharges sont remplies.

Le seuil par défaut est de **25 %**, et le conseil sur la taille de la décharge est désactivé par défaut.

Le Ministère analyse la taille de la décharge au plus une fois par mois simulé.

Il respecte aussi :

- La **période de protection de 24 mois** après un changement de taille de la décharge ou lorsqu’une décharge est rencontrée pour la première fois.
- L’**intervalle de 12 mois** après l’émission d’un conseil.

Voir [Déchets et décharges](garbage.md#garbage-landfills-advice).

**Les réparations de cases fantômes ne génèrent jamais d’avis de conseillers.**

<a id="safety-notices-game"></a>
#### Avis d’origine de SimCity 4 modifiés par le Ministère

Le Ministère masque certains messages de conseillers d’origine du jeu qui seraient trompeurs ou redondants pendant que la gestion automatique est active, **mais uniquement tant qu’il peut réellement gérer le service concerné**. Dans tous les autres cas, les messages d’origine sont exactement ceux que fournit le jeu.

Ce sont deux petits scripts propres au Ministère, dans `DepartmentEfficiency.dat`, qui s’en chargent. Ils suivent six interrupteurs que le Ministère publie dans chaque ville et met à jour sans redémarrer le jeu : quand vous désactivez un service, quand vous désactivez tout le Ministère, ou quand le Ministère ne peut pas agir sur un service (par exemple parce que des relevés manquent ou qu’un de ses contrôleurs s’est arrêté par sécurité), les messages d’origine de ce service reviennent aussitôt. Les services sont indépendants les uns des autres.

| Messages d’origine du jeu | Masqués tant que le Ministère gère |
|---|---|
| Offres de contrats, suggestions d’annulation, renouvellements et avertissements de défaut avec les voisins | les contrats de cette ressource (électricité, eau ou déchets, séparément) |
| Conseils habituels de financement de l’éducation et de la santé ; les rumeurs de pré-grève sont limitées aux périodes où une grève est réellement en cours | l’éducation, ou la santé, respectivement |
| Les avertissements de problèmes de déchets sont limités aux situations avec une quantité importante de déchets non collectés | la gestion locale des déchets |

Les vraies alertes de grève et les messages positifs des conseillers ne sont jamais modifiés. Un message dont l’événement survient pendant qu’il est masqué n’est pas affiché ensuite.

#### Pourquoi certains avertissements d’origine sur les déchets sont-ils supprimés ?

SimCity 4 avertit parfois d’un problème de déchets simplement parce que le stockage de la décharge augmente.

Or une décharge qui se remplit progressivement n’est pas forcément la preuve que le système de traitement des déchets de la ville défaille.

Le Ministère ajoute donc une exigence fondée sur la quantité de déchets réellement restée non collectée.

**Déchets non collectés importants**

```math
U>100+0.05G
```

Où :

- **U** est la quantité actuelle de déchets non collectés du jeu.
- **G** est le volume de déchets produit par la ville, selon les valeurs des conseillers du jeu.

L’avertissement d’origine sur les déchets ne peut apparaître que lorsque cette condition supplémentaire est remplie, en plus de la condition d’avertissement normale du jeu.

**Exemple**

Si la ville produit 20 000 unités de déchets, le seuil est :

```math
100+0.05\times20000=1100
```

Le jeu doit signaler **plus de 1 100 unités de déchets non collectés** avant que l’avertissement modifié sur les problèmes de déchets soit autorisé.

Cela évite des alertes inutiles quand le traitement des déchets fonctionne normalement mais que le stockage de la décharge augmente.

#### Comment restaurer les messages d’origine des conseillers du jeu ?

Il n’y a rien à supprimer. Le Ministère publie son état dans chaque ville : désactivez le service correspondant dans la fenêtre d’options (ou abrogez l’ordonnance, ou mettez `Mode=Off`) et les messages d’origine reviennent aussitôt, sans redémarrer le jeu. Ils reviennent aussi quand le Ministère ne peut pas agir sur un service. Un message déjà masqué revient selon la fréquence d’affichage propre au jeu, ce qui peut prendre un certain temps.

**Un événement qui survient pendant que son message est masqué n’est pas affiché ensuite.**

<a id="safety-isolation"></a>
### 4. Conditions de sécurité

#### Comment le Ministère gère-t-il les défaillances techniques ?

Le Ministère est divisé en composants de gestion indépendants.

**La défaillance d’un composant ne devrait pas désactiver des services sans rapport ni arrêter SimCity 4 lui-même.**

Quand un contrôle de sécurité échoue, le composant concerné cesse de faire des changements au lieu de retenter sans cesse une opération peu fiable.

| Défaillance | Ce que fait le Ministère |
|---|---|
| **La vérification du financement d’une centrale électrique ou de l’eau échoue** | Tente d’annuler les changements faits pendant ce cycle et gèle la gestion du financement de cette ressource pour la session de ville. Les autres ressources et les contrats régionaux peuvent continuer. |
| **La vérification du financement du traitement des déchets échoue** | Arrête les changements de financement du traitement des déchets pour la session. Les contrats de déchets et les autres gestionnaires de ressources peuvent continuer. |
| **Une opération de contrat échoue techniquement** | Arrête la gestion automatique des contrats pour la session. Les gestionnaires de financement locaux continuent d’utiliser les contrats existants. |
| **Un changement de financement d’école ou d’hôpital ne peut pas être vérifié** | Exclut ce bâtiment pour la session. Après des échecs concernant trois bâtiments différents, les écritures de financement de l’éducation et de la santé s’arrêtent pour la session. |
| **La commande de financement d’un bâtiment n’a aucun effet** | Exclut ce bâtiment tout en laissant les autres bâtiments continuer. |
| **La démolition automatique échoue à un contrôle de sécurité** | Arrête la démolition pour la session si l’aperçu d’un prix modifie une parcelle, si le zonage disparaît, ou si trois ordres consécutifs ne sont pas exécutés. |
| **Une opération de conseiller produit une erreur** | Cesse de contrôler et de retirer les avis du Ministère pour le reste de la session. |
| **La fenêtre Options de jeu a une disposition inattendue** | Laisse la fenêtre d’origine inchangée et n’ajoute pas le bouton du Ministère pendant cette session. |
| **Un fichier de configuration est invalide** | Désactive le composant de gestion concerné au lieu d’utiliser des réglages peu fiables. |

#### Comment les défaillances techniques sont-elles annoncées ?

Si un service que vous avez activé doit s’arrêter jusqu’à la fin de la session de la ville à cause d’une défaillance technique, le Ministère ajoute **une actualité native** à la liste des actualités en bas du jeu. Le titre apparaît dans la liste et le détail au clic. Il n’y a ni fenêtre contextuelle ni fenêtre de conseiller.

- Il y a sept types : électricité, eau, assainissement, santé, éducation, contrats régionaux avec les villes voisines et démolition automatique.
- Il y a **une actualité par service et par session de ville**. Santé et éducation sont deux actualités distinctes et n’apparaissent que si vous les avez activées.
- Elle n’est jamais affichée pour ce qui se rétablit tout seul, pour un service en **STANDBY**, pour une lecture refusée, pour un service que vous avez désactivé, ni dans les modes **Off** et **Shadow**. Il n’y a pas de message de rétablissement.
- L’actualité est retirée quand le service est désactivé ou que sa gestion est relancée. La cause technique n’est écrite que dans le journal, dans des lignes `[FailureNotice]`.
- Les textes existent dans les dix langues du plugin.

**STANDBY** signifie que le service n’a encore rien à gérer dans la ville (par exemple l’assainissement local dans une ville sans décharge, incinérateur, centre de recyclage ni centrale de valorisation énergétique des déchets). Ce n’est pas une erreur, cela ne produit aucun avis et démarre tout seul dès qu’il y a quelque chose à gérer.

**Où est le journal ?** `DepartmentEfficiency.log` est écrit dans le dossier `Logs` du dossier utilisateur de SimCity 4, normalement `Documents\SimCity 4\Logs\DepartmentEfficiency.log` ; si vous lancez le jeu avec `-UserDir`, c’est le dossier `Logs` de ce dossier. Si ce dossier est inutilisable, le journal est écrit à côté de la DLL, dans `Plugins`, et sa première ligne en donne la raison. Un journal existant est complété, jamais déplacé.

#### Que signifie un arrêt de sécurité de session ?

Un arrêt de sécurité dure pour la session de ville en cours.

Par exemple, si la vérification du financement de l’eau échoue, le Ministère cesse de faire d’autres changements de financement de l’eau tant que cette ville reste ouverte.

Il n’envoie pas la même commande à répétition.

**Recharger la ville démarre une nouvelle session et efface les arrêts de sécurité propres à la session.**

Le problème sous-jacent peut toujours exister, de sorte que le même contrôle peut échouer de nouveau s’il n’a pas été résolu.

#### Une erreur du Ministère peut-elle faire planter SimCity 4 ?

Le Ministère encadre les événements du jeu qu’il traite afin que les erreurs internes soient interceptées et consignées dans son journal.

**Une erreur dans la gestion du Ministère est censée arrêter l’opération concernée, pas le jeu lui-même.**

Cette isolation est un élément central de sa conception.

<a id="safety-version"></a>
#### Version du jeu prise en charge

Le Ministère vérifie la version de l’exécutable du jeu au démarrage.

**La version prise en charge est SimCity 4 Deluxe 1.1.641.0 (Steam, Windows).**

Sur toute autre version, ou si la version ne peut pas être lue, le Ministère reste totalement inactif.

Cela signifie :

- Aucun abonnement aux événements du jeu.
- Aucun bouton d’options du Ministère.
- Aucune ordonnance du Ministère.
- Aucune opération de financement, de contrat ou de démolition.
- Aucune modification de l’icône d’eau agricole.

La même inactivité complète s’applique lorsque le fichier de ressources principal, `DepartmentEfficiency.dat`, est absent à côté de la DLL. La DLL, le DAT et l’INI s’installent ensemble, directement dans le dossier Plugins ; le plugin ne les cherche jamais ailleurs.

**Le Ministère ne tente pas de fonctionner sur une installation non prise en charge ou incomplète.**

#### Les sauvegardes de vos villes ne dépendent pas du plugin

Le Ministère est conçu pour que les villes sauvegardées pendant qu’il est installé puissent toujours être ouvertes sans lui.

Son ordonnance **Instituer un Ministère de l’Optimisation** est temporairement retirée de la ville avant chaque sauvegarde puis restaurée ensuite.

L’ordonnance n’est donc pas stockée dans la ville sauvegardée.

**Le Ministère ne modifie jamais directement les fichiers de sauvegarde des villes ou des régions.**

Si vous retirez le plugin, les niveaux de financement ordinaires de la ville et ses contrats régionaux restent à leurs dernières valeurs établies.

Vous pouvez continuer à jouer sans le Ministère.

<a id="safety-settings"></a>
### 5. Réglages

Le système de sécurité général n’exige pas un ensemble d’options distinct.

Il suit les réglages de gestion décrits tout au long de ce guide.

**Ministère actif**

[Ministère actif](options.md#opt-master) est l’interrupteur général.

Vous pouvez aussi le contrôler via l’ordonnance **Instituer un Ministère de l’Optimisation** dans la fenêtre des ordonnances de SimCity 4.

Les deux commandes représentent le même réglage.

**Options de gestion individuelles**

Chaque service a son propre gestionnaire et, le cas échéant, une réserve ou une marge de sécurité.

Voir la [Référence des options](options.md#options) pour tous les réglages.

**Avis des conseillers**

Le Ministère n’a que deux options propres aux avis, toutes deux liées aux recommandations sur la taille de la décharge :

- [M’avertir si revoir la décharge permet d’économiser](options.md#opt-landfilladvice).
- [Seuil d’économie des alertes de décharge](options.md#opt-landfillthreshold).

**Les avis de pénurie de services ne peuvent pas être désactivés séparément.**

Ils suivent les réglages de gestion des services concernés et n’apparaissent que lorsque leurs conditions sont remplies.

<a id="safety-examples"></a>
### 6. Exemples détaillés

Les exemples suivants montrent comment le Ministère réagit face à des pénuries persistantes, à des informations incomplètes et à des modifications manuelles.

#### Exemple 1 : une pénurie d’électricité impossible à résoudre

Supposons que votre ville se trouve dans la situation suivante au **jour simulé 40** après l’ouverture :

- **Demande d’électricité :** 10 000 unités.
- **Électricité livrée :** 9 200 unités.
- **Capacité locale maximale de production :** 8 900 unités.
- **Importations régionales utilisables :** 200 unités.
- **Ventes :** aucune.

Le Ministère a déjà envisagé le financement légal maximal des centrales et tous les achats disponibles.

L’offre totale disponible est :

```math
8900+200=9100
```

C’est moins que la demande de la ville :

```math
9100<10000
```

Les compteurs du jeu confirment aussi que l’électricité livrée est inférieure à la demande.

**La pénurie est prouvée au jour 40.** Le Ministère attend 15 jours simulés.

**Jour 55**

La pénurie n’est toujours pas résolue. Le Ministère émet le premier avis, qui peut s’ouvrir en fenêtre :

**Panne de courant ! Qui a acheté les bougies ?**

**Jour 175**

La pénurie continue. Le rappel apparaît 120 jours simulés après l’avis.

**Jour 180**

Vous construisez une autre centrale. Des relevés récents confirment que l’électricité livrée couvre maintenant la demande. Le Ministère retire l’avis et arrête les rappels.

**Jour 250**

Une nouvelle pénurie d’électricité est prouvée. Comme l’épisode précédent est terminé, elle commence sa propre période d’attente ; si elle n’est pas résolue, un nouveau premier avis apparaît au **jour 265**.

#### Exemple 2 : des bâtiments sans électricité, mais une capacité de production suffisante

Prenons une ville avec :

- **Demande d’électricité :** 10 000 unités.
- **Électricité livrée :** 9 600 unités.
- **Capacité de production locale :** 12 000 unités.
- **Ni importations ni exportations.**

Bien qu’une partie de la demande ne soit pas satisfaite, la ville dispose d’une capacité de production suffisante.

```math
12000\ge10000
```

Cela signifie que le test de pénurie d’approvisionnement n’est pas satisfait.

Le problème se situe probablement dans le réseau de distribution d’électricité.

**Le Ministère n’augmente pas le financement et n’émet pas d’avis de pénurie régionale.**

Produire davantage n’atteindrait pas forcément les bâtiments déconnectés.

Le joueur doit inspecter le réseau électrique.

#### Exemple 3 : rouvrir une ville pendant une pénurie en cours

Supposons qu’une pénurie d’eau ait été signalée au **jour 70** d’une session et que vous sauvegardiez la ville au jour 100 alors qu’elle n’est pas résolue.

Plus tard, vous rouvrez la ville. Le Ministère commence une nouvelle session : il **ne se souvient de rien** de la précédente et retire l’ancien avis si le jeu l’a gardé dans la liste.

Si la pénurie d’eau est toujours là, elle est prouvée à nouveau et le premier avis arrive après les 30 jours simulés normaux. Rouvrir la ville ne produit donc jamais d’avertissement immédiat, et l’ancien calendrier de rappels n’est pas repris.

#### Exemple 4 : modifier le financement de l’eau à la main

Supposons que le Ministère ait optimisé le financement de l’eau à **62 %**.

Vous le portez manuellement à **100 %**, tandis que **Gérer l’eau** reste activé.

Le Ministère accepte votre modification comme nouvelle valeur de départ.

À sa prochaine évaluation de l’eau, au plus tard 10 jours simulés plus tard, il recalcule le financement nécessaire.

S’il constate que **64 %** suffisent, il peut ramener le financement de 100 % à 64 %.

**Le Ministère ne conserve pas les modifications manuelles des curseurs tant que la gestion automatique reste activée.**

Pour maintenir le financement de l’eau à 100 %, désactivez **Gérer l’eau**.

#### Exemple 5 : les relevés d’eau ne sont pas prêts après le chargement

Juste après l’ouverture d’une ville, le jeu indique :

- **Demande d’eau :** 20 000 unités.
- **Consommation d’eau :** 20 500 unités.

Le Ministère vérifie si la consommation est cohérente avec la demande.

Le relevé maximal accepté est :

```math
20000\times1.01+1=20201
```

Or :

```math
20500>20201
```

La consommation dépasse la tolérance admise.

**Les relevés sont incohérents ; le Ministère ne prend donc pas encore de décision de financement de l’eau.**

Il attend un nouveau relevé.

Les autres services sont évalués à partir de leurs propres informations.

**Un problème temporaire de données sur l’eau n’empêche pas l’électricité, la gestion des déchets ni les autres gestionnaires indépendants de fonctionner.**

<a id="safety-limitations"></a>
### 7. Limites connues

Le Ministère est conçu pour fonctionner avec prudence, mais il ne peut pas surmonter toutes les limites de SimCity 4.

#### Contrôle manuel

- **Les modifications manuelles du financement ne sont pas verrouillées.** Lorsqu’un gestionnaire de service est activé, il peut réviser, lors de sa prochaine évaluation, un pourcentage de financement que vous avez modifié vous-même.
- **Il n’existe pas de verrou par bâtiment.** Pour garder un service sous contrôle manuel du financement, désactivez le gestionnaire correspondant.

#### Avis de pénurie

- **Les avis de pénurie suivent la gestion locale.** Si vous gérez un service à la main, le Ministère ne vous prévient pas à son sujet.
- **Le Ministère ne peut pas savoir si un avis a réellement été affiché ou fermé.** SimCity 4 décide d’ouvrir ou non une fenêtre selon ses réglages de conseillers. Les rappels suivent des intervalles fixes.
- **Les avis de déchets reposent sur un modèle et sur des relevés mensuels.** Contrairement à l’électricité et à l’eau, les déchets sont jugés d’après la projection de traitement du Ministère et le retard du jeu, mois par mois.

#### Messages d’origine des conseillers du jeu

- **Le Ministère ne masque les messages du jeu que tant qu’il gère réellement le service.** Désactiver un service, ou un service sur lequel le Ministère ne peut pas agir, rétablit aussitôt les messages d’origine.
- **Un événement qui survient pendant que son message est masqué n’est pas affiché ensuite.** Un message qui revient reste soumis à la fréquence d’affichage propre au jeu.

#### Sécurité technique

- **Un arrêt de sécurité technique dure pour la session de ville en cours.** Le gestionnaire concerné reste désactivé jusqu’au rechargement de la ville.
- **Seule la version prise en charge du jeu est gérée.** Le Ministère prend en charge **SimCity 4 Deluxe 1.1.641.0 sur Steam, sous Windows**. Il n’effectue aucune opération sur les versions non prises en charge.

**Le Ministère est conçu pour optimiser la ville uniquement lorsqu’il dispose d’assez d’informations fiables pour agir en sécurité. Quand il ne peut pas vérifier une opération ni prouver qu’un changement est bénéfique, il préfère laisser la situation existante inchangée.**

---

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
