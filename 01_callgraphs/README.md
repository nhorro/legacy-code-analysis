# 01_callgraphs — Instrumentación de ejecución C++ con `-finstrument-functions`

---

## 📌 Objetivo

Esta lección muestra cómo:
1. Entender el flujo de ejecución de un proyecto C++ concurrente de forma práctica.
2. Aplicar instrumentación *manual* con `-finstrument-functions` para interceptar todas las llamadas de función.
3. Resolver direcciones de memoria a símbolos legibles usando `addr2line` + `c++filt` (*demangling*).
4. Convertir la traza en diagramas de secuencia con PlantUML, visualizando qué *hilo* hace cada llamada.
5. Introducir limitaciones como PIE/ASLR y best practices para traces multihilo.

**🎯 Alcance:**  
Esta lección cubre *sólo* la instrumentación con GCC y generación del diagrama de secuencia. Técnicas como `Valgrind/Callgrind` y `LTTng` se abordarán en lecciones futuras.

---

## 🗂️ Estructura del repositorio

```

01\_callgraphs/
├── calcjob/           # Proyecto original sin instrumentación
├── calcjob\_instr/     # Proyecto instrumentado (con hooks y flags extra)
├── scripts/           # Scripts Python: resolver direcciones, demangling, exportar CSV
├── results/           # Artefactos: CSV, diagramas PlantUML
├── instrumentation.patch  # Diff detallado de todos los cambios
└── README.md          # Esta guía

````

---

## 🚦 Cambios clave (según `instrumentation.patch`)

### ✅ 1️⃣ Agregar archivo `instrumentation.hpp`

Define:
```cpp
extern "C" void __cyg_profile_func_enter(void* func, void* caller);
extern "C" void __cyg_profile_func_exit(void* func, void* caller);
````

**Explicación:**

* `-finstrument-functions` inserta estos hooks alrededor de *cada función*.
* Dentro de cada hook:

  * Se captura `func` y `caller` (punteros de instrucción).
  * Se resuelve el nombre con `addr2line`.
  * Se usa `c++filt` o `__cxa_demangle` para traducir `_ZN7Manager5startEv` a `Manager::start()`.

**Detalle importante:**
Usamos `thread_local bool in_hook` para evitar reentrancia infinita (p. ej., `printf` dentro del hook que vuelve a disparar el hook).

---

### ✅ 2️⃣ Incluir `instrumentation.hpp` en `main.cpp`

```cpp
#include "instrumentation.hpp"
```

Esto asegura que tus hooks se enlacen.
No se necesita incluirlo en cada archivo fuente.

---

### ✅ 3️⃣ Cambios en `CMakeLists.txt`

```cmake
add_compile_options(-finstrument-functions)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
```

* `-finstrument-functions`: genera la instrumentación.
* `-no-pie`: desactiva *Position Independent Executable* (PIE).

  * Por defecto, compiladores modernos generan binarios PIE.
  * PIE implica direcciones relativas → el loader puede reubicar todo (ASLR).
  * Para resolver direcciones de memoria a símbolos con `addr2line` necesitas direcciones fijas.
  * Referencia: [GCC PIE](https://gcc.gnu.org/wiki/PIE).

---

## 🧩 Conceptos clave

| Concepto              | Descripción                                                                                                                                                                      |
| --------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Mangling**          | Compiladores C++ renombran símbolos para soportar sobrecarga, namespaces, plantillas.                                                                                            |
| **Demangling**        | `c++filt` o `__cxa_demangle` revierten `_ZN7Manager5startEv` → `Manager::start()`.                                                                                               |
| **PIE / ASLR**        | Position Independent Executable + Address Space Layout Randomization → protegen de exploits moviendo direcciones en cada ejecución. Para *tracing*, a veces necesitas `-no-pie`. |
| **Hooks reentrantes** | Evitar loops infinitos: `printf` dentro del hook llama a `malloc` → que puede volver a disparar el hook. Se protege con `thread_local bool in_hook`.                             |
| **addr2line**         | Herramienta para mapear direcciones de memoria a `function:line`.                                                                                                                |
| **Trace CSV**         | El hook guarda `(timestamp, thread_id, caller, callee)`. Luego un script convierte direcciones a nombres legibles y resuelve lambdas.                                            |

---

## ⚡ Buenas prácticas

✅ Filtrar símbolos irrelevantes (`std::`, allocators, internals de Qt o Boost) → evita ruido.
✅ Escribir trace a archivo (no a stdout) en proyectos multihilo.
✅ Usar `trace_resolve.py` para resolver direcciones y `trace_analysis.ipynb` para exportar PlantUML.
✅ Si tu diagrama es muy grande, divide por *thread\_id* o rango de tiempo.

---

## 🗜️ Cómo generar y aplicar el `patch`

Para ver cambios:

```bash
diff -ruN calcjob/ calcjob_instr/ > instrumentation.patch
```

Para aplicar:

```bash
patch -p1 < instrumentation.patch
```

---

## 🎥 Próximos pasos

* Valgrind/Callgrind para perfiles de consumo real (runtime).
* LTTng/Trace Compass para flujo de eventos de hilos.
* Generar flamegraphs.
* Experimentar con `LD_PRELOAD` para hook dinámico.

---

## 🔗 Referencias útiles

* [GCC `-finstrument-functions`](https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html#index-finstrument-functions)
* [PIE/ASLR](https://gcc.gnu.org/wiki/PIE)
* [`addr2line` man page](https://man7.org/linux/man-pages/man1/addr2line.1.html)
* [`c++filt` man page](https://man7.org/linux/man-pages/man1/c++filt.1.html)
* [PlantUML](https://plantuml.com/sequence-diagram)

---