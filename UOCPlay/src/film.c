#include "film.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Initialize a film
void film_init(tFilm* data, const char* name, tTime duration, tFilmGenre genre, tDate release, float rating, bool isFree) {
    assert(data != NULL);
    assert(name != NULL);
    
    data->name = (char*)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(data->name, name);
    data->duration = duration;
    data->genre = genre;
    data->release = release;
    data->rating = rating;
    data->isFree = isFree;
}

// Free film memory
void film_free(tFilm* data) {
    assert(data != NULL);
    
    if (data->name != NULL) {
        free(data->name);
        data->name = NULL;
    }
}

// Copy a film
void film_cpy(tFilm* dest, tFilm src) {
    assert(dest != NULL);
    
    dest->name = (char*)malloc((strlen(src.name) + 1) * sizeof(char));
    strcpy(dest->name, src.name);
    dest->duration = src.duration;
    dest->genre = src.genre;
    dest->release = src.release;
    dest->rating = src.rating;
    dest->isFree = src.isFree;
}

// Compare two films
bool film_equals(tFilm film1, tFilm film2) {
    if (strcmp(film1.name, film2.name) != 0) return false;
    if (film1.duration.hour != film2.duration.hour) return false;
    if (film1.duration.minutes != film2.duration.minutes) return false;
    if (film1.genre != film2.genre) return false;
    if (date_cmp(film1.release, film2.release) != 0) return false;
    if (film1.rating != film2.rating) return false;
    if (film1.isFree != film2.isFree) return false;
    return true;
}

// Get film as string
void film_get(tFilm film, char* buffer) {
    assert(buffer != NULL);
    
    sprintf(buffer, "%s;%02d:%02d;%d;%02d/%02d/%04d;%.1f;%d",
            film.name,
            film.duration.hour, film.duration.minutes,
            film.genre,
            film.release.day, film.release.month, film.release.year,
            film.rating,
            film.isFree ? 1 : 0);
}

// Parse film from CSV entry
void film_parse(tFilm* film, tCSVEntry entry) {
    assert(film != NULL);
    
    char buffer[256];
    
    // Parse name
    csv_getAsString(entry, 0, buffer, sizeof(buffer));
    film->name = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
    strcpy(film->name, buffer);
    
    // Parse duration
    csv_getAsString(entry, 1, buffer, sizeof(buffer));
    sscanf(buffer, "%d:%d", &film->duration.hour, &film->duration.minutes);
    
    // Parse genre
    film->genre = csv_getAsInteger(entry, 2);
    
    // Parse release date
    csv_getAsString(entry, 3, buffer, sizeof(buffer));
    date_parse(&film->release, buffer);
    
    // Parse rating
    film->rating = csv_getAsReal(entry, 4);
    
    // Parse isFree
    film->isFree = csv_getAsInteger(entry, 5) == 1;
}

// Initialize film catalog
tApiError film_catalog_init(tFilmCatalog* catalog) {
    assert(catalog != NULL);
    
    catalog->filmList.first = NULL;
    catalog->filmList.last = NULL;
    catalog->filmList.count = 0;
    
    catalog->freeFilmList.first = NULL;
    catalog->freeFilmList.last = NULL;
    catalog->freeFilmList.count = 0;
    
    catalog->sortedByDate = false;
    
    return E_SUCCESS;
}

// Free film catalog
tApiError film_catalog_free(tFilmCatalog* catalog) {
    assert(catalog != NULL);
    
    // Free film list
    tFilmListNode* current = catalog->filmList.first;
    while (current != NULL) {
        tFilmListNode* next = current->next;
        film_free(&current->elem);
        free(current);
        current = next;
    }
    
    // Free free film list
    tFreeFilmListNode* freeCurrent = catalog->freeFilmList.first;
    while (freeCurrent != NULL) {
        tFreeFilmListNode* next = freeCurrent->next;
        film_free(freeCurrent->elem);
        free(freeCurrent->elem);
        free(freeCurrent);
        freeCurrent = next;
    }
    
    catalog->filmList.first = NULL;
    catalog->filmList.last = NULL;
    catalog->filmList.count = 0;
    catalog->freeFilmList.first = NULL;
    catalog->freeFilmList.last = NULL;
    catalog->freeFilmList.count = 0;
    
    return E_SUCCESS;
}

