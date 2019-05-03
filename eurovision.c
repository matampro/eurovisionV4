#include <stdio.h>
#include "eurovision.h"
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>


#define NUMBER_OF_RESULTS 10
#define MAX_NUMBER_OF_WINNERS 10
#define TOTAL_PRECENT 100
#define FIRST_ROW 0
#define SECEND_ROW 1


List makeSortedListByMapData(Map map);
void printResult(List list);
int place[MAX_NUMBER_OF_WINNERS] = {12, 10, 8, 7, 6, 5, 4, 3, 2, 1};

char* stringCopy(const char* str) {
    if(str == NULL){
        return NULL;
    }
    int strLen =strlen(str);
    char *strDest = malloc(sizeof(strLen + 1));
    if (strDest == NULL) {
        return NULL;
    } else {
        strcpy(strDest, str);
    }
    return strDest;
}


static StateDataMap copyStateData(StateDataMap dataToCopy){
    if(dataToCopy == NULL){
        return NULL;
    }
    StateData data =(StateData) dataToCopy;
    StateData stateDataNew = malloc(sizeof(*stateDataNew));
    if(stateDataNew == NULL){
        return NULL;
    } else{
        stateDataNew->stateName = stringCopy(data->stateName);
        if(stateDataNew->stateName == NULL) {
            return NULL;
        }
        stateDataNew->songName = stringCopy(data->songName);
        if(stateDataNew->songName == NULL){
            free(data->stateName);
            return NULL;
        }
        Map citizenVoteDest = mapCopy(data->citizenVote);
        if(citizenVoteDest == NULL){
            free(data->stateName);
            free(data->songName);
            return NULL;
        }
        data->citizenVote = citizenVoteDest;
    }
    return stateDataNew;
}
/** Function to be used by the map for freeing elements */
static void freeStateData(StateDataMap dataToFree) {
    StateData toFree = (StateData) dataToFree;
    free(toFree->stateName);
    free(toFree->songName);
    mapDestroy(toFree->citizenVote);
}


static VoteDataElement copyVoteDataElement(VoteDataElement voteToCopy){
    if(voteToCopy == NULL){
        return NULL;
    }
    int *ptr = malloc(sizeof(int));
    if(ptr == NULL){
        return NULL;
    }
    *ptr = *(int*)voteToCopy;
    return ptr;
}

static KeyElement copyKeyElement(KeyElement keyToCopy){
    if(keyToCopy == NULL){
        return NULL;
    }
    int *ptr = malloc(sizeof(int));
    if(ptr == NULL){
        return NULL;
    }
    *ptr = *(int*)keyToCopy;
    return ptr;
}

void freeVoteDataElement(VoteDataElement voteToFree){
    free(voteToFree);
}

void freeKeyElement(KeyElement keyToFree){
    free(keyToFree);
}

