#include "eurovision.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>


#define NUMBER_OF_RESULTS 10
#define MAX_NUMBER_OF_WINNERS 10
#define TOTAL_PRECENT 100
#define MAX_VOTE_SIZE 10
#define MAX_VOTE_POSIBLE 12

typedef struct resutlstring_t {
    float score;
    int index;
    char *name;
} Resulttmp;


typedef struct node_t {
    MapDataElement mapDataElement;
    MapKeyElement mapKeyElement;
    struct node_t *next;
} *Node;

typedef struct {
    int counter;
    Node head;
    Node tail;
    copyMapDataElements data_copy;
    copyMapKeyElements key_copy;
    compareMapKeyElements compair_key;
    freeMapDataElements free_data;
    freeMapKeyElements free_key;
} *Map_cheat;


char *stringCopy(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    size_t strLength = strlen(str);
    char *strDest = (char *) malloc(sizeof(char) * strLength + 1);
    if (strDest == NULL) {
        return NULL;
    } else {
        strcpy(strDest, str);
    }
    return strDest;
}

ListElement copyStringData(ListElement strvoid) {

    if (strvoid == NULL) {
        return NULL;
    }
    char *str = (char *) strvoid;
    size_t strLength = strlen(str);
    char *strDest = malloc(sizeof(char) * strLength + 1);
    if (strDest == NULL) {
        return NULL;
    } else {
        strcpy(strDest, str);
    }
    return strDest;
}


