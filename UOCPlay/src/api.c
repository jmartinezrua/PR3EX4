#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "csv.h"
#include "api.h"
#include <string.h>

// Get the API version information
const char* api_version() 
{
    return "UOC PP 20242";
}

// Load data from a CSV file. If reset is true, remove previous data
tApiError api_loadData(tApiData* data, const char* filename, bool reset)
{
    tApiError error;
    FILE *fin;    
    char buffer[FILE_READ_BUFFER_SIZE];
    tCSVEntry entry;
    
    // Check input data
    assert( data != NULL );
    assert(filename != NULL);
    
    // Reset current data    
    if (reset) {
        // Remove previous information
        error = api_freeData(data);
        if (error != E_SUCCESS) {
            return error;
        }
        
        // Initialize the data
        error = api_initData(data);
        if (error != E_SUCCESS) {
            return error;
        }
    }

    // Open the input file
    fin = fopen(filename, "r");
    if (fin == NULL) {
        return E_FILE_NOT_FOUND;
    }
    
    // Read file line by line
    while (fgets(buffer, FILE_READ_BUFFER_SIZE, fin)) {
        // Remove new line character     
        buffer[strcspn(buffer, "\n\r")] = '\0';
        
        csv_initEntry(&entry);
        csv_parseEntry(&entry, buffer, NULL);
        // Add this new entry to the api Data
        error = api_addDataEntry(data, entry);
        if (error != E_SUCCESS) {
            csv_freeEntry(&entry);
            fclose(fin);
            return error;
        }
        csv_freeEntry(&entry);

    }
    
    fclose(fin);
    
    return E_SUCCESS;
}
// Initialize the data structure
tApiError api_initData(tApiData* data) {
    /////////////////////////////////
    // PR1_3b
    /////////////////////////////////
    // Check preconditions
    assert(data != NULL);
    
    people_init(&(data->people));
    subscriptions_init(&(data->subscriptions));
    film_catalog_init(&(data->films));
    /////////////////////////////////
    // PR2_3b
    /////////////////////////////////
    showList_init(&(data->shows));
    return E_SUCCESS;
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED;
}

// Add a person into the data if it does not exist
tApiError api_addPerson(tApiData* data, tCSVEntry entry) {
    /////////////////////////////////
    // PR1_3c
    /////////////////////////////////
    tPerson person;
    tApiError error;
    
    // Check preconditions
    assert(data != NULL);
    
    // Check the entry type
    if (strcmp(csv_getType(&entry), "PERSON") != 0)
        return E_INVALID_ENTRY_TYPE;
    
    // Check the number of fields
    if(csv_numFields(entry) != NUM_FIELDS_PERSON)
        return E_INVALID_ENTRY_FORMAT;
    
    // Parse a person
    person_parse(&person, entry);
    
    // Try to add the person or return an error if the person already exists
    error = people_add(&(data->people), person);
    
    // Release the allocated memory
    person_free(&person);
    
    return error;
    /////////////////////////////////
    // return E_NOT_IMPLEMENTED;
}

// Add a subscription if it does not exist
tApiError api_addSubscription(tApiData* data, tCSVEntry entry) {
    /////////////////////////////////
    // PR1_3d
    /////////////////////////////////
    tSubscription subscription;
    
    // Check preconditions
    assert(data != NULL);
    
    // Check the entry type
    if (strcmp(csv_getType(&entry), "SUBSCRIPTION") != 0)
        return E_INVALID_ENTRY_TYPE;
    
    // Check the number of fields
    if(csv_numFields(entry) != NUM_FIELDS_SUBSCRIPTION)
        return E_INVALID_ENTRY_FORMAT;
    
    // Parse a subscription
    subscription_parse(&subscription, entry);
    
    // Add the subscription or return an error if person does not exist or subscription already exists
    return subscriptions_add(&(data->subscriptions), data->people, subscription);
    /////////////////////////////////
    // return E_NOT_IMPLEMENTED;
}

