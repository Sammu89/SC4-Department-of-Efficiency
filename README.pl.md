[English](README.md) | [Português](README.pt.md) | [Français](README.fr.md) | [Deutsch](README.de.md) | [Español](README.es.md) | [Italiano](README.it.md) | [Nederlands](README.nl.md) | [Dansk](README.da.md) | [Svenska](README.sv.md) | **Polski**

# Ministerstwo Optymalizacji

**Ministerstwo Optymalizacji zajmuje się drobnymi szczegółami zarządzania miastem, które robią dużą różnicę.**

Ministerstwo Optymalizacji (*Department of Efficiency* po angielsku) to wtyczka do **SimCity 4 Deluxe**, która automatyzuje rutynowe zadania w zarządzaniu miastem, dzięki czemu możesz skupić się na budowaniu, planowaniu i rozwijaniu swojego miasta.

**Ty budujesz miasto. Ministerstwo optymalizuje budżet!**

Zarządza budżetami poszczególnych budynków oświaty i ochrony zdrowia, elektrowniami, finansowaniem wodociągów i gospodarki odpadami, umowami z sąsiednimi miastami oraz, opcjonalnie, wyburzaniem ruin i opuszczonych budynków. Jego celem jest zaspokojenie potrzeb miasta bez wydawania więcej, niż to konieczne; rozmieszczenie budynków, zasięg usług i ważne decyzje planistyczne pozostają po Twojej stronie.

Wszystkim steruje się w grze, w **Opcje gry → Ministerstwo Optymalizacji**. Wybierz usługi do automatyzacji, dostosuj ich marginesy bezpieczeństwa, skonfiguruj wyburzanie albo wyłącz całe Ministerstwo. Jako główny wyłącznik możesz też użyć uchwały miejskiej **Utwórz Ministerstwo Optymalizacji**. Nie potrzeba żadnej osobnej aplikacji.

> [!IMPORTANT]
> **Ministerstwo startuje włączone, ale automatyczne wyburzanie jest domyślnie wyłączone.** Sprawdź jego ustawienia, zanim otworzysz miasto, na którym Ci zależy. Wprowadza ono prawdziwe zmiany w grze: finansowanie i umowy (oraz wyburzenia, jeśli je włączysz) zostaną zapisane, jeśli zapiszesz miasto. Nie edytuje bezpośrednio plików zapisu.

## Spis treści

