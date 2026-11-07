[English](../en/water.md) | [Português](../pt/water.md) | [Français](../fr/water.md) | **Español**

[Volver al contenido](index.md#contents) | [Volver al README](../../README.es.md)

<a id="water"></a>
## C. Agua

El Ministerio gestiona automáticamente el **presupuesto del suministro de agua** de tu ciudad, asegurándose de que haya agua suficiente para tus edificios y manteniendo los costes de funcionamiento lo más bajos posible.

Tiene en cuenta:

- Cuánta agua necesita tu ciudad.
- La reserva de seguridad que hayas elegido (**10% por defecto**).
- El agua que tu ciudad compra o vende a las ciudades vecinas.
- La capacidad de producción de tus instalaciones de agua.
- El estado de tu red de tuberías.

A diferencia de la electricidad, donde cada central tiene su propio ajuste de financiación, **todo el suministro de agua comparte un único porcentaje de presupuesto**.

El Ministerio ajusta ese porcentaje para dar agua suficiente sin gastos innecesarios.

También puede proteger tus tuberías del deterioro causado por una financiación insuficiente.

Los contratos regionales de agua se incluyen en los cálculos. Su gestión automática se explica en [Acuerdos con vecinos](deals.md#neighbor-deals).

<a id="water-vanilla"></a>
### 1. Cómo funciona el agua en SimCity 4

#### Un solo presupuesto para todo el suministro de agua

SimCity 4 gestiona la financiación del agua mediante un único presupuesto de departamento.

Tu ciudad puede tener varias instalaciones de agua, como bombas, torres de agua y plantas de tratamiento. Cada instalación tiene su propio coste de mantenimiento, pero **todas las instalaciones reciben el mismo porcentaje de financiación**.

Por ejemplo, si fijas la financiación del agua en el 80%, ese porcentaje se aplica a todo el departamento del agua.

No puedes financiar una bomba al 100% y otra al 60%, como sí puedes hacer con las centrales eléctricas.

Normalmente el juego permite una financiación del agua de hasta el **120%**, aunque puede aplicarse un máximo menor si el juego lo indica.

A diferencia de la electricidad, el agua no tiene un porcentaje mínimo de financiación aceptable ni un mecanismo de huelga integrados.

#### Demanda, producción y consumo de agua

El juego registra varios valores relacionados con el agua:

| Término | Qué significa |
|---|---|
| **Demanda** | La cantidad de agua que necesitan los edificios de tu ciudad. |
| **Capacidad de producción** | La cantidad de agua que tus propias instalaciones pueden suministrar con su financiación actual. |
| **Consumo** | El agua que realmente se entrega a los edificios. |
| **Importaciones** | Agua comprada a las ciudades vecinas. |
| **Exportaciones** | Agua vendida a las ciudades vecinas. |
| **Edificios con agua** | Edificios que reciben agua en este momento. |
| **Edificios sin agua** | Edificios sin servicio de agua. |
| **Tuberías deterioradas** | Tuberías que se han deteriorado. |
| **Tuberías reventadas** | Tuberías que han fallado. |

**¿Cómo afectan los contratos regionales de agua a tu suministro?**

En nuestras observaciones, SimCity 4 usa el agua importada antes de recurrir a la producción local.

Esto significa que comprar agua a una ciudad vecina puede reducir lo que tus propias instalaciones necesitan producir.

Vender agua tiene el efecto contrario: **tu ciudad debe tener capacidad suficiente para abastecer tanto a sus propios edificios como a sus contratos de exportación**.

El Ministerio tiene en cuenta estos contratos al calcular la financiación necesaria.

**Un detalle importante al abrir una ciudad**

Justo después de cargar una ciudad, el juego puede informar de cero edificios con agua y cero edificios sin agua.

Esto no significa necesariamente que todos los edificios tengan agua ni que la ciudad no tenga demanda de agua.

Normalmente significa que la simulación del agua todavía no ha actualizado su información.

Por eso el Ministerio debe distinguir entre las lecturas iniciales ausentes y los valores reales antes de tomar decisiones de financiación.

<a id="water-efficiency-curve"></a>
#### Cómo afecta la financiación a la producción de agua

En SimCity 4, reducir la financiación del agua también reduce la cantidad de agua que pueden producir tus instalaciones.

Sin embargo, **la producción no baja al mismo ritmo que la financiación**.

El juego usa la siguiente curva de eficiencia:

| Financiación del agua | Eficiencia de producción |
|---:|---:|
| 0% | 0% |
| 50% | 30% |
| 90% | 80% |
| 100% | 100% |
| 120% | 110% |

**¿Qué significa esto?**

- Con una **financiación del 50%**, tu ciudad tiene solo el **30% de su capacidad normal de producción de agua**.
- Con una **financiación del 90%**, tiene el **80% de la capacidad normal**.
- Con una **financiación del 100%**, tiene toda la capacidad normal.
- Con una **financiación del 120%**, tiene el **110% de la capacidad normal**.

Por tanto, una financiación superior al 100% da agua adicional, pero con un coste mayor por unidad de capacidad.

Del mismo modo, reducir demasiado la financiación puede eliminar mucha más capacidad de producción de la que cabría esperar.

**¿Cómo se calculan los porcentajes intermedios?**

Entre los puntos de control del juego, la eficiencia cambia en línea recta.

Por ejemplo, entre una financiación del 90% y del 100%, la eficiencia sube del 80% al 100%.

Con una financiación del 95%, la eficiencia de producción es, por tanto, del 90%.

**Interpolación de la curva de eficiencia**

```math
y(x) = y_i + \frac{x-x_i}{x_{i+1}-x_i}(y_{i+1}-y_i)
\qquad \text{para } x_i \le x \le x_{i+1}
```

Donde:

- **x** es el porcentaje de financiación elegido.
- **(xᵢ, yᵢ)** y **(xᵢ₊₁, yᵢ₊₁)** son los dos puntos de la curva que lo rodean.
- **y(x)** es la eficiencia de producción resultante.

Fuera del rango definido de la curva, se usa el valor del extremo más cercano.

**Ejemplos de eficiencia de producción de agua**

| Financiación (%) | 30 | 50 | 60 | 70 | 75 | 78 | 80 | 90 | 95 | 100 | 105 | 110 | 120 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| Eficiencia (%) | 18 | 30 | 42,5 | 55 | 61,25 | 65 | 67,5 | 80 | 90 | 100 | 102,5 | 105 | 110 |

**¿Por qué es importante esto para el Ministerio?**

Con una financiación del 100%, la producción de agua es la más eficiente en términos de capacidad obtenida por cada simoleón gastado.

Pero eso no significa que todas las ciudades necesiten una financiación del 100%.

Imagina una ciudad que produce mucha más agua de la que consume. Reducir el presupuesto del agua puede seguir dando agua suficiente y ahorrar dinero.

Por ejemplo:

| Financiación | Capacidad de producción | Coste relativo al 100% |
|---|---:|---:|
| 50% | 30% | 50% |
| 80% | 67,5% | 80% |
| 100% | 100% | 100% |
| 120% | 110% | 120% |

**El Ministerio busca el nivel de financiación más bajo que pueda cubrir las necesidades de agua de la ciudad y su reserva de seguridad**, respetando al mismo tiempo sus demás protecciones.

A diferencia de la electricidad, no necesita comparar distintas combinaciones de financiación para cada instalación, porque solo hay un porcentaje de presupuesto del agua.

Consulta [Cómo elige el Ministerio la financiación](#water-algorithm) para ver el cálculo.

#### La producción de agua no se actualiza de inmediato

Hay un retraso importante en SimCity 4.

**Cuando cambia la financiación del agua, la capacidad de producción que informa el juego se actualiza en el siguiente mes simulado, no de inmediato.**

Por ejemplo, durante las pruebas, una ciudad tenía una capacidad de producción de **241.520 unidades** con una financiación del 100%.

Tras reducir la financiación al 95%, la capacidad que informaba el juego pasó a ser de **217.368 unidades** en la actualización mensual siguiente.

Eso es exactamente el 90% de la capacidad original, de acuerdo con la curva de eficiencia.

Este retraso importa porque el Ministerio no debe tomar una lectura de capacidad sin cambios justo después de un ajuste de financiación como prueba de que el ajuste no tuvo efecto.

#### ¿Cómo se calcula la factura mensual del agua?

El gasto total del departamento del agua depende de dos cosas:

1. Los costes de mantenimiento combinados de todas las instalaciones de agua.
2. El porcentaje de financiación actual del agua.

El juego cobra estos costes en proporción a la financiación.

**Gasto mensual del agua (juego base)**

```math
X(f) = \operatorname{round}\left(I\times\frac{f}{100}\right)
```

Donde:

- **I** es el coste mensual de mantenimiento combinado de todas las instalaciones de agua con una financiación del 100%.
- **f** es el porcentaje de financiación del departamento del agua.
- **X(f)** es el gasto mensual estimado del agua.

**Ejemplo: una ciudad con un presupuesto normal de agua de §2.000 al mes**

| Financiación | Gasto mensual | Ahorro mensual |
|---|---:|---:|
| 100% | §2.000 | — |
| 90% | §1.800 | §200 |
| 78% | §1.560 | §440 |
| 50% | §1.000 | §1.000 |

Con una financiación del 78%, la ciudad ahorra **§440 al mes** en comparación con el 100%.

Sin embargo, esa financiación ofrece solo el **65% de la capacidad normal de producción**.

Por tanto, el Ministerio debe comprobar si una capacidad del 65% basta para satisfacer la demanda, la reserva configurada y las exportaciones de agua.

La fórmula redondea los gastos a simoleones enteros. Debido a los cálculos internos del juego, el gasto que se muestra realmente puede diferir en uno o dos simoleones.

<a id="water-pipe-curve"></a>
#### Cómo afecta la financiación a las tuberías de agua

La financiación del agua no solo controla la producción. También afecta al **estado de tu red de tuberías**.

En SimCity 4, las tuberías pueden deteriorarse poco a poco cuando el departamento del agua recibe una financiación insuficiente.

El juego usa una curva aparte para determinar si las tuberías se deterioran, se mantienen estables o se reparan solas.

**Curva de deterioro y reparación de las tuberías (juego base)**

| Financiación del agua | Efecto en las tuberías |
|---:|---|
| 0% | Deterioro (−0,010) |
| 20% | Deterioro (−0,005) |
| 50% | Ni deterioro ni reparación (0) |
| 100% | Reparación (+0,020) |
| 110% | Reparación más rápida (+0,050) |

La curva sigue la misma regla de interpolación lineal que se usa para la eficiencia de producción.

Con una financiación del 120%, el valor de reparación sigue siendo +0,050, igual que con el 110%.

**El umbral más importante es una financiación del 50%.**

- **Por debajo del 50%:** las tuberías se deterioran poco a poco.
- **En el 50%:** su estado se mantiene estable.
- **Por encima del 50%:** la red de tuberías se repara sola poco a poco.

Esto **no** significa que las tuberías revienten de inmediato cuando reduces la financiación por debajo del 50%.

El deterioro ocurre con el tiempo.

A medida que las tuberías se dañan, el juego puede informar de que están *deterioradas*. Si su estado empeora lo suficiente, pueden convertirse en *tuberías reventadas*.

Volver a poner la financiación en el 50% detiene el deterioro posterior causado por la curva de financiación. Subirla por encima del 50% permite que la red se recupere.

**¿Con qué rapidez se deterioran o se recuperan las tuberías?**

La tabla siguiente muestra la curva del juego con varios niveles de financiación:

| Financiación (%) | 0 | 10 | 20 | 30 | 35 | 40 | 50 | 60 | 75 | 78 | 90 | 100 | 105 | 110 | 120 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| Curva de tuberías | −0,0100 | −0,0075 | −0,0050 | −0,0033 | −0,0025 | −0,0017 | 0 | +0,0040 | +0,0100 | +0,0112 | +0,0160 | +0,0200 | +0,0350 | +0,0500 | +0,0500 |

Un valor más negativo significa un efecto de deterioro más fuerte. Un valor positivo mayor significa un efecto de reparación más fuerte.

Por ejemplo, con una financiación del 20%, el ritmo de deterioro que representa la curva es aproximadamente el doble que con el 30%.

Con una financiación del 110%, el valor de reparación es 2,5 veces el del 100%.

**¿Cómo usa el Ministerio esta información?**

El Ministerio reconoce si un nivel de financiación provoca:

- Deterioro de las tuberías.
- Estado estable de las tuberías.
- Reparación de las tuberías.

Cuando la protección de las tuberías está activada, esta información le ayuda a evitar recortes de financiación que pondrían en riesgo la red.

**Una limitación importante:** el Ministerio no calcula el número exacto de días simulados que tardaría una tubería concreta en deteriorarse o reventar. Su modelo usa la dirección de la curva, no una fecha de fallo prevista.

Esto le permite proteger la red sin depender de una estimación sin verificar de la rapidez con que se desgasta cada tubería.
<a id="water-problem"></a>
### 2. El problema de la gestión

Gestionar a mano la financiación del agua puede parecer sencillo, ya que toda la ciudad comparte un único porcentaje de presupuesto. Sin embargo, varios factores lo complican.

- **Gasto innecesario:** una ciudad puede producir mucha más agua de la que necesita, sobre todo después de construir una bomba o una planta de tratamiento nueva. Mantener la financiación al 100% puede desperdiciar cientos de simoleones cada mes.
- **Riesgo de escasez de agua:** reducir demasiado la financiación puede dejar edificios sin agua. Peor aún, la capacidad de producción del juego solo se actualiza en el siguiente mes simulado, así que las consecuencias de un cambio de financiación pueden no ser visibles de inmediato.
- **Contratos regionales:** comprar agua reduce lo que tu ciudad necesita producir localmente, mientras que venderla lo aumenta.
- **Deterioro de las tuberías:** una financiación por debajo del 50% hace que las tuberías de agua se deterioren poco a poco. Los costes de reparación y las consecuencias de las tuberías dañadas no se reflejan directamente en el presupuesto mensual del agua.
- **Problemas de conexión:** puede haber edificios sin agua aunque la ciudad tenga capacidad de producción suficiente. En esos casos, aumentar la financiación no ayuda. El jugador debe corregir las conexiones de las tuberías.

**El Ministerio lo resuelve calculando la capacidad de agua que tu ciudad necesita de verdad, buscando el nivel de financiación adecuado más bajo y protegiendo la red de tuberías cuando esa opción está activada.**

<a id="water-algorithm"></a>
### 3. Cómo elige el Ministerio la financiación del agua

El Ministerio no se limita a bajar el presupuesto del agua siempre que la producción supera la demanda.

En su lugar, sigue cuatro pasos:

1. Calcular cuánta agua debe producir la ciudad localmente.
2. Estimar cuánta agua se puede producir con cada porcentaje de financiación.
3. Identificar qué niveles de financiación son seguros y asequibles.
4. Elegir el porcentaje adecuado más barato y aplicarlo en el orden correcto.

Todos los cálculos usan la información más reciente de la ciudad.

<a id="water-required-capacity"></a>
#### Paso 1: ¿Cuánta agua necesita la ciudad?

Primero, el Ministerio calcula la capacidad mínima de producción de agua que tu ciudad debe mantener.

Tiene en cuenta:

- La demanda de agua actual de tu ciudad.
- La reserva de agua que hayas elegido (**10% por defecto**).
- El agua comprada a las ciudades vecinas.
- El agua vendida a las ciudades vecinas.

**La reserva se aplica solo al agua que tu ciudad debe producir por sí misma.**

Por ejemplo, si tu ciudad necesita 20.000 unidades de agua y lo produce todo localmente, una reserva del 10% significa que debe mantener una capacidad de al menos **22.000 unidades**.

Si la ciudad compra parte de esa agua a un vecino, la reserva se aplica solo a la demanda local restante.

**Capacidad local de agua necesaria**

```math
L = \left\lceil R \times \max(0,D-B) \right\rceil + S
\qquad
R = 1+\frac{r}{100}
```

Donde:

- **D** es la demanda total de agua de la ciudad.
- **B** es el agua comprada a las ciudades vecinas que se puede usar con fiabilidad.
- **S** es el agua vendida a las ciudades vecinas.
- **r** es la Reserva de agua configurada, 10% por defecto.
- **R** es el multiplicador de la reserva (1,10 para una reserva del 10%).
- **L** es la capacidad local de producción de agua necesaria.

El resultado se redondea hacia arriba hasta una unidad de agua entera.

**Ejemplo: una ciudad que necesita 20.000 unidades**

| Situación | Capacidad local necesaria |
|---|---:|
| Sin importaciones ni exportaciones de agua | 22.000 unidades |
| Importando 5.000 unidades | 16.500 unidades |
| Exportando 5.000 unidades | 27.000 unidades |

Sin contratos regionales:

```math
L = \lceil 20000 \times 1.10 \rceil = 22000
```

Con una compra de 5.000 unidades:

```math
L = \lceil (20000-5000)\times1.10\rceil = 16500
```

Con una venta de 5.000 unidades:

```math
L = \lceil 20000\times1.10\rceil+5000=27000
```

Fíjate en que **la reserva no se suma al agua exportada**. Las exportaciones deben cubrirse, pero no reciben un margen de seguridad adicional.

Del mismo modo, el Ministerio no compra agua de más solo para crear una reserva sobre el suministro importado.

**¿Qué compras de agua son de fiar?**

Solo se consideran fiables las compras confirmadas como activas.

Los contratos pendientes, suspendidos o con fallos no cuentan como agua disponible.

Si el Ministerio también gestiona los contratos regionales de agua, calcula la financiación con las compras y ventas incluidas en su plan de contratos. Las dos decisiones se coordinan en lugar de tomarse por separado.

Consulta [Acuerdos con vecinos](deals.md#neighbor-deals).

#### Paso 2: ¿Cuánta agua puede producir la ciudad con cada nivel de financiación?

Una vez conocida la capacidad necesaria, el Ministerio examina la producción de agua actual de la ciudad.

Después estima cuánta agua podrían producir las instalaciones existentes con distintos porcentajes de financiación.

Como todos los productores de agua comparten el mismo ajuste de financiación, este cálculo es mucho más sencillo que la optimización de la electricidad.

**Capacidad de producción de agua proyectada**

```math
C(v)=\left\lfloor C_{\text{actual}}\times\frac{\eta(v)}{\eta(f_{\text{actual}})}\right\rfloor
```

Donde:

- **Cactual** es la capacidad de producción que informa el juego en este momento.
- **factual** es el porcentaje actual de financiación del agua, leído tal como lo guarda el juego.
- **v** es un posible porcentaje de financiación nuevo.
- **η** es la curva de eficiencia de producción de agua del juego.
- **C(v)** es la capacidad estimada con la financiación propuesta, redondeada hacia abajo.

En lenguaje llano, **el Ministerio usa la capacidad de producción actual y la curva de eficiencia del juego para predecir cuánta agua daría un nivel de financiación distinto**.

**Ejemplo: una ciudad que produce 34.000 unidades**

Supongamos que la ciudad produce actualmente 34.000 unidades con una financiación del 100%.

Con una financiación del 78%, la curva de eficiencia del juego ofrece el 65% de la capacidad normal de producción.

```math
C(78)=\left\lfloor34000\times\frac{65}{100}\right\rfloor=22100
```

Por tanto, el Ministerio estima que **una financiación del 78% daría 22.100 unidades**.

Si la ciudad necesita 22.000 unidades, reserva incluida, eso sería suficiente.

**Una limitación importante**

Este cálculo supone que todos los productores de agua responden a la financiación del departamento según la curva de eficiencia estándar.

Si la financiación actual produce una eficiencia cero, como ocurre con el 0%, el Ministerio no puede usar esta fórmula para estimar capacidades alternativas de forma fiable. En esa situación, no elabora un plan automático de financiación del agua.

<a id="water-funding-choice"></a>
#### Paso 3: ¿Qué niveles de financiación se permiten?

El Ministerio no considera todos los niveles de financiación posibles sin restricciones.

Las opciones disponibles dependen de tus ajustes y del estado actual de la red de agua.

**Financiación mínima**

| Proteger las tuberías | Mínimo para las nuevas opciones de financiación |
|---|---:|
| **Activado (por defecto)** | **50%** |
| Desactivado | 0% |

Con la protección de las tuberías activada, el Ministerio mantiene normalmente la financiación en el punto en que la curva de deterioro de tuberías del juego se vuelve neutra, o por encima.

Con una financiación del 50%, las tuberías ni se deterioran ni se reparan por efecto de la financiación del departamento.

Por encima del 50%, la red se repara sola poco a poco.

**Financiación máxima**

El máximo normal es del **120%**, o un valor menor si el juego informa de un límite menor para el departamento.

**¿Y si has fijado a mano una financiación por debajo del 50%?**

El Ministerio siempre incluye la financiación actual como opción posible, aunque quede fuera de su rango normal de búsqueda.

Por ejemplo, si fijas a mano la financiación del agua en el 40%, ese nivel existente no se descarta automáticamente.

Sin embargo, con la protección de las tuberías activada, el Ministerio no elegirá una **nueva reducción** que cause deterioro de las tuberías.

Esta distinción importa: **proteger las tuberías no significa que el Ministerio suba automáticamente todos los presupuestos existentes que estén por debajo del 50%.** La elección de la financiación sigue sus requisitos de capacidad y sus reglas de seguridad.

#### ¿Cuándo se niega el Ministerio a reducir la financiación?

Aunque un porcentaje menor permitiera ahorrar dinero, el Ministerio rechaza una reducción cuando se da cualquiera de estas condiciones:

**1. La ciudad tiene una escasez real de agua**

Si hay edificios sin agua y la capacidad de producción local es inferior a la demanda, el Ministerio no reduce más la financiación.

**2. Las tuberías de agua ya están dañadas**

Cuando **Proteger las tuberías** está activado, el Ministerio rechaza las reducciones si el juego informa de alguna tubería deteriorada o reventada.

**3. La reducción causaría deterioro de las tuberías**

Con la protección activada, se rechaza una reducción de la financiación si la curva de tuberías del juego pasara a ser negativa.

Estas restricciones se aplican a las **reducciones de financiación**.

No impiden que el Ministerio aumente la financiación para mejorar el suministro de agua.

#### Paso 4: Elegir la financiación más barata

Tras descartar las opciones poco seguras, el Ministerio compara los niveles de financiación restantes.

Su objetivo es seleccionar **el gasto mensual más bajo que siga dando la capacidad de agua necesaria**.

**Optimización de la financiación**

```math
f^* = \arg\min_{v\in V,\ C(v)\ge L} X(v)
```

Donde:

- **V** es el conjunto de porcentajes de financiación permitidos.
- **C(v)** es la capacidad de agua estimada con la financiación **v**.
- **L** es la capacidad local necesaria calculada en el Paso 1.
- **X(v)** es el gasto mensual de funcionamiento con la financiación **v**.
- **f\*** es el porcentaje de financiación elegido.

Como tanto la capacidad de agua como el gasto de funcionamiento aumentan con la financiación, **la solución más barata es normalmente el porcentaje permitido más bajo que dé agua suficiente**.

Si dos opciones tienen el mismo coste, el Ministerio prefiere mantener la financiación actual.

**Ejemplo: 34.000 unidades con la financiación al máximo**

Volvamos al ejemplo anterior:

- Capacidad actual al 100%: 34.000 unidades.
- Capacidad necesaria, reserva incluida: 22.000 unidades.
- Protección de las tuberías: activada.

| Financiación | Capacidad proyectada | Resultado |
|---|---:|---|
| 77% | 21.675 | No es suficiente |
| **78%** | **22.100** | **Suficiente** |
| 80% | 22.950 | Suficiente, pero más cara |
| 100% | 34.000 | Exceso de capacidad innecesario |

**El Ministerio elige una financiación del 78%**, siempre que ninguna de las condiciones de seguridad impida la reducción.

Si el departamento cuesta normalmente §2.000 al mes, el nuevo coste es de aproximadamente **§1.560**, con un ahorro de **§440 al mes**.

**¿Y si ni siquiera la financiación máxima basta?**

El Ministerio selecciona el nivel de financiación permitido que da la mayor capacidad disponible, normalmente el 120%.

Si varias opciones dan la misma capacidad máxima, prefiere la más barata.

No reduce la financiación cuando la ciudad ya carece de capacidad suficiente.

Cuando la gestión de los contratos regionales de agua está activada, el Ministerio también puede intentar comprar agua para cubrir la carencia restante.

Si el problema persiste, el jugador puede recibir un aviso de un asesor.

Consulta [Acuerdos con vecinos](deals.md#neighbor-deals) y [Avisos de los asesores](safety.md#notices).

#### ¿Cómo se aplican los cambios de financiación?

El Ministerio aplica directamente el porcentaje de financiación que ha elegido, sin mover el control deslizante del presupuesto poco a poco.

Sin embargo, **el orden de los cambios de financiación y de contratos importa**.

- **Los aumentos de financiación se hacen primero**, antes de cambiar los contratos de agua.
- **Las reducciones de financiación se hacen solo después** de aplicar los cambios de contratos regionales necesarios.

Por ejemplo, supongamos que el Ministerio quiere reducir la producción local de agua porque ha encontrado una compra más barata a una ciudad vecina.

Primero se asegura de que los cambios de contratos regionales estén en vigor y después reduce el presupuesto del agua.

Así evita recortar la producción local antes de tener organizado el suministro alternativo.

Cada revisión realiza como máximo una etapa coordinada para un recurso. Los cambios restantes se vuelven a considerar en una evaluación posterior, con lecturas nuevas del juego.

**El objetivo es que la transición sea segura, no solo el nivel final de financiación.**

<a id="water-cadence"></a>
#### ¿Con qué frecuencia revisa el Ministerio la financiación del agua?

El Ministerio revisa con regularidad tu suministro de agua y puede reaccionar antes cuando se producen cambios importantes.

Todos los plazos se refieren al tiempo simulado del juego.

| Cuándo | Qué ocurre |
|---|---|
| **Al abrir una ciudad** | Realiza la primera evaluación del agua en cuanto las lecturas del juego están listas y son coherentes. |
| **Cada día simulado** | Comprueba si hay carencias graves y cambios significativos en la demanda de agua, la producción o los contratos regionales. |
| **Cada 10 días simulados** | Realiza una revisión periódica de la financiación del agua, contada desde la última revisión completa. |
| **Al comienzo de cada mes simulado** | Realiza una revisión del agua. |
| **Cuando se detecta un cambio importante** | Recalcula el plan del agua durante la comprobación diaria, sin esperar a la siguiente revisión programada. |

Cada revisión completada reinicia el intervalo de revisión periódica. Si acaba de hacerse una evaluación, el Ministerio no la repite de inmediato solo porque tocara otra comprobación programada.

**¿Qué desencadena una evaluación anticipada?**

Las comprobaciones diarias del Ministerio buscan dos situaciones.

**1. Una escasez real del suministro de agua**

Existe escasez cuando la ciudad consume menos agua de la que necesita **y** el suministro utilizable total es insuficiente.

**Déficit regional de agua**

```math
W < D \qquad\text{y}\qquad C+U-S<D
```

Donde:

- **W** es el agua realmente consumida.
- **D** es la demanda de agua de la ciudad.
- **C** es la capacidad de producción local.
- **U** es el agua realmente entregada mediante contratos de compra.
- **S** es el agua vendida a las ciudades vecinas.

Por ejemplo, si una ciudad necesita 20.000 unidades pero su suministro utilizable total es de solo 15.000, el Ministerio puede recalcular la financiación sin esperar a la siguiente revisión periódica.

Pero imagina que la ciudad necesita 20.000 unidades y tiene 25.000 disponibles, y aun así algunos edificios siguen sin agua.

En ese caso, el problema está probablemente en la red de tuberías de la ciudad y no en una producción insuficiente.

**Aumentar el presupuesto no ayudaría necesariamente**, así que el Ministerio registra el problema de conexión en lugar de activar una revisión de la financiación solo por ese motivo.

Una escasez que ya se ha evaluado no activa nuevas revisiones una y otra vez, salvo que cambie la situación del suministro.

**2. Un cambio significativo en el suministro o la demanda de agua**

El Ministerio también recalcula la financiación cuando cualquiera de los siguientes valores cambia al menos un **1%** respecto a la revisión anterior:

- La demanda de agua.
- La capacidad de producción local.
- Las importaciones de agua contratadas.
- Las exportaciones de agua contratadas.

**Umbral de cambio significativo**

```math
|x-x_0|\ge0.01\times\max(1,|x_0|)
```

Donde:

- **x** es la última lectura.
- **x₀** es el valor registrado en la revisión anterior.

Por ejemplo, si la demanda sube de 20.000 a 20.300 unidades, es un aumento del 1,5%.

Por tanto, el Ministerio puede recalcular la financiación en su siguiente comprobación diaria.

**Las lecturas sin cambios no desencadenan evaluaciones innecesarias.**

Así el sistema responde con agilidad sin recalcular constantemente la misma situación.

<a id="water-regional"></a>
#### Cómo afectan los contratos regionales de agua a la financiación

Las compras y ventas de agua se tienen en cuenta al calcular cuánta agua deben producir tus propias instalaciones.

Las reglas son sencillas:

- **Las compras reducen la demanda local.** Solo las compras activas de forma fiable cuentan como agua disponible.
- **Las compras no pueden cubrir más que la demanda total de la ciudad** en el cálculo de la capacidad local.
- **Las ventas aumentan el requisito de producción local.** La ciudad debe aportar agua suficiente tanto para sus propias necesidades como para sus obligaciones de exportación.
- **Las ventas existentes se tratan de forma conservadora.** Cuando el Ministerio no gestiona los contratos, cuenta como obligación todas las ventas listadas, sea cual sea su estado, o las exportaciones que informa el juego si esa cifra es mayor.
- **Las ventas no se mantienen durante un déficit real de agua.** El planificador de contratos da prioridad al suministro de la propia ciudad.

#### ¿Cómo decide el Ministerio si un contrato es rentable?

Un contrato de agua no merece la pena automáticamente solo porque otra ciudad se ofrezca a comprar o vender agua.

El Ministerio compara el precio del contrato con el coste operativo adicional o el ahorro que crearía.

Por ejemplo:

- **Vender agua:** el Ministerio calcula cuánta financiación adicional haría falta para producir el agua que se vende.
- **Comprar agua:** calcula cuánta financiación local se podría ahorrar contando con el agua comprada.

**Coste marginal del agua**

```math
\Delta(q)=X^*(L+q)-X^*(L)
```

Donde:

- **L** es la capacidad local de agua necesaria actual.
- **q** es la cantidad del contrato, positiva para una venta y negativa para una compra.
- **X\*(n)** es el gasto mensual de agua más bajo necesario para dar al menos **n** unidades.
- **Δ(q)** es la variación de los gastos mensuales de funcionamiento.

**En una venta de agua**, el Ministerio compara el dinero recibido con el gasto adicional de producción local.

Una venta solo merece la pena si el vecino paga más que el coste extra.

**En una compra de agua**, el Ministerio compara el precio del contrato con el ahorro conseguido al reducir la producción local.

Una compra solo merece la pena si cuesta menos que ese ahorro.

**Ejemplo**

Supongamos que producir 5.000 unidades de agua adicionales exige subir el presupuesto mensual del departamento en §300.

Una ciudad vecina se ofrece a comprar esas 5.000 unidades.

| Pago mensual del vecino | Resultado económico |
|---|---|
| §200 | No merece la pena: pérdida de §100 |
| §300 | Punto de equilibrio |
| §400 | Merece la pena: ganancia de §100 |

Por eso el Ministerio tiene en cuenta **la variación real de los costes de producción**, en lugar de suponer que todos los contratos regionales son beneficiosos.

Las reglas completas de precios y gestión de contratos se explican en [Acuerdos con vecinos](deals.md#neighbor-deals).

#### ¿Y si la gestión automática del agua o de los contratos está desactivada?

Las dos funciones pueden funcionar de forma independiente.

| Ajustes | Qué ocurre |
|---|---|
| **Gestionar el agua: Activado** / **Gestionar acuerdos de agua: Activado** | El Ministerio puede optimizar tanto la financiación del agua como los contratos regionales de agua. |
| **Gestionar el agua: Activado** / **Gestionar acuerdos de agua: Desactivado** | La financiación del agua se optimiza con tus contratos existentes, que el Ministerio deja sin cambios. |
| **Gestionar el agua: Desactivado** / **Gestionar acuerdos de agua: Activado** | La financiación del agua se queda en su nivel actual, pero los contratos regionales de agua aún pueden gestionarse. |
| **Ambos desactivados** | El Ministerio no cambia automáticamente ni la financiación del agua ni los contratos de agua. |

La gestión automática de los contratos regionales de agua también requiere que esté activada la opción principal **Gestionar acuerdos con vecinos**.

<a id="water-farm-icon"></a>
#### Una pequeña mejora visual: avisos de agua en las granjas

El Ministerio también hace un pequeño ajuste en la interfaz del juego.

**Oculta el icono de aviso original de «sin agua» sobre las parcelas agrícolas.**

Este cambio afecta solo al icono de aviso que se muestra encima de las granjas.

**No**:

- Suministra agua a las granjas.
- Cambia su demanda de agua.
- Cambia las decisiones del juego sobre los edificios agrícolas.
- Cambia el sistema de agua en sí.
- Oculta los avisos de agua de los edificios residenciales, comerciales o industriales.

El ajuste se aplica cuando se inicia SimCity 4 y es independiente de los ajustes de gestión del Ministerio.

Solo se aplica a la versión compatible del juego. En otras versiones, el Ministerio deja este comportamiento de la interfaz sin cambios.

<a id="water-safety"></a>
### 4. Condiciones de seguridad

El Ministerio sigue varias reglas de seguridad para evitar dejar tu ciudad sin agua o dañar su red de tuberías.

#### Antes de cambiar la financiación del agua

El Ministerio solo cambia la financiación cuando puede fiarse de la información del juego.

Comprueba que:

- El departamento del agua y su financiación actual se pueden leer correctamente.
- La demanda de agua y la capacidad de producción están disponibles, y la demanda es mayor que cero.
- El juego ha actualizado el recuento de edificios con agua y sin agua.
- El consumo de agua informado es coherente con la demanda.
- Todos los productores de agua usan el sistema de financiación estándar del juego.

**Si falla cualquiera de estas comprobaciones, el Ministerio deja la financiación del agua sin cambios.**

Esto es especialmente importante justo después de abrir una ciudad, cuando algunas estadísticas del agua pueden no estar disponibles todavía.

Para mayor precisión técnica, el Ministerio acepta una pequeña diferencia entre el consumo y la demanda:

```math
W \le 1.01 \times D + 1
```

Donde **W** es el consumo de agua informado y **D** es la demanda.

Un consumo superior a este límite se trata como una lectura incoherente.

Si incluso un solo productor de agua usa un mecanismo de financiación no compatible, la gestión automática de la financiación del agua se desactiva para esa sesión de la ciudad, en lugar de arriesgarse a cálculos incorrectos.

<a id="water-no-cut"></a>
#### ¿Cuándo se niega el Ministerio a reducir la financiación?

Aunque un presupuesto menor permitiera ahorrar dinero, el Ministerio rechaza una reducción en las situaciones siguientes:

| Situación | Por qué se rechaza la reducción |
|---|---|
| **Producción de agua insuficiente** | Hay edificios sin agua y la capacidad de producción ya es inferior a la demanda. Recortar la financiación empeoraría la escasez. |
| **Tuberías dañadas** | Con **Proteger las tuberías** activado, cualquier tubería deteriorada o reventada impide las reducciones de financiación. |
| **Riesgo de deterioro de las tuberías** | Con la protección de las tuberías activada, el Ministerio no reduce la financiación por debajo del umbral del 50%. |
| **Capacidad insuficiente tras el recorte** | La financiación propuesta no daría agua suficiente para la demanda, la reserva y las exportaciones. |
| **Ningún nivel de financiación adecuado** | Si ni el mejor ajuste disponible cumple el requisito, el Ministerio aumenta la financiación cuando es posible en lugar de reducirla. |
| **Información poco fiable o modificada** | Las lecturas son incompletas, incoherentes o han cambiado desde que se hizo el cálculo. |
| **Financiación fija** | La financiación del agua no está bajo gestión automática. |

**Cada reducción propuesta se comprueba dos veces:** una al calcular el plan y otra inmediatamente antes de aplicar la nueva financiación.

Así el Ministerio no usa un cálculo desfasado si la situación de tu ciudad cambia.

**¿Y si hay edificios sin agua, pero la producción es suficiente?**

Es distinto.

Por ejemplo, una ciudad puede producir 30.000 unidades y necesitar solo 20.000, y aun así tener edificios sin agua porque están desconectados de la red de tuberías.

En ese caso, **aumentar la financiación no resolverá el problema de conexión**.

El Ministerio aún puede reducir el presupuesto, siempre que se sigan cumpliendo los requisitos de capacidad y las reglas de protección de las tuberías.

#### ¿Qué pasa si falla un cambio de financiación?

Antes de cambiar el presupuesto, el Ministerio comprueba que la financiación actual siga coincidiendo con el valor usado en su cálculo.

Si has movido el control deslizante a mano, el cambio previsto se omite.

Tras aplicar un cambio, vuelve a leer la financiación para verificar que el juego ha aceptado el nuevo valor.

Si la verificación falla:

1. El Ministerio intenta restaurar la financiación anterior.
2. Los cambios automáticos de la financiación del agua se detienen durante el resto de la sesión actual de la ciudad.
3. La gestión de la financiación del agua puede reiniciarse cuando vuelvas a abrir la ciudad.

**Los contratos regionales de agua no se ven afectados por esta parada de seguridad.** Su gestión puede continuar de forma independiente.

<a id="water-settings"></a>
### 5. Ajustes

La gestión del agua se puede configurar en **Opciones de juego → Ministerio de Optimización**, en la página **Servicios**.

| Ajuste | Por defecto | Rango | Qué hace |
|---|---|---|---|
| [Ministerio activo](options.md#opt-master) | Activado | Activado / Desactivado | Activa o desactiva todo el Ministerio. |
| [Gestionar el agua](options.md#opt-water) | Activado | Activado / Desactivado | Ajusta automáticamente la financiación del departamento del agua de la ciudad. |
| [Reserva de agua](options.md#opt-waterreserve) | 10% | 10–50% | Establece la capacidad local mínima de producción de agua que debe quedar de reserva. |
| [Proteger las tuberías](options.md#opt-waterpipes) | Activado | Activado / Desactivado | Impide las reducciones que podrían causar deterioro de las tuberías o interferir en la recuperación de las tuberías dañadas. |
| [Gestionar acuerdos con vecinos](options.md#opt-deals) | Activado | Activado / Desactivado | Activa la gestión automática de los contratos regionales. |
| [Gestionar acuerdos de agua](options.md#opt-tradewater) | Activado | Activado / Desactivado | Activa las compras y ventas automáticas de agua. |
| [Comprar a una ciudad para vender a otra](options.md#opt-arbitrage) | Desactivado | Activado / Desactivado | Permite que el Ministerio compre y venda agua dentro del mismo plan regional. |

#### ¿Cómo funciona la reserva de agua?

La reserva por defecto es del **10%**.

Por ejemplo, si tu ciudad debe producir 20.000 unidades localmente, el Ministerio procura mantener una capacidad de al menos **22.000 unidades**.

La reserva es un mínimo, no un objetivo exacto.

El Ministerio puede mantener más capacidad cuando lo exijan los porcentajes de financiación disponibles o el ajuste de protección de las tuberías.

Aumentar la reserva da más capacidad de producción de reserva, pero también puede aumentar tus gastos mensuales de agua.

#### ¿Qué hace Proteger las tuberías?

Con esta opción activada, el Ministerio:

- Usa normalmente el **50% como mínimo para las nuevas opciones de financiación**.
- Nunca reduce la financiación hasta un nivel que cause deterioro de las tuberías.
- Rechaza las reducciones mientras el juego informe de tuberías deterioradas o reventadas.

**No aumenta automáticamente la financiación solo para reparar tuberías.**

Si fijas a mano la financiación por debajo del 50%, el Ministerio puede dejarla ahí mientras la producción siga siendo suficiente. La subirá cuando haga falta para cumplir el requisito de suministro de agua, dentro de su rango protegido normal de financiación.

Si desactivas la protección de las tuberías, el Ministerio puede elegir una financiación tan baja como el **0%**, siempre que se sigan cumpliendo los requisitos de suministro de agua.

Esto puede ahorrar más dinero, pero permite que la red de tuberías se deteriore con el tiempo.

#### Ajuste avanzado (solo en el archivo INI)

| Ajuste | Por defecto | Rango permitido | Qué hace |
|---|---|---|---|
| `WaterCadenceDays` | 10 | 1–30 | Establece el intervalo de la revisión periódica de la financiación del agua, en días simulados. |

Por ejemplo, con `WaterCadenceDays=5` las revisiones periódicas se hacen cada cinco días simulados.

Las comprobaciones diarias de cambios significativos y la revisión al comienzo de cada mes simulado siguen siendo independientes.

<a id="water-examples"></a>
### 6. Ejemplos prácticos

Los ejemplos siguientes muestran cómo elige el Ministerio la financiación del agua y cómo afectan al resultado sus protecciones de seguridad.

Todos los ejemplos usan la **reserva de agua del 10%** por defecto, salvo que se indique lo contrario.

Las capacidades de producción y los gastos del departamento son ilustrativos.

#### Ejemplo 1: reducir la financiación del agua sin provocar escasez

Imagina una ciudad con:

- **Demanda de agua:** 20.000 unidades.
- **Capacidad de producción actual:** 34.000 unidades.
- **Financiación actual:** 100%.
- **Gasto mensual en agua al 100%:** §2.000.
- **Contratos regionales:** ninguno.
- **Tuberías dañadas:** ninguna.
- **Edificios sin agua:** ninguno.

**Paso 1: calcular la capacidad necesaria**

Con una reserva del 10%:

```math
L = \lceil 20000 \times 1.10 \rceil = 22000
```

La ciudad necesita capacidad para **22.000 unidades**.

**Paso 2: encontrar la financiación mínima suficiente**

| Financiación | Capacidad prevista | Gasto mensual | Resultado |
|---|---:|---:|---|
| 77% | 21.675 | §1.540 | No basta |
| **78%** | **22.100** | **§1.560** | **Basta** |
| 100% | 34.000 | §2.000 | Exceso de capacidad innecesario |

**El Ministerio elige una financiación del 78%.**

Con este nivel, la ciudad conserva agua suficiente para su demanda y su reserva.

La curva de reparación de las tuberías también es positiva (+0,0112), así que la reducción no provoca su deterioro.

**Ahorro mensual: §440.**

#### Ejemplo 2: ¿cuánto cuesta proteger las tuberías?

Imagina una ciudad con:

- **Demanda de agua:** 10.000 unidades.
- **Capacidad de producción al 100%:** 60.000 unidades.
- **Gasto mensual al 100%:** §3.000.

Con una reserva del 10%, solo necesita:

```math
L = 10000 \times 1.10 = 11000
```

La ciudad tiene mucha más capacidad de producción de agua de la que necesita.

Vamos a comparar el resultado con la protección de las tuberías activada y desactivada.

**Con Proteger las tuberías activado**

El Ministerio no puede elegir un nuevo objetivo de financiación por debajo del 50%.

Con una financiación del 50%, el rendimiento de producción es del 30%:

```math
C(50)=60000\times0.30=18000
```

Es más que suficiente para suministrar las 11.000 unidades necesarias.

**El Ministerio elige una financiación del 50%**, con un coste de **§1.500 al mes**.

La red de tuberías se mantiene estable.

**Con Proteger las tuberías desactivado**

El Ministerio puede considerar niveles de financiación más bajos.

| Financiación | Capacidad prevista | Resultado |
|---|---:|---|
| 30% | 10.800 | No basta |
| **31%** | **11.160** | **Basta** |
| 50% | 18.000 | Más de lo necesario |

Sin la protección de las tuberías, una **financiación del 31%** es suficiente.

El gasto mensual baja a **§930**.

Sin embargo, la curva de deterioro de las tuberías pasa a ser negativa, de aproximadamente −0,0032.

La red se irá desgastando poco a poco.

**El equilibrio económico**

| Ajuste | Financiación | Gasto mensual | Efecto en las tuberías |
|---|---:|---:|---|
| **Protección activada** | 50% | §1.500 | Estables |
| Protección desactivada | 31% | §930 | Deterioro gradual |

**Proteger las tuberías cuesta §570 más al mes en este ejemplo.**

Ese gasto adicional evita el deterioro causado por una financiación insuficiente.

Decidir si el ahorro compensa el daño a largo plazo depende del jugador, que puede activar o desactivar la protección.

#### Ejemplo 3: comprar y vender agua al mismo tiempo

Volvamos a la ciudad del Ejemplo 1:

- **Demanda de agua:** 20.000 unidades.
- **Capacidad de producción normal:** 34.000 unidades.
- **Coste mensual al 100%:** §2.000.

Esta vez, la ciudad tiene contratos regionales:

- **Agua comprada:** 5.000 unidades.
- **Agua vendida:** 8.000 unidades.

La compra está confirmada como activa.

**Paso 1: calcular la capacidad local necesaria**

```math
L = \lceil(20000-5000)\times1.10\rceil+8000
```

```math
L=16500+8000=24500
```

La ciudad necesita **24.500 unidades** de capacidad de producción local.

**Paso 2: elegir la financiación**

| Financiación | Capacidad prevista | Resultado |
|---|---:|---|
| 83% | 24.225 | No basta |
| **84%** | **24.650** | **Basta** |
| 100% | 34.000 | Más de lo necesario |

**El Ministerio elige una financiación del 84%.**

El gasto mensual en agua pasa a ser:

```math
X(84)=2000\times0.84=1680
```

**Gasto mensual del departamento de aguas: §1.680**, sin contar el coste ni los ingresos de los contratos regionales.

Este ejemplo muestra por qué el Ministerio debe considerar las compras y las ventas en conjunto, en lugar de mirar solo la demanda de la propia ciudad.

#### Ejemplo 4: ¿compensa vender agua?

Volvamos a la ciudad del Ejemplo 1, sin contratos regionales.

Su financiación más barata en este momento es:

- **Financiación del 78%.**
- **22.100 unidades de capacidad de producción.**
- **§1.560 al mes.**

Una ciudad vecina ofrece ahora comprar **5.000 unidades de agua**.

Para abastecer esta venta, la capacidad local necesaria aumenta:

```math
L=22000+5000=27000
```

El Ministerio comprueba cuánta financiación hace falta:

| Financiación | Capacidad prevista | Gasto mensual |
|---|---:|---:|
| 78% | 22.100 | §1.560 |
| 89% | 26.775 | §1.780 |
| **90%** | **27.200** | **§1.800** |

**Vender el agua adicional obliga a subir la financiación del 78% al 90%.**

El gasto extra es:

```math
\Delta=1800-1560=240
```

**La venta supone §240 más al mes en producción local de agua.**

Ahora el Ministerio puede comparar ese coste con el dinero que ofrece la ciudad vecina:

| Pago mensual | Resultado antes de otras consideraciones del contrato |
|---|---|
| §200 | §40 de pérdida |
| §240 | Punto de equilibrio |
| §300 | §60 de ganancia |

**La venta solo compensa económicamente si el vecino paga más de §240 al mes**, sujeta a las demás reglas de seguridad de los contratos.

Así evalúa el Ministerio el comercio regional: con los costes de producción reales, sin aceptar todos los acuerdos disponibles.

Consulta [Acuerdos con vecinos](deals.md#neighbor-deals).

#### Ejemplo 5: las tuberías dañadas impiden un ahorro

Volvamos a la ciudad del Ejemplo 1.

Normalmente, el Ministerio reduciría la financiación del agua del 100% al 78%, con un ahorro de §440 al mes.

Sin embargo, el juego informa ahora de **tres tuberías en mal estado**.

Con **Proteger las tuberías** activado, el Ministerio rechaza la reducción.

| Situación | Financiación | Gasto mensual |
|---|---:|---:|
| Sin tuberías dañadas | 78% | §1.560 |
| **Tres tuberías en mal estado** | **100%** | **§2.000** |

**El Ministerio mantiene la financiación al 100% mientras haya tuberías dañadas.**

Con este nivel de financiación, la curva de las tuberías es positiva (+0,020), lo que permite que la red se recupere.

Cuando el juego informe de cero tuberías en mal estado y cero reventadas, el Ministerio podrá reducir la financiación al 78% en su siguiente revisión.

**¿Aumenta el Ministerio la financiación para reparar las tuberías más rápido?**

No.

Conserva la financiación existente mientras las tuberías estén dañadas, pero no la aumenta solo para acelerar las reparaciones.

Es una distinción deliberada entre **evitar recortes peligrosos** y **gestionar activamente las reparaciones de las tuberías**.

#### Ejemplo 6: los edificios sin agua no siempre indican una producción insuficiente

Imagina una ciudad con:

- **Demanda de agua:** 20.323 unidades.
- **Capacidad de producción:** 34.081 unidades.
- **Edificios sin agua:** 55.

A primera vista, los 55 edificios sin agua sugieren una escasez de agua.

Sin embargo, la ciudad ya tiene capacidad de producción más que suficiente para cubrir su demanda.

Lo más probable es que algunos edificios no estén bien conectados a la red de agua.

**El Ministerio no aumenta automáticamente la financiación para resolver un problema de conexión.**

Incluso puede reducirla, siempre que la nueva capacidad cumpla la reserva exigida y las reglas de protección de las tuberías.

Ahora veamos una situación distinta:

- **Demanda de agua:** 20.323 unidades.
- **Capacidad de producción:** 19.000 unidades.
- **Edificios sin agua:** 55.

Aquí, la capacidad de producción está realmente por debajo de la demanda.

**El Ministerio rechaza cualquier reducción de la financiación hasta que se resuelva la escasez.**

La distinción importa: **la falta de agua causada por una producción insuficiente se puede resolver con financiación; la causada por tuberías desconectadas, no.**

#### Ejemplo 7: ni siquiera la financiación máxima basta

Imagina una ciudad con:

- **Demanda de agua:** 35.000 unidades.
- **Capacidad de producción al 100%:** 34.000 unidades.
- **Reserva de agua:** 10%.
- **Contratos regionales:** ninguno.

La capacidad necesaria es:

```math
L=35000\times1.10=38500
```

La ciudad necesita **38.500 unidades**.

Sin embargo, incluso con una financiación del 120%, el rendimiento de producción de agua es solo del 110%.

```math
C(120)=34000\times1.10=37400
```

**Capacidad máxima disponible: 37.400 unidades.**

Eso son **1.100 unidades menos que la capacidad necesaria**, incluida la reserva.

Por eso el Ministerio sube la financiación del agua al **120%**, el valor más alto disponible.

Si la gestión de los contratos regionales de agua está activada, también puede intentar comprar agua adicional a una ciudad vecina.

Si no, el jugador debe aumentar la capacidad de producción local, por ejemplo construyendo otra bomba de agua.

**La financiación no puede producir más agua de la que permite la capacidad máxima del juego.**

<a id="water-limitations"></a>
### 7. Limitaciones conocidas

El Ministerio optimiza la financiación del agua de forma automática, pero varias limitaciones vienen del propio SimCity 4.

#### Producción de agua y financiación

- **La capacidad de producción se actualiza con retraso.** Tras un cambio de financiación, SimCity 4 normalmente actualiza la capacidad de agua notificada al mes simulado siguiente. Hasta entonces, el Ministerio puede calcular temporalmente un nuevo plan con una capacidad que aún refleja la financiación anterior. La posterior actualización de la capacidad provoca otra evaluación, lo que le permite corregir el plan.
- **Solo hay un porcentaje de financiación del agua.** El Ministerio no puede financiar de forma distinta cada bomba, torre o planta de tratamiento.
- **La financiación no puede reparar todos los problemas de suministro.** Una fuente de agua contaminada o cerrada puede producir menos agua. Aumentar el presupuesto no elimina el problema de fondo.
- **La financiación máxima tiene límites.** Si la producción sigue siendo insuficiente al 120%, la ciudad necesita más capacidad de producción de agua o compras regionales fiables.

#### Protección de las tuberías

- **La protección evita recortes; no gestiona activamente las reparaciones.** El Ministerio no sube la financiación solo para reparar más rápido las tuberías en mal estado o reventadas.
- **Las redes con financiación insuficiente por decisión manual no se restablecen automáticamente al 50%.** Si estableces tú mismo una financiación inferior al 50% y la producción sigue siendo suficiente, el Ministerio puede dejarla sin cambios. Sube la financiación cuando hace falta más producción, usando el intervalo protegido normal.
- **El deterioro de las tuberías no se predice a lo largo del tiempo.** El Ministerio usa la curva de financiación del juego para determinar si las tuberías se deterioran o se recuperan, pero no calcula cuántos meses tardará en fallar una tubería concreta.
- **Desactivar la protección permite el deterioro.** Con **Proteger las tuberías** desactivado, el Ministerio puede elegir una financiación inferior al 50% si aun así se puede cubrir la necesidad de agua.

#### Distribución del agua

- **Los edificios sin agua pueden tener problemas de conexión.** Que la producción de toda la ciudad sea suficiente no garantiza que el agua llegue a todos los edificios. El Ministerio no puede arreglar las conexiones de tuberías que faltan.
- **El Ministerio no construye infraestructuras.** Las nuevas bombas, torres de agua y redes de tuberías siguen siendo responsabilidad del jugador.

#### Compatibilidad de edificios

**Todos los productores de agua deben usar el mecanismo de financiación estándar de SimCity 4.**

Si incluso un solo edificio productor de agua usa un sistema de financiación personalizado incompatible, el Ministerio no gestiona automáticamente el presupuesto de agua de la ciudad.

Esta restricción impide que el Ministerio haga cálculos basados en supuestos que no se cumplen para todos los productores.

**El objetivo del Ministerio es mantener una producción de agua suficiente al menor coste seguro, no sustituir la responsabilidad del jugador sobre la infraestructura del agua.**

[Volver al contenido](index.md#contents) | [Volver al README](../../README.es.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
