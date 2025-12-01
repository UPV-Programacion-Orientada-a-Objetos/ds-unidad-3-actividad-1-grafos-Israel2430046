from setuptools import setup, Extension
from Cython.Build import cythonize
import os

# definicion de la extension
extensions = [
    Extension(
        "puente.conector",
        sources=["puente/conector.pyx", "nucleo/Grafo.cpp"],
        include_dirs=["nucleo"],
        language="c++",
        extra_compile_args=["-std=c++11"],  # asegurar estandar c++11
    )
]

setup(
    name="NeuroNet",
    ext_modules=cythonize(extensions),
    zip_safe=False,
)
