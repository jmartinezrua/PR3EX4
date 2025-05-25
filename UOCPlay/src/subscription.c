#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "subscription.h"

// Parse input from CSVEntry
void subscription_parse(tSubscription* data, tCSVEntry entry) {
    // Check input data
    assert(data != NULL);

    // Check entry fields
    assert(csv_numFields(entry) == NUM_FIELDS_SUBSCRIPTION);

    int pos = 0; // Allow to easy incremental position of the income data

    // Copy subscription's id data
    data->id = csv_getAsInteger(entry, pos);

    // Copy identity document data
    assert(strlen(entry.fields[++pos]) == MAX_DOCUMENT);
    csv_getAsString(entry, pos, data->document, MAX_DOCUMENT + 1);

    // Parse start date
    assert(strlen(entry.fields[++pos]) == DATE_LENGTH);
    date_parse(&(data->start_date), entry.fields[pos]);

    // Parse end date
    assert(strlen(entry.fields[++pos]) == DATE_LENGTH);
    date_parse(&(data->end_date), entry.fields[pos]);

    // Copy plan data
    csv_getAsString(entry, ++pos, data->plan, MAX_PLAN + 1);

    // Copy price data
    data->price = csv_getAsReal(entry, ++pos);

    // Copy number of devices data
    data->numDevices = csv_getAsInteger(entry, ++pos);
    
    // Init watchlist
    filmstack_init(&data->watchlist);

    // Check preconditions that needs the readed values
    assert(data->price >= 0);
    assert(data->numDevices >= 1);
}

// Copy the data from the source to destination (individual data)
void subscription_cpy(tSubscription* destination, tSubscription source) {
    // Copy subscription's id data
    destination->id = source.id;

    // Copy identity document data
    strncpy(destination->document, source.document, MAX_DOCUMENT + 1);

    // Copy start date
    date_cpy(&(destination->start_date), source.start_date);

    // Copy end date
    date_cpy(&(destination->end_date), source.end_date);

    // Copy plan data
    strncpy(destination->plan, source.plan, MAX_PLAN + 1);

    // Copy price data
    destination->price = source.price;

    // Copy number of devices data
    destination->numDevices = source.numDevices;
    
    filmstack_init(&destination->watchlist);
    
    if (source.watchlist.count>0) {
        tFilmstackNode *pFimStackNodes[source.watchlist.count];
        tFilmstackNode *fimStackNode;
        fimStackNode = source.watchlist.top;
        int j = 0;
        while (fimStackNode != NULL) {
            pFimStackNodes[j] = fimStackNode;
            fimStackNode = fimStackNode->next;
            j++;
        }
        
        //adding the films to the watchlist in reverse order because filmstack_push is used
        for(j=source.watchlist.count -1;j>=0;j--) {
            filmstack_push(&destination->watchlist,pFimStackNodes[j]->elem);
        }
    }
}

// Get subscription data using a string
void subscription_get(tSubscription data, char* buffer) {
    // Print all data at same time
    sprintf(buffer,"%d;%s;%02d/%02d/%04d;%02d/%02d/%04d;%s;%g;%d",
        data.id,
        data.document,
        data.start_date.day, data.start_date.month, data.start_date.year,
        data.end_date.day, data.end_date.month, data.end_date.year,
        data.plan,
        data.price,
        data.numDevices);
}

// Initialize subscriptions data
tApiError subscriptions_init(tSubscriptions* data) {
    // Check input data
    assert(data != NULL);
    data->elems = NULL;
    data->count = 0; 
    
    return E_SUCCESS;
}

// Return the number of subscriptions
int subscriptions_len(tSubscriptions data) {
    return data.count;
}

// Add a new subscription
tApiError subscriptions_add(tSubscriptions* data, tPeople people, tSubscription subscription) {
    // Check input data
    assert(data != NULL);

    // If subscription already exists, return an error
    for (int i=0; i< data->count; i++) {
        if (subscription_equal(data->elems[i], subscription))
            return E_SUBSCRIPTION_DUPLICATED;
    }

    // If the person does not exist, return an error
    if (people_find(people, subscription.document) < 0)
        return E_PERSON_NOT_FOUND;

    // Copy the data to the new position
    if (data->elems == NULL) {
        data->elems = (tSubscription*) malloc(sizeof(tSubscription));
    } else {
        data->elems = (tSubscription*) realloc(data->elems, (data->count + 1) * sizeof(tSubscription));
    }
    assert(data->elems != NULL);
    subscription_cpy(&(data->elems[data->count]), subscription);

    // Increase the number of elements
    data->count++;
    
    // Update the IDs of all subscriptions to be sequential
    for (int i = 0; i < data->count; i++) {
        data->elems[i].id = i + 1;
    }
    
    return E_SUCCESS;
}

