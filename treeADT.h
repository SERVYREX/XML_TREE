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

  Node(std::string elem, Node* p = nullptr){
    element = elem;
    parent = p;
  }
};

// Representa una posición en el árbol (abstrae el puntero a Node)
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

// Arbol general con nodos que pueden tener cualquier número de hijos
class Tree {
private:
  Node* _root;
  int Tsize;
  std::vector<Position> posiciones;
  bool arbol_modificado; 
  // Recorre posiciones mediante recorrido pre-order
  void preorderPositions(Node* v, std::vector<Position>& pos) const {
    if (v == nullptr) return;
    pos.push_back(Position(v));
    for (Node* child : v->children) {
      preorderPositions(child, pos);
    }
  }

  // Elimina un nodo y todos sus descendientes recursivamente
  int removeSubtree(Node* v) {
    if (v == nullptr) return 0;
    int count = 1; // contar este nodo
    for (Node* child : v->children) {
      count += removeSubtree(child);
    }
    delete v;
    return count;
  }

  // Recorre el arbol en pre-order e imprime solo datos relevantes de libros
  // Parametros:
  //   - v: puntero al nodo actual
  //   - depth: profundidad actual para formatear indentacion
  // Comportamiento:
  //   - Filtra y muestra solo: ID, Titulo y Rating promedio
  //   - Cada categoria se imprime con su valor
  //   - La indentacion muestra la estructura jerarquica
  void listar(Node* v, int depth) const {
    if (v == nullptr) return;
    // Filtrar: mostrar solo categorias de interes
    if (v->element == "ID" || v->element == "Titulo" || v->element == "Rating_Promedio") {
        // imprimir indentacion
        for (int i = 0; i < depth; ++i) {
            std::cout << "  ";
        }

        // imprimir nombre dato
        std::cout << v->element;

        // imprimir valor si existe hijo
        if (!v->children.empty()) {
            std::cout << ": " << v->children[0]->element;
        }
        std::cout << "\n";
    }

    // seguir recorriendo
    for (Node* child : v->children) {
        listar(child, depth + 1);
    }
}

public:
  // Constructor: inicializa arbol vacio
  Tree(){
    _root = nullptr;
    Tsize = 0;
    arbol_modificado = true;
  }

  // Destructor: libera memoria del arbol
  ~Tree() {
    if (_root != nullptr) {
      removeSubtree(_root);
    }
  }

  // Retorna numero total de nodos
  int size() const {
    return Tsize;
  }

  // Verifica si el arbol esta vacio
  bool isEmpty() const {
    return Tsize == 0;
  }

  // Retorna elementos de todos los nodos en pre-order
  std::vector<std::string> elements() const {
    std::vector<std::string> elems;
    std::vector<Position> pos = positions();
    for (Position p : pos) {
      elems.push_back(p.element());
    }
    return elems;
  }

  // Retorna todas las posiciones del arbol en pre-order
  std::vector<Position> positions() const {
    std::vector<Position> pos;
    preorderPositions(_root, pos);
    return pos;
  }

  // Retorna la raíz del arbol
  Position root() const {
    return Position(_root);
  }

  // Retorna el padre de una posición
  Position parent(Position p) const {
    return Position(p.getNode()->parent);
  }

  // Retorna los hijos de una posición
  std::vector<Position> children(Position p) const {
    std::vector<Position> childPositions;
    for (Node* child : p.getNode()->children) {
      childPositions.push_back(Position(child));
    }
    return childPositions;
  }

  // Verifica si es nodo interno (tiene hijos)
  bool isInternal(Position p) const {
    return !p.getNode()->children.empty();
  }

  // Verifica si es nodo hoja (sin hijos)
  bool isExternal(Position p) const {
    return p.getNode()->children.empty();
  }

  // Verifica si es la raíz del árbol
  bool isRoot(Position p) const {
    return p.getNode() == _root;
  }

  // Reemplaza el elemento de una posición
  std::string replace(Position p, std::string o) {
    std::string oldElement = p.element();
    p.getNode()->element = o;
    return oldElement;
  }

  // Crea la raíz del árbol
  Position addRoot(std::string e) {
    if (!isEmpty()) throw std::runtime_error("El arbol ya tiene raiz");
    _root = new Node(e);
    Tsize++;
    arbol_modificado = true;
    return Position(_root);
  }

  // Agrega un hijo a una posición
  Position addChild(Position p, std::string e) {
    Node* v = p.getNode();
    Node* child = new Node(e, v);
    v->children.push_back(child);
    Tsize++;
    arbol_modificado = true;
    return Position(child);
  }

  // Elimina un nodo y sus descendientes
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
    arbol_modificado = true;
  }

  // Imprime el árbol mostrando categorías relevantes
  void printTree() const {
    if (isEmpty()) {
      std::cout << "El arbol está vacío.\n";
      return;
    }
    listar(_root, 0);
  }

  // Actualiza caché de posiciones si el árbol ha sido modificado
  void actualizarArbol() {
    if (arbol_modificado) {
      posiciones = positions();
      arbol_modificado = false; 
    }
  }

  // Proposito: Elimina todos los nodos libro cuyo rating sea <= r
  // Parametro:
  //   - r: float, Libros con rating <= r seran eliminados
  void borrar_ratings(float r) {
    // Obtener todas las posiciones del arbol
    std::vector<Position> pos = positions();
    std::vector<Position> eliminar;

    // Recorrer posiciones y filtrar libros a eliminar
    for (Position p : pos) {
      // Buscar nodos de categoria Rating_Promedio
      if (p.element() == "Rating_Promedio") {
        std::vector<Position> hijo = children(p);
        float rating = std::stof(hijo[0].element());  // Extraer valor del rating

        // Comparar con r
        if (rating <= r) {
          // Obtener nodo padre (libro que contiene este rating)
          Position libro = parent(p);
          if (!isRoot(libro)) {
            eliminar.push_back(libro); // Marcar libro para eliminacion
          }
        }
      }
    }
    // Eliminar libros despues del recorrido
    for (Position p : eliminar) {
      remove(p);
    }
  }

  void Precursores() {
    actualizarArbol();
    
    for(Position p : posiciones){
      if(p.element() == "ID"){
        std::string id = children(p)[0].element();
        Position libro_id;
        std::string year;
        
        // Encontrar el nodo del libro correspondiente a este ID
        for(Position pos: posiciones){
          if(pos.element() == id){
            if(parent(pos).element() == "ID"){
              libro_id = parent(parent(pos));
              break;
            }
          }
        }

        // Buscamos el año del libro
        for(Position pos: children(libro_id)){
          if(pos.element() == "year"){
            year = children(pos)[0].element();
          }
        }

        // Comparamos con los libros similares
        bool esPrecursorFlag = true;
        for(Position pos: children(libro_id)){
          if(pos.element() == "LibrosSimilares"){
            for(Position book : children(pos)){
              for(Position atribute : children(book)){
                if(atribute.element() == "year"){
                  std::string similarYear = children(atribute)[0].element();
                  if(similarYear == "" || year == ""){
                    esPrecursorFlag = false;
                    break;
                  }
                  if(std::stoi(year) >= std::stoi(similarYear)){
                    esPrecursorFlag = false;
                    break;
                  }
                }
              }
              if(!esPrecursorFlag) break;
            }
            if(!esPrecursorFlag) break;
          }
        }
        if(esPrecursorFlag){
          std::cout << id << std::endl;
        }
      }
    }   
  }
};
#endif