// Get film catalog length
int film_catalog_len(tFilmCatalog catalog) {
    return catalog.filmList.count;
}

// Get free film catalog length
int film_catalog_freeLen(tFilmCatalog catalog) {
    return catalog.freeFilmList.count;
}

// Add film to catalog
tApiError film_catalog_add(tFilmCatalog* catalog, tFilm film) {
    assert(catalog != NULL);
    
    // Check if film already exists
    tFilmListNode* current = catalog->filmList.first;
    while (current != NULL) {
        if (strcmp(current->elem.name, film.name) == 0) {
            return E_FILM_DUPLICATED;
        }
        current = current->next;
    }
    
    // Add to main film list
    tFilmListNode* newNode = (tFilmListNode*)malloc(sizeof(tFilmListNode));
    if (newNode == NULL) return E_MEMORY_ERROR;
    
    film_cpy(&newNode->elem, film);
    newNode->next = NULL;
    
    if (catalog->filmList.first == NULL) {
        catalog->filmList.first = newNode;
        catalog->filmList.last = newNode;
    } else {
        catalog->filmList.last->next = newNode;
        catalog->filmList.last = newNode;
    }
    catalog->filmList.count++;
    
    // Add to free film list if it's free
    if (film.isFree) {
        tFreeFilmListNode* freeNode = (tFreeFilmListNode*)malloc(sizeof(tFreeFilmListNode));
        if (freeNode == NULL) return E_MEMORY_ERROR;
        
        freeNode->elem = (tFilm*)malloc(sizeof(tFilm));
        film_cpy(freeNode->elem, film);
        freeNode->next = NULL;
        
        if (catalog->freeFilmList.first == NULL) {
            catalog->freeFilmList.first = freeNode;
            catalog->freeFilmList.last = freeNode;
        } else {
            catalog->freeFilmList.last->next = freeNode;
            catalog->freeFilmList.last = freeNode;
        }
        catalog->freeFilmList.count++;
    }
    
    catalog->sortedByDate = false;
    return E_SUCCESS;
}

// Find film in list
tFilm* filmList_find(tFilmList list, const char* name) {
    tFilmListNode* current = list.first;
    while (current != NULL) {
        if (strcmp(current->elem.name, name) == 0) {
            return &current->elem;
        }
        current = current->next;
    }
    return NULL;
}

// Find longest film
tFilm* filmList_longestFind(tFilmList list) {
    if (list.first == NULL) return NULL;
    
    tFilm* longest = &list.first->elem;
    tFilmListNode* current = list.first->next;
    
    while (current != NULL) {
        int currentDuration = current->elem.duration.hour * 60 + current->elem.duration.minutes;
        int longestDuration = longest->duration.hour * 60 + longest->duration.minutes;
        
        if (currentDuration > longestDuration || 
            (currentDuration == longestDuration && strcmp(current->elem.name, longest->name) < 0)) {
            longest = &current->elem;
        }
        current = current->next;
    }
    
    return longest;
}

// Find longest free film
tFilm* freeFilmList_longestFind(tFreeFilmList list) {
    if (list.first == NULL) return NULL;
    
    tFilm* longest = list.first->elem;
    tFreeFilmListNode* current = list.first->next;
    
    while (current != NULL) {
        int currentDuration = current->elem->duration.hour * 60 + current->elem->duration.minutes;
        int longestDuration = longest->duration.hour * 60 + longest->duration.minutes;
        
        if (currentDuration > longestDuration || 
            (currentDuration == longestDuration && strcmp(current->elem->name, longest->name) < 0)) {
            longest = current->elem;
        }
        current = current->next;
    }
    
    return longest;
}