// Remove a subscription
tApiError subscriptions_del(tSubscriptions* data, int id) {
    // Check preconditions
    assert(data != NULL);
    
    // Find the subscription with the specified ID
    int idx = subscriptions_find(*data, id);
    
    // If the subscription does not exist, return an error
    if (idx < 0)
        return E_SUBSCRIPTION_NOT_FOUND;
    
    // Free the watchlist of the subscription to be deleted
    filmstack_free(&data->elems[idx].watchlist);
    
    // Shift elements to remove selected
    for (int i = idx; i < data->count - 1; i++) {
        subscription_cpy(&data->elems[i], data->elems[i + 1]);
    }
    
    // Update the number of elements
    data->count--;
    
    // Update the IDs of the remaining subscriptions
    for (int i = 0; i < data->count; i++)
        data->elems[i].id = i + 1;
    
    // Reallocate memory or free if empty
    if (data->count > 0) {
        data->elems = realloc(data->elems, data->count * sizeof(tSubscription));
        assert(data->elems != NULL);
    } else {
        subscriptions_free(data);
    }
    
    return E_SUCCESS;
}

// Get subscription data of position index using a string
void subscriptions_get(tSubscriptions data, int index, char* buffer)
{
    assert(index >= 0 && index < data.count);
    subscription_get(data.elems[index], buffer);
}

// Returns the position of a subscription looking for id's subscription. -1 if it does not exist
int subscriptions_find(tSubscriptions data, int id) {
    int i = 0;
    while (i < data.count) {
        if (data.elems[i].id == id) {
            return i;
        }
        i++;
    }

    return -1;
}

// Print subscriptions data
void subscriptions_print(tSubscriptions data) {
    char buffer[1024];
    int i;
    for (i = 0; i < data.count; i++) {
        subscriptions_get(data, i, buffer);
        printf("%s\n", buffer);
    }
}

// Remove all elements 
tApiError subscriptions_free(tSubscriptions* data) {    
    if (data->elems != NULL) {
    /////////////////////////////////
    // PR2_2b
    /////////////////////////////////
        for (int i = 0; i < data->count; i++) {
            filmstack_free(&data->elems[i].watchlist);
        }
    /////////////////////////////////
        free(data->elems);
    }
    subscriptions_init(data);
    
    return E_SUCCESS;
  
}

// Calculate VIP level based on subscriptions
int calculate_vipLevel(tSubscriptions* subscriptions, const char* document) {
    // Check preconditions FIRST
    if (subscriptions == NULL || document == NULL) {
        return 0;
    }
    
    // Now it's safe to check the fields
    if (subscriptions->count == 0 || subscriptions->elems == NULL) {
        return 0;
    }
    
    float totalPrice = 0.0f;
    
    // Iterate through all subscriptions to find matching documents
    for (int i = 0; i < subscriptions->count; i++) {
        // Check if document matches (document is a char array, not pointer)
        if (strcmp(subscriptions->elems[i].document, document) == 0) {
            totalPrice += subscriptions->elems[i].price;
        }
    }
    
    // Calculate VIP level: 1 level per 500 euros
    int vipLevel = (int)(totalPrice / 500.0f);
    
    return vipLevel;
}

// Update the vipLevel of each person 
tApiError update_vipLevel(tSubscriptions* subscriptions, tPeople* people) {
    // Check preconditions
    if (people == NULL) {
        return E_SUCCESS;
    }
    
    // If there are no people, nothing to update
    if (people->count == 0 || people->elems == NULL) {
        return E_SUCCESS;
    }
    
    // Update VIP level for each person
    for (int i = 0; i < people->count; i++) {
        people->elems[i].vipLevel = calculate_vipLevel(subscriptions, people->elems[i].document);
    }
    
    return E_SUCCESS;
}

