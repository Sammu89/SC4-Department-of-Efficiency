[English](README.md) | [Português](README.pt.md) | [Français](README.fr.md) | [Deutsch](README.de.md) | [Español](README.es.md) | [Italiano](README.it.md) | [Nederlands](README.nl.md) | **Dansk** | [Svenska](README.sv.md) | [Polski](README.pl.md)

# Effektivitetsministeriet

**Effektivitetsministeriet tager sig af de små detaljer i byens drift, som gør en stor forskel.**

Effektivitetsministeriet (*Department of Efficiency* på engelsk) er et plugin til **SimCity 4 Deluxe**, der automatiserer rutineopgaver i byens drift, så du kan koncentrere dig om at bygge, planlægge og få din by til at vokse.

**Du bygger byen. Ministeriet optimerer budgettet!**

Det styrer budgetterne for de enkelte uddannelses- og sundhedsbygninger, kraftværker, finansieringen af vand og affald, aftaler med nabobyer og, som valgfri funktion, nedrivning af ruiner og forladte bygninger. Målet er at dække byens behov uden at bruge mere end nødvendigt, så placeringen af bygninger, dækningen og de store planlægningsbeslutninger bliver hos dig.

Alt styres inde i spillet under **Spilindstillinger → Effektivitetsministeriet**. Vælg de områder, der skal automatiseres, juster deres sikkerhedsmarginer, indstil nedrivning, eller slå hele Ministeriet fra. Du kan også bruge byforordningen **Opret Effektivitetsministeriet** som hovedafbryder. Der skal ikke bruges nogen separat applikation.

> [!IMPORTANT]
> **Ministeriet starter slået til, også med automatisk nedrivning af ødelagte og forladte bygninger.** Gennemgå dets indstillinger, før du åbner en by, du holder af. Det foretager rigtige ændringer i spillet: finansiering, aftaler og nedrivninger bliver gemt, hvis du gemmer byen. Det redigerer ikke gemte spil direkte.

## Indhold