static StateDataMap copyStateData(StateDataMap dataToCopy) {

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

/** Function to be used by the map for freeing elements */
static void freeStateData(StateDataMap dataToFree) {


    StateData toFree = (StateData) dataToFree;


    if (toFree != NULL) {
        free(toFree->stateName);

        free(toFree->songName);

        mapDestroy(toFree->citizenVote);
        free(toFree);
    }

}


static VoteDataElement copyVoteDataElement(VoteDataElement voteToCopy) {

    if (voteToCopy == NULL) {
        return NULL;
    }

    int *ptr = malloc(sizeof(int));
    if (ptr == NULL) {
        return NULL;
    }
    *ptr = *(int *) voteToCopy;

    return ptr;
}

static KeyElement copyKeyElement(KeyElement keyToCopy) {

    if (keyToCopy == NULL) {
        return NULL;
    }
    int *ptr = malloc(sizeof(int));
    if (ptr == NULL) {
        return NULL;
    }
    *ptr = *(int *) keyToCopy;
    return ptr;
}

void freeVoteDataElement(VoteDataElement voteToFree) {
    free(voteToFree);
}

void freeKeyElement(KeyElement keyToFree) {
    free(keyToFree);
}


int compareKeyElements(KeyElement key1, KeyElement key2) {
    int a = *(int *) key1;
    int b = *(int *) key2;
    if (a > b) {
        return 1;
    } else if (a == b) {
        return 0;

    } else { //the second is bigger
        return -1;
    }
}


static JudgeDataMap copyJudgeDataElement(JudgeDataMap judgeDataToCopy) {
    if (judgeDataToCopy == NULL) {
        return NULL;
    }

    JudgeData data = (JudgeData) judgeDataToCopy;

    if (data->judgeName == NULL || data->judgeResults == NULL) {

        return NULL;
    }
    JudgeData judgeDataNew = malloc(sizeof(*judgeDataNew));

    if (judgeDataNew == NULL) {
        return NULL;
    } else {

        judgeDataNew->judgeName = stringCopy(data->judgeName);
        if (judgeDataNew->judgeName == NULL) {
            freeJudgeDataElement(judgeDataNew);
            return NULL;
        }

        judgeDataNew->judgeResults = malloc((sizeof(int)) * NUMBER_OF_RESULTS);
        if (judgeDataNew->judgeResults == NULL) {
            freeJudgeDataElement(judgeDataNew);
         //   free(judgeDataNew->judgeName);
            return NULL;
        }

        for (int i = 0; i < NUMBER_OF_RESULTS; i++) {

            judgeDataNew->judgeResults[i] = data->judgeResults[i];
        }

        return judgeDataNew;
    }
}

static void freeJudgeDataElement(JudgeDataMap judgeDataToFree) {
    JudgeData ptr = (JudgeData) judgeDataToFree;
    free(ptr->judgeName);
    free(ptr->judgeResults);
    free(ptr);
}

Eurovision eurovisionCreate() {
    Eurovision eurovision = malloc(sizeof(*eurovision));

    if (eurovision == NULL) {
        return NULL;
    }
    eurovision->state = mapCreate(copyStateData, copyKeyElement, freeStateData, freeKeyElement, compareKeyElements);
    if (eurovision->state == NULL) {
        free(eurovision);
        return NULL;
    }
    eurovision->judge = mapCreate(copyJudgeDataElement, copyKeyElement, freeJudgeDataElement, freeKeyElement,
                                  compareKeyElements);
    if (eurovision->judge == NULL) {
        free(eurovision);
        free(eurovision->state);
        return NULL;
    }
    return eurovision;
}

void eurovisionDestroy(Eurovision eurovision) {

    mapDestroy(eurovision->state);

    mapDestroy(eurovision->judge);
    free(eurovision);
}


bool checkIfNotNegitive(int num) {
    if (num < 0) {
        return false;
    } else {
        return true;
    }
}

bool checkIfNameIsLegal(const char *name) {
    char *copyNameOriginal = stringCopy(name); /// free from middle of string need to read about behavier but not stable
    char *copyName = copyNameOriginal;
    if (copyName == NULL) {
        free(copyNameOriginal);
        return false;
    }

    while (*copyName != '\0') {
        if (((*copyName < 'a') || (*copyName > 'z')) && *copyName != ' ') {
            free(copyNameOriginal);
            return false;
        }

        copyName++; //bug no addition to pointer
    }
    free(copyNameOriginal);
    return true;
}

EurovisionResult eurovisionAddState(Eurovision eurovision, int stateId, const char *stateName, const char *songName) {


    if (eurovision == NULL) {
        return EUROVISION_NULL_ARGUMENT;
    } else if (!checkIfNotNegitive(stateId)) {
        return EUROVISION_INVALID_ID;
    } else if (mapContains(eurovision->state, &stateId)) {

        return EUROVISION_STATE_ALREADY_EXIST;
    } else if (!checkIfNameIsLegal(stateName) || !checkIfNameIsLegal(songName)) {
        return EUROVISION_INVALID_NAME;
    } else {

        StateData newStateData = (StateData) malloc(sizeof(*newStateData));
        if (newStateData == NULL) {
            eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        }

        newStateData->stateName = stringCopy(stateName);
        if (newStateData->stateName == NULL) {
            freeStateData(newStateData);
            //free(newStateData);
            eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        }

        newStateData->songName = stringCopy(songName);
        if (newStateData->songName == NULL) {
            freeStateData(newStateData);
            //free(newStateData);
            eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        }

        Map citizenVote = mapCreate(copyVoteDataElement, copyKeyElement, freeVoteDataElement, freeKeyElement,
                                    compareKeyElements);/// if you create a new cunk of data you must attach it!

        if (citizenVote == NULL) {
            freeStateData(newStateData);
//            free(newStateData);
            eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        } else {
            newStateData->citizenVote = (citizenVote);


            MapResult mapResult = mapPut(eurovision->state, &stateId,
                                         newStateData);  //we have to add a new map to the *state
            if (mapResult == MAP_OUT_OF_MEMORY) {
                freeStateData(newStateData);
//                free(newStateData);
                eurovisionDestroy(eurovision);
                return EUROVISION_OUT_OF_MEMORY;
            }

            freeStateData(newStateData);
//            free(newStateData);
            return EUROVISION_SUCCESS;

        }
    }
}

EurovisionResult eurovisionRemoveState(Eurovision eurovision, int stateId) {

    if (!checkIfNotNegitive(stateId)) {
        return EUROVISION_INVALID_ID;
    } else {
        MapResult result = mapRemove(eurovision->state, &stateId);
        if (result == MAP_ITEM_DOES_NOT_EXIST) {

            return EUROVISION_STATE_NOT_EXIST;
        }

        MAP_FOREACH(int *, iter, eurovision->state) {
            StateData stateData = mapGet(eurovision->state, iter);
            if (stateData == NULL) {

                return EUROVISION_NULL_ARGUMENT;
            }

            Map citizenVote = stateData->citizenVote;
            if (citizenVote == NULL) {
                printf("Error null votes for iter %d\n", *iter);
                return EUROVISION_NULL_ARGUMENT;
            }

            MapResult result1 = mapRemove(citizenVote, &stateId);
            if (result1 == MAP_NULL_ARGUMENT) {

                return EUROVISION_NULL_ARGUMENT;
            }
        }

        if (MAP_SUCCESS == result)
            return EUROVISION_SUCCESS;

    }
}

EurovisionResult eurovisionAddJudge(Eurovision eurovision, int judgeId, const char *judgeName, int *judgeResults) {
    if ((eurovision == NULL) || (judgeName == NULL) || (judgeResults == NULL)) {
        return EUROVISION_NULL_ARGUMENT;
    }

    if (!checkIfNotNegitive(judgeId)) {
        return EUROVISION_INVALID_ID;
    } else if (mapContains(eurovision->judge, &judgeId)) {
        return EUROVISION_JUDGE_ALREADY_EXIST;
    } else if (!checkIfNameIsLegal(judgeName)) {
        return EUROVISION_INVALID_NAME;
    }

    JudgeData newJudgeData = (JudgeData) malloc(sizeof(*newJudgeData));
    if (newJudgeData == NULL) {
        eurovisionDestroy(eurovision);
        return EUROVISION_OUT_OF_MEMORY;
    }

    newJudgeData->judgeName = stringCopy(judgeName);
    if (newJudgeData->judgeName == NULL) {
        free(newJudgeData);
        eurovisionDestroy(eurovision);
        return EUROVISION_OUT_OF_MEMORY;
    }

    newJudgeData->judgeResults = malloc(sizeof(int) * NUMBER_OF_RESULTS);
    if (newJudgeData->judgeResults == NULL) {
        free(newJudgeData->judgeName);
        free(newJudgeData);
        return EUROVISION_OUT_OF_MEMORY;
    }

    for (int i = 0; i < NUMBER_OF_RESULTS; i++) {
        if (checkIfNotNegitive(judgeResults[i])) {
            if (mapContains(eurovision->state, &(judgeResults[i]))) {

                newJudgeData->judgeResults[i] = judgeResults[i];
            } else {
                freeJudgeDataElement(newJudgeData);
                //free(newJudgeData);

                return EUROVISION_STATE_NOT_EXIST;
            }
        } else {
            freeJudgeDataElement(newJudgeData);
            //free(newJudgeData);

            return EUROVISION_INVALID_ID;

        }

    }


    MapResult result = mapPut(eurovision->judge, &judgeId, newJudgeData); // pointer to pointer

    if (result == MAP_OUT_OF_MEMORY) {
        freeJudgeDataElement(newJudgeData);
        //free(newJudgeData);
        eurovisionDestroy(eurovision);
        return EUROVISION_OUT_OF_MEMORY;
    } else {
        freeJudgeDataElement(newJudgeData);
       // free(newJudgeData);


        return EUROVISION_SUCCESS;
    }

}


EurovisionResult eurovisionRemoveJudge(Eurovision eurovision, int judgeId) {
    if (!checkIfNotNegitive(judgeId)) {

        return EUROVISION_INVALID_ID;
    } else if (!mapContains(eurovision->judge, &judgeId)) {

        return EUROVISION_JUDGE_NOT_EXIST;
    }

    MapResult result = mapRemove(eurovision->judge, &judgeId);
    if (result == MAP_NULL_ARGUMENT) {
        return EUROVISION_NULL_ARGUMENT;
    }

    if (result == MAP_SUCCESS) {

        return EUROVISION_SUCCESS;
    } else {

        return EUROVISION_JUDGE_NOT_EXIST;
    }

}

EurovisionResult eurovisionAddVote(Eurovision eurovision, int stateGiver, int stateTaker) {

    if ((!checkIfNotNegitive(stateGiver)) || (!checkIfNotNegitive(stateTaker))) {
        return EUROVISION_INVALID_ID;
    } else if (stateGiver == stateTaker) {
        return EUROVISION_SAME_STATE;
    } else {
        bool result1 = mapContains(eurovision->state, &stateGiver);
        bool result2 = mapContains(eurovision->state, &stateTaker);
        if ((!result1) || (!result2)) {
            return EUROVISION_STATE_NOT_EXIST;
        }
    }


    StateData stateData = (StateData) mapGet(eurovision->state, &stateGiver);

    void *votes = (void *) mapGet(stateData->citizenVote, &stateTaker);


    int numVotes;

    if (votes == NULL) {
        numVotes = 1;


        MapResult result = mapPut(stateData->citizenVote, &stateTaker, &numVotes);

        if (result == MAP_OUT_OF_MEMORY) {

            eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        }
        if (result == MAP_SUCCESS) {///// need to correct all possible return pattern results
            return EUROVISION_SUCCESS;
        }

    } else {

        numVotes = *(int *) votes;
        numVotes++;

        MapResult result = mapPut(stateData->citizenVote, &stateTaker, &numVotes);
        if (result == MAP_OUT_OF_MEMORY) {

            eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        }

        return EUROVISION_SUCCESS;
    }
}

EurovisionResult eurovisionRemoveVote(Eurovision eurovision, int stateGiver, int stateTaker) {
    if ((!checkIfNotNegitive(stateGiver)) || (!checkIfNotNegitive(stateTaker))) {
        return EUROVISION_INVALID_ID;
    } else if (stateGiver == stateTaker) {
        return EUROVISION_SAME_STATE;
    } else {
        bool result1 = mapContains(eurovision->state, &stateGiver);
        bool result2 = mapContains(eurovision->state, &stateTaker);
        if ((!result1) || (!result2)) {
            return EUROVISION_STATE_NOT_EXIST;
        }
    }
    StateData stateData = (StateData) mapGet(eurovision->state, &stateGiver);
    void *votes = (void *) mapGet(stateData->citizenVote, &stateTaker);
    int numVotes;
    if (votes == NULL) {
        return EUROVISION_SUCCESS;
    } else {
        numVotes = *(int *) votes;
        MapResult result = mapPut(stateData->citizenVote, &stateTaker, &numVotes - 1);
        if (result == MAP_OUT_OF_MEMORY) {
            eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        }
        return EUROVISION_SUCCESS;
    }
}

void findMax(const int *row, size_t numberOfStates, int *index, int *val) {
    int max = -1;
    *index = -1;
    *val = -1;
    for (int i = 0; i < numberOfStates; i++) {

        if (row[i] > max) {
            max = row[i];
            *val = max;
            *index = i;
        }
    }

}

void findMaxfloat(const float *row, size_t numberOfStates, int *index, float *val) {
    float max = -1;
    *index = -1;
    *val = -1;
    for (int i = 0; i < numberOfStates; i++) {

        if (row[i] > max) {
            max = row[i];
            *val = max;
            *index = i;
        }
    }

}

void convertRowToPoints(int *row, size_t numberOfStates) {

    int voteTranslation[10] = {12, 10, 8, 7, 6, 5, 4, 3, 2, 1};

    int *tempRow = (int *) malloc(sizeof(int) * numberOfStates);
    for (int i = 0; i < numberOfStates; i++) {
        tempRow[i] = 0;
    }
    int index = -1;
    int val = -1;

    for (int currentVoteindex = 0; currentVoteindex < MAX_VOTE_SIZE; currentVoteindex++) {

        findMax(row, numberOfStates, &index, &val);
        if (val > 0) {
            tempRow[index] = voteTranslation[currentVoteindex];
        } else {
            tempRow[index] = 0;
        }


        row[index] = -1;

    }

    for (int i = 0; i < numberOfStates; i++) {
        row[i] = tempRow[i];
    }


    free(tempRow);
}

void sumStateVotes(size_t row, size_t col,const int *votesTables, float *stateTotalVote) {
    int totalVoteCount = 0;

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            totalVoteCount += votesTables[i * col + j];
        }

    }


    memset(stateTotalVote, 0, sizeof(float) * col);


    for (int j = 0; j < col; j++) {
        for (int i = 0; i < row; i++) {

            stateTotalVote[j] += votesTables[i * col + j];
        }
    }

    //divide sum votes
    for (int j = 0; j < col; j++) {
        stateTotalVote[j] /= row;//totalVoteCount;

    }


}


