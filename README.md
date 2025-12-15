# Flappy Bird NES Demake 

![NES](https://img.shields.io/badge/Platform-NES-red)
![Language](https://img.shields.io/badge/Language-C-blue)
![License](https://img.shields.io/badge/License-MIT-green)

> Um *demake* funcional e *open-source* do Flappy Bird para o Nintendo Entertainment System (NES), desenvolvido em C como objeto de estudo de arquitetura de computadores e sistemas embarcados.

[Open this project in 8bitworkshop](http://8bitworkshop.com/redir.html?platform=nes&githubURL=https%3A%2F%2Fgithub.com%2Fvitimbro%2Fflappy_bird&file=flappy_bird.c).

## Sobre o Projeto

Este projeto consiste na implementação do jogo **Flappy Bird** para o hardware do NES (1985). O objetivo principal foi explorar a arquitetura do console e aplicar ferramentas de desenvolvimento modernas (FPGAs e linguagem C) para criar um jogo que respeite as restrições técnicas originais da plataforma.

O jogo roda a **60 FPS estáveis** e foi validado tanto em emuladores quanto em hardware real (via FPGA Tang Nano 20K).

## Funcionalidades

* **Física de Movimento:** Implementação de gravidade e impulso utilizando sistema de subpixels para fluidez.
* **Scrolling Horizontal:** Cenário infinito com *mirroring* vertical e *offscreen updates*.
* **Geração Procedural:** Obstáculos (canos) gerados aleatoriamente com alturas variáveis.
* **Split Screen:** Barra de status fixa (HUD) utilizando a técnica de *Sprite Zero hit*.
* **Performance:** Otimização de escrita na VRAM (buffers) para rodar a 60 FPS sem *glitches* visuais.

## Tecnologias e Ferramentas

O desenvolvimento utilizou uma abordagem moderna para *retro-coding*:

* **Linguagem:** C (compilador [cc65](https://cc65.github.io/))
* **Biblioteca:** [NESlib](https://github.com/clbr/neslib) (para abstração de hardware)
* **IDE:** [8bitworkshop](https://8bitworkshop.com/)
* **Assets:**
    * Gráficos: NES Screen Tool (NESst)
    * Áudio: FamiStudio
* **Hardware de Teste:** FPGA Tang Nano 20K (Core [NESTang](https://github.com/nand2mario/nestang))

## Como Jogar / Executar

### Opção 1: No Navegador (Rápido)
Você pode rodar o código, compilar e jogar diretamente no seu navegador através da IDE 8bitworkshop:

[![Open in 8bitworkshop](https://img.shields.io/badge/Open_in-8bitworkshop-orange)](http://8bitworkshop.com/redir.html?platform=nes&githubURL=https%3A%2F%2Fgithub.com%2Fvitimbro%2Fflappy_bird&file=flappy_bird.c)

### Opção 2: Emulador
1. Baixe o arquivo `.nes` (ROM) na aba de download no 8bitworkshop.
2. Abra com qualquer emulador compatível (recomendado: **Mesen** ou **FCEUX**).

### Opção 3: Hardware Real / FPGA
A ROM é compatível com cartuchos *flashcarts* (como Everdrive) ou implementações FPGA que suportem o mapper NROM (Mapper 0).

## Documentação Acadêmica

Este projeto foi desenvolvido como parte de uma Iniciação Científica na **Universidade Federal do ABC (UFABC)**.
A documentação detalhada do processo de desenvolvimento, explicando passo-a-passo como lidar com as limitações do NES, está disponível na Wiki deste repositório:

[Acessar a Wiki do Projeto](https://github.com/vitimbro/dragons_leap/wiki)

## Autor

**Vitor Vieira Fernandes**
* Projeto de Iniciação Científica - UFABC
* Orientador: Prof. Dr. Mario Alexandre Gazziro