// Sort film list by year using bubble sort
tApiError filmList_SortByYear_Bubble(tFilmList* list) {
    if (list == NULL || list->first == NULL || list->first->next == NULL) {
        return E_SUCCESS;
    }

    bool swapped;
    tFilmListNode *ptr1;
    tFilmListNode *lptr = NULL;

    do {
        swapped = false;
        ptr1 = list->first;
        
        while (ptr1->next != lptr) {
            int cmp = date_cmp(ptr1->elem.release, ptr1->next->elem.release);
            if (cmp > 0 || (cmp == 0 && strcmp(ptr1->elem.name, ptr1->next->elem.name) > 0)) {
                // Swap elements
                tFilm temp;
                film_cpy(&temp, ptr1->elem);
                film_free(&ptr1->elem);
                film_cpy(&ptr1->elem, ptr1->next->elem);
                film_free(&ptr1->next->elem);
                film_cpy(&ptr1->next->elem, temp);
                film_free(&temp);
                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    // Update last pointer
    tFilmListNode *lastNode = list->first;
    while (lastNode->next != NULL) {
        lastNode = lastNode->next;
    }
    list->last = lastNode;

    return E_SUCCESS;
}

// Sort free film list by year using bubble sort
tApiError freeFilmList_SortByYear_Bubble(tFreeFilmList* list) {
    if (list == NULL || list->first == NULL || list->first->next == NULL) {
        return E_SUCCESS;
    }

    bool swapped;
    tFreeFilmListNode *ptr1;
    tFreeFilmListNode *lptr = NULL;

    do {
        swapped = false;
        ptr1 = list->first;
        
        while (ptr1->next != lptr) {
            int cmp = date_cmp(ptr1->elem->release, ptr1->next->elem->release);
            if (cmp > 0 || (cmp == 0 && strcmp(ptr1->elem->name, ptr1->next->elem->name) > 0)) {
                // Swap pointers
                tFilm* temp = ptr1->elem;
                ptr1->elem = ptr1->next->elem;
                ptr1->next->elem = temp;
                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    // Update last pointer
    tFreeFilmListNode *lastNode = list->first;
    while (lastNode->next != NULL) {
        lastNode = lastNode->next;
    }
    list->last = lastNode;

    return E_SUCCESS;
}

// Sort catalog by year
tApiError filmCatalog_SortByYear(tFilmCatalog* catalog) {
    assert(catalog != NULL);
    
    tApiError error1 = filmList_SortByYear_Bubble(&catalog->filmList);
    tApiError error2 = freeFilmList_SortByYear_Bubble(&catalog->freeFilmList);
    
    if (error1 == E_SUCCESS && error2 == E_SUCCESS) {
        catalog->sortedByDate = true;
        return E_SUCCESS;
    }
    
    return (error1 != E_SUCCESS) ? error1 : error2;
}

// Find oldest film
tFilm* filmCatalog_OldestFind(tFilmCatalog catalog, bool free) {
    if (free) {
        // For free films
        if (catalog.freeFilmList.count == 0) {
            return NULL;
        }
        
        if (catalog.sortedByDate && catalog.freeFilmList.first != NULL) {
            return catalog.freeFilmList.first->elem;
        }
        
        tFreeFilmListNode *current = catalog.freeFilmList.first;
        tFilm *oldest = NULL;
        
        while (current != NULL) {
            if (oldest == NULL || date_cmp(current->elem->release, oldest->release) < 0 ||
                (date_cmp(current->elem->release, oldest->release) == 0 && strcmp(current->elem->name, oldest->name) < 0)) {
                oldest = current->elem;
            }
            current = current->next;
        }
        
        return oldest;
    } else {
        // For all films
        if (catalog.filmList.count == 0) {
            return NULL;
        }
        
        if (catalog.sortedByDate && catalog.filmList.first != NULL) {
            return &catalog.filmList.first->elem;
        }
        
        tFilmListNode *current = catalog.filmList.first;
        tFilm *oldest = NULL;
        
        while (current != NULL) {
            if (oldest == NULL || date_cmp(current->elem.release, oldest->release) < 0 ||
                (date_cmp(current->elem.release, oldest->release) == 0 && strcmp(current->elem.name, oldest->name) < 0)) {
                oldest = &current->elem;
            }
            current = current->next;
        }
        
        return oldest;
    }
}

// Sort catalog by rating using selection sort
tApiError filmCatalog_SortByRating(tFilmCatalog* catalog) {
    assert(catalog != NULL);
    
    // Sort main film list
    tFilmListNode* current = catalog->filmList.first;
    while (current != NULL) {
        tFilmListNode* maxNode = current;
        tFilmListNode* temp = current->next;
        
        while (temp != NULL) {
            if (temp->elem.rating > maxNode->elem.rating ||
                (temp->elem.rating == maxNode->elem.rating && strcmp(temp->elem.name, maxNode->elem.name) < 0)) {
                maxNode = temp;
            }
            temp = temp->next;
        }
        
        if (maxNode != current) {
            // Swap elements
            tFilm tempFilm;
            film_cpy(&tempFilm, current->elem);
            film_free(&current->elem);
            film_cpy(&current->elem, maxNode->elem);
            film_free(&maxNode->elem);
            film_cpy(&maxNode->elem, tempFilm);
            film_free(&tempFilm);
        }
        
        current = current->next;
    }
    
    // Sort free film list
    tFreeFilmListNode* freeCurrent = catalog->freeFilmList.first;
    while (freeCurrent != NULL) {
        tFreeFilmListNode* maxNode = freeCurrent;
        tFreeFilmListNode* temp = freeCurrent->next;
        
        while (temp != NULL) {
            if (temp->elem->rating > maxNode->elem->rating ||
                (temp->elem->rating == maxNode->elem->rating && strcmp(temp->elem->name, maxNode->elem->name) < 0)) {
                maxNode = temp;
            }
            temp = temp->next;
        }
        
        if (maxNode != freeCurrent) {
            // Swap pointers
            tFilm* tempPtr = freeCurrent->elem;
            freeCurrent->elem = maxNode->elem;
            maxNode->elem = tempPtr;
        }
        
        freeCurrent = freeCurrent->next;
    }
    
    catalog->sortedByDate = false;
    return E_SUCCESS;
}

// Remove film from catalog
tApiError film_catalog_del(tFilmCatalog* catalog, const char* name) {
    assert(catalog != NULL);
    assert(name != NULL);
    
    // Remove from main film list
    tFilmListNode* current = catalog->filmList.first;
    tFilmListNode* prev = NULL;
    bool found = false;
    
    while (current != NULL) {
        if (strcmp(current->elem.name, name) == 0) {
            found = true;
            
            // Remove from main list
            if (prev == NULL) {
                catalog->filmList.first = current->next;
            } else {
                prev->next = current->next;
            }
            
            if (current == catalog->filmList.last) {
                catalog->filmList.last = prev;
            }
            
            // Check if it was a free film and remove from free list too
            if (current->elem.isFree) {
                tFreeFilmListNode* freeCurrent = catalog->freeFilmList.first;
                tFreeFilmListNode* freePrev = NULL;
                
                while (freeCurrent != NULL) {
                    if (strcmp(freeCurrent->elem->name, name) == 0) {
                        if (freePrev == NULL) {
                            catalog->freeFilmList.first = freeCurrent->next;
                        } else {
                            freePrev->next = freeCurrent->next;
                        }
                        
                        if (freeCurrent == catalog->freeFilmList.last) {
                            catalog->freeFilmList.last = freePrev;
                        }
                        
                        film_free(freeCurrent->elem);
                        free(freeCurrent->elem);
                        free(freeCurrent);
                        catalog->freeFilmList.count--;
                        break;
                    }
                    freePrev = freeCurrent;
                    freeCurrent = freeCurrent->next;
                }
            }
            
            film_free(&current->elem);
            free(current);
            catalog->filmList.count--;
            break;
        }
        prev = current;
        current = current->next;
    }
    
    if (!found) {
        return E_FILM_NOT_FOUND;
    }
    
    catalog->sortedByDate = false;
    return E_SUCCESS;
}