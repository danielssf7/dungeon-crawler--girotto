#  Retro Terminal RPG (Mental Torture Edition)

Este é um jogo de RPG e exploração de masmorras baseado em texto, desenvolvido inteiramente na linguagem *C*. O projeto roda diretamente no terminal da sua máquina e apresenta mecânicas clássicas de movimentação em matrizes, renderização de hitboxes em tempo real, inteligência artificial de perseguição para inimigos e um sistema de progressão por fases.

##  Funcionalidades Principais

* *Compatibilidade Multiplataforma*: O código utiliza diretivas de pré-processador (#ifdef _WIN32) para alternar nativamente entre os comandos de terminal do Windows e sistemas UNIX (Linux/macOS), controlando a limpeza de tela (cls/clear) e taxas de atualização em milissegundos (Sleep/usleep).
* *Sistema de Combate Animado: Implementação inovadora de animação de *hitboxes no terminal. Ao atacar, o jogo gera temporariamente caracteres z no mapa para ilustrar o alcance da arma antes de calcular o dano.
* *Armas Próprias com Balanço Exclusivo*:
  *  *Espada*: Causa 15 de dano. Alcance frontal amplo de 3x2 blocos.
  *  *Arco e Flecha*: Causa 10 de dano. Alcance linear longo de até 4 blocos de distância.
  *  *Cajado*: Causa 20 de dano. Ataque místico em área circular (3x3 em volta do jogador).
* *IA de Perseguição dos Mobs*: Os monstros (X) calculam a distância cartesiana até o jogador a cada turno e o perseguem pelo mapa, contornando obstáculos e causando dano ao tocá-lo.
* *Mecânicas de Level Design*: O jogo possui 4 mapas dinâmicos (Vila + 3 Fases de Masmorra) que aumentam de tamanho progressivamente. Inclui puzzles de chaves (@), portas trancadas (D) e perigos ambientais como blocos de Lava (#) que reduzem a vida do herói.

---

##  Guia de Símbolos do Mapa

* ^, <, v, > : O Jogador (a seta indica a direção para onde você está olhando).
* X : Monstros/Inimigos.
* N : NPC Jorge (Ferreiro da Vila).
* * : Paredes e obstáculos intransponíveis.
* @ : Chaves coletáveis.
* D : Portas trancadas.
* L : Escadas de transição entre fases.
* # : Lava (Tira 1 ponto de vida a cada passo dado em cima).

---

##  Controles do Jogo

O jogo funciona em turnos baseados em entradas do teclado. Digite a tecla de ação e aperte Enter:

* w, a, s, d : Movimentação (Cima, Esquerda, Baixo, Direita).
* i : Interagir (Falar com o NPC Jorge na Vila / Abrir portas D se tiver chaves / Voltar para a fase anterior nas escadas).
* o : Desferir ataque com a arma equipada (Exibe a animação da hitbox).
* m : Subir as escadas (L) para avançar de andar (Requer que a porta da fase atual esteja aberta).
* q : Sair do jogo.

---

##  Como Compilar e Executar

Para rodar o jogo, você precisará de um compilador de C (como o GCC) instalado no seu sistema.

1. Clone este repositório:
   ```bash
   git clone [https://github.com/SEU-USUARIO/NOME-DO-REPOSITORIO.git](https://github.com/SEU-USUARIO/NOME-DO-REPOSITORIO.git)
   cd NOME-DO-REPOSITORIO
