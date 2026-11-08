[English](../en/options.md) | [Português](../pt/options.md) | [Français](../fr/options.md) | **Español**

[Volver al contenido](index.md#contents) | [Volver al README](../../README.es.md)

<a id="options"></a>
## H. Referencia de opciones

Este capítulo explica cómo abrir los ajustes del Ministerio y cómo se guardan los cambios de configuración. Las descripciones detalladas de cada opción se ofrecen en las secciones siguientes.

<a id="options-open"></a>
### Abrir la ventana de opciones

Para acceder a los ajustes del Ministerio:

1. Abre **Opciones de juego** en SimCity 4, ya sea mientras juegas una ciudad o desde la vista de región.
2. Haz clic en **Ministerio de Optimización**, el botón añadido debajo del botón original **Créditos**.
3. La ventana de Opciones de juego se cierra y la ventana de ajustes del Ministerio se abre en el centro de la pantalla.

La ventana de ajustes tiene dos páginas:

| Página | Qué contiene |
|---|---|
| **Servicios** | Financiación automática, márgenes de reserva, contratos regionales y consejos sobre el tamaño del vertedero. |
| **Demolición** | Reglas de demolición automática, periodos de espera, tipos de parcela protegidos y límites de gasto en demoliciones. |

Cada ajuste aparece como una fila con su etiqueta y una casilla o un control numérico.

**Pasa el ratón sobre la etiqueta o la casilla de un ajuste para ver su información emergente.**

#### ¿Por qué algunos ajustes aparecen en gris?

Algunos ajustes dependen de que otra opción esté activada.

Por ejemplo, **Ahorro mínimo para avisos del vertedero** no se puede modificar mientras el consejo sobre el tamaño del vertedero esté desactivado.

Cuando una opción principal está desactivada:

- Sus ajustes dependientes aparecen en gris.
- No puedes cambiar esos ajustes.
- Sus valores actuales se conservan.
- Vuelven a estar disponibles cuando se activa la opción principal.

**Desactivar una función no borra sus ajustes anteriores.**

Actualmente cada opción tiene como máximo una dependencia de otra opción principal.

<a id="options-accept"></a>
### Aceptar y Cancelar

La ventana de ajustes ofrece dos acciones.

#### Aceptar

Al hacer clic en **Aceptar** se guardan tus cambios.

El Ministerio:

1. Identifica qué ajustes has cambiado.
2. Actualiza solo esos ajustes en los archivos de configuración correspondientes.
3. Conserva los ajustes no relacionados y los comentarios existentes.
4. Verifica que los archivos actualizados se pueden volver a leer con exactamente los valores seleccionados.
5. Sustituye cada archivo de configuración como una operación completa, y solo después de que la validación tenga éxito.
6. Aplica los nuevos ajustes de inmediato.

**Si la validación falla, no se escribe ningún cambio y la ventana de ajustes permanece abierta.**

Así se evita que una configuración no válida se guarde a medias.

#### Cancelar

Al hacer clic en **Cancelar** se cierra la ventana de ajustes sin guardar ningún cambio.

Cerrar la ciudad o salir de SimCity 4 también cierra la ventana del Ministerio sin escribir los ajustes sin guardar.

#### La ordenanza del Ministerio

El juego también incluye la ordenanza:

**Crear un Ministerio de Optimización**

Aparece en la ventana normal de Ordenanzas de SimCity 4 y tiene un coste mensual de **§0**.

Esta ordenanza controla el mismo ajuste que **Ministerio activo**.

- **Promulgar la ordenanza** activa el Ministerio.
- **Derogar la ordenanza** lo desactiva.

Ambos métodos actualizan el mismo ajuste, por lo que siempre son coherentes entre sí.

**Puedes activar o desactivar el Ministerio desde cualquiera de los dos lugares sin cambiar las preferencias de cada función que ya hayas configurado.**
<a id="options-files"></a>
### ¿Dónde se guardan los ajustes?

El Ministerio guarda sus ajustes en **tres archivos de configuración de texto**, situados dentro de la carpeta `Department of Efficiency` de tu directorio Plugins de SimCity 4.

Puedes cambiar la mayoría de los ajustes desde la ventana de opciones del Ministerio en el juego, o editar los archivos a mano.

| Archivo de configuración | Qué controla |
|---|---|
| `DepartmentEfficiency.ini` | La financiación de educación y sanidad, los márgenes de seguridad y el interruptor principal del Ministerio. |
| `DepartmentEfficiencyUtility.ini` | Electricidad, agua, limpieza, consejos sobre el vertedero, contratos regionales y sus opciones relacionadas. |
| `DepartmentEfficiencyDemolition.ini` | Demolición automática, periodos de espera, zonas elegibles, motivos de abandono y gasto en demoliciones. |

Cada archivo tiene su propia sección de configuración.

#### Educación y sanidad

Archivo: `DepartmentEfficiency.ini`

Sección: `[DepartmentEfficiency]`

| Ajuste | Clave INI |
|---|---|
| Gestionar la educación | `Education` |
| Gestionar la sanidad | `Health` |
| Margen de seguridad de educación | `EducationReservePercent` |
| Margen de seguridad de sanidad | `HealthReservePercent` |
| Ministerio activo | `Mode` |

La configuración suministrada usa `ReservePercent=5` tanto para el margen de seguridad de educación como para el de sanidad.

#### Suministros y contratos regionales

Archivo: `DepartmentEfficiencyUtility.ini`

Sección: `[DepartmentEfficiencyUtility]`

| Ajuste | Clave INI |
|---|---|
| Gestionar la electricidad | `AutoPower` |
| Reserva de electricidad | `PowerReservePercent` |
| Tener en cuenta el desgaste de las centrales | `ConsiderPowerAging` |
| Gestionar el agua | `AutoWater` |
| Reserva de agua | `WaterReservePercent` |
| Proteger las tuberías | `ProtectWaterPipes` |
| Gestionar la limpieza | `AutoGarbage` |
| Reserva para tratar basura | `GarbageReservePercent` |
| Consejo sobre el tamaño del vertedero | `WarnLandfillSizeSavings` |
| Ahorro mínimo para avisos del vertedero | `LandfillSavingsAlertThreshold` |
| Gestionar acuerdos con vecinos | `AutoDeals` |
| Comprar a una ciudad para vender a otra | `AllowUtilityArbitrage` |
| Gestionar acuerdos de electricidad | `AutoTradePower` |
| Gestionar acuerdos de agua | `AutoTradeWater` |
| Gestionar acuerdos de basura | `AutoTradeGarbage` |
| Ministerio activo | `Mode` |

#### Demolición automática

Archivo: `DepartmentEfficiencyDemolition.ini`

Sección: `[LotDemolition]`

| Ajuste | Clave INI |
|---|---|
| Demoler edificios destruidos | `DemolishDestroyed` |
| Tiempo de espera (ruinas) | `DestroyedAfterDays` |
| Demoler edificios municipales e históricos destruidos | `DemolishMunicipalAndHistoricRuins` |
| Demoler edificios abandonados | `DemolishAbandoned` |
| Tiempo de espera (abandonados) | `AbandonedAfterDays` |
| Tipos de zona elegibles | `AbandonedZones` |
| Motivos de abandono elegibles | `AbandonedReasons` |
| Limitar el presupuesto mensual de demolición | `LimitMonthlyBudget` |
| Presupuesto mensual | `MonthlyBudget` |
| Ministerio activo | `Mode` |

#### ¿Puedo editar estos archivos a mano?

Sí, pero **los valores de configuración deben respetar el formato admitido y los rangos permitidos**.

El Ministerio comprueba cada archivo antes de usarlo.

Un ajuste desconocido o un valor no válido hace que el archivo de configuración afectado sea inválido, y se desactiva esa parte del Ministerio en lugar de aplicar ajustes inciertos.

Los ajustes omitidos en los archivos suministrados usan sus valores por defecto.

Por ejemplo, la configuración de demolición por defecto activa la eliminación de ruinas y de edificios abandonados elegibles, y protege las parcelas agrícolas y los edificios abandonados por falta de electricidad o de agua.

**Las ediciones manuales se detectan y se aplican normalmente en el plazo de un mes simulado.**

Para los cambios habituales, la ventana de opciones del juego es más segura, porque comprueba los valores permitidos antes de guardarlos.

---

<a id="options-services"></a>
### Página Servicios

La página **Servicios** contiene la financiación automática, las reservas de seguridad, los contratos regionales y los consejos sobre el vertedero.

La primera opción controla todo el Ministerio. Las demás te permiten decidir qué servicios debe gestionar.

<a id="opt-master"></a>
### Ministerio activo

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `Mode`</text>

**Este es el interruptor principal de todo el Ministerio.**

Cuando está activado, cada función automática sigue su propio ajuste individual.

Por ejemplo, puedes dejar el Ministerio activo y desactivar la gestión del agua o la demolición automática.

Cuando está desactivado, el Ministerio deja de:

- Ajustar la financiación de educación, sanidad y suministros.
- Crear, modificar o cancelar contratos regionales.
- Demoler edificios y ruinas.
- Reparar casillas fantasma de vertedero.
- Calcular recomendaciones sobre el tamaño del vertedero y avisos de carencia.

**Desactivar el Ministerio no deshace los cambios que ya ha hecho.**

Los niveles de financiación y los contratos regionales quedan como estaban.

Tus preferencias individuales también se conservan, de modo que vuelven a estar activas cuando reactivas el Ministerio.

Todos los demás ajustes de las páginas Servicios y Demolición dejan de estar disponibles mientras el interruptor principal esté desactivado.

**Importante:** Los archivos de recursos de asesores incluidos y la modificación que oculta los iconos de *sin agua* de las granjas siguen en vigor mientras estén instalados, incluso con el Ministerio desactivado.

También puedes controlar este mismo interruptor con **Crear un Ministerio de Optimización** en la ventana de Ordenanzas de SimCity 4.

El ajuste se guarda como `Mode=Auto` o `Mode=Off` en cada uno de los tres archivos de configuración.

Educación y sanidad funcionan cuando al menos una de sus opciones de gestión está activada. La demolición funciona cuando al menos una categoría de demolición está activada. El controlador de suministros sigue disponible mientras el Ministerio esté activo, aunque no se haya seleccionado ninguno de los suministros individuales.

Consulta [Seguridad, automatización y toma de decisiones](safety.md#safety-player-control).

<a id="opt-education"></a>
### Gestionar la educación

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `Education`</text>

Ajusta automáticamente la financiación de los centros educativos elegibles, incluidas escuelas, institutos, universidades, bibliotecas y museos.

**El Ministerio elige el porcentaje de financiación entero más bajo que puede atender a los usuarios del centro manteniendo el margen de seguridad exigido.**

Cuando está activado:

- La financiación puede aumentar durante el primer día simulado tras abrir una ciudad.
- El Ministerio puede aumentar la financiación cuando detecta un riesgo de capacidad.
- Las reducciones de financiación se evalúan en la revisión mensual.
- Un hacinamiento persistente que la financiación no puede resolver puede generar un aviso de un asesor.

Cuando está desactivado, la financiación de la educación se queda en los valores que hayas fijado a mano y los avisos de capacidad de educación se desactivan.

**Importante:** Los edificios que no admiten el mecanismo de financiación estándar del juego no se modifican. Los consejos de financiación de educación del juego original siguen suprimidos mientras el archivo de recursos de consejos cívicos esté instalado, aunque la gestión automática de la educación esté desactivada.

**Requiere:** Ministerio activo.

**Controla:** el [Margen de seguridad](#opt-educationreserve) de educación.

Consulta [Educación y sanidad](education_healthcare.md#education-healthcare).

<a id="opt-educationreserve"></a>
### Margen de seguridad (educación)

<text color="secondary">Por defecto: 5% | Rango: 0–50%, en pasos del 1% | INI: `EducationReservePercent`</text>

Determina cuánta capacidad adicional mantiene el Ministerio por encima del uso actual de cada edificio educativo.

Por ejemplo, una escuela que atiende a 1.000 alumnos con un margen del 5% tiene un requisito básico protegido de:

```math
1000\times1.05=1050
```

**Al menos 1.050 plazas.**

Un margen mayor ofrece más espacio para el crecimiento y los aumentos inesperados de asistencia, pero puede requerir más financiación.

Un margen menor reduce los gastos al permitir que la capacidad se mantenga más cerca del uso actual.

**El margen de seguridad es un mínimo, no un objetivo exacto.**

El Ministerio puede mantener capacidad adicional en los edificios pequeños o en los centros cuyo uso fluctúa mucho, para ayudar a evitar huelgas.

**Requiere:** Gestionar la educación y Ministerio activo.

Consulta [Educación y sanidad](education_healthcare.md#education-healthcare-required).

<a id="opt-health"></a>
### Gestionar la sanidad

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `Health`</text>

Gestiona automáticamente la financiación de las clínicas y los hospitales elegibles.

El Ministerio selecciona el porcentaje de financiación entero más bajo que ofrece capacidad suficiente para los pacientes actuales y el margen de seguridad de sanidad configurado.

Cuando está activado:

- Los aumentos de financiación pueden producirse con rapidez cuando se detecta un riesgo de capacidad.
- Las reducciones se estudian en las revisiones mensuales.
- Las carencias persistentes de capacidad que la financiación máxima no puede resolver pueden generar avisos de los asesores.

Cuando está desactivado, la financiación de la sanidad sigue bajo tu control manual y no se generan avisos de capacidad sanitaria.

**El Ministerio nunca cambia la partida presupuestaria independiente de las ambulancias.**

Se excluyen los edificios que no admiten el mecanismo de financiación estándar.

Los consejos habituales de financiación sanitaria del juego original siguen suprimidos mientras el archivo de recursos de consejos cívicos esté instalado, sea cual sea este ajuste.

**Requiere:** Ministerio activo.

**Controla:** el [Margen de seguridad](#opt-healthreserve) de sanidad.

Consulta [Educación y sanidad](education_healthcare.md#education-healthcare).

<a id="opt-healthreserve"></a>
### Margen de seguridad (sanidad)

<text color="secondary">Por defecto: 5% | Rango: 0–50%, en pasos del 1% | INI: `HealthReservePercent`</text>

Determina la capacidad adicional de pacientes que mantiene cada centro sanitario gestionado.

Por ejemplo, un hospital que atiende ahora a 2.000 pacientes con un margen de seguridad del 5% necesita al menos:

```math
2000\times1.05=2100
```

**2.100 plazas de capacidad.**

Aumentar el margen ofrece más espacio para el crecimiento y las fluctuaciones de pacientes, pero puede aumentar los gastos de financiación.

Reducirlo permite una financiación menor cuando el uso real es estable.

**Igual que en educación, es un requisito mínimo.** El Ministerio puede mantener más capacidad cuando sus cálculos de seguridad lo exigen.

**Requiere:** Gestionar la sanidad y Ministerio activo.

Consulta [Educación y sanidad](education_healthcare.md#education-healthcare-required).

<a id="opt-power"></a>
### Gestionar la electricidad

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `AutoPower`</text>

Ajusta automáticamente la financiación de cada central eléctrica.

En lugar de financiar todas las centrales con el mismo porcentaje, el Ministerio busca **la combinación más barata de niveles de financiación de las centrales** que pueda cubrir:

- Las necesidades de electricidad de tu ciudad.
- La reserva de electricidad configurada.
- Cualquier venta de electricidad activa a ciudades vecinas.

También tiene en cuenta las compras regionales de electricidad disponibles.

Cuando está activado, una revisión completa se realiza normalmente cada **10 días simulados**, con evaluaciones anteriores si se confirman déficits de suministro o hay cambios importantes en la demanda o la capacidad.

Cuando está desactivado, la financiación de las centrales se queda en sus valores actuales.

**Los contratos de electricidad se controlan por separado.** Se pueden seguir gestionando si Gestionar acuerdos de electricidad está activado.

El Ministerio solo cambia la financiación. Nunca construye centrales eléctricas adicionales.

Si las necesidades de electricidad de la ciudad no se pueden cubrir con las instalaciones y los contratos disponibles, una carencia persistente puede generar un aviso de un asesor.

**Requiere:** Ministerio activo.

**Controla:** [Reserva de electricidad](#opt-powerreserve) y [Tener en cuenta el desgaste de las centrales](#opt-poweraging).

Consulta [Electricidad](power.md#power).

<a id="opt-powerreserve"></a>
### Reserva de electricidad

<text color="secondary">Por defecto: 20% | Rango: 10–50%, en pasos del 1% | INI: `PowerReservePercent`</text>

Determina la capacidad adicional de producción de electricidad que el Ministerio protege frente a los cambios en la demanda.

Sin compras ni ventas, el requisito local protegido es, como mínimo, el mayor de estos valores:

- El requisito de electricidad del propio juego.
- La demanda de electricidad multiplicada por **1 + reserva**.

Por ejemplo, con una reserva del 20%, una ciudad que demanda 10.000 unidades necesita normalmente al menos 12.000 unidades de capacidad de generación, salvo que el requisito del propio juego sea mayor.

Una reserva mayor ofrece mejor protección frente a aumentos repentinos de la demanda de electricidad, pero puede aumentar los gastos de funcionamiento.

Una reserva menor reduce los costes, pero deja menos capacidad de sobra antes de que pueda producirse una carencia.

**La reserva es un mínimo, no un límite de producción.**

El Ministerio puede mantener capacidad adicional cuando unas ventas de electricidad rentables lo justifican.

**Requiere:** Gestionar la electricidad y Ministerio activo.

Consulta [Electricidad](power.md#power).

<a id="opt-poweraging"></a>
### Tener en cuenta el desgaste de las centrales

<text color="secondary">Por defecto: Desactivado | Valores: Activado / Desactivado | INI: `ConsiderPowerAging`</text>

Permite que el Ministerio tenga en cuenta **el desgaste y el envejecimiento de las centrales eléctricas** al comparar distintas formas de financiar la electricidad.

Cuando está desactivado, el optimizador se centra en los gastos mensuales de funcionamiento.

Cuando está activado, el cálculo también considera el coste económico del desgaste de las centrales.

A veces esto puede hacer que merezca la pena gastar algo más cada mes para reducir el desgaste y alargar la vida útil de una central.

**Este desgaste estimado es un coste económico, no un cargo mensual adicional en tu tesorería.**

Las necesidades de electricidad de la ciudad y la reserva configurada quedan protegidas en ambos modos.

Si Gestionar la electricidad está desactivado, la preferencia sobre el desgaste se conserva, pero no tiene efecto hasta que se vuelva a activar la gestión de la electricidad.

**Requiere:** Gestionar la electricidad y Ministerio activo.

Consulta [Electricidad](power.md#power).

<a id="opt-water"></a>
### Gestionar el agua

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `AutoWater`</text>

Gestiona automáticamente la financiación del departamento del agua.

A diferencia de la electricidad, el agua usa **un único deslizador de financiación del departamento** para sus bombas y sus instalaciones de tratamiento.

El Ministerio elige un nivel de financiación que ofrezca capacidad de agua suficiente para la demanda de la ciudad y la reserva configurada, teniendo en cuenta los contratos regionales.

Cuando está activado, la financiación del agua se revisa:

- Cada 10 días simulados desde la evaluación anterior.
- En cada cambio de mes simulado.
- Antes, cuando se detecta una carencia regional o un cambio importante.

Cuando está desactivado, la financiación del agua no cambia.

Los contratos regionales de agua se pueden seguir gestionando de forma independiente mediante Gestionar acuerdos de agua.

**El Ministerio no reduce la financiación del agua mientras haya edificios sin agua, según las condiciones de seguridad aplicables del gestor del agua.**

**Requiere:** Ministerio activo.

**Controla:** [Reserva de agua](#opt-waterreserve) y [Proteger las tuberías](#opt-waterpipes).

Consulta [Agua](water.md#water).

<a id="opt-waterreserve"></a>
### Reserva de agua

<text color="secondary">Por defecto: 10% | Rango: 10–50%, en pasos del 1% | INI: `WaterReservePercent`</text>

Determina cuánta capacidad de agua adicional mantiene el Ministerio por encima de la demanda de la ciudad.

Por ejemplo, con una reserva del 10%, una ciudad que demanda 50.000 unidades de agua necesita al menos:

```math
50000\times1.10=55000
```

**55.000 unidades de capacidad protegida**, antes de tener en cuenta los efectos detallados de los contratos.

Una reserva mayor ofrece más protección frente al aumento de la demanda de agua, pero puede aumentar los gastos.

Una reserva menor permite un funcionamiento más barato, con menos capacidad de sobra.

**La reserva es un requisito mínimo.** El Ministerio puede mantener más capacidad cuando es necesario.

**Requiere:** Gestionar el agua y Ministerio activo.

Consulta [Agua](water.md#water-required-capacity).

<a id="opt-waterpipes"></a>
### Proteger las tuberías

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `ProtectWaterPipes`</text>

Protege la red de distribución de agua frente al deterioro causado por una financiación insuficiente.

En SimCity 4, mantener el departamento del agua por debajo de ciertos niveles de financiación puede contribuir a que se dañen las tuberías.

Cuando esta opción está activada, el Ministerio:

- Nunca reduce la financiación del agua por debajo del **50%**.
- No reduce la financiación mientras la red de tuberías esté dañada.
- Evita los niveles de financiación en los que la curva de mantenimiento de tuberías del juego permite que la red se deteriore.

**El objetivo es ahorrar dinero sin dejar que la red de agua se degrade por falta de financiación.**

Cuando está desactivada, se eliminan las restricciones especiales de protección de las tuberías.

La financiación del agua sigue sujeta a los requisitos normales de servicio y de reserva, pero el Ministerio puede elegir niveles por debajo del 50% cuando por lo demás esté permitido.

Eso puede ahorrar dinero, **a riesgo de que las tuberías se deterioren o revienten**.

La opción no aumenta por sí sola la financiación solo para reparar daños ya existentes en las tuberías; su protección sirve sobre todo para impedir reducciones inseguras.

Si Gestionar el agua está desactivado, esta preferencia se guarda pero queda inactiva.

**Requiere:** Gestionar el agua y Ministerio activo.

Consulta [Agua](water.md#water-pipe-curve).

<a id="opt-sanitation"></a>
### Gestionar la limpieza

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `AutoGarbage`</text>

Ajusta automáticamente la financiación del departamento de limpieza.

La limpieza abarca el sistema de eliminación de basura de la ciudad, incluidos los vertederos y las instalaciones de tratamiento, mediante un único deslizador de financiación del departamento.

El Ministerio tiene en cuenta:

- La basura producida después del reciclaje.
- La reserva para tratar basura configurada.
- La basura que ya está sin recoger.
- Las importaciones y exportaciones regionales de basura.
- La capacidad de tratamiento de las incineradoras y de las plantas de conversión de residuos en energía.
- El almacenamiento disponible en los vertederos y su entrada mensual.

Después busca el nivel de financiación más barato que pueda gestionar la basura prevista.

**A diferencia de la electricidad y el agua, las reducciones de financiación de la limpieza son graduales.**

Las reducciones se limitan a **25 puntos porcentuales por escritura**, con al menos **7 días simulados** entre un recorte y otro.

Los aumentos de financiación requieren al menos **3 días simulados** desde la escritura anterior de financiación de la limpieza.

El Ministerio revisa la limpieza en los cambios de mes y cuando las estadísticas de basura cambian de forma importante.

Cuando está activado, también puede realizar reparaciones automáticas seguras de casillas fantasma de vertedero.

Cuando está desactivado:

- La financiación de la limpieza no cambia.
- Se detiene la reparación de casillas fantasma.
- El consejo sobre el tamaño del vertedero no está disponible.
- Los contratos de basura se pueden seguir gestionando de forma independiente si Gestionar acuerdos de basura está activado.

**El Ministerio nunca construye ni zonifica automáticamente áreas de vertedero adicionales.** La ampliación y la reducción del vertedero son solo un consejo.

**Requiere:** Ministerio activo.

**Controla:** [Reserva para tratar basura](#opt-sanitationreserve) y [Consejo sobre el tamaño del vertedero](#opt-landfilladvice).

Consulta [Basura y vertederos](garbage.md#garbage-landfills).

<a id="opt-sanitationreserve"></a>
### Reserva para tratar basura

<text color="secondary">Por defecto: 2% | Rango: 2–20%, en pasos del 1% | INI: `GarbageReservePercent`</text>

Añade un margen de seguridad a la cantidad de basura que produce tu ciudad después del reciclaje.

La cantidad de basura protegida se calcula a partir de:

```math
\text{Basura protegida}=
(\text{Producida}-\text{Reciclada})
\times(1+\text{reserva})
```

El cálculo real de planificación incluye además cualquier basura acumulada sin recoger y los efectos de los contratos regionales.

Por ejemplo, una ciudad que produce 80.000 unidades de basura después del reciclaje tiene un requisito básico protegido de **81.600 unidades** con la reserva por defecto del 2%.

Un margen mayor ofrece más protección frente a los aumentos de la producción de basura, pero puede requerir una financiación mayor de la limpieza.

Un margen menor reduce los posibles gastos.

**La reserva es un mínimo, no un objetivo máximo de eliminación.**

**Requiere:** Gestionar la limpieza y Ministerio activo.

Consulta [Basura y vertederos](garbage.md#garbage-landfills-need).

<a id="opt-landfilladvice"></a>
### Avisar si revisar el vertedero puede ahorrarme dinero

<text color="secondary">Por defecto: Desactivado | Valores: Activado / Desactivado | INI: `WarnLandfillSizeSavings`</text>

Activa recomendaciones opcionales sobre cómo cambiar el tamaño de tu vertedero.

El Ministerio busca situaciones en las que:

- **Ampliar un vertedero** daría capacidad de eliminación adicional suficiente para reducir la financiación de limpieza necesaria y el gasto mensual total.
- **Reducir un vertedero** eliminaría casillas sin usar y bajaría los gastos de mantenimiento sin comprometer la eliminación de la basura.

Si un cambio seguro produce un ahorro superior al umbral que hayas configurado, el Ministerio emite un aviso de un asesor.

El aviso identifica el área propuesta, estima el ahorro mensual e incluye un enlace que mueve la cámara hasta ese lugar.

**Esta función solo da consejos.**

No:

- Zonifica ni deszonifica terreno.
- Construye ni demuele nada.
- Cambia la financiación a causa del consejo.
- Cambia los contratos regionales a causa del consejo.

El análisis se ejecuta **como máximo una vez por mes simulado**, sujeto a periodos de protección adicionales descritos en el capítulo del vertedero.

Solo funciona en ciudades que tienen **exactamente una zona de vertedero**.

Cuando está desactivado, no se hace ningún análisis del tamaño del vertedero ni se emite ningún aviso.

El historial de tiempos existente se conserva, de modo que desactivar y volver a activar la opción no reinicia sus periodos de espera.

**Requiere:** Gestionar la limpieza y Ministerio activo.

**Controla:** [Ahorro mínimo para avisos del vertedero](#opt-landfillthreshold).

Consulta [Basura y vertederos](garbage.md#garbage-landfills-advice).

<a id="opt-landfillthreshold"></a>
### Ahorro mínimo para avisos del vertedero

<text color="secondary">Por defecto: 25% | Rango: 20–50%, en pasos del 1% | INI: `LandfillSavingsAlertThreshold`</text>

Determina cuánto debe ahorrar un cambio propuesto en el tamaño del vertedero para que el Ministerio lo recomiende.

La comparación usa el gasto mensual optimizado de limpieza, incluido el mantenimiento.

**Umbral de aviso**

```math
\text{Ahorro}\times100>
\text{Umbral}\times\text{Gasto mensual}
```

**La comparación es estricta.**

Por ejemplo, con un gasto optimizado actual de limpieza de §1.000 y un umbral del 25%:

- Ahorro de §200: sin aviso.
- Ahorro de §250: sin aviso.
- Ahorro de §251: puede emitirse un aviso.

Un umbral más alto significa menos recomendaciones, limitadas a ahorros potenciales mayores.

Un umbral más bajo permite que se avise de oportunidades de ahorro menores.

**El aviso muestra el ahorro realmente calculado, no el umbral que has elegido.**

Cambiar este ajuste no reinicia los periodos de espera de los consejos.

Cuando el consejo sobre el tamaño del vertedero está desactivado, el control del umbral deja de estar disponible, pero su valor guardado se conserva.

**Requiere:** Consejo sobre el tamaño del vertedero, Gestionar la limpieza y Ministerio activo.

Consulta [Basura y vertederos](garbage.md#garbage-landfills-advice).

<a id="opt-deals"></a>
### Gestionar acuerdos con vecinos

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `AutoDeals`</text>

Este es el **interruptor principal de la gestión automática de contratos regionales**.

Cuando está activado, el Ministerio puede examinar las ofertas de las ciudades vecinas y gestionar los contratos de electricidad, agua y basura.

Cada recurso se controla con su propia opción.

El Ministerio puede:

- Comprar recursos cuando salen más baratos que la producción local.
- Vender capacidad realmente sobrante cuando es rentable.
- Ajustar las cantidades de los contratos existentes.
- Cancelar los contratos que ya no aportan ningún beneficio.

**Los servicios propios de tu ciudad y sus reservas de seguridad siempre tienen prioridad.**

Cuando está desactivado:

- Los contratos regionales existentes se mantienen.
- Esos contratos se siguen incluyendo en los cálculos de financiación.
- No se solicitan nuevas ofertas de contratos.
- No se crea, modifica ni cancela ningún contrato.
- No se reserva capacidad local para hipotéticas ventas futuras.
- Los avisos de carencia de suministros no se pueden demostrar del todo, porque no se examinan las ofertas regionales.

**Importante:** Los mensajes originales de los asesores de SimCity 4 sobre acuerdos con vecinos siguen suprimidos mientras el archivo de recursos de consejos sobre acuerdos del Ministerio esté instalado.

Para restaurar esos mensajes originales tras desactivar la gestión automática de contratos, quita el archivo de recursos correspondiente con el juego cerrado.

**Requiere:** Ministerio activo.

**Controla:** [Comprar a una ciudad para vender a otra](#opt-arbitrage), [Gestionar acuerdos de electricidad](#opt-tradepower), [Gestionar acuerdos de agua](#opt-tradewater) y [Gestionar acuerdos de basura](#opt-tradegarbage).

Consulta [Acuerdos con vecinos](deals.md#neighbor-deals).

<a id="opt-arbitrage"></a>
### Comprar a una ciudad para vender a otra

<text color="secondary">Por defecto: Desactivado | Valores: Activado / Desactivado | INI: `AllowUtilityArbitrage`</text>

Permite que el Ministerio **compre y venda el mismo recurso dentro de un mismo plan de contratos** cuando esa combinación produce un mejor resultado económico.

Con esta opción desactivada, cada recurso sigue uno de estos tres esquemas:

- **LOCAL:** sin compras ni ventas regionales.
- **COMPRAR:** comprar el recurso a las ciudades vecinas.
- **VENDER:** vender el recurso a las ciudades vecinas.

Con la opción activada, el Ministerio puede combinar compras y ventas.

Por ejemplo, podría comprar electricidad a un vecino para cubrir parte de la demanda de la ciudad, y dejar así la electricidad generada localmente libre para venderla con beneficio a otro vecino.

**El Ministerio nunca revende directamente los recursos importados.**

Las compras siguen limitadas por la demanda propia de la ciudad, y las reglas normales de servicio y de reserva siguen aplicándose.

En el caso de la basura, un esquema mixto podría consistir en recibir basura de pago de un vecino mientras se paga a otro vecino para que elimine parte de tus propios residuos.

Un recurso cuya opción individual de gestión de acuerdos esté desactivada nunca es objeto de intercambio por parte del Ministerio, sea cual sea este ajuste.

**Requiere:** Gestionar acuerdos con vecinos y Ministerio activo.

Consulta [Acuerdos con vecinos](deals.md#neighbor-deals-arbitrage).

<a id="opt-tradepower"></a>
### Gestionar acuerdos de electricidad

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `AutoTradePower`</text>

Permite que el Ministerio cree, modifique y cancele contratos de electricidad con las ciudades vecinas.

Cuando está activado, puede:

- Comprar electricidad cuando compensa económicamente.
- Vender el excedente de electricidad cuando es rentable.
- Reducir o cancelar las ventas que amenazan el suministro propio de la ciudad.
- Ajustar los contratos existentes a medida que cambian la demanda, la producción y los precios.

Cuando está desactivado, los contratos de electricidad existentes siguen bajo tu control manual.

El Ministerio los sigue leyendo al calcular las necesidades locales de electricidad, pero no los modifica ni reserva capacidad para futuras ventas automáticas.

**Este ajuste controla los contratos, no la financiación de las centrales.**

Funciona de forma independiente de **Gestionar la electricidad**.

Por ejemplo, puedes gestionar los contratos de electricidad de forma automática y controlar tú mismo la financiación de las centrales. En ese caso, el Ministerio debe trabajar con la capacidad de generación ya disponible.

**Requiere:** Gestionar acuerdos con vecinos y Ministerio activo.

Consulta [Acuerdos con vecinos](deals.md#neighbor-deals).

<a id="opt-tradewater"></a>
### Gestionar acuerdos de agua

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `AutoTradeWater`</text>

Permite que el Ministerio gestione las compras y ventas de agua con las ciudades vecinas.

Cuando está activado, compara los precios de los contratos con el coste de suministrar agua localmente.

Puede crear, modificar o cancelar contratos cuando hacerlo mejora la situación económica de la ciudad sin comprometer el servicio de agua ni la reserva configurada.

Cuando está desactivado, los contratos de agua existentes siguen bajo tu control manual.

**Esta opción no controla la financiación del departamento del agua.**

Es independiente de **Gestionar el agua**.

Por ejemplo, puedes dejar que el Ministerio optimice la financiación del agua y mantener sin cambios tus propios contratos regionales de agua.

**Requiere:** Gestionar acuerdos con vecinos y Ministerio activo.

Consulta [Acuerdos con vecinos](deals.md#neighbor-deals).

<a id="opt-tradegarbage"></a>
### Gestionar acuerdos de basura

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `AutoTradeGarbage`</text>

Permite que el Ministerio gestione los contratos de basura en ambos sentidos.

Recuerda que los contratos de basura funcionan de forma distinta a los de electricidad y agua:

- **Enviar basura a un vecino:** tu ciudad paga por la eliminación.
- **Recibir basura de un vecino:** el vecino paga a tu ciudad por gestionar sus residuos.

Cuando está activado, el Ministerio puede crear, modificar y cancelar estos contratos según su valor económico y la capacidad de eliminación de la ciudad.

**Las necesidades de eliminación de basura de tu propia ciudad van siempre primero.**

El Ministerio no acepta residuos adicionales solo porque otra ciudad ofrezca dinero.

Tiene que poder gestionar esa basura con seguridad usando las instalaciones de limpieza y la financiación disponibles.

Cuando está desactivado, los contratos de basura existentes siguen bajo tu control manual.

**Esta opción es independiente de Gestionar la limpieza.**

Puedes gestionar los contratos de basura sin cambiar automáticamente la financiación de la limpieza, o gestionar la financiación de la limpieza dejando los contratos sin cambios.

**Requiere:** Gestionar acuerdos con vecinos y Ministerio activo.

Consulta [Acuerdos con vecinos](deals.md#neighbor-deals-garbage).

---

<a id="options-demolition"></a>
### Página Demolición

La página **Demolición** controla cómo despeja el Ministerio los edificios destruidos y las parcelas abandonadas desde hace tiempo.

Puedes decidir:

- Si las ruinas deben retirarse automáticamente.
- Cuánto tiempo deben permanecer los edificios destruidos o abandonados.
- Qué tipos de edificios abandonados se pueden demoler.
- Qué motivos de abandono son elegibles.
- Si se pueden retirar las ruinas municipales e históricas.
- Cuánto dinero se puede gastar en cada revisión mensual de demolición.

**La demolición automática usa la orden normal de la excavadora del juego y cuesta lo mismo que la demolición manual.**

No elimina la zonificación subyacente.

<a id="opt-demolishdestroyed"></a>
### Demoler edificios destruidos

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `DemolishDestroyed`</text>

Retira automáticamente las ruinas que dejan los incendios y los desastres.

Cuando está activado, el Ministerio puede despejar los edificios destruidos en terreno residencial, comercial, industrial y agrícola una vez pasado el periodo de espera configurado.

**Las ruinas se procesan antes que los edificios abandonados en cada revisión mensual de demolición.**

Cuando está desactivado, no se demuele ninguna ruina automáticamente.

La opción independiente para las ruinas municipales e históricas también deja de surtir efecto.

Toda demolición sigue sujeta al precio de la excavadora del juego, a la tesorería disponible y al presupuesto mensual opcional.

**Requiere:** Ministerio activo.

**Controla:** [Tiempo de espera (ruinas)](#opt-destroyedmonths) y [Demoler edificios municipales e históricos destruidos](#opt-demolishmunicipal).

Consulta [Demolición automática](demolition.md#demolition-destroyed).

<a id="opt-destroyedmonths"></a>
### Tiempo de espera (ruinas)

<text color="secondary">Por defecto: 1 mes (30 días) | Rango: 1–24 meses, en pasos de 1 | INI: `DestroyedAfterDays`</text>

Determina cuánto tiempo debe permanecer en ruinas un edificio destruido antes de que el Ministerio pueda despejarlo.

La ventana de opciones muestra meses simulados, pero el Ministerio guarda el periodo de espera en días simulados.

La conversión usa:

```math
\text{Días}=
\mathrm{round}\left(\frac{\text{Meses}\times365}{12}\right)
```

Ejemplos:

| Retraso seleccionado | Días guardados |
|---|---:|
| 1 mes | 30 |
| 3 meses | 91 |
| 6 meses | 183 |
| 12 meses | 365 |
| 24 meses | 730 |

Un periodo de espera más largo te da más tiempo para inspeccionar las ruinas y decidir si actúas a mano.

Un periodo más corto permite que el terreno zonificado quede disponible antes para volver a desarrollarse.

**El Ministerio comprueba la elegibilidad para la demolición una vez por mes simulado.**

Por tanto, una ruina se despeja en la primera revisión mensual posterior a que alcance la antigüedad suficiente, no necesariamente el día exacto en que termina el periodo de espera.

El mismo retraso se aplica a las ruinas municipales e históricas si activas su opción de demolición independiente.

**Rango INI avanzado:** 1–3.650 días.

**Requiere:** Demoler edificios destruidos y Ministerio activo.

Consulta [Demolición automática](demolition.md#demolition-delays).

<a id="opt-demolishmunicipal"></a>
### Demoler edificios municipales e históricos destruidos

<text color="secondary">Por defecto: Desactivado | Valores: Activado / Desactivado | INI: `DemolishMunicipalAndHistoricRuins`</text>

Controla si el Ministerio puede despejar automáticamente las ruinas de los edificios protegidos.

Entre ellos se incluyen los edificios municipales y de propósito especial, como:

- Escuelas y hospitales.
- Comisarías y parques de bomberos.
- Monumentos y recompensas.
- Otros edificios colocados con el ratón.
- Parcelas en zonas especiales.
- Edificios con designación histórica.

**Cuando está desactivada, esas ruinas quedan para que las gestiones tú a mano.**

Cuando está activada, sus ruinas siguen el mismo periodo de espera que los edificios destruidos corrientes.

**Esta opción nunca autoriza la demolición de edificios municipales o históricos en pie o abandonados.**

Solo se aplica una vez que el juego ya ha marcado la parcela como Destruida.

Por ejemplo, si un desastre destruye un monumento, activar esta opción permite que el Ministerio retire sus ruinas tras el periodo de espera.

Puede que prefieras dejarla desactivada si quieres inspeccionar los monumentos destruidos u otros edificios especiales antes de que desaparezcan sus restos.

**Requiere:** Demoler edificios destruidos y Ministerio activo.

Consulta [Demolición automática](demolition.md#demolition-protected).

<a id="opt-demolishabandoned"></a>
### Demoler edificios abandonados

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `DemolishAbandoned`</text>

Despeja automáticamente los edificios que llevan abandonados el periodo de espera configurado.

El Ministerio solo considera los edificios abandonados que coinciden con los siguientes ajustes que hayas elegido:

- Tipos de zona.
- Motivos de abandono.
- Periodo de espera.

Los edificios abandonados elegibles se procesan durante las revisiones mensuales, **después de las ruinas**, empezando por las parcelas abandonadas desde hace más tiempo.

Dos motivos de abandono se consideran siempre elegibles cuando esta función está activada:

- Ocupantes desalojados.
- Ningún motivo o un motivo no reconocido.

Siguen aplicándose todas las demás condiciones de seguridad.

**Los edificios municipales abandonados y los protegidos por su valor histórico nunca se demuelen automáticamente.**

Cuando está desactivado, no se despeja ningún edificio abandonado.

**Importante:** Los edificios que ya estaban abandonados antes de instalar el Ministerio conservan su antigüedad original. Algunos pueden cumplir los requisitos ya en la primera revisión mensual de demolición.

**Requiere:** Ministerio activo.

**Controla:** [Tiempo de espera (abandonados)](#opt-abandonedmonths), las cuatro opciones de zona y las cinco opciones de motivo de abandono.

Consulta [Demolición automática](demolition.md#demolition-abandoned).

<a id="opt-abandonedmonths"></a>
### Tiempo de espera (abandonados)

<text color="secondary">Por defecto: 12 meses (365 días) | Rango: 1–60 meses, en pasos de 1 | INI: `AbandonedAfterDays`</text>

Determina cuánto tiempo debe permanecer abandonado un edificio antes de ser elegible para la demolición.

Por ejemplo, con el retraso por defecto de 12 meses, un edificio debe haber estado abandonado al menos **365 días simulados**.

Un periodo de espera más largo da a los edificios más tiempo para volver a ocuparse.

Un periodo más corto permite despejar antes el terreno abandonado.

**El Ministerio usa el propio contador de antigüedad de abandono de SimCity 4**, que se guarda en la partida de la ciudad.

Por eso el periodo de espera no se reinicia cuando instalas el plugin ni cuando vuelves a abrir la ciudad.

La ventana de opciones muestra meses, mientras que el archivo de configuración guarda días con la misma conversión de 365 días descrita antes.

**Rango INI avanzado:** 1–7.300 días.

**Requiere:** Demoler edificios abandonados y Ministerio activo.

Consulta [Demolición automática](demolition.md#demolition-delays).

<a id="opt-zoneresidential"></a>
### Zona residencial

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `AbandonedZones` (`Residential`)</text>

Incluye los edificios residenciales abandonados en la demolición automática.

Abarca el desarrollo residencial de baja, media y alta densidad, incluidas las casas y los bloques de apartamentos.

Cuando está activada, las viviendas abandonadas elegibles se pueden despejar una vez pasado el periodo de espera.

Cuando está desactivada, los edificios residenciales abandonados se conservan.

**Este ajuste solo afecta a los edificios abandonados.**

Las ruinas residenciales se pueden seguir despejando mediante **Demoler edificios destruidos**.

**Requiere:** Demoler edificios abandonados y Ministerio activo.

Consulta [Demolición automática](demolition.md#demolition-zones).

<a id="opt-zonecommercial"></a>
### Zona comercial

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `AbandonedZones` (`Commercial`)</text>

Incluye los edificios comerciales abandonados en la demolición automática.

Abarca el desarrollo comercial de baja, media y alta densidad, incluidas las tiendas y las oficinas.

Cuando está activada, los edificios comerciales abandonados elegibles se pueden despejar, lo que permite que el terreno zonificado vuelva a desarrollarse.

Cuando está desactivada, los edificios comerciales abandonados no se tocan.

**Las ruinas comerciales se controlan por separado** mediante el ajuste de edificios destruidos.

**Requiere:** Demoler edificios abandonados y Ministerio activo.

Consulta [Demolición automática](demolition.md#demolition-zones).

<a id="opt-zoneindustrial"></a>
### Zona industrial

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `AbandonedZones` (`Industrial`)</text>

Incluye los edificios industriales abandonados en la demolición automática.

Abarca el desarrollo industrial de media y alta densidad, como fábricas y almacenes.

Cuando está activada, los edificios industriales abandonados elegibles se pueden demoler una vez pasado el periodo de espera.

Cuando está desactivada, se conservan.

**Las parcelas agrícolas no están incluidas en este ajuste.** Tienen su propia opción independiente.

Las ruinas industriales siguen sujetas al ajuste de edificios destruidos, sea cual sea esta elección.

**Requiere:** Demoler edificios abandonados y Ministerio activo.

Consulta [Demolición automática](demolition.md#demolition-zones).

<a id="opt-zoneagriculture"></a>
### Zona agrícola

<text color="secondary">Por defecto: Desactivado | Valores: Activado / Desactivado | INI: `AbandonedZones` (`Agriculture`)</text>

Controla la demolición automática de granjas y campos agrícolas abandonados.

**Este ajuste está desactivado por defecto por una razón importante.**

En SimCity 4, los campos de cultivo pueden parecer abandonados como parte de su funcionamiento normal.

Eso no significa necesariamente que la granja haya dejado de funcionar.

Cuando está desactivada, **se conservan todas las parcelas agrícolas abandonadas**.

Cuando está activada, el Ministerio puede demoler las granjas y los campos abandonados elegibles una vez pasado el periodo de espera.

**Activar esta opción puede eliminar campos de cultivo que querías conservar.**

Úsala con cuidado, sobre todo en ciudades agrícolas.

Las ruinas en terreno agrícola son un caso aparte. Se pueden seguir despejando mediante **Demoler edificios destruidos**, incluso con Zona agrícola desactivada.

**Requiere:** Demoler edificios abandonados y Ministerio activo.

Consulta [Demolición automática](demolition.md#demolition-farms).

<a id="opt-reasonlowdesirability"></a>
### Motivo: zona poco atractiva

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `AbandonedReasons` (`LowDesirability`)</text>

Incluye los edificios abandonados porque su ubicación se ha vuelto poco atractiva.

Entre las posibles causas están:

- La contaminación.
- La delincuencia.
- Los servicios insuficientes.
- Otros factores que reducen el atractivo local.

Cuando está activada, esos edificios se pueden demoler una vez pasado el periodo de espera configurado.

Cuando está desactivada, se conservan.

**Demoler un edificio abandonado no soluciona el problema de atractivo de fondo.**

Por ejemplo, si la contaminación sigue siendo excesiva, el edificio que lo sustituya puede encontrarse con dificultades parecidas.

Mejorar la zona circundante puede permitir a veces que el edificio original vuelva a ocuparse.

**Requiere:** Demoler edificios abandonados y Ministerio activo.

Consulta [Demolición automática](demolition.md#demolition-reasons).

<a id="opt-reasoncommute"></a>
### Motivo: trayecto al trabajo demasiado largo

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `AbandonedReasons` (`CommuteTime`)</text>

Incluye los edificios abandonados porque sus ocupantes no podían llegar al trabajo en un tiempo de trayecto aceptable.

Cuando está activada, los edificios elegibles se pueden demoler una vez pasado el periodo de espera.

Cuando está desactivada, se conservan.

**La demolición no resuelve directamente el problema del trayecto.**

Por ejemplo, mejorar el transporte público o el acceso por carretera podría permitir que un edificio residencial abandonado vuelva a ocuparse.

Puede que prefieras un periodo de espera más largo si mejoras con frecuencia la red de transporte de tu ciudad.

**Requiere:** Demoler edificios abandonados y Ministerio activo.

Consulta [Demolición automática](demolition.md#demolition-reasons).

<a id="opt-reasonlowdemand"></a>
### Motivo: falta de demanda

<text color="secondary">Por defecto: Activado | Valores: Activado / Desactivado | INI: `AbandonedReasons` (`LowDemand`)</text>

Incluye los edificios abandonados porque la ciudad ya no tiene demanda suficiente para ese tipo de desarrollo.

Cuando está activada, el Ministerio puede despejar esos edificios una vez pasado el periodo de espera, y deja su zonificación disponible para el crecimiento futuro.

Cuando está desactivada, no se tocan.

**La demanda puede cambiar a medida que la ciudad se desarrolla.**

Un edificio abandonado hoy por falta de demanda puede volver a ser útil más adelante.

El periodo de espera determina cuánto tiempo concedes a esa posibilidad antes de que el Ministerio despeje la parcela.

**Requiere:** Demoler edificios abandonados y Ministerio activo.

Consulta [Demolición automática](demolition.md#demolition-reasons).

<a id="opt-reasonnopower"></a>
### Motivo: falta de electricidad

<text color="secondary">Por defecto: Desactivado | Valores: Activado / Desactivado | INI: `AbandonedReasons` (`NoPower`)</text>

Controla la demolición de los edificios abandonados porque carecen de electricidad.

**Esta opción está desactivada por defecto.**

Un edificio abandonado por un fallo eléctrico puede volver a ocuparse cuando se restablece la electricidad.

Cuando está desactivada, el Ministerio conserva esos edificios.

Cuando está activada, puede demolerlos una vez pasado el periodo de espera configurado.

**Revisa la red eléctrica antes de activar esta opción.**

Puede que a un edificio abandonado solo le haga falta reparar una conexión o restablecer el suministro, y no una demolición.

**Requiere:** Demoler edificios abandonados y Ministerio activo.

Consulta [Demolición automática](demolition.md#demolition-reasons).

<a id="opt-reasonnowater"></a>
### Motivo: falta de agua

<text color="secondary">Por defecto: Desactivado | Valores: Activado / Desactivado | INI: `AbandonedReasons` (`NoWater`)</text>

Controla la demolición de los edificios abandonados porque carecen de agua.

Igual que la opción equivalente de electricidad, **esta opción está desactivada por defecto**.

Los edificios abandonados por falta de agua pueden volver a ocuparse cuando se restablece el suministro de agua o la conexión de las tuberías.

Cuando está desactivada, esos edificios se conservan.

Cuando está activada, el Ministerio puede despejarlos una vez pasado el periodo de espera.

**Revisa la red de agua antes de activar esta opción.**

Puede que la demolición sea innecesaria si restablecer el servicio permitiera que el edificio se recuperase.

**Requiere:** Demoler edificios abandonados y Ministerio activo.

Consulta [Demolición automática](demolition.md#demolition-reasons).

<a id="opt-budgetlimit"></a>
### Limitar el presupuesto mensual de demolición

<text color="secondary">Por defecto: Desactivado | Valores: Activado / Desactivado | INI: `LimitMonthlyBudget`</text>

Añade un límite de gasto opcional a la demolición automática.

El límite se aplica a **las ruinas y los edificios abandonados en conjunto** durante cada revisión mensual de demolición.

Cuando está activado, el Ministerio lleva la cuenta de los costes de excavadora de las órdenes ya enviadas durante esa revisión.

Una demolición solo se permite cuando su coste no superaría el presupuesto mensual configurado.

**Si el siguiente edificio con precio superara el límite, toda la revisión mensual de demolición se detiene.**

Los edificios restantes esperan hasta el mes siguiente.

Cuando está desactivado, no hay ningún tope de gasto mensual adicional.

Sin embargo, **la protección de la tesorería siempre sigue activa**.

El Ministerio nunca ordena más gastos de demolición de los que la ciudad puede permitirse con la tesorería disponible al comienzo de la revisión.

Las demoliciones con un precio de §0 no consumen el presupuesto mensual opcional.

**Requiere:** Ministerio activo.

**Controla:** [Presupuesto mensual](#opt-demolitionbudget).

Consulta [Demolición automática](demolition.md#demolition-money).

<a id="opt-demolitionbudget"></a>
### Presupuesto mensual

<text color="secondary">Por defecto: §1.000 | Rango en el juego: §0–§200.000, en pasos de §1.000 | INI: `MonthlyBudget`</text>

Fija la cantidad máxima que el Ministerio puede gastar en demolición automática durante una revisión mensual, cuando el límite de presupuesto está activado.

Por ejemplo, con un presupuesto de **§1.000**, el Ministerio puede ordenar demoliciones elegibles mientras su coste combinado se mantenga dentro de esa cantidad.

Si una demolición propuesta superara el límite, la revisión se detiene y los candidatos restantes esperan hasta el mes siguiente.

**¿Qué ocurre con distintos niveles de presupuesto?**

| Presupuesto mensual | Efecto |
|---|---|
| **§0** | Solo se permiten demoliciones que cuesten §0. |
| **§1.000** | Se pueden comprometer hasta §1.000 por revisión. |
| **§5.000** | Se pueden comprometer hasta §5.000 por revisión. |
| **§20.000** | Se pueden comprometer hasta §20.000 por revisión. |

Un presupuesto mayor permite más gastos de demolición en un solo mes.

Un presupuesto menor reparte una gran acumulación de edificios elegibles en más meses.

**El presupuesto mensual nunca anula la regla de la tesorería.**

Por ejemplo, fijar un presupuesto de demolición de §20.000 no permite que el Ministerio gaste §20.000 si la ciudad solo tiene §5.000 disponibles.

#### ¿Qué pasa si el límite de presupuesto está desactivado?

El control de Presupuesto mensual deja de estar disponible, pero su valor guardado se conserva.

Si activas el límite más adelante, el presupuesto anterior vuelve a estar en vigor.

Por ejemplo, la configuración suministrada guarda **§1.000** aunque el límite esté desactivado por defecto.

Activarlo no reinicia esa cantidad.

#### ¿Puedo fijar un presupuesto mayor a mano?

Sí.

La ventana de opciones del juego permite un máximo de **§200.000**.

El archivo de configuración acepta valores de hasta **2.000.000.000**, aunque la ventana de opciones no puede representar del todo los valores mayores.

**Requiere:** Limitar el presupuesto mensual de demolición y Ministerio activo.

Consulta [Demolición automática](demolition.md#demolition-money).

---

[Volver al contenido](index.md#contents) | [Volver al README](../../README.es.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
