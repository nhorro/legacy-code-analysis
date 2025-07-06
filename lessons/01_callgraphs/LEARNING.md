# 📚 LEARNING.md — 01_callgraphs

---

## 📌 Lecciones aprendidas

---

### ✅ 1️⃣ Instrumentación directa con `-finstrument-functions`

Usar `-finstrument-functions` es una forma práctica de obtener un *callgraph* dinámico de cualquier aplicación C++ — incluso multihilo — **sin depender de herramientas externas complejas**.

- Para que `addr2line` funcione correctamente, es clave compilar con:
  - `-no-pie`: evita direcciones aleatorias (PIE/ASLR) y mantiene offsets fijos.
  - `-rdynamic`: exporta símbolos adicionales si es necesario.
- Registrar el *thread ID* (`gettid()`) junto con cada evento permite reconstruir la concurrencia real.

---

### ✅ 2️⃣ Filtrado en tiempo de ejecución

- La traza puede crecer rápidamente, generando archivos masivos y difíciles de analizar.
- Para mantenerla útil:
  - Aplica filtros en tiempo de ejecución (`is_relevant()`): ignora llamadas a `std::`, `boost::`, o lambdas triviales.
  - Protege los hooks de instrumentación contra *reentrancia* (usa `thread_local bool in_hook`).

---

### ✅ 3️⃣ De direcciones de memoria a nombres legibles

- Lo que se captura es un puntero (`void*`) a la dirección de la instrucción.
- Se traduce a nombre de función + línea usando:
  - [`addr2line`](https://man7.org/linux/man-pages/man1/addr2line.1.html) → resuelve dirección a `func:line`.
  - [`c++filt`](https://man7.org/linux/man-pages/man1/c++filt.1.html) → *demangle* para convertir `_ZN7Manager5startEv` en `Manager::start()`.

---

### ✅ 4️⃣ El diagrama de secuencia necesita post-procesamiento a medida

- Concurrencia, callbacks (`std::function`), lambdas y `std::thread` generan múltiples capas de “rebote” que no aportan a la lógica de negocio.
- Por eso:
  - Diseña filtros a medida para cada proyecto.
  - Normaliza nombres de lambdas (`Manager::{lambda}` → `Manager::onJobDone()`).
  - Distingue hilos con notas (`Thread <id>`) y considera marcar timestamps relativos para ver gaps de ejecución.

---

## ⚡ Resumen

✔️ Esta técnica es **útil y versátil** para entender la secuencia de eventos y la interacción entre hilos.  
✔️ Es un punto de partida para debugging, *reverse engineering* de legado y verificar supuestos de diseño.  
✔️ Para mediciones de consumo real de CPU o memoria, flamegraphs y timelines, combina este enfoque con:
   - **Callgrind / KCachegrind** → consumo de funciones.
   - **LTTng + Trace Compass** → eventos de contexto, switch de hilos, timeline real.

---

## 🏷️ Referencias clave

- [GCC `-finstrument-functions`](https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html#index-finstrument-functions)
- [GCC PIE/ASLR](https://gcc.gnu.org/wiki/PIE)
- [`addr2line`](https://man7.org/linux/man-pages/man1/addr2line.1.html)
- [`c++filt`](https://man7.org/linux/man-pages/man1/c++filt.1.html)
- [PlantUML — Diagrama de Secuencia](https://plantuml.com/sequence-diagram)

---

## 🚀 Próximos pasos

1️⃣ Implementar versión con `Valgrind/Callgrind`.  
2️⃣ Medir flujo de hilos con `LTTng`.  
3️⃣ Automatizar flamegraphs y timeline para *bottlenecks* reales.

---

**Fin de lección — `LEARNING.md`**

