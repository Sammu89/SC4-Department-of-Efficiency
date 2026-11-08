[English](../en/safety.md) | [Português](../pt/safety.md) | [Français](../fr/safety.md) | **Español**

[Volver al contenido](index.md#contents) | [Volver al README](../../README.es.md)

<a id="safety"></a>
## G. Seguridad, automatización y toma de decisiones

El Ministerio puede cambiar presupuestos, gestionar contratos regionales, despejar edificios abandonados y corregir ciertos problemas de vertederos. Como estas acciones afectan directamente a tu ciudad, **proteger los servicios esenciales y respetar tus decisiones debe pasar siempre por delante de ahorrar dinero**.

Este capítulo explica las reglas que comparten las funciones automáticas del Ministerio:

- Cómo decide qué objetivos tienen prioridad.
- Cuándo lee la ciudad y recalcula sus planes.
- Cómo verifica los cambios antes de continuar.
- Qué ocurre cuando la información del juego está incompleta o es incoherente.
- Cómo se contienen los fallos técnicos.
- Qué decisiones siguen estando bajo tu control.
- Cómo funcionan sus acciones automáticas y los mensajes de los asesores.

<a id="safety-vanilla"></a>
### 1. Cómo funciona SimCity 4 sin el Ministerio

En el juego original, la mayoría de las decisiones de financiación de los servicios y de comercio regional son manuales.

#### Presupuestos de los servicios

Las escuelas, los hospitales y las centrales eléctricas tienen controles de financiación individuales.

El agua y la limpieza tienen cada una un único presupuesto de departamento.

SimCity 4 acepta cambios de financiación mediante estos controles, pero **no los optimiza automáticamente a medida que cambia la demanda**.

#### Estadísticas del juego

La simulación actualiza la demanda, la producción, el consumo, la capacidad y los contratos regionales según su propio calendario interno.

Estos valores no cambian necesariamente de inmediato después de ajustar un presupuesto.

Por ejemplo:

- La capacidad de producción de agua puede actualizarse en el siguiente mes simulado.
- La eficiencia de la limpieza y las capacidades de tratamiento pueden tardar varios días simulados en reflejar la nueva financiación.
- Algunas estadísticas pueden estar a cero al abrir una ciudad porque la simulación todavía no las ha rellenado.

**El Ministerio debe distinguir entre un cero genuino y una información que aún no está lista.**

#### Mensajes de los asesores

SimCity 4 tiene un sistema de asesores que muestra noticias y avisos.

Cada mensaje tiene un asesor, un estado de ánimo y una prioridad.

Según la dificultad del juego o los ajustes de los asesores, los mensajes de alta prioridad pueden aparecer como ventanas emergentes.

Los umbrales de prioridad del juego para las ventanas emergentes son **60, 80 o 100**, según el nivel de dificultad elegido.

Los mensajes por debajo del umbral aparecen normalmente en la lista de noticias del asesor, sin abrir una ventana emergente.

Cada mensaje sigue disponible durante la duración configurada.

#### Contratos regionales

El juego original también ofrece, renueva y cancela contratos regionales, con sus propios mensajes de asesor.

El Ministerio puede automatizar esas operaciones cuando está activado.

<a id="safety-problem"></a>
### 2. El problema de la gestión

La gestión automática puede ahorrar dinero y reducir el trabajo repetitivo, pero también puede crear problemas si las decisiones se toman sin las salvaguardas suficientes.

Por ejemplo, un sistema automatizado podría:

- Reducir la financiación por debajo del nivel necesario para mantener un servicio.
- Vender electricidad o agua que la ciudad necesita de verdad.
- Tomar decisiones con estadísticas incompletas justo después de cargar.
- Aplicar otro cambio antes de que el anterior haya surtido efecto.
- Repetir una operación de contrato o de financiación que el juego ya ha rechazado.
- Demoler edificios que deberían estar protegidos.
- Inundar al jugador de notificaciones sobre acciones rutinarias.

**El Ministerio está diseñado para evitar primero estos problemas y optimizar los gastos en segundo lugar.**

<a id="safety-principles"></a>
### 3. Las reglas generales del Ministerio

Cuando varios objetivos entran en conflicto, el Ministerio sigue este orden de prioridades:

1. **Mantener los servicios esenciales.** Evitar dejar la ciudad sin electricidad, agua, limpieza, educación o sanidad.
2. **Proteger las reservas configuradas.** Mantener una capacidad suficiente por encima de las necesidades previstas de la ciudad.
3. **Respetar los límites de seguridad de la reserva.** No saltarse las reglas pensadas para proteger la continuidad del servicio.
4. **Respetar las restricciones de SimCity 4.** Usar solo los niveles de financiación, las capacidades y las cantidades de contrato que permite el juego.
5. **Minimizar los gastos mensuales.** Elegir la configuración de funcionamiento más barata que sea segura.
6. **Vender el excedente genuino cuando sea rentable.** Los ingresos adicionales deben justificar el coste de generarlos.
7. **Evitar comprar y vender a la vez por defecto.** El comercio mixto requiere el ajuste opcional.
8. **No inventar nunca información que falta.** Los valores ilegibles o incoherentes no se tratan como mediciones válidas.
9. **Evitar retrasos innecesarios.** Una decisión segura y lista no recibe un tiempo de enfriamiento artificial.
10. **Verificar cada cambio.** Leer el nuevo estado, confirmar el resultado y recalcular antes de continuar.

Estos principios se aplican a los gestores individuales del Ministerio, con las salvaguardas específicas de cada recurso descritas en los capítulos anteriores.

<a id="safety-service-first"></a>
#### Los servicios esenciales van siempre primero

El Ministerio coordina la financiación y los contratos regionales para evitar interrumpir el suministro durante una transición.

#### Los cambios se aplican en un orden seguro

Para la electricidad, el agua y la basura, el Ministerio usa seis fases de ejecución:

| Fase | Acción |
|---|---|
| **1** | Aumentar la capacidad local |
| **2** | Reducir o cancelar ventas |
| **3** | Crear o aumentar compras |
| **4** | Crear o aumentar ventas |
| **5** | Reducir la capacidad local |
| **6** | Reducir o cancelar compras |

**Cada recurso ejecuta solo su fase pendiente más temprana durante una revisión.**

Por ejemplo, el Ministerio no reduce la financiación de una central eléctrica antes de que esté activa la compra de electricidad destinada a sustituir esa producción.

Si un aumento de capacidad todavía no ha surtido efecto, las fases siguientes de ese recurso esperan.

Hay una distinción importante para la limpieza: una reducción de financiación puede aplazarse, porque las escrituras de financiación de la limpieza son deliberadamente graduales.

**Una reducción de la limpieza sin terminar no impide necesariamente una reducción posterior de las compras de eliminación de basura**, porque mantener una mayor capacidad local de eliminación no puede perjudicar la continuidad del servicio.

#### La financiación no se reduce durante una escasez genuina

El Ministerio distingue entre un suministro insuficiente y los problemas dentro de la red de distribución de la ciudad.

En el caso del agua, por ejemplo, rechaza una reducción cuando a los edificios les falta agua **y la capacidad de producción está por debajo de la demanda**.

Si hay agua suficiente pero algunos edificios están desconectados de la red de tuberías, aumentar la financiación no resolvería el problema. Un problema de conexión así no impide automáticamente una reducción que, por lo demás, sea segura.

Consulta [Condiciones de seguridad del agua](water.md#water-no-cut).

#### Las ventas se detienen durante déficits confirmados de electricidad o agua

Si los contadores del juego confirman que la ciudad no tiene suficiente electricidad o agua, **el Ministerio no mantiene las ventas de ese recurso**.

La demanda local tiene prioridad sobre los ingresos por exportación.

#### Educación y sanidad suben la financiación con rapidez

La educación y la sanidad usan una estrategia de plazos distinta.

El Ministerio puede subir la financiación:

- Durante el primer día simulado después de abrir la ciudad.
- Cuando se detecta un riesgo para la capacidad del servicio.
- Durante comprobaciones adicionales mientras haya una huelga o un riesgo persistente.

**Las reducciones de financiación esperan a la evaluación mensual.**

Esto permite al Ministerio responder rápido a escuelas u hospitales saturados y evitar reducciones innecesarias entre las revisiones mensuales.

Consulta [Educación y sanidad](education_healthcare.md#education-healthcare-timing).

<a id="safety-reserves"></a>
#### Reservas de seguridad: capacidad mínima, no máxima

Cada servicio gestionado tiene una reserva configurable.

**Una reserva es un requisito mínimo, nunca un máximo.**

El Ministerio puede mantener más capacidad cuando sea necesario por los ajustes de financiación disponibles, las fluctuaciones del servicio o las ventas regionales rentables.

No debe elegir deliberadamente un plan que quede por debajo del requisito protegido aplicable.

| Servicio | Reserva por defecto | Rango permitido | Cómo funciona |
|---|---:|---:|---|
| **Educación y sanidad** | 5% | 0–50% | Añade capacidad libre por encima del uso, con protección adicional para instalaciones pequeñas o con mucha fluctuación. |
| **Electricidad** | 20% | 10–50% | Protege la parte de la demanda que cubren las centrales eléctricas locales, respetando además el requisito de electricidad del juego. |
| **Agua** | 10% | 10–50% | Protege la parte de la demanda que cubren las instalaciones locales de agua. |
| **Limpieza** | 2% | 2–20% | Añade un margen a la basura producida tras el reciclaje, junto con la basura sin recoger que ya exista. |

Consulta [Educación y sanidad](education_healthcare.md#education-healthcare-required), [Electricidad](power.md#power), [Agua](water.md#water-required-capacity) y [Limpieza y vertederos](garbage.md#garbage-landfills-need).

**¿Y si el valor de una reserva no es válido?**

Si editas un archivo de configuración a mano e introduces una reserva fuera de su rango permitido, el Ministerio no ajusta el valor en silencio.

**La configuración no válida se rechaza, y el componente de gestión afectado permanece desactivado hasta que se corrija.**

Así se evita aplicar márgenes de seguridad inesperados que tú en realidad no has configurado.

<a id="safety-economy"></a>
#### ¿Cómo minimiza los gastos el Ministerio?

Una vez cumplidos los requisitos del servicio y de seguridad, el Ministerio busca la configuración adecuada más barata.

Cada servicio usa métodos distintos porque SimCity 4 gestiona su financiación de manera diferente.

| Servicio | Método de optimización |
|---|---|
| **Educación y sanidad** | Elegir el porcentaje de financiación entero más bajo que proporcione la capacidad necesaria. |
| **Electricidad** | Encontrar la combinación más barata de niveles de financiación de cada central eléctrica. |
| **Agua** | Elegir el porcentaje de financiación del departamento más bajo que sea seguro. |
| **Limpieza** | Simular la eliminación de basura durante los días restantes del mes y encontrar la financiación más barata que pueda manejar los residuos. |
| **Contratos regionales** | Comparar los ingresos y gastos del contrato con el coste real de la producción o eliminación local. |

#### ¿Con qué rapidez se aplican los cambios de financiación?

**Electricidad y agua**

El Ministerio aplica directamente el porcentaje de financiación elegido, sin un límite artificial por pasos.

Las comprobaciones de seguridad aplicables y la coordinación con los contratos regionales siguen vigentes.

**Limpieza**

Las reducciones de financiación son deliberadamente graduales:

- Como máximo **25 puntos porcentuales** por reducción.
- Al menos **7 días simulados** entre reducciones de financiación de la limpieza.
- Al menos **3 días simulados** desde la escritura de financiación anterior de la limpieza antes de un aumento.

Estos retrasos permiten que la eficiencia de la limpieza y la capacidad de tratamiento del juego se pongan al día con los cambios de financiación.

**Contratos regionales**

El Ministerio crea, cambia o cancela contratos solo cuando el plan resultante está justificado económicamente y cumple los requisitos de servicio correspondientes.

Consulta [Acuerdos con vecinos](deals.md#neighbor-deals-objective).

<a id="safety-physical"></a>
#### Respetar los límites físicos del juego

El Ministerio no anula las restricciones normales de funcionamiento de SimCity 4.

Usa:

- Los porcentajes de financiación que acepta el juego.
- Opciones de financiación en números enteros cuando se exige.
- El rango configurado de educación y sanidad, normalmente **1–120%**.
- El mínimo de protección de las tuberías de agua cuando esa opción está activada.
- Los límites reales de almacenamiento y de admisión de los vertederos.
- Las cantidades de contrato que permite el juego.
- Los precios que devuelven las propias funciones de contratos y de demolición del juego.

**El Ministerio nunca construye edificios ni infraestructuras automáticamente.**

Tampoco:

- Crea zonificación ordinaria.
- Cambia los tipos de impuestos.
- Cambia las ordenanzas de la ciudad.
- Ajusta presupuestos no relacionados.
- Modifica las ciudades vecinas.

La única excepción relacionada con la zonificación es la [reparación de casillas fantasma](garbage.md#ghost-tiles), estrechamente controlada, que usa el propio sistema de zonificación del juego para corregir la pertenencia errónea a un vertedero sin cambiar deliberadamente la zonificación legítima de los vertederos.

<a id="safety-reevaluation"></a>
#### ¿Cuándo recalcula sus decisiones el Ministerio?

El Ministerio combina revisiones periódicas con comprobaciones más ligeras para detectar cambios significativos.

**No espera un retraso fijo arbitrario después de abrir una ciudad.**

Cada servicio empieza a evaluar en cuanto la información que necesita está disponible, actualizada y es coherente.

La demolición es una excepción: empieza en el primer cambio de mes simulado después de cargar.

#### Comprobar si las estadísticas del juego son fiables

Antes de tomar decisiones sobre electricidad o agua, el Ministerio compara la demanda informada con el consumo real.

Una pequeña diferencia puede deberse al redondeo propio del juego o al momento de actualización.

Sin embargo, las lecturas claramente contradictorias impiden las decisiones automáticas de financiación.

**Lecturas de servicio incoherentes**

```math
R>D\times1.01+1
```

Donde:

- **D** es la demanda informada por el juego.
- **R** es la cantidad del servicio informada como entregada o consumida.

Si el consumo supera la demanda en más de un **1% más una unidad**, las lecturas se consideran incoherentes.

El Ministerio espera datos nuevos en lugar de actuar sobre un estado poco fiable.

Otras señales de que la simulación no está lista son:

- La demanda sigue marcando cero justo después de cargar.
- Los recuentos de edificios con agua y sin agua todavía no están rellenados.
- Falta información legible de financiación o producción de las centrales eléctricas cuando la gestión de su financiación la necesita.

#### Comprobaciones diarias de los servicios

Cada día simulado, el Ministerio lee un pequeño número de contadores de electricidad y agua.

Estas comprobaciones son deliberadamente ligeras.

No requieren recorrer todos los edificios ni reconstruir todo el mercado de contratos regionales.

**Una lectura idéntica no se trata como una observación nueva.**

El Ministerio solo activa una evaluación anticipada cuando algo relevante ha cambiado de verdad.

#### Detectar una escasez genuina de suministro regional

Una ciudad puede informar de un consumo insuficiente por dos razones muy distintas:

1. No tiene suficiente electricidad o agua disponible.
2. Tiene suficiente suministro, pero el servicio no llega a algunos edificios.

El Ministerio usa la siguiente prueba para distinguirlas.

**Déficit de suministro regional**

```math
S<D
\qquad\text{and}\qquad
C+I-E<D
```

Donde:

- **D** es la demanda de la ciudad.
- **S** es el servicio realmente entregado.
- **C** es la capacidad de producción local.
- **I** es el suministro importado utilizable.
- **E** es el suministro exportado.

Todas las cantidades usan las unidades mensuales del recurso correspondiente.

**Existe una escasez genuina cuando el consumo está por debajo de la demanda y el suministro utilizable combinado también es insuficiente.**

Por ejemplo, si una ciudad necesita 20.000 unidades pero solo tiene 16.000 disponibles tras las importaciones y exportaciones, el Ministerio puede reconsiderar de inmediato la financiación y los contratos.

Sin embargo, supón que la ciudad necesita 20.000 unidades y tiene 25.000 unidades de capacidad disponible, pero algunos edificios siguen sin servicio.

Lo más probable es que el problema esté dentro de la ciudad, por ejemplo en líneas eléctricas o tuberías de agua desconectadas.

**El Ministerio registra el problema, pero no replanifica la producción innecesariamente solo porque a los edificios les falte una conexión.**

#### Detectar cambios significativos antes de la siguiente revisión periódica

El Ministerio también puede reaccionar cuando la demanda, la capacidad local, las importaciones o las exportaciones cambian al menos un **1%**.

**Umbral de cambio significativo**

```math
|x-x_0|\ge0.01\times\max(1,|x_0|)
```

Donde:

- **x** es la última lectura.
- **x₀** es el valor registrado en la última evaluación.

**Ejemplo**

Supón que la demanda de electricidad era de 50.000 unidades durante la revisión anterior.

| Nueva demanda | Cambio | Resultado |
|---|---:|---|
| 50.400 | 0,8% | Esperar a la revisión normal |
| **50.600** | **1,2%** | **Activar una evaluación anticipada** |

Una nueva lectura significativa puede adelantar la siguiente evaluación sin esperar al intervalo periódico.

**No hay ningún tiempo de enfriamiento adicional para una decisión que está lista y es segura de aplicar.**

#### Intervalos de evaluación periódicos

| Servicio | Cuándo se evalúa |
|---|---|
| **Electricidad** | Cada 10 días simulados, más revisiones anticipadas por escaseces confirmadas o cambios significativos. |
| **Agua** | Cada 10 días simulados desde su última revisión, en cada cambio de mes y antes cuando sea necesario. |
| **Limpieza** | En cada cambio de mes y cuando las cifras de basura vigiladas cambian de forma significativa. |
| **Educación y sanidad** | Subidas iniciales tras la apertura, comprobaciones de riesgo cada dos días (a diario durante las huelgas), nuevas comprobaciones cada siete días mientras persistan los riesgos y revisiones mensuales para aumentos y reducciones. |
| **Demolición automática** | En cada cambio de mes simulado. |
| **Consejo sobre el tamaño del vertedero** | Como máximo una vez por mes simulado, durante una evaluación ordinaria de la limpieza. |

**¿Qué ocurre mientras se sigue verificando un cambio anterior?**

El Ministerio no hace cambios superpuestos en el mismo recurso.

Por ejemplo:

- Un cambio de contrato regional debe confirmarse antes de la siguiente operación de contrato.
- Un ajuste de financiación de la limpieza debe alcanzar la eficiencia y la capacidad de tratamiento esperadas antes de hacer otro ajuste.

Mientras espera, el Ministerio sigue comprobando los valores relevantes del juego.

**No realiza una optimización completa nueva solo porque toque una comprobación de verificación.**

Esto evita que las operaciones a medio completar interfieran entre sí y mantiene el sistema receptivo a los cambios genuinos.
<a id="safety-verification"></a>
#### ¿Cómo verifica el Ministerio sus cambios?

El Ministerio no da por hecho que una operación ha tenido éxito solo porque haya enviado una orden a SimCity 4.

**Cada cambio automático se comprueba antes y después de aplicarse.**

La verificación depende del tipo de operación.

| Operación | Qué verifica el Ministerio |
|---|---|
| **Financiación de escuelas y hospitales** | La nueva financiación coincide con el valor solicitado, y el presupuesto del autobús escolar o de la ambulancia no ha cambiado. |
| **Financiación de centrales eléctricas y del agua** | El juego acepta el valor de financiación solicitado. |
| **Financiación de la limpieza** | La nueva financiación se acepta y, después, la eficiencia y las capacidades de tratamiento del juego se actualizan como se espera. |
| **Contratos regionales** | El estado, la cantidad, el precio y la contabilidad regional del contrato concuerdan con el cambio solicitado. |
| **Demolición automática** | El edificio ha desaparecido y su zonificación subyacente sigue intacta. |

#### Antes de aplicar un cambio

El Ministerio vuelve a leer el edificio, el departamento o el contrato.

El valor debe seguir coincidiendo con el que se usó al calcular el plan.

Por ejemplo, supón que el Ministerio planea reducir la financiación del agua del 100% al 78%.

Antes de aplicar el cambio, comprueba que la financiación sigue siendo del 100%.

Si tú mismo moviste el control deslizante al 90%, **el plan antiguo se cancela en lugar de aplicarse sobre tu nuevo ajuste**.

Entonces el Ministerio puede calcular un plan nuevo con la información actual.

#### Después de aplicar un cambio

El Ministerio lee el valor resultante.

En los cambios de financiación, se debe confirmar el porcentaje solicitado.

En escuelas y hospitales, comprueba además que el presupuesto aparte del autobús o la ambulancia no ha cambiado.

Los contratos regionales requieren una confirmación adicional porque los registros de contratos y los totales financieros del juego pueden actualizarse después de la operación.

La limpieza también requiere una comprobación posterior, porque su eficiencia y sus capacidades de tratamiento no responden necesariamente de inmediato a un cambio de financiación.

Las demoliciones se verifican en un escaneo posterior, cuando el juego ha tenido tiempo de ejecutar la orden.

**Un cambio no se considera exitoso solo porque se haya enviado la orden.**

El Ministerio distingue entre:

- **Planificado:** un cambio se ha calculado pero no se ha aplicado.
- **Pendiente:** un cambio se ha enviado y espera confirmación.
- **Verificado:** el juego informa del resultado esperado.

Los ahorros se cuentan como confirmados solo cuando se ha verificado el resultado correspondiente.

<a id="safety-missing-data"></a>
#### ¿Qué ocurre cuando el juego no proporciona suficiente información?

El Ministerio sigue una regla sencilla:

**La información desconocida no es lo mismo que cero.**

Por ejemplo:

- Una lista válida que no contiene centrales eléctricas significa que no hay centrales eléctricas en esa lista.
- Un intento fallido de leer la lista de centrales eléctricas no demuestra que la ciudad no tenga centrales eléctricas.
- Un vertedero con una capacidad de admisión confirmada de cero no puede aceptar más basura.
- Un vertedero cuya capacidad de admisión no se puede leer tiene una capacidad de admisión desconocida, no un cero confirmado.

El Ministerio trata estas situaciones de forma distinta.

#### Lecturas incompletas o no válidas

Si parte de la información necesaria no está disponible, el Ministerio usa solo valores que se han leído y verificado de verdad.

Cuando un valor ausente o no válido hace que el cálculo no sea fiable, **el servicio afectado no realiza ningún cambio de financiación ni de contrato durante esa evaluación**.

No sustituye la información ausente por un valor anterior en caché ni inventa una estimación verosímil.

Los demás servicios pueden seguir funcionando de forma independiente.

#### Ciudades vecinas u ofertas desconocidas

Si no se pueden leer las ofertas regionales o los registros de las ciudades vecinas, el Ministerio no supone que no exista un suministro alternativo.

Por ejemplo, no puede concluir que una escasez de electricidad sea imposible de resolver si no ha podido inspeccionar las compras regionales disponibles.

**Un mercado de contratos incompleto no puede sustentar un aviso definitivo de escasez.**

#### Geometría del vertedero desconocida

El Ministerio puede calcular la financiación de la limpieza sin conocer todos los detalles de la estructura interna de celdas del vertedero.

Hasta que esa estructura se ha calibrado, usa los cálculos conservadores descritos en [Limpieza y vertederos](garbage.md#garbage-landfills-algorithm).

No inventa capacidad de almacenamiento ni hace predicciones del mes siguiente que requieran información no disponible.

#### Precios de demolición desconocidos

Si SimCity 4 no puede proporcionar el precio del bulldozer de una parcela, **el Ministerio no la demuele**.

La operación puede reconsiderarse en una revisión mensual posterior.

<a id="safety-player-control"></a>
#### Tú sigues al mando

El Ministerio automatiza decisiones de gestión repetitivas, pero tú puedes elegir qué funciones controla.

| Lo que quieres hacer | Cómo hacerlo |
|---|---|
| **Desactivar todo el Ministerio** | Desactiva **Ministerio activo**, o deroga **Crear un Ministerio de Optimización** en la ventana de Ordenanzas del juego. |
| **Gestionar un servicio manualmente** | Desactiva la opción de gestión de ese servicio, como **Gestionar la electricidad** o **Gestionar la educación**. |
| **Gestionar los contratos regionales manualmente** | Desactiva **Gestionar acuerdos de electricidad**, **Gestionar acuerdos de agua** o **Gestionar acuerdos de basura** para el recurso correspondiente. |
| **Cambiar tú mismo un control deslizante de financiación** | Muévelo con normalidad. El Ministerio usará tu nuevo valor en su próxima evaluación si la gestión sigue activada. |
| **Crear o cancelar un contrato tú mismo** | Usa la ventana normal de Acuerdos con vecinos. El Ministerio tendrá en cuenta los contratos resultantes en su próxima revisión. |
| **Detener la demolición automática** | Desactiva tanto **Demoler edificios destruidos** como **Demoler edificios abandonados**, o desactiva todo el Ministerio. |
| **Quitar el plugin** | Cierra SimCity 4 y elimina los archivos del plugin. Tus ciudades guardadas siguen siendo utilizables sin él. |

#### ¿Qué ocurre cuando desactivas el Ministerio?

**Desactivar el Ministerio no deshace los cambios anteriores.**

Por ejemplo, si antes redujo la financiación de la electricidad al 72%, apagarlo no restablece la financiación anterior.

Asimismo, los contratos regionales permanecen en su estado actual.

El Ministerio simplemente deja de hacer más cambios automáticos.

#### ¿Qué ocurre cuando mueves un control deslizante manualmente?

Siempre puedes mover un control deslizante de financiación.

Sin embargo, **un valor manual no queda bloqueado mientras su gestor esté activado**.

El Ministerio trata tu nuevo valor como punto de partida de su siguiente cálculo.

Según el servicio:

- **Electricidad:** la financiación puede recalcularse en cuestión de días, como mucho en la revisión periódica de 10 días.
- **Agua:** la financiación puede recalcularse en un plazo de 10 días simulados.
- **Educación y sanidad:** una reducción manual que crea un riesgo para la capacidad puede corregirse en pocos días. Un aumento manual se revisa durante la pasada mensual.

Por ejemplo, si subes la financiación del agua al 100% pero dejas activada la opción **Gestionar el agua**, el Ministerio puede volver a bajarla cuando sus cálculos muestren que basta con menos financiación.

**Para mantener bajo tu control un valor de financiación concreto, desactiva el gestor de ese servicio.**

#### ¿Hay efectos que no se pueden desactivar individualmente?

Sí.

En la versión del juego compatible, el plugin oculta el icono de aviso original de *sin agua* sobre las parcelas agrícolas siempre que el plugin está cargado.

Esto no cambia el servicio de agua en sí.

Consulta [Agua](water.md#water-farm-icon).

El Ministerio también incluye tres archivos de recursos que modifican algunos de los mensajes de asesor originales de SimCity 4.

Esos archivos actúan siempre que están instalados, con independencia de las opciones de gestión correspondientes. Su comportamiento se explica en [Avisos de los asesores](#safety-notices-game).

<a id="safety-automation"></a>
#### ¿Qué funciones actúan automáticamente y cuáles solo aconsejan?

Algunas funciones del Ministerio modifican la ciudad directamente.

Otras solo muestran información para que tú decidas qué hacer.

| Función | Acción automática | Aviso del asesor |
|---|---|---|
| **Educación y sanidad** | Ajusta la financiación | Avisa cuando una financiación adicional no puede resolver un problema persistente de capacidad |
| **Electricidad** | Ajusta la financiación de las centrales eléctricas | Avisa de una escasez persistente que no se puede resolver automáticamente |
| **Agua** | Ajusta la financiación del departamento | Avisa de una escasez persistente que no se puede resolver automáticamente |
| **Limpieza** | Ajusta la financiación del departamento | Avisa de una escasez persistente en la eliminación de basura |
| **Contratos con vecinos** | Crea, cambia y cancela contratos | Sin aviso rutinario de contratos aparte |
| **Demolición automática** | Retira ruinas y edificios abandonados elegibles | Sin aviso |
| **Casillas fantasma de vertedero** | Repara automáticamente las casillas fantasma elegibles | Sin aviso |
| **Consejo sobre el tamaño del vertedero** | **Nunca cambia el tamaño del vertedero** | Recomienda opcionalmente una ampliación o una reducción concretas |

**El consejo sobre el tamaño del vertedero es la principal función que recomienda un cambio sin llevarlo a cabo.**

Puede decirte que ampliar o reducir un vertedero podría ahorrar dinero, pero la decisión de zonificación sigue siendo tuya.

<a id="safety-performance"></a>
#### ¿Cómo evita el Ministerio ralentizar el juego?

El Ministerio está diseñado para hacer solo el trabajo necesario en cada momento de la simulación.

**Todo el trabajo de simulación se ejecuta en el propio hilo del juego, entre fotogramas, usando sus eventos diarios y mensuales normales.**

No crea una simulación independiente en segundo plano.

La carga de trabajo se reparte así:

| Operación | Frecuencia o enfoque |
|---|---|
| **Vigilancia diaria de los servicios** | Lee un pequeño número de contadores del juego. |
| **Optimización completa de recursos** | Se ejecuta solo cuando toca evaluar el servicio o se produce un cambio relevante. |
| **Escaneo de parcelas para demolición** | Lee toda la cuadrícula de parcelas una vez por mes simulado. |
| **Verificación de demoliciones** | Realiza un escaneo ligero cada 10 días simulados, leyendo un valor de estado por parcela y registrando los cambios. |
| **Detección del botón de opciones** | Un temporizador de 250 milisegundos comprueba únicamente si la ventana de Opciones de juego del juego está disponible. |
| **Comprobaciones de avisos de asesores** | Una vez por día simulado. |

**Las comprobaciones diarias de vigilancia no repiten innecesariamente los costosos cálculos completos.**

Por ejemplo, una lectura de demanda de electricidad sin cambios no activa otra optimización completa de las centrales eléctricas.

Del mismo modo, el consejo sobre el tamaño del vertedero se ejecuta dentro de una evaluación normal de la limpieza, no mediante una búsqueda continua independiente.

<a id="notices"></a>
### Avisos de los asesores

El Ministerio evita deliberadamente anunciar su trabajo rutinario.

No muestra mensajes cada vez que:

- Ajusta la financiación.
- Compra o vende un recurso.
- Cambia o cancela un contrato.
- Demuele un edificio abandonado.
- Repara una casilla fantasma de vertedero.

**Tampoco hay mensaje de saludo ni de activación.**

El Ministerio solo usa avisos de asesores cuando el jugador puede necesitar intervenir, o cuando una recomendación opcional sobre el vertedero podría ofrecer un ahorro significativo.

<a id="safety-notices-utility"></a>
#### Avisos de escasez de electricidad, agua y basura

El Ministerio puede avisarte cuando una escasez persiste y **las soluciones automáticas disponibles se han agotado**.

Estos avisos proceden del **asesor de Servicios públicos**.

| Recurso | Título del aviso |
|---|---|
| **Electricidad** | **¡A la luz de las velas!** |
| **Agua** | **¡Los grifos se declaran en huelga!** |
| **Basura** | **¡Una ciudad hasta los topes!** |

Son mensajes de asesor urgentes.

Según los ajustes de consejos de SimCity 4, pueden aparecer como ventanas emergentes además de como entradas en la lista de noticias.

No incluyen un enlace a la cámara.

#### ¿Cuándo considera el Ministerio que una escasez está demostrada?

Un aviso de escasez no se genera solo porque la demanda supere temporalmente el suministro.

El Ministerio debe establecer primero que **la escasez no se puede resolver con las opciones legales disponibles en ese momento**.

Para la electricidad y el agua, esto exige comprobar:

- La capacidad máxima utilizable de las instalaciones locales.
- Las compras regionales disponibles.
- La retirada de las ventas que consumen capacidad necesaria.
- La demanda real de la ciudad y el servicio realmente entregado.
- Que los registros regionales y las ofertas de contratos estén completos.

Si gestionas los contratos manualmente, el Ministerio evalúa los contratos existentes en lugar de suponer que puede crear otros nuevos.

La información regional debe ser lo bastante completa para sustentar la conclusión.

Para la electricidad y el agua, los contadores del juego también deben confirmar un déficit regional genuino.

**Un edificio que no tiene agua porque está desconectado de la red de tuberías no demuestra, por sí solo, una escasez de suministro regional.**

Para la basura, el Ministerio se basa en su modelo físico de eliminación para determinar si se pueden gestionar los residuos de la ciudad.

Se trata de una predicción del modelo, no de una medición directa del juego de cada flujo futuro de basura.

Por último, **ningún cambio de contrato ni ajuste de financiación de la limpieza del recurso afectado puede estar todavía pendiente de confirmación**.

El Ministerio no declara que una escasez es imposible de resolver mientras su propia acción correctiva sigue en marcha.

#### ¿Por qué espera el Ministerio 30 días antes de avisarte?

Las escaseces pueden desaparecer tras un ajuste de financiación, un cambio de contrato u otra actualización normal del juego.

Por eso el Ministerio espera a que una escasez demostrada lleve **30 días simulados** sin resolverse.

**Una escasez resuelta durante esos 30 días no genera ningún aviso.**

Así se evitan avisos innecesarios sobre problemas que la gestión automática corrige con éxito.

#### ¿Cuándo se muestran los recordatorios?

**Calendario de avisos de escasez de servicios**

```math
t_{\mathrm{primer}}=t_{\mathrm{prueba}}+30
```

```math
t_k=t_{\mathrm{primer}}+kP
```

Donde:

- **tprueba** es el día simulado en que la escasez se demostró por primera vez.
- **tprimer** es la fecha del primer aviso.
- **k** es el número de recordatorio, empezando en 1.
- **P** es el intervalo entre recordatorios.

| Recurso | Primer aviso | Intervalo entre recordatorios |
|---|---|---|
| **Electricidad** | Tras 30 días simulados sin resolver | Cada 60 días simulados |
| **Agua** | Tras 30 días simulados sin resolver | Cada 60 días simulados |
| **Basura** | Tras 30 días simulados sin resolver | Cada 120 días simulados |

El Ministerio sigue enviando recordatorios solo mientras la escasez continúa sin resolverse.

#### ¿Cuándo termina un episodio de escasez?

En la electricidad y el agua, el episodio termina cuando una lectura nueva del juego confirma que el servicio entregado es al menos igual a la demanda.

En la basura, deben cumplirse dos condiciones:

- El modelo de eliminación vuelve a ser viable.
- El juego no informa de basura sin recoger.

Una vez confirmada la recuperación, el Ministerio deja de enviar recordatorios.

**Una escasez posterior se trata como un episodio nuevo**, con su propio periodo de espera de 30 días.

#### ¿Qué ocurre si cierras y vuelves a abrir la ciudad?

El Ministerio recuerda qué escaseces ya ha comunicado.

Esta información se guarda **por ciudad** en un pequeño archivo de historial de avisos dentro de la carpeta del Ministerio, fuera de tus ciudades guardadas.

Por ejemplo, si recibes un aviso de escasez de agua y vuelves a abrir la ciudad mientras la misma escasez sigue sin resolverse, el Ministerio no muestra de inmediato el mismo aviso otra vez.

Cuando la escasez se vuelve a demostrar en la nueva sesión y sigue sin resolverse durante el periodo de espera normal, programa el siguiente recordatorio en lugar de repetir el primer aviso.

Si la sesión llega a su **día simulado 75** sin que esa escasez se haya vuelto a demostrar, se descarta la memoria del episodio anterior.

Una escasez posterior puede entonces iniciar un episodio nuevo.

El archivo de historial de avisos se puede borrar sin dañar las ciudades guardadas.

#### ¿Qué ajustes se necesitan para los avisos de escasez?

Los avisos de escasez de servicios requieren que **Gestionar acuerdos con vecinos** esté activado.

Esto se debe a que el Ministerio necesita inspeccionar las ofertas y los mercados regionales antes de concluir que una escasez no se puede resolver.

El recurso afectado también debe gestionarse mediante uno de estos:

- Su gestor de financiación local.
- Su gestor de contratos regionales correspondiente.

**Si el Ministerio no puede examinar todas las alternativas relevantes, no afirma que la escasez sea imposible de resolver.**

<a id="safety-notices-civic"></a>
#### Avisos de capacidad de educación y sanidad

El Ministerio también puede avisarte cuando las escuelas o los centros sanitarios siguen gravemente saturados a pesar de la financiación máxima disponible.

Estos avisos los emite el **asesor de Salud y Educación**.

Aparecen como **entradas rojas en la lista de noticias**, sin abrir una ventana emergente.

Hay un aviso por tipo de instalación:

- Escuelas primarias.
- Institutos.
- Facultades y universidades.
- Bibliotecas.
- Museos.
- Clínicas.
- Hospitales.
- Centros de investigación de enfermedades.

#### ¿Cuándo se incluye un edificio?

Un edificio debe cumplir todo lo siguiente:

1. Su financiación máxima posible sigue sin poder proporcionar capacidad suficiente.
2. Su capacidad máxima cubre **menos del 85% de su uso**.
3. **No está en huelga** en ese momento.
4. El problema ha continuado durante **30 días simulados sin interrupción**.

**Una cobertura inferior al 85% significa que, incluso con la financiación máxima, la instalación no puede atender el 85% de su uso actual.**

Por ejemplo, si un hospital necesita capacidad para 1.000 pacientes pero con la financiación máxima solo puede atender a 800, su cobertura es del 80%.

Si esa situación persiste durante 30 días simulados sin huelga, el hospital puede aparecer en un aviso de capacidad.

Consulta [Educación y sanidad](education_healthcare.md#education-healthcare-notices) para ver el cálculo completo.

#### ¿Qué información aparece en el aviso?

Un aviso enumera hasta **10 edificios afectados**, con los problemas más graves primero.

Cada edificio de la lista tiene un enlace que mueve la cámara a su ubicación.

**El Ministerio no crea un aviso aparte para cada edificio saturado.**

En su lugar, agrupa los edificios por tipo de instalación para que la lista del asesor siga siendo manejable.

#### ¿Qué ocurre cuando cambian los edificios afectados?

El Ministerio mantiene un único aviso activo por cada tipo de instalación.

Si el número de edificios afectados sigue igual pero cambian sus identidades, actualiza los enlaces a los edificios del aviso existente.

Si el número cambia, el aviso antiguo se retira antes de mostrar uno de sustitución.

**Nunca hay dos avisos activos para el mismo tipo de instalación.**

Si se producen dos sustituciones en 30 días simulados, el Ministerio introduce una **pausa de 30 días** antes de más sustituciones para ese tipo.

Las actualizaciones de los enlaces de un aviso existente pueden continuar durante la pausa.

#### ¿Cuándo se retira el aviso?

Cuando todos los edificios afectados se han recuperado, el aviso se retira.

Un aviso que descartas manualmente no vuelve a mostrarse mientras sigan afectados los mismos edificios.

Este estado de descarte se conserva al volver a abrir la ciudad.

En la primera evaluación tras la apertura, el Ministerio también retira cualquier aviso sobrante de una sesión anterior.

Un aviso de capacidad puede permanecer en la lista de noticias hasta **720 días simulados** mientras persista el problema.

<a id="safety-notices-landfill"></a>
#### Recomendaciones sobre el tamaño del vertedero

El Ministerio puede emitir dos avisos opcionales cuando cambiar el tamaño del vertedero reduciría significativamente los gastos de limpieza.

Ambos proceden del **asesor de Servicios públicos**, con un estado de ánimo de buenas noticias y prioridad urgente.

| Recomendación | Título del aviso |
|---|---|
| **AMPLIAR** | **¿Más vertedero, menos gasto?** |
| **REDUCIR** | **¡El vertedero ocupa demasiado sitio!** |

Cada aviso describe:

- Las dimensiones del área propuesta, en casillas.
- El ahorro mensual estimado.
- La reducción porcentual de los gastos de limpieza.
- Un enlace **ver dónde** que mueve la cámara a la zona recomendada.

Estos avisos solo están disponibles cuando:

- **Gestionar la limpieza** está activado.
- **Avisar si revisar el vertedero puede ahorrarme dinero** está activado.
- El ahorro estimado es estrictamente mayor que el umbral que has configurado.
- La ciudad contiene exactamente una zona de vertedero.
- Se cumplen las condiciones de plazos y de seguridad descritas en el capítulo del vertedero.

El umbral por defecto es del **25%**, y el consejo sobre el tamaño del vertedero está desactivado por defecto.

El Ministerio analiza el tamaño del vertedero como máximo una vez por mes simulado.

También respeta:

- El **periodo de protección de 24 meses** tras los cambios de tamaño del vertedero o cuando se encuentra un vertedero por primera vez.
- El **intervalo de 12 meses** tras emitir un consejo.

Consulta [Limpieza y vertederos](garbage.md#garbage-landfills-advice).

**Las reparaciones de casillas fantasma nunca generan avisos de asesores.**

<a id="safety-notices-game"></a>
#### Avisos originales de SimCity 4 modificados por el Ministerio

El Ministerio incluye **tres archivos de recursos** que cambian algunos de los mensajes de asesor del juego original.

Su propósito es evitar avisos engañosos o redundantes mientras la gestión automática está activa.

**Estos archivos se cargan cuando se inicia SimCity 4.**

Mientras sigan instalados, sus cambios continúan aplicándose aunque desactives durante la partida las opciones relacionadas del Ministerio.

| Archivo de recursos | Mensajes originales del juego afectados | Uso previsto |
|---|---|---|
| **Archivo de consejos de acuerdos con vecinos** | Suprime las ofertas de contrato originales, las sugerencias de cancelación, los mensajes de renovación y los avisos de impago. | Cuando la gestión automática de acuerdos con vecinos está activa para al menos un recurso. |
| **Archivo de consejos de educación y sanidad** | Suprime los consejos normales de financiación y limita los rumores previos a una huelga a los periodos en que realmente hay una huelga en curso. | Cuando la gestión de la educación o de la sanidad está activa. |
| **Archivo de consejos de basura** | Restringe los avisos de problemas de basura a las situaciones con una cantidad significativa de basura sin recoger. | Siempre. |

El archivo de educación y sanidad **no** suprime las alertas de huelga genuinas ni los mensajes positivos de los asesores.

El archivo de basura deja sin cambios los mensajes de buenas noticias.

#### ¿Por qué se suprimen algunos avisos originales de basura?

SimCity 4 a veces avisa de problemas de basura simplemente porque el almacenamiento del vertedero está aumentando.

Sin embargo, que un vertedero se vaya llenando poco a poco no es necesariamente una prueba de que el sistema de limpieza de la ciudad esté fallando.

Por eso el Ministerio añade un requisito basado en la cantidad de basura que realmente queda sin recoger.

**Basura sin recoger significativa**

```math
U>100+0.05G
```

Donde:

- **U** es la cantidad actual de basura sin recoger del juego.
- **G** es la basura producida por la ciudad, según los valores de asesor del juego.

El aviso original de basura solo puede aparecer cuando se cumple esta condición adicional, junto con la condición normal de aviso del juego.

**Ejemplo**

Si la ciudad produce 20.000 unidades de basura, el umbral es:

```math
100+0.05\times20000=1100
```

El juego debe informar de **más de 1.100 unidades de basura sin recoger** antes de que se permita el aviso modificado de problema de basura.

Esto evita alertas innecesarias cuando la eliminación de basura funciona con normalidad pero el almacenamiento del vertedero está aumentando.

#### ¿Cómo puedo restaurar los mensajes de asesor del juego original?

Como SimCity 4 carga estos archivos de recursos al arrancar, cambiar las opciones del Ministerio no restaura de inmediato los mensajes originales.

Si desactivas una función pero dejas instalado su archivo de recursos de asesor, el Ministerio registra una advertencia en su log.

La advertencia identifica qué archivo conviene quitar.

Para restaurar los mensajes originales correspondientes:

1. Cierra SimCity 4.
2. Quita el archivo de recursos de consejos indicado de la carpeta Plugins.
3. Reinicia el juego.

**Los archivos de recursos de asesor afectan a los mensajes del juego con independencia de que el Ministerio esté haciendo cambios de gestión automática en ese momento.**

<a id="safety-isolation"></a>
### 4. Condiciones de seguridad

#### ¿Cómo gestiona el Ministerio los fallos técnicos?

El Ministerio se divide en componentes de gestión independientes.

**Un fallo en un componente no debería desactivar servicios no relacionados ni detener el propio SimCity 4.**

Cuando falla una comprobación de seguridad, el componente afectado deja de hacer cambios en lugar de intentar repetidamente una operación poco fiable.

| Fallo | Qué hace el Ministerio |
|---|---|
| **Falla la verificación de la financiación de una central eléctrica o del agua** | Intenta deshacer los cambios hechos en ese ciclo y congela la gestión de financiación de ese recurso durante la sesión de ciudad. Los demás recursos y los contratos regionales pueden continuar. |
| **Falla la verificación de la financiación de la limpieza** | Detiene los cambios de financiación de la limpieza durante la sesión. Los contratos de basura y los demás gestores de recursos pueden continuar. |
| **Una operación de contrato falla técnicamente** | Detiene la gestión automática de contratos durante la sesión. Los gestores de financiación local siguen usando los contratos existentes. |
| **No se puede verificar un cambio de financiación de una escuela o un hospital** | Excluye ese edificio durante la sesión. Tras fallos que afecten a tres edificios distintos, las escrituras de financiación de educación y sanidad se detienen durante la sesión. |
| **El control de financiación de un edificio no tiene efecto** | Excluye ese edificio y permite que los demás edificios continúen. |
| **La demolición automática no supera una comprobación de seguridad** | Detiene la demolición durante la sesión si una vista previa de precio cambia una parcela, desaparece la zonificación o tres órdenes consecutivas no se ejecutan. |
| **Una operación de asesor produce un error** | Deja de comprobar y retirar avisos del Ministerio durante el resto de la sesión. |
| **La ventana de Opciones de juego tiene un diseño inesperado** | Deja la ventana original sin cambios y no añade el botón del Ministerio durante esa sesión. |
| **Un archivo de configuración no es válido** | Desactiva el componente de gestión afectado en lugar de usar ajustes poco fiables. |

#### ¿Qué significa una parada de seguridad de sesión?

Una parada de seguridad dura lo que dura la sesión de ciudad en curso.

Por ejemplo, si falla la verificación de la financiación del agua, el Ministerio deja de hacer más cambios en la financiación del agua mientras esa ciudad siga abierta.

No envía repetidamente la misma orden.

**Volver a cargar la ciudad inicia una sesión nueva y elimina las paradas de seguridad propias de la sesión.**

El problema subyacente puede seguir existiendo, así que la misma comprobación puede volver a fallar si no se ha resuelto.

#### ¿Puede un error del Ministerio hacer que SimCity 4 se cierre?

El Ministerio envuelve los eventos del juego que gestiona para que los errores internos se capturen y se registren en su log.

**Un error en la gestión del Ministerio está pensado para detener la operación afectada, no el propio juego.**

Este aislamiento es una parte fundamental de su diseño.

<a id="safety-version"></a>
#### Versión del juego compatible

El Ministerio comprueba la versión del ejecutable del juego al arrancar.

**La versión compatible es SimCity 4 Deluxe 1.1.641.0 (Steam, Windows).**

En cualquier otra versión, o cuando no se puede leer la versión, el Ministerio permanece totalmente inactivo.

Esto significa:

- Ninguna suscripción a eventos del juego.
- Ningún botón de opciones del Ministerio.
- Ninguna ordenanza del Ministerio.
- Ninguna operación de financiación, de contratos ni de demolición.
- Ninguna modificación del icono de agua de las parcelas agrícolas.

La misma inactividad total se aplica cuando falta, junto a la DLL, la carpeta **Department of Efficiency** necesaria o el archivo de recursos principal.

**El Ministerio no intenta funcionar en una instalación no compatible o incompleta.**

#### Tus ciudades guardadas no dependen del plugin

El Ministerio está diseñado para que las ciudades guardadas mientras está instalado se puedan seguir abriendo sin él.

Su ordenanza **Crear un Ministerio de Optimización** se retira temporalmente de la ciudad antes de cada guardado y se restaura después.

Por tanto, la ordenanza no se almacena en la ciudad guardada.

**El Ministerio nunca edita directamente los archivos de guardado de ciudades ni de regiones.**

Si quitas el plugin, los niveles de financiación ordinarios de la ciudad y los contratos regionales permanecen en sus últimos valores establecidos.

Puedes seguir jugando sin el Ministerio.

<a id="safety-settings"></a>
### 5. Ajustes

El sistema de seguridad general no requiere un conjunto de opciones aparte.

Sigue los ajustes de gestión descritos a lo largo de esta guía.

**Ministerio activo**

[Ministerio activo](options.md#opt-master) es el interruptor principal.

También puedes controlarlo mediante la ordenanza **Crear un Ministerio de Optimización** de la ventana de Ordenanzas de SimCity 4.

Ambos controles representan el mismo ajuste.

**Opciones de gestión individuales**

Cada servicio tiene su propio gestor y, cuando procede, una reserva o un margen de seguridad.

Consulta la [Referencia de opciones](options.md#options) para ver todos los ajustes.

**Avisos de los asesores**

El Ministerio tiene solo dos opciones específicas de avisos, ambas relacionadas con las recomendaciones sobre el tamaño del vertedero:

- [Avisar si revisar el vertedero puede ahorrarme dinero](options.md#opt-landfilladvice).
- [Ahorro mínimo para avisos del vertedero](options.md#opt-landfillthreshold).

**Los avisos de escasez de servicios no se pueden desactivar por separado.**

Siguen los ajustes de gestión de los servicios correspondientes y solo aparecen cuando se cumplen sus condiciones.

<a id="safety-examples"></a>
### 6. Ejemplos prácticos

Los siguientes ejemplos muestran cómo responde el Ministerio ante carencias persistentes, información incompleta y cambios manuales.

#### Ejemplo 1: Una escasez de electricidad que no se puede resolver

Supón que tu ciudad se encuentra en esta situación el **día simulado 40** tras abrirla:

- **Demanda de electricidad:** 10.000 unidades.
- **Electricidad suministrada:** 9.200 unidades.
- **Capacidad máxima de generación local:** 8.900 unidades.
- **Importaciones regionales utilizables:** 200 unidades.
- **Ventas:** ninguna.

El Ministerio ya ha tenido en cuenta la financiación máxima legal de las centrales y todas las compras disponibles.

El suministro total disponible es:

```math
8900+200=9100
```

Eso es menos que la demanda de la ciudad:

```math
9100<10000
```

Los contadores del juego también confirman que el suministro de electricidad es inferior a la demanda.

**La carencia queda demostrada el día 40.**

Aun así, el Ministerio no muestra un aviso de inmediato.

Espera 30 días simulados.

**Día 70**

La carencia sigue sin resolverse.

El Ministerio emite:

**¡A la luz de las velas!**

**Día 130**

La carencia continúa.

Aparece el primer recordatorio, 60 días simulados después del aviso original.

**Día 150**

Construyes otra central eléctrica.

Una nueva lectura confirma que la electricidad suministrada ya cubre la demanda.

El Ministerio da por cerrado el episodio de carencia y deja de enviar recordatorios.

**Día 200**

Se demuestra una nueva carencia de electricidad.

Como el episodio anterior terminó, la nueva carencia inicia su propio periodo de espera de 30 días.

Si sigue sin resolverse, el siguiente aviso aparece el **día 230**.

#### Ejemplo 2: Edificios sin electricidad, pero con capacidad de generación suficiente

Imagina una ciudad con:

- **Demanda de electricidad:** 10.000 unidades.
- **Electricidad suministrada:** 9.600 unidades.
- **Capacidad de generación local:** 12.000 unidades.
- **Sin importaciones ni exportaciones.**

Aunque una parte de la demanda no se está atendiendo, la ciudad tiene capacidad de generación suficiente.

```math
12000\ge10000
```

Esto significa que no se cumple la prueba de escasez de suministro.

Lo más probable es que el problema esté dentro de la red de distribución eléctrica.

**El Ministerio no aumenta la financiación ni emite un aviso de escasez regional.**

Producir más no llegaría necesariamente a los edificios desconectados.

El jugador debe revisar la red eléctrica.

#### Ejemplo 3: Reabrir una ciudad durante una carencia ya existente

Supón que una carencia de agua se notificó por primera vez el **día 70** de una sesión de la ciudad.

Guardas la ciudad el día 100 mientras la carencia sigue sin resolverse.

Más tarde, vuelves a abrir la ciudad.

El Ministerio inicia una nueva sesión y evalúa de nuevo el sistema de agua.

La carencia se demuestra otra vez y permanece sin resolverse durante otros 30 días simulados.

Sin embargo, el Ministerio recuerda que ese mismo episodio de carencia ya se había notificado.

**No repite el primer aviso.**

En su lugar, programa el siguiente recordatorio según el intervalo normal de 60 días.

Así se evita que el mismo problema sin resolver genere un nuevo aviso inicial cada vez que reabres la ciudad.

#### Ejemplo 4: Cambiar la financiación del agua a mano

Supón que el Ministerio ha optimizado la financiación del agua al **62%**.

Tú la subes manualmente al **100%**, mientras **Gestionar el agua** sigue activada.

El Ministerio acepta tu cambio como el valor de partida actual.

En su siguiente evaluación del agua, como máximo 10 días simulados después, recalcula la financiación necesaria.

Si comprueba que basta con el **64%**, puede reducir la financiación del 100% al 64%.

**El Ministerio no conserva los cambios manuales de los deslizadores mientras la gestión automática siga activada.**

Para mantener la financiación del agua al 100%, desactiva **Gestionar el agua**.

#### Ejemplo 5: Las lecturas del agua no están listas tras cargar la ciudad

Justo después de abrir una ciudad, el juego informa de:

- **Demanda de agua:** 20.000 unidades.
- **Consumo de agua:** 20.500 unidades.

El Ministerio comprueba si el consumo es coherente con la demanda.

La lectura máxima aceptada es:

```math
20000\times1.01+1=20201
```

Sin embargo:

```math
20500>20201
```

El consumo supera la tolerancia permitida.

**Las lecturas son incoherentes, así que el Ministerio todavía no toma ninguna decisión sobre la financiación del agua.**

Espera a una lectura nueva.

Los demás servicios se evalúan con su propia información.

**Un problema temporal con los datos del agua no impide que la electricidad, la limpieza u otros gestores independientes sigan funcionando.**

<a id="safety-limitations"></a>
### 7. Limitaciones conocidas

El Ministerio está diseñado para actuar con prudencia, pero no puede superar todas las limitaciones de SimCity 4.

#### Control manual

- **Los cambios manuales de financiación no se bloquean.** Cuando un gestor de servicio está activado, puede revisar en su siguiente evaluación un porcentaje de financiación que hayas cambiado tú mismo.
- **No hay bloqueos por edificio.** Para mantener un servicio concreto bajo tu control manual de la financiación, desactiva el gestor correspondiente.

#### Avisos de carencia

- **Los avisos de carencia de suministros requieren Gestionar acuerdos con vecinos.** Sin acceso a las ofertas y los mercados regionales, el Ministerio no puede demostrar que se han agotado todas las soluciones posibles.
- **El Ministerio no puede saber si un aviso llegó a mostrarse en pantalla.** SimCity 4 decide si muestra una ventana emergente según los ajustes de sus asesores.
- **Los avisos de carencia de basura se basan en un modelo.** A diferencia de la electricidad y el agua, la viabilidad de la basura se determina mediante la proyección de eliminación del Ministerio, y no mediante un contador directo del juego que demuestre las necesidades futuras de eliminación.

#### Mensajes originales de los asesores del juego

- **Los tres archivos de recursos de consejos actúan en cuanto están instalados.** Desactivar las opciones correspondientes del Ministerio no deshace sus efectos de inmediato.
- **Restaurar los mensajes originales puede requerir quitar el archivo de recursos correspondiente y reiniciar SimCity 4.** El Ministerio registra en el log las discrepancias de configuración.

#### Seguridad técnica

- **Una parada técnica de seguridad dura toda la sesión actual de la ciudad.** El gestor afectado permanece desactivado hasta que se vuelva a cargar la ciudad.
- **Solo se gestiona la versión compatible del juego.** El Ministerio es compatible con **SimCity 4 Deluxe 1.1.641.0 de Steam en Windows**. No realiza ninguna operación en versiones no compatibles.

**El Ministerio está diseñado para optimizar la ciudad solo cuando dispone de información fiable suficiente para actuar con seguridad. Cuando no puede verificar una operación o demostrar que un cambio es beneficioso, prefiere dejar la situación existente tal como está.**

---

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