// Return a pointer to the longest film of the list
char* popularFilm_find(tSubscriptions subscriptions) {
    // Check if there are subscriptions
    if (subscriptions.count == 0 || subscriptions.elems == NULL) {
        return NULL;
    }
    
    // Count occurrences of each film
    typedef struct {
        char* filmName;
        int count;
        tDate releaseDate;
    } tFilmCount;
    
    tFilmCount filmCounts[1000]; // Assuming max 1000 different films
    int uniqueFilms = 0;
    
    // Iterate through all subscriptions and their watchlists
    for (int i = 0; i < subscriptions.count; i++) {
        tFilmstackNode* current = subscriptions.elems[i].watchlist.top;
        
        while (current != NULL) {
            // Check if film already exists in our count array
            int found = -1;
            for (int j = 0; j < uniqueFilms; j++) {
                if (strcmp(filmCounts[j].filmName, current->elem.name) == 0) {
                    found = j;
                    break;
                }
            }
            
            if (found >= 0) {
                // Film already exists, increment count
                filmCounts[found].count++;
            } else {
                // New film, add to array
                if (uniqueFilms < 1000) {
                    filmCounts[uniqueFilms].filmName = current->elem.name;
                    filmCounts[uniqueFilms].count = 1;
                    filmCounts[uniqueFilms].releaseDate = current->elem.release;
                    uniqueFilms++;
                }
            }
            
            current = current->next;
        }
    }
    
    // Find the most popular film
    if (uniqueFilms == 0) {
        return NULL;
    }
    
    int maxCount = 0;
    int mostPopularIndex = -1;
    
    for (int i = 0; i < uniqueFilms; i++) {
        if (filmCounts[i].count > maxCount ||
            (filmCounts[i].count == maxCount && mostPopularIndex >= 0 &&
             date_cmp(filmCounts[i].releaseDate, filmCounts[mostPopularIndex].releaseDate) > 0)) {
            maxCount = filmCounts[i].count;
            mostPopularIndex = i;
        }
    }
    
    if (mostPopularIndex >= 0) {
        // Return a copy of the film name
        char* result = (char*)malloc((strlen(filmCounts[mostPopularIndex].filmName) + 1) * sizeof(char));
        strcpy(result, filmCounts[mostPopularIndex].filmName);
        return result;
    }
    
    return NULL;
}

// Return a pointer to the subscriptions of the client with the specified document
tSubscriptions* subscriptions_findByDocument(tSubscriptions subscriptions, const char* document) {
    // Check preconditions
    if (document == NULL) {
        return NULL;
    }
    
    // Allocate memory for result
    tSubscriptions* result = (tSubscriptions*)malloc(sizeof(tSubscriptions));
    if (result == NULL) {
        return NULL;
    }
    
    // Initialize result
    subscriptions_init(result);
    
    // If no subscriptions, return empty result
    if (subscriptions.count == 0 || subscriptions.elems == NULL) {
        return result;
    }
    
    // Count matching subscriptions first
    int matchCount = 0;
    for (int i = 0; i < subscriptions.count; i++) {
        if (strcmp(subscriptions.elems[i].document, document) == 0) {
            matchCount++;
        }
    }
    
    // If no matches, return empty result
    if (matchCount == 0) {
        return result;
    }
    
    // Allocate memory for matching subscriptions
    result->elems = (tSubscription*)malloc(matchCount * sizeof(tSubscription));
    if (result->elems == NULL) {
        free(result);
        return NULL;
    }
    
    // Copy matching subscriptions
    int resultIndex = 0;
    for (int i = 0; i < subscriptions.count; i++) {
        if (strcmp(subscriptions.elems[i].document, document) == 0) {
            // Copy subscription data
            result->elems[resultIndex] = subscriptions.elems[i];

            // Set correct ID (1-based index in the result array)
            result->elems[resultIndex].id = resultIndex + 1;

            // Copy document string (use strncpy for fixed-size array)
            strncpy(result->elems[resultIndex].document, document, MAX_DOCUMENT);
            result->elems[resultIndex].document[MAX_DOCUMENT] = '\0';

            // Copy watchlist (deep copy)
            filmstack_init(&result->elems[resultIndex].watchlist);
            tFilmstackNode* current = subscriptions.elems[i].watchlist.top;
            while (current != NULL) {
                filmstack_push(&result->elems[resultIndex].watchlist, current->elem);
                current = current->next;
            }

            resultIndex++;
        }
    }
    
    result->count = matchCount;
    return result;
}

// return a pointer to the subscription with the specified id
tSubscription* subscriptions_findHash(tSubscriptions data, int id) {
    // Check if the data is empty
    if (data.count == 0) {
        return NULL;
    }
    
    // Find the subscription with the specified id
    for (int i = 0; i < data.count; i++) {
        if (data.elems[i].id == id) {
            return &data.elems[i];
        }
    }
    
    // If no subscription matches, return NULL
    return NULL;
}

// Compare two subscription
bool subscription_equal(tSubscription subscription1, tSubscription subscription2) {
    
    if (strcmp(subscription1.document,subscription2.document) != 0)
        return false;
    
    if (date_cmp(subscription1.start_date,subscription2.start_date) != 0)
        return false;
        
    if (date_cmp(subscription1.end_date,subscription2.end_date) != 0)
        return false;
    
    if (strcmp(subscription1.plan,subscription2.plan) != 0)
        return false;
        
    if (subscription1.price != subscription2.price)
        return false;
    
    if (subscription1.numDevices != subscription2.numDevices)
        return false;
    
    return true;
}