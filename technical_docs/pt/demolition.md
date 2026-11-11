[English](../en/demolition.md) | **Português** | [Français](../fr/demolition.md) | [Español](../es/demolition.md)

[Voltar ao índice](index.md#contents) | [Voltar ao README](../../README.pt.md)

<a id="demolition"></a>
## F. Demolição automática

O Ministério pode limpar automaticamente certos edifícios abandonados e ruínas, permitindo que o solo zonado da cidade volte a desenvolver-se.

Trata duas situações:

- **Ruínas:** edifícios destruídos por incêndios ou desastres que permanecem nos seus lotes.
- **Edifícios abandonados:** edifícios que estão vazios há tempo suficiente.

O Ministério verifica os lotes elegíveis **uma vez por mês simulado**.

O jogador controla:

- Quanto tempo um edifício tem de permanecer abandonado ou destruído antes de ser demolido.
- Que tipos de zona podem ser incluídos.
- Que motivos de abandono são elegíveis.

**A demolição nunca é gratuita.**

O Ministério usa o comando normal de demolição do SimCity 4 e paga o mesmo preço que o jogador pagaria ao demolir o lote manualmente.

Antes de cada demolição, pergunta ao jogo o custo real e verifica se o tesouro da cidade o pode suportar.

**Os edifícios municipais e os edifícios com proteção histórica nunca são demolidos automaticamente.**

O objetivo é limpar lotes abandonados sem destruir edifícios importantes nem gastar mais dinheiro do que a cidade tem disponível.

<a id="demolition-vanilla"></a>
### 1. Como funciona a demolição automática com o SimCity 4

#### Como o jogo acompanha o estado dos edifícios

O SimCity 4 acompanha um estado para cada lote.

Regista também **quantos dias simulados passaram desde a última alteração desse estado**.

O Ministério usa estes valores já existentes para decidir se um lote é elegível para demolição.

| Estado no jogo | O que se vê | Ação do Ministério |
|---|---|---|
| **Ocupado** | Um edifício em funcionamento | Nunca é demolido |
| **Em construção** | Um edifício em obras | Nunca é demolido |
| **Pronto / Criado** | Um lote à espera de desenvolvimento ou um edifício a ficar disponível | Nunca é demolido |
| **Vago** | Um edifício abandonado | Pode ser demolido após o período de espera escolhido |
| **Em demolição** | Um edifício que já está a ser demolido | Ignorado |
| **Destruído** | Ruínas deixadas por um incêndio ou desastre | Podem ser demolidas após o período de espera escolhido |

**Só os lotes Vagos e Destruídos são considerados para demolição automática.**

Todos os outros estados são excluídos.

#### Como se mede o período de espera?

O Ministério usa **o contador de dias do próprio jogo**, não um temporizador separado.

Por exemplo, imagine-se que um edifício está abandonado há 200 dias simulados.

Se o período de espera configurado for de 180 dias, já tem idade suficiente para ser considerado na próxima revisão mensal de demolição, desde que as restantes condições se verifiquem.

O contador é guardado com a cidade.

**Isto significa que o Ministério reconhece também os edifícios que foram abandonados antes de o mod ser instalado.**

Não é preciso esperar que o período de abandono recomece.

#### Porque foi abandonado um edifício?

O SimCity 4 regista o motivo pelo qual um edifício ficou abandonado.

O Ministério lê esse motivo e usa as opções escolhidas para decidir se o lote deve ser elegível.

| Motivo de abandono no jogo | O que significa | Opção que controla |
|---|---|---|
| **Baixa atratividade** | A zona é pouco atraente por fatores como poluição, criminalidade ou serviços deficientes. | **Motivo: zona pouco atraente** |
| **Tempo de deslocação** | Os ocupantes não conseguem chegar aos empregos num tempo de viagem aceitável. | **Motivo: trajeto demasiado longo** |
| **Baixa procura** | Não há procura suficiente para o tipo de edifício. | **Motivo: falta de procura** |
| **Sem eletricidade** | O edifício foi abandonado por não ter eletricidade. | **Motivo: sem eletricidade** |
| **Sem água** | O edifício foi abandonado por não ter água. | **Motivo: sem água** |
| **Ocupantes despejados** | O jogo indica que os ocupantes foram retirados. | Sempre elegível por motivo; sem opção própria |
| **Nenhum / desconhecido** | O jogo não indica nenhum motivo de abandono reconhecível. | Sempre elegível por motivo; sem opção própria |

**Ser elegível por motivo não significa demolição imediata.** O período de espera, as restrições de zona, a proteção histórica e as outras verificações de segurança continuam a aplicar-se.

#### Porque é que «Sem eletricidade» e «Sem água» vêm desativados por predefinição?

Um edifício abandonado por falta de eletricidade ou de água pode voltar a ser ocupado quando o serviço for restabelecido.

Por exemplo:

- Um prédio de apartamentos fica sem eletricidade e é abandonado.
- A ligação elétrica é reparada.
- O edifício pode voltar a ser ocupado sem ter sido demolido.

Demoli-lo demasiado cedo seria desnecessário.

**É por isso que o abandono causado pela falta de eletricidade ou de água é excluído por predefinição.**

Estes motivos podem ser ativados se quiser que o Ministério limpe esses edifícios depois do período de espera escolhido.

#### Que tipos de zona são elegíveis?

O Ministério distingue entre o desenvolvimento zonado normal e os lotes municipais ou de uso especial protegidos.

| Grupo de zona | Tipos de zona incluídos |
|---|---|
| **Residencial** | Zonas residenciais de baixa, média e alta densidade |
| **Comercial** | Zonas comerciais de baixa, média e alta densidade |
| **Industrial** | Zonas industriais de média e alta densidade |
| **Agrícola** | Quintas e campos agrícolas |
| **Municipal / protegido** | Edifícios colocados manualmente, lotes de uso especial e lotes sem zona |

O grupo municipal e protegido inclui:

- Escolas e universidades.
- Hospitais.
- Esquadras de polícia e quartéis de bombeiros.
- Parques e monumentos.
- Edifícios de recompensa.
- Infraestruturas de serviços públicos.
- Instalações militares.
- Aeroportos, portos marítimos e espaçoportos.
- Aterros.
- Lotes sem zonamento.

**O Ministério não demole automaticamente lotes municipais ou protegidos.**

Estes edifícios representam decisões deliberadas do jogador ou infraestruturas de uso especial, e não desenvolvimento abandonado normal.

#### E as quintas?

Os lotes agrícolas exigem um cuidado especial.

Os campos das quintas podem parecer vagos como parte do seu funcionamento normal, mesmo quando a própria quinta está a funcionar.

**Um lote agrícola com aspeto de vago não é, por si só, prova de que a quinta deva ser removida.**

Por isso, o Ministério distingue o zonamento agrícola do desenvolvimento residencial, comercial e industrial normal, permitindo controlá-lo em separado.

#### Os edifícios históricos estão sempre protegidos

O SimCity 4 permite designar certos edifícios como históricos.

O jogo regista essa designação através de uma marca de edifício histórico.

**Um edifício com designação histórica está protegido da demolição automática**, mesmo que fique abandonado e tenha ultrapassado o período de espera configurado.

O Ministério respeita essa decisão.

#### Como funciona a demolição com a escavadora?

Quando se seleciona manualmente a ferramenta de demolição (escavadora) no SimCity 4, o jogo calcula o preço da demolição do lote.

Verifica também se o lote pode realmente ser demolido no seu estado atual.

O Ministério usa as mesmas funções internas do jogo.

**Antes de demolir um lote, obtém o custo de demolição do próprio jogo.**

Não presume que todos os edifícios têm o mesmo preço de demolição.

Se o jogo indicar que um lote não pode ser demolido neste momento, o Ministério ignora-o.

**O que acontece ao terreno depois?**

Demolir um edifício não remove o seu zonamento.

Por exemplo:

- Um edifício residencial demolido deixa para trás o zonamento residencial.
- A cidade pode mais tarde desenvolver outro edifício residencial nesse lote.

O mesmo se aplica aos edifícios abandonados elegíveis e às ruínas.

**O Ministério limpa o edifício, não a zona.**

As ruínas deixadas por incêndios ou desastres permanecem nos seus lotes até que algo as demola, manualmente ou através da demolição automática do Ministério.

<a id="demolition-problem"></a>
### 2. O problema da gestão

Nas grandes cidades, os edifícios abandonados e as ruínas podem tornar-se difíceis de gerir manualmente.

Surgem vários problemas.

**1. As ruínas impedem novo desenvolvimento**

Os edifícios destruídos por incêndios ou desastres podem permanecer em terreno zonado.

Enquanto não forem demolidos, esse terreno não pode simplesmente desenvolver um edifício de substituição.

Numa cidade grande, as ruínas dispersas podem passar facilmente despercebidas.

**2. O abandono prolongado bloqueia a reconstrução**

Um edifício abandonado por baixa procura, pouca atratividade ou tempo de deslocação excessivo pode permanecer vazio durante muito tempo.

Limpar o lote pode permitir que o jogo desenvolva um edifício diferente quando as condições melhorarem.

**3. Nem todos os edifícios aparentemente vagos devem ser demolidos**

Alguns edifícios podem recuperar naturalmente.

Um edifício abandonado por falta de eletricidade ou de água pode voltar a ser ocupado quando o serviço regressar.

Os campos agrícolas podem parecer vagos como algo normal.

Os edifícios municipais e os edifícios históricos também têm de ser protegidos.

**4. A demolição custa dinheiro**

Cada operação de demolição tem um preço determinado pelo jogo.

Demolir muitos edifícios no mesmo mês poderia consumir uma parte significativa do tesouro de uma cidade pequena.

**O Ministério tem, por isso, de distinguir entre os lotes genuinamente elegíveis para demolição e os que devem ser deixados em paz, respeitando as definições do jogador e o dinheiro disponível na cidade.**
<a id="demolition-algorithm"></a>
### 3. Como o Ministério gere a demolição

O Ministério verifica a cidade **uma vez por mês simulado** à procura de ruínas e edifícios abandonados que cumpram as definições de demolição.

Não demole nada imediatamente quando se abre uma cidade. A primeira revisão de demolição realiza-se na **primeira mudança de mês simulado depois do carregamento**.

#### Como funciona a revisão mensal de demolição?

Em cada revisão mensal, o Ministério segue estes passos:

1. **Encontrar os lotes elegíveis.** Percorre a cidade à procura de edifícios destruídos ou abandonados e verifica a sua idade, zonamento, motivo de abandono e estado de proteção.
2. **Decidir que lotes se qualificam.** Aplica as definições de demolição escolhidas e as regras de elegibilidade do próprio jogo.
3. **Ordenar os candidatos.** As ruínas vêm primeiro, seguidas dos edifícios abandonados. Dentro de cada grupo, os lotes mais antigos vêm primeiro.
4. **Verificar os limites.** Respeita o número máximo de ordens de demolição, o tesouro da cidade e qualquer orçamento mensal de demolição que tenha sido configurado.
5. **Verificar de novo cada lote.** Antes de enviar uma ordem, confirma que o lote não mudou e pergunta ao jogo o preço real de demolição.
6. **Enviar a ordem de demolição.** Usa a operação normal de demolição do SimCity 4, sem remover o zonamento subjacente.
7. **Verificar o resultado.** Numa verificação posterior, confirma que o edifício desapareceu e que o terreno continua zonado.

**O Ministério nunca demole um lote só porque pareceu elegível numa verificação anterior.**

Volta a verificá-lo imediatamente antes de emitir a ordem.

#### Que edifícios são demolidos primeiro?

A ordem de prioridade é:

1. **Edifícios destruídos (ruínas)**, os mais antigos primeiro.
2. **Edifícios abandonados**, os mais antigos primeiro.

Isto significa que uma ruína elegível recente é tratada antes de um edifício abandonado mais antigo.

O Ministério pode normalmente emitir até **150 ordens de demolição durante uma revisão mensal**.

No entanto, aplica-se uma precaução adicional quando começa uma sessão de cidade.

**Antes de o jogo ter confirmado pelo menos uma ordem de demolição nessa sessão, o Ministério envia no máximo três ordens por revisão mensal.**

Isto permite-lhe verificar se o mecanismo de demolição do jogo funciona corretamente antes de tratar números maiores de edifícios.

#### O que acontece entre as revisões mensais?

A cada **10 dias simulados**, o Ministério faz uma verificação leve de demolição.

Esta verificação:

- Confirma se as ordens de demolição anteriores foram executadas.
- Regista as alterações nos lotes acompanhados.
- Procura resultados inesperados.

**A verificação de 10 dias nunca emite novas ordens de demolição.**

As novas demolições só são feitas durante as revisões mensais.

#### Como decide o Ministério se um lote se qualifica?

As regras seguintes são verificadas por ordem.

| Condição | Decisão |
|---|---|
| O jogo já está a demolir o lote | Ignorar |
| O lote não está abandonado nem destruído | Não elegível |
| O lote é histórico e não se qualifica para a exceção especial das ruínas | Manter |
| A demolição automática está desativada para esse tipo de lote | Manter |
| O lote é municipal e não se qualifica para a exceção especial das ruínas | Manter |
| Um edifício abandonado está num tipo de zona que foi excluído | Manter |
| Um edifício abandonado tem um motivo de abandono excluído | Manter |
| O lote não atingiu o período de espera configurado | Manter |
| O jogo indica que o lote não pode ser demolido | Manter |
| Nenhuma das condições anteriores se aplica | **Elegível para demolição** |

**Uma distinção importante:**

As opções de zona e de motivo de abandono aplicam-se apenas aos **edifícios abandonados**.

Não filtram as ruínas.

Por exemplo, se **Demolir edifícios destruídos** estiver ativado, as ruínas de um edifício residencial, comercial, industrial ou agrícola podem ser limpas depois do período de espera, mesmo que esse tipo de zona esteja excluído da demolição de edifícios abandonados.

As ruínas municipais e históricas têm a sua própria definição de proteção, separada.

<a id="demolition-destroyed"></a>
#### Edifícios destruídos: limpar ruínas

Quando **Demolir edifícios destruídos** está ativado, o Ministério pode remover automaticamente as ruínas deixadas por incêndios e desastres.

Por predefinição, espera **um mês simulado**, equivalente a 30 dias.

Por exemplo:

- Um edifício comercial é destruído por um incêndio.
- O jogo marca o lote como Destruído.
- O Ministério espera até terem passado pelo menos 30 dias simulados.
- Na revisão mensal de demolição seguinte, as ruínas tornam-se elegíveis.
- Se o jogo permitir a demolição e a cidade a puder pagar, o Ministério remove-as.

**O zonamento comercial subjacente mantém-se**, permitindo novo desenvolvimento mais tarde.

As ruínas em terrenos residenciais, comerciais, industriais e agrícolas normais seguem esta regra.

**As ruínas municipais e históricas estão protegidas por predefinição.**

Só podem ser incluídas se ativar explicitamente **Demolir edifícios municipais e históricos destruídos**.

<a id="demolition-abandoned"></a>
#### Edifícios abandonados

Quando **Demolir edifícios abandonados** está ativado, o Ministério considera os edifícios que permaneceram abandonados durante um período suficientemente longo.

Por predefinição, espera **12 meses simulados**, equivalentes a 365 dias.

Um edifício abandonado tem de cumprir todas estas condições:

- Pertence a um tipo de zona ativado.
- O seu motivo de abandono está ativado, ou é um dos motivos que são sempre elegíveis.
- Está abandonado há pelo menos o período de espera configurado.
- Não é municipal nem está protegido por ser histórico.
- O SimCity 4 confirma que o lote pode ser demolido.

**Comportamento predefinido**

O Ministério pode demolir edifícios residenciais, comerciais e industriais abandonados quando o abandono for causado por:

- Uma zona pouco atraente.
- Um tempo de deslocação excessivo.
- Procura insuficiente.

O abandono causado pela falta de eletricidade ou de água é excluído por predefinição.

Isto é intencional: um edifício abandonado por um problema temporário de serviço pode voltar a ser ocupado quando esse serviço for restabelecido.

<a id="demolition-delays"></a>
#### Como funcionam os períodos de espera?

A janela de opções permite escolher os períodos de espera em **meses simulados**.

Internamente, porém, o SimCity 4 regista há quanto tempo um edifício se encontra no seu estado atual em **dias simulados**.

O Ministério converte, por isso, o número de meses escolhido em dias.

**Conversão do período de espera**

```math
D=\mathrm{round}\left(\frac{M\times365}{12}\right)
```

Em que:

- **M** é o período de espera escolhido, em meses simulados.
- **D** é o período de espera guardado e usado pelo Ministério, em dias simulados.

O resultado é arredondado ao dia inteiro mais próximo, arredondando para cima os valores exatos de meio dia.

**Exemplos**

| Meses escolhidos | Dias usados |
|---:|---:|
| 1 | 30 |
| 2 | 61 |
| 3 | 91 |
| 6 | 183 |
| 12 | 365 |
| 24 | 730 |
| 36 | 1.095 |
| 60 | 1.825 |

#### Períodos de espera disponíveis

| Tipo de edifício | Intervalo no jogo | Predefinição |
|---|---|---|
| **Ruínas** | 1–24 meses | 1 mês (30 dias) |
| **Edifícios abandonados** | 1–60 meses | 12 meses (365 dias) |

Se o ficheiro de configuração for editado manualmente, os intervalos guardados permitidos são mais amplos:

- **Ruínas:** 1–3.650 dias.
- **Edifícios abandonados:** 1–7.300 dias.

#### Como sabe o Ministério quando um lote tem idade suficiente?

Usa o contador do próprio jogo.

**Requisito de idade para demolição**

```math
A\ge D
```

Em que:

- **A** é o número de dias simulados desde que o lote ficou abandonado ou destruído.
- **D** é o período de espera configurado, em dias.

Por exemplo, se o período de espera for de 91 dias:

- Aos 85 dias de idade, o lote é demasiado recente.
- Aos 91 dias, torna-se elegível.
- É efetivamente tratado na primeira revisão mensal depois de atingir essa idade.

**A demolição pode, portanto, acontecer até aproximadamente um mês simulado depois de se atingir o período de espera.**

#### E se editar o período de espera diretamente no ficheiro de configuração?

A janela de opções converte os dias guardados de volta num número aproximado de meses.

```math
M_{\mathrm{shown}}=
\min\left(
M_{\max},
\max\left(
1,\mathrm{round}\left(\frac{D\times12}{365}\right)
\right)
\right)
```

Aqui, **Mmax** é de 24 meses para as ruínas e de 60 meses para os edifícios abandonados.

Por exemplo, um atraso configurado manualmente de **45 dias** aparece como aproximadamente **1 mês** na janela de opções.

No entanto, o valor subjacente de 45 dias é preservado até que o controlo do período de espera seja de facto alterado e a alteração aceite.

**Abrir simplesmente a janela de opções não reescreve o atraso configurado manualmente.**

<a id="demolition-zones"></a>
#### Que zonas pode o Ministério demolir?

O Ministério oferece quatro opções de zona:

| Opção de zona | Predefinição |
|---|---|
| **Zona residencial** | Ligado |
| **Zona comercial** | Ligado |
| **Zona industrial** | Ligado |
| **Zona agrícola** | Desligado |

Estas opções determinam que **edifícios abandonados** são elegíveis.

Por exemplo, desligar a Zona comercial impede o Ministério de demolir edifícios comerciais abandonados.

**Estas restrições não se aplicam às ruínas.**

Se a demolição automática de ruínas estiver ativada, os edifícios destruídos em zonas de crescimento podem ainda assim ser limpos, independentemente das opções de zona acima.

Os lotes municipais continuam protegidos independentemente destas definições.

<a id="demolition-reasons"></a>
#### Que motivos de abandono são elegíveis?

É também possível escolher que motivos de abandono permitem a demolição automática.

| Motivo de abandono | Predefinição |
|---|---|
| **Motivo: zona pouco atraente** | Ligado |
| **Motivo: trajeto demasiado longo** | Ligado |
| **Motivo: falta de procura** | Ligado |
| **Motivo: sem eletricidade** | Desligado |
| **Motivo: sem água** | Desligado |

**Porque é que «Sem eletricidade» e «Sem água» estão desativados?**

Porque os edifícios abandonados por falta de serviços podem voltar a ser ocupados.

Por exemplo, se um edifício residencial ficar sem eletricidade, o Ministério normalmente deixa-o em paz enquanto o fornecimento de energia é restabelecido.

Estes motivos podem ser ativados se preferir demolir esses edifícios depois do período de espera.

#### Motivos sempre elegíveis

Duas situações indicadas pelo jogo não têm opções próprias:

- **Ocupantes despejados.**
- **Nenhum motivo, ou um motivo de abandono não reconhecido.**

Estes motivos são sempre considerados elegíveis quando a demolição automática de edifícios abandonados está ativada.

O período de espera normal, a seleção de zonas, as regras de proteção de edifícios e as verificações financeiras continuam a aplicar-se.

<a id="demolition-protected"></a>
#### Edifícios municipais e históricos

O Ministério protege os edifícios que representam escolhas deliberadas do jogador.

**Edifícios municipais**

Este grupo inclui:

- Escolas, hospitais, esquadras de polícia e quartéis de bombeiros.
- Parques, monumentos, recompensas e outros edifícios colocados manualmente.
- Infraestruturas de serviços públicos.
- Instalações militares.
- Aeroportos, portos marítimos e espaçoportos.
- Aterros.
- Lotes sem zonamento normal.

**Edifícios históricos**

Qualquer lote com a designação histórica do SimCity 4 também está protegido.

**Os edifícios municipais e históricos, de pé ou abandonados, nunca são demolidos automaticamente.**

Esta proteção aplica-se independentemente das definições de abandono.

#### E as suas ruínas?

Há uma exceção opcional.

**Demolir edifícios municipais e históricos destruídos** está **desligado por predefinição**.

Quando está desligado, as ruínas de edifícios municipais e históricos ficam intocadas.

Quando está ligado, o Ministério pode limpar essas ruínas depois do período de espera normal das ruínas, desde que:

- O lote esteja efetivamente no estado Destruído.
- A verificação de demolição do jogo o aceite.
- As regras do tesouro e do orçamento permitam a demolição.

**Esta opção nunca autoriza a demolição de um edifício municipal ou histórico que ainda esteja de pé.**

Aplica-se apenas depois de o edifício já ter sido destruído.

<a id="demolition-farms"></a>
#### Cuidado especial com os lotes agrícolas

Os lotes agrícolas comportam-se de forma diferente dos edifícios residenciais, comerciais e industriais normais.

No SimCity 4, alguns lotes de campos agrícolas podem aparecer no estado abandonado do jogo como parte do seu comportamento normal.

Podem também ser registados como sem água.

**Isto não significa necessariamente que a quinta já não esteja a funcionar.**

Por essa razão, **a Zona agrícola vem desativada por predefinição**.

Com esta opção desligada, as quintas e os campos agrícolas abandonados são preservados.

Se a ativar, o Ministério pode demolir lotes agrícolas abandonados elegíveis, incluindo campos que o jogador possa querer manter.

**Use esta opção com cuidado.**

As ruínas em terrenos agrícolas são tratadas em separado. Podem continuar a ser demolidas quando **Demolir edifícios destruídos** está ativado, mesmo que a Zona agrícola esteja desativada.

#### Ícones de aviso de água nas zonas agrícolas

O Ministério esconde também o ícone de aviso de *sem água* do jogo sobre os lotes agrícolas, na versão do jogo suportada.

Este ajuste visual é independente da demolição automática e não altera o serviço de água real.

Veja [Água](water.md#water-farm-icon).

<a id="demolition-money"></a>
#### Como paga o Ministério as demolições?

**Cada demolição usa o preço real de demolição indicado pelo SimCity 4.**

O Ministério não estima o preço a partir do tipo de edifício.

Antes de cada ordem de demolição, pergunta ao jogo o custo desse lote específico.

Se o jogo não conseguir fornecer um preço válido, **o lote não é demolido**.

Aplicam-se duas proteções financeiras.

#### 1. A proteção do tesouro

O Ministério nunca ordena demolições cujo custo combinado exceda o tesouro disponível da cidade no início da revisão mensal.

**Regra do tesouro**

```math
c_i\le T-S_i
```

Em que:

- **cᵢ** é o preço de demolição do lote atual.
- **T** é o tesouro no início da revisão de demolição.
- **Sᵢ** é o preço combinado das ordens de demolição já enviadas durante essa revisão.

**Porque é que o Ministério subtrai as ordens anteriores?**

Porque o SimCity 4 não deduz necessariamente as despesas de demolição de imediato quando se submete uma ordem de demolição.

Sem esta proteção, o Ministério poderia emitir várias ordens contra o mesmo dinheiro disponível.

Por exemplo, se o tesouro contiver §500 e o Ministério já tiver ordenado §300 de demolições, considera que só há §200 disponíveis para novas ordens.

**O que acontece quando um lote é demasiado caro?**

O Ministério ignora esse lote e passa ao candidato seguinte.

Um edifício mais barato pode ainda ser comportável.

**Se o tesouro estiver negativo, não é ordenada nenhuma demolição, nem sequer uma com o preço de §0.**

Esta proteção do tesouro está sempre ativa, mesmo que desligue o orçamento mensal de demolição opcional.

#### 2. O orçamento mensal de demolição opcional

Também é possível limitar quanto o Ministério pode gastar durante uma única revisão mensal de demolição.

A opção é:

**Limitar o orçamento mensal de demolição**

Está **desligada por predefinição**.

Quando está ativada, o Ministério aplica uma regra adicional.

**Regra do orçamento mensal**

```math
c_i=0
\quad\text{ou}\quad
S_i+c_i\le B
```

Em que **B** é o orçamento mensal de demolição configurado.

O valor predefinido do orçamento é de **§1.000**.

**Uma demolição com o preço de §0 não conta para este orçamento opcional.**

No entanto, há uma diferença importante entre esta regra e a proteção do tesouro.

**Se uma demolição com preço fizer exceder o orçamento mensal, toda a revisão de demolição pára.**

Não ignora esse edifício para continuar a procurar outros mais baratos.

Todos os candidatos restantes esperam pelo mês seguinte, incluindo quaisquer lotes posteriores que seriam gratuitos de demolir.

#### 3. Número máximo de ordens de demolição

O Ministério limita também o número de ordens de demolição numa revisão mensal.

```math
N\le
\begin{cases}
3, & \text{antes de qualquer ordem ser confirmada na sessão}\\
150, & \text{após a confirmação}
\end{cases}
```

O limite normal de 150 ordens é um valor técnico predefinido.

Pode ser ajustado através da chave de configuração:

`MaxPerRun`

Intervalo permitido: **1–1.000 ordens**.

Esta definição não está disponível na janela de opções do jogo.

O tesouro e o orçamento opcional impõem muitas vezes limites práticos mais baixos.

<a id="demolition-never"></a>
#### Quando é que o Ministério nunca demole um lote?

O Ministério deixa um lote intocado quando se verifica qualquer uma das situações seguintes:

- O Ministério está desativado, a funcionalidade de demolição em causa está desativada, ou a configuração de demolição está em falta ou é inválida.
- A cidade acabou de ser carregada e a primeira revisão mensal de demolição ainda não se realizou.
- O edifício está ocupado, em construção, à espera de desenvolvimento ou já está a ser demolido.
- É um edifício municipal ou histórico que não foi destruído.
- É uma ruína municipal ou histórica protegida e a opção especial das ruínas está desativada.
- Está abandonado num tipo de zona que foi excluído.
- Está abandonado por um motivo que foi excluído.
- Ainda não atingiu o período de espera configurado.
- O SimCity 4 indica que o lote não pode ser demolido.
- O jogo não consegue fornecer um preço de demolição válido.
- O tesouro não pode suportar a demolição.
- O orçamento mensal de demolição opcional impede a ordem.
- O lote mudou ou os seus limites no mapa não podem ser verificados.
- Uma falha de segurança da demolição parou a gestão na sessão atual.
- A versão do jogo não é suportada.

**Quando há dúvidas sobre a elegibilidade de um lote ou sobre a segurança da operação, o Ministério deixa-o em paz.**

<a id="demolition-safety"></a>
### 4. Condições de segurança

A demolição automática é uma operação potencialmente destrutiva, pelo que o Ministério faz várias verificações antes e depois de cada ordem.

#### Usar o sistema de demolição do próprio jogo

O Ministério usa o comando normal de demolição do SimCity 4.

Envia o retângulo exato ocupado pelo lote elegível e não inventa parâmetros especiais de demolição.

**Não remove diretamente os edifícios dos dados internos do jogo.**

#### Verificar o lote imediatamente antes da demolição

Antes de cada ordem, o Ministério lê o lote de novo.

O lote tem de:

- Continuar a existir na posição esperada.
- Continuar no estado esperado.
- Cumprir as regras de demolição.
- Ter as dimensões esperadas.

O retângulo do mapa tem de ter exatamente a área do lote e conter a sua célula de origem.

Se estas condições falharem, o lote é ignorado.

#### Garantir que a verificação do preço não altera nada

O Ministério pergunta ao jogo o preço da pré-visualização da demolição.

Esta pergunta não pode modificar o lote.

Se o lote mudar inesperadamente durante a verificação do preço, **a demolição automática pára durante o resto da sessão da cidade**.

#### Verificar que a demolição realmente aconteceu

As ordens de demolição não são necessariamente concluídas no momento exato em que são enviadas.

O Ministério verifica, por isso, o resultado na verificação de 10 dias seguinte ou na revisão mensal.

O lote tem de ter desaparecido.

Se **três ordens de demolição consecutivas não forem executadas pelo jogo**, a demolição automática pára durante o resto da sessão da cidade.

#### Preservar o zonamento

Depois de confirmar que o edifício desapareceu, o Ministério verifica o zonamento do quadrado de origem do lote.

**A zona original tem de continuar a existir.**

Se a demolição remover inesperadamente o zonamento, a demolição automática pára durante o resto da sessão.

Isto protege a disposição da cidade que o jogador pretendeu.

#### Tratamento de erros de configuração e erros técnicos

Se o ficheiro de configuração da demolição estiver em falta, ilegível ou inválido, o Ministério não faz nenhuma demolição automática.

Os erros durante uma revisão de demolição são intercetados e registados no registo. Não param o próprio SimCity 4.

**A demolição funciona de forma independente das outras funcionalidades do Ministério.**

Uma paragem de segurança da demolição não desativa a gestão da eletricidade, da água, do saneamento, da educação, da saúde nem dos contratos regionais.

Da mesma forma, uma falha noutro gestor não desativa automaticamente a demolição.

<a id="demolition-settings"></a>
### 5. Definições

A demolição automática configura-se na página **Demolição** da janela de opções do Ministério.

As definições aparecem pela seguinte ordem:

| Definição | Predefinição | O que controla |
|---|---|---|
| [Demolir edifícios destruídos](options.md#opt-demolishdestroyed) | Ligado | Limpa automaticamente as ruínas elegíveis. |
| [Esperar (ruínas)](options.md#opt-destroyedmonths) | 1 mês | Quanto tempo as ruínas têm de permanecer antes da demolição. |
| [Demolir edifícios municipais e históricos destruídos](options.md#opt-demolishmunicipal) | Desligado | Permite a demolição apenas das ruínas de edifícios protegidos. |
| [Demolir edifícios abandonados](options.md#opt-demolishabandoned) | Ligado | Limpa automaticamente os edifícios abandonados elegíveis. |
| [Esperar (abandonados)](options.md#opt-abandonedmonths) | 12 meses | Quanto tempo os edifícios têm de permanecer abandonados. |
| [Zona residencial](options.md#opt-zoneresidential) | Ligado | Inclui os edifícios residenciais abandonados. |
| [Zona comercial](options.md#opt-zonecommercial) | Ligado | Inclui os edifícios comerciais abandonados. |
| [Zona industrial](options.md#opt-zoneindustrial) | Ligado | Inclui os edifícios industriais abandonados. |
| [Zona agrícola](options.md#opt-zoneagriculture) | Desligado | Inclui as quintas e os campos abandonados. |
| [Motivo: zona pouco atraente](options.md#opt-reasonlowdesirability) | Ligado | Inclui o abandono causado por baixa atratividade. |
| [Motivo: trajeto demasiado longo](options.md#opt-reasoncommute) | Ligado | Inclui o abandono causado por trajetos longos. |
| [Motivo: falta de procura](options.md#opt-reasonlowdemand) | Ligado | Inclui o abandono causado por procura insuficiente. |
| [Motivo: sem eletricidade](options.md#opt-reasonnopower) | Desligado | Inclui o abandono causado pela falta de eletricidade. |
| [Motivo: sem água](options.md#opt-reasonnowater) | Desligado | Inclui o abandono causado pela falta de água. |
| [Limitar o orçamento mensal de demolição](options.md#opt-budgetlimit) | Desligado | Limita os gastos com demolições em cada revisão mensal. |
| [Orçamento mensal](options.md#opt-demolitionbudget) | §1.000 | Define o limite de gastos quando a opção de orçamento está ativada. |

Todas as definições de demolição automática dependem de [Ministério ativo](options.md#opt-master).

**Precauções recomendadas**

As definições predefinidas protegem os campos agrícolas, os edifícios abandonados por falhas temporárias de serviços e as estruturas municipais ou históricas.

Se quiser uma demolição mais agressiva, reveja estas proteções antes de alterar as opções, sobretudo em cidades com edifícios históricos valiosos ou grandes zonas agrícolas.

<a id="demolition-examples"></a>
### 6. Exemplos práticos

Estes exemplos mostram como o Ministério aplica os períodos de espera e os limites financeiros.

**Todos os preços de demolição abaixo são ilustrativos.** Os preços reais são obtidos ao SimCity 4 para cada lote individual.

#### Exemplo 1: Uma ruína atinge o período de espera

Suponha-se que o período de espera das ruínas é alterado para **3 meses**.

O Ministério guarda este valor como:

**91 dias simulados.**

Um edifício é destruído por um incêndio.

Numa revisão mensal, o jogo indica que as ruínas têm **85 dias**.

```math
85<91
```

O edifício é demasiado recente e é deixado em paz.

Na revisão mensal seguinte, cerca de 30 dias depois, as ruínas têm aproximadamente **115 dias**.

```math
115\ge91
```

O lote é agora elegível.

**O Ministério demole-o nessa revisão, se as verificações de elegibilidade e financeiras do jogo o permitirem.**

O período de espera é uma idade mínima, não uma marcação exata da demolição.

#### Exemplo 2: O orçamento mensal pára uma revisão de demolição

Suponha-se que a cidade tem:

- **Tesouro:** §20.000.
- **Orçamento mensal de demolição:** ativado.
- **Limite do orçamento:** §1.000.

O Ministério já confirmou uma demolição bem-sucedida na sessão atual, pelo que a precaução inicial das três ordens deixou de se aplicar.

Os candidatos são ordenados com as ruínas primeiro, seguidas dos edifícios abandonados.

| Ordem | Candidato | Preço | Gasto antes | Resultado do orçamento |
|---|---|---:|---:|---|
| 1 | Ruína A | §40 | §0 | Demolir |
| 2 | Ruína B | §0 | §40 | Demolir |
| 3 | Abandonado C | §300 | §40 | Demolir |
| 4 | Abandonado D | §450 | §340 | Demolir |
| 5 | Abandonado E | §700 | §790 | **Parar: excede o orçamento** |
| 6 | Abandonado F | §0 | — | Espera pelo mês seguinte |

Depois das quatro primeiras ordens, o Ministério comprometeu:

```math
40+0+300+450=790
```

**§790 em despesas de demolição.**

O edifício seguinte custa §700.

```math
790+700=1490>1000
```

Isto excederia o orçamento mensal configurado.

**O Ministério pára toda a revisão de demolição.**

Os edifícios E e F esperam pelo mês seguinte.

Repare-se que demolir F não custaria nada, mas é ainda assim adiado porque a regra do orçamento pára a revisão no primeiro candidato com preço que exceda o limite.

#### Exemplo 3: A regra do tesouro ignora um edifício caro

Considere-se agora uma cidade mais pequena com:

- **Tesouro:** §250.
- **Orçamento mensal de demolição:** desativado.

O Ministério avalia:

| Ordem | Candidato | Preço | Disponível antes | Decisão |
|---|---|---:|---:|---|
| 1 | Ruína A | §40 | §250 | Demolir |
| 2 | Ruína B | §60 | §210 | Demolir |
| 3 | Abandonado C | §300 | §150 | **Ignorar** |
| 4 | Abandonado D | §100 | §150 | Demolir |

Depois de ordenar A e B, o Ministério comprometeu §100.

O edifício C custa §300, o que a cidade não pode pagar.

**O Ministério ignora C mas continua a verificar os outros candidatos.**

O edifício D custa apenas §100 e ainda pode ser demolido.

As ordens resultantes custam:

```math
40+60+100=200
```

**São ordenados três edifícios, num total de §200.**

O edifício C permanece até uma revisão posterior em que haja dinheiro suficiente.

**A distinção é importante:**

- **Falha do tesouro:** ignorar o lote que não se pode pagar e continuar.
- **Falha do orçamento mensal:** parar a revisão por completo.

#### Exemplo 4: Instalar o Ministério numa cidade com muitos edifícios abandonados

Imagine-se uma cidade com **400 edifícios** que já estão abandonados há mais de um ano por procura insuficiente.

Instala o Ministério, liga a demolição automática e mantém as regras predefinidas.

Estes edifícios já têm idade suficiente para se qualificarem.

No entanto, **nada é demolido imediatamente quando se abre a cidade**.

Na primeira mudança de mês simulado, o Ministério envia no máximo **três ordens de demolição**.

Durante a verificação de 10 dias seguinte, confirma que o jogo executou essas ordens.

Assim que pelo menos uma demolição estiver confirmada, o limite normal fica disponível.

Na revisão mensal seguinte, o Ministério pode ordenar até **150 demolições**, desde que o tesouro e o orçamento mensal opcional o permitam.

Os edifícios elegíveis mais antigos são tratados primeiro.

**Esta precaução inicial das três ordens impede o Ministério de tentar imediatamente centenas de demolições antes de confirmar que o comando de demolição do jogo funciona corretamente.**

<a id="demolition-limitations"></a>
### 7. Limitações conhecidas

A demolição automática reduz o trabalho manual repetitivo, mas várias restrições são intencionais.

#### Calendário e elegibilidade

- **A demolição é mensal.** Um edifício é processado na primeira revisão mensal a seguir ao seu tempo de espera, o que pode significar até cerca de um mês simulado mais tarde.
- **A antiguidade do abandono já existente é preservada.** Os edifícios abandonados antes de o Ministério ser instalado mantêm a sua antiguidade original. Por isso, podem ser elegíveis logo na primeira revisão mensal.
- **Cada sessão de cidade começa de forma conservadora.** Enquanto nenhuma ordem de demolição tiver sido confirmada, cada revisão mensal envia no máximo três ordens.
- **O Ministério confia no motivo de abandono indicado pelo jogo.** Não prevê se um edifício poderá voltar a ser ocupado. O tempo de espera e as opções de motivo dão essa proteção.

#### Custos de demolição

- **É o jogo que determina os preços da demolição.** O Ministério lê o preço real antes de cada ordem, mas não mostra uma notificação de custo à parte.
- **As despesas de demolição aparecem como custos normais de demolição com a escavadora.** Em geral, limpar ruínas é mais barato do que limpar edifícios abandonados, mas os preços reais dependem do lote.
- **O orçamento mensal aplica-se a uma só revisão de demolição.** Quando está ativado, interrompe a revisão no primeiro candidato com preço conhecido que ultrapassaria o limite.

#### Edifícios protegidos

- **Os edifícios municipais e históricos abandonados nunca são demolidos.** Só as suas ruínas podem ser removidas, e apenas quando a opção de proteção própria está ativada.
- **Os campos agrícolas podem ser afetados.** Ativar Zona agrícola permite a demolição de lotes de campos abandonados elegíveis. Use esta opção com cuidado.
- **O Ministério nunca remove deliberadamente o zonamento subjacente.** O terreno continua disponível para ser reconstruído.

#### Limites técnicos

- **As paragens de segurança duram até ao fim da sessão da cidade em curso.** Voltar a abrir a cidade permite à gestão da demolição começar uma nova sessão.
- **Só a cidade atualmente aberta é gerida.** O Ministério não inspeciona nem demole edifícios das cidades vizinhas.
- **A demolição automática exige a versão suportada do jogo.** As versões não suportadas não são modificadas.

**O objetivo do Ministério é limpar ruínas elegíveis e edifícios abandonados há muito tempo, e não tomar decisões de reconstrução no lugar do jogador.**

---

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
