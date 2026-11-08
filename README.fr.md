[English](README.md) | [Português](README.pt.md) | **Français** | [Deutsch](README.de.md) | [Español](README.es.md) | [Italiano](README.it.md) | [Nederlands](README.nl.md) | [Dansk](README.da.md) | [Svenska](README.sv.md) | [Polski](README.pl.md)

# Ministère de l’Optimisation

**Le Ministère de l’Optimisation s’occupe des petits détails de la gestion de la ville qui font une grande différence.**

Le Ministère de l’Optimisation (*Department of Efficiency* en anglais) est un plugin pour **SimCity 4 Deluxe** qui automatise les tâches courantes de gestion de la ville, pour que vous puissiez vous concentrer sur la construction, la planification et la croissance de votre ville.

**Vous bâtissez la ville. Le Ministère optimise le budget !**

Il gère le budget de chaque établissement d’éducation et de santé, les centrales électriques, le financement de l’eau et des déchets, les accords avec les villes voisines et, en option, la démolition des ruines et des bâtiments abandonnés. Il vise à couvrir les besoins de la ville sans dépenser plus que nécessaire, en vous laissant l’emplacement des bâtiments, la couverture et les grandes décisions d’aménagement.

Tout se règle dans le jeu, dans **Options de jeu → Ministère de l’Optimisation**. Choisissez les services à automatiser, ajustez leurs marges de sécurité, configurez la démolition ou désactivez tout le Ministère. Vous pouvez aussi utiliser l’ordonnance **Instituer un Ministère de l’Optimisation** comme interrupteur général. Aucune application séparée n’est nécessaire.

> [!IMPORTANT]
> **Le Ministère démarre activé, y compris la démolition automatique des bâtiments détruits et abandonnés.** Vérifiez ses réglages avant d’ouvrir une ville à laquelle vous tenez. Il fait de vrais changements dans le jeu : financements, accords et démolitions seront enregistrés si vous sauvegardez la ville. Il ne modifie pas directement les fichiers de sauvegarde.

## Sommaire

