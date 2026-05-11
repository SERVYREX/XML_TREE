#include "pugixml.hpp"
#include "treeADT.h"
#include <iostream>
#include <filesystem> // libreria para manejar archivos y directorios

namespace fs = std::filesystem;

int main() {
    // Instanciamos el Arbol y le ponemos una raiz global
    Tree Arbol;
    Position posRaizGlobal = Arbol.addRoot("Biblioteca_Completa");

    // Ruta a tu carpeta con los 10000 XML
    std::string ruta_carpeta = "../books_xml";

    int archivosProcesados = 0;

    // Iteramos por cada archivo dentro de la carpeta
    for (const auto& entry : fs::directory_iterator(ruta_carpeta)) {
        
        // Filtramos para que solo lea archivos .xml
        if (entry.is_regular_file() && entry.path().extension() == ".xml") {
            
            pugi::xml_document doc;
            // Cargamos el archivo actual entry.path().c_str() nos da la ruta exacta
            pugi::xml_parse_result result = doc.load_file(entry.path().c_str());

            if (result) {
                // Buscamos el libro principal del archivo actual
                pugi::xml_node main_book = doc.select_node("//GoodreadsResponse/book").node();
                
                // Creamos un nodo padre para este libro especifico colgando de la raiz global
                Position posLibro = Arbol.addChild(posRaizGlobal, "Libro_" + std::to_string(archivosProcesados));

                // Metemos los datos
                Arbol.addChild(posLibro, "ID: " + std::string(main_book.child_value("id")));
                Arbol.addChild(posLibro, "Titulo: " + std::string(main_book.child_value("title")));
                Arbol.addChild(posLibro, "ISBN: " + std::string(main_book.child_value("isbn")));
                Arbol.addChild(posLibro, "Ano: " + std::string(main_book.child_value("publication_year")));
                Arbol.addChild(posLibro, "Idioma: " + std::string(main_book.child_value("language_code")));
                Arbol.addChild(posLibro, "Descripcion: " + std::string(main_book.child_value("description")));
                Arbol.addChild(posLibro, "Rating_Promedio: " + std::string(main_book.child_value("average_rating")));
                Arbol.addChild(posLibro, "Numero_paginas: " + std::string(main_book.child_value("num_pages")));

                // Buscamos los similares de este archivo
                Position posSimilares = Arbol.addChild(posLibro, "LibrosSimilares");
                pugi::xpath_node_set similares = doc.select_nodes("//similar_books/book");

                for (pugi::xpath_node nodo : similares) {
                    pugi::xml_node libro = nodo.node();
                    Position posLibrito = Arbol.addChild(posSimilares, "LibroSimilar");
                    Arbol.addChild(posLibrito, "ISBN: " + std::string(libro.child_value("isbn")));
                    Arbol.addChild(posLibrito, "Ano: " + std::string(libro.child_value("publication_year")));
                    Arbol.addChild(posLibrito, "titulo: " + std::string(libro.child_value("title")));
                }
                archivosProcesados++;
            }
        }
    }
    // 4. Resumen final
    std::cout << "\n=== ARBOL CONSTRUIDO EXITOSAMENTE ===\n";
    std::cout << "Libros principales procesados: " << archivosProcesados << "\n";
    std::cout << "Total de nodos en el arbol: " << Arbol.size() << "\n\n";
    
    // 5. Mostrar la estructura del árbol
    std::cout << "=== ESTRUCTURA DEL ARBOL ===\n";
   Arbol. borrar_ratings(4.76);
    Arbol.printTree();
    return 0;
}
