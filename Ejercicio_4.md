<img src="https://r2cdn.perplexity.ai/pplx-full-logo-primary-dark%402x.png" class="logo" width="120"/>

# 2,3,7,15 y 23 ¿y el puto resto?

A continuación tienes el **documento maestro ampliado** para la resolución de **todos los tests del ejercicio 4 (API)**, con el resultado esperado, explicación y esquema de implementación para cada uno, basándome en el enunciado oficial[^2][^4][^7], el código de tests y la experiencia de prácticas resueltas anteriores[^3][^5]:

---

# Ejercicio 4 — Integración en la API (PR3_EX4_1 a PR3_EX4_24)

## Resumen

Debes implementar en `api.c` y `api.h` las funciones que conectan la API con los módulos internos (`film`, `person`, `subscription`). Cada función debe delegar en los métodos internos implementados en los ejercicios 1, 2 y 3, y devolver los datos en el formato y orden que espera el test.

---

## Tabla de tests, resultado esperado y guía de implementación

| Test | Descripción / Entrada | Salida Esperada / Validación | Esquema de Implementación |
| :-- | :-- | :-- | :-- |
| **PR3_EX4_1** | Añadir película a watchlist (entrada válida) | E_SUCCESS | Buscar suscripción por ID, añadir película a su stack. |
| **PR3_EX4_2** | Obtener película más larga (catálogo vacío) | E_FILM_NOT_FOUND | Si lista vacía, devolver error. |
| **PR3_EX4_3** | Obtener película más larga | E_SUCCESS + CSVEntry correcto | Usar `filmListLongestFind`, convertir a CSVEntry. |
| **PR3_EX4_4** | Obtener película gratuita más larga (vacío) | E_FILM_NOT_FOUND | Si lista de gratuitas vacía, devolver error. |
| **PR3_EX4_5** | Obtener película gratuita más larga | E_SUCCESS + CSVEntry correcto | Usar `freeFilmListLongestFind`, convertir a CSVEntry. |
| **PR3_EX4_6** | Ordenar catálogo por año (vacío) | E_SUCCESS | Llamar a `filmCatalogSortByYear`, debe ser robusto con listas vacías. |
| **PR3_EX4_7** | Ordenar catálogo por año | E_SUCCESS + flag actualizado | Llamar a `filmCatalogSortByYear`, comprobar que `sortedByDate` es `true`. |
| **PR3_EX4_8** | Obtener película más antigua (vacío) | E_FILM_NOT_FOUND | Si lista vacía, devolver error. |
| **PR3_EX4_9** | Obtener película gratuita más antigua (vacío) | E_FILM_NOT_FOUND | Si lista de gratuitas vacía, devolver error. |
| **PR3_EX4_10** | Obtener película más antigua | E_SUCCESS + CSVEntry correcto | Usar `filmCatalogOldestFind`, convertir a CSVEntry. |
| **PR3_EX4_11** | Obtener película gratuita más antigua | E_SUCCESS + CSVEntry correcto | Usar `filmCatalogOldestFind` con parámetro free, convertir a CSVEntry. |
| **PR3_EX4_12** | Ordenar catálogo por rating (vacío) | E_SUCCESS | Llamar a `filmCatalogSortByRating`, robusto con listas vacías. |
| **PR3_EX4_13** | Ordenar catálogo por rating | E_SUCCESS + orden correcto | Llamar a `filmCatalogSortByRating`, comprobar orden descendente. |
| **PR3_EX4_14** | Actualizar nivel VIP (catálogo vacío) | E_SUCCESS | Llamar a `updateVipLevel`, debe funcionar aunque no haya personas. |
| **PR3_EX4_15** | Actualizar nivel VIP | E_SUCCESS + valores correctos | Llamar a `updateVipLevel`, comprobar cálculo correcto. |
| **PR3_EX4_16** | Ordenar personas por VIP (vacío) | E_SUCCESS | Llamar a `peopleSortByVipLevelQuickSort`, robusto con listas vacías. |
| **PR3_EX4_17** | Ordenar personas por VIP | E_SUCCESS + orden correcto | Llamar a `peopleSortByVipLevelQuickSort`, comprobar orden. |
| **PR3_EX4_18** | Ordenar personas por documento (vacío) | E_SUCCESS | Llamar a `peopleSortByDocumentQuickSort`, robusto con listas vacías. |
| **PR3_EX4_19** | Ordenar personas por documento | E_SUCCESS + orden correcto | Llamar a `peopleSortByDocumentQuickSort`, comprobar orden. |
| **PR3_EX4_20** | Obtener película popular (vacío) | E_FILM_NOT_FOUND | Si no hay películas, devolver error. |
| **PR3_EX4_21** | Obtener película popular | E_SUCCESS + CSVEntry correcto | Usar `popularFilmFind`, convertir a CSVEntry. |
| **PR3_EX4_22** | Obtener suscripciones por documento (vacío) | E_SUCCESS + lista vacía | Llamar a `subscriptionsFindByDocument`, devolver CSVData vacío. |
| **PR3_EX4_23** | Obtener suscripciones por documento | E_SUCCESS + CSVData correcto | Llamar a `subscriptionsFindByDocument`, devolver CSVData con todas las subscripciones para ese documento. |
| **PR3_EX4_24** | Obtener suscripciones por documento | E_SUCCESS + CSVData correcto | Como el anterior, pero con otro documento. |


---

## Ejemplo de implementación para cada caso

### PR3_EX4_1 - Añadir a Watchlist

