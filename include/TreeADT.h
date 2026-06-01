#ifndef TREEADT_H
#define TREEADT_H

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cstdlib>

// Nodo del árbol: contiene un elemento y referencias a padre/hijos
class Node {
public:
    std::string element;
    Node* parent;
    std::vector<Node*> children;

    Node(std::string elem, Node* p = nullptr) {
        element = elem;
        parent = p;
    }
};

// Representa una posición en el árbol (abstrae el puntero a Node)
class Position {
private:
    Node* v;
public:
    Position(Node* v = nullptr) {
        this->v = v;
    }
    
    // Para obtener el elemento de esta posición
    std::string& element() const {
        if (!v) throw std::runtime_error("Posicion nula");
        return v->element;
    }
    
    // Metodo interno para que el arbol pueda acceder al nodo
    Node* getNode() const { 
        return v;
    }
};


// Arbol general con nodos que pueden tener cualquier número de hijos
class Tree {
protected: 
    Node* _root;
    int Tsize;
    std::vector<Position> posiciones;
    bool arbol_modificado; 
    
    // Recorrea posiciones mediante recorrido pre-order
    void preorderPositions(Node* v, std::vector<Position>& pos) const;

    // Elimina un nodo y todos sus descendientes recursivamente
    int removeSubtree(Node* v);

public:
    // Constructor: inicializa arbol vacio
    Tree();

    // Destructor: libera memoria del arbol
    virtual ~Tree();

    // Retorna numero total de nodos
    int size() const;

    // Verifica si el arbol esta vacio
    bool isEmpty() const;

    // Retorna elementos de todos los nodos en pre-order
    std::vector<std::string> elements() const;

    // Retorna todas las posiciones del arbol en pre-order
    std::vector<Position> positions() const;

    // Retorna la raíz del arbol
    Position root() const;

    // Retorna el padre de una posición
    Position parent(Position p) const;

    // Retorna los hijos de una posición
    std::vector<Position> children(Position p) const;

    // Verifica si es nodo interno (tiene hijos)
    bool isInternal(Position p) const;

    // Verifica si es nodo hoja (sin hijos)
    bool isExternal(Position p) const;

    // Verifica si es la raíz del árbol
    bool isRoot(Position p) const;

    // Reemplaza el elemento de una posición
    std::string replace(Position p, std::string o);

    // Crea la raíz del árbol
    Position addRoot(std::string e);

    // Agrega un hijo a una posición
    Position addChild(Position p, std::string e);

    // Elimina un nodo y sus descendientes
    void remove(Position p);

    // Actualiza caché de posiciones si el árbol ha sido modificado
    void actualizarArbol();
};

#endif
