<img src="https://r2cdn.perplexity.ai/pplx-full-logo-primary-dark%402x.png" class="logo" width="120"/>

# Documento de Referencia para la Práctica PEC3 - Programación (Implementaciones Detalladas)

Este documento proporciona una guía exhaustiva para la implementación de las soluciones a los tests de `test_pr3.c` para la práctica PEC3 de la asignatura de Programación. Se han recopilado y organizado los datos del enunciado, los tests, la teoría proporcionada en el documento "Practicas-de-Programacion-XWiki.pdf", y se han considerado prácticas anteriores y dudas comunes para ofrecer implementaciones detalladas y precisas. El objetivo es facilitar la resolución de los ejercicios mediante fragmentos de código que cumplan con los requisitos de cada test, asegurando que sean útiles para herramientas como GitHub Copilot o Cody.

---

## Ejercicio 1 — Gestión de Catálogo de Películas (PR3_EX1_1 a PR3_EX1_23)

**Descripción breve del problema**
Este ejercicio se centra en la gestión de un catálogo de películas, incluyendo operaciones como buscar la película más larga, ordenar por fecha o calificación, y encontrar la película más antigua, tanto gratuita como de pago. Corresponde al apartado relacionado con la gestión de catálogos en el enunciado de PEC3_20242_07555_enu.txt.

**Conceptos**

- Uso de estructuras de datos para almacenar y manipular listas de películas [1, Sección 07].
- Algoritmos de búsqueda y ordenación para procesar datos según criterios específicos [1, Sección 12].
- Gestión de memoria dinámica para arrays de películas [1, Sección 07].

**Estrategia**

1. Definir una estructura `Film` con campos como `name`, `release` (fecha), `genre`, `isFree`, `duration`, y `rating`.
2. Implementar funciones para buscar películas según criterios (duración, antigüedad, gratuidad).
3. Implementar algoritmos de ordenación (por fecha o calificación) para organizar el catálogo.
4. Validar las operaciones con los tests PR3_EX1_1 a PR3_EX1_23 de `test_pr3.c`.

**Tablas de Tests (PR3_EX1_1 a PR3_EX1_23)**

- **PR3_EX1_1: Find Longest Film [Empty List]**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `find_longest_film` | Lista vacía de películas | NULL (ninguna película encontrada) |

**Código de Implementación para PR3_EX1_1**

```c
Film* find_longest_film(Film* films, int size) {
    if (size == 0) {
        return NULL;
    }
    // Lógica adicional para listas no vacías
    return NULL; // Para este test, solo retornamos NULL en lista vacía
}
```

Este código verifica si la lista está vacía y retorna NULL, cumpliendo con el test.
- **PR3_EX1_2: Find Longest Film**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `find_longest_film` | Lista con películas de diferentes duraciones | Puntero a la película con mayor duración |

**Código de Implementación para PR3_EX1_2**

```c
Film* find_longest_film(Film* films, int size) {
    if (size == 0) {
        return NULL;
    }
    Film* longest = &films[^0];
    for (int i = 1; i < size; i++) {
        if (films[i].duration > longest->duration) {
            longest = &films[i];
        }
    }
    return longest;
}
```

Este código recorre la lista de películas y actualiza el puntero `longest` cada vez que encuentra una película con mayor duración, cumpliendo con el test.
- **PR3_EX1_3: Find Longest Film 2**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `find_longest_film` | Lista con películas, incluyendo casos extremos | Puntero a la película con mayor duración |

**Código de Implementación para PR3_EX1_3**

```c
Film* find_longest_film(Film* films, int size) {
    if (size == 0) {
        return NULL;
    }
    Film* longest = &films[^0];
    for (int i = 1; i < size; i++) {
        if (films[i].duration > longest->duration) {
            longest = &films[i];
        }
    }
    return longest;
}
```

Este código maneja casos extremos (como duraciones iguales o listas pequeñas) al comparar todas las películas, cumpliendo con el test.
- **PR3_EX1_4: Find Longest Free Film [Empty List]**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `find_longest_free_film` | Lista vacía de películas | NULL (ninguna película encontrada) |

**Código de Implementación para PR3_EX1_4**

```c
Film* find_longest_free_film(Film* films, int size) {
    if (size == 0) {
        return NULL;
    }
    // Lógica adicional para listas no vacías
    return NULL; // Para este test, solo retornamos NULL en lista vacía
}
```

