[English](../en/options.md) | **Português** | [Français](../fr/options.md) | [Español](../es/options.md)

[Voltar ao índice](index.md#contents) | [Voltar ao README](../../README.pt.md)

<a id="options"></a>
## H. Referência das opções

Este capítulo explica como abrir as definições do Ministério e como as alterações de configuração são guardadas. As descrições detalhadas de cada opção encontram-se nas secções seguintes.

<a id="options-open"></a>
### Abrir a janela de opções

Para aceder às definições do Ministério:

1. Abra **Opções de Jogo** no SimCity 4, quer durante uma cidade quer a partir da vista da região.
2. Clique em **Ministério da Optimização**, o botão acrescentado por baixo do botão original **Créditos**.
3. A janela Opções de Jogo fecha-se e abre-se a janela de definições do Ministério, no centro do ecrã.

A janela de definições tem duas páginas:

| Página | O que contém |
|---|---|
| **Serviços** | Financiamento automático, margens de reserva, contratos regionais e avisos sobre o tamanho do aterro. |
| **Demolição** | Regras de demolição automática, tempos de espera, tipos de lote protegidos e limites de despesa com demolições. |

Cada definição aparece como uma linha com nome, com uma caixa de seleção ou um controlo numérico.

**Passe o rato sobre o nome ou a caixa de seleção de uma definição para ver a sua dica.**

#### Porque é que algumas definições estão a cinzento?

Algumas definições dependem de outra opção estar ligada.

Por exemplo, o **Limiar de poupança dos avisos do aterro** não pode ser ajustado enquanto os avisos sobre o tamanho do aterro estiverem desligados.

Quando uma opção principal está desligada:

- As definições que dependem dela ficam a cinzento.
- Não é possível alterá-las.
- Os valores que já tinham são preservados.
- Voltam a ficar disponíveis quando a opção principal é ligada.

**Desligar uma funcionalidade não apaga as definições anteriores.**

Atualmente, cada opção tem no máximo uma dependência de uma opção principal.

<a id="options-accept"></a>
### Aceitar e Cancelar

A janela de definições oferece duas ações.

#### Aceitar

Clicar em **Aceitar** guarda as alterações.

O Ministério:

1. Identifica as definições que alterou.
2. Atualiza apenas essas definições nos ficheiros de configuração respetivos.
3. Preserva as definições não relacionadas e os comentários existentes.
4. Verifica se os ficheiros atualizados podem ser lidos de volta com exatamente os valores escolhidos.
5. Substitui cada ficheiro de configuração como uma operação completa, só depois de a validação ter êxito.
6. Aplica as novas definições de imediato.

**Se a validação falhar, nenhuma alteração é escrita e a janela de definições continua aberta.**

Isto evita que uma configuração inválida seja guardada parcialmente.

#### Cancelar

Clicar em **Cancelar** fecha a janela de definições sem guardar nenhuma alteração.

Fechar a cidade ou sair do SimCity 4 também fecha a janela do Ministério, sem escrever as definições que não foram guardadas.

#### A lei municipal do Ministério

O jogo contém também a lei municipal:

**Instituir o Ministério da Optimização**

Aparece na janela normal de leis municipais do SimCity 4 e tem um custo mensal de **§0**.

Esta lei controla a mesma definição que **Ministério ativo**.

- **Promulgar a lei** liga o Ministério.
- **Revogar a lei** desliga-o.

Os dois métodos atualizam a mesma definição, por isso mantêm-se coerentes.

**Pode ligar ou desligar o Ministério a partir de qualquer dos dois locais sem alterar as preferências de cada funcionalidade que já configurou.**
<a id="options-files"></a>
### Onde são guardadas as definições?

O Ministério guarda as suas definições em **três ficheiros de configuração de texto**, situados na pasta `Department of Efficiency` dentro da pasta Plugins do SimCity 4.

Pode alterar a maior parte das definições através da janela de opções do Ministério, dentro do jogo, ou editar os ficheiros manualmente.

| Ficheiro de configuração | O que controla |
|---|---|
| `DepartmentEfficiency.ini` | Financiamento da educação e da saúde, margens de segurança e o interruptor principal do Ministério. |
| `DepartmentEfficiencyUtility.ini` | Eletricidade, água, saneamento, avisos sobre o aterro, contratos regionais e respetivas opções. |
| `DepartmentEfficiencyDemolition.ini` | Demolição automática, tempos de espera, zonas elegíveis, motivos de abandono e despesa com demolições. |

Cada ficheiro tem a sua própria secção de configuração.

#### Educação e saúde

Ficheiro: `DepartmentEfficiency.ini`

Secção: `[DepartmentEfficiency]`

| Definição | Chave INI |
|---|---|
| Gerir a educação | `Education` |
| Gerir a saúde | `Health` |
| Margem de segurança (educação) | `EducationReservePercent` |
| Margem de segurança (saúde) | `HealthReservePercent` |
| Ministério ativo | `Mode` |

A configuração fornecida usa `ReservePercent=5` para as margens de segurança da educação e da saúde.

#### Serviços públicos e contratos regionais

Ficheiro: `DepartmentEfficiencyUtility.ini`

Secção: `[DepartmentEfficiencyUtility]`

| Definição | Chave INI |
|---|---|
| Gerir a eletricidade | `AutoPower` |
| Reserva de eletricidade | `PowerReservePercent` |
| Ter em conta o desgaste das centrais | `ConsiderPowerAging` |
| Gerir a água | `AutoWater` |
| Reserva de água | `WaterReservePercent` |
| Proteger as canalizações | `ProtectWaterPipes` |
| Gerir o saneamento | `AutoGarbage` |
| Reserva de saneamento | `GarbageReservePercent` |
| Avisar-me quando rever o tamanho do aterro compensar | `WarnLandfillSizeSavings` |
| Limiar de poupança dos avisos do aterro | `LandfillSavingsAlertThreshold` |
| Gerir os acordos com os vizinhos | `AutoDeals` |
| Comprar a uma cidade para vender a outra | `AllowUtilityArbitrage` |
| Gerir acordos de eletricidade | `AutoTradePower` |
| Gerir acordos de água | `AutoTradeWater` |
| Gerir acordos de lixo | `AutoTradeGarbage` |
| Ministério ativo | `Mode` |

#### Demolição automática

Ficheiro: `DepartmentEfficiencyDemolition.ini`

Secção: `[LotDemolition]`

| Definição | Chave INI |
|---|---|
| Demolir edifícios destruídos | `DemolishDestroyed` |
| Esperar (ruínas) | `DestroyedAfterDays` |
| Demolir edifícios municipais e históricos destruídos | `DemolishMunicipalAndHistoricRuins` |
| Demolir edifícios abandonados | `DemolishAbandoned` |
| Esperar (abandonados) | `AbandonedAfterDays` |
| Tipos de zona elegíveis | `AbandonedZones` |
| Motivos de abandono elegíveis | `AbandonedReasons` |
| Limitar o orçamento mensal de demolição | `LimitMonthlyBudget` |
| Orçamento mensal | `MonthlyBudget` |
| Ministério ativo | `Mode` |

#### Posso editar estes ficheiros manualmente?

Sim, mas **os valores de configuração têm de respeitar o formato suportado e os intervalos permitidos**.

O Ministério verifica cada ficheiro antes de o usar.

Uma definição desconhecida ou um valor inválido tornam inválido o ficheiro de configuração afetado, desativando essa parte do Ministério em vez de aplicar definições incertas.

As definições omitidas nos ficheiros fornecidos usam os seus valores predefinidos.

Por exemplo, a configuração de demolição predefinida liga a remoção de ruínas e de edifícios abandonados elegíveis, protegendo ao mesmo tempo os lotes agrícolas e os edifícios abandonados por falta de eletricidade ou de água.

**As edições manuais são normalmente detetadas e aplicadas no prazo de um mês simulado.**

Para alterações normais, a janela de opções dentro do jogo é mais segura, porque verifica os valores permitidos antes de os guardar.

---

<a id="options-services"></a>
### Página Serviços

A página **Serviços** contém o financiamento automático, as reservas de segurança, os contratos regionais e os avisos sobre o aterro.

A primeira opção controla todo o Ministério. As restantes permitem decidir que serviços deve gerir.

<a id="opt-master"></a>
### Ministério ativo

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `Mode`</text>

**Este é o interruptor principal de todo o Ministério.**

Quando está ligado, cada funcionalidade automática segue a sua própria definição.

Por exemplo, pode deixar o Ministério ativo e desligar a gestão da água ou a demolição automática.

Quando está desligado, o Ministério deixa de:

- Ajustar o financiamento da educação, da saúde e dos serviços públicos.
- Criar, alterar ou cancelar contratos regionais.
- Demolir edifícios e ruínas.
- Reparar quadrados de aterro fantasma.
- Calcular recomendações sobre o tamanho do aterro e avisos de falta.

**Desligar o Ministério não anula as alterações que já fez.**

Os níveis de financiamento e os contratos regionais ficam como estavam.

As suas preferências individuais também são preservadas, por isso voltam a ficar ativas quando voltar a ligar o Ministério.

Todas as outras definições das páginas Serviços e Demolição ficam indisponíveis enquanto o interruptor principal estiver desligado.

**Importante:** Os ficheiros de recursos de conselhos incluídos e a modificação que esconde os ícones de *sem água* agrícolas continuam a fazer efeito enquanto estiverem instalados, mesmo com o Ministério desligado.

Pode também controlar o mesmo interruptor através de **Instituir o Ministério da Optimização**, na janela de leis municipais do SimCity 4.

A definição é guardada como `Mode=Auto` ou `Mode=Off` em cada um dos três ficheiros de configuração.

A educação e a saúde funcionam quando pelo menos uma das respetivas opções de gestão está ligada. A demolição funciona quando pelo menos uma categoria de demolição está ligada. O controlador dos serviços públicos continua disponível enquanto o Ministério estiver ativo, mesmo que nenhum dos serviços esteja selecionado.

Consulte [Segurança, automação e tomada de decisões](safety.md#safety-player-control).

<a id="opt-education"></a>
### Gerir a educação

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `Education`</text>

Ajusta automaticamente o financiamento dos equipamentos de educação elegíveis, incluindo escolas, faculdades, universidades, bibliotecas e museus.

**O Ministério escolhe a menor percentagem de financiamento, em número inteiro, que consegue servir os utilizadores do equipamento mantendo a margem de segurança exigida.**

Quando está ligado:

- O financiamento pode aumentar durante o primeiro dia simulado após a abertura de uma cidade.
- O Ministério pode aumentar o financiamento quando deteta um risco de capacidade.
- As reduções de financiamento são avaliadas na revisão mensal.
- Uma sobrelotação persistente que o financiamento não resolva pode gerar um aviso do conselheiro.

Quando está desligado, o financiamento da educação mantém os valores que definiu manualmente e os avisos de capacidade da educação ficam desativados.

**Importante:** Os edifícios que não suportam o mecanismo de financiamento normal do jogo não são modificados. Os conselhos originais do jogo sobre o financiamento da educação continuam suprimidos enquanto o ficheiro de recursos de conselhos cívicos estiver instalado, mesmo que a gestão automática da educação esteja desligada.

**Requer:** Ministério ativo.

**Controla:** a [Margem de segurança](#opt-educationreserve) da educação.

Consulte [Educação e saúde](education_healthcare.md#education-healthcare).

<a id="opt-educationreserve"></a>
### Margem de segurança (educação)

<text color="secondary">Predefinição: 5% | Intervalo: 0–50%, em passos de 1% | INI: `EducationReservePercent`</text>

Determina a capacidade adicional que o Ministério mantém acima da utilização atual de cada edifício de educação.

Por exemplo, uma escola que serve 1.000 alunos com uma margem de 5% tem uma necessidade básica protegida de:

```math
1000\times1.05=1050
```

**Pelo menos 1.050 lugares.**

Uma margem maior dá mais espaço para o crescimento e para aumentos inesperados da frequência, mas pode exigir mais financiamento.

Uma margem menor reduz as despesas, ao deixar a capacidade ficar mais perto da utilização atual.

**A margem de segurança é um mínimo, não um objetivo exato.**

O Ministério pode manter capacidade adicional para edifícios pequenos ou equipamentos cuja utilização oscila muito, ajudando a evitar greves.

**Requer:** Gerir a educação e Ministério ativo.

Consulte [Educação e saúde](education_healthcare.md#education-healthcare-required).

<a id="opt-health"></a>
### Gerir a saúde

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `Health`</text>

Gere automaticamente o financiamento das clínicas e dos hospitais elegíveis.

O Ministério seleciona a menor percentagem de financiamento, em número inteiro, que oferece capacidade suficiente para os doentes atuais e para a margem de segurança da saúde configurada.

Quando está ligado:

- Os aumentos de financiamento podem ocorrer de imediato quando é detetado um risco de capacidade.
- As reduções são consideradas nas revisões mensais.
- Faltas de capacidade persistentes que o financiamento máximo não resolva podem gerar avisos do conselheiro.

Quando está desligado, o financiamento da saúde fica sob o seu controlo manual e não são gerados avisos de capacidade da saúde.

**O Ministério nunca altera a linha de orçamento separada das ambulâncias.**

Os edifícios que não suportam o mecanismo de financiamento normal ficam excluídos.

Os conselhos originais do jogo sobre o financiamento normal da saúde continuam suprimidos enquanto o ficheiro de recursos de conselhos cívicos estiver instalado, independentemente desta definição.

**Requer:** Ministério ativo.

**Controla:** a [Margem de segurança](#opt-healthreserve) da saúde.

Consulte [Educação e saúde](education_healthcare.md#education-healthcare).

<a id="opt-healthreserve"></a>
### Margem de segurança (saúde)

<text color="secondary">Predefinição: 5% | Intervalo: 0–50%, em passos de 1% | INI: `HealthReservePercent`</text>

Determina a capacidade adicional de doentes mantida por cada equipamento de saúde gerido.

Por exemplo, um hospital que atende atualmente 2.000 doentes com uma margem de segurança de 5% exige pelo menos:

```math
2000\times1.05=2100
```

**2.100 lugares de capacidade.**

Aumentar a margem dá mais espaço para o crescimento e as flutuações do número de doentes, mas pode aumentar as despesas de financiamento.

Reduzi-la permite um financiamento menor quando a utilização real é estável.

**Tal como na educação, trata-se de um requisito mínimo.** O Ministério pode manter mais capacidade quando os seus cálculos de segurança o exigirem.

**Requer:** Gerir a saúde e Ministério ativo.

Consulte [Educação e saúde](education_healthcare.md#education-healthcare-required).

<a id="opt-power"></a>
### Gerir a eletricidade

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `AutoPower`</text>

Ajusta automaticamente o financiamento de cada central elétrica.

Em vez de financiar todas as centrais com a mesma percentagem, o Ministério procura **a combinação mais barata de níveis de financiamento das centrais** que consiga fornecer:

- As necessidades de eletricidade da cidade.
- A reserva de eletricidade configurada.
- Quaisquer vendas de eletricidade ativas às cidades vizinhas.

Considera também as compras regionais de eletricidade disponíveis.

Quando está ligado, uma revisão completa realiza-se normalmente a cada **10 dias simulados**, com avaliações mais cedo em caso de défices de fornecimento confirmados ou de alterações significativas da procura ou da capacidade.

Quando está desligado, o financiamento das centrais mantém os valores atuais.

**Os contratos de eletricidade são controlados em separado.** Podem continuar a ser geridos se Gerir acordos de eletricidade estiver ligado.

O Ministério só altera o financiamento. Nunca constrói centrais elétricas adicionais.

Se as necessidades de eletricidade da cidade não puderem ser satisfeitas com os equipamentos e contratos disponíveis, uma falta persistente pode produzir um aviso do conselheiro.

**Requer:** Ministério ativo.

**Controla:** a [Reserva de eletricidade](#opt-powerreserve) e [Ter em conta o desgaste das centrais](#opt-poweraging).

Consulte [Eletricidade](power.md#power).

<a id="opt-powerreserve"></a>
### Reserva de eletricidade

<text color="secondary">Predefinição: 20% | Intervalo: 10–50%, em passos de 1% | INI: `PowerReservePercent`</text>

Determina a capacidade de produção de eletricidade adicional que o Ministério protege contra variações da procura.

Sem compras nem vendas, a necessidade local protegida é pelo menos a maior de:

- A necessidade de eletricidade do próprio jogo.
- A procura de eletricidade multiplicada por **1 + reserva**.

Por exemplo, com uma reserva de 20%, uma cidade com uma procura de 10.000 unidades precisa normalmente de pelo menos 12.000 unidades de capacidade de produção, a não ser que a necessidade do próprio jogo seja maior.

Uma reserva maior oferece melhor proteção contra aumentos súbitos da procura de eletricidade, mas pode aumentar as despesas de funcionamento.

Uma reserva menor reduz os custos, deixando menos capacidade de reserva antes de uma falta se tornar possível.

**A reserva é um mínimo, não um limite para a produção.**

O Ministério pode manter capacidade adicional quando vendas rentáveis de eletricidade o justificarem.

**Requer:** Gerir a eletricidade e Ministério ativo.

Consulte [Eletricidade](power.md#power).

<a id="opt-poweraging"></a>
### Ter em conta o desgaste das centrais

<text color="secondary">Predefinição: Desligado | Valores: Ligado / Desligado | INI: `ConsiderPowerAging`</text>

Permite ao Ministério considerar o **desgaste e o envelhecimento das centrais** ao comparar arranjos de financiamento da eletricidade.

Quando está desligado, o optimizador concentra-se nas despesas mensais de funcionamento.

Quando está ligado, o cálculo considera também o custo económico do desgaste das centrais.

Por vezes, isto pode compensar gastar um pouco mais por mês para reduzir o desgaste e prolongar a vida útil de uma central.

**Este desgaste estimado é um custo económico, não um encargo mensal adicional para a sua tesouraria.**

As necessidades de eletricidade da cidade e a reserva configurada ficam protegidas nos dois modos.

Se Gerir a eletricidade estiver desligado, a preferência sobre o desgaste é preservada, mas não tem efeito até a gestão da eletricidade voltar a ser ligada.

**Requer:** Gerir a eletricidade e Ministério ativo.

Consulte [Eletricidade](power.md#power).

<a id="opt-water"></a>
### Gerir a água

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `AutoWater`</text>

Gere automaticamente o financiamento do departamento de águas.

Ao contrário da eletricidade, a água usa **um único cursor de financiamento do departamento** para as suas bombas e estações de tratamento.

O Ministério escolhe um nível de financiamento que ofereça capacidade de água suficiente para a procura da cidade e para a reserva configurada, tendo em conta os contratos regionais.

Quando está ligado, o financiamento da água é revisto:

- A cada 10 dias simulados desde a avaliação anterior.
- A cada mudança de mês simulado.
- Mais cedo quando é detetada uma falta regional ou uma alteração significativa.

Quando está desligado, o financiamento da água não se altera.

Os contratos regionais de água podem continuar a ser geridos de forma independente através de Gerir acordos de água.

**O Ministério não reduz o financiamento da água enquanto houver edifícios sem água, nas condições de segurança aplicáveis ao gestor da água.**

**Requer:** Ministério ativo.

**Controla:** a [Reserva de água](#opt-waterreserve) e [Proteger as canalizações](#opt-waterpipes).

Consulte [Água](water.md#water).

<a id="opt-waterreserve"></a>
### Reserva de água

<text color="secondary">Predefinição: 10% | Intervalo: 10–50%, em passos de 1% | INI: `WaterReservePercent`</text>

Determina a capacidade de água adicional que o Ministério mantém acima da procura da cidade.

Por exemplo, com uma reserva de 10%, uma cidade com uma procura de 50.000 unidades de água exige pelo menos:

```math
50000\times1.10=55000
```

**55.000 unidades de capacidade protegida**, antes de considerar os efeitos detalhados dos contratos.

Uma reserva maior oferece mais proteção contra o aumento da procura de água, mas pode aumentar as despesas.

Uma reserva menor permite um funcionamento mais barato com menos capacidade de reserva.

**A reserva é um requisito mínimo.** O Ministério pode manter mais capacidade quando for necessário.

**Requer:** Gerir a água e Ministério ativo.

Consulte [Água](water.md#water-required-capacity).

<a id="opt-waterpipes"></a>
### Proteger as canalizações

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `ProtectWaterPipes`</text>

Protege a rede de distribuição de água contra a deterioração causada por financiamento insuficiente.

No SimCity 4, manter o departamento de águas abaixo de certos níveis de financiamento pode contribuir para danos nas canalizações.

Quando esta opção está ligada, o Ministério:

- Nunca reduz o financiamento da água abaixo de **50%**.
- Não reduz o financiamento enquanto a rede de canalizações estiver danificada.
- Evita os níveis de financiamento em que a curva de manutenção das canalizações do jogo permite a deterioração da rede.

**O objetivo é poupar dinheiro sem deixar a rede de água degradar-se por falta de financiamento.**

Quando está desligada, as restrições especiais de proteção das canalizações são retiradas.

O financiamento da água continua sujeito aos requisitos normais de serviço e de reserva, mas o Ministério pode escolher níveis abaixo de 50% quando isso for permitido de outra forma.

Isto pode poupar dinheiro, **com o risco de as canalizações se deteriorarem ou rebentarem**.

A opção não aumenta por si só o financiamento apenas para reparar danos já existentes nas canalizações; a sua proteção serve sobretudo para impedir reduções inseguras.

Se Gerir a água estiver desligado, esta preferência fica guardada mas inativa.

**Requer:** Gerir a água e Ministério ativo.

Consulte [Água](water.md#water-pipe-curve).

<a id="opt-sanitation"></a>
### Gerir o saneamento

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `AutoGarbage`</text>

Ajusta automaticamente o financiamento do departamento de saneamento.

O saneamento abrange o sistema de tratamento do lixo da cidade, incluindo aterros e instalações de tratamento, através de um único cursor de financiamento do departamento.

O Ministério considera:

- O lixo produzido depois da reciclagem.
- A reserva de saneamento configurada.
- O lixo que já ficou por recolher.
- As importações e exportações regionais de lixo.
- A capacidade de tratamento das incineradoras e das centrais de conversão de lixo em energia.
- O armazenamento disponível nos aterros e a entrada mensal.

Depois procura o nível de financiamento mais barato que consiga tratar o lixo previsto.

**Ao contrário da eletricidade e da água, as reduções do financiamento do saneamento são graduais.**

As reduções ficam limitadas a **25 pontos percentuais por escrita**, com pelo menos **7 dias simulados** entre cortes.

Os aumentos de financiamento exigem pelo menos **3 dias simulados** desde a escrita anterior do financiamento do saneamento.

O Ministério revê o saneamento nas mudanças de mês e quando as estatísticas do lixo mudam de forma significativa.

Quando está ligado, pode também efetuar reparações automáticas seguras de quadrados de aterro fantasma.

Quando está desligado:

- O financiamento do saneamento não se altera.
- A reparação de quadrados fantasma pára.
- Os avisos sobre o tamanho do aterro ficam indisponíveis.
- Os contratos de lixo podem continuar a ser geridos de forma independente se Gerir acordos de lixo estiver ligado.

**O Ministério nunca constrói nem zona automaticamente áreas de aterro adicionais.** A ampliação e a redução do aterro são apenas conselhos.

**Requer:** Ministério ativo.

**Controla:** a [Reserva de saneamento](#opt-sanitationreserve) e [Avisar-me quando rever o tamanho do aterro compensar](#opt-landfilladvice).

Consulte [Lixo e aterros](garbage.md#garbage-landfills).

<a id="opt-sanitationreserve"></a>
### Reserva de saneamento

<text color="secondary">Predefinição: 2% | Intervalo: 2–20%, em passos de 1% | INI: `GarbageReservePercent`</text>

Acrescenta uma margem de segurança à quantidade de lixo que a cidade produz depois da reciclagem.

A quantidade de lixo protegida calcula-se a partir de:

```math
\text{Lixo protegido}=
(\text{Produzido}-\text{Reciclado})
\times(1+\text{reserva})
```

O cálculo de planeamento real inclui também qualquer acumulação de lixo por recolher e os efeitos dos contratos regionais.

Por exemplo, uma cidade que produz 80.000 unidades de lixo depois da reciclagem tem uma necessidade básica protegida de **81.600 unidades** com a reserva predefinida de 2%.

Uma margem maior oferece proteção adicional contra aumentos da produção de lixo, mas pode exigir um financiamento do saneamento mais elevado.

Uma margem menor reduz as despesas possíveis.

**A reserva é um mínimo, não um objetivo máximo de tratamento.**

**Requer:** Gerir o saneamento e Ministério ativo.

Consulte [Lixo e aterros](garbage.md#garbage-landfills-need).

<a id="opt-landfilladvice"></a>
### Avisar-me quando rever o tamanho do aterro compensar

<text color="secondary">Predefinição: Desligado | Valores: Ligado / Desligado | INI: `WarnLandfillSizeSavings`</text>

Ativa recomendações opcionais sobre a alteração do tamanho do aterro.

O Ministério procura situações em que:

- **Ampliar um aterro** proporcionaria capacidade de tratamento adicional suficiente para reduzir o financiamento de saneamento necessário e a despesa mensal total.
- **Reduzir um aterro** eliminaria quadrados não utilizados e baixaria as despesas de manutenção sem comprometer o tratamento do lixo.

Se uma alteração segura produzir uma poupança acima do limiar que configurou, o Ministério emite um aviso do conselheiro.

O aviso identifica a área proposta, estima a poupança mensal e inclui uma ligação que leva a câmara até ao local.

**Esta funcionalidade limita-se a dar conselhos.**

Não:

- Zona nem retira o zonamento a terrenos.
- Constrói nem demole nada.
- Altera o financiamento por causa do conselho.
- Altera os contratos regionais por causa do conselho.

A análise é feita **no máximo uma vez por mês simulado**, sujeita aos períodos de proteção adicionais descritos no capítulo do aterro.

Só funciona em cidades com **exatamente uma zona de aterro**.

Quando está desligada, não é feita nenhuma análise do tamanho do aterro nem emitido nenhum aviso.

O histórico de tempos existente mantém-se, por isso desligar e voltar a ligar a opção não reinicia os respetivos períodos de espera.

**Requer:** Gerir o saneamento e Ministério ativo.

**Controla:** o [Limiar de poupança dos avisos do aterro](#opt-landfillthreshold).

Consulte [Lixo e aterros](garbage.md#garbage-landfills-advice).

<a id="opt-landfillthreshold"></a>
### Limiar de poupança dos avisos do aterro

<text color="secondary">Predefinição: 25% | Intervalo: 20–50%, em passos de 1% | INI: `LandfillSavingsAlertThreshold`</text>

Determina quanto uma alteração proposta ao tamanho do aterro tem de poupar antes de o Ministério a recomendar.

A comparação usa a despesa mensal de saneamento optimizada, incluindo a manutenção.

**Limiar de aviso**

```math
\text{Poupança}\times100>
\text{Limiar}\times\text{Despesa mensal}
```

**A comparação é estrita.**

Por exemplo, com uma despesa de saneamento optimizada atual de §1.000 e um limiar de 25%:

- Poupança de §200: sem aviso.
- Poupança de §250: sem aviso.
- Poupança de §251: pode ser emitido um aviso.

Um limiar mais alto significa menos recomendações, limitadas a poupanças potenciais maiores.

Um limiar mais baixo permite que sejam comunicadas oportunidades de poupança menores.

**O aviso mostra a poupança efetivamente calculada, não o limiar que selecionou.**

Alterar esta definição não reinicia os períodos de espera dos avisos.

Quando os avisos sobre o tamanho do aterro estão desligados, o controlo do limiar fica indisponível, mas o seu valor guardado é preservado.

**Requer:** Avisar-me quando rever o tamanho do aterro compensar, Gerir o saneamento e Ministério ativo.

Consulte [Lixo e aterros](garbage.md#garbage-landfills-advice).

<a id="opt-deals"></a>
### Gerir os acordos com os vizinhos

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `AutoDeals`</text>

Este é o **interruptor principal da gestão automática dos contratos regionais**.

Quando está ligado, o Ministério pode examinar as ofertas das cidades vizinhas e gerir os contratos de eletricidade, água e lixo.

Cada recurso é controlado pela sua própria opção.

O Ministério pode:

- Comprar recursos quando são mais baratos do que a produção local.
- Vender capacidade verdadeiramente excedentária quando é rentável.
- Ajustar as quantidades dos contratos existentes.
- Cancelar contratos que já não trazem benefício.

**Os serviços da própria cidade e as reservas de segurança têm sempre prioridade.**

Quando está desligado:

- Os contratos regionais existentes mantêm-se.
- Esses contratos continuam incluídos nos cálculos de financiamento.
- Não são pedidas novas ofertas de contrato.
- Nenhum contrato é criado, redimensionado ou cancelado.
- Não é reservada capacidade local para vendas futuras hipotéticas.
- Os avisos de falta de serviços públicos não podem ser totalmente comprovados, porque as ofertas regionais não são examinadas.

**Importante:** As mensagens originais dos conselheiros do SimCity 4 sobre acordos com os vizinhos continuam suprimidas enquanto o ficheiro de recursos de conselhos sobre acordos do Ministério estiver instalado.

Para restaurar essas mensagens originais depois de desligar a gestão automática de contratos, remova o ficheiro de recursos em causa com o jogo fechado.

**Requer:** Ministério ativo.

**Controla:** [Comprar a uma cidade para vender a outra](#opt-arbitrage), [Gerir acordos de eletricidade](#opt-tradepower), [Gerir acordos de água](#opt-tradewater) e [Gerir acordos de lixo](#opt-tradegarbage).

Consulte [Acordos com os vizinhos](deals.md#neighbor-deals).

<a id="opt-arbitrage"></a>
### Comprar a uma cidade para vender a outra

<text color="secondary">Predefinição: Desligado | Valores: Ligado / Desligado | INI: `AllowUtilityArbitrage`</text>

Permite ao Ministério **comprar e vender o mesmo recurso no mesmo plano de contratos** quando essa combinação produz um melhor resultado económico.

Com esta opção desligada, cada recurso segue um de três arranjos:

- **LOCAL:** sem compras nem vendas regionais.
- **BUY (comprar):** compra o recurso às cidades vizinhas.
- **SELL (vender):** vende o recurso às cidades vizinhas.

Com a opção ligada, o Ministério pode combinar compras e vendas.

Por exemplo, pode comprar eletricidade a um vizinho para cobrir parte da procura da cidade, libertando eletricidade produzida localmente para uma venda rentável a outro vizinho.

**O Ministério nunca revende diretamente recursos importados.**

As compras continuam limitadas pela procura da própria cidade, e as regras normais de serviço e de reserva continuam a aplicar-se.

No caso do lixo, um arranjo misto pode consistir em receber lixo pago de um vizinho e pagar a outro vizinho para tratar parte dos seus próprios resíduos.

Um recurso cuja opção individual de gestão de acordos esteja desligada nunca é negociado pelo Ministério, independentemente desta definição.

**Requer:** Gerir os acordos com os vizinhos e Ministério ativo.

Consulte [Acordos com os vizinhos](deals.md#neighbor-deals-arbitrage).

<a id="opt-tradepower"></a>
### Gerir acordos de eletricidade

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `AutoTradePower`</text>

Permite ao Ministério criar, redimensionar e cancelar contratos de eletricidade com as cidades vizinhas.

Quando está ligado, pode:

- Comprar eletricidade quando compensa economicamente.
- Vender eletricidade excedentária quando é rentável.
- Reduzir ou cancelar vendas que ameacem o fornecimento da própria cidade.
- Ajustar os contratos existentes à medida que a procura, a produção e os preços mudam.

Quando está desligado, os contratos de eletricidade existentes ficam sob o seu controlo manual.

O Ministério continua a lê-los ao calcular as necessidades locais de eletricidade, mas não os modifica nem reserva capacidade para futuras vendas automáticas.

**Esta definição controla contratos, não o financiamento das centrais elétricas.**

Funciona de forma independente de **Gerir a eletricidade**.

Por exemplo, pode gerir os contratos de eletricidade automaticamente e controlar por si o financiamento das centrais. Nesse caso, o Ministério tem de trabalhar com a capacidade de produção já disponível.

**Requer:** Gerir os acordos com os vizinhos e Ministério ativo.

Consulte [Acordos com os vizinhos](deals.md#neighbor-deals).

<a id="opt-tradewater"></a>
### Gerir acordos de água

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `AutoTradeWater`</text>

Permite ao Ministério gerir compras e vendas de água com as cidades vizinhas.

Quando está ligado, compara os preços dos contratos com o custo de fornecer água localmente.

Pode criar, redimensionar ou cancelar contratos quando isso melhora a situação económica da cidade sem comprometer o serviço de água e a reserva configurada.

Quando está desligado, os contratos de água existentes ficam sob o seu controlo manual.

**Esta opção não controla o financiamento do departamento de águas.**

É independente de **Gerir a água**.

Por exemplo, pode deixar o Ministério optimizar o financiamento da água e manter inalterados os seus próprios contratos regionais de água.

**Requer:** Gerir os acordos com os vizinhos e Ministério ativo.

Consulte [Acordos com os vizinhos](deals.md#neighbor-deals).

<a id="opt-tradegarbage"></a>
### Gerir acordos de lixo

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `AutoTradeGarbage`</text>

Permite ao Ministério gerir os contratos de lixo nos dois sentidos.

Lembre-se de que os contratos de lixo funcionam de modo diferente dos de eletricidade e de água:

- **Enviar lixo para um vizinho:** a sua cidade paga pelo tratamento.
- **Receber lixo de um vizinho:** o vizinho paga à sua cidade para tratar os seus resíduos.

Quando está ligado, o Ministério pode criar, redimensionar e cancelar estes contratos de acordo com o seu valor económico e com a capacidade de tratamento da cidade.

**As necessidades de tratamento de lixo da própria cidade vêm sempre em primeiro lugar.**

O Ministério não aceita resíduos adicionais só porque outra cidade oferece dinheiro.

Tem de conseguir tratar esse lixo em segurança com as instalações de saneamento e o financiamento disponíveis.

Quando está desligado, os contratos de lixo existentes ficam sob o seu controlo manual.

**Esta opção é independente de Gerir o saneamento.**

Pode gerir os contratos de lixo sem alterar automaticamente o financiamento do saneamento, ou gerir o financiamento do saneamento deixando os contratos inalterados.

**Requer:** Gerir os acordos com os vizinhos e Ministério ativo.

Consulte [Acordos com os vizinhos](deals.md#neighbor-deals-garbage).

---

<a id="options-demolition"></a>
### Página Demolição

A página **Demolição** controla a forma como o Ministério limpa os edifícios destruídos e os lotes abandonados há muito tempo.

Pode decidir:

- Se as ruínas devem ser removidas automaticamente.
- Durante quanto tempo os edifícios têm de permanecer destruídos ou abandonados.
- Que tipos de edifícios abandonados podem ser demolidos.
- Que motivos de abandono são elegíveis.
- Se as ruínas municipais e históricas podem ser removidas.
- Quanto dinheiro pode ser gasto em cada revisão mensal de demolições.

**A demolição automática usa o comando normal de demolição do jogo e custa o mesmo que a demolição manual.**

Não remove o zonamento subjacente.

<a id="opt-demolishdestroyed"></a>
### Demolir edifícios destruídos

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `DemolishDestroyed`</text>

Remove automaticamente as ruínas deixadas por incêndios e desastres.

Quando está ligado, o Ministério pode limpar os edifícios destruídos em terrenos residenciais, comerciais, industriais e agrícolas depois do tempo de espera configurado.

**As ruínas são processadas antes dos edifícios abandonados em cada revisão mensal de demolições.**

Quando está desligado, não se faz nenhuma demolição automática de ruínas.

A opção separada para as ruínas municipais e históricas também deixa de ter efeito.

Toda a demolição continua sujeita ao preço da demolição do jogo, à tesouraria disponível e ao orçamento mensal opcional.

**Requer:** Ministério ativo.

**Controla:** [Esperar (ruínas)](#opt-destroyedmonths) e [Demolir edifícios municipais e históricos destruídos](#opt-demolishmunicipal).

Consulte [Demolição automática](demolition.md#demolition-destroyed).

<a id="opt-destroyedmonths"></a>
### Esperar (ruínas)

<text color="secondary">Predefinição: 1 mês (30 dias) | Intervalo: 1–24 meses, em passos de 1 | INI: `DestroyedAfterDays`</text>

Determina durante quanto tempo um edifício destruído tem de permanecer em ruínas antes de o Ministério o poder limpar.

A janela de opções mostra meses simulados, mas o Ministério guarda o tempo de espera em dias simulados.

A conversão usa:

```math
\text{Dias}=
\mathrm{round}\left(\frac{\text{Meses}\times365}{12}\right)
```

Exemplos:

| Atraso selecionado | Dias guardados |
|---|---:|
| 1 mês | 30 |
| 3 meses | 91 |
| 6 meses | 183 |
| 12 meses | 365 |
| 24 meses | 730 |

Um tempo de espera maior dá-lhe mais tempo para inspecionar as ruínas e decidir se atua manualmente.

Um tempo menor permite que o terreno zonado fique disponível mais cedo para ser reconstruído.

**O Ministério verifica a elegibilidade para demolição uma vez por mês simulado.**

Uma ruína é, por isso, limpa na primeira revisão mensal depois de ficar com idade suficiente, não necessariamente no dia exato em que o tempo de espera termina.

O mesmo atraso aplica-se às ruínas municipais e históricas se ligar a respetiva opção de demolição separada.

**Intervalo avançado no INI:** 1–3.650 dias.

**Requer:** Demolir edifícios destruídos e Ministério ativo.

Consulte [Demolição automática](demolition.md#demolition-delays).

<a id="opt-demolishmunicipal"></a>
### Demolir edifícios municipais e históricos destruídos

<text color="secondary">Predefinição: Desligado | Valores: Ligado / Desligado | INI: `DemolishMunicipalAndHistoricRuins`</text>

Controla se o Ministério pode limpar automaticamente as ruínas de edifícios protegidos.

Incluem-se edifícios municipais e de fins especiais, como:

- Escolas e hospitais.
- Esquadras da polícia e quartéis de bombeiros.
- Monumentos e recompensas.
- Outros edifícios colocados diretamente pelo jogador.
- Lotes em zonas especiais.
- Edifícios com classificação histórica.

**Quando está desligada, estas ruínas ficam para tratar manualmente.**

Quando está ligada, as respetivas ruínas seguem o mesmo tempo de espera dos edifícios destruídos comuns.

**Esta opção nunca autoriza a demolição de edifícios municipais ou históricos que estejam de pé ou abandonados.**

Aplica-se apenas depois de o jogo já ter marcado o lote como Destruído.

Por exemplo, se um monumento for destruído por um desastre, ligar esta opção permite ao Ministério remover as suas ruínas depois do tempo de espera.

Pode preferir deixá-la desligada se quiser inspecionar os monumentos destruídos ou outros edifícios especiais antes de os seus restos desaparecerem.

**Requer:** Demolir edifícios destruídos e Ministério ativo.

Consulte [Demolição automática](demolition.md#demolition-protected).

<a id="opt-demolishabandoned"></a>
### Demolir edifícios abandonados

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `DemolishAbandoned`</text>

Limpa automaticamente os edifícios que permaneceram abandonados durante o tempo de espera configurado.

O Ministério só considera os edifícios abandonados que correspondam ao que selecionou quanto a:

- Tipos de zona.
- Motivos de abandono.
- Tempo de espera.

Os edifícios abandonados elegíveis são processados nas revisões mensais, **depois das ruínas**, começando pelos lotes abandonados há mais tempo.

Dois motivos de abandono são sempre considerados elegíveis quando esta funcionalidade está ligada:

- Ocupantes despejados.
- Nenhum motivo ou um motivo não reconhecido.

Todas as outras condições de segurança continuam a aplicar-se.

**Os edifícios municipais e historicamente protegidos abandonados nunca são demolidos automaticamente.**

Quando está desligada, nenhum edifício abandonado é limpo.

**Importante:** Os edifícios que já estavam abandonados antes de instalar o Ministério mantêm a sua idade original. Alguns podem já ser elegíveis na primeira revisão mensal de demolições.

**Requer:** Ministério ativo.

**Controla:** [Esperar (abandonados)](#opt-abandonedmonths), as quatro opções de zona e as cinco opções de motivo de abandono.

Consulte [Demolição automática](demolition.md#demolition-abandoned).

<a id="opt-abandonedmonths"></a>
### Esperar (abandonados)

<text color="secondary">Predefinição: 12 meses (365 dias) | Intervalo: 1–60 meses, em passos de 1 | INI: `AbandonedAfterDays`</text>

Determina durante quanto tempo um edifício tem de permanecer abandonado antes de ficar elegível para demolição.

Por exemplo, com o atraso predefinido de 12 meses, um edifício tem de estar abandonado há pelo menos **365 dias simulados**.

Um tempo de espera maior dá aos edifícios mais tempo para voltarem a ser ocupados.

Um tempo menor permite limpar mais depressa os terrenos abandonados.

**O Ministério usa o contador de idade de abandono do próprio SimCity 4**, que fica guardado na gravação da cidade.

O tempo de espera não recomeça, por isso, quando instala o plugin ou reabre a cidade.

A janela de opções mostra meses, enquanto o ficheiro de configuração guarda dias, com a mesma conversão de 365 dias descrita acima.

**Intervalo avançado no INI:** 1–7.300 dias.

**Requer:** Demolir edifícios abandonados e Ministério ativo.

Consulte [Demolição automática](demolition.md#demolition-delays).

<a id="opt-zoneresidential"></a>
### Zona residencial

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `AbandonedZones` (`Residential`)</text>

Inclui os edifícios residenciais abandonados na demolição automática.

Abrange o desenvolvimento residencial de baixa, média e alta densidade, incluindo casas e prédios de apartamentos.

Quando está ligada, a habitação abandonada elegível pode ser limpa depois do tempo de espera.

Quando está desligada, os edifícios residenciais abandonados são preservados.

**Esta definição só afeta os edifícios abandonados.**

As ruínas residenciais continuam a poder ser limpas através de **Demolir edifícios destruídos**.

**Requer:** Demolir edifícios abandonados e Ministério ativo.

Consulte [Demolição automática](demolition.md#demolition-zones).

<a id="opt-zonecommercial"></a>
### Zona comercial

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `AbandonedZones` (`Commercial`)</text>

Inclui os edifícios comerciais abandonados na demolição automática.

Abrange o desenvolvimento comercial de baixa, média e alta densidade, incluindo lojas e escritórios.

Quando está ligada, os edifícios comerciais abandonados elegíveis podem ser limpos, permitindo que o terreno zonado volte a desenvolver-se.

Quando está desligada, os edifícios comerciais abandonados ficam intocados.

**As ruínas comerciais são controladas em separado** através da definição dos edifícios destruídos.

**Requer:** Demolir edifícios abandonados e Ministério ativo.

Consulte [Demolição automática](demolition.md#demolition-zones).

<a id="opt-zoneindustrial"></a>
### Zona industrial

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `AbandonedZones` (`Industrial`)</text>

Inclui os edifícios industriais abandonados na demolição automática.

Abrange o desenvolvimento industrial de média e alta densidade, como fábricas e armazéns.

Quando está ligada, os edifícios industriais abandonados elegíveis podem ser demolidos depois do tempo de espera.

Quando está desligada, são preservados.

**Os lotes agrícolas não estão incluídos nesta definição.** Têm a sua própria opção separada.

As ruínas industriais continuam sujeitas à definição dos edifícios destruídos, independentemente desta escolha.

**Requer:** Demolir edifícios abandonados e Ministério ativo.

Consulte [Demolição automática](demolition.md#demolition-zones).

<a id="opt-zoneagriculture"></a>
### Zona agrícola

<text color="secondary">Predefinição: Desligado | Valores: Ligado / Desligado | INI: `AbandonedZones` (`Agriculture`)</text>

Controla a demolição automática de quintas e campos agrícolas abandonados.

**Esta definição está desligada por predefinição por uma razão importante.**

No SimCity 4, os campos agrícolas podem parecer abandonados como parte do seu funcionamento normal.

Isso não significa necessariamente que a quinta tenha deixado de funcionar.

Quando está desligada, **todos os lotes agrícolas abandonados são preservados**.

Quando está ligada, o Ministério pode demolir quintas e campos abandonados elegíveis depois do tempo de espera.

**Ligar esta opção pode remover campos agrícolas que queria manter.**

Use-a com cuidado, sobretudo em cidades agrícolas.

As ruínas em terrenos agrícolas são um caso à parte. Podem continuar a ser limpas através de **Demolir edifícios destruídos**, mesmo com a Zona agrícola desligada.

**Requer:** Demolir edifícios abandonados e Ministério ativo.

Consulte [Demolição automática](demolition.md#demolition-farms).

<a id="opt-reasonlowdesirability"></a>
### Motivo: zona pouco atraente

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `AbandonedReasons` (`LowDesirability`)</text>

Inclui os edifícios abandonados porque a sua localização se tornou pouco atraente.

As causas possíveis incluem:

- Poluição.
- Criminalidade.
- Serviços inadequados.
- Outros fatores que reduzem a atratividade local.

Quando está ligada, esses edifícios podem ser demolidos depois do tempo de espera configurado.

Quando está desligada, são preservados.

**Demolir um edifício abandonado não resolve o problema de atratividade subjacente.**

Por exemplo, se a poluição continuar excessiva, o edifício que o substituir pode enfrentar dificuldades semelhantes.

Melhorar a área envolvente pode, por vezes, permitir que o edifício original volte a ser ocupado.

**Requer:** Demolir edifícios abandonados e Ministério ativo.

Consulte [Demolição automática](demolition.md#demolition-reasons).

<a id="opt-reasoncommute"></a>
### Motivo: trajeto demasiado longo

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `AbandonedReasons` (`CommuteTime`)</text>

Inclui os edifícios abandonados porque os seus ocupantes não conseguiam chegar ao trabalho num tempo de deslocação aceitável.

Quando está ligada, os edifícios elegíveis podem ser demolidos depois do tempo de espera.

Quando está desligada, são preservados.

**A demolição não resolve diretamente o problema das deslocações.**

Por exemplo, melhorar os transportes públicos ou o acesso rodoviário pode permitir que um edifício residencial abandonado volte a ser ocupado.

Pode preferir um tempo de espera maior se fizer regularmente melhorias na rede de transportes da cidade.

**Requer:** Demolir edifícios abandonados e Ministério ativo.

Consulte [Demolição automática](demolition.md#demolition-reasons).

<a id="opt-reasonlowdemand"></a>
### Motivo: falta de procura

<text color="secondary">Predefinição: Ligado | Valores: Ligado / Desligado | INI: `AbandonedReasons` (`LowDemand`)</text>

Inclui os edifícios abandonados porque a cidade já não tem procura suficiente para esse tipo de desenvolvimento.

Quando está ligada, o Ministério pode limpar esses edifícios depois do tempo de espera, deixando o seu zonamento disponível para crescimento futuro.

Quando está desligada, ficam intocados.

**A procura pode mudar à medida que a cidade se desenvolve.**

Um edifício abandonado hoje por falta de procura pode voltar a ser útil mais tarde.

O tempo de espera determina quanto tempo concede a essa possibilidade antes de o Ministério limpar o lote.

**Requer:** Demolir edifícios abandonados e Ministério ativo.

Consulte [Demolição automática](demolition.md#demolition-reasons).

<a id="opt-reasonnopower"></a>
### Motivo: sem eletricidade

<text color="secondary">Predefinição: Desligado | Valores: Ligado / Desligado | INI: `AbandonedReasons` (`NoPower`)</text>

Controla a demolição de edifícios abandonados por falta de eletricidade.

**Esta opção está desligada por predefinição.**

Um edifício abandonado por uma falha de eletricidade pode voltar a ser ocupado depois de a eletricidade ser reposta.

Quando está desligada, o Ministério preserva esses edifícios.

Quando está ligada, pode demoli-los depois do tempo de espera configurado.

**Verifique a rede elétrica antes de ligar esta opção.**

Um edifício abandonado pode precisar apenas de uma ligação reparada ou do fornecimento reposto, e não de ser demolido.

**Requer:** Demolir edifícios abandonados e Ministério ativo.

Consulte [Demolição automática](demolition.md#demolition-reasons).

<a id="opt-reasonnowater"></a>
### Motivo: sem água

<text color="secondary">Predefinição: Desligado | Valores: Ligado / Desligado | INI: `AbandonedReasons` (`NoWater`)</text>

Controla a demolição de edifícios abandonados por falta de água.

Tal como no caso da eletricidade, **esta opção está desligada por predefinição**.

Os edifícios abandonados por falta de água podem voltar a ser ocupados quando o fornecimento de água ou a ligação às canalizações for reposto.

Quando está desligada, esses edifícios são preservados.

Quando está ligada, o Ministério pode limpá-los depois do tempo de espera.

**Verifique a rede de água antes de ligar esta opção.**

A demolição pode ser desnecessária se repor o serviço permitir que o edifício recupere.

**Requer:** Demolir edifícios abandonados e Ministério ativo.

Consulte [Demolição automática](demolition.md#demolition-reasons).

<a id="opt-budgetlimit"></a>
### Limitar o orçamento mensal de demolição

<text color="secondary">Predefinição: Desligado | Valores: Ligado / Desligado | INI: `LimitMonthlyBudget`</text>

Acrescenta um limite de despesa opcional à demolição automática.

O limite aplica-se às **ruínas e aos edifícios abandonados em conjunto**, em cada revisão mensal de demolições.

Quando está ligado, o Ministério vai registando os custos de demolição das ordens já submetidas nessa revisão.

Uma demolição só é permitida quando o seu custo não ultrapassar o orçamento mensal configurado.

**Se o custo do edifício seguinte exceder o limite, toda a revisão mensal de demolições pára.**

Os edifícios restantes esperam pelo mês seguinte.

Quando está desligado, não existe nenhum limite adicional de despesa mensal.

No entanto, **a proteção da tesouraria está sempre ativa**.

O Ministério nunca ordena mais despesas de demolição do que a cidade consegue pagar com a tesouraria disponível no início da revisão.

As demolições com um preço de §0 não consomem o orçamento mensal opcional.

**Requer:** Ministério ativo.

**Controla:** o [Orçamento mensal](#opt-demolitionbudget).

Consulte [Demolição automática](demolition.md#demolition-money).

<a id="opt-demolitionbudget"></a>
### Orçamento mensal

<text color="secondary">Predefinição: §1.000 | Intervalo no jogo: §0–§200.000, em passos de §1.000 | INI: `MonthlyBudget`</text>

Define o montante máximo que o Ministério pode gastar em demolições automáticas durante uma revisão mensal, quando o limite de orçamento está ligado.

Por exemplo, com um orçamento de **§1.000**, o Ministério pode ordenar demolições elegíveis enquanto o seu custo combinado se mantiver dentro desse montante.

Se uma demolição proposta exceder o limite, a revisão pára e os restantes candidatos esperam pelo mês seguinte.

**O que acontece com diferentes níveis de orçamento?**

| Orçamento mensal | Efeito |
|---|---|
| **§0** | Só são permitidas demolições que custem §0. |
| **§1.000** | Podem ser comprometidos até §1.000 por revisão. |
| **§5.000** | Podem ser comprometidos até §5.000 por revisão. |
| **§20.000** | Podem ser comprometidos até §20.000 por revisão. |

Um orçamento maior permite mais despesas de demolição num único mês.

Um orçamento menor distribui por mais meses um grande atraso de edifícios elegíveis.

**O orçamento mensal nunca se sobrepõe à regra da tesouraria.**

Por exemplo, definir um orçamento de demolição de §20.000 não permite ao Ministério gastar §20.000 se a cidade só tiver §5.000 disponíveis.

#### E se o limite de orçamento estiver desligado?

O controlo Orçamento mensal fica indisponível, mas o valor guardado é preservado.

Se ligar o limite mais tarde, o orçamento anterior volta a ficar ativo.

Por exemplo, a configuração fornecida guarda **§1.000** mesmo que o limite esteja desligado por predefinição.

Ligá-lo não repõe esse montante.

#### Posso definir um orçamento maior manualmente?

Sim.

A janela de opções dentro do jogo permite um máximo de **§200.000**.

O ficheiro de configuração aceita valores até **2.000.000.000**, embora valores maiores não possam ser representados por completo na janela de opções.

**Requer:** Limitar o orçamento mensal de demolição e Ministério ativo.

Consulte [Demolição automática](demolition.md#demolition-money).

---

[Voltar ao índice](index.md#contents) | [Voltar ao README](../../README.pt.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