ListElement copyString(ListElement str) {
    return copyStringData(str);
}

void freeString(ListElement strvoid) {
    char *str = strvoid;
    free(str);
}


List eurovisionRunContest(Eurovision eurovision, int audiencePercent) {
    if ((eurovision == NULL) || (audiencePercent < 1) || (audiencePercent > 100)) {
        return NULL;
    }
    /// create vote table of all state
    int numberOfStates = mapGetSize(eurovision->state);
    int *votesTables = (int *) malloc(sizeof(int) * numberOfStates * numberOfStates);
    /// @todo add  check

    memset(votesTables, 0, sizeof(int) * numberOfStates * numberOfStates);


    /// later return to map for each
    for (int *iterator = mapGetFirst(eurovision->state); iterator; iterator = mapGetNext(eurovision->state)) {
        StateData state = (StateData) mapGet(eurovision->state, iterator);

        for (int *iterVotes = mapGetFirst(state->citizenVote); iterVotes; iterVotes = mapGetNext(state->citizenVote)) {
            int votesCast = *(int *) mapGet(state->citizenVote, iterVotes);


            votesTables[*iterator * numberOfStates + *iterVotes] = votesCast;
        }
    }
    for (int i = 0; i < numberOfStates; i++) {
        convertRowToPoints(&votesTables[i * numberOfStates], numberOfStates);
    }

    /// float array of country and total votes
    float *stateTotalVote = (float *) malloc(sizeof(float) * numberOfStates);
    /// @todo add  check


    sumStateVotes(numberOfStates, numberOfStates, votesTables, stateTotalVote);

    int numberOfJudge = mapGetSize(eurovision->judge);
    size_t sizeTableJudge = sizeof(int) * numberOfJudge * numberOfStates;
    int *judgeTables = (int *) malloc(sizeTableJudge);
    /// @todo add  check

    memset(judgeTables, 0, sizeTableJudge);


    /// later return to map for each
    for (int *iterator = mapGetFirst(eurovision->judge); iterator; iterator = mapGetNext(eurovision->judge)) {
        JudgeData judge = (JudgeData) mapGet(eurovision->judge, iterator);
        int voteTranslation[10] = {12, 10, 8, 7, 6, 5, 4, 3, 2, 1};
        int currentVote = 0;
        for (int id = 0; id < MAX_VOTE_SIZE; id++) {

            int stateId = judge->judgeResults[id];
            judgeTables[*iterator * numberOfStates + stateId] = voteTranslation[currentVote];
            currentVote++;
        }
    }

    float *stateTotalVotejudge = (float *) malloc(sizeof(float) * numberOfStates);
    sumStateVotes(numberOfJudge, numberOfStates, judgeTables, stateTotalVotejudge);

    /// join two table data

    ///  stateTotalVotejudge

    ///  stateTotalVote
    float *finalTally = (float *) malloc(sizeof(float) * numberOfStates);
    memset(finalTally, 0, sizeof(float) * numberOfStates);

    float audPre = audiencePercent;
    float compAudpre = 100 - audPre;
    for (int i = 0; i < numberOfStates; i++) {
        finalTally[i] =
                (float) ((int) ((stateTotalVote[i] * audPre + stateTotalVotejudge[i] * compAudpre) * 100.f)) / 100.f;
        finalTally[i] = (stateTotalVote[i] * audPre + stateTotalVotejudge[i] * compAudpre);
    }
    float sum = 0;
    for (int i = 0; i < numberOfStates; i++) {
        StateData st = (StateData) mapGet(eurovision->state, &i);
        sum += finalTally[i];
    }


    // create  list

    Resulttmp *sortTempArray = (Resulttmp *) malloc(sizeof(Resulttmp) * numberOfStates);
    float maxelem;
    int index;


    for (int i = 0; i < numberOfStates; i++) {
        findMaxfloat(finalTally, numberOfStates, &index, &maxelem);


        if (i == 0) {

            sortTempArray[i].index = index;

            sortTempArray[i].score = maxelem;

            StateData state = (StateData) mapGet(eurovision->state, &index);
            sortTempArray[i].name = stringCopy(state->stateName);
        } else {

            if (sortTempArray[i - 1].score == maxelem)// same scroe
            {
                if (sortTempArray[i - 1].index > index) {
                    // swap
                    Resulttmp tmp;
                    tmp.index = sortTempArray[i - 1].index;
                    tmp.name = sortTempArray[i - 1].name;
                    tmp.score = sortTempArray[i - 1].score;

                    sortTempArray[i - 1].index = index;
                    sortTempArray[i - 1].score = maxelem;

                    StateData state = (StateData) mapGet(eurovision->state, &index);
                    sortTempArray[i - 1].name = stringCopy(state->stateName);

                    sortTempArray[i - 1].index = tmp.index;
                    sortTempArray[i - 1].score = tmp.score;
                    sortTempArray[i - 1].name = tmp.name;


                } else {
                    sortTempArray[i].index = index;
                    sortTempArray[i].score = maxelem;

                    StateData state = (StateData) mapGet(eurovision->state, &index);
                    sortTempArray[i].name = stringCopy(state->stateName);
                    //regularcase
                }

            } else {

                sortTempArray[i].index = index;

                sortTempArray[i].score = maxelem;

                StateData state = (StateData) mapGet(eurovision->state, &index);
                sortTempArray[i].name = stringCopy(state->stateName);
            }


        }
        finalTally[index] = -1;
    }


    List list = listCreate(copyString, freeString);

    for (int i = 0; i < numberOfStates; i++) {
        ListElement s = (ListElement) sortTempArray[i].name;
        listInsertLast(list, s);
    }
     for (int i = 0; i < numberOfStates; i++) {
      free(sortTempArray[i].name);
     }

    free(votesTables);
    free(judgeTables);
    free(sortTempArray);
    free(stateTotalVotejudge);

    free( finalTally);
    free( stateTotalVote);
    return list;


}


