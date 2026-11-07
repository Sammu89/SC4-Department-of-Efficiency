[English](../en/water.md) | **Português** | [Français](../fr/water.md) | [Español](../es/water.md)

[Voltar ao índice](index.md#contents) | [Voltar ao README](../../README.pt.md)

<a id="water"></a>
## C. Água

O Ministério gere automaticamente o **orçamento de abastecimento de água** da cidade, garantindo que há água suficiente para os edifícios e mantendo os custos operacionais o mais baixos possível.

Tem em conta:

- A quantidade de água de que a cidade precisa.
- A reserva de segurança escolhida (**10% por predefinição**).
- A água que a cidade compra ou vende às cidades vizinhas.
- A capacidade de produção das instalações de água.
- O estado da rede de canalizações.

Ao contrário da eletricidade, em que cada central tem a sua própria definição de financiamento, **todo o abastecimento de água partilha uma única percentagem de orçamento**.

O Ministério ajusta essa percentagem para fornecer água suficiente sem gastos desnecessários.

Pode também proteger as canalizações da deterioração causada por um financiamento insuficiente.

Os contratos regionais de água estão incluídos nos cálculos. A sua gestão automática é explicada em [Acordos com os Vizinhos](deals.md#neighbor-deals).

<a id="water-vanilla"></a>
### 1. Como funciona a água no SimCity 4

#### Um orçamento para todo o abastecimento de água

O SimCity 4 gere o financiamento da água através de um único orçamento de departamento.

A cidade pode ter várias instalações de água, incluindo bombas, torres de água e estações de tratamento. Cada instalação tem o seu próprio custo de manutenção, mas **todas as instalações recebem a mesma percentagem de financiamento**.

Por exemplo, se definir o financiamento da água em 80%, essa percentagem aplica-se a todo o departamento da água.

Não é possível financiar uma bomba a 100% e outra a 60%, como se faz com as centrais elétricas.

Normalmente o jogo permite um financiamento da água até **120%**, embora possa aplicar-se um máximo inferior se o jogo o indicar.

Ao contrário da eletricidade, a água não tem uma percentagem mínima de financiamento aceitável incorporada nem um mecanismo de greve.

#### Procura, produção e consumo de água

O jogo acompanha vários valores relacionados com a água:

| Termo | O que significa |
|---|---|
| **Procura** | A quantidade de água de que os edifícios da cidade precisam. |
| **Capacidade de produção** | A quantidade de água que as instalações da própria cidade conseguem fornecer com o financiamento atual. |
| **Consumo** | A água efetivamente entregue aos edifícios. |
| **Importações** | Água comprada às cidades vizinhas. |
| **Exportações** | Água vendida às cidades vizinhas. |
| **Edifícios com água** | Edifícios que estão a receber água. |
| **Edifícios sem água** | Edifícios sem serviço de água. |
| **Canalizações degradadas** | Canalizações que se deterioraram. |
| **Canalizações rebentadas** | Canalizações que falharam. |

**Como afetam os contratos regionais de água o abastecimento?**

Nas nossas observações, o SimCity 4 usa a água importada antes de recorrer à produção local.

Isto significa que comprar água a uma cidade vizinha pode reduzir a quantidade que as instalações da própria cidade precisam de produzir.

Vender água tem o efeito oposto: **a cidade tem de ter capacidade suficiente para abastecer tanto os seus próprios edifícios como os seus contratos de exportação**.

O Ministério tem estes contratos em conta ao calcular o financiamento necessário.

**Um pormenor importante ao abrir uma cidade**

Logo depois de carregar uma cidade, o jogo pode indicar zero edifícios com água e zero edifícios sem água.

Isto não significa necessariamente que todos os edifícios tenham água nem que a cidade não tenha procura de água.

Normalmente significa que a simulação da água ainda não atualizou a sua informação.

Por isso, o Ministério tem de distinguir entre leituras iniciais em falta e valores genuínos antes de tomar decisões de financiamento.

<a id="water-efficiency-curve"></a>
#### Como o financiamento afeta a produção de água

No SimCity 4, reduzir o financiamento da água reduz também a quantidade de água que as instalações conseguem produzir.

No entanto, **a produção não diminui ao mesmo ritmo que o financiamento**.

O jogo usa a seguinte curva de eficiência:

| Financiamento da água | Eficiência de produção |
|---:|---:|
| 0% | 0% |
| 50% | 30% |
| 90% | 80% |
| 100% | 100% |
| 120% | 110% |

**O que significa isto?**

- Com **50% de financiamento**, a cidade tem apenas **30% da sua capacidade normal de produção de água**.
- Com **90% de financiamento**, tem **80% da capacidade normal**.
- Com **100% de financiamento**, tem a capacidade normal completa.
- Com **120% de financiamento**, tem **110% da capacidade normal**.

Um financiamento acima de 100% fornece, portanto, água adicional, mas a um custo mais elevado por unidade de capacidade.

Da mesma forma, reduzir demasiado o financiamento pode eliminar muito mais capacidade de produção do que seria de esperar.

**Como são calculadas as percentagens intermédias?**

Entre os pontos de controlo do jogo, a eficiência varia em linha reta.

Por exemplo, entre 90% e 100% de financiamento, a eficiência sobe de 80% para 100%.

Com 95% de financiamento, a eficiência de produção é, portanto, de 90%.

**Interpolação da curva de eficiência**

```math
y(x) = y_i + \frac{x-x_i}{x_{i+1}-x_i}(y_{i+1}-y_i)
\qquad \text{for } x_i \le x \le x_{i+1}
```

Onde:

- **x** é a percentagem de financiamento escolhida.
- **(xᵢ, yᵢ)** e **(xᵢ₊₁, yᵢ₊₁)** são os dois pontos da curva que rodeiam x.
- **y(x)** é a eficiência de produção resultante.

Fora do intervalo definido da curva, usa-se o valor do extremo mais próximo.

**Exemplos de eficiência da produção de água**

| Financiamento (%) | 30 | 50 | 60 | 70 | 75 | 78 | 80 | 90 | 95 | 100 | 105 | 110 | 120 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| Eficiência (%) | 18 | 30 | 42,5 | 55 | 61,25 | 65 | 67,5 | 80 | 90 | 100 | 102,5 | 105 | 110 |

**Porque é isto importante para o Ministério?**

Com 100% de financiamento, a produção de água é mais eficiente em termos de capacidade obtida por simoleão gasto.

Mas isso não significa que todas as cidades precisem de 100% de financiamento.

Imagine-se uma cidade que produz muito mais água do que consome. Reduzir o orçamento da água pode continuar a fornecer água suficiente e a poupar dinheiro.

Por exemplo:

| Financiamento | Capacidade de produção | Custo relativo a 100% |
|---|---:|---:|
| 50% | 30% | 50% |
| 80% | 67,5% | 80% |
| 100% | 100% | 100% |
| 120% | 110% | 120% |

**O Ministério procura o nível de financiamento mais baixo que consiga satisfazer as necessidades de água da cidade e a reserva de segurança**, respeitando as suas outras proteções.

Ao contrário da eletricidade, não precisa de comparar diferentes combinações de financiamento para instalações individuais, porque só existe uma percentagem de orçamento para a água.

Consulte [Como o Ministério escolhe o financiamento](#water-algorithm) para ver o cálculo.

#### A produção de água não se atualiza de imediato

Existe um atraso importante no SimCity 4.

**Quando o financiamento da água muda, a capacidade de produção comunicada pelo jogo só se atualiza no mês simulado seguinte, não de imediato.**

Por exemplo, durante os testes, uma cidade tinha uma capacidade de produção de **241.520 unidades** com 100% de financiamento.

Depois de reduzir o financiamento para 95%, a capacidade comunicada pelo jogo passou para **217.368 unidades** na atualização mensal seguinte.

Isso corresponde exatamente a 90% da capacidade original, de acordo com a curva de eficiência.

Este atraso é relevante porque o Ministério não pode confundir uma leitura de capacidade inalterada logo a seguir a um ajuste de financiamento com a prova de que o ajuste não teve efeito.

#### Como se calcula a fatura mensal da água?

A despesa total do departamento da água depende de duas coisas:

1. Os custos de manutenção combinados de todas as instalações de água.
2. A percentagem de financiamento atual da água.

O jogo cobra estes custos em proporção do financiamento.

**Despesa mensal da água (jogo base)**

```math
X(f) = \operatorname{round}\left(I\times\frac{f}{100}\right)
```

Onde:

- **I** é o custo de manutenção mensal combinado de todas as instalações de água com 100% de financiamento.
- **f** é a percentagem de financiamento do departamento da água.
- **X(f)** é a despesa mensal estimada com a água.

**Exemplo: uma cidade com um orçamento normal de água de §2.000 por mês**

| Financiamento | Despesa mensal | Poupança mensal |
|---|---:|---:|
| 100% | §2.000 | — |
| 90% | §1.800 | §200 |
| 78% | §1.560 | §440 |
| 50% | §1.000 | §1.000 |

Com 78% de financiamento, a cidade poupa **§440 por mês** em comparação com 100%.

No entanto, esse financiamento fornece apenas **65% da capacidade normal de produção**.

O Ministério tem, portanto, de verificar se 65% de capacidade chega para satisfazer a procura, a reserva configurada e quaisquer exportações de água.

A fórmula arredonda as despesas para simoleões inteiros. Por causa dos cálculos internos do jogo, a despesa realmente apresentada pode diferir em um ou dois simoleões.

<a id="water-pipe-curve"></a>
#### Como o financiamento afeta as canalizações de água

O financiamento da água não controla apenas a produção. Afeta também o **estado da rede de canalizações**.

No SimCity 4, as canalizações podem deteriorar-se gradualmente quando o departamento da água recebe financiamento insuficiente.

O jogo usa uma curva separada para determinar se as canalizações se deterioram, se mantêm estáveis ou se reparam sozinhas.

**Curva de deterioração e reparação das canalizações (jogo base)**

| Financiamento da água | Efeito nas canalizações |
|---:|---|
| 0% | Deterioração (−0,010) |
| 20% | Deterioração (−0,005) |
| 50% | Sem deterioração nem reparação (0) |
| 100% | Reparação (+0,020) |
| 110% | Reparação mais rápida (+0,050) |

A curva segue a mesma regra de interpolação linear usada para a eficiência de produção.

Com 120% de financiamento, o valor de reparação mantém-se em +0,050, igual ao de 110%.

**O limiar mais importante é o financiamento de 50%.**

- **Abaixo de 50%:** as canalizações deterioram-se gradualmente.
- **Com 50%:** o seu estado mantém-se estável.
- **Acima de 50%:** a rede de canalizações repara-se gradualmente a si própria.

Isto **não** significa que as canalizações rebentem imediatamente quando se reduz o financiamento abaixo de 50%.

A deterioração acontece ao longo do tempo.

À medida que as canalizações ficam danificadas, o jogo pode classificá-las como *degradadas*. Se o seu estado se deteriorar o suficiente, podem passar a *canalizações rebentadas*.

Repor o financiamento nos 50% interrompe a deterioração causada pela curva de financiamento. Subi-lo acima de 50% permite que a rede recupere.

**Com que rapidez se deterioram ou recuperam as canalizações?**

A tabela seguinte mostra a curva do jogo para vários níveis de financiamento:

| Financiamento (%) | 0 | 10 | 20 | 30 | 35 | 40 | 50 | 60 | 75 | 78 | 90 | 100 | 105 | 110 | 120 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| Curva das canalizações | −0,0100 | −0,0075 | −0,0050 | −0,0033 | −0,0025 | −0,0017 | 0 | +0,0040 | +0,0100 | +0,0112 | +0,0160 | +0,0200 | +0,0350 | +0,0500 | +0,0500 |

Um valor mais negativo significa um efeito de deterioração mais forte. Um valor positivo maior significa um efeito de reparação mais forte.

Por exemplo, com 20% de financiamento, a taxa de deterioração representada pela curva é aproximadamente o dobro da taxa com 30%.

Com 110% de financiamento, o valor de reparação é 2,5 vezes o valor com 100%.

**Como usa o Ministério esta informação?**

O Ministério reconhece se um nível de financiamento causa:

- Deterioração das canalizações.
- Estado estável das canalizações.
- Reparação das canalizações.

Quando a proteção das canalizações está ativa, esta informação ajuda-o a evitar reduções de financiamento que colocariam a rede em risco.

**Uma limitação importante:** o Ministério não calcula o número exato de dias simulados que uma canalização individual demoraria a deteriorar-se ou a rebentar. O seu modelo usa a direção da curva, não uma data de falha prevista.

Isto permite-lhe proteger a rede sem depender de uma estimativa não verificada da rapidez com que cada canalização se desgasta.
<a id="water-problem"></a>
### 2. O problema da gestão

Gerir manualmente o financiamento da água pode parecer simples, já que toda a cidade partilha uma única percentagem de orçamento. No entanto, vários fatores tornam a tarefa mais complicada.

- **Gastos desnecessários:** uma cidade pode produzir muito mais água do que precisa, sobretudo depois de construir uma nova bomba ou estação de tratamento. Manter o financiamento a 100% pode desperdiçar centenas de simoleões todos os meses.
- **Risco de falta de água:** reduzir demasiado o financiamento pode deixar edifícios sem água. Pior ainda, a capacidade de produção do jogo só se atualiza no mês simulado seguinte, pelo que as consequências de uma alteração de financiamento podem não ser visíveis de imediato.
- **Contratos regionais:** comprar água reduz a quantidade que a cidade precisa de produzir localmente, enquanto vender água a aumenta.
- **Deterioração das canalizações:** um financiamento abaixo de 50% faz com que as canalizações de água se deteriorem gradualmente. Os custos de reparação e as consequências de canalizações danificadas não se refletem diretamente no orçamento mensal da água.
- **Problemas de ligação:** pode haver edifícios sem água mesmo quando a cidade tem capacidade de produção suficiente. Nesses casos, aumentar o financiamento não ajuda. O jogador tem de corrigir as ligações das canalizações.

**O Ministério resolve isto calculando a capacidade de água de que a cidade realmente precisa, encontrando o nível de financiamento adequado mais baixo e protegendo a rede de canalizações quando essa opção está ativa.**

<a id="water-algorithm"></a>
### 3. Como o Ministério escolhe o financiamento da água

O Ministério não se limita a baixar o orçamento da água sempre que a produção excede a procura.

Em vez disso, segue quatro passos:

1. Calcular a quantidade de água que a cidade tem de produzir localmente.
2. Estimar a quantidade de água que se pode produzir com cada percentagem de financiamento.
3. Identificar os níveis de financiamento seguros e comportáveis.
4. Escolher a percentagem adequada mais barata e aplicá-la pela ordem correta.

Todos os cálculos usam a informação mais recente disponível sobre a cidade.

<a id="water-required-capacity"></a>
#### Passo 1: De quanta água precisa a cidade?

O Ministério começa por calcular a capacidade mínima de produção de água que a cidade tem de manter.

Tem em conta:

- A procura de água atual da cidade.
- A reserva de água que escolheu (**10% por predefinição**).
- A água comprada às cidades vizinhas.
- A água vendida às cidades vizinhas.

**A reserva aplica-se apenas à água que a cidade tem de produzir por si própria.**

Por exemplo, se a cidade precisa de 20.000 unidades de água e produz tudo localmente, uma reserva de 10% significa que deve manter capacidade para pelo menos **22.000 unidades**.

Se a cidade comprar parte dessa água a uma vizinha, a reserva aplica-se apenas à procura local restante.

**Capacidade local de água necessária**

```math
L = \left\lceil R \times \max(0,D-B) \right\rceil + S
\qquad
R = 1+\frac{r}{100}
```

Onde:

- **D** é a procura total de água da cidade.
- **B** é a água comprada às cidades vizinhas que pode ser usada com fiabilidade.
- **S** é a água vendida às cidades vizinhas.
- **r** é a Reserva de água configurada, 10% por predefinição.
- **R** é o multiplicador da reserva (1,10 para uma reserva de 10%).
- **L** é a capacidade local de produção de água necessária.

O resultado é arredondado por excesso para uma unidade de água inteira.

**Exemplo: uma cidade que precisa de 20.000 unidades**

| Situação | Capacidade local necessária |
|---|---:|
| Sem importações nem exportações de água | 22.000 unidades |
| Importando 5.000 unidades | 16.500 unidades |
| Exportando 5.000 unidades | 27.000 unidades |

Sem contratos regionais:

```math
L = \lceil 20000 \times 1.10 \rceil = 22000
```

Com uma compra de 5.000 unidades:

```math
L = \lceil (20000-5000)\times1.10\rceil = 16500
```

Com uma venda de 5.000 unidades:

```math
L = \lceil 20000\times1.10\rceil+5000=27000
```

Repare-se que **a reserva não é somada à água exportada**. As exportações têm de ser cobertas, mas não recebem uma margem de segurança adicional.

Da mesma forma, o Ministério não compra água extra só para criar uma reserva sobre o abastecimento importado.

**Que compras de água são fiáveis?**

Só as compras confirmadas como ativas são consideradas fiáveis.

Os contratos pendentes, suspensos ou com falhas não contam como água disponível.

Se o Ministério gerir também os contratos regionais de água, calcula o financiamento usando as compras e as vendas incluídas no seu plano de contratos. As duas decisões são coordenadas, e não tomadas de forma independente.

Consulte [Acordos com os Vizinhos](deals.md#neighbor-deals).

#### Passo 2: Quanta água pode a cidade produzir com cada nível de financiamento?

Conhecida a capacidade necessária, o Ministério examina a produção de água atual da cidade.

Estima depois a quantidade de água que as instalações existentes conseguiriam produzir com diferentes percentagens de financiamento.

Como todos os produtores de água partilham a mesma definição de financiamento, este cálculo é muito mais simples do que a otimização da eletricidade.

**Capacidade de produção de água projetada**

```math
C(v)=\left\lfloor C_{\text{now}}\times\frac{\eta(v)}{\eta(f_{\text{now}})}\right\rfloor
```

Onde:

- **Cnow** é a capacidade de produção atualmente comunicada pelo jogo.
- **fnow** é a percentagem de financiamento da água atual, lida tal como o jogo a guarda.
- **v** é uma possível nova percentagem de financiamento.
- **η** é a curva de eficiência da produção de água do jogo.
- **C(v)** é a capacidade estimada com o financiamento proposto, arredondada por defeito.

Em linguagem simples, **o Ministério usa a capacidade de produção atual e a curva de eficiência do jogo para prever a quantidade de água que um nível de financiamento diferente forneceria**.

**Exemplo: uma cidade que produz 34.000 unidades**

Suponha-se que a cidade produz atualmente 34.000 unidades com 100% de financiamento.

Com 78% de financiamento, a curva de eficiência do jogo fornece 65% da capacidade normal de produção.

```math
C(78)=\left\lfloor34000\times\frac{65}{100}\right\rfloor=22100
```

O Ministério estima, portanto, que **78% de financiamento forneceria 22.100 unidades**.

Se a cidade precisa de 22.000 unidades, incluindo a reserva, isso seria suficiente.

**Uma limitação importante**

Este cálculo pressupõe que todos os produtores de água respondem ao financiamento do departamento de acordo com a curva de eficiência padrão.

Se o financiamento atual der eficiência zero, como acontece a 0%, o Ministério não consegue usar esta fórmula para estimar de forma fiável capacidades alternativas. Nessa situação, não faz nenhum plano automático de financiamento da água.

<a id="water-funding-choice"></a>
#### Passo 3: Que níveis de financiamento são permitidos?

O Ministério não considera todos os níveis de financiamento possíveis sem restrições.

As opções disponíveis dependem das suas definições e do estado atual da rede de água.

**Financiamento mínimo**

| Proteger as canalizações | Mínimo para novas escolhas de financiamento |
|---|---:|
| **Ativo (predefinição)** | **50%** |
| Desativado | 0% |

Com a proteção das canalizações ativa, o Ministério mantém normalmente o financiamento no ponto em que a curva de deterioração das canalizações do jogo se torna neutra, ou acima dele.

Com 50% de financiamento, as canalizações nem se deterioram nem se reparam por causa do financiamento do departamento.

Acima de 50%, a rede repara-se gradualmente a si própria.

**Financiamento máximo**

O máximo normal é de **120%**, ou um valor inferior se o jogo indicar um limite inferior para o departamento.

**E se tiver definido manualmente um financiamento abaixo de 50%?**

O Ministério inclui sempre o financiamento atual como escolha possível, mesmo quando está fora do seu intervalo de pesquisa normal.

Por exemplo, se definir manualmente o financiamento da água em 40%, esse nível existente não é descartado automaticamente da ponderação.

No entanto, com a proteção das canalizações ativa, o Ministério não escolhe uma **nova redução** que cause deterioração das canalizações.

Esta distinção é importante: **proteger as canalizações não significa que o Ministério aumente automaticamente todos os orçamentos existentes abaixo de 50%.** A escolha do financiamento continua a seguir os requisitos de capacidade e as regras de segurança.

#### Quando recusa o Ministério reduzir o financiamento?

Mesmo que uma percentagem mais baixa poupasse dinheiro, o Ministério recusa uma redução quando se verifica qualquer uma das seguintes condições:

**1. A cidade tem uma falta de água real**

Se houver edifícios sem água e a capacidade de produção local for inferior à procura, o Ministério não reduz mais o financiamento.

**2. As canalizações já estão danificadas**

Quando **Proteger as canalizações** está ativo, o Ministério recusa reduções se o jogo indicar canalizações degradadas ou rebentadas.

**3. A redução causaria deterioração das canalizações**

Com a proteção ativa, uma redução de financiamento é rejeitada se a curva das canalizações do jogo passasse a ser negativa.

Estas restrições aplicam-se às **reduções de financiamento**.

Não impedem o Ministério de aumentar o financiamento para melhorar o abastecimento de água.

#### Passo 4: Escolher o financiamento mais barato

Depois de eliminar as opções inseguras, o Ministério compara os níveis de financiamento restantes.

O seu objetivo é selecionar **a menor despesa mensal que ainda fornece a capacidade de água necessária**.

**Otimização do financiamento**

```math
f^* = \arg\min_{v\in V,\ C(v)\ge L} X(v)
```

Onde:

- **V** é o conjunto das percentagens de financiamento permitidas.
- **C(v)** é a capacidade de água estimada com o financiamento **v**.
- **L** é a capacidade local necessária do Passo 1.
- **X(v)** é a despesa operacional mensal com o financiamento **v**.
- **f\*** é a percentagem de financiamento escolhida.

Como tanto a capacidade de água como a despesa operacional aumentam com o financiamento, **a solução mais barata é normalmente a percentagem permitida mais baixa que fornece água suficiente**.

Se duas opções tiverem o mesmo custo, o Ministério prefere manter o financiamento atual.

**Exemplo: 34.000 unidades com financiamento total**

Voltemos ao exemplo anterior:

- Capacidade atual a 100%: 34.000 unidades.
- Capacidade necessária, incluindo a reserva: 22.000 unidades.
- Proteção das canalizações: ativa.

| Financiamento | Capacidade projetada | Resultado |
|---|---:|---|
| 77% | 21.675 | Insuficiente |
| **78%** | **22.100** | **Suficiente** |
| 80% | 22.950 | Suficiente, mas mais caro |
| 100% | 34.000 | Excesso de capacidade desnecessário |

**O Ministério escolhe 78% de financiamento**, desde que nenhuma das condições de segurança impeça a redução.

Se o departamento custa normalmente §2.000 por mês, o novo custo é de aproximadamente **§1.560**, o que poupa **§440 por mês**.

**E se nem o financiamento máximo for suficiente?**

O Ministério seleciona o nível de financiamento permitido que fornece a maior capacidade disponível, normalmente 120%.

Se várias opções fornecerem a mesma capacidade máxima, prefere a mais barata.

Não reduz o financiamento quando a cidade já não tem capacidade suficiente.

Quando a gestão dos contratos regionais de água está ativa, o Ministério pode também tentar comprar água para cobrir a falta restante.

Se o problema persistir, o jogador pode receber um aviso de um conselheiro.

Consulte [Acordos com os Vizinhos](deals.md#neighbor-deals) e [Avisos dos Conselheiros](safety.md#notices).

#### Como são aplicadas as alterações de financiamento?

O Ministério aplica diretamente a percentagem de financiamento escolhida, sem mover gradualmente o cursor do orçamento.

No entanto, **a ordem das alterações de financiamento e de contratos é importante**.

- **Os aumentos de financiamento acontecem primeiro**, antes de alterar os contratos de água.
- **As reduções de financiamento só acontecem depois** de aplicadas as alterações necessárias aos contratos regionais.

Por exemplo, suponha-se que o Ministério quer reduzir a produção local de água porque encontrou uma compra mais barata a uma cidade vizinha.

Primeiro garante que as alterações ao contrato regional estão em vigor e só depois reduz o orçamento da água.

Isto evita cortar a produção local antes de o abastecimento alternativo estar assegurado.

Cada revisão executa no máximo uma etapa coordenada para um recurso. As restantes alterações são reconsideradas numa avaliação posterior, com leituras novas do jogo.

**O objetivo é tornar seguras as transições, e não apenas o nível final de financiamento.**

<a id="water-cadence"></a>
#### Com que frequência verifica o Ministério o financiamento da água?

O Ministério verifica regularmente o abastecimento de água e pode reagir mais cedo quando ocorrem alterações significativas.

Todos os prazos se referem ao tempo simulado do jogo.

| Quando | O que acontece |
|---|---|
| **Quando uma cidade é aberta** | Faz a primeira avaliação da água assim que as leituras do jogo estão prontas e coerentes. |
| **Todos os dias simulados** | Verifica se há faltas graves e alterações significativas na procura de água, na produção ou nos contratos regionais. |
| **A cada 10 dias simulados** | Faz uma revisão regular do financiamento da água, contada a partir da última revisão completa. |
| **No início de cada mês simulado** | Faz uma revisão da água. |
| **Quando é detetada uma alteração importante** | Recalcula o plano da água durante a verificação diária, sem esperar pela revisão agendada seguinte. |

Cada revisão concluída reinicia o intervalo de revisão regular. Se acabou de se realizar uma avaliação, o Ministério não a repete de imediato só porque estava prevista outra verificação agendada.

**O que desencadeia uma avaliação antecipada?**

As verificações diárias do Ministério procuram duas situações.

**1. Uma falta real de abastecimento de água**

Há falta quando a cidade consome menos água do que precisa **e** o abastecimento utilizável total é insuficiente.

**Défice regional de água**

```math
W < D \qquad\text{and}\qquad C+U-S<D
```

Onde:

- **W** é a água efetivamente consumida.
- **D** é a procura de água da cidade.
- **C** é a capacidade de produção local.
- **U** é a água efetivamente entregue através de contratos de compra.
- **S** é a água vendida às cidades vizinhas.

Por exemplo, se uma cidade precisa de 20.000 unidades mas o seu abastecimento utilizável total é de apenas 15.000, o Ministério pode recalcular o financiamento sem esperar pela revisão regular seguinte.

Mas imagine-se que a cidade precisa de 20.000 unidades e tem 25.000 disponíveis, e que mesmo assim alguns edifícios continuam sem água.

Nessa situação, o problema está provavelmente na rede de canalizações da cidade e não numa produção insuficiente.

**Aumentar o orçamento não ajudaria necessariamente**, pelo que o Ministério regista o problema de ligação em vez de desencadear uma revisão de financiamento só por esse motivo.

Uma falta já avaliada não desencadeia repetidamente novas revisões, a menos que a situação do abastecimento mude.

**2. Uma alteração significativa do abastecimento ou da procura de água**

O Ministério recalcula também o financiamento quando qualquer um dos seguintes valores varia pelo menos **1%** em relação à revisão anterior:

- A procura de água.
- A capacidade de produção local.
- As importações de água contratadas.
- As exportações de água contratadas.

**Limiar de alteração significativa**

```math
|x-x_0|\ge0.01\times\max(1,|x_0|)
```

Onde:

- **x** é a leitura mais recente.
- **x₀** é o valor registado na revisão anterior.

Por exemplo, se a procura passar de 20.000 para 20.300 unidades, isso é um aumento de 1,5%.

O Ministério pode, portanto, recalcular o financiamento na verificação diária seguinte.

**As leituras inalteradas não desencadeiam avaliações desnecessárias.**

Isto mantém o sistema reativo sem recalcular constantemente a mesma situação.

<a id="water-regional"></a>
#### Como os contratos regionais de água afetam o financiamento

As compras e as vendas de água são consideradas ao calcular a quantidade de água que as instalações da própria cidade têm de produzir.

As regras são simples:

- **As compras reduzem a procura local.** Só as compras ativas de forma fiável contam como água disponível.
- **As compras não podem cobrir mais do que a procura total da cidade** no cálculo da capacidade local.
- **As vendas aumentam a necessidade de produção local.** A cidade tem de fornecer água suficiente tanto para as suas próprias necessidades como para as suas obrigações de exportação.
- **As vendas existentes são tratadas de forma conservadora.** Quando o Ministério não está a gerir contratos, conta todas as vendas listadas como obrigação, seja qual for o seu estado, ou as exportações comunicadas pelo jogo se esse número for maior.
- **As vendas não são mantidas durante um défice real de água.** O planeador de contratos dá prioridade ao abastecimento da própria cidade.

#### Como decide o Ministério se um contrato é rentável?

Um contrato de água não compensa automaticamente só porque outra cidade se oferece para comprar ou vender água.

O Ministério compara o preço do contrato com o custo operacional adicional, ou a poupança, que ele criaria.

Por exemplo:

- **Vender água:** o Ministério calcula quanto financiamento extra seria necessário para produzir a água vendida.
- **Comprar água:** calcula quanto financiamento local se poderia poupar ao depender da água comprada.

**Custo marginal da água**

```math
\Delta(q)=X^*(L+q)-X^*(L)
```

Onde:

- **L** é a capacidade local de água necessária atual.
- **q** é a quantidade do contrato, positiva para uma venda e negativa para uma compra.
- **X\*(n)** é a menor despesa mensal de água necessária para fornecer pelo menos **n** unidades.
- **Δ(q)** é a variação das despesas operacionais mensais.

**Numa venda de água**, o Ministério compara o dinheiro recebido com a despesa adicional de produção local.

Uma venda só compensa se o vizinho pagar mais do que o custo extra.

**Numa compra de água**, o Ministério compara o preço do contrato com a poupança obtida ao reduzir a produção local.

Uma compra só compensa se custar menos do que essa poupança.

**Exemplo**

Suponha-se que produzir mais 5.000 unidades de água exige aumentar o orçamento mensal do departamento em §300.

Uma cidade vizinha propõe-se comprar essas 5.000 unidades.

| Pagamento mensal do vizinho | Resultado económico |
|---|---|
| §200 | Não compensa: prejuízo de §100 |
| §300 | Ponto de equilíbrio |
| §400 | Compensa: ganho de §100 |

É por isso que o Ministério considera **a variação real dos custos de produção**, em vez de presumir que todos os contratos regionais são vantajosos.

As regras completas de preços e de gestão dos contratos são explicadas em [Acordos com os Vizinhos](deals.md#neighbor-deals).

#### E se a gestão automática da água ou dos contratos estiver desativada?

As duas funções podem funcionar de forma independente.

| Definições | O que acontece |
|---|---|
| **Gerir a água: Ativo** / **Gerir acordos de água: Ativo** | O Ministério pode otimizar tanto o financiamento da água como os contratos regionais de água. |
| **Gerir a água: Ativo** / **Gerir acordos de água: Desativado** | O financiamento da água é otimizado com base nos contratos existentes, que o Ministério deixa inalterados. |
| **Gerir a água: Desativado** / **Gerir acordos de água: Ativo** | O financiamento da água mantém-se no nível atual, mas os contratos regionais de água continuam a poder ser geridos. |
| **Ambos desativados** | O Ministério não altera automaticamente o financiamento da água nem os contratos de água. |

A gestão automática dos contratos regionais de água exige também que a opção principal **Gerir os acordos com os vizinhos** esteja ativa.

<a id="water-farm-icon"></a>
#### Um pequeno melhoramento visual: avisos de água nas quintas

O Ministério faz também um pequeno ajuste à interface do jogo.

**Esconde o ícone de aviso original de «sem água» sobre os lotes agrícolas.**

Esta alteração afeta apenas o ícone de aviso apresentado por cima das quintas.

**Não**:

- Fornece água às quintas.
- Altera a sua procura de água.
- Altera as decisões do jogo sobre os edifícios agrícolas.
- Altera o próprio sistema de água.
- Esconde os avisos de água dos edifícios residenciais, comerciais ou industriais.

O ajuste é aplicado quando o SimCity 4 arranca e é independente das definições de gestão do Ministério.

Aplica-se apenas à versão do jogo suportada. Nas outras versões, o Ministério deixa este comportamento da interface inalterado.

<a id="water-safety"></a>
### 4. Condições de segurança

O Ministério segue várias regras de segurança para evitar deixar a cidade sem água ou danificar a sua rede de canalizações.

#### Antes de alterar o financiamento da água

O Ministério só faz alterações de financiamento quando pode confiar na informação do jogo.

Verifica que:

- O departamento da água e o seu financiamento atual podem ser lidos corretamente.
- A procura de água e a capacidade de produção estão disponíveis, com a procura superior a zero.
- O jogo atualizou as contagens de edifícios com e sem água.
- O consumo de água comunicado é coerente com a procura.
- Todos os produtores de água usam o sistema de financiamento padrão do jogo.

**Se qualquer uma destas verificações falhar, o Ministério deixa o financiamento da água inalterado.**

Isto é particularmente importante logo a seguir à abertura de uma cidade, quando algumas estatísticas da água podem ainda não estar disponíveis.

Para maior rigor técnico, o Ministério aceita uma pequena diferença entre o consumo e a procura:

```math
W \le 1.01 \times D + 1
```

Onde **W** é o consumo de água comunicado e **D** é a procura.

Um consumo superior a este limite é tratado como uma leitura incoerente.

Se um único produtor de água usar um mecanismo de financiamento não suportado, a gestão automática do financiamento da água fica desativada para essa sessão da cidade, em vez de arriscar cálculos incorretos.

<a id="water-no-cut"></a>
#### Quando recusa o Ministério reduzir o financiamento?

Mesmo quando um orçamento mais baixo poderia poupar dinheiro, o Ministério recusa uma redução nas seguintes situações:

| Situação | Porque é recusada a redução |
|---|---|
| **Produção de água insuficiente** | Há edifícios sem água e a capacidade de produção já está abaixo da procura. Cortar o financiamento agravaria a falta. |
| **Canalizações danificadas** | Com **Proteger as canalizações** ativo, qualquer canalização degradada ou rebentada impede reduções de financiamento. |
| **Risco de deterioração das canalizações** | Com a proteção das canalizações ativa, o Ministério não reduz o financiamento abaixo do limiar de 50%. |
| **Capacidade insuficiente após o corte** | O financiamento proposto não forneceria água suficiente para a procura, a reserva e as exportações. |
| **Nenhum nível de financiamento adequado** | Se nem a melhor definição disponível conseguir satisfazer o requisito, o Ministério aumenta o financiamento, sempre que possível, em vez de o reduzir. |
| **Informação pouco fiável ou alterada** | As leituras estão incompletas, são incoerentes ou mudaram desde que o cálculo foi feito. |
| **Financiamento fixo** | O financiamento da água não está sob gestão automática. |

**Toda a redução proposta é verificada duas vezes:** uma quando o plano é calculado e outra imediatamente antes de o novo financiamento ser aplicado.

Isto impede o Ministério de usar um cálculo desatualizado se a situação da cidade mudar.

**E se alguns edifícios estiverem sem água, mas a produção for suficiente?**

Isso é diferente.

Por exemplo, uma cidade pode produzir 30.000 unidades quando só precisa de 20.000 e, ainda assim, ter edifícios sem água por estarem desligados da rede de canalizações.

Nesse caso, **aumentar o financiamento não resolve o problema de ligação**.

O Ministério pode, ainda assim, reduzir o orçamento, desde que a capacidade necessária e as regras de proteção das canalizações continuem satisfeitas.

#### O que acontece se uma alteração de financiamento falhar?

Antes de alterar o orçamento, o Ministério verifica se o financiamento atual ainda corresponde ao valor usado no cálculo.

Se tiver movido o cursor manualmente, a alteração planeada é ignorada.

Depois de aplicar uma alteração, lê o financiamento outra vez para verificar que o jogo aceitou o novo valor.

Se a verificação falhar:

1. O Ministério tenta repor o financiamento anterior.
2. As alterações automáticas do financiamento da água param durante o resto da sessão atual da cidade.
3. A gestão do financiamento da água pode recomeçar quando voltar a abrir a cidade.

**Os contratos regionais de água não são afetados por esta paragem de segurança.** A sua gestão pode continuar de forma independente.

<a id="water-settings"></a>
### 5. Definições

A gestão da água pode ser configurada em **Opções de Jogo → Ministério da Optimização**, na página **Serviços**.

| Definição | Predefinição | Intervalo | O que faz |
|---|---|---|---|
| [Ministério ativo](options.md#opt-master) | Ativo | Ativo / Desativado | Ativa ou desativa todo o Ministério. |
| [Gerir a água](options.md#opt-water) | Ativo | Ativo / Desativado | Ajusta automaticamente o financiamento do departamento da água da cidade. |
| [Reserva de água](options.md#opt-waterreserve) | 10% | 10–50% | Define a capacidade mínima de reserva da produção local de água. |
| [Proteger as canalizações](options.md#opt-waterpipes) | Ativo | Ativo / Desativado | Impede reduções que possam causar a deterioração das canalizações ou interferir na recuperação de canalizações danificadas. |
| [Gerir os acordos com os vizinhos](options.md#opt-deals) | Ativo | Ativo / Desativado | Ativa a gestão automática dos contratos regionais. |
| [Gerir acordos de água](options.md#opt-tradewater) | Ativo | Ativo / Desativado | Ativa as compras e as vendas automáticas de água. |
| [Comprar a uma cidade para vender a outra](options.md#opt-arbitrage) | Desativado | Ativo / Desativado | Permite ao Ministério comprar e vender água no mesmo plano regional. |

#### Como funciona a reserva de água?

A reserva predefinida é de **10%**.

Por exemplo, se a cidade tem de produzir 20.000 unidades localmente, o Ministério procura manter capacidade para pelo menos **22.000 unidades**.

A reserva é um mínimo, não um objetivo exato.

O Ministério pode manter mais capacidade quando as percentagens de financiamento disponíveis ou a definição de proteção das canalizações o exigirem.

Aumentar a reserva fornece capacidade de produção sobresselente adicional, mas pode também aumentar as despesas mensais com a água.

#### O que faz Proteger as canalizações?

Com esta opção ativa, o Ministério:

- Usa normalmente **50% como mínimo para novas escolhas de financiamento**.
- Nunca reduz o financiamento para um nível que cause deterioração das canalizações.
- Recusa reduções enquanto o jogo indicar canalizações degradadas ou rebentadas.

**Não aumenta automaticamente o financiamento só para reparar canalizações.**

Se definir manualmente o financiamento abaixo de 50%, o Ministério pode deixá-lo aí enquanto a produção for suficiente. Aumenta o financiamento quando necessário para satisfazer a necessidade de abastecimento de água, usando o seu intervalo de financiamento protegido normal.

Se desativar a proteção das canalizações, o Ministério pode escolher um financiamento tão baixo como **0%**, desde que as necessidades de abastecimento de água continuem a ser satisfeitas.

Isto pode poupar mais dinheiro, mas permite que a rede de canalizações se deteriore com o tempo.

#### Definição avançada (apenas no ficheiro INI)

| Definição | Predefinição | Intervalo permitido | O que faz |
|---|---|---|---|
| `WaterCadenceDays` | 10 | 1–30 | Define o intervalo da revisão regular do financiamento da água, em dias simulados. |

Por exemplo, definir `WaterCadenceDays=5` faz com que as revisões regulares ocorram a cada cinco dias simulados.

As verificações diárias de alterações significativas e a revisão no início de cada mês simulado mantêm-se separadas.

<a id="water-examples"></a>
### 6. Exemplos práticos

Os exemplos seguintes mostram como o Ministério escolhe o financiamento da água e como as suas proteções de segurança afetam o resultado.

Todos os exemplos usam a **reserva de água de 10%** predefinida, salvo indicação em contrário.

As capacidades de produção e as despesas do departamento são ilustrativas.

#### Exemplo 1: Reduzir o financiamento da água sem criar uma falta

Considere uma cidade com:

- **Procura de água:** 20.000 unidades.
- **Capacidade de produção atual:** 34.000 unidades.
- **Financiamento atual:** 100%.
- **Despesa mensal de água a 100%:** §2.000.
- **Contratos regionais:** nenhum.
- **Canalizações danificadas:** nenhuma.
- **Edifícios sem água:** nenhum.

**Passo 1: Calcular a capacidade necessária**

Com uma reserva de 10%:

```math
L = \lceil 20000 \times 1.10 \rceil = 22000
```

A cidade precisa de capacidade para **22.000 unidades**.

**Passo 2: Encontrar o financiamento suficiente mais baixo**

| Financiamento | Capacidade esperada | Despesa mensal | Resultado |
|---|---:|---:|---|
| 77% | 21.675 | §1.540 | Insuficiente |
| **78%** | **22.100** | **§1.560** | **Suficiente** |
| 100% | 34.000 | §2.000 | Capacidade excedente desnecessária |

**O Ministério escolhe um financiamento de 78%.**

A este nível, a cidade mantém água suficiente para a sua procura e para a reserva.

A curva de reparação das canalizações também é positiva (+0,0112), pelo que a redução não provoca a deterioração das canalizações.

**Poupança mensal: §440.**

#### Exemplo 2: Quanto custa a proteção das canalizações?

Imagine-se uma cidade com:

- **Procura de água:** 10.000 unidades.
- **Capacidade de produção a 100%:** 60.000 unidades.
- **Despesa mensal a 100%:** §3.000.

Com uma reserva de 10%, só precisa de:

```math
L = 10000 \times 1.10 = 11000
```

A cidade tem muito mais capacidade de produção de água do que a necessária.

Compare-se o resultado com a proteção das canalizações ativada e desativada.

**Com Proteger as canalizações ativado**

O Ministério não pode escolher um novo objetivo de financiamento abaixo de 50%.

Com 50% de financiamento, a eficiência de produção é de 30%:

```math
C(50)=60000\times0.30=18000
```

É mais do que suficiente para abastecer as 11.000 unidades necessárias.

**O Ministério escolhe um financiamento de 50%**, que custa **§1.500 por mês**.

A rede de canalizações mantém-se estável.

**Com Proteger as canalizações desativado**

O Ministério pode considerar níveis de financiamento mais baixos.

| Financiamento | Capacidade esperada | Resultado |
|---|---:|---|
| 30% | 10.800 | Insuficiente |
| **31%** | **11.160** | **Suficiente** |
| 50% | 18.000 | Mais do que o necessário |

Sem a proteção das canalizações, **31% de financiamento** chega.

A despesa mensal desce para **§930**.

No entanto, a curva de deterioração das canalizações passa a ser negativa, aproximadamente −0,0032.

A rede vai-se desgastando gradualmente.

**O compromisso financeiro**

| Definição | Financiamento | Despesa mensal | Efeito nas canalizações |
|---|---:|---:|---|
| **Proteção ligada** | 50% | §1.500 | Estável |
| Proteção desligada | 31% | §930 | Deterioração gradual |

**Neste exemplo, a proteção das canalizações custa mais §570 por mês.**

Essa despesa adicional evita a deterioração causada pelo subfinanciamento.

Decidir se a poupança compensa os danos a longo prazo cabe ao jogador, ativando ou desativando a proteção.

#### Exemplo 3: Comprar e vender água ao mesmo tempo

Regresse-se à cidade do Exemplo 1:

- **Procura de água:** 20.000 unidades.
- **Capacidade de produção normal:** 34.000 unidades.
- **Custo mensal a 100%:** §2.000.

Desta vez, a cidade tem contratos regionais:

- **Água comprada:** 5.000 unidades.
- **Água vendida:** 8.000 unidades.

A compra está confirmada como ativa.

**Passo 1: Calcular a capacidade local necessária**

```math
L = \lceil(20000-5000)\times1.10\rceil+8000
```

```math
L=16500+8000=24500
```

A cidade precisa de **24.500 unidades** de capacidade de produção local.

**Passo 2: Escolher o financiamento**

| Financiamento | Capacidade esperada | Resultado |
|---|---:|---|
| 83% | 24.225 | Insuficiente |
| **84%** | **24.650** | **Suficiente** |
| 100% | 34.000 | Mais do que o necessário |

**O Ministério escolhe um financiamento de 84%.**

A despesa mensal de água passa a ser:

```math
X(84)=2000\times0.84=1680
```

**Despesa mensal do departamento da água: §1.680**, sem contar o custo e o rendimento dos contratos regionais.

Este exemplo mostra por que razão o Ministério tem de considerar as compras e as vendas em conjunto, em vez de olhar apenas para a procura da própria cidade.

#### Exemplo 4: Compensa vender água?

Considere-se de novo a cidade do Exemplo 1, sem contratos regionais.

O seu financiamento mais barato é atualmente:

- **Financiamento de 78%.**
- **22.100 unidades de capacidade de produção.**
- **§1.560 por mês.**

Uma cidade vizinha propõe agora comprar **5.000 unidades de água**.

Para abastecer esta venda, a capacidade local necessária aumenta:

```math
L=22000+5000=27000
```

O Ministério verifica de quanto financiamento precisa:

| Financiamento | Capacidade esperada | Despesa mensal |
|---|---:|---:|
| 78% | 22.100 | §1.560 |
| 89% | 26.775 | §1.780 |
| **90%** | **27.200** | **§1.800** |

**Vender a água adicional exige subir o financiamento de 78% para 90%.**

A despesa extra é:

```math
\Delta=1800-1560=240
```

**A venda custa mais §240 por mês em produção local de água.**

O Ministério pode agora comparar esse custo com o dinheiro oferecido pela cidade vizinha:

| Pagamento mensal | Resultado antes de outras considerações contratuais |
|---|---|
| §200 | Perda de §40 |
| §240 | Equilíbrio |
| §300 | Ganho de §60 |

**A venda só compensa economicamente se o vizinho pagar mais de §240 por mês**, sujeita às restantes regras de segurança dos contratos.

É assim que o Ministério avalia o comércio regional com base nos custos reais de produção, em vez de aceitar todos os acordos disponíveis.

Ver [Acordos com os vizinhos](deals.md#neighbor-deals).

#### Exemplo 5: As canalizações danificadas impedem uma poupança

Regresse-se à cidade do Exemplo 1.

Normalmente, o Ministério reduziria o financiamento da água de 100% para 78%, poupando §440 por mês.

No entanto, o jogo comunica agora **três canalizações em mau estado**.

Com **Proteger as canalizações** ativado, o Ministério recusa a redução.

| Situação | Financiamento | Despesa mensal |
|---|---:|---:|
| Sem canalizações danificadas | 78% | §1.560 |
| **Três canalizações em mau estado** | **100%** | **§2.000** |

**O Ministério mantém o financiamento a 100% enquanto as canalizações estiverem danificadas.**

A este nível de financiamento, a curva das canalizações é positiva (+0,020), o que permite à rede recuperar.

Quando o jogo indicar zero canalizações em mau estado e zero rebentadas, o Ministério pode reduzir o financiamento para 78% na avaliação seguinte.

**O Ministério aumenta o financiamento para reparar as canalizações mais depressa?**

Não.

Preserva o financiamento existente enquanto as canalizações estiverem danificadas, mas não o aumenta apenas para acelerar as reparações.

Trata-se de uma distinção deliberada entre **evitar cortes inseguros** e **gerir ativamente as reparações das canalizações**.

#### Exemplo 6: Edifícios sem água nem sempre significam produção insuficiente

Imagine-se uma cidade com:

- **Procura de água:** 20.323 unidades.
- **Capacidade de produção:** 34.081 unidades.
- **Edifícios sem água:** 55.

À primeira vista, os 55 edifícios sem água sugerem uma falta de água.

No entanto, a cidade já tem capacidade de produção mais do que suficiente para cobrir a sua procura.

O problema provável é que alguns edifícios não estão devidamente ligados à rede de água.

**O Ministério não aumenta automaticamente o financiamento para resolver um problema de ligação.**

Pode mesmo assim reduzir o financiamento, desde que a nova capacidade satisfaça a reserva exigida e as regras de proteção das canalizações.

Considere-se agora uma situação diferente:

- **Procura de água:** 20.323 unidades.
- **Capacidade de produção:** 19.000 unidades.
- **Edifícios sem água:** 55.

Aqui, a capacidade de produção está de facto abaixo da procura.

**O Ministério recusa qualquer redução de financiamento até a falta estar resolvida.**

A distinção é importante: **a falta de água causada por produção insuficiente pode ser resolvida com financiamento; a causada por canalizações desligadas não.**

#### Exemplo 7: Nem o financiamento máximo chega

Considere uma cidade com:

- **Procura de água:** 35.000 unidades.
- **Capacidade de produção a 100%:** 34.000 unidades.
- **Reserva de água:** 10%.
- **Contratos regionais:** nenhum.

A capacidade necessária é:

```math
L=35000\times1.10=38500
```

A cidade precisa de **38.500 unidades**.

No entanto, mesmo com 120% de financiamento, a eficiência de produção de água é de apenas 110%.

```math
C(120)=34000\times1.10=37400
```

**Capacidade máxima disponível: 37.400 unidades.**

Fica **1.100 unidades abaixo da capacidade necessária**, incluindo a reserva.

O Ministério sobe, por isso, o financiamento da água para **120%**, a definição mais alta disponível.

Se a gestão dos contratos regionais de água estiver ativada, pode também tentar comprar água adicional a uma cidade vizinha.

Caso contrário, o jogador tem de aumentar a capacidade de produção local, por exemplo construindo outra bomba de água.

**O financiamento não consegue produzir mais água do que a capacidade máxima do jogo permite.**

<a id="water-limitations"></a>
### 7. Limitações conhecidas

O Ministério optimiza automaticamente o financiamento da água, mas várias limitações vêm do próprio SimCity 4.

#### Produção e financiamento da água

- **A capacidade de produção atualiza-se com atraso.** Depois de uma alteração do financiamento, o SimCity 4 normalmente só atualiza a capacidade de água comunicada no mês simulado seguinte. Até lá, o Ministério pode calcular temporariamente um novo plano com uma capacidade que ainda reflete o financiamento anterior. A atualização posterior da capacidade desencadeia outra avaliação, o que lhe permite corrigir o plano.
- **Existe apenas uma percentagem de financiamento da água.** O Ministério não pode financiar de forma diferente cada bomba, torre ou estação de tratamento.
- **O financiamento não repara todos os problemas de abastecimento.** Uma fonte de água poluída ou encerrada pode produzir menos água. Aumentar o orçamento não elimina o problema de fundo.
- **O financiamento máximo tem limites.** Se a produção continuar insuficiente a 120%, a cidade precisa de mais capacidade de produção de água ou de compras regionais fiáveis.

#### Proteção das canalizações

- **A proteção evita cortes; não gere ativamente as reparações.** O Ministério não aumenta o financiamento só para reparar mais depressa as canalizações em mau estado ou rebentadas.
- **As redes subfinanciadas manualmente não são repostas automaticamente nos 50%.** Se definir pessoalmente um financiamento abaixo de 50% e a produção continuar a ser suficiente, o Ministério pode deixá-lo inalterado. Sobe o financiamento quando é necessária mais produção, dentro do intervalo protegido normal.
- **A deterioração das canalizações não é prevista ao longo do tempo.** O Ministério usa a curva de financiamento do jogo para determinar se as canalizações se deterioram ou recuperam, mas não calcula quantos meses demorará uma determinada canalização a falhar.
- **Desativar a proteção permite a deterioração.** Com **Proteger as canalizações** desligado, o Ministério pode escolher um financiamento abaixo de 50% se a necessidade de água ainda puder ser satisfeita.

#### Distribuição de água

- **Os edifícios sem água podem ter problemas de ligação.** Uma produção suficiente em toda a cidade não garante que a água chegue a todos os edifícios. O Ministério não consegue corrigir ligações de canalizações em falta.
- **O Ministério não constrói infraestruturas.** As novas bombas, torres de água e redes de canalizações continuam a ser responsabilidade do jogador.

#### Compatibilidade dos edifícios

**Todos os produtores de água têm de usar o mecanismo de financiamento padrão do SimCity 4.**

Se um único edifício produtor de água usar um sistema de financiamento personalizado incompatível, o Ministério não gere automaticamente o orçamento da água da cidade.

Esta restrição impede o Ministério de fazer cálculos baseados em pressupostos que não se aplicam a todos os produtores.

**O objetivo do Ministério é manter uma produção de água suficiente ao menor custo seguro, e não substituir a responsabilidade do jogador pela infraestrutura de água.**

[Voltar ao índice](index.md#contents) | [Voltar ao README](../../README.pt.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