- [Pobieranie i instalacja](#download-and-installation)

- [Co robi Ministerstwo](#features)

- [Pierwsza konfiguracja](#first-time-setup)

- [Ustawienia](#settings-explained)

- [Zrzuty ekranu](#screenshots)

- [Wymagania i zgodność](#requirements-and-compatibility)

- [Zgodność z Phantom Funding Slider Bug](#phantom-funding-slider-bug)

- [Aktualizacja i odinstalowanie](#updating-and-uninstalling)

- [Znane ograniczenia](#known-limitations)

- [Kod źródłowy](#source-code)

- [Licencja](#license)

- [Podziękowania](#credits)

<a id="download-and-installation"></a>

## Instalacja

### [⬇ Pobierz Department of Efficiency 1.0.3](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases/download/v1.0.3/Department_of_Efficiency_1.0.3.zip)

**Nowości w 1.0.3:** zobacz [dziennik zmian](CHANGELOG.md) (po angielsku).

1. Pobierz [najnowszą wersję](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases/latest) i zamknij SimCity 4.

2. Rozpakuj plik ZIP bezpośrednio do `Documents\SimCity 4\Plugins`.

3. Sprawdź, czy `DepartmentEfficiency.dll`, `DepartmentEfficiency.dat` i `DepartmentEfficiency.ini` znajdują się w folderze Plugins.

4. Uruchom SimCity 4 i otwórz **Opcje gry → Ministerstwo Optymalizacji**, aby ustawić preferencje.

> [!IMPORTANT]
> Ministerstwo jest domyślnie włączone; automatyczne wyburzanie jest domyślnie **wyłączone**. Jeśli je włączysz, sprawdź ustawienia **wyburzania** przed otwarciem miasta z budynkami, które chcesz zachować.

<a id="features"></a>

## Co robi Ministerstwo

Poniższa tabela opisuje, co robi Ministerstwo. Wzory, przykłady, zabezpieczenia i mechanikę stojącą za jego decyzjami znajdziesz w **[szczegółowym opisie funkcji (po angielsku)](technical_docs/en/index.md)**.

| Obszar | Co robi |
|---|---|
| [Oświata i ochrona zdrowia](technical_docs/en/education_healthcare.md#education-healthcare) | Dostosowuje finansowanie **budynek po budynku** dla kwalifikujących się szkół, uczelni, bibliotek, muzeów, przychodni i szpitali. Kieruje się rzeczywistą frekwencją i pojemnością, z marginesem bezpieczeństwa. **Nie zmienia zasięgów** ani finansowania autobusów szkolnych i karetek. Jeśli budynek przy maksymalnym finansowaniu nadal jest przepełniony, doradca informuje, że potrzebny jest kolejny obiekt; **to Ty wybierasz, gdzie go zbudować**. |
| [Energia elektryczna](technical_docs/en/power.md#power) | Zarządza **każdą elektrownią osobno**, wybierając finansowanie, które pokrywa potrzeby miasta i rezerwę bezpieczeństwa przy najniższym modelowanym koszcie. Uwzględnia sprawność i stan elektrowni; opcjonalne ustawienie bierze pod uwagę także ekonomiczny koszt ich starzenia się. |
| [Woda](technical_docs/en/water.md#water) | Dostosowuje finansowanie wodociągów miasta do zapotrzebowania plus rezerwy bezpieczeństwa. Ochrona rur jest domyślnie włączona, by uniknąć cięć, przez które sieć by niszczała. Ukrywa też mylącą ikonę „brak wody” w grze nad farmami. |
| [Gospodarka odpadami](technical_docs/en/garbage.md#garbage-landfills) | Dostosowuje finansowanie gospodarki odpadami na podstawie ilości śmieci, recyklingu, przetwarzania, pojemności wysypisk i umów regionalnych, chroniąc przy tym własne potrzeby miasta w zakresie utylizacji. |
| [Umowy z sąsiadami](technical_docs/en/deals.md#neighbor-deals) | Może **kupować lub sprzedawać prąd i wodę**, **płacić sąsiadom za przyjmowanie Twoich śmieci** albo **przyjmować ich śmieci za opłatą**. Zawiera, dostosowuje i wypowiada kwalifikujące się umowy, gdy ogólny wynik służy miastu, a jego własne potrzeby i rezerwy pozostają chronione. |
| [Automatyczne wyburzanie](technical_docs/en/demolition.md#demolition) | **Domyślnie wyłączone.** Domyślnie może uprzątać ruiny po około **30 dniach symulacji** oraz kwalifikujące się budynki opuszczone od **jednego roku symulacji**. Ty decydujesz, które strefy i które powody opuszczenia się liczą, czy chronione ruiny mogą być uprzątane i czy ustawić miesięczny limit wydatków. |
| [Komunikaty doradców](technical_docs/en/safety.md#notices) | Korzysta z doradców gry, by zgłaszać udowodnione, trwałe niedobory, których nie rozwiążą ani finansowanie, ani dostępne umowy. Gdy to ma sens, komunikaty zawierają odnośniki prowadzące do danych budynków. Dodaje też wiadomość (tytuł na liście, szczegóły po kliknięciu), gdy włączona przez Ciebie usługa musi się zatrzymać z powodu awarii technicznej. |
| [Porada o rozmiarze wysypiska](technical_docs/en/garbage.md#garbage-landfills-advice) | Opcjonalna porada wskazuje konkretne, bezpieczne zmniejszenie wysypiska (puste pola do usunięcia), które mogłoby obniżyć rachunek za odpady o więcej niż wybrany próg oszczędności. **Ministerstwo doradza; to Ty decydujesz, czy zmienić strefę.** |
| [Naprawa pól-widm](technical_docs/en/garbage.md#ghost-tiles) | Gdy zarządzanie odpadami jest włączone, bezpiecznie naprawia kwalifikujące się niewidoczne pola wysypiska, które gra nadal wlicza do utrzymania. Przypadki niebezpieczne lub niezweryfikowane są pomijane; ta naprawa nie ma osobnego przełącznika. |

Nadal kontrolujesz budowę, strefy, podatki, drogi i transport, policję, straż pożarną oraz każdą inną usługę, którą Ministerstwo nie zarządza. Jeśli włączysz zarządzanie danym rodzajem umów z sąsiadami, Ministerstwo może też zmienić lub wypowiedzieć umowę, którą wcześniej zawarto ręcznie. Wyłącz tę opcję umów, by samodzielnie nimi zarządzać.

<a id="first-time-setup"></a>

## Pierwsza konfiguracja

Ministerstwo używa ustawień dostarczonych z wtyczką i **zaczyna działać automatycznie**. Nie musisz go włączać, ale przed użyciem ważnego zapisanego miasta sprawdź, na co mu pozwalasz.

1\. Uruchom SimCity 4 i w **widoku regionu** otwórz **Opcje gry → Ministerstwo Optymalizacji**. Jeśli wolisz, na pierwszy rzut oka użyj nowego lub zbędnego miasta testowego.

2\. Na stronie **Służby miejskie** wybierz, którymi budżetami i umowami z sąsiadami Ministerstwo może zarządzać oraz ile zapasowej pojemności ma utrzymywać.

3\. Na stronie **Wyburzanie** automatyczne wyburzanie jest wyłączone. Zaznacz opcje dotyczące ruin i opuszczonych budynków tylko wtedy, gdy chcesz, aby Ministerstwo uprzątało je za Ciebie.

4\. Kliknij **Akceptuj**, aby zapisać wybór. **Anuluj** zamyka okno bez zapisywania zmian.

5\. Otwórz miasto, w które chcesz zagrać. Ministerstwo stosuje się do zapisanych preferencji i zarządza miastem w trakcie symulacji.

Preferencje są przechowywane w pliku `DepartmentEfficiency.ini` i wykorzystywane po otwarciu innego miasta. Ministerstwo **nie** buduje za Ciebie szkół, szpitali, elektrowni ani obiektów wodnych. Gdy finansowanie i dostępne umowy nie rozwiążą udowodnionego, trwałego niedoboru, doradca w grze podpowie, gdzie potrzeba większej pojemności.

Ministerstwo możesz też włączać i wyłączać za pomocą uchwał miejskich.

<a id="settings-explained"></a>

## Ustawienia

Otwórz **Opcje gry → Ministerstwo Optymalizacji**. Okno ma dwie strony: **Służby miejskie** i **Wyburzanie**. Poniższe tabele obejmują **każde ustawienie widoczne w menu**, bez wzorów technicznych. Kliknij nazwę opcji, aby przeczytać jej pełne wyjaśnienie w [opisie funkcji](technical_docs/en/options.md#options).

**Jak działają przełączniki:** „Włączone” oznacza, że Ministerstwo może zarządzać daną funkcją. Ustawienie zależne od innego jest wyszarzone, dopóki nadrzędne jest wyłączone. Ustawienia procentowe określają marginesy bezpieczeństwa i progi, pomagając Ministerstwu utrzymać zapas pojemności i radzić sobie z wahaniami popytu.

### Służby miejskie

| Opcja | Domyślnie | Co oznacza |
|---|---|---|
| [Ministerstwo aktywne](technical_docs/en/options.md#opt-master) | **Włączone** | Główny wyłącznik finansowania, umów z sąsiadami, wyburzania i komunikatów Ministerstwa. Wyłączenie nie cofa zmian już wprowadzonych. |
| [Zarządzaj oświatą](technical_docs/en/options.md#opt-education) | **Włączone** | Automatycznie dostosowuje po kolei kwalifikujące się szkoły, uczelnie, biblioteki i muzea. Nie zmienia ich zasięgu ani finansowania autobusów szkolnych. |
| [Margines bezpieczeństwa (oświata)](technical_docs/en/options.md#opt-educationreserve) | **5%** (0–50%) | Dodatkowa pojemność ponad bieżące wykorzystanie oświaty. Większa pojemność kosztuje więcej. W budynkach o niskiej frekwencji Ministerstwo może utrzymywać dodatkowy margines. Pozwala to na przykład szkole urosnąć z 10 do 20 uczniów (+100%) bez strajku, zwłaszcza na początku, gdy dodatkowy koszt jest niewielki. |
| [Zarządzaj ochroną zdrowia](technical_docs/en/options.md#opt-health) | **Włączone** | Automatycznie dostosowuje po kolei kwalifikujące się przychodnie, szpitale i inne obsługiwane obiekty zdrowotne. Nie zmienia finansowania karetek ani zasięgu. |
| [Margines bezpieczeństwa (zdrowie)](technical_docs/en/options.md#opt-healthreserve) | **5%** (0–50%) | Działa tak samo jak margines bezpieczeństwa oświaty. |
| [Zarządzaj energetyką](technical_docs/en/options.md#opt-power) | **Włączone** | Dostosowuje finansowanie poszczególnych elektrowni, aby dostarczyć dość prądu przy najniższym modelowanym koszcie. |
| [Rezerwa energii](technical_docs/en/options.md#opt-powerreserve) | **20%** (10–50%) | Zapasowa moc wytwórcza dla części zapotrzebowania pokrywanej lokalnie; większe rezerwy dają więcej miejsca na szybki wzrost i zmniejszają ryzyko awarii zasilania, ale mogą kosztować więcej. |
| [Uwzględniaj zużycie elektrowni](technical_docs/en/options.md#opt-poweraging) | **Wyłączone** | Wlicza do optymalizacji szacowany koszt zużycia i starzenia się elektrowni. Może faworyzować wyższe finansowanie, gdy długoterminowa korzyść z zachowania elektrowni przewyższa dodatkowy koszt eksploatacji. |
| [Zarządzaj wodociągami](technical_docs/en/options.md#opt-water) | **Włączone** | Dostosowuje finansowanie wodociągów, by zaopatrywać miasto bez zbędnych wydatków. |
| [Rezerwa wody](technical_docs/en/options.md#opt-waterreserve) | **10%** (10–50%) | Działa tak samo jak rezerwa energii. |
| [Chroń wodociągi](technical_docs/en/options.md#opt-waterpipes) | **Włączone** | Utrzymuje finansowanie na poziomie co najmniej 50% z gry, by uniknąć uszkodzeń sieci rur. |
| [Zarządzaj odpadami](technical_docs/en/options.md#opt-sanitation) | **Włączone** | Dostosowuje finansowanie wydziału odpadów, by pokryć potrzeby zbiórki i utylizacji. Naprawia też pola-widma (błąd gry, który nalicza koszty utrzymania za pola wysypiska już bez strefy). |
| [Rezerwa przetwarzania odpadów](technical_docs/en/options.md#opt-sanitationreserve) | **2%** (2–20%) | Dodatkowa zdolność przetwarzania odpadów miasta, ponad to, co wytwarza po recyklingu, i ponad ewentualne zaległości. |
| [Ostrzegaj, gdy rozmiar wysypiska może obniżyć koszty](technical_docs/en/options.md#opt-landfilladvice) | **Wyłączone** | Włącza opcjonalną poradę o zmianach strefy wysypiska, jeśli oszczędzają one pieniądze, a odpady miasta nadal są obsługiwane. Wymaga zarządzania odpadami. |
| [Próg oszczędności dla wysypiska](technical_docs/en/options.md#opt-landfillthreshold) | **25%** (20–50%) | Prosi o rozważenie zmiany rozmiaru wysypiska tylko wtedy, gdy modelowana miesięczna oszczędność wynosi co najmniej tę część budżetu gospodarki odpadami. |
| [Zarządzaj umowami z sąsiadami](technical_docs/en/options.md#opt-deals) | **Włączone** | Włącza automatyczne umowy regionalne; trzy osobne przełączniki poniżej decydują, jakimi zasobami wolno handlować. |
| [Kupuj od jednego miasta, sprzedawaj innemu](technical_docs/en/options.md#opt-arbitrage) | **Wyłączone** | Dopuszcza opłacalne kombinacje kupna i sprzedaży tego samego zasobu między sąsiadami, z zachowaniem pierwszeństwa potrzeb miasta. |
| [Zarządzaj umowami na prąd](technical_docs/en/options.md#opt-tradepower) | **Włączone** | Pozwala Ministerstwu zawierać, zmieniać i wypowiadać **umowy na import i eksport** prądu. Niezależne od finansowania elektrowni. |
| [Zarządzaj umowami na wodę](technical_docs/en/options.md#opt-tradewater) | **Włączone** | Pozwala Ministerstwu zawierać, zmieniać i wypowiadać **umowy na import i eksport** wody. Niezależne od finansowania wodociągów. |
| [Zarządzaj umowami na odpady](technical_docs/en/options.md#opt-tradegarbage) | **Włączone** | Pozwala Ministerstwu ustalać przetwarzanie odpadów **w obu kierunkach**. Niezależne od finansowania gospodarki odpadami. |

### Wyburzanie

| Opcja | Domyślnie | Co oznacza |
|---|---|---|
| [Wyburzaj zniszczone budynki](technical_docs/en/options.md#opt-demolishdestroyed) | **Wyłączone** | Wyburza ruiny po określonym czasie. Nie uprząta automatycznie chronionych ruin budynków miejskich ani zabytkowych, chyba że pozwala na to opcja poniżej. |
| [Okres oczekiwania (zniszczone budynki)](technical_docs/en/options.md#opt-destroyedmonths) | **1 miesiąc** (1–24) | Jak długo ruiny muszą stać, zanim będzie można je uprzątnąć; domyślnie około **30 dni symulacji**. |
| [Wyburzaj zniszczone budynki miejskie i zabytkowe](technical_docs/en/options.md#opt-demolishmunicipal) | **Wyłączone** | Po włączeniu uprząta po okresie oczekiwania także kwalifikujące się **ruiny** budynków miejskich, specjalnych i zabytkowych. |
| [Wyburzaj opuszczone budynki](technical_docs/en/options.md#opt-demolishabandoned) | **Wyłączone** | Uprząta kwalifikujące się opuszczone budynki po okresie oczekiwania, w wybranych typach stref i z wybranych powodów. |
| [Okres oczekiwania (opuszczone budynki)](technical_docs/en/options.md#opt-abandonedmonths) | **12 miesięcy** (1–60) | Jak długo budynek musi pozostawać opuszczony, zanim będzie można go wyburzyć; domyślnie jeden rok symulacji. |
| [Strefy mieszkalne](technical_docs/en/options.md#opt-zoneresidential) | **Włączone** | Obejmuje opuszczone domy i bloki mieszkalne. |
| [Strefy handlowe](technical_docs/en/options.md#opt-zonecommercial) | **Włączone** | Obejmuje opuszczone sklepy i biura. |
| [Strefy przemysłowe](technical_docs/en/options.md#opt-zoneindustrial) | **Włączone** | Obejmuje opuszczone fabryki i inne budynki przemysłowe. |
| [Strefy rolnicze](technical_docs/en/options.md#opt-zoneagriculture) | **Wyłączone** | Obejmuje opuszczone farmy. Domyślnie wyłączone, bo puste lub opuszczone pola na farmie mogą być czymś normalnym. |
| [Powód: nieatrakcyjna okolica](technical_docs/en/options.md#opt-reasonlowdesirability) | **Włączone** | Obejmuje opuszczenie spowodowane niską atrakcyjnością, np. zanieczyszczeniem lub brakiem usług. |
| [Powód: zbyt długi dojazd](technical_docs/en/options.md#opt-reasoncommute) | **Włączone** | Obejmuje opuszczenie spowodowane zbyt długim dojazdem do pracy. |
| [Powód: brak popytu](technical_docs/en/options.md#opt-reasonlowdemand) | **Włączone** | Obejmuje budynki opuszczone, ponieważ popyt na dany typ zniknął. |
| [Powód: brak prądu](technical_docs/en/options.md#opt-reasonnopower) | **Wyłączone** | Opcjonalnie obejmuje budynki opuszczone z powodu braku prądu. Zwykle lepiej najpierw przywrócić prąd i sprawdzić, czy budynek się zaludni. |
| [Powód: brak wody](technical_docs/en/options.md#opt-reasonnowater) | **Wyłączone** | Opcjonalnie obejmuje budynki opuszczone z powodu braku wody. Zwykle lepiej najpierw przywrócić wodę i sprawdzić, czy budynek się zaludni. |
| [Ogranicz miesięczny budżet wyburzeń](technical_docs/en/options.md#opt-budgetlimit) | **Wyłączone** | Ustawia opcjonalny limit wydatków na automatyczne wyburzanie, oprócz środków dostępnych w kasie miasta. |
| [Budżet miesięczny](technical_docs/en/options.md#opt-demolitionbudget) | **§1 000** (§0–§200 000) | Maksymalne wydatki na wyburzenia w miesiącu symulacji, **gdy limit budżetu jest włączony**. |

<a id="screenshots"></a>

## Zrzuty ekranu

Wszystkie zrzuty ekranu pokazują angielską wersję gry.

| | |
|---|---|
| ![Strona Służby miejskie w ustawieniach Ministerstwa](screenshots/Options-1.png) | ![Strona Wyburzanie w ustawieniach Ministerstwa](screenshots/Options-2.png) |
| *Strona Służby miejskie: każda usługa, jej rezerwa i umowy z sąsiadami.* | *Strona Wyburzanie: ruiny, opuszczone budynki i budżet miesięczny.* |
| ![Komunikat doradcy o przepełnionych liceach](screenshots/high_school.png) | ![Komunikat doradcy o przepełnionych muzeach](screenshots/museum.png) |
| *Szkoły przy maksymalnym finansowaniu, a mimo to przepełnione: komunikat prowadzi do budynków.* | *To samo dla muzeów.* |
| ![Komunikat doradcy o niedoborze wody](screenshots/water.png) | ![Komunikat doradcy o niedoborze pojemności na odpady](screenshots/real-garbage-paused.png) |
| *Woda: Ministerstwo nie może rozwiązać niedoboru przy dostępnych dostawach.* | *Odpady: miasto potrzebuje większej zdolności utylizacji.* |

<a id="requirements-and-compatibility"></a>

## Wymagania i zgodność

- **SimCity 4 Deluxe, wersja Steam, wersja gry 1.1.641.0, w systemie Windows.** Przetestowałem Ministerstwo na tej wersji; inne wersje nie zostały przetestowane.

- **Nie jest potrzebny żaden inny mod, instalator ani zewnętrzna aplikacja.**

- **Przetestowane i zgodne z tymi modami** (testowane wersje):

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

- **Zalecane:** dodaj `-CPUCount:1` w **Steam → Biblioteka → SimCity 4 Deluxe → Właściwości → Opcje uruchamiania**. Poprawiło to stabilność podczas długich sesji na moim komputerze.

- **Dziesięć języków w grze:** angielski, portugalski, francuski, niemiecki, hiszpański, włoski, niderlandzki, duński, szwedzki i polski. Ministerstwo automatycznie dopasowuje się do języka Twojej gry.

<a id="phantom-funding-slider-bug"></a>

## Zgodność z Phantom Funding Slider Bug

Ministerstwo Optymalizacji jest zgodne z miastami dotkniętymi **Phantom Funding Slider Bug** – dawnym problemem SimCity 4, w którym suwaki finansowania już postawionych budynków same się przesuwają po zmianie kosztu utrzymania tego typu budynku (na przykład przez moda).

**Nie** naprawia ono samego błędu SimCity 4. Zamiast tego wykrywa podejrzane, powtarzające się zmiany finansowania i automatycznie koryguje obiekty, którymi zarządza, co pomaga utrzymać wymaganą zdolność usług i unikać zbędnego przefinansowania.

W testowanych scenariuszach korekty następowały w ciągu jednego dnia symulacji, zamiast potencjalnie trwać cały miesiąc.

Ta ochrona dotyczy kwalifikujących się obiektów (szkół, bibliotek, muzeów, klinik i szpitali) pod zarządzaniem automatycznym. Nie naprawia wewnętrznych danych budżetowych gry i nie gwarantuje ochrony w każdej sytuacji. Zobacz [Phantom Funding Slider Bug](technical_docs/en/phantom_slider.md), aby poznać sposób działania, ograniczenia i wyniki testów (po angielsku).

<a id="updating-and-uninstalling"></a>

## Aktualizacja i odinstalowanie

Trzy pliki Ministerstwa znajdują się bezpośrednio w folderze Plugins. Przed aktualizacją zachowaj kopię `DepartmentEfficiency.ini`, aby nie utracić preferencji.

**Aktualizacja z wersji 1.0.0:** Zamknij grę, usuń cały podfolder `Department of Efficiency` i zastąp `DepartmentEfficiency.dll` nowym plikiem. Rozpakuj nowy ZIP do Plugins. Ustawienia z wersji 1.0.0 trzeba skonfigurować ponownie.

**Odinstalowanie:** Zamknij grę i usuń trzy pliki Ministerstwa z Plugins. Zapisane miasta nadal będą działać; zapisane finansowanie, umowy i wyburzenia pozostaną bez zmian.

**Plik dziennika:** `DepartmentEfficiency.log` znajduje się w folderze `Logs` Twojego folderu użytkownika SimCity 4 (zwykle `Documents\SimCity 4\Logs` albo folder `Logs` folderu podanego przez `-UserDir`). Jeśli tego folderu nie można użyć, plik jest zapisywany obok pliku DLL.

<a id="known-limitations"></a>

## Znane ograniczenia

- **Zgodność:** przetestowałem wersję **1.1.641.0** SimCity 4 Deluxe dla Windows (Steam). Inne wersje gry nie zostały przetestowane.

- **Porada o wysypisku:** wymaga dokładnie jednej strefy wysypiska. W miastach z wieloma wysypiskami Ministerstwo nadal może naprawiać kwalifikujące się błędne „pola-widma”, ale nie zaleca zmian rozmiaru.

<a id="source-code"></a>

## Kod źródłowy

Kod źródłowy wtyczki znajduje się w folderze [`source/`](source/). Buduje się go przy użyciu CMake i Visual Studio do 32-bitowej biblioteki DLL (Win32). Potrzebne są Visual Studio 2026 z pakietem „Programowanie aplikacji klasycznych w C++” (inne wersje nie były testowane), CMake 3.20 lub nowszy oraz Git:

```bat
cmake -S . -B build -G "Visual Studio 18 2026" -A Win32
cmake --build build --config Release
```

Podczas budowania pobierana jest biblioteka [gzcom-dll](https://github.com/nsgomez/gzcom-dll) w stałej rewizji (`779b669bf88112146d58fba6c7ed44f13ac6ce7d`). Aby użyć zmodyfikowanej kopii tej biblioteki albo zbudować projekt bez dostępu do Internetu, dodaj `-DFETCHCONTENT_SOURCE_DIR_GZCOM_DLL=<folder>` do pierwszego polecenia. Plik DLL powstaje w `build\Release\DepartmentEfficiency.dll`. Ponownie zbudowany plik DLL nie jest identyczny bajt po bajcie z opublikowanym.

<a id="license"></a>

## Licencja

- Oryginalny kod Department of Efficiency (pliki w [`source/`](source/)) napisałem ja, **Sammu89** i jest on udostępniany na licencji **GNU General Public License, wersja 3 wyłącznie** (GPL-3.0-only). Pełny tekst znajduje się w [LICENSE](LICENSE). Możesz go studiować, modyfikować i redystrybuować na warunkach GPLv3: zachowaj informacje o prawach autorskich i podziękowania, oznacz swoje zmiany i udostępnij swoją wersję na tej samej licencji. Nie przedstawiaj zmodyfikowanych wersji jako moich oficjalnych wersji.

- Dokumentacja (te pliki README i pliki w [`technical_docs/`](technical_docs/)) oraz teksty napisane na potrzeby tego projektu (w tym te z pliku INI i skompilowane w `DepartmentEfficiency.dat`), wraz z ich oryginalnymi tłumaczeniami, są udostępniane na tej samej licencji, GPL-3.0-only.

- **gzcom-dll** nie jest objęta tą licencją: zachowuje własną licencję, LGPL-2.1-or-later ([licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt)), i jest pobierana z oryginalnego repozytorium podczas budowania.

- **SimCity 4** i jego oryginalne zasoby należą do Electronic Arts / Maxis. **Ministerstwo Optymalizacji** jest nieoficjalnym modem fanowskim, niezależnym od EA i niezatwierdzonym przez nią. Oprócz własnych tekstów i skryptów projektu `DepartmentEfficiency.dat` zawiera dwa skrypty interfejsu (przycisk i okno opcji) wywiedzione z własnych zasobów gry, a zrzuty ekranu pokazują grę: nie są one moją własnością, nie obejmuje ich GPL, a wszelkie prawa pozostają przy ich właścicielach. To samo dotyczy każdego innego materiału stron trzecich. Plik wykonywalny gry nie jest rozpowszechniany.

<a id="credits"></a>

## Podziękowania

**Autor**

- Stworzone przez **Sammu89**.

**Specjalne podziękowania**

- **Specjalne podziękowania dla CorinaMarie** za jej szczegółowe wyjaśnienia dotyczące Phantom Funding Slider Bug, za hojne udostępnienie powtarzalnych miast testowych i plików DAT, które umożliwiły zbadanie i złagodzenie jego skutków, oraz za nieocenione wskazówki techniczne dotyczące zachowywania i przywracania oryginalnych komunikatów doradców Maxis, w tym selektywnej obsługi rekordów porad w Lua. Jej pomoc uczyniła Ministerstwo bardziej odpornym i bardziej zgodnym z grą oraz z innymi modami.

Kod Ministerstwa Optymalizacji napisał Sammu89. Wymienione tu osoby dzieliły się wiedzą, narzędziami, materiałem testowym i radami; nie są współautorami kodu.

**Użyta biblioteka**

- **gzcom-dll** autorstwa Nelsona Gomeza (simmaster07), z wkładem Nicholasa Hayesa (Null 45), Caspera Van Gheluwe, memo i innych (zobacz jej [repozytorium](https://github.com/nsgomez/gzcom-dll)), używana na licencji LGPL-2.1-or-later i statycznie dołączona do pliku DLL. Biblioteka nie jest dołączona: podczas budowania pobierana jest dokładna rewizja z oryginalnego repozytorium (zobacz [Kod źródłowy](#source-code)); informacje o prawach autorskich i tekst licencji znajdują się w [licenses/gzcom-dll.txt](licenses/gzcom-dll.txt) i [licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt). Możesz zmodyfikować bibliotekę i przebudować wtyczkę z własną wersją.

**Wykorzystane techniki, dokumentacja i przykłady** (nie skopiowano z nich kodu; ich autorzy nie są współautorami Department of Efficiency):

- Nicholas Hayes (0xC0000054 / Null 45), za badania, dokumentację i otwarte przykłady dotyczące bibliotek DLL SimCity 4: sc4-dll-utilities, sc4-custom-budget-departments, sc4-query-ui-hooks, sc4-lua-extensions, sc4-lua-extension-example. Okno opcji, zarządzenie i plik komunikatów korzystają z mechanizmów opublikowanych w jego projektach sc4-region-census, sc4-humane-society-ordinance i sc4-lua-extension-example (licencja MIT); polecenia gry korzystają z sc4-auto-run-cheats i sc4-game-command-documentation.

- CorinaMarie, za udokumentowanie techniki Urgent Advice z SimCity 4, według której działają komunikaty doradców tego moda.

- memo, za wskazówki dotyczące ładowania własnego skryptu Lua wtyczki, z własnymi identyfikatorami, po skryptach gry i zmieniania tylko potrzebnych rekordów porad, za czym podąża integracja Ministerstwa z doradcami gry.

- Kel9509, za informacje o tym, jak odtworzyć Phantom Funding Slider Bug.

- Społeczność moderska SimCity 4, za dwie dekady badań, narzędzi i wspólnie dzielonej wiedzy, w tym dokumentację właściwości exemplarów z SC4Devotion.

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