// Add a film if it does not exist
tApiError api_addFilm(tApiData* data, tCSVEntry entry) {
    /////////////////////////////////
    // PR1_3e
    /////////////////////////////////
    tFilm film;
    tApiError error;
    
    // Check preconditions
    assert(data != NULL);
    
    // Check the entry type
    if (strcmp(csv_getType(&entry), "FILM") != 0)
        return E_INVALID_ENTRY_TYPE;
    
    // Check the number of fields
    if(csv_numFields(entry) != NUM_FIELDS_FILM)
        return E_INVALID_ENTRY_FORMAT;
    
    // Parse a film
    film_parse(&film, entry);
    
    // Add the film into the list of films and into the list of free films if the film is free
    error = film_catalog_add(&(data->films), film);
    
    // Release the allocated memory
    film_free(&film);
    
    return error;
    /////////////////////////////////
    // return E_NOT_IMPLEMENTED;
}

// Find a show by its name
tShow* api_findShow(tApiData data, const char* name) {
    /////////////////////////////////
    // PR2_3d
    /////////////////////////////////
    return showList_find(data.shows,name);
    
}


// Add a show with one season and one episode (from CSV entry)
tApiError api_addShow(tApiData* data, tCSVEntry entry) {
    /////////////////////////////////
    // PR2_3f
    /////////////////////////////////
    tShow show;
    tSeason season;
    tEpisode episode;
    tApiError error;

    // Preconditions
    assert(data != NULL);

    // Check entry type
    if (strcmp(csv_getType(&entry), "SHOW") != 0)
        return E_INVALID_ENTRY_TYPE;

    // Check field count
    if (csv_numFields(entry) != NUM_FIELDS_SHOW)
        return E_INVALID_ENTRY_FORMAT;

    // Parse a show
    show_parse(&show, entry);
    
    // Add show to catalog
    error = showList_add(&data->shows, show);

    // Free episode memory (copied inside the queue)
    show_free(&show);

   
    return error;
}

// Add a film to a subscription's watchlist from a CSV entry, avoiding duplicates
tApiError api_addToWatchlist(tApiData* data, int subscriptionId, tCSVEntry entry) {
    /////////////////////////////////
    // PR2_3c
    /////////////////////////////////
    tFilm film;
    tApiError error;
    // Basic checks
    assert(data != NULL);

    // Check entry type
    if (strcmp(csv_getType(&entry), "FILM") != 0)
        return E_INVALID_ENTRY_TYPE;

    if (csv_numFields(entry) != NUM_FIELDS_FILM)
        return E_INVALID_ENTRY_FORMAT;

    // Check if the subscription exists
    int index = subscriptions_find(data->subscriptions, subscriptionId);
    if (index < 0)
        return E_SUBSCRIPTION_NOT_FOUND;

    // Parse the film
    film_parse(&film, entry);

    // Add to the watchlist
    error = filmstack_push(&data->subscriptions.elems[index].watchlist, film);

    // Free memory (for safety)
    film_free(&film);

    return error;
}

// Get the number of people registered on the application
int api_peopleCount(tApiData data) {
    /////////////////////////////////
    // PR1_3f
    /////////////////////////////////
    return people_count(data.people);
    /////////////////////////////////
    // return -1;
}

// Get the number of subscriptions registered on the application
int api_subscriptionsCount(tApiData data) {
    /////////////////////////////////
    // PR1_3f
    /////////////////////////////////
    return subscriptions_len(data.subscriptions);
    /////////////////////////////////
    // return -1;
}

// Get the number of films registered on the application
int api_filmsCount(tApiData data) {
    /////////////////////////////////
    // PR1_3f
    /////////////////////////////////
    return film_catalog_len(data.films);
    /////////////////////////////////
    // return -1;
}

// Get the number of free films registered on the application
int api_freeFilmsCount(tApiData data) {
    /////////////////////////////////
    // PR1_3f
    /////////////////////////////////
    return film_catalog_freeLen(data.films);
    /////////////////////////////////
    // return -1;
}

// Get the number of shows registered on the application
int api_showsCount(tApiData data) {
    /////////////////////////////////
    // PR2_3g
    /////////////////////////////////
    return showsList_len(data.shows);
    /////////////////////////////////
    // return -1;
}


// Free all used memory
tApiError api_freeData(tApiData* data) {
    /////////////////////////////////
    // PR1_3g
    /////////////////////////////////
    // Check preconditions
    assert(data != NULL);
    
    people_free(&(data->people));
    subscriptions_free(&(data->subscriptions));
    film_catalog_free(&(data->films));
    /////////////////////////////////
    // PR2_3h
    /////////////////////////////////
    showList_free(&(data->shows));
    return E_SUCCESS;
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED;
}

