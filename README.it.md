[English](README.md) | [Português](README.pt.md) | [Français](README.fr.md) | [Deutsch](README.de.md) | [Español](README.es.md) | **Italiano** | [Nederlands](README.nl.md) | [Dansk](README.da.md) | [Svenska](README.sv.md) | [Polski](README.pl.md)

# Ministero dell'Ottimizzazione

**Il Ministero dell'Ottimizzazione si occupa dei piccoli dettagli della gestione della città che fanno una grande differenza.**

Il Ministero dell'Ottimizzazione (*Department of Efficiency* in inglese) è un plugin per **SimCity 4 Deluxe** che automatizza i compiti di routine della gestione cittadina, così puoi concentrarti sul costruire, pianificare e far crescere la tua città.

**Tu costruisci la città. Il Ministero ottimizza il bilancio!**

Gestisce i bilanci dei singoli edifici di istruzione e sanità, le centrali elettriche, i finanziamenti di acqua e rifiuti, gli accordi con le città vicine e, a scelta, la demolizione di ruderi ed edifici abbandonati. Punta a soddisfare le esigenze della città senza spendere più del necessario, lasciando a te la posizione degli edifici, la copertura e le grandi decisioni di pianificazione.

Tutto si controlla dal gioco, in **Opzioni di gioco → Ministero dell'Ottimizzazione**. Scegli i servizi da automatizzare, regola i loro margini di sicurezza, configura la demolizione oppure disattiva l'intero Ministero. Puoi anche usare l'ordinanza **Istituzione del Ministero dell'Ottimizzazione** come interruttore generale. Non serve nessuna applicazione separata.

> [!IMPORTANT]
> **Il Ministero parte attivo, compresa la demolizione automatica degli edifici distrutti e abbandonati.** Controlla le sue impostazioni prima di aprire una città a cui tieni. Apporta modifiche reali nel gioco: finanziamenti, accordi e demolizioni verranno salvati se salvi la città. Non modifica direttamente i file di salvataggio.

## Indice