- [Téléchargement et installation](#download-and-installation)

- [Ce que fait le Ministère](#features)

- [Premiers réglages](#first-time-setup)

- [Réglages](#settings-explained)

- [Captures d’écran](#screenshots)

- [Prérequis et compatibilité](#requirements-and-compatibility)

- [Mise à jour et désinstallation](#updating-and-uninstalling)

- [Limites connues](#known-limitations)

- [Code source](#source-code)

- [Licence](#license)

- [Crédits](#credits)

<a id="download-and-installation"></a>

## Téléchargement et installation

### [⬇ Télécharger le Département de l’efficacité 1.0.0](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases/download/v1.0.0/Departement_of_Efficiency_1.00.zip)

1\. Ouvrez la [page des releases](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases) du projet et choisissez la dernière version.

2\. Sous **Assets**, téléchargez le **ZIP de distribution de Department of Efficiency**.

3\. **Fermez SimCity 4.** Extrayez le ZIP dans un dossier temporaire.

4\. Copiez **`DepartmentEfficiency.dll`** et tout le dossier **`Department of Efficiency`** à la racine de `Documents\SimCity 4\Plugins`.

5\. Lancez le jeu et suivez les instructions des [premiers réglages](#first-time-setup) avant de jouer une ville sauvegardée.

Le ZIP de distribution contient la DLL et un dossier avec cinq fichiers DAT et trois fichiers INI. Après l’installation, vous devriez avoir :

```text
Documents\SimCity 4\Plugins\
    DepartmentEfficiency.dll
    Department of Efficiency\
        DepartmentEfficiency.dat
        DepartmentEfficiencyUI.dat
        DepartmentEfficiencyGarbageAdvice.dat
        DepartmentEfficiencyDealAdvice.dat
        DepartmentEfficiencyCivicAdvice.dat
        DepartmentEfficiency.ini
        DepartmentEfficiencyUtility.ini
        DepartmentEfficiencyDemolition.ini
```

Les fichiers Markdown et les captures d’écran du dépôt public ne sont **pas** nécessaires au jeu. Gardez la DLL **directement dans Plugins**, avec le dossier qui l’accompagne à côté.

**Modifier les réglages plus tard :** ouvrez **Options de jeu → Ministère de l’Optimisation**, utilisez les pages **Services** et **Démolition**, puis cliquez sur **Accepter**. Pour l’interrupteur général, vous pouvez aussi utiliser l’ordonnance **Instituer un Ministère de l’Optimisation**, sans coût mensuel. Désactiver le Ministère arrête ses actions futures ; cela n’annule ni les financements, ni les accords, ni les démolitions déjà appliqués.

<a id="features"></a>

## Ce que fait le Ministère

Le tableau ci-dessous décrit ce que fait le Ministère. Pour les formules, les exemples, les garde-fous et la mécanique derrière ses décisions, consultez la **[référence détaillée des fonctions](technical_docs/fr/index.md)**.

| Domaine | Ce qu’il fait |
|---|---|
| [Éducation et santé](technical_docs/fr/education_healthcare.md#education-healthcare) | Ajuste le financement **bâtiment par bâtiment** pour les écoles, universités, bibliothèques, musées, cliniques et hôpitaux éligibles. Il suit la fréquentation et la capacité réelles, avec une marge de sécurité. Il **ne modifie pas les rayons de couverture** ni le financement des bus scolaires et des ambulances. Si un bâtiment reste saturé au financement maximal, un conseiller vous indique qu’un autre équipement est nécessaire ; **vous choisissez où le construire**. |
| [Électricité](technical_docs/fr/power.md#power) | Gère **chaque centrale électrique individuellement** et choisit le financement qui couvre les besoins de la ville et la réserve de sécurité au moindre coût modélisé. Il tient compte du rendement et de l’état des centrales ; une option permet d’inclure aussi le coût économique de leur usure. |
| [Eau](technical_docs/fr/water.md#water) | Ajuste le financement de l’eau de la ville pour couvrir la demande plus une réserve de sécurité. La protection des canalisations est activée par défaut, pour éviter des coupes qui laisseraient le réseau se dégrader. Il masque aussi l’icône trompeuse « pas d’eau » du jeu au-dessus des fermes. |
| [Déchets](technical_docs/fr/garbage.md#garbage-landfills) | Ajuste le financement de la gestion des déchets d’après la production d’ordures, le recyclage, le traitement, le stockage des décharges et les accords régionaux, tout en protégeant les besoins propres de la ville. |
| [Accords avec les voisins](technical_docs/fr/deals.md#neighbor-deals) | Peut **acheter ou vendre de l’électricité et de l’eau**, **payer les voisins pour qu’ils prennent vos déchets** ou **accepter les leurs contre paiement**. Il crée, ajuste et annule les accords éligibles lorsque le résultat global profite à la ville et que ses propres besoins et réserves restent protégés. |
| [Démolition automatique](technical_docs/fr/demolition.md#demolition) | Peut déblayer les ruines après environ **30 jours simulés** et les bâtiments éligibles abandonnés depuis **une année simulée**, par défaut. Vous décidez quelles zones et quels motifs d’abandon comptent, si les ruines protégées peuvent être déblayées et si vous fixez un plafond de dépenses mensuel. |
| [Avis des conseillers](technical_docs/fr/safety.md#notices) | Utilise les conseillers du jeu pour signaler les pénuries avérées et persistantes que ni le financement ni les accords disponibles ne peuvent résoudre. Quand c’est utile, les messages contiennent des liens qui mènent aux bâtiments concernés. |
| [Conseil sur la taille de la décharge](technical_docs/fr/garbage.md#garbage-landfills-advice) | Un conseil optionnel indique un agrandissement précis de la décharge, ou une réduction sans risque, qui pourrait réduire la facture des déchets de plus que le seuil d’économie choisi. **Le Ministère conseille ; c’est à vous de décider de modifier ou non le zonage.** |
| [Réparation des cases fantômes](technical_docs/fr/garbage.md#ghost-tiles) | Quand la gestion des déchets est activée, répare sans risque les cases de décharge invisibles éligibles que le jeu compte encore dans l’entretien. Les cas risqués ou non vérifiés sont laissés tels quels ; cette réparation n’a pas d’interrupteur distinct. |

Vous gardez la main sur la construction, le zonage, les impôts, les routes et les transports, la police, les pompiers et tout autre service que le Ministère ne gère pas. Si vous activez la gestion d’un type d’accord avec les voisins, le Ministère peut aussi modifier ou annuler un accord que vous aviez conclu à la main. Désactivez cette option d’accords pour les gérer vous-même.

<a id="first-time-setup"></a>

## Premiers réglages

Le Ministère utilise les réglages fournis avec le plugin et **se met au travail automatiquement**. Vous n’avez pas besoin de l’activer, mais vous devriez vérifier ce qu’il a le droit de faire avant d’utiliser une ville sauvegardée importante.

1\. Lancez SimCity 4 et, depuis la **vue de la région**, ouvrez **Options de jeu → Ministère de l’Optimisation**. Si vous préférez, utilisez une ville neuve ou jetable pour un premier essai.

2\. Sur la page **Services**, choisissez les budgets et les accords avec les voisins que le Ministère peut gérer, et la marge de capacité qu’il doit conserver.

3\. Sur la page **Démolition**, vérifiez les règles de déblaiement des ruines et des bâtiments abandonnés. **Désactivez la démolition automatique si vous voulez tout raser vous-même.**

4\. Cliquez sur **Accepter** pour enregistrer vos choix. **Annuler** ferme la fenêtre sans enregistrer les changements.

5\. Ouvrez la ville que vous voulez jouer. Le Ministère suit vos préférences enregistrées et gère la ville pendant que la simulation avance.

Vos préférences sont stockées dans les fichiers INI du plugin et réutilisées quand vous ouvrez une autre ville. Le Ministère ne construit **pas** d’écoles, d’hôpitaux, de centrales électriques ni d’installations d’eau à votre place. Quand le financement et les accords disponibles ne suffisent pas à résoudre une pénurie avérée et persistante, un conseiller du jeu vous dit où il faut plus de capacité.

Vous pouvez aussi activer ou désactiver le Ministère par les ordonnances de la ville.

<a id="settings-explained"></a>

## Réglages

Ouvrez **Options de jeu → Ministère de l’Optimisation**. La fenêtre comporte deux pages, **Services** et **Démolition**. Les tableaux ci-dessous couvrent **tous les réglages affichés dans le menu**, sans les formules techniques. Cliquez sur le nom d’une option pour lire son explication complète dans la [référence des fonctions](technical_docs/fr/options.md#options).

**Fonctionnement des commandes :** « Activé » signifie que le Ministère a le droit de gérer cette fonction. Un réglage qui dépend d’un autre est grisé tant que ce dernier est désactivé. Les réglages en pourcentage fixent des marges de sécurité et des seuils, pour aider le Ministère à garder de la capacité en réserve et à absorber les variations de la demande.

### Services

| Option | Par défaut | Signification |
|---|---|---|
| [Ministère actif](technical_docs/fr/options.md#opt-master) | **Activé** | Interrupteur général du financement, des accords avec les voisins, de la démolition et des avis du Ministère. Le désactiver n’annule pas les changements déjà faits. |
| [Gérer l’éducation](technical_docs/fr/options.md#opt-education) | **Activé** | Ajuste automatiquement, un par un, les écoles, universités, bibliothèques et musées éligibles. Ne modifie ni leur couverture ni le financement des bus scolaires. |
| [Marge de sécurité (éducation)](technical_docs/fr/options.md#opt-educationreserve) | **5 %** (0–50 %) | Capacité supplémentaire au-dessus de l’utilisation actuelle de l’éducation. Plus de capacité coûte plus cher. Pour les bâtiments peu fréquentés, le Ministère peut garder une marge supplémentaire. Par exemple, une école pourra ainsi passer de 10 à 20 élèves (+100 %) sans grève, surtout au début, quand le surcoût est faible. |
| [Gérer la santé](technical_docs/fr/options.md#opt-health) | **Activé** | Ajuste automatiquement, un par un, les cliniques, hôpitaux et autres équipements de santé éligibles pris en charge. Ne modifie ni le financement des ambulances ni la couverture. |
| [Marge de sécurité (santé)](technical_docs/fr/options.md#opt-healthreserve) | **5 %** (0–50 %) | Fonctionne comme la marge de sécurité de l’éducation. |
| [Gérer l’électricité](technical_docs/fr/options.md#opt-power) | **Activé** | Ajuste le financement de chaque centrale électrique pour fournir assez d’électricité au moindre coût modélisé. |
| [Réserve d’électricité](technical_docs/fr/options.md#opt-powerreserve) | **20 %** (10–50 %) | Capacité de production de réserve pour la part de la demande couverte localement ; une réserve plus grande donne plus de marge pour suivre une croissance rapide et réduit le risque de pannes, mais peut coûter plus cher. |
| [Tenir compte de l’usure des centrales](technical_docs/fr/options.md#opt-poweraging) | **Désactivé** | Inclut dans l’optimisation le coût estimé de l’usure et du vieillissement des centrales. Peut favoriser un financement plus élevé quand le bénéfice à long terme de la conservation de la centrale dépasse le surcoût de fonctionnement. |
| [Gérer l’eau](technical_docs/fr/options.md#opt-water) | **Activé** | Ajuste le financement du service des eaux pour alimenter la ville sans dépense inutile. |
| [Réserve d’eau](technical_docs/fr/options.md#opt-waterreserve) | **10 %** (10–50 %) | Fonctionne comme la réserve d’électricité. |
| [Protéger les canalisations](technical_docs/fr/options.md#opt-waterpipes) | **Activé** | Maintient le financement au moins au niveau des 50 % du jeu, pour éviter d’endommager le réseau de canalisations. |
| [Gérer les déchets](technical_docs/fr/options.md#opt-sanitation) | **Activé** | Ajuste le financement du service des déchets pour couvrir les besoins de collecte et d’élimination. Répare aussi les cases fantômes (un bug du jeu qui facture l’entretien de cases de décharge dézonées). |
| [Réserve de traitement des déchets](technical_docs/fr/options.md#opt-sanitationreserve) | **2 %** (2–20 %) | Capacité de traitement supplémentaire pour les déchets de la ville, en plus de ce qu’elle produit après recyclage et de tout retard accumulé. |
| [M’avertir si revoir la décharge permet d’économiser](technical_docs/fr/options.md#opt-landfilladvice) | **Désactivé** | Active le conseil optionnel sur les modifications du zonage de la décharge, lorsqu’elles font économiser de l’argent tout en continuant à traiter les déchets de la ville. Nécessite la gestion des déchets. |
| [Seuil d’économie des alertes de décharge](technical_docs/fr/options.md#opt-landfillthreshold) | **25 %** (20–50 %) | Ne vous demande d’envisager un changement de taille de la décharge que si l’économie mensuelle modélisée atteint au moins cette part du budget des déchets. |
| [Gérer les accords avec les voisins](technical_docs/fr/options.md#opt-deals) | **Activé** | Active les accords régionaux automatiques ; les trois interrupteurs individuels ci-dessous décident quelles ressources peuvent être échangées. |
| [Acheter à une ville pour vendre à une autre](technical_docs/fr/options.md#opt-arbitrage) | **Désactivé** | Autorise des combinaisons rentables d’achat et de vente de la même ressource entre voisins, en gardant les besoins de la ville en priorité. |
| [Gérer les accords d’électricité](technical_docs/fr/options.md#opt-tradepower) | **Activé** | Autorise le Ministère à créer, modifier ou annuler des **importations et exportations** d’électricité. Indépendant du financement des centrales. |
| [Gérer les accords d’eau](technical_docs/fr/options.md#opt-tradewater) | **Activé** | Autorise le Ministère à créer, modifier ou annuler des **importations et exportations** d’eau. Indépendant du financement de l’eau. |
| [Gérer les accords de déchets](technical_docs/fr/options.md#opt-tradegarbage) | **Activé** | Autorise le Ministère à organiser le traitement des déchets **dans les deux sens**. Indépendant du financement des déchets. |

### Démolition

| Option | Par défaut | Signification |
|---|---|---|
| [Démolir les bâtiments détruits](technical_docs/fr/options.md#opt-demolishdestroyed) | **Activé** | Démolit les ruines après un certain délai. Ne déblaie pas automatiquement les ruines municipales ou historiques protégées, sauf si l’option ci-dessous l’autorise. |
| [Attendre (bâtiments détruits)](technical_docs/fr/options.md#opt-destroyedmonths) | **1 mois** (1–24) | Durée pendant laquelle les ruines doivent rester avant de pouvoir être déblayées ; environ **30 jours simulés** par défaut. |
| [Démolir les bâtiments municipaux et historiques détruits](technical_docs/fr/options.md#opt-demolishmunicipal) | **Désactivé** | Si activée, déblaie aussi, après le délai, les **ruines** éligibles de bâtiments municipaux, spéciaux et historiques. |
| [Démolir les bâtiments abandonnés](technical_docs/fr/options.md#opt-demolishabandoned) | **Activé** | Déblaie les bâtiments abandonnés éligibles après le délai d’attente, dans les types de zone et pour les motifs choisis. |
| [Attendre (bâtiments abandonnés)](technical_docs/fr/options.md#opt-abandonedmonths) | **12 mois** (1–60) | Durée pendant laquelle un bâtiment doit rester abandonné avant de pouvoir être démoli ; une année simulée par défaut. |
| [Zone résidentielle](technical_docs/fr/options.md#opt-zoneresidential) | **Activé** | Inclut les maisons et immeubles d’habitation abandonnés. |
| [Zone commerciale](technical_docs/fr/options.md#opt-zonecommercial) | **Activé** | Inclut les commerces et bureaux abandonnés. |
| [Zone industrielle](technical_docs/fr/options.md#opt-zoneindustrial) | **Activé** | Inclut les usines et autres bâtiments industriels abandonnés. |
| [Zone agricole](technical_docs/fr/options.md#opt-zoneagriculture) | **Désactivé** | Inclut les fermes abandonnées. Désactivée par défaut, car des cases vides ou abandonnées dans une ferme peuvent être normales. |
| [Motif : quartier peu attractif](technical_docs/fr/options.md#opt-reasonlowdesirability) | **Activé** | Inclut l’abandon causé par une mauvaise attractivité, comme la pollution ou le manque de services. |
| [Motif : trajet trop long](technical_docs/fr/options.md#opt-reasoncommute) | **Activé** | Inclut l’abandon causé par des trajets domicile-travail trop longs. |
| [Motif : pas de demande](technical_docs/fr/options.md#opt-reasonlowdemand) | **Activé** | Inclut les bâtiments abandonnés parce que la demande pour ce type a disparu. |
| [Motif : pas d’électricité](technical_docs/fr/options.md#opt-reasonnopower) | **Désactivé** | Inclut, en option, les bâtiments abandonnés faute d’électricité. Il vaut généralement mieux rétablir d’abord le courant pour voir si le bâtiment se repeuple. |
| [Motif : pas d’eau](technical_docs/fr/options.md#opt-reasonnowater) | **Désactivé** | Inclut, en option, les bâtiments abandonnés faute d’eau. Il vaut généralement mieux rétablir d’abord l’eau pour voir si le bâtiment se repeuple. |
| [Limiter le budget mensuel de démolition](technical_docs/fr/options.md#opt-budgetlimit) | **Désactivé** | Fixe un plafond de dépenses facultatif pour la démolition automatique, en plus des fonds disponibles dans la caisse de la ville. |
| [Budget mensuel](technical_docs/fr/options.md#opt-demolitionbudget) | **1 000 §** (0–200 000 §) | Dépense maximale de démolition par mois simulé **quand la limite de budget est activée**. |

<a id="screenshots"></a>

## Captures d’écran

Toutes les captures d’écran montrent la version anglaise du jeu.

| | |
|---|---|
| ![La page Services des réglages du Ministère](screenshots/Options-1.png) | ![La page Démolition des réglages du Ministère](screenshots/Options-2.png) |
| *La page Services : chaque service, sa réserve et les accords avec les voisins.* | *La page Démolition : ruines, bâtiments abandonnés et budget mensuel.* |
| ![Un avis de conseiller sur des lycées saturés](screenshots/high_school.png) | ![Un avis de conseiller sur des musées saturés](screenshots/museum.png) |
| *Des écoles au financement maximal et toujours saturées : l’avis renvoie aux bâtiments.* | *La même chose pour les musées.* |
| ![Un avis de conseiller sur une pénurie d’eau](screenshots/water.png) | ![Un avis de conseiller sur un manque de capacité pour les déchets](screenshots/real-garbage-paused.png) |
| *Eau : le Ministère ne peut pas résoudre la pénurie avec l’approvisionnement disponible.* | *Déchets : la ville a besoin de plus de capacité d’élimination.* |

<a id="requirements-and-compatibility"></a>

## Prérequis et compatibilité

- **SimCity 4 Deluxe, édition Steam, version du jeu 1.1.641.0, sous Windows.** J’ai testé le Ministère sur cette version ; les autres versions n’ont pas été testées.

- **Aucun autre mod, installateur ni application externe n’est nécessaire.**

- **Testé et compatible avec ces mods** (versions testées) :

  - Network Addon Mod (NAM) — 1.3.1
  - Colossus Addon Mod (CAM) — 3.1.1
  - SC4 System Integration Module — 4.0.1
  - CAM Ordinances — 3.3.0
  - Industrial Revolution Mod (IRM) — 2.0.0
  - Colossus Farming
  - Advanced Water Management — 1.0
  - SC4Fix — 1.0.0.7
  - More Building Styles — 3.6.1
  - More Demand Info — 1.3.1
  - Custom Budget Departments — 1.1
  - Custom Ordinance Host — 1.0.1
  - Legalize Gambling Ordinance Upgrade — 1.0.0
  - Lua Sandbox — 1.0.0
  - Lua Extensions — 1.5
  - Query Tool UI Extensions — 2.7.1
  - Resource Loading Hooks — 2.0.1
  - Transparent Texture Fix — 1.0.0
  - I-R Fix — 1.0.0
  - Universal Commercial Cap Fix — 2.0.0
  - Save Warning — 1.0
  - Zoning Mod Maxis — 2.0.0
  - Maxis Prop Names and Query Fix — 2.0
  - Seasonal Flora Patch — 2.1
  - No Maxis I-D, I-M e I-HT — 2.0

- **Recommandé :** ajoutez `-CPUCount:1` dans **Steam → Bibliothèque → SimCity 4 Deluxe → Propriétés → Options de lancement**. Cela a aidé la stabilité lors de longues sessions sur mon ordinateur.

- **Dix langues dans le jeu :** anglais, portugais, français, allemand, espagnol, italien, néerlandais, danois, suédois et polonais. Le Ministère suit automatiquement la langue de votre jeu.

<a id="updating-and-uninstalling"></a>

## Mise à jour et désinstallation

**Conservez vos préférences.** Les trois fichiers INI du dossier `Department of Efficiency` contiennent vos réglages. Avant de remplacer le mod, sauvegardez `DepartmentEfficiency.ini`, `DepartmentEfficiencyUtility.ini` et `DepartmentEfficiencyDemolition.ini` en dehors de votre dossier Plugins.

**Pour remplacer une installation :** fermez le jeu, supprimez la `DepartmentEfficiency.dll` et le dossier `Department of Efficiency` existants, installez le nouveau paquet, puis restaurez vos fichiers INI sauvegardés **uniquement si leur format est compatible** avec la nouvelle version. Sinon, réglez de nouveau vos préférences dans le jeu.

**Pour désinstaller :** fermez le jeu, supprimez `DepartmentEfficiency.dll` et le dossier `Department of Efficiency`. Vos villes sauvegardées restent lisibles sans le mod. Les niveaux de financement, les accords en cours et les démolitions déjà enregistrées dans une ville ne sont pas annulés automatiquement.

<a id="known-limitations"></a>

## Limites connues

- **Compatibilité :** j’ai testé la version **1.1.641.0** de SimCity 4 Deluxe pour Windows (Steam). Les autres versions du jeu n’ont pas été testées.

- **Conseil sur la décharge :** nécessite exactement une zone de décharge. Dans les villes qui ont plusieurs décharges, le Ministère peut toujours réparer les « cases fantômes » éligibles, mais ne recommande pas de changement de taille.

<a id="source-code"></a>

## Code source

Le code source du plugin se trouve dans le dossier [`source/`](source/). Il se compile avec CMake et Visual Studio en une DLL 32 bits (Win32). Il faut Visual Studio 2026 avec la charge de travail « Développement Desktop en C++ » (les autres versions n'ont pas été testées), CMake 3.20 ou plus récent et Git :

```bat
cmake -S . -B build -G "Visual Studio 18 2026" -A Win32
cmake --build build --config Release
```

La compilation télécharge [gzcom-dll](https://github.com/nsgomez/gzcom-dll) à une révision fixe (`779b669bf88112146d58fba6c7ed44f13ac6ce7d`). Pour utiliser une copie modifiée de cette bibliothèque, ou compiler sans connexion Internet, ajoutez `-DFETCHCONTENT_SOURCE_DIR_GZCOM_DLL=<dossier>` à la première commande. La DLL est créée dans `build\Release\DepartmentEfficiency.dll`. Une DLL recompilée n'est pas identique octet pour octet à celle publiée.

<a id="license"></a>

## Licence

- Le code original de Department of Efficiency (les fichiers de [`source/`](source/)) a été écrit par moi, **Sammu89** et est publié sous la **GNU General Public License, version 3 uniquement** (GPL-3.0-only). Le texte complet se trouve dans [LICENSE](LICENSE). Vous pouvez l'étudier, le modifier et le redistribuer selon la GPLv3 : conservez les mentions de copyright et les crédits, signalez vos modifications et publiez votre version sous la même licence. Merci de ne pas présenter de versions modifiées comme mes versions officielles.

- La documentation (ces README et les fichiers de [`technical_docs/`](technical_docs/)) et les textes écrits pour ce projet (y compris ceux des trois fichiers INI et ceux compilés dans `DepartmentEfficiency.dat`), ainsi que leurs traductions originales, sont publiés sous la même licence, GPL-3.0-only.

- **gzcom-dll** n'est pas couverte par cette licence : elle garde sa propre licence, LGPL-2.1-or-later ([licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt)), et est téléchargée depuis son dépôt d'origine lors de la compilation.

- **SimCity 4** et ses ressources d'origine appartiennent à Electronic Arts / Maxis. **Department of Efficiency** est un mod non officiel créé par un fan, indépendant d'EA et non approuvé par elle. Quatre des fichiers de `release/Department of Efficiency/` contiennent des scripts et des définitions d'interface dérivés des ressources du jeu lui-même, et les captures d'écran montrent le jeu : ils ne sont pas ma propriété, ne sont pas couverts par la GPL et tous les droits sur ceux-ci restent à leurs propriétaires. Il en va de même pour tout autre matériel de tiers.

<a id="credits"></a>

## Crédits

**Auteur**

- Créé par **Sammu89**.

**Bibliothèque utilisée**

- **gzcom-dll**, de Nelson Gomez (simmaster07), avec des contributions de Nicholas Hayes (Null 45), Casper Van Gheluwe, memo et d'autres (voir son [dépôt](https://github.com/nsgomez/gzcom-dll)), utilisée sous licence LGPL-2.1-or-later et liée statiquement à la DLL. La bibliothèque n'est pas incluse : la compilation télécharge la révision exacte depuis son dépôt d'origine (voir [Code source](#source-code)) ; les mentions de copyright et le texte de la licence se trouvent dans [licenses/gzcom-dll.txt](licenses/gzcom-dll.txt) et [licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt). Vous pouvez modifier la bibliothèque et recompiler le plugin avec votre version.

**Techniques, documentation et exemples consultés** (aucun code n'en a été copié ; leurs auteurs ne sont pas coauteurs de Department of Efficiency) :

- Nicholas Hayes (0xC0000054 / Null 45), pour ses recherches, sa documentation et ses exemples ouverts sur les DLL de SimCity 4 : sc4-dll-utilities, sc4-custom-budget-departments, sc4-query-ui-hooks, sc4-lua-extensions, sc4-lua-extension-example. La fenêtre d'options, l'ordonnance et le fichier d'avis suivent des mécanismes publiés dans ses projets sc4-region-census, sc4-humane-society-ordinance et sc4-lua-extension-example (licence MIT) ; les commandes du jeu suivent sc4-auto-run-cheats et sc4-game-command-documentation.

- CorinaMarie, pour avoir documenté la technique des Urgent Advice de SimCity 4 que suivent les avis de conseiller du mod.

- La communauté de modding de SimCity 4, pour vingt ans de recherches, d'outils et de connaissances partagées, dont la documentation des propriétés d'exemplaires de SC4Devotion.

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
