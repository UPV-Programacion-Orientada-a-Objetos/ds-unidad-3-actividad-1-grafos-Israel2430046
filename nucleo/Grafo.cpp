#include "Grafo.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>

// constructor
GrafoDisperso::GrafoDisperso() : num_nodos(0), num_aristas(0) {}

// destructor
GrafoDisperso::~GrafoDisperso() {}

// cargar datos desde archivo de texto
void GrafoDisperso::cargarDatos(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "error al abrir el archivo" << std::endl;
        return;
    }

    // lista de adyacencia temporal para construir csr
    std::map<int, std::vector<int>> adyacencia_temp;
    int u, v;
    std::string linea;

    // leer linea por linea
    while (std::getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;
        std::stringstream ss(linea);
        if (ss >> u >> v) {
            // mapear ids si es necesario
            int u_int = obtenerIdInterno(u);
            int v_int = obtenerIdInterno(v);
            
            adyacencia_temp[u_int].push_back(v_int);
            // si es no dirigido descomentar la siguiente linea
            // adyacencia_temp[v_int].push_back(u_int); 
            num_aristas++;
        }
    }
    archivo.close();

    num_nodos = id_original_a_interno.size();

    // construir csr
    ptr_fila.resize(num_nodos + 1, 0);
    
    int acumulado = 0;
    for (int i = 0; i < num_nodos; ++i) {
        ptr_fila[i] = acumulado;
        if (adyacencia_temp.find(i) != adyacencia_temp.end()) {
            std::vector<int>& vecinos = adyacencia_temp[i];
            // ordenar vecinos para busquedas mas rapidas
            std::sort(vecinos.begin(), vecinos.end());
            for (int vecino : vecinos) {
                indices_col.push_back(vecino);
                valores.push_back(1); // peso 1 por defecto
            }
            acumulado += vecinos.size();
        }
    }
    ptr_fila[num_nodos] = acumulado;
    
    // limpiar memoria temporal
    adyacencia_temp.clear();
}

// obtener id interno creando uno nuevo si no existe
int GrafoDisperso::obtenerIdInterno(int idExterno) {
    if (id_original_a_interno.find(idExterno) == id_original_a_interno.end()) {
        int nuevoId = id_original_a_interno.size();
        id_original_a_interno[idExterno] = nuevoId;
        id_interno_a_original[nuevoId] = idExterno;
    }
    return id_original_a_interno[idExterno];
}

// obtener id externo
int GrafoDisperso::obtenerIdExterno(int idInterno) {
    if (id_interno_a_original.find(idInterno) != id_interno_a_original.end()) {
        return id_interno_a_original[idInterno];
    }
    return -1;
}

// busqueda en anchura
std::vector<int> GrafoDisperso::busquedaAnchura(int nodoInicio, int profundidadMax) {
    std::vector<int> resultado;
    int inicioInterno = obtenerIdInterno(nodoInicio);
    
    if (inicioInterno >= num_nodos) return resultado;

    std::queue<std::pair<int, int>> cola;
    std::set<int> visitados;

    cola.push({inicioInterno, 0});
    visitados.insert(inicioInterno);

    while (!cola.empty()) {
        int actual = cola.front().first;
        int profundidad = cola.front().second;
        cola.pop();

        resultado.push_back(obtenerIdExterno(actual));

        if (profundidad >= profundidadMax) continue;

        // recorrer vecinos usando csr
        int inicio_vecinos = ptr_fila[actual];
        int fin_vecinos = ptr_fila[actual + 1];

        for (int i = inicio_vecinos; i < fin_vecinos; ++i) {
            int vecino = indices_col[i];
            if (visitados.find(vecino) == visitados.end()) {
                visitados.insert(vecino);
                cola.push({vecino, profundidad + 1});
            }
        }
    }
    return resultado;
}

// obtener nodo con mayor grado
int GrafoDisperso::obtenerNodoMayorGrado() {
    int maxGrado = -1;
    int nodoMax = -1;

    for (int i = 0; i < num_nodos; ++i) {
        int grado = ptr_fila[i + 1] - ptr_fila[i];
        if (grado > maxGrado) {
            maxGrado = grado;
            nodoMax = i;
        }
    }
    return obtenerIdExterno(nodoMax);
}

// obtener vecinos de un nodo
std::vector<int> GrafoDisperso::obtenerVecinos(int nodo) {
    std::vector<int> vecinos;
    int nodoInterno = obtenerIdInterno(nodo);
    
    if (nodoInterno >= num_nodos || nodoInterno < 0) return vecinos;

    int inicio = ptr_fila[nodoInterno];
    int fin = ptr_fila[nodoInterno + 1];

    for (int i = inicio; i < fin; ++i) {
        vecinos.push_back(obtenerIdExterno(indices_col[i]));
    }
    return vecinos;
}

int GrafoDisperso::obtenerNumNodos() {
    return num_nodos;
}

int GrafoDisperso::obtenerNumAristas() {
    return num_aristas;
}
