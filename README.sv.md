[English](README.md) | [Português](README.pt.md) | [Français](README.fr.md) | [Deutsch](README.de.md) | [Español](README.es.md) | [Italiano](README.it.md) | [Nederlands](README.nl.md) | [Dansk](README.da.md) | **Svenska** | [Polski](README.pl.md)

# Effektivitetsdepartementet

**Effektivitetsdepartementet tar hand om de små detaljerna i stadens skötsel som gör stor skillnad.**

Effektivitetsdepartementet (*Department of Efficiency* på engelska) är ett tillägg för **SimCity 4 Deluxe** som automatiserar rutinuppgifter i stadens skötsel, så att du kan koncentrera dig på att bygga, planera och låta din stad växa.

**Du bygger staden. Departementet optimerar budgeten!**

Det sköter budgeten för enskilda utbildnings- och vårdbyggnader, kraftverk, finansieringen av vatten och sophantering, avtal med grannstäder och, som tillval, rivning av ruiner och övergivna byggnader. Målet är att täcka stadens behov utan att spendera mer än nödvändigt, medan du själv bestämmer var byggnaderna placeras, täckningen och de stora planeringsbesluten.

Allt styrs inne i spelet under **Spelalternativ → Effektivitetsdepartementet**. Välj vilka tjänster som ska automatiseras, justera deras säkerhetsmarginaler, ställ in rivningen eller stäng av hela Departementet. Du kan också använda stadens förordning **Inrätta ett effektivitetsdepartement** som huvudströmbrytare. Inget separat program behövs.

> [!IMPORTANT]
> **Departementet startar påslaget, inklusive automatisk rivning av förstörda och övergivna byggnader.** Gå igenom inställningarna innan du öppnar en stad som du bryr dig om. Det gör verkliga ändringar i spelet: finansiering, avtal och rivningar sparas om du sparar staden. Det redigerar inte sparfiler direkt.

## Innehåll