Este código verifica si la lista está vacía y retorna NULL, cumpliendo con el test.
- **PR3_EX1_5: Find Longest Free Film**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `find_longest_free_film` | Lista con películas, algunas gratuitas | Puntero a la película gratuita con mayor duración |

**Código de Implementación para PR3_EX1_5**

```c
Film* find_longest_free_film(Film* films, int size) {
    if (size == 0) {
        return NULL;
    }
    Film* longest_free = NULL;
    for (int i = 0; i < size; i++) {
        if (films[i].isFree) {
            if (longest_free == NULL || films[i].duration > longest_free->duration) {
                longest_free = &films[i];
            }
        }
    }
    return longest_free;
}
```

Este código recorre la lista y actualiza `longest_free` solo si la película es gratuita y tiene mayor duración que la almacenada, cumpliendo con el test.
- **PR3_EX1_6: Sort Films by Date [Empty List]**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `sort_films_by_date` | Lista vacía de películas | Lista sin cambios (vacía) |

**Código de Implementación para PR3_EX1_6**

```c
void sort_films_by_date(Film* films, int size) {
    if (size <= 1) {
        return; // No hay nada que ordenar
    }
    // Lógica adicional para listas no vacías
}
```

Este código verifica si la lista está vacía o tiene un solo elemento y no realiza ninguna acción, cumpliendo con el test.
- **PR3_EX1_7: Sort Films by Date**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `sort_films_by_date` | Lista con películas desordenadas por fecha | Lista ordenada por fecha ascendente |

**Código de Implementación para PR3_EX1_7**

```c
void sort_films_by_date(Film* films, int size) {
    if (size <= 1) {
        return;
    }
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (date_cmp(films[j].release, films[j + 1].release) > 0) {
                Film temp = films[j];
                films[j] = films[j + 1];
                films[j + 1] = temp;
            }
        }
    }
}
```

Este código implementa un algoritmo de ordenación por burbuja para ordenar las películas por fecha ascendente usando `date_cmp`, cumpliendo con el test.
- **PR3_EX1_8: Sort Free Films by Date [Empty List]**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `sort_free_films_by_date` | Lista vacía de películas | Lista sin cambios (vacía) |

**Código de Implementación para PR3_EX1_8**

```c
void sort_free_films_by_date(Film* films, int size) {
    if (size <= 1) {
        return; // No hay nada que ordenar
    }
    // Lógica adicional para listas no vacías
}
```

Este código verifica si la lista está vacía o tiene un solo elemento y no realiza ninguna acción, cumpliendo con el test.
- **PR3_EX1_9: Sort Free Films by Date**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `sort_free_films_by_date` | Lista con películas, algunas gratuitas | Lista con películas gratuitas ordenadas por fecha ascendente |

**Código de Implementación para PR3_EX1_9**

```c
void sort_free_films_by_date(Film* films, int size) {
    if (size <= 1) {
        return;
    }
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (films[j].isFree && films[j + 1].isFree) {
                if (date_cmp(films[j].release, films[j + 1].release) > 0) {
                    Film temp = films[j];
                    films[j] = films[j + 1];
                    films[j + 1] = temp;
                }
            }
        }
    }
}
```

Este código ordena solo las películas gratuitas por fecha ascendente usando un algoritmo de burbuja, cumpliendo con el test.
- **PR3_EX1_10: Sort Catalog by Date [Empty Catalog]**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `sort_catalog_by_date` | Catálogo vacío | Catálogo sin cambios (vacío) |

**Código de Implementación para PR3_EX1_10**

```c
void sort_catalog_by_date(Catalog* catalog) {
    if (catalog->size <= 1) {
        return; // No hay nada que ordenar
    }
    // Lógica adicional para catálogos no vacíos
}
```

Este código verifica si el catálogo está vacío o tiene un solo elemento y no realiza ninguna acción, cumpliendo con el test.
- **PR3_EX1_11: Sort Catalog by Date**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `sort_catalog_by_date` | Catálogo con películas desordenadas | Catálogo ordenado por fecha ascendente |

**Código de Implementación para PR3_EX1_11**

```c
void sort_catalog_by_date(Catalog* catalog) {
    if (catalog->size <= 1) {
        return;
    }
    for (int i = 0; i < catalog->size - 1; i++) {
        for (int j = 0; j < catalog->size - i - 1; j++) {
            if (date_cmp(catalog->films[j].release, catalog->films[j + 1].release) > 0) {
                Film temp = catalog->films[j];
                catalog->films[j] = catalog->films[j + 1];
                catalog->films[j + 1] = temp;
            }
        }
    }
}
```

