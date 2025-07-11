# 03_lttng — Análisis de flujo de eventos y concurrencia con LTTng

---

## 🎯 Objetivo

En esta lección vas a aprender a:

- Usar **LTTng** (*Linux Trace Toolkit Next Generation*) para registrar **eventos de kernel y user space**.
- Visualizar timelines, migración de hilos, bloqueos y sincronización usando **Trace Compass**.
- Relacionar la traza con tu código fuente para entender **qué hilos se bloquean**, *cuándo* y *dónde*.

---

## 🗂️ ¿Qué es LTTng?

**LTTng** es un *framework* de tracing de bajo overhead para Linux:

- Registra eventos a nivel kernel: syscalls, `sched_switch`, migración de procesos.
- Puede trazar user space si defines **tracepoints** con `lttng-ust`.
- Sus salidas se exploran con **Trace Compass**, una GUI basada en Eclipse.

A diferencia de **Callgrind** (que perfila coste de CPU) o `-finstrument-functions` (que inyecta hooks), LTTng se centra en **orden real de eventos y comportamiento de concurrencia**.

---

## ⚙️ Comparativa

| Aspecto               | `-finstrument-functions` | Callgrind               | LTTng                  |
|-----------------------|--------------------------|-------------------------|------------------------|
| Orden cronológico     | Sí                       | No                      | Sí                     |
| Coste CPU detallado   | No                       | Sí                      | No (solo contadores)   |
| Eventos de kernel     | No                       | No                      | Sí                     |
| Context switch & CPU  | Manual                   | No                      | Sí, nativo             |
| Ideal para...         | Call graph secuencial    | Hotspots                | Timeline, bloqueos     |

---

## ✅ Pasos para usar LTTng

---

### 1️⃣ Instalar herramientas

```bash
sudo apt install lttng-tools lttng-modules-dkms babeltrace
````

Descargar Trace Compass (GUI):

* [https://www.eclipse.org/tracecompass/](https://www.eclipse.org/tracecompass/)

---

### 2️⃣ Crear una sesión de traza

```bash
lttng create my-session --output=./results/lttng-trace
```

---

### 3️⃣ Habilitar eventos de kernel

Ejemplo básico para observar planificación y syscalls:

```bash
lttng enable-event -k sched_switch
lttng enable-event -k sched_wakeup
lttng enable-event -k sys_enter*
lttng enable-event -k sys_exit*
```

---

### 4️⃣ (Opcional) Habilitar eventos de user space

Si defines tus tracepoints:

```bash
lttng enable-event -u my_namespace:my_event
```

---

### 5️⃣ Ejecutar aplicación bajo traza

```bash
lttng start
./calcjob/build/calc_jobs
lttng stop
lttng destroy
```

Esto generará una carpeta `./results/lttng-trace/` con todos los archivos.

---

### 6️⃣ Explorar con Trace Compass

1. Abre Trace Compass.
2. Importa `./results/lttng-trace/`.
3. Usa las vistas:

   * **CPU Usage** → para ver carga de cada core.
   * **Resources View** → para ver migración de hilos.
   * **Control Flow** → diagrama de secuencia de threads.
   * **Critical Path Analysis** → dependencias y bloqueos.

---

## 🔍 Ejemplo de instrumentación user space mínima

```cpp
#include <lttng/tracepoint.h>

TRACEPOINT_EVENT(
  my_namespace,
  my_event,
  TP_ARGS(int, value),
  TP_FIELDS(
    ctf_integer(int, my_value, value)
  )
)
```

Llamada desde tu código:

```cpp
tracepoint(my_namespace, my_event, 42);
```

---

## 📈 ¿Qué información puedes obtener?

* **Cuándo se bloquean tus hilos** (`sched_switch`).
* **Qué hilos migran de CPU** y por qué.
* Duración real de secciones críticas (locks, waits).
* Visualización de deadlocks o waits inesperados.
* Puntos donde optimizar sincronización.

---

## ⚡ Tips prácticos

✅ No requiere recompilar tu app para kernel tracepoints.
✅ Para user space, usa `lttng-ust` + headers.
✅ Combina con tus propios `-finstrument-functions` o logs para correlacionar.

---

## 📌 Resumen

> 🔑 *LTTng* es la herramienta ideal para responder preguntas como:
>
> * *¿Qué hilo estaba corriendo cuándo?*
> * *¿Qué CPU ejecuta cada parte?*
> * *¿Por qué este hilo se bloquea?*
>
> Perfecto para complementar con **Callgrind** (coste) y **instrumentación secuencial** (detalle de lógica).

---

## 🗂️ Próximos pasos

✅ Ejecuta `calcjob` con `sched_switch` activado y observa la relación entre Worker y Manager.
✅ Prueba habilitar tracepoints propios para correlacionar funciones clave.
✅ Guarda y versiona tus trazas para comparar mejoras de concurrencia.

---

🚀 *Siguiente lección: integrar todo y diseñar estrategias de observabilidad en proyectos reales.*

```
