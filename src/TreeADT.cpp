#include "../include/TreeADT.h"


//Pasamos como parametro un vector que será llenado con todos los nodos en "preorder"

void Tree::preorderPositions(Node* v, std::vector<Position>& pos) const {
    if (v == nullptr) return;
    pos.push_back(Position(v));
    for (Node* child : v->children) {
        preorderPositions(child, pos);
    }
}

// Pasamos un nodo y eliminamos de forma recursiva todos sus hijos, eliminando el subarbol asociado
int Tree::removeSubtree(Node* v) {
    if (v == nullptr) return 0;
    int count = 1; 
    for (Node* child : v->children) {
        count += removeSubtree(child);
    }
    delete v;
    return count;
}

Tree::Tree() {
    _root = nullptr;
    Tsize = 0;
    arbol_modificado = true;
}

Tree::~Tree() {
    if (_root != nullptr) {
        removeSubtree(_root);
    }
}

int Tree::size() const {
    return Tsize;
}

bool Tree::isEmpty() const {
    return Tsize == 0;
}

std::vector<std::string> Tree::elements() const {
    std::vector<std::string> elems;
    std::vector<Position> pos = positions();
    for (Position p : pos) {
        elems.push_back(p.element());
    }
    return elems;
}

//Retorno el vector de posiciones en preorder

std::vector<Position> Tree::positions() const {
    std::vector<Position> pos;
    preorderPositions(_root, pos);
    return pos;
}

Position Tree::root() const {
    return Position(_root);
}

Position Tree::parent(Position p) const {
    return Position(p.getNode()->parent);
}


//Obtenemos todos los hijos de un nodo "p", devolviendo un vector con los nodos correspondientes 
std::vector<Position> Tree::children(Position p) const {
    std::vector<Position> childPositions;
    for (Node* child : p.getNode()->children) {
        childPositions.push_back(Position(child));
    }
    return childPositions;
}

bool Tree::isInternal(Position p) const {
    return !p.getNode()->children.empty();
}

bool Tree::isExternal(Position p) const {
    return p.getNode()->children.empty();
}

bool Tree::isRoot(Position p) const {
    return p.getNode() == _root;
}

std::string Tree::replace(Position p, std::string o) {
    std::string oldElement = p.element();
    p.getNode()->element = o;
    return oldElement;
}

Position Tree::addRoot(std::string e) {
    if (!isEmpty()) throw std::runtime_error("El arbol ya tiene raiz");
    _root = new Node(e);
    Tsize++;
    arbol_modificado = true;
    return Position(_root);
}

//Creamos un nuevo nodo y lo conectamos con el nodo "p" dado y le asignamos el valor "e"
Position Tree::addChild(Position p, std::string e) {
    Node* v = p.getNode();
    Node* child = new Node(e, v);
    v->children.push_back(child);
    Tsize++;
    arbol_modificado = true;
    return Position(child);
}

//Elimina un nodo "p" y todos sus hijos

void Tree::remove(Position p) {
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
    arbol_modificado = true;
}

//Actualizamos el vector de posiciones del arbol, en caso de que haya sido modificado
void Tree::actualizarArbol() {
    if (arbol_modificado) {
        posiciones = positions();
        arbol_modificado = false; 
    }
}
