[English](../en/safety.md) | **Português** | [Français](../fr/safety.md) | [Español](../es/safety.md)

[Voltar ao índice](index.md#contents) | [Voltar ao README](../../README.pt.md)

<a id="safety"></a>
## G. Segurança, automatização e tomada de decisões

O Ministério pode alterar orçamentos, gerir contratos regionais, limpar edifícios abandonados e corrigir certos problemas dos aterros. Como estas ações afetam diretamente a sua cidade, **proteger os serviços essenciais e respeitar as suas decisões tem de vir sempre antes de poupar dinheiro**.

Este capítulo explica as regras partilhadas pelas funcionalidades automáticas do Ministério:

- Como decide que objetivos têm prioridade.
- Quando lê a cidade e volta a calcular os seus planos.
- Como verifica as alterações antes de continuar.
- O que acontece quando a informação do jogo está incompleta ou é incoerente.
- Como as falhas técnicas são contidas.
- Que decisões continuam a estar nas suas mãos.
- Como funcionam as suas ações automáticas e as mensagens dos conselheiros.

<a id="safety-vanilla"></a>
### 1. Como funciona o SimCity 4 sem o Ministério

No jogo original, a maior parte das decisões de financiamento dos serviços e de comércio regional é manual.

#### Orçamentos dos serviços

As escolas, os hospitais e as centrais elétricas têm controlos de financiamento individuais.

A água e o saneamento têm, cada um, um único orçamento de departamento.

O SimCity 4 aceita alterações de financiamento através destes controlos, mas **não as optimiza automaticamente à medida que a procura muda**.

#### Estatísticas do jogo

A simulação atualiza a procura, a produção, o consumo, a capacidade e os contratos regionais de acordo com o seu próprio calendário interno.

Estes valores não mudam necessariamente logo a seguir a um ajuste de orçamento.

Por exemplo:

- A capacidade de produção de água pode ser atualizada só no mês simulado seguinte.
- A eficiência do saneamento e as capacidades de tratamento podem demorar vários dias simulados a refletir um novo financiamento.
- Algumas estatísticas podem estar inicialmente a zero depois de abrir uma cidade, porque a simulação ainda não as preencheu.

**O Ministério tem de distinguir entre um zero genuíno e informação que ainda não está pronta.**

#### Mensagens dos conselheiros

O SimCity 4 tem um sistema de conselheiros que mostra notícias e avisos.

Cada mensagem tem um conselheiro, um tom e uma prioridade.

Consoante a dificuldade do jogo ou as definições dos conselheiros, as mensagens de prioridade alta podem aparecer como pop-ups.

Os limiares de prioridade do jogo para os pop-ups são **60, 80 ou 100**, consoante o nível de dificuldade escolhido.

As mensagens abaixo do limiar aparecem normalmente na lista de notícias dos conselheiros, sem abrir um pop-up.

Cada mensagem fica disponível durante a duração configurada.

#### Contratos regionais

O jogo original também propõe, renova e cancela contratos regionais, com as suas próprias mensagens de conselheiros.

O Ministério pode automatizar essas operações quando está ativado.

<a id="safety-problem"></a>
### 2. O problema da gestão

A gestão automática pode poupar dinheiro e reduzir o trabalho repetitivo, mas também pode criar problemas se as decisões forem tomadas sem salvaguardas suficientes.

Por exemplo, um sistema automatizado poderia:

- Reduzir o financiamento abaixo do nível necessário para manter um serviço.
- Vender eletricidade ou água de que a cidade realmente precisa.
- Tomar decisões com estatísticas incompletas logo depois de carregar.
- Aplicar outra alteração antes de a anterior ter produzido efeito.
- Repetir uma operação de contrato ou de financiamento que o jogo já rejeitou.
- Demolir edifícios que deveriam estar protegidos.
- Inundar o jogador de notificações sobre ações de rotina.

**O Ministério foi concebido para evitar primeiro estes problemas e só depois optimizar as despesas.**

<a id="safety-principles"></a>
### 3. As regras gerais do Ministério

Quando vários objetivos entram em conflito, o Ministério segue esta ordem de prioridades:

1. **Manter os serviços essenciais.** Evitar deixar a cidade sem eletricidade, água, saneamento, educação ou saúde.
2. **Proteger as reservas configuradas.** Manter capacidade suficiente acima das necessidades previstas da cidade.
3. **Respeitar os limites de segurança da reserva.** Não contornar as regras destinadas a proteger a continuidade do serviço.
4. **Respeitar as restrições do SimCity 4.** Usar apenas os níveis de financiamento, as capacidades e as quantidades de contrato permitidos pelo jogo.
5. **Minimizar as despesas mensais.** Escolher a organização de funcionamento segura mais barata.
6. **Vender o excedente genuíno quando for rentável.** A receita adicional tem de justificar o custo de a fornecer.
7. **Evitar comprar e vender em simultâneo por predefinição.** O comércio misto exige a opção respetiva.
8. **Nunca inventar informação em falta.** Os valores ilegíveis ou incoerentes não são tratados como medições válidas.
9. **Evitar atrasos desnecessários.** Uma decisão segura e pronta não recebe um tempo de espera artificial.
10. **Verificar cada alteração.** Ler o novo estado, confirmar o resultado e voltar a calcular antes de continuar.

Estes princípios aplicam-se aos gestores individuais do Ministério, com as salvaguardas específicas de cada recurso descritas nos capítulos anteriores.

<a id="safety-service-first"></a>
#### Os serviços essenciais vêm sempre primeiro

O Ministério coordena o financiamento e os contratos regionais para evitar interromper o abastecimento durante uma transição.

#### As alterações são aplicadas por uma ordem segura

Para a eletricidade, a água e o lixo, o Ministério usa seis fases de execução:

| Fase | Ação |
|---|---|
| **1** | Aumentar a capacidade local |
| **2** | Reduzir ou cancelar vendas |
| **3** | Criar ou aumentar compras |
| **4** | Criar ou aumentar vendas |
| **5** | Reduzir a capacidade local |
| **6** | Reduzir ou cancelar compras |

**Cada recurso executa, durante uma revisão, apenas a sua fase pendente mais antiga.**

Por exemplo, o Ministério não reduz o financiamento de uma central elétrica antes de estar ativa a compra de eletricidade destinada a substituir essa produção.

Se um aumento de capacidade ainda não produziu efeito, as fases seguintes desse recurso esperam.

Há uma distinção importante no saneamento: uma redução de financiamento pode ser adiada, porque as escritas do saneamento são deliberadamente graduais.

**Uma redução do saneamento por concluir não impede necessariamente uma redução posterior das compras de tratamento de lixo**, porque manter uma capacidade local de eliminação mais elevada não pode prejudicar a continuidade do serviço.

#### O financiamento não é reduzido durante uma falta genuína

O Ministério distingue entre abastecimento insuficiente e problemas na rede de distribuição da cidade.

No caso da água, por exemplo, recusa uma redução quando há edifícios sem água **e a capacidade de produção está abaixo da procura**.

Se houver água suficiente, mas alguns edifícios estiverem desligados da rede de canalizações, um aumento de financiamento não resolveria o problema. Um problema de ligação desse tipo não impede automaticamente uma redução que seja segura noutros aspetos.

Consulte as [condições de segurança da água](water.md#water-no-cut).

#### As vendas param durante défices confirmados de eletricidade ou de água

Se os contadores do jogo confirmarem que a cidade não tem eletricidade ou água suficientes, **o Ministério não mantém as vendas desse recurso**.

A procura local tem prioridade sobre a receita das exportações.

#### A educação e a saúde aumentam o financiamento sem demora

A educação e a saúde usam uma estratégia de calendário diferente.

O Ministério pode aumentar o financiamento:

- Durante o primeiro dia simulado depois de abrir a cidade.
- Quando é detetado um risco para a capacidade do serviço.
- Durante verificações adicionais enquanto houver uma greve ou um risco persistente.

**As reduções de financiamento esperam pela avaliação mensal.**

Isto permite ao Ministério reagir depressa a escolas ou hospitais sobrelotados, evitando reduções desnecessárias entre as revisões mensais.

Consulte [Educação e saúde](education_healthcare.md#education-healthcare-timing).

<a id="safety-reserves"></a>
#### Reservas de segurança: capacidade mínima, não máxima

Cada serviço gerido tem uma reserva configurável.

**Uma reserva é um requisito mínimo, nunca um máximo.**

O Ministério pode manter mais capacidade quando for necessário, por causa das definições de financiamento disponíveis, das flutuações do serviço ou de vendas regionais rentáveis.

Não pode escolher deliberadamente um plano que fique abaixo do requisito protegido aplicável.

| Serviço | Reserva predefinida | Intervalo permitido | Como funciona |
|---|---:|---:|---|
| **Educação e saúde** | 5% | 0–50% | Acrescenta capacidade livre acima da utilização, com proteção adicional para os equipamentos pequenos ou com utilização oscilante. |
| **Eletricidade** | 20% | 10–50% | Protege a parte da procura fornecida pelas centrais elétricas locais, respeitando também a exigência de eletricidade do jogo. |
| **Água** | 10% | 10–50% | Protege a parte da procura fornecida pelas instalações de água locais. |
| **Saneamento** | 2% | 2–20% | Acrescenta uma margem ao lixo produzido depois da reciclagem, juntamente com o lixo por recolher que já exista. |

Consulte [Educação e saúde](education_healthcare.md#education-healthcare-required), [Eletricidade](power.md#power), [Água](water.md#water-required-capacity) e [Lixo e aterros](garbage.md#garbage-landfills-need).

**E se o valor de uma reserva for inválido?**

Se editar manualmente um ficheiro de configuração e indicar uma reserva fora do intervalo permitido, o Ministério não corrige o valor em silêncio.

**A configuração inválida é rejeitada, e o componente de gestão afetado fica desativado até ser corrigida.**

Assim evitam-se margens de segurança inesperadas que não chegou a configurar.

<a id="safety-economy"></a>
#### Como minimiza o Ministério as despesas?

Depois de cumpridos os requisitos do serviço e de segurança, o Ministério procura a organização adequada mais barata.

Os diferentes serviços usam métodos diferentes, porque o SimCity 4 trata o seu financiamento de maneira diferente.

| Serviço | Método de optimização |
|---|---|
| **Educação e saúde** | Escolher a menor percentagem de financiamento, em números inteiros, que dê a capacidade necessária. |
| **Eletricidade** | Encontrar a combinação mais barata de níveis de financiamento individuais das centrais elétricas. |
| **Água** | Escolher a menor percentagem segura de financiamento do departamento. |
| **Saneamento** | Simular a eliminação do lixo nos dias que faltam até ao fim do mês e encontrar o financiamento mais barato capaz de tratar os resíduos. |
| **Contratos regionais** | Comparar a receita e as despesas do contrato com o custo real da produção ou da eliminação locais. |

#### Com que rapidez são aplicadas as alterações de financiamento?

**Eletricidade e água**

O Ministério aplica diretamente a percentagem de financiamento escolhida, sem um limite artificial por etapas.

As verificações de segurança aplicáveis e a coordenação com os contratos regionais continuam a aplicar-se.

**Saneamento**

As reduções de financiamento são deliberadamente graduais:

- No máximo **25 pontos percentuais** por redução.
- Pelo menos **7 dias simulados** entre reduções do financiamento do saneamento.
- Pelo menos **3 dias simulados** desde a escrita anterior do financiamento do saneamento antes de um aumento.

Estes intervalos dão tempo para que a eficiência do saneamento e a capacidade de tratamento do jogo acompanhem as alterações de financiamento.

**Contratos regionais**

O Ministério cria, altera ou cancela contratos apenas quando o plano resultante se justifica economicamente e cumpre os requisitos de serviço relevantes.

Consulte [Acordos com os vizinhos](deals.md#neighbor-deals-objective).

<a id="safety-physical"></a>
#### Respeitar os limites físicos do jogo

O Ministério não contorna as restrições normais de funcionamento do SimCity 4.

Usa:

- Percentagens de financiamento aceites pelo jogo.
- Escolhas de financiamento em números inteiros, quando exigido.
- O intervalo configurado de educação e saúde, normalmente **1–120%**.
- O mínimo de proteção das canalizações de água, quando essa opção está ativada.
- Os limites reais de armazenamento e de admissão dos aterros.
- As quantidades de contrato que o jogo permite.
- Os preços devolvidos pelas funções de contrato e de demolição do próprio jogo.

**O Ministério nunca constrói edifícios nem infraestruturas automaticamente.**

Também não:

- Cria zonamento normal.
- Altera taxas de impostos.
- Altera as leis municipais.
- Ajusta orçamentos que nada têm que ver com isto.
- Modifica as cidades vizinhas.

A única exceção relacionada com o zonamento é a [reparação de quadrados fantasma](garbage.md#ghost-tiles), rigorosamente controlada, que usa o próprio sistema de zonamento do jogo para corrigir a pertença errada a um aterro, sem alterar deliberadamente o zonamento legítimo do aterro.

<a id="safety-reevaluation"></a>
#### Quando volta o Ministério a calcular as suas decisões?

O Ministério combina revisões regulares com verificações mais leves para alterações significativas.

**Não espera um atraso fixo arbitrário depois de abrir uma cidade.**

Cada serviço começa a avaliar assim que a informação de que precisa está disponível, atual e coerente.

A demolição é uma exceção: começa na primeira mudança de mês simulado depois de carregar.

#### Verificar se as estatísticas do jogo são fiáveis

Antes de tomar decisões sobre eletricidade ou água, o Ministério compara a procura comunicada com o consumo real.

Uma pequena diferença pode resultar dos arredondamentos do próprio jogo ou do momento da atualização.

No entanto, leituras claramente contraditórias impedem decisões automáticas de financiamento.

**Leituras de serviço incoerentes**

```math
R>D\times1.01+1
```

Em que:

- **D** é a procura comunicada pelo jogo.
- **R** é a quantidade do serviço comunicada como fornecida ou consumida.

Se o consumo exceder a procura em mais de **1% mais uma unidade**, as leituras são tratadas como incoerentes.

O Ministério espera por dados novos em vez de agir com base num estado pouco fiável.

Outros sinais de que a simulação ainda não está pronta:

- A procura ainda a zero logo depois de carregar.
- As contagens de edifícios com e sem água ainda por preencher.
- Falta de informação legível sobre o financiamento ou a produção das centrais elétricas, quando a gestão do financiamento das centrais a exige.

#### Verificações diárias dos serviços

Todos os dias simulados, o Ministério lê um pequeno número de contadores de eletricidade e de água.

Estas verificações são deliberadamente leves.

Não exigem examinar todos os edifícios nem reconstruir todo o mercado de contratos regionais.

**Uma leitura idêntica não é tratada como uma nova observação.**

O Ministério só desencadeia uma avaliação antecipada quando algo relevante mudou de facto.

#### Detetar uma falta genuína de abastecimento regional

Uma cidade pode comunicar consumo insuficiente por duas razões muito diferentes:

1. Não tem eletricidade ou água suficientes disponíveis.
2. Tem abastecimento suficiente, mas o serviço não chega a alguns edifícios.

O Ministério usa o teste seguinte para as distinguir.

**Défice de abastecimento regional**

```math
S<D
\qquad\text{e}\qquad
C+I-E<D
```

Em que:

- **D** é a procura da cidade.
- **S** é o serviço efetivamente fornecido.
- **C** é a capacidade de produção local.
- **I** é o abastecimento importado utilizável.
- **E** é o abastecimento exportado.

Todas as quantidades usam as unidades mensais do recurso em causa.

**Existe uma falta genuína quando o consumo está abaixo da procura e o abastecimento utilizável combinado também é insuficiente.**

Por exemplo, se uma cidade precisa de 20.000 unidades mas só tem 16.000 disponíveis depois das importações e das exportações, o Ministério pode reconsiderar de imediato o financiamento e os contratos.

No entanto, imagine que a cidade precisa de 20.000 unidades e tem 25.000 unidades de capacidade disponível, mas alguns edifícios continuam sem serviço.

O problema é provavelmente interno à cidade, como linhas elétricas ou canalizações de água desligadas.

**O Ministério regista o problema, mas não replaneia desnecessariamente a produção só porque os edifícios não têm ligação.**

#### Detetar alterações significativas antes da revisão regular seguinte

O Ministério também pode reagir quando a procura, a capacidade local, as importações ou as exportações mudam pelo menos **1%**.

**Limiar de alteração significativa**

```math
|x-x_0|\ge0.01\times\max(1,|x_0|)
```

Em que:

- **x** é a leitura mais recente.
- **x₀** é o valor registado na última avaliação.

**Exemplo**

Suponha que a procura de eletricidade era de 50.000 unidades na revisão anterior.

| Nova procura | Variação | Resultado |
|---|---:|---|
| 50.400 | 0,8% | Esperar pela revisão normal |
| **50.600** | **1,2%** | **Desencadear uma avaliação antecipada** |

Uma nova leitura significativa pode antecipar a avaliação seguinte, sem esperar pelo intervalo regular.

**Não há nenhum tempo de espera adicional para uma decisão que esteja pronta e seja segura de aplicar.**

#### Intervalos de avaliação regulares

| Serviço | Quando é avaliado |
|---|---|
| **Eletricidade** | De 10 em 10 dias simulados, mais revisões antecipadas em caso de faltas confirmadas ou alterações significativas. |
| **Água** | De 10 em 10 dias simulados desde a última revisão, em cada mudança de mês e mais cedo quando necessário. |
| **Saneamento** | Em cada mudança de mês e quando os valores de lixo monitorizados mudam significativamente. |
| **Educação e saúde** | Aumentos iniciais depois da abertura, verificações de risco de dois em dois dias (todos os dias durante as greves), novas verificações de sete em sete dias enquanto os riscos persistirem, e revisões mensais para aumentos e reduções. |
| **Demolição automática** | Em cada mudança de mês simulado. |
| **Aviso sobre o tamanho do aterro** | No máximo uma vez por mês simulado, durante uma avaliação normal do saneamento. |

**O que acontece enquanto uma alteração anterior ainda está a ser verificada?**

O Ministério não faz alterações sobrepostas ao mesmo recurso.

Por exemplo:

- Uma alteração de contrato regional tem de ser confirmada antes da operação de contrato seguinte.
- Um ajuste do financiamento do saneamento tem de atingir a eficiência e a capacidade de tratamento esperadas antes de ser feito outro ajuste.

Enquanto espera, o Ministério continua a verificar os valores relevantes do jogo.

**Não faz uma nova optimização completa só porque uma verificação está para ser feita.**

Isto evita que operações parcialmente concluídas interfiram umas com as outras, mantendo o sistema atento às alterações genuínas.
<a id="safety-verification"></a>
#### Como verifica o Ministério as suas alterações?

O Ministério não presume que uma operação teve êxito só porque enviou um comando ao SimCity 4.

**Cada alteração automática é verificada antes e depois de ser aplicada.**

A verificação depende do tipo de operação.

| Operação | O que o Ministério verifica |
|---|---|
| **Financiamento de escolas e hospitais** | O novo financiamento coincide com o valor pedido, e o orçamento dos autocarros escolares ou das ambulâncias permanece inalterado. |
| **Financiamento das centrais elétricas e da água** | O jogo aceita o valor de financiamento pedido. |
| **Financiamento do saneamento** | O novo financiamento é aceite e, depois, a eficiência e as capacidades de tratamento do jogo são atualizadas como esperado. |
| **Contratos regionais** | O estado do contrato, a quantidade, o preço e a contabilidade regional coincidem com a alteração pedida. |
| **Demolição automática** | O edifício desapareceu e o zonamento subjacente continua intacto. |

#### Antes de aplicar uma alteração

O Ministério volta a ler o edifício, o departamento ou o contrato.

O valor tem de continuar a coincidir com o que foi usado quando o plano foi calculado.

Por exemplo, suponha que o Ministério planeia reduzir o financiamento da água de 100% para 78%.

Antes de aplicar a alteração, verifica que o financiamento continua a ser 100%.

Se tiver sido o próprio jogador a mover o controlo deslizante para 90%, **o plano antigo é cancelado em vez de ser aplicado à sua nova definição**.

O Ministério pode então calcular um novo plano com a informação atual.

#### Depois de aplicar uma alteração

O Ministério lê o valor resultante.

Nas alterações de financiamento, a percentagem pedida tem de ser confirmada.

No caso das escolas e dos hospitais, verifica também que o orçamento separado dos autocarros ou das ambulâncias não mudou.

Os contratos regionais exigem uma confirmação adicional, porque os registos de contrato e os totais financeiros do jogo podem ser atualizados depois da operação.

O saneamento também exige uma verificação posterior, porque a sua eficiência e as suas capacidades de tratamento não respondem necessariamente de imediato a uma alteração de financiamento.

As demolições são verificadas numa análise posterior, quando o jogo já teve tempo de executar a ordem.

**Uma alteração não é considerada bem-sucedida só porque o comando foi enviado.**

O Ministério distingue entre:

- **Planeada:** uma alteração foi calculada, mas não aplicada.
- **Pendente:** uma alteração foi submetida e aguarda confirmação.
- **Verificada:** o jogo comunica o resultado esperado.

As poupanças só são contadas como confirmadas quando o resultado relevante foi verificado.

<a id="safety-missing-data"></a>
#### O que acontece quando o jogo não fornece informação suficiente?

O Ministério segue uma regra simples:

**Informação desconhecida não é o mesmo que zero.**

Por exemplo:

- Uma lista válida sem nenhuma central elétrica significa que não há centrais elétricas nessa lista.
- Uma tentativa falhada de ler a lista de centrais elétricas não prova que a cidade não tenha centrais.
- Um aterro com uma capacidade de admissão confirmada de zero não pode aceitar mais lixo.
- Um aterro cuja capacidade de admissão não pode ser lida tem uma capacidade de admissão desconhecida, não um zero confirmado.

O Ministério trata estas situações de forma diferente.

#### Leituras incompletas ou inválidas

Se parte da informação necessária não estiver disponível, o Ministério usa apenas valores que foram efetivamente lidos e verificados.

Quando um valor em falta ou inválido torna o cálculo pouco fiável, **o serviço afetado não faz nenhuma alteração de financiamento ou de contrato durante essa avaliação**.

Não substitui a informação em falta por um valor anterior guardado em cache nem inventa uma estimativa plausível.

Os outros serviços podem continuar a funcionar de forma independente.

#### Cidades vizinhas ou ofertas desconhecidas

Se não for possível ler as ofertas regionais ou os registos das cidades vizinhas, o Ministério não presume que não existe nenhum abastecimento alternativo.

Por exemplo, não pode concluir que uma falta de eletricidade é impossível de resolver se não conseguiu inspecionar as compras regionais disponíveis.

**Um mercado de contratos incompleto não pode sustentar um aviso definitivo de falta.**

#### Geometria do aterro desconhecida

O Ministério consegue calcular o financiamento do saneamento sem conhecer todos os pormenores da estrutura interna de células do aterro.

Enquanto essa estrutura não estiver calibrada, usa os cálculos conservadores descritos em [Lixo e aterros](garbage.md#garbage-landfills-algorithm).

Não inventa capacidade de armazenamento nem faz previsões para o mês seguinte que exijam informação indisponível.

#### Preços de demolição desconhecidos

Se o SimCity 4 não conseguir fornecer o preço da demolição de um lote, **o Ministério não o demole**.

A operação pode ser reconsiderada numa revisão mensal posterior.

<a id="safety-player-control"></a>
#### O controlo continua a ser seu

O Ministério automatiza as decisões de gestão repetitivas, mas pode escolher as funcionalidades que ele controla.

| O que pretende fazer | Como fazer |
|---|---|
| **Desativar todo o Ministério** | Desligue **Ministério ativo** ou revogue **Instituir o Ministério da Optimização** na janela de leis do jogo. |
| **Gerir um serviço manualmente** | Desative a opção de gestão desse serviço, como **Gerir a eletricidade** ou **Gerir a educação**. |
| **Gerir os contratos regionais manualmente** | Desative **Gerir acordos de eletricidade**, **Gerir acordos de água** ou **Gerir acordos de lixo** para o recurso em causa. |
| **Alterar um controlo deslizante de financiamento por si** | Mova-o normalmente. O Ministério usará o seu novo valor na avaliação seguinte, se a gestão continuar ativada. |
| **Criar ou cancelar um contrato por si** | Use a janela normal de Acordos com os vizinhos. O Ministério terá em conta os contratos resultantes na revisão seguinte. |
| **Parar a demolição automática** | Desative **Demolir edifícios destruídos** e **Demolir edifícios abandonados**, ou desative todo o Ministério. |
| **Remover o plugin** | Feche o SimCity 4 e remova os ficheiros do plugin. As gravações das suas cidades continuam utilizáveis sem ele. |

#### O que acontece quando desativa o Ministério?

**Desativar o Ministério não desfaz as alterações anteriores.**

Por exemplo, se ele tiver reduzido antes o financiamento da eletricidade para 72%, desligá-lo não repõe o financiamento anterior.

Do mesmo modo, os contratos regionais mantêm-se nos estados em que estão.

O Ministério limita-se a deixar de fazer novas alterações automáticas.

#### O que acontece quando move um controlo deslizante manualmente?

Pode sempre mover um controlo deslizante de financiamento.

No entanto, **um valor manual não fica bloqueado enquanto o respetivo gestor estiver ativado**.

O Ministério trata o seu novo valor como ponto de partida do cálculo seguinte.

Consoante o serviço:

- **Eletricidade:** o financiamento pode ser recalculado em poucos dias, o mais tardar na revisão regular de 10 dias.
- **Água:** o financiamento pode ser recalculado em 10 dias simulados.
- **Educação e saúde:** uma redução manual que crie um risco para a capacidade pode ser corrigida em poucos dias. Um aumento manual é revisto na passagem mensal.

Por exemplo, se aumentar o financiamento da água para 100% mas deixar **Gerir a água** ativado, o Ministério pode voltar a baixá-lo quando os seus cálculos mostrarem que basta menos financiamento.

**Para manter um valor de financiamento específico sob o seu controlo, desative o gestor desse serviço.**

**Se o próprio jogo mover um slider.** O SimCity 4 pode mover por conta própria os sliders de edifícios já colocados depois de o custo de manutenção de um edifício ter sido alterado (o Phantom Funding Slider Bug). Se a alteração aparecer logo a seguir a um recálculo mensal, num edifício cujo financiamento o Ministério tinha definido, o Ministério reavalia esse edifício na vigilância seguinte e repõe o financiamento no mínimo que dá a capacidade exigida, em vez de esperar pela revisão mensal. Uma alteração que faça à mão a meio do mês não é tratada assim. Ver [Phantom Funding Slider Bug](phantom_slider.md).

#### Há efeitos que não podem ser desligados individualmente?

Sim.

Na versão suportada do jogo, o plugin esconde o ícone de aviso original de *sem água* sobre os lotes agrícolas sempre que o plugin está carregado.

Isto não altera o serviço de água em si.

Consulte [Água](water.md#water-farm-icon).

O Ministério também silencia algumas das mensagens originais dos conselheiros do SimCity 4, mas apenas enquanto gere mesmo o serviço correspondente.

Quando não gere (ou não consegue gerir) o serviço, as mensagens originais são exatamente as que o jogo traz. Consulte os [Avisos dos conselheiros](#safety-notices-game).

<a id="safety-automation"></a>
#### Que funcionalidades atuam automaticamente e quais só dão conselhos?

Algumas funcionalidades do Ministério modificam diretamente a cidade.

Outras limitam-se a mostrar informação para que o jogador decida o que fazer.

| Funcionalidade | Ação automática | Aviso do conselheiro |
|---|---|---|
| **Educação e saúde** | Ajusta o financiamento | Avisa quando um financiamento adicional não consegue resolver um problema persistente de capacidade |
| **Eletricidade** | Ajusta o financiamento das centrais elétricas | Avisa de uma falta persistente que não pode ser resolvida automaticamente |
| **Água** | Ajusta o financiamento do departamento | Avisa de uma falta persistente que não pode ser resolvida automaticamente |
| **Saneamento** | Ajusta o financiamento do departamento | Avisa de uma falta persistente de capacidade de eliminação de lixo |
| **Contratos com os vizinhos** | Cria, altera e cancela contratos | Sem aviso de rotina específico para contratos |
| **Demolição automática** | Remove ruínas e edifícios abandonados elegíveis | Sem aviso |
| **Quadrados fantasma do aterro** | Repara automaticamente os quadrados fantasma elegíveis | Sem aviso |
| **Aviso sobre o tamanho do aterro** | **Nunca altera o tamanho do aterro** | Recomenda, opcionalmente, uma redução concreta |

**O aviso sobre o tamanho do aterro é a principal funcionalidade que recomenda uma alteração sem a executar.**

Pode dizer-lhe que reduzir um aterro poderia poupar dinheiro, mas a decisão de zonamento continua a ser sua.

<a id="safety-performance"></a>
#### Como evita o Ministério abrandar o jogo?

O Ministério foi concebido para fazer apenas o trabalho necessário em cada momento da simulação.

**Todo o trabalho de simulação corre no próprio fio de execução do jogo, entre fotogramas, usando os seus eventos diários e mensais normais.**

Não cria uma simulação separada em segundo plano.

A carga de trabalho está repartida assim:

| Operação | Frequência ou abordagem |
|---|---|
| **Monitorização diária dos serviços** | Lê um pequeno número de contadores do jogo. |
| **Optimização completa de um recurso** | Só corre quando o serviço está para ser avaliado ou quando ocorre uma alteração relevante. |
| **Análise dos lotes para demolição** | Lê a grelha completa de lotes uma vez por mês simulado. |
| **Verificação da demolição** | Faz uma análise leve de 10 em 10 dias simulados, lendo um valor de estado por lote e registando as alterações. |
| **Deteção do botão das opções** | Um temporizador de 250 milissegundos verifica apenas se a janela Opções de Jogo do jogo está disponível. |
| **Verificações dos avisos dos conselheiros** | Uma vez por dia simulado. |

**As verificações diárias de monitorização não repetem desnecessariamente os cálculos completos, que são dispendiosos.**

Por exemplo, uma leitura inalterada da procura de eletricidade não desencadeia outra optimização completa das centrais elétricas.

Do mesmo modo, o aviso sobre o tamanho do aterro corre dentro de uma avaliação normal do saneamento, e não através de uma pesquisa contínua separada.

<a id="notices"></a>
### Avisos dos conselheiros

O Ministério evita deliberadamente anunciar o seu trabalho de rotina.

Não mostra mensagens de cada vez que:

- Ajusta o financiamento.
- Compra ou vende um recurso.
- Altera ou cancela um contrato.
- Demole um edifício abandonado.
- Repara um quadrado fantasma de aterro.

**Também não há nenhuma mensagem de saudação ou de ativação.**

O Ministério só usa avisos dos conselheiros quando o jogador pode precisar de intervir, ou quando uma recomendação opcional sobre o aterro pode trazer uma poupança significativa.

<a id="safety-notices-utility"></a>
#### Avisos de falta de eletricidade, água e lixo

O Ministério pode avisá-lo quando uma falta persiste e **as soluções automáticas disponíveis estão esgotadas**.

Estes avisos vêm do **conselheiro dos Serviços Públicos**. O texto dirige-se ao presidente e à cidade pelo nome.

Um aviso de falta existe **apenas enquanto o Ministério gere esse serviço localmente** (eletricidade, água ou saneamento, com o Ministério ativo). Se desligar essa gestão, o aviso é retirado. Os contratos regionais são outra matéria: nunca causam um aviso, e um aviso nunca cancela um contrato.

| Recurso | Título do aviso |
|---|---|
| **Eletricidade** | **Apagão! Quem comprou as velas?** |
| **Água** | **Até as torneiras têm sede!** |
| **Lixo: falta grave** | **Lixo até ao pescoço!** |
| **Lixo: pilha antiga que não diminui** | **Este lixo já paga renda!** |

Os dois primeiros avisos de um episódio são mensagens urgentes que podem abrir como pop-up; os seguintes são entradas discretas da lista de notícias. O aviso da pilha antiga é informativo: neutro, sempre discreto, nunca um pop-up. Nenhum inclui ligação para a câmara.

#### Quando considera o Ministério que uma falta está comprovada?

Não se gera um aviso só porque a procura excede temporariamente a oferta. O Ministério tem de estabelecer, com leituras atuais, que **a falta não se resolve com o que lhe é permitido fazer**.

Na eletricidade e na água, verifica a capacidade utilizável das instalações locais, as compras regionais disponíveis, as vendas que ocupam capacidade, a procura real da cidade e o serviço entregue, e que nenhuma alteração sua aguarda confirmação. **Um edifício sem água apenas por estar desligado da rede de canalização não prova uma falta regional.**

No lixo, compara o seu modelo de tratamento com o atraso de lixo por recolher do próprio jogo, lido mês a mês. Um atraso até **1 % da produção mensal** conta como residual.

| Situação | O que o Ministério vê | Aviso |
|---|---|---|
| **Grave** | o atraso cresceu mais do que essa margem em cada um dos dois últimos meses | aviso urgente |
| **Pilha antiga** | há um atraso acima da margem, mas já não cresce: o lixo novo é praticamente todo tratado, a pilha antiga fica | aviso neutro e discreto |
| **A diminuir ou recuperado** | o atraso diminui, ou desapareceu ou é residual | nenhum aviso novo; a recuperação retira o aviso |
| **Incerto** | os dados não chegam | nada muda |

Os dois avisos de lixo nunca existem ao mesmo tempo. Quando os dados deixam de mostrar uma falta mas ainda não confirmam que acabou, o Ministério nem avisa nem retira um aviso já mostrado, e os dias de espera ficam parados.

#### Quando aparecem os avisos e os lembretes?

| Recurso | Primeiro aviso (dias com a falta comprovada) | Lembrete de |
|---|---|---|
| **Eletricidade** | 15 dias simulados | 120 em 120 dias simulados |
| **Água** | 30 dias simulados | 150 em 150 dias simulados |
| **Lixo** | 60 dias simulados | 240 em 240 dias simulados |

Uma falta resolvida antes desses dias não produz aviso. Só contam os dias em que a falta está comprovada. Os dois primeiros avisos de um episódio podem abrir como pop-up; a partir do terceiro são discretos. Não há limite de lembretes enquanto a falta durar.

O Ministério não consegue ver se dispensou um aviso, por isso nunca o presume: os lembretes seguem simplesmente estes intervalos fixos.

#### Quando termina um episódio de falta?

Na eletricidade e na água, quando leituras novas confirmam, em dois dias seguidos, que o serviço entregue é pelo menos igual à procura. No lixo, quando o modelo de tratamento volta a ser viável e o atraso desapareceu ou é residual.

Nessa altura o aviso é retirado do conselheiro e os lembretes param. **Uma falta posterior é um novo episódio**, com os seus próprios dias de espera. Desligar a gestão do serviço também termina o episódio e retira o aviso; ao ligá-la de novo, a falta tem de ser comprovada outra vez.

#### O que acontece se fechar e reabrir a cidade?

**Nada se guarda entre sessões** para estes avisos. Quando uma cidade abre, o Ministério retira qualquer aviso de falta deixado por uma sessão anterior. Se a falta continuar, tem de ser comprovada de novo e o primeiro aviso chega após os dias de espera normais (15, 30 ou 60). Os avisos de educação e saúde, abaixo, mantêm a sua memória.

#### Que definições são necessárias para os avisos de falta?

Apenas o Ministério ativo e a **gestão local desse serviço** (eletricidade, água ou saneamento). As definições dos contratos regionais não contam.

<a id="safety-notices-civic"></a>
#### Avisos de capacidade de educação e de saúde

O Ministério também pode avisá-lo quando as escolas ou os equipamentos de saúde continuam seriamente sobrelotados apesar do financiamento máximo disponível.

Estes avisos são emitidos pelo **conselheiro de Saúde e Educação**.

Aparecem como **entradas vermelhas na lista de notícias**, sem abrir um pop-up.

Existe um aviso por tipo de equipamento:

- Escolas primárias.
- Escolas secundárias.
- Faculdades e universidades.
- Bibliotecas.
- Museus.
- Clínicas.
- Hospitais.
- Centros de investigação de doenças.

#### Quando é incluído um edifício?

Um edifício tem de cumprir todas as condições seguintes:

1. O seu financiamento máximo possível continua a não dar capacidade suficiente.
2. A sua capacidade máxima cobre **menos de 85% da sua utilização**.
3. **Não está atualmente em greve**.
4. O problema continuou durante **30 dias simulados sem interrupção**.

**Uma cobertura abaixo de 85% significa que, mesmo com o financiamento máximo, o equipamento não consegue servir 85% da sua utilização atual.**

Por exemplo, se um hospital precisa de capacidade para 1.000 doentes mas só consegue servir 800 com o financiamento máximo, a sua cobertura é de 80%.

Se essa situação persistir durante 30 dias simulados sem greve, o hospital pode aparecer num aviso de capacidade.

Consulte [Educação e saúde](education_healthcare.md#education-healthcare-notices) para ver o cálculo completo.

#### Que informação aparece no aviso?

Um aviso lista até **10 edifícios afetados**, com os problemas mais graves primeiro.

Cada edifício listado tem uma ligação que leva a câmara à sua localização.

**O Ministério não cria um aviso separado para cada edifício sobrelotado.**

Em vez disso, agrupa os edifícios por tipo de equipamento para manter a lista de conselheiros gerível.

#### O que acontece quando os edifícios afetados mudam?

O Ministério mantém apenas um aviso ativo por cada tipo de equipamento.

Se o número de edifícios afetados se mantiver, mas a sua identidade mudar, atualiza as ligações para os edifícios do aviso existente.

Se o número mudar, o aviso antigo é retirado antes de ser mostrado um substituto.

**Nunca há dois avisos ativos para o mesmo tipo de equipamento.**

Se ocorrerem duas substituições em 30 dias simulados, o Ministério introduz uma **pausa de 30 dias** antes de novas substituições para esse tipo.

As atualizações das ligações de um aviso existente podem continuar durante a pausa.

#### Quando é removido o aviso?

Quando todos os edifícios afetados recuperam, o aviso é retirado.

Um aviso que dispense manualmente não volta a ser mostrado enquanto os mesmos edifícios continuarem afetados.

Este estado de dispensa sobrevive à reabertura da cidade.

Na primeira avaliação depois da abertura, o Ministério retira também qualquer aviso que tenha ficado de uma sessão anterior.

Um aviso de capacidade pode permanecer na lista de notícias até **720 dias simulados** enquanto o problema persistir.

<a id="safety-notices-landfill"></a>
#### Recomendações sobre o tamanho do aterro

O Ministério pode emitir um aviso opcional quando alterar o tamanho do aterro reduziria de forma significativa as despesas de saneamento.

Vem do **conselheiro de serviços públicos**, com tom de boa notícia e prioridade urgente.

| Recomendação | Título do aviso |
|---|---|
| **REDUZIR** | **O aterro ocupa espaço a mais!** |

Cada aviso descreve:

- As dimensões da área proposta, em quadrados.
- A poupança mensal estimada.
- A redução percentual das despesas de saneamento.
- Uma ligação **ver onde** que leva a câmara à área recomendada.

Estes avisos só estão disponíveis quando:

- **Gerir o saneamento** está ativado.
- **Avisar-me quando rever o tamanho do aterro compensar** está ativado.
- A poupança estimada é estritamente superior ao seu limiar configurado.
- A cidade contém exatamente uma zona de aterro.
- As condições de calendário e de segurança descritas no capítulo do aterro são cumpridas.

O limiar predefinido é **25%**, e o aviso sobre o tamanho do aterro está desativado por predefinição.

O Ministério analisa o tamanho do aterro no máximo uma vez por mês simulado.

Respeita também:

- O **período de proteção de 24 meses** depois de alterações ao tamanho do aterro ou quando um aterro é encontrado pela primeira vez.
- O **intervalo de 12 meses** depois de emitir um aviso.

Consulte [Lixo e aterros](garbage.md#garbage-landfills-advice).

**As reparações de quadrados fantasma nunca geram avisos dos conselheiros.**

<a id="safety-notices-game"></a>
#### Mensagens originais do SimCity 4 modificadas pelo Ministério

O Ministério silencia algumas das mensagens originais dos conselheiros do jogo que seriam enganadoras ou redundantes enquanto a gestão automática está ativa, **mas apenas enquanto consegue mesmo gerir o serviço em causa**. Em todos os outros casos as mensagens originais são exatamente as que o jogo traz.

Isto é feito por dois pequenos scripts do próprio Ministério dentro de `DepartmentEfficiency.dat`. Seguem seis interruptores que o Ministério publica em cada cidade e atualiza sem reiniciar o jogo: quando desliga um serviço, desliga todo o Ministério, ou quando o Ministério não consegue atuar sobre um serviço (por exemplo, por faltarem leituras ou porque um dos controladores parou por segurança), as mensagens originais desse serviço voltam de imediato. Os serviços são independentes entre si.

| Mensagens originais do jogo | Silenciadas enquanto o Ministério gere |
|---|---|
| Ofertas de contratos, sugestões de cancelamento, renovações e avisos de incumprimento com os vizinhos | os contratos desse recurso (eletricidade, água ou lixo, separadamente) |
| Conselhos normais de financiamento da educação e da saúde; os rumores de pré-greve limitam-se aos períodos em que há mesmo uma greve | a educação, ou a saúde, respetivamente |
| Os avisos de problemas de lixo limitam-se a situações com uma quantidade significativa de lixo por recolher | o saneamento local |

Os alertas de greve verdadeiros e as mensagens positivas dos conselheiros nunca são alterados. Uma mensagem cujo evento ocorra enquanto está silenciada não é mostrada depois.

#### Por que são suprimidos alguns avisos originais sobre o lixo?

O SimCity 4 por vezes avisa de problemas de lixo apenas porque o armazenamento do aterro está a aumentar.

No entanto, um aterro que se vai enchendo aos poucos não é necessariamente prova de que o sistema de saneamento da cidade está a falhar.

Por isso, o Ministério acrescenta uma exigência baseada na quantidade de lixo que realmente ficou por recolher.

**Lixo por recolher significativo**

```math
U>100+0.05G
```

Em que:

- **U** é a quantidade atual de lixo por recolher segundo o jogo.
- **G** é o lixo produzido pela cidade, usando os valores dos conselheiros do jogo.

O aviso original sobre o lixo só pode aparecer quando esta condição adicional é cumprida, juntamente com a condição normal de aviso do jogo.

**Exemplo**

Se a cidade produz 20.000 unidades de lixo, o limiar é:

```math
100+0.05\times20000=1100
```

O jogo tem de comunicar **mais de 1.100 unidades de lixo por recolher** para que o aviso modificado de problema de lixo seja permitido.

Isto evita alertas desnecessários quando a eliminação do lixo funciona normalmente mas o armazenamento do aterro está a aumentar.

#### Como posso repor as mensagens originais dos conselheiros do jogo?

Não há nada a remover. O Ministério publica o seu estado em cada cidade: desligue o serviço correspondente na janela de opções (ou revogue a ordenança, ou defina `Mode=Off`) e as mensagens originais voltam de imediato, sem reiniciar o jogo. Voltam também quando o Ministério não consegue atuar sobre um serviço. Uma mensagem que já estava escondida volta sujeita à frequência de apresentação do próprio jogo, o que pode demorar algum tempo.

**Um evento que ocorra enquanto a sua mensagem está silenciada não é mostrado depois.**

<a id="safety-isolation"></a>
### 4. Condições de segurança

#### Como trata o Ministério as falhas técnicas?

O Ministério está dividido em componentes de gestão independentes.

**Uma falha num componente não deve desativar serviços sem relação com ele nem parar o próprio SimCity 4.**

Quando uma verificação de segurança falha, o componente afetado deixa de fazer alterações em vez de tentar repetidamente uma operação pouco fiável.

| Falha | O que o Ministério faz |
|---|---|
| **Falha a verificação do financiamento das centrais elétricas ou da água** | Tenta desfazer as alterações feitas nesse ciclo e congela a gestão do financiamento desse recurso durante a sessão da cidade. Os outros recursos e os contratos regionais podem continuar. |
| **Falha a verificação do financiamento do saneamento** | Para as alterações ao financiamento do saneamento durante a sessão. Os contratos de lixo e os gestores dos outros recursos podem continuar. |
| **Uma operação de contrato falha tecnicamente** | Para a gestão automática de contratos durante a sessão. Os gestores de financiamento local continuam a usar os contratos existentes. |
| **Não é possível verificar uma alteração de financiamento de uma escola ou de um hospital** | Exclui esse edifício durante a sessão. Depois de falhas que envolvam três edifícios diferentes, as escritas de financiamento da educação e da saúde param durante a sessão. |
| **O controlo de financiamento de um edifício não tem efeito** | Exclui esse edifício, deixando os outros continuar. |
| **A demolição automática falha uma verificação de segurança** | Para a demolição durante a sessão se uma pré-visualização de preço alterar um lote, se o zonamento desaparecer ou se três ordens consecutivas não forem executadas. |
| **Uma operação de conselheiro produz um erro** | Deixa de verificar e retirar os avisos do Ministério durante o resto da sessão. |
| **A janela Opções de Jogo tem uma disposição inesperada** | Deixa a janela original inalterada e não acrescenta o botão do Ministério durante essa sessão. |
| **Um ficheiro de configuração é inválido** | Desativa o componente de gestão afetado em vez de usar definições pouco fiáveis. |

#### Como são anunciadas as falhas técnicas?

Se um serviço que ativou tiver de parar até ao fim da sessão da cidade por causa de uma falha técnica, o Ministério acrescenta **uma notícia nativa** à lista de notícias na barra inferior do jogo. O título aparece na lista e o detalhe ao clicar. Não há pop-up nem janela de conselheiro.

- Há sete tipos: eletricidade, água, saneamento, saúde, educação, contratos regionais com cidades vizinhas e demolição automática.
- Há **uma notícia por serviço e por sessão da cidade**. Saúde e educação são notícias separadas e só aparecem se as ativou.
- Nunca é mostrada para algo que recupera sozinho, para um serviço em **STANDBY**, para uma leitura recusada, para um serviço que desligou, nem nos modos **Off** e **Shadow**. Não há mensagens de recuperação.
- A notícia é retirada quando o serviço é desligado ou a sua gestão é reiniciada. A causa técnica fica apenas no registo, em linhas `[FailureNotice]`.
- Os textos existem nos dez idiomas do plugin.

**STANDBY** significa que o serviço ainda não tem nada para gerir na cidade (por exemplo o saneamento local numa cidade sem aterro, incineradora, centro de reciclagem nem central de resíduos para energia). Não é um erro, não gera aviso e começa sozinho quando aparece algo para gerir.

**Onde está o registo?** O `DepartmentEfficiency.log` é escrito na pasta `Logs` da pasta de utilizador do SimCity 4, normalmente `Documents\SimCity 4\Logs\DepartmentEfficiency.log`; se iniciar o jogo com `-UserDir`, é a pasta `Logs` dessa pasta. Se essa pasta não puder ser usada, o registo é escrito ao lado da DLL, em `Plugins`, e a primeira linha diz porquê. Um registo existente continua a ser acrescentado e nunca é movido.

#### O que significa uma paragem de segurança da sessão?

Uma paragem de segurança dura até ao fim da sessão da cidade em curso.

Por exemplo, se a verificação do financiamento da água falhar, o Ministério deixa de fazer novas alterações ao financiamento da água enquanto essa cidade continuar aberta.

Não envia repetidamente o mesmo comando.

**Recarregar a cidade inicia uma nova sessão e elimina as paragens de segurança específicas da sessão.**

O problema de fundo pode continuar a existir, pelo que a mesma verificação pode voltar a falhar se não tiver sido resolvido.

#### Um erro do Ministério pode fazer o SimCity 4 falhar?

O Ministério envolve os eventos do jogo que trata de modo a que os erros internos sejam apanhados e registados no seu registo.

**Um erro na gestão do Ministério destina-se a parar a operação afetada, e não o próprio jogo.**

Este isolamento é uma parte essencial da sua conceção.

<a id="safety-version"></a>
#### Versão do jogo suportada

O Ministério verifica a versão do executável do jogo quando arranca.

**A versão suportada é o SimCity 4 Deluxe 1.1.641.0 (Steam, Windows).**

Em qualquer outra versão, ou quando a versão não pode ser lida, o Ministério permanece completamente inativo.

Isto significa que não há:

- Subscrições de eventos do jogo.
- Botão de opções do Ministério.
- Lei do Ministério.
- Operações de financiamento, de contratos ou de demolição.
- Modificação do ícone de água das quintas.

A mesma inatividade total aplica-se quando o ficheiro de recursos principal, `DepartmentEfficiency.dat`, não se encontra ao lado da DLL. A DLL, o DAT e o INI são instalados juntos, diretamente na pasta Plugins; o plugin nunca os procura noutro lugar.

**O Ministério não tenta funcionar numa instalação não suportada ou incompleta.**

#### As gravações das suas cidades não dependem do plugin

O Ministério foi concebido para que as cidades gravadas enquanto está instalado possam ser abertas sem ele.

A sua lei **Instituir o Ministério da Optimização** é removida temporariamente da cidade antes de cada gravação e restaurada depois.

A lei não fica, por isso, guardada na cidade gravada.

**O Ministério nunca edita diretamente os ficheiros de gravação de cidades ou de regiões.**

Se remover o plugin, os níveis de financiamento normais da cidade e os contratos regionais ficam nos últimos valores estabelecidos.

Pode continuar a jogar sem o Ministério.

<a id="safety-settings"></a>
### 5. Definições

O sistema geral de segurança não exige um conjunto de opções separado.

Segue as definições de gestão descritas ao longo deste guia.

**Ministério ativo**

[Ministério ativo](options.md#opt-master) é o interruptor principal.

Também o pode controlar através da lei **Instituir o Ministério da Optimização** na janela de leis do SimCity 4.

Ambos os controlos representam a mesma definição.

**Opções de gestão individuais**

Cada serviço tem o seu próprio gestor e, quando apropriado, uma reserva ou margem de segurança.

Consulte a [Referência das opções](options.md#options) para ver todas as definições.

**Avisos dos conselheiros**

O Ministério tem apenas duas opções específicas de avisos, ambas relacionadas com as recomendações sobre o tamanho do aterro:

- [Avisar-me quando rever o tamanho do aterro compensar](options.md#opt-landfilladvice).
- [Limiar de poupança dos avisos do aterro](options.md#opt-landfillthreshold).

**Os avisos de falta de serviços públicos não podem ser desativados em separado.**

Seguem as definições de gestão do serviço correspondente e só aparecem quando as suas condições são cumpridas.

<a id="safety-examples"></a>
### 6. Exemplos práticos

Os exemplos seguintes mostram como o Ministério reage a faltas persistentes, a informação incompleta e a alterações manuais.

#### Exemplo 1: Uma falta de eletricidade que não se consegue resolver

Imagine que a sua cidade tem a seguinte situação no **dia simulado 40** após a abertura:

- **Procura de eletricidade:** 10 000 unidades.
- **Eletricidade entregue:** 9 200 unidades.
- **Capacidade local máxima de produção:** 8 900 unidades.
- **Importações regionais utilizáveis:** 200 unidades.
- **Vendas:** nenhuma.

O Ministério já considerou o financiamento máximo legal das centrais e todas as compras disponíveis.

A oferta total disponível é:

```math
8900+200=9100
```

É inferior à procura da cidade:

```math
9100<10000
```

Os contadores do jogo confirmam também que a eletricidade entregue está abaixo da procura.

**A falta fica comprovada no dia 40.** O Ministério espera 15 dias simulados.

**Dia 55**

A falta continua por resolver. O Ministério emite o primeiro aviso, que pode abrir como pop-up:

**Apagão! Quem comprou as velas?**

**Dia 175**

A falta continua. O lembrete aparece 120 dias simulados depois do aviso.

**Dia 180**

Constrói outra central elétrica. Leituras novas confirmam que a eletricidade entregue já cobre a procura. O Ministério retira o aviso e termina os lembretes.

**Dia 250**

Fica comprovada uma nova falta de eletricidade. Como o episódio anterior terminou, começa o seu próprio período de espera; se continuar por resolver, um novo primeiro aviso aparece no **dia 265**.

#### Exemplo 2: Edifícios sem eletricidade, mas com capacidade de produção suficiente

Considere uma cidade com:

- **Procura de eletricidade:** 10.000 unidades.
- **Eletricidade fornecida:** 9.600 unidades.
- **Capacidade de produção local:** 12.000 unidades.
- **Sem importações nem exportações.**

Embora parte da procura não esteja a ser satisfeita, a cidade tem capacidade de produção suficiente.

```math
12000\ge10000
```

Isto significa que o teste de falta de fornecimento não se verifica.

O problema situa-se provavelmente na rede de distribuição de eletricidade.

**O Ministério não aumenta o financiamento nem emite um aviso de falta regional.**

Produzir mais não chegaria necessariamente aos edifícios desligados.

O jogador tem de inspecionar a rede elétrica.

#### Exemplo 3: Reabrir uma cidade durante uma falta existente

Imagine que uma falta de água foi comunicada no **dia 70** de uma sessão e que guarda a cidade no dia 100 enquanto continua por resolver.

Mais tarde, reabre a cidade. O Ministério começa uma nova sessão: **não se lembra de nada** da anterior e retira o aviso antigo, se o jogo o manteve na lista.

Se a falta de água continuar, é comprovada de novo e o primeiro aviso chega após os 30 dias simulados normais. Reabrir a cidade nunca produz, portanto, um aviso imediato, e o calendário de lembretes antigo não continua.

#### Exemplo 4: Alterar manualmente o financiamento da água

Imagine que o Ministério optimizou o financiamento da água para **62%**.

Aumenta-o manualmente para **100%**, com **Gerir a água** ainda ligado.

O Ministério aceita a alteração como o novo valor de partida.

Na avaliação seguinte da água, no máximo 10 dias simulados depois, recalcula o financiamento necessário.

Se concluir que **64%** é suficiente, pode reduzir o financiamento de 100% para 64%.

**O Ministério não preserva as alterações manuais dos cursores enquanto a gestão automática estiver ligada.**

Para manter o financiamento da água em 100%, desligue **Gerir a água**.

#### Exemplo 5: As leituras da água não estão prontas depois de carregar a cidade

Logo a seguir a abrir uma cidade, o jogo indica:

- **Procura de água:** 20.000 unidades.
- **Consumo de água:** 20.500 unidades.

O Ministério verifica se o consumo é coerente com a procura.

A leitura máxima aceite é:

```math
20000\times1.01+1=20201
```

No entanto:

```math
20500>20201
```

O consumo excede a tolerância permitida.

**As leituras são incoerentes, por isso o Ministério ainda não toma nenhuma decisão sobre o financiamento da água.**

Espera por uma nova leitura.

Os outros serviços são avaliados com a sua própria informação.

**Um problema temporário nos dados da água não impede que a eletricidade, o saneamento ou outros gestores independentes funcionem.**

<a id="safety-limitations"></a>
### 7. Limitações conhecidas

O Ministério foi concebido para funcionar de forma conservadora, mas não consegue ultrapassar todas as limitações do SimCity 4.

#### Controlo manual

- **As alterações manuais ao financiamento não ficam bloqueadas.** Quando um gestor de serviço está ligado, pode rever, na avaliação seguinte, uma percentagem de financiamento que tenha alterado por si.
- **Não existem bloqueios por edifício.** Para manter um determinado serviço sob controlo manual do financiamento, desligue o gestor correspondente.

#### Avisos de falta

- **Os avisos de falta seguem a gestão local.** Se gerir um serviço à mão, o Ministério não o avisa sobre ele.
- **O Ministério não consegue saber se um aviso foi mesmo mostrado ou dispensado.** O SimCity 4 decide se abre um pop-up consoante as suas definições de conselheiros. Os lembretes seguem intervalos fixos.
- **Os avisos de lixo assentam num modelo e em leituras mensais.** Ao contrário da eletricidade e da água, o lixo é avaliado pela projeção de tratamento do Ministério e pelo atraso do jogo, mês a mês.

#### Mensagens originais dos conselheiros do jogo

- **O Ministério só silencia as mensagens do jogo enquanto gere mesmo o serviço.** Desligar um serviço, ou um serviço sobre o qual o Ministério não consegue atuar, faz voltar de imediato as mensagens originais.
- **Um evento que ocorra enquanto a sua mensagem está silenciada não é mostrado depois.** Uma mensagem que volta fica sujeita à frequência de apresentação do próprio jogo.

#### Segurança técnica

- **Uma paragem de segurança técnica dura toda a sessão da cidade em curso.** O gestor afetado fica desligado até a cidade ser carregada de novo.
- **Só a versão do jogo suportada é gerida.** O Ministério suporta o **SimCity 4 Deluxe 1.1.641.0 da Steam, no Windows**. Não executa nenhuma operação em versões não suportadas.

**O Ministério foi concebido para optimizar a cidade apenas quando tem informação fiável suficiente para agir em segurança. Quando não consegue verificar uma operação ou provar que uma alteração é benéfica, prefere deixar a situação existente como está.**

---

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
