[English](../en/garbage.md) | [Português](../pt/garbage.md) | [Français](../fr/garbage.md) | **Español**

[Volver al contenido](index.md#contents) | [Volver al README](../../README.es.md)

<a id="garbage-landfills"></a>
## D. Basura y vertederos

El Ministerio ayuda a tu ciudad a gestionar la basura con eficiencia, manteniendo los costes de limpieza lo más bajos posible sin dejar que los residuos se acumulen de forma innecesaria.

Ofrece cuatro funciones independientes:

1. **Financiación automática de la limpieza:** ajusta el presupuesto de limpieza para tratar la basura de la ciudad, incluida la reserva de seguridad que hayas elegido.
2. **Contratos regionales de basura:** gestiona las importaciones y exportaciones de basura cuando la gestión de acuerdos con vecinos está activada. Consulta [Acuerdos con vecinos](deals.md#neighbor-deals).
3. **Consejo sobre el tamaño del vertedero (opcional):** recomienda ampliar o reducir las zonas de vertedero cuando hacerlo podría reducir de forma notable tus gastos mensuales. **El Ministerio nunca cambia por sí mismo la zonificación del vertedero con este fin.**
4. **Reparación de casillas fantasma de vertedero:** corrige automáticamente un error del juego por el que algunas casillas que antes eran de vertedero se siguen contando y cobrando como vertedero activo. Usa el propio mecanismo de zonificación del juego y no tiene una opción independiente.

Para entender cómo funcionan estas funciones, es importante distinguir entre **producción de basura, capacidad de tratamiento, admisión de los vertederos y almacenamiento a largo plazo**.

Todas las cantidades de basura de este capítulo usan las unidades internas de basura de SimCity 4. Los gastos se expresan en simoleones (§) por mes simulado.

<a id="garbage-landfills-vanilla"></a>
### 1. Cómo funciona la basura en SimCity 4

#### Cómo se produce y se recicla la basura

Cada día simulado, SimCity 4 procesa los edificios de la ciudad y calcula cuánta basura generan.

El juego registra esta cantidad como **basura producida**.

Los centros de reciclaje eliminan una parte de esa basura antes de que entre en el sistema de eliminación.

La cantidad reciclada depende de las instalaciones de reciclaje de la ciudad. **Cambiar la financiación de la limpieza no cambia la tasa de reciclaje.**

La basura restante, junto con los residuos que ya se hayan quedado sin recoger, debe ser gestionada por el sistema de eliminación de la ciudad.

#### ¿En qué orden elimina el juego la basura?

SimCity 4 procesa la basura en un orden concreto.

| Prioridad | Método de eliminación | Qué ocurre |
|---|---|---|
| **1** | Exportaciones de basura | Los residuos se envían a ciudades vecinas mediante contratos de exportación activos. |
| **2** | Incineradoras y plantas de conversión de residuos en energía | Las instalaciones de tratamiento procesan la basura, hasta su capacidad disponible. |
| **3** | Vertederos | La basura restante se envía a los vertederos, sujeta a sus límites de admisión y almacenamiento. |
| **4** | Eliminación oculta de basura | El juego elimina en silencio una pequeña cantidad adicional de basura. |

**El juego prueba cada método en este orden antes de pasar al siguiente.**

Por ejemplo, si tu ciudad exporta parte de la basura y trata el resto en incineradoras, sus vertederos solo tienen que admitir lo que queda después.

**¿Qué es la eliminación oculta de basura?**

SimCity 4 tiene un mecanismo integrado que a veces se llama *abducción de basura*.

Puede eliminar en silencio hasta **2.000 unidades de basura por mes simulado**, según la configuración del juego original.

Estos residuos desaparecen sin contarse en las estadísticas normales de eliminación de basura.

El Ministerio debe tener en cuenta este comportamiento al interpretar las cifras de basura del juego.

**¿Qué ocurre con la basura que no se puede eliminar?**

Toda la basura que queda después de las cuatro etapas permanece en la cuadrícula de la ciudad como **basura sin recoger**.

Son los residuos que pueden acumularse en tus calles y edificios cuando el sistema de eliminación no da abasto con las necesidades de la ciudad.

El juego mantiene un total de toda la ciudad de esta basura sin recoger, que el Ministerio puede leer.

#### Cómo funcionan los contratos de basura con las ciudades vecinas

Los contratos de basura funcionan de forma distinta a los de electricidad y agua.

Hay dos tipos:

- **Importaciones de basura:** una ciudad vecina te envía su basura y te paga por eliminarla.
- **Exportaciones de basura:** tu ciudad paga a una ciudad vecina para que se lleve su basura.

Aunque estos contratos especifican una cantidad mensual, **la basura no se entrega de forma uniforme a lo largo del mes**.

En cambio, el juego normalmente mueve cada día simulado aproximadamente el 10% de la cantidad mensual del contrato, hasta que se ha entregado la asignación de ese mes.

**Entrega diaria de un contrato de basura (juego base)**

```math
s = \operatorname{round}(0.1 \times A)
\qquad
\text{mientras } A-U>0
```

Después de cada entrega diaria:

```math
U \leftarrow U+s
```

Donde:

- **A** es la cantidad mensual de basura del contrato.
- **U** es la cantidad ya entregada durante el mes en curso.
- **s** es la cantidad entregada ese día.

La cantidad entregada se reinicia al comienzo de cada mes simulado.

**Ejemplo: exportar 38.990 unidades al mes**

```math
s = 38990 \times 0.1 = 3899
```

El juego exporta **3.899 unidades al día**, y normalmente completa la cantidad mensual en unos diez días.

Esto significa que la mayoría de las entregas de los contratos de basura se producen **durante los primeros diez días más o menos de cada mes**, en lugar de repartirse de forma uniforme entre los 30 o 31 días.

También hay un pequeño detalle de redondeo: la última entrega diaria no se limita a la cantidad exacta que queda. Por eso algunos contratos pueden entregar algo más que su cantidad mensual nominal.

**¿Por qué importa esto?**

Una ciudad puede recibir una gran cantidad de basura importada durante la primera parte del mes.

El sistema de limpieza debe ser capaz de soportar esa concentración temporal, aunque la cantidad mensual total parezca razonable.

**¿Y las estadísticas de basura importada del juego?**

El contador *Basura importada el mes pasado* del juego indica lo que se entregó realmente **durante el mes anterior**.

Permanece sin cambios durante todo el mes en curso.

No debe confundirse con la cantidad que están entregando en ese momento los contratos activos.

#### Cómo afecta la financiación de la limpieza al tratamiento de la basura

A diferencia de la electricidad, la limpieza tiene **un único porcentaje de financiación para todo el departamento**.

Este porcentaje afecta a la capacidad de las instalaciones de tratamiento de basura y a la cantidad de basura que pueden admitir los vertederos.

El juego original normalmente permite una financiación **del 0% al 120%**, salvo que el departamento tenga otro máximo o una financiación fija.

Sin embargo, **la capacidad de tratamiento de basura no aumenta en proporción directa a la financiación**.

SimCity 4 usa la misma curva de financiación a rendimiento que sus centrales eléctricas.

**Rendimiento de la limpieza (juego base)**

```math
e = \frac{f(p)}{100}
```

Donde:

- **p** es el porcentaje de financiación de la limpieza.
- **f(p)** es el rendimiento que da la curva del juego.
- **e** es el rendimiento resultante, expresado como fracción.

La curva original tiene estos puntos de control:

| Financiación de la limpieza | Rendimiento del tratamiento |
|---:|---:|
| 0% | 0% |
| 50% | 25% |
| 90% | 80% |
| 100% | 100% |
| 120% | 110% |

Los valores intermedios se calculan mediante interpolación lineal, como se explicó en los capítulos anteriores.

**Ejemplos de rendimiento de la limpieza**

| Financiación | Rendimiento del tratamiento |
|---:|---:|
| 50% | 25% |
| 60% | 38,75% |
| 61% | 40,125% |
| 75% | 59,375% |
| 90% | 80% |
| 100% | 100% |
| 110% | 105% |
| 120% | 110% |

**¿Qué significa esto para tu ciudad?**

Con una **financiación del 50%**, tu departamento de limpieza ofrece solo el **25% de su capacidad normal de tratamiento**.

Con una **financiación del 90%**, ofrece el 80%.

Con una **financiación del 120%**, ofrece el 110%, a pesar de costar un 20% más de lo normal.

La relación es especialmente importante entre el 50% y el 90%, donde cada punto adicional de financiación aumenta el rendimiento en 1,375 puntos porcentuales.

**¿Qué efectos tiene el rendimiento?**

El rendimiento de la limpieza cambia:

- La capacidad de tratamiento de las incineradoras.
- La capacidad de procesamiento de basura de las plantas de conversión de residuos en energía.
- La cantidad de basura que los vertederos pueden admitir durante un mes.

**No** cambia:

- La cantidad entregada por los contratos regionales de basura.
- La cantidad eliminada por los centros de reciclaje.

**¿Cambia la capacidad inmediatamente después de ajustar la financiación?**

No del todo.

El porcentaje de financiación cambia de inmediato, pero el rendimiento de limpieza calculado por el juego y las capacidades de tratamiento se actualizan a lo largo de los días simulados siguientes.

Por eso el Ministerio debe tener en cuenta estos retrasos al evaluar si un cambio de financiación ha surtido efecto.

#### Incineradoras y plantas de conversión de residuos en energía

SimCity 4 calcula cada día simulado la capacidad de tratamiento disponible de las incineradoras y de las plantas de conversión de residuos en energía.

Para cada instalación, esta capacidad ya tiene en cuenta:

- La capacidad máxima de tratamiento del edificio.
- El rendimiento actual de la limpieza.
- El estado propio del edificio y sus modificadores de capacidad.

Después, el juego proporciona la capacidad de tratamiento combinada de cada tipo de instalación.

**El Ministerio lee estos valores directamente.**

No los multiplica otra vez por el rendimiento de la limpieza, porque el juego ya ha aplicado ese ajuste.

Así se evita contar dos veces el efecto de la financiación.

**¿Y la electricidad generada a partir de la basura?**

Las plantas de conversión de residuos en energía también producen electricidad.

Su función de tratamiento de basura pertenece al sistema de limpieza, mientras que su producción eléctrica se considera por separado en el capítulo de [Electricidad](power.md#power).

<a id="garbage-landfills-valve"></a>
#### ¿Cuánta basura puede admitir un vertedero?

Esta es una de las mecánicas más importantes de SimCity 4.

**Tener espacio libre en un vertedero no significa que pueda admitir basura ilimitada cada mes.**

El juego aplica un límite mensual de admisión independiente, que el Ministerio modela como una **válvula del vertedero**.

Piensa en ello como dos restricciones distintas:

- **Capacidad de almacenamiento:** cuánta basura puede contener el vertedero en total.
- **Capacidad de admisión:** cuánta basura nueva se le puede enviar durante el mes en curso.

Un vertedero puede tener mucho espacio de almacenamiento vacío y aun así no poder admitir más basura ese mes.

**Admisión diaria del vertedero (juego base)**

```math
\text{taken} =
\min\left(
G,\ F,\ \operatorname{round}(5000 \times L \times e)-S
\right)
```

La cantidad realmente admitida no puede ser negativa.

Donde:

- **G** es la basura que aún espera su eliminación.
- **F** es el espacio de almacenamiento libre restante.
- **L** es el número de casillas de vertedero que cuenta el juego.
- **e** es el rendimiento actual de la limpieza.
- **S** es la cantidad ya enviada a través de la válvula del vertedero durante el mes en curso.
- **5.000** es la capacidad base de admisión mensual por casilla de vertedero.

Cada vez que entra basura en el vertedero, se suma al total acumulado mensual de la válvula.

**En pocas palabras:**

El juego limita la admisión del vertedero según el número de casillas de vertedero, la financiación de la limpieza y la cantidad de basura que ya se ha admitido ese mes.

**Ejemplo: un vertedero de 48 casillas**

Supongamos que la ciudad tiene:

- **48 casillas de vertedero.**
- **50% de financiación de la limpieza.**
- **25% de rendimiento de la limpieza.**

El límite mensual de admisión es aproximadamente:

```math
5000 \times 48 \times 0.25 = 60000
```

**El vertedero puede admitir aproximadamente 60.000 unidades de basura ese mes**, suponiendo que tenga suficiente almacenamiento libre y ningún arrastre previo en la válvula.

Con una financiación del 100%, el mismo vertedero tiene una admisión mensual teórica de:

```math
5000 \times 48 \times 1.00 = 240000
```

Son **240.000 unidades al mes**.

<text strong>Comparación</text>

| Financiación | Rendimiento | Admisión mensual aproximada del vertedero |
|---|---:|---:|
| 50% | 25% | 60.000 unidades |
| 100% | 100% | 240.000 unidades |

**La superficie del vertedero no ha cambiado. Solo el presupuesto de limpieza.**

Por eso reducir demasiado la financiación de la limpieza puede generar basura sin recoger incluso cuando tus vertederos aún tienen mucho espacio vacío.

<a id="garbage-landfills-rollover"></a>
#### ¿Qué ocurre con la basura del vertedero a final de mes?

Hay otra distinción importante:

**La basura que entra en un vertedero durante un mes no es lo mismo que la basura almacenada allí de forma permanente.**

SimCity 4 procesa el almacenamiento de los vertederos en cada cambio de mes simulado.

Ocurren dos cosas:

1. Parte de la basura ya almacenada en el vertedero desaparece gradualmente por el mecanismo de descomposición del juego.
2. La basura acumulada en la válvula de admisión mensual se transfiere al almacenamiento del vertedero, sujeta a los límites del juego.

Esto significa que el almacenamiento del vertedero y la admisión mensual deben considerarse por separado.

#### 1. La basura almacenada se descompone gradualmente

SimCity 4 elimina una pequeña cantidad de basura almacenada de cada celda del vertedero en cada mes simulado.

La cantidad depende de lo llena que esté la celda.

**Descomposición mensual de basura por celda de vertedero (juego base)**

```math
d =
\min\left(
\text{stock},
\max\left(
1,
\min\left(
1500,
\operatorname{round}(0.07 \times \text{stock})
\right)
\right)
\right)
```

Donde:

- **stock** es la basura almacenada actualmente en una celda del vertedero.
- **d** es la cantidad que desaparece durante la actualización mensual.

Una celda vacía no pierde nada.

En las celdas ocupadas, el juego normalmente elimina aproximadamente el **7% de la basura almacenada**, con un mínimo de una unidad y un máximo de 1.500 unidades por celda y mes.

**Ejemplo**

Imagina que una celda del vertedero contiene 10.000 unidades de basura.

```math
10000 \times 0.07 = 700
```

El juego elimina **700 unidades** durante la actualización mensual.

Si una celda contiene 30.000 unidades:

```math
30000 \times 0.07 = 2100
```

Pero la descomposición máxima es de 1.500 unidades.

Por tanto, el juego elimina **1.500 unidades**, no 2.100.

En las ciudades de prueba, una celda de vertedero completamente llena contenía aproximadamente 40.000 unidades y perdía exactamente 1.500 unidades cada mes.

**¿Cómo se calcula la descomposición de un vertedero entero?**

El juego almacena la basura de forma compacta: primero las celdas llenas, después una celda parcialmente llena y luego las celdas vacías.

Por eso el Ministerio puede calcular la descomposición total a partir de la cantidad de basura almacenada y la capacidad de cada celda.

**Descomposición mensual total del vertedero (juego base)**

```math
D(U) =
\left\lfloor\frac{U}{C}\right\rfloor d(C)
+
d(U \bmod C)
```

Donde:

- **U** es la cantidad total de basura almacenada.
- **C** es la capacidad de una celda de vertedero.
- **d(C)** es la descomposición mensual de una celda llena.
- **U mod C** es la basura que queda en la última celda parcialmente llena.

La primera parte calcula la descomposición de las celdas llenas. La segunda calcula la de la celda parcialmente llena, si existe.

Este modelo reprodujo exactamente la descomposición registrada por el juego a lo largo de **11 actualizaciones mensuales consecutivas** en una ciudad de prueba.

**¿Por qué importa esto?**

Los vertederos no se llenan simplemente para siempre a un ritmo constante.

Parte de los residuos almacenados desaparece cada mes.

El Ministerio lo tiene en cuenta al estimar cuánto espacio de vertedero hay disponible y con qué rapidez puede llenarse.

#### 2. La admisión mensual del vertedero pasa a ser basura almacenada

En la actualización mensual, SimCity 4 transfiere basura de la válvula de admisión a los residuos almacenados del vertedero.

Sin embargo, **la válvula no siempre se vacía por completo**.

La cantidad absorbida depende de:

- La basura que espera en la válvula.
- El espacio de almacenamiento disponible del vertedero.
- El número de casillas de vertedero.
- La financiación de la limpieza que se proporciona realmente.

**Absorción mensual del vertedero (juego base)**

```math
\begin{aligned}
H &= \min(S,\ 327\,680\,000) \\[4pt]
\text{si } A < B:\quad
H &= \left\lfloor\frac{H\times A}{B}\right\rfloor \\[4pt]
\text{absorbed} &=
\min\left(
H,\ 5000\times L,\ T_{\mathrm{cap}}-(U-D(U))
\right) \\[4pt]
U' &= U-D(U)+\text{absorbed} \\
S' &= S-\text{absorbed}
\end{aligned}
```

Donde:

- **S** es la basura que espera en la válvula mensual del vertedero.
- **A** es el importe de mantenimiento del vertedero realmente financiado.
- **B** es el coste de mantenimiento del vertedero con financiación del 100%.
- **L** es el número de casillas de vertedero.
- **Tcap** es la capacidad total de almacenamiento del vertedero.
- **U** es la cantidad ya almacenada antes de la actualización mensual.
- **D(U)** es la basura eliminada por descomposición mensual.
- **absorbed** es la basura transferida al almacenamiento.
- **U′** es el nuevo total de basura almacenada.
- **S′** es la cantidad que queda en la válvula para el mes siguiente.

El cálculo del juego también impone un límite superior de **327.680.000 unidades** a la cantidad de la válvula que se considera para la absorción.

No necesitas calcular esto a mano. Lo importante es cómo afecta la financiación al resultado.

**Con una financiación de la limpieza del 100%**, no se aplica la reducción relacionada con la financiación. La válvula puede absorberse por completo cuando el almacenamiento suficiente y los demás límites lo permiten.

**Por debajo del 100% de financiación**, el juego puede transferir solo una parte de la válvula al almacenamiento permanente.

El resto permanece en la válvula y se arrastra al mes siguiente.

#### ¿Por qué es un problema la basura sobrante en la válvula?

Recuerda que el vertedero tiene un límite mensual de admisión.

Si parte de la válvula del mes anterior sigue ocupada, el vertedero empieza el mes nuevo con menos capacidad de admisión disponible.

Esto crea una segunda consecuencia de la financiación baja de la limpieza:

1. **Una financiación menor reduce la cantidad de basura que puede entrar en el vertedero durante el mes.**
2. **Una financiación menor también puede reducir la cantidad de basura que se absorbe en el almacenamiento en el cambio de mes**, dejando ocupada parte de la admisión permitida.

**Por tanto, una financiación baja de la limpieza puede restringir el rendimiento del vertedero de dos maneras distintas.**

Por eso mirar solo el almacenamiento disponible del vertedero da una imagen incompleta del sistema de limpieza.

Un vertedero puede estar lejos de llenarse y aun así tener dificultades para gestionar la basura de la ciudad.

#### ¿Cuándo actualiza el juego las estadísticas del vertedero?

El juego publica la cantidad de basura absorbida en el almacenamiento del vertedero en la actualización mensual siguiente, mediante su contador *Basura depositada en vertederos*.

Por tanto, este valor está ligado al proceso mensual de almacenamiento, y no simplemente a la basura que llega a los vertederos durante el día en curso.

**La distinción clave**

| Concepto | Qué mide | Principal limitación |
|---|---|---|
| **Basura producida** | Residuos generados por los edificios de la ciudad | Actividad de la ciudad y reciclaje |
| **Capacidad de tratamiento** | Residuos que pueden procesar las incineradoras y las plantas de conversión de residuos en energía | Financiación de la limpieza y estado de los edificios |
| **Admisión del vertedero** | Basura nueva que los vertederos pueden admitir durante el mes | Casillas de vertedero, rendimiento y arrastre de la válvula |
| **Almacenamiento del vertedero** | Basura acumulada dentro de las celdas del vertedero | Espacio de almacenamiento disponible y descomposición mensual |
| **Absorción mensual** | Basura transferida de la válvula al almacenamiento | Financiación, tamaño del vertedero y espacio restante |

**El Ministerio debe tener en cuenta todos estos mecanismos en lugar de limitarse a comparar la producción de basura con el espacio vacío de los vertederos.**
<a id="garbage-landfills-cells"></a>
#### Cómo funcionan las zonas de vertedero: casillas y celdas de almacenamiento

En SimCity 4, un vertedero es más complicado que la zona que dibujas en el mapa.

El juego usa dos elementos distintos:

- **Casillas de vertedero:** las casillas individuales que zonificas para la eliminación de basura.
- **Celdas de almacenamiento:** zonas invisibles donde el juego almacena realmente la basura acumulada.

Cada celda de almacenamiento corresponde a un **grupo de 2 × 2 casillas de vertedero**.

Una celda solo puede almacenar basura cuando sus cuatro casillas pertenecen a la zona de vertedero.

**¿Cuántas celdas de almacenamiento tiene un vertedero?**

El juego registra cada vertedero dentro de un área rectangular, llamada *rectángulo delimitador*.

Este rectángulo también contiene información que identifica qué casillas pertenecen realmente al vertedero.

Para un rectángulo de **w casillas de ancho y h casillas de alto**, la cuadrícula posible de celdas de almacenamiento contiene:

```math
N_{\text{possible}}=(w-1)(h-1)
```

Sin embargo, solo están activas las celdas cuyas cuatro casillas forman parte del vertedero.

<box gap={2}>
**Ejemplo: un vertedero de 3 × 3**

<svg viewBox="0 0 340 155" width="100%" xmlns="http://www.w3.org/2000/svg">
  {#each [0,1,2] as r}
    {#each [0,1,2] as c}
      <rect x={22+c*34} y={16+r*34} width="32" height="32" rx="2" fill="#8fba7b" stroke="#45683e" strokeWidth="1"/>
    {/each}
  {/each}
  {#each [0,1] as r}
    {#each [0,1] as c}
      <rect x={178+c*34} y={33+r*34} width="32" height="32" rx="2" fill="#d9b477" stroke="#956b35" strokeWidth="1"/>
    {/each}
  {/each}
  <text x="73" y="136" textAnchor="middle" fill="currentColor" fontSize="12">9 casillas de vertedero</text>
  <text x="228" y="136" textAnchor="middle" fill="currentColor" fontSize="12">4 celdas de almacenamiento</text>
  <path d="M138 65 L162 65 M156 60 L162 65 L156 70" fill="none" stroke="currentColor" strokeWidth="1.5"/>
</svg>
<caption>El diagrama de la derecha muestra las posiciones posibles de las celdas de almacenamiento, no una segunda zona de vertedero.</caption>
</box>

Un vertedero de 3 × 3 totalmente zonificado tiene **9 casillas pero 4 celdas de almacenamiento superpuestas**.

Un vertedero de 2 × 2 tiene una sola celda de almacenamiento.

Esta distinción importa porque **el mantenimiento del vertedero se calcula por casillas, mientras que el almacenamiento de basura se organiza en celdas**.

#### ¿Cuánta basura puede almacenar un vertedero?

El juego asigna capacidad de almacenamiento a cada celda activa.

Esa capacidad depende de:

- El número de casillas que pertenecen a la zona de vertedero.
- El número de celdas de almacenamiento activas.

**Capacidad de almacenamiento del vertedero (juego base)**

```math
C = \operatorname{round}\left(\frac{T\times30000}{N}\right)
\qquad
T_{\mathrm{cap}}=N\times C
```

Donde:

- **T** es el número de casillas registradas en la zona de vertedero, incluidas las casillas fantasma (que se explican más abajo).
- **N** es el número de celdas de almacenamiento activas.
- **30.000** es la capacidad de almacenamiento del juego por casilla de vertedero.
- **C** es la capacidad asignada a cada celda activa.
- **Tcap** es la capacidad total de almacenamiento del vertedero.

El juego redondea la capacidad de la celda a la unidad entera más próxima, y los valores justo a medio camino se redondean al entero par más próximo.

**Ejemplos**

| Vertedero | Celdas activas | Capacidad por celda | Capacidad total |
|---|---:|---:|---:|
| Vertedero de 2 × 2 | 1 | 120.000 | 120.000 |
| Vertedero de 129 casillas | 70 | 55.286 | 3.870.020 |

En general, un vertedero ofrece aproximadamente **30.000 unidades de almacenamiento por casilla zonificada**.

El total exacto puede diferir ligeramente porque la capacidad se redondea por separado para cada celda.

La capacidad total de vertederos de la ciudad es la suma de las capacidades de todas sus zonas de vertedero.

#### ¿Qué ocurre cuando amplías o reduces un vertedero?

Aquí es donde la estructura interna de celdas del juego cobra importancia.

**Ampliar un vertedero**

Cuando añades casillas nuevas de vertedero, SimCity 4 reconstruye la cuadrícula de celdas de almacenamiento de la zona.

Las celdas existentes normalmente conservan su basura almacenada, mientras que las celdas válidas nuevas empiezan vacías.

Sin embargo, **la dirección en la que amplías puede afectar a la basura almacenada**.

El juego identifica las celdas de almacenamiento por sus posiciones relativas a la esquina del rectángulo delimitador del vertedero.

- **Ampliar hacia la derecha o hacia abajo:** la esquina original se queda en su sitio, de modo que las celdas existentes normalmente conservan sus posiciones.
- **Ampliar hacia la izquierda o hacia arriba:** la esquina se mueve y desplaza las posiciones relativas de las celdas existentes. Esto puede hacer que la basura almacenada del juego deje de corresponder a las ubicaciones previstas.

Esta es la causa de un problema conocido de los vertederos de SimCity 4 que a veces aparece después de ampliar un vertedero.

**Lo más seguro es ampliar los vertederos hacia la derecha o hacia abajo de su área existente, según las coordenadas del mapa del juego.**

La ampliación también puede cambiar la cantidad de basura que puede contener cada celda.

Si la nueva capacidad de una celda pasa a ser menor que la cantidad ya almacenada allí, **el exceso de basura se devuelve a la válvula de admisión del vertedero**.

Esa basura debe volver a procesarse a través del sistema del vertedero.

Una operación de zonificación también hace que el juego compruebe la información de pertenencia de la zona y elimine las casillas que ya no están realmente zonificadas como vertedero.

**Reducir un vertedero**

Quitar casillas de vertedero se comporta de otra manera.

Cuando quitas la zonificación de una casilla, el juego:

1. La elimina del área registrada del vertedero.
2. Desactiva las celdas de almacenamiento afectadas.
3. Recalcula la capacidad de las celdas activas restantes.
4. Devuelve a la válvula de admisión el exceso de basura almacenada.

A diferencia de la ampliación, **quitar la zonificación no reconstruye toda la cuadrícula de celdas de almacenamiento ni concilia por completo la información de pertenencia de la zona**.

El juego también impide ciertos cambios:

- No se puede quitar la zonificación de una casilla que contiene un montón de basura.
- Las zonas de vertedero deben respetar las restricciones de tamaño del juego, incluida una anchura mínima de dos casillas y una máxima de 48 casillas.

**¿Por qué importa esto?**

Cambiar la forma de un vertedero puede afectar a algo más que su superficie visible.

También puede cambiar la cantidad de basura que pueden contener las celdas y la que se devuelve al sistema de admisión mensual.

El Ministerio tiene en cuenta estos efectos al evaluar los cambios de tamaño del vertedero.

#### ¿Cuánto cuesta un vertedero?

El mantenimiento del vertedero se calcula **casilla por casilla**.

Las casillas de vertedero vacías tienen un coste de mantenimiento fijo, mientras que las casillas con montones de basura cuestan más.

**Mantenimiento del vertedero con financiación del 100% (juego base)**

```math
B=\sum_{\text{casillas de la zona}}c_i
\qquad
c_i=
\begin{cases}
\text{coste de mantenimiento del montón}, & \text{si hay un montón de basura}\\
5, & \text{en otro caso}
\end{cases}
```

Donde:

- **B** es el coste mensual de mantenimiento del vertedero con financiación del 100%.
- **cᵢ** es el coste de mantenimiento de una casilla individual.

En las ciudades examinadas, los costes observados fueron:

| Tipo de casilla | Mantenimiento mensual al 100% |
|---|---:|
| Casilla de vertedero vacía | §5 |
| Casilla con un montón de basura | §20, §30 o §40, según el montón |

**Un vertedero se encarece a medida que se acumula la basura.**

Por ejemplo, un vertedero con 100 casillas vacías cuesta:

```math
100\times5=500
```

Son **§500 al mes con financiación del 100%**, incluso antes de que empiecen a aparecer montones de basura.

Si 20 de esas casillas desarrollan montones de basura que cuestan §20 cada uno, mientras las 80 restantes siguen vacías:

```math
(80\times5)+(20\times20)=800
```

El mantenimiento mensual sube a **§800**.

Por eso el tamaño del vertedero importa económicamente.

Mantener una gran superficie de vertedero sin usar puede generar gastos de mantenimiento innecesarios.

El Ministerio puede identificar situaciones en las que reducir la zonificación sin usar del vertedero ahorraría dinero, siempre que el cambio sea seguro.

#### Casillas fantasma: zonas de vertedero que no deberían existir

SimCity 4 tiene un error que puede dejar **casillas fantasma de vertedero**.

Una casilla fantasma es una ubicación que el juego sigue contando como parte de un vertedero aunque ya no esté zonificada como vertedero.

Por ejemplo, la casilla puede contener ahora:

- Una carretera normal.
- Un terreno sin zonificar.
- Otra zona que no es de vertedero.

Visualmente, el vertedero ya no está en esa casilla.

**Internamente, sin embargo, el juego sigue contándola como parte de la zona de vertedero.**

Esto tiene dos consecuencias:

1. **La casilla sigue costando §5 al mes** en mantenimiento del vertedero con financiación del 100%.
2. **Se incluye en el recuento de casillas registrado de la zona** cuando se recalcula la capacidad de almacenamiento del vertedero.

Las casillas fantasma no cuentan para el total de casillas activas de vertedero que se usa para calcular el límite mensual de admisión.

Es decir, **el juego puede cobrarte por casillas de vertedero que en realidad no ayudan a eliminar basura**.

#### ¿Por qué vuelven las casillas fantasma al reabrir una ciudad?

El problema viene de cómo SimCity 4 guarda y recarga las zonas de vertedero.

Al guardar un vertedero, el juego registra su rectángulo delimitador y qué casillas pertenecen a la zona.

Sin embargo, no guarda correctamente la pertenencia de las casillas de la última fila y la última columna del rectángulo.

Cuando se reabre la ciudad, el juego supone inicialmente que todas las casillas dentro del rectángulo pertenecen al vertedero y después restaura la información de pertenencia guardada.

Como la última fila y la última columna tienen información guardada incompleta, **las casillas de esos bordes pueden volver a ser miembros del vertedero por error**.

Esto crea dos tipos de casillas fantasma:

| Ubicación de la casilla fantasma | Qué ocurre tras la reparación |
|---|---|
| **Dentro del rectángulo de la zona** | Una operación de zonificación adecuada puede eliminarla de forma permanente, porque se guarda su pertenencia corregida. |
| **En la última fila o columna** | Puede volver al reabrir la ciudad, porque el juego no guarda correctamente la pertenencia de ese borde. |

Las herramientas de zonificación del juego original no pueden eliminar de forma permanente todas las casillas fantasma del borde, porque el rectángulo delimitador registrado de la zona no se encoge.

**¿Cómo ayuda el Ministerio?**

El Ministerio incluye un mecanismo automático de reparación de casillas fantasma.

Cuando detecta casillas fantasma elegibles, usa el propio sistema de zonificación del juego para corregirlas, en lugar de modificar directamente el archivo de guardado.

La reparación está diseñada para no perturbar las zonas de vertedero legítimas ni la basura almacenada.

**No hay un ajuste independiente para la reparación de casillas fantasma.** Funciona como parte de la gestión automática de la limpieza.

Sin embargo, el error de guardado y carga subyacente sigue existiendo en SimCity 4. Por eso, algunas casillas fantasma del borde pueden volver tras reabrir una ciudad y requerir otra corrección.

<a id="garbage-landfills-problem"></a>
### 2. El problema de la gestión

Gestionar la basura en SimCity 4 es más complicado que simplemente proporcionar suficiente espacio de vertedero.

El Ministerio debe tener en cuenta varios problemas.

- **La basura llega en momentos distintos:** tu ciudad produce basura a diario, mientras que los contratos regionales de basura suelen entregar la mayor parte de sus cantidades mensuales durante los primeros diez días más o menos.
- **El almacenamiento del vertedero no es lo mismo que la capacidad de eliminación:** un vertedero puede tener mucho espacio vacío y aun así alcanzar su límite mensual de admisión.
- **La financiación baja afecta a la eliminación de basura de varias maneras:** reduce la capacidad de tratamiento y puede dejar ocupada parte de la válvula de admisión del vertedero cuando empieza el mes siguiente.
- **Las estadísticas del juego pueden ir con retraso:** las cifras de basura importada se refieren al mes anterior, y algunos valores de contratos y de limpieza tardan en actualizarse.
- **La curva de financiación no es proporcional:** reducir demasiado la financiación de la limpieza puede eliminar mucha más capacidad de tratamiento de la esperada.
- **Las zonas de vertedero sin usar cuestan dinero:** las casillas vacías tienen gastos de mantenimiento mensuales, y las casillas fantasma pueden generar cargos adicionales.
- **Cambiar la forma del vertedero puede afectar a la basura almacenada:** ampliar o reducir un vertedero puede redistribuir la capacidad de almacenamiento y devolver el exceso de basura al sistema de admisión.

**El Ministerio debe considerar todos estos factores antes de decidir cuánta financiación de la limpieza necesita tu ciudad.**

No puede optimizar la limpieza de forma segura mirando solo la producción actual de basura o la cantidad de espacio libre en los vertederos.

<a id="garbage-landfills-algorithm"></a>
### 3. Cómo gestiona el Ministerio la basura

El Ministerio usa las estadísticas de basura del juego para estimar la cantidad de residuos que debe gestionar tu ciudad.

Después considera las instalaciones de tratamiento de basura, la capacidad de los vertederos, la acumulación actual de residuos y los contratos regionales al elegir un presupuesto de limpieza adecuado.

El objetivo es **eliminar la basura que la ciudad necesita eliminar, mantener una reserva de seguridad y evitar gastos mensuales innecesarios**.

<a id="garbage-landfills-need"></a>
#### 3.1 ¿Cuánta basura debe gestionar la ciudad?

El Ministerio empieza calculando cuánta basura produce tu propia ciudad después del reciclaje.

**Paso 1: calcular la basura propia de la ciudad**

```math
G_{\mathrm{own}}=\max(0,P-R)
```

Donde:

- **P** es la basura producida durante el mes simulado anterior.
- **R** es la basura reciclada durante ese mes.
- **Gown** es la basura que queda por eliminar.

Por ejemplo, supongamos que tu ciudad produjo 100.000 unidades de basura el mes pasado y recicló 20.000.

```math
G_{\mathrm{own}}=100000-20000=80000
```

Por tanto, el Ministerio estima que la ciudad genera **80.000 unidades de basura al mes que requieren eliminación**.

**La basura importada no se incluye en este cálculo.**

Las estadísticas de basura importada del juego se refieren a las entregas del mes anterior.

El Ministerio trata la basura ajena por separado, usando los contratos regionales reales en lugar de mezclar los residuos importados con la producción propia de la ciudad.

#### Paso 2: añadir la reserva de seguridad

La producción de basura puede aumentar a medida que crece la ciudad.

Para evitar elegir un presupuesto que solo basta para la cantidad exacta del mes anterior, el Ministerio añade una reserva de seguridad.

La **reserva para tratar basura** por defecto es del 2%, ajustable del 2% al 20%.

**Producción protegida de basura**

```math
G_{\mathrm{prot}}=
\left\lceil
G_{\mathrm{own}}\left(1+\frac{r}{100}\right)
\right\rceil
```

Donde:

- **Gown** es la basura propia de la ciudad después del reciclaje.
- **r** es la reserva de limpieza configurada.
- **Gprot** es la cantidad de basura protegida, redondeada al alza.

**Ejemplo: una ciudad que produce 75.464 unidades de basura tras el reciclaje**

Con la reserva por defecto del 2%:

```math
G_{\mathrm{prot}}=
\lceil75464\times1.02\rceil
=76974
```

Por tanto, el Ministerio planifica para **76.974 unidades** en lugar de exactamente 75.464.

La reserva es un mínimo, no un máximo. El Ministerio puede necesitar gestionar más basura por la acumulación de residuos existente o por los contratos regionales.

#### Paso 3: incluir la basura acumulada y las importaciones

Es posible que la ciudad ya tenga basura esperando a ser recogida.

También puede recibir residuos de ciudades vecinas.

El Ministerio suma estas cantidades para obtener una necesidad mensual global de gestión.

**Necesidad mensual de gestión de basura**

```math
H=G_{\mathrm{prot}}+Q+\sum\text{importaciones}
```

Donde:

- **Gprot** es la producción mensual protegida de basura de la ciudad.
- **Q** es la basura sin recoger acumulada actualmente en la cuadrícula de la ciudad.
- **Las cantidades importadas** son las cantidades de basura que especifican los contratos regionales de importación.
- **H** es la necesidad mensual global de gestión.

**Ejemplo**

Supongamos que la ciudad tiene:

| Origen | Unidades de basura |
|---|---:|
| Basura propia, incluida la reserva | 76.974 |
| Basura ya sin recoger | 5.000 |
| Basura importada de los vecinos | 20.000 |
| **Necesidad total de gestión** | **101.974** |

Por tanto, el Ministerio debe considerar **101.974 unidades** de residuos al planificar la limpieza.

Sin embargo, **este total por sí solo no basta para decidir si un nivel de financiación funcionará**.

La basura llega y se procesa en momentos distintos a lo largo del mes.

Una ciudad puede tener capacidad total mensual de tratamiento suficiente y aun así sufrir una acumulación temporal de basura porque las importaciones llegan pronto o porque la admisión del vertedero está limitada temporalmente.

Por eso, el Ministerio usa este total para su resumen y sus registros mensuales, pero toma la decisión final mediante una **simulación día a día de la eliminación de basura**.

Esa simulación considera cuándo llega la basura, qué instalaciones la procesan, cómo se comporta la válvula del vertedero y si quedan residuos sin recoger.

Las secciones siguientes explican cómo funciona esta predicción.
<a id="garbage-landfills-uncollected"></a>
#### 3.2 ¿Qué ocurre con la basura sin recoger?

A veces tu ciudad produce más basura de la que su sistema de eliminación puede gestionar.

El exceso permanece en la cuadrícula de la ciudad como **basura sin recoger**.

El Ministerio lee la cantidad total directamente del juego y la trata como un **atraso**, representado por **Q**.

**El objetivo es eliminar este atraso antes de que termine el mes simulado en curso**, además de gestionar la basura producida durante ese mes.

Por ejemplo, si tu ciudad ya tiene 10.000 unidades de basura sin recoger, el Ministerio incluye esas 10.000 unidades en sus cálculos.

No ignora la basura existente solo porque la producción normal de la ciudad esté cubierta.

**¿Y la basura que el juego elimina automáticamente?**

Como se explicó antes, SimCity 4 tiene un mecanismo oculto que puede eliminar en silencio una cantidad limitada de basura excedente.

El Ministerio no trata esta eliminación como un fallo de la limpieza ni la usa como motivo para bloquear reducciones de financiación.

Solo contribuye al atraso la basura que realmente permanece sin recoger en la cuadrícula de la ciudad.

<a id="garbage-landfills-contracts"></a>
#### 3.3 Cómo se incluyen los contratos de basura

Los contratos de basura son más complicados de lo que parecen porque **sus cantidades mensuales se entregan a lo largo de varios días**, y no de golpe ni de manera uniforme durante todo el mes.

El Ministerio calcula las entregas de **cada contrato por separado**.

No se limita a sumar todas las cantidades de los contratos y suponer que llegan de forma uniforme.

#### Importaciones de basura

Cuando otra ciudad te envía basura, el Ministerio supone que el juego entrega aproximadamente el **10% de la cantidad mensual de ese contrato cada día**, hasta que se agota la asignación mensual.

Por ejemplo, un contrato para importar 20.000 unidades entrega aproximadamente 2.000 unidades al día durante los primeros diez días del mes.

El Ministerio también tiene en cuenta el redondeo. Como la última entrega del juego puede superar ligeramente la cantidad contratada, no subestima la basura entrante.

**La basura importada siempre se trata como residuos adicionales que tu ciudad debe eliminar.**

Una importación suspendida se sigue contando de forma conservadora al evaluar los residuos que la ciudad podría tener que gestionar.

#### Exportaciones de basura

Cuando pagas a otra ciudad para que se lleve tu basura, el contrato reduce la cantidad que deben procesar tus propias instalaciones de limpieza.

El Ministerio calcula este alivio con el mismo calendario de entrega diaria.

Sin embargo, **nunca supone que se pueda exportar más basura de la que el contrato permite realmente**.

Por ejemplo, un contrato de exportación de 20.000 unidades proporciona aproximadamente 2.000 unidades de alivio de eliminación cada día hasta que se agota la asignación mensual.

A diferencia de las importaciones, el último crédito diario de exportación se limita a la cantidad que aún queda disponible.

**¿Y si un contrato de exportación está suspendido?**

El Ministerio no cuenta con una exportación suspendida para eliminar basura.

Tampoco da ningún crédito de eliminación cuando el uso registrado del contrato por el juego no se puede leer de forma fiable.

El contrato sigue siendo una obligación existente, pero el Ministerio no supone que vaya a proporcionar capacidad de eliminación utilizable.

Esto evita que el plan de limpieza dependa de exportaciones que quizá no se produzcan.

#### Un problema con las estadísticas de contratos del juego

Durante los primeros días simulados de un mes nuevo, SimCity 4 puede mostrar temporalmente la cantidad usada durante el **mes anterior**.

Por ejemplo, imagina un contrato de exportación de 7.000 unidades.

El primer día de un mes nuevo, el juego podría seguir indicando:

**Cantidad usada: 7.000 unidades**

Eso no significa necesariamente que el contrato del mes nuevo ya se haya agotado.

El Ministerio detecta esta situación durante los **tres primeros días simulados**.

Si el uso notificado es mayor de lo que el contrato podría haber entregado como máximo desde el comienzo del mes, el Ministerio considera esa lectura obsoleta y empieza el cálculo del mes desde cero.

**Prueba de lectura obsoleta**

```math
U > (\text{día del mes}-1)\times\operatorname{round}(0.1\times A)
```

Donde:

- **A** es la cantidad mensual del contrato.
- **U** es la cantidad que el juego indica como ya usada este mes.

Esto evita que las estadísticas obsoletas provoquen decisiones de financiación incorrectas.

#### ¿Cuándo compensa importar basura?

Importar basura genera dinero de la ciudad vecina, pero también crea más trabajo para tu sistema de limpieza.

El Ministerio solo considera aceptable una importación cuando la financiación de limpieza elegida puede gestionar la basura adicional **sin dejar un atraso al final del mes**.

A veces los ingresos adicionales pueden justificar un aumento de la financiación de la limpieza.

Por ejemplo, aceptar basura importada puede exigir un presupuesto de limpieza mayor, pero aun así producir un beneficio neto si el vecino paga lo suficiente.

El gestor de contratos compara estos costes e ingresos antes de tomar sus decisiones.

Consulta [Acuerdos con vecinos](deals.md#neighbor-deals) para ver las reglas completas de los contratos.

<a id="garbage-landfills-projection"></a>
#### 3.4 Simulación de los días que quedan del mes

Esta es la parte más importante de los cálculos de limpieza del Ministerio.

**En lugar de comparar solo los totales mensuales, el Ministerio simula cómo se gestionará la basura en cada uno de los días que quedan del mes.**

¿Por qué?

Porque la basura llega y se elimina en momentos distintos.

- Tu ciudad produce basura todos los días.
- Las importaciones suelen llegar durante los primeros diez días, más o menos.
- Las exportaciones alivian la eliminación según sus propios calendarios diarios.
- Las incineradoras y las centrales de conversión de residuos en energía tienen una capacidad de tratamiento limitada.
- Los vertederos tienen un límite mensual de admisión que se va agotando progresivamente.

Una ciudad puede tener capacidad mensual teórica de sobra y, aun así, sufrir una acumulación temporal de basura.

El Ministerio debe determinar si esa basura acumulada se puede eliminar antes de que acabe el mes.

#### Cómo funciona la simulación diaria

Para cada posible porcentaje de financiación de la limpieza, el Ministerio realiza el siguiente cálculo en cada uno de los días restantes.

<box gap={2}>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="trash-2" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **1. Sumar la basura que entra**

      Se parte de la basura que quedó del día anterior y se añaden la producción diaria estimada de la ciudad y las importaciones de ese día.
    </box>
  </row>
  <divider color="subtle"/>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="truck" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **2. Aplicar las exportaciones de basura**

      Se retira toda la basura que puedan absorber los contratos de exportación disponibles. El crédito de exportación sin usar puede pasar al día siguiente.
    </box>
  </row>
  <divider color="subtle"/>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="factory" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **3. Usar las instalaciones de tratamiento**

      Las incineradoras y las centrales de conversión de residuos en energía procesan basura hasta el límite de su capacidad diaria de tratamiento disponible.
    </box>
  </row>
  <divider color="subtle"/>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="layers" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **4. Usar la capacidad de los vertederos**

      La basura restante se envía a los vertederos, siempre que su válvula de admisión mensual tenga capacidad libre.
    </box>
  </row>
  <divider color="subtle"/>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="calendar-check" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **5. Trasladar la basura sin recoger**

      La basura que hoy no se puede gestionar queda en la acumulación pendiente para el día siguiente.
    </box>
  </row>
</box>

El Ministerio repite este proceso hasta el último día del mes.

**Un nivel de financiación se considera suficiente cuando la acumulación pendiente proyectada al final del mes es cero.**

Se permite una acumulación temporal durante el mes, siempre que se pueda eliminar antes de que termine.

El Ministerio no impone ningún límite artificial adicional a la acumulación temporal máxima.

**Proyección diaria de basura**

```math
\begin{aligned}
a_t &= Q_{t-1}+\frac{G_{\mathrm{prot}}}{n}+I_t\\
X_t &= R_{t-1}+c_t\\
x_t &= \min(a_t,X_t)\\
R_t &= X_t-x_t\\
b_t &= \min\left(a_t-x_t,\frac{K(p)}{n}\right)\\
l_t &= \min(a_t-x_t-b_t,V_{t-1})\\
V_t &= V_{t-1}-l_t\\
Q_t &= a_t-x_t-b_t-l_t
\end{aligned}
```

Donde:

- **n** es el número de días del mes simulado.
- **Gprot** es la producción mensual de basura protegida de la ciudad.
- **Q₀** es la acumulación actual de basura sin recoger.
- **Iₜ** es la basura importada el día *t*.
- **cₜ** es el crédito de exportación generado ese día.
- **Rₜ** es el crédito de exportación sin usar que se traslada al día siguiente.
- **aₜ** es la basura que hay que eliminar ese día.
- **xₜ** es la basura retirada mediante exportaciones.
- **bₜ** es la basura procesada por las instalaciones de tratamiento.
- **lₜ** es la basura enviada a los vertederos.
- **K(p)** es la capacidad mensual de tratamiento con la financiación **p**.
- **Vₜ** es la admisión restante permitida en los vertederos.
- **Qₜ** es la basura que sigue sin recoger al final del día *t*.

El Ministerio aproxima la producción de basura propia de la ciudad como repartida de forma uniforme a lo largo del mes.

Las importaciones y las exportaciones, en cambio, siguen los calendarios de entrega de cada contrato.

El cálculo se realiza para cada nivel de financiación candidato.

#### ¿Cómo se ajusta la capacidad de tratamiento a distintos niveles de financiación?

El juego ya informa de la capacidad mensual de tratamiento combinada de las incineradoras y las centrales de conversión de residuos en energía.

El Ministerio usa ese valor para estimar cuál sería la capacidad con otro porcentaje de financiación.

**Capacidad de tratamiento proyectada**

```math
K(p)=\operatorname{round}\left(K_{\mathrm{act}}\times\frac{e(p)}{e_{\mathrm{act}}}\right)
```

Donde:

- **Kact** es la capacidad de tratamiento combinada que el juego indica en este momento.
- **e(p)** es la eficiencia de la limpieza con la financiación propuesta.
- **eact** es la eficiencia actual.
- **K(p)** es la capacidad de tratamiento proyectada.

**Ejemplo**

Durante las pruebas, al pasar la financiación de la limpieza del 61% al 50%, la eficiencia cambió del 40,125% al 25%.

La capacidad de las centrales de conversión de residuos en energía cayó de unas **34.088 a 21.238 unidades**, en línea con el modelo.

Si la eficiencia actual es cero mientras existen edificios de tratamiento, el Ministerio no puede calcular de forma fiable las capacidades alternativas y detiene el plan de financiación en lugar de adivinar.

#### ¿Cómo se calcula la admisión disponible de los vertederos?

El Ministerio también estima cuánta basura más pueden aceptar los vertederos durante el mes en curso.

**Admisión restante de los vertederos**

```math
V_0(p)=
\max\left(
0,\min\left(
F,\operatorname{round}(5000\times L\times e(p))-S
\right)
\right)
```

Donde:

- **F** es el almacenamiento libre restante de los vertederos.
- **L** es el número de casillas de vertedero.
- **e(p)** es la eficiencia de la limpieza con la financiación propuesta.
- **S** es la basura ya acumulada en la válvula mensual del vertedero.
- **V₀(p)** es la admisión restante permitida con esa financiación.

Por ejemplo, un vertedero puede tener 100.000 unidades de almacenamiento libre, pero solo 20.000 unidades de capacidad mensual de admisión restante.

**El Ministerio cuenta únicamente esas 20.000 unidades como disponibles para eliminar basura durante el resto de ese mes.**

Si no existe ningún vertedero, la admisión disponible de los vertederos es cero.

Si no se puede leer la información necesaria sobre el vertedero, el Ministerio no da por hecho que este pueda aceptar más basura.

Cuando las exportaciones y las instalaciones de tratamiento pueden gestionar toda la basura restante, no hace falta capacidad de vertedero.

<a id="garbage-landfills-nextmonth"></a>
#### 3.5 ¿Por qué el Ministerio comprueba a veces también el mes siguiente?

Como se explicó antes, una financiación baja de la limpieza puede dejar basura en la válvula de admisión del vertedero cuando termina un mes.

Esa basura sobrante reduce la admisión disponible del vertedero durante el mes siguiente.

**Por lo tanto, un nivel de financiación que despeja el mes actual puede crear un problema en el siguiente.**

Para evitarlo, el Ministerio también puede simular **todo el mes siguiente**.

Sin embargo, solo realiza esta comprobación adicional cuando dispone de información suficiente para predecir con fiabilidad el comportamiento del almacenamiento interno del vertedero.

#### ¿Cómo aprende la estructura de almacenamiento de un vertedero?

SimCity 4 no proporciona directamente todos los detalles de la disposición interna de las celdas del vertedero que este cálculo necesita.

Por eso el Ministerio aprende la estructura observando los cambios mensuales normales.

Compara:

- La basura almacenada antes del cambio de mes.
- La basura almacenada después.
- La cantidad recién absorbida en el almacenamiento del vertedero.

**Degradación mensual observada del vertedero**

```math
D_{\mathrm{obs}}=
U_{\mathrm{antes}}-U_{\mathrm{después}}+\text{absorbida}
```

Donde:

- **Uantes** es la cantidad almacenada antes de la actualización mensual.
- **Udespués** es la cantidad almacenada después.
- **absorbida** es la cantidad transferida al almacenamiento durante esa actualización.
- **Dobs** es la basura eliminada por degradación.

El Ministerio compara este resultado con la fórmula de degradación de vertederos del juego.

Prueba posibles números de celdas de almacenamiento y comprueba si reproducen **exactamente** la degradación observada.

Además, el número posible de celdas debe dividir la capacidad total de almacenamiento del vertedero en unidades enteras.

**Cuando una única disposición coincide con las observaciones, la geometría del vertedero se considera calibrada.**

Si siguen siendo posibles varias disposiciones, el Ministerio espera a tener más observaciones mensuales.

Si ninguna coincide, o si la estructura del vertedero de la ciudad no se puede representar de forma fiable con una sola disposición, no usa la predicción del mes siguiente.

Por ejemplo, una ciudad con varios vertederos de estructuras internas de celdas distintas puede no producir observaciones que encajen en un único modelo de geometría.

**¿Cuánto tarda la calibración?**

El Ministerio aprende de las actualizaciones mensuales normales, sin que tengas que hacer nada especial.

En una ciudad de prueba, un solo cambio de mes ordinario aportó información suficiente para identificar la geometría del vertedero.

Eso significó que la comprobación del mes siguiente estuvo disponible en el segundo evento mensual de la sesión.

Sin embargo, la calibración puede requerir más observaciones o no ser posible en algunas ciudades.

**La geometría aprendida se conserva solo durante la sesión actual de la ciudad.**

No se guarda entre sesiones.

Si la capacidad del vertedero cambia porque zonificas o deszonificas casillas, el Ministerio vuelve a empezar a reunir pruebas.

#### ¿Cómo funciona la predicción del mes siguiente?

Una vez calibrada la geometría del vertedero, el Ministerio puede estimar:

1. Cuánta basura almacenada se degradará en el próximo cambio de mes.
2. Cuánta basura de la válvula de admisión se absorberá en el almacenamiento.
3. Cuánta basura puede quedar en la válvula.
4. Cuánta capacidad de admisión del vertedero habrá disponible durante el mes siguiente.

Usa las fórmulas mensuales de absorción del juego descritas antes.

Como el valor de mantenimiento financiado que indica el juego puede diferir en ocasiones en un simoleón del cálculo esperado, el Ministerio prueba ambos:

```math
A=\left\lfloor B\times\frac{p}{100}\right\rfloor
```

y el mismo valor aumentado en uno.

A continuación, pone a cero el uso mensual simulado de cada contrato de basura y repite la proyección diaria de basura durante todo el mes siguiente, empezando sin acumulación de basura sin recoger.

**El nivel de financiación solo se acepta cuando ambos meses terminan sin basura sin recoger.**

Si no se puede calibrar la geometría del vertedero, el Ministerio usa únicamente el cálculo del mes actual.

En ese caso, el gestor regional de contratos también se comporta de forma más conservadora al considerar cambios en los contratos de basura existentes.

Consulta [Acuerdos con vecinos](deals.md#neighbor-deals).

<a id="garbage-landfills-funding"></a>
#### 3.6 ¿Cómo elige el Ministerio la financiación de la limpieza?

Tras calcular cuánta basura se puede gestionar con cada nivel de financiación, el Ministerio busca **el porcentaje más barato que cumple los requisitos de eliminación**.

Los gastos de limpieza aumentan en proporción a la financiación.

**Gasto mensual de limpieza**

```math
E(p)=\operatorname{round}\left(I\times\frac{p}{100}\right)
```

Donde:

- **I** es el coste mensual del departamento de limpieza con el 100% de financiación, según indica el juego.
- **p** es el porcentaje de financiación propuesto.
- **E(p)** es el gasto mensual estimado.

Este coste incluye el mantenimiento de los vertederos y las demás partidas del presupuesto del departamento.

Por ejemplo, si el departamento de limpieza cuesta §2.000 al mes con el 100% de financiación:

| Financiación | Gasto mensual estimado |
|---|---:|
| 50% | §1.000 |
| 75% | §1.500 |
| 100% | §2.000 |
| 120% | §2.400 |

Pero el porcentaje más barato no es necesariamente seguro.

Un presupuesto bajo puede reducir la capacidad de tratamiento, limitar la admisión de los vertederos o dejar basura sin recoger al final del mes.

**Optimización de la financiación**

```math
p^*=
\arg\min_{p\in[0,p_{\max}]}E(p)
```

Sujeto a:

- Que el mes actual termine sin basura sin recoger.
- Que el mes siguiente también termine sin basura sin recoger, cuando la geometría del vertedero esté calibrada.

Aquí, **pmax** es la financiación máxima de la limpieza que permite el juego.

**¿Cómo encuentra el porcentaje adecuado más bajo?**

Como la capacidad de eliminación modelada normalmente aumenta con la financiación, el Ministerio puede buscar de forma eficiente el porcentaje más bajo que cumple los requisitos.

Usa una búsqueda binaria cuando la relación es monótona. Si esa suposición no se cumple, comprueba los porcentajes disponibles uno por uno.

Si varios porcentajes tienen el mismo gasto mensual redondeado, prefiere el más cercano a la financiación actual.

Así evita cambios innecesarios que no ahorran dinero.

#### ¿Y si ni siquiera la financiación máxima puede despejar la basura?

A veces el sistema de limpieza simplemente no tiene capacidad de eliminación suficiente.

Por ejemplo, un vertedero puede estar casi lleno.

En ese caso, aumentar la financiación de la limpieza no crea necesariamente más espacio de almacenamiento utilizable.

Por eso el Ministerio evita elegir automáticamente el 120% cuando no serviría de nada.

En su lugar:

1. Calcula la menor acumulación pendiente que se puede conseguir con la financiación máxima.
2. Busca el **nivel de financiación más barato que produce esa misma acumulación**.
3. Elige ese nivel en lugar de pagar de más sin obtener mejora.

**El Ministerio no gasta más cuando una financiación adicional no puede mejorar el resultado.**

Si una financiación mayor reduce de verdad la acumulación pendiente, aún puede elegir la financiación máxima.

Los contratos regionales de basura también pueden ayudar a resolver una carencia, según las ciudades vecinas disponibles y las reglas de los contratos.

<a id="garbage-landfills-writing"></a>
#### 3.7 ¿Cómo se aplican los cambios de financiación?

A diferencia de la electricidad y el agua, la financiación de la limpieza **no siempre se cambia directamente a su objetivo final**.

El motivo es que SimCity 4 tarda varios días simulados en actualizar la eficiencia de la limpieza y las capacidades de tratamiento tras un cambio de presupuesto.

Hacer varios ajustes grandes antes de que el juego termine de actualizarse podría provocar decisiones inestables.

**Por eso el Ministerio cambia la financiación de la limpieza de forma gradual cuando la reduce.**

| Cambio | Cómo se aplica |
|---|---|
| **Reducción de la financiación** | Como máximo **25 puntos porcentuales** cada vez, con al menos **7 días simulados** entre cambios de financiación de la limpieza. |
| **Aumento de la financiación** | El aumento completo necesario se puede aplicar de una sola vez, con al menos **3 días simulados** desde el cambio anterior de financiación de la limpieza. |

Son puntos porcentuales, no porcentajes del presupuesto actual.

**Ejemplo: reducir la financiación del 120% al 31%**

En lugar de pasar directamente del 120% al 31%, el Ministerio sigue esta secuencia:

<box gap={2}>
  <row align="center" justify="between">
    <text weight="medium">Financiación inicial</text>
    <text weight="medium" tabularNums>120%</text>
  </row>
  <box background="surface-tertiary" height="8px" radius="full" clip>
    <box background="rgba(100,116,139,0.85)" width="100%" height="100%" radius="full"/>
  </box>
  {#each [{p:95,w:"79.2%",d:"Al menos 7 días después"},{p:70,w:"58.3%",d:"Al menos 7 días más"},{p:45,w:"37.5%",d:"Al menos 7 días más"},{p:31,w:"25.8%",d:"Al menos 7 días más"}] as s}
    <row align="center" justify="between">
      <text color="secondary" size="sm">{s.d}</text>
      <text weight="medium" tabularNums>{s.p}%</text>
    </row>
    <box background="surface-tertiary" height="8px" radius="full" clip>
      <box background="rgba(100,116,139,0.85)" width={s.w} height="100%" radius="full"/>
    </box>
  {/each}
</box>

Por lo tanto, llegar al objetivo final lleva **al menos varias semanas simuladas**, posiblemente cerca de un mes.

Esto da tiempo al juego para actualizar su eficiencia y su capacidad de eliminación entre un cambio y otro.

**Los aumentos de financiación son más rápidos** porque recuperar la capacidad de eliminación puede ser urgente.

Aun así, incluso los aumentos respetan el intervalo mínimo de tres días simulados desde el ajuste anterior de la financiación de la limpieza.

#### ¿Cómo verifica el Ministerio cada cambio?

Todo ajuste de la financiación usa el propio control del presupuesto de limpieza de SimCity 4.

Antes de aplicar un cambio, el Ministerio vuelve a leer la financiación actual.

Si has movido el control deslizante a mano desde que se calculó el plan, cancela ese cambio pendiente y recalcula a partir de tu nuevo ajuste.

**Tras aplicar la financiación, el Ministerio realiza dos comprobaciones.**

**1. Verificación inmediata de la financiación**

Confirma que el juego indica el nuevo porcentaje de financiación.

Si el valor no fue aceptado, **la gestión automática de la financiación de la limpieza se detiene durante el resto de la sesión actual de la ciudad**.

Los contratos regionales de basura pueden seguir gestionándose, usando la financiación de la limpieza que ya quedó fija.

**2. Verificación de la eficiencia y la capacidad**

Como la capacidad de la limpieza no se actualiza al instante, el Ministerio también espera a que los cálculos del juego se pongan al día.

Comprueba que:

- La eficiencia indicada coincide con el valor esperado según la curva de financiación, con una tolerancia de **0,001**.
- Las capacidades de los edificios de tratamiento coinciden con los valores esperados, con una tolerancia de **3% u 8 unidades de basura**, la que sea mayor.

**No se hace ningún otro cambio de financiación de la limpieza ni ajuste de contratos de basura hasta que se confirmen estas comprobaciones.**

Así se evita que el Ministerio tome nuevas decisiones basadas en datos del juego actualizados solo en parte.

**El objetivo no es solo encontrar el presupuesto de limpieza más barato, sino llegar a él sin desestabilizar el sistema de eliminación de basura de la ciudad.**
<a id="garbage-landfills-when"></a>
#### 3.8 ¿Cuándo revisa el Ministerio la financiación de la limpieza?

El Ministerio revisa la financiación de la limpieza **al comienzo de cada mes simulado**.

También puede recalcular el presupuesto antes cuando la situación de la basura de la ciudad cambia de forma significativa.

Se activa una evaluación anticipada cuando:

- La **producción de basura** cambia al menos un 1%.
- La **capacidad de tratamiento de las incineradoras o de las centrales de conversión de residuos en energía** cambia al menos un 1%.
- La **eficiencia de la limpieza** cambia al menos un 1%.
- Las **importaciones o exportaciones de basura** cambian al menos un 1%.
- **Aparece o desaparece basura sin recoger** en la ciudad.

Estos cambios se comparan con los valores registrados en la evaluación anterior.

**El Ministerio no repite el mismo cálculo cuando no ha cambiado nada.**

Por ejemplo, si tu ciudad construye una incineradora nueva, el aumento de la capacidad de tratamiento puede activar una nueva evaluación sin esperar al mes siguiente.

Del mismo modo, si la basura empieza a acumularse en las calles, el Ministerio puede revisar si hace falta más financiación.

Cada evaluación usa las últimas lecturas disponibles y respeta los ajustes graduales de financiación descritos en la [Sección 3.7](#garbage-landfills-writing).

<a id="garbage-landfills-advice"></a>
#### 3.9 Consejo sobre el tamaño del vertedero: cuándo ampliar o reducir un vertedero puede ahorrar dinero

A veces, cambiar el tamaño de un vertedero puede reducir los gastos de limpieza de tu ciudad.

Puede parecer sorprendente. Añadir casillas de vertedero aumenta los costes de mantenimiento, pero también puede permitir que el Ministerio haga funcionar el sistema de limpieza con un porcentaje de financiación más bajo.

A la inversa, quitar casillas sin usar puede reducir el mantenimiento sin afectar a la capacidad de la ciudad para eliminar basura.

El Ministerio puede analizar ambas posibilidades y recomendar un cambio cuando el ahorro esperado es significativo.

**Esta función está desactivada por defecto.**

Puedes activarla con:

**Avisar si revisar el vertedero puede ahorrarme dinero**

A continuación, el Ministerio busca dos tipos de recomendaciones:

| Recomendación | Qué significa |
|---|---|
| **AMPLIAR** | Añadir una zona concreta a tu vertedero podría reducir la factura mensual total de la limpieza. |
| **REDUCIR** | Quitar una zona concreta sin usar podría reducir los costes de mantenimiento sin comprometer la eliminación de la basura. |

**El Ministerio nunca cambia automáticamente la zonificación del vertedero por estas recomendaciones.**

Solo muestra el consejo. Tú decides si lo sigues, y eres tú quien zonifica o deszonifica.

El consejo tampoco modifica la financiación de tu limpieza ni los contratos regionales.

#### ¿Qué vertederos se pueden analizar?

El Ministerio solo da consejos sobre el tamaño cuando puede entender con seguridad la estructura del vertedero.

Para ello exige:

- **Exactamente una zona de vertedero** en la ciudad.
- Una versión compatible del juego (**SimCity 4 Deluxe 1.1.641.0**).
- Acceso fiable a las casillas, las celdas de almacenamiento, los montones de basura y los costes de mantenimiento del vertedero.
- Concordancia total entre la estructura interna del vertedero y los totales que indica el juego.

Comprueba que la capacidad, la basura almacenada, el mantenimiento, el número de casillas y el contenido de las celdas son coherentes entre sí.

También verifica que cada casilla pertenece a la zona esperada.

**Si alguna información es incoherente, el Ministerio no hace ninguna recomendación.**

Así se evitan sugerencias basadas en datos del vertedero incompletos o incorrectos.

#### ¿Cómo encuentra el Ministerio zonas de ampliación seguras?

Para una recomendación de **AMPLIAR**, el Ministerio busca áreas rectangulares de terreno sin usar junto al vertedero existente.

Solo considera ampliaciones hacia la **derecha o la parte inferior del vertedero en las coordenadas internas del mapa del juego**.

Esta restricción es importante porque ampliar hacia arriba o hacia la izquierda puede desplazar las celdas de almacenamiento internas del vertedero y alterar la basura almacenada en ellas.

El área propuesta debe cumplir varias condiciones:

- Debe tocar el vertedero existente a lo largo de su borde derecho o inferior.
- Debe empezar en una esquina del borde elegido.
- Debe tener entre **2 y 8 casillas de fondo**.
- Debe tener al menos **2 casillas de largo**, sin sobrepasar el borde contiguo ni el límite de 48 casillas del juego.
- No debe contener más de **64 casillas**.
- Todas las casillas deben estar disponibles, dentro del mapa y sin edificar ni zonificar.
- No debe fusionarse con otra zona de vertedero.
- No debe provocar que la basura almacenada se empuje de vuelta a la válvula de admisión del vertedero.

El Ministerio calcula cómo afectaría la nueva forma a las celdas de almacenamiento y a la capacidad del vertedero.

También realiza una **simulación previa con las propias comprobaciones de zonificación del juego**, para confirmar que SimCity 4 aceptaría de verdad el área propuesta.

Si el juego rechazara la operación u omitiera en silencio alguna casilla, ese candidato se descarta.

**Solo se tienen en cuenta las ampliaciones verificadas como seguras.**

Cuando varias ubicaciones seguras tienen el mismo número de casillas, el Ministerio se queda con la que ofrece la mayor capacidad de almacenamiento resultante.

#### ¿Cómo encuentra el Ministerio zonas seguras que quitar?

Para una recomendación de **REDUCIR**, el Ministerio busca casillas sin usar en los bordes exteriores del vertedero.

Considera quitar **de 1 a 8 filas o columnas exteriores completas**, formando un área rectangular de no más de 64 casillas.

La reducción propuesta debe cumplir todas estas condiciones:

- Ninguna casilla quitada contiene un montón de basura.
- Ninguna de las celdas de almacenamiento afectadas contiene basura.
- No se devolvería basura almacenada a la válvula de admisión.
- El vertedero restante debe seguir conectado como una sola área.
- Debe quedar al menos un **bloque de vertedero de 2 × 2** completo.
- La propia comprobación de deszonificación del juego debe aceptar la operación.

**El Ministerio nunca recomienda quitar casillas que alterarían la basura almacenada.**

Por ejemplo, un vertedero grande puede tener varias filas completamente sin usar a lo largo de un borde.

Quitar esas filas podría reducir el mantenimiento sin afectar a la basura ya almacenada ni a las necesidades de eliminación de la ciudad.

Pero si quitarlas dañara la estructura interna de almacenamiento del vertedero, la sugerencia se rechaza.

#### ¿Cómo calcula el Ministerio el ahorro potencial?

El Ministerio considera dos fuentes de ahorro:

1. Los cambios en el coste de mantenimiento del vertedero.
2. Los cambios en la financiación de la limpieza necesaria tras cambiar el tamaño del vertedero.

**Cambios en el mantenimiento del vertedero**

```math
\begin{aligned}
\text{AMPLIAR:}\quad
\Delta B &= 5n_{\text{añadidas}}-5g_{\text{reconciled}}
\\
L' &= L+n
\\[6pt]
\text{REDUCIR:}\quad
\Delta B &= -5n
\\
L' &= L-n
\end{aligned}
```

Donde:

- **n** es el número de casillas de vertedero añadidas o quitadas.
- **gᵣₑcₒₙcᵢₗₑd** es el número de casillas fantasma eliminadas durante la ampliación.
- **ΔB** es el cambio en el mantenimiento mensual del vertedero con el 100% de financiación.
- **L** y **L′** son los números de casillas del vertedero antes y después del cambio.

Las casillas de vertedero vacías nuevas cuestan **§5 cada una al mes** con el 100% de financiación.

Quitar una casilla sin usar ahorra ese gasto de mantenimiento.

Una ampliación también puede eliminar casillas fantasma que ya cuestan dinero, lo que puede compensar en parte el mantenimiento de la nueva área añadida.

El Ministerio también recalcula la capacidad de almacenamiento del vertedero:

```math
C'=\operatorname{round}\left(\frac{T'\times30000}{N'}\right)
\qquad
T'_{\mathrm{cap}}=N'\times C'
```

Donde **T′** es el número resultante de casillas pertenecientes a la zona y **N′** es el número resultante de celdas de almacenamiento activas.

El nuevo coste mensual nominal del departamento de limpieza es:

```math
I'=I+\Delta B
```

**Lo importante es lo que ocurre a continuación.**

El Ministerio no se limita a multiplicar por §5 el número de casillas añadidas o quitadas.

También recalcula **la financiación de limpieza más barata necesaria tras el cambio propuesto**, con la misma previsión de basura, la misma reserva, los mismos contratos y el mismo calendario.

**Ahorro mensual estimado**

```math
\begin{aligned}
E_0 &= \operatorname{round}\left(I\times\frac{p_0}{100}\right)\\
E_1 &= \operatorname{round}\left(I'\times\frac{p_1}{100}\right)\\
\text{Ahorro} &= E_0-E_1
\end{aligned}
```

Donde:

- **E₀** es el gasto mensual de la disposición original del vertedero.
- **E₁** es el gasto mensual tras el cambio propuesto.
- **p₀** es la financiación que el Ministerio elegiría para el vertedero actual.
- **p₁** es la financiación que elegiría tras el cambio propuesto.

Ambos niveles de financiación los calcula el optimizador de la limpieza.

**La comparación usa la financiación que el Ministerio elegiría hoy, no necesariamente la que muestra ahora el control deslizante de tu presupuesto.**

Esto es importante porque las reducciones de la limpieza se hacen de forma gradual. Una reducción sin terminar no debe confundirse con un ahorro causado por cambiar el tamaño del vertedero.

Un cambio propuesto solo se acepta como consejo cuando el plan de limpieza resultante sigue pudiendo despejar la basura necesaria, incluido el mes siguiente cuando esa comprobación adicional está disponible.

El cálculo usa los costes de mantenimiento vigentes en el momento de la operación de zonificación propuesta. No se incluyen los aumentos futuros causados por nuevos montones de basura, y el gasto único de la zonificación no cuenta como parte del ahorro mensual.

#### ¿Cuánto debe ahorrar un cambio para que el Ministerio lo recomiende?

Puedes configurar el **Ahorro mínimo para avisos del vertedero**.

El valor por defecto es el **25%**, ajustable **del 20% al 50%**.

El Ministerio solo recomienda un cambio cuando su ahorro mensual esperado es **estrictamente mayor que tu umbral**.

**Umbral para el consejo**

```math
\text{Ahorro}\times100>\theta\times E_0
```

Donde **θ** es el umbral de ahorro que has configurado.

El porcentaje que muestra el aviso se redondea hacia abajo:

```math
\text{Ahorro mostrado (\%)}=
\left\lfloor\frac{100\times\text{Ahorro}}{E_0}\right\rfloor
```

**Ejemplo: un presupuesto de limpieza de §1.000 al mes**

Con el umbral por defecto del 25%:

| Ahorro mensual esperado | Porcentaje de ahorro | Recomendación |
|---|---:|---|
| §200 | 20% | No |
| §250 | 25% | No |
| **§251** | **25,1%** | **Sí** |
| §350 | 35% | Sí |

**Un ahorro de exactamente el 25% no activa una recomendación.**

El ahorro debe superar el 25%, no solo alcanzarlo.

#### ¿Con qué frecuencia analiza el Ministerio el tamaño del vertedero?

El consejo sobre el tamaño del vertedero es conservador a propósito.

El Ministerio evita reconsiderar una y otra vez tus decisiones de zonificación o recomendar cambios mientras un vertedero aún se está asentando en su nueva configuración.

Se aplican tres reglas de tiempo:

| Regla | Qué significa |
|---|---|
| **Como máximo una vez por mes simulado** | El Ministerio no analiza el tamaño de los vertederos varias veces durante el mismo mes. |
| **Periodo de protección de 24 meses** | No analiza un vertedero durante 24 meses simulados tras ciertos cambios o cuando su historial es incierto. |
| **Intervalo de 12 meses entre consejos** | Tras emitir una recomendación, espera al menos 12 meses simulados antes de analizar la ciudad para dar otra. |

**El periodo de protección de 24 meses**

El Ministerio espera 24 meses simulados después de que:

- Amplíes o reduzcas un vertedero.
- Se encuentre por primera vez con una ciudad que ya contiene un vertedero, porque no sabe cuándo se creó ni cuándo se cambió por última vez.
- Cargues un guardado antiguo y el calendario del juego retroceda.

Esto evita consejos prematuros mientras el mantenimiento del vertedero, la distribución de la basura y los cálculos del almacenamiento interno aún están cambiando.

**El intervalo de 12 meses entre consejos**

Tras dar una recomendación, el Ministerio espera 12 meses simulados antes de volver a comprobar.

Si la misma oportunidad sigue superando tu umbral, puede volver a recomendarse.

Desactivar la opción y volver a activarla, o cambiar el umbral de ahorro, **no reinicia estos periodos de espera**.

El Ministerio registra por separado para cada ciudad las fechas necesarias y la información del vertedero, de modo que los periodos de espera se conservan al cerrar y volver a abrir el juego.

Las reparaciones automáticas de casillas fantasma no reinician el periodo de protección de 24 meses.

#### ¿Cómo mantiene el Ministerio el análisis rápido?

Analizar posibles cambios de zonificación implica comprobar muchas formas posibles del vertedero.

Para no ralentizar SimCity 4, el Ministerio limita el trabajo que realiza en cada análisis.

- Evalúa **como máximo 64 candidatos** por análisis.
- Se fija un presupuesto de cálculo de **10 milisegundos**.
- Al principio reparte las evaluaciones de candidatos entre AMPLIAR y REDUCIR.
- Si una dirección tiene menos candidatos, la otra puede usar el margen de evaluación sobrante.
- Si varios análisis seguidos superan el presupuesto de tiempo, el Ministerio reduce progresivamente cuántos candidatos comprueba.

El número máximo de candidatos puede bajar temporalmente a **48, 32, 16, 8, 4 y 2**, según el rendimiento.

Tras seis análisis consecutivos dentro del presupuesto de tiempo, el límite puede volver a subir un escalón.

El Ministerio también comprueba los tamaños de candidato más pequeños antes de recomendar los más grandes, porque el ahorro no aumenta necesariamente con el tamaño del vertedero.

**Una consecuencia importante:** solo recomienda un tamaño después de evaluar todos los tamaños permitidos más pequeños. Esto ayuda a no pasar por alto un cambio menor y mejor.

Entre los candidatos seguros que evalúa, el Ministerio selecciona:

1. El candidato con mayor ahorro mensual.
2. Si los ahorros son iguales, el candidato que usa menos casillas.
3. Si sigue habiendo empate, AMPLIAR antes que REDUCIR.

El análisis se ejecuta como parte de una evaluación normal de la limpieza. No requiere un proceso aparte en segundo plano.

#### ¿Qué aspecto tiene la recomendación?

El Ministerio envía un mensaje a través del **asesor de servicios públicos** de SimCity 4, clasificado como buena noticia.

Son posibles dos tipos de aviso:

**Para una ampliación:**

> **¿Más vertedero, menos gasto?**

**Para una reducción:**

> **¡El vertedero ocupa demasiado sitio!**

El aviso explica:

- Si hay que ampliar o reducir el vertedero.
- Cuántas casillas hay que añadir o quitar.
- El largo y el fondo del rectángulo propuesto.
- El ahorro mensual estimado en simoleones.
- El porcentaje del presupuesto de limpieza que se podría ahorrar.

También incluye un enlace en el que se puede hacer clic, **ver dónde**.

Al seleccionar el enlace, la cámara se mueve al centro del área sugerida.

El mensaje evita decir «izquierda», «derecha», «arriba» o «abajo», porque la dirección que ves en pantalla depende de cómo hayas girado la vista de la ciudad.

**El Ministerio solo registra que ha dado un consejo cuando el juego acepta correctamente el aviso del asesor.**

Recuerda: **la recomendación es meramente informativa. Tú decides si cambias tu vertedero.**

<a id="ghost-tiles"></a>
#### 3.10 Casillas fantasma: reparación automática

Las casillas fantasma de vertedero son distintas de un vertedero sobredimensionado o ineficiente.

Son **un error de SimCity 4**, no una decisión de zonificación.

Como se explicó antes, una casilla fantasma es una ubicación que el juego sigue contando como parte de un vertedero, aunque ya no esté zonificada realmente como vertedero.

Esto puede generar gastos de mantenimiento innecesarios.

**El Ministerio puede reparar automáticamente estas casillas fantasma cuando gestiona la limpieza.**

A diferencia del consejo sobre el tamaño del vertedero, esta reparación:

- No requiere un ajuste aparte.
- No depende de un umbral de ahorro.
- No genera un aviso del asesor.
- No añade ni quita ninguna casilla legítima de vertedero.
- No demuele edificios ni cambia la zonificación visible.
- Nunca reduce a propósito la capacidad de almacenamiento del vertedero.

La actividad de reparación se registra en el log.

#### ¿Cómo puede reparar una casilla fantasma el Ministerio?

La reparación aprovecha un comportamiento concreto del sistema de zonificación de SimCity 4.

**Volver a aplicar la zonificación de vertedero a casillas que ya están legítimamente zonificadas como vertedero hace que el juego reconstruya la estructura interna de la zona.**

Esta operación no cuesta dinero ni cambia el tipo de zona de esas casillas, pero la reconstrucción también elimina la pertenencia obsoleta de las casillas fantasma.

En la práctica, el Ministerio le pide al juego que reconstruya un vertedero con sus propias reglas de zonificación, sin cambiar el área legítima del vertedero.

#### ¿Cómo funciona la reparación automática?

El Ministerio sigue cuatro pasos.

**1. Detectar las casillas fantasma**

Durante la primera evaluación de la limpieza de cada mes simulado, comprueba las zonas de vertedero disponibles.

Tras abrir una ciudad, esta comprobación se hace a más tardar en la primera evaluación mensual de la limpieza, siempre que la información necesaria esté disponible.

Usa la misma información de vertedero, validada y de solo lectura, que se necesita para el consejo sobre el tamaño.

Si los totales que indica el juego y los datos internos del vertedero no coinciden, el Ministerio no intenta ninguna reparación.

**2. Verificar que la reparación es segura**

El Ministerio elige una región que contiene casillas legítimas de vertedero sin montones de basura.

Usa las dimensiones de zonificación admitidas por el juego y comprueba el resultado esperado antes de hacer ningún cambio.

La reparación se rechaza si pudiera:

- Añadir casillas nuevas de vertedero.
- Devolver basura almacenada a la válvula de admisión del vertedero.
- Reducir la capacidad total de almacenamiento del vertedero.
- Fusionar dos zonas de vertedero distintas.
- Producir un resultado de capacidad incierto por el redondeo.
- Afectar a un área que las comprobaciones de zonificación del juego rechazarían.

También rechaza la reparación si no hay casillas de vertedero adecuadas sin montones de basura.

La región de reparación elegida debe caber en el **límite de zonificación de 48 × 48** del juego.

**3. Realizar la reparación**

Cuando las comprobaciones se superan, el Ministerio usa la propia operación de zonificación de SimCity 4 para volver a aplicar la zonificación de vertedero a la región elegida.

La operación no debe omitir en silencio ninguna casilla.

Si la llamada de zonificación falla o el juego rechaza parte de la región, el intento se considera fallido.

**4. Verificar el resultado**

El juego procesa la operación de zonificación después de recibir la solicitud.

Por eso el Ministerio comprueba el resultado en las evaluaciones siguientes, con hasta **cuatro comprobaciones de confirmación**.

Verifica que:

- El rectángulo envolvente del vertedero no ha cambiado.
- Las casillas fantasma han desaparecido.
- La disposición de las celdas de almacenamiento y la capacidad coinciden con el resultado previsto.
- El número de casillas legítimas de vertedero no ha cambiado.
- La capacidad de vertedero de la ciudad coincide con el valor esperado.
- La información interna del vertedero sigue coincidiendo con los totales del juego.

La cantidad de basura almacenada cambia de forma natural mientras el juego avanza, así que esa cantidad se registra en lugar de exigir que se mantenga idéntica.

**La reparación solo se acepta cuando la estructura resultante del vertedero coincide con el resultado seguro esperado.**

#### ¿Qué pasa si una reparación falla?

El Ministerio no repite intentos de reparación arriesgados.

Cada estado distinto de casillas fantasma se intenta **como máximo una vez por sesión de la ciudad**.

Si la operación de zonificación falla o la estructura resultante no coincide con la predicción, **la reparación automática de casillas fantasma se desactiva hasta que se cierre SimCity 4**.

Las demás funciones del Ministerio pueden seguir funcionando.

El Ministerio también repara **como máximo una zona de vertedero por mes simulado**.

Si varias zonas contienen casillas fantasma, las restantes se consideran en meses posteriores.

Mientras una reparación espera confirmación, el consejo sobre el tamaño del vertedero se aplaza para no analizar un área cuya estructura interna aún está cambiando.

#### ¿Por qué pueden volver las casillas fantasma?

Por desgracia, el comportamiento de guardado y carga del juego original no se corrige de forma permanente.

Al volver a abrir una ciudad, SimCity 4 puede recrear la pertenencia fantasma a lo largo de la última fila o columna del rectángulo envolvente interno de un vertedero.

**Por lo tanto, puede que el Ministerio tenga que reparar de nuevo las mismas casillas fantasma tras volver a abrir la ciudad.**

Eso no significa que la reparación anterior fallara. Significa que el juego recreó el error durante la carga.

Cada casilla fantasma eliminada ahorra **§5 por mes simulado** con el 100% de financiación de la limpieza.

El ahorro real con un porcentaje de financiación menor es proporcionalmente más pequeño.

**La reparación automática de casillas fantasma es una corrección prudente de un error del juego, no un rediseño automático del vertedero.**

<a id="garbage-landfills-safety"></a>
### 4. Condiciones de seguridad

El Ministerio aplica reglas de seguridad distintas a la financiación de la limpieza, al consejo sobre el tamaño del vertedero y a la reparación de casillas fantasma.

#### Financiación automática de la limpieza

- **La gestión debe estar activada.** Tanto **Ministerio activo** como **Gestionar la limpieza** deben estar activados.
- **Solo se usan niveles de financiación válidos.** La financiación se mantiene dentro de los límites que acepta el sistema de presupuesto de limpieza del juego.
- **Se respeta la financiación fija.** El Ministerio no modifica un presupuesto de limpieza que el juego indica como fijo.
- **La reserva configurada siempre está incluida.** El plan de limpieza debe cubrir la necesidad de basura protegida.
- **El mes proyectado debe terminar sin basura sin recoger.** Cuando hay predicciones del mes siguiente, este también debe ser viable.
- **La información poco fiable nunca se trata como capacidad disponible.** Una válvula de vertedero ilegible no aporta ninguna admisión supuesta, y una exportación de basura ilegible no aporta ningún alivio de eliminación supuesto.
- **Los cambios de financiación son graduales y se verifican.** Las reducciones siguen las reglas de ritmo, y no se intenta un nuevo cambio hasta que se haya confirmado el anterior.
- **Los fallos técnicos detienen los cambios de financiación durante la sesión.** El Ministerio evita seguir con un control de financiación poco fiable.

Si **Gestionar la limpieza** está desactivado, el Ministerio deja el presupuesto de limpieza sin cambios.

Los contratos regionales de basura aún se pueden gestionar de forma independiente, usando la financiación actual para calcular la capacidad de eliminación disponible.

#### Consejo sobre el tamaño del vertedero

**El consejo sobre el tamaño del vertedero nunca cambia la zonificación ni la financiación.**

Solo está disponible cuando se gestiona la financiación de la limpieza y la información interna del vertedero se puede verificar con exactitud.

El Ministerio también exige:

- Exactamente una zona de vertedero.
- Un cambio propuesto seguro que conserve la basura almacenada.
- Ningún riesgo de fusionar zonas.
- Una ampliación que no desplace las posiciones de las celdas de almacenamiento del vertedero.
- La aprobación de la propia validación de zonificación o deszonificación del juego.
- Un ahorro estrictamente superior al umbral configurado.

Si no se cumplen estas condiciones, **no se emite ningún consejo**.

#### Reparación automática de casillas fantasma

La reparación automática de casillas fantasma solo está disponible en la versión compatible **SimCity 4 Deluxe 1.1.641.0**, mientras la gestión de la limpieza está activa.

Toda reparación debe:

- Usar datos de vertedero validados.
- Conservar todas las casillas legítimas del vertedero.
- Evitar cambiar tipos de zona o demoler nada.
- Conservar o aumentar la capacidad total de almacenamiento del vertedero.
- Evitar alterar la basura almacenada o devolverla a la válvula de admisión.
- Evitar fusionar zonas o depender de un redondeo incierto.
- Superar la propia validación de zonificación del juego.
- Producir exactamente el resultado esperado al comprobarlo después.

Si la verificación falla, las reparaciones automáticas se detienen durante el resto de la sesión de juego.

**El Ministerio siempre prefiere dejar un vertedero sin cambios antes que hacer una modificación que no puede verificar como segura.**

<a id="garbage-landfills-settings"></a>
### 5. Ajustes

La gestión de la basura y de los vertederos se puede configurar en **Opciones de juego → Ministerio de Optimización**, en la página **Servicios**.

| Ajuste | Por defecto | Rango | Qué hace |
|---|---|---|---|
| [Ministerio activo](options.md#opt-master) | Activado | Activado / Desactivado | Activa o desactiva todo el Ministerio. |
| [Gestionar la limpieza](options.md#opt-sanitation) | Activado | Activado / Desactivado | Gestiona automáticamente la financiación de la limpieza. También es necesario para el consejo sobre el tamaño del vertedero y la reparación de casillas fantasma. |
| [Reserva para tratar basura](options.md#opt-sanitationreserve) | 2% | 2–20% | Añade capacidad de eliminación de reserva para absorber aumentos en la producción de basura. |
| [Gestionar acuerdos con vecinos](options.md#opt-deals) | Activado | Activado / Desactivado | Activa la gestión automática de los contratos regionales. |
| [Gestionar acuerdos de basura](options.md#opt-tradegarbage) | Activado | Activado / Desactivado | Activa las importaciones y exportaciones automáticas de basura cuando la gestión de acuerdos con vecinos está activa. |
| [Avisar si revisar el vertedero puede ahorrarme dinero](options.md#opt-landfilladvice) | Desactivado | Activado / Desactivado | Activa las recomendaciones de ampliar o reducir áreas de vertedero cuando es posible un ahorro significativo. |
| [Ahorro mínimo para avisos del vertedero](options.md#opt-landfillthreshold) | 25% | 20–50% | Fija el porcentaje mínimo de ahorro que se exige antes de que el Ministerio recomiende cambiar el tamaño del vertedero. |

#### ¿Cómo funcionan juntas estas opciones?

**Gestionar la limpieza**

Cuando está activada, el Ministerio calcula y ajusta automáticamente el presupuesto de limpieza.

Si está desactivada, la financiación de la limpieza permanece sin cambios. Los contratos regionales de basura aún se pueden gestionar de forma independiente, usando la financiación existente.

El consejo sobre el tamaño del vertedero y la reparación automática de casillas fantasma requieren que la gestión de la limpieza esté activa.

**Reserva para tratar basura**

La reserva por defecto es del **2%**.

Por ejemplo, si tu ciudad produce 100.000 unidades de basura al mes después del reciclaje, el Ministerio planifica gestionar al menos **102.000 unidades**, antes de incluir las importaciones o la basura sin recoger que ya exista.

Aumentar esta reserva ofrece un margen adicional frente al crecimiento de la producción de basura, pero puede exigir una financiación mayor.

**Consejo sobre el tamaño del vertedero**

Esta función está **desactivada por defecto**.

Cuando se activa, el Ministerio puede recomendar:

- **AMPLIAR:** añadir casillas de vertedero si la capacidad de eliminación adicional permitiera una reducción suficientemente grande de los gastos de limpieza.
- **REDUCIR:** quitar casillas de vertedero sin usar si eso rebajara los costes de mantenimiento sin comprometer la capacidad de eliminación.

**Estas recomendaciones nunca cambian automáticamente la zonificación del vertedero.**

La opción de consejo no está disponible en la ventana de ajustes mientras el Ministerio o la gestión de la limpieza estén desactivados, pero su valor guardado se conserva.

**Ahorro mínimo para avisos del vertedero**

El umbral por defecto es del **25%**.

Un cambio propuesto en el vertedero debe ahorrar **más del 25%** del gasto mensual de limpieza optimizado antes de que se emita un aviso del asesor.

El ajuste del umbral no está disponible mientras el consejo sobre el tamaño del vertedero esté desactivado, pero su valor se conserva.

#### Ajustes avanzados (archivo INI)

Estas opciones también se guardan en el archivo de configuración de servicios del Ministerio:

| Clave INI | Finalidad |
|---|---|
| `AutoGarbage` | Financiación automática de la limpieza |
| `GarbageReservePercent` | Reserva de seguridad de la limpieza |
| `AutoTradeGarbage` | Contratos automáticos de basura |
| `WarnLandfillSizeSavings` | Consejo para ampliar y reducir el vertedero |
| `LandfillSavingsAlertThreshold` | Ahorro mínimo exigido para el consejo |

**La reparación automática de casillas fantasma no tiene un ajuste aparte.** Se ejecuta como parte de la gestión de la limpieza, en la versión compatible del juego, siempre que se superen las comprobaciones de seguridad necesarias.

<a id="garbage-landfills-examples"></a>
### 6. Ejemplos prácticos

Los ejemplos siguientes muestran cómo funcionan en la práctica la financiación de la limpieza, el almacenamiento de los vertederos y las reglas de seguridad del Ministerio.

Varios ejemplos usan **Aqua**, una ciudad de prueba en la que se midió el comportamiento de los vertederos del juego.

#### Ejemplo 1: un vertedero con mucho almacenamiento pero una admisión mensual limitada

Imagina un vertedero con:

- **48 casillas de vertedero.**
- **Financiación de la limpieza:** 50%.
- **Eficiencia de la limpieza:** 25%.
- **Almacenamiento disponible:** 900.000 unidades de basura.
- **Basura ya aceptada este mes:** 41.000 unidades.
- **Fecha actual:** día 20 del mes simulado.

**¿Cuánta basura puede aceptar todavía?**

Primero, el Ministerio calcula el límite de admisión mensual:

```math
5000 \times 48 \times 0.25 = 60000
```

El vertedero puede aceptar aproximadamente **60.000 unidades durante todo el mes**.

Sin embargo, 41.000 unidades ya han pasado por la válvula del vertedero.

```math
60000-41000=19000
```

**Solo quedan 19.000 unidades de capacidad de admisión para el mes**, a pesar de que el vertedero tiene 900.000 unidades de almacenamiento libre.

Ahora imagina que subes la financiación de la limpieza al **61%**.

Con esa financiación, la eficiencia pasa a ser del 40,125%.

```math
5000\times48\times0.40125=96300
```

La admisión mensual restante pasa a ser:

```math
96300-41000=55300
```

| Financiación | Límite de admisión mensual | Admisión restante |
|---|---:|---:|
| 50% | 60.000 | 19.000 |
| **61%** | **96.300** | **55.300** |

Una vez que la eficiencia del juego se ha actualizado, la mayor financiación aporta **36.300 unidades adicionales de admisión mensual**.

**La distinción importante:** un vertedero puede tener una enorme capacidad de almacenamiento y, aun así, no poder aceptar más basura porque ha alcanzado su límite de admisión mensual.

Por eso el Ministerio tiene en cuenta tanto el almacenamiento como la admisión al elegir la financiación de la limpieza.

#### Ejemplo 2: contratos de basura justo después de un cambio de mes

Imagina que tu ciudad tiene un contrato para exportar **7.000 unidades de basura al mes**.

Normalmente, el juego exporta:

```math
7000\times0.10=700
```

Es decir, **700 unidades por día simulado**, durante unos diez días.

Sin embargo, SimCity 4 puede mostrar temporalmente el uso del contrato del mes anterior después de que cambie el calendario.

**El primer día de un mes nuevo**, el juego podría seguir mostrando:

*Cantidad usada: 7.000*

Si el Ministerio aceptara ese valor, supondría por error que toda la asignación de exportación del mes nuevo ya se ha agotado.

En cambio, comprueba si la cantidad indicada podría haberse entregado de forma realista desde que empezó el mes.

El día 1:

```math
7000>(1-1)\times700
```

El uso indicado es imposible para el mes nuevo, así que **el Ministerio lo trata como cero**.

Después planifica la exportación de 7.000 unidades a lo largo de los días siguientes.

**¿Qué ocurre el día 4?**

Supongamos que el juego indica 2.100 unidades usadas.

```math
2100\le(4-1)\times700
```

Ese valor es coherente con tres entregas diarias completadas.

**El Ministerio acepta la lectura de 2.100 unidades.**

Esta protección evita que unas estadísticas de contrato desactualizadas provoquen cálculos de limpieza erróneos durante los tres primeros días de un mes.

#### Ejemplo 3: cómo desaparece con el tiempo la basura almacenada

Este ejemplo procede de la **ciudad de prueba Aqua**.

El vertedero tiene:

- **Capacidad por celda de almacenamiento:** 42.000 unidades.
- **Basura almacenada en total:** 985.887 unidades.

SimCity 4 almacena la basura de forma compacta, llenando las celdas antes de empezar otras nuevas.

Por tanto, la basura almacenada ocupa:

- **23 celdas completamente llenas:** 966.000 unidades.
- **Una celda parcialmente llena:** 19.887 unidades.

Al comienzo de cada mes simulado, el juego elimina parte de la basura almacenada mediante su mecanismo de descomposición.

**Descomposición de las celdas llenas**

Cada celda llena pierde como máximo 1.500 unidades al mes.

```math
23\times1500=34500
```

**Descomposición de la celda parcialmente llena**

El juego elimina aproximadamente el 7% de su contenido:

```math
19887\times0.07=1392.09
```

Redondeado a la unidad entera más cercana, son **1.392 unidades**.

**Descomposición mensual total**

```math
D(U)=34500+1392=35892
```

<text strong>Resultado: 35.892 unidades de basura desaparecen del almacenamiento durante la actualización mensual.</text>

Esto ocurre por la mecánica normal de los vertederos del juego, sin ninguna intervención del Ministerio.

**¿Por qué es útil?**

El almacenamiento de un vertedero no es completamente permanente. Cada mes desaparece parte de la basura almacenada, lo que libera capacidad poco a poco.

El Ministerio aprovecha este comportamiento al predecir cuánta basura puede contener un vertedero con el tiempo.

#### Ejemplo 4: reparar casillas fantasma en Aqua

En la ciudad de prueba Aqua, el vertedero contenía **siete casillas fantasma**.

Eran casillas que el juego seguía contando como parte del vertedero aunque ya no estuvieran zonificadas como vertedero.

La estructura registrada del vertedero era:

| Propiedad | Antes de la reparación |
|---|---:|
| Rectángulo delimitador | 6 × 9 casillas |
| Posiciones posibles de celdas de almacenamiento | 40 |
| Celdas de almacenamiento activas | 30 |
| Pertenencia registrada al vertedero | 49 casillas |
| Casillas de vertedero reales | 42 |
| **Casillas fantasma** | **7** |
| Capacidad de almacenamiento por celda activa | 42.000 |
| Capacidad de almacenamiento total | 1.260.000 |
| Mantenimiento mensual con financiación al 100% | §1.030 |

Las siete casillas fantasma estaban situadas a lo largo de la última fila o columna del rectángulo interno del vertedero.

**¿Qué calculó el Ministerio?**

Al eliminar las siete casillas fantasma quedarían:

- 42 casillas de vertedero legítimas.
- 30 celdas de almacenamiento activas.

La capacidad resultante por celda sería:

```math
C'=\frac{42\times30000}{30}=42000
```

Capacidad total:

```math
30\times42000=1260000
```

**La capacidad de almacenamiento seguiría siendo exactamente la misma.**

No se devolvería nada a la válvula de admisión del vertedero.

Además, cada casilla fantasma costaba §5 al mes con la financiación al 100%.

```math
7\times5=35
```

**Ahorro mensual de mantenimiento previsto: §35.**

El Ministerio realizó la reparación automática usando el sistema de zonificación del juego.

**Resultados observados**

| Propiedad | Antes de la reparación | Después de la reparación |
|---|---:|---:|
| Casillas fantasma | 7 | **0** |
| Mantenimiento mensual | §1.030 | **§995** |
| Capacidad de almacenamiento | 1.260.000 | **1.260.000** |

Todos los resultados coincidieron con la predicción del Ministerio.

**Pero las casillas fantasma volvieron al reabrir la ciudad.**

Una copia de Aqua guardada después de la reparación recreó las mismas siete casillas fantasma al cargarse de nuevo.

Esto confirmó el error de guardado y carga del juego descrito anteriormente.

El Ministerio pudo repararlas otra vez durante la nueva sesión de la ciudad.

**La reparación funciona, pero el juego original puede recrear el problema cada vez que se carga la ciudad.**

#### Ejemplo 5: una ampliación que ahorra dinero, pero no lo bastante para recomendarla

El Ministerio también probó a ampliar el vertedero de Aqua.

Al principio, una calle recorría su borde derecho.

Las comprobaciones de zonificación del juego rechazaron todos los candidatos de ampliación de ese lado porque el terreno propuesto estaba ocupado.

**El Ministerio no recomendó ni realizó ninguna ampliación.**

Después de quitar el tramo de calle, pasó a ser posible una **ampliación de 2 × 2**.

Esto añadiría cuatro casillas de vertedero.

La operación de zonificación también eliminaría las siete casillas fantasma existentes.

**Nueva estructura del vertedero**

```math
T'=49+4-7=46
```

El vertedero ampliado tendría 32 celdas de almacenamiento activas.

```math
C'=\frac{46\times30000}{32}=43125
```

Capacidad de almacenamiento total:

```math
32\times43125=1380000
```

La capacidad del vertedero aumentaría de **1.260.000 a 1.380.000 unidades** sin devolver nada de la basura almacenada a la válvula.

**¿Y el mantenimiento?**

Las cuatro casillas vacías nuevas costarían:

```math
4\times5=20
```

Eliminar las siete casillas fantasma ahorraría:

```math
7\times5=35
```

Variación neta del mantenimiento:

```math
\Delta B=20-35=-15
```

El mantenimiento mensual nominal del vertedero bajaría de **§1.030 a §1.015**, a pesar de que el vertedero se hizo más grande.

La ampliación se realizó durante las pruebas, y las cifras observadas en el juego coincidieron con las predicciones.

**¿La recomendaría el Ministerio?**

No.

La reducción estimada de la factura total de limpieza era de solo un **4–5%**.

El umbral de aviso mínimo configurable es del 20%, y el predeterminado es del 25%.

**La ampliación era segura y podía ahorrar dinero, pero el ahorro era demasiado pequeño para justificar un aviso del asesor.**

Esto ilustra por qué el Ministerio no recomienda todos los cambios de zonificación que sean técnicamente beneficiosos.

#### Ejemplo 6: por qué el Ministerio a veces se niega a reparar casillas fantasma

Otro experimento en Aqua mostró por qué la reparación de casillas fantasma tiene que ser conservadora.

El vertedero original tenía:

- **49 casillas registradas**, incluidas siete casillas fantasma.
- **30 celdas de almacenamiento activas.**
- **Capacidad total:** 1.260.000 unidades.

Se eliminó una fila exterior vacía con cinco casillas de vertedero legítimas mientras las casillas fantasma seguían presentes.

Después de esa operación:

- La pertenencia registrada bajó a **44 casillas**.
- Las celdas de almacenamiento activas bajaron a **25**.

La nueva capacidad por celda pasó a ser:

```math
C'=\frac{44\times30000}{25}=52800
```

Capacidad de almacenamiento total:

```math
25\times52800=1320000
```

**Sorprendentemente, la capacidad total del vertedero aumentó hasta 1.320.000 unidades**, a pesar de haberse eliminado cinco casillas.

Esto ocurrió porque las casillas fantasma seguían contando en la pertenencia registrada que se usa en el cálculo de la capacidad.

**¿Qué pasaría si el Ministerio eliminara a continuación las siete casillas fantasma?**

La pertenencia registrada bajaría de 44 a 37 casillas.

Con las mismas 25 celdas activas:

```math
C'=\frac{37\times30000}{25}=44400
```

La capacidad de cada celda bajaría de 52.800 a 44.400 unidades.

El modelo del Ministerio predijo que **151.200 unidades de basura almacenada serían devueltas a la válvula de admisión del vertedero**.

Eso perturbaría el sistema de limpieza.

**Por eso el Ministerio rechazó la reparación de las casillas fantasma y dejó el vertedero sin cambios.**

Este es un principio de seguridad importante:

**Solo se permite eliminar casillas fantasma cuando el Ministerio puede demostrar que la operación no reducirá la capacidad de almacenamiento ni alterará la basura existente.**

Ahorrar unos pocos simoleones no compensa arriesgar los residuos almacenados en un vertedero.

#### Ejemplo 7: ¿cuándo aparece un aviso de ahorro del vertedero?

Supongamos que el Ministerio calcula que el presupuesto de limpieza actual más barato es de:

**§1.015 al mes.**

Después encuentra un cambio de tamaño del vertedero que sería seguro y que reduciría el gasto optimizado a:

**§760 al mes.**

El ahorro previsto es:

```math
1015-760=255
```

Es decir, **§255 ahorrados cada mes simulado**.

Con el umbral de aviso predeterminado del 25%, el Ministerio comprueba:

```math
255\times100>25\times1015
```

```math
25500>25375
```

Se cumple la condición.

**El Ministerio emite una recomendación.**

El ahorro real es de aproximadamente un **25,12%**, aunque el aviso muestra **25%** porque el porcentaje se redondea hacia abajo.

| Umbral de aviso | Resultado |
|---|---|
| 20% | Se emite la recomendación |
| **25% (predeterminado)** | **Se emite la recomendación** |
| 30% | Sin recomendación |

**Un detalle importante:** la comparación usa el ahorro real antes de redondear el porcentaje que se muestra.

Por eso un aviso puede mostrar 25% aunque la regla exija un ahorro estrictamente superior al 25%.

<a id="garbage-landfills-limitations"></a>
### 7. Limitaciones conocidas

El modelo de limpieza del Ministerio se basa en observaciones detalladas de SimCity 4, pero siguen existiendo varias limitaciones.

#### Producción de basura y financiación

- **La producción futura de basura es una estimación.** El Ministerio usa la producción del mes anterior menos el reciclaje y supone que la cantidad resultante se produce de forma uniforme durante los días que quedan del mes en curso y el siguiente. No predice el crecimiento futuro de la ciudad. Los cambios se incorporan cuando hay información nueva.
- **La financiación puede cambiar durante el mes.** Las importaciones y exportaciones de basura y la admisión de los vertederos no siguen un calendario perfectamente uniforme. Por eso el nivel de financiación más barato puede cambiar a medida que avanza el mes. En Aqua, las decisiones observadas incluyeron **47%, 63% y 53%** en momentos distintos. Es un comportamiento esperado, no necesariamente un fallo.
- **El Ministerio usa las curvas de limpieza del juego original.** Sus predicciones suponen la curva de eficiencia estándar, una admisión de los vertederos de 5.000 unidades por casilla y mes, un almacenamiento de aproximadamente 30.000 unidades por casilla, las reglas de descomposición estándar y un mantenimiento de §5 por casilla vacía. Los mods que cambian esta mecánica pueden hacer que los cálculos sean inexactos. Si la eficiencia del juego no coincide con el valor esperado, el Ministerio deja de hacer cambios de financiación en lugar de seguir con datos poco fiables.

#### Predicción del mes siguiente

- **La comprobación del mes siguiente requiere una geometría del vertedero calibrada.** Normalmente necesita una sola zona de vertedero y una observación mensual con al menos una celda de almacenamiento llena.
- **La calibración no es permanente.** El Ministerio vuelve a aprender la geometría después de reabrir la ciudad y reinicia la calibración cuando cambia la capacidad del vertedero.
- **La predicción supone que se mantiene la financiación seleccionada.** No anticipa todos los cambios futuros en la producción de basura ni en los contratos regionales.
- **El modelo de la válvula del vertedero tiene una limitación.** Cuando la válvula ya está llena, parte de la basura desbordada puede no incluirse en la estimación del mes siguiente. En esas situaciones, el Ministerio puede subestimar cuánta basura queda en la válvula en el cambio de mes.

Si no hay una calibración fiable, el Ministerio usa solo su proyección del mes en curso.

#### Consejo sobre el tamaño del vertedero

- **Solo son elegibles las ciudades con exactamente una zona de vertedero.** El Ministerio no ofrece recomendaciones de AMPLIAR ni REDUCIR cuando existen varias zonas de vertedero.
- **Las áreas de ampliación están restringidas.** Solo se consideran áreas seguras a lo largo de los bordes derecho o inferior de las coordenadas internas del mapa del vertedero, con una profundidad máxima de ocho casillas y una superficie máxima de 64 casillas.
- **Las reducciones se limitan a filas o columnas exteriores sin usar.** Las reducciones propuestas deben conservar la estructura del vertedero restante y la basura almacenada.
- **Los ahorros son estimaciones.** Las casillas de vertedero nuevas se tratan al principio como vacías, con un coste de §5 cada una con la financiación al 100%. Su mantenimiento puede aumentar más adelante cuando aparezcan montones de basura.
- **No se incluyen los gastos únicos de zonificación.** El Ministerio compara los costes mensuales recurrentes de la limpieza, no el coste inicial de cambiar la zona.
- **En las ciudades de prueba, los ahorros grandes son poco frecuentes.** Las oportunidades observadas se mantuvieron por lo general muy por debajo del umbral de aviso mínimo del 20%, así que el consejo sobre el tamaño del vertedero puede aparecer rara vez.

#### Reparación de casillas fantasma

- **Algunas casillas fantasma vuelven después de cada carga de la ciudad.** El error de guardado y carga del juego original puede recrearlas a lo largo de la última fila o columna del rectángulo interno de un vertedero.
- **La reparación no puede corregir de forma permanente el formato de guardado del juego.** Puede que el Ministerio tenga que realizar la misma corrección segura en cada sesión nueva.
- **Se rechazan las reparaciones de casillas fantasma que no sean seguras.** Una casilla fantasma queda intacta si eliminarla podría reducir la capacidad del vertedero, alterar la basura almacenada o devolver residuos a la válvula de admisión.
- **Dejar una casilla fantasma puede seguir costando dinero.** Cada casilla fantasma sin reparar sigue aportando §5 al mes con la financiación de la limpieza al 100%.
- **Las zonas de vertedero múltiples se tratan por separado.** El Ministerio puede intentar reparar casillas fantasma de forma segura zona por zona, pero normalmente no puede usar la predicción del mes siguiente de geometría única ni ofrecer consejo sobre el tamaño del vertedero.

#### La eliminación oculta de basura del juego

SimCity 4 puede eliminar en silencio una cantidad limitada de basura mediante su mecanismo integrado de *abducción*.

**El Ministerio no trata esa eliminación como un fallo de la limpieza y no intenta impedirla.**

Su objetivo es evitar las acumulaciones reales y persistentes de basura sin recoger, no cambiar todas las reglas ocultas de eliminación del juego original.

**El Ministerio da prioridad a la eliminación segura de la basura y a decisiones de financiación fiables. Cuando no puede predecir con la confianza suficiente una operación del vertedero o un cambio en la limpieza, deja la situación existente sin cambios.**

[Volver al contenido](index.md#contents) | [Volver al README](../../README.es.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
