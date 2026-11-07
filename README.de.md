[English](README.md) | [Português](README.pt.md) | [Français](README.fr.md) | **Deutsch** | [Español](README.es.md) | [Italiano](README.it.md) | [Nederlands](README.nl.md) | [Dansk](README.da.md) | [Svenska](README.sv.md) | [Polski](README.pl.md)

# Ministerium für Effizienz

**Das Ministerium für Effizienz kümmert sich um die kleinen Details der Stadtverwaltung, die einen großen Unterschied machen.**

Das Ministerium für Effizienz (*Department of Efficiency* auf Englisch) ist ein Plugin für **SimCity 4 Deluxe**, das Routineaufgaben der Stadtverwaltung automatisiert, damit Sie sich aufs Bauen, Planen und Wachsenlassen Ihrer Stadt konzentrieren können.

**Sie bauen die Stadt. Das Ministerium optimiert das Budget!**

Es verwaltet die Budgets einzelner Bildungs- und Gesundheitseinrichtungen, Kraftwerke, die Finanzierung von Wasser und Müllentsorgung, Verträge mit Nachbarstädten und, optional, den Abriss von Ruinen und verlassenen Gebäuden. Ziel ist es, den Bedarf der Stadt zu decken, ohne mehr als nötig auszugeben; die Platzierung von Gebäuden, die Abdeckung und die großen Planungsentscheidungen bleiben bei Ihnen.

Alles wird im Spiel unter **Spieloptionen → Ministerium für Effizienz** gesteuert. Wählen Sie die Dienste, die automatisiert werden sollen, passen Sie deren Sicherheitsreserven an, richten Sie den Abriss ein oder schalten Sie das ganze Ministerium aus. Als Hauptschalter können Sie auch die Verordnung **Ministerium für Effizienz einrichten** verwenden. Eine separate Anwendung ist nicht nötig.

> [!IMPORTANT]
> **Das Ministerium startet eingeschaltet, einschließlich des automatischen Abrisses zerstörter und verlassener Gebäude.** Prüfen Sie seine Einstellungen, bevor Sie eine Stadt öffnen, die Ihnen wichtig ist. Es nimmt echte Änderungen im Spiel vor: Finanzierung, Verträge und Abrisse werden gespeichert, wenn Sie die Stadt speichern. Spielstände werden nicht direkt bearbeitet.

## Inhalt

