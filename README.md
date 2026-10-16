# Malhas com Half-Edge e OpenGL

Trabalho de CGI feito em C++ com OpenGL e GLUT. O programa lê uma malha OBJ, monta a estrutura Half-Edge e permite consultar as relações entre faces, meias-arestas e vértices pelo terminal. Depois, desenha as arestas no plano XY.

## Como executar

Com um compilador C++ e as bibliotecas OpenGL, GLU e GLUT/freeglut instaladas, compile e execute na pasta do projeto. Em Linux:

```bash
g++ -std=c++11 main.cpp -o renderizador -lGL -lGLU -lglut
./renderizador casa.obj
```

No Windows, os caminhos e nomes das bibliotecas dependem da instalação. O arquivo `modelo.obj` é outro exemplo disponível.

No terminal, use `f` para consultar uma face, `a` para uma meia-aresta e `v` para um vértice, informando o ID solicitado. Os IDs começam em zero. Digite `e` para encerrar as consultas e abrir a janela gráfica.

Autores: Lucas Lima e João Guska.