List eurovisionRunAudienceFavorite(Eurovision eurovision) {
    if (eurovision == NULL) {
        return NULL;
    }
    /// create vote table of all state
    int numberOfStates = mapGetSize(eurovision->state);
    int *votesTables = (int *) malloc(sizeof(int) * numberOfStates * numberOfStates);
    /// @todo add  check

    memset(votesTables, 0, sizeof(int) * numberOfStates * numberOfStates);


    /// later return to map for each
    for (int *iterator = mapGetFirst(eurovision->state); iterator; iterator = mapGetNext(eurovision->state)) {
        StateData state = (StateData) mapGet(eurovision->state, iterator);

        for (int *iterVotes = mapGetFirst(state->citizenVote); iterVotes; iterVotes = mapGetNext(state->citizenVote)) {
            int votesCast = *(int *) mapGet(state->citizenVote, iterVotes);
//            printf(" state %d gives %d votes: %d \n",*iterator,*iterVotes,votesCast);

            votesTables[*iterator * numberOfStates + *iterVotes] = votesCast;
        }
    }
    for (int i = 0; i < numberOfStates; i++) {
        convertRowToPoints(&votesTables[i * numberOfStates], numberOfStates);
    }

    /// float array of country and total votes
    float *stateTotalVote = (float *) malloc(sizeof(float) * numberOfStates);
    /// @todo add  check


    sumStateVotes(numberOfStates, numberOfStates, votesTables, stateTotalVote);

    Resulttmp *sortTempArray = (Resulttmp *) malloc(sizeof(Resulttmp) * numberOfStates);
    float maxelem;
    int index;


    for (int i = 0; i < numberOfStates; i++) {
        findMaxfloat(stateTotalVote, numberOfStates, &index, &maxelem);


        if (i == 0) {

            sortTempArray[i].index = index;

            sortTempArray[i].score = maxelem;

            StateData state = (StateData) mapGet(eurovision->state, &index);
            sortTempArray[i].name = stringCopy(state->stateName);
        } else {

            if (sortTempArray[i - 1].score == maxelem)// same scroe
            {
                if (sortTempArray[i - 1].index > index) {
                    // swap
                    Resulttmp tmp;
                    tmp.index = sortTempArray[i - 1].index;
                    tmp.name = sortTempArray[i - 1].name;
                    tmp.score = sortTempArray[i - 1].score;

                    sortTempArray[i - 1].index = index;
                    sortTempArray[i - 1].score = maxelem;

                    StateData state = (StateData) mapGet(eurovision->state, &index);
                    sortTempArray[i - 1].name = stringCopy(state->stateName);

                    sortTempArray[i - 1].index = tmp.index;
                    sortTempArray[i - 1].score = tmp.score;
                    sortTempArray[i - 1].name = tmp.name;


                } else {
                    sortTempArray[i].index = index;
                    sortTempArray[i].score = maxelem;

                    StateData state = (StateData) mapGet(eurovision->state, &index);
                    sortTempArray[i].name = stringCopy(state->stateName);
                    //regularcase
                }

            } else {

                sortTempArray[i].index = index;

                sortTempArray[i].score = maxelem;

                StateData state = (StateData) mapGet(eurovision->state, &index);
                sortTempArray[i].name = stringCopy(state->stateName);
            }


        }
        stateTotalVote[index] = -1;
    }


    List list = listCreate(copyString, freeString);

    for (int i = 0; i < numberOfStates; i++) {
        ListElement s = (ListElement) sortTempArray[i].name;
        listInsertLast(list, s);
    }
    for (int i = 0; i < numberOfStates; i++) {
     free(sortTempArray[i].name);
    }
    free(stateTotalVote);
    free(votesTables);
    free(sortTempArray);
    return list;

}

