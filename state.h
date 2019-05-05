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

typedef void *StateDataMap;

typedef struct {
    char *stateName;
    char *songName;
    Map citizenVote;
}*StateData;

StateDataMap copyStateData(StateDataMap dataToCopy);
char *stringCopy(const char *str);

/** Function to be used by the map for freeing elements */
void freeStateData(StateDataMap dataToFree);

#endif //EUROVISIONV4_STATE_H