- [Hämtning och installation](#download-and-installation)

- [Vad Departementet gör](#features)

- [Första inställningen](#first-time-setup)

- [Inställningar](#settings-explained)

- [Skärmbilder](#screenshots)

- [Krav och kompatibilitet](#requirements-and-compatibility)

- [Kompatibilitet med Phantom Funding Slider Bug](#phantom-funding-slider-bug)

- [Uppdatera och avinstallera](#updating-and-uninstalling)

- [Kända begränsningar](#known-limitations)

- [Källkod](#source-code)

- [Licens](#license)

- [Tack till](#credits)

<a id="download-and-installation"></a>

## Hämtning och installation

### [⬇ Ladda ner Department of Efficiency 1.0.2](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases/download/v1.0.2/Departement_of_Efficiency_1.02.zip)

**Nytt i 1.0.2:** se [ändringsloggen](CHANGELOG.md) (på engelska).

1\. Öppna projektets [releasesida](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases) och välj den senaste utgåvan.

2\. Under **Assets** hämtar du **distributions-ZIP:en för Department of Efficiency**.

3\. **Stäng SimCity 4.** Packa upp ZIP-filen i en tillfällig mapp.

4\. Kopiera de **tre filerna** **`DepartmentEfficiency.dll`**, **`DepartmentEfficiency.dat`** och **`DepartmentEfficiency.ini`** direkt till `Documents\SimCity 4\Plugins`. Skapa ingen undermapp.

5\. Starta spelet och följ anvisningarna under [första inställningen](#first-time-setup) innan du spelar en sparad stad.

Distributions-ZIP:en innehåller exakt dessa tre filer. Efter installationen ska du ha:

```text
Documents\SimCity 4\Plugins\
    DepartmentEfficiency.dll
    DepartmentEfficiency.dat
    DepartmentEfficiency.ini
```

Det publika förvarets Markdown-filer och skärmbilder behövs **inte** för spelet. Behåll de tre filerna **direkt i Plugins**, sida vid sida. Mappen `Department of Efficiency` finns inte längre.

> [!WARNING]
> **Uppdaterar du från en tidigare version?** Kopiera inte de nya filerna över de gamla. Ta först bort Departementets gamla filer, som beskrivs under [Uppdatering och avinstallation](#updating-and-uninstalling).

**Ändra inställningar senare:** öppna **Spelalternativ → Effektivitetsdepartementet**, använd sidorna **Samhällsservice** och **Rivning** och klicka på **Godkänn**. För huvudströmbrytaren kan du också använda förordningen **Inrätta ett effektivitetsdepartement**, utan månadskostnad. Att stänga av Departementet stoppar framtida åtgärder; det ångrar inte finansiering, avtal eller rivningar som redan genomförts.

<a id="features"></a>

## Vad Departementet gör

Tabellen nedan beskriver vad Departementet gör. För formler, exempel, skyddsåtgärder och mekaniken bakom besluten, se den **[utförliga funktionsreferensen (på engelska)](technical_docs/en/index.md)**.

| Område | Vad det gör |
|---|---|
| [Utbildning och sjukvård](technical_docs/en/education_healthcare.md#education-healthcare) | Justerar finansieringen **byggnad för byggnad** för lämpliga skolor, universitet, bibliotek, museer, kliniker och sjukhus. Det följer verklig beläggning och kapacitet, med en säkerhetsmarginal. Det **ändrar inte täckningsradier** eller finansieringen av skolbussar och ambulanser. Om en byggnad fortfarande är överfull vid maximal finansiering meddelar en rådgivare att ytterligare en anläggning behövs; **du väljer var den ska ligga**. |
| [El](technical_docs/en/power.md#power) | Sköter **varje kraftverk för sig** och väljer den finansiering som täcker stadens behov och säkerhetsreserven till lägsta modellerade kostnad. Det tar hänsyn till kraftverkens verkningsgrad och skick; en valfri inställning räknar också med de ekonomiska kostnaderna för kraftverkens åldrande. |
| [Vatten](technical_docs/en/water.md#water) | Justerar stadens vattenfinansiering så att den täcker behovet plus en säkerhetsreserv. Skydd av vattenledningarna är påslaget som standard för att undvika nedskärningar som skulle låta nätet förfalla. Det döljer också spelets vilseledande ikon „inget vatten“ över gårdar. |
| [Sophantering](technical_docs/en/garbage.md#garbage-landfills) | Justerar finansieringen av sophanteringen utifrån sopproduktion, återvinning, behandling, soptippens lagringsutrymme och regionala avtal, samtidigt som stadens eget behov av bortskaffning skyddas. |
| [Grannavtal](technical_docs/en/deals.md#neighbor-deals) | Kan **köpa eller sälja el och vatten**, **betala grannar för att ta emot ditt avfall** eller **ta emot deras mot betalning**. Det skapar, justerar och säger upp lämpliga avtal när det samlade resultatet gynnar staden och dess egna behov och reserver förblir skyddade. |
| [Automatisk rivning](technical_docs/en/demolition.md#demolition) | Kan som standard röja ruiner efter ungefär **30 simulerade dagar** och lämpliga byggnader som varit övergivna i **ett simulerat år**. Du bestämmer vilka zoner och vilka orsaker till övergivenhet som räknas, om skyddade ruiner får röjas och om en månatlig utgiftsgräns ska gälla. |
| [Rådgivarnas meddelanden](technical_docs/en/safety.md#notices) | Använder spelets egna rådgivare för att flagga styrkta, ihållande brister som varken finansiering eller tillgängliga avtal kan åtgärda. Där det passar innehåller meddelandena länkar som tar dig till de berörda byggnaderna. Det lägger också till en nyhet (rubrik i listan, detaljer vid klick) när en tjänst du har aktiverat måste stoppas på grund av ett tekniskt fel. |
| [Råd om soptippens storlek](technical_docs/en/garbage.md#garbage-landfills-advice) | Det valfria rådet pekar ut en bestämd, säker minskning av soptippen (tomma rutor som kan tas bort), som kan sänka sophanteringsräkningen med mer än din valda spargräns. **Departementet ger råd; du avgör om zonindelningen ska ändras.** |
| [Reparation av spökrutor](technical_docs/en/garbage.md#ghost-tiles) | När skötseln av sophanteringen är påslagen repareras lämpliga osynliga soptippsrutor som spelet fortfarande räknar med i underhållet, på ett säkert sätt. Osäkra eller overifierade fall lämnas ifred; det finns ingen separat strömbrytare för den här reparationen. |

Du styr fortfarande över byggande, zonindelning, skatter, vägar och transporter, polis, brandkår och alla andra tjänster som Departementet inte sköter. Om du slår på skötseln av en typ av grannavtal kan Departementet också ändra eller säga upp ett avtal som du tidigare gjort för hand. Stäng av det avtalsalternativet om du vill sköta de avtalen själv.

<a id="first-time-setup"></a>

## Första inställningen

Departementet använder de inställningar som följer med tillägget och **börjar arbeta automatiskt**. Du behöver inte slå på det, men du bör gå igenom vad det får göra innan du använder en viktig sparad stad.

1\. Starta SimCity 4 och öppna från **regionvyn** **Spelalternativ → Effektivitetsdepartementet**. Om du vill kan du använda en ny eller engångsstad för en första titt.

2\. På sidan **Samhällsservice** väljer du vilka budgetar och grannavtal Departementet får sköta och hur mycket reservkapacitet det ska hålla.

3\. På sidan **Rivning** kontrollerar du reglerna för röjning av ruiner och övergivna byggnader. **Stäng av den automatiska rivningen om du vill jämna allt med marken själv.**

4\. Klicka på **Godkänn** för att spara dina val. **Avbryt** stänger fönstret utan att spara ändringar.

5\. Öppna den stad du vill spela. Departementet följer dina sparade inställningar och sköter staden medan simuleringen pågår.

Dina inställningar lagras i `DepartmentEfficiency.ini` och återanvänds när du öppnar en annan stad. Departementet bygger **inte** skolor, sjukhus, kraftverk eller vattenanläggningar åt dig. När finansiering och tillgängliga avtal inte kan lösa en styrkt, ihållande brist talar en rådgivare i spelet om var mer kapacitet behövs.

Du kan också slå på eller stänga av Departementet via stadens förordningar.

<a id="settings-explained"></a>

## Inställningar

Öppna **Spelalternativ → Effektivitetsdepartementet**. Fönstret har två sidor, **Samhällsservice** och **Rivning**. Tabellerna nedan täcker **alla inställningar som visas i menyn**, utan de tekniska formlerna. Klicka på ett alternativs namn för att läsa hela förklaringen i [funktionsreferensen](technical_docs/en/options.md#options).

**Så fungerar reglagen:** „På“ betyder att Departementet får sköta den funktionen. En inställning som beror på en annan är nedtonad så länge den överordnade är avstängd. Procentinställningar styr säkerhetsmarginaler och gränsvärden och hjälper Departementet att hålla reservkapacitet och hantera svängningar i efterfrågan.

### Samhällsservice

| Alternativ | Standard | Vad det betyder |
|---|---|---|
| [Departementet aktivt](technical_docs/en/options.md#opt-master) | **På** | Huvudströmbrytare för finansiering, grannavtal, rivning och Departementets meddelanden. Att stänga av det ångrar inte ändringar som redan gjorts. |
| [Sköt utbildningen](technical_docs/en/options.md#opt-education) | **På** | Justerar automatiskt lämpliga skolor, universitet, bibliotek och museer en och en. Ändrar varken deras täckning eller finansieringen av skolbussar. |
| [Säkerhetsmarginal (utbildning)](technical_docs/en/options.md#opt-educationreserve) | **5 %** (0–50 %) | Extra kapacitet utöver utbildningens nuvarande användning. Mer kapacitet kostar mer. För byggnader med låg beläggning kan Departementet hålla en extra marginal. Det kan till exempel låta en skola växa från 10 till 20 elever (+100 %) utan strejk, särskilt i början när merkostnaden är liten. |
| [Sköt sjukvården](technical_docs/en/options.md#opt-health) | **På** | Justerar automatiskt lämpliga kliniker, sjukhus och andra stödda vårdbyggnader en och en. Ändrar varken ambulansernas finansiering eller täckningen. |
| [Säkerhetsmarginal (sjukvård)](technical_docs/en/options.md#opt-healthreserve) | **5 %** (0–50 %) | Fungerar på samma sätt som utbildningens säkerhetsmarginal. |
| [Sköt elförsörjningen](technical_docs/en/options.md#opt-power) | **På** | Justerar finansieringen av enskilda kraftverk för att leverera tillräckligt med el till lägsta modellerade kostnad. |
| [Elreserv](technical_docs/en/options.md#opt-powerreserve) | **20 %** (10–50 %) | Reservproduktionskapacitet för den del av efterfrågan som täcks lokalt; större reserver ger mer utrymme för snabb tillväxt och minskar risken för strömavbrott, men kan kosta mer. |
| [Ta hänsyn till kraftverkens åldrande](technical_docs/en/options.md#opt-poweraging) | **Av** | Räknar med den uppskattade kostnaden för kraftverkens slitage och åldrande i optimeringen. Kan gynna högre finansiering när den långsiktiga vinsten av att bevara kraftverket överstiger den extra driftkostnaden. |
| [Sköt vattenförsörjningen](technical_docs/en/options.md#opt-water) | **På** | Justerar vattenverkets finansiering för att förse staden utan onödiga utgifter. |
| [Vattenreserv](technical_docs/en/options.md#opt-waterreserve) | **10 %** (10–50 %) | Fungerar på samma sätt som elreserven. |
| [Skydda vattenledningarna](technical_docs/en/options.md#opt-waterpipes) | **På** | Håller finansieringen på eller över spelets 50 % för att undvika skador på ledningsnätet. |
| [Sköt sophanteringen](technical_docs/en/options.md#opt-sanitation) | **På** | Justerar sophanteringens finansiering för att täcka behovet av insamling och bortskaffning. Reparerar också spökrutor (ett fel i spelet som tar ut underhåll för soptippsrutor som inte längre är zonade). |
| [Reserv för sophantering](technical_docs/en/options.md#opt-sanitationreserve) | **2 %** (2–20 %) | Extra behandlingskapacitet för stadens sopor, utöver det den producerar efter återvinning och eventuell eftersläpning. |
| [Varna mig när soptippens storlek kan spara pengar](technical_docs/en/options.md#opt-landfilladvice) | **Av** | Slår på det valfria rådet om ändringar i soptippens zonindelning när det sparar pengar och stadens sopor ändå hanteras. Kräver att sophanteringen sköts. |
| [Spargräns för soptippsmeddelanden](technical_docs/en/options.md#opt-landfillthreshold) | **25 %** (20–50 %) | Ber dig bara överväga en ändring av soptippens storlek om den modellerade månatliga besparingen uppgår till minst denna andel av sophanteringsbudgeten. |
| [Sköt grannavtal](technical_docs/en/options.md#opt-deals) | **På** | Slår på automatiska regionala avtal; de tre enskilda reglagen nedan avgör vilka resurser som får handlas. |
| [Köp från en stad och sälj till en annan](technical_docs/en/options.md#opt-arbitrage) | **Av** | Tillåter lönsamma kombinationer av köp och försäljning av samma resurs mellan grannar, med stadens egna behov främst. |
| [Sköt elavtal](technical_docs/en/options.md#opt-tradepower) | **På** | Låter Departementet skapa, ändra eller säga upp **import och export** av el. Oberoende av finansieringen av kraftverken. |
| [Sköt vattenavtal](technical_docs/en/options.md#opt-tradewater) | **På** | Låter Departementet skapa, ändra eller säga upp **import och export** av vatten. Oberoende av vattenfinansieringen. |
| [Sköt sopavtal](technical_docs/en/options.md#opt-tradegarbage) | **På** | Låter Departementet ordna sopbehandling **i båda riktningarna**. Oberoende av sophanteringens finansiering. |

### Rivning

| Alternativ | Standard | Vad det betyder |
|---|---|---|
| [Riv förstörda byggnader](technical_docs/en/options.md#opt-demolishdestroyed) | **På** | River ruiner efter en viss tid. Röjer inte automatiskt skyddade kommunala eller historiska ruiner om inte alternativet nedan tillåter det. |
| [Väntetid (förstörda byggnader)](technical_docs/en/options.md#opt-destroyedmonths) | **1 månad** (1–24) | Hur länge ruiner måste ligga kvar innan de kan röjas; som standard ungefär **30 simulerade dagar**. |
| [Riv förstörda kommunala och historiska byggnader](technical_docs/en/options.md#opt-demolishmunicipal) | **Av** | Om påslaget röjs efter väntetiden även lämpliga **ruiner** av kommunala, speciella och historiska byggnader. |
| [Riv övergivna byggnader](technical_docs/en/options.md#opt-demolishabandoned) | **På** | Röjer lämpliga övergivna byggnader efter väntetiden, i de valda zontyperna och av de valda orsakerna. |
| [Väntetid (övergivna byggnader)](technical_docs/en/options.md#opt-abandonedmonths) | **12 månader** (1–60) | Hur länge en byggnad måste stå övergiven innan den får rivas; som standard ett simulerat år. |
| [Bostadszon](technical_docs/en/options.md#opt-zoneresidential) | **På** | Omfattar övergivna hus och flerbostadshus. |
| [Handelszon](technical_docs/en/options.md#opt-zonecommercial) | **På** | Omfattar övergivna butiker och kontor. |
| [Industrizon](technical_docs/en/options.md#opt-zoneindustrial) | **På** | Omfattar övergivna fabriker och andra industribyggnader. |
| [Jordbrukszon](technical_docs/en/options.md#opt-zoneagriculture) | **Av** | Omfattar övergivna gårdar. Avstängd som standard eftersom tomma eller övergivna rutor på en gård kan vara normala. |
| [Orsak: oattraktivt område](technical_docs/en/options.md#opt-reasonlowdesirability) | **På** | Omfattar övergivenhet som beror på låg attraktivitet, till exempel föroreningar eller brist på service. |
| [Orsak: för lång arbetsresa](technical_docs/en/options.md#opt-reasoncommute) | **På** | Omfattar övergivenhet som beror på alltför långa resor till arbetet. |
| [Orsak: ingen efterfrågan](technical_docs/en/options.md#opt-reasonlowdemand) | **På** | Omfattar byggnader som övergivits för att efterfrågan på den typen har försvunnit. |
| [Orsak: ingen el](technical_docs/en/options.md#opt-reasonnopower) | **Av** | Omfattar som tillval byggnader som övergivits på grund av elbrist. Oftast är det bättre att först återställa elen och se om byggnaden fylls på igen. |
| [Orsak: inget vatten](technical_docs/en/options.md#opt-reasonnowater) | **Av** | Omfattar som tillval byggnader som övergivits på grund av vattenbrist. Oftast är det bättre att först återställa vattnet och se om byggnaden fylls på igen. |
| [Begränsa den månatliga rivningsbudgeten](technical_docs/en/options.md#opt-budgetlimit) | **Av** | Sätter ett valfritt utgiftstak för automatisk rivning, utöver de medel som finns i stadens kassa. |
| [Månadsbudget](technical_docs/en/options.md#opt-demolitionbudget) | **§1 000** (§0–§200 000) | Högsta rivningsutgift per simulerad månad **när budgetgränsen är påslagen**. |

<a id="screenshots"></a>

## Skärmbilder

Alla skärmbilder visar den engelska versionen av spelet.

| | |
|---|---|
| ![Sidan Samhällsservice bland Departementets inställningar](screenshots/Options-1.png) | ![Sidan Rivning bland Departementets inställningar](screenshots/Options-2.png) |
| *Sidan Samhällsservice: varje tjänst, dess reserv och grannavtalen.* | *Sidan Rivning: ruiner, övergivna byggnader och månadsbudgeten.* |
| ![Ett rådgivarmeddelande om överfulla gymnasieskolor](screenshots/high_school.png) | ![Ett rådgivarmeddelande om överfulla museer](screenshots/museum.png) |
| *Skolor vid maximal finansiering och fortfarande överfulla: meddelandet länkar till byggnaderna.* | *Detsamma för museer.* |
| ![Ett rådgivarmeddelande om vattenbrist](screenshots/water.png) | ![Ett rådgivarmeddelande om brist på sopkapacitet](screenshots/real-garbage-paused.png) |
| *Vatten: Departementet kan inte lösa bristen med den tillgängliga försörjningen.* | *Sopor: staden behöver mer kapacitet för bortskaffning.* |

<a id="requirements-and-compatibility"></a>

## Krav och kompatibilitet

- **SimCity 4 Deluxe, Steam-utgåvan, spelversion 1.1.641.0, på Windows.** Jag har testat Departementet på den här versionen; andra versioner har inte testats.

- **Ingen annan mod, installationsprogram eller extern applikation behövs.**

- **Testad och kompatibel med dessa moddar** (testade versioner):

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

- **Rekommenderat:** lägg till `-CPUCount:1` under **Steam → Bibliotek → SimCity 4 Deluxe → Egenskaper → Startalternativ**. Det förbättrade stabiliteten i långa sessioner på min dator.

- **Tio språk i spelet:** engelska, portugisiska, franska, tyska, spanska, italienska, nederländska, danska, svenska och polska. Departementet följer automatiskt spelets språk.

<a id="phantom-funding-slider-bug"></a>

## Kompatibilitet med Phantom Funding Slider Bug

Effektivitetsdepartementet är kompatibelt med städer som drabbats av **Phantom Funding Slider Bug**, ett gammalt SimCity 4-problem där finansieringsreglagen för redan placerade byggnader fortsätter att röra sig av sig själva efter att underhållskostnaden för den byggnadstypen har ändrats (till exempel av en mod).

Det åtgärdar **inte** själva SimCity 4-felet. I stället upptäcker det misstänkta, återkommande förändringar i finansieringen och justerar automatiskt de anläggningar det hanterar, vilket hjälper till att behålla den nödvändiga servicekapaciteten och undvika onödig överfinansiering.

I de testade scenarierna skedde rättelserna inom en simulerad dag i stället för att potentiellt ta en hel månad.

Skyddet gäller lämpliga anläggningar (skolor, bibliotek, museer, kliniker och sjukhus) under automatisk hantering. Det reparerar inte spelets interna budgetdata och garanterar inte skydd i alla situationer. Se [Phantom Funding Slider Bug](technical_docs/en/phantom_slider.md) för hur det fungerar, dess gränser och testresultaten (på engelska).

<a id="updating-and-uninstalling"></a>

## Uppdatera och avinstallera

**Inställningarna förs inte över.** Sedan version 1.0.2 har Departementet en enda inställningsfil, `DepartmentEfficiency.ini`, som ersätter de tre INI-filerna från version 1.0.0 och mappen `Department of Efficiency` från det första 1.0.2-paketet. Inställningar i äldre filer läses eller migreras inte: ställ in dina inställningar på nytt under **Spelalternativ → Effektivitetsdepartementet**.

**Så uppdaterar du en tidigare installation (en ren installation krävs).** Stäng spelet och ta bort Departementets gamla filer **innan** du kopierar de tre nya. Ta bara bort dessa, i `Documents\SimCity 4\Plugins`:

- `DepartmentEfficiency.dll`, `DepartmentEfficiency.dat` och `DepartmentEfficiency.ini`;
- hela mappen `Department of Efficiency`, om den finns;
- från version 1.0.0, om de fortfarande ligger kvar: `DepartmentEfficiencyUI.dat`, `DepartmentEfficiencyCivicAdvice.dat`, `DepartmentEfficiencyDealAdvice.dat`, `DepartmentEfficiencyGarbageAdvice.dat`, `DepartmentEfficiencyUtility.ini` och `DepartmentEfficiencyDemolition.ini`;
- `DepartmentEfficiency.told.txt`, en liten minnesfil som tillägget kan ha lämnat kvar (den skapas på nytt vid behov).

Rör inte andra moddars filer. Att kopiera de nya filerna över en gammal installation stöds inte: de gamla resursfilerna skulle ligga kvar och kunna hålla spelets egna råd tysta. Tillägget läser aldrig filer från den gamla mappen; om mappen fortfarande finns skriver det bara en varning i sin logg.

**Avinstallation:** stäng spelet och ta bort samma filer. Dina sparade städer kan fortfarande läsas utan modden. Finansieringsnivåer, befintliga avtal och rivningar som redan sparats i en stad ångras inte automatiskt.

**Loggfil:** `DepartmentEfficiency.log` finns i mappen `Logs` i din SimCity 4-användarmapp (normalt `Documents\SimCity 4\Logs`, eller mappen `Logs` i den mapp som anges med `-UserDir`). Kan den mappen inte användas skrivs den bredvid DLL-filen.

<a id="known-limitations"></a>

## Kända begränsningar

- **Kompatibilitet:** jag har testat Windows-Steam-versionen **1.1.641.0** av SimCity 4 Deluxe. Andra spelversioner har inte testats.

- **Råd om soptippen:** kräver exakt en soptippszon. I städer med flera soptippar kan Departementet fortfarande reparera lämpliga buggade „spökrutor“ men rekommenderar inga storleksändringar.

<a id="source-code"></a>

## Källkod

Pluginets källkod finns i mappen [`source/`](source/). Den byggs med CMake och Visual Studio till en 32-bitars (Win32) DLL. Du behöver Visual Studio 2026 med arbetsbelastningen för skrivbordsutveckling med C++ (andra versioner är inte testade), CMake 3.20 eller senare och Git:

```bat
cmake -S . -B build -G "Visual Studio 18 2026" -A Win32
cmake --build build --config Release
```

Vid bygget hämtas [gzcom-dll](https://github.com/nsgomez/gzcom-dll) i en fast revision (`779b669bf88112146d58fba6c7ed44f13ac6ce7d`). Om du vill använda en ändrad kopia av biblioteket, eller bygga utan internetanslutning, lägg till `-DFETCHCONTENT_SOURCE_DIR_GZCOM_DLL=<mapp>` i det första kommandot. DLL-filen skapas i `build\Release\DepartmentEfficiency.dll`. En ombyggd DLL är inte byte för byte identisk med den publicerade.

<a id="license"></a>

## Licens

- Den ursprungliga koden i Department of Efficiency (filerna i [`source/`](source/)) är skriven av mig, **Sammu89** och ges ut under **GNU General Public License, version 3 endast** (GPL-3.0-only). Hela texten finns i [LICENSE](LICENSE). Du får studera, ändra och vidaredistribuera den enligt GPLv3: behåll upphovsrättsmeddelanden och tack, ange dina ändringar och publicera din version under samma licens. Presentera inte ändrade versioner som mina officiella versioner.

- Dokumentationen (dessa README-filer och filerna i [`technical_docs/`](technical_docs/)) och de texter som skrivits för detta projekt (inklusive de i INI-filen och de som kompilerats i `DepartmentEfficiency.dat`), tillsammans med deras ursprungliga översättningar, ges ut under samma licens, GPL-3.0-only.

- **gzcom-dll** omfattas inte av denna licens: biblioteket behåller sin egen licens, LGPL-2.1-or-later ([licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt)), och hämtas från sitt ursprungliga arkiv vid bygget.

- **SimCity 4** och dess ursprungliga resurser tillhör Electronic Arts / Maxis. **Effektivitetsdepartementet** är en inofficiell fanmod, oberoende av EA och inte godkänd av EA. Förutom projektets egna texter och skript innehåller `DepartmentEfficiency.dat` två gränssnittsskript (alternativknappen och alternativfönstret) som härleds från spelets egna resurser, och skärmbilderna visar spelet: detta är inte min egendom, omfattas inte av GPL och alla rättigheter ligger kvar hos sina ägare. Detsamma gäller allt annat material från tredje part. Spelets körbara fil distribueras inte.

<a id="credits"></a>

## Tack till

**Upphovsperson**

- Skapad av **Sammu89**.

**Särskilt tack**

- **Särskilt tack till CorinaMarie** för hennes utförliga förklaringar av Phantom Funding Slider Bug, för att hon generöst delat de reproducerbara teststäderna och DAT-filerna som gjorde det möjligt att undersöka och mildra dess effekter, och för hennes ovärderliga tekniska vägledning om att bevara och återställa Maxis ursprungliga rådgivarmeddelanden, inklusive den selektiva hanteringen av Lua-rådposter. Hennes hjälp gjorde Departementet mer robust och mer kompatibelt med spelet och med andra moddar.

Koden till Effektivitetsdepartementet är skriven av Sammu89. De personer som nämns här delade kunskap, verktyg, testmaterial och råd; de är inte medförfattare till koden.

**Använt bibliotek**

- **gzcom-dll** av Nelson Gomez (simmaster07), med bidrag från Nicholas Hayes (Null 45), Casper Van Gheluwe, memo och andra (se dess [arkiv](https://github.com/nsgomez/gzcom-dll)), använd under licensen LGPL-2.1-or-later och statiskt länkad i DLL-filen. Biblioteket medföljer inte: bygget hämtar den exakta revisionen från det ursprungliga arkivet (se [Källkod](#source-code)); upphovsrättsmeddelandena och licenstexten finns i [licenses/gzcom-dll.txt](licenses/gzcom-dll.txt) och [licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt). Du får ändra biblioteket och bygga om pluginet med din version.

**Konsulterade tekniker, dokumentation och exempel** (ingen kod har kopierats från dem; deras upphovspersoner är inte medförfattare till Department of Efficiency):

- Nicholas Hayes (0xC0000054 / Null 45), för forskning, dokumentation och öppna exempel om SimCity 4-DLL:er: sc4-dll-utilities, sc4-custom-budget-departments, sc4-query-ui-hooks, sc4-lua-extensions, sc4-lua-extension-example. Inställningsfönstret, förordningen och meddelandefilen följer mekanismer publicerade i hans projekt sc4-region-census, sc4-humane-society-ordinance och sc4-lua-extension-example (MIT-licens); spelkommandona följer sc4-auto-run-cheats och sc4-game-command-documentation.

- CorinaMarie, för att ha dokumenterat SimCity 4:s Urgent Advice-teknik som moddens rådgivarmeddelanden följer.

- memo, för vägledningen om att läsa in ett eget Lua-skript i tillägget, med egna identifierare, efter spelets skript och ändra endast de rådposter som behövs, vilket Departementets integration med spelets rådgivare följer.

- Kel9509, för informationen om hur man återskapar Phantom Funding Slider Bug.

- SimCity 4-modding-gemenskapen, för två decennier av forskning, verktyg och delad kunskap, inklusive SC4Devotions dokumentation av exemplar-egenskaper.

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
