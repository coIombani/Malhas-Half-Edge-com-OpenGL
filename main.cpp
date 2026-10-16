#include <GL/glut.h>
#include <iostream>
#include "Mesh.h"

Mesh malha;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    malha.renderMesh();
    glFlush(); 
}

void init() {
    glClearColor(0, 0, 0, 1);
    glColor3f(1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 5.0, -1.0, 5.0); 
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Faltou passar o .obj\n";
        return 1;
    }

    if (!malha.loadObj(argv[1])) {
        std::cout << "Erro ao ler arquivo\n";
        return 1;
    }
    malha.buildTopology();

    char op;
    int id = 0;

    // menu no terminal antes de abrir a janela
    while (op != 'e') {
        std::cin >> op;

        if (op == 'f') {
            std::cout << "ID da face: ";
            std::cin >> id;
            if (id >= 0 && id < malha.faces.size()) malha.consultaFace(malha.faces[id]);
            else std::cout << "Face invalida\n";
        } 
        else if (op == 'a') {
            std::cout << "ID da aresta: ";
            std::cin >> id;
            if (id >= 0 && id < malha.halfEdges.size()) malha.consultaAresta(malha.halfEdges[id]);
            else std::cout << "Aresta invalida\n";
        } 
        else if (op == 'v') {
            std::cout << "ID do vertice: ";
            std::cin >> id;
            if (id >= 0 && id < malha.vertices.size()) malha.consultaVertice(malha.vertices[id]);
            else std::cout << "Vertice invalido\n";
        }
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Trabalho 2");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}