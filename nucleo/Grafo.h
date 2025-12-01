#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <map>

// clase base abstracta para el grafo
class GrafoBase {
public:
    virtual ~GrafoBase() {}
    virtual void cargarDatos(const std::string& nombreArchivo) = 0;
    virtual std::vector<int> busquedaAnchura(int nodoInicio, int profundidadMax) = 0;
    virtual int obtenerNodoMayorGrado() = 0;
    virtual std::vector<int> obtenerVecinos(int nodo) = 0;
    virtual int obtenerNumNodos() = 0;
    virtual int obtenerNumAristas() = 0;
};

// clase concreta usando formato csr (compressed sparse row)
class GrafoDisperso : public GrafoBase {
private:
    // estructura csr
    std::vector<int> valores; // no se usa realmente para grafos no ponderados pero es parte del estandar
    std::vector<int> indices_col;
    std::vector<int> ptr_fila;
    
    int num_nodos;
    int num_aristas;
    
    // mapa para remapear ids de nodos si no son consecutivos
    std::map<int, int> id_original_a_interno;
    std::map<int, int> id_interno_a_original;

public:
    GrafoDisperso();
    ~GrafoDisperso();

    void cargarDatos(const std::string& nombreArchivo) override;
    std::vector<int> busquedaAnchura(int nodoInicio, int profundidadMax) override;
    int obtenerNodoMayorGrado() override;
    std::vector<int> obtenerVecinos(int nodo) override;
    int obtenerNumNodos() override;
    int obtenerNumAristas() override;
    
    // metodo auxiliar para convertir id externo a interno
    int obtenerIdInterno(int idExterno);
    // metodo auxiliar para convertir id interno a externo
    int obtenerIdExterno(int idInterno);
};

#endif
