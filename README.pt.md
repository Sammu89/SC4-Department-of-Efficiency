[English](README.md) | **Português** | [Français](README.fr.md) | [Deutsch](README.de.md) | [Español](README.es.md) | [Italiano](README.it.md) | [Nederlands](README.nl.md) | [Dansk](README.da.md) | [Svenska](README.sv.md) | [Polski](README.pl.md)

# Ministério da Optimização

### [⬇ Transferir o Ministério da Optimização 1.0.0](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases/download/v1.0.0/Departement_of_Efficiency_1.00.zip)

**O Ministério da Optimização trata dos pequenos pormenores da gestão da cidade que fazem uma grande diferença.**

O Ministério da Optimização (*Department of Efficiency*, em inglês) é um plugin para **SimCity 4 Deluxe** que automatiza tarefas rotineiras de gestão da cidade, para que se possa concentrar em construir, planear e fazer crescer a sua cidade.

**Constrói a cidade. O Ministério optimiza o orçamento!**

Gere os orçamentos de cada estabelecimento de educação e de saúde, as centrais elétricas, o financiamento da água e do saneamento, os acordos com as cidades vizinhas e, opcionalmente, a demolição de ruínas e de edifícios abandonados. O objetivo é satisfazer as necessidades da cidade sem gastar mais do que o necessário, deixando consigo a localização dos edifícios, a cobertura e as grandes decisões de planeamento.

Tudo se controla dentro do jogo, em **Opções de Jogo → Ministério da Optimização**. Escolha os serviços a automatizar, ajuste as respetivas margens de segurança, configure a demolição ou desligue todo o Ministério. Pode também usar a lei municipal **Instituir o Ministério da Optimização** como interruptor geral. Não é preciso nenhuma aplicação à parte.

> [!IMPORTANT]
> **O Ministério começa ligado, incluindo a demolição automática de edifícios destruídos e abandonados.** Reveja as suas definições antes de abrir uma cidade a que dê valor. Faz alterações reais no jogo: o financiamento, os contratos e as demolições ficam guardados se gravar a cidade. Não edita diretamente os ficheiros de gravação.

## Índice

