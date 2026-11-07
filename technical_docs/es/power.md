[English](../en/power.md) | [Português](../pt/power.md) | [Français](../fr/power.md) | **Español**

[Volver al contenido](index.md#contents) | [Volver al README](../../README.es.md)

<a id="power"></a>
## B. Electricidad

El Ministerio gestiona automáticamente **cada central eléctrica de forma individual**, ajustando su financiación para ofrecer electricidad suficiente a la ciudad con los costes de funcionamiento más bajos posibles.

Tiene en cuenta:

- Cuánta electricidad necesita tu ciudad.
- La reserva de seguridad que has elegido (**20% por defecto**).
- La electricidad comprada a ciudades vecinas o vendida a ellas.
- La capacidad de generación, el estado y el coste de funcionamiento de cada central eléctrica.

El Ministerio revisa la financiación de la electricidad **cada 10 días simulados**. También hace una comprobación rápida cada día simulado y puede ajustar la financiación antes cuando la demanda, la capacidad de producción o los contratos regionales cambian de forma significativa.

En lugar de aplicar el mismo porcentaje de financiación a todas las centrales, busca **la combinación más barata de niveles de financiación** que cubra las necesidades de la ciudad.

El Ministerio nunca construye, demuele ni sustituye centrales eléctricas. Solo ajusta su financiación, dentro de su propio rango permitido y con sus comprobaciones de seguridad, para proteger el suministro eléctrico y la red de líneas eléctricas.

<a id="power-vanilla"></a>
### 1. Cómo funciona la electricidad en SimCity 4

#### Demanda, producción y consumo de electricidad

SimCity 4 registra varios valores de electricidad de tu ciudad:

| Término | Qué significa |
|---|---|
| **Demanda** | La electricidad que piden los edificios que tienen corriente en ese momento. |
| **Necesidad** | La estimación del juego de la electricidad que necesita la ciudad, incluidos los edificios que ahora no tienen corriente. Suele ser algo mayor que la demanda. |
| **Consumo** | La electricidad que realmente se entrega a los edificios. |
| **Capacidad de producción** | La electricidad que pueden producir ahora tus centrales eléctricas, según su financiación y su estado. |
| **Importaciones** | Electricidad comprada a ciudades vecinas. |
| **Exportaciones** | Electricidad vendida a ciudades vecinas. |

Todos estos valores se miden en las unidades de electricidad del juego por mes simulado.

**No toda escasez de electricidad se debe a una producción insuficiente.**

Hay dos situaciones posibles:

- **No hay electricidad suficiente:** tus centrales eléctricas y las importaciones disponibles no pueden cubrir la demanda de la ciudad. El Ministerio puede responder subiendo la financiación o ajustando los contratos regionales.
- **La electricidad no llega a algunos edificios:** la ciudad tiene capacidad de generación suficiente, pero algunos edificios no están bien conectados a la red eléctrica. Subir la financiación no lo resolverá. Tienes que arreglar las conexiones tú mismo.

El Ministerio distingue entre estas situaciones para no aumentar innecesariamente el gasto en electricidad.

**¿Cómo afecta la electricidad importada a la producción local?**

Según nuestras observaciones, SimCity 4 usa la electricidad importada antes de tomar energía de las centrales locales. Esto significa que una central eléctrica a veces puede mostrar un uso muy bajo, o incluso nulo, mientras una ciudad vecina suministra la electricidad.

El Ministerio tiene en cuenta los contratos regionales al decidir cuánta capacidad de producción local hace falta.

#### Cómo afecta la financiación a la producción de una central eléctrica

Cada central eléctrica tiene tres características importantes:

- **Producción máxima:** cuánta electricidad puede producir la central con su capacidad nominal.
- **Financiación:** el porcentaje de su presupuesto de funcionamiento que se asigna a la central.
- **Estado:** el estado actual de la central, de 0 (totalmente deteriorada) a 1 (nueva).

La producción real de una central depende tanto de **su financiación como de su estado**.

Por ejemplo, una central eléctrica nueva financiada al 100% puede producir toda su capacidad nominal. A medida que su estado se deteriora, su producción disponible disminuye.

<a id="power-plant-efficiency"></a>
**Eficiencia de la central (juego base)**

```math
\text{Efic} = \frac{\text{Curva}(f)}{100} \times \text{Estado}
```

Donde:

- **f** es el porcentaje de financiación de la central.
- **Curva(f)** es la eficiencia que ofrece la curva de financiación del juego.
- **Estado** es el estado actual de la central, entre 0 y 1.
- **Efic** es la eficiencia resultante, expresada como fracción.

**Producción efectiva (juego base)**

```math
\text{Prod} = \text{ProdMáx} \times \text{Efic}
```

Donde:

- **ProdMáx** es la producción máxima de la central.
- **Prod** es su capacidad de generación efectiva.

**Ejemplo: una central eléctrica con una producción máxima de 7.500 unidades**

Supongamos que la central está en un estado casi perfecto (0,9998).

| Financiación | Eficiencia tras el estado | Producción efectiva |
|---|---:|---:|
| 100% | Alrededor del 100% | Unas 7.498 unidades |
| 95% | Alrededor del 90% | Unas 6.748 unidades |

Reducir la financiación solo **5 puntos porcentuales** reduce la capacidad de producción en aproximadamente un **10%**.

Esto ocurre porque la relación del juego entre financiación y producción no es proporcional.

<a id="power-efficiency-curve"></a>
#### La relación entre financiación y eficiencia

SimCity 4 usa una curva integrada para determinar cuánta electricidad puede producir una central eléctrica con cada nivel de financiación.

Estos son los puntos de control del juego original:

| Financiación de la central | Eficiencia de producción |
|---:|---:|
| 0% | 0% |
| 50% | 25% |
| 90% | 80% |
| 100% | 100% |
| 120% | 110% |

**El detalle importante es que reducir la financiación también reduce la eficiencia.**

Por ejemplo:

- Con **50% de financiación**, una central ofrece solo **el 25% de su producción nominal**.
- Con **90% de financiación**, ofrece **el 80% de su producción nominal**.
- Con **100% de financiación**, ofrece **el 100% de su producción nominal**.
- Con **120% de financiación**, ofrece **el 110% de su producción nominal**.

Por tanto, financiar por encima del 100% es menos rentable: pagas un 20% más para obtener solo un 10% adicional de capacidad de generación.

**¿Cómo se calculan los valores intermedios?**

El juego usa interpolación lineal entre los puntos de control. El Ministerio aplica la misma regla.

**Interpolación de la curva**

```math
y(x) = y_i + \frac{x-x_i}{x_{i+1}-x_i}(y_{i+1}-y_i)
\qquad \text{for } x_i \le x \le x_{i+1}
```

Esto significa simplemente que, entre dos puntos conocidos, la eficiencia cambia a un ritmo constante.

Por ejemplo, entre el 90% y el 100% de financiación, la eficiencia de producción sube del 80% al 100%. Con un 95% de financiación, la eficiencia es por tanto del 90%.

Fuera del intervalo definido, la curva mantiene su primer o su último valor.

**Eficiencia con los niveles de financiación que usa el Ministerio**

Normalmente, el Ministerio considera niveles de financiación del 60% al 120%.

| Financiación (%) | 60 | 65 | 70 | 75 | 80 | 85 | 90 | 95 | 100 | 105 | 110 | 115 | 120 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| Eficiencia (%) | 38,75 | 45,625 | 52,5 | 59,375 | 66,25 | 73,125 | 80 | 90 | 100 | 102,5 | 105 | 107,5 | 110 |
| Producción por § gastado (relativa al 100%) | 0,65 | 0,70 | 0,75 | 0,79 | 0,83 | 0,86 | 0,89 | 0,95 | 1,00 | 0,98 | 0,95 | 0,93 | 0,92 |

La última fila compara cuánta capacidad de generación ofrece cada nivel de financiación por el dinero gastado, en relación con el 100% de financiación.

**¿Qué significa esto para el presupuesto de electricidad de tu ciudad?**

Con **100% de financiación**, una central eléctrica ofrece la mayor capacidad de generación por simoleón gastado.

Sin embargo, eso **no** significa que todas las centrales deban financiarse siempre al 100%.

Imagina que tu ciudad tiene dos centrales eléctricas, pero no necesita toda su producción combinada.

Financiar ambas por igual puede costar más que hacer funcionar una cerca del 100% y mantener la otra con una financiación menor.

**Esto es exactamente lo que optimiza el Ministerio:** compara las combinaciones disponibles y elige la más barata que sigue proporcionando la electricidad necesaria y la reserva de seguridad.

Consulta el [Ejemplo 3](#power-example-mixed) para ver un cálculo con dos centrales distintas.

#### Financiación de cada central eléctrica y costes de funcionamiento

A diferencia del servicio de agua, **la financiación de la electricidad se gestiona por separado para cada central eléctrica**.

Cada central tiene su propio ajuste de financiación y su propio coste mensual de funcionamiento.

Esto permite al Ministerio aumentar la financiación de una central y reducir la de otra, según la combinación que resulte más económica.

**¿Qué niveles de financiación puede usar el Ministerio?**

La configuración de electricidad del juego señala el **60%** como su *financiación mínima aceptable*. Sin embargo, ese valor no debe confundirse con un límite estricto demostrado del control de financiación del juego.

La función local de presupuesto en la que se apoya acepta valores desde el 0%, sujetos al máximo del departamento.

El optimizador del Ministerio considera deliberadamente niveles de financiación del **60% al 120%**, en porcentajes enteros, y aplica comprobaciones adicionales antes de cambiar el presupuesto de una central.

Estas comprobaciones garantizan que los cambios planificados cubran las necesidades de electricidad de la ciudad y no provoquen el deterioro de las líneas eléctricas.

**¿Cómo se calcula el coste mensual de funcionamiento?**

El coste de funcionamiento cambia en proporción al porcentaje de financiación.

**Gasto mensual de la central (juego base)**

```math
E(f) = \operatorname{round}\left(C \times \frac{f}{100}\right)
```

Donde:

- **C** es el coste mensual de funcionamiento de la central con el 100% de financiación.
- **f** es el porcentaje de financiación elegido.
- **E(f)** es el gasto mensual estimado con esa financiación.

Por ejemplo, una central eléctrica que cuesta §253 al mes con el 100% de financiación costaría aproximadamente:

| Financiación | Coste mensual de funcionamiento |
|---|---:|
| 60% | §152 |
| 84% | §213 |
| 100% | §253 |
| 120% | §304 |

Con **84% de financiación**, la ciudad ahorra aproximadamente **§40 al mes** en comparación con financiar esa central al 100%.

Sin embargo, el Ministerio solo elige el 84% si la capacidad de producción resultante es suficiente para el plan general de electricidad.

El juego informa por separado del coste nominal de generación de una central y de lo que se cobra realmente con su financiación actual. El Ministerio usa esta última relación para estimar los gastos, redondeando a simoleones enteros, y lee el coste nominal más reciente de la central cada vez que recalcula el plan.

#### ¿Por qué el departamento de electricidad muestra un porcentaje de financiación distinto?

Aunque las centrales eléctricas se financian individualmente, SimCity 4 también muestra en el presupuesto de la ciudad un porcentaje de financiación global del departamento de electricidad.

**No es un control deslizante de financiación adicional.**

Se calcula como un **promedio ponderado por costes** de los niveles de financiación de cada central.

Esto significa que una central grande y cara influye más en el porcentaje del departamento que una pequeña y barata.

**Financiación global de la electricidad (juego base)**

```math
F_{\text{dept}} = \frac{\sum_i C_i f_i}{\sum_i C_i}
```

Donde:

- **Cᵢ** es el coste mensual nominal de funcionamiento de la central *i*.
- **fᵢ** es el porcentaje de financiación de esa central.
- **Fdept** es el porcentaje de financiación global del departamento de electricidad.

**Ejemplo**

Imagina que las centrales eléctricas de tu ciudad tienen un coste nominal de funcionamiento combinado de **§11.660 al mes**.

Una central cuesta §300 al mes y está financiada al 95%, mientras que todas las demás siguen al 100%.

Esa central ahorra §15 al mes.

La financiación global del departamento de electricidad pasa a ser:

```math
F_{\text{dept}} = \frac{11660-15}{11660}\times100
\approx 99.87\%
```

Aunque una central esté financiada al 95%, el departamento en su conjunto sigue mostrando aproximadamente un **99,87% de financiación**.

Esto es normal.

**El Ministerio gestiona cada central eléctrica de forma individual. El presupuesto global de electricidad refleja el resultado combinado de esas decisiones.**
#### Líneas eléctricas: cómo afecta la financiación a la red

En SimCity 4, la financiación de la electricidad afecta a algo más que a las centrales. También influye en el estado de la **red de líneas eléctricas** de la ciudad.

El juego usa la financiación global del departamento de electricidad para determinar si las líneas eléctricas se deterioran o se reparan con el tiempo.

| Financiación del departamento | Efecto en las líneas eléctricas |
|---:|---|
| 0% | Deterioro rápido (−0,020) |
| 25% | Deterioro (−0,005) |
| 50% | Sin deterioro ni reparación (0) |
| 100% | Reparación (+0,025) |
| 110% | Reparación más rápida (+0,050) |

Los valores intermedios se calculan con la interpolación lineal estándar del juego.

**El umbral importante es el 50% de financiación del departamento:**

- **Por debajo del 50%:** las líneas eléctricas se deterioran poco a poco.
- **Con el 50%:** su estado se mantiene estable.
- **Por encima del 50%:** la red se repara poco a poco por sí sola.

Normalmente, el Ministerio considera niveles de financiación **entre el 60% y el 120% para cada central eléctrica gestionada**. Esto ayuda a mantener la financiación global de la electricidad por encima del nivel en el que las líneas eléctricas se deterioran.

Además, comprueba el efecto de los cambios de financiación en la red de líneas eléctricas antes de aplicarlos.

**El Ministerio no recorta sin más los presupuestos de las centrales sin tener en cuenta el estado de la red eléctrica.**

<a id="power-aging"></a>
#### Estado y envejecimiento de las centrales eléctricas

Las centrales eléctricas no duran para siempre. Mientras funcionan, su estado va disminuyendo poco a poco.

En SimCity 4, **la rapidez con que envejece una central eléctrica depende sobre todo de lo intensamente que se use**, aunque su financiación también influye.

El Ministerio lee información de cada central, entre otros datos:

- **Ritmo base de envejecimiento:** el parámetro de envejecimiento subyacente de la central.
- **Umbral de envejecimiento:** el estado en el que el juego considera vieja la central. Era 0,9 (90%) en las centrales examinadas.
- **Coste de reposición:** la cantidad necesaria para construir otra central del mismo tipo.

Estos valores ayudan al Ministerio a estimar el coste económico de hacer funcionar una central cuando el cálculo opcional del envejecimiento está activado.

**Financiación y uso no son lo mismo**

Esta distinción es esencial.

- La **financiación** determina cuánta electricidad *puede producir* una central.
- El **uso** mide qué parte de esa capacidad disponible utiliza realmente la ciudad.

Por ejemplo:

| Central eléctrica | Financiación | Uso real |
|---|---:|---:|
| Central A | 100% | 30% |
| Central B | 70% | 95% |

La central A está financiada por completo, pero produce mucha menos electricidad de la que podría.

La central B recibe menos financiación, pero la ciudad usa casi toda la electricidad que puede producir con ese nivel de financiación.

**Uso de la central (juego base)**

```math
u = \frac{\text{Used}}{\text{Out}}
```

Donde:

- **Used** es la electricidad que realmente se extrae de la central.
- **Out** es la capacidad de generación disponible de la central.
- **u** es la proporción de esa capacidad que se está usando.

Por ejemplo, si una central puede producir 10.000 unidades y la ciudad extrae 8.000, su uso es del **80%**.

Cuando una central no produce nada, el Ministerio considera que su uso es cero.

### ¿Con qué rapidez envejecen las centrales eléctricas?

El cálculo del envejecimiento del juego depende de tres factores:

1. El ritmo base de envejecimiento de la central.
2. Lo intensamente que se usa la central.
3. Su nivel de financiación.

**Envejecimiento estimado por mes simulado (SimCity 4, modelado por el Ministerio)**

```math
A = \frac{B \times U(\min(\max(u,0),2)) \times G\left(\frac{f}{100}\right)}{16}
```

Donde:

- **B** es el parámetro base de envejecimiento de la central.
- **u** es su uso actual, expresado como fracción.
- **U(u)** es el multiplicador de envejecimiento asociado al uso.
- **f** es su porcentaje de financiación.
- **G(f/100)** es el multiplicador de envejecimiento asociado a la financiación.
- **A** es el estado perdido estimado por mes simulado.

En palabras sencillas:

**Envejecimiento de la central = Envejecimiento base × Efecto del uso × Efecto de la financiación × Factor de tiempo**

El ritmo base y las dos curvas de envejecimiento proceden de la mecánica del juego.

El **factor de tiempo de 1/16** se estableció midiendo siete centrales eléctricas durante 13 meses simulados. Es un valor medido experimentalmente, no una constante leída directamente del juego.

Por tanto, las estimaciones de envejecimiento resultantes deben tomarse como aproximaciones, no como predicciones exactas de la vida útil que le queda a una central.

#### 1. Cómo afecta la financiación al envejecimiento

La curva de envejecimiento según la financiación del juego tiene tres puntos de control:

| Financiación | Multiplicador de envejecimiento |
|---:|---:|
| 0% | 1,10 |
| 100% | 1,00 |
| 120% | 0,90 |

Los valores entre estos puntos se interpolan.

Dentro del intervalo normal de financiación del Ministerio:

| Financiación | Multiplicador de envejecimiento |
|---:|---:|
| 60% | 1,04 |
| 80% | 1,02 |
| 90% | 1,01 |
| 100% | 1,00 |
| 110% | 0,95 |
| 120% | 0,90 |

**La financiación tiene un efecto directo relativamente pequeño en el envejecimiento.**

Por ejemplo, reducir la financiación de una central del 100% al 60% eleva su multiplicador directo de envejecimiento de 1,00 a 1,04, un aumento de solo el **4%**.

Sin embargo, reducir la financiación también reduce la cantidad de electricidad que la central puede producir. Si la ciudad sigue extrayendo la misma cantidad, el uso de la central aumenta, y eso puede tener un efecto mucho mayor en el envejecimiento.

#### 2. Cómo afecta el uso al envejecimiento

La curva de envejecimiento según el uso del juego es mucho más significativa:

| Uso de la central | Multiplicador de envejecimiento |
|---:|---:|
| 0% | 0,10 |
| 25% | 0,20 |
| 50% | 0,45 |
| 75% | 0,85 |
| 90% | 1,25 |
| 100% | 1,55 |
| 110% o más | 2,00 |

Como en las demás curvas, los valores intermedios se calculan con interpolación lineal.

**Cuanto más trabaja una central, más rápido envejece.**

Por ejemplo:

- Con un **50% de uso**, el multiplicador de envejecimiento es 0,45.
- Con un **90% de uso**, llega a 1,25.

Por tanto, una central que funciona al 90% de uso envejece aproximadamente **2,8 veces más rápido** que la misma central funcionando al 50% de uso, suponiendo que todo lo demás sea igual.

Por eso reducir sin más la financiación de las centrales no siempre es lo ideal desde el punto de vista económico.

**Ejemplo: reducir la financiación puede casi duplicar el envejecimiento**

Imagina una central financiada inicialmente al 100%, con la ciudad usando alrededor del 60% de su producción disponible.

El Ministerio reduce la financiación para ahorrar costes de funcionamiento. Ahora la central puede producir menos electricidad, pero la ciudad sigue extrayendo más o menos la misma cantidad.

Su uso puede subir hasta cerca del 83%.

El multiplicador de envejecimiento por uso sube de aproximadamente 0,60 a 1,06, mientras que el multiplicador directo de financiación cambia solo ligeramente.

**El ritmo de envejecimiento estimado de la central pasa a ser casi el doble.**

Esto no significa necesariamente que reducir la financiación sea una mala decisión. Puede que el ahorro mensual siga valiendo más que el desgaste adicional.

El Ministerio puede comparar estos costes cuando **Tener en cuenta el desgaste de las centrales** está activado.

**¿Qué hace «Tener en cuenta el desgaste de las centrales»?**

Esta opción está **desactivada por defecto**.

- **Desactivada:** el Ministerio minimiza los gastos mensuales de funcionamiento de la electricidad manteniendo el suministro y la reserva de seguridad necesarios.
- **Activada:** el Ministerio incluye además el coste económico estimado del desgaste de las centrales al comparar combinaciones de financiación.

La opción puede favorecer una distribución de financiación distinta cuando la reducción del desgaste compensa el gasto de funcionamiento adicional.

**No impone un porcentaje mínimo de financiación propio, no impide el envejecimiento ni sustituye automáticamente las centrales viejas.**

<a id="power-usage-myth"></a>
#### ¿Es el 80% de uso un límite de seguridad?

**No. SimCity 4 no tiene ningún umbral repentino de envejecimiento o de apagón en el 80% de uso.**

La confusión viene de la curva de envejecimiento del juego.

Con aproximadamente un **80,6% de uso**, el multiplicador de envejecimiento llega a 1,0. Por debajo de ese punto el multiplicador es menor; por encima, aumenta.

Pero la curva es continua.

Una central que funciona al 81% de uso no se vuelve insegura de repente. Del mismo modo, una central que funciona al 95% de uso no provoca automáticamente un apagón.

**Los apagones ocurren cuando el suministro de electricidad es insuficiente o no llega a los edificios, no porque una central concreta supere el 80% de uso.**

Por eso el Ministerio no impone un tope del 80% de uso.

En su lugar, usa el ajuste **Reserva de electricidad** para mantener capacidad de generación de sobra.

#### ¿Cómo afecta la reserva de electricidad al uso?

La reserva de electricidad por defecto es del **20%**.

Para una ciudad sencilla abastecida por una sola central local, sin comercio regional de electricidad, la relación es aproximadamente:

```math
u \approx \frac{1}{1+r}
```

Donde **r** es la reserva configurada expresada como fracción (0,20 para una reserva del 20%).

Esto supone que la central aporta exactamente la capacidad necesaria e ignora las pequeñas diferencias causadas por el redondeo y por los requisitos adicionales del juego.

| Reserva de electricidad | Uso aproximado de la central | Multiplicador de envejecimiento por uso |
|---|---:|---:|
| 10% | 90,9% | 1,28 |
| **20% (por defecto)** | **83,3%** | **1,07** |
| 25% | 80,0% | 0,98 |
| 30% | 76,9% | 0,90 |
| 50% | 66,7% | 0,72 |

Una reserva mayor suele dejar más capacidad de generación de sobra y puede reducir lo intensamente que tiene que trabajar una central.

Sin embargo, **más reserva también puede exigir más financiación y, por tanto, aumentar los gastos mensuales**.

Con varias centrales eléctricas, la relación se vuelve más complicada, porque SimCity 4 decide cómo se reparte la demanda de electricidad entre ellas.

El Ministerio no controla directamente ese reparto.

Su objetivo es mantener capacidad total suficiente, con la reserva que hayas elegido, minimizando los costes incluidos en su optimización.

#### Comercio regional de electricidad

El Ministerio también tiene en cuenta la electricidad comprada a las ciudades vecinas o vendida a ellas.

- **Comprar electricidad** reduce la capacidad de generación local que necesita la ciudad.
- **Vender electricidad** aumenta la capacidad que deben aportar las centrales de la propia ciudad.

Por ejemplo, si tu ciudad necesita 10.000 unidades y de forma fiable importa 3.000, sus centrales eléctricas no necesitan producir localmente las 10.000 unidades enteras.

En cambio, vender 2.000 unidades a un vecino significa que esas unidades deben estar disponibles además de las necesidades de tu propia ciudad.

El Ministerio incluye estos contratos al calcular la capacidad necesaria y elegir la financiación de las centrales.

Cómo crea, modifica, valora y cancela los contratos se explica en [Acuerdos con vecinos](deals.md#neighbor-deals).

<a id="power-problem"></a>
### 2. El problema de la gestión

Gestionar la electricidad a mano es más complicado que ajustar un único control deslizante del presupuesto.

El Ministerio debe equilibrar varios factores:

- **Evitar el gasto innecesario:** una central financiada al 100% puede producir mucha más electricidad de la que necesita la ciudad.
- **Mantener un suministro suficiente:** reducir demasiado la financiación puede dejar edificios sin electricidad.
- **Proteger la red eléctrica:** una financiación global de la electricidad muy baja puede hacer que las líneas eléctricas se deterioren.
- **Tener en cuenta la eficiencia de las centrales:** la financiación y la producción no son proporcionales. Recortar todas las centrales al mismo porcentaje no es necesariamente la solución más barata.
- **Tener en cuenta el estado y el envejecimiento:** las centrales más viejas producen menos electricidad, y las que trabajan más cerca de su plena capacidad suelen envejecer más rápido.
- **Considerar el comercio regional:** comprar electricidad puede reducir la producción local necesaria, mientras que venderla la aumenta.
- **Gestionar varias centrales a la vez:** la combinación más barata puede consistir en financiar una central cerca del 100% y otra mucho menos, en lugar de aplicar el mismo porcentaje a ambas.

SimCity 4 no muestra directamente qué combinación de niveles de financiación de las centrales proporcionará la electricidad necesaria al menor coste global.

**El Ministerio calcula esa combinación automáticamente.**

Tiene en cuenta la capacidad, el estado y el gasto de funcionamiento de cada central, junto con la reserva que hayas elegido y los contratos regionales.

Cuando el ajuste opcional del envejecimiento está activado, también incluye el coste económico estimado del desgaste de las centrales.

**El resultado es un presupuesto de electricidad diseñado para cubrir las necesidades de la ciudad sin gastar más de lo que el modelo del Ministerio considera necesario.**
<a id="power-algorithm"></a>
### 3. Cómo optimiza el Ministerio la financiación de la electricidad

El Ministerio no reduce sin más la financiación de todas las centrales eléctricas en el mismo porcentaje.

En su lugar, calcula cuánta electricidad necesita la ciudad y compara distintas combinaciones de financiación para encontrar **la solución de menor coste que mantenga una capacidad de generación suficiente**.

El proceso tiene tres pasos principales:

1. Calcular cuánta electricidad deben aportar las centrales de la ciudad.
2. Calcular cuánto puede producir cada central con distintos niveles de financiación.
3. Encontrar la combinación más barata que cumpla el requisito.

Si **Tener en cuenta el desgaste de las centrales** está activado, el Ministerio también incluye el coste económico estimado del desgaste de las centrales al comparar estas combinaciones.

<a id="power-required-capacity"></a>
#### Paso 1: ¿cuánta electricidad necesita la ciudad?

Primero, el Ministerio calcula la capacidad de generación que debe haber disponible a nivel local.

Tiene en cuenta:

- La demanda actual de electricidad de la ciudad.
- Los edificios que aún puedan necesitar electricidad.
- Tu **Reserva de electricidad** configurada, del 20% por defecto.
- La electricidad comprada a las ciudades vecinas.
- La electricidad vendida a las ciudades vecinas.

**La reserva se aplica solo a la electricidad que la ciudad necesita producir por sí misma.**

Por ejemplo, si tu ciudad necesita 5.000 unidades e importa 2.000 de un vecino, sus centrales eléctricas solo necesitan cubrir las 3.000 unidades restantes, más la reserva configurada.

La electricidad vendida a los vecinos también debe producirse, así que las exportaciones aumentan el requisito local.

**Capacidad de generación local necesaria**

```math
N = \max\left(\left\lceil R \times \max(0,D-B)\right\rceil,\ Q-B\right)+S
```

Donde:

- **D** es la demanda de electricidad de la ciudad.
- **Q** es el requisito total de electricidad del juego, incluidos los edificios que ahora mismo no tienen electricidad.
- **B** es la electricidad comprada a los vecinos con la que el plan puede contar.
- **S** es la electricidad vendida a los vecinos.
- **R** es el multiplicador de la reserva (1,20 para la reserva por defecto del 20%).
- **N** es la capacidad de generación que deben aportar las centrales locales.

El Ministerio usa el mayor de estos dos valores: la demanda local con reserva o el requisito de electricidad restante del juego. Después se suman las exportaciones.

**Ejemplo: una ciudad con 5.000 unidades de demanda**

Supongamos que el requisito total del juego es de 5.050 unidades y que la reserva es del 20%.

| Situación | Capacidad local necesaria |
|---|---:|
| Sin importaciones ni exportaciones de electricidad | 6.000 unidades |
| Importando 2.000 unidades | 3.600 unidades |

Sin importaciones:

```math
N = \max(5000 \times 1.20,\ 5050)=6000
```

Con 2.000 unidades importadas:

```math
N = \max(3000 \times 1.20,\ 3050)=3600
```

**El Ministerio no compra una reserva adicional además de la electricidad importada.** Su reserva protege la parte de la demanda que deben abastecer las centrales locales.

Si el Ministerio gestiona los contratos de electricidad, planifica a la vez los contratos y la financiación de las centrales.

Si no, usa los contratos existentes y cuenta las compras solo cuando están activas de forma fiable. Las compras pendientes, suspendidas o con fallos no se tratan como suministro garantizado. Las ventas existentes se cuentan como obligaciones.

Consulta [Acuerdos con vecinos](deals.md#neighbor-deals) para más información.

#### Paso 2: ¿cuánto puede producir cada central?

Una vez conocida la capacidad necesaria, el Ministerio examina todas las centrales eléctricas que puede gestionar con seguridad.

De cada central considera **los porcentajes de financiación enteros del 60% al 120%**.

También considera la financiación actual de la central, aunque la hayas fijado tú a mano fuera de ese intervalo.

Para cada nivel de financiación posible, el Ministerio calcula:

- La capacidad de generación disponible de la central.
- Su gasto mensual de funcionamiento.
- Su coste estimado de desgaste, cuando la opción del envejecimiento está activada.

**Capacidad de generación planificada**

```math
\text{Cap}(f) = \left\lfloor \text{MaxOut} \times \frac{\text{Curve}(f)}{100} \times \text{Cond} \right\rfloor
```

Donde:

- **MaxOut** es la producción máxima de la central.
- **Curve(f)** es la eficiencia del juego con el nivel de financiación **f**.
- **Cond** es el estado actual de la central, de 0 a 1.
- **Cap(f)** es la capacidad de generación resultante, redondeada hacia abajo.

El Ministerio usa el **estado real** de la central, no la capacidad que tenía cuando era nueva.

**Ejemplo: una central eléctrica en buen estado**

Supongamos que una central tiene:

- Producción máxima: 6.000 unidades.
- Estado actual: 98%.
- Financiación propuesta: 84%.

Con el 84% de financiación, la curva de eficiencia del juego da 71,75%.

```math
\text{Cap}(84) = \left\lfloor 6000 \times 0.7175 \times 0.98 \right\rfloor = 4218
```

Por tanto, el Ministerio cuenta **4.218 unidades** de capacidad de generación de esta central.

Repite este cálculo para los demás niveles de financiación posibles y para cada central elegible.

**¿Y las centrales que el Ministerio no puede gestionar?**

Algunas centrales no se pueden ajustar con seguridad. Por ejemplo:

- Centrales con información de financiación no compatible o ilegible.
- Centrales cuyos costes de funcionamiento o características de producción no se pueden determinar de forma fiable.
- Plantas de conversión de residuos en energía que ahora mismo queman basura, porque cambiar su financiación podría interferir con el sistema de limpieza de la ciudad.

Estas centrales **no se ignoran**.

El Ministerio incluye en el cálculo su capacidad de generación actual, pero deja su financiación sin cambios. Sus costes se contabilizan sin contar dos veces el mismo gasto.

Si **Gestionar la electricidad** está desactivado, todas las centrales eléctricas conservan su financiación actual. Los contratos de electricidad se pueden seguir gestionando por separado si sus opciones siguen activadas.

<a id="power-optimizer"></a>
#### Paso 3: encontrar la combinación más barata

Aquí es donde el Ministerio marca la diferencia.

Imagina una ciudad con varias centrales eléctricas. Cada central tiene su propio coste de funcionamiento, su propia curva de eficiencia y su propio estado.

Puede haber muchas combinaciones de financiación posibles que proporcionen electricidad suficiente.

**El Ministerio compara estas combinaciones y elige la de menor coste total modelado.**

No reduce sin más primero la central más cara ni financia todas las centrales con el mismo porcentaje.

**Fórmula de optimización de la electricidad**

```math
\min_{f_1,\ldots,f_n}\sum_{i=1}^{n}\left(E_i(f_i)+W_i(f_i)\right)
\quad\text{subject to}\quad
K+\sum_{i=1}^{n}\text{Cap}_i(f_i)\ge N
```

Donde:

- **Eᵢ** es el gasto mensual de funcionamiento de la central *i*.
- **Wᵢ** es su coste estimado de desgaste, o cero cuando la opción del envejecimiento está desactivada.
- **Capᵢ** es su capacidad disponible con la financiación seleccionada.
- **K** es la capacidad combinada de las centrales cuya financiación no se puede cambiar.
- **N** es la capacidad local necesaria calculada en el Paso 1.

En lenguaje sencillo:

**Encontrar la combinación más barata de niveles de financiación de las centrales que produzca electricidad suficiente para la ciudad y su reserva.**

Por ejemplo, la solución más económica podría ser:

| Central | Financiación elegida |
|---|---:|
| Central eléctrica de carbón | 91% |
| Central eléctrica de gas natural | 61% |

Otra ciudad con una demanda, unas centrales o unos costes de funcionamiento distintos podría beneficiarse de una combinación completamente diferente.

Consulta el [Ejemplo 3](#power-example-mixed) para ver un cálculo desarrollado con dos centrales.

**¿Cómo encuentra el Ministerio la mejor combinación?**

Usa un algoritmo de optimización que compara conjuntamente las opciones de financiación de las centrales, en lugar de tomar decisiones independientes para cada una.

Para que los cálculos sean rápidos en ciudades grandes, organiza la capacidad de generación en unidades pequeñas.

**Unidad de capacidad de optimización**

```math
\text{unit} = \max\left(1,\left\lceil\frac{N}{20000}\right\rceil\right)
```

Para ciudades que necesitan hasta **20.000 unidades**, la capacidad de generación se compara de unidad en unidad.

Para ciudades mayores, el Ministerio usa unidades algo más grandes para reducir la cantidad de cálculo.

Esto puede dar un plan con algo más de capacidad de sobra que el mínimo matemático, pero el redondeo es prudente: **el plan seleccionado debe cubrir igualmente la capacidad de generación necesaria**.

Al comparar muchos contratos regionales posibles, el planificador de contratos usa un cálculo más grueso para ganar velocidad. El plan final de las centrales eléctricas usa el cálculo más detallado.

**¿Qué ocurre si varias combinaciones cuestan lo mismo?**

El Ministerio prefiere la combinación que cambia el menor número de centrales.

Por ejemplo, si mantener la financiación actual cuesta exactamente lo mismo que cambiar tres centrales, las deja como están.

Así se evitan ajustes innecesarios del presupuesto.

**¿Qué ocurre si no hay electricidad suficiente?**

A veces ni siquiera financiar todas las centrales disponibles con el ajuste de mayor capacidad basta.

En ese caso, el Ministerio:

1. Elige la mayor capacidad de producción disponible de las centrales gestionadas, normalmente subiéndolas al 120%.
2. Intenta cubrir el déficit restante con compras regionales, si la gestión de los contratos de electricidad está activada.
3. Emite un aviso de un asesor si el déficit se confirma y persiste.

**El Ministerio nunca construye ni sustituye centrales eléctricas.** Si la financiación y las compras disponibles no pueden resolver el problema, tendrás que aportar tú mismo más capacidad de generación.

<a id="power-wear"></a>
#### Opcional: tener en cuenta el desgaste de las centrales

Por defecto, el Ministerio se centra en **minimizar la factura mensual de electricidad**.

Sin embargo, el presupuesto de funcionamiento más barato no siempre es la opción más barata a lo largo de la vida de una central eléctrica.

Como se explicó antes, reducir la financiación puede aumentar lo intensamente que se usa una central, lo que podría acelerar su deterioro.

El ajuste opcional **Tener en cuenta el desgaste de las centrales** permite al Ministerio incluir un coste estimado de ese desgaste.

| Ajuste | Cómo se optimiza la financiación |
|---|---|
| **Desactivado (por defecto)** | Minimiza los gastos mensuales reales de funcionamiento manteniendo la capacidad y la reserva necesarias. |
| **Activado** | Minimiza los gastos de funcionamiento más el coste económico estimado del desgaste de las centrales. |

**Importante:** el coste estimado de desgaste no es un cargo adicional para el tesoro de tu ciudad. Se usa únicamente para comparar opciones de financiación.

Las centrales eléctricas envejecen con normalidad tanto si esta opción está activada como si no.

#### ¿Cómo se calcula el coste del envejecimiento?

Cada central eléctrica tiene un coste de construcción y un umbral de envejecimiento.

El Ministerio estima qué parte del estado útil restante de la central se consume durante un mes simulado y luego asigna un valor monetario a ese desgaste.

**Coste mensual estimado de desgaste**

```math
W(f) = \operatorname{round}\left(P \times \frac{\min\left(A(f),\ \text{Cond}-T\right)}{1-T}\right)
```

Esta fórmula se aplica mientras el estado de la central esté por encima de su umbral de envejecimiento. En caso contrario, el coste de desgaste modelado es cero.

Donde:

- **P** es el coste de construcción o de reposición de la central.
- **A(f)** es su envejecimiento mensual estimado con el nivel de financiación considerado.
- **Cond** es su estado actual.
- **T** es el umbral de estado en el que el juego la considera envejecida.
- **W(f)** es el coste económico estimado del desgaste por mes simulado.

Por ejemplo, si construir una central cuesta §10.000 y el intervalo de estado útil va del 100% al 90%, el Ministerio considera que ese intervalo de estado del 10% tiene un valor económico de §10.000.

Una pequeña parte de ese intervalo consumida durante un mes representa la fracción correspondiente del coste de reposición.

El resultado se redondea a simoleones enteros.

**Costes de construcción de las centrales eléctricas del juego original**

| Central eléctrica | Coste de reposición |
|---|---:|
| Eólica | §500 |
| Gas natural | §9.000 |
| Carbón | §10.000 |
| Petróleo | §17.000 |
| Conversión de residuos en energía | §25.000 |
| Solar | §30.000 |
| Nuclear | §40.000 |
| Hidrógeno | §100.000 |

Para las centrales personalizadas compatibles, el Ministerio lee el coste de construcción que proporciona el propio edificio.

**¿Y si falta la información necesaria?**

Si no se puede determinar de forma fiable el coste de reposición, el ritmo de envejecimiento o el umbral de envejecimiento de la central, el Ministerio simplemente deja fuera del cálculo el coste de desgaste **solo para esa central**.

La central se puede seguir gestionando con normalidad a partir de su gasto de funcionamiento.

Las demás centrales con información de envejecimiento válida siguen incluyendo sus costes de desgaste estimados.

#### ¿Qué diferencia supone esta opción?

El efecto suele ser menor de lo que muchos jugadores esperarían.

Dentro del intervalo de financiación del Ministerio, del 60% al 120%, el multiplicador directo de envejecimiento por financiación varía solo de **1,04 a 0,90**.

El Ministerio calcula el coste de desgaste de cada candidata usando el **uso observado actualmente** de la central. No predice exactamente cómo redistribuirá SimCity 4 la carga de electricidad tras cada posible cambio de financiación.

En consecuencia, el coste de desgaste estimado suele cambiar solo ligeramente entre un nivel de financiación y otro.

En muchas centrales eléctricas del juego original, esto supone apenas unos pocos simoleones al mes.

**Ejemplo**

Supongamos que el Ministerio compara dos opciones de financiación:

| Opción | Gasto de funcionamiento | Desgaste estimado | Coste total modelado |
|---|---:|---:|---:|
| A | §200 | §8 | §208 |
| B | §205 | §2 | §207 |

<italic>Cifras ilustrativas, no mediciones de una central concreta.</italic>

Con el envejecimiento desactivado, gana la **opción A** porque su gasto mensual de funcionamiento es menor.

Con el envejecimiento activado, gana la **opción B** porque su coste combinado de funcionamiento y desgaste es ligeramente menor.

El gasto mensual real de electricidad de la ciudad sería de §205, no de §207. Los §2 adicionales solo existen en la comparación económica.

En la práctica, los dos modos de optimización suelen elegir la misma combinación de financiación, sobre todo cuando la diferencia de desgaste es pequeña.

Consulta el [Ejemplo 5](#power-example-wear) para ver un cálculo con los datos de las centrales del juego.

**Lo que esta opción no hace**

Activar **Tener en cuenta el desgaste de las centrales** no:

- Impide que las centrales eléctricas se deterioren.
- Obliga a las centrales a funcionar por debajo del 80% de uso.
- Introduce un nivel mínimo de financiación adicional.
- Sustituye automáticamente las centrales que envejecen.
- Relaja la reserva de electricidad de la ciudad ni otros requisitos de seguridad.

Simplemente permite al Ministerio considerar **el coste económico estimado del desgaste a largo plazo junto con la factura mensual de funcionamiento**.

El objetivo sigue siendo el mismo: **proporcionar electricidad suficiente a tu ciudad al menor coste modelado, con las opciones y los límites de seguridad que hayas elegido.**
<a id="power-cadence"></a>
#### Paso 4: ¿cuándo ajusta el Ministerio la financiación de la electricidad?

El Ministerio supervisa con regularidad el suministro de electricidad de tu ciudad y ajusta la financiación de las centrales cuando es necesario.

**Normalmente revisa la financiación cada 10 días simulados, pero puede reaccionar antes si la demanda o el suministro de electricidad cambian de forma significativa.**

Todos los plazos se refieren al tiempo simulado del juego.

| Cuándo | Qué hace el Ministerio |
|---|---|
| **Al abrir una ciudad** | Realiza su primera evaluación en cuanto la información de electricidad del juego está disponible y es coherente. No hay un periodo de espera fijo. |
| **Cada día simulado** | Comprueba rápidamente si hay cambios significativos en la demanda, el suministro, el consumo y la capacidad de generación de electricidad y en los contratos regionales. |
| **Cada 10 días simulados** | Realiza una evaluación periódica de las centrales eléctricas y su financiación. |
| **Cuando detecta un cambio significativo** | Recalcula el plan de electricidad sin esperar a la siguiente revisión periódica. |

**¿Qué desencadena una evaluación anticipada?**

En sus comprobaciones diarias, el Ministerio busca:

- **Una carencia real de electricidad:** la ciudad no tiene electricidad suficiente de sus centrales y de las importaciones utilizables para cubrir la demanda.
- **Un cambio de al menos el 1% en la demanda de electricidad**, en comparación con la última evaluación.
- **Un cambio de al menos el 1% en la capacidad de generación local.**
- **Un cambio de al menos el 1% en las importaciones o exportaciones de electricidad contratadas.**

Una lectura sin cambios no desencadena otra evaluación.

El Ministerio también distingue entre una carencia genuina de electricidad y un problema con las conexiones eléctricas de la ciudad.

Por ejemplo, si hay electricidad suficiente pero algunos edificios están desconectados de la red, el Ministerio no aumenta innecesariamente la financiación de las centrales. El problema de conexión se anota en el registro.

**¿Cómo se aplican los cambios de financiación?**

Cuando el Ministerio elige un nuevo nivel de financiación, lo aplica directamente.

Por ejemplo, una central puede pasar del 60% al 95% de financiación en un solo ajuste. No hay un aumento gradual ni un periodo de espera artificial.

Sin embargo, **los aumentos siempre tienen prioridad sobre las reducciones**.

El Ministerio sube la financiación donde hace falta y se asegura de que las demás partes del plan, como las nuevas compras de electricidad, estén en marcha antes de hacer reducciones en una evaluación posterior.

Así se evitan carencias temporales de capacidad mientras se ajustan varias centrales eléctricas o contratos regionales.

Cada evaluación usa la información actual de la ciudad. Si cambias a mano la financiación de una central, o guardas y vuelves a abrir la ciudad, el Ministerio tiene en cuenta la nueva situación.

<a id="power-safety"></a>
### 4. Condiciones de seguridad

El Ministerio sigue varias reglas de seguridad para evitar que sus ajustes de financiación alteren el suministro de electricidad.

#### Antes de calcular un plan

El Ministerio solo toma decisiones de financiación cuando la información del juego es fiable.

- La demanda de electricidad y las demás lecturas necesarias deben estar disponibles y ser coherentes.
- Una lectura de demanda igual a cero se trata como posiblemente sin inicializar, no automáticamente como prueba de que la ciudad no necesita electricidad.
- Al menos una central eléctrica debe tener información legible de financiación y producción antes de que se pueda gestionar la financiación de las centrales.
- Las lecturas contradictorias, como un consumo de electricidad notificado que supera la demanda en más de un 1% más una unidad, impiden los cambios automáticos de financiación.

Una ciudad **sin centrales eléctricas** puede seguir evaluándose para comprar electricidad a las ciudades vecinas, si la gestión de contratos regionales está activada.

#### Proteger el suministro de electricidad

**El Ministerio nunca reduce deliberadamente la financiación por debajo de la capacidad exigida por sus reglas de seguridad.**

Antes de cada reducción de financiación:

1. Vuelve a leer la información de electricidad de la ciudad.
2. Comprueba la capacidad de generación actualizada de cada central eléctrica, según su estado actual y la financiación planificada.
3. Cuenta solo las compras de electricidad que ya están activas y son utilizables.
4. Verifica que la capacidad restante cubrirá la demanda local, la reserva configurada y las exportaciones.

Si la reducción dejara una capacidad insuficiente, **se cancela**.

Una compra de electricidad pendiente, suspendida o con fallos nunca se trata como suministro garantizado.

Los aumentos de financiación no se rechazan solo porque la ciudad siga sin electricidad suficiente. Aunque la producción máxima no pueda resolver todo el déficit, aumentar la capacidad disponible puede seguir ayudando.

#### Proteger las centrales eléctricas y las líneas eléctricas

- Los nuevos objetivos de financiación suelen estar **entre el 60% y el 120%**, en porcentajes enteros.
- El Ministerio respeta los límites de financiación que acepta el sistema de presupuesto del juego.
- Rechaza los cambios de financiación que harían que la red de líneas eléctricas se deteriorase.
- Cuando la ciudad tiene un déficit real de electricidad, el planificador de contratos regionales no mantiene las ventas de electricidad.

El Ministerio nunca construye, demuele, sustituye ni apaga una central eléctrica. Sus acciones se limitan a cambiar los ajustes de financiación elegibles.

#### Comprobar cada cambio de financiación

Antes de cambiar la financiación de una central, el Ministerio verifica que su valor actual siga coincidiendo con el que se usó para calcular el plan.

Si has cambiado ese valor a mano, el Ministerio se salta el ajuste y recalcula en una evaluación posterior.

También identifica las centrales por su posición en el mapa de la ciudad. Si no puede distinguir de forma fiable dos centrales que están en la misma posición, no adivina cuál modificar.

**Tras cada cambio, el Ministerio comprueba que el juego lo haya aceptado.**

Tanto el valor de financiación de la central como su línea de presupuesto deben reflejar el nuevo ajuste.

Si la verificación falla:

- El Ministerio intenta deshacer los cambios realizados durante esa revisión.
- La gestión automática de la financiación de las centrales se detiene durante el resto de la sesión actual de la ciudad.
- Al volver a abrir la ciudad, la gestión puede reiniciarse.
- Los demás servicios y los contratos regionales de electricidad siguen funcionando de forma independiente.

**La prioridad es preservar un suministro de electricidad fiable, aunque eso signifique renunciar a un posible ahorro.**

Consulta [Seguridad general](safety.md#safety) para ver las salvaguardas comunes a todos los servicios.

<a id="power-settings"></a>
### 5. Ajustes

La gestión de la electricidad se puede configurar en **Opciones de juego → Ministerio de Optimización**, en la página **Servicios**.

| Ajuste | Por defecto | Intervalo | Qué hace |
|---|---|---|---|
| [Ministerio activo](options.md#opt-master) | Activado | Activado / Desactivado | Activa o desactiva todo el Ministerio. |
| [Gestionar la electricidad](options.md#opt-power) | Activado | Activado / Desactivado | Optimiza automáticamente la financiación de cada central eléctrica. |
| [Reserva de electricidad](options.md#opt-powerreserve) | 20% | 10–50% | Fija la capacidad de generación local mínima de sobra que el Ministerio intenta mantener. |
| [Tener en cuenta el desgaste de las centrales](options.md#opt-poweraging) | Desactivado | Activado / Desactivado | Incluye el desgaste estimado de las centrales al comparar costes de financiación. |
| [Gestionar acuerdos con vecinos](options.md#opt-deals) | Activado | Activado / Desactivado | Activa la gestión automática de los contratos regionales. |
| [Gestionar acuerdos de electricidad](options.md#opt-tradepower) | Activado | Activado / Desactivado | Activa la gestión automática de las compras y ventas de electricidad. |

La opción [Comprar a una ciudad para vender a otra](options.md#opt-arbitrage) controla si el Ministerio puede comprar electricidad a una ciudad vecina para revenderla a otra. Consulta [Acuerdos con vecinos](deals.md#neighbor-deals).

#### ¿Cómo funciona la reserva de electricidad?

Con una reserva del 20%, el Ministerio intenta normalmente mantener un 20% más de capacidad de generación local que la electricidad que la ciudad debe producir por sí misma.

Por ejemplo, si la ciudad necesita producir 5.000 unidades a nivel local, el Ministerio busca al menos **6.000 unidades de capacidad**.

Es un **mínimo**, no un máximo. El Ministerio puede mantener más capacidad cuando los ajustes disponibles de las centrales o las ventas rentables de electricidad lo justifiquen.

Aumentar la reserva puede dar una protección adicional frente al crecimiento y a los cambios en la demanda, pero también puede aumentar los costes de funcionamiento.

Si desactivas **Gestionar la electricidad**, el Ministerio deja sin cambios la financiación de las centrales. Aun así, puede gestionar las compras y ventas de electricidad si las opciones correspondientes siguen activadas.

#### Ajustes avanzados (solo en el archivo INI)

| Ajuste | Por defecto | Intervalo permitido | Qué hace |
|---|---|---|---|
| `PowerReservePercent` | 20 | 10–50 | Fija el porcentaje de reserva de electricidad. |
| `PowerCadenceDays` | 10 | 1–30 | Fija cada cuánto se hacen las revisiones periódicas de la financiación de la electricidad, en días simulados. |

Por ejemplo, con `PowerCadenceDays=5` el Ministerio hace una revisión periódica cada cinco días simulados en lugar de cada diez.

**Las comprobaciones diarias de cambios urgentes en el suministro son independientes de este intervalo.**

Un valor no válido de `PowerReservePercent` invalida el archivo de configuración de los servicios. En esa situación, el Ministerio no gestiona ningún servicio hasta que se corrige la configuración.

Para la mayoría de los jugadores, los ajustes por defecto son un buen punto de partida.

<a id="power-examples"></a>
### 6. Ejemplos desarrollados

Los siguientes ejemplos muestran cómo elige el Ministerio la financiación para distintas centrales eléctricas y necesidades de electricidad.

Todas las capacidades, gastos de funcionamiento y estados de las centrales son ilustrativos. Los cálculos usan las curvas de eficiencia del juego original.

Salvo que se indique lo contrario, la reserva de electricidad es del **20%** y no hay contratos regionales de electricidad.

Las cifras de envejecimiento de la central de carbón usan valores medidos durante las pruebas: un ritmo base de envejecimiento de 0,001111 por mes y un umbral de envejecimiento de 0,9. Los mismos parámetros de envejecimiento se usan para la central de gas natural cuando hacen falta a modo de ilustración.

<a id="power-example-coal"></a>
#### Ejemplo 1: una central eléctrica de carbón con capacidad sin usar

Considera una central eléctrica de carbón con:

- **Producción máxima:** 6.000 unidades.
- **Coste mensual de funcionamiento al 100%:** §253.
- **Estado actual:** 98%.
- **Demanda de la ciudad:** 3.500 unidades.
- **Requisito del juego:** 3.550 unidades.
- **Reserva de electricidad:** 20%.

**Paso 1: calcular la capacidad necesaria**

```math
N = \max(3500 \times 1.20,\ 3550)=4200
```

La ciudad necesita **4.200 unidades** de capacidad de generación local.

**Paso 2: comparar niveles de financiación**

| Financiación | Capacidad disponible | Coste mensual | Resultado |
|---|---:|---:|---|
| 83% | 4.138 | §210 | No es suficiente |
| **84%** | **4.218** | **§213** | **Suficiente** |
| 85% | 4.299 | §215 | Suficiente, pero más cara |
| 100% | 5.880 | §253 | Mucha más capacidad de la necesaria |

**El Ministerio elige el 84% de financiación.**

La central puede producir 4.218 unidades, algo más que las 4.200 necesarias.

Su coste mensual de funcionamiento baja de §253 a §213.

<box gap={1}>
**Ahorro mensual**
<title size="xl" color="success">§40</title>
</box>

**¿Qué ocurre con el envejecimiento de la central?**

Con el 100% de financiación, la central puede producir 5.880 unidades, mientras que la ciudad extrae 3.500. Su uso es de aproximadamente el **59,5%**.

Con el 84% de financiación, la producción disponible baja a 4.218 unidades. Si la ciudad sigue extrayendo 3.500, el uso sube a aproximadamente el **83%**.

La pérdida mensual de estado modelada cambia:

| Financiación | Uso de la central | Pérdida mensual de estado estimada |
|---|---:|---:|
| 100% | 59,5% | 0,0000418 |
| 84% | 83,0% | 0,0000750 |

**El ritmo de envejecimiento estimado pasa a ser casi 1,8 veces mayor.**

Gran parte de esa diferencia procede del mayor uso, no directamente de la menor financiación.

Esto ilustra por qué reducir los costes de funcionamiento puede acelerar el envejecimiento de la central, y por qué el cálculo opcional del envejecimiento puede ser útil.

<a id="power-example-gas"></a>
#### Ejemplo 2: una central de gas natural

Considera una central de gas natural con:

- **Producción máxima:** 3.000 unidades.
- **Coste mensual al 100%:** §230.
- **Estado:** 100%.
- **Demanda de la ciudad:** 2.000 unidades.
- **Requisito del juego:** 2.020 unidades.

Con una reserva del 20%, la ciudad necesita:

```math
N = \max(2000 \times 1.20,\ 2020)=2400
```

El Ministerio compara los niveles de financiación disponibles:

| Financiación | Capacidad disponible | Resultado |
|---|---:|---|
| 89% | 2.358 | No es suficiente |
| **90%** | **2.400** | **Suficiente** |
| 100% | 3.000 | Más de lo necesario |

**El Ministerio elige el 90% de financiación.**

El coste mensual de funcionamiento pasa a ser:

```math
E = 230 \times 0.90 = 207
```

La central sigue suministrando la electricidad y la reserva necesarias, y ahorra **§23 al mes**.

<a id="power-example-mixed"></a>
#### Ejemplo 3: dos centrales eléctricas trabajando juntas

Este ejemplo muestra por qué el Ministerio no financia todas las centrales con el mismo porcentaje.

Imagina una ciudad con las dos centrales de los ejemplos anteriores:

| Central | Producción máxima | Estado | Coste al 100% |
|---|---:|---:|---:|
| Carbón | 6.000 | 98% | §253 |
| Gas natural | 3.000 | 100% | §230 |
| **Total** | **9.000** | | **§483** |

La ciudad tiene:

- **Demanda de electricidad:** 5.000 unidades.
- **Requisito del juego:** 5.050 unidades.
- **Reserva de electricidad:** 20%.

El Ministerio calcula una capacidad local necesaria de:

```math
N = \max(5000 \times 1.20,\ 5050)=6000
```

**La ciudad necesita 6.000 unidades de capacidad de generación.**

El Ministerio compara distintas estrategias de financiación:

| Estrategia de financiación | Capacidad combinada | Coste mensual |
|---|---:|---:|
| Ambas centrales al 100% | 8.880 | §483 |
| Ambas centrales al 81% | 6.004 | §391 |
| **Carbón al 91%, gas al 61%** | **6.024** | **§370** |

**El Ministerio elige el 91% para la central de carbón y el 61% para la central de gas natural.**

La capacidad resultante es:

```math
\text{Coal} = \lfloor6000\times0.82\times0.98\rfloor=4821
```

```math
\text{Gas} = \lfloor3000\times0.40125\rfloor=1203
```

Juntas aportan **6.024 unidades**, suficientes para cumplir el requisito.

Los gastos mensuales son:

```math
\text{Coal} = \operatorname{round}(253\times0.91)=230
```

```math
\text{Gas} = \operatorname{round}(230\times0.61)=140
```

<grid columns={2} gap={3}>
  <grid-item>
    <box gap={1}>
      <text color="secondary" size="xs">En comparación con el 100% de financiación</text>
      <title size="xl" color="success">§113/mes ahorrados</title>
    </box>
  </grid-item>
  <grid-item>
    <box gap={1}>
      <text color="secondary" size="xs">En comparación con recortes iguales</text>
      <title size="xl" color="success">§21/mes ahorrados</title>
    </box>
  </grid-item>
</grid>

Esto es posible porque **la eficiencia de las centrales eléctricas no es proporcional a la financiación**.

En este caso, mantener la central de carbón cerca de la plena eficiencia y la de gas cerca de la financiación mínima es más barato que reducir ambas por igual.

**¿Y si la ciudad vende electricidad?**

Supongamos que firma un contrato para vender **1.000 unidades** a una ciudad vecina.

La capacidad local necesaria aumenta de 6.000 a **7.000 unidades**.

La combinación más barata del Ministerio pasa a ser:

| Central | Financiación | Capacidad disponible |
|---|---:|---:|
| Carbón | 100% | 5.880 |
| Gas natural | 60% | 1.162 |
| **Total** | | **7.042** |

El coste mensual de funcionamiento es de **§391**.

Por tanto, el Ministerio aumenta la producción para cubrir el contrato de exportación sin sacrificar el suministro propio de la ciudad.

**¿Y si la demanda llega a ser demasiado alta?**

Con el 120% de financiación, estas dos centrales pueden aportar un máximo combinado de **9.768 unidades**.

Si la capacidad local necesaria supera esa cantidad, el Ministerio sube las centrales a los ajustes de mayor capacidad, con un coste mensual combinado de **§580**.

Si la gestión de contratos regionales está activada, también intenta comprar electricidad suficiente para cubrir el déficit restante.

Si no, el jugador debe aportar más capacidad de generación.

<a id="power-example-trade"></a>
#### Ejemplo 4: comprar electricidad a una ciudad vecina

Considera la misma ciudad con dos centrales del Ejemplo 3.

Su demanda de electricidad es de 5.000 unidades, pero ahora tiene un contrato activo para comprar **2.000 unidades** a una ciudad vecina.

Las centrales eléctricas locales solo necesitan suministrar las 3.000 unidades restantes, más la reserva del 20%.

```math
N = \max(3000\times1.20,\ 5050-2000)=3600
```

**Capacidad local necesaria: 3.600 unidades.**

El Ministerio elige:

| Central | Financiación | Capacidad | Coste mensual |
|---|---:|---:|---:|
| Carbón | 62% | 2.440 | §157 |
| Gas natural | 60% | 1.162 | §138 |
| **Total** | | **3.602** | **§295** |

Sin la compra, las centrales cuestan §370 al mes para proporcionar la electricidad y la reserva de la ciudad.

Con la compra, su coste de funcionamiento baja a **§295 al mes**.

Es una reducción de **§75 al mes en los gastos de las centrales eléctricas locales**.

Sin embargo, la ciudad también debe pagar la electricidad importada.

**Que la compra ahorre dinero de verdad depende del precio del contrato.**

El planificador de contratos regionales del Ministerio compara el coste de la compra con el ahorro en producción local antes de decidir si el contrato merece la pena.

Consulta [Acuerdos con vecinos](deals.md#neighbor-deals).

**¿Por qué no reducir más las centrales?**

Normalmente, el Ministerio no selecciona una financiación inferior al 60%.

Incluso si ambas centrales se financian al 60%, aportan aproximadamente 3.440 unidades y cuestan §290 al mes.

Esta financiación mínima evita que el Ministerio reduzca a cero las centrales innecesarias mediante su optimización normal.

<a id="power-example-wear"></a>
#### Ejemplo 5: ¿Cambia el resultado si se tiene en cuenta el desgaste de las centrales?

Volvamos a la central de carbón del Ejemplo 1.

Sus características son:

- **Coste de reposición:** §10.000.
- **Estado actual:** 98%.
- **Umbral de envejecimiento:** 90%.
- **Ritmo base de envejecimiento:** 0,001111 al mes.
- **Uso observado antes de la optimización:** 59,5%.

Ahora activa **Tener en cuenta el desgaste de las centrales**.

El Ministerio estima el coste económico del desgaste para cada nivel de financiación posible, usando el uso que se observa actualmente en la central.

| Financiación | Coste mensual de funcionamiento | Coste estimado del desgaste | Coste total modelado |
|---|---:|---:|---:|
| **84%** | **§213** | **§4** | **§217** |
| 85% | §215 | §4 | §219 |
| 100% | §253 | §4 | §257 |
| 120% | §304 | §4 | §308 |

**El Ministerio sigue eligiendo una financiación del 84%.**

¿Por qué?

Porque la diferencia en el coste estimado del desgaste es demasiado pequeña para justificar un mayor gasto en el presupuesto mensual de la central.

Los §4 de coste de desgaste no se descuentan de tu tesorería. Es una estimación económica que solo se usa para comparar opciones.

**¿Y la ciudad con dos centrales?**

En el Ejemplo 3, con un uso observado del 50% en la central de carbón y del 60% en la de gas, activar el desgaste da el mismo resultado:

- Central de carbón: 91%.
- Central de gas natural: 61%.
- Coste mensual de funcionamiento: §370.
- Desgaste estimado: §7.
- Coste total modelado: §377.

De nuevo, la combinación económicamente más barata es también la combinación de funcionamiento más barata.

**¿Puede el envejecimiento llegar a marcar alguna diferencia?**

Sí, sobre todo en las centrales caras.

Por ejemplo, imagina una central con:

- Coste de reposición: §40.000.
- Ritmo base de envejecimiento: 0,001389 al mes.
- Uso observado: 90%.

Su coste mensual estimado de desgaste es:

| Financiación | Coste estimado del desgaste |
|---|---:|
| 60% | §45 |
| 100% | §43 |
| 120% | §39 |

La diferencia entre una financiación del 60% y una del 120% es de **§6 al mes**.

Eso puede influir en la elección cuando dos combinaciones de financiación tienen gastos de funcionamiento casi idénticos.

Sin embargo, no hace que una financiación mayor sea preferible de forma automática.

**¿Cuánto tiempo puede durar una central eléctrica?**

Con el mismo modelo de envejecimiento, la central de carbón del Ejemplo 1 tardaría aproximadamente el siguiente número de meses simulados en pasar de un estado del 98% a su umbral de envejecimiento del 90%, suponiendo un uso constante y una financiación del 100%:

| Uso de la central | Tiempo estimado hasta el umbral de envejecimiento |
|---|---:|
| 50% | 2.560 meses |
| 80% | 1.170 meses |
| 90% | 920 meses |
| 100% | 740 meses |

Son **estimaciones teóricas**, no duraciones garantizadas.

El juego real puede comportarse de otra manera a medida que cambian el uso, la financiación y el estado.

**Una limitación importante de la opción de envejecimiento**

El Ministerio usa el **uso observado actualmente** en cada central para estimar el desgaste con los posibles niveles de financiación.

No predice con exactitud cómo repartirá el juego la demanda de electricidad después de esos cambios de financiación.

Como demuestra el Ejemplo 1, reducir la financiación puede aumentar mucho el uso real, y eso puede acelerar el envejecimiento más de lo que sugiere la estimación económica inicial.

El Ministerio observa el nuevo uso en las evaluaciones posteriores.

**Por tanto, Tener en cuenta el desgaste de las centrales es una estimación económica, no una garantía de que la central dure más.**

<a id="power-limitations"></a>
### 7. Limitaciones conocidas

El Ministerio gestiona automáticamente la financiación de la electricidad, pero varios aspectos siguen bajo el control de SimCity 4 o del jugador.

#### Compatibilidad de las centrales eléctricas

- **Las curvas de eficiencia y de envejecimiento del juego original están integradas en el Ministerio.** Si otro mod cambia esas curvas, el Ministerio no detecta automáticamente los nuevos valores.
- **Las propiedades de cada central se leen del juego.** La producción máxima, el estado, el coste de funcionamiento, los parámetros de envejecimiento y el coste de construcción se obtienen de cada central, lo que permite gestionar centrales eléctricas personalizadas compatibles.
- **Algunas centrales no se pueden gestionar de forma segura.** Los sistemas de financiación no compatibles o la información ilegible hacen que una central quede excluida de los cambios automáticos de financiación.
- **Las centrales de residuos a energía que están quemando basura en ese momento no se ajustan.** Su financiación se deja sin cambios porque el mismo edificio interviene en la limpieza. Su capacidad de generación disponible sigue contando para el plan de electricidad.

#### Financiación y capacidad de generación

- **El Ministerio no construye ni sustituye centrales.** Una carencia real requiere más capacidad de generación o compras de electricidad.
- **La financiación mínima normal es del 60%.** Una ciudad con muchas más centrales de las que necesita puede seguir pagando al menos el 60% del coste nominal de funcionamiento de cada central gestionada.
- **Las centrales nunca se apagan deliberadamente mediante la financiación.** El Ministerio no elige nuevos objetivos por debajo de su mínimo normal.
- **Las ciudades muy grandes usan redondeo de capacidad.** Cuando la capacidad necesaria supera las 20.000 unidades, el optimizador puede conservar algo más de capacidad de generación de la estrictamente necesaria. Nunca redondea deliberadamente un plan por debajo de lo requerido.

#### Envejecimiento de las centrales

- **El cálculo del envejecimiento usa el uso observado actualmente.** No predice con exactitud cómo repartirá un cambio de financiación la generación de electricidad entre las centrales.
- **El factor de tiempo del envejecimiento se basa en mediciones.** El factor de 1/16 se observó en las pruebas, en lugar de leerse directamente de los datos internos del juego.
- **Los costes de desgaste y las estimaciones de vida útil son aproximados.** Están pensados para apoyar decisiones económicas, no para predecir una fecha exacta de reposición.
- **El Ministerio nunca sustituye automáticamente las centrales envejecidas.** El jugador sigue siendo responsable de construir los reemplazos cuando hagan falta.

#### Distribución de electricidad y comercio regional

- **Se usa la necesidad de electricidad del juego tal como la informa.** El Ministerio no intenta sustituirla por su propia estimación de las necesidades totales de la ciudad.
- **Los problemas de conexión no son problemas de producción.** Si hay electricidad suficiente pero no llega a algunos edificios, cambiar la financiación de las centrales no resolverá el problema.
- **Los contratos de electricidad afectan al plan de financiación.** Las importaciones y exportaciones se incluyen al calcular las necesidades de capacidad local, pero solo las compras utilizables se tratan como suministro disponible.

**La función del Ministerio es gestionar la electricidad que tu ciudad puede producir, no rediseñar la red ni construir nuevas centrales.**

[Volver al contenido](index.md#contents) | [Volver al README](../../README.es.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