- [Download und Installation](#download-and-installation)

- [Was das Ministerium tut](#features)

- [Ersteinrichtung](#first-time-setup)

- [Einstellungen](#settings-explained)

- [Screenshots](#screenshots)

- [Voraussetzungen und Kompatibilität](#requirements-and-compatibility)

- [Aktualisieren und Deinstallieren](#updating-and-uninstalling)

- [Bekannte Einschränkungen](#known-limitations)

- [Quellcode](#source-code)

- [Lizenz](#license)

- [Danksagungen](#credits)

<a id="download-and-installation"></a>

## Download und Installation

### [⬇ Department of Efficiency 1.0.0 herunterladen](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases/download/v1.0.0/Departement_of_Efficiency_1.00.zip)

1\. Öffnen Sie die [Releases-Seite](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases) des Projekts und wählen Sie die neueste Version.

2\. Laden Sie unter **Assets** die **Distributions-ZIP von Department of Efficiency** herunter.

3\. **Schließen Sie SimCity 4.** Entpacken Sie die ZIP in einen temporären Ordner.

4\. Kopieren Sie **`DepartmentEfficiency.dll`** und den gesamten Ordner **`Department of Efficiency`** in das Stammverzeichnis von `Dokumente\SimCity 4\Plugins`.

5\. Starten Sie das Spiel und folgen Sie der [Ersteinrichtung](#first-time-setup), bevor Sie eine gespeicherte Stadt spielen.

Die Distributions-ZIP enthält die DLL und einen Ordner mit fünf DAT- und drei INI-Dateien. Nach der Installation sollten Sie Folgendes haben:

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

Die Markdown-Dateien und Screenshots des öffentlichen Repositorys werden vom Spiel **nicht** benötigt. Legen Sie die DLL **direkt in Plugins** ab, den zugehörigen Ordner daneben.

**Einstellungen später ändern:** Öffnen Sie **Spieloptionen → Ministerium für Effizienz**, nutzen Sie die Seiten **Versorgung** und **Abriss** und klicken Sie auf **Übernehmen**. Als Hauptschalter können Sie auch die Verordnung **Ministerium für Effizienz einrichten** ohne monatliche Kosten verwenden. Das Ausschalten des Ministeriums stoppt künftige Aktionen; bereits angewendete Finanzierungen, Verträge und Abrisse werden nicht rückgängig gemacht.

<a id="features"></a>

## Was das Ministerium tut

Die folgende Tabelle beschreibt, was das Ministerium tut. Formeln, Beispiele, Schutzmechanismen und die Mechanik hinter den Entscheidungen finden Sie in der **[ausführlichen Funktionsreferenz (auf Englisch)](technical_docs/en/index.md)**.

| Bereich | Was es tut |
|---|---|
| [Bildung und Gesundheit](technical_docs/en/education_healthcare.md#education-healthcare) | Passt die Finanzierung **Gebäude für Gebäude** für geeignete Schulen, Hochschulen, Universitäten, Bibliotheken, Museen, Kliniken und Krankenhäuser an. Es richtet sich nach tatsächlicher Auslastung und Kapazität, mit einer Sicherheitsreserve. Es **ändert keine Einzugsbereiche** und nicht die Finanzierung von Schulbussen und Krankenwagen. Ist ein Gebäude trotz maximaler Finanzierung überfüllt, weist ein Berater darauf hin, dass eine weitere Einrichtung nötig ist; **wo sie gebaut wird, entscheiden Sie**. |
| [Strom](technical_docs/en/power.md#power) | Verwaltet **jedes Kraftwerk einzeln** und wählt die Finanzierung, die den Bedarf der Stadt und die Sicherheitsreserve zu den niedrigsten modellierten Kosten deckt. Es berücksichtigt Wirkungsgrad und Zustand der Kraftwerke; eine optionale Einstellung bezieht auch die wirtschaftlichen Kosten der Kraftwerksalterung ein. |
| [Wasser](technical_docs/en/water.md#water) | Passt die Wasserfinanzierung der Stadt an den Bedarf plus eine Sicherheitsreserve an. Der Schutz der Wasserleitungen ist standardmäßig eingeschaltet, um Kürzungen zu vermeiden, bei denen das Netz verfallen würde. Es blendet außerdem das irreführende „Kein Wasser“-Symbol des Spiels über Bauernhöfen aus. |
| [Müllentsorgung](technical_docs/en/garbage.md#garbage-landfills) | Passt die Finanzierung der Müllentsorgung an Müllaufkommen, Recycling, Behandlung, Deponiespeicher und regionale Verträge an und schützt dabei den eigenen Entsorgungsbedarf der Stadt. |
| [Nachbarschaftsverträge](technical_docs/en/deals.md#neighbor-deals) | Kann **Strom und Wasser kaufen oder verkaufen**, **Nachbarn dafür bezahlen, Ihren Müll zu übernehmen**, oder **deren Müll gegen Bezahlung annehmen**. Es schließt, ändert und kündigt geeignete Verträge, wenn das Gesamtergebnis der Stadt nützt und der eigene Bedarf und die Reserven geschützt bleiben. |
| [Automatischer Abriss](technical_docs/en/demolition.md#demolition) | Kann standardmäßig Ruinen nach etwa **30 simulierten Tagen** räumen und geeignete Gebäude, die **ein simuliertes Jahr** lang verlassen waren. Sie legen fest, welche Gebiete und welche Verlassensgründe zählen, ob geschützte Ruinen geräumt werden dürfen und ob ein monatliches Ausgabenlimit gelten soll. |
| [Hinweise der Berater](technical_docs/en/safety.md#notices) | Nutzt die Berater des Spiels, um nachgewiesene, anhaltende Engpässe zu melden, die weder Finanzierung noch verfügbare Verträge beheben können. Wo es passt, enthalten die Meldungen Links zu den betroffenen Gebäuden. |
| [Hinweis zur Deponiegröße](technical_docs/en/garbage.md#garbage-landfills-advice) | Der optionale Hinweis nennt eine konkrete Deponieerweiterung oder eine sichere Verkleinerung, die die Müllkosten um mehr als Ihre gewählte Sparschwelle senken könnte. **Das Ministerium rät; ob Sie die Zonierung ändern, entscheiden Sie.** |
| [Reparatur von Geisterfeldern](technical_docs/en/garbage.md#ghost-tiles) | Bei eingeschalteter Müllverwaltung werden geeignete unsichtbare Deponiefelder, die das Spiel weiterhin zur Instandhaltung rechnet, sicher repariert. Unsichere oder ungeprüfte Fälle bleiben unberührt; für diese Reparatur gibt es keinen eigenen Schalter. |

Bau, Zonierung, Steuern, Straßen und Verkehr, Polizei, Feuerwehr und alle anderen Dienste, die das Ministerium nicht verwaltet, bleiben in Ihrer Hand. Schalten Sie die Verwaltung einer Vertragsart mit Nachbarn ein, kann das Ministerium auch einen Vertrag ändern oder kündigen, den Sie zuvor von Hand geschlossen haben. Schalten Sie diese Vertragsoption aus, um solche Verträge selbst zu verwalten.

<a id="first-time-setup"></a>

## Ersteinrichtung

Das Ministerium verwendet die mit dem Plugin gelieferten Einstellungen und **beginnt automatisch zu arbeiten**. Sie müssen es nicht einschalten, sollten aber prüfen, was es tun darf, bevor Sie eine wichtige gespeicherte Stadt spielen.

1\. Starten Sie SimCity 4 und öffnen Sie in der **Regionsansicht** **Spieloptionen → Ministerium für Effizienz**. Wenn Sie möchten, probieren Sie es zuerst in einer neuen oder entbehrlichen Teststadt aus.

2\. Wählen Sie auf der Seite **Versorgung**, welche Budgets und Nachbarschaftsverträge das Ministerium verwalten darf und wie viel freie Kapazität es vorhalten soll.

3\. Prüfen Sie auf der Seite **Abriss** die Regeln zum Räumen von Ruinen und verlassenen Gebäuden. **Schalten Sie den automatischen Abriss aus, wenn Sie alles selbst abreißen möchten.**

4\. Klicken Sie auf **Übernehmen**, um Ihre Auswahl zu speichern. **Abbrechen** schließt das Fenster, ohne Änderungen zu speichern.

5\. Öffnen Sie die Stadt, die Sie spielen möchten. Das Ministerium folgt Ihren gespeicherten Einstellungen und verwaltet die Stadt, während die Simulation läuft.

Ihre Einstellungen liegen in den INI-Dateien des Plugins und werden beim Öffnen einer anderen Stadt wiederverwendet. Das Ministerium baut **keine** Schulen, Krankenhäuser, Kraftwerke oder Wasseranlagen für Sie. Wenn Finanzierung und verfügbare Verträge einen nachgewiesenen, anhaltenden Engpass nicht beheben können, sagt Ihnen ein Berater im Spiel, wo mehr Kapazität nötig ist.

Sie können das Ministerium auch über die Verordnungen der Stadt ein- oder ausschalten.

<a id="settings-explained"></a>

## Einstellungen

Öffnen Sie **Spieloptionen → Ministerium für Effizienz**. Das Fenster hat zwei Seiten, **Versorgung** und **Abriss**. Die folgenden Tabellen behandeln **jede im Menü angezeigte Einstellung**, ohne die technischen Formeln. Klicken Sie auf den Namen einer Option, um die vollständige Erklärung in der [Funktionsreferenz](technical_docs/en/options.md#options) zu lesen.

**So funktionieren die Schalter:** „An“ bedeutet, dass das Ministerium diese Funktion verwalten darf. Eine Einstellung, die von einer anderen abhängt, ist ausgegraut, solange die übergeordnete ausgeschaltet ist. Prozenteinstellungen steuern Sicherheitsreserven und Schwellenwerte; sie helfen dem Ministerium, freie Kapazität vorzuhalten und Schwankungen der Nachfrage aufzufangen.

### Versorgung

| Option | Standard | Bedeutung |
|---|---|---|
| [Ministerium aktiv](technical_docs/en/options.md#opt-master) | **An** | Hauptschalter für Finanzierung, Nachbarschaftsverträge, Abriss und die Hinweise des Ministeriums. Ausschalten macht bereits vorgenommene Änderungen nicht rückgängig. |
| [Bildung verwalten](technical_docs/en/options.md#opt-education) | **An** | Passt geeignete Schulen, Universitäten, Bibliotheken und Museen automatisch einzeln an. Ändert weder deren Abdeckung noch die Schulbus-Finanzierung. |
| [Sicherheitsreserve (Bildung)](technical_docs/en/options.md#opt-educationreserve) | **5%** (0–50%) | Zusätzliche Kapazität über der aktuellen Auslastung der Bildung. Mehr Kapazität kostet mehr. Bei schwach ausgelasteten Gebäuden kann das Ministerium eine zusätzliche Reserve vorhalten. So kann eine Schule beispielsweise von 10 auf 20 Schüler (+100%) wachsen, ohne dass gestreikt wird, besonders zu Beginn, wenn die Mehrkosten klein sind. |
| [Gesundheit verwalten](technical_docs/en/options.md#opt-health) | **An** | Passt geeignete Kliniken, Krankenhäuser und andere unterstützte Gesundheitseinrichtungen automatisch einzeln an. Ändert weder die Krankenwagen-Finanzierung noch die Abdeckung. |
| [Sicherheitsreserve (Gesundheit)](technical_docs/en/options.md#opt-healthreserve) | **5%** (0–50%) | Funktioniert wie die Sicherheitsreserve der Bildung. |
| [Strom verwalten](technical_docs/en/options.md#opt-power) | **An** | Passt die Finanzierung einzelner Kraftwerke an, damit zu den niedrigsten modellierten Kosten genug Strom bereitsteht. |
| [Stromreserve](technical_docs/en/options.md#opt-powerreserve) | **20%** (10–50%) | Freie Erzeugungskapazität für den Teil des Bedarfs, der vor Ort gedeckt wird; höhere Reserven bieten mehr Spielraum für schnelles Wachstum und senken das Ausfallrisiko, können aber mehr kosten. |
| [Alterung der Kraftwerke berücksichtigen](technical_docs/en/options.md#opt-poweraging) | **Aus** | Bezieht die geschätzten Kosten für Verschleiß und Alterung der Kraftwerke in die Optimierung ein. Das kann eine höhere Finanzierung begünstigen, wenn der langfristige Nutzen für die Erhaltung des Kraftwerks die zusätzlichen Betriebskosten überwiegt. |
| [Wasser verwalten](technical_docs/en/options.md#opt-water) | **An** | Passt die Finanzierung der Wasserwerke an, um die Stadt ohne unnötige Ausgaben zu versorgen. |
| [Wasserreserve](technical_docs/en/options.md#opt-waterreserve) | **10%** (10–50%) | Funktioniert wie die Stromreserve. |
| [Wasserleitungen schützen](technical_docs/en/options.md#opt-waterpipes) | **An** | Hält die Finanzierung bei mindestens den 50% des Spiels, um Schäden am Leitungsnetz zu vermeiden. |
| [Müllentsorgung verwalten](technical_docs/en/options.md#opt-sanitation) | **An** | Passt die Finanzierung der Müllabfuhr an, damit Einsammeln und Entsorgung abgedeckt sind. Repariert außerdem Geisterfelder (ein Fehler des Spiels, der Instandhaltungskosten für nicht mehr ausgewiesene Deponiefelder berechnet). |
| [Reserve der Müllentsorgung](technical_docs/en/options.md#opt-sanitationreserve) | **2%** (2–20%) | Zusätzliche Behandlungskapazität für den Müll der Stadt, über das hinaus, was sie nach dem Recycling produziert, und über einen vorhandenen Rückstau hinaus. |
| [Warnen, wenn eine andere Deponiegröße Geld spart](technical_docs/en/options.md#opt-landfilladvice) | **Aus** | Schaltet den optionalen Hinweis auf Änderungen an der Deponiezonierung ein, wenn das Geld spart und der Müll der Stadt weiterhin bewältigt wird. Erfordert die Verwaltung der Müllentsorgung. |
| [Sparschwelle für Deponiehinweise](technical_docs/en/options.md#opt-landfillthreshold) | **25%** (20–50%) | Bittet Sie nur dann, eine andere Deponiegröße zu erwägen, wenn die modellierte monatliche Ersparnis mindestens diesen Anteil des Müllbudgets beträgt. |
| [Nachbarschaftsverträge verwalten](technical_docs/en/options.md#opt-deals) | **An** | Schaltet automatische regionale Verträge ein; die drei einzelnen Schalter unten bestimmen, welche Ressourcen gehandelt werden dürfen. |
| [Bei einer Stadt kaufen, an eine andere verkaufen](technical_docs/en/options.md#opt-arbitrage) | **Aus** | Erlaubt gewinnbringende Kauf-und-Verkauf-Kombinationen derselben Ressource zwischen Nachbarn, wobei der Bedarf der eigenen Stadt Vorrang hat. |
| [Stromverträge verwalten](technical_docs/en/options.md#opt-tradepower) | **An** | Erlaubt dem Ministerium, **Stromimporte und -exporte** zu schließen, zu ändern oder zu kündigen. Unabhängig von der Kraftwerksfinanzierung. |
| [Wasserverträge verwalten](technical_docs/en/options.md#opt-tradewater) | **An** | Erlaubt dem Ministerium, **Wasserimporte und -exporte** zu schließen, zu ändern oder zu kündigen. Unabhängig von der Wasserfinanzierung. |
| [Müllverträge verwalten](technical_docs/en/options.md#opt-tradegarbage) | **An** | Erlaubt dem Ministerium, die Müllbehandlung **in beide Richtungen** zu vereinbaren. Unabhängig von der Finanzierung der Müllentsorgung. |

### Abriss

| Option | Standard | Bedeutung |
|---|---|---|
| [Zerstörte Gebäude abreißen](technical_docs/en/options.md#opt-demolishdestroyed) | **An** | Reißt Ruinen nach einer bestimmten Zeit ab. Räumt geschützte städtische oder historische Ruinen nicht automatisch, es sei denn, die Option weiter unten erlaubt es. |
| [Wartezeit (zerstörte Gebäude)](technical_docs/en/options.md#opt-destroyedmonths) | **1 Monat** (1–24) | Wie lange Ruinen stehen bleiben müssen, bevor sie geräumt werden dürfen; standardmäßig etwa **30 simulierte Tage**. |
| [Zerstörte städtische und historische Gebäude abreißen](technical_docs/en/options.md#opt-demolishmunicipal) | **Aus** | Wenn eingeschaltet, werden nach der Wartezeit auch geeignete **Ruinen** städtischer, besonderer und historischer Gebäude geräumt. |
| [Verlassene Gebäude abreißen](technical_docs/en/options.md#opt-demolishabandoned) | **An** | Räumt geeignete verlassene Gebäude nach der Wartezeit, in den gewählten Gebietsarten und aus den gewählten Gründen. |
| [Wartezeit (verlassene Gebäude)](technical_docs/en/options.md#opt-abandonedmonths) | **12 Monate** (1–60) | Wie lange ein Gebäude verlassen sein muss, bevor es abgerissen werden darf; standardmäßig ein simuliertes Jahr. |
| [Wohngebiet](technical_docs/en/options.md#opt-zoneresidential) | **An** | Umfasst verlassene Häuser und Mehrfamilienhäuser. |
| [Gewerbegebiet](technical_docs/en/options.md#opt-zonecommercial) | **An** | Umfasst verlassene Läden und Büros. |
| [Industriegebiet](technical_docs/en/options.md#opt-zoneindustrial) | **An** | Umfasst verlassene Fabriken und andere Industriegebäude. |
| [Landwirtschaftsgebiet](technical_docs/en/options.md#opt-zoneagriculture) | **Aus** | Umfasst verlassene Bauernhöfe. Standardmäßig aus, weil leere oder verlassene Felder auf einem Bauernhof normal sein können. |
| [Grund: unattraktive Umgebung](technical_docs/en/options.md#opt-reasonlowdesirability) | **An** | Umfasst Verlassen wegen geringer Attraktivität, etwa durch Verschmutzung oder fehlende Dienste. |
| [Grund: zu langer Arbeitsweg](technical_docs/en/options.md#opt-reasoncommute) | **An** | Umfasst Verlassen wegen übermäßig langer Wege zur Arbeit. |
| [Grund: keine Nachfrage](technical_docs/en/options.md#opt-reasonlowdemand) | **An** | Umfasst Gebäude, die verlassen wurden, weil die Nachfrage nach diesem Typ verschwunden ist. |
| [Grund: kein Strom](technical_docs/en/options.md#opt-reasonnopower) | **Aus** | Umfasst optional Gebäude, die wegen fehlenden Stroms verlassen wurden. Meist ist es besser, zuerst den Strom wiederherzustellen und zu sehen, ob das Gebäude wieder bezogen wird. |
| [Grund: kein Wasser](technical_docs/en/options.md#opt-reasonnowater) | **Aus** | Umfasst optional Gebäude, die wegen fehlenden Wassers verlassen wurden. Meist ist es besser, zuerst das Wasser wiederherzustellen und zu sehen, ob das Gebäude wieder bezogen wird. |
| [Monatliches Abrissbudget begrenzen](technical_docs/en/options.md#opt-budgetlimit) | **Aus** | Legt eine optionale Ausgabengrenze für den automatischen Abriss fest, zusätzlich zu den verfügbaren Mitteln der Stadtkasse. |
| [Monatliches Budget](technical_docs/en/options.md#opt-demolitionbudget) | **§1.000** (§0–§200.000) | Höchste Abrissausgabe pro simuliertem Monat, **wenn die Budgetgrenze eingeschaltet ist**. |

<a id="screenshots"></a>

## Screenshots

Alle Screenshots zeigen die englische Version des Spiels.

| | |
|---|---|
| ![Die Seite Versorgung der Einstellungen des Ministeriums](screenshots/Options-1.png) | ![Die Seite Abriss der Einstellungen des Ministeriums](screenshots/Options-2.png) |
| *Die Seite Versorgung: jeder Dienst, seine Reserve und die Nachbarschaftsverträge.* | *Die Seite Abriss: Ruinen, verlassene Gebäude und das monatliche Budget.* |
| ![Ein Beraterhinweis zu überfüllten Oberschulen](screenshots/high_school.png) | ![Ein Beraterhinweis zu überfüllten Museen](screenshots/museum.png) |
| *Schulen bei maximaler Finanzierung und trotzdem überfüllt: Der Hinweis verlinkt die Gebäude.* | *Dasselbe für Museen.* |
| ![Ein Beraterhinweis zu einem Wassermangel](screenshots/water.png) | ![Ein Beraterhinweis zu einem Mangel an Müllkapazität](screenshots/real-garbage-paused.png) |
| *Wasser: Das Ministerium kann den Engpass mit der verfügbaren Versorgung nicht beheben.* | *Müll: Die Stadt braucht mehr Entsorgungskapazität.* |

<a id="requirements-and-compatibility"></a>

## Voraussetzungen und Kompatibilität

- **SimCity 4 Deluxe, Steam-Version, Spielversion 1.1.641.0, unter Windows.** Das ist die unterstützte Version; auf anderen Versionen bleibt das Ministerium inaktiv.

- **Kein weiterer Mod, Installer oder keine externe Anwendung ist nötig.**

- **Getestet und kompatibel mit diesen Mods** (getestete Versionen):

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

- **Empfohlen:** Fügen Sie `-CPUCount:1` unter **Steam → Bibliothek → SimCity 4 Deluxe → Eigenschaften → Startoptionen** hinzu. Das hat auf dem Rechner des Autors die Stabilität in langen Sitzungen verbessert.

- **Zehn Sprachen im Spiel:** Englisch, Portugiesisch, Französisch, Deutsch, Spanisch, Italienisch, Niederländisch, Dänisch, Schwedisch und Polnisch. Das Ministerium folgt automatisch der Sprache Ihres Spiels.

<a id="updating-and-uninstalling"></a>

## Aktualisieren und Deinstallieren

**Behalten Sie Ihre Einstellungen.** Die drei INI-Dateien im Ordner `Department of Efficiency` speichern Ihre Einstellungen. Sichern Sie vor dem Austausch des Mods `DepartmentEfficiency.ini`, `DepartmentEfficiencyUtility.ini` und `DepartmentEfficiencyDemolition.ini` außerhalb Ihres Plugins-Ordners.

**So ersetzen Sie eine Installation:** Schließen Sie das Spiel, entfernen Sie die vorhandene `DepartmentEfficiency.dll` und den Ordner `Department of Efficiency`, installieren Sie das neue Paket und stellen Sie Ihre gesicherten INI-Dateien **nur dann** wieder her, wenn ihr Format mit der neuen Version kompatibel ist. Andernfalls stellen Sie Ihre Einstellungen im Spiel neu ein.

**So deinstallieren Sie:** Schließen Sie das Spiel und löschen Sie `DepartmentEfficiency.dll` und den Ordner `Department of Efficiency`. Ihre gespeicherten Städte bleiben ohne den Mod lesbar. Finanzierungsstufen, bestehende Verträge und bereits in einer Stadt gespeicherte Abrisse werden nicht automatisch rückgängig gemacht.

<a id="known-limitations"></a>

## Bekannte Einschränkungen

- **Kompatibilität:** Unterstützt wird nur die Windows-Steam-Version **1.1.641.0** von SimCity 4 Deluxe. Andere Spielversionen bleiben inaktiv.

- **Deponiehinweis:** Erfordert genau ein Deponiegebiet. In Städten mit mehreren Deponien kann das Ministerium weiterhin geeignete fehlerhafte „Geisterfelder“ reparieren, empfiehlt aber keine Größenänderungen.

<a id="source-code"></a>

## Quellcode

Der Quellcode des Plugins liegt im Ordner [`source/`](source/). Er wird mit CMake und Visual Studio zu einer 32-Bit-DLL (Win32) gebaut. Benötigt werden Visual Studio 2026 mit der Workload „Desktopentwicklung mit C++“ (andere Versionen sind ungetestet), CMake ab Version 3.20 und Git:

```bat
cmake -S . -B build -G "Visual Studio 18 2026" -A Win32
cmake --build build --config Release
```

Beim Bauen wird [gzcom-dll](https://github.com/nsgomez/gzcom-dll) in einer festen Revision (`779b669bf88112146d58fba6c7ed44f13ac6ce7d`) heruntergeladen. Um eine geänderte Kopie dieser Bibliothek zu verwenden oder ohne Internetverbindung zu bauen, fügen Sie dem ersten Befehl `-DFETCHCONTENT_SOURCE_DIR_GZCOM_DLL=<Ordner>` hinzu. Die DLL entsteht unter `build\Release\DepartmentEfficiency.dll`. Eine neu gebaute DLL ist nicht Byte für Byte identisch mit der veröffentlichten.

<a id="license"></a>

## Lizenz

- Der Originalcode von Department of Efficiency (die Dateien in [`source/`](source/)) stammt von **Sammu89** und wird unter der **GNU General Public License, Version 3 (nur)** (GPL-3.0-only) veröffentlicht. Der vollständige Text steht in [LICENSE](LICENSE). Sie dürfen ihn nach den Bedingungen der GPLv3 studieren, ändern und weitergeben: Urheberrechtsvermerke und Danksagungen bleiben erhalten, Änderungen werden kenntlich gemacht, und Ihre Version wird unter derselben Lizenz veröffentlicht. Bitte geben Sie geänderte Versionen nicht als offizielle Versionen des Autors aus.

- Die Dokumentation (diese README-Dateien und die Dateien in [`technical_docs/`](technical_docs/)) und die für dieses Projekt geschriebenen Texte (einschließlich der Texte in den drei INI-Dateien und der in `DepartmentEfficiency.dat` kompilierten) werden zusammen mit ihren Originalübersetzungen unter derselben Lizenz veröffentlicht, GPL-3.0-only.

- **gzcom-dll** fällt nicht unter diese Lizenz: Sie behält ihre eigene Lizenz, LGPL-2.1-or-later ([licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt)), und wird beim Bauen aus ihrem Originalrepository geladen.

- **SimCity 4** und seine Originalressourcen gehören Electronic Arts / Maxis. **Department of Efficiency** ist eine inoffizielle Fan-Mod, unabhängig von EA und nicht von EA unterstützt. Vier der Dateien in `release/Department of Efficiency/` enthalten Skripte und Schnittstellendefinitionen, die aus den Ressourcen des Spiels selbst abgeleitet sind, und die Screenshots zeigen das Spiel: Sie sind nicht Eigentum des Autors, fallen nicht unter die GPL, und alle Rechte daran verbleiben bei den jeweiligen Inhabern. Dasselbe gilt für jedes andere Material Dritter.

<a id="credits"></a>

## Danksagungen

**Autor**

- Erstellt von **Sammu89**.

**Verwendete Bibliothek**

- **gzcom-dll** von Nelson Gomez (simmaster07), mit Beiträgen von Nicholas Hayes (Null 45), Casper Van Gheluwe, memo und anderen (siehe das [Repository](https://github.com/nsgomez/gzcom-dll)), verwendet unter der Lizenz LGPL-2.1-or-later und statisch in die DLL gelinkt. Die Bibliothek liegt nicht bei: Beim Bauen wird die genaue Revision aus dem Originalrepository geladen (siehe [Quellcode](#source-code)); Urheberrechtsvermerke und Lizenztext stehen in [licenses/gzcom-dll.txt](licenses/gzcom-dll.txt) und [licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt). Sie dürfen die Bibliothek ändern und das Plugin mit Ihrer Version neu bauen.

**Konsultierte Techniken, Dokumentation und Beispiele** (daraus wurde kein Code kopiert; ihre Autoren sind keine Mitautoren von Department of Efficiency):

- Nicholas Hayes (0xC0000054 / Null 45), für Forschung, Dokumentation und offene Beispiele zu SimCity-4-DLLs: sc4-dll-utilities, sc4-custom-budget-departments, sc4-query-ui-hooks, sc4-lua-extensions, sc4-lua-extension-example. Das Optionsfenster, die Verordnung und die Hinweisdatei folgen Mechanismen aus seinen Projekten sc4-region-census, sc4-humane-society-ordinance und sc4-lua-extension-example (MIT-Lizenz); die Spielbefehle folgen sc4-auto-run-cheats und sc4-game-command-documentation.

- CorinaMarie, für die Dokumentation der Urgent-Advice-Technik von SimCity 4, der die Beraterhinweise des Mods folgen.

- Die SimCity-4-Modding-Community, für zwei Jahrzehnte Forschung, Werkzeuge und geteiltes Wissen, darunter die Dokumentation der Exemplar-Eigenschaften von SC4Devotion.

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