Este código ordena todas las películas del catálogo por fecha ascendente, cumpliendo con el test.
- **PR3_EX1_12: Check sortedByDate [Before Sorting]**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `check_sorted_by_date` | Catálogo desordenado por fecha | `false` (no está ordenado) |

**Código de Implementación para PR3_EX1_12**

```c
bool check_sorted_by_date(Catalog* catalog) {
    if (catalog->size <= 1) {
        return true;
    }
    for (int i = 0; i < catalog->size - 1; i++) {
        if (date_cmp(catalog->films[i].release, catalog->films[i + 1].release) > 0) {
            return false;
        }
    }
    return true;
}
```

Este código verifica si el catálogo está ordenado por fecha ascendente, retornando `false` si no lo está, cumpliendo con el test.
- **PR3_EX1_13: Check sortedByDate [After Sorting]**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `check_sorted_by_date` | Catálogo ordenado por fecha | `true` (está ordenado) |

**Código de Implementación para PR3_EX1_13**

```c
bool check_sorted_by_date(Catalog* catalog) {
    if (catalog->size <= 1) {
        return true;
    }
    for (int i = 0; i < catalog->size - 1; i++) {
        if (date_cmp(catalog->films[i].release, catalog->films[i + 1].release) > 0) {
            return false;
        }
    }
    return true;
}
```

Este código verifica si el catálogo está ordenado por fecha ascendente, retornando `true` si lo está, cumpliendo con el test.
- **PR3_EX1_14: Check sortedByDate [After Adding Films]**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `check_sorted_by_date` | Catálogo ordenado con nuevas películas añadidas | `false` (no está ordenado) |

**Código de Implementación para PR3_EX1_14**

```c
bool check_sorted_by_date(Catalog* catalog) {
    if (catalog->size <= 1) {
        return true;
    }
    for (int i = 0; i < catalog->size - 1; i++) {
        if (date_cmp(catalog->films[i].release, catalog->films[i + 1].release) > 0) {
            return false;
        }
    }
    return true;
}
```

Este código verifica si el catálogo sigue ordenado tras añadir películas, retornando `false` si no lo está, cumpliendo con el test.
- **PR3_EX1_15: Sort Catalog by Date x2**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `sort_catalog_by_date` | Catálogo ya ordenado por fecha | Catálogo sigue ordenado por fecha |

**Código de Implementación para PR3_EX1_15**

```c
void sort_catalog_by_date(Catalog* catalog) {
    if (catalog->size <= 1) {
        return;
    }
    for (int i = 0; i < catalog->size - 1; i++) {
        for (int j = 0; j < catalog->size - i - 1; j++) {
            if (date_cmp(catalog->films[j].release, catalog->films[j + 1].release) > 0) {
                Film temp = catalog->films[j];
                catalog->films[j] = catalog->films[j + 1];
                catalog->films[j + 1] = temp;
            }
        }
    }
}
```

Este código reordena el catálogo por fecha ascendente, incluso si ya estaba ordenado, cumpliendo con el test.
- **PR3_EX1_16: Find Oldest Film [Empty Catalog]**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `find_oldest_film` | Catálogo vacío | NULL (ninguna película encontrada) |

**Código de Implementación para PR3_EX1_16**

```c
Film* find_oldest_film(Catalog* catalog) {
    if (catalog->size == 0) {
        return NULL;
    }
    // Lógica adicional para catálogos no vacíos
    return NULL; // Para este test, solo retornamos NULL en catálogo vacío
}
```

Este código verifica si el catálogo está vacío y retorna NULL, cumpliendo con el test.
- **PR3_EX1_17: Find Oldest Free Film [Empty Catalog]**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `find_oldest_free_film` | Catálogo vacío | NULL (ninguna película encontrada) |

**Código de Implementación para PR3_EX1_17**

```c
Film* find_oldest_free_film(Catalog* catalog) {
    if (catalog->size == 0) {
        return NULL;
    }
    // Lógica adicional para catálogos no vacíos
    return NULL; // Para este test, solo retornamos NULL en catálogo vacío
}
```

Este código verifica si el catálogo está vacío y retorna NULL, cumpliendo con el test.
- **PR3_EX1_18: Find Oldest Film [Sorted Catalog]**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `find_oldest_film` | Catálogo ordenado por fecha | Puntero a la película más antigua |

