[English](README.md) | [Português](README.pt.md) | [Français](README.fr.md) | [Deutsch](README.de.md) | **Español** | [Italiano](README.it.md) | [Nederlands](README.nl.md) | [Dansk](README.da.md) | [Svenska](README.sv.md) | [Polski](README.pl.md)

# Ministerio de Optimización

**El Ministerio de Optimización se ocupa de los pequeños detalles de la gestión de la ciudad que marcan una gran diferencia.**

El Ministerio de Optimización (*Department of Efficiency*, en inglés) es un plugin para **SimCity 4 Deluxe** que automatiza tareas rutinarias de gestión de la ciudad, para que puedas centrarte en construir, planificar y hacer crecer tu ciudad.

**Tú construyes la ciudad. ¡El Ministerio optimiza el presupuesto!**

Gestiona los presupuestos de cada centro de educación y de sanidad, las centrales eléctricas, la financiación del agua y de la limpieza, los acuerdos con las ciudades vecinas y, de forma opcional, la demolición de ruinas y edificios abandonados. Su objetivo es cubrir las necesidades de la ciudad sin gastar más de lo necesario, y deja en tus manos la colocación de los edificios, la cobertura y las grandes decisiones de planificación.

Todo se controla desde el juego, en **Opciones de juego → Ministerio de Optimización**. Elige los servicios que quieres automatizar, ajusta sus márgenes de seguridad, configura la demolición o desactiva todo el Ministerio. También puedes usar la ordenanza **Crear un Ministerio de Optimización** como interruptor general. No hace falta ninguna aplicación aparte.

> [!IMPORTANT]
> **El Ministerio empieza activado, incluida la demolición automática de edificios destruidos y abandonados.** Revisa sus ajustes antes de abrir una ciudad que te importe. Hace cambios reales en el juego: la financiación, los contratos y las demoliciones se guardarán si guardas la ciudad. No edita directamente los archivos de guardado.

## Contenido

