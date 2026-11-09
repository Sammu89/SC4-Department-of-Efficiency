[English](../en/phantom_slider.md) | **Português** | [Français](../fr/phantom_slider.md) | [Español](../es/phantom_slider.md)

[Voltar à referência técnica](index.md) · [Voltar ao README](../../README.pt.md)

# Phantom Funding Slider Bug (deriva «fantasma» dos sliders de financiamento)

Esta página explica um problema antigo do SimCity 4, como foi reproduzido para este projeto, o que o Ministério da Optimização faz quando o encontra e, igualmente importante, o que **não** faz.

## 1. O que é o bug

Depois de o **custo de manutenção** de um tipo de edifício ter sido alterado (por exemplo por um mod que edita o exemplar do edifício), o SimCity 4 pode começar a mover os **sliders de financiamento** dos edifícios desse tipo que **já estavam colocados** numa cidade guardada. O movimento não é ruído aleatório: pode repetir-se nos recálculos mensais, prolongar-se por vários meses e empurrar o slider sempre no mesmo sentido, até algo o travar.

A comunidade conhece-o como **Phantom Funding Slider Bug** («slider fantasma»). Foi descrito e reproduzido por vários membros da comunidade do SimCity 4, nomeadamente CorinaMarie, Kel9509 e Null 45 (ver [Créditos e referências](#7-créditos-e-referências)).

**Um movimento inesperado de um slider não prova este bug.** Uma alteração do próprio jogador, um recálculo legítimo, outro mod ou outro problema do jogo podem ter o mesmo aspeto. O Ministério nunca conclui que «o bug» está presente.

## 2. Como foi reproduzido

O Ministério foi testado com o material de reprodução partilhado por CorinaMarie: pequenas cidades descartáveis e ficheiros DAT que alteram o custo de manutenção de uma clínica. A sequência seguinte reproduz o problema:

1. Um slider de financiamento de um edifício é **definido e confirmado** (nos testes, por uma escrita verificada do próprio Ministério; a comunidade também o relata com uma alteração manual do slider).
2. A cidade é guardada e o jogo fechado.
3. Instala-se um DAT que altera o **custo de manutenção desse tipo de edifício**.
4. A cidade é aberta de novo e simulada mês a mês.

Nas cidades usadas, reduzir o custo da linha de pessoal do edifício de **300 para 200** fez o slider descer mês após mês (7% → 4,5% → 2,5% → 1,5%), o que reduziu a capacidade do edifício até ele entrar em greve. Aumentar o custo de **300 para 450** fez o slider subir mês após mês (7% → 10% → 15% → 33% → 49% → 109% → 120%, o limite máximo). Sem um slider confirmado, a mesma alteração de custo não moveu nada.

Isto aconteceu **sem a gestão automática do Ministério** (o Ministério apenas observou): é um comportamento do jogo, não um defeito introduzido pelo plugin.

**O que foi medido (não é uma explicação do funcionamento interno do jogo).** Depois da alteração de custo, o custo da linha do edifício e o seu «financiamento pleno» guardado acompanharam o novo custo, mas o valor que o próprio departamento orçamental guarda para o seu financiamento mensal ideal manteve o total antigo. Em cada recálculo mensal a percentagem foi multiplicada pela razão entre os dois custos, vezes seguidas, até parar: ao fim de três recálculos quando o custo baixou, no limite máximo quando subiu. Não foi estabelecido porque é que o jogo mantém o valor antigo.

## 3. O que faz o Ministério

A proteção usa mecanismos que o Ministério já tinha. Não acrescenta nenhuma opção ao ficheiro INI, nenhum aviso para o jogador, nenhuma DLL externa nem um novo calendário global.

1. **Regista cada alteração de financiamento que faz e verificou**: depois de cada escrita, relê o valor e memoriza-o para esse edifício.
2. **Vigia os edifícios que gere.** A vigilância cívica já observa escolas e hospitais de dois em dois dias simulados.
3. **Reconhece uma alteração suspeita**: o financiamento de um edifício deixou de ser o que o Ministério verificou por último, e a alteração aparece nos primeiros dias **depois de um recálculo mensal**. Nas cidades usadas, todas as alterações feitas pelo jogo por conta própria apareceram a menos de um dia do recálculo mensal e nenhuma a meio do mês.
4. **Reavalia esse edifício** com o mesmo cálculo de sempre, a partir da utilização e da capacidade reais.
5. **Corrige o financiamento antecipadamente, nos dois sentidos**: sobe o que o jogo cortou em demasia e baixa o que o jogo empurrou para cima.
6. **Aplica o financiamento mínimo que ainda dá a capacidade e a reserva exigidas pela política**, e não simplesmente o valor antigo.
7. **Confirma o resultado**: aplicam-se as verificações de segurança habituais, pelo que o valor é relido depois da alteração e comparado com o pretendido.
8. **Continua com os outros edifícios** e com a sua revisão mensal normal.

Regras que mantêm isto seguro:

- Uma alteração **a meio do mês**, ou num edifício onde o Ministério nunca escreveu, **não** é tratada como este problema. Espera pela revisão mensal normal, como antes. Uma alteração manual do jogador não é, portanto, «corrigida antecipadamente», exceto nos poucos dias a seguir a um recálculo mensal, em que não se distingue da alteração do próprio jogo (a revisão mensal tê-la-ia substituído de qualquer modo).
- Uma **redução** antecipada só é permitida num edifício cuja alteração foi reconhecida desta forma. As reduções normais causadas por uma procura em queda continuam a esperar pela revisão mensal.
- **No máximo uma correção antecipada por edifício e por recálculo mensal.** O Ministério nunca entra num ciclo de luta contra o jogo.
- Um edifício **não** é posto fora de serviço por causa de uma deriva. Se uma escrita não puder ser verificada, aplicam-se as regras de segurança existentes: esse edifício é deixado em paz durante a sessão e, ao fim de três edifícios diferentes, o escritor cívico pára.

Nos testes, o maior intervalo entre uma alteração feita pelo jogo e a sua correção **passou de cerca de 30 dias simulados para um dia simulado**.

## 4. O que a proteção não faz

- **Não** corrige os dados internos do jogo que estão na origem do problema.
- **Não** impede que o jogo volte a mover os sliders no recálculo seguinte.
- **Não** repara necessariamente custos internos desatualizados.
- **Não** remove entradas orçamentais fantasma.
- **Não** garante que o dinheiro que o jogo realmente cobra corresponda ao custo que o Ministério calcula.
- **Não** protege edifícios que o Ministério não consegue gerir (por exemplo esquadras de bombeiros ou da polícia, ou edifícios sem linha de financiamento padrão).
- **Não** garante que um edifício nunca tenha uma greve ou um défice de capacidade **entre** duas observações.

O que ficou demonstrado é mais estreito: **o Ministério continua a otimizar a capacidade e o financiamento dos edifícios elegíveis que gere automaticamente, em vez de os sliders ficarem onde o jogo os deixou.** O bug do SimCity 4 não é corrigido, mas os seus efeitos observados nos edifícios geridos são substancialmente mitigados.

## 5. Limites e segurança

A deteção baseia-se em sinais observáveis, sobretudo uma alteração inesperada próxima do recálculo mensal. Não consegue identificar com certeza a causa de cada alteração, nem tenta. As verificações que se aplicam a toda a escrita (compare-and-set antes de escrever, leitura de confirmação depois, exclusão de um edifício cuja escrita não se consegue verificar, paragem do escritor após três edifícios diferentes) aplicam-se também aqui, e o Ministério recusa agir com leituras em que não pode confiar.

## 6. Evidência

Quatro execuções dos cenários acima (duas com o custo a descer, duas com o custo a subir, 10 meses simulados cada), com o Ministério a gerir a clínica automaticamente, comparadas com quatro execuções anteriores dos mesmos cenários feitas antes desta proteção:

| | Antes | Depois |
|---|---|---|
| Maior atraso entre uma alteração do jogo e a sua correção | até 30 dias simulados | 1 dia simulado |
| Alterações do jogo / correções verificadas (por execução) | 4 a 10 alterações, 3 a 8 correções | 10 e 10 em todas as execuções: uma correção verificada por alteração, nenhuma sem alteração |
| Financiamento e capacidade finais | 7% e 35 | 7% e 35 (capacidade exigida: 32 a 33) |
| Falhas de escrita, edifícios postos fora de serviço, greves | nenhuma | nenhuma |
| Dias seguidos abaixo da capacidade exigida (ou com financiamento acima do mínimo) | até 31 | no máximo 2 |

Uma execução de controlo de 8 meses simulados **sem qualquer alteração de custo** **não** fez nenhuma escrita adicional. O tempo total do Ministério foi de cerca de 2 ms por mês antes e depois, abaixo da resolução de 1 ms do seu próprio registo.

São **resultados observados em poucas cidades de teste descartáveis**, não uma garantia para todas as cidades.

## 7. Créditos e referências

- **CorinaMarie** explicou o fenómeno e partilhou as cidades e os ficheiros DAT de reprodução que tornaram este trabalho possível. O seu tutorial sobre como atualizar o custo mensal de um lote evitando problemas de orçamento fantasma descreve como evitar o problema ao **criar** mods (dando ao edifício alterado uma nova identidade de exemplar), o que é prevenção e não reparação de uma cidade já afetada: [tutorial no Simtropolis](https://community.simtropolis.com/forums/topic/761229-tutorial-updating-lot-monthly-cost-while-avoiding-phantom-budget-predicaments/).
- **Kel9509** descreveu uma forma simples de provocar o problema (alterar a capacidade de um edifício com o [SC4PIM-X](https://community.simtropolis.com/files/file/37485-sc4pim-x-2026/), guardar, reabrir, mover o slider). Esse método não foi usado nos testes aqui descritos.
- **Null 45 (Nicholas Hayes)** escreveu o [Remove Phantom Budget Items](https://community.simtropolis.com/files/file/36437-remove-phantom-budget-items-dll-plugin/) ([código](https://github.com/0xC0000054/sc4-remove-phantom-budget-items)), um comando de consola que remove **entradas orçamentais órfãs** (itens de edifícios cujo exemplar já não existe). Não lê nem altera os sliders de financiamento e **não** corrige o comportamento descrito aqui. Não faz parte do Ministério nem é necessário para ele.
- A discussão decorreu nos [comentários da página do Ministério no Simtropolis](https://community.simtropolis.com/files/file/37700-department-of-efficiency-automatic-city-management-dll/?tab=comments).

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
