[English](../en/index.md) | [Português](../pt/index.md) | [Français](../fr/index.md) | **Español**

[Volver al README](../../README.es.md)


# Ministerio de Optimización: referencia técnica

Esta guía explica cómo funciona SimCity 4 entre bambalinas y cómo toma el Ministerio de Optimización sus decisiones, servicio por servicio. Está pensada para los jugadores que quieren entender el razonamiento detrás de las acciones del Ministerio, y también para los modders interesados en la mecánica interna del juego.

## Referencia rápida

| Servicio | Reserva o ajuste principal | Valor por defecto | Protección principal | Capítulo |
|---|---|---|---|---|
| Educación | Margen de seguridad de capacidad | 5% | Mantiene capacidad libre para absorber las fluctuaciones en la asistencia de alumnos. La financiación de los autobuses escolares no se modifica. | [Educación](education_healthcare.md#education-healthcare) |
| Sanidad | Margen de seguridad de capacidad | 5% | Mantiene capacidad libre para absorber las fluctuaciones en el número de pacientes. La financiación de las ambulancias no se modifica. | [Sanidad](education_healthcare.md#education-healthcare) |
| Electricidad | Reserva de electricidad | 20% | Mantiene capacidad de generación libre para absorber el crecimiento de la ciudad y reducir el riesgo de apagones. Las líneas eléctricas también están protegidas contra el deterioro. | [Electricidad](power.md#power) |
| Agua | Reserva de agua | 10% | Mantiene capacidad libre de producción de agua, con la protección de las tuberías activada por defecto. | [Agua](water.md#water) |
| Limpieza | Reserva de basura | 2% | Garantiza capacidad suficiente de tratamiento y eliminación de residuos, con el objetivo de despejar la basura acumulada antes de que acabe el mes simulado. | [Basura y vertederos](garbage.md#garbage-landfills) |
| Consejo sobre el vertedero | Umbral de aviso de ahorro | Consejo desactivado; umbral del 25% | Recomienda quitar casillas vacías del vertedero cuando podrían rebajar de forma notable los costes de limpieza. | [Consejo sobre el tamaño del vertedero](garbage.md#garbage-landfills-advice) |
| Acuerdos con vecinos | Gestión por recurso | Activada; comprar a una ciudad para vender a otra desactivado | Gestiona los contratos de electricidad, agua y basura dando prioridad a las necesidades propias de la ciudad y a sus reservas de seguridad. | [Acuerdos con vecinos](deals.md#neighbor-deals) |
| Demolición | Tiempos de espera | Ruinas: 1 mes; edificios abandonados: 12 meses | Protege por defecto los edificios municipales e históricos, con un límite opcional de gasto mensual en demolición. | [Demolición automática](demolition.md#demolition) |

<a id="contents"></a>

## Contenido

* [A. Educación y sanidad](education_healthcare.md#education-healthcare)
* [B. Electricidad](power.md#power)
* [C. Agua](water.md#water)
* [D. Basura y vertederos](garbage.md#garbage-landfills)
  * [Consejo sobre el tamaño del vertedero](garbage.md#garbage-landfills-advice)
  * [Casillas fantasma](garbage.md#ghost-tiles)
* [E. Acuerdos con vecinos](deals.md#neighbor-deals)
* [F. Demolición automática](demolition.md#demolition)
* [G. Seguridad, automatización y toma de decisiones](safety.md#safety)
  * [Avisos de los asesores](safety.md#notices)
* [H. Referencia de opciones](options.md#options)

* [I. Referencia de direcciones del ejecutable](../en/game_addresses.md) (en inglés)
* [J. Phantom Funding Slider Bug](phantom_slider.md) (deriva «fantasma» de los controles)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
