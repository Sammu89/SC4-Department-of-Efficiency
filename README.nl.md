[English](README.md) | [Português](README.pt.md) | [Français](README.fr.md) | [Deutsch](README.de.md) | [Español](README.es.md) | [Italiano](README.it.md) | **Nederlands** | [Dansk](README.da.md) | [Svenska](README.sv.md) | [Polski](README.pl.md)

# Ministerie van Efficiëntie

**Het Ministerie van Efficiëntie zorgt voor de kleine details van het stadsbeheer die een groot verschil maken.**

Het Ministerie van Efficiëntie (*Department of Efficiency* in het Engels) is een plug-in voor **SimCity 4 Deluxe** die routinetaken van het stadsbeheer automatiseert, zodat u zich kunt richten op bouwen, plannen en uw stad laten groeien.

**U bouwt de stad. Het Ministerie optimaliseert het budget!**

Het beheert de budgetten van afzonderlijke onderwijs- en zorggebouwen, elektriciteitscentrales, de financiering van water en afvalverwerking, contracten met buursteden en, optioneel, het slopen van ruïnes en verlaten gebouwen. Het streeft ernaar de behoeften van de stad te dekken zonder meer uit te geven dan nodig is; de plaatsing van gebouwen, de dekking en de grote planningsbeslissingen blijven aan u.

Alles wordt in het spel geregeld via **Spelopties → Ministerie van Efficiëntie**. Kies de diensten die u wilt automatiseren, pas hun veiligheidsmarges aan, stel het slopen in of schakel het hele Ministerie uit. U kunt ook de verordening **Ministerie van Efficiëntie oprichten** gebruiken als hoofdschakelaar. Een aparte toepassing is niet nodig.

> [!IMPORTANT]
> **Het Ministerie start ingeschakeld, inclusief het automatisch slopen van verwoeste en verlaten gebouwen.** Controleer de instellingen voordat u een stad opent die u dierbaar is. Het voert echte wijzigingen door in het spel: financiering, contracten en sloop worden opgeslagen als u de stad opslaat. Opgeslagen bestanden worden niet rechtstreeks bewerkt.

## Inhoud

