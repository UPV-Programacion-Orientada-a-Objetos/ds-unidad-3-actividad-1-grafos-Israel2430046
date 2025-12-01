import sys
import os
import tkinter as tk
from tkinter import filedialog, messagebox
import networkx as nx
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

# agregar directorio raiz al path para importar el modulo compilado
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

try:
    from puente.conector import PyGrafo
except ImportError:
    messagebox.showerror("error", "no se pudo importar el modulo c++. asegurate de compilar primero.")
    sys.exit(1)

class AplicacionNeuroNet:
    def __init__(self, raiz):
        self.raiz = raiz
        self.raiz.title("neuronet - analisis de grafos masivos")
        self.raiz.geometry("1000x800")

        self.grafo_backend = PyGrafo()
        self.grafo_nx = nx.Graph()

        self.crear_widgets()

    def crear_widgets(self):
        # panel de control
        panel_control = tk.Frame(self.raiz, padx=10, pady=10)
        panel_control.pack(side=tk.TOP, fill=tk.X)

        btn_cargar = tk.Button(panel_control, text="cargar dataset", command=self.cargar_archivo)
        btn_cargar.pack(side=tk.LEFT, padx=5)

        self.lbl_info = tk.Label(panel_control, text="nodos: 0 | aristas: 0")
        self.lbl_info.pack(side=tk.LEFT, padx=20)

        # panel de analisis
        marco_analisis = tk.LabelFrame(panel_control, text="analisis", padx=5, pady=5)
        marco_analisis.pack(side=tk.LEFT, padx=20)

        tk.Button(marco_analisis, text="nodo mayor grado", command=self.mostrar_mayor_grado).pack(side=tk.LEFT, padx=5)
        
        tk.Label(marco_analisis, text="nodo inicio:").pack(side=tk.LEFT, padx=5)
        self.ent_inicio = tk.Entry(marco_analisis, width=5)
        self.ent_inicio.pack(side=tk.LEFT)

        tk.Label(marco_analisis, text="profundidad:").pack(side=tk.LEFT, padx=5)
        self.ent_profundidad = tk.Entry(marco_analisis, width=3)
        self.ent_profundidad.insert(0, "1")
        self.ent_profundidad.pack(side=tk.LEFT)

        tk.Button(marco_analisis, text="ejecutar bfs", command=self.ejecutar_bfs).pack(side=tk.LEFT, padx=5)

        # area de visualizacion
        self.figura = plt.Figure(figsize=(5, 4), dpi=100)
        self.ax = self.figura.add_subplot(111)
        self.canvas = FigureCanvasTkAgg(self.figura, master=self.raiz)
        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=True)

    def cargar_archivo(self):
        archivo = filedialog.askopenfilename(filetypes=[("archivos de texto", "*.txt"), ("todos", "*.*")])
        if archivo:
            try:
                self.grafo_backend.cargar_datos(archivo)
                nodos = self.grafo_backend.obtener_num_nodos()
                aristas = self.grafo_backend.obtener_num_aristas()
                self.lbl_info.config(text=f"nodos: {nodos} | aristas: {aristas}")
                messagebox.showinfo("exito", "grafo cargado correctamente")
            except Exception as e:
                messagebox.showerror("error", f"error al cargar: {str(e)}")

    def mostrar_mayor_grado(self):
        nodo = self.grafo_backend.obtener_nodo_mayor_grado()
        messagebox.showinfo("resultado", f"el nodo con mayor grado es: {nodo}")

    def ejecutar_bfs(self):
        try:
            inicio = int(self.ent_inicio.get())
            profundidad = int(self.ent_profundidad.get())
            
            # ejecutar bfs en c++
            nodos_visitados = self.grafo_backend.busqueda_anchura(inicio, profundidad)
            
            # dibujar subgrafo
            self.dibujar_subgrafo(nodos_visitados, inicio)
            
        except ValueError:
            messagebox.showerror("error", "ingresa valores numericos validos")

    def dibujar_subgrafo(self, nodos, centro):
        self.ax.clear()
        self.grafo_nx.clear()
        
        # construir subgrafo para visualizar
        # esto es ineficiente para grafos grandes pero ok para subgrafos pequeños
        for nodo in nodos:
            vecinos = self.grafo_backend.obtener_vecinos(nodo)
            for vecino in vecinos:
                if vecino in nodos:
                    self.grafo_nx.add_edge(nodo, vecino)
        
        pos = nx.spring_layout(self.grafo_nx, seed=42)
        nx.draw(self.grafo_nx, pos, ax=self.ax, with_labels=True, node_size=300, node_color='lightblue', font_size=8)
        
        # resaltar nodo central
        if centro in self.grafo_nx:
            nx.draw_networkx_nodes(self.grafo_nx, pos, nodelist=[centro], node_color='red', node_size=500, ax=self.ax)
            
        self.canvas.draw()

if __name__ == "__main__":
    raiz = tk.Tk()
    app = AplicacionNeuroNet(raiz)
    raiz.mainloop()
