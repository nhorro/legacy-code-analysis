# Introducción — Taller de Autoaprendizaje: Legacy C++/Qt/Linux

## Objetivo

Proveer un esquema **práctico** para abordar proyectos **legacy** (sin documentación técnica actualizada, con bugs descritos desde la perspectiva del usuario final).  
Combina **herramientas reales** con técnicas de análisis estructuradas, tomando como referencia el libro [The Legacy Code Programmer’s Toolbox (Jonathan Boccara)](https://www.amazon.com/Legacy-Code-Programmers-Toolbox-Professionals/dp/1691064130).

## Contexto

- Escenario: base de código grande, arquitectura poco clara, deuda técnica acumulada.
- El enfoque se apoya en **técnicas de exploración y entendimiento** extraídas de Boccara.

## Estructura

### 1. Comprensión & estimación de complejidad
- Navegación estructural: Doxygen (relaciones entre archivos y clases).
- Navegación semántica: ctags, clangd en VS Code.
- Métricas: cloc y lizard para estimar tamaño y puntos críticos.

### 2. Debugging
- GDB para ejecución real.
- Uso de breakpoints, backtrace y formulación de hipótesis.

### 3. Profiling & tracing
- Herramientas: perf, LTTng u otras.
- Registro de stacks, hilos, sincronización y detección de cuellos de botella.

### 4. Comprensión/refactoring asistido por IA
- Indexación (ej. gitingest).
- Prompts para resúmenes de funciones, diagramas (PlantUML).
- Documentación mínima generada.

## Relación con Boccara

Las **10 técnicas** se distribuyen transversalmente:
- Explorar desde entradas/salidas.
- Identificar *strongholds*.
- Usar stacks representativos (debug/profiling).
- Filtrar por flujo de control y términos clave.
- Leer en equipo o usar IA como copiloto, no como reemplazo.

## Mapa resumido

| 🧩 **Bloque** | **Técnicas aplicadas (Boccara)** | **Herramientas / método** |
|---------------|----------------------------------|---------------------------|
| **1 — Comprensión** | Stronghold, Inputs/Outputs, Well-chosen stacks | Doxygen, ctags, clangd, cloc, lizard |
| **2 — Debugging** | Trabajar backwards, Well-chosen stacks, Distinguir acción principal | GDB, backtrace |
| **3 — Profiling** | Well-chosen stacks, Filtrar flujo de control | Perf, LTTng |
| **4 — IA** | Stronghold narrativo, Decoupling, Practice functions | LLMs, PlantUML |

## Notas

- Las prácticas ayudan a decidir **qué intervenir** y **qué no**, según impacto.
- Cada fase produce insumos para planificar pruebas y estimar esfuerzo.
- No reemplaza criterio ni revisión en equipo.
