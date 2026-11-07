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

- [Wat het Ministerie doet](#features)

- [Eerste installatie-instellingen](#first-time-setup)

- [Instellingen](#settings-explained)

- [Schermafbeeldingen](#screenshots)

- [Vereisten en compatibiliteit](#requirements-and-compatibility)

- [Downloaden en installeren](#download-and-installation)

- [Bijwerken en verwijderen](#updating-and-uninstalling)

- [Bekende beperkingen](#known-limitations)

- [Dankbetuigingen](#credits)

<a id="features"></a>

## Wat het Ministerie doet

De tabel hieronder beschrijft wat het Ministerie doet. Voor formules, voorbeelden, waarborgen en de mechanica achter de beslissingen, zie de **[uitgebreide functiereferentie (in het Engels)](features/en.md)**.

| Gebied | Wat het doet |
|---|---|
| [Onderwijs en zorg](features/en.md#education-healthcare) | Past de financiering **gebouw voor gebouw** aan voor geschikte scholen, hogescholen, universiteiten, bibliotheken, musea, klinieken en ziekenhuizen. Het volgt het werkelijke gebruik en de capaciteit, met een veiligheidsmarge. Het **wijzigt geen dekkingsgebieden** en ook niet de financiering van schoolbussen en ambulances. Is een gebouw bij maximale financiering nog steeds overvol, dan meldt een adviseur dat er een extra voorziening nodig is; **u kiest waar die komt**. |
| [Elektriciteit](features/en.md#electricity) | Beheert **elke elektriciteitscentrale afzonderlijk** en kiest de financiering die de behoefte van de stad en de veiligheidsreserve dekt tegen de laagste gemodelleerde kosten. Het houdt rekening met rendement en staat van de centrales; een optionele instelling neemt ook de economische kosten van veroudering mee. |
| [Water](features/en.md#water) | Past de waterfinanciering van de stad aan op de vraag plus een veiligheidsreserve. Bescherming van de waterleidingen staat standaard aan, om bezuinigingen te voorkomen waardoor het netwerk zou verslechteren. Het verbergt ook het misleidende „geen water”-pictogram van het spel boven boerderijen. |
| [Afvalverwerking](features/en.md#garbage-landfills) | Past de financiering van de afvalverwerking aan op basis van afvalproductie, recycling, verwerking, opslag in stortplaatsen en regionale contracten, en beschermt daarbij de eigen verwerkingsbehoefte van de stad. |
| [Contracten met buursteden](features/en.md#neighbor-deals) | Kan **elektriciteit en water kopen of verkopen**, **buren betalen om uw afval over te nemen** of **hun afval tegen betaling accepteren**. Het sluit, past aan en zegt geschikte contracten op als het totaalresultaat de stad ten goede komt en de eigen behoeften en reserves beschermd blijven. |
| [Automatisch slopen](features/en.md#demolition) | Kan standaard ruïnes na ongeveer **30 gesimuleerde dagen** opruimen en geschikte gebouwen die **een gesimuleerd jaar** verlaten zijn. U bepaalt welke zones en welke redenen van verlating meetellen, of beschermde ruïnes mogen worden opgeruimd en of u een maandelijkse bestedingslimiet instelt. |
| [Meldingen van adviseurs](features/en.md#notices) | Gebruikt de eigen adviseurs van het spel om bewezen, aanhoudende tekorten te melden die financiering of beschikbare contracten niet kunnen verhelpen. Waar nuttig bevatten de berichten links naar de betrokken gebouwen. |
| [Advies over de omvang van de stortplaats](features/en.md#garbage-landfills-advice) | Het optionele advies wijst een concrete uitbreiding of veilige verkleining van de stortplaats aan die de afvalrekening met meer dan uw gekozen besparingsdrempel zou kunnen verlagen. **Het Ministerie adviseert; u beslist of u de zonering wijzigt.** |
| [Herstel van spookvakjes](features/en.md#ghost-squares) | Als het beheer van de afvalverwerking aanstaat, herstelt het veilig geschikte onzichtbare stortplaatsvakjes die het spel nog in het onderhoud meetelt. Onveilige of niet-geverifieerde gevallen blijven met rust; voor dit herstel bestaat geen aparte schakelaar. |

U houdt de controle over bouw, zonering, belastingen, wegen en vervoer, politie, brandweer en elke andere dienst die het Ministerie niet beheert. Schakelt u het beheer van een soort contract met buren in, dan kan het Ministerie ook een contract wijzigen of opzeggen dat u eerder handmatig hebt gesloten. Schakel die contractoptie uit om die contracten zelf te beheren.

<a id="first-time-setup"></a>

## Eerste installatie-instellingen

Het Ministerie gebruikt de instellingen die bij de plug-in worden geleverd en **gaat automatisch aan het werk**. U hoeft het niet in te schakelen, maar u moet nagaan wat het mag doen voordat u een belangrijke opgeslagen stad gebruikt.

1\. Start SimCity 4 en open vanuit de **regioweergave** **Spelopties → Ministerie van Efficiëntie**. Wilt u liever, gebruik dan voor een eerste kennismaking een nieuwe of wegwerpstad.

2\. Kies op de pagina **Diensten** welke budgetten en contracten met buursteden het Ministerie mag beheren en hoeveel reservecapaciteit het moet aanhouden.

3\. Controleer op de pagina **Sloop** de regels voor het opruimen van ruïnes en verlaten gebouwen. **Schakel het automatisch slopen uit als u alles zelf wilt platgooien.**

4\. Klik op **Accepteren** om uw keuzes op te slaan. **Annuleren** sluit het venster zonder wijzigingen op te slaan.

5\. Open de stad die u wilt spelen. Het Ministerie volgt uw opgeslagen voorkeuren en beheert de stad terwijl de simulatie loopt.

Uw voorkeuren staan in de INI-bestanden van de plug-in en worden hergebruikt als u een andere stad opent. Het Ministerie bouwt **geen** scholen, ziekenhuizen, elektriciteitscentrales of waterwerken voor u. Als financiering en beschikbare contracten een bewezen, aanhoudend tekort niet kunnen oplossen, meldt een adviseur in het spel waar meer capaciteit nodig is.

U kunt het Ministerie ook in- of uitschakelen via de verordeningen van de stad.

<a id="settings-explained"></a>

## Instellingen

Open **Spelopties → Ministerie van Efficiëntie**. Het venster heeft twee pagina's, **Diensten** en **Sloop**. De tabellen hieronder behandelen **elke instelling die in het menu staat**, zonder de technische formules. Klik op de naam van een optie om de volledige uitleg in de [functiereferentie](features/en.md#options) te lezen.

**Hoe de bediening werkt:** „Aan” betekent dat het Ministerie die functie mag beheren. Een instelling die van een andere afhangt, is grijs zolang de bovenliggende uitstaat. Instellingen in procenten bepalen veiligheidsmarges en drempels en helpen het Ministerie reservecapaciteit aan te houden en schommelingen in de vraag op te vangen.

### Diensten

| Optie | Standaard | Wat het betekent |
|---|---|---|
| [Ministerie actief](features/en.md#opt-master) | **Aan** | Hoofdschakelaar voor financiering, contracten met buursteden, sloop en meldingen van het Ministerie. Uitschakelen maakt reeds doorgevoerde wijzigingen niet ongedaan. |
| [Onderwijs beheren](features/en.md#opt-education) | **Aan** | Past geschikte scholen, universiteiten, bibliotheken en musea automatisch afzonderlijk aan. Wijzigt noch hun dekking noch de financiering van schoolbussen. |
| [Veiligheidsmarge (onderwijs)](features/en.md#opt-educationreserve) | **5%** (0–50%) | Extra capaciteit boven het huidige gebruik van het onderwijs. Meer capaciteit kost meer. Bij gebouwen met weinig bezoekers kan het Ministerie een extra marge aanhouden. Zo kan een school bijvoorbeeld van 10 naar 20 leerlingen (+100%) groeien zonder staking, vooral in het begin, wanneer de extra kosten klein zijn. |
| [Gezondheidszorg beheren](features/en.md#opt-health) | **Aan** | Past geschikte klinieken, ziekenhuizen en andere ondersteunde zorggebouwen automatisch afzonderlijk aan. Wijzigt noch de financiering van ambulances noch de dekking. |
| [Veiligheidsmarge (zorg)](features/en.md#opt-healthreserve) | **5%** (0–50%) | Werkt op dezelfde manier als de veiligheidsmarge van het onderwijs. |
| [Stroom beheren](features/en.md#opt-power) | **Aan** | Past de financiering van afzonderlijke centrales aan om tegen de laagste gemodelleerde kosten voldoende elektriciteit te leveren. |
| [Stroomreserve](features/en.md#opt-powerreserve) | **20%** (10–50%) | Reserve-opwekkingscapaciteit voor het deel van de vraag dat lokaal wordt geleverd; hogere reserves geven meer ruimte voor snelle groei en verkleinen de kans op stroomuitval, maar kunnen meer kosten. |
| [Slijtage van elektriciteitscentrales meewegen](features/en.md#opt-poweraging) | **Uit** | Neemt de geschatte kosten van slijtage en veroudering van centrales mee bij het optimaliseren. Kan hogere financiering bevoordelen als het langetermijnvoordeel van het behoud van de centrale zwaarder weegt dan de extra bedrijfskosten. |
| [Water beheren](features/en.md#opt-water) | **Aan** | Past de financiering van de waterdienst aan om de stad zonder onnodige uitgaven te bevoorraden. |
| [Waterreserve](features/en.md#opt-waterreserve) | **10%** (10–50%) | Werkt op dezelfde manier als de stroomreserve. |
| [Waterleidingen beschermen](features/en.md#opt-waterpipes) | **Aan** | Houdt de financiering op of boven de 50% van het spel, om schade aan het leidingnet te voorkomen. |
| [Afvalverwerking beheren](features/en.md#opt-sanitation) | **Aan** | Past de financiering van de afvaldienst aan om de behoefte aan ophaling en verwerking te dekken. Herstelt ook spookvakjes (een fout in het spel die onderhoud rekent voor stortplaatsvakjes zonder zone). |
| [Reserve afvalverwerking](features/en.md#opt-sanitationreserve) | **2%** (2–20%) | Extra verwerkingscapaciteit voor het afval van de stad, bovenop wat die na recycling produceert en een eventuele achterstand. |
| [Waarschuw me als de stortplaats aanpassen geld bespaart](features/en.md#opt-landfilladvice) | **Uit** | Schakelt het optionele advies in over wijzigingen in de zonering van de stortplaats, als dat geld bespaart en het afval van de stad toch wordt verwerkt. Vereist beheer van de afvalverwerking. |
| [Besparingsdrempel voor de stortplaats](features/en.md#opt-landfillthreshold) | **25%** (20–50%) | Vraagt u alleen om een andere omvang van de stortplaats te overwegen als de gemodelleerde maandelijkse besparing minstens dit deel van het budget voor afvalverwerking bedraagt. |
| [Contracten met buursteden beheren](features/en.md#opt-deals) | **Aan** | Schakelt automatische regionale contracten in; de drie afzonderlijke schakelaars hieronder bepalen welke grondstoffen verhandeld mogen worden. |
| [Bij de ene stad inkopen, aan de andere verkopen](features/en.md#opt-arbitrage) | **Uit** | Staat winstgevende combinaties van inkoop en verkoop van dezelfde grondstof tussen buren toe, waarbij de behoeften van de stad voorgaan. |
| [Stroomcontracten beheren](features/en.md#opt-tradepower) | **Aan** | Staat het Ministerie toe **import en export** van elektriciteit te sluiten, te wijzigen of op te zeggen. Los van de financiering van de centrales. |
| [Watercontracten beheren](features/en.md#opt-tradewater) | **Aan** | Staat het Ministerie toe **import en export** van water te sluiten, te wijzigen of op te zeggen. Los van de waterfinanciering. |
| [Afvalcontracten beheren](features/en.md#opt-tradegarbage) | **Aan** | Staat het Ministerie toe afvalverwerking **in beide richtingen** te regelen. Los van de financiering van de afvalverwerking. |

### Sloop

| Optie | Standaard | Wat het betekent |
|---|---|---|
| [Verwoeste gebouwen slopen](features/en.md#opt-demolishdestroyed) | **Aan** | Sloopt ruïnes na een bepaalde tijd. Ruimt beschermde gemeentelijke of historische ruïnes niet automatisch op, tenzij de optie hieronder dat toestaat. |
| [Wachttijd (verwoeste gebouwen)](features/en.md#opt-destroyedmonths) | **1 maand** (1–24) | Hoe lang ruïnes moeten blijven staan voordat ze kunnen worden opgeruimd; standaard ongeveer **30 gesimuleerde dagen**. |
| [Verwoeste gemeentelijke en historische gebouwen slopen](features/en.md#opt-demolishmunicipal) | **Uit** | Indien ingeschakeld worden na de wachttijd ook geschikte **ruïnes** van gemeentelijke, speciale en historische gebouwen opgeruimd. |
| [Verlaten gebouwen slopen](features/en.md#opt-demolishabandoned) | **Aan** | Ruimt geschikte verlaten gebouwen op na de wachttijd, in de gekozen zonetypen en om de gekozen redenen. |
| [Wachttijd (verlaten gebouwen)](features/en.md#opt-abandonedmonths) | **12 maanden** (1–60) | Hoe lang een gebouw verlaten moet zijn voordat het gesloopt mag worden; standaard een gesimuleerd jaar. |
| [Woonzone](features/en.md#opt-zoneresidential) | **Aan** | Omvat verlaten huizen en appartementsgebouwen. |
| [Commerciële zone](features/en.md#opt-zonecommercial) | **Aan** | Omvat verlaten winkels en kantoren. |
| [Industriezone](features/en.md#opt-zoneindustrial) | **Aan** | Omvat verlaten fabrieken en andere industriële gebouwen. |
| [Landbouwzone](features/en.md#opt-zoneagriculture) | **Uit** | Omvat verlaten boerderijen. Standaard uit omdat lege of verlaten vakjes op een boerderij normaal kunnen zijn. |
| [Reden: onaantrekkelijke omgeving](features/en.md#opt-reasonlowdesirability) | **Aan** | Omvat verlating door lage aantrekkelijkheid, zoals vervuiling of gebrek aan voorzieningen. |
| [Reden: te lange reistijd](features/en.md#opt-reasoncommute) | **Aan** | Omvat verlating door te lange reistijden naar het werk. |
| [Reden: geen vraag](features/en.md#opt-reasonlowdemand) | **Aan** | Omvat gebouwen die verlaten zijn omdat de vraag naar dat type is verdwenen. |
| [Reden: geen stroom](features/en.md#opt-reasonnopower) | **Uit** | Omvat optioneel gebouwen die verlaten zijn door een tekort aan elektriciteit. Meestal is het beter eerst de stroom te herstellen en te kijken of het gebouw weer bewoond raakt. |
| [Reden: geen water](features/en.md#opt-reasonnowater) | **Uit** | Omvat optioneel gebouwen die verlaten zijn door een tekort aan water. Meestal is het beter eerst het water te herstellen en te kijken of het gebouw weer bewoond raakt. |
| [Maandelijks sloopbudget begrenzen](features/en.md#opt-budgetlimit) | **Uit** | Stelt een optioneel bestedingsplafond in voor automatisch slopen, naast de beschikbare middelen in de stadskas. |
| [Maandbudget](features/en.md#opt-demolitionbudget) | **§1.000** (§0–§200.000) | Maximale sloopuitgaven per gesimuleerde maand **als de budgetgrens is ingeschakeld**. |

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

- **SimCity 4 Deluxe, Steam-versie, spelversie 1.1.641.0, op Windows.** Dit is de ondersteunde versie; op andere versies blijft het Ministerie inactief.

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

- **Aanbevolen:** voeg `-CPUCount:1` toe onder **Steam → Bibliotheek → SimCity 4 Deluxe → Eigenschappen → Startopties**. Dit hielp de stabiliteit in lange sessies op de computer van de auteur.

- **Tien talen in het spel:** Engels, Portugees, Frans, Duits, Spaans, Italiaans, Nederlands, Deens, Zweeds en Pools. Het Ministerie volgt automatisch de taal van uw spel.

<a id="download-and-installation"></a>

## Downloaden en installeren

1\. Open de **Releases**-pagina van het project (`<PUBLIC-REPOSITORY-URL>/releases`) en kies de nieuwste release. Vervang deze plaatsaanduiding vóór publicatie door het adres van de openbare repository.

2\. Download onder **Assets** de **distributie-ZIP van Department of Efficiency**.

3\. **Sluit SimCity 4.** Pak de ZIP uit in een tijdelijke map.

4\. Kopieer **`DepartmentEfficiency.dll`** en de hele map **`Department of Efficiency`** naar de hoofdmap van `Documenten\SimCity 4\Plugins`.

5\. Start het spel en volg de [eerste installatie-instellingen](#first-time-setup) voordat u een opgeslagen stad speelt.

De distributie-ZIP bevat de DLL, een `README.txt` en een map met vijf DAT- en drie INI-bestanden. Na de installatie hebt u:

```text
Documents\SimCity 4\Plugins\
    README files
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
    features\
    screenshots\
```

De Markdown-bestanden en schermafbeeldingen van de openbare repository zijn **niet** nodig voor het spel. Houd de DLL **direct in Plugins**, met de bijbehorende map ernaast.

**Instellingen later wijzigen:** open **Spelopties → Ministerie van Efficiëntie**, gebruik de pagina's **Diensten** en **Sloop** en klik op **Accepteren**. Voor de hoofdschakelaar kunt u ook de verordening **Ministerie van Efficiëntie oprichten** gebruiken, zonder maandelijkse kosten. Het Ministerie uitschakelen stopt toekomstige acties; reeds toegepaste financiering, contracten en sloop worden niet ongedaan gemaakt.

<a id="updating-and-uninstalling"></a>

## Bijwerken en verwijderen

**Bewaar uw voorkeuren.** De drie INI-bestanden in de map `Department of Efficiency` bevatten uw instellingen. Maak vóór het vervangen van de mod een back-up van `DepartmentEfficiency.ini`, `DepartmentEfficiencyUtility.ini` en `DepartmentEfficiencyDemolition.ini` buiten uw Plugins-map.

**Een installatie vervangen:** sluit het spel, verwijder de bestaande `DepartmentEfficiency.dll` en de map `Department of Efficiency`, installeer het nieuwe pakket en zet uw back-up van de INI-bestanden **alleen terug als het formaat compatibel is** met de nieuwe release. Stel anders uw voorkeuren opnieuw in het spel in.

**Verwijderen:** sluit het spel en verwijder `DepartmentEfficiency.dll` en de map `Department of Efficiency`. Uw opgeslagen steden blijven zonder de mod leesbaar. Financieringsniveaus, bestaande contracten en sloop die al in een stad is opgeslagen, worden niet automatisch teruggedraaid.

<a id="known-limitations"></a>

## Bekende beperkingen

- **Compatibiliteit:** alleen de Windows-Steamversie **1.1.641.0** van SimCity 4 Deluxe wordt ondersteund. Andere spelversies blijven inactief.

- **Advies over de stortplaats:** vereist precies één stortplaatszone. In steden met meerdere stortplaatsen kan het Ministerie nog wel geschikte foutieve „spookvakjes” herstellen, maar het doet geen aanbevelingen over de omvang.

<a id="credits"></a>

## Dankbetuigingen

- Gemaakt door **Sammu89**.

- Gebouwd met **gzcom-dll** van Nelson Gomez (simmaster07), gebruikt onder de licentie LGPL-2.1-or-later en statisch in de DLL gelinkt.

- Nicholas Hayes (0xC0000054 / Null 45), voor onderzoek, documentatie en open voorbeelden over SimCity 4-DLL's: sc4-dll-utilities, sc4-custom-budget-departments, sc4-query-ui-hooks, sc4-lua-extensions, sc4-lua-extension-example.

- CorinaMarie, voor het documenteren van de Urgent Advice-techniek van SimCity 4 waarop de adviseurmeldingen van de mod zijn gebaseerd.

- De SimCity 4-moddinggemeenschap, voor twee decennia onderzoek, hulpmiddelen en gedeelde kennis.
