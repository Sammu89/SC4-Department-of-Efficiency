[English](../en/index.md) | **Português** | [Français](../fr/index.md) | [Español](../es/index.md)

[Voltar ao README](../../README.pt.md)


# Ministério da Optimização: Referência Técnica

Este guia explica como funciona o SimCity 4 nos bastidores e como o Ministério da Optimização toma as suas decisões, serviço a serviço. Destina-se aos jogadores que querem perceber a lógica por trás das ações do Ministério, bem como aos criadores de mods interessados na mecânica subjacente do jogo.

## Referência rápida

| Serviço | Reserva ou definição principal | Predefinição | Proteção principal | Capítulo |
|---|---|---|---|---|
| Educação | Margem de segurança da capacidade | 5% | Mantém capacidade de reserva para acomodar as oscilações na frequência dos alunos. O financiamento dos autocarros escolares não é alterado. | [Educação](education_healthcare.md#education-healthcare) |
| Saúde | Margem de segurança da capacidade | 5% | Mantém capacidade de reserva para acomodar as oscilações no número de doentes. O financiamento das ambulâncias não é alterado. | [Saúde](education_healthcare.md#education-healthcare) |
| Eletricidade | Reserva de eletricidade | 20% | Mantém capacidade de produção de reserva para acomodar o crescimento da cidade e reduzir o risco de apagões. As linhas elétricas também ficam protegidas contra a deterioração. | [Eletricidade](power.md#power) |
| Água | Reserva de água | 10% | Mantém capacidade de produção de água de reserva, com a proteção das canalizações ativada por predefinição. | [Água](water.md#water) |
| Saneamento | Reserva de lixo | 2% | Garante capacidade suficiente de tratamento e eliminação de resíduos, com o objetivo de limpar o lixo acumulado até ao fim do mês simulado. | [Lixo e aterros](garbage.md#garbage-landfills) |
| Aviso do aterro | Limiar de poupança dos avisos | Aviso desligado; limiar de 25% | Recomenda retirar quadrados vazios do aterro quando estas podem baixar de forma significativa os custos do saneamento. | [Aviso sobre o tamanho do aterro](garbage.md#garbage-landfills-advice) |
| Acordos com os vizinhos | Gestão por recurso | Ligada; intermediação entre vizinhos desligada | Gere os contratos de eletricidade, água e lixo, dando prioridade às necessidades da própria cidade e às reservas de segurança. | [Acordos com os vizinhos](deals.md#neighbor-deals) |
| Demolição | Tempos de espera | Ruínas: 1 mês; edifícios abandonados: 12 meses | Protege por predefinição os edifícios municipais e históricos, com um limite mensal opcional para a despesa em demolições. | [Demolição automática](demolition.md#demolition) |

<a id="contents"></a>

## Índice

* [A. Educação e saúde](education_healthcare.md#education-healthcare)
* [B. Eletricidade](power.md#power)
* [C. Água](water.md#water)
* [D. Lixo e aterros](garbage.md#garbage-landfills)
  * [Aviso sobre o tamanho do aterro](garbage.md#garbage-landfills-advice)
  * [Quadrados fantasma](garbage.md#ghost-tiles)
* [E. Acordos com os vizinhos](deals.md#neighbor-deals)
* [F. Demolição automática](demolition.md#demolition)
* [G. Segurança, automatização e tomada de decisões](safety.md#safety)
  * [Avisos dos conselheiros](safety.md#notices)
* [H. Referência das opções](options.md#options)

* [I. Referência dos endereços do executável](../en/game_addresses.md) (em inglês)
* [J. Phantom Funding Slider Bug](phantom_slider.md) (deriva «fantasma» dos sliders)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