// Add a new entry
tApiError api_addDataEntry(tApiData* data, tCSVEntry entry) {
    tApiError error;
    
    // Check preconditions
    assert(data != NULL);
    
    // Assign default value to return it if does not match any type
    error = E_INVALID_ENTRY_TYPE;
    
    if (strcmp(csv_getType(&entry), "PERSON") == 0) {
        // Add a person
        error = api_addPerson(data, entry);
    }
    else if (strcmp(csv_getType(&entry), "SUBSCRIPTION") == 0) {
        // Add a subscription
        error = api_addSubscription(data, entry);
    }        
    else if (strcmp(csv_getType(&entry), "FILM") == 0) {
        // Add a film to the films catalog
        error = api_addFilm(data, entry);
    }
    else if (strcmp(csv_getType(&entry), "SHOW") == 0) {
        // Add a film to the films catalog
        error = api_addShow(data, entry);   
    } 
    else if (strcmp(csv_getType(&entry), "WATCHLIST") == 0) {
        // PR3_4a: Add a film to a subscription's watchlist
        // Format: "FilmName;FilmDuration;FilmGenre;ReleaseDate;Rating;Free;SubscriptionID"
        
        // Check if the entry has the correct number of fields
        if (csv_numFields(entry) != NUM_FIELDS_FILM + 1) {
            return E_INVALID_ENTRY_FORMAT;
        }
        
        // Get the subscription ID from the last field
        int subscriptionId = csv_getAsInteger(entry, NUM_FIELDS_FILM);
        
        // Create a new CSV entry for the film (without the subscription ID)
        tCSVEntry filmEntry;
        csv_initEntry(&filmEntry);
        
        // Copy the type
        filmEntry.type = strdup("FILM");
        
        // Copy the fields (except the last one)
        filmEntry.numFields = NUM_FIELDS_FILM;
        filmEntry.fields = (char**)malloc(NUM_FIELDS_FILM * sizeof(char*));
        for (int i = 0; i < NUM_FIELDS_FILM; i++) {
            filmEntry.fields[i] = strdup(entry.fields[i]);
        }
        
        // Add the film to the subscription's watchlist
        error = api_addToWatchlist(data, subscriptionId, filmEntry);
        
        // Free the temporary film entry
        csv_freeEntry(&filmEntry);
    }
    
    return error;
}

// Get subscription data
tApiError api_getSubscription(tApiData data, int id, tCSVEntry *entry) {
    /////////////////////////////////
    // Ex1 PR1 4a
    /////////////////////////////////
    char buffer[FILE_READ_BUFFER_SIZE];
    tSubscription *subscription = NULL;
    int idx;
    
    // Check preconditions
    assert(entry != NULL);
    
    // Search the subscription
    idx = subscriptions_find(data.subscriptions, id);
    
    if (idx < 0)
        return E_SUBSCRIPTION_NOT_FOUND;
    
    subscription = &(data.subscriptions.elems[idx]);
    
    // Print data in the buffer
    subscription_get(*subscription, buffer);
    
    // Initialize the ouput structure
    csv_initEntry(entry);
    csv_parseEntry(entry, buffer, "SUBSCRIPTION");
    
    return E_SUCCESS;
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED;
}

// Get film data
tApiError api_getFilm(tApiData data, const char* name, tCSVEntry *entry) {
    /////////////////////////////////
    // Ex1 PR1 4b
    /////////////////////////////////
    char buffer[FILE_READ_BUFFER_SIZE];
    tFilm *film;
    
    // Check preconditions
    assert(name != NULL);
    assert(entry != NULL);
    
    // Search the film
    film = filmList_find(data.films.filmList, name);
    
    if (film == NULL)
        return E_FILM_NOT_FOUND;
    
    film_get(*film, buffer);
    
    // Initialize the output structure
    csv_initEntry(entry);
    csv_parseEntry(entry, buffer, "FILM");
    
    return E_SUCCESS;
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED;
}

