# TOOLS.md

## 📊 Herramientas OSS/Gratuitas para estimar métricas y complejidad en C++

Este documento resume utilidades recomendadas para:

* Contar líneas de código (LOC)
* Calcular complejidad ciclomática
* Realizar análisis estático básico

---

## ✅ Herramientas seleccionadas

### 1. [**cloc**](https://github.com/AlDanial/cloc)

* Cuenta líneas de código, comentarios y líneas en blanco.
* Multi-lenguaje.
* No mide complejidad ni estructura de control.
* Ejemplo:

  ```bash
  cloc ./src
  ```

---

### 2. [**sloccount**](https://www.dwheeler.com/sloccount/)

* Similar a `cloc`, pero menos mantenido.
* Muestra LOC y coste aproximado (COCOMO).
* Ejemplo:

  ```bash
  sloccount ./src
  ```

---

### 3. [**lizard**](https://github.com/terryyin/lizard)

* Mide complejidad ciclomática por función/método.
* Reporta LOC por función, nesting, número de parámetros.
* Soporta C++ bien.
* Ejemplo:

  ```bash
  lizard ./src
  ```

---

### 4. [**pmccabe**](https://linux.die.net/man/1/pmccabe)

* Calcula complejidad ciclomática.
* Ligero y directo.
* Ejemplo:

  ```bash
  pmccabe *.cpp
  ```

---

### 5. [**cppcheck**](http://cppcheck.sourceforge.net/)

* Análisis estático con detección de bugs potenciales.
* Reporta complejidad ciclomática por función.
* Soporta `compile_commands.json` para un análisis más preciso.
* Ejemplo:

  ```bash
  cppcheck --enable=all --inconclusive ./src
  ```

---

### 6. [**clang-tidy**](https://clang.llvm.org/extra/clang-tidy/)

* Análisis estático extensible.
* No es un medidor de métricas en sí, pero algunos checks (`readability-`, `modernize-`) ayudan a mejorar la estructura y legibilidad.
* Ejemplo:

  ```bash
  clang-tidy archivo.cpp --fix
  ```

---

## ✅ Tabla comparativa

| Herramienta  | LOC | Complejidad   | OO Métricas   | OSS/Gratis |
| ------------ | --- | ------------- | ------------- | ---------- |
| `cloc`       | ✅   | ❌             | ❌             | ✅          |
| `sloccount`  | ✅   | ❌             | ❌             | ✅          |
| `lizard`     | ✅   | ✅             | ⚪ (limitado)  | ✅          |
| `pmccabe`    | ⚪   | ✅             | ❌             | ✅          |
| `cppcheck`   | ⚪   | ✅             | ⚪ (limitado)  | ✅          |
| `clang-tidy` | ⚪   | ⚪ (indirecto) | ⚪ (indirecto) | ✅          |

---

## ⚡ Recomendación práctica

* Para **tamaño y conteo**: `cloc`
* Para **complejidad**: `lizard` o `pmccabe`
* Para **análisis estático**: `cppcheck` o `clang-tidy`
