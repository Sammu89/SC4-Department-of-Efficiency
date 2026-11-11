[English](README.md) | [Português](README.pt.md) | [Français](README.fr.md) | [Deutsch](README.de.md) | [Español](README.es.md) | [Italiano](README.it.md) | [Nederlands](README.nl.md) | **Dansk** | [Svenska](README.sv.md) | [Polski](README.pl.md)

# Effektivitetsministeriet

**Effektivitetsministeriet tager sig af de små detaljer i byens drift, som gør en stor forskel.**

Effektivitetsministeriet (*Department of Efficiency* på engelsk) er et plugin til **SimCity 4 Deluxe**, der automatiserer rutineopgaver i byens drift, så du kan koncentrere dig om at bygge, planlægge og få din by til at vokse.

**Du bygger byen. Ministeriet optimerer budgettet!**

Det styrer budgetterne for de enkelte uddannelses- og sundhedsbygninger, kraftværker, finansieringen af vand og affald, aftaler med nabobyer og, som valgfri funktion, nedrivning af ruiner og forladte bygninger. Målet er at dække byens behov uden at bruge mere end nødvendigt, så placeringen af bygninger, dækningen og de store planlægningsbeslutninger bliver hos dig.

Alt styres inde i spillet under **Spilindstillinger → Effektivitetsministeriet**. Vælg de områder, der skal automatiseres, juster deres sikkerhedsmarginer, indstil nedrivning, eller slå hele Ministeriet fra. Du kan også bruge byforordningen **Opret Effektivitetsministeriet** som hovedafbryder. Der skal ikke bruges nogen separat applikation.

> [!IMPORTANT]
> **Ministeriet starter slået til, men automatisk nedrivning er slået fra som standard.** Gennemgå dets indstillinger, før du åbner en by, du holder af. Det foretager rigtige ændringer i spillet: finansiering og aftaler (og nedrivninger, hvis du slår dem til) bliver gemt, hvis du gemmer byen. Det redigerer ikke gemte spil direkte.

## Indhold

