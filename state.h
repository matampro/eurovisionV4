//
// Created by DONTO on 5/5/2019.
//

#ifndef EUROVISIONV4_STATE_H
#define EUROVISIONV4_STATE_H

#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

/** Element state data map type  */
typedef void *StateDataMap;

/** Type for defining the state data struct */
typedef struct {
    char *stateName;
    char *songName;
    Map citizenVote;
}*StateData;

/**
* Copies state data element
*
 * returns
* NULL if there is an allocation problem
* pointer to the data element otherwise
**/
StateDataMap copyStateData(StateDataMap dataToCopy);

/**
* function to use from eurovision
**/
char *stringCopy(const char *str);

/** Function to be used by the map for freeing elements */
void freeStateData(StateDataMap dataToFree);

#endif //EUROVISIONV4_STATE_H
