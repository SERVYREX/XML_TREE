#include "XMLReader.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

XMLReader::XMLReader(const std::string& ruta) : carpetaRuta(ruta) {}

void XMLReader::setCarpetaRuta(const std::string& ruta) {
    carpetaRuta = ruta;
}

void XMLReader::procesarLibroPrincipal(pugi::xml_node main_book, Tree& arbol, Position posLibro, [[maybe_unused]] int numeroLibro) {
    // Metemos los datos: Primero creamos el nodo de la categoría y luego le añadimos el valor como hijo
    Position posId = arbol.addChild(posLibro, "ID");
    arbol.addChild(posId, std::string(main_book.child_value("id")));

    Position posTitulo = arbol.addChild(posLibro, "Titulo");
    arbol.addChild(posTitulo, std::string(main_book.child_value("title")));

    Position posIsbn = arbol.addChild(posLibro, "ISBN");
    arbol.addChild(posIsbn, std::string(main_book.child_value("isbn")));

    Position posAno = arbol.addChild(posLibro, "year");
    arbol.addChild(posAno, std::string(main_book.child_value("publication_year")));

    Position posIdioma = arbol.addChild(posLibro, "Idioma");
    arbol.addChild(posIdioma, std::string(main_book.child_value("language_code")));

    Position posDesc = arbol.addChild(posLibro, "Descripcion");
    arbol.addChild(posDesc, std::string(main_book.child_value("description")));

    Position posRating = arbol.addChild(posLibro, "Rating_Promedio");
    arbol.addChild(posRating, std::string(main_book.child_value("average_rating")));

    Position posPags = arbol.addChild(posLibro, "Numero_paginas");
    arbol.addChild(posPags, std::string(main_book.child_value("num_pages")));
}

void XMLReader::procesarLibrosSimilares(pugi::xml_document& doc, Tree& arbol, Position posSimilares) {
    // Buscamos los similares de este archivo
    pugi::xpath_node_set similares = doc.select_nodes("//similar_books/book");

    for (pugi::xpath_node nodo : similares) {
        pugi::xml_node libro = nodo.node();
        Position posLibrito = arbol.addChild(posSimilares, "LibroSimilar");
        
        // Aplicamos la misma lógica para los libros similares
        Position posSimIsbn = arbol.addChild(posLibrito, "ISBN");
        arbol.addChild(posSimIsbn, std::string(libro.child_value("isbn")));

        Position posSimAno = arbol.addChild(posLibrito, "year");
        arbol.addChild(posSimAno, std::string(libro.child_value("publication_year")));

        Position posSimTitulo = arbol.addChild(posLibrito, "titulo");
        arbol.addChild(posSimTitulo, std::string(libro.child_value("title")));
    }
}

int XMLReader::cargarLibrosDesdeFolder(Tree& arbol, Position posRaiz) {
    int archivosProcesados = 0;

    // Iteramos por cada archivo dentro de la carpeta
    for (const auto& entry : fs::directory_iterator(carpetaRuta)) {
        // Filtramos para que solo lea archivos .xml
        if (entry.is_regular_file() && entry.path().extension() == ".xml") {
            
            pugi::xml_document doc;
            pugi::xml_parse_result result = doc.load_file(entry.path().c_str());

            if (result) {
                // Buscamos el libro principal del archivo actual
                pugi::xml_node main_book = doc.select_node("//GoodreadsResponse/book").node();
                
                // Creamos un nodo padre para este libro específico
                Position posLibro = arbol.addChild(posRaiz, "Libro_" + std::to_string(archivosProcesados));

                // Procesamos el libro principal
                procesarLibroPrincipal(main_book, arbol, posLibro, archivosProcesados);

                // Procesamos los libros similares
                Position posSimilares = arbol.addChild(posLibro, "LibrosSimilares");
                procesarLibrosSimilares(doc, arbol, posSimilares);

                archivosProcesados++;
            }
        }
    }
    return archivosProcesados;
}
