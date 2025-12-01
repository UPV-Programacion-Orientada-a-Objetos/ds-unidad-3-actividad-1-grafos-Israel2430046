from setuptools import setup, Extension
from Cython.Build import cythonize
import os
import sys

# definicion de la extension
extra_compile_args = []
# en gcc/clang se usa -std=c++11, en msvc no es necesario para este codigo
if os.name != "nt":
    extra_compile_args = ["-std=c++11"]

extensions = [
    Extension(
        "puente.conector",
        sources=["puente/conector.pyx", "nucleo/Grafo.cpp"],
        include_dirs=["nucleo"],
        language="c++",
        extra_compile_args=extra_compile_args,
    )
]

setup(
    name="NeuroNet",
    ext_modules=cythonize(extensions, language_level="3"),
    zip_safe=False,
)