int compareKeyElements(KeyElement key1,KeyElement key2) {
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


static  JudgeDataMap copyJudgeDataElement(JudgeDataMap judgeDataToCopy){
    if(judgeDataToCopy == NULL){
        return NULL;
    }
    JudgeData data =(JudgeData)judgeDataToCopy;
    JudgeData judgeDataNew = malloc(sizeof(*judgeDataNew));
    if(judgeDataNew == NULL){
        return NULL;
    }else {
        judgeDataNew->judgeName = stringCopy(data->judgeName);
        if (judgeDataNew->judgeName == NULL) {
            freeJudgeDataElement(judgeDataNew);
            return NULL;
        }
        judgeDataNew->judgeResults = malloc((sizeof(int))*NUMBER_OF_RESULTS);
        if(judgeDataNew->judgeResults == NULL){
            freeJudgeDataElement(judgeDataNew);
            free(judgeDataNew->judgeName);
            return NULL;
        }
        for (int i = 0; i < NUMBER_OF_RESULTS; i++) {
            judgeDataNew->judgeResults[i] = data->judgeResults[i];
        }
        return judgeDataNew;
    }
}

static void freeJudgeDataElement(JudgeDataMap judgeDataToFree){
    JudgeData ptr = (JudgeData) judgeDataToFree;
    free(ptr->judgeName);
    free(ptr->judgeResults);
}

Eurovision eurovisionCreate(){
    Eurovision eurovision = malloc(sizeof(*eurovision));
    if(eurovision == NULL){
        return NULL;
    }
    eurovision->state = mapCreate(copyStateData,copyKeyElement,freeStateData,freeKeyElement,compareKeyElements);
    if(eurovision->state == NULL){
        free(eurovision);
        return NULL;
    }
    eurovision->judge = mapCreate(copyJudgeDataElement,copyKeyElement,freeJudgeDataElement,freeKeyElement,
                                  compareKeyElements);
    if(eurovision->judge == NULL){
        free(eurovision);
        free(eurovision->state);
        return NULL;
    }
    return eurovision;
}

void eurovisionDestroy(Eurovision eurovision){
    mapDestroy(eurovision->state);
    mapDestroy(eurovision->judge);
}


bool checkIfNotNegitive(int num){
    if(num < 0) {
        return false;
    } else{
        return true;
    }
}

bool checkIfNameIsLegal(const char *name){
    char *copyName = stringCopy(name);
    while (*copyName != '\0'){
        if(((*copyName <= 'a') || (*copyName >= 'z')) && *copyName != ' '){
            free(copyName);
            return false;
        }
    }
    free(copyName);
    return true;
}

EurovisionResult eurovisionAddState(Eurovision eurovision, int stateId, const char *stateName, const char *songName)
{
    if(!checkIfNotNegitive(stateId)){
        return EUROVISION_INVALID_ID;
    } else if(mapContains(eurovision->state, &stateId)){
        return EUROVISION_STATE_ALREADY_EXIST;
    }else if( !checkIfNameIsLegal(stateName) || !checkIfNameIsLegal(songName)){
        return EUROVISION_INVALID_NAME;
    }else{
        StateData newStateData = (StateData) malloc(sizeof(*newStateData));
        if(newStateData == NULL){
            eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        }
        newStateData->stateName = stringCopy(stateName);
        if(newStateData->stateName == NULL){
            freeStateData(newStateData);
            free(newStateData);
            eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        }
        newStateData->songName = stringCopy(songName);
        if(newStateData->songName == NULL){
            freeStateData(newStateData);
            free(newStateData);
            eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        }
        void* result = mapCreate(copyVoteDataElement,copyKeyElement,freeVoteDataElement,freeKeyElement,                  //where goes the pointer of mapCreate?
                                 compareKeyElements);
        if(result == NULL) {
            freeStateData(newStateData);
            free(newStateData);
            eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        }else{
            return EUROVISION_SUCCESS;
        }
    }
}

EurovisionResult eurovisionRemoveState(Eurovision eurovision, int stateId){
    if(!checkIfNotNegitive(stateId)){
        return EUROVISION_INVALID_ID;
    }else{
        MapResult result = mapRemove(eurovision->state,&stateId);
        if(result == MAP_ITEM_DOES_NOT_EXIST){
            return EUROVISION_STATE_NOT_EXIST;
        }
        MAP_FOREACH(int *,iter,eurovision->state)
        {
            StateData stateData = mapGet(eurovision->state,iter);
            if(stateData == NULL){
                return EUROVISION_NULL_ARGUMENT;
            }
            Map citizenVote = stateData->citizenVote;
            MapResult result1 = mapRemove(citizenVote, &stateId);
            if(result1 == MAP_NULL_ARGUMENT){
                return EUROVISION_NULL_ARGUMENT;
            }
        }
    }
}

EurovisionResult eurovisionAddJudge(Eurovision eurovision, int judgeId,const char *judgeName,int *judgeResults) {
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
        newJudgeData->judgeResults[i] = judgeResults[i];
    }
    MapResult result = mapPut(eurovision->judge, &judgeId, &newJudgeData);
    if (result == MAP_OUT_OF_MEMORY) {
        freeJudgeDataElement(newJudgeData);
        free(newJudgeData);
        eurovisionDestroy(eurovision);
        return EUROVISION_OUT_OF_MEMORY;
    } else {
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
    if(result == MAP_NULL_ARGUMENT){
        return EUROVISION_NULL_ARGUMENT;
    }
    return EUROVISION_SUCCESS;
}

EurovisionResult eurovisionAddVote(Eurovision eurovision, int stateGiver, int stateTaker)
{
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
    } else {
        numVotes = *(int *) votes;
        MapResult result = mapPut(stateData->citizenVote, &stateTaker, &numVotes + 1);
        if (result == MAP_OUT_OF_MEMORY) {
            eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        }
        return EUROVISION_SUCCESS;
    }
}

EurovisionResult eurovisionRemoveVote(Eurovision eurovision, int stateGiver, int stateTaker)
{
    if((!checkIfNotNegitive(stateGiver)) || (!checkIfNotNegitive(stateTaker))){
        return EUROVISION_INVALID_ID;
    }else if(stateGiver == stateTaker){
        return EUROVISION_SAME_STATE;
    }else{
        bool result1 = mapContains(eurovision->state,&stateGiver);
        bool result2 = mapContains(eurovision->state,&stateTaker);
        if((!result1) || (!result2)){
            return EUROVISION_STATE_NOT_EXIST;
        }
    }
    StateData stateData = (StateData) mapGet(eurovision->state,&stateGiver);
    void*  votes = (void*) mapGet(stateData->citizenVote,&stateTaker);
    int numVotes;
    if(votes == NULL){
        return EUROVISION_SUCCESS;
    }else{
        numVotes = *(int *) votes;
        MapResult result = mapPut(stateData->citizenVote, &stateTaker, &numVotes - 1);
        if (result == MAP_OUT_OF_MEMORY) {
            eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        }
        return EUROVISION_SUCCESS;
    }
}

