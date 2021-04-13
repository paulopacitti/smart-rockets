**Nome** Douglas Daisuke | **RA** _____

**Nome** Gabriel Teston  | **RA** _____

**Nome** Paulo  Pacitti  | **RA** 185447

# MC536 | Trabalho 1: Threads com smart-rockets
## Motivação
Inspirado pela ideia de implementar o *Game of Life* do John Horton Conway utilizando threads, nosso grupo decidiu implementar a simulação que se utiliza de algoritmos genéticos conhecida como *Smart Rockets*, porém esta sendo otimizada com o uso de threads.

A simulação consiste em uma coleção de foguetes partir de uma mesma origem em direção a um alvo, sendo que cada foguete contém em seu "DNA" uma rota para tentar atingir o alvo. Após os foguetes terminarem de percorrer as suas rotas, é dado uma pontuação baseada na distância final do foguete ao alvo. Depois disso, os foguetes com maiores pontuações realizam um "cruzamento", gerando uma nova população baseada nos DNAs dos foguetes com maior sucesso da geração anterior. A ideia é que a cada geração, foguetes com maior performance (menor distância ao alvo) são gerados a partir desse cruzamento.

## Estrutura de dados e estratégia para divisão de trabalho
A implementação feita em C feita pelo grupo é feita em duas partes: uma *lib* que contém as rotinas e entidades da simulação, e um arquivo onde o "loop" de simulação é feito. Temos as seguintes entidades:

- **Rocket**: entidade que define as propriedades do foguete:
- **Population**: Define a coleção de foguetes, com o tamanho da coleção e o fator de mutação para cruzamentos.
- **Board**: O "tabuleiro" onde acontece a simulação.
- **Obstacle**: obstáculo que é posicionado no tabuleiro com o fim de dificultar a rota até o alvo;

Além das entidades, temos as rotinas necessárias para a simulação acontecer, decritas no *header* `smart-rockets.h` (para saber mais, recomenda-se a análise do código-fonte). Dentre essas rotinas, estão as essenciais para a simulação como de cruzamento de foguetes, a geração de uma nova coleção de foguetes baseada na geração anterior, o cálculo de *fitness* (pontuação de cada foguete baseada na distância final ao alvo) e a rotina de atualização da posição de cada fogute.

## Utilizando as Threads
As *threads* foram utilizadas em seções do código onde a progração paralela poderia oferecer vantagem em relação a programação serial. As seções onde se utilizam *threads* são:

- **Ordenação dos foguetes:** a ordenação dos foguetes com maior performance é feito através de um *merge *sort* adaptado, onde a coleção de foguetes é divido em 4 partes e cada uma das partes é ordenada usando o citado algoritmo de ordenação. Após o *join* das *threads*, ocorre a intercalação entre as partes e a coleção de foguetes termina ordenada em relação ao *score*;
- **Atualização da posição dos foguetes:** para otimização, a atualização da posição de cada foguete pode ser feita de forma paralela, então nessa rotina também se é utilizado as vantagens das *threads*;
- **Colisão:** dados os obstáculos e paredes do tabuleiro, é necessário detectar se houve colisão ou não pois esta constituí uma penalidade. Tal verificação pode ser feita de forma paralela, otimizando o tempo de execução