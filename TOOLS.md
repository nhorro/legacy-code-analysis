# 🛠️ TOOLS.md — Mapa de herramientas para Legacy C++ Analysis

---

## 🎯 Propósito

Este archivo resume herramientas y técnicas prácticas para:
- Entender **cómo funciona** un código legacy.
- Ver su comportamiento en ejecución
- Depurarlo, modificarlo y mantenerlo
- Generar diagramas, métricas y artefactos de apoyo

---

## ✅ 1️⃣ Code Understanding

### ✨ Herramientas clave

| Herramienta | Qué hace | Usos típicos |
| ----------- | -------- | ------------- |
| **VSCode + clangd** | Navegación semántica, autocompletado, goto. | Entender referencias cruzadas, firmas, refactor. |
| **`ctags`** | Indexa símbolos para búsquedas con `fzf` o `vim`. | Ideal en terminal o entornos mixtos. |
| **`cscope`** | Similar a `ctags` pero soporta queries "quién llama a esto". | Bien para código procedural grande. |
| **Doxygen** | Extrae diagramas de clases, dependencias, callgraph estático. | Documentar y visualizar relaciones. |
| **Graphviz** | Renderiza `.dot` de Doxygen u otros graphs. | Visualiza árboles y dependencias. |
| **IWYU** | Analiza headers usados vs. incluidos. | Elimina includes redundantes. |

---

## ✅ 2️⃣ Debugging

### 🐞 Herramientas clave

| Herramienta | Qué hace | Usos típicos |
| ----------- | -------- | ------------- |
| **GDB** | Debug interactivo: breakpoints, watchpoints, backtrace. | Diagnóstico paso a paso. |
| **VSCode Debug** | Frontend para GDB. | Depuración interactiva más visual. |
| **rr** (Record & Replay) | Graba la ejecución, permite debug “hacia atrás”. | Bugs esquivos o no deterministas. |
| **Valgrind Memcheck** | Detecta leaks, uso de memoria inválida. | Ver problemas que causan crashes sutiles. |

---

## ✅ 3️⃣ Dynamic Analysis

### 🔍 Herramientas clave

| Herramienta | Qué hace | Usos típicos |
| ----------- | -------- | ------------- |
| **`-finstrument-functions`** | Hooks de entrada/salida de funciones. | Ver flujo real, reentrancias, orden de llamadas. |
| **Valgrind Callgrind** | Cuenta instrucciones, saltos, simula CPU y caché. | Saber qué funciones consumen más CPU. |
| **KCachegrind** | Visualiza la salida de Callgrind. | Analizar hotspots. |
| **perf** | Profiling por sampling. | Análisis rápido en producción. |
| **LTTng** + **Trace Compass** | Traza eventos kernel/user-space, genera timelines. | Analizar concurrencia, context switches, locks. |
| **SystemTap / BPF / bpftrace** | Observabilidad avanzada sin recompilar. | Probes dinámicos en producción. |
| **Sanitizers (ASan, TSan)** | Detecta bugs de memoria o data races. | Alternativa rápida a Valgrind, requiere recompilar. |

---

## ✅ 4️⃣ Glue & Scripts Python

### 🐍 Papel clave de Python

✔️ Transformar dumps (Callgrind, Babeltrace, perf script) → CSV  
✔️ Generar diagramas (PlantUML, Mermaid)  
✔️ Filtrar eventos irrelevantes, agregar timestamp relativo, etc.  
✔️ Construir pipelines reproducibles: `./scripts/` como *caja de utilidades*.

---

## ✅ 🗃️ Otras herramientas complementarias

- **`strace`** → Traza syscalls (I/O, permisos). Usa `ptrace` internamente.
- **`ltrace`** → Traza llamadas a librerías dinámicas.
- **`htop`, `pidstat`** → Ver procesos en tiempo real.
- **`pwndbg`** → Extensión de GDB para mejor visualización.
- **AddressSanitizer** → Excelente para CI/CD rápido.

---

## ✅ 📌 Tips prácticos

✔️ **Siempre arma tu `compile_commands.json`** → clangd y IWYU lo necesitan.  
✔️ **Combina indexación + tracing + timeline** → ninguna herramienta por sí sola da la película completa.  
✔️ **Versiona tus scripts Python** → Es tu *meta-herramienta* para dar forma a los datos.  
✔️ **Mantén diagramas actualizados** → Nada más mortal que un diagram que no coincide con la base de código real.

---

## ✅ 📚 Referencias útiles

- `man strace`, `man ptrace`, `man gdb`
- [Brendan Gregg’s `perf` Guide](http://www.brendangregg.com/perf.html)
- [LTTng Docs](https://lttng.org/docs/)
- [Valgrind Manual](http://valgrind.org/docs/manual/manual.html)
- [Graphviz Gallery](https://graphviz.org/gallery/)