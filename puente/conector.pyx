# distutils: language = c++

from libcpp.vector cimport vector
from libcpp.string cimport string
from c_grafo cimport GrafoDisperso

# clase python que envuelve a la clase c++
cdef class PyGrafo:
    cdef GrafoDisperso* c_grafo  # puntero a la instancia c++

    def __cinit__(self):
        self.c_grafo = new GrafoDisperso()

    def __dealloc__(self):
        del self.c_grafo

    def cargar_datos(self, nombre_archivo):
        # convertir string python a string c++
        cdef string c_nombre = nombre_archivo.encode('utf-8')
        self.c_grafo.cargarDatos(c_nombre)

    def busqueda_anchura(self, nodo_inicio, profundidad_max):
        return self.c_grafo.busquedaAnchura(nodo_inicio, profundidad_max)

    def obtener_nodo_mayor_grado(self):
        return self.c_grafo.obtenerNodoMayorGrado()

    def obtener_vecinos(self, nodo):
        return self.c_grafo.obtenerVecinos(nodo)

    def obtener_num_nodos(self):
        return self.c_grafo.obtenerNumNodos()

    def obtener_num_aristas(self):
        return self.c_grafo.obtenerNumAristas()
