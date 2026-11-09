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

- [Compatibilità con il Phantom Funding Slider Bug](#phantom-funding-slider-bug)

- [Aggiornamento e disinstallazione](#updating-and-uninstalling)

- [Limiti noti](#known-limitations)

- [Codice sorgente](#source-code)

- [Licenza](#license)

- [Crediti](#credits)

<a id="download-and-installation"></a>

## Download e installazione

### [⬇ Scarica Department of Efficiency 1.0.2](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases/download/v1.0.2/Departement_of_Efficiency_1.02.zip)

**Novità della 1.0.2:** vedi il [registro delle modifiche](CHANGELOG.md) (in inglese).

1\. Apri la [pagina delle release](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases) del progetto e scegli la versione più recente.

2\. In **Assets**, scarica lo **ZIP di distribuzione di Department of Efficiency**.

3\. **Chiudi SimCity 4.** Estrai lo ZIP in una cartella temporanea.

4\. Copia i **tre file** **`DepartmentEfficiency.dll`**, **`DepartmentEfficiency.dat`** e **`DepartmentEfficiency.ini`** direttamente in `Documents\SimCity 4\Plugins`. Non creare nessuna sottocartella.

5\. Avvia il gioco e segui le istruzioni della [configurazione iniziale](#first-time-setup) prima di giocare una città salvata.

Lo ZIP di distribuzione contiene esattamente questi tre file. Dopo l'installazione dovresti avere:

```text
Documents\SimCity 4\Plugins\
    DepartmentEfficiency.dll
    DepartmentEfficiency.dat
    DepartmentEfficiency.ini
```

I file Markdown e gli screenshot del repository pubblico **non** servono al gioco. Tieni i tre file **direttamente in Plugins**, affiancati. La cartella `Department of Efficiency` non esiste più.

> [!WARNING]
> **Aggiorni da una versione precedente?** Non copiare i nuovi file sopra i vecchi. Elimina prima i vecchi file del Ministero, come spiegato in [Aggiornamento e disinstallazione](#updating-and-uninstalling).

**Cambiare le impostazioni in seguito:** apri **Opzioni di gioco → Ministero dell'Ottimizzazione**, usa le pagine **Servizi** e **Demolizioni** e fai clic su **Accetta**. Per l'interruttore generale puoi anche usare l'ordinanza **Istituzione del Ministero dell'Ottimizzazione**, senza costi mensili. Disattivare il Ministero ferma le azioni future; non annulla finanziamenti, contratti e demolizioni già applicati.

<a id="features"></a>

## Cosa fa il Ministero

La tabella seguente descrive cosa fa il Ministero. Per formule, esempi, salvaguardie e i meccanismi dietro le sue decisioni, consulta il **[riferimento dettagliato delle funzioni (in inglese)](technical_docs/en/index.md)**.

| Ambito | Cosa fa |
|---|---|
| [Istruzione e sanità](technical_docs/en/education_healthcare.md#education-healthcare) | Regola il finanziamento **edificio per edificio** di scuole, università, biblioteche, musei, cliniche e ospedali idonei. Segue la frequenza e la capacità reali, con un margine di sicurezza. **Non modifica i raggi di copertura** né il finanziamento di scuolabus e ambulanze. Se un edificio resta sovraffollato anche al finanziamento massimo, un consigliere avvisa che serve un'altra struttura; **sei tu a scegliere dove costruirla**. |
| [Elettricità](technical_docs/en/power.md#power) | Gestisce **ogni centrale elettrica singolarmente**, scegliendo il finanziamento che copre le esigenze della città e la riserva di sicurezza al minor costo modellato. Tiene conto del rendimento e dello stato delle centrali; un'impostazione facoltativa include anche il costo economico dell'usura. |
| [Acqua](technical_docs/en/water.md#water) | Regola il finanziamento dell'acqua della città per soddisfare la domanda più una riserva di sicurezza. La protezione delle condutture è attiva per impostazione predefinita, per evitare tagli che lascerebbero deteriorare la rete. Nasconde inoltre la fuorviante icona «niente acqua» del gioco sopra le fattorie. |
| [Smaltimento e rifiuti](technical_docs/en/garbage.md#garbage-landfills) | Regola il finanziamento dello smaltimento in base alla produzione di rifiuti, al riciclaggio, al trattamento, alla capienza delle discariche e ai contratti regionali, proteggendo le esigenze di smaltimento della città. |
| [Accordi con i vicini](technical_docs/en/deals.md#neighbor-deals) | Può **comprare o vendere elettricità e acqua**, **pagare i vicini perché prendano i tuoi rifiuti** o **accettare i loro dietro compenso**. Crea, adegua e annulla i contratti idonei quando il risultato complessivo avvantaggia la città e le sue esigenze e riserve restano protette. |
| [Demolizione automatica](technical_docs/en/demolition.md#demolition) | Può sgomberare i ruderi dopo circa **30 giorni simulati** e gli edifici idonei abbandonati da **un anno simulato**, per impostazione predefinita. Sei tu a decidere quali zone e quali motivi di abbandono contano, se i ruderi protetti possono essere sgomberati e se fissare un limite di spesa mensile. |
| [Avvisi dei consiglieri](technical_docs/en/safety.md#notices) | Usa i consiglieri del gioco per segnalare carenze accertate e persistenti che né il finanziamento né gli accordi disponibili possono risolvere. Quando serve, i messaggi contengono collegamenti che portano agli edifici interessati. Aggiunge anche una notizia (titolo nell'elenco, dettaglio al clic) quando un servizio che hai attivato deve fermarsi per un guasto tecnico. |
| [Consiglio sulla dimensione della discarica](technical_docs/en/garbage.md#garbage-landfills-advice) | Un consiglio facoltativo indica una riduzione precisa e sicura della discarica (caselle vuote da rimuovere), che potrebbe tagliare la spesa per lo smaltimento oltre la soglia di risparmio scelta. **Il Ministero consiglia; sei tu a decidere se cambiare la zonizzazione.** |
| [Riparazione delle caselle fantasma](technical_docs/en/garbage.md#ghost-tiles) | Con la gestione dello smaltimento attiva, ripara in sicurezza le caselle di discarica invisibili idonee che il gioco continua a contare nella manutenzione. I casi non sicuri o non verificati vengono lasciati stare; questa riparazione non ha un interruttore a parte. |

Continui a controllare costruzione, zonizzazione, tasse, strade e trasporti, polizia, vigili del fuoco e ogni altro servizio che il Ministero non gestisce. Se attivi la gestione di un tipo di accordo con i vicini, il Ministero può anche modificare o annullare un contratto che avevi stipulato a mano. Disattiva quell'opzione per gestire tu stesso quei contratti.

<a id="first-time-setup"></a>

## Configurazione iniziale

Il Ministero usa le impostazioni fornite con il plugin e **inizia a lavorare automaticamente**. Non devi attivarlo, ma dovresti controllare cosa gli è permesso fare prima di usare una città salvata importante.

1\. Avvia SimCity 4 e, dalla **vista regione**, apri **Opzioni di gioco → Ministero dell'Ottimizzazione**. Se preferisci, usa una città nuova o di prova per un primo sguardo.

2\. Nella pagina **Servizi**, scegli quali bilanci e accordi con i vicini il Ministero può gestire e quanta capacità di scorta deve mantenere.

3\. Nella pagina **Demolizioni**, controlla le regole per sgomberare ruderi ed edifici abbandonati. **Disattiva la demolizione automatica se vuoi abbattere tutto da solo.**

4\. Fai clic su **Accetta** per salvare le scelte. **Annulla** chiude la finestra senza salvare le modifiche.

5\. Apri la città che vuoi giocare. Il Ministero segue le preferenze salvate e gestisce la città mentre la simulazione procede.

Le preferenze sono memorizzate in `DepartmentEfficiency.ini` e riutilizzate quando apri un'altra città. Il Ministero **non** costruisce scuole, ospedali, centrali elettriche o impianti idrici al posto tuo. Quando il finanziamento e gli accordi disponibili non bastano a risolvere una carenza accertata e persistente, un consigliere del gioco ti indica dove serve più capacità.

Puoi anche attivare o disattivare il Ministero tramite le ordinanze della città.

<a id="settings-explained"></a>

## Impostazioni

Apri **Opzioni di gioco → Ministero dell'Ottimizzazione**. La finestra ha due pagine, **Servizi** e **Demolizioni**. Le tabelle seguenti coprono **tutte le impostazioni mostrate nel menu**, senza le formule tecniche. Fai clic sul nome di un'opzione per leggerne la spiegazione completa nel [riferimento delle funzioni](technical_docs/en/options.md#options).

**Come funzionano i controlli:** «Attivo» significa che il Ministero può gestire quella funzione. Un'impostazione che dipende da un'altra è in grigio finché quella principale è disattivata. Le impostazioni in percentuale controllano margini di sicurezza e soglie, aiutando il Ministero a mantenere capacità di riserva e ad assorbire le oscillazioni della domanda.

### Servizi

| Opzione | Predefinito | Significato |
|---|---|---|
| [Ministero attivo](technical_docs/en/options.md#opt-master) | **Attivo** | Interruttore generale di finanziamenti, contratti con i vicini, demolizione e avvisi del Ministero. Disattivarlo non annulla le modifiche già fatte. |
| [Gestisci l'istruzione](technical_docs/en/options.md#opt-education) | **Attivo** | Regola automaticamente, una per una, scuole, università, biblioteche e musei idonei. Non modifica la loro copertura né il finanziamento degli scuolabus. |
| [Margine di sicurezza (istruzione)](technical_docs/en/options.md#opt-educationreserve) | **5%** (0–50%) | Capacità in più rispetto all'utilizzo attuale dell'istruzione. Più capacità costa di più. Per gli edifici poco frequentati, il Ministero può mantenere un margine aggiuntivo. Ad esempio, una scuola potrebbe così passare da 10 a 20 studenti (+100%) senza scioperi, soprattutto all'inizio, quando il costo extra è piccolo. |
| [Gestisci la sanità](technical_docs/en/options.md#opt-health) | **Attivo** | Regola automaticamente, uno per uno, cliniche, ospedali e altre strutture sanitarie idonee e supportate. Non modifica il finanziamento delle ambulanze né la copertura. |
| [Margine di sicurezza (sanità)](technical_docs/en/options.md#opt-healthreserve) | **5%** (0–50%) | Funziona come il margine di sicurezza dell'istruzione. |
| [Gestisci l'elettricità](technical_docs/en/options.md#opt-power) | **Attivo** | Regola il finanziamento delle singole centrali elettriche per fornire elettricità sufficiente al minor costo modellato. |
| [Riserva elettrica](technical_docs/en/options.md#opt-powerreserve) | **20%** (10–50%) | Capacità di generazione di scorta per la parte della domanda soddisfatta localmente; riserve maggiori danno più margine per seguire una crescita rapida e riducono il rischio di blackout, ma possono costare di più. |
| [Considera l'usura delle centrali](technical_docs/en/options.md#opt-poweraging) | **Disattivo** | Include nell'ottimizzazione il costo stimato dell'usura e dell'invecchiamento delle centrali. Può favorire un finanziamento più alto quando il beneficio a lungo termine della conservazione della centrale supera il costo di esercizio aggiuntivo. |
| [Gestisci l'acqua](technical_docs/en/options.md#opt-water) | **Attivo** | Regola il finanziamento del reparto idrico per rifornire la città senza spese inutili. |
| [Riserva idrica](technical_docs/en/options.md#opt-waterreserve) | **10%** (10–50%) | Funziona come la riserva elettrica. |
| [Proteggi le condutture idriche](technical_docs/en/options.md#opt-waterpipes) | **Attivo** | Mantiene il finanziamento almeno al 50% del gioco, per evitare danni alla rete di condutture. |
| [Gestisci lo smaltimento rifiuti](technical_docs/en/options.md#opt-sanitation) | **Attivo** | Regola il finanziamento del reparto rifiuti per coprire le esigenze di raccolta e smaltimento. Ripara anche le caselle fantasma (un difetto del gioco che addebita la manutenzione di caselle di discarica non più in zona). |
| [Riserva di smaltimento](technical_docs/en/options.md#opt-sanitationreserve) | **2%** (2–20%) | Capacità di trattamento in più per i rifiuti della città, oltre a quanto produce dopo il riciclaggio e a ogni arretrato accumulato. |
| [Avvisami se rivedere la discarica può farmi risparmiare](technical_docs/en/options.md#opt-landfilladvice) | **Disattivo** | Attiva il consiglio facoltativo sulle modifiche alla zonizzazione della discarica quando fanno risparmiare e i rifiuti della città restano comunque gestiti. Richiede la gestione dello smaltimento. |
| [Soglia di risparmio per la discarica](technical_docs/en/options.md#opt-landfillthreshold) | **25%** (20–50%) | Ti chiede di valutare una modifica alla dimensione della discarica solo se il risparmio mensile modellato raggiunge almeno questa quota del bilancio dello smaltimento. |
| [Gestisci gli accordi con i vicini](technical_docs/en/options.md#opt-deals) | **Attivo** | Attiva i contratti regionali automatici; i tre interruttori singoli qui sotto decidono quali risorse si possono scambiare. |
| [Acquista da una città e vendi a un'altra](technical_docs/en/options.md#opt-arbitrage) | **Disattivo** | Consente combinazioni redditizie di acquisto e vendita della stessa risorsa tra vicini, dando comunque la precedenza alle esigenze della città. |
| [Gestisci gli accordi per l'elettricità](technical_docs/en/options.md#opt-tradepower) | **Attivo** | Consente al Ministero di creare, modificare o annullare **importazioni ed esportazioni** di elettricità. Indipendente dal finanziamento delle centrali. |
| [Gestisci gli accordi per l'acqua](technical_docs/en/options.md#opt-tradewater) | **Attivo** | Consente al Ministero di creare, modificare o annullare **importazioni ed esportazioni** di acqua. Indipendente dal finanziamento dell'acqua. |
| [Gestisci gli accordi per i rifiuti](technical_docs/en/options.md#opt-tradegarbage) | **Attivo** | Consente al Ministero di concordare il trattamento dei rifiuti **in entrambe le direzioni**. Indipendente dal finanziamento dello smaltimento. |

### Demolizioni

| Opzione | Predefinito | Significato |
|---|---|---|
| [Demolisci gli edifici distrutti](technical_docs/en/options.md#opt-demolishdestroyed) | **Attivo** | Demolisce i ruderi dopo un certo periodo. Non sgombera automaticamente i ruderi comunali o storici protetti, a meno che l'opzione qui sotto lo consenta. |
| [Attesa (edifici distrutti)](technical_docs/en/options.md#opt-destroyedmonths) | **1 mese** (1–24) | Per quanto tempo i ruderi devono restare prima di poter essere sgomberati; circa **30 giorni simulati** per impostazione predefinita. |
| [Demolisci gli edifici comunali e storici distrutti](technical_docs/en/options.md#opt-demolishmunicipal) | **Disattivo** | Se attiva, sgombera anche, dopo l'attesa, i **ruderi** idonei di edifici comunali, speciali e storici. |
| [Demolisci gli edifici abbandonati](technical_docs/en/options.md#opt-demolishabandoned) | **Attivo** | Sgombera gli edifici abbandonati idonei dopo il periodo di attesa, nei tipi di zona e per i motivi selezionati. |
| [Attesa (edifici abbandonati)](technical_docs/en/options.md#opt-abandonedmonths) | **12 mesi** (1–60) | Per quanto tempo un edificio deve restare abbandonato prima di poter essere demolito; un anno simulato per impostazione predefinita. |
| [Zona residenziale](technical_docs/en/options.md#opt-zoneresidential) | **Attivo** | Include case e condomini abbandonati. |
| [Zona commerciale](technical_docs/en/options.md#opt-zonecommercial) | **Attivo** | Include negozi e uffici abbandonati. |
| [Zona industriale](technical_docs/en/options.md#opt-zoneindustrial) | **Attivo** | Include fabbriche e altri edifici industriali abbandonati. |
| [Zona agricola](technical_docs/en/options.md#opt-zoneagriculture) | **Disattivo** | Include le fattorie abbandonate. Disattivata per impostazione predefinita perché caselle vuote o abbandonate in una fattoria possono essere normali. |
| [Causa: zona poco attraente](technical_docs/en/options.md#opt-reasonlowdesirability) | **Attivo** | Include l'abbandono causato da scarsa attrattiva, come inquinamento o mancanza di servizi. |
| [Causa: tragitto troppo lungo](technical_docs/en/options.md#opt-reasoncommute) | **Attivo** | Include l'abbandono causato da tragitti eccessivamente lunghi verso il lavoro. |
| [Causa: assenza di domanda](technical_docs/en/options.md#opt-reasonlowdemand) | **Attivo** | Include gli edifici abbandonati perché la domanda per quel tipo è scomparsa. |
| [Causa: assenza di elettricità](technical_docs/en/options.md#opt-reasonnopower) | **Disattivo** | Include, a scelta, gli edifici abbandonati per mancanza di elettricità. Di solito conviene ripristinare prima l'elettricità e vedere se l'edificio torna a popolarsi. |
| [Causa: assenza di acqua](technical_docs/en/options.md#opt-reasonnowater) | **Disattivo** | Include, a scelta, gli edifici abbandonati per mancanza di acqua. Di solito conviene ripristinare prima l'acqua e vedere se l'edificio torna a popolarsi. |
| [Limita la spesa mensile per le demolizioni](technical_docs/en/options.md#opt-budgetlimit) | **Disattivo** | Fissa un tetto di spesa facoltativo per la demolizione automatica, oltre ai fondi disponibili nella cassa della città. |
| [Bilancio mensile](technical_docs/en/options.md#opt-demolitionbudget) | **§1.000** (§0–§200.000) | Spesa massima per le demolizioni per mese simulato **quando il limite di bilancio è attivo**. |

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

- **SimCity 4 Deluxe, edizione Steam, versione del gioco 1.1.641.0, su Windows.** Ho testato il Ministero su questa versione; le altre versioni non sono state testate.

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

- **Consigliato:** aggiungi `-CPUCount:1` in **Steam → Libreria → SimCity 4 Deluxe → Proprietà → Opzioni di avvio**. Ha migliorato la stabilità nelle sessioni lunghe sul mio computer.

- **Dieci lingue nel gioco:** inglese, portoghese, francese, tedesco, spagnolo, italiano, olandese, danese, svedese e polacco. Il Ministero segue automaticamente la lingua del tuo gioco.

<a id="phantom-funding-slider-bug"></a>

## Compatibilità con il Phantom Funding Slider Bug

Il Ministero dell'Ottimizzazione è compatibile con le città colpite dal **Phantom Funding Slider Bug**, un vecchio problema di SimCity 4 in cui i cursori di finanziamento di edifici già posizionati continuano a muoversi da soli dopo che il costo di manutenzione di quel tipo di edificio è stato modificato (per esempio da una mod).

**Non** corregge il bug di SimCity 4 in sé. Rileva invece variazioni sospette e ricorrenti del finanziamento e riadatta automaticamente le strutture che gestisce, aiutando a mantenere la capacità di servizio richiesta ed evitando sovrafinanziamenti inutili.

Negli scenari testati le correzioni sono avvenute entro un giorno simulato, invece di poter richiedere un mese intero.

Questa protezione si applica alle strutture idonee (scuole, biblioteche, musei, cliniche e ospedali) in gestione automatica. Non ripara i dati di bilancio interni del gioco e non garantisce protezione in ogni situazione. Vedi [Phantom Funding Slider Bug](technical_docs/en/phantom_slider.md) per come funziona, i suoi limiti e i risultati dei test (in inglese).

<a id="updating-and-uninstalling"></a>

## Aggiornamento e disinstallazione

**Le impostazioni non vengono trasferite.** Dalla versione 1.0.2 il Ministero ha un unico file di impostazioni, `DepartmentEfficiency.ini`, che sostituisce i tre file INI della versione 1.0.0 e la cartella `Department of Efficiency` del primo pacchetto 1.0.2. Le impostazioni dei file precedenti non vengono lette né migrate: reimposta le preferenze in **Opzioni di gioco → Ministero dell'Ottimizzazione**.

**Per aggiornare un'installazione precedente (è obbligatoria un'installazione pulita).** Chiudi il gioco ed elimina i vecchi file del Ministero **prima** di copiare i tre nuovi. Elimina solo questi, in `Documents\SimCity 4\Plugins`:

- `DepartmentEfficiency.dll`, `DepartmentEfficiency.dat` e `DepartmentEfficiency.ini`;
- l'intera cartella `Department of Efficiency`, se esiste;
- della versione 1.0.0, se sono ancora presenti: `DepartmentEfficiencyUI.dat`, `DepartmentEfficiencyCivicAdvice.dat`, `DepartmentEfficiencyDealAdvice.dat`, `DepartmentEfficiencyGarbageAdvice.dat`, `DepartmentEfficiencyUtility.ini` e `DepartmentEfficiencyDemolition.ini`;
- `DepartmentEfficiency.told.txt`, un piccolo file di memoria che il plugin può aver lasciato (viene ricreato quando serve).

Non toccare i file delle altre mod. Copiare i nuovi file sopra una vecchia installazione non è supportato: i vecchi file di risorse resterebbero e potrebbero tenere silenziati i suggerimenti del gioco. Il plugin non legge mai i file della vecchia cartella; se la cartella è ancora presente, scrive solo un avviso nel suo registro.

**Per disinstallare:** chiudi il gioco ed elimina gli stessi file. Le tue città salvate restano leggibili senza la mod. I livelli di finanziamento, i contratti esistenti e le demolizioni già salvate in una città non vengono annullati automaticamente.

**File di registro:** `DepartmentEfficiency.log` si trova nella cartella `Logs` della tua cartella utente di SimCity 4 (normalmente `Documents\SimCity 4\Logs`, o la cartella `Logs` della cartella indicata con `-UserDir`). Se quella cartella non è utilizzabile, viene scritto accanto alla DLL.

<a id="known-limitations"></a>

## Limiti noti

- **Compatibilità:** ho testato la versione **1.1.641.0** di SimCity 4 Deluxe per Windows (Steam). Le altre versioni del gioco non sono state testate.

- **Consiglio sulla discarica:** richiede esattamente una zona di discarica. Nelle città con più discariche, il Ministero può comunque riparare le «caselle fantasma» idonee, ma non raccomanda modifiche di dimensione.

<a id="source-code"></a>

## Codice sorgente

Il codice sorgente del plugin si trova nella cartella [`source/`](source/). Si compila con CMake e Visual Studio in una DLL a 32 bit (Win32). Servono Visual Studio 2026 con il carico di lavoro per lo sviluppo desktop in C++ (le altre versioni non sono state provate), CMake 3.20 o successivo e Git:

```bat
cmake -S . -B build -G "Visual Studio 18 2026" -A Win32
cmake --build build --config Release
```

La compilazione scarica [gzcom-dll](https://github.com/nsgomez/gzcom-dll) a una revisione fissa (`779b669bf88112146d58fba6c7ed44f13ac6ce7d`). Per usare una copia modificata di quella libreria, o per compilare senza connessione a Internet, aggiungi `-DFETCHCONTENT_SOURCE_DIR_GZCOM_DLL=<cartella>` al primo comando. La DLL viene creata in `build\Release\DepartmentEfficiency.dll`. Una DLL ricompilata non è identica byte per byte a quella pubblicata.

<a id="license"></a>

## Licenza

- Il codice originale di Department of Efficiency (i file in [`source/`](source/)) è stato scritto da me, **Sammu89** ed è pubblicato con la **GNU General Public License, versione 3 soltanto** (GPL-3.0-only). Il testo completo è in [LICENSE](LICENSE). Puoi studiarlo, modificarlo e ridistribuirlo secondo la GPLv3: conserva gli avvisi di copyright e i crediti, indica le tue modifiche e pubblica la tua versione con la stessa licenza. Per favore, non presentare versioni modificate come mie versioni ufficiali.

- La documentazione (questi README e i file in [`technical_docs/`](technical_docs/)) e i testi scritti per questo progetto (inclusi quelli del file INI e quelli compilati in `DepartmentEfficiency.dat`), con le loro traduzioni originali, sono pubblicati con la stessa licenza, GPL-3.0-only.

- **gzcom-dll** non è coperta da questa licenza: mantiene la propria, LGPL-2.1-or-later ([licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt)), e viene scaricata dal repository originale durante la compilazione.

- **SimCity 4** e le sue risorse originali appartengono a Electronic Arts / Maxis. Il **Ministero dell'Ottimizzazione** è una mod non ufficiale creata da un fan, indipendente da EA e non approvata da essa. Oltre ai testi e agli script propri del progetto, `DepartmentEfficiency.dat` contiene due script di interfaccia (il pulsante e la finestra delle opzioni) derivati dalle risorse del gioco stesso, e gli screenshot mostrano il gioco: non sono di mia proprietà, non sono coperti dalla GPL e tutti i diritti restano ai rispettivi titolari. Lo stesso vale per qualsiasi altro materiale di terzi. L'eseguibile del gioco non viene distribuito.

<a id="credits"></a>

## Crediti

**Autore**

- Creato da **Sammu89**.

**Ringraziamento speciale**

- **Un ringraziamento speciale a CorinaMarie** per le sue spiegazioni dettagliate del Phantom Funding Slider Bug, per aver condiviso con generosità le città di prova e i file DAT riproducibili che hanno reso possibile indagarne e mitigarne gli effetti, e per la sua preziosa guida tecnica su come preservare e ripristinare i messaggi originali dei consiglieri di Maxis, compresa la gestione selettiva dei record di avviso in Lua. Il suo aiuto ha reso il Ministero più robusto e più compatibile con il gioco e con altre mod.

Il codice del Ministero dell'Ottimizzazione è stato scritto da Sammu89. Le persone citate qui hanno condiviso conoscenze, strumenti, materiale di prova e consigli; non sono coautori del codice.

**Libreria utilizzata**

- **gzcom-dll**, di Nelson Gomez (simmaster07), con contributi di Nicholas Hayes (Null 45), Casper Van Gheluwe, memo e altri (vedi il suo [repository](https://github.com/nsgomez/gzcom-dll)), usata con licenza LGPL-2.1-or-later e collegata staticamente alla DLL. La libreria non è inclusa: la compilazione scarica la revisione esatta dal repository originale (vedi [Codice sorgente](#source-code)); gli avvisi di copyright e il testo della licenza sono in [licenses/gzcom-dll.txt](licenses/gzcom-dll.txt) e [licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt). Puoi modificare la libreria e ricompilare il plugin con la tua versione.

**Tecniche, documentazione ed esempi consultati** (non è stato copiato codice da essi; i loro autori non sono coautori di Department of Efficiency):

- Nicholas Hayes (0xC0000054 / Null 45), per ricerca, documentazione ed esempi aperti sulle DLL di SimCity 4: sc4-dll-utilities, sc4-custom-budget-departments, sc4-query-ui-hooks, sc4-lua-extensions, sc4-lua-extension-example. La finestra delle opzioni, l'ordinanza e il file degli avvisi seguono meccanismi pubblicati nei suoi progetti sc4-region-census, sc4-humane-society-ordinance e sc4-lua-extension-example (licenza MIT); i comandi del gioco seguono sc4-auto-run-cheats e sc4-game-command-documentation.

- CorinaMarie, per aver documentato la tecnica Urgent Advice di SimCity 4 che gli avvisi dei consiglieri del mod seguono.

- memo, per le indicazioni su come caricare uno script Lua proprio del plugin, con identificatori propri, dopo gli script del gioco e modificando solo i record di avviso necessari, che l'integrazione del Ministero con i consiglieri del gioco segue.

- Kel9509, per le informazioni su come riprodurre il Phantom Funding Slider Bug.

- La comunità di modding di SimCity 4, per due decenni di ricerca, strumenti e conoscenze condivise, inclusa la documentazione delle proprietà degli exemplar di SC4Devotion.

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