List eurovisionRunContest(Eurovision eurovision, int audiencePrecent) {
    if ((eurovision == NULL) || (audiencePrecent < 1) || (audiencePrecent > 100)) {
        return NULL;
    }
    int number_of_voted_states = MAX_NUMBER_OF_WINNERS;
    Map winnersMap = mapCreate(copyVoteDataElement, copyKeyElement, freeVoteDataElement, freeKeyElement,
                               compareKeyElements);      // we count the score here
    if (winnersMap == NULL){
        return NULL;
    }
    int initData = 0;
    KeyElement keyElement = mapGetFirst(eurovision->state);
    MapResult result = mapPut(winnersMap, keyElement,&initData); //we set first state
    if (result == MAP_OUT_OF_MEMORY){
        return NULL;
    }
    while (mapGetNext(eurovision->state) != NULL){
        keyElement = mapGetFirst(eurovision->state);
        result = mapPut(winnersMap, keyElement,&initData); //we set rest of the states
        if (result == MAP_OUT_OF_MEMORY){
            return NULL;
        }
    }
    MAP_FOREACH(int *,iterator,eurovision->state){
        StateData stateData = mapGet(eurovision->state,iterator);
        if(stateData == NULL){
            return NULL;
        }
        Map citizenVote = stateData->citizenVote;
        List current_list = NULL;
        current_list = makeSortedListByMapData(citizenVote);
        if (current_list == NULL){
            return NULL;
        }
        int list_size = listGetSize(current_list);
        if (list_size == -1){
            continue;
        }
        if (list_size < MAX_NUMBER_OF_WINNERS){
            number_of_voted_states = list_size;
        }
        ListElement current_state = NULL;
        int i = 0;
        for (current_state = listGetFirst(current_list);
                i < number_of_voted_states; i++, current_state = listGetNext(current_list)) {
            result = mapPut(winnersMap, current_state,
                    mapGet(winnersMap, current_state) + (place[i] * audiencePrecent / TOTAL_PRECENT));
            // ( int place[NUMBER_OF_WINNERS] = (12, 10, 8, 7, 6, 5, 4, 3, 2, 1) )
            if (result == MAP_OUT_OF_MEMORY){
                return NULL;
            }
        }
    }
    MAP_FOREACH(int*, iterator, eurovision->judge){
        JudgeData judgeData = NULL;
        judgeData = mapGet(eurovision->judge, iterator);
        for (int i = 0; i < MAX_NUMBER_OF_WINNERS; i++) {
            if (!mapContains(winnersMap, &judgeData->judgeResults[i])){
                eurovisionRemoveJudge(eurovision, *(int*)iterator);
                return NULL;
            }
            result = mapPut(winnersMap, &judgeData->judgeResults[i],
                    mapGet(winnersMap, &judgeData->judgeResults[i]) +
                    ((place[i] * (TOTAL_PRECENT - audiencePrecent)) / TOTAL_PRECENT));
            if (result == MAP_OUT_OF_MEMORY){
                return NULL;
            }
        }
    }
    List winnersList = makeSortedListByMapData(winnersMap);
    if (winnersList == NULL){
        return NULL;
    }
    printResult(winnersList);
}

List makeSortedListByMapData(Map map){
    List list = listCreate(copyKeyElement, freeKeyElement);
    if (list == NULL) {
        return NULL;
    }
    MAP_FOREACH(int*, iterator, map) {
        if (listGetSize(list) == 0) {                   // this is the first element in the list
            ListResult result = listInsertFirst(list, mapGet(map, iterator));
            if (result == LIST_OUT_OF_MEMORY) {
                return NULL;
            }
        }
        else{                                                   //this is not the first element. we need to find its place in list
            bool inserted = 0;
            for(ListElement next_key_in_list = listGetFirst(list) ; next_key_in_list == NULL; next_key_in_list = listGetNext(list)){
                if (mapGet(map, next_key_in_list) < mapGet(map, iterator)) {
                    inserted = 1;
                    ListResult result = listInsertBeforeCurrent(list, iterator);
                    if (result == LIST_OUT_OF_MEMORY) {
                        return NULL;
                    }
                }
                else if (mapGet(map, next_key_in_list) == mapGet(map, iterator)) {
                    inserted = 1;
                    if (*(int*)next_key_in_list < *(int*)iterator) {
                        ListResult result = listInsertBeforeCurrent(list, iterator);
                        if (result == LIST_OUT_OF_MEMORY) {
                            return NULL;
                        }
                    }
                }
            }
            if (inserted == 0){
                ListResult result = listInsertAfterCurrent(list, iterator);
                if (result == LIST_OUT_OF_MEMORY) {
                    return NULL;
                }
            }
        }
    }
    return list;
}

