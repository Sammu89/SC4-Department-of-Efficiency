[English](../en/power.md) | **Português** | [Français](../fr/power.md) | [Español](../es/power.md)

[Voltar ao índice](index.md#contents) | [Voltar ao README](../../README.pt.md)

<a id="power"></a>
## B. Eletricidade

O Ministério gere automaticamente **cada central elétrica individualmente**, ajustando o seu financiamento para dar eletricidade suficiente à cidade e manter os custos de funcionamento o mais baixos possível.

Tem em conta:

- A quantidade de eletricidade de que a cidade precisa.
- A reserva de segurança que escolheu (**20% por predefinição**).
- A eletricidade comprada às cidades vizinhas ou vendida a elas.
- A capacidade de produção, o estado e o custo de funcionamento de cada central.

O Ministério revê o financiamento da eletricidade **a cada 10 dias simulados**. Faz também uma verificação rápida todos os dias simulados e pode ajustar o financiamento mais cedo quando a procura, a capacidade de produção ou os contratos regionais mudam de forma significativa.

Em vez de aplicar a mesma percentagem de financiamento a todas as centrais, procura **a combinação de níveis de financiamento menos dispendiosa** que satisfaz as necessidades da cidade.

O Ministério nunca constrói, demole nem substitui centrais elétricas. Só ajusta o seu financiamento, dentro do intervalo permitido que lhe é próprio e com as suas verificações de segurança, para proteger o fornecimento de eletricidade e a rede de linhas elétricas.

<a id="power-vanilla"></a>
### 1. Como funciona a eletricidade no SimCity 4

#### Procura, produção e consumo de eletricidade

O SimCity 4 acompanha vários valores de eletricidade da sua cidade:

| Termo | O que significa |
|---|---|
| **Procura** | A eletricidade pedida pelos edifícios que estão atualmente alimentados. |
| **Necessidade** | A estimativa do jogo da eletricidade de que a cidade precisa, incluindo os edifícios atualmente sem eletricidade. Costuma ser ligeiramente superior à procura. |
| **Consumo** | A eletricidade efetivamente entregue aos edifícios. |
| **Capacidade de produção** | A eletricidade que as suas centrais conseguem produzir atualmente, com base no seu financiamento e estado. |
| **Importações** | Eletricidade comprada às cidades vizinhas. |
| **Exportações** | Eletricidade vendida às cidades vizinhas. |

Todos estes valores são medidos nas unidades de eletricidade do jogo por mês simulado.

**Nem toda a falta de eletricidade é causada por produção insuficiente.**

Há duas situações possíveis:

- **Eletricidade insuficiente:** As suas centrais e as importações disponíveis não conseguem satisfazer a procura da cidade. O Ministério pode responder aumentando o financiamento ou ajustando os contratos regionais.
- **A eletricidade não chega a alguns edifícios:** A cidade tem capacidade de produção suficiente, mas alguns edifícios não estão devidamente ligados à rede elétrica. Aumentar o financiamento não resolve isto. É preciso corrigir as ligações por si.

O Ministério distingue estas situações para não aumentar desnecessariamente a despesa com eletricidade.

**Como afeta a eletricidade importada a produção local?**

Nas nossas observações, o SimCity 4 usa a eletricidade importada antes de recorrer às centrais locais. Isto significa que uma central pode por vezes mostrar uma utilização muito baixa, ou até nula, enquanto uma cidade vizinha fornece a eletricidade.

O Ministério tem em conta os contratos regionais ao decidir de quanta capacidade de produção local é preciso.

#### Como afeta o financiamento a produção de uma central

Cada central elétrica tem três características importantes:

- **Produção máxima:** A quantidade de eletricidade que a central consegue produzir na sua capacidade nominal.
- **Financiamento:** A percentagem do seu orçamento de funcionamento atribuída à central.
- **Estado:** O estado atual da central, de 0 (completamente deteriorada) a 1 (nova).

A produção real de uma central depende tanto **do seu financiamento como do seu estado**.

Por exemplo, uma central nova financiada a 100% consegue produzir toda a sua capacidade nominal. À medida que o seu estado se deteriora, a produção disponível diminui.

<a id="power-plant-efficiency"></a>
**Eficiência da central (jogo base)**

```math
\text{Eff} = \frac{\text{Curve}(f)}{100} \times \text{Cond}
```

Onde:

- **f** é a percentagem de financiamento da central.
- **Curve(f)** é a eficiência dada pela curva de financiamento do jogo.
- **Cond** é o estado atual da central, entre 0 e 1.
- **Eff** é a eficiência resultante, expressa como fração.

**Produção efetiva (jogo base)**

```math
\text{Out} = \text{MaxOut} \times \text{Eff}
```

Onde:

- **MaxOut** é a produção máxima da central.
- **Out** é a sua capacidade de produção efetiva.

**Exemplo: uma central com uma produção máxima de 7.500 unidades**

Suponha-se que a central está em estado quase perfeito (0,9998).

| Financiamento | Eficiência após o estado | Produção efetiva |
|---|---:|---:|
| 100% | Cerca de 100% | Cerca de 7.498 unidades |
| 95% | Cerca de 90% | Cerca de 6.748 unidades |

Reduzir o financiamento em apenas **5 pontos percentuais** reduz a capacidade de produção em aproximadamente **10%**.

Isto acontece porque a relação do jogo entre financiamento e produção não é proporcional.

<a id="power-efficiency-curve"></a>
#### A relação entre o financiamento e a eficiência

O SimCity 4 usa uma curva incorporada para determinar quanta eletricidade uma central pode produzir a cada nível de financiamento.

Estes são os pontos de controlo do jogo original:

| Financiamento da central | Eficiência de produção |
|---:|---:|
| 0% | 0% |
| 50% | 25% |
| 90% | 80% |
| 100% | 100% |
| 120% | 110% |

**O pormenor importante é que reduzir o financiamento reduz também a eficiência.**

Por exemplo:

- Com **50% de financiamento**, uma central fornece apenas **25% da sua produção nominal**.
- Com **90% de financiamento**, fornece **80% da sua produção nominal**.
- Com **100% de financiamento**, fornece **100% da sua produção nominal**.
- Com **120% de financiamento**, fornece **110% da sua produção nominal**.

Financiar acima de 100% é, por isso, menos rentável: paga-se mais 20% para obter apenas mais 10% de capacidade de produção.

**Como se calculam os valores intermédios?**

O jogo usa interpolação em linha reta entre os pontos de controlo. O Ministério aplica a mesma regra.

**Interpolação da curva**

```math
y(x) = y_i + \frac{x-x_i}{x_{i+1}-x_i}(y_{i+1}-y_i)
\qquad \text{para } x_i \le x \le x_{i+1}
```

Isto significa simplesmente que, entre dois pontos conhecidos, a eficiência varia a um ritmo constante.

Por exemplo, entre 90% e 100% de financiamento, a eficiência de produção sobe de 80% para 100%. Com 95% de financiamento, a eficiência é, portanto, de 90%.

Fora do intervalo definido, a curva mantém o seu primeiro ou último valor.

**Eficiência aos níveis de financiamento que o Ministério usa**

Normalmente, o Ministério considera níveis de financiamento de 60% a 120%.

| Financiamento (%) | 60 | 65 | 70 | 75 | 80 | 85 | 90 | 95 | 100 | 105 | 110 | 115 | 120 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| Eficiência (%) | 38,75 | 45,625 | 52,5 | 59,375 | 66,25 | 73,125 | 80 | 90 | 100 | 102,5 | 105 | 107,5 | 110 |
| Produção por § gasto (relativa a 100%) | 0,65 | 0,70 | 0,75 | 0,79 | 0,83 | 0,86 | 0,89 | 0,95 | 1,00 | 0,98 | 0,95 | 0,93 | 0,92 |

A última linha compara a capacidade de produção que cada nível de financiamento proporciona pelo dinheiro gasto, relativamente a um financiamento de 100%.

**O que significa isto para o orçamento de eletricidade da cidade?**

Com **100% de financiamento**, uma central fornece a maior capacidade de produção por simoleão gasto.

No entanto, isso **não** quer dizer que todas as centrais devam ser sempre financiadas a 100%.

Imagine que a cidade tem duas centrais, mas não precisa de toda a produção combinada.

Financiar as duas por igual pode custar mais do que manter uma perto de 100% e a outra com um financiamento mais baixo.

**É exatamente isto que o Ministério optimiza:** compara as combinações disponíveis e escolhe a mais barata que ainda assegura a eletricidade necessária e a reserva de segurança.

Veja o [Exemplo 3](#power-example-mixed) para um cálculo com duas centrais diferentes.

#### Financiamento individual das centrais e custos de funcionamento

Ao contrário do serviço de água, **o financiamento da eletricidade é gerido separadamente para cada central**.

Cada central tem a sua própria definição de financiamento e o seu próprio custo mensal de funcionamento.

Isto permite ao Ministério aumentar o financiamento de uma central e reduzir o de outra, consoante a combinação mais económica.

**Que níveis de financiamento pode o Ministério usar?**

A configuração de eletricidade do jogo identifica **60%** como o seu *financiamento mínimo aceitável*. Contudo, esse valor não deve ser confundido com um limite rígido comprovado do controlo de financiamento do jogo.

A função de orçamento local subjacente aceita valores a partir de 0%, sujeitos ao máximo do departamento.

O optimizador do Ministério considera deliberadamente níveis de financiamento de **60% a 120%**, em percentagens inteiras, e aplica verificações adicionais antes de alterar o orçamento de uma central.

Essas verificações garantem que as alterações planeadas respondem às necessidades de eletricidade da cidade e não provocam a deterioração das linhas elétricas.

**Como se calcula o custo mensal de funcionamento?**

O custo de funcionamento varia em proporção à percentagem de financiamento.

**Despesa mensal da central (jogo base)**

```math
E(f) = \mathrm{round}\left(C \times \frac{f}{100}\right)
```

Onde:

- **C** é o custo mensal de funcionamento da central com 100% de financiamento.
- **f** é a percentagem de financiamento escolhida.
- **E(f)** é a despesa mensal estimada com esse financiamento.

Por exemplo, uma central que custe §253 por mês com 100% de financiamento custaria aproximadamente:

| Financiamento | Custo mensal de funcionamento |
|---|---:|
| 60% | §152 |
| 84% | §213 |
| 100% | §253 |
| 120% | §304 |

Com **84% de financiamento**, a cidade poupa cerca de **§40 por mês** em comparação com financiar essa central a 100%.

No entanto, o Ministério só escolhe 84% se a capacidade de produção resultante for suficiente para o plano global de eletricidade.

O jogo comunica o custo nominal de geração de uma central em separado do que é realmente cobrado ao financiamento atual. O Ministério usa esta última relação para estimar as despesas, arredondando para simoleões inteiros, e lê o custo nominal mais recente da central sempre que volta a calcular o plano.

#### Porque é que o departamento de eletricidade mostra uma percentagem de financiamento diferente?

Embora as centrais sejam financiadas individualmente, o SimCity 4 mostra também, no orçamento da cidade, uma percentagem global de financiamento do departamento de eletricidade.

**Não se trata de um cursor de financiamento adicional.**

É calculada como uma **média ponderada pelo custo** dos níveis de financiamento das centrais individuais.

Isto significa que uma central grande e cara tem mais influência na percentagem do departamento do que uma pequena e barata.

**Financiamento global da eletricidade (jogo base)**

```math
F_{\text{dep}} = \frac{\sum_i C_i f_i}{\sum_i C_i}
```

Onde:

- **Cᵢ** é o custo mensal nominal de funcionamento da central *i*.
- **fᵢ** é a percentagem de financiamento dessa central.
- **Fdep** é a percentagem global de financiamento do departamento de eletricidade.

**Exemplo**

Imagine que as centrais da cidade têm um custo nominal de funcionamento combinado de **§11.660 por mês**.

Uma central custa §300 por mês e está financiada a 95%, enquanto todas as outras permanecem a 100%.

Essa central poupa §15 por mês.

O financiamento global do departamento de eletricidade passa a ser:

```math
F_{\text{dep}} = \frac{11660-15}{11660}\times100
\approx 99.87\%
```

Mesmo que uma central esteja financiada a 95%, o departamento no seu conjunto continua a mostrar cerca de **99,87% de financiamento**.

Isto é normal.

**O Ministério gere cada central individualmente. O orçamento global da eletricidade reflete o resultado combinado dessas decisões.**
#### Linhas elétricas: como o financiamento afeta a rede

No SimCity 4, o financiamento da eletricidade afeta mais do que as centrais. Influencia também o estado da **rede de linhas elétricas** da cidade.

O jogo usa o financiamento global do departamento de eletricidade para determinar se as linhas elétricas se deterioram ou são reparadas ao longo do tempo.

| Financiamento do departamento | Efeito nas linhas elétricas |
|---:|---|
| 0% | Deterioração rápida (−0,020) |
| 25% | Deterioração (−0,005) |
| 50% | Sem deterioração nem reparação (0) |
| 100% | Reparação (+0,025) |
| 110% | Reparação mais rápida (+0,050) |

Os valores entre estes pontos são calculados com a interpolação linear habitual do jogo.

**O limiar importante é 50% de financiamento do departamento:**

- **Abaixo de 50%:** as linhas elétricas deterioram-se gradualmente.
- **Em 50%:** o seu estado mantém-se estável.
- **Acima de 50%:** a rede repara-se gradualmente.

Normalmente, o Ministério considera níveis de financiamento entre **60% e 120% para cada central gerida**. Isto ajuda a manter o financiamento global da eletricidade acima do nível em que as linhas elétricas se deterioram.

Verifica também o efeito das alterações de financiamento na rede de linhas elétricas antes de as aplicar.

**O Ministério não corta simplesmente os orçamentos das centrais sem ter em conta o estado da rede elétrica.**

<a id="power-aging"></a>
#### Estado e envelhecimento das centrais

As centrais não duram para sempre. À medida que funcionam, o seu estado vai diminuindo gradualmente.

No SimCity 4, **a rapidez com que uma central envelhece depende sobretudo da intensidade com que é usada**, embora o financiamento também tenha um papel.

O Ministério lê de cada central informações como:

- **Taxa base de envelhecimento:** o parâmetro de envelhecimento subjacente da central.
- **Limiar de envelhecimento:** o estado a partir do qual o jogo considera a central velha. Foi 0,9 (90%) nas centrais examinadas.
- **Custo de substituição:** o montante necessário para construir outra central do mesmo tipo.

Estes valores ajudam o Ministério a estimar o custo económico de explorar uma central quando o cálculo opcional do envelhecimento está ligado.

**Financiamento e utilização não são a mesma coisa**

Esta distinção é essencial.

- O **financiamento** determina quanta eletricidade uma central *pode produzir*.
- A **utilização** mede quanto dessa capacidade disponível a cidade realmente usa.

Por exemplo:

| Central | Financiamento | Utilização real |
|---|---:|---:|
| Central A | 100% | 30% |
| Central B | 70% | 95% |

A central A está totalmente financiada, mas produz muito menos eletricidade do que poderia.

A central B recebe menos financiamento, mas a cidade usa quase toda a eletricidade que ela consegue produzir a esse nível de financiamento.

**Utilização da central (jogo base)**

```math
u = \frac{\text{Usada}}{\text{Saída}}
```

Onde:

- **Usada** é a eletricidade efetivamente retirada da central.
- **Saída** é a capacidade de produção disponível da central.
- **u** é a proporção dessa capacidade que está a ser usada.

Por exemplo, se uma central pode produzir 10.000 unidades e a cidade retira 8.000, a sua utilização é de **80%**.

Quando uma central não produz nada, o Ministério trata a sua utilização como zero.

### Com que rapidez envelhecem as centrais?

O cálculo do envelhecimento do jogo depende de três fatores:

1. A taxa base de envelhecimento da central.
2. A intensidade com que a central é usada.
3. O seu nível de financiamento.

**Envelhecimento estimado por mês simulado (SimCity 4, modelado pelo Ministério)**

```math
A = \frac{B \times U(\min(\max(u,0),2)) \times G\left(\frac{f}{100}\right)}{16}
```

Onde:

- **B** é o parâmetro base de envelhecimento da central.
- **u** é a sua utilização atual, expressa como fração.
- **U(u)** é o multiplicador de envelhecimento associado à utilização.
- **f** é a sua percentagem de financiamento.
- **G(f/100)** é o multiplicador de envelhecimento associado ao financiamento.
- **A** é o estado estimado perdido por mês simulado.

Em termos simples:

**Envelhecimento da central = Envelhecimento base × Efeito da utilização × Efeito do financiamento × Fator de tempo**

A taxa base e as duas curvas de envelhecimento provêm da mecânica do jogo.

O **fator de tempo de 1/16** foi estabelecido através de medições em sete centrais ao longo de 13 meses simulados. É um valor medido experimentalmente, não uma constante lida diretamente do jogo.

As estimativas de envelhecimento resultantes devem, por isso, ser tratadas como aproximações e não como previsões exatas da vida útil restante de uma central.

#### 1. Como o financiamento afeta o envelhecimento

A curva de envelhecimento por financiamento do jogo tem três pontos de controlo:

| Financiamento | Multiplicador de envelhecimento |
|---:|---:|
| 0% | 1,10 |
| 100% | 1,00 |
| 120% | 0,90 |

Os valores entre estes pontos são interpolados.

Dentro do intervalo normal de financiamento do Ministério:

| Financiamento | Multiplicador de envelhecimento |
|---:|---:|
| 60% | 1,04 |
| 80% | 1,02 |
| 90% | 1,01 |
| 100% | 1,00 |
| 110% | 0,95 |
| 120% | 0,90 |

**O financiamento tem um efeito direto relativamente pequeno no envelhecimento.**

Por exemplo, reduzir o financiamento de uma central de 100% para 60% aumenta o seu multiplicador direto de envelhecimento de 1,00 para 1,04, um aumento de apenas **4%**.

No entanto, reduzir o financiamento reduz também a quantidade de eletricidade que a central pode produzir. Se a cidade continuar a retirar a mesma quantidade, a utilização da central aumenta, e isso pode ter um efeito muito maior no envelhecimento.

#### 2. Como a utilização afeta o envelhecimento

A curva de envelhecimento por utilização do jogo é muito mais significativa:

| Utilização da central | Multiplicador de envelhecimento |
|---:|---:|
| 0% | 0,10 |
| 25% | 0,20 |
| 50% | 0,45 |
| 75% | 0,85 |
| 90% | 1,25 |
| 100% | 1,55 |
| 110% ou mais | 2,00 |

Tal como nas outras curvas, os valores intermédios são calculados por interpolação linear.

**Quanto mais uma central trabalha, mais depressa envelhece.**

Por exemplo:

- Com **50% de utilização**, o multiplicador de envelhecimento é 0,45.
- Com **90% de utilização**, chega a 1,25.

Uma central a funcionar com 90% de utilização envelhece, portanto, cerca de **2,8 vezes mais depressa** do que a mesma central a funcionar com 50%, mantendo-se tudo o resto igual.

É por isso que reduzir simplesmente o financiamento das centrais nem sempre é a opção economicamente ideal.

**Exemplo: reduzir o financiamento pode quase duplicar o envelhecimento**

Imagine uma central inicialmente financiada a 100%, com a cidade a usar cerca de 60% da sua produção disponível.

O Ministério reduz o financiamento para poupar custos de funcionamento. A central passa a poder produzir menos eletricidade, mas a cidade continua a retirar aproximadamente a mesma quantidade.

A sua utilização pode subir para cerca de 83%.

O multiplicador de envelhecimento por utilização sobe de aproximadamente 0,60 para 1,06, enquanto o multiplicador direto do financiamento muda apenas ligeiramente.

**A taxa de envelhecimento estimada da central passa a ser quase o dobro.**

Isto não significa necessariamente que a redução do financiamento seja uma má decisão. A poupança mensal pode continuar a valer mais do que o desgaste adicional.

O Ministério pode comparar estes custos quando **Ter em conta o desgaste das centrais** está ligado.

**O que faz «Ter em conta o desgaste das centrais»?**

Esta opção está **desligada por predefinição**.

- **Desligada:** o Ministério minimiza as despesas mensais de funcionamento da eletricidade, mantendo o fornecimento necessário e a reserva de segurança.
- **Ligada:** o Ministério inclui também o custo económico estimado do desgaste das centrais ao comparar as combinações de financiamento.

A opção pode favorecer uma disposição de financiamento diferente quando a redução do desgaste compensa a despesa de funcionamento adicional.

**Não impõe uma percentagem mínima de financiamento própria, não impede o envelhecimento nem substitui automaticamente as centrais velhas.**

<a id="power-usage-myth"></a>
#### Será 80% de utilização da central um limite de segurança?

**Não. O SimCity 4 não tem um limiar súbito de envelhecimento ou de apagão aos 80% de utilização.**

A confusão vem da curva de envelhecimento do jogo.

Com cerca de **80,6% de utilização**, o multiplicador de envelhecimento chega a 1,0. Abaixo desse ponto, o multiplicador é mais baixo; acima dele, o multiplicador aumenta.

Mas a curva é contínua.

Uma central a funcionar com 81% de utilização não se torna de repente insegura. Do mesmo modo, uma central a funcionar com 95% de utilização não provoca automaticamente um apagão.

**Os apagões acontecem quando o fornecimento de eletricidade é insuficiente ou não chega aos edifícios, e não porque uma determinada central ultrapassa os 80% de utilização.**

O Ministério não impõe, por isso, um teto de 80% de utilização.

Em vez disso, usa a definição **Reserva de eletricidade** para manter capacidade de produção de reserva.

#### Como afeta a reserva de eletricidade a utilização?

A reserva de eletricidade predefinida é de **20%**.

Para uma cidade simples abastecida por uma única central local, sem comércio regional de eletricidade, a relação é aproximadamente:

```math
u \approx \frac{1}{1+r}
```

Onde **r** é a reserva configurada expressa como fração (0,20 para uma reserva de 20%).

Isto pressupõe que a central fornece exatamente a capacidade necessária e ignora pequenas diferenças causadas por arredondamentos e pelas exigências adicionais do jogo.

| Reserva de eletricidade | Utilização aproximada da central | Multiplicador de envelhecimento por utilização |
|---|---:|---:|
| 10% | 90,9% | 1,28 |
| **20% (predefinição)** | **83,3%** | **1,07** |
| 25% | 80,0% | 0,98 |
| 30% | 76,9% | 0,90 |
| 50% | 66,7% | 0,72 |

Uma reserva mais alta deixa, em geral, mais capacidade de produção disponível e pode reduzir a intensidade com que uma central precisa de funcionar.

No entanto, **mais reserva pode exigir também mais financiamento e, por conseguinte, aumentar as despesas mensais**.

Com várias centrais, a relação torna-se mais complicada, porque o SimCity 4 decide como a procura de eletricidade se distribui entre elas.

O Ministério não controla diretamente essa distribuição.

O seu objetivo é manter capacidade total suficiente, com a reserva que escolheu, minimizando os custos incluídos na sua optimização.

#### Comércio regional de eletricidade

O Ministério considera também a eletricidade comprada às cidades vizinhas ou vendida a elas.

- **Comprar eletricidade** reduz a capacidade de produção local de que a cidade precisa.
- **Vender eletricidade** aumenta a capacidade que as centrais da própria cidade têm de fornecer.

Por exemplo, se a cidade precisa de 10.000 unidades e importa de forma fiável 3.000, as suas centrais não precisam de produzir localmente as 10.000 na totalidade.

Por outro lado, vender 2.000 unidades a uma vizinha significa que essas unidades têm de estar disponíveis além das necessidades da própria cidade.

O Ministério inclui estes contratos no cálculo da capacidade necessária e na escolha do financiamento das centrais.

A forma como cria, altera, fixa o preço e cancela contratos é explicada em [Acordos com os vizinhos](deals.md#neighbor-deals).

<a id="power-problem"></a>
### 2. O problema da gestão

Gerir a eletricidade manualmente é mais complicado do que ajustar um único cursor de orçamento.

O Ministério tem de equilibrar vários fatores:

- **Evitar gastos desnecessários:** uma central financiada a 100% pode produzir muito mais eletricidade do que a cidade precisa.
- **Manter o fornecimento suficiente:** reduzir demasiado o financiamento pode deixar edifícios sem eletricidade.
- **Proteger a rede elétrica:** um financiamento global da eletricidade muito baixo pode fazer as linhas elétricas deteriorarem-se.
- **Ter em conta a eficiência das centrais:** o financiamento e a produção não são proporcionais. Cortar todas as centrais para a mesma percentagem não é necessariamente a solução mais barata.
- **Ter em conta o estado e o envelhecimento:** as centrais mais velhas produzem menos eletricidade, e as que trabalham mais perto da capacidade máxima envelhecem, em geral, mais depressa.
- **Considerar o comércio regional:** comprar eletricidade pode reduzir as necessidades de produção local, enquanto vender eletricidade as aumenta.
- **Gerir várias centrais em conjunto:** a combinação mais barata pode passar por financiar uma central perto de 100% e outra muito mais baixo, em vez de aplicar a mesma percentagem às duas.

O SimCity 4 não mostra diretamente que combinação de níveis de financiamento das centrais assegura a eletricidade necessária ao menor custo global.

**O Ministério calcula essa combinação automaticamente.**

Considera a capacidade, o estado e a despesa de funcionamento de cada central, juntamente com a reserva escolhida e os contratos regionais.

Quando a definição opcional do envelhecimento está ligada, inclui também o custo económico estimado do desgaste das centrais.

**O resultado é um orçamento de eletricidade concebido para satisfazer as necessidades da cidade sem gastar mais do que o modelo do Ministério considera necessário.**
<a id="power-algorithm"></a>
### 3. Como o Ministério optimiza o financiamento da eletricidade

O Ministério não se limita a reduzir o financiamento de todas as centrais na mesma percentagem.

Em vez disso, calcula quanta eletricidade a cidade precisa e compara diferentes combinações de financiamento para encontrar **a solução de menor custo que mantém capacidade de produção suficiente**.

O processo tem três passos principais:

1. Calcular quanta eletricidade as centrais da cidade têm de fornecer.
2. Calcular quanto cada central pode produzir a diferentes níveis de financiamento.
3. Encontrar a combinação mais barata que cumpre a necessidade.

Se **Ter em conta o desgaste das centrais** estiver ligado, o Ministério inclui também o custo económico estimado do desgaste das centrais ao comparar estas combinações.

<a id="power-required-capacity"></a>
#### Passo 1: De quanta eletricidade precisa a cidade?

O Ministério começa por calcular a capacidade de produção que tem de estar disponível localmente.

Tem em conta:

- A procura atual de eletricidade da cidade.
- Os edifícios que ainda possam precisar de eletricidade.
- A **Reserva de eletricidade** configurada, 20% por predefinição.
- A eletricidade comprada às cidades vizinhas.
- A eletricidade vendida às cidades vizinhas.

**A reserva aplica-se apenas à eletricidade que a cidade precisa de produzir por si própria.**

Por exemplo, se a cidade precisa de 5.000 unidades e importa 2.000 de uma vizinha, as suas centrais só têm de cobrir as restantes 3.000 unidades, mais a reserva configurada.

A eletricidade vendida aos vizinhos também tem de ser produzida, pelo que as exportações aumentam a necessidade local.

**Capacidade local de produção necessária**

```math
N = \max\left(\left\lceil R \times \max(0,D-B)\right\rceil,\ Q-B\right)+S
```

Onde:

- **D** é a procura de eletricidade da cidade.
- **Q** é a necessidade total de eletricidade do jogo, incluindo os edifícios atualmente sem eletricidade.
- **B** é a eletricidade comprada aos vizinhos com que o plano pode contar.
- **S** é a eletricidade vendida aos vizinhos.
- **R** é o multiplicador da reserva (1,20 para a reserva predefinida de 20%).
- **N** é a capacidade de produção exigida às centrais locais.

O Ministério usa o maior dos dois valores: a procura local com reserva ou a necessidade restante de eletricidade do jogo. As exportações são depois somadas.

**Exemplo: uma cidade com 5.000 unidades de procura**

Admita que a necessidade total do jogo é de 5.050 unidades e que a reserva é de 20%.

| Situação | Capacidade local necessária |
|---|---:|
| Sem importações nem exportações de eletricidade | 6.000 unidades |
| Importando 2.000 unidades | 3.600 unidades |

Sem importações:

```math
N = \max(5000 \times 1.20,\ 5050)=6000
```

Com 2.000 unidades importadas:

```math
N = \max(3000 \times 1.20,\ 3050)=3600
```

**O Ministério não compra uma reserva adicional por cima da eletricidade importada.** A sua reserva protege a parte da procura que as centrais locais têm de fornecer.

Se o Ministério gerir os contratos de eletricidade, planeia os contratos e o financiamento das centrais em conjunto.

Caso contrário, usa os contratos existentes, contando as compras apenas quando estão ativas de forma fiável. As compras pendentes, suspensas ou com falhas não são tratadas como fornecimento garantido. As vendas existentes são contadas como obrigações.

Veja [Acordos com os vizinhos](deals.md#neighbor-deals) para mais informações.

#### Passo 2: Quanto pode produzir cada central?

Depois de conhecida a capacidade necessária, o Ministério examina todas as centrais que pode gerir em segurança.

Para cada central, considera **percentagens de financiamento inteiras de 60% a 120%**.

Considera também o financiamento atual da central, mesmo que o tenha definido manualmente fora desse intervalo.

Para cada nível de financiamento possível, o Ministério calcula:

- A capacidade de produção disponível da central.
- A sua despesa mensal de funcionamento.
- O seu custo estimado de desgaste, quando a opção do envelhecimento está ligada.

**Capacidade de produção planeada**

```math
\text{Cap}(f) = \left\lfloor \text{SaídaMáx} \times \frac{\text{Curva}(f)}{100} \times \text{Cond} \right\rfloor
```

Onde:

- **SaídaMáx** é a produção máxima da central.
- **Curva(f)** é a eficiência do jogo ao nível de financiamento **f**.
- **Cond** é o estado atual da central, de 0 a 1.
- **Cap(f)** é a capacidade de produção resultante, arredondada para baixo.

O Ministério usa o **estado real** da central, não a capacidade que tinha quando era nova.

**Exemplo: uma central em bom estado**

Suponha que uma central tem:

- Produção máxima: 6.000 unidades.
- Estado atual: 98%.
- Financiamento proposto: 84%.

Com 84% de financiamento, a curva de eficiência do jogo dá 71,75%.

```math
\text{Cap}(84) = \left\lfloor 6000 \times 0.7175 \times 0.98 \right\rfloor = 4218
```

O Ministério conta, portanto, **4.218 unidades** de capacidade de produção desta central.

Repete este cálculo para os outros níveis de financiamento possíveis e para todas as centrais elegíveis.

**E as centrais que o Ministério não pode gerir?**

Algumas centrais não podem ser ajustadas em segurança. Por exemplo:

- Centrais com informações de financiamento não suportadas ou ilegíveis.
- Centrais cujos custos de funcionamento ou características de produção não podem ser determinados de forma fiável.
- Centrais de valorização energética de resíduos que estejam a queimar lixo, porque alterar o seu financiamento poderia interferir com o sistema de saneamento da cidade.

Estas centrais **não são ignoradas**.

O Ministério inclui no cálculo a sua capacidade de produção atual, mas deixa o seu financiamento inalterado. Os seus custos são contabilizados sem contar a mesma despesa duas vezes.

Se **Gerir a eletricidade** estiver desligado, todas as centrais mantêm o financiamento atual. Os contratos de eletricidade podem continuar a ser geridos em separado, se as respetivas opções continuarem ligadas.

<a id="power-optimizer"></a>
#### Passo 3: Encontrar a combinação mais barata

É aqui que o Ministério faz a diferença.

Imagine uma cidade com várias centrais. Cada central tem o seu próprio custo de funcionamento, a sua curva de eficiência e o seu estado.

Pode haver muitas combinações de financiamento possíveis que fornecem eletricidade suficiente.

**O Ministério compara essas combinações e escolhe a que tem o menor custo total modelado.**

Não baixa simplesmente primeiro a central mais cara nem financia todas as centrais com a mesma percentagem.

**Fórmula de optimização da eletricidade**

```math
\min_{f_1,\ldots,f_n}\sum_{i=1}^{n}\left(E_i(f_i)+W_i(f_i)\right)
\quad\text{sujeito a}\quad
K+\sum_{i=1}^{n}\text{Cap}_i(f_i)\ge N
```

Onde:

- **Eᵢ** é a despesa mensal de funcionamento da central *i*.
- **Wᵢ** é o seu custo estimado de desgaste, ou zero quando a opção do envelhecimento está desligada.
- **Capᵢ** é a sua capacidade disponível ao financiamento selecionado.
- **K** é a capacidade combinada das centrais cujo financiamento não pode ser alterado.
- **N** é a capacidade local necessária, calculada no Passo 1.

Em linguagem simples:

**Encontrar a combinação mais barata de níveis de financiamento das centrais que produza eletricidade suficiente para a cidade e para a sua reserva.**

Por exemplo, a solução mais económica poderia ser:

| Central | Financiamento escolhido |
|---|---:|
| Central a carvão | 91% |
| Central a gás natural | 61% |

Outra cidade com procura, centrais ou custos de funcionamento diferentes poderia beneficiar de uma combinação completamente diferente.

Veja o [Exemplo 3](#power-example-mixed) para um cálculo detalhado com duas centrais.

**Como encontra o Ministério a melhor combinação?**

Usa um algoritmo de optimização que compara em conjunto as opções de financiamento das centrais, em vez de tomar decisões independentes para cada central.

Para manter os cálculos rápidos em cidades grandes, organiza a capacidade de produção em pequenas unidades.

**Unidade de capacidade da optimização**

```math
\text{unidade} = \max\left(1,\left\lceil\frac{N}{20000}\right\rceil\right)
```

Para cidades que necessitam de até **20.000 unidades**, a capacidade de produção é comparada uma unidade de cada vez.

Para cidades maiores, o Ministério usa unidades ligeiramente maiores para reduzir o volume de cálculo.

Isto pode resultar num plano com um pouco mais de capacidade de reserva do que o mínimo matemático, mas o arredondamento é conservador: **o plano selecionado tem sempre de cumprir a capacidade de produção necessária**.

Ao comparar muitos contratos regionais possíveis, o planeador de contratos usa um cálculo mais grosseiro, por velocidade. O plano final das centrais usa o cálculo mais detalhado.

**E se várias combinações custarem o mesmo?**

O Ministério prefere a combinação que altera o menor número de centrais.

Por exemplo, se manter o financiamento atual custar exatamente o mesmo que alterar três centrais, deixa-as como estão.

Isto evita ajustes de orçamento desnecessários.

**E se não houver eletricidade suficiente?**

Por vezes, nem financiar todas as centrais disponíveis à sua definição de maior capacidade é suficiente.

Nesse caso, o Ministério:

1. Escolhe a maior capacidade de produção disponível das centrais geridas, normalmente subindo-as para 120%.
2. Tenta cobrir a falta restante com compras regionais, se a gestão de contratos de eletricidade estiver ligada.
3. Emite um aviso de conselheiro se a falta for confirmada e persistir.

**O Ministério nunca constrói nem substitui centrais.** Se o financiamento e as compras disponíveis não resolverem o problema, tem de ser o jogador a providenciar capacidade de produção adicional.

<a id="power-wear"></a>
#### Opcional: ter em conta o envelhecimento das centrais

Por predefinição, o Ministério concentra-se em **minimizar a fatura mensal de eletricidade**.

No entanto, o orçamento de funcionamento mais barato nem sempre é a escolha mais barata ao longo da vida de uma central.

Como se explicou atrás, reduzir o financiamento pode aumentar a intensidade de utilização de uma central, acelerando potencialmente a sua deterioração.

A definição opcional **Ter em conta o desgaste das centrais** permite ao Ministério incluir um custo estimado desse desgaste.

| Definição | Como se optimiza o financiamento |
|---|---|
| **Desligada (predefinição)** | Minimiza as despesas mensais reais de funcionamento, mantendo a capacidade e a reserva necessárias. |
| **Ligada** | Minimiza as despesas de funcionamento mais o custo económico estimado do desgaste das centrais. |

**Importante:** o custo estimado de desgaste não é um encargo adicional para o tesouro da cidade. É usado apenas para comparar opções de financiamento.

As centrais continuam a envelhecer normalmente, com esta opção ligada ou não.

#### Como se calcula o custo do envelhecimento?

Cada central tem um custo de construção e um limiar de envelhecimento.

O Ministério estima quanto do estado útil restante da central é consumido durante um mês simulado e atribui depois um valor monetário a esse desgaste.

**Custo mensal estimado de desgaste**

```math
W(f) = \mathrm{round}\left(P \times \frac{\min\left(A(f),\ \text{Cond}-T\right)}{1-T}\right)
```

Esta fórmula aplica-se enquanto o estado da central estiver acima do seu limiar de envelhecimento. Caso contrário, o custo de desgaste modelado é zero.

Onde:

- **P** é o custo de construção ou de substituição da central.
- **A(f)** é o seu envelhecimento mensal estimado ao nível de financiamento em consideração.
- **Cond** é o seu estado atual.
- **T** é o limiar de estado a partir do qual o jogo a considera envelhecida.
- **W(f)** é o custo económico estimado do desgaste por mês simulado.

Por exemplo, se uma central custa §10.000 a construir e o intervalo útil de estado vai de 100% até 90%, o Ministério trata esse intervalo de estado de 10% como tendo um valor económico de §10.000.

Uma pequena parte desse intervalo consumida durante um mês representa a fração correspondente do custo de substituição.

O resultado é arredondado para simoleões inteiros.

**Custos de construção das centrais do jogo original**

| Central | Custo de substituição |
|---|---:|
| Eólica | §500 |
| A gás natural | §9.000 |
| A carvão | §10.000 |
| A petróleo | §17.000 |
| De valorização energética de resíduos | §25.000 |
| Solar | §30.000 |
| Nuclear | §40.000 |
| De hidrogénio | §100.000 |

Para as centrais personalizadas compatíveis, o Ministério lê o custo de construção fornecido pelo edifício.

**E se faltar a informação necessária?**

Se o custo de substituição, a taxa de envelhecimento ou o limiar de envelhecimento da central não puderem ser determinados de forma fiável, o Ministério deixa simplesmente o custo de desgaste fora do cálculo **apenas para essa central**.

A central pode continuar a ser gerida normalmente a partir da sua despesa de funcionamento.

As outras centrais com informação de envelhecimento válida continuam a incluir os seus custos estimados de desgaste.

#### Que diferença faz esta opção?

O efeito é muitas vezes menor do que os jogadores poderiam esperar.

Dentro do intervalo de financiamento do Ministério, de 60% a 120%, o multiplicador direto de envelhecimento por financiamento varia apenas de **1,04 a 0,90**.

O Ministério calcula o custo de desgaste de cada candidata usando a **utilização atualmente observada** da central. Não prevê exatamente como o SimCity 4 redistribuirá a carga de eletricidade depois de cada alteração de financiamento possível.

Por conseguinte, o custo estimado de desgaste costuma variar apenas ligeiramente entre níveis de financiamento.

Para muitas centrais do jogo original, isto representa apenas alguns simoleões por mês.

**Exemplo**

Suponha que o Ministério compara duas opções de financiamento:

| Opção | Despesa de funcionamento | Desgaste estimado | Custo total modelado |
|---|---:|---:|---:|
| A | §200 | §8 | §208 |
| B | §205 | §2 | §207 |

<italic>Valores ilustrativos, não medições de uma central específica.</italic>

Com o envelhecimento desligado, a **opção A** ganha, porque a sua despesa mensal de funcionamento é mais baixa.

Com o envelhecimento ligado, a **opção B** ganha, porque o seu custo combinado de funcionamento e desgaste é ligeiramente mais baixo.

A despesa mensal real de eletricidade da cidade seria de §205, e não §207. Os §2 adicionais existem apenas na comparação económica.

Na prática, os dois modos de optimização escolhem muitas vezes a mesma combinação de financiamento, sobretudo quando a diferença de desgaste é pequena.

Veja o [Exemplo 5](#power-example-wear) para um cálculo com os dados de centrais do jogo.

**O que esta opção não faz**

Ligar **Ter em conta o desgaste das centrais** não:

- Impede as centrais de se deteriorarem.
- Obriga as centrais a funcionar abaixo de 80% de utilização.
- Introduz um nível mínimo de financiamento adicional.
- Substitui automaticamente as centrais que envelhecem.
- Reduz a reserva de eletricidade da cidade nem outros requisitos de segurança.

Apenas permite ao Ministério considerar **o custo económico estimado do desgaste a longo prazo a par da fatura mensal de funcionamento**.

O objetivo continua o mesmo: **fornecer eletricidade suficiente à sua cidade ao menor custo modelado, com as opções e os limites de segurança que escolheu.**
<a id="power-cadence"></a>
#### Passo 4: Quando ajusta o Ministério o financiamento da eletricidade?

O Ministério acompanha regularmente o fornecimento de eletricidade da cidade e ajusta o financiamento das centrais quando necessário.

**Normalmente revê o financiamento a cada 10 dias simulados, mas pode reagir mais cedo se a procura ou a oferta de eletricidade mudarem de forma significativa.**

Todos os tempos se referem ao tempo simulado do jogo.

| Quando | O que faz o Ministério |
|---|---|
| **Quando se abre uma cidade** | Faz a primeira avaliação assim que as informações de eletricidade do jogo estão disponíveis e coerentes. Não há um período de espera fixo. |
| **Todos os dias simulados** | Verifica rapidamente a procura, a oferta, o consumo, a capacidade de produção e os contratos regionais de eletricidade, à procura de alterações significativas. |
| **A cada 10 dias simulados** | Faz uma avaliação regular das centrais e do seu financiamento. |
| **Quando se deteta uma alteração significativa** | Recalcula o plano de eletricidade sem esperar pela revisão regular seguinte. |

**O que desencadeia uma avaliação antecipada?**

Nas suas verificações diárias, o Ministério procura:

- **Uma falta real de eletricidade:** a cidade não tem eletricidade suficiente, vinda das centrais e de importações utilizáveis, para satisfazer a procura.
- **Uma variação de pelo menos 1% na procura de eletricidade**, em comparação com a última avaliação.
- **Uma variação de pelo menos 1% na capacidade de produção local.**
- **Uma variação de pelo menos 1% nas importações ou exportações de eletricidade contratadas.**

Uma leitura inalterada não desencadeia nova avaliação.

O Ministério distingue também entre uma falta genuína de eletricidade e um problema nas ligações elétricas da cidade.

Por exemplo, se houver eletricidade suficiente, mas alguns edifícios estiverem desligados da rede, o Ministério não aumenta desnecessariamente o financiamento das centrais. O problema de ligação fica registado no registo.

**Como são aplicadas as alterações de financiamento?**

Quando o Ministério escolhe um novo nível de financiamento, aplica-o diretamente.

Por exemplo, uma central pode passar de 60% para 95% de financiamento num único ajuste. Não há aumento gradual nem tempo de espera artificial.

No entanto, **os aumentos têm sempre prioridade sobre as reduções**.

O Ministério aumenta o financiamento onde é necessário e garante que as outras partes do plano, como novas compras de eletricidade, estão em vigor antes de fazer reduções numa avaliação posterior.

Isto evita faltas temporárias de capacidade enquanto várias centrais ou contratos regionais estão a ser ajustados.

Cada avaliação usa as informações atuais da cidade. Se alterar manualmente o financiamento de uma central, ou se gravar e reabrir a cidade, o Ministério tem em conta a nova situação.

<a id="power-safety"></a>
### 4. Condições de segurança

O Ministério segue várias regras de segurança para evitar que os seus ajustes de financiamento perturbem o fornecimento de eletricidade.

#### Antes de calcular um plano

O Ministério só toma decisões de financiamento quando as informações do jogo são fiáveis.

- A procura de eletricidade e as outras leituras necessárias têm de estar disponíveis e ser coerentes.
- Uma leitura de procura igual a zero é tratada como possivelmente não inicializada, e não automaticamente como prova de que a cidade não precisa de eletricidade.
- Pelo menos uma central tem de ter informações legíveis de financiamento e de produção antes de o financiamento das centrais poder ser gerido.
- Leituras contraditórias, como um consumo de eletricidade comunicado que excede a procura em mais de 1% mais uma unidade, impedem alterações automáticas de financiamento.

Uma cidade **sem centrais** pode, ainda assim, ser avaliada quanto a compras de eletricidade às cidades vizinhas, se a gestão de contratos regionais estiver ligada.

#### Proteger o fornecimento de eletricidade

**O Ministério nunca reduz deliberadamente o financiamento abaixo da capacidade exigida pelas suas regras de segurança.**

Antes de cada redução de financiamento, o Ministério:

1. Lê de novo as informações de eletricidade da cidade.
2. Verifica a capacidade de produção atualizada de cada central, usando o seu estado atual e o financiamento planeado.
3. Conta apenas as compras de eletricidade que já estão ativas e utilizáveis.
4. Verifica se a capacidade restante cobrirá a procura local, a reserva configurada e as exportações.

Se a redução deixasse capacidade insuficiente, **é cancelada**.

Uma compra de eletricidade pendente, suspensa ou com falhas nunca é tratada como fornecimento garantido.

Os aumentos de financiamento não são rejeitados só porque a cidade continua com falta de eletricidade. Mesmo que a produção máxima não consiga resolver toda a falta, aumentar a capacidade disponível pode ajudar na mesma.

#### Proteger as centrais e as linhas elétricas

- Os novos objetivos de financiamento variam normalmente de **60% a 120%**, em percentagens inteiras.
- O Ministério respeita os limites de financiamento aceites pelo sistema de orçamento do jogo.
- Rejeita alterações de financiamento que fariam a rede de linhas elétricas deteriorar-se.
- Quando a cidade tem um défice genuíno de eletricidade, o planeador de contratos regionais não mantém vendas de eletricidade.

O Ministério nunca constrói, demole, substitui nem desliga uma central. As suas ações limitam-se a alterar definições de financiamento elegíveis.

#### Verificar cada alteração de financiamento

Antes de alterar o financiamento de uma central, o Ministério verifica se o valor atual ainda corresponde ao que foi usado para calcular o plano.

Se tiver alterado esse valor manualmente, o Ministério ignora o ajuste e volta a calcular numa avaliação posterior.

Identifica também as centrais pela sua posição no mapa da cidade. Se duas centrais não puderem ser distinguidas de forma fiável na mesma posição, não adivinha qual modificar.

**Depois de cada alteração, o Ministério verifica se o jogo a aceitou.**

Tanto o valor de financiamento da central como a sua linha de orçamento têm de refletir a nova definição.

Se a verificação falhar:

- O Ministério tenta anular as alterações feitas durante essa revisão.
- A gestão automática do financiamento das centrais pára durante o resto da sessão atual da cidade.
- Abrir a cidade de novo permite que a gestão recomece.
- Os outros serviços e os contratos regionais de eletricidade continuam a funcionar de forma independente.

**A prioridade é preservar um fornecimento de eletricidade fiável, mesmo que isso signifique abdicar de uma poupança potencial.**

Veja [Segurança geral](safety.md#safety) para as salvaguardas comuns a todos os serviços.

<a id="power-settings"></a>
### 5. Definições

A gestão da eletricidade pode ser configurada em **Opções de Jogo → Ministério da Optimização**, na página **Serviços**.

| Definição | Predefinição | Intervalo | O que faz |
|---|---|---|---|
| [Ministério ativo](options.md#opt-master) | Ligado | Ligado / Desligado | Ativa ou desativa todo o Ministério. |
| [Gerir a eletricidade](options.md#opt-power) | Ligado | Ligado / Desligado | Optimiza automaticamente o financiamento individual das centrais. |
| [Reserva de eletricidade](options.md#opt-powerreserve) | 20% | 10–50% | Define a capacidade local de produção de reserva mínima que o Ministério procura manter. |
| [Ter em conta o desgaste das centrais](options.md#opt-poweraging) | Desligado | Ligado / Desligado | Inclui o desgaste estimado das centrais ao comparar custos de financiamento. |
| [Gerir os acordos com os vizinhos](options.md#opt-deals) | Ligado | Ligado / Desligado | Ativa a gestão automática dos contratos regionais. |
| [Gerir acordos de eletricidade](options.md#opt-tradepower) | Ligado | Ligado / Desligado | Ativa a gestão automática das compras e vendas de eletricidade. |

A opção [Comprar a uma cidade para vender a outra](options.md#opt-arbitrage) controla se o Ministério pode comprar eletricidade a uma cidade vizinha para a revender a outra. Veja [Acordos com os vizinhos](deals.md#neighbor-deals).

#### Como funciona a reserva de eletricidade?

Com uma reserva de 20%, o Ministério procura normalmente manter mais 20% de capacidade local de produção do que a eletricidade que a cidade tem de produzir por si própria.

Por exemplo, se a cidade precisa de produzir 5.000 unidades localmente, o Ministério procura pelo menos **6.000 unidades de capacidade**.

Trata-se de um **mínimo**, não de um máximo. O Ministério pode manter mais capacidade quando as definições disponíveis das centrais ou vendas de eletricidade rentáveis o justificarem.

Aumentar a reserva pode dar proteção adicional contra o crescimento e as variações da procura, mas pode também aumentar os custos de funcionamento.

Se desligar **Gerir a eletricidade**, o Ministério deixa o financiamento das centrais inalterado. Pode continuar a gerir as compras e vendas de eletricidade se as opções correspondentes continuarem ligadas.

#### Definições avançadas (apenas no ficheiro INI)

| Definição | Predefinição | Intervalo permitido | O que faz |
|---|---|---|---|
| `PowerReservePercent` | 20 | 10–50 | Define a percentagem de reserva de eletricidade. |
| `PowerCadenceDays` | 10 | 1–30 | Define com que frequência se fazem as revisões regulares do financiamento da eletricidade, em dias simulados. |

Por exemplo, definir `PowerCadenceDays=5` faz o Ministério efetuar uma revisão regular a cada cinco dias simulados em vez de dez.

**As verificações diárias de alterações urgentes no fornecimento mantêm-se separadas deste intervalo.**

Um valor inválido de `PowerReservePercent` torna inválido o ficheiro de configuração dos serviços. Nessa situação, o Ministério não gere nenhum serviço até a configuração ser corrigida.

Para a maioria dos jogadores, as predefinições são um bom ponto de partida.

<a id="power-examples"></a>
### 6. Exemplos práticos

Os exemplos seguintes mostram como o Ministério escolhe o financiamento para diferentes centrais e necessidades de eletricidade.

Todas as capacidades, despesas de funcionamento e estados das centrais são ilustrativos. Os cálculos usam as curvas de eficiência do jogo original.

Salvo indicação em contrário, a reserva de eletricidade é de **20%** e não há contratos regionais de eletricidade.

Os valores de envelhecimento da central a carvão usam valores medidos durante os testes: uma taxa base de envelhecimento de 0,001111 por mês e um limiar de envelhecimento de 0,9. Os mesmos parâmetros de envelhecimento são usados para a central a gás natural quando necessário para ilustração.

<a id="power-example-coal"></a>
#### Exemplo 1: Uma central a carvão com capacidade por usar

Considere uma central a carvão com:

- **Produção máxima:** 6.000 unidades.
- **Custo mensal de funcionamento a 100%:** §253.
- **Estado atual:** 98%.
- **Procura da cidade:** 3.500 unidades.
- **Necessidade do jogo:** 3.550 unidades.
- **Reserva de eletricidade:** 20%.

**Passo 1: Calcular a capacidade necessária**

```math
N = \max(3500 \times 1.20,\ 3550)=4200
```

A cidade precisa de **4.200 unidades** de capacidade local de produção.

**Passo 2: Comparar níveis de financiamento**

| Financiamento | Capacidade disponível | Custo mensal | Resultado |
|---|---:|---:|---|
| 83% | 4.138 | §210 | Não chega |
| **84%** | **4.218** | **§213** | **Chega** |
| 85% | 4.299 | §215 | Chega, mas é mais caro |
| 100% | 5.880 | §253 | Muito mais capacidade do que a necessária |

**O Ministério escolhe 84% de financiamento.**

A central pode produzir 4.218 unidades, um pouco mais do que as 4.200 necessárias.

O seu custo mensal de funcionamento desce de §253 para §213.

<box gap={1}>
**Poupança mensal**
<title size="xl" color="success">§40</title>
</box>

**O que acontece ao envelhecimento da central?**

Com 100% de financiamento, a central pode produzir 5.880 unidades, enquanto a cidade retira 3.500. A sua utilização é de aproximadamente **59,5%**.

Com 84% de financiamento, a produção disponível desce para 4.218 unidades. Se a cidade continuar a retirar 3.500, a utilização sobe para aproximadamente **83%**.

A perda mensal de estado modelada muda:

| Financiamento | Utilização da central | Perda mensal de estado estimada |
|---|---:|---:|
| 100% | 59,5% | 0,0000418 |
| 84% | 83,0% | 0,0000750 |

**A taxa de envelhecimento estimada passa a ser quase 1,8 vezes superior.**

A maior parte dessa diferença vem da maior utilização, e não diretamente do menor financiamento.

Isto ilustra por que razão reduzir os custos de funcionamento pode acelerar o envelhecimento das centrais, e por que o cálculo opcional do envelhecimento pode ser útil.

<a id="power-example-gas"></a>
#### Exemplo 2: Uma central a gás natural

Considere uma central a gás natural com:

- **Produção máxima:** 3.000 unidades.
- **Custo mensal a 100%:** §230.
- **Estado:** 100%.
- **Procura da cidade:** 2.000 unidades.
- **Necessidade do jogo:** 2.020 unidades.

Com uma reserva de 20%, a cidade precisa de:

```math
N = \max(2000 \times 1.20,\ 2020)=2400
```

O Ministério compara os níveis de financiamento disponíveis:

| Financiamento | Capacidade disponível | Resultado |
|---|---:|---|
| 89% | 2.358 | Não chega |
| **90%** | **2.400** | **Chega** |
| 100% | 3.000 | Mais do que o necessário |

**O Ministério escolhe 90% de financiamento.**

O custo mensal de funcionamento passa a ser:

```math
E = 230 \times 0.90 = 207
```

A central continua a fornecer a eletricidade e a reserva necessárias, poupando **§23 por mês**.

<a id="power-example-mixed"></a>
#### Exemplo 3: Duas centrais a trabalhar em conjunto

Este exemplo mostra por que razão o Ministério não financia todas as centrais com a mesma percentagem.

Imagine uma cidade com as duas centrais dos exemplos anteriores:

| Central | Produção máxima | Estado | Custo a 100% |
|---|---:|---:|---:|
| A carvão | 6.000 | 98% | §253 |
| A gás natural | 3.000 | 100% | §230 |
| **Total** | **9.000** | | **§483** |

A cidade tem:

- **Procura de eletricidade:** 5.000 unidades.
- **Necessidade do jogo:** 5.050 unidades.
- **Reserva de eletricidade:** 20%.

O Ministério calcula uma capacidade local necessária de:

```math
N = \max(5000 \times 1.20,\ 5050)=6000
```

**A cidade precisa de 6.000 unidades de capacidade de produção.**

O Ministério compara diferentes estratégias de financiamento:

| Estratégia de financiamento | Capacidade combinada | Custo mensal |
|---|---:|---:|
| Ambas as centrais a 100% | 8.880 | §483 |
| Ambas as centrais a 81% | 6.004 | §391 |
| **Carvão a 91%, gás a 61%** | **6.024** | **§370** |

**O Ministério escolhe 91% para a central a carvão e 61% para a central a gás natural.**

A capacidade resultante é:

```math
\text{Carvão} = \lfloor6000\times0.82\times0.98\rfloor=4821
```

```math
\text{Gás} = \lfloor3000\times0.40125\rfloor=1203
```

Em conjunto, fornecem **6.024 unidades**, o suficiente para satisfazer a necessidade.

As despesas mensais são:

```math
\text{Carvão} = \mathrm{round}(253\times0.91)=230
```

```math
\text{Gás} = \mathrm{round}(230\times0.61)=140
```

<grid columns={2} gap={3}>
  <grid-item>
    <box gap={1}>
      <text color="secondary" size="xs">Em comparação com 100% de financiamento</text>
      <title size="xl" color="success">§113/mês poupados</title>
    </box>
  </grid-item>
  <grid-item>
    <box gap={1}>
      <text color="secondary" size="xs">Em comparação com cortes iguais</text>
      <title size="xl" color="success">§21/mês poupados</title>
    </box>
  </grid-item>
</grid>

Isto é possível porque **a eficiência das centrais não é proporcional ao financiamento**.

Neste caso, manter a central a carvão perto da eficiência máxima e a central a gás perto do financiamento mínimo é mais barato do que reduzir ambas por igual.

**E se a cidade vender eletricidade?**

Suponha que assina um contrato para vender **1.000 unidades** a uma cidade vizinha.

A capacidade local necessária sobe de 6.000 para **7.000 unidades**.

A combinação mais barata do Ministério passa a ser:

| Central | Financiamento | Capacidade disponível |
|---|---:|---:|
| A carvão | 100% | 5.880 |
| A gás natural | 60% | 1.162 |
| **Total** | | **7.042** |

O custo mensal de funcionamento é de **§391**.

O Ministério aumenta, portanto, a produção para cobrir o contrato de exportação sem sacrificar o fornecimento da própria cidade.

**E se a procura ficar demasiado alta?**

Com 120% de financiamento, estas duas centrais podem fornecer, no máximo e em conjunto, **9.768 unidades**.

Se a capacidade local necessária exceder esse valor, o Ministério sobe as centrais para as suas definições de maior capacidade, a um custo mensal combinado de **§580**.

Se a gestão de contratos regionais estiver ligada, tenta também comprar eletricidade suficiente para cobrir a falta restante.

Caso contrário, o jogador tem de providenciar capacidade de produção adicional.

<a id="power-example-trade"></a>
#### Exemplo 4: Comprar eletricidade a uma cidade vizinha

Considere a mesma cidade com duas centrais do Exemplo 3.

A sua procura de eletricidade é de 5.000 unidades, mas tem agora um contrato ativo para comprar **2.000 unidades** a uma cidade vizinha.

As centrais locais só precisam de fornecer as restantes 3.000 unidades, mais a reserva de 20%.

```math
N = \max(3000\times1.20,\ 5050-2000)=3600
```

**Capacidade local necessária: 3.600 unidades.**

O Ministério escolhe:

| Central | Financiamento | Capacidade | Custo mensal |
|---|---:|---:|---:|
| A carvão | 62% | 2.440 | §157 |
| A gás natural | 60% | 1.162 | §138 |
| **Total** | | **3.602** | **§295** |

Sem a compra, as centrais custam §370 por mês para fornecer a eletricidade e a reserva da cidade.

Com a compra, o seu custo de funcionamento desce para **§295 por mês**.

É uma redução de **§75 por mês nas despesas das centrais locais**.

No entanto, a cidade tem também de pagar a eletricidade importada.

**Se a compra poupa realmente dinheiro depende do preço do contrato.**

O planeador de contratos regionais do Ministério compara o custo da compra com a poupança na produção local antes de decidir se o contrato compensa.

Veja [Acordos com os vizinhos](deals.md#neighbor-deals).

**Porque não reduzir mais as centrais?**

Normalmente, o Ministério não seleciona um financiamento abaixo de 60%.

Mesmo que ambas as centrais sejam financiadas a 60%, continuam a fornecer aproximadamente 3.440 unidades e custam §290 por mês.

Este financiamento mínimo impede o Ministério de reduzir a zero centrais desnecessárias através da sua optimização normal.

<a id="power-example-wear"></a>
#### Exemplo 5: Ter em conta o desgaste das centrais altera o resultado?

Voltemos à central a carvão do Exemplo 1.

As suas características são:

- **Custo de substituição:** §10.000.
- **Estado atual:** 98%.
- **Limiar de envelhecimento:** 90%.
- **Taxa base de envelhecimento:** 0,001111 por mês.
- **Utilização observada antes da otimização:** 59,5%.

Ative agora **Ter em conta o desgaste das centrais**.

O Ministério estima o custo económico do desgaste em cada nível de financiamento possível, usando a utilização atualmente observada da central.

| Financiamento | Custo operacional mensal | Custo estimado do desgaste | Custo total modelado |
|---|---:|---:|---:|
| **84%** | **§213** | **§4** | **§217** |
| 85% | §215 | §4 | §219 |
| 100% | §253 | §4 | §257 |
| 120% | §304 | §4 | §308 |

**O Ministério continua a escolher 84% de financiamento.**

Porquê?

Porque a diferença no custo estimado do desgaste é demasiado pequena para justificar gastar mais no orçamento mensal da central.

Os §4 de custo de desgaste não saem do tesouro da cidade. É uma estimativa económica usada apenas para comparar opções.

**E a cidade com duas centrais?**

No Exemplo 3, com uma utilização observada de 50% na central a carvão e de 60% na central a gás, ativar o desgaste produz o mesmo resultado:

- Central a carvão: 91%.
- Central a gás natural: 61%.
- Custo operacional mensal: §370.
- Desgaste estimado: §7.
- Custo total modelado: §377.

Mais uma vez, a combinação economicamente mais barata é também a combinação operacionalmente mais barata.

**O envelhecimento pode alguma vez fazer diferença?**

Sim, sobretudo nas centrais caras.

Por exemplo, considere-se uma central com:

- Custo de substituição: §40.000.
- Taxa base de envelhecimento: 0,001389 por mês.
- Utilização observada: 90%.

O seu custo mensal estimado de desgaste é:

| Financiamento | Custo estimado do desgaste |
|---|---:|
| 60% | §45 |
| 100% | §43 |
| 120% | §39 |

A diferença entre 60% e 120% de financiamento é de **§6 por mês**.

Isso pode influenciar a escolha quando duas combinações de financiamento têm despesas operacionais quase idênticas.

No entanto, não torna automaticamente preferível um financiamento mais alto.

**Quanto tempo pode durar uma central elétrica?**

Com o mesmo modelo de envelhecimento, a central a carvão do Exemplo 1 demoraria aproximadamente o seguinte número de meses simulados a passar de um estado de 98% ao seu limiar de envelhecimento de 90%, assumindo utilização constante e 100% de financiamento:

| Utilização da central | Tempo estimado até ao limiar de envelhecimento |
|---|---:|
| 50% | 2.560 meses |
| 80% | 1.170 meses |
| 90% | 920 meses |
| 100% | 740 meses |

Trata-se de **estimativas teóricas**, não de durações de vida garantidas.

O jogo real pode comportar-se de forma diferente à medida que a utilização, o financiamento e o estado se alteram.

**Uma limitação importante da opção de envelhecimento**

O Ministério usa a **utilização atualmente observada** de cada central para estimar o desgaste nos vários níveis de financiamento possíveis.

Não prevê com exatidão como o jogo vai redistribuir a procura de eletricidade depois dessas alterações de financiamento.

Como demonstra o Exemplo 1, reduzir o financiamento pode aumentar bastante a utilização real, o que pode acelerar o envelhecimento mais do que a estimativa económica inicial sugere.

O Ministério observa a nova utilização nas avaliações seguintes.

**Ter em conta o desgaste das centrais é, portanto, uma estimativa económica, não uma garantia de uma vida mais longa para as centrais.**

<a id="power-limitations"></a>
### 7. Limitações conhecidas

O Ministério gere automaticamente o financiamento da eletricidade, mas vários aspetos continuam a ser controlados pelo SimCity 4 ou pelo jogador.

#### Compatibilidade das centrais elétricas

- **As curvas de eficiência e de envelhecimento do jogo original estão incorporadas no Ministério.** Se outro mod alterar essas curvas, o Ministério não deteta automaticamente os novos valores.
- **As propriedades de cada central são lidas do jogo.** A produção máxima, o estado, o custo operacional, os parâmetros de envelhecimento e o custo de construção são obtidos de cada central, o que permite gerir centrais elétricas personalizadas compatíveis.
- **Algumas centrais não podem ser geridas em segurança.** Sistemas de financiamento não suportados ou informação ilegível fazem com que uma central seja excluída das alterações automáticas de financiamento.
- **As centrais de conversão de lixo em energia que estão a queimar lixo não são ajustadas.** O seu financiamento fica inalterado porque o mesmo edifício intervém no saneamento. A sua capacidade de produção disponível continua, ainda assim, a contar para o plano de eletricidade.

#### Financiamento e capacidade de produção

- **O Ministério não constrói nem substitui centrais.** Uma falta real exige capacidade de produção adicional ou compras de eletricidade.
- **O financiamento mínimo normal é de 60%.** Uma cidade com muito mais centrais do que as necessárias pode continuar a pagar pelo menos 60% do custo operacional nominal de cada central gerida.
- **As centrais nunca são desligadas deliberadamente através do financiamento.** O Ministério não escolhe novos objetivos abaixo do seu mínimo normal.
- **As cidades muito grandes usam arredondamento da capacidade.** Quando a capacidade necessária excede as 20.000 unidades, o otimizador pode manter um pouco mais de capacidade de produção do que o estritamente necessário. Nunca arredonda deliberadamente um plano para abaixo do necessário.

#### Envelhecimento das centrais elétricas

- **O cálculo do envelhecimento usa a utilização atualmente observada.** Não prevê com exatidão como uma alteração de financiamento vai redistribuir a produção de eletricidade entre as centrais.
- **O fator de tempo do envelhecimento baseia-se em medições.** O fator de 1/16 foi observado em testes e não lido diretamente dos dados internos do jogo.
- **Os custos de desgaste e as estimativas de duração são aproximados.** Destinam-se a apoiar decisões económicas, não a prever uma data exata de substituição.
- **O Ministério nunca substitui automaticamente as centrais envelhecidas.** O jogador continua responsável por construir substituições quando necessário.

#### Distribuição de eletricidade e comércio regional

- **A necessidade de eletricidade do jogo é usada tal como é comunicada.** O Ministério não tenta substituí-la por uma estimativa própria das necessidades totais da cidade.
- **Os problemas de ligação não são problemas de produção.** Se existir eletricidade suficiente mas ela não chegar a alguns edifícios, alterar o financiamento das centrais não resolve a questão.
- **Os contratos de eletricidade afetam o plano de financiamento.** As importações e as exportações são incluídas no cálculo das necessidades de capacidade local, mas só as compras utilizáveis são tratadas como oferta disponível.

**O papel do Ministério é gerir a eletricidade que a cidade pode produzir, não redesenhar a rede nem construir novas centrais elétricas.**

[Voltar ao índice](index.md#contents) | [Voltar ao README](../../README.pt.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