- [Downloaden en installeren](#download-and-installation)

- [Wat het Ministerie doet](#features)

- [Eerste installatie-instellingen](#first-time-setup)

- [Instellingen](#settings-explained)

- [Schermafbeeldingen](#screenshots)

- [Vereisten en compatibiliteit](#requirements-and-compatibility)

- [Compatibiliteit met de Phantom Funding Slider Bug](#phantom-funding-slider-bug)

- [Bijwerken en verwijderen](#updating-and-uninstalling)

- [Bekende beperkingen](#known-limitations)

- [Broncode](#source-code)

- [Licentie](#license)

- [Dankbetuigingen](#credits)

<a id="download-and-installation"></a>

## Downloaden en installeren

### [⬇ Download Department of Efficiency 1.0.2](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases/download/v1.0.2/Departement_of_Efficiency_1.02.zip)

**Nieuw in 1.0.2:** zie het [wijzigingslogboek](CHANGELOG.md) (in het Engels).

1\. Open de [releasepagina](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases) van het project en kies de nieuwste release.

2\. Download onder **Assets** de **distributie-ZIP van Department of Efficiency**.

3\. **Sluit SimCity 4.** Pak de ZIP uit in een tijdelijke map.

4\. Kopieer de **drie bestanden** **`DepartmentEfficiency.dll`**, **`DepartmentEfficiency.dat`** en **`DepartmentEfficiency.ini`** rechtstreeks naar `Documents\SimCity 4\Plugins`. Maak geen submap aan.

5\. Start het spel en volg de [eerste installatie-instellingen](#first-time-setup) voordat u een opgeslagen stad speelt.

De distributie-ZIP bevat precies deze drie bestanden. Na de installatie hebt u:

```text
Documents\SimCity 4\Plugins\
    DepartmentEfficiency.dll
    DepartmentEfficiency.dat
    DepartmentEfficiency.ini
```

De Markdown-bestanden en schermafbeeldingen van de openbare repository zijn **niet** nodig voor het spel. Houd de drie bestanden **rechtstreeks in Plugins**, naast elkaar. De map `Department of Efficiency` bestaat niet meer.

> [!WARNING]
> **Werkt u bij vanaf een eerdere versie?** Kopieer de nieuwe bestanden niet over de oude heen. Verwijder eerst de oude bestanden van het Ministerie, zoals uitgelegd bij [Bijwerken en verwijderen](#updating-and-uninstalling).

**Instellingen later wijzigen:** open **Spelopties → Ministerie van Efficiëntie**, gebruik de pagina's **Diensten** en **Sloop** en klik op **Accepteren**. Voor de hoofdschakelaar kunt u ook de verordening **Ministerie van Efficiëntie oprichten** gebruiken, zonder maandelijkse kosten. Het Ministerie uitschakelen stopt toekomstige acties; reeds toegepaste financiering, contracten en sloop worden niet ongedaan gemaakt.

<a id="features"></a>

## Wat het Ministerie doet

De tabel hieronder beschrijft wat het Ministerie doet. Voor formules, voorbeelden, waarborgen en de mechanica achter de beslissingen, zie de **[uitgebreide functiereferentie (in het Engels)](technical_docs/en/index.md)**.

| Gebied | Wat het doet |
|---|---|
| [Onderwijs en zorg](technical_docs/en/education_healthcare.md#education-healthcare) | Past de financiering **gebouw voor gebouw** aan voor geschikte scholen, hogescholen, universiteiten, bibliotheken, musea, klinieken en ziekenhuizen. Het volgt het werkelijke gebruik en de capaciteit, met een veiligheidsmarge. Het **wijzigt geen dekkingsgebieden** en ook niet de financiering van schoolbussen en ambulances. Is een gebouw bij maximale financiering nog steeds overvol, dan meldt een adviseur dat er een extra voorziening nodig is; **u kiest waar die komt**. |
| [Elektriciteit](technical_docs/en/power.md#power) | Beheert **elke elektriciteitscentrale afzonderlijk** en kiest de financiering die de behoefte van de stad en de veiligheidsreserve dekt tegen de laagste gemodelleerde kosten. Het houdt rekening met rendement en staat van de centrales; een optionele instelling neemt ook de economische kosten van veroudering mee. |
| [Water](technical_docs/en/water.md#water) | Past de waterfinanciering van de stad aan op de vraag plus een veiligheidsreserve. Bescherming van de waterleidingen staat standaard aan, om bezuinigingen te voorkomen waardoor het netwerk zou verslechteren. Het verbergt ook het misleidende „geen water”-pictogram van het spel boven boerderijen. |
| [Afvalverwerking](technical_docs/en/garbage.md#garbage-landfills) | Past de financiering van de afvalverwerking aan op basis van afvalproductie, recycling, verwerking, opslag in stortplaatsen en regionale contracten, en beschermt daarbij de eigen verwerkingsbehoefte van de stad. |
| [Contracten met buursteden](technical_docs/en/deals.md#neighbor-deals) | Kan **elektriciteit en water kopen of verkopen**, **buren betalen om uw afval over te nemen** of **hun afval tegen betaling accepteren**. Het sluit, past aan en zegt geschikte contracten op als het totaalresultaat de stad ten goede komt en de eigen behoeften en reserves beschermd blijven. |
| [Automatisch slopen](technical_docs/en/demolition.md#demolition) | Kan standaard ruïnes na ongeveer **30 gesimuleerde dagen** opruimen en geschikte gebouwen die **een gesimuleerd jaar** verlaten zijn. U bepaalt welke zones en welke redenen van verlating meetellen, of beschermde ruïnes mogen worden opgeruimd en of u een maandelijkse bestedingslimiet instelt. |
| [Meldingen van adviseurs](technical_docs/en/safety.md#notices) | Gebruikt de eigen adviseurs van het spel om bewezen, aanhoudende tekorten te melden die financiering of beschikbare contracten niet kunnen verhelpen. Waar nuttig bevatten de berichten links naar de betrokken gebouwen. Het voegt ook een nieuwsbericht toe (titel in de lijst, details bij een klik) wanneer een dienst die u hebt ingeschakeld moet stoppen vanwege een technische fout. |
| [Advies over de omvang van de stortplaats](technical_docs/en/garbage.md#garbage-landfills-advice) | Het optionele advies wijst een concrete, veilige verkleining van de stortplaats aan (lege vakken die weg kunnen) die de afvalrekening met meer dan uw gekozen besparingsdrempel zou kunnen verlagen. **Het Ministerie adviseert; u beslist of u de zonering wijzigt.** |
| [Herstel van spookvakjes](technical_docs/en/garbage.md#ghost-tiles) | Als het beheer van de afvalverwerking aanstaat, herstelt het veilig geschikte onzichtbare stortplaatsvakjes die het spel nog in het onderhoud meetelt. Onveilige of niet-geverifieerde gevallen blijven met rust; voor dit herstel bestaat geen aparte schakelaar. |

U houdt de controle over bouw, zonering, belastingen, wegen en vervoer, politie, brandweer en elke andere dienst die het Ministerie niet beheert. Schakelt u het beheer van een soort contract met buren in, dan kan het Ministerie ook een contract wijzigen of opzeggen dat u eerder handmatig hebt gesloten. Schakel die contractoptie uit om die contracten zelf te beheren.

<a id="first-time-setup"></a>

## Eerste installatie-instellingen

Het Ministerie gebruikt de instellingen die bij de plug-in worden geleverd en **gaat automatisch aan het werk**. U hoeft het niet in te schakelen, maar u moet nagaan wat het mag doen voordat u een belangrijke opgeslagen stad gebruikt.

1\. Start SimCity 4 en open vanuit de **regioweergave** **Spelopties → Ministerie van Efficiëntie**. Wilt u liever, gebruik dan voor een eerste kennismaking een nieuwe of wegwerpstad.

2\. Kies op de pagina **Diensten** welke budgetten en contracten met buursteden het Ministerie mag beheren en hoeveel reservecapaciteit het moet aanhouden.

3\. Controleer op de pagina **Sloop** de regels voor het opruimen van ruïnes en verlaten gebouwen. **Schakel het automatisch slopen uit als u alles zelf wilt platgooien.**

4\. Klik op **Accepteren** om uw keuzes op te slaan. **Annuleren** sluit het venster zonder wijzigingen op te slaan.

5\. Open de stad die u wilt spelen. Het Ministerie volgt uw opgeslagen voorkeuren en beheert de stad terwijl de simulatie loopt.

Uw voorkeuren staan in `DepartmentEfficiency.ini` en worden hergebruikt als u een andere stad opent. Het Ministerie bouwt **geen** scholen, ziekenhuizen, elektriciteitscentrales of waterwerken voor u. Als financiering en beschikbare contracten een bewezen, aanhoudend tekort niet kunnen oplossen, meldt een adviseur in het spel waar meer capaciteit nodig is.

U kunt het Ministerie ook in- of uitschakelen via de verordeningen van de stad.

<a id="settings-explained"></a>

## Instellingen

Open **Spelopties → Ministerie van Efficiëntie**. Het venster heeft twee pagina's, **Diensten** en **Sloop**. De tabellen hieronder behandelen **elke instelling die in het menu staat**, zonder de technische formules. Klik op de naam van een optie om de volledige uitleg in de [functiereferentie](technical_docs/en/options.md#options) te lezen.

**Hoe de bediening werkt:** „Aan” betekent dat het Ministerie die functie mag beheren. Een instelling die van een andere afhangt, is grijs zolang de bovenliggende uitstaat. Instellingen in procenten bepalen veiligheidsmarges en drempels en helpen het Ministerie reservecapaciteit aan te houden en schommelingen in de vraag op te vangen.

### Diensten

| Optie | Standaard | Wat het betekent |
|---|---|---|
| [Ministerie actief](technical_docs/en/options.md#opt-master) | **Aan** | Hoofdschakelaar voor financiering, contracten met buursteden, sloop en meldingen van het Ministerie. Uitschakelen maakt reeds doorgevoerde wijzigingen niet ongedaan. |
| [Onderwijs beheren](technical_docs/en/options.md#opt-education) | **Aan** | Past geschikte scholen, universiteiten, bibliotheken en musea automatisch afzonderlijk aan. Wijzigt noch hun dekking noch de financiering van schoolbussen. |
| [Veiligheidsmarge (onderwijs)](technical_docs/en/options.md#opt-educationreserve) | **5%** (0–50%) | Extra capaciteit boven het huidige gebruik van het onderwijs. Meer capaciteit kost meer. Bij gebouwen met weinig bezoekers kan het Ministerie een extra marge aanhouden. Zo kan een school bijvoorbeeld van 10 naar 20 leerlingen (+100%) groeien zonder staking, vooral in het begin, wanneer de extra kosten klein zijn. |
| [Gezondheidszorg beheren](technical_docs/en/options.md#opt-health) | **Aan** | Past geschikte klinieken, ziekenhuizen en andere ondersteunde zorggebouwen automatisch afzonderlijk aan. Wijzigt noch de financiering van ambulances noch de dekking. |
| [Veiligheidsmarge (zorg)](technical_docs/en/options.md#opt-healthreserve) | **5%** (0–50%) | Werkt op dezelfde manier als de veiligheidsmarge van het onderwijs. |
| [Stroom beheren](technical_docs/en/options.md#opt-power) | **Aan** | Past de financiering van afzonderlijke centrales aan om tegen de laagste gemodelleerde kosten voldoende elektriciteit te leveren. |
| [Stroomreserve](technical_docs/en/options.md#opt-powerreserve) | **20%** (10–50%) | Reserve-opwekkingscapaciteit voor het deel van de vraag dat lokaal wordt geleverd; hogere reserves geven meer ruimte voor snelle groei en verkleinen de kans op stroomuitval, maar kunnen meer kosten. |
| [Slijtage van elektriciteitscentrales meewegen](technical_docs/en/options.md#opt-poweraging) | **Uit** | Neemt de geschatte kosten van slijtage en veroudering van centrales mee bij het optimaliseren. Kan hogere financiering bevoordelen als het langetermijnvoordeel van het behoud van de centrale zwaarder weegt dan de extra bedrijfskosten. |
| [Water beheren](technical_docs/en/options.md#opt-water) | **Aan** | Past de financiering van de waterdienst aan om de stad zonder onnodige uitgaven te bevoorraden. |
| [Waterreserve](technical_docs/en/options.md#opt-waterreserve) | **10%** (10–50%) | Werkt op dezelfde manier als de stroomreserve. |
| [Waterleidingen beschermen](technical_docs/en/options.md#opt-waterpipes) | **Aan** | Houdt de financiering op of boven de 50% van het spel, om schade aan het leidingnet te voorkomen. |
| [Afvalverwerking beheren](technical_docs/en/options.md#opt-sanitation) | **Aan** | Past de financiering van de afvaldienst aan om de behoefte aan ophaling en verwerking te dekken. Herstelt ook spookvakjes (een fout in het spel die onderhoud rekent voor stortplaatsvakjes zonder zone). |
| [Reserve afvalverwerking](technical_docs/en/options.md#opt-sanitationreserve) | **2%** (2–20%) | Extra verwerkingscapaciteit voor het afval van de stad, bovenop wat die na recycling produceert en een eventuele achterstand. |
| [Waarschuw me als de stortplaats aanpassen geld bespaart](technical_docs/en/options.md#opt-landfilladvice) | **Uit** | Schakelt het optionele advies in over wijzigingen in de zonering van de stortplaats, als dat geld bespaart en het afval van de stad toch wordt verwerkt. Vereist beheer van de afvalverwerking. |
| [Besparingsdrempel voor de stortplaats](technical_docs/en/options.md#opt-landfillthreshold) | **25%** (20–50%) | Vraagt u alleen om een andere omvang van de stortplaats te overwegen als de gemodelleerde maandelijkse besparing minstens dit deel van het budget voor afvalverwerking bedraagt. |
| [Contracten met buursteden beheren](technical_docs/en/options.md#opt-deals) | **Aan** | Schakelt automatische regionale contracten in; de drie afzonderlijke schakelaars hieronder bepalen welke grondstoffen verhandeld mogen worden. |
| [Bij de ene stad inkopen, aan de andere verkopen](technical_docs/en/options.md#opt-arbitrage) | **Uit** | Staat winstgevende combinaties van inkoop en verkoop van dezelfde grondstof tussen buren toe, waarbij de behoeften van de stad voorgaan. |
| [Stroomcontracten beheren](technical_docs/en/options.md#opt-tradepower) | **Aan** | Staat het Ministerie toe **import en export** van elektriciteit te sluiten, te wijzigen of op te zeggen. Los van de financiering van de centrales. |
| [Watercontracten beheren](technical_docs/en/options.md#opt-tradewater) | **Aan** | Staat het Ministerie toe **import en export** van water te sluiten, te wijzigen of op te zeggen. Los van de waterfinanciering. |
| [Afvalcontracten beheren](technical_docs/en/options.md#opt-tradegarbage) | **Aan** | Staat het Ministerie toe afvalverwerking **in beide richtingen** te regelen. Los van de financiering van de afvalverwerking. |

### Sloop

| Optie | Standaard | Wat het betekent |
|---|---|---|
| [Verwoeste gebouwen slopen](technical_docs/en/options.md#opt-demolishdestroyed) | **Aan** | Sloopt ruïnes na een bepaalde tijd. Ruimt beschermde gemeentelijke of historische ruïnes niet automatisch op, tenzij de optie hieronder dat toestaat. |
| [Wachttijd (verwoeste gebouwen)](technical_docs/en/options.md#opt-destroyedmonths) | **1 maand** (1–24) | Hoe lang ruïnes moeten blijven staan voordat ze kunnen worden opgeruimd; standaard ongeveer **30 gesimuleerde dagen**. |
| [Verwoeste gemeentelijke en historische gebouwen slopen](technical_docs/en/options.md#opt-demolishmunicipal) | **Uit** | Indien ingeschakeld worden na de wachttijd ook geschikte **ruïnes** van gemeentelijke, speciale en historische gebouwen opgeruimd. |
| [Verlaten gebouwen slopen](technical_docs/en/options.md#opt-demolishabandoned) | **Aan** | Ruimt geschikte verlaten gebouwen op na de wachttijd, in de gekozen zonetypen en om de gekozen redenen. |
| [Wachttijd (verlaten gebouwen)](technical_docs/en/options.md#opt-abandonedmonths) | **12 maanden** (1–60) | Hoe lang een gebouw verlaten moet zijn voordat het gesloopt mag worden; standaard een gesimuleerd jaar. |
| [Woonzone](technical_docs/en/options.md#opt-zoneresidential) | **Aan** | Omvat verlaten huizen en appartementsgebouwen. |
| [Commerciële zone](technical_docs/en/options.md#opt-zonecommercial) | **Aan** | Omvat verlaten winkels en kantoren. |
| [Industriezone](technical_docs/en/options.md#opt-zoneindustrial) | **Aan** | Omvat verlaten fabrieken en andere industriële gebouwen. |
| [Landbouwzone](technical_docs/en/options.md#opt-zoneagriculture) | **Uit** | Omvat verlaten boerderijen. Standaard uit omdat lege of verlaten vakjes op een boerderij normaal kunnen zijn. |
| [Reden: onaantrekkelijke omgeving](technical_docs/en/options.md#opt-reasonlowdesirability) | **Aan** | Omvat verlating door lage aantrekkelijkheid, zoals vervuiling of gebrek aan voorzieningen. |
| [Reden: te lange reistijd](technical_docs/en/options.md#opt-reasoncommute) | **Aan** | Omvat verlating door te lange reistijden naar het werk. |
| [Reden: geen vraag](technical_docs/en/options.md#opt-reasonlowdemand) | **Aan** | Omvat gebouwen die verlaten zijn omdat de vraag naar dat type is verdwenen. |
| [Reden: geen stroom](technical_docs/en/options.md#opt-reasonnopower) | **Uit** | Omvat optioneel gebouwen die verlaten zijn door een tekort aan elektriciteit. Meestal is het beter eerst de stroom te herstellen en te kijken of het gebouw weer bewoond raakt. |
| [Reden: geen water](technical_docs/en/options.md#opt-reasonnowater) | **Uit** | Omvat optioneel gebouwen die verlaten zijn door een tekort aan water. Meestal is het beter eerst het water te herstellen en te kijken of het gebouw weer bewoond raakt. |
| [Maandelijks sloopbudget begrenzen](technical_docs/en/options.md#opt-budgetlimit) | **Uit** | Stelt een optioneel bestedingsplafond in voor automatisch slopen, naast de beschikbare middelen in de stadskas. |
| [Maandbudget](technical_docs/en/options.md#opt-demolitionbudget) | **§1.000** (§0–§200.000) | Maximale sloopuitgaven per gesimuleerde maand **als de budgetgrens is ingeschakeld**. |

<a id="screenshots"></a>

## Schermafbeeldingen

Alle schermafbeeldingen tonen de Engelse versie van het spel.

| | |
|---|---|
| ![De pagina Diensten van de instellingen van het Ministerie](screenshots/Options-1.png) | ![De pagina Sloop van de instellingen van het Ministerie](screenshots/Options-2.png) |
| *De pagina Diensten: elke dienst, de reserve ervan en de contracten met buursteden.* | *De pagina Sloop: ruïnes, verlaten gebouwen en het maandbudget.* |
| ![Een melding van een adviseur over overvolle middelbare scholen](screenshots/high_school.png) | ![Een melding van een adviseur over overvolle musea](screenshots/museum.png) |
| *Scholen op maximale financiering die toch overvol zijn: de melding verwijst naar de gebouwen.* | *Hetzelfde voor musea.* |
| ![Een melding van een adviseur over een watertekort](screenshots/water.png) | ![Een melding van een adviseur over een tekort aan afvalcapaciteit](screenshots/real-garbage-paused.png) |
| *Water: het Ministerie kan het tekort niet oplossen met het beschikbare aanbod.* | *Afval: de stad heeft meer verwerkingscapaciteit nodig.* |

<a id="requirements-and-compatibility"></a>

## Vereisten en compatibiliteit

- **SimCity 4 Deluxe, Steam-versie, spelversie 1.1.641.0, op Windows.** Ik heb het Ministerie met deze versie getest; andere versies zijn niet getest.

- **Geen andere mod, installatieprogramma of externe toepassing is nodig.**

- **Getest en compatibel met deze mods** (geteste versies):

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

- **Aanbevolen:** voeg `-CPUCount:1` toe onder **Steam → Bibliotheek → SimCity 4 Deluxe → Eigenschappen → Startopties**. Dit hielp de stabiliteit in lange sessies op mijn computer.

- **Tien talen in het spel:** Engels, Portugees, Frans, Duits, Spaans, Italiaans, Nederlands, Deens, Zweeds en Pools. Het Ministerie volgt automatisch de taal van uw spel.

<a id="phantom-funding-slider-bug"></a>

## Compatibiliteit met de Phantom Funding Slider Bug

Het Ministerie van Efficiëntie is compatibel met steden die door de **Phantom Funding Slider Bug** worden getroffen, een oud SimCity 4-probleem waarbij de financieringsschuifregelaars van al geplaatste gebouwen uit zichzelf blijven bewegen nadat de onderhoudskosten van dat gebouwtype zijn gewijzigd (bijvoorbeeld door een mod).

Het verhelpt de onderliggende SimCity 4-bug **niet**. In plaats daarvan detecteert het verdachte, terugkerende wijzigingen in de financiering en past het de voorzieningen die het beheert automatisch opnieuw aan, zodat de vereiste dienstcapaciteit behouden blijft en onnodige overfinanciering wordt vermeden.

In de geteste scenario's vonden correcties plaats binnen één gesimuleerde dag, in plaats van mogelijk een hele maand te duren.

Deze bescherming geldt voor geschikte voorzieningen (scholen, bibliotheken, musea, klinieken en ziekenhuizen) onder automatisch beheer. Ze herstelt de interne begrotingsgegevens van het spel niet en garandeert geen bescherming in elke situatie. Zie [Phantom Funding Slider Bug](technical_docs/en/phantom_slider.md) voor de werking, de grenzen en de testresultaten (in het Engels).

<a id="updating-and-uninstalling"></a>

## Bijwerken en verwijderen

**Instellingen worden niet overgenomen.** Sinds versie 1.0.2 heeft het Ministerie één instellingenbestand, `DepartmentEfficiency.ini`, dat de drie INI-bestanden van versie 1.0.0 en de map `Department of Efficiency` van het eerste 1.0.2-pakket vervangt. Instellingen in oudere bestanden worden niet gelezen of overgenomen: stel uw voorkeuren opnieuw in bij **Spelopties → Ministerie van Efficiëntie**.

**Een eerdere installatie bijwerken (een schone installatie is verplicht).** Sluit het spel en verwijder de oude bestanden van het Ministerie **voordat** u de drie nieuwe kopieert. Verwijder alleen deze, in `Documents\SimCity 4\Plugins`:

- `DepartmentEfficiency.dll`, `DepartmentEfficiency.dat` en `DepartmentEfficiency.ini`;
- de hele map `Department of Efficiency`, als die bestaat;
- uit versie 1.0.0, als ze er nog staan: `DepartmentEfficiencyUI.dat`, `DepartmentEfficiencyCivicAdvice.dat`, `DepartmentEfficiencyDealAdvice.dat`, `DepartmentEfficiencyGarbageAdvice.dat`, `DepartmentEfficiencyUtility.ini` en `DepartmentEfficiencyDemolition.ini`;
- `DepartmentEfficiency.told.txt`, een klein geheugenbestand dat de plug-in kan hebben achtergelaten (het wordt zo nodig opnieuw aangemaakt).

Raak de bestanden van andere mods niet aan. De nieuwe bestanden over een oude installatie kopiëren wordt niet ondersteund: de oude resourcebestanden zouden blijven staan en de eigen adviezen van het spel stil kunnen houden. De plug-in leest nooit bestanden uit de oude map; als die map er nog is, schrijft hij alleen een waarschuwing in zijn logboek.

**Verwijderen:** sluit het spel en verwijder dezelfde bestanden. Uw opgeslagen steden blijven zonder de mod leesbaar. Financieringsniveaus, bestaande contracten en al opgeslagen sloopacties in een stad worden niet automatisch teruggedraaid.

**Logbestand:** `DepartmentEfficiency.log` staat in de map `Logs` van uw SimCity 4-gebruikersmap (normaal `Documents\SimCity 4\Logs`, of de map `Logs` van de map die met `-UserDir` is opgegeven). Kan die map niet worden gebruikt, dan wordt het naast de DLL geschreven.

<a id="known-limitations"></a>

## Bekende beperkingen

- **Compatibiliteit:** ik heb de Windows-Steamversie **1.1.641.0** van SimCity 4 Deluxe getest. Andere spelversies zijn niet getest.

- **Advies over de stortplaats:** vereist precies één stortplaatszone. In steden met meerdere stortplaatsen kan het Ministerie nog wel geschikte foutieve „spookvakjes” herstellen, maar het doet geen aanbevelingen over de omvang.

<a id="source-code"></a>

## Broncode

De broncode van de plug-in staat in de map [`source/`](source/). Hij wordt met CMake en Visual Studio gebouwd tot een 32-bits (Win32) DLL. Je hebt Visual Studio 2026 met de workload voor desktopontwikkeling met C++ nodig (andere versies zijn niet getest), CMake 3.20 of nieuwer en Git:

```bat
cmake -S . -B build -G "Visual Studio 18 2026" -A Win32
cmake --build build --config Release
```

Tijdens het bouwen wordt [gzcom-dll](https://github.com/nsgomez/gzcom-dll) in een vaste revisie (`779b669bf88112146d58fba6c7ed44f13ac6ce7d`) gedownload. Om een aangepaste kopie van die bibliotheek te gebruiken, of om zonder internetverbinding te bouwen, voeg je `-DFETCHCONTENT_SOURCE_DIR_GZCOM_DLL=<map>` toe aan het eerste commando. De DLL wordt aangemaakt in `build\Release\DepartmentEfficiency.dll`. Een opnieuw gebouwde DLL is niet byte voor byte identiek aan de gepubliceerde.

<a id="license"></a>

## Licentie

- De originele code van Department of Efficiency (de bestanden in [`source/`](source/)) is geschreven door mij, **Sammu89** en wordt uitgebracht onder de **GNU General Public License, versie 3 uitsluitend** (GPL-3.0-only). De volledige tekst staat in [LICENSE](LICENSE). Je mag de code bestuderen, aanpassen en herdistribueren onder de voorwaarden van de GPLv3: behoud de copyrightvermeldingen en dankbetuigingen, geef je wijzigingen aan en publiceer je versie onder dezelfde licentie. Presenteer aangepaste versies alsjeblieft niet als mijn officiële versies.

- De documentatie (deze README's en de bestanden in [`technical_docs/`](technical_docs/)) en de voor dit project geschreven teksten (inclusief die in het INI-bestand en die welke in `DepartmentEfficiency.dat` zijn gecompileerd), samen met hun oorspronkelijke vertalingen, worden onder dezelfde licentie uitgebracht, GPL-3.0-only.

- **gzcom-dll** valt niet onder deze licentie: de bibliotheek behoudt haar eigen licentie, LGPL-2.1-or-later ([licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt)), en wordt bij het bouwen uit de oorspronkelijke repository gedownload.

- **SimCity 4** en de originele bronnen ervan zijn eigendom van Electronic Arts / Maxis. Het **Ministerie van Efficiëntie** is een onofficiële fanmod, onafhankelijk van EA en niet door EA onderschreven. Naast de eigen teksten en scripts van het project bevat `DepartmentEfficiency.dat` twee interfacescripts (de optieknop en het optievenster) die zijn afgeleid van de eigen bronnen van het spel, en de schermafbeeldingen tonen het spel: dat is niet van mij, valt niet onder de GPL en alle rechten blijven bij de rechthebbenden. Hetzelfde geldt voor ander materiaal van derden. Het uitvoerbare bestand van het spel wordt niet verspreid.

<a id="credits"></a>

## Dankbetuigingen

**Auteur**

- Gemaakt door **Sammu89**.

**Speciale dank**

- **Speciale dank aan CorinaMarie** voor haar uitgebreide uitleg over de Phantom Funding Slider Bug, voor het royaal delen van de reproduceerbare teststeden en DAT-bestanden waarmee de gevolgen konden worden onderzocht en verzacht, en voor haar onmisbare technische begeleiding bij het behouden en herstellen van de oorspronkelijke adviseursberichten van Maxis, inclusief de selectieve behandeling van Lua-adviesrecords. Haar hulp maakte het Ministerie robuuster en beter compatibel met het spel en met andere mods.

De code van het Ministerie van Efficiëntie is geschreven door Sammu89. De hier genoemde mensen deelden kennis, hulpmiddelen, testmateriaal en advies; zij zijn geen coauteurs van de code.

**Gebruikte bibliotheek**

- **gzcom-dll** van Nelson Gomez (simmaster07), met bijdragen van Nicholas Hayes (Null 45), Casper Van Gheluwe, memo en anderen (zie de [repository](https://github.com/nsgomez/gzcom-dll)), gebruikt onder de licentie LGPL-2.1-or-later en statisch in de DLL gelinkt. De bibliotheek is niet meegeleverd: bij het bouwen wordt de exacte revisie uit de oorspronkelijke repository gedownload (zie [Broncode](#source-code)); de copyrightvermeldingen en de licentietekst staan in [licenses/gzcom-dll.txt](licenses/gzcom-dll.txt) en [licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt). Je mag de bibliotheek aanpassen en de plug-in met jouw versie opnieuw bouwen.

**Geraadpleegde technieken, documentatie en voorbeelden** (er is geen code uit gekopieerd; hun auteurs zijn geen coauteurs van Department of Efficiency):

- Nicholas Hayes (0xC0000054 / Null 45), voor onderzoek, documentatie en open voorbeelden over SimCity 4-DLL's: sc4-dll-utilities, sc4-custom-budget-departments, sc4-query-ui-hooks, sc4-lua-extensions, sc4-lua-extension-example. Het optievenster, de verordening en het meldingsbestand volgen mechanismen uit zijn projecten sc4-region-census, sc4-humane-society-ordinance en sc4-lua-extension-example (MIT-licentie); de spelcommando's volgen sc4-auto-run-cheats en sc4-game-command-documentation.

- CorinaMarie, voor het documenteren van de Urgent Advice-techniek van SimCity 4 waarop de adviseurmeldingen van de mod zijn gebaseerd.

- memo, voor de begeleiding bij het laden van een eigen Lua-script van de plug-in, met eigen identificaties, na de scripts van het spel en met alleen de nodige adviesrecords gewijzigd, wat de koppeling van het Ministerie met de adviseurs van het spel volgt.

- Kel9509, voor de informatie over hoe de Phantom Funding Slider Bug te reproduceren is.

- De SimCity 4-moddinggemeenschap, voor twee decennia onderzoek, hulpmiddelen en gedeelde kennis, waaronder de documentatie van exemplar-eigenschappen van SC4Devotion.

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
