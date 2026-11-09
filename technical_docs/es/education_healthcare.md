[English](../en/education_healthcare.md) | [Português](../pt/education_healthcare.md) | [Français](../fr/education_healthcare.md) | **Español**

[Volver al contenido](index.md#contents) | [Volver al README](../../README.es.md)

<a id="education-healthcare"></a>
## A. Educación y sanidad

Las direcciones del ejecutable relacionadas con este capítulo figuran en la [referencia de direcciones del juego](../en/game_addresses.md) (en inglés).

El Ministerio ajusta automáticamente la financiación de cada escuela, instituto, universidad, biblioteca, museo, clínica y hospital que puede gestionar. Su objetivo es sencillo: **ofrecer capacidad suficiente para todos los que necesitan el servicio, con un margen de seguridad, gastando lo mínimo posible.**

Cada edificio se gestiona de forma individual. Una escuela con 200 alumnos no necesita el mismo presupuesto que una con 2.000.

El Ministerio solo ajusta el presupuesto de funcionamiento del edificio. **Nunca cambia la financiación de los autobuses escolares ni de las ambulancias, las áreas de cobertura de los edificios ni el número de instalaciones de tu ciudad.** Esas decisiones siguen siendo tuyas.

### Cómo afecta la financiación a la capacidad

En SimCity 4, cada edificio de educación y sanidad tiene un presupuesto de funcionamiento, que se muestra en simoleones (§) al mes. Internamente, el juego representa esa financiación como un porcentaje.

La financiación determina tanto lo que cuesta el edificio como cuántos alumnos, pacientes, visitantes o libros puede acoger.

La relación no es del todo proporcional:

| Financiación | Capacidad disponible | Coste mensual de funcionamiento |
|---|---:|---:|
| 50% | 50% | 50% |
| 75% | 75% | 75% |
| 100% | 100% | 100% |
| 120% | 110% | 120% |

Los porcentajes son relativos a la capacidad y al coste de funcionamiento del edificio con una financiación del 100%.

**Hasta el 100% de financiación**, la capacidad crece al mismo ritmo que el gasto. **Por encima del 100%**, cada punto porcentual adicional de financiación aporta solo la mitad de capacidad extra.

Por ejemplo, una escuela con 1.000 plazas al 100% de financiación puede acoger aproximadamente a 500 alumnos al 50%, o a 1.100 alumnos al 120%.

<a id="education-healthcare-capacity-curve"></a>
**Fórmula de la capacidad (juego base)**

```math
\phi(f) = \min(f,1) + \frac{\max(0,f-1)}{2}
```

```math
C = \left\lfloor B \times \phi(f) \right\rfloor
```

Donde:

- **B** es la capacidad del edificio con una financiación del 100%.
- **f** es el nivel de financiación (1,0 = 100%).
- **C** es la capacidad resultante.
- **φ(f)** es el multiplicador de capacidad del juego.

El juego usa redondeos internos y puede aplicar modificadores de capacidad adicionales. Por eso, la capacidad mostrada a veces puede diferir ligeramente de un cálculo directo. El Ministerio tiene en cuenta estas diferencias al tomar sus decisiones.

**Coste mensual de funcionamiento (juego base)**


```math
K = \mathrm{round}(K_0 \times f)
```

Donde **K₀** es el coste mensual de funcionamiento del edificio con una financiación del 100%.

Por ejemplo, una escuela que cuesta §200 al mes con el 100% de financiación cuesta aproximadamente §100 con el 50%.

### Cómo elige el Ministerio la financiación

El Ministerio comprueba cuántas personas usan cada edificio y calcula la capacidad que necesita mantener.

Por defecto, intenta conservar **al menos un 5% de capacidad libre**, pero puede mantener un margen mayor cuando es necesario.

Esto es especialmente importante en las instalaciones pequeñas, donde incluso un pequeño aumento de la asistencia puede suponer un gran cambio porcentual.

Por ejemplo, una clínica que pasa de 40 a 60 pacientes ha sufrido un aumento de la demanda del 50%. El Ministerio tiene en cuenta este tipo de fluctuaciones en lugar de aplicar a ciegas el mismo margen a todos los edificios.

Después elige el **nivel de financiación más bajo que puede cubrir con seguridad la capacidad necesaria**.

Si la demanda aumenta, la financiación puede subir. Si la demanda baja, el Ministerio puede reducir la financiación para evitar gastos innecesarios.

### ¿Con qué frecuencia comprueba y ajusta la financiación?

Todos los plazos siguientes se refieren al **tiempo simulado del juego**, no al tiempo real.

| Cuándo | Qué hace el Ministerio |
|---|---|
| **Al abrir una ciudad** | Realiza su primera evaluación completa el primer día simulado. Si algunos edificios aún no tienen datos utilizables, puede reintentarlo en los días siguientes. |
| **Cada 2 días simulados** | Busca edificios que puedan necesitar subidas urgentes de financiación. |
| **Cada día simulado durante una huelga** | Busca problemas urgentes para poder subir la financiación lo antes posible. |
| **Una vez al mes simulado** | Revisa a fondo cada edificio y ajusta la financiación al alza o a la baja según las necesidades actuales. |

**Las subidas de financiación no siempre esperan al mes siguiente.** Si un edificio se satura gravemente, o está en huelga, el Ministerio puede subir la financiación en sus comprobaciones más frecuentes.

**Las reducciones de financiación se hacen solo en la revisión mensual.** Así se evita recortar presupuestos demasiado deprisa tras una bajada temporal de la asistencia.

El Ministerio nunca baja la financiación mientras las propias cifras del juego indiquen riesgo de un servicio insuficiente.

<a id="education-healthcare-strike"></a>
### Cómo ayuda el Ministerio a evitar huelgas

En SimCity 4, las escuelas y los centros sanitarios pueden ponerse en huelga cuando su capacidad es demasiado baja en comparación con la demanda.

El juego usa un **ratio de cobertura** para determinar este riesgo.

```math
V = \min\left(1,\frac{C}{U}\right)
```

Donde:

- **C** es la capacidad disponible del edificio.
- **U** es su demanda.
- **V** es el ratio de cobertura.

Un ratio de cobertura de **1,0 (100%)** significa que el edificio tiene capacidad suficiente para su demanda.

Un ratio inferior a **0,8 (80%)** crea riesgo de huelga. Cuanto más bajo es el ratio, mayor es el riesgo.

**Probabilidad de huelga (juego base)**

```math
S(V) = 100 \times \max\left(0,1-\frac{V}{0.8}\right)
```

| Cobertura | Probabilidad de huelga |
|---|---:|
| 100% | 0% |
| 80% | 0% |
| 60% | 25% |
| 40% | 50% |
| 0% | 100% |

Una huelga termina cuando el ratio de cobertura del juego alcanza el 90%.

Normalmente, el Ministerio mantiene más capacidad que el mínimo necesario para evitar huelgas. Además, vigila que los edificios no sufran aumentos inesperados de la demanda.

Si ya hay una huelga en curso, **el Ministerio sube la financiación del edificio hasta el máximo configurado**, siempre que pueda controlar ese edificio.

El cálculo interno de cobertura del juego a veces puede diferir de la demanda que muestra la ventana de consulta del edificio. Por ese motivo, el Ministerio también usa la propia información de cobertura del juego para evitar reducciones de financiación peligrosas.

### ¿Qué ocurre cuando la financiación ya no basta?

La financiación no puede resolver todos los problemas de capacidad.

Con una financiación del 120%, un edificio ofrece solo un 110% de su capacidad normal. Si la demanda supera lo que el edificio puede atender con seguridad con su financiación máxima, el Ministerio no puede crear plazas adicionales.

**Tienes que construir otra instalación.**

Cuando el Ministerio confirma que una carencia no se puede resolver con financiación y persiste durante **30 días simulados**, envía un aviso de un asesor dentro del juego.

Cuando es posible, el aviso incluye un enlace que te lleva directamente al edificio afectado.

El Ministerio no construye edificios ni cambia sus áreas de cobertura. Tú sigues siendo quien decide dónde colocar nuevas escuelas, hospitales y otras instalaciones.

Para más salvaguardas y casos especiales, consulta las [condiciones de seguridad](#education-healthcare-safety).
<a id="education-healthcare-problem"></a>
### 2. El problema de la gestión

Gestionar a mano los presupuestos de educación y sanidad es más complicado de lo que parece.

- **Dinero desperdiciado:** una escuela medio vacía no necesita la misma financiación que una llena. Mantener todos los edificios al 100% puede malgastar miles de simoleones con el tiempo.
- **Riesgo de huelgas:** recortar demasiado la financiación reduce la capacidad y puede provocar huelgas. El Ministerio debe ahorrar dinero sin poner en riesgo los servicios.
- **Demanda cambiante:** el número de alumnos y pacientes fluctúa a medida que crece la ciudad. Algunos cambios pueden ser repentinos, sobre todo cuando los alumnos pasan de un nivel educativo a otro.
- **Edificios pequeños:** una clínica que pasa de 40 a 60 pacientes sufre un aumento de la demanda del 50%. Las instalaciones pequeñas necesitan una protección extra frente a estas fluctuaciones.
- **Edificios nuevos:** el juego puede tardar unos días simulados en informar de cuántas personas usan una instalación recién colocada.
- **Capacidad limitada:** incluso con una financiación del 120%, un edificio solo puede ofrecer alrededor del 110% de su capacidad normal. Más allá, el jugador debe construir otra instalación.
- **Demasiados edificios que revisar:** en una ciudad grande, revisar a mano cada escuela, biblioteca, museo, clínica y hospital todos los meses resulta tedioso.

**El Ministerio lo resuelve vigilando cada edificio de forma individual, manteniendo capacidad suficiente para que los servicios sigan funcionando y reduciendo el gasto innecesario siempre que es seguro hacerlo.**

<a id="education-healthcare-algorithm"></a>
### 3. Cómo toma sus decisiones el Ministerio

<a id="education-healthcare-eligibility"></a>
#### 3.1 ¿Qué edificios se pueden gestionar?

El Ministerio gestiona automáticamente los edificios de educación y sanidad que usan el sistema de financiación estándar del juego.

Esto incluye:

| Educación | Sanidad |
|---|---|
| Escuelas de primaria | Clínicas |
| Institutos | Hospitales |
| Facultades y universidades | Centros de investigación de enfermedades |
| Bibliotecas | |
| Museos | |

También se pueden gestionar edificios personalizados compatibles, siempre que usen los mismos mecanismos de financiación que el juego original.

Antes de gestionar un edificio, el Ministerio comprueba que:

1. El juego lo identifica como una instalación de educación o sanidad.
2. Tiene su propio presupuesto de funcionamiento que realmente se puede ajustar.
3. Su ventana de consulta ofrece información reconocible de demanda y capacidad.
4. Las cifras de su financiación y de su presupuesto son coherentes.

**Si un edificio no cumple estas condiciones, el Ministerio lo deja en paz.**

Por ejemplo, las escuelas privadas originales del juego no tienen un presupuesto de funcionamiento propio que se pueda ajustar, así que el Ministerio no tiene nada que gestionar. Algunos edificios personalizados también usan sistemas de financiación o ventanas de consulta especiales que el Ministerio no puede controlar con seguridad.

Los edificios se identifican por cómo funcionan, **no por su nombre**. Por tanto, una escuela personalizada compatible puede gestionarse igual que una escuela original de SimCity 4.

Estos servicios no gestionan las comisarías, los parques de bomberos ni las cárceles.

<a id="education-healthcare-required"></a>
#### 3.2 ¿Cuánta capacidad necesita un edificio?

El Ministerio no se limita a ajustar la financiación a la asistencia actual. Mantiene **capacidad extra para afrontar aumentos inesperados de la demanda**.

El margen de seguridad por defecto es del **5%**, pero el Ministerio puede mantener automáticamente más cuando un edificio necesita protección adicional.

Tiene en cuenta tres factores:

**1. El margen de seguridad que has elegido**

Por defecto, un edificio que atiende a 1.000 personas debería tener capacidad para al menos 1.050.

Puedes cambiar este margen en los ajustes del Ministerio.

**2. Aumentos anteriores de la demanda**

El Ministerio recuerda cuánto ha aumentado el uso de un edificio de un mes a otro.

Usa hasta **36 observaciones mensuales de la sesión de juego actual** y busca el mayor aumento. Solo se incluyen las comparaciones que parten de al menos 50 usuarios, porque las cifras muy pequeñas pueden producir porcentajes engañosos.

Por seguridad, el aumento usado en el cálculo se limita al **50%**.

Los institutos, las facultades y las universidades reciben una protección especial. Los traslados de alumnos entre niveles educativos pueden causar grandes aumentos anuales, así que el Ministerio supone un posible aumento del 50% hasta que ha reunido 13 observaciones mensuales.

**Mayor aumento mensual**

```math
v = \min\left(0.5,\ \max\left(0,\ \max_i\left(\frac{U_i}{U_{i-1}}-1\right)\right)\right)
```

Donde:

- **Uᵢ** es el número de usuarios registrado en el mes *i*.
- **v** es el mayor aumento mensual válido, expresado como fracción y limitado a 0,5.

**3. Protección extra para los edificios pequeños**

Las instalaciones pequeñas pueden sufrir grandes cambios porcentuales incluso cuando solo llegan unas pocas personas.

Por eso el Ministerio mantiene capacidad adicional:

| Uso actual | Margen mínimo de capacidad |
|---|---|
| Menos de 100 usuarios | 50% de capacidad extra |
| De 100 a 199 usuarios | 25% de capacidad extra |
| 200 usuarios o más | El margen que has elegido, ajustado según las fluctuaciones de la demanda cuando sea necesario |

Por ejemplo, una clínica con 60 pacientes mantiene capacidad para al menos 90 pacientes, aunque el margen de seguridad que has configurado sea solo del 5%.

Esta protección adicional es especialmente útil en los edificios pequeños, donde el coste extra de funcionamiento suele ser modesto.

**Cómo funcionan juntas estas protecciones**

El Ministerio compara los tres requisitos y usa el que exige más capacidad:

- El margen de seguridad que has configurado.
- La protección frente a aumentos anteriores de la demanda y frente a huelgas.
- La protección extra para los edificios pequeños.

**Fórmula del margen de seguridad **

```math
M = \max\left(1+\frac{r}{100},\ 0.85(1+v),\ s(U)\right)
```

Donde:

- **r** es el margen de seguridad que has configurado, un 5% por defecto.
- **v** es el mayor aumento mensual descrito arriba.
- **U** es el uso actual del edificio.
- **s(U)** vale 1,5 por debajo de 100 usuarios, 1,25 de 100 a 199 usuarios y 0 en los demás casos.
- **M** es el multiplicador de capacidad resultante.

El factor 0,85 de la segunda regla pretende mantener la cobertura por encima del umbral de riesgo de huelga del 80% del juego, incluso después de un aumento de la demanda observado anteriormente.

**Capacidad requerida **

```math
R = \left\lceil U \times M \right\rceil
```

Donde **R** es la capacidad mínima que el Ministerio quiere que ofrezca el edificio.

Por ejemplo, una escuela con 610 alumnos, sin aumentos de demanda inusualmente grandes y con un margen de seguridad del 5% necesita:

```math
R = \lceil 610 \times 1.05 \rceil = 641
```

Por tanto, el Ministerio busca el nivel de financiación más barato que ofrezca al menos 641 plazas, con un pequeño margen para el comportamiento de redondeo del juego.

<a id="education-healthcare-choice"></a>
#### 3.3 ¿Cómo elige el Ministerio la financiación?

Una vez conocida la capacidad requerida, el Ministerio comprueba qué nivel de financiación puede ofrecerla.

Elige **el porcentaje entero más bajo que cumple el requisito con seguridad**.

Por defecto, considera niveles de financiación **del 1% al 120%**.

**Elección de la financiación **

```math
p^* = \min\left\{p\in[p_{\min},p_{\max}]: C(p)-1\ge R\right\}
```

Donde:

- **p** es un posible porcentaje de financiación.
- **C(p)** es la capacidad esperada con esa financiación.
- **R** es la capacidad requerida calculada arriba.
- Restar una unidad ofrece una pequeña salvaguarda frente al redondeo de la capacidad del juego.

Por ejemplo, si una escuela necesita 641 plazas y el 54% de financiación ofrece aproximadamente 647, el Ministerio elige el **54%**, suponiendo que el 53% no pueda ofrecer capacidad suficiente.

Así se evita pagar por cientos de plazas sin usar.

**Reglas de seguridad adicionales**

El Ministerio también sigue varias reglas importantes:

- **Sin cambios innecesarios:** si la financiación ya está en el nivel adecuado, la deja como está.
- **Huelgas:** si un edificio está en huelga, su financiación sube directamente al máximo configurado, normalmente el 120%.
- **Edificios nuevos:** si el juego aún no ha informado de la asistencia, el Ministerio no da por hecho que el edificio está vacío. Mantiene la financiación al menos al 100%, o al máximo configurado si este es menor, hasta que haya datos utilizables.
- **Sin reducciones peligrosas:** la financiación nunca se baja mientras la cobertura que informa el juego sea inferior al 85% o haya una huelga en curso.
- **Sin financiación cero:** con la configuración por defecto, el Ministerio nunca deja la financiación de un edificio en el 0%.
- **Las reducciones son mensuales:** la financiación puede subir en las comprobaciones urgentes, pero las reducciones solo se hacen en la revisión mensual.

Si ni siquiera la financiación máxima puede ofrecer la capacidad deseada, el Ministerio deja el edificio en su máximo y comprueba si existe una carencia real.

Que un edificio quede ligeramente por debajo de su margen de seguridad no significa necesariamente que haga falta otra instalación. El Ministerio solo te avisa cuando la capacidad disponible es realmente insuficiente y el problema persiste.

Consulta las [carencias](#education-healthcare-shortage) para ver las condiciones que activan un aviso de un asesor.

<a id="education-healthcare-capacity-evidence"></a>
#### 3.4 ¿Y si la capacidad real del juego difiere del cálculo?

SimCity 4 no siempre informa exactamente de la capacidad que predice su fórmula de financiación estándar.

Las pequeñas diferencias de redondeo son normales. Algunas ciudades o edificios personalizados también pueden aplicar modificadores de capacidad adicionales.

Por eso el Ministerio **comprueba la capacidad real que informa el juego antes de cambiar la financiación de un edificio**.

Distingue tres situaciones:

| Situación | Qué hace el Ministerio |
|---|---|
| **La capacidad coincide con el cálculo** | Usa la fórmula estándar y gestiona el edificio con normalidad. Se acepta como redondeo normal una diferencia de una plaza. |
| **La capacidad sigue un modificador constante** | Aprende la diferencia y ajusta sus cálculos, tomando precauciones hasta que se confirme el modificador. |
| **La capacidad se comporta de forma impredecible** | Evita las reducciones automáticas de financiación en lugar de arriesgarse a una pérdida inesperada de capacidad. |

**Aprender la capacidad real de un edificio**

Por ejemplo, una escuela personalizada podría ofrecer de forma constante un 15% más de capacidad de la que predice la fórmula estándar.

En lugar de ignorar esa capacidad extra o de fiarse de ella de inmediato, el Ministerio comprueba si la diferencia es constante.

Cuando procede, puede hacer una pequeña subida de financiación, segura, durante una revisión mensual para confirmar cómo responde la capacidad. Después usa la relación verificada para elegir la financiación con más precisión.

Esta calibración es prudente: solo se intenta cuando el edificio ya tiene capacidad suficiente y no da por hecha una bonificación sin confirmar.

Si la capacidad no se puede predecir de forma fiable, el Ministerio deja sin cambios la financiación del edificio o se limita a subidas seguras cuando la capacidad observada es insuficiente.

**La prioridad es siempre evitar recortes de financiación peligrosos.** Ahorrar unos pocos simoleones no compensa arriesgar una escuela o un hospital cuya capacidad el juego informa de forma incoherente.

<a id="education-healthcare-timing"></a>
#### 3.5 ¿Cuándo comprueba y ajusta la financiación el Ministerio?

El Ministerio vigila los edificios de educación y sanidad durante toda la simulación. **Puede subir la financiación cuando aparece un problema, pero solo la reduce durante su revisión mensual.**

Todos los plazos se refieren al tiempo simulado del juego.

| Cuándo | Qué ocurre | Subidas de financiación | Reducciones de financiación |
|---|---|---|---|
| **Al abrir una ciudad** | Comprueba todos los edificios gestionados el primer día simulado. Si algunos edificios aún no tienen datos de asistencia, puede repetir la comprobación en los días siguientes, hasta seis veces en total. | Sí | No |
| **Cada 2 días simulados** | Busca edificios que necesitan atención urgente por saturación o financiación insuficiente. | Sí | No |
| **Cada día durante una huelga** | Busca problemas urgentes de financiación mientras se informa de una huelga de educación o sanidad. | Sí | No |
| **Cada mes simulado** | Revisa todos los edificios, actualiza el historial de asistencia y ajusta la financiación a las necesidades actuales. | Sí | Sí |

**¿Qué se considera un problema urgente?**

El Ministerio interviene entre las revisiones mensuales cuando un edificio:

- No está ya en su financiación máxima.
- Necesita una subida de financiación según sus reglas de seguridad.
- Está en huelga, o su capacidad actual está más de un 3% por debajo de su demanda.

La última condición se expresa así:

```math
C < 0.97 \times U
```

Donde:

- **C** es la capacidad disponible del edificio.
- **U** es su uso actual.

Por ejemplo, una escuela con capacidad para 900 alumnos y una asistencia de 1.000 está claramente saturada. El Ministerio puede subir su financiación sin esperar al mes siguiente.

Un problema nuevo o cambiante se evalúa en las comprobaciones habituales. Si el mismo problema persiste sin cambios, el Ministerio puede repetir su evaluación cada **7 días simulados**.

A un edificio que ya está en la financiación máxima no le ayuda otra subida. El Ministerio trata esa situación como una posible carencia de capacidad.

**La prioridad es siempre restablecer primero los servicios y optimizar el gasto después.**

<a id="education-healthcare-buses"></a>
#### 3.6 ¿Qué cambia el Ministerio y cómo comprueba su trabajo?

El Ministerio cambia **solo el presupuesto de funcionamiento de cada edificio elegible**, con el mismo sistema de financiación que el control deslizante de presupuesto del propio juego.

Nunca cambia:

- La financiación de los autobuses escolares.
- La financiación de las ambulancias.
- Las áreas de cobertura de los edificios.
- La ubicación de los edificios ni el número de instalaciones.

**Cada cambio de financiación se comprueba antes y después de hacerse.**

Antes de cambiar un presupuesto, el Ministerio vuelve a leer los ajustes actuales del edificio. Si has movido tú el control deslizante desde que se hizo el cálculo, el Ministerio omite ese cambio y lo recalcula más tarde.

Tras hacer un cambio, verifica que:

1. El juego informa correctamente de la nueva financiación.
2. El coste mensual de funcionamiento del edificio es coherente con la nueva financiación.
3. La capacidad disponible responde como se esperaba.
4. La financiación de los autobuses escolares o de las ambulancias, donde existan, no ha cambiado.

**¿Qué ocurre si algo sale mal?**

Si un cambio no se puede verificar, el Ministerio deja de gestionar ese edificio concreto durante el resto de la sesión actual de la ciudad.

Si fallan estas comprobaciones tres edificios distintos, deja de hacer cualquier cambio de financiación de educación y sanidad hasta que se vuelva a abrir la ciudad.

Esto evita intentos repetidos de modificar edificios cuyos sistemas de financiación se comportan de forma inesperada.

**¿Puedes seguir cambiando un presupuesto a mano?**

Sí. Puedes ajustar tú mismo la financiación de cualquier edificio. El Ministerio detecta el cambio y lo tiene en cuenta en la siguiente evaluación, siempre que ese edificio siga bajo gestión automática.

Si quieres conservar el control manual completo de estos presupuestos, desactiva **Gestionar la educación** o **Gestionar la sanidad** en los ajustes del Ministerio.

<a id="education-healthcare-shortage"></a>
#### 3.7 ¿Qué ocurre cuando la financiación no basta?

Algunos edificios se saturan tanto que ni siquiera la financiación máxima puede ofrecer capacidad suficiente.

En esa situación, **el Ministerio no puede resolver el problema. Necesitas otro edificio.**

Sin embargo, el Ministerio no trata de inmediato como carencia todo edificio saturado.

Primero comprueba si más financiación podría resolver el problema.

| Situación | Qué hace el Ministerio |
|---|---|
| **La financiación está por debajo del máximo** | Sube la financiación cuando es necesario. Todavía no considera inevitable la carencia. |
| **La financiación está al máximo, con capacidad suficiente** | Mantiene la financiación del edificio y no emite ningún aviso de carencia. |
| **La financiación está al máximo, pero la cobertura es inferior al 85%** | Confirma que el edificio no puede atender adecuadamente su demanda actual. |
| **El edificio está en huelga pero normalmente podría ofrecer capacidad suficiente** | Trata la huelga como un problema temporal y no como prueba de que hace falta un edificio nuevo. |

**Cobertura máxima alcanzable **

```math
V_{\max} = \min\left(1,\frac{C_{\max}}{U}\right)
```

Donde:

- **Cₘₐₓ** es la capacidad disponible con la financiación máxima.
- **U** es la demanda actual del edificio.
- **Vₘₐₓ** es la mayor cobertura que el edificio puede alcanzar.

Si el edificio está en huelga y su capacidad informada se ve afectada temporalmente, el Ministerio estima cuál sería su capacidad sin la huelga.

**Una carencia se confirma cuando la cobertura máxima alcanzable es inferior al 85%.**

Por ejemplo, un hospital que puede atender a 1.500 pacientes con la financiación máxima pero tiene una demanda de 2.000 pacientes solo puede cubrir el 75% de esa demanda.

Ninguna subida de financiación puede resolverlo. Hace falta otro centro sanitario.

Aun así, el Ministerio espera antes de avisarte, para no informar de problemas temporales que pueden resolverse solos.

<a id="education-healthcare-notices"></a>
#### 3.8 ¿Cuándo y cómo te avisa el Ministerio?

El Ministerio intenta gestionar en silencio los problemas cotidianos de financiación.

**No recibes mensajes cada vez que ahorra dinero, sube la financiación o evita una posible huelga.**

Te avisa cuando un edificio realmente necesita tu intervención.

**¿Cuándo se envía un aviso?**

Una carencia debe seguir confirmada en todas las evaluaciones durante **30 días simulados** antes de que el Ministerio emita un aviso.

Si el edificio se recupera durante ese periodo, la cuenta atrás empieza de nuevo si la carencia vuelve.

Los edificios que están en huelga en ese momento no se incluyen en los avisos de carencia, porque SimCity 4 ya ofrece sus propios avisos de huelga.

**¿Qué muestra el aviso?**

El aviso aparece en la lista normal de noticias de los asesores del juego.

Te dice qué tipo de edificio necesita más capacidad y ofrece enlaces en los que puedes hacer clic para ir directamente a los lugares afectados.

Por ejemplo, un aviso de escuela saturada podría decir:

> **¡Sobran alumnos, falta espacio!**
>
> Alcalde *(tu nombre)*, ¡esta escuela está a reventar! La financiación ya está al máximo, pero los alumnos siguen llegando. ¡Va siendo hora de construir otra escuela, antes de que haya que dar las clases en el patio! Para verlo de cerca, visita **la escuela** donde los pupitres han pedido refuerzos.

En el juego, **la escuela** es un enlace en el que se puede hacer clic y que mueve la cámara hasta el edificio afectado.

**¿Cómo se tratan varias carencias?**

- El Ministerio agrupa las carencias por tipo de edificio, como escuelas de primaria, hospitales o bibliotecas.
- Cada aviso puede enumerar hasta **10 edificios**, mostrando primero los más saturados.
- Los demás edificios afectados siguen vigilados y pueden aparecer cuando se resuelvan las carencias anteriores.
- Si cambian los edificios afectados, el Ministerio actualiza el aviso existente siempre que es posible, en lugar de crear mensajes nuevos innecesarios.
- Si cambia el número de edificios enumerados, el aviso antiguo se sustituye. Los cambios repetidos pueden activar una **pausa de 30 días** entre sustituciones para no inundar la lista de asesores.
- Cuando se recuperan todos los edificios cubiertos por un aviso de carencia, el aviso se retira.

**¿Aparecerá el mismo aviso cada vez que vuelvas a abrir tu ciudad?**

No. El Ministerio recuerda de qué carencias ya ha informado para cada ciudad identificable.

Si cierras y vuelves a abrir una ciudad con el mismo problema sin resolver, normalmente no repite el aviso. Descartar un aviso también evita que se vuelva a anunciar la misma carencia sin cambios.

Esta información se guarda aparte de los archivos de guardado de tus ciudades.

**¿Dónde aparecen los avisos?**

Los avisos de carencia de educación y sanidad se entregan a través de los propios asesores del juego. Aparecen en la lista de noticias de los asesores sin abrir ninguna ventana emergente intrusiva.

Para el sistema de avisos compartido con otros servicios, consulta [Avisos de los asesores](safety.md#notices).

<a id="education-healthcare-safety"></a>
### 4. Condiciones de seguridad

El Ministerio sigue varias salvaguardas para no reducir los servicios por error.

**Antes de cambiar la financiación**

- Solo gestiona edificios cuyo sistema de financiación estándar se puede identificar y verificar.
- Nunca da por hecho que la falta de datos de asistencia significa que un edificio está vacío.
- Vuelve a comprobar la financiación y la capacidad del edificio antes de hacer un cambio.
- Deja en paz los edificios con lecturas poco fiables o incoherentes.

**Al reducir la financiación**

- Las reducciones se hacen solo durante la revisión mensual.
- La financiación nunca se reduce mientras un edificio está en huelga.
- La financiación nunca se reduce cuando la cobertura que informa el juego es inferior al 85%.
- Los edificios sin datos de asistencia utilizables nunca quedan infrafinanciados.
- Los edificios cuya capacidad no se puede predecir de forma fiable están protegidos contra las reducciones automáticas.
- Con la configuración por defecto, la financiación nunca baja del 1%.

**Al subir la financiación**

- Un edificio en huelga sube hasta su financiación máxima configurada.
- Los edificios con problemas urgentes de capacidad pueden recibir subidas entre las revisiones mensuales.
- Si la financiación máxima no puede resolver una carencia real, el Ministerio deja el edificio en la financiación máxima y, cuando el problema persiste, te avisa.

**Tras hacer un cambio**

- El Ministerio comprueba que el nuevo presupuesto y la capacidad son coherentes.
- La financiación de los autobuses escolares y de las ambulancias debe permanecer sin cambios.
- Un edificio que no supera la verificación queda excluido de nuevos cambios automáticos durante la sesión.
- Tres fallos de verificación distintos detienen los cambios de financiación de educación y sanidad durante esa sesión.

**Cuando la gestión automática está desactivada**

Apagar el Ministerio no restaura los presupuestos anteriores. Cada edificio conserva su financiación actual hasta que la cambies a mano o vuelvas a activar la gestión.

#### Los mensajes originales de los asesores del juego

SimCity 4 normalmente muestra avisos cuando la financiación de la educación o de la sanidad baja de ciertos porcentajes.

Por ejemplo, el juego puede quejarse de presupuestos de educación por debajo del 90 %, incluso cuando una escuela tiene capacidad de sobra con una financiación mucho menor.

Estos avisos no siempre son relevantes cuando el Ministerio gestiona los edificios de uno en uno.

Mientras el Ministerio gestiona la educación, o la sanidad, el plugin ajusta por eso algunos consejos originales del juego para ese servicio:

- Se suprimen los avisos rutinarios basados solo en porcentajes bajos de financiación.
- También se suprimen algunos avisos sobre exceso de financiación o niveles de personal.
- Los avisos previos a una huelga relevantes se limitan a las situaciones en que hay una huelga.
- **Las alertas de huelga reales siguen activas.**
- Se conservan los mensajes normales de elogio y de estado del juego.

Esto sigue el estado del Ministerio en cada ciudad, sin reiniciar el juego. Si desactivas la gestión de la educación o de la sanidad (o todo el Ministerio), o si el Ministerio no puede actuar sobre ella, los consejos originales del juego para ese servicio vuelven al instante.

Consulta la [seguridad general](safety.md#safety) para más información.

#### Si el juego mueve un control por sí solo

Cuando el juego cambia la financiación de una escuela o de un hospital que el Ministerio había fijado, justo después de un recálculo mensual, el Ministerio reevalúa ese edificio en su siguiente vigilancia, en ambos sentidos, y aplica la financiación más baja que aún da la capacidad exigida. Véase [Phantom Funding Slider Bug](phantom_slider.md).

<a id="education-healthcare-settings"></a>
### 5. Ajustes

La gestión de la educación y de la sanidad se puede configurar directamente en **Opciones de juego → Ministerio de Optimización**, en la página **Servicios**.

| Ajuste | Valor por defecto | Rango | Qué hace |
|---|---|---|---|
| [Ministerio activo](options.md#opt-master) | Sí | Sí / No | Activa o desactiva todo el Ministerio. |
| [Gestionar la educación](options.md#opt-education) | Sí | Sí / No | Gestiona automáticamente la financiación de las escuelas, facultades, universidades, bibliotecas y museos elegibles. |
| [Margen de seguridad (educación)](options.md#opt-educationreserve) | 5% | 0–50% | Establece la capacidad libre mínima que el Ministerio intenta mantener en los edificios de educación. |
| [Gestionar la sanidad](options.md#opt-health) | Sí | Sí / No | Gestiona automáticamente la financiación de las clínicas, hospitales y centros de investigación de enfermedades elegibles. |
| [Margen de seguridad (sanidad)](options.md#opt-healthreserve) | 5% | 0–50% | Establece la capacidad libre mínima que el Ministerio intenta mantener en los edificios sanitarios. |

**¿Cómo funciona el margen de seguridad?**

Un margen de seguridad del 5% significa que un edificio con 1.000 usuarios debería tener capacidad para al menos 1.050.

Sin embargo, **el 5% es un mínimo, no un objetivo fijo**. El Ministerio puede mantener más capacidad cuando un edificio es pequeño o ha sufrido grandes aumentos de la demanda.

Aumentar el margen ofrece un colchón adicional frente a un crecimiento repentino, pero también puede aumentar los costes de funcionamiento.

Los cambios hechos desde la ventana de ajustes del juego se aplican de inmediato. Si editas a mano los archivos de configuración, esos cambios se leen en el siguiente mes simulado.

Los ajustes no válidos no se corrigen en silencio. Un valor fuera de rango o un ajuste desconocido puede hacer que la gestión de la educación y de la sanidad siga desactivada hasta que se corrija la configuración.

#### Ajustes avanzados (solo en el archivo INI)

Hay dos ajustes adicionales disponibles en el archivo de configuración. No aparecen en la ventana de opciones del juego.

| Ajuste | Valor por defecto | Rango permitido | Qué hace |
|---|---|---|---|
| `MinFundingPercent` | 1% | 0–100% | Establece el porcentaje de financiación más bajo que el Ministerio puede elegir. |
| `MaxFundingPercent` | 120% | 1–120% | Establece el porcentaje de financiación más alto que el Ministerio puede elegir, también durante las huelgas. |

**Financiación mínima:** el 1% por defecto impide que el Ministerio retire por completo el presupuesto de funcionamiento de un edificio, incluso cuando no tiene usuarios registrados.

**Financiación máxima:** el 120% por defecto permite que los edificios funcionen con el nivel de financiación más alto disponible en el juego original. Bajar este límite restringe el gasto, pero también reduce la capacidad que el Ministerio puede ofrecer durante una saturación o una huelga.

El mínimo no debe superar al máximo.

Para la mayoría de los jugadores, **se recomiendan los valores por defecto**.

<a id="education-healthcare-examples"></a>
### 6. Ejemplos prácticos

Los siguientes ejemplos muestran cómo ajusta el Ministerio la financiación en distintas situaciones.

Las capacidades y los costes de funcionamiento de los edificios son ilustrativos. Tus cifras pueden diferir según el edificio y los mods que uses.

El juego también usa redondeos internos, así que las capacidades mostradas pueden diferir a veces en una unidad de un simple cálculo porcentual.

#### Ejemplo 1: una escuela con más capacidad de la que necesita

Una escuela de primaria tiene:

- **Capacidad normal máxima:** 1.200 alumnos.
- **Asistencia actual:** 610 alumnos.
- **Coste de funcionamiento con el 100% de financiación:** §187/mes.
- **Margen de seguridad:** 5%.

El Ministerio calcula la capacidad requerida:

```math
R = \lceil 610 \times 1.05 \rceil = 641
```

Después compara niveles de financiación:

| Financiación | Capacidad disponible | Resultado |
|---|---:|---|
| 53% | 635 | No basta |
| 54% | 647 | Basta |
| 100% | 1.200 | Más de lo necesario |

**El Ministerio elige el 54% de financiación.**

El coste mensual de funcionamiento baja de §187 a aproximadamente **§101**, con un ahorro de **§86 al mes**.

La financiación de los autobuses escolares no cambia.

#### Ejemplo 2: una clínica pequeña necesita un margen de seguridad mayor

Una clínica tiene una capacidad normal de 1.400 pacientes, pero ahora solo atiende a 60.

Aunque el margen de seguridad configurado es del 5%, los edificios pequeños reciben una protección adicional frente a cambios repentinos en la asistencia.

Como esta clínica tiene menos de 100 pacientes, el Ministerio mantiene un **50% de capacidad extra**:

```math
R = \lceil 60 \times 1.5 \rceil = 90
```

Con el 6% de financiación, la clínica solo puede atender a 84 pacientes. Con el 7%, puede atender aproximadamente a 98.

**El Ministerio elige el 7% de financiación.**

Es algo más de lo que exigiría el margen habitual del 5%, pero ofrece una protección útil frente a aumentos repentinos con un coste adicional muy pequeño.

#### Ejemplo 3: un instituto con cambios estacionales en la asistencia

Un instituto tiene:

- **Capacidad normal:** 5.000 alumnos.
- **Asistencia actual:** 900 alumnos.
- **Menos de 13 meses de historial de asistencia en la sesión actual.**

Los institutos pueden recibir gran cantidad de alumnos cuando los estudiantes pasan de primaria.

Hasta que se hayan reunido suficientes observaciones mensuales, el Ministerio supone un posible **aumento del 50% en la asistencia**.

Esto da una capacidad requerida de:

```math
R = \lceil 900 \times 0.85 \times 1.5 \rceil = 1148
```

**El Ministerio elige el 23% de financiación**, que ofrece aproximadamente 1.149 plazas.

Tras 13 observaciones mensuales, supongamos que el mayor aumento registrado en realidad fue solo del 8%.

Entonces el Ministerio puede fiarse del patrón de demanda observado y volver al margen de seguridad normal del 5%.

La capacidad requerida pasa a ser 945 alumnos.

**La financiación puede bajar al 19%**, que ofrece aproximadamente 949 plazas.

El Ministerio evita el gasto innecesario sin dejar de tener en cuenta la demanda estacional.

#### Ejemplo 4: una biblioteca con capacidad sin usar

Una biblioteca tiene una capacidad de colección de 40.000 libros, pero ahora solo hay 9.000 prestados.

Con un margen del 5%, el Ministerio necesita capacidad para:

```math
R = \lceil 9000 \times 1.05 \rceil = 9450
```

Con el 23% de financiación, la capacidad es de aproximadamente 9.199 libros, lo cual no basta.

Con el 24%, la capacidad llega a aproximadamente 9.599.

**El Ministerio elige el 24% de financiación.**

La biblioteca sigue atendiendo a sus lectores sin pagar por toda su capacidad de colección sin usar.

#### Ejemplo 5: un museo con pocos visitantes

Un museo puede acoger a 2.800 visitantes, pero ahora solo recibe 150.

Como atiende a menos de 200 visitantes, el Ministerio mantiene un **margen de seguridad del 25%**.

```math
R = \lceil 150 \times 1.25 \rceil = 188
```

Con el 7% de financiación, el museo tiene capacidad para aproximadamente 196 visitantes.

**El Ministerio elige el 7% de financiación.**

El museo sigue en funcionamiento, con capacidad libre suficiente para las fluctuaciones de la asistencia.

#### Ejemplo 6: un aumento repentino entre revisiones mensuales

Pensemos en la escuela de primaria del ejemplo 1.

Ahora tiene:

- **Financiación:** 54%.
- **Capacidad:** 647 alumnos.
- **Asistencia:** 610 alumnos.

A mitad de mes, la asistencia sube de repente a **700 alumnos**.

La escuela está ahora gravemente saturada.

El Ministerio busca problemas urgentes cada dos días simulados. Como la capacidad disponible está más de un 3% por debajo de la demanda, no espera a la siguiente revisión mensual.

La nueva capacidad requerida es:

```math
R = \lceil 700 \times 1.05 \rceil = 735
```

**La financiación sube del 54% al 62%**, lo que ofrece aproximadamente 744 plazas.

Si después la asistencia baja, el Ministerio espera a la siguiente revisión mensual antes de reducir el presupuesto.

Es intencionado: **las subidas de financiación protegen los servicios de inmediato, mientras que las reducciones se tratan con más cautela**.

#### Ejemplo 7: la financiación máxima no siempre significa una carencia

Una clínica tiene una capacidad normal de 1.400 pacientes.

Con el 120% de financiación, puede acoger aproximadamente a **1.539 pacientes**.

Veamos dos situaciones:

| Pacientes actuales | Capacidad máxima | Cobertura | Decisión del Ministerio |
|---|---:|---:|---|
| 1.500 | 1.539 | 100% | Mantener la financiación máxima; sin aviso de carencia |
| 1.900 | 1.539 | 81% | Confirmar una carencia de capacidad |

Con 1.500 pacientes, la clínica tiene capacidad suficiente para atender a todos, aunque no puede mantener el margen de seguridad preferido.

**Todavía no hace falta ningún edificio nuevo.**

Con 1.900 pacientes, ni siquiera la financiación máxima puede ofrecer una cobertura adecuada.

El Ministerio mantiene la financiación al 120%. Si la carencia sigue confirmada durante 30 días simulados y la clínica no está en huelga, un aviso de un asesor te dice que hace falta otra clínica.

También hay una contrapartida económica: el 120% de financiación cuesta un **20% más**, pero ofrece solo un **10% más de capacidad** que el 100%.

#### Ejemplo 8: un hospital que necesita capacidad adicional

Un hospital tiene:

- **Capacidad normal:** 3.000 pacientes.
- **Demanda actual:** 4.376 pacientes.
- **Financiación máxima:** 120%.

Incluso con la financiación máxima, el hospital solo puede acoger a unos **3.299 pacientes**.

Su cobertura máxima es:

```math
V_{\max} = \frac{3299}{4376} \approx 0.754
```

Eso significa que el hospital solo puede atender aproximadamente el **75,4% de su demanda**.

Esto está por debajo tanto del umbral de carencia del 85% del Ministerio como del umbral de riesgo de huelga del 80% del juego.

**Subir la financiación no puede resolver el problema.**

El Ministerio mantiene la financiación al 120%, pero el hospital sigue en riesgo de huelga.

Si la carencia continúa durante 30 días simulados, el Ministerio puede emitir un aviso pidiéndote que construyas otro hospital. Una huelga en curso la gestionan, en cambio, las propias alertas de huelga del juego.

<a id="education-healthcare-limits"></a>
### 7. Limitaciones conocidas

El Ministerio automatiza las decisiones de financiación, pero no puede controlar todos los aspectos de SimCity 4.

**Capacidad y cobertura de los edificios**

- **No puede construir instalaciones nuevas.** Si una escuela o un hospital está saturado incluso con la financiación máxima, tienes que construir otro edificio.
- **No puede cambiar las áreas de cobertura.** La financiación de los autobuses escolares y de las ambulancias, y por tanto las decisiones de cobertura asociadas, siguen bajo tu control.
- **Usa la asistencia que informa cada edificio.** A veces el juego puede calcular el riesgo de huelga con la demanda de toda el área de cobertura, que puede ser mayor que el número de usuarios que muestra la ventana de consulta del edificio. El Ministerio usa la información de cobertura del juego para evitar recortes peligrosos, pero puede no detectar de inmediato todos los aumentos ocultos de la demanda.

**Supervisión y plazos**

- **El historial de asistencia se conserva durante la sesión de la ciudad actual.** Al cerrar la ciudad, el Ministerio olvida el historial mensual de asistencia y los ajustes de capacidad que había aprendido. Por eso los institutos y las facultades vuelven a recibir su protección estacional inicial tras reabrir la ciudad.
- **La información de cobertura del juego puede llegar con retraso.** Algunas cifras de cobertura se actualizan solo una vez al mes simulado aproximadamente. El Ministerio usa la asistencia y la capacidad actuales para planificar la financiación y, además, comprueba la información de cobertura del juego antes de permitir reducciones.
- **Las comprobaciones urgentes no son continuas.** Normalmente, el Ministerio busca problemas urgentes cada dos días simulados, o a diario durante las huelgas. Un aumento repentino de la demanda aún puede causar una huelga antes de la siguiente comprobación.
- **Los ahorros se revisan cada mes.** Tras un aumento temporal de la asistencia, la financiación se mantiene en el nivel más alto hasta la siguiente revisión mensual.
- **Pueden producirse pequeños cambios de financiación entre meses.** Si la asistencia fluctúa cerca de un umbral de financiación, el Ministerio puede subir o bajar el presupuesto un punto porcentual en revisiones mensuales sucesivas.

**Compatibilidad de los edificios**

El Ministerio gestiona los edificios que usan mecanismos de financiación reconocibles y coherentes.

No gestiona automáticamente:

- Escuelas privadas sin un presupuesto de funcionamiento propio que se pueda ajustar.
- Edificios personalizados que usan ventanas de consulta o sistemas de financiación no compatibles.
- Edificios cuyas lecturas de financiación y de presupuesto no coinciden.
- Edificios cuya capacidad no se puede predecir de forma fiable a partir de su financiación.

Los edificios que no se pueden gestionar con seguridad se dejan en paz.

**Avisos de los asesores**

- Los avisos de carencia cubren ocho tipos de edificios de educación y sanidad.
- Cada aviso puede enumerar hasta **10 edificios afectados**.
- Los edificios que están en huelga en ese momento los gestionan las alertas de huelga originales del juego, no los avisos de carencia del Ministerio.
- El Ministerio recuerda los avisos de cada ciudad identificable. Si una ciudad no se puede identificar de forma fiable, el mismo aviso puede volver a aparecer tras reabrirla.

**Consejos originales del juego**

El plugin incluye un archivo de datos que suprime ciertos avisos de financiación engañosos del juego original.

Este ajuste es independiente de los interruptores de gestión de la educación y de la sanidad. Mientras el archivo esté instalado, esos avisos originales siguen suprimidos incluso si desactivas la gestión automática del Ministerio.

Consulta las [condiciones de seguridad](#education-healthcare-safety) para más detalles.

[Volver al contenido](index.md#contents) | [Volver al README](../../README.es.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
