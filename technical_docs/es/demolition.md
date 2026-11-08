[English](../en/demolition.md) | [Português](../pt/demolition.md) | [Français](../fr/demolition.md) | **Español**

[Volver al contenido](index.md#contents) | [Volver al README](../../README.es.md)

<a id="demolition"></a>
## F. Demolición automática

El Ministerio puede despejar automáticamente ciertos edificios abandonados y ruinas, para que el suelo zonificado de tu ciudad pueda volver a desarrollarse.

Se ocupa de dos situaciones:

- **Ruinas:** edificios destruidos por incendios o desastres que siguen en sus parcelas.
- **Edificios abandonados:** edificios que llevan vacíos el tiempo suficiente.

El Ministerio comprueba las parcelas elegibles **una vez por mes simulado**.

Tú controlas:

- Cuánto tiempo debe permanecer un edificio abandonado o destruido antes de su demolición.
- Qué tipos de zona se pueden incluir.
- Qué motivos de abandono son elegibles.

**La demolición nunca es gratuita.**

El Ministerio usa el comando normal de excavadora de SimCity 4 y paga el mismo precio que pagarías tú al demoler la parcela manualmente.

Antes de cada demolición, pregunta al juego el coste real y comprueba si la tesorería de tu ciudad puede asumirlo.

**Los edificios municipales y los edificios protegidos por su valor histórico nunca se demuelen automáticamente.**

El objetivo es despejar las parcelas descuidadas sin destruir edificios importantes ni gastar más dinero del que tiene tu ciudad.

<a id="demolition-vanilla"></a>
### 1. Cómo funciona la demolición automática con SimCity 4

#### Cómo registra el juego el estado de los edificios

SimCity 4 registra un estado para cada parcela.

También anota **cuántos días simulados han pasado desde el último cambio de ese estado**.

El Ministerio usa estos valores existentes para decidir si una parcela es elegible para la demolición.

| Estado del juego | Qué ves | Acción del Ministerio |
|---|---|---|
| **Ocupado** | Un edificio en funcionamiento | Nunca se demuele |
| **En construcción** | Un edificio en obras | Nunca se demuele |
| **Listo / Creado** | Una parcela que espera desarrollo o un edificio que va a quedar disponible | Nunca se demuele |
| **Vacío** | Un edificio abandonado | Se puede demoler tras el tiempo de espera elegido |
| **En demolición** | Un edificio que ya se está demoliendo | Se omite |
| **Destruido** | Ruinas dejadas por un incendio o un desastre | Se pueden demoler tras el tiempo de espera elegido |

**Solo las parcelas Vacías y Destruidas se tienen en cuenta para la demolición automática.**

Todos los demás estados quedan excluidos.

#### ¿Cómo se mide el tiempo de espera?

El Ministerio usa **el contador de días del propio juego**, no un temporizador aparte.

Por ejemplo, imagina que un edificio lleva abandonado 200 días simulados.

Si el tiempo de espera configurado es de 180 días, ya tiene la antigüedad suficiente para tenerse en cuenta en la siguiente revisión mensual de demolición, siempre que se cumplan todas las demás condiciones.

El contador se guarda con la ciudad.

**Esto significa que el Ministerio también reconoce los edificios que se abandonaron antes de instalar el mod.**

No hace falta esperar a que el periodo de abandono vuelva a empezar.

#### ¿Por qué se abandonó un edificio?

SimCity 4 registra el motivo por el que un edificio quedó abandonado.

El Ministerio lee ese motivo y usa las opciones que hayas elegido para decidir si la parcela debe ser elegible.

| Motivo de abandono del juego | Qué significa | Opción que lo controla |
|---|---|---|
| **Poca atractividad** | La zona es poco atractiva por factores como la contaminación, la delincuencia o unos servicios deficientes. | **Motivo: zona poco atractiva** |
| **Tiempo de trayecto** | Los ocupantes no pueden llegar a su trabajo en un tiempo de viaje aceptable. | **Motivo: trayecto al trabajo demasiado largo** |
| **Poca demanda** | No hay demanda suficiente para el tipo de edificio. | **Motivo: falta de demanda** |
| **Sin electricidad** | El edificio se abandonó porque no tiene electricidad. | **Motivo: falta de electricidad** |
| **Sin agua** | El edificio se abandonó porque no tiene agua. | **Motivo: falta de agua** |
| **Ocupantes desalojados** | El juego indica que se expulsó a los ocupantes. | Siempre elegible por el motivo; sin opción propia |
| **Ninguno / desconocido** | El juego no indica ningún motivo de abandono reconocible. | Siempre elegible por el motivo; sin opción propia |

**Ser elegible por el motivo no significa demolición inmediata.** Siguen aplicándose el tiempo de espera, las restricciones de zona, la protección histórica y las demás comprobaciones de seguridad.

#### ¿Por qué Sin electricidad y Sin agua están desactivados por defecto?

Un edificio abandonado por falta de electricidad o de agua puede volver a ocuparse cuando se restablece el servicio.

Por ejemplo:

- Un edificio de apartamentos se queda sin electricidad y queda abandonado.
- Reparas la conexión eléctrica.
- El edificio puede volver a ocuparse sin haber sido demolido.

Demolerlo demasiado pronto sería innecesario.

**Por eso el abandono causado por la falta de electricidad o de agua está excluido por defecto.**

Puedes activar estos motivos si quieres que el Ministerio despeje esos edificios tras el tiempo de espera que elijas.

#### ¿Qué tipos de zona son elegibles?

El Ministerio distingue entre el desarrollo zonificado ordinario y las parcelas municipales protegidas o de uso especial.

| Grupo de zona | Tipos de zona incluidos |
|---|---|
| **Residencial** | Zonas residenciales de baja, media y alta densidad |
| **Comercial** | Zonas comerciales de baja, media y alta densidad |
| **Industrial** | Zonas industriales de media y alta densidad |
| **Agrícola** | Granjas y campos agrícolas |
| **Municipal / protegido** | Edificios colocados a mano, parcelas de uso especial y parcelas sin zonificar |

El grupo municipal y protegido incluye:

- Escuelas y universidades.
- Hospitales.
- Comisarías de policía y parques de bomberos.
- Parques y monumentos.
- Edificios de recompensa.
- Servicios públicos (suministros).
- Instalaciones militares.
- Aeropuertos, puertos marítimos y puertos espaciales.
- Vertederos.
- Parcelas sin zonificación.

**El Ministerio no demuele automáticamente las parcelas municipales ni las protegidas.**

Estos edificios representan decisiones deliberadas del jugador o infraestructuras de uso especial, y no un desarrollo abandonado corriente.

#### ¿Y las granjas?

Las parcelas agrícolas requieren un cuidado especial.

Los campos de cultivo pueden aparecer vacíos como parte de su funcionamiento normal, incluso cuando la propia granja funciona.

**Una parcela agrícola que parece vacía no es automáticamente una prueba de que haya que retirar una granja.**

Por eso el Ministerio distingue la zonificación agrícola del desarrollo residencial, comercial e industrial ordinario, lo que permite controlarla por separado.

#### Los edificios históricos siempre están protegidos

SimCity 4 permite designar ciertos edificios como históricos.

El juego registra esa designación mediante un indicador histórico.

**Un edificio con designación histórica está protegido contra la demolición automática**, incluso si queda abandonado y ha superado el tiempo de espera configurado.

El Ministerio respeta esa decisión.

#### ¿Cómo funciona la demolición con la excavadora?

Cuando seleccionas manualmente la herramienta excavadora en SimCity 4, el juego calcula el precio de demolición de la parcela.

También comprueba si la parcela se puede demoler realmente en su estado actual.

El Ministerio usa las mismas funciones internas del juego.

**Antes de demoler una parcela, obtiene el coste de excavadora del propio juego.**

No da por supuesto que todos los edificios tengan el mismo precio de demolición.

Si el juego indica que una parcela no se puede demoler en ese momento, el Ministerio la omite.

**¿Qué pasa con el terreno después?**

Demoler un edificio no elimina su zonificación.

Por ejemplo:

- Un edificio residencial demolido deja la zonificación residencial.
- La ciudad podrá desarrollar más adelante otro edificio residencial en esa parcela.

Lo mismo se aplica a los edificios abandonados elegibles y a las ruinas.

**El Ministerio despeja el edificio, no la zona.**

Las ruinas dejadas por incendios o desastres permanecen en sus parcelas hasta que algo las demuele, ya sea manualmente o mediante la función de demolición automática del Ministerio.

<a id="demolition-problem"></a>
### 2. El problema de la gestión

En las ciudades grandes, los edificios abandonados y las ruinas pueden resultar difíciles de gestionar a mano.

Surgen varios problemas.

**1. Las ruinas impiden un nuevo desarrollo**

Los edificios destruidos por incendios o desastres pueden permanecer en suelo zonificado.

Mientras no se demuelan, ese terreno no puede simplemente desarrollar un edificio de sustitución.

En una ciudad grande, es fácil pasar por alto las ruinas dispersas.

**2. El abandono prolongado bloquea la reconstrucción**

Un edificio abandonado por falta de demanda, poca atractividad o un tiempo de trayecto excesivo puede permanecer vacío durante mucho tiempo.

Despejar la parcela puede permitir que el juego desarrolle otro edificio cuando mejoren las condiciones.

**3. No todos los edificios aparentemente vacíos deben demolerse**

Algunos edificios pueden recuperarse por sí solos.

Un edificio abandonado por falta de electricidad o de agua puede volver a ocuparse cuando regresa el servicio.

Los campos agrícolas pueden aparecer vacíos con normalidad.

También hay que proteger los edificios municipales y los edificios históricos.

**4. Demoler cuesta dinero**

Cada operación de excavadora tiene un precio determinado por el juego.

Demoler muchos edificios durante el mismo mes podría consumir una parte importante de la tesorería de una ciudad pequeña.

**El Ministerio debe, por tanto, distinguir entre las parcelas que son realmente elegibles para la demolición y las que hay que dejar en paz, respetando tus ajustes y el dinero disponible de la ciudad.**
<a id="demolition-algorithm"></a>
### 3. Cómo gestiona el Ministerio la demolición

El Ministerio comprueba tu ciudad **una vez por mes simulado** en busca de ruinas y edificios abandonados que cumplan tus ajustes de demolición.

No demuele nada de inmediato cuando abres una ciudad. La primera revisión de demolición se realiza en el **primer cambio de mes simulado tras la carga**.

#### ¿Cómo funciona la revisión mensual de demolición?

En cada revisión mensual, el Ministerio sigue estos pasos:

1. **Buscar las parcelas elegibles.** Examina la ciudad en busca de edificios destruidos o abandonados y comprueba su antigüedad, su zonificación, el motivo de abandono y su estado de protección.
2. **Decidir qué parcelas cumplen los requisitos.** Aplica los ajustes de demolición que hayas elegido y las propias reglas de elegibilidad del juego.
3. **Ordenar los candidatos.** Primero van las ruinas y después los edificios abandonados. Dentro de cada grupo, van primero las parcelas más antiguas.
4. **Comprobar los límites.** Respeta el número máximo de órdenes de demolición, la tesorería de la ciudad y cualquier presupuesto mensual de demolición que hayas configurado.
5. **Verificar de nuevo cada parcela.** Antes de enviar una orden, comprueba que la parcela no ha cambiado y pregunta al juego su precio real de excavadora.
6. **Enviar la orden de demolición.** Usa la operación normal de excavadora de SimCity 4, sin eliminar la zonificación subyacente.
7. **Verificar el resultado.** En una exploración posterior, confirma que el edificio ha desaparecido y que el terreno sigue zonificado.

**El Ministerio nunca demuele una parcela solo porque pareciera elegible en una exploración anterior.**

La vuelve a comprobar inmediatamente antes de emitir la orden.

#### ¿Qué edificios se demuelen primero?

El orden de prioridad es:

1. **Edificios destruidos (ruinas)**, los más antiguos primero.
2. **Edificios abandonados**, los más antiguos primero.

Esto significa que una ruina elegible reciente se procesa antes que un edificio abandonado más antiguo.

Normalmente, el Ministerio puede emitir hasta **150 órdenes de demolición durante una revisión mensual**.

Sin embargo, se aplica una precaución adicional al comenzar una sesión de la ciudad.

**Antes de que el juego haya confirmado al menos una orden de demolición en esa sesión, el Ministerio envía como máximo tres órdenes por revisión mensual.**

Esto le permite comprobar que el mecanismo de demolición del juego funciona correctamente antes de procesar un número mayor de edificios.

#### ¿Qué ocurre entre las revisiones mensuales?

Cada **10 días simulados**, el Ministerio realiza una comprobación ligera de demolición.

Esta comprobación:

- Confirma si las órdenes de demolición anteriores se llevaron a cabo.
- Registra los cambios en las parcelas vigiladas.
- Busca resultados inesperados.

**La comprobación de 10 días nunca emite nuevas órdenes de demolición.**

Las nuevas demoliciones se realizan únicamente en las revisiones mensuales.

#### ¿Cómo decide el Ministerio si una parcela cumple los requisitos?

Las siguientes reglas se comprueban por orden.

| Condición | Decisión |
|---|---|
| El juego ya está demoliendo la parcela | Omitirla |
| La parcela no está ni abandonada ni destruida | No elegible |
| La parcela es histórica y no cumple la excepción especial de las ruinas | Conservarla |
| La demolición automática está desactivada para ese tipo de parcela | Conservarla |
| La parcela es municipal y no cumple la excepción especial de las ruinas | Conservarla |
| Un edificio abandonado está en un tipo de zona que has excluido | Conservarlo |
| Un edificio abandonado tiene un motivo de abandono excluido | Conservarlo |
| La parcela no ha alcanzado el tiempo de espera configurado | Conservarla |
| El juego indica que la parcela no se puede demoler | Conservarla |
| No se aplica ninguna de las anteriores | **Elegible para la demolición** |

**Una distinción importante:**

Las opciones de zona y de motivo de abandono se aplican solo a los **edificios abandonados**.

No filtran las ruinas.

Por ejemplo, si **Demoler edificios destruidos** está activada, las ruinas de un edificio residencial, comercial, industrial o agrícola se pueden despejar tras el tiempo de espera, incluso cuando ese tipo de zona está excluido de la demolición de edificios abandonados.

Las ruinas municipales e históricas tienen su propio ajuste de protección independiente.

<a id="demolition-destroyed"></a>
#### Edificios destruidos: despejar las ruinas

Cuando **Demoler edificios destruidos** está activada, el Ministerio puede retirar automáticamente las ruinas que dejan los incendios y los desastres.

Por defecto, espera **un mes simulado**, equivalente a 30 días.

Por ejemplo:

- Un edificio comercial queda destruido por un incendio.
- El juego marca su parcela como Destruida.
- El Ministerio espera hasta que hayan pasado al menos 30 días simulados.
- En la siguiente revisión mensual de demolición, las ruinas pasan a ser elegibles.
- Si el juego permite la demolición y la ciudad puede permitírsela, el Ministerio las retira.

**La zonificación comercial subyacente se mantiene**, lo que permite un nuevo desarrollo más adelante.

Las ruinas en suelo residencial, comercial, industrial y agrícola ordinario siguen esta regla.

**Las ruinas municipales e históricas están protegidas por defecto.**

Solo se pueden incluir si activas explícitamente **Demoler edificios municipales e históricos destruidos**.

<a id="demolition-abandoned"></a>
#### Edificios abandonados

Cuando **Demoler edificios abandonados** está activada, el Ministerio tiene en cuenta los edificios que llevan abandonados el tiempo suficiente.

Por defecto, espera **12 meses simulados**, equivalentes a 365 días.

Un edificio abandonado debe cumplir todas estas condiciones:

- Pertenece a un tipo de zona activado.
- Su motivo de abandono está activado, o es uno de los motivos que siempre son elegibles.
- Lleva abandonado al menos el tiempo de espera configurado.
- No es municipal ni está protegido por su valor histórico.
- SimCity 4 confirma que la parcela se puede demoler.

**Comportamiento por defecto**

El Ministerio puede demoler edificios residenciales, comerciales e industriales abandonados cuando su abandono se debe a:

- Una zona poco atractiva.
- Un tiempo de trayecto excesivo.
- Una demanda insuficiente.

El abandono causado por la falta de electricidad o de agua está excluido por defecto.

Es intencionado: un edificio abandonado por un problema de servicio temporal puede volver a ocuparse cuando se restablece ese servicio.

<a id="demolition-delays"></a>
#### ¿Cómo funcionan los tiempos de espera?

La ventana de opciones permite elegir los tiempos de espera en **meses simulados**.

Internamente, sin embargo, SimCity 4 registra cuánto tiempo lleva un edificio en su estado actual en **días simulados**.

Por eso el Ministerio convierte el número de meses elegido en días.

**Conversión del tiempo de espera**

```math
D=\mathrm{round}\left(\frac{M\times365}{12}\right)
```

Donde:

- **M** es el tiempo de espera elegido, en meses simulados.
- **D** es el tiempo de espera que el Ministerio guarda y utiliza, en días simulados.

El resultado se redondea al día entero más cercano, y los valores de exactamente medio día se redondean hacia arriba.

**Ejemplos**

| Meses elegidos | Días utilizados |
|---:|---:|
| 1 | 30 |
| 2 | 61 |
| 3 | 91 |
| 6 | 183 |
| 12 | 365 |
| 24 | 730 |
| 36 | 1.095 |
| 60 | 1.825 |

#### Tiempos de espera disponibles

| Tipo de edificio | Intervalo en el juego | Valor por defecto |
|---|---|---|
| **Ruinas** | 1–24 meses | 1 mes (30 días) |
| **Edificios abandonados** | 1–60 meses | 12 meses (365 días) |

Si editas el archivo de configuración manualmente, los intervalos guardados permitidos son más amplios:

- **Ruinas:** 1–3.650 días.
- **Edificios abandonados:** 1–7.300 días.

#### ¿Cómo sabe el Ministerio cuándo una parcela tiene antigüedad suficiente?

Usa el contador del propio juego.

**Requisito de antigüedad para la demolición**

```math
A\ge D
```

Donde:

- **A** es el número de días simulados desde que la parcela quedó abandonada o destruida.
- **D** es el tiempo de espera configurado, en días.

Por ejemplo, si el tiempo de espera es de 91 días:

- Con 85 días de antigüedad, la parcela es demasiado reciente.
- Con 91 días, pasa a ser elegible.
- En la práctica se procesa en la primera revisión mensual tras alcanzar esa antigüedad.

**Por tanto, la demolición puede producirse hasta aproximadamente un mes simulado después de alcanzar el tiempo de espera.**

#### ¿Y si edito el tiempo de espera directamente en el archivo de configuración?

La ventana de opciones convierte los días guardados de nuevo en un número aproximado de meses.

```math
M_{\mathrm{mostrado}}=
\min\left(
M_{\max},
\max\left(
1,\mathrm{round}\left(\frac{D\times12}{365}\right)
\right)
\right)
```

Aquí, **Mmax** es de 24 meses para las ruinas y de 60 meses para los edificios abandonados.

Por ejemplo, un retraso configurado manualmente de **45 días** aparece como aproximadamente **1 mes** en la ventana de opciones.

Sin embargo, el valor subyacente de 45 días se conserva hasta que cambies realmente el control del tiempo de espera y aceptes el cambio.

**El simple hecho de abrir la ventana de opciones no reescribe el retraso que configuraste manualmente.**

<a id="demolition-zones"></a>
#### ¿Qué zonas puede demoler el Ministerio?

El Ministerio ofrece cuatro opciones de zona:

| Opción de zona | Valor por defecto |
|---|---|
| **Zona residencial** | Sí |
| **Zona comercial** | Sí |
| **Zona industrial** | Sí |
| **Zona agrícola** | No |

Estas opciones determinan qué **edificios abandonados** son elegibles.

Por ejemplo, desactivar Zona comercial impide que el Ministerio demuela edificios comerciales abandonados.

**Estas restricciones no se aplican a las ruinas.**

Si la demolición automática de ruinas está activada, los edificios destruidos en zonas con capacidad de crecimiento se pueden despejar igualmente, independientemente de las opciones de zona anteriores.

Las parcelas municipales siguen protegidas con independencia de estos ajustes.

<a id="demolition-reasons"></a>
#### ¿Qué motivos de abandono son elegibles?

También puedes elegir qué motivos de abandono permiten la demolición automática.

| Motivo de abandono | Valor por defecto |
|---|---|
| **Motivo: zona poco atractiva** | Sí |
| **Motivo: trayecto al trabajo demasiado largo** | Sí |
| **Motivo: falta de demanda** | Sí |
| **Motivo: falta de electricidad** | No |
| **Motivo: falta de agua** | No |

**¿Por qué Sin electricidad y Sin agua están desactivados?**

Porque los edificios abandonados por falta de servicios pueden volver a ocuparse.

Por ejemplo, si un edificio residencial se queda sin electricidad, el Ministerio normalmente lo deja en paz mientras restableces el suministro eléctrico.

Puedes activar estos motivos si prefieres demoler esos edificios tras el tiempo de espera.

#### Motivos que siempre son elegibles

Dos situaciones que indica el juego no tienen opciones propias:

- **Ocupantes desalojados.**
- **Ningún motivo o un motivo de abandono no reconocido.**

Estos motivos se consideran siempre elegibles cuando la demolición automática de edificios abandonados está activada.

Siguen aplicándose el tiempo de espera normal, la selección de zonas, las reglas de edificios protegidos y las comprobaciones financieras.

<a id="demolition-protected"></a>
#### Edificios municipales e históricos

El Ministerio protege los edificios que representan decisiones deliberadas del jugador.

**Edificios municipales**

Este grupo incluye:

- Escuelas, hospitales, comisarías de policía y parques de bomberos.
- Parques, monumentos, edificios de recompensa y otros edificios colocados a mano.
- Servicios públicos (suministros).
- Instalaciones militares.
- Aeropuertos, puertos marítimos y puertos espaciales.
- Vertederos.
- Parcelas sin zonificación ordinaria.

**Edificios históricos**

Cualquier parcela con la designación histórica de SimCity 4 también está protegida.

**Los edificios municipales e históricos, ya estén en pie o abandonados, nunca se demuelen automáticamente.**

Esta protección se aplica con independencia de tus ajustes de abandono.

#### ¿Y sus ruinas?

Existe una excepción opcional.

**Demoler edificios municipales e históricos destruidos** está **desactivada por defecto**.

Cuando está desactivada, las ruinas de los edificios municipales e históricos no se tocan.

Cuando está activada, el Ministerio puede despejar esas ruinas tras el tiempo de espera normal de las ruinas, siempre que:

- La parcela esté realmente en el estado Destruido.
- La comprobación de demolición del juego la acepte.
- Las reglas de tesorería y de presupuesto permitan la demolición.

**Esta opción nunca autoriza la demolición de un edificio municipal o histórico que siga en pie.**

Solo se aplica una vez que el edificio ya ha sido destruido.

<a id="demolition-farms"></a>
#### Cuidado especial con las parcelas agrícolas

Las parcelas agrícolas se comportan de forma distinta a los edificios residenciales, comerciales e industriales ordinarios.

En SimCity 4, algunas parcelas de campos de cultivo pueden aparecer en el estado de abandonado del juego como parte de su comportamiento normal.

También pueden figurar como sin agua.

**Esto no significa necesariamente que la granja haya dejado de funcionar.**

Por ese motivo, **Zona agrícola está desactivada por defecto**.

Con esta opción desactivada, se conservan las granjas y los campos agrícolas abandonados.

Si la activas, el Ministerio puede demoler las parcelas agrícolas abandonadas elegibles, incluidos campos que quizá quieras conservar.

**Usa esta opción con cuidado.**

Las ruinas en suelo agrícola se tratan por separado. Se pueden demoler igualmente cuando **Demoler edificios destruidos** está activada, aunque Zona agrícola esté desactivada.

#### Iconos de aviso de agua en las parcelas agrícolas

El Ministerio también oculta el icono de aviso de *sin agua* del juego sobre las parcelas agrícolas en la versión compatible del juego.

Este ajuste visual es independiente de la demolición automática y no cambia el servicio de agua real.

Consulta [Agua](water.md#water-farm-icon).

<a id="demolition-money"></a>
#### ¿Cómo paga el Ministerio las demoliciones?

**Cada demolición usa el precio real de excavadora que indica SimCity 4.**

El Ministerio no estima el precio a partir del tipo de edificio.

Antes de cada orden de demolición, pregunta al juego el coste de esa parcela concreta.

Si el juego no puede proporcionar un precio válido, **la parcela no se demuele**.

Se aplican dos protecciones financieras.

#### 1. La protección de la tesorería

El Ministerio nunca ordena demoliciones cuyo coste combinado supere la tesorería disponible de la ciudad al comienzo de la revisión mensual.

**Regla de la tesorería**

```math
c_i\le T-S_i
```

Donde:

- **cᵢ** es el precio de excavadora de la parcela actual.
- **T** es la tesorería al comienzo de la revisión de demolición.
- **Sᵢ** es el precio combinado de las órdenes de demolición ya enviadas durante esa revisión.

**¿Por qué resta el Ministerio las órdenes anteriores?**

Porque SimCity 4 no necesariamente descuenta los gastos de excavadora en el momento en que se envía una orden de demolición.

Sin esta protección, el Ministerio podría emitir varias órdenes contra el mismo dinero disponible.

Por ejemplo, si tu tesorería tiene §500 y el Ministerio ya ha ordenado demoliciones por §300, solo considera disponibles §200 para más órdenes.

**¿Qué ocurre cuando una parcela resulta demasiado cara?**

El Ministerio omite esa parcela y continúa con el siguiente candidato.

Puede que un edificio más barato todavía sea asequible.

**Si la tesorería es negativa, no se ordena ninguna demolición, ni siquiera una con un precio de §0.**

Esta protección de la tesorería siempre está activa, incluso cuando desactivas el presupuesto mensual de demolición opcional.

#### 2. El presupuesto mensual de demolición opcional

También puedes limitar cuánto puede gastar el Ministerio durante una sola revisión mensual de demolición.

La opción es:

**Limitar el presupuesto mensual de demolición**

Está **desactivada por defecto**.

Cuando está activada, el Ministerio aplica una regla adicional.

**Regla del presupuesto mensual**

```math
c_i=0
\quad\text{o}\quad
S_i+c_i\le B
```

Donde **B** es el presupuesto mensual de demolición que hayas configurado.

El valor por defecto del presupuesto es de **§1.000**.

**Una demolición con un precio de §0 no cuenta para este presupuesto opcional.**

Sin embargo, hay una diferencia importante entre esta regla y la protección de la tesorería.

**Si una demolición con precio superase el presupuesto mensual, se detiene toda la revisión de demolición.**

No omite ese edificio para seguir buscando otros más baratos.

Todos los candidatos restantes esperan hasta el mes siguiente, incluidas las parcelas posteriores que habrían sido gratuitas de demoler.

#### 3. Número máximo de órdenes de demolición

El Ministerio también limita el número de órdenes de demolición en una revisión mensual.

```math
N\le
\begin{cases}
3, & \text{antes de confirmar ninguna orden en la sesión}\\
150, & \text{tras la confirmación}
\end{cases}
```

El límite normal de 150 órdenes es un valor técnico por defecto.

Se puede ajustar mediante la clave de configuración:

`MaxPerRun`

Intervalo permitido: **1–1.000 órdenes**.

Este ajuste no está disponible en la ventana de opciones del juego.

La tesorería y el presupuesto opcional suelen imponer límites prácticos más bajos.

<a id="demolition-never"></a>
#### ¿Cuándo no demolerá nunca el Ministerio una parcela?

El Ministerio deja una parcela intacta cuando se da cualquiera de estos casos:

- El Ministerio está desactivado, la función de demolición correspondiente está desactivada, o falta la configuración de demolición o no es válida.
- La ciudad acaba de cargarse y todavía no se ha producido la primera revisión mensual de demolición.
- El edificio está ocupado, en construcción, a la espera de desarrollo o ya se está demoliendo.
- Es un edificio municipal o histórico que no ha sido destruido.
- Es una ruina municipal o histórica protegida y la opción especial de ruinas está desactivada.
- Está abandonado en un tipo de zona que has excluido.
- Está abandonado por un motivo que has excluido.
- No ha alcanzado el tiempo de espera configurado.
- SimCity 4 indica que la parcela no se puede demoler.
- El juego no puede proporcionar un precio de demolición válido.
- La tesorería no puede asumir la demolición.
- El presupuesto mensual de demolición opcional impide la orden.
- La parcela ha cambiado o no se pueden verificar los límites de su mapa.
- Un fallo de seguridad de la demolición ha detenido la gestión durante la sesión actual.
- La versión del juego no es compatible.

**Cuando hay dudas sobre la elegibilidad de una parcela o sobre la seguridad de la operación, el Ministerio la deja en paz.**

<a id="demolition-safety"></a>
### 4. Condiciones de seguridad

La demolición automática es una operación potencialmente destructiva, así que el Ministerio realiza varias comprobaciones antes y después de cada orden.

#### Uso del propio sistema de excavadora del juego

El Ministerio usa el comando normal de demolición de SimCity 4.

Envía el rectángulo exacto que ocupa la parcela elegible y no inventa parámetros especiales de demolición.

**No elimina directamente edificios de los datos internos del juego.**

#### Comprobación de la parcela justo antes de la demolición

Antes de cada orden, el Ministerio vuelve a leer la parcela.

Debe seguir:

- Existiendo en la posición prevista.
- Estando en el estado previsto.
- Cumpliendo las reglas de demolición.
- Teniendo las dimensiones previstas.

El rectángulo del mapa debe tener exactamente el área de la parcela y contener su celda de origen.

Si no se cumplen estas condiciones, la parcela se omite.

#### Asegurarse de que la consulta del precio no cambia nada

El Ministerio pregunta al juego el precio de la vista previa de la excavadora.

Esta consulta no debe modificar la parcela.

Si la parcela cambia inesperadamente durante la consulta del precio, **la demolición automática se detiene durante el resto de la sesión de la ciudad**.

#### Verificar que la demolición se ha producido realmente

Las órdenes de demolición no se completan necesariamente en el momento exacto en que se envían.

Por eso el Ministerio comprueba el resultado en la siguiente exploración de 10 días o revisión mensual.

La parcela debe haber desaparecido.

Si **el juego no ejecuta tres órdenes de demolición consecutivas**, la demolición automática se detiene durante el resto de la sesión de la ciudad.

#### Conservación de la zonificación

Tras confirmar que el edificio ha desaparecido, el Ministerio comprueba la zonificación de la casilla de origen de la parcela.

**La zona original debe seguir existiendo.**

Si la demolición elimina inesperadamente la zonificación, la demolición automática se detiene durante el resto de la sesión.

Esto protege la disposición de la ciudad que el jugador ha previsto.

#### Gestión de los errores de configuración y técnicos

Si el archivo de configuración de la demolición falta, no se puede leer o no es válido, el Ministerio no realiza ninguna demolición automática.

Los errores durante una revisión de demolición se capturan y se anotan en el registro. No detienen el propio SimCity 4.

**La demolición funciona con independencia de las demás funciones del Ministerio.**

Una parada de seguridad de la demolición no desactiva la gestión de la electricidad, el agua, la limpieza, la educación, la sanidad ni los contratos regionales.

Del mismo modo, el fallo de otro gestor no desactiva automáticamente la demolición.

<a id="demolition-settings"></a>
### 5. Ajustes

La demolición automática se configura en la página **Demolición** de la ventana de opciones del Ministerio.

Los ajustes aparecen en el siguiente orden:

| Ajuste | Valor por defecto | Qué controla |
|---|---|---|
| [Demoler edificios destruidos](options.md#opt-demolishdestroyed) | Sí | Despeja automáticamente las ruinas elegibles. |
| [Tiempo de espera (ruinas)](options.md#opt-destroyedmonths) | 1 mes | Cuánto tiempo deben permanecer las ruinas antes de su demolición. |
| [Demoler edificios municipales e históricos destruidos](options.md#opt-demolishmunicipal) | No | Permite demoler solo las ruinas de los edificios protegidos. |
| [Demoler edificios abandonados](options.md#opt-demolishabandoned) | Sí | Despeja automáticamente los edificios abandonados elegibles. |
| [Tiempo de espera (abandonados)](options.md#opt-abandonedmonths) | 12 meses | Cuánto tiempo deben permanecer abandonados los edificios. |
| [Zona residencial](options.md#opt-zoneresidential) | Sí | Incluye los edificios residenciales abandonados. |
| [Zona comercial](options.md#opt-zonecommercial) | Sí | Incluye los edificios comerciales abandonados. |
| [Zona industrial](options.md#opt-zoneindustrial) | Sí | Incluye los edificios industriales abandonados. |
| [Zona agrícola](options.md#opt-zoneagriculture) | No | Incluye las granjas y los campos abandonados. |
| [Motivo: zona poco atractiva](options.md#opt-reasonlowdesirability) | Sí | Incluye el abandono causado por poca atractividad. |
| [Motivo: trayecto al trabajo demasiado largo](options.md#opt-reasoncommute) | Sí | Incluye el abandono causado por trayectos largos. |
| [Motivo: falta de demanda](options.md#opt-reasonlowdemand) | Sí | Incluye el abandono causado por una demanda insuficiente. |
| [Motivo: falta de electricidad](options.md#opt-reasonnopower) | No | Incluye el abandono causado por la falta de electricidad. |
| [Motivo: falta de agua](options.md#opt-reasonnowater) | No | Incluye el abandono causado por la falta de agua. |
| [Limitar el presupuesto mensual de demolición](options.md#opt-budgetlimit) | No | Limita el gasto en demolición durante cada revisión mensual. |
| [Presupuesto mensual](options.md#opt-demolitionbudget) | §1.000 | Fija el límite de gasto cuando la opción de presupuesto está activada. |

Todos los ajustes de demolición automática dependen de [Ministerio activo](options.md#opt-master).

**Precauciones recomendadas**

Los ajustes por defecto protegen los campos agrícolas, los edificios abandonados por fallos temporales de los servicios y las estructuras municipales o históricas.

Si quieres una demolición más agresiva, revisa estas protecciones antes de cambiar las opciones, especialmente en ciudades con edificios históricos valiosos o con grandes zonas agrícolas.

<a id="demolition-examples"></a>
### 6. Ejemplos desarrollados

Estos ejemplos muestran cómo aplica el Ministerio los tiempos de espera y los límites financieros.

**Todos los precios de demolición que aparecen a continuación son ilustrativos.** Los precios reales se obtienen de SimCity 4 para cada parcela individual.

#### Ejemplo 1: una ruina alcanza su tiempo de espera

Cambias el tiempo de espera de las ruinas a **3 meses**.

El Ministerio lo guarda como:

**91 días simulados.**

Un edificio queda destruido por un incendio.

En una revisión mensual, el juego indica que las ruinas tienen **85 días**.

```math
85<91
```

El edificio es demasiado reciente y se deja en paz.

En la siguiente revisión mensual, unos 30 días después, las ruinas tienen unos **115 días**.

```math
115\ge91
```

La parcela ya es elegible.

**El Ministerio la demuele durante esa revisión si las comprobaciones de elegibilidad y financieras del juego lo permiten.**

El tiempo de espera es una antigüedad mínima, no una cita exacta de demolición.

#### Ejemplo 2: el presupuesto mensual detiene una revisión de demolición

Supón que tu ciudad tiene:

- **Tesorería:** §20.000.
- **Presupuesto mensual de demolición:** activado.
- **Límite del presupuesto:** §1.000.

El Ministerio ya ha confirmado una demolición correcta en la sesión actual, así que la precaución inicial de tres órdenes ya no se aplica.

Los candidatos se ordenan con las ruinas primero, seguidas de los edificios abandonados.

| Orden | Candidato | Precio | Gastado antes | Resultado del presupuesto |
|---|---|---:|---:|---|
| 1 | Ruina A | §40 | §0 | Demoler |
| 2 | Ruina B | §0 | §40 | Demoler |
| 3 | Abandonado C | §300 | §40 | Demoler |
| 4 | Abandonado D | §450 | §340 | Demoler |
| 5 | Abandonado E | §700 | §790 | **Parar: supera el presupuesto** |
| 6 | Abandonado F | §0 | — | Espera al mes siguiente |

Tras las cuatro primeras órdenes, el Ministerio ha comprometido:

```math
40+0+300+450=790
```

**§790 en gastos de demolición.**

El siguiente edificio cuesta §700.

```math
790+700=1490>1000
```

Esto superaría el presupuesto mensual configurado.

**El Ministerio detiene toda la revisión de demolición.**

Los edificios E y F esperan hasta el mes siguiente.

Fíjate en que demoler F no costaría nada, pero se aplaza igualmente porque la regla del presupuesto detiene la revisión en el primer candidato con precio que supera el límite.

#### Ejemplo 3: la regla de la tesorería omite un edificio caro

Ahora considera una ciudad más pequeña con:

- **Tesorería:** §250.
- **Presupuesto mensual de demolición:** desactivado.

El Ministerio evalúa:

| Orden | Candidato | Precio | Disponible antes | Decisión |
|---|---|---:|---:|---|
| 1 | Ruina A | §40 | §250 | Demoler |
| 2 | Ruina B | §60 | §210 | Demoler |
| 3 | Abandonado C | §300 | §150 | **Omitir** |
| 4 | Abandonado D | §100 | §150 | Demoler |

Tras ordenar A y B, el Ministerio ha comprometido §100.

El edificio C cuesta §300, que la ciudad no puede permitirse.

**El Ministerio omite C pero sigue comprobando a los demás candidatos.**

El edificio D cuesta solo §100 y todavía se puede demoler.

Las órdenes resultantes cuestan:

```math
40+60+100=200
```

**Se ordenan tres edificios por un total de §200.**

El edificio C permanece hasta una revisión posterior en la que haya dinero suficiente.

**La distinción es importante:**

- **Fallo de tesorería:** se omite la parcela que no se puede pagar y se continúa.
- **Fallo del presupuesto mensual:** la revisión se detiene por completo.

#### Ejemplo 4: instalar el Ministerio en una ciudad con muchos edificios abandonados

Imagina una ciudad con **400 edificios** que llevan abandonados más de un año por demanda insuficiente.

Instalas el Ministerio con sus ajustes de demolición por defecto.

Estos edificios ya tienen la antigüedad suficiente para cumplir los requisitos.

Sin embargo, **no se demuele nada de inmediato al abrir la ciudad**.

En el primer cambio de mes simulado, el Ministerio envía como máximo **tres órdenes de demolición**.

Durante la siguiente comprobación de 10 días, confirma que el juego llevó a cabo esas órdenes.

Cuando se ha confirmado al menos una demolición, queda disponible el límite normal.

En la siguiente revisión mensual, el Ministerio puede ordenar hasta **150 demoliciones**, siempre que la tesorería y el presupuesto mensual opcional lo permitan.

Los edificios elegibles más antiguos se procesan primero.

**Esta precaución inicial de tres órdenes evita que el Ministerio intente de inmediato cientos de demoliciones antes de confirmar que el comando de demolición del juego funciona correctamente.**

<a id="demolition-limitations"></a>
### 7. Limitaciones conocidas

La demolición automática reduce el trabajo manual repetitivo, pero varias restricciones son intencionadas.

#### Plazos y elegibilidad

- **La demolición es mensual.** Un edificio se procesa en la primera revisión mensual posterior a su tiempo de espera, es decir, potencialmente hasta aproximadamente un mes simulado más tarde.
- **Se conserva la antigüedad del abandono existente.** Los edificios abandonados antes de instalar el Ministerio mantienen su antigüedad original. Por eso pueden ser elegibles ya en la primera revisión mensual.
- **Cada sesión de ciudad empieza con cautela.** Hasta que se confirma una orden de demolición, cada revisión mensual envía como máximo tres órdenes.
- **El Ministerio se fía del motivo de abandono que indica el juego.** No predice si un edificio podría volver a ocuparse. El tiempo de espera y las opciones de motivo ofrecen esa protección.

#### Costes de la demolición

- **El juego determina los precios del bulldozer.** El Ministerio lee el precio real antes de cada orden, pero no muestra un aviso de coste aparte.
- **Los gastos de demolición aparecen como costes normales del bulldozer.** Despejar ruinas suele costar menos que demoler edificios abandonados, pero los precios reales dependen de la parcela.
- **El presupuesto mensual se aplica a una sola revisión de demolición.** Cuando está activado, detiene la revisión en el primer candidato con precio que superaría el límite.

#### Edificios protegidos

- **Los edificios municipales e históricos abandonados no se demuelen nunca.** Solo se pueden retirar sus ruinas, y solo cuando la opción de protección independiente está activada.
- **Los campos agrícolas pueden verse afectados.** Activar Zona agrícola permite demoler las parcelas de campos abandonadas que sean elegibles. Usa esta opción con cuidado.
- **El Ministerio nunca elimina deliberadamente la zonificación subyacente.** El terreno sigue disponible para reurbanizarlo.

#### Límites técnicos

- **Las paradas de seguridad duran lo que dura la sesión de ciudad en curso.** Al volver a abrir la ciudad, la gestión de la demolición puede empezar una sesión nueva.
- **Solo se gestiona la ciudad abierta en ese momento.** El Ministerio no inspecciona ni demuele edificios de las ciudades vecinas.
- **La demolición automática requiere la versión del juego compatible.** Las versiones no compatibles no se modifican.

**El propósito del Ministerio es despejar las ruinas elegibles y los edificios abandonados desde hace mucho tiempo, no tomar decisiones de reurbanización en lugar del jugador.**

---

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
