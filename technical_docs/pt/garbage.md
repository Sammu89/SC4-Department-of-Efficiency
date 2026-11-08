[English](../en/garbage.md) | **Português** | [Français](../fr/garbage.md) | [Español](../es/garbage.md)

[Voltar ao índice](index.md#contents) | [Voltar ao README](../../README.pt.md)

<a id="garbage-landfills"></a>
## D. Lixo e aterros

O Ministério ajuda a gerir o lixo da cidade de forma eficiente, mantendo os custos de saneamento o mais baixos possível sem deixar que os resíduos se acumulem desnecessariamente.

Oferece quatro funções distintas:

1. **Financiamento automático do saneamento:** ajusta o orçamento do saneamento para tratar o lixo da cidade, incluindo a reserva de segurança escolhida.

2. **Contratos regionais de lixo:** gere as importações e exportações de lixo quando a gestão dos acordos com os vizinhos está ativada. Ver [Acordos com os vizinhos](deals.md#neighbor-deals).

3. **Aviso sobre o tamanho do aterro (opcional):** recomenda ampliar ou reduzir as zonas de aterro quando isso puder baixar de forma significativa as despesas mensais. **O Ministério nunca altera automaticamente a zona do aterro com este fim.**

4. **Reparação dos quadrados fantasma do aterro:** corrige automaticamente um erro do jogo em que alguns antigos quadrados de aterro continuam a ser contados e cobrados como aterro ativo. Usa o mecanismo de zonamento do próprio jogo e não tem uma opção separada.

Para perceber como funcionam estas funcionalidades, é importante distinguir **produção de lixo, capacidade de tratamento, entrada nos aterros e armazenamento a longo prazo**.

Todas as quantidades de lixo neste capítulo usam as unidades internas de lixo do SimCity 4. As despesas são expressas em simoleões (§) por mês simulado.

<a id="garbage-landfills-vanilla"></a>
### 1. Como funciona o lixo no SimCity 4

#### Como o lixo é produzido e reciclado

Todos os dias simulados, o SimCity 4 processa os edifícios da cidade e calcula quanto lixo geram.

O jogo regista esta quantidade como **lixo produzido**.

Os centros de reciclagem retiram uma parte desse lixo antes de ele entrar no sistema de eliminação.

A quantidade reciclada depende das instalações de reciclagem da cidade. **Alterar o financiamento do saneamento não altera a taxa de reciclagem.**

O lixo restante, juntamente com qualquer lixo já deixado por recolher, tem de ser tratado pelo sistema de eliminação da cidade.

#### Por que ordem o jogo elimina o lixo?

O SimCity 4 processa o lixo por uma ordem específica.

| Prioridade | Método de eliminação | O que acontece |
|---|---|---|
| **1** | Exportações de lixo | Os resíduos são enviados para cidades vizinhas através de contratos de exportação ativos. |
| **2** | Incineradoras e centrais de conversão de resíduos em energia | As instalações de tratamento processam o lixo, até à capacidade disponível. |
| **3** | Aterros | O lixo restante é enviado para os aterros, sujeito aos seus limites de entrada e de armazenamento. |
| **4** | Remoção oculta de lixo | O jogo remove discretamente uma pequena quantidade adicional de lixo. |

**O jogo tenta cada método por esta ordem antes de passar ao seguinte.**

Por exemplo, se a cidade exportar parte do lixo e tratar o resto em incineradoras, os aterros só precisam de aceitar o que sobrar depois disso.

**O que é a remoção oculta de lixo?**

O SimCity 4 tem um mecanismo integrado, por vezes chamado *garbage abduction* (rapto de lixo).

Pode remover discretamente até **2.000 unidades de lixo por mês simulado**, segundo as definições do jogo original.

Estes resíduos desaparecem sem serem contados nas estatísticas normais de eliminação de lixo.

O Ministério tem de ter este comportamento em conta ao interpretar os números de lixo do jogo.

**O que acontece ao lixo que não pode ser eliminado?**

Todo o lixo que sobra depois das quatro fases fica na grelha da cidade como **lixo por recolher**.

São os resíduos que se podem acumular nas ruas e nos edifícios quando o sistema de eliminação não consegue dar resposta às necessidades da cidade.

O jogo mantém um total, para toda a cidade, deste lixo por recolher, que o Ministério consegue ler.

#### Como funcionam os contratos de lixo com as cidades vizinhas

Os contratos de lixo funcionam de forma diferente dos de eletricidade e de água.

Existem dois tipos:

- **Importações de lixo:** uma cidade vizinha envia-lhe o seu lixo e paga-lhe para o eliminar.
- **Exportações de lixo:** a sua cidade paga a uma cidade vizinha para levar o seu lixo.

Embora estes contratos especifiquem uma quantidade mensal, **o lixo não é entregue de forma uniforme ao longo do mês**.

Em vez disso, o jogo move normalmente cerca de 10% da quantidade mensal do contrato em cada dia simulado, até a quota desse mês estar entregue.

**Entrega diária do contrato de lixo (jogo base)**

```math
s = \mathrm{round}(0.1 \times A)
\qquad
\text{enquanto } A-U>0
```

Depois de cada entrega diária:

```math
U \leftarrow U+s
```

Em que:

- **A** é a quantidade mensal de lixo do contrato.
- **U** é a quantidade já entregue durante o mês atual.
- **s** é a quantidade entregue nesse dia.

A quantidade entregue é reposta a zero no início de cada mês simulado.

**Exemplo: exportar 38.990 unidades por mês**

```math
s = 38990 \times 0.1 = 3899
```

O jogo exporta **3.899 unidades por dia**, completando normalmente a quantidade mensal em cerca de dez dias.

Isto significa que a maior parte das entregas dos contratos de lixo acontece **durante os primeiros dez dias, mais ou menos, de cada mês**, e não distribuída uniformemente pelos 30 ou 31 dias.

Há também um pequeno pormenor de arredondamento: a última entrega diária não está limitada à quantidade exata que falta. Alguns contratos podem, por isso, entregar um pouco mais do que a sua quantidade mensal nominal.

**Porque é que isto importa?**

Uma cidade pode receber uma grande quantidade de lixo importado na primeira parte de um mês.

O sistema de saneamento tem de conseguir lidar com essa concentração temporária, mesmo que a quantidade mensal total pareça razoável.

**E as estatísticas de lixo importado do jogo?**

O contador *Garbage imported last month* (lixo importado no mês passado) do jogo regista o que foi realmente entregue **durante o mês anterior**.

Mantém-se inalterado durante todo o mês atual.

Não deve ser confundido com a quantidade que está a ser entregue neste momento pelos contratos ativos.

#### Como o financiamento do saneamento afeta o tratamento do lixo

Ao contrário da eletricidade, o saneamento tem **uma única percentagem de financiamento para todo o departamento**.

Esta percentagem afeta a capacidade das instalações de tratamento de lixo e a quantidade de lixo que os aterros podem aceitar.

O jogo original permite normalmente um financiamento de **0% a 120%**, a menos que o departamento tenha um máximo diferente ou um financiamento fixo.

No entanto, **a capacidade de tratamento de lixo não aumenta em proporção direta com o financiamento**.

O SimCity 4 usa a mesma curva de financiamento-eficiência que as suas centrais elétricas.

**Eficiência do saneamento (jogo base)**

```math
e = \frac{f(p)}{100}
```

Em que:

- **p** é a percentagem de financiamento do saneamento.
- **f(p)** é a eficiência dada pela curva do jogo.
- **e** é a eficiência resultante, expressa como fração.

A curva original tem estes pontos de controlo:

| Financiamento do saneamento | Eficiência de tratamento |
|---:|---:|
| 0% | 0% |
| 50% | 25% |
| 90% | 80% |
| 100% | 100% |
| 120% | 110% |

Os valores intermédios calculam-se por interpolação linear, como explicado nos capítulos anteriores.

**Exemplos de eficiência do saneamento**

| Financiamento | Eficiência de tratamento |
|---:|---:|
| 50% | 25% |
| 60% | 38,75% |
| 61% | 40,125% |
| 75% | 59,375% |
| 90% | 80% |
| 100% | 100% |
| 110% | 105% |
| 120% | 110% |

**O que significa isto para a cidade?**

Com **50% de financiamento**, o departamento de saneamento oferece apenas **25% da sua capacidade normal de tratamento**.

Com **90% de financiamento**, oferece 80%.

Com **120% de financiamento**, oferece 110%, apesar de custar mais 20% do que o normal.

A relação é especialmente importante entre 50% e 90%, onde cada ponto adicional de financiamento aumenta a eficiência em 1,375 pontos percentuais.

**O que afeta a eficiência?**

A eficiência do saneamento altera:

- A capacidade de tratamento das incineradoras.
- A capacidade de processamento de lixo das centrais de conversão de resíduos em energia.
- A quantidade de lixo que os aterros podem aceitar durante um mês.

**Não** altera:

- A quantidade entregue pelos contratos regionais de lixo.
- A quantidade retirada pelos centros de reciclagem.

**A capacidade muda imediatamente depois de o financiamento ser ajustado?**

Não completamente.

A percentagem de financiamento muda de imediato, mas a eficiência do saneamento calculada pelo jogo e as capacidades de tratamento atualizam-se ao longo dos dias simulados seguintes.

O Ministério tem, por isso, de ter estes atrasos em conta ao avaliar se uma alteração de financiamento produziu efeito.

#### Incineradoras e centrais de conversão de resíduos em energia

O SimCity 4 calcula a capacidade de tratamento disponível das incineradoras e das centrais de conversão de resíduos em energia todos os dias simulados.

Para cada instalação, esta capacidade já tem em conta:

- A capacidade máxima de tratamento do edifício.
- A eficiência atual do saneamento.
- O estado do próprio edifício e os modificadores de capacidade.

O jogo fornece depois a capacidade de tratamento combinada de cada tipo de instalação.

**O Ministério lê estes valores diretamente.**

Não os volta a multiplicar pela eficiência do saneamento, porque o jogo já aplicou esse ajuste.

Assim evita contar duas vezes o efeito do financiamento.

**E a eletricidade gerada a partir do lixo?**

As centrais de conversão de resíduos em energia também produzem eletricidade.

A sua função de tratamento de lixo pertence ao sistema de saneamento, enquanto a sua produção de eletricidade é considerada em separado no capítulo [Eletricidade](power.md#power).

<a id="garbage-landfills-valve"></a>
#### Quanto lixo pode um aterro aceitar?

Esta é uma das mecânicas mais importantes do SimCity 4.

**Ter espaço livre num aterro não significa que ele possa aceitar lixo ilimitado todos os meses.**

O jogo aplica um limite mensal de entrada separado, que o Ministério modela como uma **válvula do aterro**.

Pense-se nisto como duas restrições diferentes:

- **Capacidade de armazenamento:** quanto lixo o aterro consegue conter no total.
- **Capacidade de entrada:** quanto lixo novo lhe pode ser enviado durante o mês atual.

Um aterro pode ter muito espaço de armazenamento vazio e, mesmo assim, não conseguir aceitar mais lixo nesse mês.

**Entrada diária no aterro (jogo base)**

```math
\text{aceite} =
\min\left(
G,\ F,\ \mathrm{round}(5000 \times L \times e)-S
\right)
```

A quantidade efetivamente aceite não pode ser negativa.

Em que:

- **G** é o lixo que ainda aguarda eliminação.
- **F** é o espaço de armazenamento livre restante.
- **L** é o número de quadrados de aterro contados pelo jogo.
- **e** é a eficiência atual do saneamento.
- **S** é a quantidade já enviada através da válvula do aterro durante o mês atual.
- **5.000** é a capacidade mensal base de entrada por quadrado de aterro.

Sempre que lixo entra no aterro, é somado ao total mensal acumulado da válvula.

**Em termos simples:**

O jogo limita a entrada nos aterros de acordo com o número de quadrados de aterro, o financiamento do saneamento e a quantidade de lixo já aceite nesse mês.

**Exemplo: um aterro com 48 quadrados**

Suponha-se que a cidade tem:

- **48 quadrados de aterro.**
- **50% de financiamento do saneamento.**
- **25% de eficiência do saneamento.**

O limite mensal de entrada é aproximadamente:

```math
5000 \times 48 \times 0.25 = 60000
```

**O aterro pode aceitar aproximadamente 60.000 unidades de lixo nesse mês**, partindo do princípio de que tem espaço de armazenamento livre suficiente e nenhum resto anterior na válvula.

Com 100% de financiamento, o mesmo aterro tem uma entrada mensal teórica de:

```math
5000 \times 48 \times 1.00 = 240000
```

Ou seja, **240.000 unidades por mês**.

<text strong>Comparação</text>

| Financiamento | Eficiência | Entrada mensal aproximada no aterro |
|---|---:|---:|
| 50% | 25% | 60.000 unidades |
| 100% | 100% | 240.000 unidades |

**A área do aterro não mudou. Só mudou o orçamento do saneamento.**

É por isso que reduzir demasiado o financiamento do saneamento pode criar lixo por recolher mesmo quando os aterros ainda têm muito espaço vazio.

<a id="garbage-landfills-rollover"></a>
#### O que acontece ao lixo do aterro no fim do mês?

Há outra distinção importante:

**O lixo que entra num aterro durante um mês não é o mesmo que o lixo armazenado lá de forma permanente.**

O SimCity 4 processa o armazenamento dos aterros a cada mudança de mês simulado.

Acontecem duas coisas:

1. Parte do lixo já armazenado no aterro desaparece gradualmente através do mecanismo de decomposição do jogo.
2. O lixo acumulado na válvula de entrada mensal é transferido para o armazenamento do aterro, sujeito aos limites do jogo.

Isto significa que o armazenamento do aterro e a entrada mensal têm de ser considerados em separado.

#### 1. O lixo armazenado decompõe-se gradualmente

O SimCity 4 retira uma pequena quantidade de lixo armazenado de cada célula do aterro em cada mês simulado.

A quantidade depende do grau de enchimento da célula.

**Decomposição mensal de lixo por célula de aterro (jogo base)**

```math
d =
\min\left(
\text{armazenado},
\max\left(
1,
\min\left(
1500,
\mathrm{round}(0.07 \times \text{armazenado})
\right)
\right)
\right)
```

Em que:

- **armazenado** é o lixo atualmente armazenado numa célula do aterro.
- **d** é a quantidade que desaparece durante a atualização mensal.

Uma célula vazia não perde nada.

Nas células ocupadas, o jogo retira normalmente cerca de **7% do lixo armazenado**, com um mínimo de uma unidade e um máximo de 1.500 unidades por célula e por mês.

**Exemplo**

Imagine-se que uma célula do aterro contém 10.000 unidades de lixo.

```math
10000 \times 0.07 = 700
```

O jogo retira **700 unidades** durante a atualização mensal.

Se uma célula contiver 30.000 unidades:

```math
30000 \times 0.07 = 2100
```

Mas a decomposição máxima é de 1.500 unidades.

O jogo retira, portanto, **1.500 unidades**, e não 2.100.

Nas cidades testadas, uma célula de aterro completamente cheia continha aproximadamente 40.000 unidades e perdia exatamente 1.500 unidades por mês.

**Como se calcula a decomposição de um aterro inteiro?**

O jogo guarda o lixo de forma compacta: primeiro as células cheias, depois uma célula parcialmente preenchida e a seguir as células vazias.

O Ministério consegue, por isso, calcular a decomposição total a partir da quantidade de lixo armazenado e da capacidade de cada célula.

**Decomposição mensal total do aterro (jogo base)**

```math
D(U) =
\left\lfloor\frac{U}{C}\right\rfloor d(C)
+
d(U \bmod C)
```

Em que:

- **U** é a quantidade total de lixo armazenado.
- **C** é a capacidade de uma célula de aterro.
- **d(C)** é a decomposição mensal de uma célula cheia.
- **U mod C** é o lixo que resta na última célula parcialmente preenchida.

A primeira parcela calcula a decomposição das células cheias. A segunda calcula a decomposição da célula parcialmente preenchida, se existir.

Este modelo reproduziu exatamente a decomposição registada pelo jogo ao longo de **11 atualizações mensais consecutivas** numa cidade de teste.

**Porque é que isto importa?**

Os aterros não se enchem simplesmente para sempre a um ritmo constante.

Parte dos resíduos armazenados desaparece todos os meses.

O Ministério tem isto em conta ao estimar quanto espaço de aterro está disponível e com que rapidez se pode encher.

#### 2. A entrada mensal no aterro passa a lixo armazenado

Na atualização mensal, o SimCity 4 transfere lixo da válvula de entrada para os resíduos armazenados do aterro.

No entanto, **a válvula nem sempre é esvaziada por completo**.

A quantidade absorvida depende de:

- O lixo que aguarda na válvula.
- O espaço de armazenamento disponível no aterro.
- O número de quadrados de aterro.
- Quanto financiamento do saneamento é realmente fornecido.

**Absorção mensal do aterro (jogo base)**

```math
\begin{aligned}
H &= \min(S,\ 327\,680\,000) \\[4pt]
\text{se } A < B:\quad
H &= \left\lfloor\frac{H\times A}{B}\right\rfloor \\[4pt]
\text{absorvido} &=
\min\left(
H,\ 5000\times L,\ T_{\mathrm{cap}}-(U-D(U))
\right) \\[4pt]
U' &= U-D(U)+\text{absorvido} \\
S' &= S-\text{absorvido}
\end{aligned}
```

Em que:

- **S** é o lixo que aguarda na válvula mensal do aterro.
- **A** é o montante de manutenção do aterro efetivamente financiado.
- **B** é o custo de manutenção do aterro com 100% de financiamento.
- **L** é o número de quadrados de aterro.
- **Tcap** é a capacidade total de armazenamento do aterro.
- **U** é a quantidade já armazenada antes da atualização mensal.
- **D(U)** é o lixo retirado através da decomposição mensal.
- **absorvido** é o lixo transferido para o armazenamento.
- **U′** é o novo total de lixo armazenado.
- **S′** é a quantidade que fica na válvula para o mês seguinte.

O cálculo do jogo também impõe um limite máximo de **327.680.000 unidades** para a quantidade da válvula considerada na absorção.

Não é preciso calcular isto manualmente. O importante é como o financiamento afeta o resultado.

**Com 100% de financiamento do saneamento**, a redução ligada ao financiamento não se aplica. A válvula pode ser totalmente absorvida quando o armazenamento suficiente e os restantes limites o permitirem.

**Abaixo de 100% de financiamento**, o jogo pode transferir apenas parte da válvula para o armazenamento permanente.

O restante fica na válvula e transita para o mês seguinte.

#### Porque é que o lixo que sobra na válvula é um problema?

Lembre-se de que o aterro tem um limite mensal de entrada.

Se parte da válvula do mês anterior continuar ocupada, o aterro começa o novo mês com menos capacidade de entrada disponível.

Isto cria uma segunda consequência do baixo financiamento do saneamento:

1. **Um financiamento mais baixo reduz a quantidade de lixo que pode entrar no aterro durante o mês.**
2. **Um financiamento mais baixo também pode reduzir a quantidade de lixo absorvida pelo armazenamento na mudança de mês**, deixando parte da quota de entrada ocupada.

**O baixo financiamento do saneamento pode, portanto, limitar o fluxo do aterro de duas formas distintas.**

É por isso que olhar apenas para o armazenamento disponível nos aterros dá uma imagem incompleta do sistema de saneamento.

Um aterro pode estar longe de cheio e, ainda assim, ter dificuldade em tratar o lixo da cidade.

#### Quando é que o jogo atualiza as estatísticas do aterro?

O jogo publica a quantidade de lixo absorvida pelo armazenamento do aterro na atualização mensal seguinte, através do seu contador *Garbage landfilled* (lixo depositado em aterro).

Este valor está, portanto, ligado ao processo mensal de armazenamento, e não simplesmente ao lixo que chega aos aterros no dia em curso.

**A distinção fundamental**

| Conceito | O que mede | Principal limitação |
|---|---|---|
| **Lixo produzido** | Resíduos gerados pelos edifícios da cidade | Atividade da cidade e reciclagem |
| **Capacidade de tratamento** | Resíduos que as incineradoras e as centrais de conversão de resíduos em energia conseguem processar | Financiamento do saneamento e estado dos edifícios |
| **Entrada no aterro** | Lixo novo que os aterros podem aceitar durante o mês | Quadrados de aterro, eficiência e resto da válvula |
| **Armazenamento do aterro** | Lixo acumulado dentro das células do aterro | Espaço de armazenamento disponível e decomposição mensal |
| **Absorção mensal** | Lixo transferido da válvula para o armazenamento | Financiamento, tamanho do aterro e espaço restante |

**O Ministério tem de ter em conta todos estes mecanismos, em vez de simplesmente comparar a produção de lixo com o espaço livre nos aterros.**
<a id="garbage-landfills-cells"></a>
#### Como funcionam as zonas de aterro: quadrados e células de armazenamento

No SimCity 4, um aterro é mais complicado do que a área que se desenha no mapa.

O jogo usa dois elementos diferentes:

- **Quadrados de aterro:** os quadrados individuais que se zoneiam para a eliminação de lixo.
- **Células de armazenamento:** áreas invisíveis onde o jogo guarda realmente o lixo acumulado.

Cada célula de armazenamento corresponde a um **grupo de 2 × 2 quadrados de aterro**.

Uma célula só pode armazenar lixo quando os quatro quadrados pertencem à zona do aterro.

**Quantas células de armazenamento tem um aterro?**

O jogo regista cada aterro dentro de uma área retangular, chamada *bounding box* (caixa delimitadora).

Este retângulo contém também informação que identifica os quadrados que pertencem realmente ao aterro.

Para um retângulo de **w quadrados de largura e h quadrados de altura**, a grelha de células de armazenamento possíveis contém:

```math
N_{\text{possible}}=(w-1)(h-1)
```

No entanto, só estão ativas as células cujos quatro quadrados fazem parte do aterro.

<box gap={2}>
**Exemplo: um aterro de 3 × 3**

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
  <text x="73" y="136" textAnchor="middle" fill="currentColor" fontSize="12">9 quadrados de aterro</text>
  <text x="228" y="136" textAnchor="middle" fill="currentColor" fontSize="12">4 células de armazenamento</text>
  <path d="M138 65 L162 65 M156 60 L162 65 L156 70" fill="none" stroke="currentColor" strokeWidth="1.5"/>
</svg>
<caption>O diagrama da direita mostra as posições possíveis das células de armazenamento, não uma segunda área de aterro.</caption>
</box>

Um aterro de 3 × 3 totalmente zoneado tem **9 quadrados, mas 4 células de armazenamento sobrepostas**.

Um aterro de 2 × 2 tem apenas uma célula de armazenamento.

Esta distinção importa porque **a manutenção do aterro é calculada a partir dos quadrados, enquanto o armazenamento de lixo está organizado em células**.

#### Quanto lixo pode um aterro armazenar?

O jogo atribui capacidade de armazenamento a cada célula ativa.

Essa capacidade depende de:

- O número de quadrados que pertencem à zona do aterro.
- O número de células de armazenamento ativas.

**Capacidade de armazenamento do aterro (jogo base)**

```math
C = \mathrm{round}\left(\frac{T\times30000}{N}\right)
\qquad
T_{\mathrm{cap}}=N\times C
```

Em que:

- **T** é o número de quadrados registados na zona do aterro, incluindo os quadrados fantasma (explicados abaixo).
- **N** é o número de células de armazenamento ativas.
- **30.000** é a capacidade de armazenamento do jogo por quadrado de aterro.
- **C** é a capacidade atribuída a cada célula ativa.
- **Tcap** é a capacidade total de armazenamento do aterro.

O jogo arredonda a capacidade da célula à unidade inteira mais próxima, arredondando os valores exatamente a meio para o inteiro par mais próximo.

**Exemplos**

| Aterro | Células ativas | Capacidade por célula | Capacidade total |
|---|---:|---:|---:|
| Aterro de 2 × 2 | 1 | 120.000 | 120.000 |
| Aterro de 129 quadrados | 70 | 55.286 | 3.870.020 |

Em geral, um aterro oferece aproximadamente **30.000 unidades de armazenamento por quadrado zoneado**.

O total exato pode diferir ligeiramente porque a capacidade é arredondada separadamente para cada célula.

A capacidade total de aterro da cidade é a soma das capacidades de todas as suas zonas de aterro.

#### O que acontece quando se amplia ou se reduz um aterro?

É aqui que a estrutura interna de células do jogo se torna importante.

**Ampliar um aterro**

Quando se acrescentam novos quadrados de aterro, o SimCity 4 reconstrói a grelha de células de armazenamento da zona.

As células existentes mantêm normalmente o lixo armazenado, enquanto as novas células válidas começam vazias.

No entanto, **a direção em que se amplia pode afetar o lixo armazenado**.

O jogo identifica as células de armazenamento pelas suas posições relativas ao canto da caixa delimitadora do aterro.

- **Ampliar para a direita ou para baixo:** o canto original mantém-se no lugar, pelo que as células existentes conservam normalmente as suas posições.
- **Ampliar para a esquerda ou para cima:** o canto desloca-se, alterando as posições relativas das células existentes. Isto pode fazer com que o lixo armazenado deixe de corresponder aos locais previstos.

Esta é a causa de um problema conhecido dos aterros do SimCity 4, que por vezes surge depois de ampliar um aterro.

**A abordagem mais segura é ampliar os aterros para a direita ou para baixo da sua área existente, segundo as coordenadas do mapa do jogo.**

A ampliação pode também alterar a quantidade de lixo que cada célula pode conter.

Se a nova capacidade de uma célula passar a ser inferior à quantidade já armazenada nela, **o lixo em excesso é devolvido à válvula de entrada do aterro**.

Esse lixo tem então de ser processado de novo pelo sistema do aterro.

Uma operação de zonamento faz também com que o jogo verifique a informação de pertença da zona e remova os quadrados que já não estão efetivamente zoneados como aterro.

**Reduzir um aterro**

Retirar quadrados de aterro comporta-se de forma diferente.

Quando se retira o zonamento de um quadrado, o jogo:

1. Retira-o da área registada do aterro.
2. Desativa as células de armazenamento afetadas.
3. Recalcula a capacidade das células ativas restantes.
4. Devolve à válvula de entrada qualquer lixo armazenado em excesso.

Ao contrário da ampliação, **retirar o zonamento não reconstrói toda a grelha de células de armazenamento nem reconcilia por completo a informação de pertença da zona**.

O jogo impede também certas alterações:

- Não se pode retirar o zonamento a um quadrado que contenha uma pilha de lixo.
- As zonas de aterro têm de respeitar as restrições de tamanho do jogo, incluindo a largura mínima de dois quadrados e a largura máxima de 48 quadrados.

**Porque é que isto importa?**

Alterar a forma de um aterro pode afetar mais do que a sua área visível.

Pode também alterar a quantidade de lixo que cada célula consegue conter e a quantidade de lixo devolvida ao sistema de entrada mensal.

O Ministério considera estes efeitos ao avaliar alterações ao tamanho do aterro.

#### Quanto custa um aterro?

A manutenção do aterro calcula-se **quadrado a quadrado**.

Os quadrados de aterro vazios têm um custo de manutenção fixo, enquanto os quadrados com pilhas de lixo custam mais.

**Manutenção do aterro com 100% de financiamento (jogo base)**

```math
B=\sum_{\text{quadrados da zona}}c_i
\qquad
c_i=
\begin{cases}
\text{custo de manutenção da pilha}, & \text{se existir uma pilha de lixo}\\
5, & \text{caso contrário}
\end{cases}
```

Em que:

- **B** é o custo mensal de manutenção do aterro com 100% de financiamento.
- **cᵢ** é o custo de manutenção de um quadrado individual.

Nas cidades examinadas, os custos observados foram:

| Tipo de quadrado | Manutenção mensal a 100% |
|---|---:|
| Quadrado de aterro vazio | §5 |
| Quadrado com uma pilha de lixo | §20, §30 ou §40, consoante a pilha |

**Um aterro fica mais caro à medida que o lixo se acumula.**

Por exemplo, um aterro com 100 quadrados vazios custa:

```math
100\times5=500
```

Ou seja, **§500 por mês com 100% de financiamento**, mesmo antes de começarem a surgir pilhas de lixo.

Se 20 desses quadrados desenvolverem pilhas de lixo que custam §20 cada, enquanto os restantes 80 ficam vazios:

```math
(80\times5)+(20\times20)=800
```

A manutenção mensal sobe para **§800**.

É por isso que o tamanho do aterro importa do ponto de vista económico.

Manter uma grande área de aterro por utilizar pode gerar despesas de manutenção desnecessárias.

O Ministério consegue identificar situações em que reduzir o zonamento de aterro não utilizado pouparia dinheiro, desde que a alteração seja segura.

#### Quadrados fantasma: áreas de aterro que não deviam existir

O SimCity 4 tem um erro que pode deixar para trás **quadrados de aterro fantasma**.

Um quadrado fantasma é um local que o jogo continua a contar como parte de um aterro, apesar de já não estar zoneado como aterro.

Por exemplo, o quadrado pode agora conter:

- Uma estrada vulgar.
- Um terreno sem zonamento.
- Outra área que não é de aterro.

Visualmente, o aterro desapareceu desse quadrado.

**Internamente, porém, o jogo continua a contá-lo como parte da zona de aterro.**

Isto tem duas consequências:

1. **O quadrado continua a custar §5 por mês** em manutenção do aterro com 100% de financiamento.
2. **É incluído na contagem de quadrados registada da zona** quando a capacidade de armazenamento do aterro é recalculada.

Os quadrados fantasma não contam para o total de quadrados de aterro ativos usado para calcular o limite mensal de entrada.

Por outras palavras, **o jogo pode cobrar quadrados de aterro que, na prática, não ajudam a eliminar lixo**.

#### Porque é que os quadrados fantasma regressam ao reabrir uma cidade?

O problema vem da forma como o SimCity 4 grava e volta a carregar as zonas de aterro.

Ao gravar um aterro, o jogo regista a sua caixa delimitadora retangular e os quadrados que pertencem à zona.

No entanto, não grava corretamente a pertença dos quadrados da última linha e da última coluna do retângulo.

Quando a cidade é reaberta, o jogo assume inicialmente que todos os quadrados dentro do retângulo pertencem ao aterro e depois restaura a informação de pertença gravada.

Como a última linha e a última coluna têm informação gravada incompleta, **os quadrados desses bordos podem voltar, incorretamente, a ser membros do aterro**.

Isto cria dois tipos de quadrados fantasma:

| Localização do quadrado fantasma | O que acontece depois da reparação |
|---|---|
| **Dentro do retângulo da zona** | Uma operação de zonamento adequada pode removê-lo de forma permanente, porque a sua pertença corrigida fica gravada. |
| **Na última linha ou coluna** | Pode regressar quando a cidade é reaberta, porque o jogo não grava corretamente a pertença desse bordo. |

As ferramentas de zonamento do jogo original não conseguem remover permanentemente todos os fantasmas de bordo, porque a caixa delimitadora registada da zona não encolhe.

**Como ajuda o Ministério?**

O Ministério inclui um mecanismo automático de reparação de quadrados fantasma.

Quando deteta quadrados fantasma elegíveis, usa o próprio sistema de zonamento do jogo para os corrigir, em vez de modificar diretamente o ficheiro de gravação.

A reparação foi concebida para não perturbar as áreas de aterro legítimas nem o lixo armazenado.

**Não existe uma definição separada para a reparação de quadrados fantasma.** Funciona como parte da gestão automática do saneamento.

No entanto, o erro subjacente de gravação e carregamento continua no SimCity 4. Alguns fantasmas de bordo podem, por isso, regressar ao reabrir uma cidade e exigir nova correção.

<a id="garbage-landfills-problem"></a>
### 2. O problema da gestão

Gerir o lixo no SimCity 4 é mais complicado do que simplesmente dispor de espaço de aterro suficiente.

O Ministério tem de ter em conta vários problemas.

- **O lixo chega em momentos diferentes:** a cidade produz lixo todos os dias, enquanto os contratos regionais de lixo costumam entregar a maior parte das suas quantidades mensais nos primeiros dez dias, mais ou menos.
- **O armazenamento do aterro não é o mesmo que a capacidade de eliminação:** um aterro pode ter muito espaço vazio e, mesmo assim, atingir o seu limite mensal de entrada.
- **O baixo financiamento afeta a eliminação do lixo de várias formas:** reduz a capacidade de tratamento e pode deixar parte da válvula de entrada do aterro ocupada quando o mês seguinte começa.
- **As estatísticas do jogo podem ter atraso:** os números de lixo importado referem-se ao mês anterior, e alguns valores de contratos e de saneamento demoram a atualizar-se.
- **A curva de financiamento não é proporcional:** reduzir demasiado o financiamento do saneamento pode retirar muito mais capacidade de tratamento do que o esperado.
- **As áreas de aterro por utilizar custam dinheiro:** os quadrados vazios têm despesas mensais de manutenção, e os quadrados fantasma podem criar encargos adicionais.
- **Alterar a forma do aterro pode afetar o lixo armazenado:** ampliar ou reduzir um aterro pode redistribuir a capacidade de armazenamento e devolver o excesso de lixo ao sistema de entrada.

**O Ministério tem de considerar todos estes fatores antes de decidir de quanto financiamento do saneamento a cidade precisa.**

Não consegue optimizar o saneamento em segurança olhando apenas para a produção de lixo atual ou para a quantidade de espaço livre nos aterros.

<a id="garbage-landfills-algorithm"></a>
### 3. Como o Ministério gere o lixo

O Ministério usa as estatísticas de lixo do jogo para estimar a quantidade de resíduos que a cidade tem de tratar.

Considera depois as instalações de tratamento de lixo, a capacidade dos aterros, a acumulação atual de resíduos e os contratos regionais ao escolher um orçamento de saneamento adequado.

O objetivo é **eliminar o lixo que a cidade precisa de tratar, manter uma reserva de segurança e evitar despesas mensais desnecessárias**.

<a id="garbage-landfills-need"></a>
#### 3.1 Quanto lixo tem a cidade de tratar?

O Ministério começa por calcular quanto lixo a própria cidade produz depois da reciclagem.

**Passo 1: Calcular o lixo da própria cidade**

```math
G_{\mathrm{own}}=\max(0,P-R)
```

Em que:

- **P** é o lixo produzido durante o mês simulado anterior.
- **R** é o lixo reciclado nesse mês.
- **Gown** é o lixo que resta para eliminar.

Por exemplo, suponha-se que a cidade produziu 100.000 unidades de lixo no mês passado e reciclou 20.000.

```math
G_{\mathrm{own}}=100000-20000=80000
```

O Ministério estima, portanto, que a cidade gera **80.000 unidades de lixo por mês que exigem eliminação**.

**O lixo importado não é incluído neste cálculo.**

As estatísticas de lixo importado do jogo referem-se às entregas do mês anterior.

O Ministério trata o lixo estrangeiro em separado, usando os contratos regionais reais em vez de misturar os resíduos importados com a produção da própria cidade.

#### Passo 2: Acrescentar a reserva de segurança

A produção de lixo pode aumentar à medida que a cidade cresce.

Para evitar escolher um orçamento que só chega para a quantidade exata do mês anterior, o Ministério acrescenta uma reserva de segurança.

A **reserva de saneamento** predefinida é de **2%**, ajustável de 2% a 20%.

**Produção de lixo protegida**

```math
G_{\mathrm{prot}}=
\left\lceil
G_{\mathrm{own}}\left(1+\frac{r}{100}\right)
\right\rceil
```

Em que:

- **Gown** é o lixo da própria cidade depois da reciclagem.
- **r** é a reserva de saneamento configurada.
- **Gprot** é a quantidade de lixo protegida, arredondada por excesso.

**Exemplo: uma cidade que produz 75.464 unidades de lixo depois da reciclagem**

Com a reserva predefinida de 2%:

```math
G_{\mathrm{prot}}=
\lceil75464\times1.02\rceil
=76974
```

O Ministério planeia, portanto, para **76.974 unidades** em vez de exatamente 75.464.

A reserva é um mínimo, não um máximo. O Ministério pode ter de tratar mais lixo por causa de resíduos já acumulados ou de contratos regionais.

#### Passo 3: Incluir o lixo acumulado e as importações

A cidade pode já ter lixo à espera de ser recolhido.

Pode também receber resíduos de cidades vizinhas.

O Ministério soma estas quantidades para obter uma necessidade global mensal de tratamento.

**Necessidade mensal de tratamento de lixo**

```math
H=G_{\mathrm{prot}}+Q+\sum\text{quantidades importadas}
```

Em que:

- **Gprot** é a produção mensal de lixo protegida da cidade.
- **Q** é o lixo por recolher atualmente acumulado na grelha da cidade.
- **Quantidades importadas** são as quantidades de lixo especificadas pelos contratos regionais de importação.
- **H** é a necessidade global mensal de tratamento.

**Exemplo**

Suponha-se que a cidade tem:

| Origem | Unidades de lixo |
|---|---:|
| Lixo próprio, incluindo a reserva | 76.974 |
| Lixo já por recolher | 5.000 |
| Lixo importado dos vizinhos | 20.000 |
| **Necessidade total de tratamento** | **101.974** |

O Ministério tem, portanto, de considerar **101.974 unidades** de resíduos ao planear o saneamento.

No entanto, **só este total não chega para decidir se um nível de financiamento vai funcionar**.

O lixo chega e é processado em momentos diferentes ao longo do mês.

Uma cidade pode ter capacidade mensal total de tratamento suficiente e, mesmo assim, sofrer uma acumulação temporária de lixo porque as importações chegam cedo ou a entrada nos aterros está temporariamente limitada.

Por essa razão, o Ministério usa este total para o seu resumo e registos mensais, mas toma a decisão final com uma **simulação dia a dia da eliminação do lixo**.

Essa simulação considera quando o lixo chega, que instalações o processam, como se comporta a válvula do aterro e se restam resíduos por recolher.

As secções seguintes explicam como funciona esta previsão.
<a id="garbage-landfills-uncollected"></a>
#### 3.2 O que acontece ao lixo por recolher?

Por vezes, a cidade produz mais lixo do que o seu sistema de eliminação consegue tratar.

O excesso fica na grelha da cidade como **lixo por recolher**.

O Ministério lê a quantidade total diretamente do jogo e trata-a como um **atraso acumulado**, representado por **Q**.

**O objetivo é eliminar este atraso até ao fim do mês simulado em curso**, além de tratar o lixo produzido nesse mês.

Por exemplo, se a cidade já tem 10.000 unidades de lixo por recolher, o Ministério inclui essas 10.000 unidades nos seus cálculos.

Não ignora o lixo existente só porque a produção normal da cidade está coberta.

**E o lixo que o jogo remove automaticamente?**

Como explicado antes, o SimCity 4 tem um mecanismo oculto que pode remover discretamente uma quantidade limitada de lixo em excesso.

O Ministério não trata esta remoção como uma falha do saneamento nem a usa como motivo para impedir reduções de financiamento.

Só o lixo que realmente continua por recolher na grelha da cidade contribui para o atraso acumulado.

<a id="garbage-landfills-contracts"></a>
#### 3.3 Como se incluem os contratos de lixo

Os contratos de lixo são mais complicados do que parecem, porque **as suas quantidades mensais são entregues ao longo de vários dias**, e não de uma só vez nem de forma uniforme durante o mês.

O Ministério calcula as entregas de **cada contrato em separado**.

Não se limita a somar todas as quantidades dos contratos e a assumir que chegam de modo uniforme.

#### Importações de lixo

Quando outra cidade lhe envia lixo, o Ministério assume que o jogo entrega aproximadamente **10% da quantidade mensal desse contrato em cada dia**, até a quota mensal se esgotar.

Por exemplo, um contrato para importar 20.000 unidades entrega aproximadamente 2.000 unidades por dia durante os primeiros dez dias do mês.

O Ministério tem também em conta o arredondamento. Como a última entrega do jogo pode exceder ligeiramente a quantidade contratada, não subestima o lixo que chega.

**O lixo importado é sempre tratado como resíduos adicionais que a cidade tem de eliminar.**

Uma importação suspensa continua a ser contada de forma prudente ao avaliar os resíduos que a cidade poderá ter de tratar.

#### Exportações de lixo

Quando se paga a outra cidade para levar o seu lixo, o contrato reduz a quantidade que as suas próprias instalações de saneamento têm de processar.

O Ministério calcula este alívio com o mesmo calendário de entregas diárias.

No entanto, **nunca assume que se pode exportar mais lixo do que o contrato realmente permite**.

Por exemplo, um contrato de exportação de 20.000 unidades proporciona aproximadamente 2.000 unidades de alívio de eliminação por dia, até a quota mensal se esgotar.

Ao contrário das importações, o último crédito diário de exportação fica limitado à quantidade ainda disponível.

**E se um contrato de exportação estiver suspenso?**

O Ministério não conta com uma exportação suspensa para remover lixo.

Também não atribui qualquer crédito de eliminação quando o uso do contrato registado pelo jogo não pode ser lido de forma fiável.

O contrato continua a ser uma obrigação existente, mas o Ministério não assume que fornecerá capacidade de eliminação utilizável.

Isto impede que o plano de saneamento dependa de exportações que podem não acontecer.

#### Um problema com as estatísticas dos contratos do jogo

Durante os primeiros dias simulados de um novo mês, o SimCity 4 pode mostrar temporariamente a quantidade utilizada durante o **mês anterior**.

Por exemplo, imagine-se um contrato de exportação de 7.000 unidades.

No primeiro dia de um novo mês, o jogo pode ainda indicar:

**Quantidade utilizada: 7.000 unidades**

Isso não significa necessariamente que o contrato do novo mês já se esgotou.

O Ministério deteta esta situação durante os **três primeiros dias simulados**.

Se a utilização comunicada for superior ao que o contrato poderia ter entregue desde o início do mês, o Ministério trata essa leitura como desatualizada e começa o cálculo do mês a partir de zero.

**Teste de leitura desatualizada**

```math
U > (\text{dia do mês}-1)\times\mathrm{round}(0.1\times A)
```

Em que:

- **A** é a quantidade mensal do contrato.
- **U** é a quantidade que o jogo indica como já utilizada neste mês.

Isto impede que estatísticas desatualizadas provoquem decisões de financiamento incorretas.

#### Quando compensa importar lixo?

Importar lixo rende dinheiro da cidade vizinha, mas também cria mais trabalho para o sistema de saneamento.

O Ministério só considera aceitável uma importação quando o financiamento do saneamento escolhido consegue tratar o lixo adicional **sem deixar um atraso acumulado no fim do mês**.

Por vezes, o rendimento extra pode justificar um aumento do financiamento do saneamento.

Por exemplo, aceitar lixo importado pode exigir um orçamento de saneamento mais alto, mas ainda assim dar um lucro líquido se o vizinho pagar o suficiente.

O gestor de contratos compara estes custos e receitas antes de tomar as suas decisões.

Ver [Acordos com os vizinhos](deals.md#neighbor-deals) para as regras completas dos contratos.

<a id="garbage-landfills-projection"></a>
#### 3.4 Simulação dos dias restantes do mês

Esta é a parte mais importante dos cálculos de saneamento do Ministério.

**Em vez de comparar apenas totais mensais, o Ministério simula como o lixo será tratado em cada um dos dias que faltam até ao fim do mês.**

Porquê?

Porque o lixo chega e é eliminado em momentos diferentes.

- A cidade produz lixo todos os dias.
- As importações costumam chegar durante os primeiros dez dias, mais ou menos.
- As exportações aliviam a eliminação de acordo com os seus próprios calendários diários.
- As incineradoras e as centrais de conversão de lixo em energia têm uma capacidade de tratamento limitada.
- Os aterros têm um limite mensal de admissão que se vai esgotando progressivamente.

Uma cidade pode ter capacidade mensal teórica suficiente e, mesmo assim, sofrer uma acumulação temporária de lixo.

O Ministério tem de determinar se esse lixo acumulado pode ser eliminado antes de o mês terminar.

#### Como funciona a simulação diária

Para cada percentagem de financiamento do saneamento possível, o Ministério faz o cálculo seguinte para cada um dos dias que faltam.

<box gap={2}>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="trash-2" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **1. Somar o lixo que chega**

      Parte-se do lixo que sobrou do dia anterior e somam-se a produção diária estimada da cidade e as importações desse dia.
    </box>
  </row>
  <divider color="subtle"/>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="truck" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **2. Aplicar as exportações de lixo**

      Retira-se todo o lixo que os contratos de exportação disponíveis conseguirem receber. O crédito de exportação não utilizado pode transitar para o dia seguinte.
    </box>
  </row>
  <divider color="subtle"/>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="factory" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **3. Usar as instalações de tratamento**

      As incineradoras e as centrais de conversão de lixo em energia tratam o lixo até à sua capacidade diária de tratamento disponível.
    </box>
  </row>
  <divider color="subtle"/>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="layers" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **4. Usar a capacidade dos aterros**

      O lixo que restar é enviado para os aterros, desde que a válvula de admissão mensal ainda tenha capacidade.
    </box>
  </row>
  <divider color="subtle"/>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="calendar-check" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **5. Transitar o lixo por recolher**

      O lixo que não puder ser tratado hoje fica acumulado para o dia seguinte.
    </box>
  </row>
</box>

O Ministério repete este processo até ao último dia do mês.

**Considera-se que um nível de financiamento é suficiente quando o lixo acumulado projetado no fim do mês é zero.**

A acumulação temporária durante o mês é permitida, desde que possa ser eliminada antes de o mês terminar.

O Ministério não impõe nenhum limite artificial adicional ao pico temporário de lixo acumulado.

**Projeção diária do lixo**

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

Em que:

- **n** é o número de dias do mês simulado.
- **Gprot** é a produção mensal de lixo protegida da cidade.
- **Q₀** é o lixo acumulado por recolher neste momento.
- **Iₜ** é o lixo importado no dia *t*.
- **cₜ** é o crédito de exportação gerado nesse dia.
- **Rₜ** é o crédito de exportação não utilizado que transita.
- **aₜ** é o lixo que precisa de ser eliminado nesse dia.
- **xₜ** é o lixo retirado através das exportações.
- **bₜ** é o lixo tratado pelas instalações de tratamento.
- **lₜ** é o lixo enviado para os aterros.
- **K(p)** é a capacidade mensal de tratamento com o financiamento **p**.
- **Vₜ** é a margem de admissão dos aterros que resta.
- **Qₜ** é o lixo que continua por recolher no fim do dia *t*.

O Ministério aproxima a produção de lixo da própria cidade como uniformemente distribuída ao longo do mês.

As importações e as exportações, porém, seguem os calendários de entrega de cada contrato.

O cálculo é feito para cada nível de financiamento candidato.

#### Como se ajusta a capacidade de tratamento a diferentes níveis de financiamento?

O jogo já indica a capacidade mensal de tratamento combinada das incineradoras e das centrais de conversão de lixo em energia.

O Ministério usa esse valor para estimar qual seria a capacidade com outra percentagem de financiamento.

**Capacidade de tratamento projetada**

```math
K(p)=\mathrm{round}\left(K_{\mathrm{now}}\times\frac{e(p)}{e_{\mathrm{now}}}\right)
```

Em que:

- **Know** é a capacidade de tratamento combinada que o jogo indica atualmente.
- **e(p)** é a eficiência do saneamento com o financiamento proposto.
- **enow** é a eficiência atual.
- **K(p)** é a capacidade de tratamento projetada.

**Exemplo**

Durante os testes, ao passar o financiamento do saneamento de 61% para 50%, a eficiência passou de 40,125% para 25%.

A capacidade da central de conversão de lixo em energia desceu de aproximadamente **34.088 para 21.238 unidades**, de acordo com o modelo.

Se a eficiência atual for zero e existirem edifícios de tratamento, o Ministério não consegue calcular com fiabilidade capacidades alternativas e interrompe o plano de financiamento em vez de adivinhar.

#### Como se calcula a admissão disponível nos aterros?

O Ministério estima também quanto lixo mais os aterros podem aceitar durante o mês em curso.

**Admissão restante dos aterros**

```math
V_0(p)=
\max\left(
0,\min\left(
F,\mathrm{round}(5000\times L\times e(p))-S
\right)
\right)
```

Em que:

- **F** é o espaço livre de armazenamento que resta nos aterros.
- **L** é o número de quadrados de aterro.
- **e(p)** é a eficiência do saneamento com o financiamento proposto.
- **S** é o lixo já acumulado na válvula mensal do aterro.
- **V₀(p)** é a margem de admissão restante com esse financiamento.

Por exemplo, um aterro pode ter 100.000 unidades de armazenamento livre, mas apenas 20.000 unidades de capacidade mensal de admissão por usar.

**O Ministério conta apenas as 20.000 unidades como disponíveis para eliminação durante o resto desse mês.**

Se não existir nenhum aterro, a admissão disponível nos aterros é zero.

Se não for possível ler a informação necessária sobre o aterro, o Ministério não assume que o aterro possa aceitar mais lixo.

Quando as exportações e as instalações de tratamento conseguem tratar todo o lixo restante, não é preciso capacidade de aterro.

<a id="garbage-landfills-nextmonth"></a>
#### 3.5 Porque é que o Ministério verifica por vezes também o mês seguinte?

Como explicado antes, um financiamento baixo do saneamento pode deixar lixo na válvula de admissão do aterro quando um mês termina.

Esse lixo que sobra reduz a admissão disponível do aterro durante o mês seguinte.

**Um nível de financiamento que resolve o mês em curso pode, por isso, criar um problema no seguinte.**

Para o evitar, o Ministério pode simular também **todo o mês seguinte**.

No entanto, só faz esta verificação adicional quando tem informação suficiente para prever com fiabilidade o comportamento interno do armazenamento do aterro.

#### Como aprende a estrutura de armazenamento de um aterro?

O SimCity 4 não fornece diretamente todos os pormenores da disposição interna das células do aterro necessários para este cálculo.

Por isso, o Ministério aprende a estrutura observando as alterações mensais normais.

Compara:

- O lixo armazenado antes da mudança de mês.
- O lixo armazenado depois.
- A quantidade acabada de absorver pelo armazenamento do aterro.

**Decaimento mensal observado do aterro**

```math
D_{\mathrm{obs}}=
U_{\mathrm{before}}-U_{\mathrm{after}}+\text{absorbed}
```

Em que:

- **Ubefore** é a quantidade armazenada antes da atualização mensal.
- **Uafter** é a quantidade armazenada depois.
- **absorbed** é a quantidade transferida para o armazenamento durante essa atualização.
- **Dobs** é o lixo eliminado por decaimento.

O Ministério compara este resultado com a fórmula de decaimento do aterro do jogo.

Testa números possíveis de células de armazenamento e verifica se reproduzem **exatamente** o decaimento observado.

O número possível de células tem também de dividir a capacidade total de armazenamento do aterro em unidades inteiras.

**Quando uma única disposição corresponde às observações, considera-se que a geometria do aterro está calibrada.**

Se continuarem a ser possíveis várias disposições, o Ministério espera por mais observações mensais.

Se nenhuma corresponder, ou se a estrutura do aterro da cidade não puder ser representada com fiabilidade por uma única disposição, não usa a previsão do mês seguinte.

Por exemplo, uma cidade com vários aterros de estruturas internas de células diferentes pode não produzir observações que se ajustem a um único modelo de geometria.

**Quanto tempo demora a calibração?**

O Ministério aprende com as atualizações mensais normais, sem exigir qualquer ação especial da sua parte.

Numa cidade testada, uma única mudança de mês normal deu informação suficiente para identificar a geometria do aterro.

Isso significou que a verificação do mês seguinte ficou disponível no segundo evento mensal da sessão.

No entanto, a calibração pode exigir mais observações ou pode não ser possível em algumas cidades.

**A geometria aprendida só é mantida durante a sessão atual da cidade.**

Não é guardada entre sessões.

Se a capacidade do aterro mudar porque zonou ou retirou o zonamento a quadrados, o Ministério volta a recolher indícios.

#### Como funciona a previsão do mês seguinte?

Depois de a geometria do aterro estar calibrada, o Ministério pode estimar:

1. Quanto lixo armazenado vai decair na mudança para o mês seguinte.
2. Quanto lixo da válvula de admissão vai ser absorvido pelo armazenamento.
3. Quanto lixo poderá ficar na válvula.
4. Que capacidade de admissão do aterro estará disponível durante o mês seguinte.

Usa as fórmulas de absorção mensal do jogo descritas antes.

Como o valor de manutenção financiada do jogo pode, por vezes, diferir um simoleão do cálculo esperado, o Ministério testa ambos:

```math
A=\left\lfloor B\times\frac{p}{100}\right\rfloor
```

e o mesmo valor aumentado de uma unidade.

Depois repõe a zero a utilização mensal simulada de cada contrato de lixo e repete a projeção diária do lixo para todo o mês seguinte, começando sem lixo acumulado por recolher.

**O nível de financiamento só é aceite quando os dois meses terminam sem lixo por recolher.**

Se a geometria do aterro não puder ser calibrada, o Ministério usa apenas o cálculo do mês em curso.

Nessa situação, o gestor regional de contratos comporta-se também de forma mais conservadora ao considerar alterações aos contratos de lixo existentes.

Ver [Acordos com os vizinhos](deals.md#neighbor-deals).

<a id="garbage-landfills-funding"></a>
#### 3.6 Como escolhe o Ministério o financiamento do saneamento?

Depois de calcular quanto lixo pode ser tratado com cada nível de financiamento, o Ministério procura **a percentagem mais barata que cumpre os requisitos de eliminação**.

As despesas de saneamento aumentam em proporção com o financiamento.

**Despesa mensal de saneamento**

```math
E(p)=\mathrm{round}\left(I\times\frac{p}{100}\right)
```

Em que:

- **I** é o custo mensal do departamento de saneamento com 100% de financiamento, conforme indicado pelo jogo.
- **p** é a percentagem de financiamento proposta.
- **E(p)** é a despesa mensal estimada.

Este custo inclui a manutenção dos aterros e as restantes linhas de orçamento do departamento.

Por exemplo, se o departamento de saneamento custar §2.000 por mês com 100% de financiamento:

| Financiamento | Despesa mensal estimada |
|---|---:|
| 50% | §1.000 |
| 75% | §1.500 |
| 100% | §2.000 |
| 120% | §2.400 |

Mas a percentagem mais barata não é necessariamente segura.

Um orçamento baixo pode reduzir a capacidade de tratamento, restringir a admissão dos aterros ou deixar lixo por recolher no fim do mês.

**Optimização do financiamento**

```math
p^*=
\arg\min_{p\in[0,p_{\max}]}E(p)
```

Sujeito a:

- O mês em curso terminar sem lixo por recolher.
- O mês seguinte terminar também sem lixo por recolher, quando a geometria do aterro está calibrada.

Aqui, **pmax** é o financiamento máximo do saneamento permitido pelo jogo.

**Como encontra a percentagem adequada mais baixa?**

Como a capacidade de eliminação modelada normalmente aumenta com o financiamento, o Ministério pode procurar de forma eficiente a percentagem mais baixa que cumpre os requisitos.

Usa uma pesquisa binária quando a relação é monótona. Se esse pressuposto não se verificar, verifica individualmente as percentagens disponíveis.

Se várias percentagens tiverem a mesma despesa mensal arredondada, prefere a mais próxima do financiamento atual.

Isto evita alterações desnecessárias que não poupam dinheiro.

#### E se nem o financiamento máximo conseguir eliminar o lixo?

Por vezes, o sistema de saneamento simplesmente não tem capacidade de eliminação suficiente.

Por exemplo, um aterro pode estar quase cheio.

Nesse caso, aumentar o financiamento do saneamento não cria necessariamente mais espaço de armazenamento utilizável.

Por isso, o Ministério evita escolher automaticamente 120% quando isso não resolveria nada.

Em vez disso:

1. Calcula o menor acumulado de lixo que se consegue alcançar com o financiamento máximo.
2. Procura o **nível de financiamento mais barato que produz esse mesmo acumulado**.
3. Escolhe esse nível, em vez de pagar a mais sem melhoria nenhuma.

**O Ministério não gasta mais quando um financiamento adicional não consegue melhorar o resultado.**

Se um financiamento mais alto reduzir de facto o lixo acumulado, pode continuar a escolher o financiamento máximo.

Os contratos regionais de lixo também podem ajudar a resolver uma falta, consoante as cidades vizinhas disponíveis e as regras dos contratos.

<a id="garbage-landfills-writing"></a>
#### 3.7 Como são aplicadas as alterações de financiamento?

Ao contrário da energia e da água, o financiamento do saneamento **nem sempre é alterado diretamente para o valor final pretendido**.

O motivo é que o SimCity 4 demora vários dias simulados a atualizar a eficiência do saneamento e as capacidades de tratamento depois de uma alteração ao orçamento.

Fazer várias alterações grandes antes de o jogo acabar de atualizar poderia levar a decisões instáveis.

**Por isso, o Ministério reduz o financiamento do saneamento de forma gradual.**

| Alteração | Como é aplicada |
|---|---|
| **Redução do financiamento** | No máximo **25 pontos percentuais** de cada vez, com pelo menos **7 dias simulados** entre alterações do financiamento do saneamento. |
| **Aumento do financiamento** | O aumento necessário completo pode ser aplicado de uma só vez, com pelo menos **3 dias simulados** desde a alteração anterior do financiamento do saneamento. |

São pontos percentuais, não percentagens do orçamento atual.

**Exemplo: reduzir o financiamento de 120% para 31%**

Em vez de passar logo de 120% para 31%, o Ministério segue esta sequência:

<box gap={2}>
  <row align="center" justify="between">
    <text weight="medium">Financiamento inicial</text>
    <text weight="medium" tabularNums>120%</text>
  </row>
  <box background="surface-tertiary" height="8px" radius="full" clip>
    <box background="rgba(100,116,139,0.85)" width="100%" height="100%" radius="full"/>
  </box>
  {#each [{p:95,w:"79.2%",d:"Pelo menos 7 dias depois"},{p:70,w:"58.3%",d:"Pelo menos mais 7 dias"},{p:45,w:"37.5%",d:"Pelo menos mais 7 dias"},{p:31,w:"25.8%",d:"Pelo menos mais 7 dias"}] as s}
    <row align="center" justify="between">
      <text color="secondary" size="sm">{s.d}</text>
      <text weight="medium" tabularNums>{s.p}%</text>
    </row>
    <box background="surface-tertiary" height="8px" radius="full" clip>
      <box background="rgba(100,116,139,0.85)" width={s.w} height="100%" radius="full"/>
    </box>
  {/each}
</box>

Chegar ao valor final demora, por isso, **pelo menos várias semanas simuladas**, possivelmente cerca de um mês.

Isto dá ao jogo tempo para atualizar a sua eficiência e a capacidade de eliminação entre alterações.

**Os aumentos de financiamento são mais rápidos**, porque repor a capacidade de eliminação pode ser urgente.

No entanto, mesmo os aumentos respeitam o intervalo mínimo de três dias simulados desde o ajuste anterior do financiamento do saneamento.

#### Como verifica o Ministério cada alteração?

Cada ajuste de financiamento usa o próprio controlo do orçamento de saneamento do SimCity 4.

Antes de aplicar uma alteração, o Ministério volta a ler o financiamento atual.

Se tiver movido o cursor manualmente desde que o plano foi calculado, cancela essa alteração pendente e recalcula a partir da nova definição.

**Depois de aplicar o financiamento, o Ministério faz duas verificações.**

**1. Verificação imediata do financiamento**

Confirma que o jogo indica a nova percentagem de financiamento.

Se o valor não tiver sido aceite, **a gestão automática do financiamento do saneamento para durante o resto da sessão atual da cidade**.

Os contratos regionais de lixo podem continuar a ser geridos, com o financiamento do saneamento já fixo.

**2. Verificação da eficiência e da capacidade**

Como a capacidade do saneamento não se atualiza imediatamente, o Ministério espera também que os cálculos do jogo se atualizem.

Verifica que:

- A eficiência indicada coincide com o valor esperado a partir da curva de financiamento, com uma tolerância de **0,001**.
- As capacidades dos edifícios de tratamento coincidem com os valores esperados, com uma tolerância de **3% ou 8 unidades de lixo**, consoante a que for maior.

**Não é feita nenhuma outra alteração do financiamento do saneamento nem ajuste de contratos de lixo até estas verificações estarem confirmadas.**

Isto evita que o Ministério tome novas decisões com base em dados do jogo parcialmente atualizados.

**O objetivo não é apenas encontrar o orçamento de saneamento mais barato, mas chegar a esse orçamento sem desestabilizar o sistema de eliminação de lixo da cidade.**
<a id="garbage-landfills-when"></a>
#### 3.8 Quando revê o Ministério o financiamento do saneamento?

O Ministério revê o financiamento do saneamento **no início de cada mês simulado**.

Pode também recalcular o orçamento mais cedo quando a situação do lixo da cidade muda de forma significativa.

Uma avaliação antecipada é desencadeada quando:

- A **produção de lixo** muda pelo menos 1%.
- A **capacidade de tratamento das incineradoras ou das centrais de conversão de lixo em energia** muda pelo menos 1%.
- A **eficiência do saneamento** muda pelo menos 1%.
- As **importações ou exportações de lixo** mudam pelo menos 1%.
- **Aparece ou desaparece lixo por recolher** na cidade.

Estas alterações são comparadas com os valores registados na avaliação anterior.

**O Ministério não repete o mesmo cálculo quando nada mudou.**

Por exemplo, se a cidade construir uma nova incineradora, o aumento da capacidade de tratamento pode desencadear uma nova avaliação sem esperar pelo mês seguinte.

Do mesmo modo, se o lixo começar a acumular-se nas ruas, o Ministério pode verificar se é preciso mais financiamento.

Cada avaliação usa as leituras mais recentes disponíveis e respeita os ajustes graduais de financiamento descritos na [secção 3.7](#garbage-landfills-writing).

<a id="garbage-landfills-advice"></a>
#### 3.9 Aviso sobre o tamanho do aterro: quando ampliar ou reduzir um aterro pode poupar dinheiro

Por vezes, alterar o tamanho de um aterro pode reduzir as despesas de saneamento da cidade.

Pode parecer surpreendente. Acrescentar quadrados de aterro aumenta os custos de manutenção, mas também pode permitir ao Ministério fazer funcionar o sistema de saneamento com uma percentagem de financiamento mais baixa.

Inversamente, retirar quadrados sem uso pode reduzir a manutenção sem afetar a capacidade da cidade para eliminar o lixo.

O Ministério pode analisar as duas possibilidades e recomendar uma alteração quando a poupança esperada for significativa.

**Esta funcionalidade vem desativada por predefinição.**

Pode ativá-la com:

**Avisar-me quando rever o tamanho do aterro compensar**

O Ministério procura então dois tipos de recomendações:

| Recomendação | O que significa |
|---|---|
| **AMPLIAR** | Acrescentar uma área concreta ao aterro poderia reduzir a fatura mensal total de saneamento. |
| **REDUZIR** | Retirar uma área concreta sem uso poderia reduzir os custos de manutenção sem comprometer a eliminação do lixo. |

**O Ministério nunca altera automaticamente o zonamento do aterro por causa destas recomendações.**

Limita-se a mostrar o aviso. É o jogador que decide se o segue e que faz o zonamento ou a remoção do zonamento.

O aviso também não altera o financiamento do saneamento nem os contratos regionais.

#### Que aterros podem ser analisados?

O Ministério só dá avisos sobre o tamanho quando consegue compreender com confiança a estrutura do aterro.

Exige:

- **Exatamente uma zona de aterro** na cidade.
- Uma versão do jogo suportada (**SimCity 4 Deluxe 1.1.641.0**).
- Acesso fiável aos quadrados, às células de armazenamento, às pilhas de lixo e aos custos de manutenção do aterro.
- Concordância total entre a estrutura interna do aterro e os totais indicados pelo jogo.

Verifica que a capacidade, o lixo armazenado, a manutenção, as contagens de quadrados e o conteúdo das células são coerentes.

Verifica também que todos os quadrados pertencem à zona esperada.

**Se alguma informação for incoerente, o Ministério não faz nenhuma recomendação.**

Isto evita sugestões baseadas em dados do aterro incompletos ou incorretos.

#### Como encontra o Ministério áreas de ampliação seguras?

Para uma recomendação de **AMPLIAR**, o Ministério procura áreas retangulares de terreno livre junto ao aterro existente.

Só considera ampliações para a **direita ou para baixo do aterro, nas coordenadas internas do mapa do jogo**.

Esta restrição é importante porque ampliar para cima ou para a esquerda pode deslocar as células de armazenamento internas do aterro e perturbar o lixo que nelas está armazenado.

A área proposta tem de cumprir várias condições:

- Tem de tocar no aterro existente ao longo do seu lado direito ou inferior.
- Tem de começar num canto do lado escolhido.
- Tem de ter entre **2 e 8 quadrados de profundidade**.
- Tem de ter pelo menos **2 quadrados de comprimento**, sem exceder o lado adjacente nem o limite de 48 quadrados do jogo.
- Não pode conter mais de **64 quadrados**.
- Todos os quadrados têm de estar disponíveis, dentro do mapa e ainda sem ocupação nem zonamento.
- Não pode juntar-se a outra zona de aterro.
- Não pode fazer com que o lixo armazenado seja empurrado de volta para a válvula de admissão do aterro.

O Ministério calcula como a nova forma afetaria as células de armazenamento e a capacidade do aterro.

Faz também uma **simulação com as próprias verificações de zonamento do jogo**, para confirmar que o SimCity 4 aceitaria de facto a área proposta.

Se o jogo rejeitasse a operação ou omitisse silenciosamente algum quadrado, esse candidato é descartado.

**Só são consideradas as ampliações verificadas como seguras.**

Quando várias localizações seguras têm o mesmo número de quadrados, o Ministério fica com a que oferece maior capacidade de armazenamento resultante.

#### Como encontra o Ministério áreas seguras para retirar?

Para uma recomendação de **REDUZIR**, o Ministério procura quadrados sem uso ao longo dos limites exteriores do aterro.

Considera retirar **entre 1 e 8 filas ou colunas exteriores completas**, formando uma área retangular de não mais de 64 quadrados.

A redução proposta tem de cumprir todas estas condições:

- Nenhum quadrado retirado contém uma pilha de lixo.
- Nenhuma das células de armazenamento afetadas contém lixo.
- Nenhum lixo armazenado seria devolvido à válvula de admissão.
- O aterro restante tem de continuar ligado, como uma só área.
- Tem de ficar pelo menos um **bloco de aterro de 2 × 2** completo.
- A própria verificação de remoção de zonamento do jogo tem de aceitar a operação.

**O Ministério nunca recomenda retirar quadrados que perturbariam o lixo armazenado.**

Por exemplo, um aterro grande pode ter várias filas completamente sem uso ao longo de um dos lados.

Retirá-las poderia reduzir a manutenção sem afetar o lixo já armazenado nem as necessidades de eliminação da cidade.

Mas, se retirá-las danificasse a estrutura interna de armazenamento do aterro, a sugestão é rejeitada.

#### Como calcula o Ministério as poupanças potenciais?

O Ministério considera duas fontes de poupança:

1. As alterações no custo de manutenção do aterro.
2. As alterações no financiamento do saneamento necessário depois de redimensionar o aterro.

**Alterações na manutenção do aterro**

```math
\begin{aligned}
\text{AMPLIAR:}\quad
\Delta B &= 5n_{\text{acrescentados}}-5g_{\text{reconciliados}}
\\
L' &= L+n
\\[6pt]
\text{REDUZIR:}\quad
\Delta B &= -5n
\\
L' &= L-n
\end{aligned}
```

Em que:

- **n** é o número de quadrados de aterro acrescentados ou retirados.
- **gᵣₑcₒₙcᵢₗₑd** é o número de quadrados fantasma eliminados durante a ampliação.
- **ΔB** é a variação da manutenção mensal do aterro com 100% de financiamento.
- **L** e **L′** são as contagens de quadrados do aterro antes e depois da alteração.

Os novos quadrados de aterro vazios custam **§5 cada por mês** com 100% de financiamento.

Retirar um quadrado sem uso poupa essa despesa de manutenção.

Uma ampliação pode também eliminar quadrados fantasma que já custam dinheiro, o que pode compensar parcialmente a manutenção da área recém-acrescentada.

O Ministério recalcula também a capacidade de armazenamento do aterro:

```math
C'=\mathrm{round}\left(\frac{T'\times30000}{N'}\right)
\qquad
T'_{\mathrm{cap}}=N'\times C'
```

Em que **T′** é a contagem resultante de quadrados pertencentes à zona e **N′** é o número resultante de células de armazenamento ativas.

O novo custo mensal nominal do departamento de saneamento é:

```math
I'=I+\Delta B
```

**O mais importante é o que acontece a seguir.**

O Ministério não se limita a multiplicar por §5 o número de quadrados acrescentados ou retirados.

Recalcula também **o financiamento de saneamento mais barato necessário depois da alteração proposta**, usando a mesma previsão de lixo, a mesma reserva, os mesmos contratos e o mesmo calendário.

**Poupança mensal estimada**

```math
\begin{aligned}
E_0 &= \mathrm{round}\left(I\times\frac{p_0}{100}\right)\\
E_1 &= \mathrm{round}\left(I'\times\frac{p_1}{100}\right)\\
\text{Poupança} &= E_0-E_1
\end{aligned}
```

Em que:

- **E₀** é a despesa mensal da disposição original do aterro.
- **E₁** é a despesa mensal depois da alteração proposta.
- **p₀** é o financiamento que o Ministério escolheria para o aterro atual.
- **p₁** é o financiamento que escolheria depois da alteração proposta.

Os dois níveis de financiamento são calculados pelo optimizador de saneamento.

**A comparação usa o financiamento que o Ministério escolheria hoje, e não necessariamente o financiamento que aparece neste momento no cursor do seu orçamento.**

Isto é importante porque as reduções do saneamento são graduais. Uma redução por concluir não pode ser confundida com uma poupança causada pelo redimensionamento do aterro.

Uma alteração proposta só é aceite para aviso quando o plano de saneamento resultante continuar a conseguir eliminar o lixo necessário, incluindo o mês seguinte quando essa verificação adicional está disponível.

O cálculo usa os custos de manutenção no momento da operação de zonamento proposta. Os aumentos futuros causados por novas pilhas de lixo não são incluídos, e a despesa única do zonamento não conta como parte da poupança mensal.

#### Quanto tem de poupar uma alteração para o Ministério a recomendar?

Pode configurar o **Limiar de poupança dos avisos do aterro**.

O valor predefinido é **25%**, ajustável de **20% a 50%**.

O Ministério só recomenda uma alteração quando a poupança mensal esperada é **estritamente superior ao seu limiar**.

**Limiar do aviso**

```math
\text{Poupança}\times100>\theta\times E_0
```

Em que **θ** é o limiar de poupança que configurou.

A percentagem mostrada no aviso é arredondada por defeito:

```math
\text{Poupança mostrada (\%)}=
\left\lfloor\frac{100\times\text{Poupança}}{E_0}\right\rfloor
```

**Exemplo: um orçamento de saneamento de §1.000 por mês**

Com o limiar predefinido de 25%:

| Poupança mensal esperada | Percentagem de poupança | Recomendação |
|---|---:|---|
| §200 | 20% | Não |
| §250 | 25% | Não |
| **§251** | **25,1%** | **Sim** |
| §350 | 35% | Sim |

**Uma poupança de exatamente 25% não desencadeia uma recomendação.**

A poupança tem de exceder 25%, e não apenas atingi-lo.

#### Com que frequência analisa o Ministério o tamanho do aterro?

O aviso sobre o tamanho do aterro é intencionalmente conservador.

O Ministério evita reconsiderar repetidamente as suas decisões de zonamento ou recomendar alterações enquanto um aterro ainda está a assentar na nova configuração.

Aplicam-se três regras de calendário:

| Regra | O que significa |
|---|---|
| **No máximo uma vez por mês simulado** | O Ministério não analisa repetidamente o tamanho dos aterros durante o mesmo mês. |
| **Período de proteção de 24 meses** | Não analisa um aterro durante 24 meses simulados depois de certas alterações ou quando o seu historial é incerto. |
| **Intervalo de 12 meses entre avisos** | Depois de emitir uma recomendação, espera pelo menos 12 meses simulados antes de analisar a cidade à procura de outra. |

**O período de proteção de 24 meses**

O Ministério espera 24 meses simulados depois de:

- Ampliar ou reduzir um aterro.
- Encontrar pela primeira vez uma cidade que já tem um aterro, porque não sabe quando esse aterro foi criado nem quando foi alterado pela última vez.
- Carregar uma gravação mais antiga e o calendário do jogo recuar.

Isto evita avisos prematuros enquanto a manutenção do aterro, a distribuição do lixo e os cálculos internos de armazenamento ainda estão a mudar.

**O intervalo de 12 meses entre avisos**

Depois de dar uma recomendação, o Ministério espera 12 meses simulados antes de voltar a verificar.

Se a mesma oportunidade continuar a exceder o seu limiar, pode ser recomendada outra vez.

Desligar e voltar a ligar a opção, ou alterar o limiar de poupança, **não repõe estes períodos de espera**.

O Ministério regista as datas necessárias e a informação do aterro separadamente para cada cidade, pelo que os períodos de espera se mantêm quando fecha e volta a abrir o jogo.

As reparações automáticas de quadrados fantasma não reiniciam o período de proteção de 24 meses.

#### Como mantém o Ministério a análise rápida?

Analisar possíveis alterações de zonamento implica verificar muitas formas possíveis do aterro.

Para não tornar o SimCity 4 mais lento, o Ministério limita o trabalho feito em cada análise.

- Avalia **no máximo 64 candidatos** por análise.
- Tem como objetivo um orçamento de cálculo de **10 milissegundos**.
- Divide inicialmente as avaliações de candidatos entre AMPLIAR e REDUZIR.
- Se uma direção tiver menos candidatos, a margem de avaliação que sobra pode ser usada pela outra.
- Se análises repetidas excederem o orçamento de tempo, o Ministério reduz progressivamente o número de candidatos que verifica.

O número máximo de candidatos pode descer temporariamente por **48, 32, 16, 8, 4 e 2**, consoante o desempenho.

Depois de seis análises consecutivas dentro do orçamento de tempo, o limite pode voltar a subir um degrau.

O Ministério verifica também tamanhos de candidatos mais pequenos antes de recomendar os maiores, porque a poupança não aumenta necessariamente com o tamanho do aterro.

**Uma consequência importante:** só recomenda um tamanho depois de avaliar todos os tamanhos permitidos mais pequenos. Isto ajuda a não deixar escapar uma alteração mais pequena e melhor.

Entre os candidatos seguros que avalia, o Ministério seleciona:

1. O candidato com a maior poupança mensal.
2. Se as poupanças forem iguais, o candidato que usa menos quadrados.
3. Se ainda houver empate, AMPLIAR antes de REDUZIR.

A análise é feita como parte de uma avaliação normal do saneamento. Não exige nenhum processo separado em segundo plano.

#### Qual é o aspeto da recomendação?

O Ministério envia uma mensagem através do **conselheiro de serviços públicos** do SimCity 4, classificada como boa notícia.

São possíveis dois tipos de avisos:

**Para a ampliação:**

> **Mais aterro, menos despesa?**

**Para a redução:**

> **O aterro ocupa espaço a mais!**

O aviso explica:

- Se deve ampliar ou reduzir o aterro.
- Quantos quadrados devem ser acrescentados ou retirados.
- O comprimento e a profundidade do retângulo proposto.
- A poupança mensal estimada em simoleões.
- A percentagem do orçamento de saneamento que poderia ser poupada.

Inclui também uma ligação clicável **ver onde**.

Ao selecionar a ligação, a câmara move-se para o centro da área sugerida.

A mensagem evita dizer «esquerda», «direita», «cima» ou «baixo», porque a direção que vê no ecrã depende de como rodou a vista da cidade.

**O Ministério só regista que emitiu um aviso depois de o jogo aceitar com sucesso o aviso do conselheiro.**

Lembre-se: **a recomendação é meramente informativa. É o jogador que decide se altera o aterro.**

<a id="ghost-tiles"></a>
#### 3.10 Quadrados fantasma: reparação automática

Os quadrados de aterro fantasma são diferentes de um aterro sobredimensionado ou ineficiente.

São **um erro do SimCity 4**, não uma decisão de zonamento.

Como explicado antes, um quadrado fantasma é um local que o jogo continua a contar como parte de um aterro, apesar de já não estar de facto zonado como aterro.

Isto pode criar despesas de manutenção desnecessárias.

**O Ministério pode reparar automaticamente estes quadrados fantasma quando gere o saneamento.**

Ao contrário do aviso sobre o tamanho do aterro, esta reparação:

- Não exige uma definição própria.
- Não depende de um limiar de poupança.
- Não gera nenhum aviso do conselheiro.
- Não acrescenta nem retira nenhum quadrado de aterro legítimo.
- Não demole edifícios nem altera o zonamento visível.
- Nunca reduz deliberadamente a capacidade de armazenamento do aterro.

A atividade de reparação fica registada no ficheiro de registo.

#### Como pode o Ministério reparar um quadrado fantasma?

A reparação usa um comportamento particular do sistema de zonamento do SimCity 4.

**Voltar a aplicar o zonamento de aterro a quadrados que já estão legitimamente zonados como aterro faz com que o jogo reconstrua a estrutura interna da zona.**

Esta operação não custa dinheiro nem altera os tipos de zona desses quadrados, mas a reconstrução elimina também a pertença obsoleta dos quadrados fantasma.

Na prática, o Ministério pede ao jogo que reconstrua um aterro segundo as suas próprias regras de zonamento, sem alterar a área legítima do aterro.

#### Como funciona a reparação automática?

O Ministério segue quatro passos.

**1. Detetar os quadrados fantasma**

Durante a primeira avaliação do saneamento de cada mês simulado, verifica as zonas de aterro disponíveis.

Depois de abrir uma cidade, esta verificação ocorre o mais tardar na primeira avaliação mensal do saneamento, desde que a informação necessária esteja disponível.

Usa a mesma informação do aterro, validada e só de leitura, que é exigida para o aviso sobre o tamanho.

Se os totais indicados pelo jogo e os dados internos do aterro não coincidirem, o Ministério não tenta nenhuma reparação.

**2. Verificar que a reparação é segura**

O Ministério escolhe uma região que contenha quadrados de aterro legítimos sem pilhas de lixo.

Usa as dimensões de zonamento suportadas pelo jogo e verifica o resultado esperado antes de fazer qualquer alteração.

A reparação é recusada se puder:

- Acrescentar novos quadrados de aterro.
- Devolver lixo armazenado à válvula de admissão do aterro.
- Reduzir a capacidade total de armazenamento do aterro.
- Juntar duas zonas de aterro separadas.
- Produzir um resultado de capacidade incerto por causa de arredondamentos.
- Afetar uma área que as verificações de zonamento do jogo rejeitariam.

Recusa também a reparação se não houver quadrados de aterro adequados e sem pilhas.

A região de reparação selecionada tem de caber no **limite de zonamento de 48 × 48** do jogo.

**3. Executar a reparação**

Quando as verificações têm êxito, o Ministério usa a própria operação de zonamento do SimCity 4 para voltar a aplicar o zonamento de aterro à região selecionada.

A operação não pode omitir silenciosamente nenhum quadrado.

Se a chamada de zonamento falhar ou o jogo rejeitar parte da região, a tentativa é considerada falhada.

**4. Verificar o resultado**

O jogo processa a operação de zonamento depois de receber o pedido.

Por isso, o Ministério verifica o resultado nas avaliações seguintes, com até **quatro verificações de confirmação**.

Verifica que:

- A caixa delimitadora do aterro não mudou.
- Os quadrados fantasma desapareceram.
- A disposição das células de armazenamento e a capacidade coincidem com o resultado previsto.
- O número de quadrados de aterro legítimos continua igual.
- A capacidade de aterro da cidade coincide com o valor esperado.
- A informação interna do aterro continua a concordar com os totais do jogo.

A quantidade de lixo armazenado muda naturalmente à medida que o jogo decorre, pelo que essa quantidade é registada e não se exige que se mantenha idêntica.

**A reparação só é aceite quando a estrutura do aterro resultante coincide com o resultado seguro esperado.**

#### O que acontece se uma reparação falhar?

O Ministério não repete tentativas de reparação arriscadas.

Cada estado distinto de quadrados fantasma é tentado **no máximo uma vez por sessão de cidade**.

Se a operação de zonamento falhar ou a estrutura resultante não coincidir com a previsão, **a reparação automática de quadrados fantasma fica desativada até o SimCity 4 ser fechado**.

As outras funções do Ministério podem continuar a funcionar.

O Ministério repara também **no máximo uma zona de aterro por mês simulado**.

Se várias zonas tiverem quadrados fantasma, as restantes são consideradas nos meses seguintes.

Enquanto uma reparação aguarda confirmação, o aviso sobre o tamanho do aterro é adiado para não analisar uma área cuja estrutura interna ainda está a mudar.

#### Porque podem os quadrados fantasma voltar?

Infelizmente, o comportamento de gravação e carregamento do jogo original não é corrigido de forma permanente.

Quando volta a abrir uma cidade, o SimCity 4 pode recriar a pertença fantasma ao longo da última fila ou coluna da caixa delimitadora interna de um aterro.

**O Ministério pode, por isso, ter de reparar os mesmos quadrados fantasma outra vez depois de reabrir a cidade.**

Isso não significa que a reparação anterior tenha falhado. Significa que o jogo recriou o erro durante o carregamento.

Cada quadrado fantasma eliminado poupa **§5 por mês simulado com 100% de financiamento do saneamento**.

A poupança real com uma percentagem de financiamento mais baixa é correspondentemente menor.

**A reparação automática de quadrados fantasma é uma correção conservadora de um erro do jogo, não uma remodelação automática do aterro.**

<a id="garbage-landfills-safety"></a>
### 4. Condições de segurança

O Ministério aplica regras de segurança separadas ao financiamento do saneamento, ao aviso sobre o tamanho do aterro e à reparação de quadrados fantasma.

#### Financiamento automático do saneamento

- **A gestão tem de estar ativada.** Têm de estar ligadas tanto **Ministério ativo** como **Gerir o saneamento**.
- **Só são usados níveis de financiamento válidos.** O financiamento mantém-se dentro dos limites aceites pelo sistema de orçamento de saneamento do jogo.
- **O financiamento fixo é respeitado.** O Ministério não modifica um orçamento de saneamento que o jogo indique como fixo.
- **A reserva configurada é sempre incluída.** O plano de saneamento tem de dar resposta à necessidade de lixo protegida.
- **O mês projetado tem de terminar sem lixo por recolher.** Quando há previsões do mês seguinte, o mês seguinte também tem de ser viável.
- **A informação pouco fiável nunca é tratada como capacidade disponível.** Uma válvula de aterro ilegível não dá nenhuma admissão assumida, e uma exportação de lixo ilegível não dá nenhum alívio de eliminação assumido.
- **As alterações de financiamento são graduais e verificadas.** As reduções seguem as regras de ritmo, e não se tenta uma nova alteração até a anterior estar confirmada.
- **As falhas técnicas interrompem as alterações de financiamento durante a sessão.** O Ministério evita continuar com um controlo de financiamento pouco fiável.

Se **Gerir o saneamento** estiver desativado, o Ministério deixa o orçamento do saneamento como está.

Os contratos regionais de lixo podem continuar a ser geridos de forma independente, usando o financiamento atual para calcular a capacidade de eliminação disponível.

#### Aviso sobre o tamanho do aterro

**O aviso sobre o tamanho do aterro nunca altera o zonamento nem o financiamento.**

Só está disponível quando o financiamento do saneamento está a ser gerido e a informação interna do aterro pode ser verificada com exatidão.

O Ministério exige também:

- Exatamente uma zona de aterro.
- Uma alteração proposta segura que preserve o lixo armazenado.
- Nenhum risco de juntar zonas.
- Uma ampliação que não desloque as posições das células de armazenamento do aterro.
- A aprovação da própria validação de zonamento ou de remoção de zonamento do jogo.
- Poupança estritamente acima do limiar configurado.

Se estas condições não estiverem cumpridas, **não é emitido nenhum aviso**.

#### Reparação automática de quadrados fantasma

A reparação automática de quadrados fantasma só está disponível na versão suportada, **SimCity 4 Deluxe versão 1.1.641.0**, enquanto a gestão do saneamento está ativa.

Cada reparação tem de:

- Usar dados do aterro validados.
- Preservar todos os quadrados de aterro legítimos.
- Evitar alterar tipos de zona ou demolir seja o que for.
- Preservar ou aumentar a capacidade total de armazenamento do aterro.
- Evitar perturbar o lixo armazenado ou devolvê-lo à válvula de admissão.
- Evitar juntar zonas ou depender de arredondamentos incertos.
- Passar a própria validação de zonamento do jogo.
- Produzir o resultado exato esperado quando verificada depois.

Se a verificação falhar, as reparações automáticas param durante o resto da sessão de jogo.

**O Ministério prefere sempre deixar um aterro como está a fazer uma modificação que não consiga verificar como segura.**

<a id="garbage-landfills-settings"></a>
### 5. Definições

A gestão do lixo e dos aterros pode ser configurada em **Opções de Jogo → Ministério da Optimização**, na página **Serviços**.

| Definição | Predefinição | Intervalo | O que faz |
|---|---|---|---|
| [Ministério ativo](options.md#opt-master) | Ligado | Ligado / Desligado | Ativa ou desativa todo o Ministério. |
| [Gerir o saneamento](options.md#opt-sanitation) | Ligado | Ligado / Desligado | Gere automaticamente o financiamento do saneamento. É também necessária para o aviso sobre o tamanho do aterro e para a reparação de quadrados fantasma. |
| [Reserva de saneamento](options.md#opt-sanitationreserve) | 2% | 2–20% | Acrescenta capacidade de eliminação de reserva para acomodar aumentos da produção de lixo. |
| [Gerir os acordos com os vizinhos](options.md#opt-deals) | Ligado | Ligado / Desligado | Ativa a gestão automática dos contratos regionais. |
| [Gerir acordos de lixo](options.md#opt-tradegarbage) | Ligado | Ligado / Desligado | Ativa as importações e exportações automáticas de lixo quando a gestão dos acordos com os vizinhos está ativa. |
| [Avisar-me quando rever o tamanho do aterro compensar](options.md#opt-landfilladvice) | Desligado | Ligado / Desligado | Ativa as recomendações de ampliar ou reduzir áreas do aterro quando são possíveis poupanças significativas. |
| [Limiar de poupança dos avisos do aterro](options.md#opt-landfillthreshold) | 25% | 20–50% | Define a poupança percentual mínima exigida antes de o Ministério recomendar uma alteração do tamanho do aterro. |

#### Como funcionam estas opções em conjunto?

**Gerir o saneamento**

Quando está ativada, o Ministério calcula e ajusta automaticamente o orçamento do saneamento.

Se estiver desativada, o financiamento do saneamento mantém-se como está. Os contratos regionais de lixo podem continuar a ser geridos de forma independente, usando o financiamento existente.

O aviso sobre o tamanho do aterro e a reparação automática de quadrados fantasma exigem que a gestão do saneamento esteja ativa.

**Reserva de saneamento**

A reserva predefinida é **2%**.

Por exemplo, se a cidade produzir 100.000 unidades de lixo por mês depois da reciclagem, o Ministério planeia tratar pelo menos **102.000 unidades**, antes de incluir importações ou lixo por recolher já existente.

Aumentar esta reserva dá uma margem adicional contra o aumento da produção de lixo, mas pode exigir um financiamento mais elevado.

**Aviso sobre o tamanho do aterro**

Esta funcionalidade está **desligada por predefinição**.

Quando está ativada, o Ministério pode recomendar:

- **AMPLIAR:** acrescentar quadrados de aterro se a capacidade de eliminação adicional permitir uma redução suficientemente grande das despesas de saneamento.
- **REDUZIR:** retirar quadrados de aterro sem uso se isso baixar os custos de manutenção sem comprometer a capacidade de eliminação.

**Estas recomendações nunca alteram automaticamente o zonamento do aterro.**

A opção de aviso não está disponível na janela de definições enquanto o Ministério ou a gestão do saneamento estiverem desativados, mas o valor guardado é preservado.

**Limiar de poupança dos avisos do aterro**

O limiar predefinido é **25%**.

Uma alteração proposta ao aterro tem de poupar **mais de 25%** da despesa mensal de saneamento optimizada antes de ser emitido um aviso do conselheiro.

A definição do limiar não está disponível enquanto o aviso sobre o tamanho do aterro estiver desativado, mas o seu valor é preservado.

#### Definições avançadas (ficheiro INI)

Estas opções ficam também guardadas no ficheiro de configuração de infraestruturas do Ministério:

| Chave INI | Finalidade |
|---|---|
| `AutoGarbage` | Financiamento automático do saneamento |
| `GarbageReservePercent` | Reserva de segurança do saneamento |
| `AutoTradeGarbage` | Contratos automáticos de lixo |
| `WarnLandfillSizeSavings` | Aviso de ampliação e redução do aterro |
| `LandfillSavingsAlertThreshold` | Poupança mínima exigida para o aviso |

**A reparação automática de quadrados fantasma não tem uma definição própria.** Funciona como parte da gestão do saneamento, na versão do jogo suportada, sempre que as verificações de segurança necessárias passam.

<a id="garbage-landfills-examples"></a>
### 6. Exemplos práticos

Os exemplos seguintes mostram, na prática, como funcionam o financiamento do saneamento, o armazenamento nos aterros e as regras de segurança do Ministério.

Vários exemplos usam uma cidade de teste onde foi medido o comportamento dos aterros do jogo.

#### Exemplo 1: Um aterro com muito armazenamento, mas com entrada mensal limitada

Considere-se um aterro com:

- **48 quadrados de aterro.**
- **Financiamento do saneamento:** 50%.
- **Eficiência do saneamento:** 25%.
- **Armazenamento disponível:** 900.000 unidades de lixo.
- **Lixo já aceite este mês:** 41.000 unidades.
- **Data atual:** dia 20 do mês simulado.

**Quanto lixo pode ainda aceitar?**

Primeiro, o Ministério calcula o limite mensal de entrada:

```math
5000 \times 48 \times 0.25 = 60000
```

O aterro pode aceitar aproximadamente **60.000 unidades durante o mês inteiro**.

No entanto, já entraram 41.000 unidades na válvula do aterro.

```math
60000-41000=19000
```

**Só restam 19.000 unidades de capacidade de entrada para o mês**, apesar de o aterro ter 900.000 unidades de armazenamento livre.

Imagine-se agora que o financiamento do saneamento sobe para **61%**.

Com esse financiamento, a eficiência passa a ser 40,125%.

```math
5000\times48\times0.40125=96300
```

A entrada mensal restante passa a ser:

```math
96300-41000=55300
```

| Financiamento | Limite mensal de entrada | Entrada restante |
|---|---:|---:|
| 50% | 60.000 | 19.000 |
| **61%** | **96.300** | **55.300** |

Depois de a eficiência do jogo ser atualizada, o financiamento mais elevado proporciona **36.300 unidades adicionais de entrada mensal**.

**A distinção importante:** um aterro pode ter uma enorme capacidade de armazenamento e, mesmo assim, não conseguir aceitar mais lixo porque o seu limite mensal de entrada foi atingido.

É por isso que o Ministério considera tanto o armazenamento como a entrada ao escolher o financiamento do saneamento.

#### Exemplo 2: Contratos de lixo logo após a mudança de mês

Imagine-se que a cidade tem um contrato para exportar **7.000 unidades de lixo por mês**.

O jogo exporta normalmente:

```math
7000\times0.10=700
```

Ou seja, **700 unidades por dia simulado**, durante cerca de dez dias.

No entanto, o SimCity 4 pode mostrar temporariamente a utilização do contrato do mês anterior depois de o calendário mudar.

**No primeiro dia de um novo mês**, o jogo pode ainda mostrar:

*Quantidade utilizada: 7.000*

Se o Ministério aceitasse esse valor, assumiria erradamente que toda a quota de exportação do novo mês já tinha sido esgotada.

Em vez disso, verifica se a quantidade indicada poderia realisticamente ter sido entregue desde o início do mês.

No dia 1:

```math
7000>(1-1)\times700
```

A utilização indicada é impossível para o novo mês, pelo que **o Ministério a trata como zero**.

Planeia então a exportação das 7.000 unidades ao longo dos dias seguintes.

**O que acontece no dia 4?**

Suponha-se que o jogo indica 2.100 unidades utilizadas.

```math
2100\le(4-1)\times700
```

Esse valor é coerente com três entregas diárias concluídas.

**O Ministério aceita a leitura de 2.100 unidades.**

Esta proteção evita que estatísticas de contratos desatualizadas provoquem cálculos de saneamento incorretos durante os três primeiros dias de um mês.

#### Exemplo 3: Como desaparece o lixo armazenado ao longo do tempo

Este exemplo provém da **cidade de teste**.

O aterro tem:

- **Capacidade por célula de armazenamento:** 42.000 unidades.
- **Total de lixo armazenado:** 985.887 unidades.

O SimCity 4 armazena o lixo de forma compacta, enchendo as células antes de começar novas.

O lixo armazenado ocupa, portanto:

- **23 células completamente cheias:** 966.000 unidades.
- **Uma célula parcialmente preenchida:** 19.887 unidades.

No início de cada mês simulado, o jogo remove parte do lixo armazenado através do seu mecanismo de decomposição.

**Decomposição das células cheias**

Cada célula cheia perde no máximo 1.500 unidades por mês.

```math
23\times1500=34500
```

**Decomposição da célula parcialmente preenchida**

O jogo remove aproximadamente 7% do seu conteúdo:

```math
19887\times0.07=1392.09
```

Arredondado à unidade inteira mais próxima, dá **1.392 unidades**.

**Decomposição mensal total**

```math
D(U)=34500+1392=35892
```

<text strong>Resultado: 35.892 unidades de lixo desaparecem do armazenamento durante a atualização mensal.</text>

Isto acontece através da mecânica normal dos aterros do jogo, sem qualquer intervenção do Ministério.

**Para que serve isto?**

O armazenamento dos aterros não é totalmente permanente. Todos os meses desaparece algum lixo armazenado, libertando capacidade gradualmente.

O Ministério usa este comportamento para prever quanto lixo um aterro consegue conter ao longo do tempo.

#### Exemplo 4: Reparar quadrados fantasma na cidade de teste

Na cidade de teste, o aterro continha **sete quadrados fantasma**.

Eram quadrados que o jogo continuava a contar como parte do aterro, embora já não estivessem realmente zonados como aterro.

A estrutura registada do aterro era a seguinte:

| Propriedade | Antes da reparação |
|---|---:|
| Retângulo envolvente | 6 × 9 quadrados |
| Posições possíveis de células de armazenamento | 40 |
| Células de armazenamento ativas | 30 |
| Pertença registada ao aterro | 49 quadrados |
| Quadrados de aterro reais | 42 |
| **Quadrados fantasma** | **7** |
| Capacidade de armazenamento por célula ativa | 42.000 |
| Capacidade total de armazenamento | 1.260.000 |
| Manutenção mensal com 100% de financiamento | §1.030 |

Os sete fantasmas estavam todos ao longo da última linha ou coluna do retângulo interno do aterro.

**O que calculou o Ministério?**

Remover os sete fantasmas deixaria:

- 42 quadrados de aterro legítimos.
- 30 células de armazenamento ativas.

A capacidade resultante por célula seria:

```math
C'=\frac{42\times30000}{30}=42000
```

Capacidade total:

```math
30\times42000=1260000
```

**A capacidade de armazenamento permaneceria exatamente igual.**

Nada seria empurrado de volta para a válvula de entrada do aterro.

Cada quadrado fantasma custava ainda §5 por mês com 100% de financiamento.

```math
7\times5=35
```

**Poupança mensal de manutenção esperada: §35.**

O Ministério efetuou a reparação automática através do sistema de zonamento do jogo.

**Resultados observados**

| Propriedade | Antes da reparação | Depois da reparação |
|---|---:|---:|
| Quadrados fantasma | 7 | **0** |
| Manutenção mensal | §1.030 | **§995** |
| Capacidade de armazenamento | 1.260.000 | **1.260.000** |

Todos os resultados coincidiram com a previsão do Ministério.

**Mas os fantasmas voltaram depois de reabrir a cidade.**

Uma cópia da cidade de teste gravada depois da reparação recriou os mesmos sete quadrados fantasma quando foi carregada de novo.

Isto confirmou o erro de gravação/carregamento do jogo descrito anteriormente.

O Ministério conseguiu repará-los outra vez durante a nova sessão da cidade.

**A reparação funciona, mas o jogo original pode recriar o problema sempre que a cidade é carregada.**

#### Exemplo 5: Uma ampliação que poupa dinheiro, mas não o suficiente para ser recomendada

O Ministério testou também a ampliação do aterro da cidade de teste.

Inicialmente, passava uma rua ao longo do seu limite direito.

As verificações de zonamento do jogo rejeitaram todos os candidatos de ampliação nesse lado, porque o terreno proposto estava ocupado.

**O Ministério não recomendou nem executou nenhuma ampliação.**

Depois de o troço de rua ser removido, passou a ser possível uma **ampliação de 2 × 2**.

Isto acrescentaria quatro quadrados de aterro.

A operação de zonamento removeria também os sete quadrados fantasma existentes.

**Nova estrutura do aterro**

```math
T'=49+4-7=46
```

O aterro ampliado teria 32 células de armazenamento ativas.

```math
C'=\frac{46\times30000}{32}=43125
```

Capacidade total de armazenamento:

```math
32\times43125=1380000
```

A capacidade do aterro aumentaria de **1.260.000 para 1.380.000 unidades**, sem empurrar de volta para a válvula qualquer lixo armazenado.

**E a manutenção?**

Os quatro novos quadrados vazios custariam:

```math
4\times5=20
```

Remover os sete fantasmas pouparia:

```math
7\times5=35
```

Variação líquida da manutenção:

```math
\Delta B=20-35=-15
```

A manutenção mensal nominal do aterro desceria de **§1.030 para §1.015**, apesar de o aterro ter ficado maior.

A ampliação foi realizada durante os testes e os valores observados no jogo coincidiram com as previsões.

**O Ministério recomendá-la-ia?**

Não.

A redução estimada da fatura total do saneamento era de apenas cerca de **4–5%**.

O limiar de aviso mínimo configurável é 20% e o predefinido é 25%.

**A ampliação era segura e poderia poupar dinheiro, mas a poupança era demasiado pequena para justificar um aviso do conselheiro.**

Isto ilustra por que razão o Ministério não recomenda todas as alterações de zonamento tecnicamente vantajosas.

#### Exemplo 6: Por que razão o Ministério recusa por vezes reparar fantasmas

Uma experiência diferente na cidade de teste mostrou por que motivo a reparação de quadrados fantasma tem de ser conservadora.

O aterro original tinha:

- **49 quadrados registados**, incluindo sete fantasmas.
- **30 células de armazenamento ativas.**
- **Capacidade total:** 1.260.000 unidades.

Foi removida uma linha exterior vazia com cinco quadrados de aterro legítimos, enquanto os fantasmas ainda lá estavam.

Depois dessa operação:

- A pertença registada desceu para **44 quadrados**.
- As células de armazenamento ativas desceram para **25**.

A nova capacidade por célula passou a ser:

```math
C'=\frac{44\times30000}{25}=52800
```

Capacidade total de armazenamento:

```math
25\times52800=1320000
```

**Surpreendentemente, a capacidade total do aterro aumentou para 1.320.000 unidades**, apesar de terem sido removidos cinco quadrados.

Isto aconteceu porque os quadrados fantasma continuavam a contar para a pertença registada usada no cálculo da capacidade.

**O que aconteceria se o Ministério removesse a seguir os sete fantasmas?**

A pertença registada desceria de 44 para 37 quadrados.

Com as mesmas 25 células ativas:

```math
C'=\frac{37\times30000}{25}=44400
```

A capacidade de cada célula desceria de 52.800 para 44.400 unidades.

O modelo do Ministério previu que **151.200 unidades de lixo armazenado seriam empurradas de volta para a válvula de entrada do aterro**.

Isso perturbaria o sistema de saneamento.

**O Ministério recusou, por isso, a reparação dos fantasmas e deixou o aterro inalterado.**

Este é um importante princípio de segurança:

**Só é permitido remover quadrados fantasma quando o Ministério consegue provar que a operação não reduzirá a capacidade de armazenamento nem perturbará o lixo existente.**

Poupar alguns simoleões não compensa o risco de comprometer os resíduos armazenados num aterro.

#### Exemplo 7: Quando aparece um aviso de poupança no aterro?

Suponha-se que o Ministério calcula que o orçamento de saneamento atual mais barato é de:

**§1.015 por mês.**

Encontra então uma alteração segura do tamanho do aterro que reduziria a despesa optimizada para:

**§760 por mês.**

A poupança esperada é:

```math
1015-760=255
```

Ou seja, **§255 poupados em cada mês simulado**.

Com o limiar de aviso predefinido de 25%, o Ministério verifica:

```math
255\times100>25\times1015
```

```math
25500>25375
```

A condição é satisfeita.

**O Ministério emite uma recomendação.**

A poupança real é de aproximadamente **25,12%**, embora o aviso apresente **25%** porque a percentagem é arredondada por defeito.

| Limiar de aviso | Resultado |
|---|---|
| 20% | Recomendação emitida |
| **25% (predefinição)** | **Recomendação emitida** |
| 30% | Sem recomendação |

**Um pormenor importante:** a comparação usa a poupança real, antes de arredondar a percentagem apresentada.

É por isso que um aviso pode mostrar 25% mesmo que a regra exija uma poupança estritamente superior a 25%.

<a id="garbage-landfills-limitations"></a>
### 7. Limitações conhecidas

O modelo de saneamento do Ministério baseia-se em observações detalhadas do SimCity 4, mas subsistem várias limitações.

#### Produção e financiamento do lixo

- **A produção futura de lixo é estimada.** O Ministério usa a produção do mês anterior menos a reciclagem e assume que a quantidade resultante é produzida de forma uniforme ao longo dos dias restantes do mês atual e do seguinte. Não prevê o crescimento futuro da cidade. As alterações são incorporadas quando surge informação nova.
- **O financiamento pode mudar durante o mês.** As importações e exportações de lixo e a entrada nos aterros não seguem um calendário perfeitamente uniforme. O nível de financiamento mais barato pode, por isso, mudar à medida que o mês avança. Na cidade de teste, as decisões observadas incluíram **47%, 63% e 53%** em momentos diferentes. Este é um comportamento esperado, não necessariamente uma falha.
- **O Ministério usa as curvas de saneamento do jogo original.** As suas previsões assumem a curva de eficiência padrão, uma entrada nos aterros de 5.000 unidades por quadrado e por mês, um armazenamento de aproximadamente 30.000 unidades por quadrado, as regras de decomposição padrão e uma manutenção de §5 para os quadrados vazios. Mods que alterem estas mecânicas podem tornar os cálculos imprecisos. Se a eficiência do jogo não coincidir com o valor esperado, o Ministério deixa de fazer alterações de financiamento em vez de continuar com dados pouco fiáveis.

#### Previsão do mês seguinte

- **A verificação do mês seguinte exige uma geometria do aterro calibrada.** Normalmente precisa de uma única zona de aterro e de uma observação mensal com pelo menos uma célula de armazenamento cheia.
- **A calibração não é permanente.** O Ministério volta a aprender a geometria depois de a cidade ser reaberta e recomeça a calibração quando a capacidade do aterro muda.
- **A previsão assume que o financiamento selecionado se mantém.** Não antecipa todas as alterações futuras na produção de lixo ou nos contratos regionais.
- **O modelo da válvula do aterro tem uma limitação.** Quando a válvula já está cheia, parte do lixo excedente pode não ser incluída na estimativa do mês seguinte. Nessas situações, o Ministério pode subestimar a quantidade de lixo que fica na válvula na mudança de mês.

Se não estiver disponível uma calibração fiável, o Ministério usa apenas a sua projeção do mês atual.

#### Aviso sobre o tamanho do aterro

- **Só as cidades com exatamente uma zona de aterro são elegíveis.** O Ministério não dá recomendações de AMPLIAR ou REDUZIR quando existem várias zonas de aterro.
- **As áreas de ampliação são restritas.** Só são consideradas áreas seguras ao longo dos limites direito ou inferior das coordenadas internas do mapa do aterro, com uma profundidade máxima de oito quadrados e uma área máxima de 64 quadrados.
- **As reduções limitam-se a linhas ou colunas exteriores não utilizadas.** As reduções propostas têm de preservar a estrutura do aterro restante e o lixo armazenado.
- **As poupanças são estimativas.** Os novos quadrados de aterro são tratados inicialmente como vazios, custando §5 cada um com 100% de financiamento. A sua manutenção pode aumentar mais tarde, quando surgirem montes de lixo.
- **Os custos pontuais de zonamento não estão incluídos.** O Ministério compara os custos mensais recorrentes do saneamento, não o custo inicial de alterar a zona.
- **Nas cidades testadas, as grandes poupanças são raras.** As oportunidades observadas ficaram, em geral, bastante abaixo do limiar mínimo de aviso de 20%, pelo que o aviso sobre o tamanho do aterro pode aparecer raramente.

#### Reparação de quadrados fantasma

- **Alguns quadrados fantasma regressam sempre que a cidade é carregada.** O erro de gravação/carregamento do jogo original pode recriá-los ao longo da última linha ou coluna do retângulo interno de um aterro.
- **A reparação não consegue corrigir permanentemente o formato de gravação do jogo.** O Ministério pode ter de efetuar a mesma correção segura em cada nova sessão.
- **As reparações de fantasmas inseguras são recusadas.** Um quadrado fantasma fica intacto se a sua remoção puder reduzir a capacidade do aterro, perturbar o lixo armazenado ou empurrar resíduos de volta para a válvula de entrada.
- **Deixar um fantasma pode continuar a custar dinheiro.** Cada fantasma por reparar continua a contribuir com §5 por mês com 100% de financiamento do saneamento.
- **As várias zonas de aterro são tratadas em separado.** O Ministério pode tentar reparações seguras de fantasmas, uma zona de cada vez, mas normalmente não pode usar a previsão do mês seguinte baseada numa geometria única nem dar avisos sobre o tamanho do aterro.

#### A remoção oculta de lixo do jogo

O SimCity 4 pode remover silenciosamente uma quantidade limitada de lixo através do seu mecanismo integrado de *abduction* (rapto de lixo).

**O Ministério não trata essa remoção como uma falha do saneamento e não tenta impedi-la.**

O seu objetivo é evitar acumulações genuínas e persistentes de lixo por recolher, e não alterar todas as regras ocultas de eliminação do jogo original.

**O Ministério dá prioridade à eliminação segura do lixo e a decisões de financiamento fiáveis. Quando não consegue prever com confiança suficiente uma operação num aterro ou uma alteração do saneamento, deixa a situação existente inalterada.**

[Voltar ao índice](index.md#contents) | [Voltar ao README](../../README.pt.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