```c
tApiError apiAddToWatchlist(tApiData* data, tCSVEntry* entry) {
    int subId = csvGetAsInteger(entry, 6);
    tSubscription* sub = subscriptionsFindHash(&data->subscriptions, subId);
    if (!sub) return E_SUBSCRIPTION_NOT_FOUND;  // CORREGIDO
    tFilm film;
    filmParse(&film, *entry);
    filmStackPush(&sub->stack, &film);
    return E_SUCCESS;  // CORREGIDO
}
```


### PR3_EX4_2 - Obtener película más larga (catálogo vacío)

```c
tApiError apiGetLongestFilm(tApiData* data, tCSVEntry* entry) {
    if (!data || data->catalog.filmList.count == 0) {
        csvInitEntry(entry);
        return E_FILM_NOT_FOUND;  // CORREGIDO
    }
    tFilm* film = filmListLongestFind(&data->catalog.filmList);
    if (!film) return E_FILM_NOT_FOUND;  // CORREGIDO
    filmToCSVEntry(film, entry);
    return E_SUCCESS;  // CORREGIDO
}
```


### PR3_EX4_3 - Obtener película más larga

Igual que el anterior, pero la lista no está vacía y debe devolver la película correcta en formato CSVEntry.

### PR3_EX4_4/5 - Obtener película gratuita más larga

Igual que PR3_EX4_2/3, pero usando `freeFilmListLongestFind`.

### PR3_EX4_6/7 - Ordenar catálogo por año

```c
tApiError apiSortCatalogByYear(tApiData* data) {
    if (!data) return E_INVALID_ENTRY_TYPE;  // CORREGIDO
    tApiError err = filmCatalogSortByYear(&data->catalog);
    if (err == E_SUCCESS) data->catalog.sortedByDate = true;  // CORREGIDO
    return err;
}
```


### PR3_EX4_8/9/10/11 - Obtener película más antigua (o gratuita)

Usar `filmCatalogOldestFind` con el parámetro adecuado y convertir a CSVEntry.

### PR3_EX4_12/13 - Ordenar catálogo por rating

```c
tApiError apiSortCatalogByRating(tApiData* data) {
    return filmCatalogSortByRating(&data->catalog);
}
```


### PR3_EX4_14/15 - Actualizar nivel VIP

```c
tApiError apiUpdateVipLevel(tApiData* data) {
    return updateVipLevel(&data->people, &data->subscriptions);
}
```


### PR3_EX4_16/17 - Ordenar personas por VIP

```c
tApiError apiSortPeopleByVipLevel(tApiData* data) {
    return peopleSortByVipLevelQuickSort(&data->people);
}
```


### PR3_EX4_18/19 - Ordenar personas por documento

```c
tApiError apiSortPeopleByDocument(tApiData* data) {
    return peopleSortByDocumentQuickSort(&data->people);
}
```


### PR3_EX4_20/21 - Obtener película popular

Usar `popularFilmFind` y convertir a CSVEntry.

### PR3_EX4_22/23/24 - Obtener suscripciones por documento

```c
tApiError apiGetSubscriptionsByDocument(tApiData* data, tCSVData* out, const char* document) {
    tSubscriptions* subs = subscriptionsFindByDocument(&data->subscriptions, document);
    if (!subs || subs->count == 0) {
        csvInitData(out);
        return E_SUCCESS;
    }
    for (int i = 0; i < subs->count; ++i) {
        tCSVEntry entry;
        subscriptionToCSVEntry(&subs->elems[i], &entry);
        csvAddEntry(out, &entry);
    }
    subscriptionsFree(subs);
    return E_SUCCESS;
}
```


---

## Consejos para el resto de tests

- **Siempre comprueba errores y casos vacíos** antes de operar sobre listas o estructuras.
- **Convierte correctamente los datos** entre estructuras internas y CSVEntry/CSVData.
- **Asegúrate de que los métodos internos están bien implementados** y devuelven los resultados en el formato y orden esperado.
- **Gestiona la memoria**: libera cualquier estructura dinámica creada para devolver resultados.
- **Mantén la modularidad**: la API debe ser un "pegamento" entre los módulos, no reimplementar lógica interna.

---

## Referencias

- [^2] PEC3_20242_07555_enu.pdf, Ejercicio 4, páginas 10-12.
- [^3] pec3-20221-75555-sol-solucion-pec3-practicas-de-programacion-primer-semestre-ano-20222023.pdf, Ejercicio 4.
- [^4] enunciadoPEC3.pdf, Ejercicio 4.
- [^5] Exercise-1-Implementation-Report.txt, sección sobre integración y testing.
- [^7] PEC3_20242_07555_enu.txt, Ejercicio 4.

---

Este esquema cubre **todos los tests** del ejercicio 4, con el resultado esperado y la lógica que debe implementar cada función de la API para superarlos.

<div style="text-align: center">⁂</div>

[^1]: Enunciado_y_codigol.pdf

[^2]: PEC3_20242_07555_enu.pdf

[^3]: pec3-20221-75555-sol-solucion-pec3-practicas-de-programacion-primer-semestre-ano-20222023.pdf

[^4]: enunciadoPEC3.pdf

[^5]: Exercise-1-Implementation-Report.txt

[^6]: enunciadoPEC3.docx

[^7]: PEC3_20242_07555_enu.txt

[^8]: Fundamentos-de-programacion-XWiki.pdf

[^9]: Practicas-de-Programacion-XWiki.pdf

[^10]: Exercise-3-Implementation-Report.txt

[^11]: dudas.txt

