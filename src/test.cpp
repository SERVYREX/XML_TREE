#include "../pugixml/pugixml.hpp"
#include "../treeADT.h"
#include "../include/XMLReader.h"
#include <iostream>

int main() {
    Tree Arbol;
    Position posRaizGlobal = Arbol.addRoot("Biblioteca_Completa");

    // Crear lector de XML y cargar los libros
    XMLReader lector("../Data_XML");
    lector.cargarLibrosDesdeFolder(Arbol, posRaizGlobal);

    //Arbol.Precursores();
    Arbol.borrar_ratings(4.7);
    Arbol.listar();
    return 0;
}
