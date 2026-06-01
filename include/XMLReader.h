#ifndef XMLREADER_H
#define XMLREADER_H

#include "../pugixml/pugixml.hpp"
#include "../treeADT.h"
#include <string>

class XMLReader {
private:
    std::string carpetaRuta;
    
    // Métodos privados para procesar cada libro
    void procesarLibroPrincipal(pugi::xml_node main_book, Tree& arbol, Position posLibro, int numeroLibro);
    void procesarLibrosSimilares(pugi::xml_document& doc, Tree& arbol, Position posSimilares);

public:
    XMLReader(const std::string& ruta = "../books_xml");
    int cargarLibrosDesdeFolder(Tree& arbol, Position posRaiz);
    void setCarpetaRuta(const std::string& ruta);
};

#endif
