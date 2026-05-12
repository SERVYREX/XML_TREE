#include "pugixml.hpp"
#include "treeADT.h"
#include <iostream>
#include <filesystem> 

namespace fs = std::filesystem;

int main() {
    // 1. Instanciamos tu Mega Arbol y le ponemos una raiz global
    Tree Arbol;
    Position posRaizGlobal = Arbol.addRoot("Biblioteca_Completa");

    // 2. Ruta a tu carpeta con los 10000 XML (cambia esto por tu ruta real)
    std::string ruta_carpeta = "../books_xml"; 
    
    int archivosProcesados = 0;

    // 3. Iteramos por cada archivo dentro de la carpeta
    for (const auto& entry : fs::directory_iterator(ruta_carpeta)) {
        
        // Filtramos para que solo lea archivos .xml
        if (entry.is_regular_file() && entry.path().extension() == ".xml") {
            
            pugi::xml_document doc;
            pugi::xml_parse_result result = doc.load_file(entry.path().c_str());

            if (result) {
                // Buscamos el libro principal del archivo actual
                pugi::xml_node main_book = doc.select_node("//GoodreadsResponse/book").node();
                
                // Creamos un nodo padre para este libro específico colgando de la raíz global
                Position posLibro = Arbol.addChild(posRaizGlobal, "Libro_" + std::to_string(archivosProcesados));

                // Metemos los datos: Primero creamos el nodo de la categoría, y luego le añadimos el valor como hijo
                Position posId = Arbol.addChild(posLibro, "ID");
                Arbol.addChild(posId, std::string(main_book.child_value("id")));

                Position posTitulo = Arbol.addChild(posLibro, "Titulo");
                Arbol.addChild(posTitulo, std::string(main_book.child_value("title")));

                Position posIsbn = Arbol.addChild(posLibro, "ISBN");
                Arbol.addChild(posIsbn, std::string(main_book.child_value("isbn")));

                Position posAno = Arbol.addChild(posLibro, "Ano");
                Arbol.addChild(posAno, std::string(main_book.child_value("publication_year")));

                Position posIdioma = Arbol.addChild(posLibro, "Idioma");
                Arbol.addChild(posIdioma, std::string(main_book.child_value("language_code")));

                Position posDesc = Arbol.addChild(posLibro, "Descripcion");
                Arbol.addChild(posDesc, std::string(main_book.child_value("description")));

                Position posRating = Arbol.addChild(posLibro, "Rating_Promedio");
                Arbol.addChild(posRating, std::string(main_book.child_value("average_rating")));

                Position posPags = Arbol.addChild(posLibro, "Numero_paginas");
                Arbol.addChild(posPags, std::string(main_book.child_value("num_pages")));

                // Buscamos los similares de este archivo
                Position posSimilares = Arbol.addChild(posLibro, "LibrosSimilares");
                pugi::xpath_node_set similares = doc.select_nodes("//similar_books/book");

                for (pugi::xpath_node nodo : similares) {
                    pugi::xml_node libro = nodo.node();
                    Position posLibrito = Arbol.addChild(posSimilares, "LibroSimilar");
                    
                    // Aplicamos la misma lógica para los libros similares
                    Position posSimIsbn = Arbol.addChild(posLibrito, "ISBN");
                    Arbol.addChild(posSimIsbn, std::string(libro.child_value("isbn")));

                    Position posSimAno = Arbol.addChild(posLibrito, "Ano");
                    Arbol.addChild(posSimAno, std::string(libro.child_value("publication_year")));

                    Position posSimTitulo = Arbol.addChild(posLibrito, "titulo");
                    Arbol.addChild(posSimTitulo, std::string(libro.child_value("title")));
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
    Arbol.borrar_ratings(4.70);
    Arbol.printTree();
    return 0;
}
