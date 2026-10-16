#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <GL/gl.h>
#include "HalfEdge.h"

class Mesh {
public:
    std::vector<Vertex*> vertices;
    std::vector<Face*> faces;
    std::vector<HalfEdge*> halfEdges;
    std::vector<std::vector<int>> faceIndices;

    Mesh() {}
    
    ~Mesh() {
        for (auto v : vertices) delete v;
        for (auto f : faces) delete f;
        for (auto he : halfEdges) delete he;
    }

    bool loadObj(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v") {
                float x, y, z;
                iss >> x >> y >> z;
                vertices.push_back(new Vertex(x, y, z));
            } 
            else if (prefix == "f") {
                std::vector<int> currentFace;
                std::string vertexStr;
                
                while (iss >> vertexStr) {
                    size_t pos = vertexStr.find('/');
                    int vIdx = std::stoi(vertexStr.substr(0, pos)) - 1; // obj eh 1-based
                    currentFace.push_back(vIdx);
                }
                faceIndices.push_back(currentFace);
            }
        }
        file.close();
        return true;
    }

    void buildTopology() {
        std::map<std::pair<int, int>, HalfEdge*> edgeMap;

        for (const auto& indices : faceIndices) {
            int n = indices.size();
            if (n < 3) continue;

            Face* newFace = new Face();
            faces.push_back(newFace);
            std::vector<HalfEdge*> faceEdges;

            for (int i = 0; i < n; ++i) {
                HalfEdge* he = new HalfEdge();
                halfEdges.push_back(he);
                faceEdges.push_back(he);

                int currIdx = indices[i];
                int nextIdx = indices[(i + 1) % n];

                he->origin = vertices[currIdx];
                he->face = newFace;

                if (!he->origin->edge) he->origin->edge = he;

                // conecta as gemeas usando o map
                std::pair<int, int> twinKey = {nextIdx, currIdx};
                if (edgeMap.count(twinKey)) {
                    HalfEdge* twinEdge = edgeMap[twinKey];
                    he->twin = twinEdge;
                    twinEdge->twin = he;
                } else {
                    edgeMap[{currIdx, nextIdx}] = he;
                }
            }

            // arruma next e prev
            for (int i = 0; i < n; ++i) {
                faceEdges[i]->next = faceEdges[(i + 1) % n];
                faceEdges[i]->prev = faceEdges[(i - 1 + n) % n];
            }

            newFace->edge = faceEdges[0];
        }
    }

    void drawLineEquation(float x1, float y1, float x2, float y2) {
        float dx = x2 - x1;
        float dy = y2 - y1;
        float step = 0.005f; 

        glBegin(GL_POINTS);

        if (std::abs(dx) >= std::abs(dy)) {
            if (x1 > x2) { std::swap(x1, x2); std::swap(y1, y2); }
            float m = dy / dx;
            float b = y1 - m * x1;
        
            for (float x = x1; x <= x2; x += step) {
                glVertex2f(x, m * x + b);
            }
        } else {
            if (y1 > y2) { std::swap(x1, x2); std::swap(y1, y2); }
        
            if (dx == 0.0f) {
                for (float y = y1; y <= y2; y += step) glVertex2f(x1, y);
            } else {
                float m = dy / dx;
                float b = y1 - m * x1;
                for (float y = y1; y <= y2; y += step) {
                    glVertex2f((y - b) / m, y);
                }
            }
        }
        glEnd();
    }

    void renderMesh() {
        glColor3f(1.0f, 1.0f, 1.0f);
        for (HalfEdge* he : halfEdges) {
            // checa o endereco pra nao desenhar a mesma linha 2x
            if (!he->twin || he < he->twin) {
                Vertex* v1 = he->origin;
                Vertex* v2 = he->next->origin; 
                drawLineEquation(v1->x, v1->y, v2->x, v2->y);
            }
        }
    }

    // helpers pra printar os IDs nas consultas
    int getFaceID(Face* f) { return f ? std::distance(faces.begin(), std::find(faces.begin(), faces.end(), f)) : -1; }
    int getVertexID(Vertex* v) { return v ? std::distance(vertices.begin(), std::find(vertices.begin(), vertices.end(), v)) : -1; }
    int getEdgeID(HalfEdge* he) { return he ? std::distance(halfEdges.begin(), std::find(halfEdges.begin(), halfEdges.end(), he)) : -1; }

    void consultaFace(Face* f) {
        if (!f) return;
        std::cout << "\n[Face " << getFaceID(f) << "]\nVertices: ";
        HalfEdge* curr = f->edge;
        do {
            std::cout << getVertexID(curr->origin) << " ";
            curr = curr->next;
        } while (curr != f->edge);
        
        std::cout << "\nArestas: ";
        curr = f->edge;
        do {
            std::cout << getEdgeID(curr) << " ";
            curr = curr->next;
        } while (curr != f->edge);

        std::cout << "\nFaces adjacentes: ";
        curr = f->edge;
        do {
            if (curr->twin && curr->twin->face) std::cout << getFaceID(curr->twin->face) << " ";
            curr = curr->next;
        } while (curr != f->edge);
        std::cout << "\n";
    }

    void consultaAresta(HalfEdge* he) {
        if (!he) return;
        std::cout << "\n[Aresta " << getEdgeID(he) << "]\nFaces adjacentes: " << getFaceID(he->face);
        if (he->twin) std::cout << ", " << getFaceID(he->twin->face);
        std::cout << "\nVertices: inicio " << getVertexID(he->origin) << ", fim " << getVertexID(he->next->origin);
        std::cout << "\nArestas adjacentes: " << getEdgeID(he->prev) << " (prev), " << getEdgeID(he->next) << " (next)\n";
    }

    void consultaVertice(Vertex* v) {
        if (!v) return;
        std::cout << "\n[Vertice " << getVertexID(v) << "]\nArestas incidentes: ";
        
        for (HalfEdge* he : halfEdges) {
            if (he->origin == v) {
                std::cout << getEdgeID(he) << " ";
            }
        }

        std::cout << "\nFaces adjacentes: ";
        for (HalfEdge* he : halfEdges) {
            if (he->origin == v && he->face) {
                std::cout << getFaceID(he->face) << " ";
            }
        }
        std::cout << "\n";
    }
};

#endif