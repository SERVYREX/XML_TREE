#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cstdlib>
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


 void listar(Node* v, int depth) const {

    if (v == nullptr) return;

    // Categorias que queremos mostrar
    if (v->element == "ID" ||
        v->element == "Titulo" ||
        v->element == "Rating_Promedio") {

        // imprimir indentacion
        for (int i = 0; i < depth; ++i) {
            std::cout << "  ";
        }

        // imprimir nombre categoria
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
    listar(_root, 0);
  }


  void borrar_ratings(float r) {

    std::vector<Position> pos = positions();
    std::vector<Position> eliminar;

    for (Position p : pos) {

      // Buscar el nodo categoria
      if (p.element() == "Rating_Promedio") {

	std::vector<Position> hijo = children(p);

	float rating = std::stof(hijo[0].element());

	if (rating <= r) {

	  // padre del rating = libro
	  Position libro = parent(p);

	  if (!isRoot(libro)) {
	    eliminar.push_back(libro);
	  }
	}
                
      }
    }
 

  // eliminar despues para evitar invalidar recorridos
  for (Position p : eliminar) {
    remove(p);
  }
}



  
  bool precursores(std::string id){
    std::vector<Position> pos = positions();
    Position libro_id;
    std::string year;
    
    for(Position p: pos){
      if( p.element() == id){
	if(parent(p).element() == "ID"){
	
	libro_id = parent(parent(p));
	break;
	}
      }
    }
    for(Position pos: children(libro_id)){
      if(pos.element() == "year"){
	year = children(pos)[0].element();
      }
    }
     for(Position pos: children(libro_id)){
      if(pos.element() == "LibrosSimilares"){
	for(Position book : children(pos)){
	  for(Position atribute : children(book)){
	    if(atribute.element() == "year"){
	      std::string similarYear = children(atribute)[0].element();
	      if(similarYear == "" || std::stoi(year) > std::stoi(similarYear)){
		return false;
	      }
	    }
	  }	
	}
      }
    }
    return true;
  }
};