- [Download og installation](#download-and-installation)

- [Hvad Ministeriet gør](#features)

- [Første opsætning](#first-time-setup)

- [Indstillinger](#settings-explained)

- [Skærmbilleder](#screenshots)

- [Krav og kompatibilitet](#requirements-and-compatibility)

- [Opdatering og afinstallation](#updating-and-uninstalling)

- [Kendte begrænsninger](#known-limitations)

- [Tak til](#credits)

- [LGPL-licensmateriale](#lgpl-licensing)

<a id="download-and-installation"></a>

## Download og installation

### [⬇ Download Department of Efficiency 1.0.0](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases/download/v1.0.0/Departement_of_Efficiency_1.00.zip)

1\. Åbn projektets [release-side](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases), og vælg den seneste udgave.

2\. Under **Assets** downloader du **distributions-ZIP'en til Department of Efficiency**.

3\. **Luk SimCity 4.** Pak ZIP'en ud i en midlertidig mappe.

4\. Kopiér **`DepartmentEfficiency.dll`** og hele mappen **`Department of Efficiency`** til roden af `Dokumenter\SimCity 4\Plugins`.

5\. Start spillet, og følg vejledningen i [første opsætning](#first-time-setup), før du spiller en gemt by.

Distributions-ZIP'en indeholder DLL'en og en mappe med fem DAT- og tre INI-filer. Efter installationen skal du have:

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

Det offentlige repositorys Markdown-filer og skærmbilleder er **ikke** nødvendige for spillet. Behold DLL'en **direkte i Plugins** med den tilhørende mappe ved siden af.

**Ændre indstillinger senere:** åbn **Spilindstillinger → Effektivitetsministeriet**, brug siderne **Serviceområder** og **Nedrivning**, og klik på **Accepter**. Til hovedafbryderen kan du også bruge forordningen **Opret Effektivitetsministeriet** uden månedlige omkostninger. At slå Ministeriet fra stopper fremtidige handlinger; det fortryder ikke finansiering, aftaler og nedrivninger, der allerede er udført.

<a id="features"></a>

## Hvad Ministeriet gør

Tabellen nedenfor beskriver, hvad Ministeriet gør. Formler, eksempler, sikkerhedsforanstaltninger og mekanikken bag beslutningerne finder du i den **[detaljerede funktionsreference (på engelsk)](features/en.md)**.

| Område | Hvad det gør |
|---|---|
| [Uddannelse og sundhed](features/en.md#education-healthcare) | Justerer finansieringen **bygning for bygning** for egnede skoler, universiteter, biblioteker, museer, klinikker og hospitaler. Det følger den faktiske brug og kapacitet med en sikkerhedsmargin. Det **ændrer ikke dækningsradier** og heller ikke finansieringen af skolebusser og ambulancer. Hvis en bygning stadig er overfyldt ved maksimal finansiering, melder en rådgiver, at der er brug for endnu en bygning; **du vælger, hvor den skal ligge**. |
| [El](features/en.md#electricity) | Styrer **hvert enkelt kraftværk for sig** og vælger den finansiering, der dækker byens behov og sikkerhedsreserven til de laveste modellerede omkostninger. Det tager hensyn til kraftværkernes effektivitet og stand; en valgfri indstilling medregner også de økonomiske omkostninger ved kraftværkernes slid. |
| [Vand](features/en.md#water) | Justerer byens vandfinansiering, så den dækker behovet plus en sikkerhedsreserve. Beskyttelse af vandrørene er slået til som standard for at undgå nedskæringer, der ville lade nettet forfalde. Det skjuler også spillets misvisende „intet vand“-ikon over gårde. |
| [Affaldshåndtering](features/en.md#garbage-landfills) | Justerer finansieringen af affaldshåndteringen ud fra affaldsmængde, genbrug, behandling, lossepladslager og regionale aftaler og beskytter samtidig byens eget behov for bortskaffelse. |
| [Aftaler med nabobyer](features/en.md#neighbor-deals) | Kan **købe eller sælge el og vand**, **betale naboer for at tage imod dit affald** eller **modtage deres mod betaling**. Det opretter, justerer og opsiger egnede aftaler, når det samlede resultat gavner byen, og dens egne behov og reserver forbliver beskyttet. |
| [Automatisk nedrivning](features/en.md#demolition) | Kan som standard rydde ruiner efter ca. **30 simulerede dage** og egnede bygninger, der har stået forladt i **ét simuleret år**. Du bestemmer, hvilke zoner og hvilke årsager til forladthed der tæller, om beskyttede ruiner må ryddes, og om der skal gælde en månedlig udgiftsgrænse. |
| [Rådgivernes beskeder](features/en.md#notices) | Bruger spillets egne rådgivere til at melde påviste, vedvarende mangler, som hverken finansiering eller tilgængelige aftaler kan afhjælpe. Hvor det giver mening, indeholder beskederne links til de berørte bygninger. |
| [Råd om lossepladsens størrelse](features/en.md#garbage-landfills-advice) | Det valgfrie råd peger på en bestemt udvidelse af lossepladsen eller en sikker indskrænkning, der kan skære affaldsregningen med mere end din valgte sparegrænse. **Ministeriet rådgiver; du afgør, om zoneinddelingen skal ændres.** |
| [Reparation af spøgelsesfelter](features/en.md#ghost-squares) | Når styringen af affaldshåndteringen er slået til, repareres egnede usynlige lossepladsfelter, som spillet stadig tæller med i vedligeholdelsen, på en sikker måde. Usikre eller ikke-verificerede tilfælde lades i fred; der er ingen separat kontakt til denne reparation. |

Du bestemmer fortsat over byggeri, zoneinddeling, skatter, veje og transport, politi, brandvæsen og alle andre områder, som Ministeriet ikke styrer. Hvis du slår styringen af en type nabo-aftale til, kan Ministeriet også ændre eller opsige en aftale, du selv har lavet manuelt. Slå den aftalefunktion fra, hvis du selv vil styre de aftaler.

<a id="first-time-setup"></a>

## Første opsætning

Ministeriet bruger de indstillinger, der følger med pluginnet, og **går automatisk i gang**. Du behøver ikke slå det til, men du bør gennemgå, hvad det må, før du bruger en vigtig gemt by.

1\. Start SimCity 4, og åbn fra **regionsvisningen** **Spilindstillinger → Effektivitetsministeriet**. Hvis du vil, kan du bruge en ny eller overflødig testby til det første kig.

2\. På siden **Serviceområder** vælger du, hvilke budgetter og nabo-aftaler Ministeriet må styre, og hvor meget ekstra kapacitet det skal holde i reserve.

3\. På siden **Nedrivning** tjekker du reglerne for rydning af ruiner og forladte bygninger. **Slå automatisk nedrivning fra, hvis du selv vil jævne alt med jorden.**

4\. Klik på **Accepter** for at gemme dine valg. **Annuller** lukker vinduet uden at gemme ændringer.

5\. Åbn den by, du vil spille. Ministeriet følger dine gemte præferencer og styrer byen, mens simuleringen kører.

Dine præferencer gemmes i pluginnets INI-filer og genbruges, når du åbner en anden by. Ministeriet bygger **ikke** skoler, hospitaler, kraftværker eller vandanlæg for dig. Når finansiering og tilgængelige aftaler ikke kan løse en påvist, vedvarende mangel, fortæller en rådgiver i spillet, hvor der er brug for mere kapacitet.

Du kan også aktivere eller deaktivere Ministeriet via byens forordninger.

<a id="settings-explained"></a>

## Indstillinger

Åbn **Spilindstillinger → Effektivitetsministeriet**. Vinduet har to sider, **Serviceområder** og **Nedrivning**. Tabellerne nedenfor dækker **alle indstillinger i menuen** uden de tekniske formler. Klik på en indstillings navn for at læse den fulde forklaring i [funktionsreferencen](features/en.md#options).

**Sådan virker knapperne:** „Til“ betyder, at Ministeriet må styre den pågældende funktion. En indstilling, der afhænger af en anden, er grå, så længe den overordnede er slået fra. Procentindstillinger styrer sikkerhedsmarginer og grænser og hjælper Ministeriet med at holde ekstra kapacitet i reserve og klare udsving i efterspørgslen.

### Serviceområder

| Indstilling | Standard | Hvad det betyder |
|---|---|---|
| [Ministeriet er aktivt](features/en.md#opt-master) | **Til** | Hovedafbryder for finansiering, nabo-aftaler, nedrivning og Ministeriets beskeder. At slå det fra fortryder ikke de ændringer, der allerede er foretaget. |
| [Styr uddannelse](features/en.md#opt-education) | **Til** | Justerer automatisk egnede skoler, universiteter, biblioteker og museer enkeltvis. Ændrer hverken deres dækning eller finansieringen af skolebusser. |
| [Sikkerhedsmargin (uddannelse)](features/en.md#opt-educationreserve) | **5%** (0–50%) | Ekstra kapacitet ud over uddannelsernes nuværende brug. Mere kapacitet koster mere. For bygninger med lav belægning kan Ministeriet holde en ekstra margin. Det kan fx lade en skole vokse fra 10 til 20 elever (+100%) uden strejke, især i starten, hvor merudgiften er lille. |
| [Styr sundhed](features/en.md#opt-health) | **Til** | Justerer automatisk egnede klinikker, hospitaler og andre understøttede sundhedsbygninger enkeltvis. Ændrer hverken ambulancernes finansiering eller dækningen. |
| [Sikkerhedsmargin (sundhed)](features/en.md#opt-healthreserve) | **5%** (0–50%) | Virker på samme måde som sikkerhedsmarginen for uddannelse. |
| [Styr el](features/en.md#opt-power) | **Til** | Justerer finansieringen af de enkelte kraftværker, så der leveres nok el til de laveste modellerede omkostninger. |
| [Elreserve](features/en.md#opt-powerreserve) | **20%** (10–50%) | Ekstra produktionskapacitet til den del af efterspørgslen, der dækkes lokalt; større reserver giver mere plads til hurtig vækst og mindsker risikoen for strømsvigt, men kan koste mere. |
| [Tag hensyn til kraftværkernes slid](features/en.md#opt-poweraging) | **Fra** | Medregner de anslåede omkostninger ved kraftværkernes slid og aldring i optimeringen. Kan favorisere højere finansiering, når den langsigtede gevinst ved at bevare kraftværket opvejer de ekstra driftsomkostninger. |
| [Styr vand](features/en.md#opt-water) | **Til** | Justerer vandforsyningens finansiering, så byen forsynes uden unødvendige udgifter. |
| [Vandreserve](features/en.md#opt-waterreserve) | **10%** (10–50%) | Virker på samme måde som elreserven. |
| [Beskyt vandrør](features/en.md#opt-waterpipes) | **Til** | Holder finansieringen på eller over spillets 50% for at undgå skader på rørnettet. |
| [Styr affaldshåndtering](features/en.md#opt-sanitation) | **Til** | Justerer affaldsafdelingens finansiering, så behovet for indsamling og bortskaffelse dækkes. Reparerer også spøgelsesfelter (en fejl i spillet, der opkræver vedligeholdelse for lossepladsfelter uden zone). |
| [Reserve til affaldsbehandling](features/en.md#opt-sanitationreserve) | **2%** (2–20%) | Ekstra behandlingskapacitet til byens affald oven i det, den producerer efter genbrug, og eventuelle efterslæb. |
| [Advar mig, når lossepladsens størrelse kan spare penge](features/en.md#opt-landfilladvice) | **Fra** | Slår det valgfrie råd om ændringer i lossepladsens zoneinddeling til, hvis det sparer penge, og byens affald stadig bliver håndteret. Kræver styring af affaldshåndtering. |
| [Sparegrænse for lossepladsbeskeder](features/en.md#opt-landfillthreshold) | **25%** (20–50%) | Beder dig kun overveje en ændring af lossepladsens størrelse, hvis den modellerede månedlige besparelse udgør mindst denne andel af affaldsbudgettet. |
| [Styr aftaler med nabobyer](features/en.md#opt-deals) | **Til** | Slår automatiske regionale aftaler til; de tre enkeltkontakter nedenfor afgør, hvilke ressourcer der må handles. |
| [Køb fra én by, og sælg til en anden](features/en.md#opt-arbitrage) | **Fra** | Tillader rentable kombinationer af køb og salg af samme ressource mellem naboer, og byens egne behov kommer stadig først. |
| [Styr aftaler om el](features/en.md#opt-tradepower) | **Til** | Lader Ministeriet oprette, ændre eller opsige **import og eksport** af el. Uafhængig af finansieringen af kraftværkerne. |
| [Styr aftaler om vand](features/en.md#opt-tradewater) | **Til** | Lader Ministeriet oprette, ændre eller opsige **import og eksport** af vand. Uafhængig af vandfinansieringen. |
| [Styr aftaler om affald](features/en.md#opt-tradegarbage) | **Til** | Lader Ministeriet arrangere affaldsbehandling **i begge retninger**. Uafhængig af finansieringen af affaldshåndteringen. |

### Nedrivning

| Indstilling | Standard | Hvad det betyder |
|---|---|---|
| [Ryd ødelagte bygninger](features/en.md#opt-demolishdestroyed) | **Til** | River ruiner ned efter en vis tid. Rydder ikke automatisk beskyttede kommunale eller historiske ruiner, medmindre indstillingen nedenfor tillader det. |
| [Ventetid (ødelagte bygninger)](features/en.md#opt-destroyedmonths) | **1 måned** (1–24) | Hvor længe ruiner skal blive stående, før de kan ryddes; som standard ca. **30 simulerede dage**. |
| [Ryd ødelagte kommunale og historiske bygninger](features/en.md#opt-demolishmunicipal) | **Fra** | Hvis slået til, ryddes også egnede **ruiner** af kommunale, særlige og historiske bygninger efter ventetiden. |
| [Riv forladte bygninger ned](features/en.md#opt-demolishabandoned) | **Til** | Rydder egnede forladte bygninger efter ventetiden, i de valgte zonetyper og af de valgte årsager. |
| [Ventetid (forladte bygninger)](features/en.md#opt-abandonedmonths) | **12 måneder** (1–60) | Hvor længe en bygning skal stå forladt, før den må rives ned; som standard ét simuleret år. |
| [Boligzone](features/en.md#opt-zoneresidential) | **Til** | Omfatter forladte huse og etageejendomme. |
| [Handelszone](features/en.md#opt-zonecommercial) | **Til** | Omfatter forladte butikker og kontorer. |
| [Industrizone](features/en.md#opt-zoneindustrial) | **Til** | Omfatter forladte fabrikker og andre industribygninger. |
| [Landbrugszone](features/en.md#opt-zoneagriculture) | **Fra** | Omfatter forladte gårde. Slået fra som standard, fordi tomme eller forladte felter på en gård kan være normale. |
| [Årsag: uattraktivt område](features/en.md#opt-reasonlowdesirability) | **Til** | Omfatter forladthed på grund af dårlig attraktivitet, fx forurening eller mangel på service. |
| [Årsag: for lang vej til arbejde](features/en.md#opt-reasoncommute) | **Til** | Omfatter forladthed på grund af for lange rejser til arbejde. |
| [Årsag: ingen efterspørgsel](features/en.md#opt-reasonlowdemand) | **Til** | Omfatter bygninger, der er forladt, fordi efterspørgslen efter den type er forsvundet. |
| [Årsag: ingen strøm](features/en.md#opt-reasonnopower) | **Fra** | Omfatter valgfrit bygninger, der er forladt på grund af manglende el. Normalt er det bedre først at genoprette strømmen og se, om bygningen bliver beboet igen. |
| [Årsag: intet vand](features/en.md#opt-reasonnowater) | **Fra** | Omfatter valgfrit bygninger, der er forladt på grund af manglende vand. Normalt er det bedre først at genoprette vandet og se, om bygningen bliver beboet igen. |
| [Begræns det månedlige nedrivningsbudget](features/en.md#opt-budgetlimit) | **Fra** | Fastsætter et valgfrit udgiftsloft for automatisk nedrivning ud over de tilgængelige midler i byens kasse. |
| [Månedligt budget](features/en.md#opt-demolitionbudget) | **§1.000** (§0–§200.000) | Maksimale nedrivningsudgifter pr. simuleret måned, **når budgetgrænsen er slået til**. |

<a id="screenshots"></a>

## Skærmbilleder

Alle skærmbilleder viser den engelske version af spillet.

| | |
|---|---|
| ![Siden Serviceområder i Ministeriets indstillinger](screenshots/Options-1.png) | ![Siden Nedrivning i Ministeriets indstillinger](screenshots/Options-2.png) |
| *Siden Serviceområder: hvert område, dets reserve og nabo-aftalerne.* | *Siden Nedrivning: ruiner, forladte bygninger og det månedlige budget.* |
| ![En rådgiverbesked om overfyldte gymnasier](screenshots/high_school.png) | ![En rådgiverbesked om overfyldte museer](screenshots/museum.png) |
| *Skoler ved maksimal finansiering og stadig overfyldte: beskeden linker til bygningerne.* | *Det samme for museer.* |
| ![En rådgiverbesked om vandmangel](screenshots/water.png) | ![En rådgiverbesked om mangel på affaldskapacitet](screenshots/real-garbage-paused.png) |
| *Vand: Ministeriet kan ikke afhjælpe manglen med den tilgængelige forsyning.* | *Affald: byen har brug for mere kapacitet til bortskaffelse.* |

<a id="requirements-and-compatibility"></a>

## Krav og kompatibilitet

- **SimCity 4 Deluxe, Steam-udgaven, spilversion 1.1.641.0, på Windows.** Det er den understøttede version; Ministeriet forbliver inaktivt på andre versioner.

- **Ingen anden mod, installationsprogram eller ekstern applikation er nødvendig.**

- **Testet og kompatibel med disse mods** (testede versioner):

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

- **Anbefalet:** tilføj `-CPUCount:1` under **Steam → Bibliotek → SimCity 4 Deluxe → Egenskaber → Startindstillinger**. Det hjalp på stabiliteten i lange sessioner på forfatterens computer.

- **Ti sprog i spillet:** engelsk, portugisisk, fransk, tysk, spansk, italiensk, nederlandsk, dansk, svensk og polsk. Ministeriet følger automatisk dit spils sprog.

<a id="updating-and-uninstalling"></a>

## Opdatering og afinstallation

**Bevar dine præferencer.** De tre INI-filer i mappen `Department of Efficiency` gemmer dine indstillinger. Tag, før du udskifter modden, en sikkerhedskopi af `DepartmentEfficiency.ini`, `DepartmentEfficiencyUtility.ini` og `DepartmentEfficiencyDemolition.ini` uden for din Plugins-mappe.

**Sådan udskifter du en installation:** luk spillet, fjern den eksisterende `DepartmentEfficiency.dll` og mappen `Department of Efficiency`, installér den nye pakke, og gendan dine sikkerhedskopierede INI-filer **kun hvis deres format er kompatibelt** med den nye udgave. Ellers indstiller du dine præferencer igen i spillet.

**Sådan afinstallerer du:** luk spillet, og slet `DepartmentEfficiency.dll` og mappen `Department of Efficiency`. Dine gemte byer kan stadig læses uden modden. Finansieringsniveauer, eksisterende aftaler og nedrivninger, der allerede er gemt i en by, fortrydes ikke automatisk.

<a id="known-limitations"></a>

## Kendte begrænsninger

- **Kompatibilitet:** kun Windows-Steam-versionen **1.1.641.0** af SimCity 4 Deluxe understøttes. Andre spilversioner forbliver inaktive.

- **Råd om lossepladsen:** kræver præcis én lossepladszone. I byer med flere losseplader kan Ministeriet stadig reparere egnede fejlramte „spøgelsesfelter“, men anbefaler ikke ændringer af størrelsen.

<a id="credits"></a>

## Tak til

- Skabt af **Sammu89**.

- Bygget med **gzcom-dll** af Nelson Gomez (simmaster07), brugt under licensen LGPL-2.1-or-later og statisk linket ind i DLL'en.

- Nicholas Hayes (0xC0000054 / Null 45), for forskning, dokumentation og åbne eksempler om SimCity 4-DLL'er: sc4-dll-utilities, sc4-custom-budget-departments, sc4-query-ui-hooks, sc4-lua-extensions, sc4-lua-extension-example.

- CorinaMarie, for at dokumentere SimCity 4's Urgent Advice-teknik, som modens rådgiverbeskeder følger.

- SimCity 4-modding-miljøet, for to årtiers forskning, værktøjer og delt viden.

<a id="lgpl-licensing"></a>

## LGPL-licensmateriale

Effektivitetsministeriet bruger [gzcom-dll](https://github.com/nsgomez/gzcom-dll), som er udgivet under LGPL-2.1-or-later.

Den tilhørende kildekode til biblioteket, licensmeddelelser og materialer til genlinkning findes i [pakken med LGPL-materiale](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases/download/v1.0.0/DepartmentEfficiency-LGPL-Compliance-1.0.0.zip).

Disse materialer er ikke nødvendige for at installere eller bruge moddet.
