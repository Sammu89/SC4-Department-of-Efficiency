[English](../en/deals.md) | [Português](../pt/deals.md) | [Français](../fr/deals.md) | **Español**

[Volver al contenido](index.md#contents) | [Volver al README](../../README.es.md)

<a id="neighbor-deals"></a>
## E. Acuerdos con vecinos

Las direcciones del ejecutable relacionadas con este capítulo figuran en la [referencia de direcciones del juego](../en/game_addresses.md) (en inglés).

El Ministerio puede gestionar automáticamente los **contratos de electricidad, agua y basura entre tu ciudad y sus vecinas**.

Decide cuándo:

- **Crear un contrato** si comerciar con una vecina beneficiaría a tu ciudad.
- **Aumentar o reducir un contrato existente** a medida que cambian las necesidades de tu ciudad.
- **Cancelar un contrato** cuando ya no es útil o seguro.

El objetivo es **reducir los gastos generales de tu ciudad, ganar dinero con exportaciones rentables y mantener servicios esenciales fiables**.

El Ministerio considera primero las necesidades propias de tu ciudad. Después compara el coste de producir o eliminar un recurso localmente con el precio que ofrecen las ciudades vecinas.

Por ejemplo, comprar electricidad puede ser más barato que aumentar la financiación de una central eléctrica. A la inversa, vender el excedente de electricidad puede ser rentable si los ingresos superan el coste adicional de producción.

El mismo principio se aplica al agua y a la eliminación de basura.

Este capítulo explica **cómo funcionan los contratos regionales y cómo los gestiona el Ministerio**.

Para más detalles sobre la producción local y la financiación, consulta [Electricidad](power.md#power), [Agua](water.md#water) y [Basura y vertederos](garbage.md#garbage-landfills).

<a id="neighbor-deals-mechanism"></a>
### 1. Cómo funcionan los acuerdos con vecinos en SimCity 4

#### Los seis tipos de contratos regionales

SimCity 4 admite seis tipos de contratos con cada ciudad vecina: compra y venta de electricidad, de agua y de servicios de eliminación de basura.

Sin embargo, **los contratos de basura funcionan al revés que los de electricidad y agua**.

Cuando envías basura a una vecina, le pagas para que la elimine. Cuando aceptas la basura de una vecina, esa ciudad te paga a ti.

| Recurso | Tu ciudad paga | Tu ciudad recibe un pago |
|---|---|---|
| **Electricidad** | Comprar electricidad a una vecina | Vender electricidad a una vecina |
| **Agua** | Comprar agua a una vecina | Vender agua a una vecina |
| **Basura** | Enviar tu basura a una vecina para que la elimine | Aceptar la basura de una vecina para eliminarla |

**¿Qué significa esto para tu ciudad?**

- **Comprar electricidad o agua** reduce lo que tu ciudad tiene que producir localmente.
- **Vender electricidad o agua** exige tener capacidad de producción suficiente para abastecer a tu ciudad y a la vecina.
- **Exportar basura** reduce la cantidad que tu sistema de limpieza tiene que gestionar, pero cuesta dinero.
- **Importar basura** genera ingresos, pero aumenta la cantidad que tu sistema de limpieza tiene que gestionar.

El juego permite **un contrato de cada tipo con cada ciudad vecina**.

Las cantidades de los contratos usan las mismas unidades mensuales que se muestran en la ventana de Acuerdos con vecinos del juego. Los precios y los pagos se expresan en simoleones (§) por mes simulado.

#### ¿Qué conexiones hacen falta?

Los contratos regionales requieren conexiones adecuadas entre las ciudades vecinas.

La conexión depende del recurso:

| Recurso | Conexión necesaria |
|---|---|
| **Electricidad** | Una línea eléctrica que cruce la frontera de la ciudad |
| **Agua** | Una tubería de agua que cruce la frontera de la ciudad |
| **Basura** | Una carretera o conexión de transporte adecuada que reconozca el sistema de contratos del juego |

En electricidad y agua, la regla es sencilla: la red correspondiente debe cruzar la frontera regional.

Las conexiones de basura son más complicadas porque SimCity 4 realiza comprobaciones internas distintas.

**¿Qué se ha confirmado sobre las conexiones de basura?**

La comprobación de presencia de conexiones regionales del juego reconoce:

- Carreteras.
- Ferrocarriles.

Esa comprobación concreta no reconoce las autopistas como conexiones de basura.

Sin embargo, el juego también usa una búsqueda distinta cuando comprueba ciudades vecinas y contratos concretos. Esa búsqueda examina varios tipos de conexión, entre ellos autopistas, avenidas, carreteras y caminos de tierra.

**Estas dos comprobaciones no son lo mismo.**

Que un tipo de carretera aparezca en la segunda búsqueda no demuestra, por sí solo, que pueda establecer un contrato de basura elegible.

Por eso el Ministerio se apoya en las comprobaciones reales de conexión y de elegibilidad de contratos del juego, en lugar de suponer que cualquier conexión de transporte es válida.

**¿Y si se pierde una conexión?**

En una venta, la conexión de la ciudad local debe seguir activa.

En una compra, la conexión correspondiente del lado de la ciudad vecina debe existir y estar activa.

Si un contrato pierde la conexión que necesita, SimCity 4 puede señalar el problema durante el mantenimiento mensual de los contratos.

**Un contrato no puede considerarse fiable solo porque siga apareciendo en la lista.**

#### ¿Qué información tiene el juego sobre las ciudades vecinas?

SimCity 4 guarda un pequeño registro regional de cada ciudad.

Para cada recurso, ese registro incluye:

| Información | Qué representa |
|---|---|
| **Producido** | El recurso producido localmente. |
| **Importado** | La cantidad comprada o recibida de las ciudades vecinas. |
| **Demandado** | La necesidad propia de la ciudad de ese recurso. |
| **Exportado** | La cantidad comprometida con las ciudades vecinas. |
| **Coste unitario** | El coste de referencia del juego para ese recurso. |
| **Tesorería** | El dinero disponible de la ciudad. |

**Una limitación importante: las ciudades vecinas no se simulan mientras juegas tu ciudad actual.**

SimCity 4 usa la última información guardada de esas vecinas.

Por ejemplo, imagina que tu ciudad vecina tuvo en su día un gran excedente de electricidad. Si más tarde cambias sus centrales eléctricas pero no guardas esa ciudad, la información regional puede no reflejar tus últimos cambios.

El Ministerio debe trabajar con los registros de las ciudades vecinas que SimCity 4 pone a su disposición.

**¿Cuándo se actualizan estos registros?**

El juego actualiza la información regional de la ciudad actual:

- En las actualizaciones mensuales.
- Cuando se solicitan propuestas de contratos regionales, por ejemplo al abrir la ventana de Acuerdos con vecinos.

En la electricidad, *Producido* representa la capacidad de generación con la financiación actual de las centrales, mientras que *Demandado* usa la necesidad mensual de electricidad del juego, que puede ser algo superior al consumo real.

**¿Representan Importado y Exportado entregas medidas?**

No necesariamente.

En los registros regionales, estas cifras se basan en **cantidades mensuales contratadas**, no en una medición de lo que se movió físicamente ese día.

Por ejemplo, un contrato para vender 5.000 unidades de electricidad puede contar como 5.000 unidades exportadas en las cifras regionales desde el momento en que existe el contrato.

Normalmente el juego cuenta la cantidad completa del contrato en ambos lados, salvo que el contrato esté cancelado, no sea válido o todavía no se haya incluido en los registros.

Esta distinción importa cuando el Ministerio calcula si se puede ofrecer otro contrato.

<a id="neighbor-deals-max"></a>
#### ¿Cuánto pueden comerciar las ciudades vecinas?

SimCity 4 no permite un comercio ilimitado.

La cantidad máxima depende de dos cosas:

1. **Cuánto excedente tiene la ciudad vendedora.**
2. **Cuánto suministro adicional puede recibir la ciudad compradora.**

El máximo real es el menor de esos dos límites.

**Cantidad máxima del contrato (juego base)**

```math
Q_{\max} =
\max\left(
0,\min\left(S_{\mathrm{surplus}},B_{\mathrm{need}}\right)
\right)
```

Donde:

- **Qmax** es la cantidad máxima que el juego permite para ese contrato.
- **Ssurplus** es el excedente disponible de la ciudad vendedora.
- **Bneed** es el suministro adicional permitido a la ciudad compradora.

El juego calcula estos valores a partir de los registros regionales de las ciudades.

**Suministro adicional permitido a la ciudad compradora**

```math
B_{\mathrm{need}} =
F \times
\max\left(
B_D+B_E,\frac{M}{F}+1
\right)
-(B_P+B_I)+q_{\mathrm{cur}}
```

**Excedente disponible de la ciudad vendedora**

```math
S_{\mathrm{surplus}} =
(S_P+S_I)-(S_D+S_E)+q_{\mathrm{cur}}
```

Donde:

- **P** significa Producido.
- **I** significa Importado.
- **D** significa Demandado.
- **E** significa Exportado.
- **S** identifica a la ciudad vendedora.
- **B** identifica a la ciudad compradora.
- **qcur** es la cantidad de un contrato existente de este tipo entre las dos ciudades, o cero si no hay ningún contrato.
- **F** es el factor de sobredemanda del comprador del juego, **2,5** por defecto.
- **M** es la cantidad mínima de contrato del juego, **1.000 unidades** por defecto.

Todas las cantidades de recursos son unidades mensuales.

**¿Qué significa el factor de 2,5?**

SimCity 4 permite que un comprador tenga entre suministro contratado y local hasta **2,5 veces su propia demanda más sus compromisos de exportación**, sujeto a los demás límites del juego.

Es un tope máximo, no la obligación de comprar tanto.

El vendedor, por su parte, solo puede ofrecer el excedente que queda tras contar sus propias necesidades y sus compromisos existentes.

**Ejemplo: vender electricidad a una ciudad vecina**

Imagina que tu ciudad tiene:

- Producción de electricidad: **14.000 unidades**.
- Necesidad de electricidad: **10.300 unidades**.
- Ningún contrato de electricidad existente.

La ciudad vecina tiene:

- Producción de electricidad: **3.000 unidades**.
- Necesidad de electricidad: **6.000 unidades**.
- Ningún contrato existente.

**Paso 1: calcular tu excedente**

```math
S_{\mathrm{surplus}}=14000-10300=3700
```

Tu ciudad tiene **3.700 unidades** disponibles para vender.

**Paso 2: calcular lo que puede comprar la vecina**

```math
B_{\mathrm{need}}=
2.5\times6000-3000=12000
```

Según la fórmula del juego, la vecina puede recibir hasta **12.000 unidades adicionales**.

**Paso 3: determinar el contrato máximo**

```math
Q_{\max}=\min(3700,12000)=3700
```

**SimCity 4 permite un contrato de hasta 3.700 unidades al mes.**

Aunque la vecina podría aceptar más, tu ciudad no tiene ahora más excedente que ofrecer.

| Limitación | Cantidad |
|---|---:|
| Excedente de tu ciudad | 3.700 unidades |
| Suministro que puede comprar la vecina | 12.000 unidades |
| **Contrato máximo permitido** | **3.700 unidades** |

Si tu ciudad aumenta su capacidad de generación, la cantidad de contrato permitida también puede aumentar.

Sin embargo, la fórmula del juego por sí sola no garantiza que se ofrezca un contrato. Deben cumplirse además otras condiciones.

**¿Qué ocurre cuando ya existe un contrato?**

Al calcular cuánto puede renegociarse un contrato existente, el juego vuelve a sumar su cantidad actual a ambos lados del cálculo.

Así se evita que el contrato existente reduzca artificialmente su propio máximo.

Por ejemplo, si ya vendes 2.000 unidades, el juego todavía puede evaluar si el contrato puede aumentarse o ajustarse, en lugar de tratar esas 2.000 unidades como permanentemente no disponibles para renegociar.

El juego no redondea la cantidad máxima de contrato calculada.

Además, **el mínimo de 1.000 unidades se usa para decidir si se puede ofrecer una propuesta**. No se aplica automáticamente como paso de redondeo a la cantidad máxima.

<a id="neighbor-deals-offer"></a>
#### ¿Cuándo ofrece SimCity 4 un contrato regional?

Aunque dos ciudades tengan conexiones compatibles y recursos suficientes para comerciar, el juego puede no ofrecer un contrato nuevo.

SimCity 4 también comprueba si **la ciudad que paga tiene dinero suficiente para sostener el acuerdo**.

La cantidad mínima de contrato del juego original es de **1.000 unidades al mes**.

El juego exige que la ciudad que paga tenga:

- Una tesorería superior a **§5.000**.
- Dinero suficiente para pagar el precio mensual del contrato mínimo.
- Dinero suficiente para cubrir **más de 60 pagos mensuales** a ese precio.

La última condición equivale a **cinco años de pagos en tiempo de juego simulado**.

**Requisitos para proponer un contrato (juego base)**

```math
\text{ofrecido}\iff
Q_{\max}\ge M
\;\land\;
T>T_{\min}
\;\land\;
T>P(M)
\;\land\;
T>60\times P(M)
```

Donde:

- **Qmax** es la cantidad máxima de contrato permitida.
- **M** es la cantidad mínima de contrato, normalmente 1.000 unidades.
- **T** es la tesorería de la ciudad que paga.
- **Tmin** es la tesorería mínima exigida, normalmente §5.000.
- **P(M)** es el precio mensual de un contrato de tamaño mínimo.
- **60** es el número de pagos mensuales que el juego exige que el comprador pueda sostener.

La comparación es estricta para los valores de tesorería: tener exactamente la cantidad exigida no basta.

En cambio, una cantidad máxima de contrato de exactamente 1.000 unidades sí cumple la condición de cantidad mínima.

**¿Quién necesita el dinero?**

Depende del contrato.

| Contrato | Ciudad cuya tesorería se comprueba |
|---|---|
| Tu ciudad compra electricidad | Tu ciudad |
| Tu ciudad vende electricidad | La vecina |
| Tu ciudad compra agua | Tu ciudad |
| Tu ciudad vende agua | La vecina |
| Tu ciudad exporta basura | Tu ciudad |
| Tu ciudad importa basura | La vecina |

Recuerda que las exportaciones de basura son un servicio que compras, mientras que las importaciones de basura son un servicio que prestas.

**Ejemplo: por qué puede no aparecer un contrato**

Supongamos que el precio mensual mínimo del contrato es de **§200**.

El juego exige que la ciudad que paga tenga más de:

```math
60\times200=12000
```

**§12.000**, además de cumplir las demás condiciones.

| Tesorería de la ciudad que paga | Oferta del contrato |
|---|---|
| §5.000 | No se ofrece |
| §10.000 | No se ofrece |
| §12.000 | No se ofrece |
| **§12.001** | **Puede ofrecerse** |

Con §12.001 se cumple la condición de tesorería. El contrato sigue necesitando una conexión válida, una cantidad de comercio suficiente y las demás condiciones de elegibilidad del juego.

**¿Por qué puede seguir sin estar disponible un acuerdo potencialmente rentable?**

Porque **la rentabilidad económica y la elegibilidad no son lo mismo**.

Una ciudad puede estar dispuesta a comprar electricidad, por ejemplo, pero SimCity 4 no ofrecerá el contrato hasta que el vendedor tenga al menos 1.000 unidades de excedente disponible.

Un vendedor que podría aumentar su producción con más financiación no recibe automáticamente una oferta basada en esa capacidad futura hipotética.

La oferta solo es posible cuando las cifras regionales del juego muestran realmente un excedente suficiente.

Del mismo modo, una ciudad puede tener recursos suficientes pero carecer de la tesorería o de la conexión de red necesarias.

**El Ministerio debe respetar estas restricciones del juego original.** No puede dar por sentado que un contrato económicamente atractivo esté disponible antes de que SimCity 4 lo considere elegible.

<a id="neighbor-deals-pricing"></a>
#### ¿Cómo se calculan los precios de los contratos regionales?

SimCity 4 calcula los precios de los contratos a partir del **coste medio que le supone al vendedor suministrar el recurso**.

Después añade un pequeño margen de beneficio.

Esto se aplica a la electricidad, al agua y a los servicios de eliminación de basura.

La distinción importante es que **el precio de los contratos del juego se basa en el coste medio, no en cuánto costaría de más suministrar ese contrato concreto**.

Esa diferencia es esencial para que el Ministerio decida qué contratos merece la pena firmar.

#### Paso 1: el coste medio del vendedor

Primero, el juego calcula cuánto gasta la ciudad vendedora por unidad de suministro disponible.

**Coste unitario del vendedor (juego base)**

```math
u=\frac{E_{\mathrm{dept}}+C_{\mathrm{buy}}}{I+P+1}
```

Donde:

- **Edept** es el gasto mensual del vendedor en el departamento correspondiente.
- **Cbuy** es lo que paga el vendedor por comprar el mismo recurso a otras ciudades.
- **I** es el recurso importado por el vendedor.
- **P** es el recurso producido localmente por el vendedor.
- **u** es el coste medio por unidad resultante.

El **1** adicional del denominador forma parte de la fórmula del juego.

**Ejemplo: una ciudad que vende electricidad**

Supongamos que tu ciudad produce 14.000 unidades de electricidad y gasta §1.400 al mes en el funcionamiento de sus centrales eléctricas.

Sin importaciones:

```math
u=\frac{1400}{14000+1}\approx0.09999
```

El coste medio de producción calculado por el juego es de aproximadamente **§0,10 por unidad**.

El juego usa este valor como punto de partida para fijar el precio de los contratos de electricidad.

Recalcula el coste unitario de la ciudad actual al actualizar los registros regionales, incluidas las actualizaciones mensuales y las solicitudes de propuestas de contratos nuevos.

#### Paso 2: el margen de beneficio del juego

El precio del contrato incluye un recargo sobre el coste medio del vendedor.

Con la configuración del juego original, el recargo va de aproximadamente un **10% a un 13%**.

El valor exacto depende del suministro disponible del comprador y del excedente que le quedará al vendedor después del contrato.

**Precio mensual del contrato (juego base)**

```math
P(q)=q\times\mathrm{clamp}\left(
u(1+m_{\min})+
\frac{g(V/q)+g(R/S_{\mathrm{sup}})}{2}
\,u(m_{\max}-m_{\min}),
\ u(1+m_{\min}),
\ u(1+m_{\max})
\right)
```

Con:

```math
\begin{aligned}
V &= 1+(B_P+B_I-q_{\mathrm{cur}})
-\max\left(B_D+B_E,\frac{M}{F}+1\right)\\
S_{\mathrm{sup}} &= \max(S_P+S_I,10^{-8})\\
R &= S_{\mathrm{sup}}-q-(S_D+S_E-q_{\mathrm{cur}})
\end{aligned}
```

Donde:

- **q** es la cantidad mensual de contrato propuesta.
- **u** es el coste unitario medio del vendedor.
- **mmin = 0,10** es el factor de beneficio mínimo del juego.
- **mmax = 0,50** es el factor de beneficio máximo del juego.
- **V** representa el suministro disponible del comprador por encima de lo que necesita.
- **R** representa el suministro que le queda al vendedor después del contrato y de los demás compromisos.
- **Ssup** representa la producción y las importaciones totales disponibles del vendedor.
- **qcur** es la cantidad actual del contrato que se evalúa, o cero si es un contrato nuevo.
- **g** es la curva de precios del juego.

Para una cantidad de contrato prácticamente igual a cero (por debajo de \(10^{-8}\)), el precio calculado es cero.

**La curva de precios que usa el juego**

| Entrada | Valor de la curva |
|---:|---:|
| 0 | 0,00001 |
| 0,0075 | 0,0075 |
| 1 | 0,075 |

El juego interpola entre estos puntos. Por debajo del primer punto, el valor se mantiene en 0,00001; por encima del último, se mantiene en 0,075.

Aunque los ajustes generales de recargo del juego permiten un intervalo mucho mayor, los valores pequeños que produce esta curva limitan el recargo efectivo de los contratos.

**Con el ajuste del juego original, un contrato tiene un precio de entre aproximadamente 1,10 y 1,13 veces el coste unitario medio del vendedor.**

El recargo tiende a ser:

- **Menor** cuando el comprador necesita más suministro o el contrato usa la mayor parte del excedente restante del vendedor.
- **Mayor** cuando el comprador ya tiene suministro de sobra y el vendedor conserva mucha capacidad.

En la práctica, la diferencia es relativamente pequeña.

#### Paso 3: la cantidad que se cobra realmente

Una vez que el juego calcula el precio mensual del contrato, lo convierte en un número entero de simoleones.

**Coste mensual almacenado del contrato (juego base)**

```math
\text{coste}=\max\left(1,\lfloor P(q)\rfloor\right)
```

El valor resultante se almacena en el contrato y aparece en el presupuesto de la ciudad.

**Por tanto, un contrato puede costar como mínimo §1 al mes, incluso cuando su cantidad es cero.**

Esta es una de las razones por las que el Ministerio elimina los contratos innecesarios de cantidad cero.

#### Ejemplo: fijar el precio de las ventas de electricidad

Volvamos al ejemplo de electricidad de la sección anterior.

El vendedor tiene un coste medio de electricidad de aproximadamente **§0,09999 por unidad**.

El comprador vecino no tiene suministro local suficiente, así que su contribución al precio es la mínima.

La fórmula de precios del juego produce los valores siguientes:

| Cantidad del contrato | Proporción de suministro restante del vendedor | Valor de la curva de precios | Precio relativo al coste unitario | Precio mensual del contrato |
|---:|---:|---:|---:|---:|
| 1.200 | 0,1786 | 0,01913 | 1,1038 × | §132 |
| 2.400 | 0,0929 | 0,01331 | 1,1027 × | §264 |
| 3.600 | 0,0071 | 0,00714 | 1,1014 × | §396 |
| 4.800 | Por debajo de cero | 0,00001 | 1,1000 × | §527 |

<text color="secondary">La fila de 4.800 unidades ilustra la fórmula de precios más allá del límite de contrato de 3.700 unidades calculado antes. No significa que el juego permitiera ese contrato en el estado original del ejemplo.</text>

Fíjate en que el precio por unidad cambia ligeramente al cambiar la cantidad del contrato.

**Sin embargo, este precio no nos dice si vender electricidad es rentable para la ciudad.**

Por ejemplo, ganar §396 al mes puede parecer interesante, pero ¿y si suministrar esas 3.600 unidades adicionales exige aumentar el gasto de las centrales eléctricas en §450?

El contrato daría pérdidas.

A la inversa, la ciudad podría tener capacidad de generación sin usar suficiente para suministrar el contrato con solo §100 de gasto adicional.

En ese caso, sería rentable.

**El Ministerio tiene en cuenta el coste operativo adicional real, no solo el precio medio de venta del juego.**

<a id="neighbor-deals-ladder"></a>
#### ¿Qué cantidades de contrato permite el juego?

SimCity 4 calcula una cantidad máxima posible de contrato, pero la ventana de Acuerdos con vecinos no te permite elegir todos los números entre cero y ese máximo.

En su lugar, ofrece una **lista de cantidades permitidas**.

Por ejemplo, si el contrato máximo es de 3.700 unidades, la ventana puede ofrecer solo:

**1.000, 2.000 o 3.000 unidades.**

No ofrece necesariamente 3.700.

Las cantidades disponibles dependen de:

- La cantidad máxima actual.
- La cantidad mínima de acuerdo del juego.
- Si el contrato es nuevo o ya está activo.
- El incremento almacenado en un contrato existente.
- Si el contrato está suspendido.

#### ¿Cómo se construye la lista de cantidades?

Para un contrato nuevo, el juego normalmente divide la cantidad máxima en unos diez pasos, respetando el tamaño mínimo de contrato.

**Incremento de un contrato nuevo (juego base)**

```text
maxQ = truncate(Qmax)
d10  = maxQ / 10       (integer division)
step = max(d10, M)
```

Donde **M** es la cantidad mínima de acuerdo, normalmente 1.000 unidades.

Las cantidades ofrecidas son múltiplos de ese paso, hasta el máximo permitido.

**Ejemplo 1: cantidad máxima de 15.442 unidades**

```text
step = max(1544, 1000)
     = 1544
```

La ventana ofrece:

1.544; 3.088; 4.632; 6.176; 7.720; 9.264; 10.808; 12.352; 13.896 y 15.440 unidades.

**Ejemplo 2: cantidad máxima de 3.700 unidades**

```text
step = max(370, 1000)
     = 1000
```

La ventana ofrece:

1.000; 2.000 y 3.000 unidades.

El máximo exacto de 3.700 no está disponible como cantidad seleccionable.

#### Los contratos existentes tienen reglas adicionales

Cuando ya existe un contrato, SimCity 4 intenta conservar su cantidad actual entre las opciones disponibles.

También usa el incremento almacenado en el contrato.

Si el intervalo disponible se hace mucho mayor, el juego puede construir una lista de dos niveles, con pasos más pequeños cerca de la cantidad actual y pasos mayores por encima.

**Ejemplo**

Considera un contrato activo con:

- Cantidad actual: 2.000 unidades.
- Incremento almacenado: 1.000 unidades.
- Nuevo máximo: 30.000 unidades.

La ventana ofrece las cantidades más pequeñas de **1.000 y 2.000**, seguidas de pasos mayores:

**3.000; 6.000; 9.000; 12.000; 15.000; 18.000; 21.000; 24.000; 27.000 y 30.000.**

Esto permite ajustar un contrato existente sin obligar al jugador a recorrer decenas de pasos pequeños.

**Regla completa de la lista de cantidades (juego base)**

```text
maxQ = trunc(Qmax)
       (if maxQ <= 0: use the current amount)

d10  = maxQ / 10
step = max(d10, M)

New proposal:
    Inc = step

Active contract:
    if (Inc == Amount and Inc > M) or Inc == 0:
        Inc = d10

    step     = Inc
    selected = Amount / Inc

Suspended contract:
    only the current amount

Q' = max(maxQ, step * selected, step)

If active, Amount > 0, and Q' / Inc > 15:
    Inc2 = Q' / 10

    entries =
        multiples of Inc up to Amount
        plus multiples of Inc2 above Amount, up to Q'
Otherwise:
    entries = multiples of step up to Q'
```

Todas las divisiones se truncan a enteros.

La selección de **cero o «sin acuerdo»** del juego cancela un contrato.

Un contrato suspendido no puede cambiarse de tamaño mediante la lista de cantidades normal.

**El Ministerio respeta las opciones de cantidad legales del juego.** No inventa cantidades de contrato arbitrarias solo porque otro número sería más rentable.

<a id="neighbor-deals-accounting"></a>
#### Estados de los contratos: activo, pendiente y suspendido

Un contrato regional puede encontrarse en varios estados distintos.

| Estado | Qué significa |
|---|---|
| **Pendiente** | Existe una propuesta, pero el contrato todavía no está activo. |
| **Activo** | El contrato está en vigor en este momento. |
| **Suspendido** | El juego ha interrumpido temporalmente el contrato por un problema. |
| **Cancelado** | El contrato ha terminado. |
| **No válido** | El contrato incumple una condición de elegibilidad, como una conexión que falta o una capacidad insuficiente. |
| **Suspensión terminada** | El contrato ha llegado al final de su periodo de suspensión. |

SimCity 4 comprueba los contratos durante su mantenimiento mensual.

Por ejemplo, el juego puede marcar un contrato cuando:

- La tesorería de la ciudad que paga ya no es superior al coste mensual del contrato.
- Ha desaparecido la conexión regional necesaria.
- La ciudad ya no puede cumplir el requisito de capacidad correspondiente.

**Un contrato que falla normalmente se suspende antes de que el juego lo termine definitivamente.**

La suspensión actúa como periodo de gracia.

Mientras está suspendido, el contrato no puede cambiarse de tamaño ni cancelarse mediante las operaciones normales de contratos. El juego gestiona su resolución posterior.

#### ¿Cómo se cuentan los contratos en el suministro regional?

Las cifras regionales y el presupuesto financiero no siempre tratan los contratos de la misma manera.

**Cantidades regionales de recursos**

Normalmente el juego cuenta la cantidad completa de un contrato en el total Importado del comprador y en el total Exportado del vendedor, incluidas las propuestas pendientes y los contratos suspendidos.

Los contratos cancelados y no válidos quedan excluidos.

**Ingresos y gastos mensuales de los contratos**

Los cálculos financieros del juego incluyen los contratos activos y suspendidos, pero excluyen los pendientes, cancelados y no válidos.

| Estado del contrato | Se cuenta en las cantidades regionales | Se cuenta en los ingresos o gastos mensuales de contratos |
|---|---|---|
| Pendiente | Sí | No |
| Activo | Sí | Sí |
| Suspendido | Sí | Sí |
| Cancelado | No | No |
| No válido | No | No |

**¿Por qué es importante?**

Una propuesta pendiente puede afectar a las cifras de suministro regional del juego antes de generar ningún pago.

Del mismo modo, un contrato suspendido puede seguir figurando en la contabilidad regional aunque su servicio no sea fiable.

**El Ministerio distingue entre que un contrato esté registrado y que su suministro sea realmente utilizable.**

Así evita depender de compras de electricidad o agua suspendidas solo porque sus cantidades sigan apareciendo en los totales de la región.

<a id="neighbor-deals-delivery"></a>
#### ¿Cuándo se entregan los recursos contratados?

La electricidad, el agua y la basura no siguen las mismas reglas de entrega.

**Electricidad y agua**

Estos contratos se tratan como compromisos de suministro mensuales.

La cantidad contratada cuenta como suministro para el comprador y como obligación para el vendedor mientras el contrato siga en vigor, con independencia de cuánto se consuma realmente.

**Basura**

Los contratos de basura funcionan mediante entregas diarias.

**Entrega diaria de un contrato de basura (juego base)**

```math
\text{paso diario}=
\max\left(1,\mathrm{round}(0.1\times A)\right)
```

La entrega diaria continúa mientras:

```math
A-U>0
```

Donde:

- **A** es la cantidad mensual contratada.
- **U** es la cantidad ya entregada durante el mes.

El juego reinicia el uso mensual en el cambio de mes.

Por consiguiente, **la mayoría de los contratos de basura entregan toda su cantidad mensual durante los diez primeros días simulados, más o menos**.

**Ejemplo: un contrato de basura de 25.004 unidades**

La cantidad diaria es:

```math
\mathrm{round}(25004\times0.1)=2500
```

Después de diez días, el juego ha entregado:

```math
10\times2500=25000
```

Solo quedan cuatro unidades, pero el juego no reduce la última entrega a cuatro.

En su lugar, hace otra entrega de 2.500 unidades el undécimo día.

**El total pasa a ser de 27.500 unidades**, que supera la cantidad mensual nominal del contrato.

Esto es especialmente importante cuando tu ciudad recibe basura de una vecina.

**Los cálculos de limpieza del Ministerio no deben subestimar la basura importada solo porque la cantidad contractual sea ligeramente inferior a lo que el juego entrega realmente.**

#### ¿Cuánta basura puede recibir tu ciudad?

SimCity 4 también limita la basura total que una ciudad puede aceptar de sus vecinas.

Este límite depende sobre todo de:

- El almacenamiento disponible en los vertederos.
- La producción de basura de la propia ciudad.
- La basura ya importada.
- El ajuste de duración de contratos del juego.

**Basura recibida máxima (juego base)**

```math
\sum A_{\mathrm{received}}
\le
L_{\mathrm{free}}
\left(1-\frac{1}{60+0.000107}\right)
+G_P+G_I
```

Donde:

- **Lfree** es el almacenamiento disponible en los vertederos de la ciudad.
- **GP** es la producción de basura registrada de la ciudad.
- **GI** son sus importaciones de basura registradas.
- **60** representa el plazo de contrato de cinco años del juego, medido en meses.

**Ejemplo**

Supongamos que una ciudad tiene:

- Almacenamiento libre en vertederos: 500.000 unidades.
- Basura producida: 40.000 unidades.
- Basura importada: 10.000 unidades.

El máximo de basura recibida que permite el juego es aproximadamente:

```math
500000
\left(1-\frac{1}{60+0.000107}\right)
+40000+10000
\approx541667
```

**Máximo de basura recibida permitido: aproximadamente 541.667 unidades.**

Si el total recibido mediante contratos supera este límite, el juego marca los contratos excedentes como de capacidad insuficiente.

Esos contratos se suspenden entonces, siguiendo el orden de la lista de contratos del juego.

**El Ministerio debe respetar este límite incluso cuando aceptar más basura parezca rentable por otros motivos.**

<a id="neighbor-deals-problem"></a>
### 2. El problema de la gestión

Gestionar a mano los contratos regionales es difícil porque varias decisiones se afectan entre sí.

#### Un contrato que parece rentable puede dar pérdidas

SimCity 4 basa los precios de venta en el **coste medio**.

Pero el valor económico de un contrato depende del **coste marginal**: lo que la ciudad tiene que gastar de más, o lo que puede ahorrar, por ese contrato concreto.

Por ejemplo, vender electricidad por §400 al mes no compensa si exige §500 adicionales de gasto en centrales eléctricas.

El juego no muestra directamente esta comparación.

#### Las ofertas de contratos cambian constantemente

Un contrato que hoy está disponible puede desaparecer cuando:

- Cambia la demanda de tu ciudad.
- Cambia el suministro registrado de una ciudad vecina.
- Una central pierde capacidad de producción.
- Una ciudad ya no tiene dinero suficiente.
- Otro contrato usa el excedente disponible.

Las cantidades legales que se pueden elegir también cambian con la capacidad disponible.

#### Varias vecinas compiten por los mismos recursos

Imagina que tu ciudad puede vender 5.000 unidades de electricidad.

Una vecina ofrece comprar 3.000 unidades, mientras que otra ofrece comprar 4.000.

**Tu ciudad no puede aceptar necesariamente los dos contratos**, porque el total superaría su suministro disponible.

El Ministerio debe comparar las combinaciones posibles en lugar de elegir cada contrato por separado.

#### Las compras afectan a las necesidades de producción local

Comprar electricidad o agua puede reducir lo que tus propias instalaciones necesitan producir.

Con las reglas de reserva del Ministerio, esto puede generar un ahorro mayor de lo que sugeriría la cantidad comprada por sí sola.

#### El cálculo de excedente del juego no protege tu reserva

Los propios límites de contratos de SimCity 4 no incluyen la reserva de seguridad adicional del Ministerio.

El juego puede permitir una venta que deje demasiado poca capacidad sobrante de electricidad o de agua para las necesidades de tu ciudad.

**El Ministerio debe aplicar sus propios requisitos de reserva y de servicio además de los límites legales del juego.**

#### Los contratos de basura dependen del calendario diario

Dos combinaciones de contratos de basura pueden tener la misma cantidad mensual total pero calendarios de entrega distintos.

Eso puede cambiar si el sistema de limpieza despeja o no la basura del mes.

**Por eso el Ministerio evalúa las combinaciones completas de contratos de basura con la simulación diaria de la limpieza, y no solo con los totales mensuales.**

<a id="neighbor-deals-algorithm"></a>
### 3. Cómo gestiona el Ministerio los contratos regionales

El Ministerio combina la selección de contratos con los optimizadores de recursos locales.

En lugar de considerar cada ciudad vecina de forma independiente, busca **la combinación de contratos que ofrece el mayor beneficio económico global y protege al mismo tiempo los servicios de la propia ciudad**.

Realiza una optimización distinta para la electricidad, el agua y la basura.

#### ¿Firma el Ministerio contratos automáticamente?

**Sí.**

Cuando la gestión automática de acuerdos con vecinos está activada, el Ministerio puede:

- Crear contratos nuevos.
- Aumentar o reducir las cantidades de los contratos existentes.
- Cancelar los contratos que ya no son útiles.

Realiza estas operaciones directamente con las funciones de contratos normales del juego.

Para un contrato nuevo, pide a SimCity 4 las propuestas disponibles, activa la propuesta elegida y fija su cantidad.

Para un contrato existente, usa las operaciones normales del juego para cambiar la cantidad o cancelar el contrato.

**No necesitas abrir la ventana de Acuerdos con vecinos ni aceptar los contratos a mano.**

El Ministerio no modifica las ciudades vecinas, no reescribe los archivos de guardado, no cambia las fechas de los contratos ni obliga al juego a aceptar contratos que normalmente no ofrecería.

**¿Y los contratos que hayas creado tú mismo?**

Cuando el Ministerio gestiona un recurso, tus contratos creados a mano se incluyen en su optimización.

Puede cambiarles el tamaño o cancelarlos si otra combinación legal es mejor.

Si quieres conservar el control manual total de los contratos de un recurso concreto, desactiva la opción de gestión automática de acuerdos de ese recurso.

#### ¿Con qué frecuencia se revisan los contratos?

Los contratos se revisan normalmente junto con su recurso correspondiente.

| Recurso | Revisión periódica |
|---|---|
| **Electricidad** | Cada 10 días simulados, con revisiones anteriores si hay cambios significativos o carencias de suministro confirmadas. |
| **Agua** | Cada 10 días simulados desde la última revisión del agua, en cada cambio de mes, y antes cuando es necesario. |
| **Basura** | En cada cambio de mes y cuando cambian de forma significativa los valores de limpieza que se vigilan. |

Una comprobación diaria ligera también busca carencias reales y cambios significativos en la demanda, la producción o los contratos regionales.

Cada evaluación parte del estado actual de la ciudad.

**Normalmente, el Ministerio no impone un periodo de espera adicional ni un tiempo de enfriamiento solo porque haya cambiado un contrato hace poco.**

Existen protecciones específicas contra los cambios de contrato innecesarios, que se explican más abajo y en la sección sobre la reevaluación de contratos.

#### Paso 1: Encontrar todas las opciones de contrato disponibles

El Ministerio empieza examinando las ciudades vecinas con conexiones adecuadas y registros regionales completos.

Para cada recurso y dirección de intercambio, identifica las cantidades de contrato legales.

**Contratos existentes**

Se incluye la cantidad actual, junto con las demás cantidades que permite el juego.

**Compras nuevas de electricidad y agua**

El Ministerio usa las cantidades que la lista de propuestas del juego permite en ese momento.

Excluye las compras cuyo precio mensual no se mantendría por debajo de la tesorería disponible de la ciudad local.

**Ventas nuevas de electricidad y agua**

La ciudad vecina ya tiene que aparecer en las ofertas disponibles del juego.

Sin embargo, algunas cantidades de venta mayores solo pueden ser posibles **después de que el Ministerio aumente la capacidad de producción local**.

El Ministerio puede considerarlas *candidatos de venta latente*.

Por ejemplo, imagina que tu ciudad tiene ahora solo 2.000 unidades de excedente de electricidad, pero podría producir otras 3.000 unidades aumentando la financiación de las centrales.

Un contrato mayor podría ser posible cuando esa capacidad esté realmente disponible.

El Ministerio puede evaluar si el coste adicional de producción estaría justificado.

**Un contrato latente no se firma hasta que las reglas reales de elegibilidad del juego lo permitan.**

**Basura recibida de los vecinos**

El Ministerio considera las cantidades que permite la lista de propuestas actual del juego.

Además, debe comprobar que la ciudad puede manejar los residuos adicionales.

#### ¿Por qué usa el Ministerio una base de precios común?

Al evaluar las alternativas de contrato, el Ministerio empieza quitando sus propios contratos ajustables de los cálculos de precios.

Después evalúa las cantidades candidatas frente a esta base común.

Así evita una situación indeseable: que cambiar un contrato altere los registros regionales, que eso cambie el precio calculado y que el Ministerio vuelva a preferir de inmediato el arreglo anterior.

**La misma cantidad candidata no debería parecer más o menos atractiva solo porque el Ministerio acabe de escribir un contrato equivalente.**

Al evaluar carteras completas, los precios se calculan con la combinación de contratos seleccionada.

Los candidatos sin información fiable suficiente para calcular su precio se excluyen. Los valores que faltan nunca se tratan en silencio como cero.

<a id="neighbor-deals-objective"></a>
#### Paso 2: Encontrar la combinación más rentable

El Ministerio no se limita a maximizar los ingresos de los contratos.

Considera **conjuntamente los ingresos, los gastos de los contratos y el coste de abastecer los servicios de la propia ciudad**.

**Objetivo de la cartera de contratos regionales**

```math
\max_x\ \mathrm{Net}(x)=
\sum_{i\in\mathrm{sell\text{-}like}}P_i(x_i\mid x)
-
\sum_{j\in\mathrm{buy\text{-}like}}P_j(x_j\mid x)
-
C_{\mathrm{local}}(x)
```

Donde:

- **x** representa las cantidades elegidas para todos los contratos candidatos de un recurso.
- **Pi** son los ingresos de un contrato de tipo venta.
- **Pj** es el gasto de un contrato de tipo compra.
- **Clocal(x)** es el menor coste de explotación local que puede respaldar con seguridad los contratos elegidos.
- **Net(x)** es el valor económico mensual resultante.

El precio de cada contrato se calcula en el contexto de toda la cartera seleccionada.

**¿Qué se incluye en el coste local?**

Depende del recurso:

| Recurso | Coste local considerado |
|---|---|
| **Electricidad** | La financiación de las centrales eléctricas necesaria para cubrir las necesidades locales, la reserva y las exportaciones, una vez descontadas las importaciones. |
| **Agua** | La financiación del servicio de aguas necesaria para cubrir las necesidades locales, la reserva y las exportaciones, una vez descontadas las importaciones. |
| **Basura** | La financiación de la limpieza necesaria para manejar la basura de la ciudad, las importaciones y exportaciones elegidas y la reserva configurada. |

El Ministerio usa los mismos cálculos descritos en los capítulos anteriores.

Cuando **Tener en cuenta el desgaste de las centrales** está activado, el coste económico local de la electricidad incluye también el desgaste estimado de las centrales.

Ese desgaste se usa para comparar opciones, pero **no es un gasto adicional de la tesorería**.

#### Ejemplo: comparar carteras de contratos

Considera una ciudad ilustrativa con tres arreglos posibles:

| Arreglo | Ingresos de contratos | Gastos de contratos | Coste de explotación local | Neto |
|---|---:|---:|---:|---:|
| Solo producción local | §0 | §0 | §1.000 | −§1.000 |
| Comprar a un vecino | §0 | §300 | §600 | −§900 |
| Vender a otro vecino | §400 | §0 | §1.200 | −§800 |

En este ejemplo simplificado, vender ofrece el mejor resultado económico, aunque los costes de explotación locales aumenten.

El Ministerio preferiría ese arreglo **siempre que se cumplan todas las condiciones legales y de seguridad**.

Las cifras son ilustrativas; el planificador real usa los precios de contrato del juego y los cálculos de financiación propios de cada recurso.

**¿Cómo se tratan los resultados iguales?**

Si dos carteras legales tienen el mismo valor económico, el Ministerio prefiere:

1. La cartera con **menos contratos**.
2. Si sigue habiendo empate, la cartera que requiera **menos cambios**.

Así se evitan contratos y ajustes innecesarios que no mejoran el resultado.

#### Las reglas de seguridad que toda cartera debe cumplir

El Ministerio solo considera las combinaciones de contratos que respetan las restricciones siguientes.

**1. Los servicios de tu ciudad van primero**

El plan de producción local o de limpieza debe seguir siendo viable con los contratos elegidos.

Se rechaza una cartera que los servicios propios de la ciudad no puedan respaldar con seguridad.

**2. Las compras no pueden superar la demanda propia de tu ciudad**

El Ministerio no compra electricidad ni agua solo para revenderlas.

Tampoco compra electricidad o agua extra únicamente para crear su reserva configurada.

**3. Normalmente, un recurso se compra o se vende**

Por defecto, el Ministerio elige una de tres posiciones para cada recurso:

- **LOCAL:** Sin compras ni ventas.
- **COMPRAR:** Comprar el recurso.
- **VENDER:** Vender el recurso.

En la configuración por defecto no se permite comprar y vender el mismo recurso a la vez.

El ajuste opcional **Comprar a una ciudad para vender a otra** se explica en los ajustes de contratos.

**4. Las importaciones de basura deben mantenerse dentro de límites seguros**

La basura total recibida no puede superar el límite de basura entrante del juego ni la cantidad adicional que el sistema de limpieza de la ciudad pueda manejar.

**5. Los contratos suspendidos son fijos**

Un contrato suspendido no se puede cambiar con las funciones normales del juego.

El Ministerio incluye su cantidad actual en los cálculos, pero no intenta cambiarle el tamaño ni cancelarlo.

**6. Un déficit confirmado de electricidad o agua impide las ventas**

Si los contadores del juego confirman una carencia regional real, el Ministerio no mantiene las ventas de ese recurso.

**El suministro esencial de su propia ciudad tiene prioridad sobre los ingresos por exportación.**

#### ¿Cómo se incluyen las reservas de electricidad y agua?

El Ministerio calcula cuánta capacidad local sigue siendo necesaria después de las importaciones y las exportaciones.

**Capacidad local necesaria**

```math
N(B,S)=
\max\left(
\left\lceil R\times\max(0,D-B)\right\rceil,
R_q-B
\right)+S
```

Donde:

- **D** es la demanda propia de la ciudad.
- **B** es la cantidad total comprada.
- **S** es la cantidad total vendida.
- **R** es el multiplicador de reserva.
- **Rq** es el requisito mensual de electricidad propio del juego.

Para la electricidad, la reserva por defecto es del 20%, lo que da **R = 1,20**.

Para el agua, la reserva por defecto es del 10%, lo que da **R = 1,10**.

**Para el agua se omite el término Rq**, ya que el cálculo del agua no usa el contador de requisito de electricidad, que es independiente.

**Ejemplo: electricidad con una reserva del 20%**

Supón que la ciudad tiene:

- Demanda: 10.000 unidades.
- Requisito del juego: 10.300 unidades.
- Reserva: 20%.

Sin contratos:

```math
N(0,0)=12000
```

Si compra 2.000 unidades:

```math
N(2000,0)=
\max(9600,8300)=9600
```

Si en cambio vende 2.400 unidades:

```math
N(0,2400)=14400
```

| Situación | Capacidad de generación local necesaria |
|---|---:|
| Sin contratos | 12.000 |
| Comprar 2.000 unidades | 9.600 |
| Vender 2.400 unidades | 14.400 |

**Comprar 2.000 unidades reduce el requisito de capacidad local en 2.400 unidades** en este ejemplo, porque la reserva se aplica solo a lo que la ciudad tiene que producir por sí misma.

Vender 2.400 unidades aumenta el requisito exactamente en 2.400.

Por eso el Ministerio compara los precios de los contratos con el cambio real en los costes locales que causan estos requisitos de capacidad.

#### ¿Por qué no puede el Ministerio comprar más de lo que necesita la ciudad?

El Ministerio aplica un límite de compra adicional.

**Límite de compra**

```math
\sum_j x_j\le
\min\left(
D,\;
F\times\max\left(L_D+L_E,\frac{M}{F}+1\right)
-(L_P+L_I)
\right)
```

Donde:

- **D** es la demanda propia de la ciudad.
- **F** es el factor de sobredemanda del comprador del juego.
- **M** es la cantidad mínima de contrato.
- Los valores **L** son los registros regionales de la ciudad local, sin contar sus propias compras ajustables.

Para la basura, el límite de demanda se basa en **la producción de basura de la propia ciudad menos el reciclaje**.

Esto impide que el Ministerio:

- Compre más de lo que necesita.
- Compre recursos únicamente para revenderlos.
- Compre recursos adicionales para crear una reserva sobre las importaciones.
- Use el suministro comprado como base de nuevos compromisos de exportación.

También se respetan los límites legales de compra del propio juego.

#### ¿Cómo evalúa el Ministerio un contrato?

El cálculo de la cartera también se puede entender mirando un solo contrato.

**Para una venta**

El Ministerio compara el pago recibido con el coste de explotación local adicional.

```math
\mathrm{Profit}(q)=
P(q)-
\left[
C_{\mathrm{local}}(N+q)
-
C_{\mathrm{local}}(N)
\right]
```

Por ejemplo, si una venta aporta §300 pero requiere §200 más en gastos de producción:

**Beneficio neto: §100 al mes.**

La venta es rentable.

**Para una compra**

El Ministerio compara el precio del contrato con el gasto local que evita.

```math
\mathrm{Saving}(q)=
\left[
C_{\mathrm{local}}(N(0))
-
C_{\mathrm{local}}(N(q))
\right]
-
P(q)
```

Por ejemplo, si comprar electricidad cuesta §250 pero permite a tu ciudad ahorrar §400 en financiación de las centrales eléctricas:

**Ahorro neto: §150 al mes.**

La compra es rentable.

**Esto es rentabilidad marginal:** el Ministerio evalúa el efecto económico real de añadir o cambiar un contrato.

No da por hecho que una venta merezca la pena solo porque el juego aplique un recargo al coste medio del vendedor.

#### Paso 3: Buscar la mejor combinación de contratos

Una vez conocidas las cantidades disponibles y los requisitos de seguridad, el Ministerio compara las carteras legales.

La búsqueda funciona de forma distinta para la electricidad, el agua y la basura.

**Electricidad y agua**

El Ministerio usa un algoritmo de optimización para comparar combinaciones de compras y ventas totales.

Para que el cálculo sea eficiente, las cantidades se agrupan inicialmente en **bloques de 100 unidades**.

Para rangos de intercambio muy grandes, el tamaño del bloque aumenta para limitar el cálculo.

Tras elegir al mejor candidato, el Ministerio vuelve a comprobar la combinación completa con **cantidades exactas**, sin la agrupación aproximada.

**Basura**

La basura requiere un método distinto.

Como cada contrato tiene su propio calendario de entregas diarias, el Ministerio no puede decidir con seguridad usando solo totales mensuales.

Por eso evalúa combinaciones completas de contratos y ejecuta el cálculo de la financiación de la limpieza para cada una.

**Toda cartera candidata debe someterse a prueba con el proceso previsto de eliminación de la basura.**

Esta búsqueda puede resultar costosa cuando hay muchos vecinos y cantidades disponibles.

Si el número de combinaciones supera **5 millones en uno de los lados de la búsqueda**, el Ministerio considera que el óptimo no está demostrado.

**No hace un cambio de contrato especulativo cuando no puede establecer un resultado fiable.**

#### Paso 4: Eliminar contratos y cambios innecesarios

Tras encontrar la cartera preferida, el Ministerio realiza varias comprobaciones finales.

**1. Eliminar los contratos que no aportan ningún beneficio**

Si quitar un contrato no empeora económicamente la cartera, el Ministerio lo descarta.

Todo contrato opcional debe aportar un beneficio estrictamente positivo.

**2. No sustituir nunca un arreglo existente mejor**

Si conservar todos los contratos actuales es legal, viable y estrictamente más valioso que el resultado propuesto, el Ministerio deja los contratos sin cambios.

**3. Ignorar los cambios opcionales insignificantes**

Un ajuste avanzado del INI puede especificar un beneficio mínimo exigido para los cambios de contrato opcionales.

El umbral por defecto es **§0**, de modo que cualquier beneficio estrictamente positivo puede cumplirlo.

Cuando se configura un umbral más alto, el Ministerio evita las creaciones de contratos o los cambios de cantidad opcionales cuyo beneficio sea inferior a ese importe.

**Los cambios necesarios para mantener los servicios esenciales están exentos de este umbral económico.**

**4. Eliminar los contratos inactivos de cantidad cero**

A veces SimCity 4 puede dejar un contrato activo con una cantidad de cero.

Ese contrato no aporta ningún recurso y sigue costando §1 al mes.

El Ministerio cancela estos contratos innecesarios.

#### ¿Cómo se mide el beneficio de la decisión sobre un contrato?

Cuando varios contratos interactúan, mirar solo los ingresos o el gasto de uno puede dar una respuesta equivocada.

En su lugar, el Ministerio se pregunta:

**¿Cuánto peor sería el mejor plan legal si este contrato en concreto tuviera que mantenerse en su cantidad actual?**

**Beneficio marginal de la decisión sobre un contrato**

```math
\Delta_k=
\mathrm{Net}(x^*)
-
\max_{x:\,x_k=x_k^{\mathrm{now}}}
\mathrm{Net}(x)
```

Donde:

- **x\*** es la cartera óptima seleccionada.
- **xk** es la cantidad del contrato que se evalúa.
- **xk ahora** es su cantidad actual.
- **Net** es el valor económico total de la cartera.
- **Δk** es el beneficio económico atribuible a permitir que ese contrato en concreto cambie.

El Ministerio compara la cartera seleccionada con la **mejor alternativa que mantiene sin cambios este contrato**, y no solo con el estado global actual de la ciudad.

Esto importa porque otro vecino podría ofrecer un contrato igual de bueno.

Por ejemplo, supón que dos vecinos pueden suministrar electricidad por el mismo coste económico total.

Crear un contrato con cualquiera de los dos puede ser rentable en comparación con producirlo todo localmente.

Pero si el otro vecino puede aportar el mismo beneficio, **ninguno de los dos contratos es imprescindible**.

El Ministerio lo tiene en cuenta al decidir si un cambio individual está justificado.

**Normalmente, la creación o el aumento de un contrato solo se escribe cuando su beneficio marginal es estrictamente positivo.**

Hay una excepción: un cambio necesario para mantener el servicio cuando ninguna cartera legal puede mantener ese contrato en su cantidad actual.

En ese caso, el Ministerio no se inventa un beneficio monetario. Reconoce que el cambio es necesario para cumplir los requisitos del servicio.

#### Ahorro en efectivo frente a ahorro económico

Para la electricidad, el Ministerio mantiene dos versiones del beneficio de la decisión sobre un contrato:

| Medida | Qué incluye |
|---|---|
| **Beneficio en efectivo** | Los cambios en los gastos de explotación reales y en los ingresos o pagos de los contratos. |
| **Beneficio económico** | El beneficio en efectivo más el valor estimado del desgaste de las centrales, cuando el desgaste está activado. |

**La decisión usa el beneficio económico.**

El beneficio en efectivo describe el efecto en el presupuesto real de la ciudad.

El desgaste estimado de las centrales nunca se carga directamente a la tesorería.

Esta distinción permite al Ministerio tener en cuenta el deterioro de las centrales a largo plazo sin presentar como dinero realmente gastado un gasto mensual imaginario.

**El objetivo global sigue siendo el mismo: elegir la combinación de contratos legales más beneficiosa económicamente sin comprometer los servicios esenciales de tu ciudad, sus reservas ni las protecciones de seguridad existentes.**
<a id="neighbor-deals-exclusive"></a>
#### Comprar o vender: una sola dirección por defecto

Por defecto, el Ministerio no compra y vende el mismo recurso al mismo tiempo.

Para cada recurso, elige uno de tres arreglos posibles:

| Arreglo | Qué hace el Ministerio |
|---|---|
| **LOCAL** | Usa la producción o la eliminación locales sin comprar ni vender ese recurso. |
| **COMPRAR** | Compra el recurso a una o más ciudades vecinas. |
| **VENDER** | Vende el recurso o el servicio de eliminación a una o más ciudades vecinas. |

En la electricidad y el agua, comprar y vender tienen su significado habitual.

En la basura, las direcciones se invierten:

- **COMPRAR:** Tu ciudad paga a un vecino para que elimine su basura.
- **VENDER:** Tu ciudad recibe basura de un vecino y cobra por tratarla.

Este comportamiento se controla con **Comprar a una ciudad para vender a otra**, que está **desactivado por defecto**.

#### ¿Y si la ciudad ya tiene contratos en ambas direcciones?

El Ministerio compara los arreglos disponibles y avanza hacia el que ofrece el mayor beneficio económico global.

Por ejemplo, si tu ciudad compra y vende electricidad a la vez, el Ministerio normalmente elige si conservar las compras o las ventas y después cancela los contratos innecesarios.

**Hay una excepción importante: los contratos suspendidos.**

SimCity 4 no permite cambiar los contratos suspendidos con sus funciones de contratos normales.

Supón que tu ciudad tiene una venta de electricidad suspendida pero ahora necesita comprar electricidad para evitar una carencia.

El Ministerio no puede eliminar de inmediato la venta suspendida.

En esa situación, **el suministro de electricidad de la ciudad tiene prioridad sobre la restricción habitual de comprar o vender**.

El Ministerio puede comprar la electricidad necesaria para mantener el servicio, evita crear ventas adicionales y registra el conflicto de política en el registro.

No permite que la regla de exclusividad deje a la ciudad sin un recurso esencial.

<a id="neighbor-deals-arbitrage"></a>
#### Opcional: comprar y vender al mismo tiempo

Puedes activar **Comprar a una ciudad para vender a otra** para que el Ministerio pueda considerar contratos en ambas direcciones para el mismo recurso.

Esto requiere que **Gestionar acuerdos con vecinos** esté activado.

El Ministerio solo elige un arreglo mixto cuando produce un resultado económico mejor que todos los arreglos de una sola dirección disponibles.

**¿Cómo puede ser útil?**

Considera una ciudad con dos vecinos.

El vecino A vende electricidad relativamente barata, mientras que el vecino B está dispuesto a pagar más por ella.

Tu ciudad podría beneficiarse de comprar electricidad a A y de vender electricidad a B.

Sin embargo, **el Ministerio no se limita a comprar electricidad y revender esas mismas unidades importadas**.

En cambio, la compra reduce la cantidad de electricidad que tu ciudad debe producir para sus propios edificios.

Eso puede liberar capacidad de generación local para una venta rentable.

**Ejemplo: electricidad**

Imagina el arreglo siguiente:

- Tu ciudad necesita 10.000 unidades de electricidad.
- Compra 3.000 unidades al vecino A.
- Sus propias centrales cubren el resto de la demanda local y la reserva.
- La capacidad de generación local disponible respalda una venta al vecino B.

El Ministerio evalúa si el coste de la compra, los ingresos de la venta y los gastos resultantes de las centrales eléctricas producen un mejor resultado global.

La reserva de electricidad normal y todas las demás reglas de seguridad siguen aplicándose.

**¿Y la basura?**

La basura también puede beneficiarse de un arreglo mixto.

Por ejemplo:

- El vecino A paga a tu ciudad para que acepte su basura.
- Tu ciudad paga al vecino B para que elimine parte de su propia basura.

Si los ingresos por recibir basura superan los costes adicionales de limpieza y de exportación, la combinación puede merecer la pena.

El Ministerio evalúa el calendario completo de eliminación para asegurarse de que la ciudad puede manejar la basura resultante.

#### ¿Qué restricciones siguen aplicándose?

Activar esta opción **no** elimina las protecciones normales.

- Las compras no pueden superar la demanda propia de tu ciudad.
- La electricidad o el agua importadas nunca se usan como base para una reventa.
- Las reservas de electricidad y agua configuradas siguen siendo obligatorias.
- Las importaciones de basura deben mantenerse dentro de los límites de eliminación permitidos.
- Todo contrato debe respetar las reglas de elegibilidad, cantidad y tesorería del juego.
- Los servicios locales deben seguir siendo viables.
- El Ministerio nunca comercia con un recurso cuya gestión automática de acuerdos esté desactivada.

**Una ciudad vecina no puede ser a la vez proveedora y clienta tuya para el mismo recurso.**

Por ejemplo, el vecino A puede vender electricidad a tu ciudad o comprársela, pero no las dos cosas en el mismo plan.

Distintos vecinos pueden asumir distintos papeles.

#### ¿Cómo elige el Ministerio el papel de cada vecino?

Cuando una ciudad vecina podría intercambiar potencialmente en cualquiera de las dos direcciones, el Ministerio evalúa las asignaciones posibles.

Con hasta **cuatro vecinos así**, puede evaluar todas las asignaciones de forma exacta.

Cuatro vecinos crean como máximo:

```math
2^4=16
```

asignaciones posibles de compra o venta.

Después, cada asignación se evalúa con las reglas normales de optimización de contratos.

Para evitar un cálculo excesivo, el Ministerio recurre al intercambio en una sola dirección cuando:

- Más de **cuatro vecinos** requieren una asignación de compra o venta.
- La búsqueda mixta de basura superaría las **4.000 combinaciones de compra por venta**.

En esas situaciones, el Ministerio usa la optimización ordinaria de un solo lado para esa revisión.

**La opción amplía las opciones económicas a disposición del Ministerio sin permitir un intercambio ilimitado ni saltarse las reglas de seguridad.**

<a id="neighbor-deals-execution"></a>
#### Paso 4: Aplicar los cambios de contratos y de financiación con seguridad

Encontrar la mejor combinación de contratos es solo una parte del trabajo.

El Ministerio también debe aplicar los cambios en un orden que no interrumpa los servicios de tu ciudad.

Por ejemplo, imagina que quiere comprar electricidad a un vecino y reducir la financiación de las centrales eléctricas.

**Reducir la producción antes de que la compra esté disponible podría causar un apagón.**

Del mismo modo, vender agua adicional antes de aumentar la producción local podría dejar a tus propios edificios sin suministro suficiente.

El Ministerio coordina la financiación local y los contratos regionales mediante seis fases de ejecución.

#### Las seis fases de ejecución

| Fase | Acción | Por qué va en este orden |
|---|---|---|
| **1** | Aumentar la capacidad de producción local | Dejar disponible el suministro local adicional antes de que haga falta. |
| **2** | Reducir o cancelar ventas | Liberar la capacidad que está comprometida con los vecinos. |
| **3** | Crear o aumentar compras | Establecer el suministro de sustitución antes de reducir la producción local. |
| **4** | Crear o aumentar ventas | Empezar nuevas exportaciones solo cuando el suministro necesario esté disponible. |
| **5** | Reducir la capacidad de producción local | Bajar la financiación cuando el nuevo arreglo ya esté respaldado. |
| **6** | Reducir o cancelar compras | Retirar el suministro importado solo cuando el arreglo restante pueda sostener la ciudad. |

**El principio rector es que la transición debe ser segura, no solo el arreglo final.**

En la electricidad y el agua, el requisito de suministro local depende de la demanda propia de la ciudad, de las ventas y de las compras.

De forma simplificada:

```math
\text{Necesidad local}=\text{Necesidad propia}+\text{Ventas}-\text{Compras}
```

El cálculo completo incluye también la reserva configurada y, en la electricidad, el contador de requisito independiente del juego.

El orden de ejecución garantiza que los cambios intermedios no requieran más capacidad local que el arreglo inicial o el final.

#### Ejemplo: sustituir la electricidad local por una compra

Supón que las centrales eléctricas de tu ciudad lo suministran todo ahora mismo a nivel local.

El Ministerio descubre que comprar electricidad a un vecino reduciría los gastos totales.

Procede en este orden:

1. Establece la nueva compra de electricidad.
2. Verifica que el juego ha aceptado el contrato.
3. Vuelve a calcular el requisito de capacidad local.
4. Reduce la financiación de las centrales eléctricas cuando es seguro hacerlo.

**No recorta tus centrales eléctricas antes de confirmar el suministro alternativo.**

#### Ejemplo: sustituir una compra de electricidad por producción local

Ahora supón que ocurre lo contrario y resulta más económico.

Tu ciudad quiere dejar de comprar electricidad y producirla localmente.

El Ministerio:

1. Aumenta la financiación necesaria de las centrales eléctricas.
2. Verifica que hay capacidad de generación local disponible.
3. Reduce o cancela la compra de electricidad.

**No cancela el suministro importado antes de que exista la capacidad local que lo sustituye.**

#### ¿Hay excepciones al orden de las fases?

Sí. Dos cambios pueden adelantarse a la fase 2 cuando el Ministerio ha confirmado que no pueden comprometer el servicio local:

- **Reducir una compra** cuando la capacidad local confirmada de la ciudad ya es suficiente sin ella.
- **Crear o aumentar una venta** que el juego ofrece en ese momento, cuando la capacidad local confirmada existente ya es suficiente para respaldarla.

En estos casos, la capacidad ya está disponible. No hace falta esperar a un aumento de financiación innecesario.

#### ¿Aplica el Ministerio las seis fases a la vez?

No.

**Cada revisión ejecuta solo la fase pendiente más temprana de cada recurso, con un cambio de contrato cada vez.**

Después, el Ministerio lee el estado actualizado de la ciudad antes de continuar en una revisión posterior.

Así se evita aplicar juntos varios cambios sin verificar.

#### ¿Qué pasa con las ventas que requieren antes más producción?

Como se ha explicado antes, el Ministerio puede identificar una venta de electricidad o agua potencialmente rentable que todavía no está disponible en la cantidad deseada.

Esto se llama *venta latente*.

Por ejemplo, una venta podría pasar a ser legal tras aumentar la financiación de las centrales eléctricas.

El Ministerio primero debe aumentar la capacidad de generación.

Solo cuando el juego reconoce esa capacidad y permite el contrato previsto se puede crear la venta.

**Una venta latente nunca se firma antes de que el juego lo permita.**

También existe una salvaguarda contra la financiación innecesaria.

Si el Ministerio ha planificado una venta latente pero no hay ningún aumento de capacidad ni reducción de ventas correspondiente que la haga posible, vuelve a calcular el plan sin esa cantidad latente.

**Tu ciudad no se queda pagando a propósito una capacidad de producción adicional para respaldar una venta que no puede producirse.**

<a id="neighbor-deals-garbage"></a>
#### Reglas especiales para los contratos de basura

Los contratos de basura requieren un cuidado adicional porque los residuos se entregan según un calendario diario y no como un suministro mensual continuo.

#### Enviar tu basura a un vecino

Exportar basura se trata como comprar capacidad de eliminación.

El Ministerio limita la cantidad enviada a los vecinos a:

- La producción de basura de la propia ciudad después del reciclaje.
- La cantidad máxima que permiten las reglas de compra del juego.

No exporta más basura de la que produce la propia ciudad tras el reciclaje.

#### Recibir basura de un vecino

Importar basura genera ingresos, pero añade residuos que tu sistema de limpieza debe eliminar.

El Ministerio incluye la basura entrante **además de la producción propia de tu ciudad y de la reserva de limpieza**.

Solo acepta un contrato cuando:

- La financiación de limpieza elegida puede manejar los residuos adicionales.
- El mes previsto termina **sin basura sin recoger**.
- Los ingresos justifican cualquier gasto adicional de limpieza.
- Se respetan los límites de basura recibida del juego.
- La capacidad de eliminación restante de la ciudad es suficiente.

**El Ministerio no acepta basura solo porque un vecino esté dispuesto a pagar por ella.**

La ciudad debe poder manejar esa basura de forma segura y económica.

#### ¿Por qué evalúa el Ministerio cada contrato de basura por separado?

Dos contratos pueden tener la misma cantidad mensual combinada pero calendarios de entrega diarios distintos.

Por ejemplo, un contrato grande puede entregar de forma ligeramente distinta a dos contratos más pequeños, porque el juego redondea de forma independiente la cantidad diaria de cada contrato.

Por eso el Ministerio simula cada contrato con sus propios:

- Cantidad mensual.
- Cantidad de entrega diaria.
- Cantidad ya entregada durante el mes.
- Entregas restantes.

**Nunca sustituye los calendarios de entrega individuales por un único total mensual combinado.**

Esto es esencial para predecir si los vertederos y las instalaciones de tratamiento pueden despejar la basura antes de que acabe el mes.

#### ¿Por qué se mantienen algunos contratos de basura hasta el mes siguiente?

Los contratos de basura suelen completar sus entregas mensuales durante los diez primeros días, más o menos.

Supón que un contrato ya ha entregado toda su asignación.

Sustituirlo por un contrato nuevo durante el mismo mes puede introducir un calendario de entregas nuevo que la simulación del mes actual no había previsto.

Cuando el Ministerio no puede predecir con fiabilidad el comportamiento del vertedero el mes siguiente, **mantiene un contrato ya agotado hasta el cambio de mes**, en lugar de sustituirlo sin necesidad.

Esta restricción puede relajarse cuando conservar el contrato impediría que la ciudad mantuviera su servicio de limpieza.

#### ¿Cuenta como producción local la basura importada el mes pasado?

No.

El contador del juego *Basura importada el mes pasado* informa de entregas pasadas.

**El Ministerio nunca cuenta esas importaciones como basura producida por tu propia ciudad.**

La producción local se calcula a partir de los valores Producida y Reciclada de la ciudad.

La basura extranjera entra en el plan a través de sus contratos individuales.

<a id="neighbor-deals-verification"></a>
#### Paso 5: Comprobar cada contrato antes y después de cambiarlo

El Ministerio no aplica a ciegas un plan de contratos calculado antes.

**Cada cambio de contrato individual debe volver a comprobarse justo antes de escribirse.**

Esto protege frente a cambios en la demanda, la capacidad, la tesorería, la elegibilidad de los contratos y los propios registros regionales del juego.

#### Antes de cambiar un contrato

**1. Actualizar la información regional**

El Ministerio pide a SimCity 4 que actualice sus registros regionales de recursos, igual que hace el juego en sus actualizaciones mensuales normales.

Después lee el estado actual de la ciudad y calcula un plan de contratos nuevo.

El cambio previsto debe seguir coincidiendo con el plan nuevo:

- El mismo tipo de contrato y la misma ciudad vecina.
- La misma cantidad inicial.
- La misma cantidad objetivo.
- La misma fase de ejecución pendiente más temprana.
- La justificación económica exigida.

**Si el cambio propuesto ya no supera estas comprobaciones, no se escribe nada.**

El Ministerio no impone una decisión de contrato anticuada a una ciudad que ha cambiado.

**2. Comprobar el precio real del juego**

Para un contrato nuevo o un cambio de cantidad, el Ministerio pide a SimCity 4 su precio oficial.

Lo compara con la cantidad prevista por su propia fórmula de precios.

**Tolerancia del precio del contrato**

```math
\left|P_{\mathrm{game}}-P_{\mathrm{model}}\right|
\le
\max\left(
2,\left\lfloor\frac{P_{\mathrm{model}}}{100}\right\rfloor
\right)
```

Donde:

- **Pgame** es el precio mensual devuelto por SimCity 4.
- **Pmodel** es el precio calculado por el Ministerio.

La diferencia permitida es de al menos **§2**, y aumenta hasta aproximadamente el **1%** en los contratos grandes.

**Ejemplo**

Supón que el Ministerio prevé un precio mensual de **§1.874**.

La tolerancia es:

```math
\max(2,\lfloor1874/100\rfloor)=18
```

El juego debe devolver un precio entre **§1.856 y §1.892**, ambos incluidos.

| Precio informado por el juego | Resultado |
|---|---|
| §1.855 | Rechazado |
| §1.856 | Aceptado |
| §1.874 | Aceptado |
| §1.892 | Aceptado |
| §1.893 | Rechazado |

**¿Y si la diferencia es demasiado grande?**

El Ministerio considera que su modelo de precios no es fiable para la ciudad actual.

Detiene la gestión automática de contratos con vecinos durante el resto de esa sesión de la ciudad.

**La gestión local de la financiación de electricidad, agua y limpieza puede continuar de forma independiente.**

Así se evita que el Ministerio tome decisiones financieras con precios que el juego no confirma.

**3. Verificar el contrato exacto**

El Ministerio localiza el contrato vivo por su **tipo y ciudad vecina**, en lugar de fiarse de una posición recordada de antes en la lista de contratos.

Rechaza el cambio cuando:

- Más de un contrato coincide con la misma identidad.
- El contrato está suspendido.
- La cantidad actual ya no coincide con la cantidad usada en el plan.
- La cantidad objetivo solicitada ya no es legal.

Un objetivo debe ser una de las cantidades permitidas por el juego, o un múltiplo permitido del incremento de propuesta dentro del máximo actual del juego.

#### Una regla especial para la basura recibida

Hay un ajuste conservador para la basura recibida de los vecinos.

Supón que el Ministerio había planificado aceptar 10.000 unidades, pero ahora el juego solo permite 8.000.

Si 8.000 es una cantidad legal, el Ministerio puede reducir la cantidad de basura recibida prevista a esa cantidad menor.

Elige la **mayor cantidad legal que no supere el objetivo original**.

Es seguro porque aceptar menos basura extranjera no puede aumentar la carga de eliminación de la ciudad.

**El mismo ajuste no se aplica a las compras.**

Por ejemplo, reducir en silencio una compra de electricidad prevista podría dejar a la ciudad sin suministro suficiente.

Por eso el Ministerio no sustituye una cantidad de compra por otra menor sin recalcular el plan.

#### Después de cambiar un contrato

Que una operación de contrato tenga éxito no significa que todos los contadores del juego ya se hayan actualizado.

El Ministerio espera la confirmación antes de dar el cambio por completado.

Comprueba que:

1. **El contrato tiene el estado correcto:** Activo tras la creación o el cambio de tamaño, o cancelado con cantidad cero o eliminado tras la cancelación.
2. **La cantidad es correcta:** La cantidad coincide con el valor solicitado.
3. **El precio mensual almacenado está actualizado.**
4. **Las cantidades regionales son coherentes:** Los totales Importado y Exportado concuerdan con la lista de contratos.
5. **Los totales financieros son coherentes:** Los ingresos y gastos mensuales de los acuerdos con vecinos concuerdan con los contratos activos y suspendidos, según las reglas de contabilidad del juego.

**El juego puede actualizar estos valores un poco después de la operación del contrato.**

Si los valores nuevos todavía no han aparecido, el Ministerio espera.

Una actualización retrasada no se trata de inmediato como una transacción fallida.

Mientras un cambio de contrato espera confirmación:

- No se hace ningún otro cambio de contrato para ese recurso.
- La transacción pendiente se vuelve a comprobar cada día simulado.
- Los demás recursos pueden continuar con su gestión independiente.

**El Ministerio solo continúa cuando se ha confirmado el cambio de contrato anterior.**

<a id="neighbor-deals-revenue"></a>
#### ¿Cómo mide el Ministerio el ahorro de los contratos?

El Ministerio distingue tres etapas en la decisión sobre un contrato.

| Etapa | Qué significa |
|---|---|
| **Proyectado** | El beneficio mensual esperado de un plan de contratos, calculado antes de aplicar los cambios. |
| **Pendiente** | El beneficio en efectivo esperado de un cambio de contrato que se ha escrito pero sigue esperando verificación. |
| **Realizado** | El cambio en los ingresos y gastos mensuales de los acuerdos con vecinos confirmado por los propios contadores del presupuesto del juego. |

Estas distinciones impiden que el Ministerio presente como dinero ya ganado un contrato planificado o sin verificar.

#### Ahorro proyectado

Antes de cambiar contratos, el Ministerio estima cuánto mejor sería el arreglo propuesto que conservar los contratos existentes.

Calcula dos valores:

- **Beneficio en efectivo:** El cambio esperado en los ingresos y gastos reales.
- **Beneficio económico:** El beneficio en efectivo incluido el desgaste estimado de las centrales eléctricas, cuando esa opción está activada.

Esta comparación se hace cuando conservar los contratos existentes es una alternativa viable.

Un contrato planificado o una venta latente **no se cuenta como ingreso realizado** solo porque aparezca en una cartera optimizada.

#### Ahorro pendiente

Una vez escrito un cambio de contrato, pero antes de que el juego lo confirme, el Ministerio registra su margen en efectivo esperado como **pendiente**.

Sigue siendo una estimación.

No pasa a ser un resultado realizado hasta que se han verificado el estado del contrato y los contadores financieros del juego.

#### Ingresos de contrato realizados

Tras la confirmación, el Ministerio compara los ingresos y gastos mensuales de los acuerdos con vecinos del juego antes y después del cambio.

**Resultado realizado del contrato**

```math
\Delta_{\mathrm{realized}}=
(I_{\mathrm{after}}-X_{\mathrm{after}})
-
(I_{\mathrm{before}}-X_{\mathrm{before}})
```

Donde:

- **Iantes** e **Idespués** son los ingresos mensuales totales de los acuerdos con vecinos del juego antes y después del cambio.
- **Xantes** y **Xdespués** son los gastos mensuales correspondientes de los acuerdos con vecinos.
- **Δrealizado** es el cambio verificado en los ingresos netos mensuales de los contratos.

**Ejemplo: una nueva venta de electricidad**

Supón que las cifras mensuales de los acuerdos con vecinos de la ciudad eran:

| | Antes | Después |
|---|---:|---:|
| Ingresos de contratos | §200 | §500 |
| Gastos de contratos | §100 | §100 |
| **Ingresos netos de contratos** | **§100** | **§400** |

El cambio realizado es:

```math
\Delta_{\mathrm{realized}}=
(500-100)-(200-100)=300
```

**Mejora verificada de los ingresos mensuales de los contratos: §300.**

Este es el cambio en los ingresos de los contratos únicamente.

Todavía no resta ningún gasto adicional de las centrales eléctricas necesario para respaldar la venta.

Esos cambios se registran por separado cuando el Ministerio ajusta la financiación local.

#### ¿Y si no se pueden leer los contadores financieros?

Si SimCity 4 no proporciona totales fiables de ingresos y gastos de contratos antes y después, el Ministerio usa como alternativa el margen en efectivo planificado.

**El registro deja constancia explícita de que se usó una estimación en lugar de una medición financiera verificada.**

Ambas cosas no se presentan en silencio como equivalentes.

#### ¿Y el ahorro por la financiación local reducida?

Una compra regional puede crear dos efectos financieros distintos.

Por ejemplo, comprar electricidad puede:

1. Añadir un gasto mensual de contrato.
2. Permitir al Ministerio reducir la financiación de las centrales eléctricas.

El primer efecto pertenece a la contabilidad de los acuerdos con vecinos.

El segundo pertenece a la gestión de la financiación de la electricidad.

**El Ministerio los registra por separado para no contar dos veces el mismo ahorro.**

Del mismo modo, un contrato de basura puede aumentar los ingresos de contratos y requerir al mismo tiempo más financiación de limpieza. Son cambios de presupuesto distintos.

**El objetivo del Ministerio es elegir los contratos según su efecto económico completo, aplicarlos sin alterar los servicios esenciales y distinguir los beneficios esperados de los cambios realmente confirmados por el juego.**
<a id="neighbor-deals-reevaluation"></a>
#### ¿Cuándo reconsidera el Ministerio los contratos existentes?

El Ministerio no da por hecho que un contrato existente deba continuar para siempre.

**En cada revisión, vuelve a calcular el valor económico de todos los contratos con la información más reciente de la ciudad.**

Por ejemplo:

- Una venta de electricidad puede volverse menos rentable a medida que aumenta la demanda de tu ciudad.
- Una compra de agua puede dejar de ser necesaria después de que construyas otra instalación de bombeo.
- Una importación de basura puede resultar demasiado cara de manejar si tus vertederos se acercan a sus límites.

Cuando la situación cambia, el Ministerio puede aumentar, reducir o cancelar contratos para encontrar un arreglo mejor.

**Las necesidades propias de tu ciudad y sus reservas de seguridad siempre tienen prioridad.**

Si la demanda de electricidad crece, por ejemplo, el Ministerio reduce o cancela las ventas de electricidad antes de dejar que consuman la capacidad que necesitan tu ciudad y su reserva.

#### ¿Recuerda el Ministerio los fallos anteriores?

Normalmente, cada revisión empieza de cero. No hay un periodo de retención ni un tiempo de enfriamiento general para los contratos.

Sin embargo, dos excepciones impiden repetir acciones que el juego ya ha rechazado.

**1. El juego rechazó un contrato nuevo**

Si SimCity 4 se niega a crear un contrato, el Ministerio no intenta esa misma creación de nuevo hasta el **tercer cambio de mes simulado después del rechazo**.

Así las condiciones de elegibilidad del juego, como la tesorería, la demanda y las conexiones, tienen tiempo de cambiar.

**2. El juego se niega a cancelar un contrato inactivo**

Un contrato inactivo es un contrato activo con una cantidad de cero.

Si el juego rechaza su cancelación, el Ministerio lo deja en paz durante el resto de la sesión actual de la ciudad en lugar de intentar repetidamente la misma operación.

<a id="neighbor-deals-emergency"></a>
#### ¿Qué ocurre durante una carencia grave de suministro?

A veces una ciudad simplemente no puede obtener electricidad o agua suficientes para cubrir todas sus necesidades.

Por ejemplo:

- Se demuele una central eléctrica importante.
- Varias instalaciones de agua dejan de producir.
- Se pierde una conexión regional.
- Una ciudad vecina ya no puede suministrar un recurso contratado.

Normalmente, el Ministerio busca contratos que cubran tanto tu demanda como tu reserva de seguridad configurada.

**Pero ¿y si ninguna combinación legal de producción local y compras regionales lo consigue?**

En esa situación, el Ministerio cambia de prioridades.

En lugar de rechazar todos los arreglos posibles porque no pueden cubrir el requisito completo, **intenta abastecer la mayor parte posible de la demanda real de la ciudad**.

#### ¿Cuánta demanda se puede seguir atendiendo?

**Demanda atendida durante una emergencia**

```math
\mathrm{served}(B,S)=
D-\max\left(
0,\max(0,D-B)+S-C_{\max}
\right)
```

Donde:

- **D** es la demanda real de la ciudad.
- **B** es la cantidad comprada a las ciudades vecinas.
- **S** es la cantidad vendida a las ciudades vecinas.
- **Cmax** es la mayor capacidad local de generación o bombeo que las instalaciones de la ciudad pueden proporcionar legalmente.

La fórmula determina cuánta demanda sigue cubierta una vez tenidas en cuenta las importaciones, las exportaciones y la capacidad local máxima disponible.

**Ejemplo: una ciudad con una carencia de electricidad**

Supón que tu ciudad necesita **10.000 unidades**, pero sus centrales eléctricas solo pueden producir **6.000 unidades**.

Sin importaciones, la ciudad no puede cubrir 4.000 unidades de su demanda.

Si una ciudad vecina puede proporcionar legalmente 3.000 unidades, comprarlas aumenta la demanda atendida hasta **9.000 unidades**.

La ciudad sigue con falta de electricidad, pero el Ministerio ha reducido el déficit.

**Durante una emergencia, el Ministerio sigue estas prioridades:**

1. **Atender la mayor cantidad posible de demanda real.**
2. **Restablecer la reserva de seguridad cuando sea posible.**
3. **Elegir el arreglo menos costoso entre soluciones igual de eficaces.**

Por tanto, la reserva normal se sigue teniendo en cuenta, pero el servicio real va primero cuando no hay capacidad suficiente para satisfacerlo todo.

#### ¿Qué pasa con las ventas de electricidad y agua?

El Ministerio cancela todas las ventas que legalmente puede cambiar.

Esto libera capacidad para tu propia ciudad.

Después considera las cantidades de compra legales disponibles de las ciudades vecinas.

Normalmente, las compras no pueden superar la demanda propia de la ciudad.

**Durante una emergencia se puede permitir un escalón de cantidad adicional por encima de la demanda**, pero solo si ese escalón aumenta en más del 1% la cantidad de demanda realmente atendida.

Es una excepción limitada pensada para tratar con las opciones discretas de cantidad de contrato del juego.

Los contratos suspendidos se siguen incluyendo en los cálculos, pero el Ministerio no intenta modificarlos.

#### ¿Y una emergencia de eliminación de basura?

Para la basura, el Ministerio sigue el principio equivalente.

Intenta:

- Cancelar los contratos que traen basura adicional a tu ciudad.
- Aumentar las exportaciones de basura a las ciudades vecinas.
- Usar primero los arreglos de eliminación disponibles menos costosos.

Las exportaciones de basura siguen sujetas a las cantidades legales del juego y a la cantidad de basura que produce tu ciudad después del reciclaje.

**El Ministerio no puede garantizar que se resuelva toda carencia.**

Si las instalaciones y los contratos disponibles no son suficientes, el jugador debe aportar infraestructura adicional o resolver el problema de fondo.

El Ministerio solo emite un aviso de carencia cuando el problema está **confirmado y es persistente**, en lugar de avisarte de cada fluctuación temporal.

Consulta [Avisos](safety.md#notices).

<a id="neighbor-deals-safety"></a>
### 4. Condiciones de seguridad

El Ministerio sigue reglas estrictas al crear, cambiar o cancelar contratos regionales.

#### Proteger los servicios propios de tu ciudad

**Las necesidades de tu ciudad van primero.**

Las ventas de electricidad y agua no deben consumir la capacidad necesaria para atender a la ciudad y mantener su reserva configurada.

La propia definición de excedente disponible de SimCity 4 no incluye los márgenes de seguridad del Ministerio.

Por eso el Ministerio realiza sus propias comprobaciones de capacidad antes de aceptar una venta.

#### Confiar solo en compras fiables

Una compra se trata como suministro fiable solo mientras está **correctamente activa**.

El Ministerio no confía en los contratos que están:

- Pendientes.
- Suspendidos.
- Fallando por una conexión que falta.
- Fallando porque la ciudad que paga no puede permitírselos.

**La producción local nunca se reduce antes de que la compra de sustitución esté en vigor.**

No hay ninguna regla artificial que exija que una compra activa tenga un número concreto de días restantes.

El Ministerio la considera utilizable hasta que el juego cambie su estado.

#### Respetar las ventas existentes

Una venta activa de electricidad o agua se trata como una obligación.

El Ministerio incluye su cantidad completa al calcular cuánta capacidad necesita tu ciudad.

El mismo principio se aplica a los contratos de basura y a la capacidad de limpieza necesaria para respaldarlos.

Una venta puede reducirse o cancelarse en una revisión posterior, pero **mientras siga en vigor, el plan de servicios locales debe tenerla en cuenta**.

#### Respetar las reglas de contratos del juego

El Ministerio solo usa:

- Las ofertas de contrato que SimCity 4 pone realmente a disposición.
- Las cantidades que permite el sistema de contratos del juego.
- Las propias funciones del juego para crear, cambiar de tamaño y cancelar contratos.

Nunca modifica directamente las ciudades vecinas ni fuerza un contrato que el juego rechazaría.

**Los contratos suspendidos nunca se modifican.**

#### Detenerse con seguridad cuando algo sale mal

Si se produce un error técnico durante una operación de contrato, el Ministerio detiene la gestión automática de contratos durante el resto de la sesión actual de la ciudad.

Esto puede ocurrir cuando:

- El juego rechaza una operación de contrato de forma inesperada.
- Un contrato entra en un estado inesperado después de cambiarlo.
- El precio informado por el juego difiere demasiado del cálculo del Ministerio.
- La información resultante del contrato no se puede verificar de forma fiable.

**Los gestores de financiación de electricidad, agua y limpieza pueden seguir funcionando.**

Simplemente usan los contratos regionales existentes como compromisos fijos.

Si recargas la ciudad o desactivas la gestión de contratos mientras una transacción está sin terminar, el Ministerio abandona las demás acciones de esa transacción.

Una transacción que solo espera las actualizaciones contables del juego puede seguir observándose hasta que esos valores se asienten, sin hacer llamadas de contrato adicionales.

#### Un cambio de contrato cada vez

Para cada recurso, el Ministerio aplica **un cambio de contrato cada vez**.

Después verifica el estado resultante del contrato y los totales regionales y financieros del juego antes de hacer otro cambio.

Así se evita que varias operaciones de contrato sin verificar interfieran entre sí.

#### ¿Qué pasa con los mensajes de los asesores del juego original?

Mientras el Ministerio gestiona los contratos regionales, los mensajes de los asesores del juego original sobre acuerdos con vecinos quedan suprimidos por un archivo de recursos incluido.

Entre ellos están:

- Las ofertas de contratos nuevos.
- Los mensajes de renovación de contratos.
- Las advertencias de impago de contratos.

El Ministerio se ocupa de las situaciones correspondientes con su propio sistema de gestión.

Cuando una carencia real no se puede resolver automáticamente, puede emitir su propio aviso.

Consulta [Avisos](safety.md#notices).

**Importante:** Los mensajes del juego original los suprime un archivo de recursos que se carga al iniciar SimCity 4. Desactivar la gestión automática de contratos en las opciones no restablece esos mensajes de inmediato. El procedimiento para restablecerlos se explica en [Limitaciones conocidas](#neighbor-deals-limitations).

<a id="neighbor-deals-settings"></a>
### 5. Ajustes

La gestión de los contratos regionales se puede configurar en **Opciones de juego → Ministerio de Optimización**, en la página **Servicios**.

| Ajuste | Valor por defecto | Qué hace |
|---|---|---|
| [Gestionar acuerdos con vecinos](options.md#opt-deals) | Sí | Activa la gestión automática de los contratos regionales. |
| [Gestionar acuerdos de electricidad](options.md#opt-tradepower) | Sí | Permite que el Ministerio compre y venda electricidad. |
| [Gestionar acuerdos de agua](options.md#opt-tradewater) | Sí | Permite que el Ministerio compre y venda agua. |
| [Gestionar acuerdos de basura](options.md#opt-tradegarbage) | Sí | Permite que el Ministerio envíe basura a los vecinos o reciba la suya. |
| [Comprar a una ciudad para vender a otra](options.md#opt-arbitrage) | **No** | Permite comprar y vender el mismo recurso en un mismo plan cuando resulta económicamente beneficioso. |
| [Reserva de electricidad](options.md#opt-powerreserve) | 20% | Protege el suministro local de electricidad. |
| [Reserva de agua](options.md#opt-waterreserve) | 10% | Protege el suministro local de agua. |
| [Reserva para tratar basura](options.md#opt-sanitationreserve) | 2% | Aporta capacidad adicional para eliminar basura. |

#### ¿Cómo interactúan los interruptores?

**Gestionar acuerdos con vecinos** es el interruptor principal de los contratos regionales.

Cuando está desactivado:

- El Ministerio no solicita nuevas ofertas de contrato.
- No crea, redimensiona ni cancela contratos.
- Los contratos existentes siguen en vigor y se incluyen en los cálculos de financiación local.

Los tres interruptores de recurso permiten gestionar la electricidad, el agua y la basura de forma independiente.

**También puedes gestionar los contratos sin gestionar la financiación local.**

Por ejemplo:

| Ajustes | Resultado |
|---|---|
| **Gestionar acuerdos de electricidad: Sí** / **Gestionar la electricidad: Sí** | El Ministerio puede coordinar los contratos de electricidad y la financiación de las centrales. |
| **Gestionar acuerdos de electricidad: Sí** / **Gestionar la electricidad: No** | Se pueden gestionar los contratos, pero la financiación de las centrales eléctricas no cambia. |
| **Gestionar acuerdos de electricidad: No** / **Gestionar la electricidad: Sí** | Se puede optimizar la financiación de las centrales, pero los contratos de electricidad existentes no cambian. |
| **Ambos en No** | Ni los contratos de electricidad ni la financiación de las centrales se modifican automáticamente. |

La misma independencia se aplica al agua y a la limpieza.

**Si la gestión de contratos está activada pero la gestión de la financiación local está desactivada**, el Ministerio solo puede usar la capacidad de producción o de eliminación que ya está disponible con la financiación actual.

No aumenta la financiación para respaldar un contrato nuevo.

**Si la gestión de la financiación local está activada pero la gestión de contratos está desactivada**, las compras y ventas existentes se tratan como compromisos fijos.

El Ministerio sigue pudiendo vigilar el recurso y avisarte de las carencias persistentes.

Desactivar **Gestionar acuerdos con vecinos** conserva los interruptores individuales de cada recurso y el ajuste de arbitraje. Sus valores vuelven a aplicarse cuando se reactiva el interruptor principal.

#### Ajuste avanzado: beneficio mínimo del contrato

Puedes configurar un beneficio mensual mínimo para las decisiones opcionales sobre contratos.

Este ajuste está disponible a través del archivo INI, no en la ventana de opciones del juego.

| Ajuste INI | Valor por defecto | Intervalo permitido |
|---|---:|---:|
| `MinContractBenefit` | §0/mes | §0–§100.000/mes |

Archivo: `DepartmentEfficiencyUtility.ini`

Cuando vale cero, **basta cualquier beneficio económico estrictamente positivo**.

Por ejemplo, puede cumplir los requisitos un contrato que mejore en §1 el resultado económico mensual.

Si estableces:

```ini
MinContractBenefit=20
```

Las creaciones opcionales de contratos y los cambios de cantidad deben producir al menos **§20 al mes** de beneficio adicional.

Este ajuste puede ser útil si prefieres menos cambios de contrato a cambio de ignorar pequeños ahorros potenciales.

**El umbral nunca bloquea un cambio necesario para mantener los servicios esenciales.**

SimCity 4 no cobra una tarifa aparte solo por crear, modificar o cancelar un contrato regional.

<a id="neighbor-deals-examples"></a>
### 6. Ejemplos desarrollados

Los siguientes ejemplos muestran cómo evalúa el Ministerio los contratos regionales.

**Las fórmulas y los precios de los contratos del juego se calculan con sus reglas reales.**

Sin embargo, los costes de funcionamiento locales que aparecen en estos ejemplos son valores ilustrativos que representan lo que el optimizador de financiación correspondiente podría calcular para una ciudad concreta.

#### Ejemplo 1: vender electricidad sin sacrificar la reserva

Imagina dos ciudades vecinas.

**Tu ciudad (ciudad A)**

- Demanda de electricidad: **10.000 unidades**.
- Necesidad de electricidad según el juego: **10.300 unidades**.
- Reserva de electricidad: **20%**.
- Producción de electricidad actual: **14.000 unidades**.
- Gastos actuales de las centrales eléctricas: **§1.400 al mes**.

**Ciudad vecina (ciudad B)**

- Producción de electricidad: **3.000 unidades**.
- Necesidad de electricidad: **6.000 unidades**.
- Tesorería: **§250.000**.

#### Paso 1: ¿qué permite SimCity 4?

La ciudad A tiene un excedente de:

```math
14000-10300=3700
```

El límite de compra de la ciudad B es:

```math
2.5\times6000-3000=12000
```

Por tanto, la cantidad máxima de contrato del juego es:

```math
Q_{\max}=\min(3700,12000)=3700
```

La ciudad B cumple los requisitos de propuesta del juego.

El precio mínimo del contrato es de aproximadamente **§110 al mes** por cada 1.000 unidades, y la tesorería de la ciudad B supera con holgura los cinco años de pagos exigidos.

**SimCity 4 está dispuesto a ofrecer una compra de electricidad a la ciudad A.**

#### Paso 2: ¿qué exige el Ministerio?

El Ministerio aplica su propia reserva de electricidad del 20%.

Sin una venta:

```math
N(0,0)=\max(12000,10300)=12000
```

La ciudad A debe mantener al menos **12.000 unidades de capacidad de generación local**.

Cada unidad vendida aumenta ese requisito en una unidad.

El Ministerio evalúa las cantidades de venta candidatas usando el incremento de propuesta derivado del límite del vecino:

```math
\text{paso}=\max(12000/10,1000)=1200
```

Por tanto, considera cantidades como 1.200, 2.400 y 3.600 unidades.

También se pueden considerar cantidades mayores como *ventas latentes*, es decir, ventas que requerirían capacidad local adicional antes de que el juego pudiera aceptarlas legalmente.

#### Paso 3: ¿qué cantidad es la más rentable?

| Electricidad vendida | Ingresos mensuales | Coste local ilustrativo | Coste local adicional | Beneficio neto |
|---:|---:|---:|---:|---:|
| 0 | §0 | §1.250 | — | §0 |
| 1.200 | §132 | §1.330 | §80 | +§52 |
| **2.400** | **§264** | **§1.420** | **§170** | **+§94** |
| 3.600 | §396 | §1.560 | §310 | +§86 |
| 4.800 (latente) | §527 | §1.720 | §470 | +§57 |

**La opción más rentable es vender 2.400 unidades.**

Esto supone una mejora estimada de **§94 al mes**.

Sin embargo, la venta requiere:

```math
12000+2400=14400
```

**14.400 unidades de capacidad de generación local.**

La ciudad A actualmente produce solo 14.000.

#### Paso 4: ¿cómo se aplica el plan?

El Ministerio no firma la venta de inmediato.

Primero aumenta la financiación de las centrales eléctricas para alcanzar la capacidad necesaria de 14.400 unidades.

Cuando el juego reconoce la producción adicional, el contrato se reconsidera en una revisión posterior.

Con 14.400 unidades de producción, el excedente disponible según el juego pasa a ser:

```math
14400-10300=4100
```

La venta planificada de 2.400 unidades ya está dentro del máximo permitido, y se puede escribir si el plan actualizado sigue seleccionándola.

**El Ministerio aumenta la producción antes de firmar la venta, y así protege la reserva de la ciudad durante toda la transición.**

Sin la protección de reserva del Ministerio, el jugador habría podido vender las 3.700 unidades completas que permiten los cálculos originales del juego, sin dejar ningún margen de reserva.

#### Ejemplo 2: comprar electricidad y ahorrar más gracias a la reserva

Imagina otra ciudad, la **ciudad C**.

- Demanda de electricidad: **10.000 unidades**.
- Necesidad según el juego: **10.300 unidades**.
- Producción actual: **12.500 unidades**.
- Reserva de electricidad: **20%**.

Su proveedor vecino tiene:

- Producción: **30.000 unidades**.
- Necesidad: **20.000 unidades**.
- Coste medio por unidad: **§0,08**.

El proveedor tiene 10.000 unidades de excedente.

El límite de compra del comprador es:

```math
2.5\times10300-12500=13250
```

Por tanto, el máximo del juego es de **10.000 unidades**, y las cantidades de las propuestas aumentan en escalones de 1.000.

El límite de compra propio del Ministerio también es de 10.000 unidades.

#### Paso 1: ¿cuánta capacidad local hace falta?

Sin compras, la reserva del 20% de la ciudad exige:

```math
N(0,0)=12000
```

Comprar electricidad reduce la cantidad que hay que producir localmente.

**Cada unidad comprada puede reducir en 1,2 unidades el requisito de capacidad local**, mientras el requisito basado en la reserva siga siendo vinculante.

#### Paso 2: comparar las posibles compras

| Unidades compradas | Capacidad local necesaria | Coste local ilustrativo | Ahorro local | Precio del contrato | Ahorro neto |
|---:|---:|---:|---:|---:|---:|
| 0 | 12.000 | §1.250 | — | — | §0 |
| 1.000 | 10.800 | §1.160 | §90 | §89 | +§1 |
| **2.000** | **9.600** | **§1.060** | **§190** | **§179** | **+§11** |
| 3.000 | 8.400 | §975 | §275 | §267 | +§8 |
| 4.000 | 7.200 | §900 | §350 | §356 | −§6 |

**El acuerdo más barato en conjunto es comprar 2.000 unidades.**

La compra cuesta **§179 al mes**, pero permite reducir en **§190 los gastos de las centrales eléctricas locales**.

Ahorro neto estimado:

```math
190-179=11
```

**§11 al mes.**

#### Paso 3: aplicar los cambios con seguridad

El Ministerio primero crea y verifica la compra.

Después reduce la financiación de las centrales eléctricas durante la fase de ejecución posterior que corresponda.

**¿Y si el beneficio mínimo se establece en §20?**

Con:

```ini
MinContractBenefit=20
```

El contrato opcional no se crea.

Su beneficio mensual de §11 está por debajo del umbral configurado.

Este ajuste permite evitar pequeños reajustes económicos si prefieres un conjunto de contratos más estable.

#### Ejemplo 3: por qué un contrato no siempre se puede redimensionar a cualquier cantidad

Supón que tu ciudad ya tiene una venta de electricidad con:

- Cantidad actual: **3.088 unidades**.
- Incremento del contrato: **1.544 unidades**.
- Máximo del juego: **15.442 unidades**.

Las cantidades que se pueden elegir en el juego siguen el incremento:

1.544; 3.088; 4.632; 6.176; y así sucesivamente, hasta 15.440.

**Una cantidad de contrato de 6.000 unidades no está disponible.**

La cantidad permitida más cercana por encima es **6.176 unidades**.

El Ministerio respeta esas cantidades legales en lugar de enviar al juego un número arbitrario.

#### Ejemplo 4: aceptar basura de otra ciudad

Una ciudad vecina se ofrece a pagarte por eliminar **20.000 unidades de basura al mes**.

Normalmente el juego entrega:

```math
20000\times0.10=2000
```

**2.000 unidades por día simulado**, durante aproximadamente diez días.

El Ministerio evalúa si tus instalaciones de limpieza pueden gestionar esas 20.000 unidades adicionales.

Tiene en cuenta:

- La producción de basura de tu propia ciudad tras el reciclaje.
- Tu reserva de limpieza configurada.
- La basura que ya está sin recoger.
- Las importaciones y exportaciones existentes.
- La capacidad de tratamiento de las incineradoras y las plantas de conversión de residuos en energía.
- La admisión disponible en los vertederos.
- El calendario de entregas diarias de cada contrato.

**El contrato solo se acepta si el mes proyectado termina sin basura sin recoger.**

Los ingresos también deben justificar cualquier financiación adicional de la limpieza que haga falta para gestionar los residuos entrantes.

Por último, la basura total recibida debe mantenerse por debajo del límite permitido por el juego.

En el ejemplo de la Sección 1, ese límite era de aproximadamente **541.667 unidades**.

**Un contrato que paga bien se rechaza igualmente si tu ciudad no puede gestionar con seguridad la basura adicional.**

<a id="neighbor-deals-limitations"></a>
### 7. Limitaciones conocidas

El Ministerio puede gestionar los contratos regionales de forma automática, pero sigue sujeto a las reglas originales de SimCity 4 y a la información disponible.

#### Ciudades vecinas e información regional

- **Los registros de los vecinos son instantáneas.** SimCity 4 no simula las ciudades vecinas mientras juegas la ciudad actual. El Ministerio usa las últimas cifras regionales guardadas de esos vecinos, que pueden estar desactualizadas.
- **El Ministerio nunca modifica las ciudades vecinas.** No las carga, no modifica sus edificios ni actualiza su información guardada.
- **Las fechas de los contratos siguen bajo el control de SimCity 4.** El juego gestiona las fechas de inicio, el vencimiento y la renovación de los contratos. El Ministerio no intenta predecirlas ni modificarlas. Cuando un contrato termina o queda suspendido, su nuevo estado se tiene en cuenta en la revisión siguiente.
- **Las ventas nuevas requieren una oferta existente.** El Ministerio puede aumentar la producción de tu ciudad para respaldar una venta mayor a un vecino que ya ofrece ese recurso. No puede hacer que aparezca un comprador completamente nuevo cuando el juego no tiene ninguna oferta de ese vecino.

#### Aplicación de los cambios de contrato

- **Solo se aplica un paso por revisión.** Cada recurso realiza como máximo un cambio de contrato o un paso de financiación local durante una revisión. Por eso, una reorganización importante puede requerir varias revisiones.
- **Se verifica el precio de cada contrato.** Si el precio de referencia del juego difiere del modelo del Ministerio más allá de la tolerancia permitida, la gestión de contratos se detiene durante el resto de la sesión de la ciudad.
- **Los contratos suspendidos no se pueden modificar.** El juego controla su resolución, y el Ministerio debe trabajar sin contar con ellos.
- **Las búsquedas en contratos de basura grandes tienen límites.** Si el Ministerio no puede evaluar de forma exhaustiva las combinaciones legales porque hay más de cinco millones en uno de los lados, no hace ningún cambio de contrato especulativo. Las búsquedas mixtas de compra y venta también tienen sus propios límites, descritos anteriormente.

#### Mensajes originales de los asesores sobre acuerdos con vecinos

El Ministerio incluye un archivo de recursos que suprime los mensajes originales de los asesores de SimCity 4 sobre los acuerdos con vecinos.

Este archivo es:

`DepartmentEfficiencyDealAdvice.dat`

**SimCity 4 carga los archivos de recursos al iniciarse el juego.**

Por tanto, desactivar las opciones de gestión de contratos del Ministerio no restablece de inmediato los mensajes originales de los asesores.

Si quieres gestionar los contratos manualmente y restaurar los mensajes originales:

1. Cierra SimCity 4.
2. Elimina `DepartmentEfficiencyDealAdvice.dat` de la carpeta Plugins del juego.
3. Reinicia el juego.

Esto también restaura mensajes originales como *Acuerdo incumplido*.

El Ministerio registra una advertencia en su registro cuando el archivo de recursos y los ajustes de gestión actuales no coinciden.

#### Propuestas pendientes

Cuando el Ministerio solicita ofertas de contrato, usa la misma operación del juego que se emplea al abrir la ventana de Acuerdos con vecinos.

Esto puede hacer que aparezcan en las listas de contratos del juego propuestas pendientes de otras ciudades vecinas.

Como en el juego original, **las propuestas pendientes no generan ingresos ni gastos** hasta que se convierten en contratos activos.

**El Ministerio trabaja dentro del sistema de contratos del juego. Puede optimizar las opciones disponibles, pero no puede crear recursos, conexiones, elegibilidad ni información fiable que SimCity 4 no proporcione.**

[Volver al contenido](index.md#contents) | [Volver al README](../../README.es.md)

---

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
