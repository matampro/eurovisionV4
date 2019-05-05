//
// Created by DONTO on 5/5/2019.
//
#include "state.h"


StateDataMap copyStateData(StateDataMap dataToCopy) {

    if (dataToCopy == NULL) {
        return NULL;
    }
    StateData data = (StateData) dataToCopy;
    StateData stateDataNew = malloc(sizeof(*stateDataNew));
    if (stateDataNew == NULL) {
        return NULL;
    } else {

        stateDataNew->stateName = stringCopy(data->stateName);
        if (stateDataNew->stateName == NULL) {
            free(stateDataNew);
            return NULL;
        }

        stateDataNew->songName = stringCopy(data->songName);
        if (stateDataNew->songName == NULL) {
            free(stateDataNew->stateName);
            free(stateDataNew);
            return NULL;
        }

        Map citizenVoteDest = mapCopy(data->citizenVote);
        if (citizenVoteDest == NULL) {
            free(stateDataNew->stateName);
            free(stateDataNew->songName);
            free(stateDataNew);
            return NULL;
        }
        stateDataNew->citizenVote = citizenVoteDest;
        return stateDataNew;
    }
}



void freeStateData(StateDataMap dataToFree) {


    StateData toFree = (StateData) dataToFree;


    if (toFree != NULL) {
        free(toFree->stateName);

        free(toFree->songName);

        mapDestroy(toFree->citizenVote);
        free(toFree);
    }

}
