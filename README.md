# XML_TREE

## Descripción

**XML_TREE** es un proyecto cuyo objetivo es implementar la estructura de datos **Tree** de forma general, permitiendo leer, almacenar y consultar información proveniente de un dataset en formato XML.

Para la lectura de archivos XML se utilizó la librería **pugixml**, seleccionada por su facilidad para manejar casos borde y datos faltantes dentro del dataset.

---

## Estructura del Proyecto

```text
XML_TREE/
│
├── DATA_XML/     # Dataset XML utilizado por el proyecto
├── include/      # Contratos (ADT) e interfaces
├── src/          # Implementaciones y programa principal
├── pugixml/      # Librería externa para procesamiento XML
└── output/       # Ejecutable generado tras la compilación
```

### Carpetas principales

| Carpeta    | Descripción                                                              |
| ---------- | ------------------------------------------------------------------------ |
| `DATA_XML` | Contiene el dataset XML utilizado para poblar el árbol.                  |
| `include`  | Contiene las definiciones de las estructuras de datos y contratos (ADT). |
| `src`      | Implementaciones del árbol, lector XML y programa principal de pruebas.  |
| `pugixml`  | Librería utilizada para leer y procesar archivos XML.                    |
| `output`   | Carpeta donde se genera el ejecutable compilado.                         |

---

## Dependencias

* Compilador compatible con **C++17**
* Librería **pugixml**

Repositorio oficial de pugixml:

https://github.com/zeux/pugixml

---
## Estructura del Árbol

El árbol almacena la información de todos los libros cargados desde los archivos XML del dataset.

La raíz del árbol contiene un nodo hijo por cada archivo XML procesado. Cada uno de estos nodos representa un libro principal y almacena tanto sus atributos como la información de sus libros similares.

La estructura general es la siguiente:

```text id="vz1bzj"
Raiz
│
├── Libro_0
│   ├── ID
│   │   └── valor
│   ├── Titulo
│   │   └── valor
│   ├── ISBN
│   │   └── valor
│   ├── year
│   │   └── valor
│   ├── Idioma
│   │   └── valor
│   ├── Descripcion
│   │   └── valor
│   ├── Rating_Promedio
│   │   └── valor
│   ├── Numero_paginas
│   │   └── valor
│   └── LibrosSimilares
│       ├── LibroSimilar
│       │   ├── ISBN
│       │   │   └── valor
│       │   ├── year
│       │   │   └── valor
│       │   └── titulo
│       │       └── valor
│       ├── LibroSimilar
│       └── ...
│
├── Libro_1
├── Libro_2
└── ...
```

### Representación de los Datos

Cada atributo de un libro se almacena mediante dos niveles de nodos:

1. Un nodo que representa la categoría del atributo.
2. Un nodo hijo que contiene el valor asociado.

Por ejemplo:

```text id="s6xb3u"
ID
└── 123456

Titulo
└── The Hobbit

Rating_Promedio
└── 4.35
```

Esta representación permite mantener una estructura homogénea para todos los atributos del dataset y facilita la navegación y consulta de la información mediante recorridos sobre el árbol.

### Libros Similares

Los libros similares se almacenan como un subárbol dentro del nodo `LibrosSimilares` de cada libro principal.

A diferencia de los libros principales, los libros similares almacenan únicamente:

* ISBN
* Año de publicación (`year`)
* Título (`titulo`)

ya que son los atributos relevantes para las consultas implementadas en el proyecto.


## Funcionalidades Implementadas

Además de la construcción del árbol a partir del dataset XML, se implementaron las siguientes consultas:

### `listar()`

Imprime por consola los IDs de todos los libros principales presentes actualmente en el árbol.

### `borrar_ratings(r)`

Recibe un parámetro numérico `r` y elimina del árbol todos los libros principales cuyo rating promedio sea menor o igual a dicho valor.

### `precursores()`

Imprime por consola los IDs de los libros principales cuya fecha de publicación sea anterior a la fecha de publicación de cualquiera de sus libros similares.

**Consideración:** Si algún libro involucrado posee una fecha desconocida, no es posible garantizar que sea un precursor, por lo que se descarta automáticamente.

---

## Complejidad de las Operaciones

Sea:

* **n** = número total de nodos del árbol.
* **s** = número total de relaciones de similitud entre libros.

### `listar()`

**Complejidad temporal:** `O(n)`

La función obtiene todas las posiciones del árbol en recorrido preorden y visita cada nodo exactamente una vez para determinar si corresponde a un identificador (`ID`) de un libro.

---

### `borrar_ratings(r)`

**Complejidad temporal:** `O(n)`

1. Se realiza un recorrido completo del árbol para localizar todos los nodos de tipo `Rating_Promedio`.
2. Los libros que cumplen la condición se almacenan temporalmente.
3. Posteriormente se eliminan los subárboles correspondientes.

Aunque la operación de eliminación recorre los nodos del subárbol eliminado, la suma total de nodos eliminados nunca puede superar el número total de nodos del árbol. Por ello, el costo total permanece acotado por `O(n)`.

---

### `precursores()`

**Complejidad temporal:** `O(n + s)`

La función recorre los nodos del árbol para localizar los libros y posteriormente examina cada relación de similitud una única vez para comparar los años de publicación.

Donde:

* `n` representa el número total de nodos almacenados.
* `s` representa el número total de relaciones de similitud entre libros.

---

## Compilación

Desde la carpeta `src`, ejecutar:

```bash
g++ -Wall -Wextra -g3 -std=c++17 main.cpp BookTree.cpp XMLReader.cpp TreeADT.cpp ../pugixml/pugixml.cpp -o ../output/XMLtree.exe
```

---

## Ejecución

Una vez compilado, ingresar a la carpeta `output` y ejecutar:

```bash
./XMLtree.exe
```

---

## Consideraciones

* El árbol se construye dinámicamente a partir del dataset XML.
* La implementación busca mantener una estructura de árbol genérica que pueda reutilizarse con otros datasets XML.
* Los casos con información faltante son manejados explícitamente para evitar conclusiones incorrectas durante las consultas.
* La consulta de precursores descarta automáticamente aquellos libros para los cuales no sea posible determinar con certeza la fecha de publicación propia o de alguno de sus libros similares.

---

## Autores

* Alfonso Arturo González Giusti
* Bryan Eliseo Aguirre Fuentes

---

## Uso Académico

Proyecto desarrollado con fines académicos para el estudio e implementación de estructuras de datos basadas en árboles.
