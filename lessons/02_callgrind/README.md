# 02_callgrind — Introducción práctica a Valgrind, Callgrind y KCachegrind

---

## 🎯 Objetivo

En esta lección vamos a:

- Generar un **perfil detallado de instrucciones y llamadas** usando **Valgrind** con su herramienta **Callgrind**.
- Visualizar el **call graph real** con **KCachegrind**, identificar *hotspots* y analizar *call trees*.
- Convertir los resultados a otros formatos de grafo usando **gprof2dot** y **Graphviz**.

Esta técnica es clave cuando quieres optimizar **caminos críticos**, descubrir **bottlenecks** o entender cómo se distribuye el trabajo entre funciones y módulos.

---

## 🛠️ ¿Qué es Valgrind?

**Valgrind** es un *framework* de instrumentación dinámica: ejecuta tu programa instrucción por instrucción en un entorno controlado.  
Incluye herramientas como:

- **Memcheck** → Busca errores de acceso a memoria.
- **Cachegrind** → Simula cachés y branch prediction.
- **Callgrind** → Extiende Cachegrind para capturar un **call graph** real y contar instrucciones.

---

## 🔍 ¿Qué hace Callgrind?

- Registra **qué funciones se ejecutan realmente** y cuántas instrucciones consumen.
- Construye un árbol de llamadas: *quién llama a quién* y con qué coste relativo.
- Estima el coste de CPU y caché de forma detallada.
- No requiere recompilar con flags especiales (solo `-g` para símbolos).

> 📌 *A diferencia de instrumentar funciones manualmente, Callgrind ve solo lo que se ejecuta, sin hooks invasivos.*

---

## 🗺️ ¿Qué es KCachegrind?

**KCachegrind** es una GUI para explorar los perfiles generados (`callgrind.out.<pid>`):

- Visualiza **call tree**, **call graph**, **cost tables**, **source view** y **assembler view**.
- Usa `objdump` para ver bloques de instrucciones y `dot` (Graphviz) para renderizar diagramas.
- Te permite filtrar y enfocar solo lo relevante.

---

## ⚡ Notas prácticas

✅ Usa **Valgrind** desde Snap si quieres versión reciente.

✅ Instala **KCachegrind** desde APT, no Snap: necesita acceder a `objdump`, `dot` y otras herramientas del sistema.

---

## ✅ Pasos para generar resultados

---

### 1️⃣ Instalar herramientas

```bash
sudo snap install valgrind
sudo apt install kcachegrind graphviz python3-pip
pip install gprof2dot
````

---

### 2️⃣ Ejecutar Callgrind

```bash
valgrind \
  --tool=callgrind \
  --dump-instr=yes --collect-jumps=yes \
  --callgrind-out-file=./results/callgrind.out.%p \
  ./calcjob/build/calc_jobs
```

* `--dump-instr=yes` y `--collect-jumps=yes` agregan información de saltos y flujos.
* `%p` inserta el PID para distinguir múltiples ejecuciones.

---

### 3️⃣ Explorar con KCachegrind

```bash
kcachegrind ./results/callgrind.out.<pid>
```

Ahí podrás:

* Navegar por el **Call Tree** y ver qué funciones consumen más.
* Usar el **Call Graph** para visualizar rutas críticas.
* Revisar inclusive vs exclusive cost.
* Combinar con el desensamblado (`objdump`) y el grafo (`dot`).

---

### 4️⃣ Generar gráficos extra con gprof2dot

```bash
callgrind_annotate ./results/callgrind.out.<pid>
```

Convierte a `.dot`:

```bash
gprof2dot -f callgrind ./results/callgrind.out.<pid> -o ./results/callgraph.dot
```

Y renderiza con Graphviz:

```bash
dot -Tpng ./results/callgraph.dot -o ./results/callgraph.png
```

---

## 📈 ¿Qué información obtenemos?

* **Hotspots**: funciones con mayor coste acumulado.
* **Inclusive vs Exclusive cost**: diferencia entre tiempo total (función + hijos) y tiempo propio.
* **Callers/Callees**: ver quién llama a quién y con qué peso.
* **Path Analysis**: rutas largas o inesperadas.

Combina esto con flamegraphs (`flamegraph.pl`) o timelines (`LTTng`) para un análisis más rico.

---

## 📌 Resumen

✅ `Callgrind` + `KCachegrind` es ideal para optimizar caminos críticos en C/C++.
✅ Es no invasivo: no necesitas recompilar con `-finstrument-functions`.
✅ Entrega datos cuantitativos para priorizar refactors y optimizaciones reales.

> 💡 *Si quieres el Control Flow Graph más nuevo, revisa que `KCachegrind` tenga acceso a `objdump` y `dot`. Evita Snap para la GUI.*


“Un profiler como Callgrind es perfecto para cuantificar hotspots y optimizar coste de funciones, pero no reemplaza a un trace de secuencia real cuando queremos entender el flujo de control concurrente. Cada herramienta sirve para responder preguntas distintas.”
