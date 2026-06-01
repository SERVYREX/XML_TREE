#include "../pugixml/pugixml.hpp"
#include "../include/TreeADT.h"
#include "../include/XMLReader.h"
#include "../include/BookTree.h"
#include <iostream>

int main() {
  //Creamos el arbol que vamos a usar y asignamos su raiz
    BookTree Arbol;
    Position posRaizGlobal = Arbol.addRoot("Biblioteca_Completa");
    float r = 4.7;
    // Crear lector de XML y cargar los libros
    XMLReader lector("../Data_XML");
    lector.cargarLibrosDesdeFolder(Arbol, posRaizGlobal);

    
    std::cout << "\n============================================================\n";
    std::cout << "                    LIBROS PRECURSORES\n";
    std::cout << "============================================================\n\n";
    Arbol.precursores();


    Arbol.borrar_ratings(r);

    std::cout << "\n=================================================================================\n";
    std::cout << "                  ARBOL RESULTANTE, libros eliminados en base al rating: " << r << "\n";
    std::cout << "==================================================================================\n\n";
    Arbol.listar();
    return 0;
}