- [Download e installazione](#download-and-installation)

- [Cosa fa il Ministero](#features)

- [Configurazione iniziale](#first-time-setup)

- [Impostazioni](#settings-explained)

- [Schermate](#screenshots)

- [Requisiti e compatibilità](#requirements-and-compatibility)

- [Aggiornamento e disinstallazione](#updating-and-uninstalling)

- [Limiti noti](#known-limitations)

- [Crediti](#credits)

- [Materiali della licenza LGPL](#lgpl-licensing)

<a id="download-and-installation"></a>

## Download e installazione

### [⬇ Scarica Department of Efficiency 1.0.0](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases/download/v1.0.0/Departement_of_Efficiency_1.00.zip)

1\. Apri la [pagina delle release](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases) del progetto e scegli la versione più recente.

2\. In **Assets**, scarica lo **ZIP di distribuzione di Department of Efficiency**.

3\. **Chiudi SimCity 4.** Estrai lo ZIP in una cartella temporanea.

4\. Copia **`DepartmentEfficiency.dll`** e l'intera cartella **`Department of Efficiency`** nella radice di `Documenti\SimCity 4\Plugins`.

5\. Avvia il gioco e segui le istruzioni della [configurazione iniziale](#first-time-setup) prima di giocare una città salvata.

Lo ZIP di distribuzione contiene la DLL e una cartella con cinque file DAT e tre INI. Dopo l'installazione dovresti avere:

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

I file Markdown e le schermate del repository pubblico **non** servono al gioco. Tieni la DLL **direttamente in Plugins**, con la cartella che la accompagna accanto.

**Cambiare le impostazioni in seguito:** apri **Opzioni di gioco → Ministero dell'Ottimizzazione**, usa le pagine **Servizi** e **Demolizioni** e fai clic su **Accetta**. Per l'interruttore generale puoi anche usare l'ordinanza **Istituzione del Ministero dell'Ottimizzazione**, senza costi mensili. Disattivare il Ministero ferma le azioni future; non annulla finanziamenti, contratti e demolizioni già applicati.

<a id="features"></a>

## Cosa fa il Ministero

La tabella seguente descrive cosa fa il Ministero. Per formule, esempi, salvaguardie e i meccanismi dietro le sue decisioni, consulta il **[riferimento dettagliato delle funzioni (in inglese)](features/en.md)**.

| Ambito | Cosa fa |
|---|---|
| [Istruzione e sanità](features/en.md#education-healthcare) | Regola il finanziamento **edificio per edificio** di scuole, università, biblioteche, musei, cliniche e ospedali idonei. Segue la frequenza e la capacità reali, con un margine di sicurezza. **Non modifica i raggi di copertura** né il finanziamento di scuolabus e ambulanze. Se un edificio resta sovraffollato anche al finanziamento massimo, un consigliere avvisa che serve un'altra struttura; **sei tu a scegliere dove costruirla**. |
| [Elettricità](features/en.md#electricity) | Gestisce **ogni centrale elettrica singolarmente**, scegliendo il finanziamento che copre le esigenze della città e la riserva di sicurezza al minor costo modellato. Tiene conto del rendimento e dello stato delle centrali; un'impostazione facoltativa include anche il costo economico dell'usura. |
| [Acqua](features/en.md#water) | Regola il finanziamento dell'acqua della città per soddisfare la domanda più una riserva di sicurezza. La protezione delle condutture è attiva per impostazione predefinita, per evitare tagli che lascerebbero deteriorare la rete. Nasconde inoltre la fuorviante icona «niente acqua» del gioco sopra le fattorie. |
| [Smaltimento e rifiuti](features/en.md#garbage-landfills) | Regola il finanziamento dello smaltimento in base alla produzione di rifiuti, al riciclaggio, al trattamento, alla capienza delle discariche e ai contratti regionali, proteggendo le esigenze di smaltimento della città. |
| [Accordi con i vicini](features/en.md#neighbor-deals) | Può **comprare o vendere elettricità e acqua**, **pagare i vicini perché prendano i tuoi rifiuti** o **accettare i loro dietro compenso**. Crea, adegua e annulla i contratti idonei quando il risultato complessivo avvantaggia la città e le sue esigenze e riserve restano protette. |
| [Demolizione automatica](features/en.md#demolition) | Può sgomberare i ruderi dopo circa **30 giorni simulati** e gli edifici idonei abbandonati da **un anno simulato**, per impostazione predefinita. Sei tu a decidere quali zone e quali motivi di abbandono contano, se i ruderi protetti possono essere sgomberati e se fissare un limite di spesa mensile. |
| [Avvisi dei consiglieri](features/en.md#notices) | Usa i consiglieri del gioco per segnalare carenze accertate e persistenti che né il finanziamento né gli accordi disponibili possono risolvere. Quando serve, i messaggi contengono collegamenti che portano agli edifici interessati. |
| [Consiglio sulla dimensione della discarica](features/en.md#garbage-landfills-advice) | Un consiglio facoltativo indica un ampliamento preciso della discarica, o una riduzione sicura, che potrebbe tagliare la spesa per lo smaltimento oltre la soglia di risparmio scelta. **Il Ministero consiglia; sei tu a decidere se cambiare la zonizzazione.** |
| [Riparazione delle caselle fantasma](features/en.md#ghost-squares) | Con la gestione dello smaltimento attiva, ripara in sicurezza le caselle di discarica invisibili idonee che il gioco continua a contare nella manutenzione. I casi non sicuri o non verificati vengono lasciati stare; questa riparazione non ha un interruttore a parte. |

Continui a controllare costruzione, zonizzazione, tasse, strade e trasporti, polizia, vigili del fuoco e ogni altro servizio che il Ministero non gestisce. Se attivi la gestione di un tipo di accordo con i vicini, il Ministero può anche modificare o annullare un contratto che avevi stipulato a mano. Disattiva quell'opzione per gestire tu stesso quei contratti.

<a id="first-time-setup"></a>

## Configurazione iniziale

Il Ministero usa le impostazioni fornite con il plugin e **inizia a lavorare automaticamente**. Non devi attivarlo, ma dovresti controllare cosa gli è permesso fare prima di usare una città salvata importante.

1\. Avvia SimCity 4 e, dalla **vista regione**, apri **Opzioni di gioco → Ministero dell'Ottimizzazione**. Se preferisci, usa una città nuova o di prova per un primo sguardo.

2\. Nella pagina **Servizi**, scegli quali bilanci e accordi con i vicini il Ministero può gestire e quanta capacità di scorta deve mantenere.

3\. Nella pagina **Demolizioni**, controlla le regole per sgomberare ruderi ed edifici abbandonati. **Disattiva la demolizione automatica se vuoi abbattere tutto da solo.**

4\. Fai clic su **Accetta** per salvare le scelte. **Annulla** chiude la finestra senza salvare le modifiche.

5\. Apri la città che vuoi giocare. Il Ministero segue le preferenze salvate e gestisce la città mentre la simulazione procede.

Le preferenze sono memorizzate nei file INI del plugin e riutilizzate quando apri un'altra città. Il Ministero **non** costruisce scuole, ospedali, centrali elettriche o impianti idrici al posto tuo. Quando il finanziamento e gli accordi disponibili non bastano a risolvere una carenza accertata e persistente, un consigliere del gioco ti indica dove serve più capacità.

Puoi anche attivare o disattivare il Ministero tramite le ordinanze della città.

<a id="settings-explained"></a>

## Impostazioni

Apri **Opzioni di gioco → Ministero dell'Ottimizzazione**. La finestra ha due pagine, **Servizi** e **Demolizioni**. Le tabelle seguenti coprono **tutte le impostazioni mostrate nel menu**, senza le formule tecniche. Fai clic sul nome di un'opzione per leggerne la spiegazione completa nel [riferimento delle funzioni](features/en.md#options).

**Come funzionano i controlli:** «Attivo» significa che il Ministero può gestire quella funzione. Un'impostazione che dipende da un'altra è in grigio finché quella principale è disattivata. Le impostazioni in percentuale controllano margini di sicurezza e soglie, aiutando il Ministero a mantenere capacità di riserva e ad assorbire le oscillazioni della domanda.

### Servizi

| Opzione | Predefinito | Significato |
|---|---|---|
| [Ministero attivo](features/en.md#opt-master) | **Attivo** | Interruttore generale di finanziamenti, contratti con i vicini, demolizione e avvisi del Ministero. Disattivarlo non annulla le modifiche già fatte. |
| [Gestisci l'istruzione](features/en.md#opt-education) | **Attivo** | Regola automaticamente, una per una, scuole, università, biblioteche e musei idonei. Non modifica la loro copertura né il finanziamento degli scuolabus. |
| [Margine di sicurezza (istruzione)](features/en.md#opt-educationreserve) | **5%** (0–50%) | Capacità in più rispetto all'utilizzo attuale dell'istruzione. Più capacità costa di più. Per gli edifici poco frequentati, il Ministero può mantenere un margine aggiuntivo. Ad esempio, una scuola potrebbe così passare da 10 a 20 studenti (+100%) senza scioperi, soprattutto all'inizio, quando il costo extra è piccolo. |
| [Gestisci la sanità](features/en.md#opt-health) | **Attivo** | Regola automaticamente, uno per uno, cliniche, ospedali e altre strutture sanitarie idonee e supportate. Non modifica il finanziamento delle ambulanze né la copertura. |
| [Margine di sicurezza (sanità)](features/en.md#opt-healthreserve) | **5%** (0–50%) | Funziona come il margine di sicurezza dell'istruzione. |
| [Gestisci l'elettricità](features/en.md#opt-power) | **Attivo** | Regola il finanziamento delle singole centrali elettriche per fornire elettricità sufficiente al minor costo modellato. |
| [Riserva elettrica](features/en.md#opt-powerreserve) | **20%** (10–50%) | Capacità di generazione di scorta per la parte della domanda soddisfatta localmente; riserve maggiori danno più margine per seguire una crescita rapida e riducono il rischio di blackout, ma possono costare di più. |
| [Considera l'usura delle centrali](features/en.md#opt-poweraging) | **Disattivo** | Include nell'ottimizzazione il costo stimato dell'usura e dell'invecchiamento delle centrali. Può favorire un finanziamento più alto quando il beneficio a lungo termine della conservazione della centrale supera il costo di esercizio aggiuntivo. |
| [Gestisci l'acqua](features/en.md#opt-water) | **Attivo** | Regola il finanziamento del reparto idrico per rifornire la città senza spese inutili. |
| [Riserva idrica](features/en.md#opt-waterreserve) | **10%** (10–50%) | Funziona come la riserva elettrica. |
| [Proteggi le condutture idriche](features/en.md#opt-waterpipes) | **Attivo** | Mantiene il finanziamento almeno al 50% del gioco, per evitare danni alla rete di condutture. |
| [Gestisci lo smaltimento rifiuti](features/en.md#opt-sanitation) | **Attivo** | Regola il finanziamento del reparto rifiuti per coprire le esigenze di raccolta e smaltimento. Ripara anche le caselle fantasma (un difetto del gioco che addebita la manutenzione di caselle di discarica non più in zona). |
| [Riserva di smaltimento](features/en.md#opt-sanitationreserve) | **2%** (2–20%) | Capacità di trattamento in più per i rifiuti della città, oltre a quanto produce dopo il riciclaggio e a ogni arretrato accumulato. |
| [Avvisami se rivedere la discarica può farmi risparmiare](features/en.md#opt-landfilladvice) | **Disattivo** | Attiva il consiglio facoltativo sulle modifiche alla zonizzazione della discarica quando fanno risparmiare e i rifiuti della città restano comunque gestiti. Richiede la gestione dello smaltimento. |
| [Soglia di risparmio per la discarica](features/en.md#opt-landfillthreshold) | **25%** (20–50%) | Ti chiede di valutare una modifica alla dimensione della discarica solo se il risparmio mensile modellato raggiunge almeno questa quota del bilancio dello smaltimento. |
| [Gestisci gli accordi con i vicini](features/en.md#opt-deals) | **Attivo** | Attiva i contratti regionali automatici; i tre interruttori singoli qui sotto decidono quali risorse si possono scambiare. |
| [Acquista da una città e vendi a un'altra](features/en.md#opt-arbitrage) | **Disattivo** | Consente combinazioni redditizie di acquisto e vendita della stessa risorsa tra vicini, dando comunque la precedenza alle esigenze della città. |
| [Gestisci gli accordi per l'elettricità](features/en.md#opt-tradepower) | **Attivo** | Consente al Ministero di creare, modificare o annullare **importazioni ed esportazioni** di elettricità. Indipendente dal finanziamento delle centrali. |
| [Gestisci gli accordi per l'acqua](features/en.md#opt-tradewater) | **Attivo** | Consente al Ministero di creare, modificare o annullare **importazioni ed esportazioni** di acqua. Indipendente dal finanziamento dell'acqua. |
| [Gestisci gli accordi per i rifiuti](features/en.md#opt-tradegarbage) | **Attivo** | Consente al Ministero di concordare il trattamento dei rifiuti **in entrambe le direzioni**. Indipendente dal finanziamento dello smaltimento. |

### Demolizioni

| Opzione | Predefinito | Significato |
|---|---|---|
| [Demolisci gli edifici distrutti](features/en.md#opt-demolishdestroyed) | **Attivo** | Demolisce i ruderi dopo un certo periodo. Non sgombera automaticamente i ruderi comunali o storici protetti, a meno che l'opzione qui sotto lo consenta. |
| [Attesa (edifici distrutti)](features/en.md#opt-destroyedmonths) | **1 mese** (1–24) | Per quanto tempo i ruderi devono restare prima di poter essere sgomberati; circa **30 giorni simulati** per impostazione predefinita. |
| [Demolisci gli edifici comunali e storici distrutti](features/en.md#opt-demolishmunicipal) | **Disattivo** | Se attiva, sgombera anche, dopo l'attesa, i **ruderi** idonei di edifici comunali, speciali e storici. |
| [Demolisci gli edifici abbandonati](features/en.md#opt-demolishabandoned) | **Attivo** | Sgombera gli edifici abbandonati idonei dopo il periodo di attesa, nei tipi di zona e per i motivi selezionati. |
| [Attesa (edifici abbandonati)](features/en.md#opt-abandonedmonths) | **12 mesi** (1–60) | Per quanto tempo un edificio deve restare abbandonato prima di poter essere demolito; un anno simulato per impostazione predefinita. |
| [Zona residenziale](features/en.md#opt-zoneresidential) | **Attivo** | Include case e condomini abbandonati. |
| [Zona commerciale](features/en.md#opt-zonecommercial) | **Attivo** | Include negozi e uffici abbandonati. |
| [Zona industriale](features/en.md#opt-zoneindustrial) | **Attivo** | Include fabbriche e altri edifici industriali abbandonati. |
| [Zona agricola](features/en.md#opt-zoneagriculture) | **Disattivo** | Include le fattorie abbandonate. Disattivata per impostazione predefinita perché caselle vuote o abbandonate in una fattoria possono essere normali. |
| [Causa: zona poco attraente](features/en.md#opt-reasonlowdesirability) | **Attivo** | Include l'abbandono causato da scarsa attrattiva, come inquinamento o mancanza di servizi. |
| [Causa: tragitto troppo lungo](features/en.md#opt-reasoncommute) | **Attivo** | Include l'abbandono causato da tragitti eccessivamente lunghi verso il lavoro. |
| [Causa: assenza di domanda](features/en.md#opt-reasonlowdemand) | **Attivo** | Include gli edifici abbandonati perché la domanda per quel tipo è scomparsa. |
| [Causa: assenza di elettricità](features/en.md#opt-reasonnopower) | **Disattivo** | Include, a scelta, gli edifici abbandonati per mancanza di elettricità. Di solito conviene ripristinare prima l'elettricità e vedere se l'edificio torna a popolarsi. |
| [Causa: assenza di acqua](features/en.md#opt-reasonnowater) | **Disattivo** | Include, a scelta, gli edifici abbandonati per mancanza di acqua. Di solito conviene ripristinare prima l'acqua e vedere se l'edificio torna a popolarsi. |
| [Limita la spesa mensile per le demolizioni](features/en.md#opt-budgetlimit) | **Disattivo** | Fissa un tetto di spesa facoltativo per la demolizione automatica, oltre ai fondi disponibili nella cassa della città. |
| [Bilancio mensile](features/en.md#opt-demolitionbudget) | **§1.000** (§0–§200.000) | Spesa massima per le demolizioni per mese simulato **quando il limite di bilancio è attivo**. |

<a id="screenshots"></a>

## Schermate

Tutte le schermate mostrano la versione inglese del gioco.

| | |
|---|---|
| ![La pagina Servizi delle impostazioni del Ministero](screenshots/Options-1.png) | ![La pagina Demolizioni delle impostazioni del Ministero](screenshots/Options-2.png) |
| *La pagina Servizi: ogni servizio, la sua riserva e gli accordi con i vicini.* | *La pagina Demolizioni: ruderi, edifici abbandonati e bilancio mensile.* |
| ![Un avviso del consigliere su licei sovraffollati](screenshots/high_school.png) | ![Un avviso del consigliere su musei sovraffollati](screenshots/museum.png) |
| *Scuole al finanziamento massimo e comunque sovraffollate: l'avviso rimanda agli edifici.* | *Lo stesso per i musei.* |
| ![Un avviso del consigliere su una carenza d'acqua](screenshots/water.png) | ![Un avviso del consigliere su una carenza di capacità per i rifiuti](screenshots/real-garbage-paused.png) |
| *Acqua: il Ministero non riesce a risolvere la carenza con l'approvvigionamento disponibile.* | *Rifiuti: la città ha bisogno di più capacità di smaltimento.* |

<a id="requirements-and-compatibility"></a>

## Requisiti e compatibilità

- **SimCity 4 Deluxe, edizione Steam, versione del gioco 1.1.641.0, su Windows.** È la versione supportata; il Ministero resta inattivo sulle altre versioni.

- **Non servono altri mod, installer o applicazioni esterne.**

- **Testato e compatibile con questi mod** (versioni testate):

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

- **Consigliato:** aggiungi `-CPUCount:1` in **Steam → Libreria → SimCity 4 Deluxe → Proprietà → Opzioni di avvio**. Ha migliorato la stabilità nelle sessioni lunghe sul computer dell'autore.

- **Dieci lingue nel gioco:** inglese, portoghese, francese, tedesco, spagnolo, italiano, olandese, danese, svedese e polacco. Il Ministero segue automaticamente la lingua del tuo gioco.

<a id="updating-and-uninstalling"></a>

## Aggiornamento e disinstallazione

**Conserva le tue preferenze.** I tre file INI nella cartella `Department of Efficiency` contengono le tue impostazioni. Prima di sostituire il mod, copia `DepartmentEfficiency.ini`, `DepartmentEfficiencyUtility.ini` e `DepartmentEfficiencyDemolition.ini` fuori dalla cartella Plugins.

**Per sostituire un'installazione:** chiudi il gioco, rimuovi la `DepartmentEfficiency.dll` e la cartella `Department of Efficiency` esistenti, installa il nuovo pacchetto e ripristina i file INI copiati **solo se il loro formato è compatibile** con la nuova versione. Altrimenti, reimposta le preferenze nel gioco.

**Per disinstallare:** chiudi il gioco ed elimina `DepartmentEfficiency.dll` e la cartella `Department of Efficiency`. Le tue città salvate restano leggibili senza il mod. I livelli di finanziamento, i contratti esistenti e le demolizioni già salvate in una città non vengono annullati automaticamente.

<a id="known-limitations"></a>

## Limiti noti

- **Compatibilità:** è supportata solo la versione **1.1.641.0** di SimCity 4 Deluxe per Windows (Steam). Le altre versioni del gioco restano inattive.

- **Consiglio sulla discarica:** richiede esattamente una zona di discarica. Nelle città con più discariche, il Ministero può comunque riparare le «caselle fantasma» idonee, ma non raccomanda modifiche di dimensione.

<a id="credits"></a>

## Crediti

- Creato da **Sammu89**.

- Realizzato con **gzcom-dll** di Nelson Gomez (simmaster07), usata con licenza LGPL-2.1-or-later e collegata staticamente alla DLL.

- Nicholas Hayes (0xC0000054 / Null 45), per ricerca, documentazione ed esempi aperti sulle DLL di SimCity 4: sc4-dll-utilities, sc4-custom-budget-departments, sc4-query-ui-hooks, sc4-lua-extensions, sc4-lua-extension-example.

- CorinaMarie, per aver documentato la tecnica Urgent Advice di SimCity 4 che gli avvisi dei consiglieri del mod seguono.

- La comunità di modding di SimCity 4, per due decenni di ricerca, strumenti e conoscenze condivise.

<a id="lgpl-licensing"></a>

## Materiali della licenza LGPL

Il Ministero dell’Ottimizzazione utilizza [gzcom-dll](https://github.com/nsgomez/gzcom-dll), con licenza LGPL-2.1-or-later.

Il codice sorgente corrispondente della libreria, gli avvisi di licenza e i materiali necessari per il collegamento sono disponibili nel [pacchetto di materiali LGPL](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases/download/v1.0.0/DepartmentEfficiency-LGPL-Compliance-1.0.0.zip).

Questi materiali non sono necessari per installare o usare la mod.