- [O que o Ministério faz](#features)

- [Configuração inicial](#first-time-setup)

- [Definições](#settings-explained)

- [Capturas de ecrã](#screenshots)

- [Requisitos e compatibilidade](#requirements-and-compatibility)

- [Transferência e instalação](#download-and-installation)

- [Atualizar e desinstalar](#updating-and-uninstalling)

- [Limitações conhecidas](#known-limitations)

- [Créditos](#credits)

<a id="features"></a>

## O que o Ministério faz

A tabela seguinte descreve o que o Ministério faz. Para fórmulas, exemplos, salvaguardas e a mecânica por trás das decisões, consulte a **[referência detalhada das funcionalidades](features/pt.md)**.

| Área | O que faz |
|---|---|
| [Educação e saúde](features/pt.md#education-healthcare) | Ajusta o financiamento **edifício a edifício** das escolas, universidades, bibliotecas, museus, clínicas e hospitais elegíveis. Segue a frequência e a capacidade reais, com uma margem de segurança. **Não altera os raios de cobertura** nem o financiamento dos autocarros escolares e das ambulâncias. Se um edifício continuar sobrelotado com o financiamento no máximo, um conselheiro avisa que é preciso outro equipamento; **a escolha do local é sua**. |
| [Eletricidade](features/pt.md#electricity) | Gere **cada central elétrica individualmente**, escolhendo o financiamento que cobre as necessidades da cidade e a reserva de segurança ao menor custo modelado. Tem em conta a eficiência e o estado de cada central; uma opção permite incluir também o custo económico do desgaste. |
| [Água](features/pt.md#water) | Ajusta o financiamento da água da cidade para satisfazer a procura mais uma reserva de segurança. A proteção das canalizações vem ligada por predefinição, para evitar cortes que deixariam a rede degradar-se. Esconde também o ícone enganador de «sem água» do jogo sobre as quintas. |
| [Saneamento e lixo](features/pt.md#garbage-landfills) | Ajusta o financiamento do saneamento a partir da produção de lixo, da reciclagem, do tratamento, do armazenamento nos aterros e dos contratos regionais, protegendo sempre as necessidades da própria cidade. |
| [Acordos com os vizinhos](features/pt.md#neighbor-deals) | Pode **comprar ou vender eletricidade e água**, **pagar às vizinhas para tratarem o seu lixo** ou **aceitar o delas mediante pagamento**. Cria, ajusta e cancela contratos elegíveis quando o resultado global beneficia a cidade e as suas próprias necessidades e reservas ficam protegidas. |
| [Demolição automática](features/pt.md#demolition) | Pode limpar ruínas ao fim de cerca de **30 dias simulados** e edifícios elegíveis abandonados há **um ano simulado**, por predefinição. Decide que zonas e que motivos de abandono contam, se as ruínas protegidas podem ser limpas e se quer fixar um limite mensal de gastos. |
| [Avisos dos conselheiros](features/pt.md#notices) | Usa os próprios conselheiros do jogo para assinalar faltas comprovadas e persistentes que o financiamento ou os acordos disponíveis não conseguem resolver. Quando faz sentido, as mensagens trazem ligações que levam aos edifícios afetados. |
| [Aviso sobre o tamanho do aterro](features/pt.md#garbage-landfills-advice) | O aviso opcional indica uma ampliação concreta do aterro, ou uma redução segura, que poderia baixar a fatura do saneamento em mais do que o limiar de poupança escolhido. **O Ministério aconselha; a decisão de alterar a zona é sua.** |
| [Reparação de quadrados fantasma](features/pt.md#ghost-squares) | Com a gestão do saneamento ligada, repara com segurança os quadrados de aterro invisíveis elegíveis que o jogo ainda conta na manutenção. Os casos inseguros ou não verificados ficam como estão; não há interruptor próprio para esta reparação. |

Continua a controlar a construção, as zonas, os impostos, as estradas e os transportes, a polícia, os bombeiros e qualquer outro serviço que o Ministério não gere. Se ligar a gestão de um tipo de acordo com os vizinhos, o Ministério pode também alterar ou cancelar um contrato que tenha feito manualmente. Desligue essa opção de acordos para gerir esses contratos por si.

<a id="first-time-setup"></a>

## Configuração inicial

O Ministério usa as definições fornecidas com o plugin e **começa a trabalhar automaticamente**. Não precisa de o ligar, mas deve rever o que lhe é permitido fazer antes de usar uma cidade gravada importante.

1\. Inicie o SimCity 4 e, a partir da **vista da região**, abra **Opções de Jogo → Ministério da Optimização**. Se preferir, use uma cidade nova ou descartável para a primeira vista de olhos.

2\. Na página **Serviços**, escolha que orçamentos e acordos com os vizinhos o Ministério pode gerir e que folga de capacidade deve manter.

3\. Na página **Demolição**, verifique as regras para limpar ruínas e edifícios abandonados. **Desligue a demolição automática se quiser deitar tudo abaixo pessoalmente.**

4\. Clique em **Aceitar** para guardar as escolhas. **Cancelar** fecha a janela sem guardar as alterações.

5\. Abra a cidade que quer jogar. O Ministério segue as suas preferências guardadas e gere a cidade enquanto a simulação decorre.

As preferências ficam nos ficheiros INI do plugin e são reutilizadas quando abre outra cidade. O Ministério **não** constrói escolas, hospitais, centrais elétricas nem instalações de água por si. Quando o financiamento e os acordos disponíveis não conseguem resolver uma falta comprovada e persistente, um conselheiro do jogo diz-lhe onde faz falta mais capacidade.

Também pode ativar ou desativar o Ministério pelas leis municipais.

<a id="settings-explained"></a>

## Definições

Abra **Opções de Jogo → Ministério da Optimização**. A janela tem duas páginas, **Serviços** e **Demolição**. As tabelas seguintes cobrem **todas as definições que aparecem no menu**, sem as fórmulas técnicas. Clique no nome de uma opção para ler a explicação completa na [referência de funcionalidades](features/pt.md#options).

**Como funcionam os controlos:** «Ligado» significa que o Ministério pode gerir essa funcionalidade. Uma definição que depende de outra fica a cinzento enquanto a principal estiver desligada. As definições em percentagem controlam margens de segurança e limiares, ajudando o Ministério a manter capacidade de reserva e a absorver as flutuações da procura.

### Serviços

| Opção | Predefinição | O que significa |
|---|---|---|
| [Ministério ativo](features/pt.md#opt-master) | **Ligado** | Interruptor geral do financiamento, dos contratos com os vizinhos, da demolição e dos avisos do Ministério. Desligá-lo não anula as alterações já feitas. |
| [Gerir a educação](features/pt.md#opt-education) | **Ligado** | Ajusta automaticamente, uma a uma, as escolas, universidades, bibliotecas e museus elegíveis. Não altera a cobertura nem o financiamento dos autocarros escolares. |
| [Margem de segurança (educação)](features/pt.md#opt-educationreserve) | **5%** (0–50%) | Capacidade a mais acima da utilização atual da educação. Mais capacidade custa mais. Nos edifícios com pouca frequência, o Ministério pode manter uma margem adicional. Por exemplo, isto pode permitir que uma escola passe de 10 para 20 alunos (um aumento de 100%) sem greve, sobretudo no início, quando o custo extra é pequeno. |
| [Gerir a saúde](features/pt.md#opt-health) | **Ligado** | Ajusta automaticamente, um a um, as clínicas, hospitais e outros equipamentos de saúde suportados e elegíveis. Não altera o financiamento das ambulâncias nem a cobertura. |
| [Margem de segurança (saúde)](features/pt.md#opt-healthreserve) | **5%** (0–50%) | Funciona da mesma forma que a margem de segurança da educação. |
| [Gerir a eletricidade](features/pt.md#opt-power) | **Ligado** | Ajusta o financiamento de cada central elétrica para fornecer eletricidade suficiente ao menor custo modelado. |
| [Reserva de eletricidade](features/pt.md#opt-powerreserve) | **20%** (10–50%) | Capacidade de produção de reserva para a parte da procura abastecida localmente; reservas maiores dão mais folga para acompanhar um crescimento rápido e reduzem o risco de apagões, mas podem custar mais. |
| [Ter em conta o desgaste das centrais](features/pt.md#opt-poweraging) | **Desligado** | Inclui na optimização o custo estimado do desgaste e do envelhecimento das centrais. Pode favorecer um financiamento mais alto quando o benefício, a longo prazo, de conservar a central compensa o custo de funcionamento adicional. |
| [Gerir a água](features/pt.md#opt-water) | **Ligado** | Ajusta o financiamento do departamento da água para abastecer a cidade sem gastos desnecessários. |
| [Reserva de água](features/pt.md#opt-waterreserve) | **10%** (10–50%) | Funciona da mesma forma que a reserva de eletricidade. |
| [Proteger as canalizações](features/pt.md#opt-waterpipes) | **Ligado** | Mantém o financiamento igual ou acima dos 50% do jogo, para evitar danos na rede de canalizações. |
| [Gerir o saneamento](features/pt.md#opt-sanitation) | **Ligado** | Ajusta o financiamento do departamento do lixo para cobrir as necessidades de recolha e eliminação. Repara também os quadrados fantasma (um erro do jogo que cobra manutenção por quadrados de aterro já sem zona). |
| [Reserva de saneamento](features/pt.md#opt-sanitationreserve) | **2%** (2–20%) | Capacidade de tratamento extra para o lixo da cidade, além do que ela produz depois da reciclagem e de qualquer atraso acumulado. |
| [Avisar-me quando rever o tamanho do aterro compensar](features/pt.md#opt-landfilladvice) | **Desligado** | Ativa o aviso opcional sobre alterações à zona do aterro, quando isso poupa dinheiro e o lixo da cidade continua a ser tratado. Exige a gestão do saneamento. |
| [Limiar de poupança dos avisos do aterro](features/pt.md#opt-landfillthreshold) | **25%** (20–50%) | Só lhe pede que pense em alterar o tamanho do aterro se a poupança mensal modelada atingir, pelo menos, esta parte do orçamento do saneamento. |
| [Gerir os acordos com os vizinhos](features/pt.md#opt-deals) | **Ligado** | Ativa os contratos regionais automáticos; os três interruptores individuais abaixo decidem que recursos podem ser negociados. |
| [Comprar a uma cidade para vender a outra](features/pt.md#opt-arbitrage) | **Desligado** | Permite combinações lucrativas de compra e venda do mesmo recurso entre vizinhas, dando sempre prioridade às necessidades da cidade. |
| [Gerir acordos de eletricidade](features/pt.md#opt-tradepower) | **Ligado** | Permite ao Ministério criar, alterar ou cancelar **importações e exportações** de eletricidade. Independente do financiamento das centrais. |
| [Gerir acordos de água](features/pt.md#opt-tradewater) | **Ligado** | Permite ao Ministério criar, alterar ou cancelar **importações e exportações** de água. Independente do financiamento da água. |
| [Gerir acordos de lixo](features/pt.md#opt-tradegarbage) | **Ligado** | Permite ao Ministério combinar o tratamento de lixo **nos dois sentidos**. Independente do financiamento do saneamento. |

### Demolição

| Opção | Predefinição | O que significa |
|---|---|---|
| [Demolir edifícios destruídos](features/pt.md#opt-demolishdestroyed) | **Ligado** | Demole as ruínas ao fim de um certo tempo. Não limpa automaticamente as ruínas municipais ou históricas protegidas, a não ser que a opção abaixo o permita. |
| [Esperar (edifícios destruídos)](features/pt.md#opt-destroyedmonths) | **1 mês** (1–24) | Quanto tempo as ruínas têm de permanecer antes de poderem ser limpas; por predefinição, cerca de **30 dias simulados**. |
| [Demolir edifícios municipais e históricos destruídos](features/pt.md#opt-demolishmunicipal) | **Desligado** | Se ligada, limpa também, depois da espera, as **ruínas** elegíveis de edifícios municipais, especiais e históricos. |
| [Demolir edifícios abandonados](features/pt.md#opt-demolishabandoned) | **Ligado** | Limpa os edifícios abandonados elegíveis depois do tempo de espera, nos tipos de zona e pelos motivos escolhidos. |
| [Esperar (edifícios abandonados)](features/pt.md#opt-abandonedmonths) | **12 meses** (1–60) | Quanto tempo um edifício tem de ficar abandonado antes de poder ser demolido; por predefinição, um ano simulado. |
| [Zona residencial](features/pt.md#opt-zoneresidential) | **Ligado** | Inclui moradias e prédios de apartamentos abandonados. |
| [Zona comercial](features/pt.md#opt-zonecommercial) | **Ligado** | Inclui lojas e escritórios abandonados. |
| [Zona industrial](features/pt.md#opt-zoneindustrial) | **Ligado** | Inclui fábricas e outros edifícios industriais abandonados. |
| [Zona agrícola](features/pt.md#opt-zoneagriculture) | **Desligado** | Inclui quintas abandonadas. Desligada por predefinição porque quadrados vazios ou abandonados numa quinta podem ser normais. |
| [Motivo: zona pouco atraente](features/pt.md#opt-reasonlowdesirability) | **Ligado** | Inclui o abandono causado por pouca atratividade, como poluição ou falta de serviços. |
| [Motivo: trajeto demasiado longo](features/pt.md#opt-reasoncommute) | **Ligado** | Inclui o abandono causado por deslocações demasiado longas para o trabalho. |
| [Motivo: falta de procura](features/pt.md#opt-reasonlowdemand) | **Ligado** | Inclui edifícios abandonados porque a procura desse tipo desapareceu. |
| [Motivo: sem eletricidade](features/pt.md#opt-reasonnopower) | **Desligado** | Inclui, opcionalmente, edifícios abandonados por falta de eletricidade. Normalmente é melhor restabelecer primeiro a eletricidade e ver se o edifício volta a ser ocupado. |
| [Motivo: sem água](features/pt.md#opt-reasonnowater) | **Desligado** | Inclui, opcionalmente, edifícios abandonados por falta de água. Normalmente é melhor restabelecer primeiro a água e ver se o edifício volta a ser ocupado. |
| [Limitar o orçamento mensal de demolição](features/pt.md#opt-budgetlimit) | **Desligado** | Fixa um limite opcional de gastos para a demolição automática, além do dinheiro disponível no tesouro da cidade. |
| [Orçamento mensal](features/pt.md#opt-demolitionbudget) | **§1.000** (§0–§200.000) | Gasto máximo em demolições por mês simulado **quando o limite de orçamento está ligado**. |

<a id="screenshots"></a>

## Capturas de ecrã

Todas as capturas de ecrã mostram a versão inglesa do jogo.

| | |
|---|---|
| ![A página Serviços das definições do Ministério](screenshots/Options-1.png) | ![A página Demolição das definições do Ministério](screenshots/Options-2.png) |
| *A página Serviços: cada serviço, a sua reserva e os acordos com os vizinhos.* | *A página Demolição: ruínas, edifícios abandonados e o orçamento mensal.* |
| ![Um aviso de conselheiro sobre escolas secundárias sobrelotadas](screenshots/high_school.png) | ![Um aviso de conselheiro sobre museus sobrelotados](screenshots/museum.png) |
| *Escolas com o financiamento no máximo e ainda sobrelotadas: o aviso liga aos edifícios.* | *O mesmo para os museus.* |
| ![Um aviso de conselheiro sobre falta de água](screenshots/water.png) | ![Um aviso de conselheiro sobre falta de capacidade para o lixo](screenshots/real-garbage-paused.png) |
| *Água: o Ministério não consegue resolver a falta com o abastecimento disponível.* | *Lixo: a cidade precisa de mais capacidade de eliminação.* |

<a id="requirements-and-compatibility"></a>

## Requisitos e compatibilidade

- **SimCity 4 Deluxe, edição Steam, versão do jogo 1.1.641.0, em Windows.** É a versão suportada; o Ministério fica inativo noutras versões.

- **Não é necessário nenhum outro mod, instalador ou aplicação externa.**

- **Testado e compatível com estes mods** (versões testadas):

  - Network Addon Mod (NAM) — 1.3.1
  - Colossus Addon Mod (CAM) — 3.1.1
  - SC4 System Integration Module — 4.0.1
  - CAM Ordinances — 3.3.0
  - Industrial Revolution Mod (IRM) — 2.0.0
  - Colossus Farming
  - Advanced Water Management — 1.0
  - SC4Fix — 1.0.0.7
  - More Building Styles — 3.6.1
  - More Demand Info — 1.3.1
  - Custom Budget Departments — 1.1
  - Custom Ordinance Host — 1.0.1
  - Legalize Gambling Ordinance Upgrade — 1.0.0
  - Lua Sandbox — 1.0.0
  - Lua Extensions — 1.5
  - Query Tool UI Extensions — 2.7.1
  - Resource Loading Hooks — 2.0.1
  - Transparent Texture Fix — 1.0.0
  - I-R Fix — 1.0.0
  - Universal Commercial Cap Fix — 2.0.0
  - Save Warning — 1.0
  - Zoning Mod Maxis — 2.0.0
  - Maxis Prop Names and Query Fix — 2.0
  - Seasonal Flora Patch — 2.1
  - No Maxis I-D, I-M e I-HT — 2.0

- **Recomendado:** acrescente `-CPUCount:1` em **Steam → Biblioteca → SimCity 4 Deluxe → Propriedades → Opções de lançamento**. Isto ajudou a estabilidade em sessões longas no computador do autor.

- **Dez idiomas no jogo:** inglês, português, francês, alemão, espanhol, italiano, neerlandês, dinamarquês, sueco e polaco. O Ministério segue automaticamente o idioma do seu jogo.

<a id="download-and-installation"></a>

## Transferência e instalação

1\. Abra a página **Releases** do projeto (`<PUBLIC-REPOSITORY-URL>/releases`) e escolha a versão mais recente. Substitua este marcador pelo endereço do repositório público antes de publicar.

2\. Em **Assets**, transfira o **ZIP de distribuição do Department of Efficiency**.

3\. **Feche o SimCity 4.** Extraia o ZIP para uma pasta temporária.

4\. Copie **`DepartmentEfficiency.dll`** e toda a pasta **`Department of Efficiency`** para a raiz de `Documentos\SimCity 4\Plugins`.

5\. Inicie o jogo e siga as instruções da [configuração inicial](#first-time-setup) antes de jogar uma cidade gravada.

O ZIP de distribuição contém a DLL, um `README.txt` e uma pasta com cinco ficheiros DAT e três INI. Depois da instalação, deverá ter:

```text
Documents\SimCity 4\Plugins\
    README files
    DepartmentEfficiency.dll
    Department of Efficiency\
        DepartmentEfficiency.dat
        DepartmentEfficiencyUI.dat
        DepartmentEfficiencyGarbageAdvice.dat
        DepartmentEfficiencyDealAdvice.dat
        DepartmentEfficiencyCivicAdvice.dat
        DepartmentEfficiency.ini
        DepartmentEfficiencyUtility.ini
        DepartmentEfficiencyDemolition.ini
    features\
    screenshots\
```

Os ficheiros Markdown e as capturas de ecrã do repositório público **não** são necessários ao jogo. Mantenha a DLL **diretamente em Plugins**, com a pasta que a acompanha ao lado.

**Alterar as definições mais tarde:** abra **Opções de Jogo → Ministério da Optimização**, use as páginas **Serviços** e **Demolição** e clique em **Aceitar**. Para o interruptor geral pode também usar a lei **Instituir o Ministério da Optimização**, sem custo mensal. Desligar o Ministério impede ações futuras; não anula o financiamento, os contratos nem as demolições já aplicados.

<a id="updating-and-uninstalling"></a>

## Atualizar e desinstalar

**Guarde as suas preferências.** Os três ficheiros INI na pasta `Department of Efficiency` guardam as definições. Antes de substituir o mod, faça uma cópia de `DepartmentEfficiency.ini`, `DepartmentEfficiencyUtility.ini` e `DepartmentEfficiencyDemolition.ini` para fora da pasta Plugins.

**Para substituir uma instalação:** feche o jogo, remova a `DepartmentEfficiency.dll` e a pasta `Department of Efficiency` existentes, instale o novo pacote e restaure os INI de que fez cópia **apenas se o formato for compatível** com a nova versão. Caso contrário, defina de novo as preferências no jogo.

**Para desinstalar:** feche o jogo e apague `DepartmentEfficiency.dll` e a pasta `Department of Efficiency`. As suas cidades gravadas continuam legíveis sem o mod. Os níveis de financiamento, os contratos existentes e as demolições já gravadas numa cidade não são revertidos automaticamente.

<a id="known-limitations"></a>

## Limitações conhecidas

- **Compatibilidade:** só é suportada a versão **1.1.641.0** do SimCity 4 Deluxe para Windows (Steam). As outras versões do jogo ficam inativas.

- **Aviso sobre o aterro:** exige exatamente uma zona de aterro. Em cidades com vários aterros, o Ministério continua a poder reparar os «quadrados fantasma» elegíveis, mas não recomenda alterações de tamanho.

<a id="credits"></a>

## Créditos

- Criado por **Sammu89**.

- Construído com a **gzcom-dll** de Nelson Gomez (simmaster07), usada sob a licença LGPL-2.1-or-later e ligada estaticamente à DLL.

- Nicholas Hayes (0xC0000054 / Null 45), pela investigação, documentação e exemplos abertos sobre DLL do SimCity 4: sc4-dll-utilities, sc4-custom-budget-departments, sc4-query-ui-hooks, sc4-lua-extensions, sc4-lua-extension-example.

- CorinaMarie, por documentar a técnica de Urgent Advice do SimCity 4 que os avisos de conselheiro do mod seguem.

- A comunidade de modding do SimCity 4, por duas décadas de investigação, ferramentas e conhecimento partilhado.
