#include "pugixml.hpp"
#include "treeADT.h"
#include "XMLReader.h"
#include <iostream>

int main() {
    Tree Arbol;
    Position posRaizGlobal = Arbol.addRoot("Biblioteca_Completa");

    // Crear lector de XML y cargar los libros
    XMLReader lector("../books_xml");
    int archivosProcesados = lector.cargarLibrosDesdeFolder(Arbol, posRaizGlobal);

    // Solo listar IDs de precursores
    Arbol.Precursores();
    
    return 0;
}
