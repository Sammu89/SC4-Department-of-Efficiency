[English](../en/education_healthcare.md) | **Português** | [Français](../fr/education_healthcare.md) | [Español](../es/education_healthcare.md)

[Voltar ao índice](index.md#contents) | [Voltar ao README](../../README.pt.md)

<a id="education-healthcare"></a>
## A. Educação e saúde

Os endereços do executável relacionados com este capítulo constam da [referência de endereços do jogo](../en/game_addresses.md) (em inglês).

O Ministério ajusta automaticamente o financiamento de cada escola, faculdade, universidade, biblioteca, museu, clínica e hospital que consiga gerir. O seu objetivo é simples: **dar capacidade suficiente a todos os que precisam do serviço, com uma margem de segurança, gastando o menos possível.**

Cada edifício é gerido individualmente. Uma escola com 200 alunos não precisa do mesmo orçamento que uma com 2.000 alunos.

O Ministério ajusta apenas o orçamento de funcionamento do edifício. **Nunca altera o financiamento dos autocarros escolares ou das ambulâncias, as áreas de cobertura dos edifícios, nem o número de equipamentos da cidade.** Essas decisões continuam a ser suas.

### Como o financiamento afeta a capacidade

No SimCity 4, cada edifício de educação e de saúde tem um orçamento de funcionamento, apresentado em simoleões (§) por mês. Nos bastidores, o jogo representa este financiamento como uma percentagem.

O financiamento determina tanto o custo do edifício como o número de alunos, doentes, visitantes ou livros que consegue acolher.

A relação não é totalmente proporcional:

| Financiamento | Capacidade disponível | Custo mensal de funcionamento |
|---|---:|---:|
| 50% | 50% | 50% |
| 75% | 75% | 75% |
| 100% | 100% | 100% |
| 120% | 110% | 120% |

As percentagens são relativas à capacidade e ao custo de funcionamento do edifício com 100% de financiamento.

**Até 100% de financiamento**, a capacidade aumenta ao mesmo ritmo que a despesa. **Acima de 100%**, cada ponto percentual adicional de financiamento dá apenas metade da capacidade extra.

Por exemplo, uma escola com 1.000 lugares a 100% de financiamento consegue acolher cerca de 500 alunos a 50%, ou 1.100 alunos a 120%.

<a id="education-healthcare-capacity-curve"></a>
**Fórmula da capacidade (jogo base)**

```math
\phi(f) = \min(f,1) + \frac{\max(0,f-1)}{2}
```

```math
C = \left\lfloor B \times \phi(f) \right\rfloor
```

Onde:

- **B** é a capacidade do edifício com 100% de financiamento.
- **f** é o nível de financiamento (1,0 = 100%).
- **C** é a capacidade resultante.
- **φ(f)** é o multiplicador de capacidade do jogo.

O jogo usa arredondamentos internos e pode aplicar modificadores de capacidade adicionais. Por isso, a capacidade apresentada pode por vezes diferir ligeiramente de um cálculo direto. O Ministério tem em conta estas diferenças ao decidir.

**Custo mensal de funcionamento (jogo base)**


```math
K = \mathrm{round}(K_0 \times f)
```

Onde **K₀** é o custo mensal de funcionamento do edifício com 100% de financiamento.

Por exemplo, uma escola que custa §200 por mês a 100% de financiamento custa cerca de §100 a 50%.

### Como o Ministério escolhe o financiamento

O Ministério verifica quantas pessoas utilizam cada edifício e calcula a capacidade que precisa de manter.

Por predefinição, procura manter **pelo menos 5% de capacidade de reserva**, mas pode manter uma margem maior quando necessário.

Isto é particularmente importante nos equipamentos pequenos, onde até um pequeno aumento da frequência pode representar uma grande variação percentual.

Por exemplo, uma clínica que passa de 40 para 60 doentes teve um aumento de 50% da procura. O Ministério tem em conta este tipo de oscilação, em vez de aplicar cegamente a mesma margem a todos os edifícios.

Escolhe depois o **nível de financiamento mais baixo que cumpre a capacidade necessária em segurança**.

Se a procura aumentar, o financiamento pode ser subido. Se a procura diminuir, o Ministério pode reduzi-lo para evitar despesa desnecessária.

### Com que frequência verifica e ajusta o financiamento?

Todos os prazos abaixo referem-se ao **tempo simulado do jogo**, não ao tempo real.

| Quando | O que o Ministério faz |
|---|---|
| **Quando se abre uma cidade** | Faz a primeira avaliação completa no primeiro dia simulado. Se alguns edifícios ainda não tiverem dados utilizáveis, pode repetir a tentativa nos dias seguintes. |
| **A cada 2 dias simulados** | Procura os edifícios que possam precisar de aumentos urgentes de financiamento. |
| **Todos os dias simulados durante uma greve** | Procura problemas urgentes para que o financiamento possa ser aumentado o mais depressa possível. |
| **Uma vez por mês simulado** | Reavê cada edifício por completo e ajusta o financiamento, para cima ou para baixo, conforme as necessidades atuais. |

**Os aumentos de financiamento nem sempre esperam pelo mês seguinte.** Se um edifício ficar seriamente sobrelotado, ou entrar em greve, o Ministério pode subir o financiamento nas verificações mais frequentes.

**As reduções de financiamento só acontecem na revisão mensal.** Assim evita-se cortar orçamentos depressa demais depois de uma descida temporária da frequência.

O Ministério nunca baixa o financiamento enquanto os números do próprio jogo indicarem risco de um serviço inadequado.

<a id="education-healthcare-strike"></a>
### Como o Ministério ajuda a evitar greves

No SimCity 4, as escolas e os equipamentos de saúde podem entrar em greve quando a sua capacidade é demasiado baixa face à procura.

O jogo usa uma **taxa de cobertura** para determinar este risco.

```math
V = \min\left(1,\frac{C}{U}\right)
```

Onde:

- **C** é a capacidade disponível do edifício.
- **U** é a sua procura.
- **V** é a taxa de cobertura.

Uma taxa de cobertura de **1,0 (100%)** significa que o edifício tem capacidade suficiente para a sua procura.

Uma taxa inferior a **0,8 (80%)** cria risco de greve. Quanto mais baixa a taxa, maior o risco.

**Probabilidade de greve (jogo base)**

```math
S(V) = 100 \times \max\left(0,1-\frac{V}{0.8}\right)
```

| Cobertura | Probabilidade de greve |
|---|---:|
| 100% | 0% |
| 80% | 0% |
| 60% | 25% |
| 40% | 50% |
| 0% | 100% |

Uma greve termina quando a taxa de cobertura do jogo chega aos 90%.

O Ministério mantém normalmente mais capacidade do que o mínimo necessário para evitar greves. Vigia também os edifícios à procura de aumentos inesperados da procura.

Se já houver uma greve em curso, **o Ministério sobe o financiamento do edifício até ao máximo configurado**, desde que consiga controlar esse edifício.

O cálculo interno de cobertura do jogo pode por vezes diferir da procura mostrada na janela de consulta do edifício. Por isso, o Ministério usa também a informação de cobertura do próprio jogo para evitar reduções de financiamento pouco seguras.

### O que acontece quando o financiamento já não chega?

O financiamento não resolve todos os problemas de capacidade.

Com 120% de financiamento, um edifício dá apenas cerca de 110% da sua capacidade normal. Se a procura exceder o que o edifício consegue suportar em segurança com o financiamento máximo, o Ministério não pode criar lugares adicionais.

**É preciso construir outro equipamento.**

Quando o Ministério confirma que uma falta de capacidade não se resolve com financiamento e que persiste durante **30 dias simulados**, envia um aviso de conselheiro no jogo.

Quando é possível, o aviso inclui uma ligação que leva diretamente ao edifício afetado.

O Ministério não constrói edifícios nem altera as suas áreas de cobertura. Continua a ser a si que cabe decidir onde colocar novas escolas, hospitais e outros equipamentos.

Para salvaguardas adicionais e casos especiais, consulte as [Condições de segurança](#education-healthcare-safety).
<a id="education-healthcare-problem"></a>
### 2. O problema da gestão

Gerir manualmente os orçamentos de educação e de saúde é mais complicado do que parece.

- **Dinheiro desperdiçado:** Uma escola meio vazia não precisa do mesmo financiamento que uma cheia. Manter todos os edifícios a 100% pode desperdiçar milhares de simoleões ao longo do tempo.
- **Risco de greves:** Cortar demasiado o financiamento reduz a capacidade e pode provocar greves. O Ministério tem de poupar dinheiro sem pôr os serviços em risco.
- **Procura variável:** O número de alunos e de doentes oscila à medida que a cidade cresce. Algumas variações podem ser súbitas, sobretudo quando os alunos passam de um nível de ensino para outro.
- **Edifícios pequenos:** Uma clínica que passa de 40 para 60 doentes sofre um aumento de 50% da procura. Os equipamentos pequenos precisam de proteção extra contra estas oscilações.
- **Edifícios novos:** O jogo pode demorar alguns dias simulados a indicar quantas pessoas utilizam um equipamento acabado de colocar.
- **Capacidade limitada:** Mesmo com 120% de financiamento, um edifício só dá cerca de 110% da sua capacidade normal. Daí em diante, cabe ao jogador construir outro equipamento.
- **Edifícios a mais para verificar:** Numa cidade grande, rever todos os meses, à mão, cada escola, biblioteca, museu, clínica e hospital torna-se fastidioso.

**O Ministério resolve isto vigiando cada edifício individualmente, mantendo capacidade suficiente para os serviços continuarem a funcionar e reduzindo a despesa desnecessária sempre que for seguro fazê-lo.**

<a id="education-healthcare-algorithm"></a>
### 3. Como o Ministério toma as suas decisões

<a id="education-healthcare-eligibility"></a>
#### 3.1 Que edifícios podem ser geridos?

O Ministério gere automaticamente os edifícios de educação e de saúde que usam o sistema de financiamento padrão do jogo.

Isto inclui:

| Educação | Saúde |
|---|---|
| Escolas primárias | Clínicas |
| Escolas secundárias | Hospitais |
| Faculdades e universidades | Centros de investigação de doenças |
| Bibliotecas | |
| Museus | |

Os edifícios personalizados compatíveis também podem ser geridos, desde que usem os mesmos mecanismos de financiamento do jogo original.

Antes de gerir um edifício, o Ministério verifica que:

1. O jogo o identifica como um equipamento de educação ou de saúde.
2. Tem um orçamento de funcionamento próprio que pode realmente ser ajustado.
3. A sua janela de consulta fornece informação reconhecível sobre procura e capacidade.
4. Os seus valores de financiamento e de orçamento são coerentes.

**Se um edifício não cumprir estas condições, o Ministério deixa-o em paz.**

Por exemplo, as escolas privadas do jogo original não têm um orçamento de funcionamento próprio que se possa ajustar, pelo que o Ministério não tem nada para gerir. Alguns edifícios personalizados usam também sistemas de financiamento ou janelas de consulta especiais que o Ministério não consegue controlar em segurança.

Os edifícios são identificados pela forma como funcionam, **não pelos seus nomes**. Uma escola personalizada compatível pode, por isso, ser gerida exatamente como uma escola original do SimCity 4.

As esquadras da polícia, os quartéis de bombeiros e as prisões não são geridos por estes serviços.

<a id="education-healthcare-required"></a>
#### 3.2 De quanta capacidade precisa um edifício?

O Ministério não se limita a igualar o financiamento à frequência atual. Mantém **capacidade extra para lidar com aumentos inesperados da procura**.

A margem de segurança predefinida é de **5%**, mas o Ministério pode manter automaticamente mais quando um edifício precisa de proteção adicional.

Considera três fatores:

**1. A margem de segurança que escolheu**

Por predefinição, um edifício que serve 1.000 pessoas deve ter capacidade para pelo menos 1.050.

Pode alterar esta margem nas definições do Ministério.

**2. Aumentos anteriores da procura**

O Ministério recorda o quanto a utilização de um edifício aumentou de um mês para o seguinte.

Usa até **36 observações mensais da sessão de jogo atual**, à procura do maior aumento. Só entram as comparações que partem de pelo menos 50 utilizadores, porque números muito pequenos podem produzir percentagens enganadoras.

Por segurança, o aumento usado no cálculo tem um teto de **50%**.

As escolas secundárias, as faculdades e as universidades recebem uma proteção especial. As transferências de alunos entre níveis de ensino podem causar grandes aumentos anuais, pelo que o Ministério assume um possível aumento de 50% até ter reunido 13 observações mensais.

**Maior aumento mensal**

```math
v = \min\left(0.5,\ \max\left(0,\ \max_i\left(\frac{U_i}{U_{i-1}}-1\right)\right)\right)
```

Onde:

- **Uᵢ** é o número de utilizadores registado no mês *i*.
- **v** é o maior aumento mensal válido, expresso como fração e limitado a 0,5.

**3. Proteção extra para os edifícios pequenos**

Os equipamentos pequenos podem sofrer grandes variações percentuais mesmo quando chegam apenas algumas pessoas.

Por isso, o Ministério mantém capacidade adicional:

| Utilização atual | Margem mínima de capacidade |
|---|---|
| Menos de 100 utilizadores | 50% de capacidade extra |
| 100–199 utilizadores | 25% de capacidade extra |
| 200 ou mais utilizadores | A margem que escolheu, ajustada às oscilações da procura quando necessário |

Por exemplo, uma clínica com 60 doentes mantém capacidade para pelo menos 90 doentes, mesmo que a margem de segurança configurada seja apenas de 5%.

Esta proteção adicional é particularmente útil nos edifícios pequenos, onde o custo extra de funcionamento costuma ser modesto.

**Como estas proteções funcionam em conjunto**

O Ministério compara os três requisitos e usa o que exigir mais capacidade:

- A margem de segurança que configurou.
- A proteção contra aumentos anteriores da procura e contra greves.
- A proteção extra para os edifícios pequenos.

**Fórmula da margem de segurança**

```math
M = \max\left(1+\frac{r}{100},\ 0.85(1+v),\ s(U)\right)
```

Onde:

- **r** é a margem de segurança que configurou, 5% por predefinição.
- **v** é o maior aumento mensal descrito acima.
- **U** é a utilização atual do edifício.
- **s(U)** é 1,5 abaixo de 100 utilizadores, 1,25 de 100 a 199 utilizadores e 0 nos restantes casos.
- **M** é o multiplicador de capacidade resultante.

O fator de 0,85 na segunda regra visa manter a cobertura acima do limiar de risco de greve de 80% do jogo, mesmo depois de um aumento da procura observado anteriormente.

**Capacidade necessária**

```math
R = \left\lceil U \times M \right\rceil
```

Onde **R** é a capacidade mínima que o Ministério quer que o edifício proporcione.

Por exemplo, uma escola com 610 alunos, sem aumentos da procura invulgarmente grandes e com uma margem de segurança de 5%, precisa de:

```math
R = \lceil 610 \times 1.05 \rceil = 641
```

O Ministério procura, assim, o nível de financiamento mais barato que dê pelo menos 641 lugares, com uma pequena tolerância para o comportamento de arredondamento do jogo.

<a id="education-healthcare-choice"></a>
#### 3.3 Como escolhe o Ministério o financiamento?

Depois de conhecida a capacidade necessária, o Ministério verifica que nível de financiamento a consegue proporcionar.

Escolhe **a percentagem inteira mais baixa que cumpre o requisito em segurança**.

Por predefinição, considera níveis de financiamento de **1% a 120%**.

**Escolha do financiamento**

```math
p^* = \min\left\{p\in[p_{\min},p_{\max}]: C(p)-1\ge R\right\}
```

Onde:

- **p** é uma percentagem de financiamento possível.
- **C(p)** é a capacidade esperada com esse financiamento.
- **R** é a capacidade necessária calculada acima.
- A subtração de uma unidade dá uma pequena salvaguarda contra o arredondamento da capacidade feito pelo jogo.

Por exemplo, se uma escola precisa de 641 lugares e 54% de financiamento dá aproximadamente 647, o Ministério seleciona **54%**, partindo do princípio de que 53% não dá capacidade suficiente.

Assim evita-se pagar centenas de lugares por utilizar.

**Regras de segurança adicionais**

O Ministério segue também várias regras importantes:

- **Sem alterações desnecessárias:** Se o financiamento já estiver no nível adequado, deixa-o como está.
- **Greves:** Se um edifício estiver em greve, o seu financiamento é subido diretamente para o máximo configurado, normalmente 120%.
- **Edifícios novos:** Se o jogo ainda não indicou a frequência, o Ministério não assume que o edifício está vazio. Mantém o financiamento pelo menos a 100%, ou no máximo configurado se este for inferior, até haver dados utilizáveis.
- **Sem reduções pouco seguras:** O financiamento nunca é baixado enquanto a cobertura indicada pelo jogo estiver abaixo dos 85% ou enquanto houver uma greve em curso.
- **Sem financiamento zero:** Na configuração predefinida, o Ministério nunca põe o financiamento de um edifício a 0%.
- **As reduções são mensais:** O financiamento pode ser aumentado em verificações urgentes, mas as reduções só se fazem na revisão mensal.

Se nem o financiamento máximo conseguir dar a capacidade pretendida, o Ministério mantém o edifício no máximo e verifica se existe uma falta de capacidade genuína.

Um edifício que fique ligeiramente abaixo da sua margem de segurança não precisa necessariamente de outro equipamento. O Ministério só avisa quando a capacidade disponível é realmente insuficiente e o problema persiste.

Consulte as [Faltas de capacidade](#education-healthcare-shortage) para ver as condições que desencadeiam um aviso de conselheiro.

<a id="education-healthcare-capacity-evidence"></a>
#### 3.4 E se a capacidade real do jogo for diferente do cálculo?

O SimCity 4 nem sempre indica exatamente a capacidade prevista pela sua fórmula de financiamento padrão.

Pequenas diferenças de arredondamento são normais. Algumas cidades ou edifícios personalizados podem também aplicar modificadores de capacidade adicionais.

Por isso, o Ministério **verifica a capacidade real indicada pelo jogo antes de alterar o financiamento de um edifício**.

Trata três situações:

| Situação | O que o Ministério faz |
|---|---|
| **A capacidade coincide com o cálculo** | Usa a fórmula padrão e gere o edifício normalmente. Uma diferença de um lugar é aceite como arredondamento normal. |
| **A capacidade segue um modificador consistente** | Aprende a diferença e ajusta os cálculos, tomando precauções até o modificador estar confirmado. |
| **A capacidade comporta-se de forma imprevisível** | Evita reduções automáticas de financiamento, em vez de arriscar uma perda inesperada de capacidade. |

**Aprender a capacidade real de um edifício**

Por exemplo, uma escola personalizada pode dar de forma consistente mais 15% de capacidade do que a fórmula padrão prevê.

Em vez de ignorar essa capacidade extra ou de contar logo com ela, o Ministério verifica se a diferença é consistente.

Quando é adequado, pode fazer um pequeno aumento de financiamento, seguro, durante uma revisão mensal para confirmar como a capacidade responde. Usa depois a relação verificada para escolher o financiamento com mais exatidão.

Esta calibração é prudente: só se tenta quando o edifício já tem capacidade suficiente, e não assume um bónus por confirmar.

Se a capacidade não puder ser prevista de forma fiável, o Ministério deixa o financiamento do edifício como está ou limita-se a aumentos seguros quando a capacidade observada é insuficiente.

**A prioridade é sempre evitar cortes de financiamento pouco seguros.** Poupar alguns simoleões não compensa o risco de afetar uma escola ou um hospital cuja capacidade o jogo indica de forma incoerente.

<a id="education-healthcare-timing"></a>
#### 3.5 Quando verifica e ajusta o Ministério o financiamento?

O Ministério vigia os edifícios de educação e de saúde durante toda a simulação. **Pode aumentar o financiamento quando surge um problema, mas só o reduz na revisão mensal.**

Todos os prazos se referem ao tempo simulado do jogo.

| Quando | O que acontece | Aumentos de financiamento | Reduções de financiamento |
|---|---|---|---|
| **Quando se abre uma cidade** | Verifica todos os edifícios geridos no primeiro dia simulado. Se alguns edifícios ainda não tiverem dados de frequência, pode repetir a verificação nos dias seguintes, até seis vezes no total. | Sim | Não |
| **A cada 2 dias simulados** | Procura os edifícios que precisam de atenção urgente por sobrelotação ou financiamento insuficiente. | Sim | Não |
| **Todos os dias durante uma greve** | Procura problemas urgentes de financiamento enquanto houver uma greve de educação ou de saúde assinalada. | Sim | Não |
| **Todos os meses simulados** | Reavê todos os edifícios, atualiza o histórico de frequência e ajusta o financiamento às necessidades atuais. | Sim | Sim |

**O que conta como problema urgente?**

O Ministério intervém entre as revisões mensais quando um edifício:

- Ainda não está no financiamento máximo.
- Precisa de um aumento de financiamento segundo as suas regras de segurança.
- Está em greve, ou a sua capacidade atual está mais de 3% abaixo da procura.

A última condição exprime-se assim:

```math
C < 0.97 \times U
```

Onde:

- **C** é a capacidade disponível do edifício.
- **U** é a sua utilização atual.

Por exemplo, uma escola com capacidade para 900 alunos e uma frequência de 1.000 está claramente sobrelotada. O Ministério pode aumentar-lhe o financiamento sem esperar pelo mês seguinte.

Um problema novo ou em mudança é avaliado nas verificações regulares. Se o mesmo problema persistir sem se alterar, o Ministério pode repetir a avaliação a cada **7 dias simulados**.

Um edifício que já esteja no financiamento máximo não pode ser ajudado por outro aumento. O Ministério trata essa situação como uma possível falta de capacidade.

**A prioridade é sempre repor primeiro os serviços e só depois optimizar a despesa.**

<a id="education-healthcare-buses"></a>
#### 3.6 O que altera o Ministério e como verifica o seu trabalho?

O Ministério altera **apenas o orçamento de funcionamento de cada edifício elegível**, usando o mesmo sistema de financiamento do controlo deslizante de orçamento do próprio jogo.

Nunca altera:

- O financiamento dos autocarros escolares.
- O financiamento das ambulâncias.
- As áreas de cobertura dos edifícios.
- A localização dos edifícios ou o número de equipamentos.

**Cada alteração de financiamento é verificada antes e depois de acontecer.**

Antes de alterar um orçamento, o Ministério volta a ler as definições atuais do edifício. Se, entretanto, o jogador tiver mexido no controlo deslizante depois de o cálculo ter sido feito, o Ministério salta essa alteração e recalcula mais tarde.

Depois de fazer uma alteração, verifica que:

1. O novo financiamento é indicado corretamente pelo jogo.
2. O custo mensal de funcionamento do edifício é coerente com o novo financiamento.
3. A capacidade disponível responde como esperado.
4. O financiamento dos autocarros escolares ou das ambulâncias, quando existe, se mantém inalterado.

**O que acontece se algo correr mal?**

Se uma alteração não puder ser verificada, o Ministério deixa de gerir esse edifício em particular durante o resto da sessão atual da cidade.

Se três edifícios diferentes falharem estas verificações, deixa por completo de fazer alterações de financiamento de educação e de saúde até a cidade ser reaberta.

Isto evita tentativas repetidas de modificar edifícios cujos sistemas de financiamento se comportam de forma inesperada.

**Ainda se pode alterar um orçamento à mão?**

Sim. Pode ajustar por si o financiamento de qualquer edifício. O Ministério deteta a alteração e tem-na em conta na avaliação seguinte, desde que esse edifício continue sob gestão automática.

Se quiser manter o controlo manual total destes orçamentos, desative **Gerir a educação** ou **Gerir a saúde** nas definições do Ministério.

<a id="education-healthcare-shortage"></a>
#### 3.7 O que acontece quando o financiamento não chega?

Alguns edifícios ficam tão sobrelotados que nem o financiamento máximo consegue dar capacidade suficiente.

Nessa situação, **o Ministério não pode resolver o problema. É preciso outro edifício.**

No entanto, o Ministério não trata de imediato todos os edifícios sobrelotados como uma falta de capacidade.

Verifica primeiro se mais financiamento poderia resolver o problema.

| Situação | O que o Ministério faz |
|---|---|
| **O financiamento está abaixo do máximo** | Aumenta o financiamento quando necessário. Ainda não considera a falta de capacidade inevitável. |
| **O financiamento está no máximo, com capacidade suficiente** | Mantém o edifício financiado e não emite aviso de falta de capacidade. |
| **O financiamento está no máximo, mas a cobertura está abaixo de 85%** | Confirma que o edifício não consegue servir adequadamente a procura atual. |
| **O edifício está em greve mas normalmente conseguiria dar capacidade suficiente** | Trata a greve como um problema temporário e não como prova de que é preciso um edifício novo. |

**Cobertura máxima atingível**

```math
V_{\max} = \min\left(1,\frac{C_{\max}}{U}\right)
```

Onde:

- **Cₘₐₓ** é a capacidade disponível com o financiamento máximo.
- **U** é a procura atual do edifício.
- **Vₘₐₓ** é a cobertura mais alta que o edifício pode atingir.

Se o edifício estiver em greve e a capacidade indicada estiver temporariamente afetada, o Ministério estima qual seria a sua capacidade sem a greve.

**Confirma-se uma falta de capacidade quando a cobertura máxima atingível é inferior a 85%.**

Por exemplo, um hospital que consegue acolher 1.500 doentes com o financiamento máximo mas tem uma procura de 2.000 doentes só consegue cobrir 75% dessa procura.

Nenhum aumento de financiamento resolve isto. É preciso outro equipamento de saúde.

Mesmo assim, o Ministério espera antes de o avisar, para evitar comunicar problemas temporários que se podem resolver sozinhos.

<a id="education-healthcare-notices"></a>
#### 3.8 Quando e como avisa o Ministério?

O Ministério tenta tratar em silêncio os problemas de financiamento do dia a dia.

**Não recebe mensagens de cada vez que poupa dinheiro, aumenta o financiamento ou evita uma possível greve.**

Avisa quando um edifício precisa realmente da sua intervenção.

**Quando é enviado um aviso?**

Uma falta de capacidade tem de permanecer confirmada em todas as avaliações durante **30 dias simulados** antes de o Ministério emitir um aviso.

Se o edifício recuperar durante esse período, a contagem recomeça do zero caso a falta de capacidade volte.

Os edifícios atualmente em greve não entram nos avisos de falta de capacidade, porque o SimCity 4 já dá os seus próprios avisos de greve.

**O que mostra o aviso?**

O aviso aparece na lista normal de notícias dos conselheiros do jogo.

Indica que tipo de edifício precisa de mais capacidade e traz ligações clicáveis que levam diretamente aos locais afetados.

Por exemplo, o aviso de uma escola sobrelotada pode dizer:

> **Carteiras a mais, espaço a menos!**
>
> Presidente *(o seu nome)*, esta escola está a rebentar pelas costuras! O financiamento já está no máximo, mas os alunos continuam a chegar. Talvez esteja na altura de construir outra escola, antes que tenham aulas no recreio! Para ver o caso de perto, visite **a escola** onde as carteiras pediram reforços.

No jogo, **a escola** é uma ligação clicável que leva a câmara ao edifício afetado.

**Como se tratam várias faltas de capacidade?**

- O Ministério agrupa as faltas de capacidade por tipo de edifício, como escolas primárias, hospitais ou bibliotecas.
- Cada aviso pode listar até **10 edifícios**, mostrando primeiro os mais sobrelotados.
- Os outros edifícios afetados continuam a ser vigiados e podem aparecer quando as faltas anteriores se resolverem.
- Se os edifícios afetados mudarem, o Ministério atualiza o aviso existente sempre que possível, em vez de criar mensagens novas desnecessárias.
- Se o número de edifícios listados mudar, o aviso antigo é substituído. Alterações repetidas podem desencadear uma **pausa de 30 dias** entre substituições, para evitar encher a lista de conselheiros.
- Quando todos os edifícios abrangidos por um aviso de falta de capacidade recuperam, o aviso é removido.

**O mesmo aviso aparece de cada vez que se reabre a cidade?**

Não. O Ministério recorda que faltas de capacidade já comunicou para cada cidade identificável.

Se fechar e reabrir uma cidade com o mesmo problema por resolver, normalmente não repete o aviso. Dispensar um aviso também impede que a mesma falta de capacidade, sem alterações, volte a ser anunciada.

Esta informação é guardada separadamente das gravações das suas cidades.

**Onde aparecem os avisos?**

Os avisos de falta de capacidade de educação e de saúde são entregues pelos conselheiros do próprio jogo. Aparecem na lista de notícias dos conselheiros, sem abrir uma janela emergente intrusiva.

Para o sistema de avisos partilhado com os outros serviços, consulte os [Avisos dos conselheiros](safety.md#notices).

<a id="education-healthcare-safety"></a>
### 4. Condições de segurança

O Ministério segue várias salvaguardas para não reduzir os serviços por engano.

**Antes de alterar o financiamento**

- Só gere edifícios cujo sistema de financiamento padrão possa ser identificado e verificado.
- Nunca assume que a falta de dados de frequência significa que um edifício está vazio.
- Volta a verificar o financiamento e a capacidade do edifício antes de fazer uma alteração.
- Deixa em paz os edifícios com leituras pouco fiáveis ou incoerentes.

**Ao reduzir o financiamento**

- As reduções só se fazem na revisão mensal.
- O financiamento nunca é reduzido enquanto um edifício estiver em greve.
- O financiamento nunca é reduzido quando a cobertura indicada pelo jogo está abaixo de 85%.
- Os edifícios sem dados de frequência utilizáveis nunca ficam subfinanciados.
- Os edifícios cuja capacidade não pode ser prevista de forma fiável ficam protegidos contra reduções automáticas.
- Na configuração predefinida, o financiamento nunca desce abaixo de 1%.

**Ao aumentar o financiamento**

- Um edifício em greve é subido até ao financiamento máximo configurado.
- Os edifícios com problemas urgentes de capacidade podem receber aumentos entre as revisões mensais.
- Se o financiamento máximo não resolver uma falta de capacidade genuína, o Ministério deixa o edifício no financiamento máximo e, se o problema persistir, avisa-o.

**Depois de fazer uma alteração**

- O Ministério verifica que o novo orçamento e a capacidade são coerentes.
- O financiamento dos autocarros escolares e das ambulâncias tem de se manter inalterado.
- Um edifício que falhe a verificação fica excluído de novas alterações automáticas durante a sessão.
- Três falhas de verificação diferentes interrompem as alterações de financiamento de educação e de saúde durante essa sessão.

**Quando a gestão automática está desativada**

Desligar o Ministério não repõe os orçamentos anteriores. Cada edifício mantém o financiamento atual até o alterar manualmente ou reativar a gestão.

#### As mensagens originais dos conselheiros do jogo

O SimCity 4 costuma mostrar avisos quando o financiamento da educação ou da saúde desce abaixo de certas percentagens.

Por exemplo, o jogo pode queixar-se de orçamentos de educação abaixo de 90%, mesmo quando uma escola tem capacidade mais do que suficiente com um financiamento muito inferior.

Estes avisos nem sempre são relevantes quando o Ministério gere os edifícios individualmente.

Enquanto o Ministério gere a educação, ou a saúde, o plugin ajusta por isso alguns conselhos originais do jogo para esse serviço:

- Os avisos de rotina baseados apenas em percentagens baixas de financiamento são suprimidos.
- Alguns avisos sobre excesso de financiamento ou níveis de pessoal também são suprimidos.
- Os avisos de pré-greve relevantes limitam-se às situações em que há mesmo uma greve.
- **Os alertas de greve verdadeiros mantêm-se ativos.**
- As mensagens normais de elogio e de estado do jogo são preservadas.

Isto segue o estado do Ministério em cada cidade, sem reiniciar o jogo. Se desativar a gestão da educação ou da saúde (ou todo o Ministério), ou se o Ministério não puder atuar sobre ela, os conselhos originais do jogo para esse serviço voltam de imediato.

Consulte a [Segurança geral](safety.md#safety) para mais informação.

#### Se o jogo mover um slider por conta própria

Quando o jogo altera o financiamento de uma escola ou de um hospital que o Ministério tinha definido, logo a seguir a um recálculo mensal, o Ministério reavalia esse edifício na vigilância seguinte, nos dois sentidos, e aplica o financiamento mínimo que ainda dá a capacidade exigida. Ver [Phantom Funding Slider Bug](phantom_slider.md).

<a id="education-healthcare-settings"></a>
### 5. Definições

A gestão da educação e da saúde pode ser configurada diretamente em **Opções de Jogo → Ministério da Optimização**, na página **Serviços**.

| Definição | Predefinição | Intervalo | O que faz |
|---|---|---|---|
| [Ministério ativo](options.md#opt-master) | Ligado | Ligado / Desligado | Ativa ou desativa todo o Ministério. |
| [Gerir a educação](options.md#opt-education) | Ligado | Ligado / Desligado | Gere automaticamente o financiamento das escolas, faculdades, universidades, bibliotecas e museus elegíveis. |
| [Margem de segurança (educação)](options.md#opt-educationreserve) | 5% | 0–50% | Define a capacidade de reserva mínima que o Ministério procura manter nos edifícios de educação. |
| [Gerir a saúde](options.md#opt-health) | Ligado | Ligado / Desligado | Gere automaticamente o financiamento das clínicas, hospitais e centros de investigação de doenças elegíveis. |
| [Margem de segurança (saúde)](options.md#opt-healthreserve) | 5% | 0–50% | Define a capacidade de reserva mínima que o Ministério procura manter nos edifícios de saúde. |

**Como funciona a margem de segurança?**

Uma margem de segurança de 5% significa que um edifício com 1.000 utilizadores deve ter capacidade para pelo menos 1.050.

No entanto, **5% é um mínimo, não um objetivo fixo**. O Ministério pode manter mais capacidade quando um edifício é pequeno ou teve grandes aumentos da procura.

Aumentar a margem dá uma reserva adicional contra o crescimento súbito, mas pode também aumentar os custos de funcionamento.

As alterações feitas na janela de definições do jogo aplicam-se imediatamente. Se editar manualmente os ficheiros de configuração, essas alterações são lidas no mês simulado seguinte.

As definições inválidas não são corrigidas em silêncio. Um valor fora do intervalo ou uma definição desconhecida pode fazer com que a gestão da educação e da saúde fique desativada até a configuração ser corrigida.

#### Definições avançadas (apenas no ficheiro INI)

Existem duas definições adicionais no ficheiro de configuração. Não aparecem na janela de opções do jogo.

| Definição | Predefinição | Intervalo permitido | O que faz |
|---|---|---|---|
| `MinFundingPercent` | 1% | 0–100% | Define a percentagem de financiamento mais baixa que o Ministério pode escolher. |
| `MaxFundingPercent` | 120% | 1–120% | Define a percentagem de financiamento mais alta que o Ministério pode escolher, inclusive durante as greves. |

**Financiamento mínimo:** O valor predefinido de 1% impede que o Ministério retire por completo o orçamento de funcionamento de um edifício, mesmo quando este não tem utilizadores registados.

**Financiamento máximo:** O valor predefinido de 120% permite que os edifícios funcionem com o nível de financiamento mais alto disponível no jogo original. Baixar este limite restringe a despesa, mas reduz também a capacidade que o Ministério pode dar em situações de sobrelotação ou de greve.

O mínimo não pode exceder o máximo.

Para a maioria dos jogadores, **recomendam-se os valores predefinidos**.

<a id="education-healthcare-examples"></a>
### 6. Exemplos práticos

Os exemplos seguintes mostram como o Ministério ajusta o financiamento em diferentes situações.

As capacidades e os custos de funcionamento dos edifícios são ilustrativos. Os seus valores podem diferir consoante o edifício e os mods que usar.

O jogo também usa arredondamentos internos, pelo que as capacidades apresentadas podem por vezes diferir numa unidade de um simples cálculo percentual.

#### Exemplo 1: Uma escola com mais capacidade do que a necessária

Uma escola primária tem:

- **Capacidade normal máxima:** 1.200 alunos.
- **Frequência atual:** 610 alunos.
- **Custo de funcionamento com 100% de financiamento:** §187/mês.
- **Margem de segurança:** 5%.

O Ministério calcula a capacidade necessária:

```math
R = \lceil 610 \times 1.05 \rceil = 641
```

Compara depois os níveis de financiamento:

| Financiamento | Capacidade disponível | Resultado |
|---|---:|---|
| 53% | 635 | Insuficiente |
| 54% | 647 | Suficiente |
| 100% | 1.200 | Mais do que o necessário |

**O Ministério escolhe 54% de financiamento.**

O custo mensal de funcionamento desce de §187 para cerca de **§101**, poupando **§86 por mês**.

O financiamento dos autocarros escolares mantém-se inalterado.

#### Exemplo 2: Uma clínica pequena precisa de uma margem de segurança maior

Uma clínica tem uma capacidade normal de 1.400 doentes mas está a atender apenas 60.

Embora a margem de segurança configurada seja de 5%, os edifícios pequenos recebem proteção adicional contra mudanças súbitas na frequência.

Como esta clínica tem menos de 100 doentes, o Ministério mantém **50% de capacidade extra**:

```math
R = \lceil 60 \times 1.5 \rceil = 90
```

Com 6% de financiamento, a clínica só consegue atender 84 doentes. Com 7%, consegue atender aproximadamente 98.

**O Ministério escolhe 7% de financiamento.**

É ligeiramente mais do que a margem habitual de 5% exigiria, mas dá uma proteção útil contra aumentos súbitos por um custo adicional muito pequeno.

#### Exemplo 3: Uma escola secundária com mudanças sazonais na frequência

Uma escola secundária tem:

- **Capacidade normal:** 5.000 alunos.
- **Frequência atual:** 900 alunos.
- **Menos de 13 meses de histórico de frequência na sessão atual.**

As escolas secundárias podem receber grandes quantidades de alunos quando estes passam da escola primária.

Até terem sido reunidas observações mensais suficientes, o Ministério assume um possível **aumento de 50% na frequência**.

Isto dá uma capacidade necessária de:

```math
R = \lceil 900 \times 0.85 \times 1.5 \rceil = 1148
```

**O Ministério escolhe 23% de financiamento**, o que dá aproximadamente 1.149 lugares.

Passadas 13 observações mensais, suponha-se que o maior aumento realmente registado foi de apenas 8%.

O Ministério pode então confiar no padrão de procura observado e voltar à margem de segurança normal de 5%.

A capacidade necessária passa a ser de 945 alunos.

**O financiamento pode descer para 19%**, o que dá aproximadamente 949 lugares.

O Ministério evita despesa desnecessária sem deixar de ter em conta a procura sazonal.

#### Exemplo 4: Uma biblioteca com capacidade por utilizar

Uma biblioteca tem capacidade para uma coleção de 40.000 livros, mas só 9.000 estão atualmente requisitados.

Com uma margem de 5%, o Ministério precisa de capacidade para:

```math
R = \lceil 9000 \times 1.05 \rceil = 9450
```

Com 23% de financiamento, a capacidade é de aproximadamente 9.199 livros, o que é insuficiente.

Com 24%, a capacidade chega aos 9.599 aproximadamente.

**O Ministério escolhe 24% de financiamento.**

A biblioteca continua a servir os seus leitores sem pagar toda a capacidade de coleção que não utiliza.

#### Exemplo 5: Um museu com poucos visitantes

Um museu consegue acolher 2.800 visitantes mas recebe atualmente apenas 150.

Como serve menos de 200 visitantes, o Ministério mantém uma **margem de segurança de 25%**.

```math
R = \lceil 150 \times 1.25 \rceil = 188
```

Com 7% de financiamento, o museu tem capacidade para aproximadamente 196 visitantes.

**O Ministério escolhe 7% de financiamento.**

O museu continua operacional, com capacidade de reserva suficiente para as oscilações na frequência.

#### Exemplo 6: Um aumento súbito entre revisões mensais

Considere-se a escola primária do Exemplo 1.

Neste momento tem:

- **Financiamento:** 54%.
- **Capacidade:** 647 alunos.
- **Frequência:** 610 alunos.

A meio do mês, a frequência sobe de repente para **700 alunos**.

A escola fica agora seriamente sobrelotada.

O Ministério procura problemas urgentes a cada dois dias simulados. Como a capacidade disponível está mais de 3% abaixo da procura, não espera pela revisão mensal seguinte.

A nova capacidade necessária é:

```math
R = \lceil 700 \times 1.05 \rceil = 735
```

**O financiamento é aumentado de 54% para 62%**, o que dá aproximadamente 744 lugares.

Se a frequência descer depois, o Ministério espera pela revisão mensal seguinte antes de reduzir o orçamento.

Isto é intencional: **os aumentos de financiamento protegem os serviços de imediato, enquanto as reduções são tratadas com mais cautela**.

#### Exemplo 7: O financiamento máximo nem sempre significa falta de capacidade

Uma clínica tem uma capacidade normal de 1.400 doentes.

Com 120% de financiamento, consegue acolher aproximadamente **1.539 doentes**.

Considerem-se duas situações:

| Doentes atuais | Capacidade máxima | Cobertura | Decisão do Ministério |
|---|---:|---:|---|
| 1.500 | 1.539 | 100% | Manter o financiamento máximo; sem aviso de falta de capacidade |
| 1.900 | 1.539 | 81% | Confirmar uma falta de capacidade |

Com 1.500 doentes, a clínica tem capacidade suficiente para servir toda a gente, embora não consiga manter a margem de segurança preferida.

**Ainda não é preciso nenhum edifício novo.**

Com 1.900 doentes, nem o financiamento máximo consegue dar uma cobertura adequada.

O Ministério mantém o financiamento a 120%. Se a falta de capacidade se mantiver confirmada durante 30 dias simulados e a clínica não estiver em greve, um aviso de conselheiro indica que é preciso outra clínica.

Há também um compromisso financeiro: 120% de financiamento custa **mais 20%**, mas dá apenas **mais 10% de capacidade** do que 100%.

#### Exemplo 8: Um hospital que precisa de capacidade adicional

Um hospital tem:

- **Capacidade normal:** 3.000 doentes.
- **Procura atual:** 4.376 doentes.
- **Financiamento máximo:** 120%.

Mesmo com o financiamento máximo, o hospital só consegue acolher cerca de **3.299 doentes**.

A sua cobertura máxima é:

```math
V_{\max} = \frac{3299}{4376} \approx 0.754
```

Isto significa que o hospital só consegue servir cerca de **75,4% da sua procura**.

Está abaixo do limiar de falta de capacidade de 85% do Ministério e também do limiar de risco de greve de 80% do jogo.

**Aumentar o financiamento não resolve o problema.**

O Ministério mantém o financiamento a 120%, mas o hospital continua em risco de greve.

Se a falta de capacidade continuar durante 30 dias simulados, o Ministério pode emitir um aviso a pedir-lhe que construa outro hospital. Uma greve em curso é tratada, em vez disso, pelos alertas de greve do próprio jogo.

<a id="education-healthcare-limits"></a>
### 7. Limitações conhecidas

O Ministério automatiza as decisões de financiamento, mas não consegue controlar todos os aspetos do SimCity 4.

**Capacidade e cobertura dos edifícios**

- **Não pode construir novos equipamentos.** Se uma escola ou um hospital estiver sobrelotado mesmo com o financiamento máximo, tem de construir outro edifício.
- **Não pode alterar as áreas de cobertura.** O financiamento dos autocarros escolares e das ambulâncias, e portanto as decisões de cobertura associadas, continua sob o seu controlo.
- **Usa a frequência indicada por cada edifício.** O jogo pode por vezes calcular o risco de greve com a procura de toda a área de cobertura, que pode ser superior ao número de utilizadores mostrado na janela de consulta do edifício. O Ministério usa a informação de cobertura do jogo para evitar cortes pouco seguros, mas pode não detetar de imediato todos os aumentos ocultos da procura.

**Monitorização e prazos**

- **O histórico de frequência é mantido durante a sessão atual da cidade.** Quando fecha a cidade, o Ministério esquece o histórico mensal de frequência e os ajustes de capacidade que aprendeu. As escolas secundárias e as faculdades voltam, portanto, a receber a proteção sazonal inicial depois de a cidade ser reaberta.
- **A informação de cobertura do jogo pode atrasar-se.** Alguns valores de cobertura só são atualizados cerca de uma vez por mês simulado. O Ministério usa a frequência e a capacidade atuais para planear o financiamento, verificando também a informação de cobertura do jogo antes de permitir reduções.
- **As verificações urgentes não são contínuas.** O Ministério procura normalmente problemas urgentes a cada dois dias simulados, ou diariamente durante as greves. Um aumento súbito da procura pode ainda provocar uma greve antes da verificação seguinte.
- **As poupanças são revistas mensalmente.** Depois de um aumento temporário da frequência, o financiamento mantém-se no nível mais alto até à revisão mensal seguinte.
- **Podem ocorrer pequenas alterações de financiamento entre meses.** Se a frequência oscilar perto de um limiar de financiamento, o Ministério pode aumentar ou diminuir o orçamento num ponto percentual em revisões mensais sucessivas.

**Compatibilidade dos edifícios**

O Ministério gere os edifícios que usam mecanismos de financiamento reconhecíveis e consistentes.

Não gere automaticamente:

- As escolas privadas sem orçamento de funcionamento próprio que se possa ajustar.
- Os edifícios personalizados que usam janelas de consulta ou sistemas de financiamento não suportados.
- Os edifícios cujas leituras de financiamento e de orçamento não coincidem.
- Os edifícios cuja capacidade não pode ser prevista de forma fiável a partir do financiamento.

Os edifícios que não podem ser geridos em segurança são deixados em paz.

**Avisos dos conselheiros**

- Os avisos de falta de capacidade abrangem oito tipos de edifícios de educação e de saúde.
- Cada aviso pode listar até **10 edifícios afetados**.
- Os edifícios atualmente em greve são tratados pelos alertas de greve originais do jogo e não pelos avisos de falta de capacidade do Ministério.
- O Ministério recorda os avisos de cada cidade identificável. Se uma cidade não puder ser identificada de forma fiável, o mesmo aviso pode voltar a aparecer depois de a reabrir.

**Conselhos originais do jogo**

O plugin inclui um ficheiro de dados que suprime certos avisos de financiamento enganadores do jogo original.

Este ajuste é independente dos interruptores de gestão da educação e da saúde. Enquanto o ficheiro estiver instalado, esses avisos originais permanecem suprimidos mesmo que desative a gestão automática do Ministério.

Consulte as [Condições de segurança](#education-healthcare-safety) para mais pormenores.

[Voltar ao índice](index.md#contents) | [Voltar ao README](../../README.pt.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