// Get free films data
tApiError api_getFreeFilms(tApiData data, tCSVData *freeFilms) {
    /////////////////////////////////
    // Ex1 PR1 4c
    /////////////////////////////////
    char buffer[FILE_READ_BUFFER_SIZE];
    tFreeFilmListNode *pNode = NULL;
    
    // Check preconditions
    assert(freeFilms != NULL);
    
    // Initialize the output structure
    csv_init(freeFilms);
    
    // Iterate all free films
    pNode = data.films.freeFilmList.first;
    while (pNode != NULL) {
        film_get(*(pNode->elem), buffer);
        csv_addStrEntry(freeFilms, buffer, "FILM");
        pNode = pNode->next;
    }
    
    return E_SUCCESS;
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED;
}

// Get films data by genre
tApiError api_getFilmsByGenre(tApiData data, tCSVData *films, int genre) {
    /////////////////////////////////
    // Ex1 PR1 4d
    /////////////////////////////////
    char buffer[FILE_READ_BUFFER_SIZE];
    tFilmListNode *pNode = NULL;
    
    // Check preconditions
    assert(films != NULL);
    
    // Initialize the output structure
    csv_init(films);
    
    // Iterate all free films
    pNode = data.films.filmList.first;
    while (pNode != NULL) {
        if (pNode->elem.genre == genre) {
            film_get(pNode->elem, buffer);
            csv_addStrEntry(films, buffer, "FILM");
        }
        pNode = pNode->next;
    }
    
    return E_SUCCESS;
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED;
}

// Get longest film
tApiError api_getLongestFilm(tApiData data, tCSVEntry *entry) {
    /////////////////////////////////
    // PR3_4b
    /////////////////////////////////
    
    // Check preconditions
    assert(entry != NULL);
    
    // Initialize the output entry
    csv_initEntry(entry);
    
    // Check if catalog is empty
    if (data.films.filmList.first == NULL) {
        // For empty catalog, return E_SUCCESS with empty entry
        return E_SUCCESS;
    }
    
    // Find the longest film
    tFilm* longestFilm = filmList_longestFind(data.films.filmList);
    
    if (longestFilm == NULL) {
        return E_SUCCESS; // Return success even if no film found
    }
    
    // Convert film to CSV entry
    char buffer[FILE_READ_BUFFER_SIZE];
    film_get(*longestFilm, buffer);
    csv_parseEntry(entry, buffer, "FILM");
    
    return E_SUCCESS;
    
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED;
}

// Get longest free film
tApiError api_getLongestFreeFilm(tApiData data, tCSVEntry *entry) {
    /////////////////////////////////
    // PR3_4c
    /////////////////////////////////
    
    // Check preconditions
    assert(entry != NULL);
    
    // Initialize the output entry
    csv_initEntry(entry);
    
    // Check if free film catalog is empty
    if (data.films.freeFilmList.first == NULL) {
        // For empty catalog, return E_SUCCESS with empty entry
        return E_SUCCESS;
    }
    
    // Find the longest free film
    tFilm* longestFreeFilm = freeFilmList_longestFind(data.films.freeFilmList);
    
    if (longestFreeFilm == NULL) {
        return E_SUCCESS; // Return success even if no film found
    }
    
    // Convert film to CSV entry
    char buffer[FILE_READ_BUFFER_SIZE];
    film_get(*longestFreeFilm, buffer);
    csv_parseEntry(entry, buffer, "FILM");
    
    return E_SUCCESS;
    
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED;
}

// Sort catalog by year, oldest to newest
tApiError api_sortCatalogByYear(tApiData *data) {
    /////////////////////////////////
    // PR3_4d
    /////////////////////////////////
    
    // Check preconditions
    assert(data != NULL);
    
    // Call the film catalog sort by year function
    tApiError error = filmCatalog_SortByYear(&data->films);
    
    // The sortedByDate flag is set inside filmCatalog_SortByYear
    
    // If successful, update the sortedByDate flag
    if (error == E_SUCCESS) {
    }
    
    if (error == E_SUCCESS) {
    }
    
    return error;
    
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED;
}

// Get oldest film
tApiError api_getOldestFilm(tApiData data, tCSVEntry *entry, bool free) {
    /////////////////////////////////
    // PR3_4e
    /////////////////////////////////
    
    // Check preconditions
    assert(entry != NULL);
    
    // Initialize the output entry
    csv_initEntry(entry);
    
    // Check if catalog is empty - return E_SUCCESS with empty entry
    if (data.films.filmList.first == NULL) {
        return E_SUCCESS;
    }
    
    // Find the oldest film
    tFilm* oldestFilm = filmCatalog_OldestFind(data.films, free);
    
    if (oldestFilm == NULL) {
        return E_SUCCESS; // Return success even if no film found
    }
    
    // Convert film to CSV entry
    char buffer[FILE_READ_BUFFER_SIZE];
    film_get(*oldestFilm, buffer);
    csv_parseEntry(entry, buffer, "FILM");
    
    return E_SUCCESS;
}