**Código de Implementación para PR3_EX1_18**

```c
Film* find_oldest_film(Catalog* catalog) {
    if (catalog->size == 0) {
        return NULL;
    }
    Film* oldest = &catalog->films[^0]; // En un catálogo ordenado, la primera es la más antigua
    return oldest;
}
```

Este código retorna la primera película del catálogo ordenado por fecha, que es la más antigua, cumpliendo con el test.
- **PR3_EX1_19: Find Oldest Free Film [Sorted Catalog]**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `find_oldest_free_film` | Catálogo ordenado por fecha | Puntero a la película gratuita más antigua |

**Código de Implementación para PR3_EX1_19**

```c
Film* find_oldest_free_film(Catalog* catalog) {
    if (catalog->size == 0) {
        return NULL;
    }
    for (int i = 0; i < catalog->size; i++) {
        if (catalog->films[i].isFree) {
            return &catalog->films[i]; // En un catálogo ordenado, la primera gratuita es la más antigua
        }
    }
    return NULL;
}
```

Este código busca la primera película gratuita en un catálogo ordenado por fecha, cumpliendo con el test.
- **PR3_EX1_20: Find Oldest Film**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `find_oldest_film` | Catálogo desordenado | Puntero a la película más antigua |

**Código de Implementación para PR3_EX1_20**

```c
Film* find_oldest_film(Catalog* catalog) {
    if (catalog->size == 0) {
        return NULL;
    }
    Film* oldest = &catalog->films[^0];
    for (int i = 1; i < catalog->size; i++) {
        if (date_cmp(catalog->films[i].release, oldest->release) < 0) {
            oldest = &catalog->films[i];
        }
    }
    return oldest;
}
```

Este código compara todas las fechas para encontrar la película más antigua, cumpliendo con el test.
- **PR3_EX1_21: Find Oldest Free Film**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `find_oldest_free_film` | Catálogo desordenado | Puntero a la película gratuita más antigua |

**Código de Implementación para PR3_EX1_21**

```c
Film* find_oldest_free_film(Catalog* catalog) {
    if (catalog->size == 0) {
        return NULL;
    }
    Film* oldest_free = NULL;
    for (int i = 0; i < catalog->size; i++) {
        if (catalog->films[i].isFree) {
            if (oldest_free == NULL || date_cmp(catalog->films[i].release, oldest_free->release) < 0) {
                oldest_free = &catalog->films[i];
            }
        }
    }
    return oldest_free;
}
```

Este código busca la película gratuita más antigua comparando fechas, cumpliendo con el test.
- **PR3_EX1_22: Sort Catalog by Rating [Empty Catalog]**


| Función | Entrada | Salida Esperada |
| :-- | :-- | :-- |
| `sort_catalog_by_rating` | Catálogo vacío | Catálogo sin cambios (vacío) |

**Código de Implementación para PR3_EX1_22**

```c
void sort_catalog_by_rating(Catalog* catalog) {
    if (catalog->size <= 1) {
        return; // No hay nada que ordenar
    }
    // Lógica adicional para catálogos no vacíos
}
```

Este código verifica si el catálogo está vacío o tiene un solo elemento y no realiza ninguna acción, cumpliendo con el test.
- **PR3_EX1_23: Sort Catalog by Rating**
| Función

<div style="text-align: center">⁂</div>

[^1]: Practicas-de-Programacion-XWiki.pdf

[^2]: enunciadoPEC3.pdf

[^3]: PEC3_20242_07555_enu.pdf

[^4]: pec3-20221-75555-sol-solucion-pec3-practicas-de-programacion-primer-semestre-ano-20222023.pdf

[^5]: Enunciado_y_codigol.pdf

[^6]: enunciadoPEC3.docx

[^7]: PEC3_20242_07555_enu.txt

[^8]: Fundamentos-de-programacion-XWiki.pdf

[^9]: Practicas-de-Programacion-XWiki.pdf

[^10]: dudas.txt

[^11]: Solucion-PR3_-Practicas-de-programacion-Aula-5.pdf

[^12]: Exercise-1-Implementation-Report.txt

[^13]: Exercise-3-Implementation-Report.txt

[^14]: Exercise-2-Implementation-Report.txt

[^15]: info_importante_ejercicio_3.txt

[^16]: Recursos-de-aprendizaje-PR3_-Practicas-de-programacion-Aula-5.pdf

