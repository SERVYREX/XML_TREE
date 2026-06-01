#include "../include/TreeADT.h"
#include "../include/BookTree.h"
// Recorre el árbol en pre-order e imprime solo los IDS de cada libro
void BookTree::listar() const {
    // Obtenemos todas las posiciones del árbol (ya vienen en pre-order)
    std::vector<Position> pos = positions();
    
    for (Position p : pos) {
        // Filtramos para encontrar solo los nodos de categoría "ID"
        if (p.element() == "ID") {
            // El valor del ID está en el primer hijo del nodo
            if (!p.getNode()->children.empty()) {
                std::cout << p.getNode()->children[0]->element << "\n";
            }
        }
    }
}

// Proposito: Elimina todos los nodos libro cuyo rating sea <= r
// Parametro:
//    - r: float, Libros con rating <= r seran eliminados
void BookTree::borrar_ratings(float r) {
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

void BookTree::Precursores() {
    actualizarArbol();
    
    for(Position p : posiciones){
        if(p.element() == "ID"){
            std::string id = children(p)[0].element();
            std::string year;

            Position libro_id = parent(p);
            
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