int strincompare(ListElement s1, ListElement s2) {
    return strcmp(s1, s2);
}

List eurovisionRunGetFriendlyStates(Eurovision eurovision) {


    if (eurovision == NULL) {
        return NULL;
    }
    /// create vote table of all state
    int numberOfStates = mapGetSize(eurovision->state);
    int *votesTables = (int *) malloc(sizeof(int) * numberOfStates * numberOfStates);
    /// @todo add  check

    memset(votesTables, 0, sizeof(int) * numberOfStates * numberOfStates);


    /// later return to map for each
    for (int *iterator = mapGetFirst(eurovision->state); iterator; iterator = mapGetNext(eurovision->state)) {
        StateData state = (StateData) mapGet(eurovision->state, iterator);

        for (int *iterVotes = mapGetFirst(state->citizenVote); iterVotes; iterVotes = mapGetNext(state->citizenVote)) {
            int votesCast = *(int *) mapGet(state->citizenVote, iterVotes);

            votesTables[*iterator * numberOfStates + *iterVotes] = votesCast;
        }
    }
    for (int i = 0; i < numberOfStates; i++) {
        convertRowToPoints(&votesTables[i * numberOfStates], numberOfStates);
    }

//    /// float array of country and total votes
    int *friendMat = (int *) malloc(sizeof(int) * numberOfStates * numberOfStates);
//    /// @todo add  check
    for (int i = 0; i < numberOfStates; i++) {
        for (int j = 0; j < numberOfStates; j++) {
            friendMat[i * numberOfStates + j] = 0;
        }
    }

    for (int i = 0; i < numberOfStates; i++) {
        for (int j = 0; j < numberOfStates; j++) {
            if ((votesTables[i * numberOfStates + j] == votesTables[j * numberOfStates + i])
                && votesTables[j * numberOfStates + i] == MAX_VOTE_POSIBLE) {
                if (j > i) {
                    friendMat[i * numberOfStates + j] = 1;
                }

            }

        }
    }
    List list = listCreate(copyString, freeString);

    for (int i = 0; i < numberOfStates; i++) {
        for (int j = 0; j < numberOfStates; j++) {
            if (friendMat[i * numberOfStates + j] == 1) {
                StateData state1 = (StateData) mapGet(eurovision->state, &i);
                StateData state2 = (StateData) mapGet(eurovision->state, &j);
                char *finalStr = (char *) malloc(
                        sizeof(char) * strlen(state1->stateName) * strlen(state2->stateName) + 3 + 1);
                if (strcmp(state1->stateName, state2->stateName) < 0) {

                    strcpy(finalStr, state1->stateName);
                    char *currpoint = finalStr + strlen(state1->stateName);
                    *currpoint = ' ';
                    currpoint++;
                    *currpoint = '-';
                    currpoint++;
                    *currpoint = ' ';
                    currpoint++;
                    strcpy(currpoint, state2->stateName);
                } else {
                    strcpy(finalStr, state2->stateName);
                    char *currpoint = finalStr + strlen(state2->stateName);
                    *currpoint = ' ';
                    currpoint++;
                    *currpoint = '-';
                    currpoint++;
                    *currpoint = ' ';
                    currpoint++;
                    strcpy(currpoint, state1->stateName);
                }
                ListElement s = (ListElement) finalStr;
                listInsertLast(list, s);
                free(finalStr);
            }

            listSort(list, strincompare);
        }
    }
   free(votesTables);
    free(friendMat);
    return list;

}