- [Download og installation](#download-and-installation)

- [Hvad Ministeriet gør](#features)

- [Første opsætning](#first-time-setup)

- [Indstillinger](#settings-explained)

- [Skærmbilleder](#screenshots)

- [Krav og kompatibilitet](#requirements-and-compatibility)

- [Kompatibilitet med Phantom Funding Slider Bug](#phantom-funding-slider-bug)

- [Opdatering og afinstallation](#updating-and-uninstalling)

- [Kendte begrænsninger](#known-limitations)

- [Kildekode](#source-code)

- [Licens](#license)

- [Tak til](#credits)

<a id="download-and-installation"></a>

## Installation

### [⬇ Download Department of Efficiency 1.0.3](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases/download/v1.0.3/Department_of_Efficiency_1.0.3.zip)

**Nyt i 1.0.3:** se [ændringsloggen](CHANGELOG.md) (på engelsk).

1. Download den [nyeste version](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases/latest) og luk SimCity 4.

2. Udpak ZIP-filen direkte til `Documents\SimCity 4\Plugins`.

3. Kontrollér, at `DepartmentEfficiency.dll`, `DepartmentEfficiency.dat` og `DepartmentEfficiency.ini` ligger i Plugins-mappen.

4. Start SimCity 4, og åbn **Spilindstillinger → Effektivitetsministeriet** for at vælge dine indstillinger.

> [!IMPORTANT]
> Ministeriet er aktiveret som standard; automatisk nedrivning er som standard **slået fra**. Hvis du slår den til, så kontrollér indstillingerne for **nedrivning**, før du åbner en by med bygninger, du vil bevare.

<a id="features"></a>

## Hvad Ministeriet gør

Tabellen nedenfor beskriver, hvad Ministeriet gør. Formler, eksempler, sikkerhedsforanstaltninger og mekanikken bag beslutningerne finder du i den **[detaljerede funktionsreference (på engelsk)](technical_docs/en/index.md)**.

| Område | Hvad det gør |
|---|---|
| [Uddannelse og sundhed](technical_docs/en/education_healthcare.md#education-healthcare) | Justerer finansieringen **bygning for bygning** for egnede skoler, universiteter, biblioteker, museer, klinikker og hospitaler. Det følger den faktiske brug og kapacitet med en sikkerhedsmargin. Det **ændrer ikke dækningsradier** og heller ikke finansieringen af skolebusser og ambulancer. Hvis en bygning stadig er overfyldt ved maksimal finansiering, melder en rådgiver, at der er brug for endnu en bygning; **du vælger, hvor den skal ligge**. |
| [El](technical_docs/en/power.md#power) | Styrer **hvert enkelt kraftværk for sig** og vælger den finansiering, der dækker byens behov og sikkerhedsreserven til de laveste modellerede omkostninger. Det tager hensyn til kraftværkernes effektivitet og stand; en valgfri indstilling medregner også de økonomiske omkostninger ved kraftværkernes slid. |
| [Vand](technical_docs/en/water.md#water) | Justerer byens vandfinansiering, så den dækker behovet plus en sikkerhedsreserve. Beskyttelse af vandrørene er slået til som standard for at undgå nedskæringer, der ville lade nettet forfalde. Det skjuler også spillets misvisende „intet vand“-ikon over gårde. |
| [Affaldshåndtering](technical_docs/en/garbage.md#garbage-landfills) | Justerer finansieringen af affaldshåndteringen ud fra affaldsmængde, genbrug, behandling, lossepladslager og regionale aftaler og beskytter samtidig byens eget behov for bortskaffelse. |
| [Aftaler med nabobyer](technical_docs/en/deals.md#neighbor-deals) | Kan **købe eller sælge el og vand**, **betale naboer for at tage imod dit affald** eller **modtage deres mod betaling**. Det opretter, justerer og opsiger egnede aftaler, når det samlede resultat gavner byen, og dens egne behov og reserver forbliver beskyttet. |
| [Automatisk nedrivning](technical_docs/en/demolition.md#demolition) | **Slået fra som standard.** Kan som standard rydde ruiner efter ca. **30 simulerede dage** og egnede bygninger, der har stået forladt i **ét simuleret år**. Du bestemmer, hvilke zoner og hvilke årsager til forladthed der tæller, om beskyttede ruiner må ryddes, og om der skal gælde en månedlig udgiftsgrænse. |
| [Rådgivernes beskeder](technical_docs/en/safety.md#notices) | Bruger spillets egne rådgivere til at melde påviste, vedvarende mangler, som hverken finansiering eller tilgængelige aftaler kan afhjælpe. Hvor det giver mening, indeholder beskederne links til de berørte bygninger. Det tilføjer også en nyhed (titel på listen, detaljer ved klik), når en tjeneste, du har aktiveret, må stoppe på grund af en teknisk fejl. |
| [Råd om lossepladsens størrelse](technical_docs/en/garbage.md#garbage-landfills-advice) | Det valgfrie råd peger på en bestemt, sikker indskrænkning af lossepladsen (tomme felter, der kan fjernes), der kan skære affaldsregningen med mere end din valgte sparegrænse. **Ministeriet rådgiver; du afgør, om zoneinddelingen skal ændres.** |
| [Reparation af spøgelsesfelter](technical_docs/en/garbage.md#ghost-tiles) | Når styringen af affaldshåndteringen er slået til, repareres egnede usynlige lossepladsfelter, som spillet stadig tæller med i vedligeholdelsen, på en sikker måde. Usikre eller ikke-verificerede tilfælde lades i fred; der er ingen separat kontakt til denne reparation. |

Du bestemmer fortsat over byggeri, zoneinddeling, skatter, veje og transport, politi, brandvæsen og alle andre områder, som Ministeriet ikke styrer. Hvis du slår styringen af en type nabo-aftale til, kan Ministeriet også ændre eller opsige en aftale, du selv har lavet manuelt. Slå den aftalefunktion fra, hvis du selv vil styre de aftaler.

<a id="first-time-setup"></a>

## Første opsætning

Ministeriet bruger de indstillinger, der følger med pluginnet, og **går automatisk i gang**. Du behøver ikke slå det til, men du bør gennemgå, hvad det må, før du bruger en vigtig gemt by.

1\. Start SimCity 4, og åbn fra **regionsvisningen** **Spilindstillinger → Effektivitetsministeriet**. Hvis du vil, kan du bruge en ny eller overflødig testby til det første kig.

2\. På siden **Serviceområder** vælger du, hvilke budgetter og nabo-aftaler Ministeriet må styre, og hvor meget ekstra kapacitet det skal holde i reserve.

3\. På siden **Nedrivning** er automatisk nedrivning slået fra. Sæt kryds ved indstillingerne for ruiner og forladte bygninger, kun hvis du vil have Ministeriet til at rydde dem for dig.

4\. Klik på **Accepter** for at gemme dine valg. **Annuller** lukker vinduet uden at gemme ændringer.

5\. Åbn den by, du vil spille. Ministeriet følger dine gemte præferencer og styrer byen, mens simuleringen kører.

Dine præferencer gemmes i `DepartmentEfficiency.ini` og genbruges, når du åbner en anden by. Ministeriet bygger **ikke** skoler, hospitaler, kraftværker eller vandanlæg for dig. Når finansiering og tilgængelige aftaler ikke kan løse en påvist, vedvarende mangel, fortæller en rådgiver i spillet, hvor der er brug for mere kapacitet.

Du kan også aktivere eller deaktivere Ministeriet via byens forordninger.

<a id="settings-explained"></a>

## Indstillinger

Åbn **Spilindstillinger → Effektivitetsministeriet**. Vinduet har to sider, **Serviceområder** og **Nedrivning**. Tabellerne nedenfor dækker **alle indstillinger i menuen** uden de tekniske formler. Klik på en indstillings navn for at læse den fulde forklaring i [funktionsreferencen](technical_docs/en/options.md#options).

**Sådan virker knapperne:** „Til“ betyder, at Ministeriet må styre den pågældende funktion. En indstilling, der afhænger af en anden, er grå, så længe den overordnede er slået fra. Procentindstillinger styrer sikkerhedsmarginer og grænser og hjælper Ministeriet med at holde ekstra kapacitet i reserve og klare udsving i efterspørgslen.

### Serviceområder

| Indstilling | Standard | Hvad det betyder |
|---|---|---|
| [Ministeriet er aktivt](technical_docs/en/options.md#opt-master) | **Til** | Hovedafbryder for finansiering, nabo-aftaler, nedrivning og Ministeriets beskeder. At slå det fra fortryder ikke de ændringer, der allerede er foretaget. |
| [Styr uddannelse](technical_docs/en/options.md#opt-education) | **Til** | Justerer automatisk egnede skoler, universiteter, biblioteker og museer enkeltvis. Ændrer hverken deres dækning eller finansieringen af skolebusser. |
| [Sikkerhedsmargin (uddannelse)](technical_docs/en/options.md#opt-educationreserve) | **5%** (0–50%) | Ekstra kapacitet ud over uddannelsernes nuværende brug. Mere kapacitet koster mere. For bygninger med lav belægning kan Ministeriet holde en ekstra margin. Det kan fx lade en skole vokse fra 10 til 20 elever (+100%) uden strejke, især i starten, hvor merudgiften er lille. |
| [Styr sundhed](technical_docs/en/options.md#opt-health) | **Til** | Justerer automatisk egnede klinikker, hospitaler og andre understøttede sundhedsbygninger enkeltvis. Ændrer hverken ambulancernes finansiering eller dækningen. |
| [Sikkerhedsmargin (sundhed)](technical_docs/en/options.md#opt-healthreserve) | **5%** (0–50%) | Virker på samme måde som sikkerhedsmarginen for uddannelse. |
| [Styr el](technical_docs/en/options.md#opt-power) | **Til** | Justerer finansieringen af de enkelte kraftværker, så der leveres nok el til de laveste modellerede omkostninger. |
| [Elreserve](technical_docs/en/options.md#opt-powerreserve) | **20%** (10–50%) | Ekstra produktionskapacitet til den del af efterspørgslen, der dækkes lokalt; større reserver giver mere plads til hurtig vækst og mindsker risikoen for strømsvigt, men kan koste mere. |
| [Tag hensyn til kraftværkernes slid](technical_docs/en/options.md#opt-poweraging) | **Fra** | Medregner de anslåede omkostninger ved kraftværkernes slid og aldring i optimeringen. Kan favorisere højere finansiering, når den langsigtede gevinst ved at bevare kraftværket opvejer de ekstra driftsomkostninger. |
| [Styr vand](technical_docs/en/options.md#opt-water) | **Til** | Justerer vandforsyningens finansiering, så byen forsynes uden unødvendige udgifter. |
| [Vandreserve](technical_docs/en/options.md#opt-waterreserve) | **10%** (10–50%) | Virker på samme måde som elreserven. |
| [Beskyt vandrør](technical_docs/en/options.md#opt-waterpipes) | **Til** | Holder finansieringen på eller over spillets 50% for at undgå skader på rørnettet. |
| [Styr affaldshåndtering](technical_docs/en/options.md#opt-sanitation) | **Til** | Justerer affaldsafdelingens finansiering, så behovet for indsamling og bortskaffelse dækkes. Reparerer også spøgelsesfelter (en fejl i spillet, der opkræver vedligeholdelse for lossepladsfelter uden zone). |
| [Reserve til affaldsbehandling](technical_docs/en/options.md#opt-sanitationreserve) | **2%** (2–20%) | Ekstra behandlingskapacitet til byens affald oven i det, den producerer efter genbrug, og eventuelle efterslæb. |
| [Advar mig, når lossepladsens størrelse kan spare penge](technical_docs/en/options.md#opt-landfilladvice) | **Fra** | Slår det valgfrie råd om ændringer i lossepladsens zoneinddeling til, hvis det sparer penge, og byens affald stadig bliver håndteret. Kræver styring af affaldshåndtering. |
| [Sparegrænse for lossepladsbeskeder](technical_docs/en/options.md#opt-landfillthreshold) | **25%** (20–50%) | Beder dig kun overveje en ændring af lossepladsens størrelse, hvis den modellerede månedlige besparelse udgør mindst denne andel af affaldsbudgettet. |
| [Styr aftaler med nabobyer](technical_docs/en/options.md#opt-deals) | **Til** | Slår automatiske regionale aftaler til; de tre enkeltkontakter nedenfor afgør, hvilke ressourcer der må handles. |
| [Køb fra én by, og sælg til en anden](technical_docs/en/options.md#opt-arbitrage) | **Fra** | Tillader rentable kombinationer af køb og salg af samme ressource mellem naboer, og byens egne behov kommer stadig først. |
| [Styr aftaler om el](technical_docs/en/options.md#opt-tradepower) | **Til** | Lader Ministeriet oprette, ændre eller opsige **import og eksport** af el. Uafhængig af finansieringen af kraftværkerne. |
| [Styr aftaler om vand](technical_docs/en/options.md#opt-tradewater) | **Til** | Lader Ministeriet oprette, ændre eller opsige **import og eksport** af vand. Uafhængig af vandfinansieringen. |
| [Styr aftaler om affald](technical_docs/en/options.md#opt-tradegarbage) | **Til** | Lader Ministeriet arrangere affaldsbehandling **i begge retninger**. Uafhængig af finansieringen af affaldshåndteringen. |

### Nedrivning

| Indstilling | Standard | Hvad det betyder |
|---|---|---|
| [Ryd ødelagte bygninger](technical_docs/en/options.md#opt-demolishdestroyed) | **Fra** | River ruiner ned efter en vis tid. Rydder ikke automatisk beskyttede kommunale eller historiske ruiner, medmindre indstillingen nedenfor tillader det. |
| [Ventetid (ødelagte bygninger)](technical_docs/en/options.md#opt-destroyedmonths) | **1 måned** (1–24) | Hvor længe ruiner skal blive stående, før de kan ryddes; som standard ca. **30 simulerede dage**. |
| [Ryd ødelagte kommunale og historiske bygninger](technical_docs/en/options.md#opt-demolishmunicipal) | **Fra** | Hvis slået til, ryddes også egnede **ruiner** af kommunale, særlige og historiske bygninger efter ventetiden. |
| [Riv forladte bygninger ned](technical_docs/en/options.md#opt-demolishabandoned) | **Fra** | Rydder egnede forladte bygninger efter ventetiden, i de valgte zonetyper og af de valgte årsager. |
| [Ventetid (forladte bygninger)](technical_docs/en/options.md#opt-abandonedmonths) | **12 måneder** (1–60) | Hvor længe en bygning skal stå forladt, før den må rives ned; som standard ét simuleret år. |
| [Boligzone](technical_docs/en/options.md#opt-zoneresidential) | **Til** | Omfatter forladte huse og etageejendomme. |
| [Handelszone](technical_docs/en/options.md#opt-zonecommercial) | **Til** | Omfatter forladte butikker og kontorer. |
| [Industrizone](technical_docs/en/options.md#opt-zoneindustrial) | **Til** | Omfatter forladte fabrikker og andre industribygninger. |
| [Landbrugszone](technical_docs/en/options.md#opt-zoneagriculture) | **Fra** | Omfatter forladte gårde. Slået fra som standard, fordi tomme eller forladte felter på en gård kan være normale. |
| [Årsag: uattraktivt område](technical_docs/en/options.md#opt-reasonlowdesirability) | **Til** | Omfatter forladthed på grund af dårlig attraktivitet, fx forurening eller mangel på service. |
| [Årsag: for lang vej til arbejde](technical_docs/en/options.md#opt-reasoncommute) | **Til** | Omfatter forladthed på grund af for lange rejser til arbejde. |
| [Årsag: ingen efterspørgsel](technical_docs/en/options.md#opt-reasonlowdemand) | **Til** | Omfatter bygninger, der er forladt, fordi efterspørgslen efter den type er forsvundet. |
| [Årsag: ingen strøm](technical_docs/en/options.md#opt-reasonnopower) | **Fra** | Omfatter valgfrit bygninger, der er forladt på grund af manglende el. Normalt er det bedre først at genoprette strømmen og se, om bygningen bliver beboet igen. |
| [Årsag: intet vand](technical_docs/en/options.md#opt-reasonnowater) | **Fra** | Omfatter valgfrit bygninger, der er forladt på grund af manglende vand. Normalt er det bedre først at genoprette vandet og se, om bygningen bliver beboet igen. |
| [Begræns det månedlige nedrivningsbudget](technical_docs/en/options.md#opt-budgetlimit) | **Fra** | Fastsætter et valgfrit udgiftsloft for automatisk nedrivning ud over de tilgængelige midler i byens kasse. |
| [Månedligt budget](technical_docs/en/options.md#opt-demolitionbudget) | **§1.000** (§0–§200.000) | Maksimale nedrivningsudgifter pr. simuleret måned, **når budgetgrænsen er slået til**. |

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

- **SimCity 4 Deluxe, Steam-udgaven, spilversion 1.1.641.0, på Windows.** Jeg har testet Ministeriet på denne version; andre versioner er ikke testet.

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

- **Anbefalet:** tilføj `-CPUCount:1` under **Steam → Bibliotek → SimCity 4 Deluxe → Egenskaber → Startindstillinger**. Det hjalp på stabiliteten i lange sessioner på min computer.

- **Ti sprog i spillet:** engelsk, portugisisk, fransk, tysk, spansk, italiensk, nederlandsk, dansk, svensk og polsk. Ministeriet følger automatisk dit spils sprog.

<a id="phantom-funding-slider-bug"></a>

## Kompatibilitet med Phantom Funding Slider Bug

Effektivitetsministeriet er kompatibelt med byer, der er ramt af **Phantom Funding Slider Bug**, et gammelt SimCity 4-problem, hvor finansieringsskyderne for allerede placerede bygninger bliver ved med at flytte sig af sig selv, efter at vedligeholdelsesomkostningen for den bygningstype er ændret (for eksempel af en mod).

Det retter **ikke** selve SimCity 4-fejlen. I stedet opdager det mistænkelige, tilbagevendende ændringer af finansieringen og justerer automatisk de anlæg, det styrer, så den krævede servicekapacitet bevares, og unødig overfinansiering undgås.

I de testede scenarier skete rettelserne inden for én simuleret dag i stedet for potentielt at tage en hel måned.

Denne beskyttelse gælder for egnede anlæg (skoler, biblioteker, museer, klinikker og hospitaler) under automatisk styring. Den reparerer ikke spillets interne budgetdata og garanterer ikke beskyttelse i alle situationer. Se [Phantom Funding Slider Bug](technical_docs/en/phantom_slider.md) for, hvordan det virker, dets grænser og testresultaterne (på engelsk).

<a id="updating-and-uninstalling"></a>

## Opdatering og afinstallation

Ministeriets tre filer installeres direkte i Plugins. Sikkerhedskopiér `DepartmentEfficiency.ini` før en opdatering for at bevare dine indstillinger.

**Opdatering fra version 1.0.0:** Luk spillet, slet hele undermappen `Department of Efficiency`, og erstat `DepartmentEfficiency.dll` med den nye fil. Udpak den nye ZIP-fil til Plugins. Indstillinger fra version 1.0.0 skal indstilles igen.

**Afinstallation:** Luk spillet, og fjern ministeriets tre filer fra Plugins. Dine gemte byer kan stadig bruges; allerede gemt finansiering, kontrakter og nedrivninger forbliver uændrede.

**Logfil:** `DepartmentEfficiency.log` ligger i mappen `Logs` i din SimCity 4-brugermappe (normalt `Documents\SimCity 4\Logs`, eller mappen `Logs` i den mappe, der er angivet med `-UserDir`). Kan den mappe ikke bruges, skrives den ved siden af DLL-filen.

<a id="known-limitations"></a>

## Kendte begrænsninger

- **Kompatibilitet:** jeg har testet Windows-Steam-versionen **1.1.641.0** af SimCity 4 Deluxe. Andre spilversioner er ikke testet.

- **Råd om lossepladsen:** kræver præcis én lossepladszone. I byer med flere losseplader kan Ministeriet stadig reparere egnede fejlramte „spøgelsesfelter“, men anbefaler ikke ændringer af størrelsen.

<a id="source-code"></a>

## Kildekode

Pluginets kildekode ligger i mappen [`source/`](source/). Den bygges med CMake og Visual Studio til en 32-bit (Win32) DLL. Du skal bruge Visual Studio 2026 med arbejdsbelastningen til desktopudvikling i C++ (andre versioner er ikke testet), CMake 3.20 eller nyere og Git:

```bat
cmake -S . -B build -G "Visual Studio 18 2026" -A Win32
cmake --build build --config Release
```

Under bygningen hentes [gzcom-dll](https://github.com/nsgomez/gzcom-dll) i en fast revision (`779b669bf88112146d58fba6c7ed44f13ac6ce7d`). Hvis du vil bruge en ændret kopi af biblioteket, eller bygge uden internetforbindelse, skal du tilføje `-DFETCHCONTENT_SOURCE_DIR_GZCOM_DLL=<mappe>` til den første kommando. DLL'en oprettes i `build\Release\DepartmentEfficiency.dll`. En genopbygget DLL er ikke byte for byte identisk med den udgivne.

<a id="license"></a>

## Licens

- Den originale kode i Department of Efficiency (filerne i [`source/`](source/)) er skrevet af mig, **Sammu89** og udgives under **GNU General Public License, version 3 alene** (GPL-3.0-only). Den fulde tekst findes i [LICENSE](LICENSE). Du må studere, ændre og videredistribuere den efter GPLv3: bevar ophavsretsmeddelelser og tak-til-omtaler, angiv dine ændringer, og udgiv din version under samme licens. Præsentér venligst ikke ændrede versioner som mine officielle versioner.

- Dokumentationen (disse README-filer og filerne i [`technical_docs/`](technical_docs/)) og de tekster, der er skrevet til dette projekt (herunder dem i INI-filen og dem, der er kompileret i `DepartmentEfficiency.dat`), sammen med deres originale oversættelser, udgives under samme licens, GPL-3.0-only.

- **gzcom-dll** er ikke omfattet af denne licens: biblioteket beholder sin egen licens, LGPL-2.1-or-later ([licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt)), og hentes fra sit oprindelige repository under bygningen.

- **SimCity 4** og dets originale ressourcer tilhører Electronic Arts / Maxis. **Effektivitetsministeriet** er en uofficiel fanmod, uafhængig af EA og ikke godkendt af EA. Ud over projektets egne tekster og scripts indeholder `DepartmentEfficiency.dat` to grænsefladescripts (indstillingsknappen og -vinduet), der er afledt af spillets egne ressourcer, og skærmbillederne viser spillet: det er ikke min ejendom, det er ikke omfattet af GPL, og alle rettigheder forbliver hos deres ejere. Det samme gælder alt andet tredjepartsmateriale. Spillets eksekverbare fil distribueres ikke.

<a id="credits"></a>

## Tak til

**Forfatter**

- Skabt af **Sammu89**.

**Særlig tak**

- **Særlig tak til CorinaMarie** for hendes detaljerede forklaringer af Phantom Funding Slider Bug, for generøst at have delt de reproducerbare testbyer og DAT-filer, der gjorde det muligt at undersøge og afbøde dens virkninger, og for hendes uvurderlige tekniske vejledning i at bevare og genoprette Maxis' oprindelige rådgiverbeskeder, herunder den selektive håndtering af Lua-rådposter. Hendes hjælp gjorde Ministeriet mere robust og mere kompatibelt med spillet og med andre mods.

Koden til Effektivitetsministeriet er skrevet af Sammu89. De personer, der nævnes her, delte viden, værktøjer, testmateriale og råd; de er ikke medforfattere til koden.

**Anvendt bibliotek**

- **gzcom-dll** af Nelson Gomez (simmaster07), med bidrag fra Nicholas Hayes (Null 45), Casper Van Gheluwe, memo og andre (se dets [repository](https://github.com/nsgomez/gzcom-dll)), brugt under licensen LGPL-2.1-or-later og statisk linket ind i DLL'en. Biblioteket følger ikke med: bygningen henter den præcise revision fra det oprindelige repository (se [Kildekode](#source-code)); copyrightmeddelelserne og licensteksten findes i [licenses/gzcom-dll.txt](licenses/gzcom-dll.txt) og [licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt). Du må ændre biblioteket og genopbygge pluginet med din version.

**Konsulterede teknikker, dokumentation og eksempler** (der er ikke kopieret kode fra dem; deres forfattere er ikke medforfattere til Department of Efficiency):

- Nicholas Hayes (0xC0000054 / Null 45), for forskning, dokumentation og åbne eksempler om SimCity 4-DLL'er: sc4-dll-utilities, sc4-custom-budget-departments, sc4-query-ui-hooks, sc4-lua-extensions, sc4-lua-extension-example. Indstillingsvinduet, forordningen og beskedfilen følger mekanismer offentliggjort i hans projekter sc4-region-census, sc4-humane-society-ordinance og sc4-lua-extension-example (MIT-licens); spilkommandoerne følger sc4-auto-run-cheats og sc4-game-command-documentation.

- CorinaMarie, for at dokumentere SimCity 4's Urgent Advice-teknik, som modens rådgiverbeskeder følger.

- memo, for vejledningen i at indlæse et eget Lua-script i pluginet, med egne identifikatorer, efter spillets scripts og kun ændre de nødvendige rådposter, som Ministeriets integration med spillets rådgivere følger.

- Kel9509, for oplysningerne om, hvordan Phantom Funding Slider Bug kan reproduceres.

- SimCity 4-modding-miljøet, for to årtiers forskning, værktøjer og delt viden, herunder SC4Devotions dokumentation af exemplar-egenskaber.

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
