#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

class Node {
public:
    std::string element;
    Node* parent;
    std::vector<Node*> children;

    Node(std::string elem, Node* p = nullptr){
        element = elem;
        parent = p;
    }
};

class Position {
private:
    Node* v;
public:
    Position(Node* v = nullptr){
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

class Tree {
private:
    Node* _root;
    int Tsize;

    // Funcion recursiva auxiliar para recolectar posiciones (recorrido pre-order)
    void preorderPositions(Node* v, std::vector<Position>& pos) const {
        if (v == nullptr) return;
        pos.push_back(Position(v));
        for (Node* child : v->children) {
            preorderPositions(child, pos);
        }
    }

    // Funcion auxiliar para eliminar recursivamente un nodo y sus descendientes
    int removeSubtree(Node* v) {
        if (v == nullptr) return 0;
        int count = 1; // contar este nodo
        for (Node* child : v->children) {
            count += removeSubtree(child);
        }
        delete v;
        return count;
    }

    // Funcion auxiliar para imprimir el arbol recursivamente
    void printTreeHelper(Node* v, int depth) const {
        if (v == nullptr) return;
        
        // Imprimir indentación
        for (int i = 0; i < depth; ++i) {
            std::cout << "  ";
        }
        
        // Imprimir el elemento
        std::cout << "├─ " << v->element << "\n";
        
        // Imprimir hijos
        for (Node* child : v->children) {
            printTreeHelper(child, depth + 1);
        }
    }

public:
    Tree(){
        _root = nullptr;
        Tsize = 0;
    }

    ~Tree() {
        if (_root != nullptr) {
            removeSubtree(_root);
        }
    }

    int size() const {
        return Tsize;
    }

    bool isEmpty() const {
        return Tsize == 0;
    }

    // Retorna un vector con los elementos (strings)
    std::vector<std::string> elements() const {
        std::vector<std::string> elems;
        std::vector<Position> pos = positions();
        for (Position p : pos) {
            elems.push_back(p.element());
        }
        return elems;
    }

    // Retorna un vector con las posiciones
    std::vector<Position> positions() const {
        std::vector<Position> pos;
        preorderPositions(_root, pos);
        return pos;
    }

    Position root() const {
        return Position(_root);
    }

    Position parent(Position p) const {
        return Position(p.getNode()->parent);
    }

    std::vector<Position> children(Position p) const {
        std::vector<Position> childPositions;
        for (Node* child : p.getNode()->children) {
            childPositions.push_back(Position(child));
        }
        return childPositions;
    }

    bool isInternal(Position p) const {
        return !p.getNode()->children.empty(); // Es interno si TIENE hijos
    }

    bool isExternal(Position p) const {
        return p.getNode()->children.empty(); // Es externo si NO TIENE hijos
    }

    bool isRoot(Position p) const {
        return p.getNode() == _root;
    }

    std::string replace(Position p, std::string o) {
        std::string oldElement = p.element();
        p.getNode()->element = o;
        return oldElement; // Retorna el string que fue reemplazado
    }

    Position addRoot(std::string e) {
        if (!isEmpty()) throw std::runtime_error("El arbol ya tiene raiz");
        _root = new Node(e);
        Tsize++;
        return Position(_root);
    }

    Position addChild(Position p, std::string e) {
        Node* v = p.getNode();
        Node* child = new Node(e, v);
        v->children.push_back(child);
        Tsize++;
        return Position(child);
    }

    // Elimina un nodo y todos sus descendientes del arbol
    void remove(Position p) {
        Node* v = p.getNode();
        
        if (v == nullptr) {
            throw std::runtime_error("No se puede eliminar una posicion nula");
        }
        
        if (isRoot(p)) {
            throw std::runtime_error("No se puede eliminar la raiz");
        }
        
        Node* parentNode = v->parent;
        
        // Buscar y eliminar v de la lista de hijos del padre
        auto it = std::find(parentNode->children.begin(), parentNode->children.end(), v);
        if (it != parentNode->children.end()) {
            parentNode->children.erase(it);
        }
        
        // Eliminar recursivamente el nodo y sus descendientes
        int removed = removeSubtree(v);
        Tsize -= removed;
    }

    // Imprime el arbol
    void printTree() const {
        if (isEmpty()) {
            std::cout << "El arbol está vacío.\n";
            return;
        }
        printTreeHelper(_root, 0);
    }
};