// Sort catalog by rating, highest to lowest
tApiError api_sortCatalogByRating(tApiData *data) {
    /////////////////////////////////
    // PR3_4f
    /////////////////////////////////
    
    // Check preconditions
    assert(data != NULL);
    
    // Call the film catalog sort by rating function
    tApiError error = filmCatalog_SortByRating(&data->films);
    
    // Reset sortedByDate flag since we're sorting by rating now
    if (error == E_SUCCESS) {
        data->films.sortedByDate = false;
    }
    
    return error;
}

// updateVipLevel of each person
tApiError api_updateVipLevel(tApiData *data) {
    /////////////////////////////////
    // PR3_4g
    /////////////////////////////////
    
    // Check preconditions
    assert(data != NULL);
    
    // Call the update VIP level function with correct parameter order
    return update_vipLevel(&data->subscriptions, &data->people);
}

// Sort people by VIP level, highest to lowest
tApiError api_sortPeopleByVipLevel(tApiData *data) {
    /////////////////////////////////
    // PR3_4h
    /////////////////////////////////
    
    // Check preconditions
    assert(data != NULL);
    
    // Call the people sort by VIP level function
    return people_sortByVipLevel_QickSort(&data->people);
}

// Sort people by document, lowest to highest
tApiError api_sortPeopleByDocument(tApiData *data) {
    /////////////////////////////////
    // PR3_4i
    /////////////////////////////////
    
    // Check preconditions
    assert(data != NULL);
    
    // Call the people sort by document function
    return people_sortByDocument_QickSort(&data->people);
}

// Get popular film
tApiError api_getPopularFilm(tApiData data, tCSVEntry *entry) {
    /////////////////////////////////
    // PR3_4j
    /////////////////////////////////
    
    // Check preconditions
    assert(entry != NULL);
    
    // Initialize the output entry
    csv_initEntry(entry);
    
    // Check if there are subscriptions
    if (data.subscriptions.count == 0) {
        return E_SUCCESS; // Return success with empty entry
    }
    
    // Find the most popular film
    char* popularFilmName = popularFilm_find(data.subscriptions);
    
    if (popularFilmName == NULL) {
        return E_SUCCESS; // Return success even if no film found
    }
    
    // Find the film in the catalog to get full details
    tFilm* film = filmList_find(data.films.filmList, popularFilmName);
    
    if (film != NULL) {
        // Convert film to CSV entry
        char buffer[FILE_READ_BUFFER_SIZE];
        film_get(*film, buffer);
        csv_parseEntry(entry, buffer, "FILM");
    }
    
    // Free the allocated film name
    free(popularFilmName);
    
    return E_SUCCESS;
}

// Get subscription data for the specified document
tApiError api_getSubscriptionsByDocument(tApiData data, char *document, tCSVData *csvData) {
    /////////////////////////////////
    // PR3_4k
    /////////////////////////////////
    
    // Check preconditions
    assert(document != NULL);
    assert(csvData != NULL);
    
    // Initialize the output CSV data
    csv_init(csvData);
    
    // Find subscriptions by document
    tSubscriptions* foundSubscriptions = subscriptions_findByDocument(data.subscriptions, document);
    
    if (foundSubscriptions == NULL || foundSubscriptions->count == 0) {
        if (foundSubscriptions != NULL) {
            subscriptions_free(foundSubscriptions);
            free(foundSubscriptions);
        }
        return E_SUCCESS; // Return success with empty data
    }
    
    // Convert each subscription to CSV entry
    for (int i = 0; i < foundSubscriptions->count; i++) {
        char buffer[FILE_READ_BUFFER_SIZE];
        subscription_get(foundSubscriptions->elems[i], buffer);
        csv_addStrEntry(csvData, buffer, "SUBSCRIPTION");
    }
    
    // Free the allocated subscriptions structure
    subscriptions_free(foundSubscriptions);
    free(foundSubscriptions);
    
    return E_SUCCESS;
}