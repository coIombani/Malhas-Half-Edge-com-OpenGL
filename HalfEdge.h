#ifndef HALF_EDGE_H
#define HALF_EDGE_H

// 1. Forward declarations para evitar erros de dependência circular no compilador
struct HalfEdge;
struct Face;

// 2. Registro do Vértice
struct Vertex {
    float x, y, z;  // Coordenadas (vamos manter o z para compatibilidade com o .obj, mesmo sendo 2D)
    HalfEdge* edge; // Ponteiro para UMA meia-aresta arbitrária que se origina deste vértice

    // Construtor para facilitar a criação
    Vertex(float x, float y, float z = 0.0f) : x(x), y(y), z(z), edge(nullptr) {}
};

// 3. Registro da Face
struct Face {
    HalfEdge* edge; // Ponteiro para UMA meia-aresta arbitrária que pertence (circunda) a esta face

    Face() : edge(nullptr) {}
};

// 4. Registro da Meia-Aresta (O coração da estrutura)
struct HalfEdge {
    HalfEdge* twin;   // Ponteiro para a meia-aresta gêmea (direção oposta)
    HalfEdge* next;   // Ponteiro para a próxima meia-aresta no ciclo da face
    HalfEdge* prev;   // Ponteiro para a meia-aresta anterior no ciclo da face
    Vertex* origin;   // Ponteiro para o vértice de onde esta meia-aresta parte
    Face* face;       // Ponteiro para a face incidente (a face que esta aresta contorna)

    HalfEdge() : twin(nullptr), next(nullptr), prev(nullptr), origin(nullptr), face(nullptr) {}
};

#endif // HALF_EDGE_H