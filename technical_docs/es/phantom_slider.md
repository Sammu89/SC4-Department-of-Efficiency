[English](../en/phantom_slider.md) | [Português](../pt/phantom_slider.md) | [Français](../fr/phantom_slider.md) | **Español**

[Volver a la referencia técnica](index.md) · [Volver al README](../../README.es.md)

# Phantom Funding Slider Bug (deriva «fantasma» de los controles de financiación)

Esta página explica un problema antiguo de SimCity 4, cómo se reprodujo para este proyecto, qué hace el Ministerio de Optimización cuando lo encuentra y, igual de importante, qué **no** hace.

## 1. Qué es el bug

Después de que se haya modificado el **coste de mantenimiento** de un tipo de edificio (por ejemplo, con un mod que edita el exemplar del edificio), SimCity 4 puede empezar a mover los **controles de financiación** de los edificios de ese tipo que **ya estaban colocados** en una ciudad guardada. El movimiento no es ruido aleatorio: puede repetirse en los recálculos mensuales, prolongarse varios meses y empujar el control siempre en el mismo sentido hasta que algo lo detiene.

La comunidad lo conoce como **Phantom Funding Slider Bug** («control fantasma»). Lo describieron y reprodujeron varios miembros de la comunidad de SimCity 4, en particular CorinaMarie, Kel9509 y Null 45 (véase [Créditos y referencias](#7-créditos-y-referencias)).

**Un movimiento inesperado de un control no demuestra este bug.** Un cambio del propio jugador, un recálculo legítimo, otro mod u otro problema del juego pueden tener el mismo aspecto. El Ministerio nunca concluye que «el bug» esté presente.

## 2. Cómo se reprodujo

El Ministerio se probó con el material de reproducción compartido por CorinaMarie: pequeñas ciudades desechables y archivos DAT que cambian el coste de mantenimiento de una clínica. La secuencia siguiente reproduce el problema:

1. Un control de financiación de un edificio se **fija y se confirma** (en las pruebas, mediante una escritura verificada del propio Ministerio; la comunidad también lo comunica con un cambio manual del control).
2. Se guarda la ciudad y se cierra el juego.
3. Se instala un DAT que cambia el **coste de mantenimiento de ese tipo de edificio**.
4. Se vuelve a abrir la ciudad y se simula mes a mes.

En las ciudades usadas, bajar el coste de la línea de personal del edificio de **300 a 200** hizo que el control bajase mes tras mes (7 % → 4,5 % → 2,5 % → 1,5 %), lo que redujo la capacidad del edificio hasta que entró en huelga. Subir el coste de **300 a 450** hizo que el control subiese mes tras mes (7 % → 10 % → 15 % → 33 % → 49 % → 109 % → 120 %, el límite superior). Sin un control confirmado, el mismo cambio de coste no movió nada.

Esto ocurrió **sin la gestión automática del Ministerio** (el Ministerio solo observaba): es un comportamiento del juego, no un defecto introducido por el plugin.

**Lo que se midió (no es una explicación del funcionamiento interno del juego).** Tras el cambio de coste, el coste de la línea del edificio y su «financiación plena» almacenada siguieron el nuevo coste, pero la cifra que el propio departamento presupuestario almacena como su financiación mensual ideal conservó el total antiguo. En cada recálculo mensual el porcentaje se multiplicó por la razón entre los dos costes, una y otra vez, hasta detenerse: tras tres recálculos cuando el coste bajó, en el límite superior cuando subió. No se ha establecido por qué el juego conserva el valor antiguo.

## 3. Qué hace el Ministerio

La protección usa mecanismos que el Ministerio ya tenía. No añade ningún ajuste al archivo INI, ningún aviso para el jugador, ninguna DLL externa ni un nuevo calendario global.

1. **Registra cada cambio de financiación que hace y ha verificado**: tras cada escritura, vuelve a leer el valor y lo recuerda para ese edificio.
2. **Vigila los edificios que gestiona.** La vigilancia cívica ya observa escuelas y hospitales cada dos días simulados.
3. **Reconoce un cambio sospechoso**: la financiación de un edificio ya no es lo último que el Ministerio verificó, y el cambio aparece en los primeros días **después de un recálculo mensual**. En las ciudades usadas, todos los cambios que el juego hizo por su cuenta aparecieron a menos de un día del recálculo mensual y ninguno a mitad de mes.
4. **Reevalúa ese edificio** con el mismo cálculo de siempre, a partir del uso y de la capacidad reales.
5. **Corrige la financiación antes de tiempo, en ambos sentidos**: sube lo que el juego recortó demasiado y baja lo que el juego empujó demasiado arriba.
6. **Aplica la financiación mínima que aún da la capacidad y la reserva que exige la política**, y no simplemente el valor antiguo.
7. **Comprueba el resultado**: se aplican las comprobaciones de seguridad habituales, de modo que el valor se vuelve a leer tras el cambio y se compara con el previsto.
8. **Sigue con los demás edificios** y con su revisión mensual normal.

Reglas que mantienen esto seguro:

- Un cambio **a mitad de mes**, o en un edificio en el que el Ministerio nunca ha escrito, **no** se trata como este problema. Espera a la revisión mensual normal, como antes. Un cambio manual del jugador no se «corrige antes de tiempo», salvo en los pocos días que siguen a un recálculo mensual, en los que no se distingue de un cambio del propio juego (la revisión mensual lo habría sustituido de todos modos).
- Una **reducción** anticipada solo se permite en un edificio cuyo cambio se reconoció de esta forma. Las reducciones normales debidas a una demanda en descenso siguen esperando a la revisión mensual.
- **Como máximo una corrección anticipada por edificio y por recálculo mensual.** El Ministerio nunca entra en un bucle de lucha contra el juego.
- Un edificio **no** se deja fuera de servicio por una deriva. Si una escritura no puede verificarse, se aplican las reglas de seguridad existentes: ese edificio se deja en paz durante la sesión y, tras tres edificios distintos, el escritor cívico se detiene.

En las pruebas, el mayor tiempo entre un cambio hecho por el juego y su corrección **pasó de unos 30 días simulados a un día simulado**.

## 4. Lo que la protección no hace

- **No** corrige los datos internos del juego que originan el problema.
- **No** impide que el juego vuelva a mover los controles en el siguiente recálculo.
- **No** repara necesariamente costes internos desactualizados.
- **No** elimina entradas presupuestarias fantasma.
- **No** garantiza que el dinero que el juego cobra realmente coincida con el coste que calcula el Ministerio.
- **No** protege edificios que el Ministerio no puede gestionar (por ejemplo parques de bomberos o comisarías, o edificios sin línea de financiación estándar).
- **No** garantiza que un edificio nunca tenga una huelga o un déficit de capacidad **entre** dos observaciones.

Lo que se ha demostrado es más acotado: **el Ministerio sigue optimizando la capacidad y la financiación de los edificios elegibles que gestiona automáticamente, en lugar de que los controles se queden donde el juego los dejó.** El bug de SimCity 4 no se corrige, pero sus efectos observados en los edificios gestionados se mitigan de forma sustancial.

## 5. Límites y seguridad

La detección se basa en señales observables, sobre todo un cambio inesperado cercano al recálculo mensual. No puede identificar con certeza la causa de cada cambio, ni lo intenta. Las comprobaciones que se aplican a toda escritura (compare-and-set antes de escribir, lectura de confirmación después, exclusión de un edificio cuya escritura no puede verificarse, parada del escritor tras tres edificios distintos) se aplican también aquí, y el Ministerio se niega a actuar con lecturas en las que no puede confiar.

## 6. Evidencia

Cuatro ejecuciones de los escenarios anteriores (dos con el coste a la baja, dos con el coste al alza, 10 meses simulados cada una), con el Ministerio gestionando la clínica automáticamente, comparadas con cuatro ejecuciones anteriores de los mismos escenarios hechas antes de esta protección:

| | Antes | Después |
|---|---|---|
| Mayor retraso entre un cambio del juego y su corrección | hasta 30 días simulados | 1 día simulado |
| Cambios del juego / correcciones verificadas (por ejecución) | de 4 a 10 cambios, de 3 a 8 correcciones | 10 y 10 en todas las ejecuciones: una corrección verificada por cambio, ninguna sin cambio |
| Financiación y capacidad finales | 7 % y 35 | 7 % y 35 (capacidad exigida: 32 a 33) |
| Fallos de escritura, edificios dejados fuera de servicio, huelgas | ninguno | ninguno |
| Días seguidos por debajo de la capacidad exigida (o con financiación por encima del mínimo) | hasta 31 | 2 como máximo |

Una ejecución de control de 8 meses simulados **sin ningún cambio de coste** **no** hizo ninguna escritura adicional. El tiempo total del Ministerio fue de unos 2 ms por mes antes y después, por debajo de la resolución de 1 ms de su propio registro.

Son **resultados observados en pocas ciudades de prueba desechables**, no una garantía para todas las ciudades.

## 7. Créditos y referencias

- **CorinaMarie** explicó el fenómeno y compartió las ciudades y los archivos DAT de reproducción que hicieron posible este trabajo. Su tutorial sobre cómo actualizar el coste mensual de un lote evitando problemas de presupuesto fantasma describe cómo evitar el problema al **crear** mods (dando al edificio modificado una nueva identidad de exemplar), que es prevención y no la reparación de una ciudad ya afectada: [tutorial en Simtropolis](https://community.simtropolis.com/forums/topic/761229-tutorial-updating-lot-monthly-cost-while-avoiding-phantom-budget-predicaments/).
- **Kel9509** describió una forma sencilla de provocar el problema (cambiar la capacidad de un edificio con [SC4PIM-X](https://community.simtropolis.com/files/file/37485-sc4pim-x-2026/), guardar, reabrir, mover el control). Ese método no se usó en las pruebas descritas aquí.
- **Null 45 (Nicholas Hayes)** escribió [Remove Phantom Budget Items](https://community.simtropolis.com/files/file/36437-remove-phantom-budget-items-dll-plugin/) ([código fuente](https://github.com/0xC0000054/sc4-remove-phantom-budget-items)), un comando de consola que elimina **entradas presupuestarias huérfanas** (elementos de edificios cuyo exemplar ya no existe). No lee ni modifica los controles de financiación y **no** corrige el comportamiento descrito aquí. No forma parte del Ministerio ni este lo necesita.
- La discusión tuvo lugar en los [comentarios de la página del Ministerio en Simtropolis](https://community.simtropolis.com/files/file/37700-department-of-efficiency-automatic-city-management-dll/?tab=comments).

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
