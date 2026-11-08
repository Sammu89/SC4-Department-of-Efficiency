[English](../en/deals.md) | **Português** | [Français](../fr/deals.md) | [Español](../es/deals.md)

[Voltar ao índice](index.md#contents) | [Voltar ao README](../../README.pt.md)

<a id="neighbor-deals"></a>
## E. Acordos com os vizinhos

O Ministério pode gerir automaticamente os **contratos de eletricidade, água e lixo entre a sua cidade e as cidades vizinhas**.

Decide quando:

- **Criar um contrato** quando negociar com uma vizinha beneficiar a sua cidade.
- **Aumentar ou reduzir um contrato existente** à medida que as necessidades da cidade mudam.
- **Cancelar um contrato** quando já não é útil ou seguro.

O objetivo é **reduzir as despesas globais da cidade, ganhar dinheiro com exportações rentáveis e manter serviços essenciais fiáveis**.

O Ministério considera primeiro as necessidades da própria cidade. Compara depois o custo de produzir ou eliminar um recurso localmente com o preço oferecido pelas cidades vizinhas.

Por exemplo, comprar eletricidade pode ser mais barato do que aumentar o financiamento de uma central elétrica. Inversamente, vender eletricidade excedente pode ser rentável se a receita for superior ao custo de produção adicional.

O mesmo princípio aplica-se à água e à eliminação de lixo.

Este capítulo explica **como funcionam os contratos regionais e como o Ministério os gere**.

Para mais pormenores sobre a produção local e o financiamento, consulte [Eletricidade](power.md#power), [Água](water.md#water) e [Lixo e aterros](garbage.md#garbage-landfills).

<a id="neighbor-deals-mechanism"></a>
### 1. Como funcionam os acordos com os vizinhos no SimCity 4

#### Os seis tipos de contratos regionais

O SimCity 4 suporta seis tipos de contratos com cada cidade vizinha: compra e venda de eletricidade, de água e de serviços de eliminação de lixo.

No entanto, **os contratos de lixo funcionam no sentido inverso ao da eletricidade e da água**.

Quando envia lixo para uma vizinha, paga a essa cidade para o eliminar. Quando aceita o lixo de uma vizinha, essa cidade paga-lhe a si.

| Recurso | A sua cidade paga | A sua cidade recebe o pagamento |
|---|---|---|
| **Eletricidade** | Comprar eletricidade a uma vizinha | Vender eletricidade a uma vizinha |
| **Água** | Comprar água a uma vizinha | Vender água a uma vizinha |
| **Lixo** | Enviar o seu lixo a uma vizinha para eliminação | Aceitar o lixo de uma vizinha para eliminação |

**O que significa isto para a sua cidade?**

- **Comprar eletricidade ou água** reduz o que a cidade tem de produzir localmente.
- **Vender eletricidade ou água** exige capacidade de produção suficiente para abastecer a sua cidade e a cidade vizinha.
- **Exportar lixo** reduz a quantidade que o sistema de saneamento tem de tratar, mas custa dinheiro.
- **Importar lixo** rende dinheiro, mas aumenta a quantidade que o sistema de saneamento tem de tratar.

O jogo permite **um contrato de cada tipo com cada cidade vizinha**.

As quantidades dos contratos usam as mesmas unidades mensais apresentadas na janela de Acordos com os vizinhos do jogo. Os preços e os pagamentos exprimem-se em simoleões (§) por mês simulado.

#### Que ligações são necessárias?

Os contratos regionais exigem ligações adequadas entre as cidades vizinhas.

A ligação depende do recurso:

| Recurso | Ligação necessária |
|---|---|
| **Eletricidade** | Uma linha elétrica que atravesse a fronteira da cidade |
| **Água** | Uma conduta de água que atravesse a fronteira da cidade |
| **Lixo** | Uma estrada ou ligação de transporte adequada, reconhecida pelo sistema de contratos do jogo |

Para a eletricidade e a água, a regra é simples: a rede em causa tem de atravessar a fronteira regional.

As ligações de lixo são mais complicadas porque o SimCity 4 efetua verificações internas diferentes.

**O que ficou confirmado sobre as ligações de lixo?**

A verificação de presença de ligação regional do jogo reconhece:

- Estradas.
- Caminhos de ferro.

Essa verificação em particular não reconhece as autoestradas como ligações de lixo.

No entanto, o jogo usa também uma consulta separada ao verificar cada cidade vizinha e cada contrato. Essa consulta examina vários tipos de ligação, incluindo autoestradas, avenidas, estradas e caminhos de terra batida.

**Estas duas verificações não são a mesma.**

A presença de um determinado tipo de estrada na segunda consulta não prova, por si só, que consiga estabelecer um contrato de lixo elegível.

O Ministério apoia-se, por isso, nas verificações reais de ligação e de elegibilidade dos contratos do jogo, em vez de assumir que todas as ligações de transporte são válidas.

**E se uma ligação for perdida?**

Numa venda, a ligação da cidade local tem de se manter ativa.

Numa compra, a ligação correspondente do lado da cidade vizinha tem de existir e estar ativa.

Se um contrato perder a ligação necessária, o SimCity 4 pode assinalar o problema durante a manutenção mensal dos contratos.

**Um contrato não pode ser considerado fiável só porque continua a aparecer na lista.**

#### Que informação tem o jogo sobre as cidades vizinhas?

O SimCity 4 mantém um pequeno registo regional de cada cidade.

Para cada recurso, esse registo inclui:

| Informação | O que representa |
|---|---|
| **Produzido** | O recurso produzido localmente. |
| **Importado** | A quantidade comprada ou recebida de cidades vizinhas. |
| **Procurado** | A necessidade própria da cidade desse recurso. |
| **Exportado** | A quantidade comprometida com cidades vizinhas. |
| **Custo unitário** | O custo de referência do jogo para esse recurso. |
| **Tesouraria** | O dinheiro disponível da cidade. |

**Uma limitação importante: as cidades vizinhas não são simuladas enquanto joga a sua cidade atual.**

O SimCity 4 usa a informação gravada mais recentemente dessas vizinhas.

Por exemplo, imagine-se que a cidade vizinha teve em tempos um grande excedente de eletricidade. Se mais tarde alterar as suas centrais elétricas mas não gravar essa cidade, a informação regional pode não refletir as alterações mais recentes.

O Ministério tem de trabalhar com os registos das cidades vizinhas que o SimCity 4 disponibiliza.

**Quando é que estes registos são atualizados?**

O jogo atualiza a informação regional da cidade atual:

- Nas atualizações mensais.
- Quando são pedidas propostas de contratos regionais, por exemplo quando abre a janela de Acordos com os vizinhos.

No caso da eletricidade, *Produzido* representa a capacidade de geração com o financiamento atual das centrais, enquanto *Procurado* usa a necessidade mensal de eletricidade do jogo, que pode ser ligeiramente superior ao consumo real.

**Importado e Exportado representam entregas medidas?**

Não necessariamente.

Nos registos regionais, estes valores baseiam-se em **quantidades mensais contratadas**, e não numa medição do que se deslocou fisicamente nesse dia.

Por exemplo, um contrato para vender 5.000 unidades de eletricidade pode contar como 5.000 unidades exportadas nos valores regionais logo que o contrato existe.

O jogo conta normalmente a quantidade total do contrato nos dois lados, salvo se o contrato estiver cancelado, inválido ou ainda não incluído nos registos.

Esta distinção é importante quando o Ministério calcula se pode ser oferecido outro contrato.

<a id="neighbor-deals-max"></a>
#### Quanto podem negociar as cidades vizinhas?

O SimCity 4 não permite comércio ilimitado.

A quantidade máxima depende de duas coisas:

1. **Quanto excedente tem a cidade vendedora.**
2. **Quanto abastecimento adicional a cidade compradora pode receber.**

O máximo real é o mais baixo desses dois limites.

**Quantidade máxima do contrato (jogo base)**

```math
Q_{\max} =
\max\left(
0,\min\left(S_{\mathrm{surplus}},B_{\mathrm{need}}\right)
\right)
```

Onde:

- **Qmax** é a quantidade máxima que o jogo permite para esse contrato.
- **Ssurplus** é o excedente disponível da cidade vendedora.
- **Bneed** é o abastecimento adicional permitido à cidade compradora.

O jogo calcula estes valores a partir dos registos regionais das cidades.

**Margem de abastecimento adicional da cidade compradora**

```math
B_{\mathrm{need}} =
F \times
\max\left(
B_D+B_E,\frac{M}{F}+1
\right)
-(B_P+B_I)+q_{\mathrm{cur}}
```

**Excedente disponível da cidade vendedora**

```math
S_{\mathrm{surplus}} =
(S_P+S_I)-(S_D+S_E)+q_{\mathrm{cur}}
```

Onde:

- **P** significa Produzido.
- **I** significa Importado.
- **D** significa Procurado.
- **E** significa Exportado.
- **S** identifica a cidade vendedora.
- **B** identifica a cidade compradora.
- **qcur** é a quantidade de um contrato existente deste tipo entre as duas cidades, ou zero se não existir nenhum contrato.
- **F** é o fator de sobreprocura do comprador no jogo, **2,5** por predefinição.
- **M** é a quantidade mínima de um contrato no jogo, **1.000 unidades** por predefinição.

Todas as quantidades de recursos são unidades mensais.

**O que significa o fator de 2,5?**

O SimCity 4 permite que um comprador tenha um abastecimento contratado e local equivalente a até **2,5 vezes a sua própria procura mais os compromissos de exportação**, sujeito aos outros limites do jogo.

Trata-se de um limite máximo, não de uma obrigação de comprar tanto.

O vendedor, por seu lado, só pode oferecer o excedente que resta depois de contabilizadas as suas próprias necessidades e os compromissos existentes.

**Exemplo: vender eletricidade a uma cidade vizinha**

Imagine-se que a sua cidade tem:

- Produção de eletricidade: **14.000 unidades**.
- Necessidade de eletricidade: **10.300 unidades**.
- Nenhum contrato de eletricidade existente.

A cidade vizinha tem:

- Produção de eletricidade: **3.000 unidades**.
- Necessidade de eletricidade: **6.000 unidades**.
- Nenhum contrato existente.

**Passo 1: Calcular o seu excedente**

```math
S_{\mathrm{surplus}}=14000-10300=3700
```

A sua cidade tem **3.700 unidades** disponíveis para vender.

**Passo 2: Calcular a margem de compra da vizinha**

```math
B_{\mathrm{need}}=
2.5\times6000-3000=12000
```

A vizinha pode receber até **12.000 unidades adicionais** segundo a fórmula do jogo.

**Passo 3: Determinar o contrato máximo**

```math
Q_{\max}=\min(3700,12000)=3700
```

**O SimCity 4 permite um contrato até 3.700 unidades por mês.**

Embora a vizinha pudesse aceitar mais, a sua cidade não tem atualmente mais excedente para oferecer.

| Limitação | Quantidade |
|---|---:|
| Excedente da sua cidade | 3.700 unidades |
| Margem de compra da vizinha | 12.000 unidades |
| **Contrato máximo permitido** | **3.700 unidades** |

Se a sua cidade aumentar a capacidade de geração, a quantidade de contrato permitida pode também aumentar.

No entanto, só a fórmula do jogo não garante que um contrato venha a ser oferecido. É preciso satisfazer condições adicionais.

**O que acontece quando já existe um contrato?**

Ao calcular quanto um contrato existente pode ser renegociado, o jogo volta a somar a sua quantidade atual aos dois lados do cálculo.

Isto impede que o contrato existente reduza artificialmente o seu próprio máximo.

Por exemplo, se já vende 2.000 unidades, o jogo ainda consegue avaliar se o contrato pode ser aumentado ou ajustado, em vez de tratar as 2.000 unidades como permanentemente indisponíveis para renegociação.

O jogo não arredonda a quantidade máxima calculada do contrato.

Além disso, **o mínimo de 1.000 unidades serve para decidir se uma proposta pode ser oferecida**. Não é aplicado automaticamente como passo de arredondamento à quantidade máxima.

<a id="neighbor-deals-offer"></a>
#### Quando é que o SimCity 4 oferece um contrato regional?

Mesmo quando duas cidades têm ligações compatíveis e recursos suficientes para negociar, o jogo pode não oferecer um novo contrato.

O SimCity 4 verifica também se **a cidade pagadora tem dinheiro suficiente para suportar o acordo**.

A quantidade mínima de um contrato no jogo original é de **1.000 unidades por mês**.

O jogo exige que a cidade pagadora tenha:

- Uma tesouraria superior a **§5.000**.
- Dinheiro suficiente para pagar o preço mensal do contrato mínimo.
- Dinheiro suficiente para cobrir **mais de 60 pagamentos mensais** a esse preço.

A última condição corresponde a **cinco anos de pagamentos em tempo de jogo simulado**.

**Requisitos de uma proposta de contrato (jogo base)**

```math
\text{oferecido}\iff
Q_{\max}\ge M
\;\land\;
T>T_{\min}
\;\land\;
T>P(M)
\;\land\;
T>60\times P(M)
```

Onde:

- **Qmax** é a quantidade máxima permitida do contrato.
- **M** é a quantidade mínima do contrato, normalmente 1.000 unidades.
- **T** é a tesouraria da cidade pagadora.
- **Tmin** é a tesouraria mínima exigida, normalmente §5.000.
- **P(M)** é o preço mensal de um contrato de tamanho mínimo.
- **60** é o número de pagamentos mensais que o jogo exige que o comprador consiga suportar.

A comparação é estrita para os valores da tesouraria: ter exatamente o montante exigido não chega.

Em contrapartida, uma quantidade máxima de contrato de exatamente 1.000 unidades satisfaz a condição da quantidade mínima.

**Quem precisa do dinheiro?**

Depende do contrato.

| Contrato | Cidade cuja tesouraria é verificada |
|---|---|
| A sua cidade compra eletricidade | A sua cidade |
| A sua cidade vende eletricidade | A vizinha |
| A sua cidade compra água | A sua cidade |
| A sua cidade vende água | A vizinha |
| A sua cidade exporta lixo | A sua cidade |
| A sua cidade importa lixo | A vizinha |

Lembre-se de que as exportações de lixo são um serviço que compra, enquanto as importações de lixo são um serviço que presta.

**Exemplo: por que razão um contrato pode não aparecer**

Suponha-se que o preço mensal mínimo do contrato é de **§200**.

O jogo exige que a cidade pagadora tenha mais de:

```math
60\times200=12000
```

**§12.000**, além de satisfazer as outras condições.

| Tesouraria da cidade pagadora | Oferta de contrato |
|---|---|
| §5.000 | Não oferecido |
| §10.000 | Não oferecido |
| §12.000 | Não oferecido |
| **§12.001** | **Pode ser oferecido** |

Com §12.001, a condição da tesouraria fica satisfeita. O contrato continua a precisar de uma ligação válida, de uma quantidade de comércio suficiente e das restantes condições de elegibilidade do jogo.

**Por que razão pode um negócio potencialmente rentável continuar indisponível?**

Porque **a rentabilidade económica e a elegibilidade não são a mesma coisa**.

Uma cidade pode estar disposta a comprar eletricidade, por exemplo, mas o SimCity 4 não oferece o contrato enquanto o vendedor não tiver pelo menos 1.000 unidades de excedente disponível.

Um vendedor que pudesse aumentar a produção com mais financiamento não recebe automaticamente uma oferta baseada nessa capacidade futura hipotética.

A oferta só se torna possível quando os valores regionais do jogo mostram realmente excedente suficiente.

Do mesmo modo, uma cidade pode ter recursos suficientes mas não ter a tesouraria ou a ligação de rede necessárias.

**O Ministério tem de respeitar estas restrições do jogo original.** Não pode assumir que um contrato financeiramente atrativo está disponível antes de o SimCity 4 o considerar elegível.

<a id="neighbor-deals-pricing"></a>
#### Como são calculados os preços dos contratos regionais?

O SimCity 4 calcula os preços dos contratos com base no **custo médio de fornecimento do recurso pelo vendedor**.

Depois acrescenta uma pequena margem de lucro.

Isto aplica-se à eletricidade, à água e aos serviços de eliminação de lixo.

A distinção importante é que **o preço do contrato no jogo se baseia no custo médio, e não no custo adicional de fornecer esse contrato em particular**.

Essa diferença é central na forma como o Ministério decide que contratos vale a pena assinar.

#### Passo 1: O custo médio do vendedor

O jogo calcula primeiro quanto a cidade vendedora gasta por unidade de oferta disponível.

**Custo unitário do vendedor (jogo base)**

```math
u=\frac{E_{\mathrm{dept}}+C_{\mathrm{buy}}}{I+P+1}
```

Onde:

- **Edept** é a despesa mensal do vendedor com o departamento em causa.
- **Cbuy** é o que o vendedor paga para comprar o mesmo recurso a outras cidades.
- **I** é o recurso importado pelo vendedor.
- **P** é o recurso produzido localmente pelo vendedor.
- **u** é o custo médio por unidade resultante.

O **1** adicional no denominador faz parte da fórmula do jogo.

**Exemplo: uma cidade que vende eletricidade**

Suponha-se que a sua cidade produz 14.000 unidades de eletricidade e gasta §1.400 por mês a explorar as suas centrais elétricas.

Sem importações:

```math
u=\frac{1400}{14000+1}\approx0.09999
```

O custo médio de produção calculado pelo jogo é de aproximadamente **§0,10 por unidade**.

O jogo usa este valor como ponto de partida para o preço dos contratos de eletricidade.

Recalcula o custo unitário da cidade atual ao atualizar os registos regionais, incluindo as atualizações mensais e os pedidos de novas propostas de contratos.

#### Passo 2: A margem de lucro do jogo

O preço do contrato inclui uma margem acima do custo médio do vendedor.

Com as definições do jogo original, a margem varia aproximadamente entre **10% e 13%**.

O valor exato depende do abastecimento disponível do comprador e do excedente que sobrará ao vendedor depois do contrato.

**Preço mensal do contrato (jogo base)**

```math
P(q)=q\times\mathrm{clamp}\left(
u(1+m_{\min})+
\frac{g(V/q)+g(R/S_{\mathrm{sup}})}{2}
\,u(m_{\max}-m_{\min}),
\ u(1+m_{\min}),
\ u(1+m_{\max})
\right)
```

Com:

```math
\begin{aligned}
V &= 1+(B_P+B_I-q_{\mathrm{cur}})
-\max\left(B_D+B_E,\frac{M}{F}+1\right)\\
S_{\mathrm{sup}} &= \max(S_P+S_I,10^{-8})\\
R &= S_{\mathrm{sup}}-q-(S_D+S_E-q_{\mathrm{cur}})
\end{aligned}
```

Onde:

- **q** é a quantidade mensal proposta para o contrato.
- **u** é o custo unitário médio do vendedor.
- **mmin = 0,10** é o fator de lucro mínimo do jogo.
- **mmax = 0,50** é o fator de lucro máximo do jogo.
- **V** representa o abastecimento disponível do comprador para além do necessário.
- **R** representa o abastecimento que resta ao vendedor depois do contrato e dos outros compromissos.
- **Ssup** representa o total de produção e importações disponíveis do vendedor.
- **qcur** é a quantidade atual do contrato em avaliação, ou zero para um contrato novo.
- **g** é a curva de preços do jogo.

Para uma quantidade de contrato efetivamente igual a zero (abaixo de \(10^{-8}\)), o preço calculado é zero.

**A curva de preços usada pelo jogo**

| Entrada | Valor da curva |
|---:|---:|
| 0 | 0,00001 |
| 0,0075 | 0,0075 |
| 1 | 0,075 |

O jogo interpola entre estes pontos. Abaixo do primeiro ponto, o valor mantém-se em 0,00001; acima do último, mantém-se em 0,075.

Embora as definições gerais de margem do jogo permitam um intervalo muito maior, os valores pequenos produzidos por esta curva limitam a margem efetiva dos contratos.

**Com a afinação do jogo original, um contrato é avaliado entre aproximadamente 1,10 e 1,13 vezes o custo unitário médio do vendedor.**

A margem tende a ser:

- **Mais baixa** quando o comprador precisa de mais abastecimento ou o contrato usa a maior parte do excedente restante do vendedor.
- **Mais alta** quando o comprador já tem abastecimento de sobra e o vendedor mantém muita capacidade.

Na prática, a diferença é relativamente pequena.

#### Passo 3: O montante efetivamente cobrado

Depois de o jogo calcular o preço mensal do contrato, converte-o num número inteiro de simoleões.

**Custo mensal do contrato guardado (jogo base)**

```math
\text{custo}=\max\left(1,\lfloor P(q)\rfloor\right)
```

O valor resultante fica guardado no contrato e aparece no orçamento da cidade.

**Um contrato pode, assim, custar pelo menos §1 por mês, mesmo quando a sua quantidade é zero.**

Esta é uma das razões por que o Ministério remove contratos desnecessários de quantidade zero.

#### Exemplo: Preço das vendas de eletricidade

Regresse-se ao exemplo da eletricidade da secção anterior.

O vendedor tem um custo médio de eletricidade de aproximadamente **§0,09999 por unidade**.

O comprador vizinho tem abastecimento local insuficiente, pelo que a sua contribuição para o preço está no mínimo.

A fórmula de preços do jogo produz os seguintes valores:

| Quantidade do contrato | Rácio de abastecimento restante do vendedor | Valor da curva de preços | Preço relativo ao custo unitário | Preço mensal do contrato |
|---:|---:|---:|---:|---:|
| 1.200 | 0,1786 | 0,01913 | 1,1038 × | §132 |
| 2.400 | 0,0929 | 0,01331 | 1,1027 × | §264 |
| 3.600 | 0,0071 | 0,00714 | 1,1014 × | §396 |
| 4.800 | Abaixo de zero | 0,00001 | 1,1000 × | §527 |

<text color="secondary">A linha das 4.800 unidades ilustra a fórmula de preços para além do limite de contrato de 3.700 unidades calculado anteriormente. Não significa que o jogo permitiria esse contrato no estado original do exemplo.</text>

Repare-se que o preço por unidade varia ligeiramente com a quantidade do contrato.

**No entanto, este preço não nos diz se vender eletricidade é rentável para a cidade.**

Por exemplo, ganhar §396 por mês pode parecer compensador, mas e se fornecer essas 3.600 unidades adicionais exigir aumentar as despesas das centrais elétricas em §450?

O contrato daria prejuízo.

Inversamente, a cidade pode ter capacidade de geração por usar suficiente para fornecer o contrato com apenas §100 de despesas adicionais.

Nesse caso, seria rentável.

**O Ministério considera o custo operacional adicional real, e não apenas o preço médio de venda do jogo.**

<a id="neighbor-deals-ladder"></a>
#### Que quantidades de contrato permite o jogo?

O SimCity 4 calcula uma quantidade máxima possível para o contrato, mas a janela de Acordos com os vizinhos não permite escolher todos os números entre zero e esse máximo.

Em vez disso, oferece uma **lista de quantidades permitidas**.

Por exemplo, se o contrato máximo for de 3.700 unidades, a janela pode oferecer apenas:

**1.000, 2.000 ou 3.000 unidades.**

Não oferece necessariamente 3.700.

As quantidades disponíveis dependem de:

- A quantidade máxima atual.
- A quantidade mínima de negócio do jogo.
- Se o contrato é novo ou já está ativo.
- O incremento guardado num contrato existente.
- Se o contrato está suspenso.

#### Como é construída a lista de quantidades?

Para um contrato novo, o jogo divide normalmente a quantidade máxima em cerca de dez passos, respeitando o tamanho mínimo do contrato.

**Incremento de um contrato novo (jogo base)**

```text
maxQ = truncate(Qmax)
d10  = maxQ / 10       (integer division)
step = max(d10, M)
```

Onde **M** é a quantidade mínima de negócio, normalmente 1.000 unidades.

As quantidades oferecidas são múltiplos desse passo, até ao máximo permitido.

**Exemplo 1: quantidade máxima de 15.442 unidades**

```text
step = max(1544, 1000)
     = 1544
```

A janela oferece:

1.544; 3.088; 4.632; 6.176; 7.720; 9.264; 10.808; 12.352; 13.896; e 15.440 unidades.

**Exemplo 2: quantidade máxima de 3.700 unidades**

```text
step = max(370, 1000)
     = 1000
```

A janela oferece:

1.000; 2.000; e 3.000 unidades.

O máximo exato de 3.700 não está disponível como quantidade selecionável.

#### Os contratos existentes têm regras adicionais

Quando já existe um contrato, o SimCity 4 tenta preservar a sua quantidade atual nas opções disponíveis.

Usa também o incremento guardado no contrato.

Se o intervalo disponível ficar muito maior, o jogo pode construir uma lista de dois níveis, com passos menores perto da quantidade atual e passos maiores acima dela.

**Exemplo**

Considere-se um contrato ativo com:

- Quantidade atual: 2.000 unidades.
- Incremento guardado: 1.000 unidades.
- Novo máximo: 30.000 unidades.

A janela oferece as quantidades menores de **1.000 e 2.000**, seguidas de passos maiores:

**3.000; 6.000; 9.000; 12.000; 15.000; 18.000; 21.000; 24.000; 27.000; e 30.000.**

Isto permite ajustar um contrato existente sem obrigar o jogador a percorrer dezenas de passos pequenos.

**Regra completa da lista de quantidades (jogo base)**

```text
maxQ = trunc(Qmax)
       (if maxQ <= 0: use the current amount)

d10  = maxQ / 10
step = max(d10, M)

New proposal:
    Inc = step

Active contract:
    if (Inc == Amount and Inc > M) or Inc == 0:
        Inc = d10

    step     = Inc
    selected = Amount / Inc

Suspended contract:
    only the current amount

Q' = max(maxQ, step * selected, step)

If active, Amount > 0, and Q' / Inc > 15:
    Inc2 = Q' / 10

    entries =
        multiples of Inc up to Amount
        plus multiples of Inc2 above Amount, up to Q'
Otherwise:
    entries = multiples of step up to Q'
```

Todas as divisões truncam para inteiros.

A seleção **zero ou «sem acordo» do jogo cancela um contrato**.

Um contrato suspenso não pode ser redimensionado através da lista normal de quantidades.

**O Ministério respeita as escolhas de quantidade legais do jogo.** Não inventa quantidades de contrato arbitrárias só porque outro número seria mais rentável.

<a id="neighbor-deals-accounting"></a>
#### Estados dos contratos: ativo, pendente e suspenso

Um contrato regional pode estar em vários estados diferentes.

| Estado | O que significa |
|---|---|
| **Pendente** | Existe uma proposta, mas o contrato ainda não está ativo. |
| **Ativo** | O contrato está atualmente em vigor. |
| **Suspenso** | O jogo interrompeu temporariamente o contrato por causa de um problema. |
| **Cancelado** | O contrato foi terminado. |
| **Inválido** | O contrato não cumpre uma condição de elegibilidade, como a falta de uma ligação ou capacidade insuficiente. |
| **Suspensão terminada** | O contrato chegou ao fim do seu período de suspensão. |

O SimCity 4 verifica os contratos durante a sua manutenção mensal.

Por exemplo, o jogo pode assinalar um contrato quando:

- A tesouraria da cidade pagadora deixa de ser superior ao custo mensal do contrato.
- A ligação regional necessária desapareceu.
- A cidade já não consegue satisfazer o requisito de capacidade em causa.

**Um contrato em falha é normalmente suspenso antes de o jogo o terminar definitivamente.**

A suspensão funciona como um período de tolerância.

Enquanto está suspenso, o contrato não pode ser redimensionado nem cancelado através das operações normais de contratos. O jogo gere a sua resolução posterior.

#### Como são contabilizados os contratos no abastecimento regional?

Os valores regionais e o orçamento financeiro nem sempre tratam os contratos da mesma forma.

**Quantidades regionais de recursos**

O jogo conta normalmente a quantidade total de um contrato no total Importado do comprador e no total Exportado do vendedor, incluindo propostas pendentes e contratos suspensos.

Os contratos cancelados e inválidos ficam excluídos.

**Receitas e despesas mensais dos contratos**

Os cálculos financeiros do jogo incluem os contratos ativos e suspensos, mas excluem os pendentes, cancelados e inválidos.

| Estado do contrato | Contado nas quantidades regionais | Contado nas receitas ou despesas mensais do contrato |
|---|---|---|
| Pendente | Sim | Não |
| Ativo | Sim | Sim |
| Suspenso | Sim | Sim |
| Cancelado | Não | Não |
| Inválido | Não | Não |

**Por que razão é isto importante?**

Uma proposta pendente pode afetar os valores de abastecimento regional do jogo antes de gerar qualquer pagamento financeiro.

Da mesma forma, um contrato suspenso pode continuar representado na contabilidade regional, mesmo que o seu serviço possa não ser fiável.

**O Ministério distingue entre um contrato estar registado e o seu abastecimento ser realmente utilizável.**

Isto evita que dependa de compras de eletricidade ou de água suspensas só porque as suas quantidades continuam a aparecer nos totais da região.

<a id="neighbor-deals-delivery"></a>
#### Quando são entregues os recursos contratados?

A eletricidade, a água e o lixo não seguem as mesmas regras de entrega.

**Eletricidade e água**

Estes contratos são tratados como compromissos mensais de abastecimento.

A quantidade contratada conta como abastecimento para o comprador e como obrigação para o vendedor enquanto o contrato estiver em vigor, independentemente da quantidade que for realmente consumida.

**Lixo**

Os contratos de lixo funcionam através de entregas diárias.

**Entrega diária de um contrato de lixo (jogo base)**

```math
\text{passo diário}=
\max\left(1,\mathrm{round}(0.1\times A)\right)
```

A entrega diária continua enquanto:

```math
A-U>0
```

Onde:

- **A** é a quantidade mensal contratada.
- **U** é a quantidade já entregue durante o mês.

O jogo repõe a utilização mensal na mudança de mês.

Consequentemente, **a maior parte dos contratos de lixo entrega toda a sua quantidade mensal durante os primeiros dez dias simulados, aproximadamente**.

**Exemplo: um contrato de lixo de 25.004 unidades**

A quantidade diária é:

```math
\mathrm{round}(25004\times0.1)=2500
```

Passados dez dias, o jogo entregou:

```math
10\times2500=25000
```

Só restam quatro unidades, mas o jogo não reduz a última entrega para quatro.

Em vez disso, faz outra entrega de 2.500 unidades no décimo primeiro dia.

**O total passa a ser 27.500 unidades**, excedendo a quantidade mensal nominal do contrato.

Isto é particularmente importante quando a sua cidade recebe lixo de uma vizinha.

**Os cálculos de saneamento do Ministério não podem subestimar o lixo importado só porque a quantidade contratual é ligeiramente inferior à que o jogo realmente entrega.**

#### Quanto lixo pode a sua cidade receber?

O SimCity 4 limita também o total de lixo que uma cidade pode aceitar das vizinhas.

Este limite depende sobretudo de:

- O armazenamento disponível nos aterros.
- A produção de lixo da própria cidade.
- O lixo já importado.
- A afinação da duração dos contratos no jogo.

**Lixo recebido máximo (jogo base)**

```math
\sum A_{\mathrm{received}}
\le
L_{\mathrm{free}}
\left(1-\frac{1}{60+0.000107}\right)
+G_P+G_I
```

Onde:

- **Lfree** é o armazenamento disponível nos aterros da cidade.
- **GP** é a produção de lixo registada da cidade.
- **GI** são as importações de lixo registadas da cidade.
- **60** representa o prazo de cinco anos dos contratos no jogo, medido em meses.

**Exemplo**

Suponha-se que uma cidade tem:

- Armazenamento livre nos aterros: 500.000 unidades.
- Lixo produzido: 40.000 unidades.
- Lixo importado: 10.000 unidades.

A margem máxima de lixo recebido do jogo é de aproximadamente:

```math
500000
\left(1-\frac{1}{60+0.000107}\right)
+40000+10000
\approx541667
```

**Lixo recebido máximo permitido: aproximadamente 541.667 unidades.**

Se o total recebido através de contratos exceder este limite, o jogo assinala os contratos em excesso como tendo capacidade insuficiente.

Esses contratos são então suspensos, seguindo a ordem da lista de contratos do jogo.

**O Ministério tem de respeitar este limite mesmo quando aceitar lixo adicional pareça de outro modo rentável.**

<a id="neighbor-deals-problem"></a>
### 2. O problema da gestão

Gerir manualmente os contratos regionais é difícil, porque várias decisões afetam-se umas às outras.

#### Um contrato com ar rentável pode, na realidade, dar prejuízo

O SimCity 4 baseia os preços de venda no **custo médio**.

Mas o valor económico de um contrato depende do **custo marginal**: o que a cidade tem de gastar a mais, ou o que pode poupar, por causa desse contrato específico.

Por exemplo, vender eletricidade por §400 por mês não compensa se exigir mais §500 em despesas das centrais elétricas.

O jogo não mostra esta comparação diretamente.

#### As ofertas de contratos mudam constantemente

Um contrato que está disponível hoje pode desaparecer quando:

- A procura da sua cidade muda.
- O abastecimento registado de uma cidade vizinha muda.
- Uma central perde capacidade de produção.
- Uma cidade deixa de ter dinheiro suficiente.
- Outro contrato usa o excedente disponível.

As quantidades legais também mudam com a capacidade disponível.

#### Várias vizinhas competem pelos mesmos recursos

Imagine-se que a sua cidade pode vender 5.000 unidades de eletricidade.

Uma vizinha propõe comprar 3.000 unidades, enquanto outra propõe comprar 4.000.

**A sua cidade não pode necessariamente aceitar os dois contratos**, porque o total excederia o abastecimento disponível.

O Ministério tem de comparar as combinações possíveis em vez de escolher cada contrato separadamente.

#### As compras afetam as necessidades de produção local

Comprar eletricidade ou água pode reduzir o que as suas próprias instalações precisam de produzir.

Com as regras de reserva do Ministério, isto pode criar poupanças superiores ao que a quantidade comprada, por si só, sugeriria.

#### O cálculo de excedente do jogo não protege a sua reserva

Os próprios limites de contratos do SimCity 4 não incluem a reserva de segurança adicional do Ministério.

O jogo pode permitir uma venda que deixe pouca capacidade de eletricidade ou de água de reserva para as necessidades da sua cidade.

**O Ministério tem de aplicar os seus próprios requisitos de reserva e de serviço por cima dos limites legais do jogo.**

#### Os contratos de lixo dependem do calendário diário

Duas combinações de contratos de lixo podem ter a mesma quantidade mensal total mas calendários de entrega diferentes.

Isso pode alterar se o sistema de saneamento consegue limpar o lixo do mês.

**O Ministério avalia, por isso, combinações completas de contratos de lixo usando a simulação diária do saneamento, e não apenas os totais mensais.**

<a id="neighbor-deals-algorithm"></a>
### 3. Como o Ministério gere os contratos regionais

O Ministério combina a seleção de contratos com os optimizadores de recursos locais.

Em vez de considerar cada cidade vizinha de forma independente, procura **a combinação de contratos que dá o maior benefício económico global, protegendo os serviços da própria cidade**.

Faz uma optimização separada para a eletricidade, a água e o lixo.

#### O Ministério assina contratos automaticamente?

**Sim.**

Quando a gestão automática dos acordos com os vizinhos está ativa, o Ministério pode:

- Criar novos contratos.
- Aumentar ou reduzir as quantidades dos contratos existentes.
- Cancelar contratos que já não sejam úteis.

Executa estas operações diretamente através das funções normais de contratos do jogo.

Para um contrato novo, pede ao SimCity 4 as propostas disponíveis, ativa a proposta escolhida e define a respetiva quantidade.

Para um contrato existente, usa as operações normais do jogo para alterar a quantidade ou cancelar o contrato.

**Não é preciso abrir a janela dos Acordos com os vizinhos nem aceitar os contratos manualmente.**

O Ministério não modifica as cidades vizinhas, não reescreve ficheiros de gravação, não altera datas de contratos nem obriga o jogo a aceitar contratos que normalmente não ofereceria.

**E os contratos que criou pessoalmente?**

Quando o Ministério gere um recurso, os contratos criados manualmente entram na sua optimização.

Pode redimensioná-los ou cancelá-los se outra combinação legal for melhor.

Se quiser manter o controlo manual total dos contratos de um recurso em particular, desative a opção de gestão automática de acordos desse recurso.

#### Com que frequência os contratos são revistos?

Os contratos são normalmente revistos em conjunto com o recurso correspondente.

| Recurso | Revisão regular |
|---|---|
| **Eletricidade** | De 10 em 10 dias simulados, com revisões mais cedo em caso de alterações significativas ou de faltas de fornecimento confirmadas. |
| **Água** | De 10 em 10 dias simulados desde a última revisão da água, em cada mudança de mês e mais cedo quando necessário. |
| **Lixo** | Em cada mudança de mês e quando os valores de saneamento monitorizados mudam de forma significativa. |

Uma verificação diária leve procura também faltas reais e alterações significativas da procura, da produção ou dos contratos regionais.

Cada avaliação parte do estado atual da cidade.

**Normalmente, o Ministério não impõe um período de espera adicional só porque alterou um contrato recentemente.**

Existem proteções específicas contra alterações desnecessárias de contratos, explicadas mais abaixo e na reavaliação de contratos.

#### Passo 1: Encontrar todas as opções de contrato disponíveis

O Ministério começa por examinar as cidades vizinhas com ligações adequadas e registos regionais completos.

Para cada recurso e sentido de comércio, identifica as quantidades de contrato legais.

**Contratos existentes**

A quantidade atual é incluída, juntamente com as outras quantidades permitidas pelo jogo.

**Novas compras de eletricidade e de água**

O Ministério usa as quantidades que a lista de propostas do jogo permite atualmente.

Exclui as compras cujo preço mensal não ficaria abaixo da tesouraria disponível da cidade local.

**Novas vendas de eletricidade e de água**

A cidade vizinha tem de já constar das ofertas disponíveis do jogo.

No entanto, algumas quantidades de venda maiores só podem tornar-se possíveis **depois de o Ministério aumentar a capacidade de produção local**.

O Ministério pode considerá-las *candidatas de venda latentes*.

Por exemplo, imagine que a cidade tem neste momento apenas 2.000 unidades de eletricidade excedentárias, mas poderia produzir mais 3.000 unidades se aumentasse o financiamento das centrais.

Um contrato maior poderá tornar-se possível quando essa capacidade estiver efetivamente disponível.

O Ministério pode avaliar se o custo de produção adicional se justificaria.

**Um contrato latente só é assinado quando as regras de elegibilidade reais do jogo o permitirem.**

**Lixo recebido dos vizinhos**

O Ministério considera as quantidades permitidas pela lista de propostas atual do jogo.

Tem também de verificar que a cidade consegue tratar o lixo adicional.

#### Porque é que o Ministério usa uma base de preços comum?

Ao avaliar alternativas de contratos, o Ministério começa por retirar dos cálculos de preços os seus próprios contratos ajustáveis.

Avalia depois as quantidades candidatas em relação a esta base comum.

Isto evita uma situação indesejável em que alterar um contrato modifica os registos regionais, o que muda o preço calculado, o que faz o Ministério preferir de imediato o arranjo anterior.

**A mesma quantidade candidata não deve parecer mais ou menos atrativa só porque o Ministério acabou de escrever um contrato equivalente.**

Ao avaliar carteiras completas, os preços são calculados com a combinação de contratos selecionada.

Os candidatos sem informação fiável suficiente para calcular o preço são excluídos. Os valores em falta nunca são tratados silenciosamente como zero.

<a id="neighbor-deals-objective"></a>
#### Passo 2: Encontrar a combinação mais rentável

O Ministério não se limita a maximizar o rendimento dos contratos.

Considera em conjunto **os rendimentos, as despesas dos contratos e o custo de abastecer os serviços da própria cidade**.

**Objetivo da carteira de contratos regionais**

```math
\max_x\ \mathrm{Net}(x)=
\sum_{i\in\mathrm{sell\text{-}like}}P_i(x_i\mid x)
-
\sum_{j\in\mathrm{buy\text{-}like}}P_j(x_j\mid x)
-
C_{\mathrm{local}}(x)
```

Em que:

- **x** representa as quantidades escolhidas para todos os contratos candidatos de um recurso.
- **Pi** é o rendimento de um contrato de tipo venda.
- **Pj** é a despesa de um contrato de tipo compra.
- **Clocal(x)** é o custo de funcionamento local mais baixo que consegue sustentar em segurança os contratos escolhidos.
- **Net(x)** é o valor económico mensal resultante.

O preço de cada contrato é calculado no contexto da carteira completa selecionada.

**O que está incluído no custo local?**

Depende do recurso:

| Recurso | Custo local considerado |
|---|---|
| **Eletricidade** | Financiamento das centrais elétricas necessário para satisfazer as necessidades locais, a reserva e as exportações, depois das importações. |
| **Água** | Financiamento do departamento de água necessário para satisfazer as necessidades locais, a reserva e as exportações, depois das importações. |
| **Lixo** | Financiamento do saneamento necessário para tratar o lixo da cidade, as importações e exportações escolhidas e a reserva configurada. |

O Ministério usa os mesmos cálculos descritos nos capítulos anteriores.

Quando **Ter em conta o desgaste das centrais** está ativa, o custo económico local da eletricidade inclui também o desgaste estimado das centrais.

Esse desgaste serve para comparar opções, mas **não é uma despesa adicional da tesouraria**.

#### Exemplo: Comparar carteiras de contratos

Considere uma cidade ilustrativa com três arranjos possíveis:

| Arranjo | Rendimento dos contratos | Despesa dos contratos | Custo de funcionamento local | Líquido |
|---|---:|---:|---:|---:|
| Só produção local | §0 | §0 | §1.000 | −§1.000 |
| Comprar a um vizinho | §0 | §300 | §600 | −§900 |
| Vender a outro vizinho | §400 | §0 | §1.200 | −§800 |

Neste exemplo simplificado, vender dá o melhor resultado económico, apesar de os custos de funcionamento locais aumentarem.

O Ministério preferiria esse arranjo, **desde que todas as condições legais e de segurança sejam satisfeitas**.

Os valores são ilustrativos; o planeador real usa os preços dos contratos do jogo e os cálculos de financiamento específicos de cada recurso.

**Como são tratados os resultados iguais?**

Se duas carteiras legais tiverem o mesmo valor económico, o Ministério prefere:

1. A carteira com **menos contratos**.
2. Se continuar o empate, a carteira que exija **menos alterações**.

Isto evita contratos e ajustes desnecessários que não melhoram o resultado.

#### As regras de segurança que toda a carteira tem de cumprir

O Ministério só considera combinações de contratos que respeitem as restrições seguintes.

**1. Os serviços da sua cidade vêm primeiro**

O plano de produção local ou de saneamento tem de continuar a ser viável com os contratos escolhidos.

Uma carteira que os serviços da própria cidade não consigam sustentar em segurança é rejeitada.

**2. As compras não podem exceder a procura da própria cidade**

O Ministério não compra eletricidade nem água só para as revender.

Também não compra eletricidade ou água adicionais apenas para criar a reserva configurada.

**3. Normalmente, um recurso é comprado ou vendido**

Por predefinição, o Ministério escolhe uma de três posições para cada recurso:

- **LOCAL:** Sem compras nem vendas.
- **COMPRA:** Comprar o recurso.
- **VENDA:** Vender o recurso.

Comprar e vender o mesmo recurso em simultâneo não é permitido na configuração predefinida.

A opção **Comprar a uma cidade para vender a outra** é explicada nas definições dos contratos.

**4. As importações de lixo têm de ficar dentro de limites seguros**

O total de lixo recebido não pode exceder o limite de lixo recebido do jogo nem a quantidade adicional que o sistema de saneamento da cidade consegue tratar.

**5. Os contratos suspensos são fixos**

Um contrato suspenso não pode ser alterado através das funções normais do jogo.

O Ministério inclui a sua quantidade atual nos cálculos, mas não tenta redimensioná-lo nem cancelá-lo.

**6. Um défice confirmado de eletricidade ou de água impede vendas**

Se os contadores do jogo confirmarem uma falta regional genuína, o Ministério não mantém vendas desse recurso.

**O abastecimento essencial da própria cidade tem prioridade sobre o rendimento das exportações.**

#### Como são incluídas as reservas de eletricidade e de água?

O Ministério calcula a capacidade local que continua a ser necessária depois das importações e das exportações.

**Capacidade local necessária**

```math
N(B,S)=
\max\left(
\left\lceil R\times\max(0,D-B)\right\rceil,
R_q-B
\right)+S
```

Em que:

- **D** é a procura da própria cidade.
- **B** é a quantidade total comprada.
- **S** é a quantidade total vendida.
- **R** é o multiplicador da reserva.
- **Rq** é a necessidade mensal de eletricidade do próprio jogo.

Para a eletricidade, a reserva predefinida é de 20%, o que dá **R = 1,20**.

Para a água, a reserva predefinida é de 10%, o que dá **R = 1,10**.

**Para a água, o termo Rq é omitido**, porque o cálculo da água não usa o contador separado de necessidade de eletricidade.

**Exemplo: Eletricidade com uma reserva de 20%**

Suponha que a cidade tem:

- Procura: 10.000 unidades.
- Necessidade do jogo: 10.300 unidades.
- Reserva: 20%.

Sem contratos:

```math
N(0,0)=12000
```

Se comprar 2.000 unidades:

```math
N(2000,0)=
\max(9600,8300)=9600
```

Se, em vez disso, vender 2.400 unidades:

```math
N(0,2400)=14400
```

| Situação | Capacidade de geração local necessária |
|---|---:|
| Sem contratos | 12.000 |
| Comprar 2.000 unidades | 9.600 |
| Vender 2.400 unidades | 14.400 |

**Comprar 2.000 unidades reduz em 2.400 unidades a necessidade de capacidade local** neste exemplo, porque a reserva se aplica apenas ao que a cidade tem de produzir por si própria.

Vender 2.400 unidades aumenta a necessidade exatamente em 2.400.

O Ministério compara, por isso, os preços dos contratos com a variação real dos custos locais causada por estas necessidades de capacidade.

#### Porque é que o Ministério não pode comprar mais do que a cidade precisa?

O Ministério aplica um limite de compra adicional.

**Limite de compra**

```math
\sum_j x_j\le
\min\left(
D,\;
F\times\max\left(L_D+L_E,\frac{M}{F}+1\right)
-(L_P+L_I)
\right)
```

Em que:

- **D** é a procura da própria cidade.
- **F** é o fator de sobreprocura do comprador do jogo.
- **M** é a quantidade mínima do contrato.
- Os valores **L** são os registos regionais da cidade local, excluindo as suas próprias compras ajustáveis.

No caso do lixo, o limite de procura baseia-se na **produção de lixo da própria cidade menos a reciclagem**.

Isto impede o Ministério de:

- Comprar mais do que as suas próprias necessidades.
- Comprar recursos apenas para os revender.
- Comprar recursos adicionais para criar uma reserva sobre as importações.
- Usar o fornecimento comprado como base de novos compromissos de exportação.

Os limites legais de compra do próprio jogo são igualmente respeitados.

#### Como avalia o Ministério um contrato isolado?

O cálculo da carteira também se pode entender olhando para um único contrato.

**Numa venda**

O Ministério compara o pagamento recebido com o custo de funcionamento local adicional.

```math
\mathrm{Profit}(q)=
P(q)-
\left[
C_{\mathrm{local}}(N+q)
-
C_{\mathrm{local}}(N)
\right]
```

Por exemplo, se uma venda render §300 mas exigir mais §200 em despesas de produção:

**Benefício líquido: §100 por mês.**

A venda é rentável.

**Numa compra**

O Ministério compara o preço do contrato com a despesa local que evita.

```math
\mathrm{Saving}(q)=
\left[
C_{\mathrm{local}}(N(0))
-
C_{\mathrm{local}}(N(q))
\right]
-
P(q)
```

Por exemplo, se comprar eletricidade custar §250 mas permitir à cidade poupar §400 no financiamento das centrais:

**Poupança líquida: §150 por mês.**

A compra é rentável.

**Esta é a rentabilidade marginal:** o Ministério avalia o efeito económico real de acrescentar ou alterar um contrato.

Não presume que uma venda compensa só porque o jogo aplica uma margem ao custo médio do vendedor.

#### Passo 3: Procurar a melhor combinação de contratos

Depois de conhecidas as quantidades disponíveis e os requisitos de segurança, o Ministério compara as carteiras legais.

A pesquisa funciona de forma diferente para a eletricidade, a água e o lixo.

**Eletricidade e água**

O Ministério usa um algoritmo de optimização para comparar combinações de compras e vendas totais.

Para manter o cálculo eficiente, as quantidades são inicialmente agrupadas em **blocos de 100 unidades**.

Para intervalos de comércio muito grandes, o tamanho do bloco aumenta para limitar o cálculo.

Depois de escolher o melhor candidato, o Ministério verifica novamente a combinação completa com **quantidades exatas**, sem o agrupamento aproximado.

**Lixo**

O lixo exige um método diferente.

Como cada contrato tem o seu próprio calendário de entregas diárias, o Ministério não pode decidir em segurança apenas com totais mensais.

Avalia, por isso, combinações completas de contratos e executa o cálculo do financiamento do saneamento para cada uma.

**Todas as carteiras candidatas têm de ser testadas em relação ao processo previsto de eliminação do lixo.**

Esta pesquisa pode tornar-se dispendiosa quando há muitos vizinhos e quantidades disponíveis.

Se o número de combinações exceder **5 milhões num dos lados da pesquisa**, o Ministério considera o ótimo não comprovado.

**Não faz uma alteração especulativa de contratos quando não consegue estabelecer um resultado fiável.**

#### Passo 4: Remover contratos e alterações desnecessários

Depois de encontrar a carteira preferida, o Ministério faz várias verificações finais.

**1. Remover os contratos que não trazem benefício**

Se remover um contrato não piorar a carteira do ponto de vista económico, o Ministério abandona esse contrato.

Todo o contrato opcional tem de contribuir com um benefício estritamente positivo.

**2. Nunca substituir um arranjo existente melhor**

Se manter todos os contratos atuais for legal, viável e estritamente mais valioso do que o resultado proposto, o Ministério deixa os contratos como estão.

**3. Ignorar alterações opcionais insignificantes**

Uma definição INI avançada pode especificar o benefício mínimo exigido para alterações opcionais de contratos.

O limiar predefinido é **§0**, pelo que qualquer benefício estritamente positivo pode qualificar-se.

Quando se configura um limiar mais alto, o Ministério evita criações opcionais de contratos ou alterações de quantidade cujo benefício fique abaixo desse valor.

**As alterações necessárias para manter os serviços essenciais estão isentas deste limiar económico.**

**4. Remover contratos dormentes de quantidade zero**

O SimCity 4 pode, por vezes, deixar um contrato ativo com a quantidade zero.

Esse contrato não fornece nenhum recurso e continua a custar §1 por mês.

O Ministério cancela estes contratos desnecessários.

#### Como se mede o benefício de uma decisão sobre um contrato?

Quando vários contratos interagem, olhar apenas para o rendimento ou a despesa de um contrato pode dar a resposta errada.

Em vez disso, o Ministério pergunta:

**Quanto pior ficaria o melhor plano legal se este contrato em particular tivesse de continuar com a sua quantidade atual?**

**Benefício marginal de uma decisão sobre um contrato**

```math
\Delta_k=
\mathrm{Net}(x^*)
-
\max_{x:\,x_k=x_k^{\mathrm{now}}}
\mathrm{Net}(x)
```

Em que:

- **x\*** é a carteira ótima selecionada.
- **xk** é a quantidade do contrato que está a ser avaliado.
- **xk atual** é a sua quantidade atual.
- **Net** é o valor económico total da carteira.
- **Δk** é o benefício económico atribuível a permitir que esse contrato em particular mude.

O Ministério compara a carteira selecionada com a **melhor alternativa que mantém este contrato inalterado**, e não apenas com o estado global atual da cidade.

Isto importa porque outro vizinho pode oferecer um contrato igualmente bom.

Por exemplo, suponha que dois vizinhos podem fornecer eletricidade pelo mesmo custo económico total.

Criar um contrato com qualquer um deles pode ser rentável em comparação com produzir tudo localmente.

Mas se o outro vizinho puder dar o mesmo benefício, **nenhum dos contratos em particular é indispensável**.

O Ministério tem isso em conta ao decidir se uma alteração individual se justifica.

**A criação ou o aumento de um contrato só é normalmente escrita quando o seu benefício marginal é estritamente positivo.**

Há uma exceção: uma alteração necessária para manter o serviço quando nenhuma carteira legal consegue manter esse contrato na sua quantidade atual.

Nesse caso, o Ministério não inventa um benefício monetário. Reconhece que a alteração é necessária para satisfazer os requisitos do serviço.

#### Poupanças em numerário versus poupanças económicas

Para a eletricidade, o Ministério mantém duas versões do benefício de uma decisão sobre um contrato:

| Medida | O que inclui |
|---|---|
| **Benefício em numerário** | Variações nas despesas de funcionamento reais e nos rendimentos ou pagamentos dos contratos. |
| **Benefício económico** | O benefício em numerário mais o valor estimado do desgaste das centrais, quando o desgaste está ativo. |

**A decisão usa o benefício económico.**

O benefício em numerário descreve o efeito no orçamento real da cidade.

O desgaste estimado das centrais nunca é cobrado diretamente à tesouraria.

Esta distinção permite ao Ministério ter em conta a deterioração das centrais a longo prazo sem apresentar uma despesa mensal imaginária como dinheiro realmente gasto.

**O objetivo global mantém-se: escolher a combinação de contratos legais mais vantajosa do ponto de vista económico, sem comprometer os serviços essenciais da cidade, as reservas nem as proteções de segurança existentes.**
<a id="neighbor-deals-exclusive"></a>
#### Comprar ou vender: um sentido por predefinição

Por predefinição, o Ministério não compra e vende o mesmo recurso ao mesmo tempo.

Para cada recurso, escolhe um de três arranjos possíveis:

| Arranjo | O que o Ministério faz |
|---|---|
| **LOCAL** | Usa a produção ou a eliminação locais sem comprar nem vender esse recurso. |
| **COMPRA** | Compra o recurso a uma ou mais cidades vizinhas. |
| **VENDA** | Vende o recurso ou o serviço de eliminação a uma ou mais cidades vizinhas. |

Na eletricidade e na água, comprar e vender têm o seu significado habitual.

No lixo, os sentidos estão invertidos:

- **COMPRA:** A cidade paga a um vizinho para eliminar o seu lixo.
- **VENDA:** A cidade recebe lixo de um vizinho e é paga por o tratar.

Este comportamento é controlado por **Comprar a uma cidade para vender a outra**, que está **desligada por predefinição**.

#### E se a cidade já tiver contratos nos dois sentidos?

O Ministério compara os arranjos disponíveis e avança para o que dá o maior benefício económico global.

Por exemplo, se a cidade estiver a comprar e a vender eletricidade, o Ministério escolhe normalmente se mantém as compras ou as vendas e depois cancela os contratos desnecessários.

**Há uma exceção importante: os contratos suspensos.**

O SimCity 4 não permite que os contratos suspensos sejam alterados através das suas funções normais de contratos.

Suponha que a cidade tem uma venda de eletricidade suspensa, mas agora precisa de comprar eletricidade para evitar uma falta.

O Ministério não pode remover de imediato a venda suspensa.

Nessa situação, **o abastecimento de eletricidade da cidade tem prioridade sobre a restrição habitual de comprar ou vender**.

O Ministério pode comprar a eletricidade necessária para manter o serviço, evita criar vendas adicionais e regista o conflito de política no registo.

Não deixa que a regra de exclusividade deixe a cidade sem um recurso essencial.

<a id="neighbor-deals-arbitrage"></a>
#### Opcional: Comprar e vender ao mesmo tempo

Pode ativar **Comprar a uma cidade para vender a outra** para permitir que o Ministério considere contratos nos dois sentidos para o mesmo recurso.

Isto exige que **Gerir os acordos com os vizinhos** esteja ativa.

O Ministério só escolhe um arranjo misto quando este produz um resultado económico melhor do que todos os arranjos de sentido único disponíveis.

**Para que pode ser útil?**

Considere uma cidade com dois vizinhos.

O Vizinho A vende eletricidade relativamente barata, enquanto o Vizinho B está disposto a pagar mais por eletricidade.

A cidade poderá beneficiar de comprar eletricidade ao A e vender eletricidade ao B.

No entanto, **o Ministério não se limita a comprar eletricidade e a revender essas mesmas unidades importadas**.

Em vez disso, a compra reduz a quantidade de eletricidade que a cidade tem de produzir para os seus próprios edifícios.

Isso pode libertar capacidade de geração local para uma venda rentável.

**Exemplo: eletricidade**

Imagine o seguinte arranjo:

- A cidade precisa de 10.000 unidades de eletricidade.
- Compra 3.000 unidades ao Vizinho A.
- As suas próprias centrais fornecem a restante procura local e a reserva.
- A capacidade de geração local disponível suporta uma venda ao Vizinho B.

O Ministério avalia se o custo da compra, o rendimento da venda e as despesas resultantes das centrais produzem um melhor resultado global.

A reserva normal de eletricidade e todas as outras regras de segurança continuam a aplicar-se.

**E o lixo?**

O lixo também pode beneficiar de um arranjo misto.

Por exemplo:

- O Vizinho A paga à cidade para aceitar o seu lixo.
- A cidade paga ao Vizinho B para eliminar parte do seu próprio lixo.

Se o rendimento de receber lixo exceder os custos adicionais de saneamento e de exportação, a combinação pode compensar.

O Ministério avalia o calendário de eliminação completo para garantir que a cidade consegue tratar o lixo resultante.

#### Que restrições continuam a aplicar-se?

Ativar esta opção **não** remove as proteções normais.

- As compras não podem exceder a procura da própria cidade.
- A eletricidade ou a água importadas nunca são usadas como base de revenda.
- As reservas de eletricidade e de água configuradas continuam a ser obrigatórias.
- As importações de lixo têm de ficar dentro dos limites de eliminação permitidos.
- Todos os contratos têm de respeitar as regras de elegibilidade, quantidade e tesouraria do jogo.
- Os serviços locais têm de continuar a ser viáveis.
- Um recurso com a gestão automática de acordos desativada nunca é negociado pelo Ministério.

**Uma cidade vizinha não pode ser em simultâneo fornecedora e cliente da sua cidade para o mesmo recurso.**

Por exemplo, o Vizinho A pode vender eletricidade à cidade ou comprar-lhe eletricidade, mas não as duas coisas no mesmo plano.

Vizinhos diferentes podem ter papéis diferentes.

#### Como escolhe o Ministério o papel de cada vizinho?

Quando uma cidade vizinha poderia, potencialmente, comerciar em qualquer dos sentidos, o Ministério avalia as atribuições possíveis.

Até **quatro vizinhos** nestas condições, consegue avaliar todas as atribuições de forma exata.

Quatro vizinhos criam, no máximo:

```math
2^4=16
```

atribuições possíveis de compra ou venda.

Cada atribuição é depois avaliada com as regras normais de optimização de contratos.

Para evitar um cálculo excessivo, o Ministério recorre ao comércio de sentido único quando:

- Mais de **quatro vizinhos** precisam de uma atribuição de compra ou venda.
- A pesquisa mista de lixo excederia **4.000 combinações de compra por venda**.

Nessas situações, o Ministério usa a optimização normal de um só sentido nessa revisão.

**A opção alarga as escolhas económicas ao dispor do Ministério sem permitir comércio ilimitado nem contornar as regras de segurança.**

<a id="neighbor-deals-execution"></a>
#### Passo 4: Aplicar em segurança as alterações de contratos e de financiamento

Encontrar a melhor combinação de contratos é apenas parte do trabalho.

O Ministério tem também de aplicar as alterações por uma ordem que não interrompa os serviços da cidade.

Por exemplo, imagine que quer comprar eletricidade a um vizinho e reduzir o financiamento das centrais.

**Reduzir a produção antes de a compra estar disponível poderia causar um apagão.**

Do mesmo modo, vender água adicional antes de aumentar a produção local poderia deixar os edifícios da própria cidade sem fornecimento suficiente.

O Ministério coordena o financiamento local e os contratos regionais em seis fases de execução.

#### As seis fases de execução

| Fase | Ação | Porque vem nesta ordem |
|---|---|---|
| **1** | Aumentar a capacidade de produção local | Disponibilizar fornecimento local adicional antes de ser necessário. |
| **2** | Reduzir ou cancelar vendas | Libertar a capacidade atualmente comprometida com os vizinhos. |
| **3** | Criar ou aumentar compras | Estabelecer o fornecimento de substituição antes de reduzir a produção local. |
| **4** | Criar ou aumentar vendas | Iniciar novas exportações só depois de o fornecimento necessário estar disponível. |
| **5** | Reduzir a capacidade de produção local | Baixar o financiamento quando o novo arranjo já estiver sustentado. |
| **6** | Reduzir ou cancelar compras | Retirar o fornecimento importado apenas quando o arranjo restante conseguir sustentar a cidade. |

**O princípio orientador é que a transição tem de ser segura, e não apenas o arranjo final.**

Na eletricidade e na água, a necessidade de fornecimento local depende da procura da própria cidade, das vendas e das compras.

De forma simplificada:

```math
\text{Local need}=\text{Own need}+\text{Sales}-\text{Purchases}
```

O cálculo completo inclui também a reserva configurada e, na eletricidade, o contador de necessidade separado do jogo.

A ordem de execução garante que as alterações intermédias não exigem mais capacidade local do que o arranjo inicial ou o final.

#### Exemplo: Substituir a eletricidade local por uma compra

Suponha que as centrais da cidade fornecem neste momento tudo localmente.

O Ministério conclui que comprar eletricidade a um vizinho reduziria as despesas totais.

Procede por esta ordem:

1. Estabelece a nova compra de eletricidade.
2. Verifica que o jogo aceitou o contrato.
3. Recalcula a necessidade de capacidade local.
4. Reduz o financiamento das centrais quando for seguro fazê-lo.

**Não corta as centrais antes de confirmar o fornecimento alternativo.**

#### Exemplo: Substituir uma compra de eletricidade por produção local

Suponha agora que o contrário passa a ser mais económico.

A cidade quer deixar de comprar eletricidade e produzi-la localmente.

O Ministério:

1. Aumenta o financiamento das centrais necessário.
2. Verifica que a capacidade de geração local está disponível.
3. Reduz ou cancela a compra de eletricidade.

**Não cancela o fornecimento importado antes de existir a capacidade local de substituição.**

#### Há exceções à ordem das fases?

Sim. Duas alterações podem ser antecipadas para a Fase 2 quando o Ministério confirmou que não podem comprometer o serviço local:

- **Reduzir uma compra** quando a capacidade local confirmada da cidade já é suficiente sem ela.
- **Criar ou aumentar uma venda** que o jogo oferece atualmente, quando a capacidade local confirmada existente já é suficiente para a suportar.

Nestes casos, a capacidade já está disponível. Não é preciso esperar por um aumento de financiamento desnecessário.

#### O Ministério aplica as seis fases de uma só vez?

Não.

**Cada revisão executa apenas a fase pendente mais antiga de cada recurso, com uma alteração de contrato de cada vez.**

O Ministério lê depois o estado atualizado da cidade antes de continuar numa revisão posterior.

Isto impede que várias alterações não verificadas sejam aplicadas em conjunto.

#### O que acontece às vendas que exigem primeiro produção adicional?

Como explicado atrás, o Ministério pode identificar uma venda de eletricidade ou de água potencialmente rentável que ainda não está disponível na quantidade desejada.

Chama-se a isto *venda latente*.

Por exemplo, uma venda pode tornar-se legal depois de aumentar o financiamento das centrais.

O Ministério tem primeiro de aumentar a capacidade de geração.

Só depois de o jogo reconhecer essa capacidade e permitir o contrato pretendido é que a venda pode ser criada.

**Uma venda latente nunca é assinada antes de o jogo a permitir.**

Existe também uma salvaguarda contra financiamento desnecessário.

Se o Ministério planeou uma venda latente mas não há nenhum aumento de capacidade nem redução de vendas correspondente que a torne possível, recalcula o plano sem essa quantidade latente.

**A cidade não fica deliberadamente a pagar capacidade de produção adicional para suportar uma venda que não pode acontecer.**

<a id="neighbor-deals-garbage"></a>
#### Regras especiais para os contratos de lixo

Os contratos de lixo exigem cuidados adicionais, porque o lixo é entregue segundo um calendário diário e não como um fornecimento mensal contínuo.

#### Enviar o lixo da cidade para um vizinho

Exportar lixo é tratado como comprar capacidade de eliminação.

O Ministério limita a quantidade enviada aos vizinhos a:

- A produção de lixo da própria cidade depois da reciclagem.
- A quantidade máxima permitida pelas regras de compra do jogo.

Não exporta mais lixo do que a própria cidade produz depois da reciclagem.

#### Receber lixo de um vizinho

Importar lixo gera receitas, mas acrescenta resíduos que o sistema de saneamento tem de eliminar.

O Ministério inclui o lixo recebido **além da produção própria da cidade e da reserva de saneamento**.

Só aceita um contrato quando:

- O financiamento do saneamento escolhido consegue tratar os resíduos adicionais.
- O mês previsto termina **sem lixo por recolher**.
- A receita justifica qualquer despesa de saneamento adicional.
- Os limites de lixo recebido do jogo são respeitados.
- A capacidade de eliminação restante da cidade é suficiente.

**O Ministério não aceita lixo só porque um vizinho está disposto a pagar por ele.**

A cidade tem de conseguir tratar esse lixo de forma segura e económica.

#### Porque é que o Ministério avalia cada contrato de lixo em separado?

Dois contratos podem ter o mesmo valor mensal combinado, mas calendários de entrega diários diferentes.

Por exemplo, um contrato grande pode entregar de forma ligeiramente diferente de dois contratos mais pequenos, porque o jogo arredonda a quantidade diária de cada contrato de forma independente.

O Ministério simula, por isso, cada contrato com os seus próprios:

- Valor mensal.
- Quantidade de entrega diária.
- Quantidade já entregue durante o mês.
- Entregas restantes.

**Nunca substitui os calendários de entrega individuais por um único total mensal combinado.**

Isto é essencial para prever se os aterros e as instalações de tratamento conseguem eliminar o lixo antes do fim do mês.

#### Porque é que alguns contratos de lixo são mantidos até ao mês seguinte?

Os contratos de lixo costumam concluir as entregas mensais nos primeiros dez dias, mais ou menos.

Suponha que um contrato já entregou toda a sua quota.

Substituí-lo por um contrato novo no mesmo mês pode introduzir um calendário de entregas novo que a simulação do mês atual não previu.

Quando o Ministério não consegue prever com fiabilidade o comportamento dos aterros no mês seguinte, **mantém, por isso, um contrato já esgotado até à mudança de mês**, em vez de o substituir sem necessidade.

Esta restrição pode ser relaxada quando manter o contrato impediria a cidade de manter o seu serviço de saneamento.

#### O lixo importado no mês passado conta como produção local?

Não.

O contador *Lixo importado no mês passado* do jogo indica entregas passadas.

**O Ministério nunca conta essas importações como lixo produzido pela própria cidade.**

A produção local é calculada a partir dos valores de Produzido e Reciclado da cidade.

O lixo estrangeiro entra no plano através dos seus contratos individuais.

<a id="neighbor-deals-verification"></a>
#### Passo 5: Verificar cada contrato antes e depois de o alterar

O Ministério não aplica cegamente um plano de contratos calculado antes.

**Cada alteração individual de um contrato tem de ser verificada de novo imediatamente antes de ser escrita.**

Isto protege contra alterações da procura, da capacidade, da tesouraria, da elegibilidade dos contratos e dos próprios registos regionais do jogo.

#### Antes de alterar um contrato

**1. Atualizar a informação regional**

O Ministério pede ao SimCity 4 que atualize os seus registos de recursos regionais, tal como o jogo faz nas suas atualizações mensais normais.

Lê depois o estado atual da cidade e calcula um novo plano de contratos.

A alteração pretendida tem de continuar a corresponder ao novo plano:

- O mesmo tipo de contrato e a mesma cidade vizinha.
- A mesma quantidade inicial.
- A mesma quantidade alvo.
- A mesma fase de execução pendente mais antiga.
- A justificação económica exigida.

**Se a alteração proposta já não passar estas verificações, nada é escrito.**

O Ministério não força uma decisão de contrato desatualizada sobre uma cidade que mudou.

**2. Verificar o preço real do jogo**

Para um contrato novo ou uma alteração de quantidade, o Ministério pede ao SimCity 4 o respetivo preço oficial.

Compara esse preço com o valor previsto pela sua própria fórmula de preços.

**Tolerância do preço do contrato**

```math
\left|P_{\mathrm{game}}-P_{\mathrm{model}}\right|
\le
\max\left(
2,\left\lfloor\frac{P_{\mathrm{model}}}{100}\right\rfloor
\right)
```

Em que:

- **Pgame** é o preço mensal devolvido pelo SimCity 4.
- **Pmodel** é o preço calculado pelo Ministério.

A diferença permitida é de, no mínimo, **§2**, aumentando para cerca de **1%** nos contratos maiores.

**Exemplo**

Suponha que o Ministério prevê um preço mensal de **§1.874**.

A tolerância é:

```math
\max(2,\lfloor1874/100\rfloor)=18
```

O jogo tem de devolver um preço entre **§1.856 e §1.892**, inclusive.

| Preço comunicado pelo jogo | Resultado |
|---|---|
| §1.855 | Rejeitado |
| §1.856 | Aceite |
| §1.874 | Aceite |
| §1.892 | Aceite |
| §1.893 | Rejeitado |

**E se a diferença for demasiado grande?**

O Ministério trata o seu modelo de preços como pouco fiável para a cidade atual.

Interrompe a gestão automática dos contratos com vizinhos durante o resto dessa sessão da cidade.

**A gestão local do financiamento da eletricidade, da água e do saneamento pode continuar de forma independente.**

Isto impede o Ministério de tomar decisões financeiras com preços que o jogo não confirma.

**3. Verificar o contrato exato**

O Ministério encontra o contrato em vigor pelo seu **tipo e cidade vizinha**, em vez de confiar numa posição memorizada antes na lista de contratos.

Recusa a alteração quando:

- Mais de um contrato corresponde à mesma identidade.
- O contrato está suspenso.
- A quantidade atual já não coincide com a quantidade usada no plano.
- A quantidade alvo pedida já não é legal.

Uma quantidade alvo tem de ser uma das quantidades permitidas pelo jogo ou um múltiplo permitido do incremento da proposta dentro do máximo atual do jogo.

#### Uma regra especial para o lixo recebido

Há um ajuste conservador para o lixo recebido dos vizinhos.

Suponha que o Ministério planeou aceitar 10.000 unidades, mas o jogo permite agora apenas 8.000.

Se 8.000 for uma quantidade legal, o Ministério pode reduzir a quantidade planeada de lixo recebido para esse valor mais baixo.

Escolhe a **maior quantidade legal que não exceda o alvo original**.

É seguro, porque aceitar menos lixo estrangeiro não pode aumentar a carga de eliminação da cidade.

**O mesmo ajuste não se aplica às compras.**

Por exemplo, reduzir em silêncio uma compra de eletricidade planeada poderia deixar a cidade sem fornecimento suficiente.

O Ministério não substitui, por isso, por uma quantidade de compra menor sem recalcular o plano.

#### Depois de alterar um contrato

Uma operação de contrato bem-sucedida não significa que todos os contadores do jogo já tenham sido atualizados.

O Ministério espera por confirmação antes de dar a alteração por concluída.

Verifica que:

1. **O contrato tem o estado correto:** Ativo após a criação ou o redimensionamento, ou cancelado com quantidade zero ou removido após o cancelamento.
2. **A quantidade está correta:** O valor corresponde ao valor pedido.
3. **O preço mensal guardado está atualizado.**
4. **As quantidades regionais são coerentes:** Os totais de Importado e Exportado concordam com a lista de contratos.
5. **Os totais financeiros são coerentes:** Os rendimentos e as despesas mensais dos acordos com os vizinhos concordam com os contratos ativos e suspensos, segundo as regras de contabilidade do jogo.

**O jogo pode atualizar estes valores ligeiramente depois da operação do contrato.**

Se os novos valores ainda não tiverem aparecido, o Ministério espera.

Uma atualização atrasada não é tratada de imediato como uma transação falhada.

Enquanto uma alteração de contrato aguarda confirmação:

- Não é feita nenhuma alteração de contrato adicional para esse recurso.
- A transação pendente é verificada de novo todos os dias simulados.
- Os outros recursos podem continuar a sua gestão independente.

**O Ministério só avança depois de a alteração de contrato anterior ter sido confirmada.**

<a id="neighbor-deals-revenue"></a>
#### Como mede o Ministério as poupanças dos contratos?

O Ministério distingue três fases de uma decisão sobre um contrato.

| Fase | O que significa |
|---|---|
| **Projetada** | O benefício mensal esperado de um plano de contratos, calculado antes de aplicar as alterações. |
| **Pendente** | O benefício em numerário esperado de uma alteração de contrato que foi escrita mas ainda aguarda verificação. |
| **Realizada** | A variação dos rendimentos e despesas mensais dos acordos com os vizinhos confirmada pelos contadores de orçamento do próprio jogo. |

Estas distinções impedem o Ministério de apresentar um contrato planeado ou não verificado como dinheiro já ganho.

#### Poupanças projetadas

Antes de alterar contratos, o Ministério estima quanto melhor seria o arranjo proposto do que manter os contratos existentes.

Calcula dois valores:

- **Benefício em numerário:** A variação esperada dos rendimentos e despesas reais.
- **Benefício económico:** O benefício em numerário incluindo o desgaste estimado das centrais, quando essa opção está ativa.

Esta comparação é feita quando manter os contratos existentes é uma alternativa viável.

Um contrato planeado ou uma venda latente **não é contado como rendimento realizado** só por aparecer numa carteira optimizada.

#### Poupanças pendentes

Depois de uma alteração de contrato ser escrita, mas antes de o jogo a confirmar, o Ministério acompanha a sua margem em numerário esperada como **pendente**.

Continua a ser uma estimativa.

Só passa a resultado realizado depois de o estado do contrato e os contadores financeiros do jogo terem sido verificados.

#### Rendimento realizado dos contratos

Após a confirmação, o Ministério compara os rendimentos e despesas mensais dos acordos com os vizinhos do jogo antes e depois da alteração.

**Resultado realizado do contrato**

```math
\Delta_{\mathrm{realized}}=
(I_{\mathrm{after}}-X_{\mathrm{after}})
-
(I_{\mathrm{before}}-X_{\mathrm{before}})
```

Em que:

- **Iantes** e **Idepois** são o total dos rendimentos mensais dos acordos com os vizinhos do jogo antes e depois da alteração.
- **Xantes** e **Xdepois** são as despesas mensais correspondentes dos acordos com os vizinhos.
- **Δrealizado** é a variação verificada do rendimento líquido mensal dos contratos.

**Exemplo: uma nova venda de eletricidade**

Suponha que os valores mensais dos acordos com os vizinhos da cidade eram:

| | Antes | Depois |
|---|---:|---:|
| Rendimento dos contratos | §200 | §500 |
| Despesa dos contratos | §100 | §100 |
| **Rendimento líquido dos contratos** | **§100** | **§400** |

A variação realizada é:

```math
\Delta_{\mathrm{realized}}=
(500-100)-(200-100)=300
```

**Melhoria verificada do rendimento mensal dos contratos: §300.**

Esta é a variação apenas do rendimento dos contratos.

Ainda não subtrai qualquer despesa adicional das centrais necessária para suportar a venda.

Essas alterações são registadas em separado quando o Ministério ajusta o financiamento local.

#### E se os contadores financeiros não puderem ser lidos?

Se o SimCity 4 não fornecer totais fiáveis de rendimentos e despesas dos contratos antes e depois, o Ministério usa como alternativa a margem em numerário planeada.

**O registo indica explicitamente que foi usada uma estimativa em vez de uma medição financeira verificada.**

Os dois valores não são apresentados em silêncio como equivalentes.

#### E as poupanças da redução do financiamento local?

Uma compra regional pode criar dois efeitos financeiros distintos.

Por exemplo, comprar eletricidade pode:

1. Acrescentar uma despesa mensal de contrato.
2. Permitir ao Ministério reduzir o financiamento das centrais.

O primeiro efeito pertence à contabilidade dos acordos com os vizinhos.

O segundo pertence à gestão do financiamento da eletricidade.

**O Ministério regista-os em separado para evitar contar duas vezes a mesma poupança.**

Do mesmo modo, um contrato de lixo pode aumentar o rendimento dos contratos e exigir ao mesmo tempo financiamento de saneamento adicional. São alterações de orçamento distintas.

**O objetivo do Ministério é selecionar contratos de acordo com o seu efeito económico completo, aplicá-los sem perturbar os serviços essenciais e distinguir os benefícios esperados das alterações realmente confirmadas pelo jogo.**
<a id="neighbor-deals-reevaluation"></a>
#### Quando é que o Ministério reconsidera os contratos existentes?

O Ministério não presume que um contrato existente deva continuar para sempre.

**Em cada revisão, recalcula o valor económico de todos os contratos com a informação mais recente da cidade.**

Por exemplo:

- Uma venda de eletricidade pode tornar-se menos rentável à medida que a procura da cidade aumenta.
- Uma compra de água pode deixar de ser necessária depois de construir outra instalação de bombagem.
- Uma importação de lixo pode tornar-se demasiado cara de tratar se os aterros se aproximarem dos limites.

Quando a situação muda, o Ministério pode aumentar, reduzir ou cancelar contratos para encontrar um arranjo melhor.

**As necessidades próprias da cidade e as reservas de segurança têm sempre prioridade.**

Se a procura de eletricidade crescer, por exemplo, o Ministério reduz ou cancela as vendas de eletricidade antes de lhes permitir consumir a capacidade necessária para a cidade e para a sua reserva.

#### O Ministério memoriza falhas anteriores?

Normalmente, cada revisão começa de novo. Não há nenhum período geral de retenção nem de espera de contratos.

No entanto, duas exceções impedem a repetição de ações que o jogo já recusou.

**1. O jogo rejeitou um contrato novo**

Se o SimCity 4 recusar criar um contrato, o Ministério não tenta essa mesma criação outra vez até à **terceira mudança de mês simulada depois da recusa**.

Isto dá tempo para que mudem as condições de elegibilidade do jogo, como a tesouraria, a procura e as ligações.

**2. O jogo recusa cancelar um contrato dormente**

Um contrato dormente é um contrato ativo com quantidade zero.

Se o jogo recusar o cancelamento, o Ministério deixa-o em paz durante o resto da sessão atual da cidade, em vez de tentar repetidamente a mesma operação.

<a id="neighbor-deals-emergency"></a>
#### O que acontece durante uma falta grave de fornecimento?

Por vezes, uma cidade simplesmente não consegue obter eletricidade ou água suficientes para satisfazer todas as suas necessidades.

Por exemplo:

- Uma central elétrica importante é demolida.
- Várias instalações de água deixam de produzir.
- Perde-se uma ligação regional.
- Uma cidade vizinha deixa de poder fornecer um recurso contratado.

Normalmente, o Ministério procura contratos que satisfaçam tanto a procura como a reserva de segurança configurada.

**Mas e se nenhuma combinação legal de produção local e compras regionais o conseguir?**

Nessa situação, o Ministério muda de prioridades.

Em vez de rejeitar todos os arranjos possíveis por não conseguirem satisfazer a necessidade completa, **tenta abastecer o máximo possível da procura real da cidade**.

#### Que parte da procura ainda pode ser servida?

**Procura servida durante uma emergência**

```math
\mathrm{served}(B,S)=
D-\max\left(
0,\max(0,D-B)+S-C_{\max}
\right)
```

Em que:

- **D** é a procura real da cidade.
- **B** é a quantidade comprada às cidades vizinhas.
- **S** é a quantidade vendida às cidades vizinhas.
- **Cmax** é a maior capacidade local de geração ou de bombagem que as instalações da cidade podem fornecer legalmente.

A fórmula determina que parte da procura continua coberta depois de contabilizar as importações, as exportações e a capacidade local máxima disponível.

**Exemplo: uma cidade com falta de eletricidade**

Suponha que a cidade precisa de **10.000 unidades**, mas as suas centrais só conseguem produzir **6.000 unidades**.

Sem importações, a cidade não consegue fornecer 4.000 unidades da sua procura.

Se uma cidade vizinha puder fornecer legalmente 3.000 unidades, comprá-las aumenta a procura servida para **9.000 unidades**.

A cidade continua com falta de eletricidade, mas o Ministério reduziu o défice.

**Durante uma emergência, o Ministério segue estas prioridades:**

1. **Servir o máximo possível da procura real.**
2. **Repor a reserva de segurança sempre que possível.**
3. **Escolher o arranjo menos dispendioso entre soluções igualmente eficazes.**

A reserva normal continua, por isso, a ser considerada, mas o serviço real vem primeiro quando não há capacidade para satisfazer tudo.

#### O que acontece às vendas de eletricidade e de água?

O Ministério cancela todas as vendas que consiga legalmente alterar.

Isto liberta capacidade para a própria cidade.

Considera depois as quantidades de compra legais disponíveis nas cidades vizinhas.

Normalmente, as compras não podem exceder a procura da própria cidade.

**Durante uma emergência, pode ser permitido um degrau de quantidade adicional além da procura**, mas só se esse degrau aumentar em mais de 1% a procura efetivamente servida.

É uma exceção limitada, pensada para lidar com as escolhas discretas de quantidade de contrato do jogo.

Os contratos suspensos continuam incluídos nos cálculos, mas o Ministério não tenta modificá-los.

#### E uma emergência de eliminação de lixo?

Para o lixo, o Ministério segue o princípio equivalente.

Tenta:

- Cancelar os contratos que trazem lixo adicional para a cidade.
- Aumentar as exportações de lixo para as cidades vizinhas.
- Usar primeiro os arranjos de eliminação disponíveis menos dispendiosos.

As exportações de lixo continuam sujeitas às quantidades legais do jogo e à quantidade de lixo que a cidade produz depois da reciclagem.

**O Ministério não pode garantir que todas as faltas serão resolvidas.**

Se as instalações e os contratos disponíveis forem insuficientes, cabe ao jogador fornecer infraestruturas adicionais ou resolver o problema de fundo.

O Ministério só emite um aviso de falta quando o problema está **confirmado e é persistente**, em vez de o avisar de todas as flutuações temporárias.

Veja os [Avisos dos conselheiros](safety.md#notices).

<a id="neighbor-deals-safety"></a>
### 4. Condições de segurança

O Ministério segue regras rigorosas ao criar, alterar ou cancelar contratos regionais.

#### Proteger os serviços da própria cidade

**As necessidades da cidade vêm primeiro.**

As vendas de eletricidade e de água não podem consumir a capacidade necessária para servir a cidade e manter a reserva configurada.

A definição de excedente disponível do próprio SimCity 4 não inclui as margens de segurança do Ministério.

O Ministério faz, por isso, as suas próprias verificações de capacidade antes de aceitar uma venda.

#### Contar apenas com compras fiáveis

Uma compra só é tratada como fornecimento fiável enquanto estiver **devidamente ativa**.

O Ministério não conta com contratos que estejam:

- Pendentes.
- Suspensos.
- A falhar por falta de ligação.
- A falhar porque a cidade que paga não os consegue suportar.

**A produção local nunca é reduzida antes de a compra de substituição estar em vigor.**

Não existe nenhuma regra artificial que exija que uma compra ativa tenha um número determinado de dias restantes.

O Ministério considera-a utilizável até o jogo alterar o seu estado.

#### Honrar as vendas existentes

Uma venda ativa de eletricidade ou de água é tratada como uma obrigação.

O Ministério inclui a sua quantidade total ao calcular a capacidade de que a cidade precisa.

O mesmo princípio aplica-se aos contratos de lixo e à capacidade de saneamento necessária para os suportar.

Uma venda pode ser reduzida ou cancelada numa revisão posterior, mas **enquanto estiver em vigor, o plano de serviço local tem de a contabilizar**.

#### Respeitar as regras de contratos do jogo

O Ministério usa apenas:

- As ofertas de contratos que o SimCity 4 disponibiliza efetivamente.
- As quantidades permitidas pelo sistema de contratos do jogo.
- As funções do próprio jogo para criar, redimensionar e cancelar contratos.

Nunca modifica diretamente as cidades vizinhas nem força um contrato que o jogo rejeitaria.

**Os contratos suspensos nunca são modificados.**

#### Parar em segurança quando algo corre mal

Se ocorrer um erro técnico durante uma operação de contrato, o Ministério interrompe a gestão automática de contratos durante o resto da sessão atual da cidade.

Isto pode acontecer quando:

- O jogo recusa inesperadamente uma operação de contrato.
- Um contrato entra num estado inesperado depois de alterado.
- O preço comunicado pelo jogo difere demasiado do cálculo do Ministério.
- A informação resultante do contrato não pode ser verificada de forma fiável.

**Os gestores de financiamento da eletricidade, da água e do saneamento podem continuar a funcionar.**

Limitam-se a usar os contratos regionais existentes como compromissos fixos.

Se recarregar a cidade ou desativar a gestão de contratos enquanto uma transação está por concluir, o Ministério abandona as ações seguintes dessa transação.

Uma transação que apenas aguarda as atualizações contabilísticas do jogo pode continuar a ser observada até esses valores estabilizarem, sem fazer chamadas de contrato adicionais.

#### Uma alteração de contrato de cada vez

Para cada recurso, o Ministério aplica **uma alteração de contrato de cada vez**.

Verifica depois o estado resultante do contrato e os totais regionais e financeiros do jogo antes de fazer outra alteração.

Isto impede que várias operações de contrato não verificadas interfiram umas com as outras.

#### O que acontece às mensagens dos conselheiros do jogo original?

Enquanto o Ministério gere os contratos regionais, as mensagens dos conselheiros do jogo original sobre acordos com os vizinhos são suprimidas por um ficheiro de recursos incluído.

Estas incluem:

- Ofertas de novos contratos.
- Mensagens de renovação de contratos.
- Avisos de incumprimento de contratos.

O Ministério trata as situações correspondentes através do seu próprio sistema de gestão.

Quando uma falta genuína não pode ser resolvida automaticamente, pode emitir o seu próprio aviso.

Veja os [Avisos dos conselheiros](safety.md#notices).

**Importante:** As mensagens do jogo original são suprimidas por um ficheiro de recursos carregado quando o SimCity 4 arranca. Desativar a gestão automática de contratos nas opções não restaura essas mensagens de imediato. O procedimento para as restaurar é explicado em [Limitações conhecidas](#neighbor-deals-limitations).

<a id="neighbor-deals-settings"></a>
### 5. Definições

A gestão dos contratos regionais pode ser configurada em **Opções de Jogo → Ministério da Optimização**, na página **Serviços**.

| Definição | Predefinição | O que faz |
|---|---|---|
| [Gerir os acordos com os vizinhos](options.md#opt-deals) | Ligado | Ativa a gestão automática dos contratos regionais. |
| [Gerir acordos de eletricidade](options.md#opt-tradepower) | Ligado | Permite ao Ministério comprar e vender eletricidade. |
| [Gerir acordos de água](options.md#opt-tradewater) | Ligado | Permite ao Ministério comprar e vender água. |
| [Gerir acordos de lixo](options.md#opt-tradegarbage) | Ligado | Permite ao Ministério enviar lixo para os vizinhos ou receber o deles. |
| [Comprar a uma cidade para vender a outra](options.md#opt-arbitrage) | **Desligado** | Permite comprar e vender o mesmo recurso num único plano quando isso for economicamente vantajoso. |
| [Reserva de eletricidade](options.md#opt-powerreserve) | 20% | Protege o abastecimento local de eletricidade. |
| [Reserva de água](options.md#opt-waterreserve) | 10% | Protege o abastecimento local de água. |
| [Reserva de saneamento](options.md#opt-sanitationreserve) | 2% | Dá capacidade extra de eliminação de lixo. |

#### Como interagem os interruptores?

**Gerir os acordos com os vizinhos** é o interruptor principal dos contratos regionais.

Quando está desligado:

- O Ministério não pede novas ofertas de contrato.
- Não cria, não redimensiona nem cancela contratos.
- Os contratos existentes continuam em vigor e são incluídos nos cálculos do financiamento local.

Os três interruptores de recursos permitem gerir a eletricidade, a água e o lixo de forma independente.

**Também é possível gerir contratos sem gerir o financiamento local.**

Por exemplo:

| Definições | Resultado |
|---|---|
| **Gerir acordos de eletricidade: Ligado** / **Gerir a eletricidade: Ligado** | O Ministério pode coordenar os contratos de eletricidade e o financiamento das centrais. |
| **Gerir acordos de eletricidade: Ligado** / **Gerir a eletricidade: Desligado** | Os contratos podem ser geridos, mas o financiamento das centrais elétricas permanece inalterado. |
| **Gerir acordos de eletricidade: Desligado** / **Gerir a eletricidade: Ligado** | O financiamento das centrais pode ser optimizado, mas os contratos de eletricidade existentes permanecem inalterados. |
| **Ambos desligados** | Nem os contratos de eletricidade nem o financiamento das centrais são alterados automaticamente. |

O mesmo princípio de independência aplica-se à água e ao saneamento.

**Se a gestão de contratos estiver ligada mas a gestão do financiamento local estiver desligada**, o Ministério só pode usar a capacidade de produção ou de eliminação que já existe com o financiamento atual.

Não aumenta o financiamento para sustentar um novo contrato.

**Se a gestão do financiamento local estiver ligada mas a gestão de contratos estiver desligada**, as compras e vendas existentes são tratadas como compromissos fixos.

O Ministério pode, ainda assim, acompanhar o recurso e avisar o jogador de faltas persistentes.

Desligar **Gerir os acordos com os vizinhos** preserva os interruptores de cada recurso e a definição de arbitragem. Os seus valores voltam a aplicar-se quando o interruptor principal for ligado de novo.

#### Definição avançada: benefício mínimo do contrato

É possível configurar um benefício mensal mínimo para as decisões opcionais sobre contratos.

Esta definição está disponível através do ficheiro INI, não na janela de opções do jogo.

| Definição INI | Predefinição | Intervalo permitido |
|---|---:|---:|
| `MinContractBenefit` | §0/mês | §0–§100.000/mês |

Ficheiro: `DepartmentEfficiencyUtility.ini`

Quando está a zero, **qualquer benefício económico estritamente positivo é suficiente**.

Por exemplo, um contrato que melhore em §1 o resultado económico mensal pode ser considerado.

Se definir:

```ini
MinContractBenefit=20
```

As criações opcionais de contratos e as alterações de quantidade têm de produzir pelo menos **§20 por mês** de benefício adicional.

Esta definição pode ser útil se preferir menos alterações de contratos em troca de ignorar pequenas poupanças potenciais.

**O limiar nunca bloqueia uma alteração necessária para manter os serviços essenciais.**

O SimCity 4 não cobra uma taxa separada apenas por criar, alterar ou cancelar um contrato regional.

<a id="neighbor-deals-examples"></a>
### 6. Exemplos práticos

Os exemplos seguintes mostram como o Ministério avalia os contratos regionais.

**As fórmulas e os preços dos contratos do jogo são calculados com as regras reais do jogo.**

No entanto, os custos de funcionamento locais mostrados nestes exemplos são valores ilustrativos, que representam o que o optimizador de financiamento adequado poderia calcular para uma determinada cidade.

#### Exemplo 1: Vender eletricidade sem sacrificar a reserva

Considerem-se duas cidades vizinhas.

**A sua cidade (Cidade A)**

- Procura de eletricidade: **10.000 unidades**.
- Necessidade de eletricidade do jogo: **10.300 unidades**.
- Reserva de eletricidade: **20%**.
- Produção atual de eletricidade: **14.000 unidades**.
- Despesas atuais das centrais elétricas: **§1.400 por mês**.

**Cidade vizinha (Cidade B)**

- Produção de eletricidade: **3.000 unidades**.
- Necessidade de eletricidade: **6.000 unidades**.
- Tesouro: **§250.000**.

#### Passo 1: O que permite o SimCity 4?

A Cidade A tem um excedente de:

```math
14000-10300=3700
```

O limite de compra da Cidade B é:

```math
2.5\times6000-3000=12000
```

A quantidade máxima de contrato do jogo é, portanto:

```math
Q_{\max}=\min(3700,12000)=3700
```

A Cidade B cumpre os requisitos de proposta do jogo.

O preço mínimo do contrato é de aproximadamente **§110 por mês** por 1.000 unidades, e o tesouro da Cidade B excede com folga os cinco anos de pagamentos exigidos.

**O SimCity 4 está disposto a oferecer uma compra de eletricidade à Cidade A.**

#### Passo 2: O que exige o Ministério?

O Ministério aplica a sua própria reserva de eletricidade de 20%.

Sem venda:

```math
N(0,0)=\max(12000,10300)=12000
```

A Cidade A tem de manter pelo menos **12.000 unidades de capacidade de geração local**.

Cada unidade vendida aumenta essa exigência em uma unidade.

O Ministério avalia as quantidades de venda candidatas usando o incremento de proposta derivado do limite do vizinho:

```math
\text{passo}=\max(12000/10,1000)=1200
```

Considera, por isso, quantidades como 1.200, 2.400 e 3.600 unidades.

Também podem ser consideradas quantidades maiores, como *vendas latentes*, ou seja, vendas que exigiriam capacidade local adicional antes de o jogo as poder aceitar legalmente.

#### Passo 3: Que quantidade é a mais rentável?

| Eletricidade vendida | Receita mensal | Custo local ilustrativo | Custo local adicional | Lucro líquido |
|---:|---:|---:|---:|---:|
| 0 | §0 | §1.250 | — | §0 |
| 1.200 | §132 | §1.330 | §80 | +§52 |
| **2.400** | **§264** | **§1.420** | **§170** | **+§94** |
| 3.600 | §396 | §1.560 | §310 | +§86 |
| 4.800 (latente) | §527 | §1.720 | §470 | +§57 |

**A escolha mais rentável é vender 2.400 unidades.**

Isto dá uma melhoria estimada de **§94 por mês**.

No entanto, a venda exige:

```math
12000+2400=14400
```

**14.400 unidades de capacidade de geração local.**

A Cidade A produz atualmente apenas 14.000.

#### Passo 4: Como é aplicado o plano?

O Ministério não assina a venda de imediato.

Primeiro, aumenta o financiamento das centrais elétricas até atingir a capacidade necessária de 14.400 unidades.

Depois de o jogo reconhecer a produção adicional, o contrato é reconsiderado numa revisão posterior.

Com 14.400 unidades de produção, o excedente disponível segundo o jogo passa a ser:

```math
14400-10300=4100
```

A venda planeada de 2.400 unidades está agora dentro do máximo permitido e pode ser escrita se o plano atualizado ainda a escolher.

**O Ministério aumenta a produção antes de assinar a venda, protegendo a reserva da cidade durante toda a transição.**

Sem a proteção da reserva do Ministério, o jogador poderia ter vendido as 3.700 unidades completas permitidas pelos cálculos originais do jogo, ficando sem margem de reserva.

#### Exemplo 2: Comprar eletricidade e poupar mais através da reserva

Considere-se outra cidade, a **Cidade C**.

- Procura de eletricidade: **10.000 unidades**.
- Necessidade do jogo: **10.300 unidades**.
- Produção atual: **12.500 unidades**.
- Reserva de eletricidade: **20%**.

O fornecedor vizinho tem:

- Produção: **30.000 unidades**.
- Necessidade: **20.000 unidades**.
- Custo médio por unidade: **§0,08**.

O fornecedor tem 10.000 unidades de excedente.

O limite do comprador é:

```math
2.5\times10300-12500=13250
```

O máximo do jogo é, portanto, de **10.000 unidades**, e as quantidades das propostas aumentam em passos de 1.000.

O limite de compra do próprio Ministério também é de 10.000 unidades.

#### Passo 1: Quanta capacidade local é necessária?

Sem compras, a reserva de 20% da cidade exige:

```math
N(0,0)=12000
```

Comprar eletricidade reduz a quantidade que tem de ser produzida localmente.

**Cada unidade comprada pode reduzir em 1,2 unidades a exigência de capacidade local**, enquanto a exigência baseada na reserva continuar a ser a que vincula.

#### Passo 2: Comparar as compras possíveis

| Unidades compradas | Capacidade local necessária | Custo local ilustrativo | Poupança local | Preço do contrato | Poupança líquida |
|---:|---:|---:|---:|---:|---:|
| 0 | 12.000 | §1.250 | — | — | §0 |
| 1.000 | 10.800 | §1.160 | §90 | §89 | +§1 |
| **2.000** | **9.600** | **§1.060** | **§190** | **§179** | **+§11** |
| 3.000 | 8.400 | §975 | §275 | §267 | +§8 |
| 4.000 | 7.200 | §900 | §350 | §356 | −§6 |

**O arranjo globalmente mais barato é comprar 2.000 unidades.**

A compra custa **§179 por mês**, mas permite reduzir em **§190** as despesas das centrais elétricas locais.

Poupança líquida estimada:

```math
190-179=11
```

**§11 por mês.**

#### Passo 3: Aplicar as alterações em segurança

O Ministério começa por criar e verificar a compra.

Só depois reduz o financiamento das centrais, numa fase de execução posterior adequada.

**E se o benefício mínimo estiver definido em §20?**

Com:

```ini
MinContractBenefit=20
```

O contrato opcional não é criado.

O seu benefício mensal de §11 fica abaixo do limiar configurado.

Esta definição permite evitar pequenos ajustes económicos se preferir um conjunto de contratos mais estável.

#### Exemplo 3: Porque é que um contrato nem sempre pode ser redimensionado para qualquer quantidade

Suponha-se que a cidade já tem uma venda de eletricidade com:

- Quantidade atual: **3.088 unidades**.
- Incremento do contrato: **1.544 unidades**.
- Máximo do jogo: **15.442 unidades**.

As quantidades que o jogo permite escolher seguem o incremento:

1.544; 3.088; 4.632; 6.176; e assim sucessivamente, até 15.440.

**Uma quantidade de contrato de 6.000 unidades não está disponível.**

A quantidade permitida mais próxima acima dela é **6.176 unidades**.

O Ministério respeita essas quantidades legais em vez de submeter ao jogo um número arbitrário.

#### Exemplo 4: Aceitar lixo de outra cidade

Uma cidade vizinha oferece-se para lhe pagar para tratar **20.000 unidades de lixo por mês**.

O jogo entrega normalmente:

```math
20000\times0.10=2000
```

**2.000 unidades por dia simulado**, durante aproximadamente dez dias.

O Ministério avalia se as suas instalações de saneamento conseguem tratar estas 20.000 unidades adicionais.

Tem em conta:

- A produção de lixo da própria cidade depois da reciclagem.
- A reserva de saneamento configurada.
- O lixo já por recolher.
- As importações e exportações existentes.
- A capacidade de tratamento das incineradoras e das centrais de conversão de lixo em energia.
- A admissão disponível nos aterros.
- O calendário diário de entregas de cada contrato.

**O contrato só é aceite se o mês projetado terminar sem lixo por recolher.**

A receita tem também de justificar qualquer financiamento adicional do saneamento necessário para tratar os resíduos recebidos.

Por fim, o total de lixo recebido tem de permanecer abaixo do limite permitido pelo jogo.

No exemplo da Secção 1, esse limite era de aproximadamente **541.667 unidades**.

**Um contrato bem pago é, ainda assim, rejeitado se a cidade não conseguir tratar o lixo adicional em segurança.**

<a id="neighbor-deals-limitations"></a>
### 7. Limitações conhecidas

O Ministério consegue gerir automaticamente os contratos regionais, mas continua sujeito às regras originais do SimCity 4 e à informação disponível.

#### Cidades vizinhas e informação regional

- **Os registos dos vizinhos são instantâneos.** O SimCity 4 não simula as cidades vizinhas enquanto joga a cidade atual. O Ministério usa os valores regionais guardados pela última vez para esses vizinhos, que podem estar desatualizados.
- **O Ministério nunca altera as cidades vizinhas.** Não as carrega, não modifica os seus edifícios nem atualiza a informação que têm guardada.
- **As datas dos contratos continuam a ser controladas pelo SimCity 4.** O jogo trata do início, da expiração e da renovação dos contratos. O Ministério não tenta prevê-las nem modificá-las. Quando um contrato termina ou fica suspenso, o seu novo estado é considerado na revisão seguinte.
- **As novas vendas exigem uma oferta existente.** O Ministério pode aumentar a produção da cidade para sustentar uma venda maior a um vizinho que já oferece esse recurso. Não consegue fazer aparecer um comprador totalmente novo quando o jogo não tem nenhuma oferta desse vizinho.

#### Aplicação das alterações aos contratos

- **Só é aplicado um passo por revisão.** Cada recurso faz, no máximo, uma alteração de contrato ou um passo de financiamento local por revisão. Uma reorganização profunda pode, por isso, demorar várias revisões.
- **O preço de cada contrato é verificado.** Se o preço de referência do jogo diferir do modelo do Ministério para lá da tolerância permitida, a gestão de contratos pára durante o resto da sessão da cidade.
- **Os contratos suspensos não podem ser alterados.** O jogo controla a sua resolução e o Ministério tem de os contornar.
- **As pesquisas em grandes contratos de lixo têm limites.** Se o Ministério não conseguir avaliar exaustivamente as combinações legais por haver mais de cinco milhões de um dos lados, não faz nenhuma alteração especulativa de contratos. As pesquisas mistas de compra e venda têm também os seus próprios limites, descritos atrás.

#### Mensagens originais do conselheiro sobre acordos com os vizinhos

O Ministério inclui um ficheiro de recursos que suprime as mensagens originais do conselheiro do SimCity 4 sobre acordos com os vizinhos.

Esse ficheiro é:

`DepartmentEfficiencyDealAdvice.dat`

**O SimCity 4 carrega os ficheiros de recursos quando o jogo arranca.**

Por isso, desligar as opções de gestão de contratos do Ministério não restaura de imediato as mensagens originais do conselheiro.

Se quiser gerir os contratos manualmente e restaurar as mensagens originais:

1. Feche o SimCity 4.
2. Retire `DepartmentEfficiencyDealAdvice.dat` da pasta Plugins do jogo.
3. Reinicie o jogo.

Isto restaura também mensagens originais como *Acordo Incumprido*.

O Ministério regista um aviso no seu registo quando o ficheiro de recursos e as definições de gestão atuais não coincidem.

#### Propostas pendentes

Quando pede ofertas de contrato, o Ministério usa a mesma operação do jogo que abre a janela de Acordos com os Vizinhos.

Isto pode fazer com que apareçam nas listas de contratos do jogo propostas pendentes de outras cidades vizinhas.

Tal como no jogo original, **as propostas pendentes não geram receitas nem despesas** enquanto não se tornarem contratos ativos.

**O Ministério trabalha dentro do sistema de contratos do jogo. Pode optimizar as escolhas disponíveis, mas não pode criar recursos, ligações, elegibilidade nem informação fiável que o SimCity 4 não fornece.**

[Voltar ao índice](index.md#contents) | [Voltar ao README](../../README.pt.md)

---

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