- [Descarga e instalación](#download-and-installation)

- [Qué hace el Ministerio](#features)

- [Configuración inicial](#first-time-setup)

- [Ajustes](#settings-explained)

- [Capturas de pantalla](#screenshots)

- [Requisitos y compatibilidad](#requirements-and-compatibility)

- [Actualizar y desinstalar](#updating-and-uninstalling)

- [Limitaciones conocidas](#known-limitations)

- [Código fuente](#source-code)

- [Licencia](#license)

- [Créditos](#credits)

<a id="download-and-installation"></a>

## Descarga e instalación

### [⬇ Descargar Department of Efficiency 1.0.0](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases/download/v1.0.0/Departement_of_Efficiency_1.00.zip)

1\. Abre la [página de releases](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases) del proyecto y elige la última versión.

2\. En **Assets**, descarga el **ZIP de distribución de Department of Efficiency**.

3\. **Cierra SimCity 4.** Extrae el ZIP en una carpeta temporal.

4\. Copia **`DepartmentEfficiency.dll`** y toda la carpeta **`Department of Efficiency`** en la raíz de `Documentos\SimCity 4\Plugins`.

5\. Inicia el juego y sigue las instrucciones de la [configuración inicial](#first-time-setup) antes de jugar una ciudad guardada.

El ZIP de distribución contiene la DLL y una carpeta con cinco archivos DAT y tres INI. Tras la instalación, deberías tener:

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

Los archivos Markdown y las capturas de pantalla del repositorio público **no** son necesarios para el juego. Mantén la DLL **directamente en Plugins**, con su carpeta al lado.

**Cambiar los ajustes más tarde:** abre **Opciones de juego → Ministerio de Optimización**, usa las páginas **Servicios** y **Demolición** y pulsa **Aceptar**. Para el interruptor general también puedes usar la ordenanza **Crear un Ministerio de Optimización**, sin coste mensual. Desactivar el Ministerio detiene las acciones futuras; no deshace la financiación, los contratos ni las demoliciones ya aplicados.

<a id="features"></a>

## Qué hace el Ministerio

La tabla siguiente describe lo que hace el Ministerio. Para las fórmulas, los ejemplos, las salvaguardas y la mecánica detrás de sus decisiones, consulta la **[referencia detallada de funciones](technical_docs/es/index.md)**.

| Área | Qué hace |
|---|---|
| [Educación y sanidad](technical_docs/es/education_healthcare.md#education-healthcare) | Ajusta la financiación **edificio por edificio** de las escuelas, universidades, bibliotecas, museos, clínicas y hospitales elegibles. Sigue la asistencia y la capacidad reales, con un margen de seguridad. **No cambia los radios de cobertura** ni la financiación de los autobuses escolares y las ambulancias. Si un edificio sigue saturado con la financiación al máximo, un asesor avisa de que hace falta otro centro; **tú eliges dónde construirlo**. |
| [Electricidad](technical_docs/es/power.md#power) | Gestiona **cada central eléctrica por separado** y elige la financiación que cubre las necesidades de la ciudad y la reserva de seguridad al menor coste modelado. Tiene en cuenta el rendimiento y el estado de las centrales; un ajuste opcional incluye también el coste económico de su desgaste. |
| [Agua](technical_docs/es/water.md#water) | Ajusta la financiación del agua de la ciudad para cubrir la demanda más una reserva de seguridad. La protección de las tuberías viene activada por defecto, para evitar recortes que dejarían que la red se deteriorase. También oculta el icono engañoso de «sin agua» del juego sobre las granjas. |
| [Limpieza y basura](technical_docs/es/garbage.md#garbage-landfills) | Ajusta la financiación de la limpieza según la producción de basura, el reciclaje, el tratamiento, el almacenamiento de los vertederos y los contratos regionales, protegiendo siempre las necesidades propias de la ciudad. |
| [Acuerdos con vecinos](technical_docs/es/deals.md#neighbor-deals) | Puede **comprar o vender electricidad y agua**, **pagar a los vecinos para que se queden tu basura** o **aceptar la suya a cambio de dinero**. Crea, ajusta y cancela contratos elegibles cuando el resultado global beneficia a la ciudad y sus propias necesidades y reservas siguen protegidas. |
| [Demolición automática](technical_docs/es/demolition.md#demolition) | Puede despejar ruinas pasados unos **30 días simulados** y edificios elegibles abandonados durante **un año simulado**, por defecto. Tú decides qué zonas y qué motivos de abandono cuentan, si se pueden despejar las ruinas protegidas y si quieres fijar un límite de gasto mensual. |
| [Avisos de los asesores](technical_docs/es/safety.md#notices) | Usa los propios asesores del juego para señalar carencias demostradas y persistentes que ni la financiación ni los acuerdos disponibles pueden resolver. Cuando procede, los mensajes incluyen enlaces que llevan a los edificios afectados. |
| [Consejo sobre el tamaño del vertedero](technical_docs/es/garbage.md#garbage-landfills-advice) | El consejo opcional indica una ampliación concreta del vertedero, o una reducción segura, que podría recortar la factura de la limpieza en más del umbral de ahorro elegido. **El Ministerio aconseja; tú decides si cambias la zonificación.** |
| [Reparación de casillas fantasma](technical_docs/es/garbage.md#ghost-tiles) | Con la gestión de la limpieza activada, repara con seguridad las casillas de vertedero invisibles elegibles que el juego sigue contando en el mantenimiento. Los casos inseguros o sin verificar se dejan como están; esta reparación no tiene un interruptor propio. |

Sigues controlando la construcción, la zonificación, los impuestos, las carreteras y el transporte, la policía, los bomberos y cualquier otro servicio que el Ministerio no gestione. Si activas la gestión de un tipo de acuerdo con vecinos, el Ministerio también puede modificar o cancelar un contrato que hubieras hecho a mano. Desactiva esa opción de acuerdos para gestionar tú mismo esos contratos.

<a id="first-time-setup"></a>

## Configuración inicial

El Ministerio usa los ajustes que vienen con el plugin y **empieza a trabajar automáticamente**. No tienes que activarlo, pero conviene que revises lo que se le permite hacer antes de usar una ciudad guardada importante.

1\. Inicia SimCity 4 y, desde la **vista de región**, abre **Opciones de juego → Ministerio de Optimización**. Si lo prefieres, usa una ciudad nueva o desechable para echar un primer vistazo.

2\. En la página **Servicios**, elige qué presupuestos y acuerdos con vecinos puede gestionar el Ministerio y cuánta capacidad de sobra debe mantener.

3\. En la página **Demolición**, comprueba las reglas para despejar ruinas y edificios abandonados. **Desactiva la demolición automática si prefieres derribarlo todo tú mismo.**

4\. Pulsa **Aceptar** para guardar tus elecciones. **Cancelar** cierra la ventana sin guardar los cambios.

5\. Abre la ciudad que quieres jugar. El Ministerio sigue tus preferencias guardadas y gestiona la ciudad mientras avanza la simulación.

Tus preferencias se guardan en los archivos INI del plugin y se reutilizan al abrir otra ciudad. El Ministerio **no** construye escuelas, hospitales, centrales eléctricas ni instalaciones de agua por ti. Cuando la financiación y los acuerdos disponibles no bastan para resolver una carencia demostrada y persistente, un asesor del juego te dice dónde hace falta más capacidad.

También puedes activar o desactivar el Ministerio desde las ordenanzas de la ciudad.

<a id="settings-explained"></a>

## Ajustes

Abre **Opciones de juego → Ministerio de Optimización**. La ventana tiene dos páginas, **Servicios** y **Demolición**. Las tablas siguientes cubren **todos los ajustes que muestra el menú**, sin las fórmulas técnicas. Haz clic en el nombre de una opción para leer su explicación completa en la [referencia de funciones](technical_docs/es/options.md#options).

**Cómo funcionan los controles:** «Activado» significa que el Ministerio puede gestionar esa función. Un ajuste que depende de otro aparece en gris mientras el principal está desactivado. Los ajustes en porcentaje controlan márgenes de seguridad y umbrales, y ayudan al Ministerio a mantener capacidad de reserva y a absorber las fluctuaciones de la demanda.

### Servicios

| Opción | Por defecto | Qué significa |
|---|---|---|
| [Ministerio activo](technical_docs/es/options.md#opt-master) | **Activado** | Interruptor general de la financiación, los contratos con vecinos, la demolición y los avisos del Ministerio. Desactivarlo no deshace los cambios ya hechos. |
| [Gestionar la educación](technical_docs/es/options.md#opt-education) | **Activado** | Ajusta automáticamente, una por una, las escuelas, universidades, bibliotecas y museos elegibles. No cambia su cobertura ni la financiación de los autobuses escolares. |
| [Margen de seguridad (educación)](technical_docs/es/options.md#opt-educationreserve) | **5%** (0–50%) | Capacidad extra por encima del uso actual de la educación. Más capacidad cuesta más. En los edificios con poca asistencia, el Ministerio puede mantener un margen adicional. Por ejemplo, así una escuela podría pasar de 10 a 20 alumnos (un aumento del 100%) sin huelga, sobre todo al principio, cuando el coste extra es pequeño. |
| [Gestionar la sanidad](technical_docs/es/options.md#opt-health) | **Activado** | Ajusta automáticamente, uno por uno, las clínicas, hospitales y otros centros sanitarios compatibles y elegibles. No cambia la financiación de las ambulancias ni la cobertura. |
| [Margen de seguridad (sanidad)](technical_docs/es/options.md#opt-healthreserve) | **5%** (0–50%) | Funciona igual que el margen de seguridad de la educación. |
| [Gestionar la electricidad](technical_docs/es/options.md#opt-power) | **Activado** | Ajusta la financiación de cada central eléctrica para suministrar electricidad suficiente al menor coste modelado. |
| [Reserva de electricidad](technical_docs/es/options.md#opt-powerreserve) | **20%** (10–50%) | Capacidad de generación de reserva para la parte de la demanda abastecida localmente; una reserva mayor da más margen para un crecimiento rápido y reduce el riesgo de apagones, pero puede costar más. |
| [Tener en cuenta el desgaste de las centrales](technical_docs/es/options.md#opt-poweraging) | **Desactivado** | Incluye en la optimización el coste estimado del desgaste y el envejecimiento de las centrales. Puede favorecer una financiación mayor cuando el beneficio a largo plazo de conservar la central supera el coste de funcionamiento adicional. |
| [Gestionar el agua](technical_docs/es/options.md#opt-water) | **Activado** | Ajusta la financiación del departamento de aguas para abastecer la ciudad sin gastos innecesarios. |
| [Reserva de agua](technical_docs/es/options.md#opt-waterreserve) | **10%** (10–50%) | Funciona igual que la reserva de electricidad. |
| [Proteger las tuberías](technical_docs/es/options.md#opt-waterpipes) | **Activado** | Mantiene la financiación en el 50% del juego o por encima, para evitar daños en la red de tuberías. |
| [Gestionar la limpieza](technical_docs/es/options.md#opt-sanitation) | **Activado** | Ajusta la financiación del departamento de basuras para cubrir las necesidades de recogida y eliminación. También repara las casillas fantasma (un error del juego que cobra mantenimiento por casillas de vertedero ya sin zona). |
| [Reserva para tratar basura](technical_docs/es/options.md#opt-sanitationreserve) | **2%** (2–20%) | Capacidad de tratamiento adicional para la basura de la ciudad, además de lo que produce tras el reciclaje y de cualquier atraso acumulado. |
| [Avisar si revisar el vertedero puede ahorrarme dinero](technical_docs/es/options.md#opt-landfilladvice) | **Desactivado** | Activa el consejo opcional sobre cambios en la zonificación del vertedero cuando eso ahorra dinero y la basura de la ciudad sigue tratándose. Requiere la gestión de la limpieza. |
| [Ahorro mínimo para avisos del vertedero](technical_docs/es/options.md#opt-landfillthreshold) | **25%** (20–50%) | Solo te pide que valores un cambio de tamaño del vertedero si el ahorro mensual modelado alcanza como mínimo esta parte del presupuesto de la limpieza. |
| [Gestionar acuerdos con vecinos](technical_docs/es/options.md#opt-deals) | **Activado** | Activa los contratos regionales automáticos; los tres interruptores individuales de abajo deciden qué recursos se pueden comerciar. |
| [Comprar a una ciudad para vender a otra](technical_docs/es/options.md#opt-arbitrage) | **Desactivado** | Permite combinaciones rentables de compra y venta del mismo recurso entre vecinos, dando siempre prioridad a las necesidades de la ciudad. |
| [Gestionar acuerdos de electricidad](technical_docs/es/options.md#opt-tradepower) | **Activado** | Permite al Ministerio crear, modificar o cancelar **importaciones y exportaciones** de electricidad. Independiente de la financiación de las centrales. |
| [Gestionar acuerdos de agua](technical_docs/es/options.md#opt-tradewater) | **Activado** | Permite al Ministerio crear, modificar o cancelar **importaciones y exportaciones** de agua. Independiente de la financiación del agua. |
| [Gestionar acuerdos de basura](technical_docs/es/options.md#opt-tradegarbage) | **Activado** | Permite al Ministerio concertar el tratamiento de basura **en ambos sentidos**. Independiente de la financiación de la limpieza. |

### Demolición

| Opción | Por defecto | Qué significa |
|---|---|---|
| [Demoler edificios destruidos](technical_docs/es/options.md#opt-demolishdestroyed) | **Activado** | Demuele las ruinas pasado un tiempo. No despeja automáticamente las ruinas municipales o históricas protegidas, salvo que la opción de más abajo lo permita. |
| [Tiempo de espera (edificios destruidos)](technical_docs/es/options.md#opt-destroyedmonths) | **1 mes** (1–24) | Cuánto tiempo deben permanecer las ruinas antes de poder despejarse; unos **30 días simulados** por defecto. |
| [Demoler edificios municipales e históricos destruidos](technical_docs/es/options.md#opt-demolishmunicipal) | **Desactivado** | Si se activa, despeja también, pasada la espera, las **ruinas** elegibles de edificios municipales, especiales e históricos. |
| [Demoler edificios abandonados](technical_docs/es/options.md#opt-demolishabandoned) | **Activado** | Despeja los edificios abandonados elegibles pasado el tiempo de espera, en los tipos de zona y por los motivos seleccionados. |
| [Tiempo de espera (edificios abandonados)](technical_docs/es/options.md#opt-abandonedmonths) | **12 meses** (1–60) | Cuánto tiempo debe permanecer abandonado un edificio antes de poder demolerse; un año simulado por defecto. |
| [Zona residencial](technical_docs/es/options.md#opt-zoneresidential) | **Activado** | Incluye casas y bloques de viviendas abandonados. |
| [Zona comercial](technical_docs/es/options.md#opt-zonecommercial) | **Activado** | Incluye tiendas y oficinas abandonadas. |
| [Zona industrial](technical_docs/es/options.md#opt-zoneindustrial) | **Activado** | Incluye fábricas y otros edificios industriales abandonados. |
| [Zona agrícola](technical_docs/es/options.md#opt-zoneagriculture) | **Desactivado** | Incluye granjas abandonadas. Desactivada por defecto porque las casillas vacías o abandonadas de una granja pueden ser normales. |
| [Motivo: zona poco atractiva](technical_docs/es/options.md#opt-reasonlowdesirability) | **Activado** | Incluye el abandono causado por poco atractivo, como la contaminación o la falta de servicios. |
| [Motivo: trayecto al trabajo demasiado largo](technical_docs/es/options.md#opt-reasoncommute) | **Activado** | Incluye el abandono causado por trayectos excesivamente largos al trabajo. |
| [Motivo: falta de demanda](technical_docs/es/options.md#opt-reasonlowdemand) | **Activado** | Incluye edificios abandonados porque la demanda de ese tipo ha desaparecido. |
| [Motivo: falta de electricidad](technical_docs/es/options.md#opt-reasonnopower) | **Desactivado** | Incluye, de forma opcional, edificios abandonados por falta de electricidad. Suele ser mejor restablecer primero la electricidad y ver si el edificio se vuelve a ocupar. |
| [Motivo: falta de agua](technical_docs/es/options.md#opt-reasonnowater) | **Desactivado** | Incluye, de forma opcional, edificios abandonados por falta de agua. Suele ser mejor restablecer primero el agua y ver si el edificio se vuelve a ocupar. |
| [Limitar el presupuesto mensual de demolición](technical_docs/es/options.md#opt-budgetlimit) | **Desactivado** | Fija un límite de gasto opcional para la demolición automática, además de los fondos disponibles en la tesorería de la ciudad. |
| [Presupuesto mensual](technical_docs/es/options.md#opt-demolitionbudget) | **§1.000** (§0–§200.000) | Gasto máximo en demoliciones por mes simulado **cuando el límite de presupuesto está activado**. |

<a id="screenshots"></a>

## Capturas de pantalla

Todas las capturas de pantalla muestran la versión inglesa del juego.

| | |
|---|---|
| ![La página Servicios de los ajustes del Ministerio](screenshots/Options-1.png) | ![La página Demolición de los ajustes del Ministerio](screenshots/Options-2.png) |
| *La página Servicios: cada servicio, su reserva y los acuerdos con vecinos.* | *La página Demolición: ruinas, edificios abandonados y el presupuesto mensual.* |
| ![Un aviso de asesor sobre institutos saturados](screenshots/high_school.png) | ![Un aviso de asesor sobre museos saturados](screenshots/museum.png) |
| *Escuelas con la financiación al máximo y aun así saturadas: el aviso enlaza con los edificios.* | *Lo mismo para los museos.* |
| ![Un aviso de asesor sobre escasez de agua](screenshots/water.png) | ![Un aviso de asesor sobre falta de capacidad para la basura](screenshots/real-garbage-paused.png) |
| *Agua: el Ministerio no puede resolver la escasez con el suministro disponible.* | *Basura: la ciudad necesita más capacidad de eliminación.* |

<a id="requirements-and-compatibility"></a>

## Requisitos y compatibilidad

- **SimCity 4 Deluxe, edición de Steam, versión del juego 1.1.641.0, en Windows.** He probado el Ministerio en esta versión; las demás versiones no se han probado.

- **No hace falta ningún otro mod, instalador ni aplicación externa.**

- **Probado y compatible con estos mods** (versiones probadas):

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

- **Recomendado:** añade `-CPUCount:1` en **Steam → Biblioteca → SimCity 4 Deluxe → Propiedades → Opciones de lanzamiento**. Esto ayudó a la estabilidad en sesiones largas en mi ordenador.

- **Diez idiomas en el juego:** inglés, portugués, francés, alemán, español, italiano, neerlandés, danés, sueco y polaco. El Ministerio sigue automáticamente el idioma de tu juego.

<a id="updating-and-uninstalling"></a>

## Actualizar y desinstalar

**Conserva tus preferencias.** Los tres archivos INI de la carpeta `Department of Efficiency` guardan tus ajustes. Antes de reemplazar el mod, haz una copia de `DepartmentEfficiency.ini`, `DepartmentEfficiencyUtility.ini` y `DepartmentEfficiencyDemolition.ini` fuera de tu carpeta Plugins.

**Para reemplazar una instalación:** cierra el juego, elimina la `DepartmentEfficiency.dll` y la carpeta `Department of Efficiency` existentes, instala el nuevo paquete y restaura tus archivos INI copiados **solo si su formato es compatible** con la nueva versión. En caso contrario, vuelve a definir tus preferencias en el juego.

**Para desinstalar:** cierra el juego y elimina `DepartmentEfficiency.dll` y la carpeta `Department of Efficiency`. Tus ciudades guardadas siguen siendo legibles sin el mod. Los niveles de financiación, los contratos existentes y las demoliciones ya guardadas en una ciudad no se revierten automáticamente.

<a id="known-limitations"></a>

## Limitaciones conocidas

- **Compatibilidad:** he probado la versión **1.1.641.0** de SimCity 4 Deluxe para Windows (Steam). Las demás versiones del juego no se han probado.

- **Consejo sobre el vertedero:** requiere exactamente una zona de vertedero. En ciudades con varios vertederos, el Ministerio aún puede reparar las «casillas fantasma» elegibles, pero no recomienda cambios de tamaño.

<a id="source-code"></a>

## Código fuente

El código fuente del plugin está en la carpeta [`source/`](source/). Se compila con CMake y Visual Studio en una DLL de 32 bits (Win32). Necesitas Visual Studio 2026 con la carga de trabajo de escritorio con C++ (otras versiones no se han probado), CMake 3.20 o posterior y Git:

```bat
cmake -S . -B build -G "Visual Studio 18 2026" -A Win32
cmake --build build --config Release
```

La compilación descarga [gzcom-dll](https://github.com/nsgomez/gzcom-dll) en una revisión fija (`779b669bf88112146d58fba6c7ed44f13ac6ce7d`). Para usar una copia modificada de esa biblioteca, o para compilar sin conexión a Internet, añade `-DFETCHCONTENT_SOURCE_DIR_GZCOM_DLL=<carpeta>` al primer comando. La DLL se crea en `build\Release\DepartmentEfficiency.dll`. Una DLL recompilada no es idéntica byte a byte a la publicada.

<a id="license"></a>

## Licencia

- El código original de Department of Efficiency (los archivos de [`source/`](source/)) fue escrito por mí, **Sammu89** y se publica bajo la **GNU General Public License, versión 3 solamente** (GPL-3.0-only). El texto completo está en [LICENSE](LICENSE). Puedes estudiarlo, modificarlo y redistribuirlo según la GPLv3: conserva los avisos de copyright y los créditos, indica tus cambios y publica tu versión bajo la misma licencia. Por favor, no presentes versiones modificadas como mis versiones oficiales.

- La documentación (estos README y los archivos de [`technical_docs/`](technical_docs/)) y los textos escritos para este proyecto (incluidos los de los tres archivos INI y los compilados en `DepartmentEfficiency.dat`), junto con sus traducciones originales, se publican bajo la misma licencia, GPL-3.0-only.

- **gzcom-dll** no está cubierta por esta licencia: conserva la suya, LGPL-2.1-or-later ([licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt)), y se descarga de su repositorio original al compilar.

- **SimCity 4** y sus recursos originales pertenecen a Electronic Arts / Maxis. **Department of Efficiency** es un mod no oficial hecho por un aficionado, independiente de EA y sin su respaldo. Cuatro de los archivos de `release/Department of Efficiency/` contienen scripts y definiciones de interfaz derivados de los recursos del propio juego, y las capturas de pantalla muestran el juego: no son mi propiedad, no están cubiertos por la GPL y todos los derechos sobre ellos pertenecen a sus propietarios. Lo mismo vale para cualquier otro material de terceros.

<a id="credits"></a>

## Créditos

**Autor**

- Creado por **Sammu89**.

**Biblioteca utilizada**

- **gzcom-dll**, de Nelson Gomez (simmaster07), con contribuciones de Nicholas Hayes (Null 45), Casper Van Gheluwe, memo y otros (consulta su [repositorio](https://github.com/nsgomez/gzcom-dll)), usada bajo la licencia LGPL-2.1-or-later y enlazada estáticamente en la DLL. La biblioteca no se incluye: la compilación descarga la revisión exacta de su repositorio original (consulta [Código fuente](#source-code)); los avisos de copyright y el texto de la licencia están en [licenses/gzcom-dll.txt](licenses/gzcom-dll.txt) y [licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt). Puedes modificar la biblioteca y recompilar el plugin con tu versión.

**Técnicas, documentación y ejemplos consultados** (no se copió código de ellos; sus autores no son coautores de Department of Efficiency):

- Nicholas Hayes (0xC0000054 / Null 45), por su investigación, documentación y ejemplos abiertos sobre las DLL de SimCity 4: sc4-dll-utilities, sc4-custom-budget-departments, sc4-query-ui-hooks, sc4-lua-extensions, sc4-lua-extension-example. La ventana de opciones, la ordenanza y el archivo de avisos siguen mecanismos publicados en sus proyectos sc4-region-census, sc4-humane-society-ordinance y sc4-lua-extension-example (licencia MIT); los comandos del juego siguen sc4-auto-run-cheats y sc4-game-command-documentation.

- CorinaMarie, por documentar la técnica de Urgent Advice de SimCity 4 que siguen los avisos de asesor del mod.

- La comunidad de modding de SimCity 4, por dos décadas de investigación, herramientas y conocimiento compartido, incluida la documentación de propiedades de exemplars de SC4Devotion.

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
