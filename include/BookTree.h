#ifndef BOOKTREE_H
#define BOOKTREE_H

#include "TreeADT.h"

// Clase que extiende el árbol base e incorpora las funciones requeridas
class BookTree: public Tree {
public:
    // Mismos constructores heredados de Tree
    using Tree::Tree;

    // Recorre el árbol en pre-order e imprime solo los IDS de cada libro
    void listar() const;

    // Elimina todos los nodos libro cuyo rating sea <= r
    void borrar_ratings(float r);

    // Analiza y muestra los ids que cumplen la condición de precursores
    void Precursores();
};

#endif