void printResult(List list) {
    char *next_winner = listGetFirst(list);
    if (next_winner == NULL) {
        return;
    } else {
        printf( "%s", &next_winner);
    }
    while (next_winner != NULL) {
        char *next_winner = listGetNext(list);
        if (next_winner == NULL) {
            return;
        } else {
            printf( "%s", &next_winner);
        }
    }
}

List eurovisionRunAudienceFavorite(Eurovision eurovision) {
    int number_of_voted_states = MAX_NUMBER_OF_WINNERS;
    Map winnersMap = mapCreate(copyVoteDataElement, copyKeyElement, freeVoteDataElement, freeKeyElement,
                               compareKeyElements);      // we count the score here
    if (winnersMap == NULL) {
        return NULL;
    }
    int initData = 0;
    KeyElement keyElement = mapGetFirst(eurovision->state);
    MapResult result = mapPut(winnersMap, keyElement, &initData); //we set first state
    if (result == MAP_OUT_OF_MEMORY) {
        return NULL;
    }
    while (mapGetNext(eurovision->state) != NULL) {
        keyElement = mapGetFirst(eurovision->state);
        result = mapPut(winnersMap, keyElement, &initData); //we set rest of the states
        if (result == MAP_OUT_OF_MEMORY) {
            return NULL;
        }
    }
    MAP_FOREACH(int *, iterator, eurovision->state) {
        StateData stateData = mapGet(eurovision->state, iterator);
        if (stateData == NULL) {
            return NULL;
        }
        Map citizenVote = stateData->citizenVote;
        List current_list = NULL;
        current_list = makeSortedListByMapData(citizenVote);
        if (current_list == NULL) {
            return NULL;
        }
        int list_size = listGetSize(current_list);
        if (list_size == -1) {
            continue;
        }
        if (list_size < MAX_NUMBER_OF_WINNERS) {
            number_of_voted_states = list_size;
        }
        ListElement current_state = NULL;
        int i = 0;
        for (current_state = listGetFirst(current_list);
             i < number_of_voted_states; i++, current_state = listGetNext(current_list)) {
            result = mapPut(winnersMap, current_state, mapGet(winnersMap, current_state) + place[i]);
            // ( int place[NUMBER_OF_WINNERS] = (12, 10, 8, 7, 6, 5, 4, 3, 2, 1) )
            if (result == MAP_OUT_OF_MEMORY) {
                return NULL;
            }
        }
    }
    List winnersList = makeSortedListByMapData(winnersMap);
    if (winnersList == NULL) {
        return NULL;
    }
    printResult(winnersList);
}


List eurovisionRunGetFriendlyStates(Eurovision eurovision) {
    if (eurovision == NULL) {
        return NULL;
    }
    int numState = 0;
    numState = mapGetSize(eurovision->state);
    if (numState > 0) {
        int friendlyState[][numState] = {{0}};
        for (int i = 0; i < numState; i++) {
            int curStateId = *(int *) mapGetFirst(eurovision->state);
            friendlyState[FIRST_ROW][i] = curStateId;
        }
        MAP_FOREACH(int *, iter, eurovision->state) {
            StateData stateData = mapGet(eurovision->state, iter);
            if (stateData == NULL) {
                return NULL;
            }
            ListResult sortedList = listSort((List)stateData->citizenVote, compareKeyElements);  ///???
            if (sortedList != LIST_SUCCESS) {
                return NULL;
            }
            KeyElement favoriteState = listGetFirst((List)stateData->citizenVote);
            if (favoriteState = NULL) {
                return NULL;
            } else {
                for (int j = 0; j < numState; j++) {
                    if (friendlyState[FIRST_ROW][j] == *(int *)iter) {
                        friendlyState[SECEND_ROW][j] = *(int *)favoriteState;
                    }
                }

            }
        }
        for(int i=0; i <numState; i++){
            int firstChoice = friendlyState[SECEND_ROW][i];
            for(int j=0; j< numState ;j++){
                if((friendlyState[FIRST_ROW][j] == firstChoice) &&
                                                        (friendlyState[SECEND_ROW][j] == friendlyState[FIRST_ROW][i])){

                }
            }
        }

    }

}
