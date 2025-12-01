from libcpp.vector cimport vector
from libcpp.string cimport string

cdef extern from "../nucleo/Grafo.h":
    cdef cppclass GrafoBase:
        pass

    cdef cppclass GrafoDisperso(GrafoBase):
        GrafoDisperso() except +
        void cargarDatos(string nombreArchivo)
        vector[int] busquedaAnchura(int nodoInicio, int profundidadMax)
        int obtenerNodoMayorGrado()
        vector[int] obtenerVecinos(int nodo)
        int obtenerNumNodos()
        int obtenerNumAristas()
