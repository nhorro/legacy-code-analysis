# TOOLS.md — Mapa de herramientas de análisis y observabilidad en C++/Linux

---

## 🎯 Objetivo

Tener a mano un resumen de **qué herramientas existen**, **qué responden** y **cuándo conviene usarlas** para no perder tiempo explorando técnicas que no aplican.

---

## 🌐 Clasificación de herramientas

---

## 🗂️ 1️⃣ Nivel Syscall: interceptar llamadas al sistema

| Herramienta | Qué hace | Ideal para |
| ----------- | -------- | ---------- |
| `strace` | Usa `ptrace` internamente para capturar todas las **syscalls** (open, read, write, connect, etc.). | Ver por qué un proceso se bloquea, falta de permisos, I/O inesperado. |
| `ptrace` | API de bajo nivel para enganchar procesos. | Usado por `strace` y `gdb`. Normalmente no se usa directo. |
| `ltrace` | Similar a `strace` pero para **librerías dinámicas** (`libc.so`, `libpthread.so`). | Ver llamadas a funciones de librerías compartidas. |

---

## 🗂️ 2️⃣ Nivel Función: flujo de control y coste

| Herramienta | Qué hace | Ideal para |
| ----------- | -------- | ---------- |
| `-finstrument-functions` | GCC inyecta hooks en entrada/salida de cada función. | Obtener un callgraph secuencial. Detectar reentradas, trampas de sincronización. |
| `Callgrind` (Valgrind) | Simula CPU y caché, cuenta instrucciones y saltos. | Identificar hotspots: dónde se consume más CPU. |
| `perf` | Sampling real con PMU. | Profiling ligero sin recompilar. Genera callgraphs basados en muestras. |
| `gprof2dot` + `Graphviz` | Visualiza la salida de `gprof` o `callgrind`. | Callgraphs visuales, no muestra orden cronológico. |

---

## 🗂️ 3️⃣ Nivel concurrencia y flujo de eventos

| Herramienta | Qué hace | Ideal para |
| ----------- | -------- | ---------- |
| `LTTng` + Trace Compass | Trazado kernel y user space, genera timeline, migración de threads, bloqueos. | Entender cuándo y dónde se bloquean hilos. |
| `perf sched` | Muestra planificación de procesos/hilos. | Alternativa rápida a `sched_switch`. |
| SystemTap / BPF | Inyecta probes dinámicos a kernel/user space. | Observabilidad avanzada en producción. |
| `htop` / `pidstat` | Monitoreo interactivo de uso de CPU, memoria, I/O. | Inspección en tiempo real. |

---

## 🗂️ 4️⃣ Nivel memoria

| Herramienta | Qué hace | Ideal para |
| ----------- | -------- | ---------- |
| Valgrind Memcheck | Detección de leaks, double free, overflows. | Debug de bugs de memoria en desarrollo. |
| `mtrace` | Traza simple de malloc/free. | Análisis rápido sin mucho overhead. |
| AddressSanitizer (ASan) | Instrumenta binario para detectar errores de memoria. | Rápido, ideal en CI. |

---

## 🧩 ¿Dónde encajan ptrace y strace?

- `ptrace` es solo la **API** → se usa para inspeccionar registros, memoria y syscalls.
- `strace` construye sobre `ptrace` → intercepta syscalls, muestra parámetros, resultados y errores.
- `strace` NO sirve para profiling de rendimiento: solo para ver *qué llama a qué en el nivel kernel*.

---

## ⚡ Tips prácticos

✅ Usa `strace` para problemas de I/O y permisos.  
✅ Usa `Callgrind` o `perf` para ver **dónde se gasta CPU**.  
✅ Usa LTTng/Trace Compass para entender **timeline real y bloqueos entre hilos**.  
✅ Combina todo: correlaciona logs, tracepoints, perf y syscalls según la pregunta.

---

## 📌 Resumen rápido

> **`strace`** → nivel syscall: *¿Qué hace el proceso en kernel?*  
> **`-finstrument-functions`** → callgraph: *¿Quién llama a quién y cuándo?*  
> **`Callgrind/perf`** → coste: *¿Dónde se va el tiempo?*  
> **LTTng** → concurrencia real: *¿Quién bloquea a quién, cuándo y dónde?*

---

## 📚 Lecturas recomendadas

- `man ptrace` / `man strace`
- [Brendan Gregg’s `perf` page](http://www.brendangregg.com/perf.html)
- [LTTng official docs](https://lttng.org/docs/)

---

🟢 *Este archivo sirve como mapa mental para tus próximas lecciones. Actualízalo a medida que experimentes con cada herramienta.* 🚀
