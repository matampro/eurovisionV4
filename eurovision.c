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


typedef struct resutlstring_t{
    float  score ;
    int index  ;
    char* name;
}Resulttmp;




typedef struct node_t{
    MapDataElement mapDataElement;
    MapKeyElement mapKeyElement;
    struct node_t* next;
}*Node;

typedef struct {
    int counter;
    Node head;
    Node tail;
    copyMapDataElements data_copy;
    copyMapKeyElements key_copy;
    compareMapKeyElements compair_key;
    freeMapDataElements free_data;
    freeMapKeyElements free_key;
}*Map_cheat;


List makeSortedListByMapData(Map map);
void printResult(List list);
char* createStr (char* str1, char* str2);
ListElement copyFriendly(ListElement);
void freeFriendly(ListElement);
int compareFriendly(ListElement , ListElement);


int place[MAX_NUMBER_OF_WINNERS] = {12, 10, 8, 7, 6, 5, 4, 3, 2, 1};

char* stringCopy(const char* str) {
    if(str == NULL){
        return NULL;
    }
    size_t strLength =strlen(str);
    char *strDest = malloc(sizeof(strLength + 1));
    if (strDest == NULL) {
        return NULL;
    } else {
        strcpy(strDest, str);
    }
    return strDest;
}
//static char* copyStringData(char* str)
ListElement copyStringData(ListElement strvoid)
 {

    if(strvoid  == NULL){
        return NULL;
    }
    char * str = (char *)strvoid;
    size_t strLength = strlen (str);
    char *strDest = malloc(sizeof(strLength + 1));
    if (strDest == NULL) {
        return NULL;
    } else {
        strcpy(strDest, str);
    }
    return strDest;
}




static StateDataMap copyStateData(StateDataMap dataToCopy){
    LOG
            if(dataToCopy == NULL){
        return NULL;
    }
    StateData data =(StateData) dataToCopy;
    StateData stateDataNew = malloc(sizeof(*stateDataNew));
    if(stateDataNew == NULL){
        return NULL;
    } else{
        LOG
                stateDataNew->stateName = stringCopy(data->stateName);
        if(stateDataNew->stateName == NULL) {
            return NULL;
        }
        LOG
                stateDataNew->songName = stringCopy(data->songName);
        if(stateDataNew->songName == NULL){
            free(data->stateName);
            return NULL;
        }
        LOG
                Map citizenVoteDest = mapCopy(data->citizenVote);
        if(citizenVoteDest == NULL){
            printf(" map coput fail!!!!!!!!!!!!---------%s   %s-------\n\n\n",data->stateName,data->songName);
            free(data->stateName);
            free(data->songName);
            return NULL;
        }
        LOG
                stateDataNew->citizenVote = citizenVoteDest;
    }
    LOG
            return stateDataNew;
}
/** Function to be used by the map for freeing elements */
static void freeStateData(StateDataMap dataToFree) {
    LOG

            if(dataToFree == NULL) /// protection from null is nice :)
            printf("origin NULL \n");
    StateData toFree = (StateData) dataToFree;


    if(toFree == NULL) /// protection from null is nice :)
        printf("NULL \n");
    LOG

            free(toFree->stateName);
    LOG
            free(toFree->songName);
    LOG
            mapDestroy(toFree->citizenVote);
    LOG
}


static VoteDataElement copyVoteDataElement(VoteDataElement voteToCopy){
    LOG4
            if(voteToCopy == NULL){
        return NULL;
    }
    LOG4
            int *ptr = malloc(sizeof(int));
    if(ptr == NULL){
        return NULL;
    }
    *ptr = *(int*)voteToCopy;
    LOG4
            return ptr;
}

static KeyElement copyKeyElement(KeyElement keyToCopy){
    LOG4
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

void freeStringData(ListElement str)
{
    free(str);
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
    LOG4
            JudgeData data =(JudgeData)judgeDataToCopy;

    if(data->judgeName == NULL || data->judgeResults == NULL){
        LOGJ
                return NULL;
    }
    JudgeData judgeDataNew = malloc(sizeof(*judgeDataNew));
    LOGJ
            if(judgeDataNew == NULL){
        return NULL;
    }else {
        LOGJ
                judgeDataNew->judgeName = stringCopy(data->judgeName);
        if (judgeDataNew->judgeName == NULL) {
            freeJudgeDataElement(judgeDataNew);
            return NULL;
        }
        LOGJ
                judgeDataNew->judgeResults = malloc((sizeof(int)) * NUMBER_OF_RESULTS);
        if(judgeDataNew->judgeResults == NULL){
            freeJudgeDataElement(judgeDataNew);
            free(judgeDataNew->judgeName);
            return NULL;
        }
        LOGJ
                for (int i = 0; i < NUMBER_OF_RESULTS; i++) {

            LOGJ

                    judgeDataNew->judgeResults[i] = data->judgeResults[i];
        }
        LOGJ
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
    LOG
            mapDestroy(eurovision->state);
    LOG
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
    char *copyNameOriginal = stringCopy(name); /// free from middle of string need to read about behavier but not stable
    char *copyName = copyNameOriginal;
    if(copyName == NULL)
    {
        free(copyNameOriginal);
        return false;
    }

    while (*copyName != '\0'){
        if(((*copyName < 'a') || (*copyName > 'z')) && *copyName != ' '){
            printf (" %c \n", *copyName);
            free(copyNameOriginal);
            return false;
        }

        copyName++; //bug no addition to pointer
    }
    free(copyNameOriginal);
    return true;
}

EurovisionResult eurovisionAddState(Eurovision eurovision, int stateId, const char *stateName, const char *songName){
    LOG

            if (eurovision == NULL){
        return EUROVISION_NULL_ARGUMENT;
    } else if (!checkIfNotNegitive(stateId)){
        return EUROVISION_INVALID_ID;
    } else if (mapContains(eurovision->state, &stateId)){
        LOG
                return EUROVISION_STATE_ALREADY_EXIST;
    }else if( !checkIfNameIsLegal(stateName) || !checkIfNameIsLegal(songName)){
        return EUROVISION_INVALID_NAME;
    }else{
        LOG
                StateData newStateData = (StateData) malloc(sizeof(*newStateData));
        if(newStateData == NULL){
            eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        }
        LOG
                newStateData->stateName = stringCopy(stateName);
        if(newStateData->stateName == NULL){
            freeStateData(newStateData);
            free(newStateData);
            eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        }
        LOG
                newStateData->songName = stringCopy(songName);
        if(newStateData->songName == NULL){
            freeStateData(newStateData);
            free(newStateData);
            eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        }

        Map citizenVote = mapCreate(copyVoteDataElement,copyKeyElement,freeVoteDataElement,freeKeyElement,
                                    compareKeyElements);/// if you create a new cunk of data you must attach it!
        LOG
                if(citizenVote == NULL) {
            freeStateData(newStateData);
            free(newStateData);
            eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        }else{
            //            printf(" =====++++++++has copy pont stateid %d %p \n",stateId,((Map_cheat)(citizenVote))->data_copy);
            newStateData->citizenVote = mapCopy(citizenVote);

            //printf(" =====++++++++has copy pont stateid %d %p \n",stateId,((Map_cheat)(citizenVote))->data_copy);
            printf("%d Adding state data -----%d-------%s----%s----%p\n",
                   __LINE__,stateId,newStateData->songName,newStateData->stateName);
            printf("  for the map has copy pont %p \n",((Map_cheat)(newStateData->citizenVote))->data_copy);

            MapResult mapResult = mapPut(eurovision->state, &stateId, newStateData);  //we have to add a new map to the *state
            if(mapResult == MAP_OUT_OF_MEMORY) {
                freeStateData(newStateData);
                free(newStateData);
                eurovisionDestroy(eurovision);
                return EUROVISION_OUT_OF_MEMORY;
            }

            LOG
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
            LOGR
                    return EUROVISION_STATE_NOT_EXIST;
        }
        LOGR
                MAP_FOREACH(int *,iter,eurovision->state)
        {
            StateData stateData = mapGet(eurovision->state,iter);
            if(stateData == NULL){
                LOGR
                        return EUROVISION_NULL_ARGUMENT;
            }
            LOGR
                    Map citizenVote = stateData->citizenVote;
            if(citizenVote == NULL )
            {printf("Error null votes for iter %d\n", *iter);
                return EUROVISION_NULL_ARGUMENT;
            }
            LOGR
                    MapResult result1 = mapRemove(citizenVote, &stateId);
            if(result1 == MAP_NULL_ARGUMENT){
                LOGR
                        return EUROVISION_NULL_ARGUMENT;
            }
        }
        LOGR
                if(MAP_SUCCESS == result)
                return EUROVISION_SUCCESS;

    }
}

EurovisionResult eurovisionAddJudge(Eurovision eurovision, int judgeId,const char *judgeName,int *judgeResults) {
    if ((eurovision == NULL) || (judgeName == NULL) || (judgeResults == NULL)) {
        return EUROVISION_NULL_ARGUMENT;
    }
    LOGJ
            if (!checkIfNotNegitive(judgeId)) {
        return EUROVISION_INVALID_ID;
    } else if (mapContains(eurovision->judge, &judgeId)) {
        return EUROVISION_JUDGE_ALREADY_EXIST;
    } else if (!checkIfNameIsLegal(judgeName)) {
        return EUROVISION_INVALID_NAME;
    }
    LOGJ
            JudgeData newJudgeData = (JudgeData) malloc(sizeof(*newJudgeData));
    if (newJudgeData == NULL) {
        eurovisionDestroy(eurovision);
        return EUROVISION_OUT_OF_MEMORY;
    }
    LOGJ
            newJudgeData->judgeName = stringCopy(judgeName);
    if (newJudgeData->judgeName == NULL) {
        free(newJudgeData);
        eurovisionDestroy(eurovision);
        return EUROVISION_OUT_OF_MEMORY;
    }
    LOGJ
            newJudgeData->judgeResults = malloc(sizeof(int) * NUMBER_OF_RESULTS);
    if (newJudgeData->judgeResults == NULL) {
        free(newJudgeData->judgeName);
        free(newJudgeData);
        return EUROVISION_OUT_OF_MEMORY;
    }
    LOGJ
            for (int i = 0; i < NUMBER_OF_RESULTS; i++) {
        if(checkIfNotNegitive(judgeResults[i]) )
        {
            if( mapContains(eurovision->state,&(judgeResults[i])))
            {

                newJudgeData->judgeResults[i] = judgeResults[i];
            }
            else
            {


                return EUROVISION_STATE_NOT_EXIST;
            }
        }
        else{


            return EUROVISION_INVALID_ID;

        }

    }
    LOGJ

            MapResult result = mapPut(eurovision->judge, &judgeId, newJudgeData); // pointer to pointer
    LOGJ
            if (result == MAP_OUT_OF_MEMORY) {
        freeJudgeDataElement(newJudgeData);
        free(newJudgeData);
        eurovisionDestroy(eurovision);
        return EUROVISION_OUT_OF_MEMORY;
    } else {
        return EUROVISION_SUCCESS;
    }
    LOGJ
}


EurovisionResult eurovisionRemoveJudge(Eurovision eurovision, int judgeId) {
    if (!checkIfNotNegitive(judgeId)) {
        LOGJ
                return EUROVISION_INVALID_ID;
    } else if (!mapContains(eurovision->judge, &judgeId)) {
        LOGJ
                return EUROVISION_JUDGE_NOT_EXIST;
    }
    LOGJ
            MapResult result = mapRemove(eurovision->judge, &judgeId);
    if(result == MAP_NULL_ARGUMENT){
        return EUROVISION_NULL_ARGUMENT;
    }
    LOGJ
            if(result == MAP_SUCCESS)
    {LOGJ
                return EUROVISION_SUCCESS;
    }
    else
    {
        LOGJ
                return MAP_ITEM_DOES_NOT_EXIST;
    }

}

EurovisionResult eurovisionAddVote(Eurovision eurovision, int stateGiver, int stateTaker)
{
    LOG4
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
    LOG4

            StateData stateData = (StateData) mapGet(eurovision->state, &stateGiver);
    // printf(" =====++++++++has copy pont stateid %d %p \n",stateGiver,((Map_cheat)(stateData->citizenVote))->data_copy);

    void *votes = (void *) mapGet(stateData->citizenVote, &stateTaker);


    int numVotes;
    LOG4
            if (votes == NULL) {
        numVotes = 1;
        LOG4

                MapResult result = mapPut(stateData->citizenVote, &stateTaker, &numVotes);
        LOG4
                if (result == MAP_OUT_OF_MEMORY) {
            LOG4
                    eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        }
        if  (result == MAP_SUCCESS) {///// need to correct all possible return pattern results
            return EUROVISION_SUCCESS;
        }

    } else {
        LOG4
                numVotes = *(int *) votes;
        numVotes++;
        if(stateTaker == 2)
            printf(" ===== %d %d \n",stateTaker,numVotes);
        MapResult result = mapPut(stateData->citizenVote, &stateTaker, &numVotes );
        if (result == MAP_OUT_OF_MEMORY) {
            LOG4
                    eurovisionDestroy(eurovision);
            return EUROVISION_OUT_OF_MEMORY;
        }
        LOG4
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
void findMax( int* row ,size_t  numberOfStates,int *index,int *val)
{
    int max=-1;
    *index=-1;
    *val =-1;
    for(int i=0;i<numberOfStates;i++)
    {
        // printf(" %d %d \n",__LINE__ ,i);

        if(row[i]>max)
        {
            max=row[i];
            *val=max;
            *index = i;
        }
    }
    // printf(" %d\n",__LINE__);

}
void findMaxfloat( float* row ,size_t  numberOfStates,int *index,float *val)
{
    float max=-1;
    *index=-1;
    *val =-1;
    for(int i=0;i<numberOfStates;i++)
    {
        // printf(" %d %d \n",__LINE__ ,i);

        if(row[i]>max)
        {
            max=row[i];
            *val=max;
            *index = i;
        }
    }
    // printf(" %d\n",__LINE__);

}

void convertRowToPoints( int* row ,size_t  numberOfStates)
{

    int voteTranslation[10] = {12,10,8,7,6,5,4,3,2,1};

    int* tempRow = (int* )malloc(sizeof(int)*numberOfStates);
    for(int i=0;i<numberOfStates;i++ )
    {
        tempRow[i] =0;
    }
    int index = -1;
    int val =-1;

    for(int currentVoteindex=0;currentVoteindex<MAX_VOTE_SIZE;currentVoteindex++ )
    {

        findMax( row ,numberOfStates,&index,&val);
        if(val>0)
        {
            tempRow[index]=voteTranslation[currentVoteindex];
        }
        else
        {
            tempRow[index]=0;
        }


        row[index]=-1;

    }

    for(int i=0;i<numberOfStates;i++ )
    {
        row[i]= tempRow[i];
    }

    //    memcpy(row,tempRow,sizeof(int)*numberOfStates);

    free(tempRow);
}

void sumStateVotes(size_t row, size_t col ,int *votesTables ,float* stateTotalVote )
{
    int totalVoteCount = 0;

    for(int i=0;i<row ;i++)
    {
        for(int j=0;j<col ;j++)
        {
            totalVoteCount+=votesTables[i*col+ j];
        }

    }
    printf("total votes %d\n\n\n",totalVoteCount);

    /// float array of country and total votes
    //    float* stateTotalVote = (float *)malloc(sizeof(float)*col );
    /// @todo add  check

    memset(stateTotalVote,sizeof(float)*col ,0);

    for(int i=0;i<row ;i++)
    {
        for(int j=0;j<col ;j++)
        {
            printf(" %d ",votesTables[i*col+ j]);
        }
        printf("\n");
    }

    for(int j=0;j<col ;j++)
    {
        for(int i=0;i<row  ;i++)
        {

            stateTotalVote[j]+=votesTables[i*col + j];
        }
    }

    //divide sum votes
    for(int j=0;j<col ;j++)
    {
        stateTotalVote[j]/=row;//totalVoteCount;

    }

    for(int i=0;i<col ;i++)
    { printf(" %.2f",stateTotalVote[i]);
    }
    printf(" \n");

}


ListElement copyString(ListElement str){
   return copyStringData(str);
}

void freeString(ListElement str)
{
    free(str);
}



List eurovisionRunContest1(Eurovision eurovision, int audiencePrecent) {
    if ((eurovision == NULL) || (audiencePrecent < 1) || (audiencePrecent > 100)) {
        return NULL;
    }
    /// create vote table of all state
    int numberOfStates = mapGetSize( eurovision->state);
    int* votesTables = (int *)malloc(sizeof(int)*numberOfStates * numberOfStates);
    /// @todo add  check

    memset(votesTables,sizeof(int)*numberOfStates * numberOfStates,0);


    /// later return to map for each
    for(int * iterator = mapGetFirst(eurovision->state);iterator;iterator =mapGetNext(eurovision->state)){
        StateData state = (StateData) mapGet(eurovision->state,iterator);

        for(int * iterVotes = mapGetFirst(state->citizenVote);iterVotes;iterVotes = mapGetNext(state->citizenVote))
        {
            int votesCast=*(int*)mapGet(state->citizenVote,iterVotes);
            printf(" state %d gives %d votes: %d \n",*iterator,*iterVotes,votesCast);

            votesTables[*iterator * numberOfStates + *iterVotes] = votesCast;
        }
    }
    /// convert row to points
    ///
    ///
    ///
    for(int i=0;i<numberOfStates ;i++)
    {
        for(int j=0;j<numberOfStates ;j++)
        {
            printf(" %d ",votesTables[i*numberOfStates+ j]);
        }
        printf("\n");
    }
    printf(" ------------------------------Convert -------------------- \n");
    for(int i=0;i<numberOfStates ;i++)
    {
        convertRowToPoints(&votesTables[i*numberOfStates],numberOfStates);
    }

    /// float array of country and total votes
    float* stateTotalVote = (float *)malloc(sizeof(float)*numberOfStates );
    /// @todo add  check


    sumStateVotes(numberOfStates,numberOfStates , votesTables ,stateTotalVote );

    ////--------------------------------------------------------judse part
    ///
    ///
    int numberOfJudge = mapGetSize(eurovision->judge);
    size_t sizeTableJudge= sizeof(int)* numberOfJudge * numberOfStates;
    int* judgeTables = (int *)malloc(sizeTableJudge);
    /// @todo add  check

    memset(judgeTables,sizeTableJudge,0);


    /// later return to map for each
    for(int * iterator = mapGetFirst(eurovision->judge);iterator;iterator =mapGetNext(eurovision->judge)){
        JudgeData judge = (JudgeData) mapGet(eurovision->judge,iterator);
        int voteTranslation[10] = {12,10,8,7,6,5,4,3,2,1};
        int currentVote=0;
        for(int  id = 0;id <MAX_VOTE_SIZE;id++)
        {

            int stateId=judge->judgeResults[id];
            judgeTables[*iterator * numberOfStates + stateId] = voteTranslation[currentVote];
            currentVote++;
        }
    }
    printf("=======================Judge Table \n");
    for(int i=0;i<numberOfJudge ;i++)
    {
        for(int j=0;j<numberOfStates ;j++)
        {
            printf(" %d ",judgeTables[i*numberOfStates+ j]);
        }
        printf("\n");
    }

    float* stateTotalVotejudge = (float *)malloc(sizeof(float)*numberOfStates );
    sumStateVotes(numberOfJudge,numberOfStates , judgeTables ,stateTotalVotejudge );

    /// join two table data

    ///  stateTotalVotejudge

    ///  stateTotalVote
    float* finalTally  = (float *)malloc(sizeof(float)*numberOfStates );
    memset(finalTally,sizeof(float)*numberOfStates ,0);

    float audPre =  audiencePrecent;
    float compAudpre = 100 - audPre;
    for(int i=0;i<numberOfStates ;i++)
    {
        //finalTally[i]= (float)((int) ((stateTotalVote [i] *audPre +stateTotalVotejudge[i]*compAudpre)*100.f))/100.f;
        finalTally[i]= (stateTotalVote [i] *audPre + stateTotalVotejudge[i]*compAudpre);
    }
    printf("\n=======================final tally %d   \n",audiencePrecent);
    float sum=0;
    for(int i=0;i<numberOfStates ;i++)
    {
        StateData st= (StateData)mapGet(eurovision->state,&i);
            printf("state : %s  %.6f  \n",finalTally[ i],st->stateName);
            sum+=finalTally[ i];
     }
     printf("\n%f \n",sum);


     /// create fucking list
     ///
     ///
     ///
     Resulttmp* sortTempArray  = (Resulttmp *) malloc(sizeof(Resulttmp)*numberOfStates );
     float maxelem;
     int index;


     for(int i=0;i<numberOfStates ;i++)
     {
         findMaxfloat(finalTally,numberOfStates,&index,&maxelem);

         ///so ugly :)
         if(i==0)
         {

         sortTempArray[i].index=index;

         sortTempArray[i].score=maxelem;

         StateData state = (StateData) mapGet(eurovision->state,&index);
         sortTempArray[i].name=stringCopy(state->stateName);
         }
         else
         {
             printf("addition %f \n",maxelem);

             if(sortTempArray[i-1].score == maxelem)// same scroe
             {
                 if(sortTempArray[i-1],index>index)
                 {
                     // swap
                     Resulttmp tmp;
                     tmp.index = sortTempArray[i-1].index;
                     tmp.name = sortTempArray[i-1].name;
                     tmp.score = sortTempArray[i-1].score;

                     sortTempArray[i-1].index=index;
                     sortTempArray[i-1].score=maxelem;

                     StateData state = (StateData) mapGet(eurovision->state,&index);
                     sortTempArray[i-1].name=stringCopy(state->stateName);

                     sortTempArray[i-1].index=tmp.index;
                     sortTempArray[i-1].score=tmp.score ;
                     sortTempArray[i-1].name=tmp.name;


                 }
                 else
                 {
                     sortTempArray[i].index=index;
                     sortTempArray[i].score=maxelem;

                     StateData state = (StateData) mapGet(eurovision->state,&index);
                     sortTempArray[i].name=stringCopy(state->stateName);
                     //regularcase
                 }

             }
             else
             {

                 sortTempArray[i].index=index;

                 sortTempArray[i].score=maxelem;

                 StateData state = (StateData) mapGet(eurovision->state,&index);
                 sortTempArray[i].name=stringCopy(state->stateName);
             }


         }
         finalTally[index]=-1;
     }

//     for(int i=0;i<numberOfStates ;i++)
//     {
//         printf("%s %f \n",sortTempArray[i].name ,sortTempArray[i].score);
//     }

    List list = listCreate(copyString, freeString);
//    printf("list size %d %s \n",listGetSize(list),sortTempArray[0].name);
//    ListElement s = (ListElement) sortTempArray[0].name;
//    printf("funcp %p %p \n",copyString,s );
//listInsertFirst(list,s);
//printf("funcp %p %p \n",copyString,s );
//    printf("list sssadds ize %d \n",listGetSize(list));
//    //listInsertFirst(list,s);
//     printf("list size %d \n",listGetSize(list));
//     if (list == NULL) {
//         return NULL;
//    }
     for(int i=0;i<numberOfStates ;i++)
     {
printf("name %d  %s %f\n",i,sortTempArray[i].name,sortTempArray[i].score);
          ListElement s = (ListElement) sortTempArray[i].name;
        listInsertLast(list,s);
     }

//         printf("%d \n",listGetSize(list));

         //listInsertFirst(list,sortTempArray[i].name);
//          ListElement elem1 = listGetCurrent(list);
//          printf("%p \n",elem1);
//         printf("%d \n",listGetSize(list));
//         ListElement elem =  listGetFirst(list);

//         printf("%s %f \n",sortTempArray[i].name ,sortTempArray[i].score);
//         ListResult result = listInsertLast(list,sortTempArray[i].name );
//         if (result != LIST_SUCCESS) {
//              printf("Error list\n" );
//             return NULL;
//         }
//        }

//     ListResult result = listInsertFirst(list, );
//     if (result == LIST_OUT_OF_MEMORY) {
//         return NULL;
//     }
//      listGetSize(list)
//              ListResult result = listInsertAfterCurrent(list, iterator);
//              if (result == LIST_OUT_OF_MEMORY) {
//                  return NULL;
//              }


                                                      //this is not the first element. we need to find its place in list


printf("exit contest  \n");

    return list;
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
    LOG5
            for (keyElement=mapGetNext(eurovision->state); keyElement!= NULL;keyElement=mapGetNext(eurovision->state)){

        LOG5
                result = mapPut(winnersMap, keyElement,&initData); //we set rest of the states
        if (result == MAP_OUT_OF_MEMORY){
            return NULL;
        }
    }
    LOG5
            MAP_FOREACH(int *,iterator,eurovision->state){
        StateData stateData = mapGet(eurovision->state,iterator);
        if(stateData == NULL){
            return NULL;
        }
        LOG5
                Map citizenVote = stateData->citizenVote;
        List current_list = NULL;
        current_list = makeSortedListByMapData(citizenVote);
        LOG5
                if (current_list == NULL){
            return NULL;
        }
        LOG5
                int list_size = listGetSize(current_list);
        LOG5
                if (list_size == -1){
            continue;
        }
        LOG5
                if (list_size < MAX_NUMBER_OF_WINNERS){
            number_of_voted_states = list_size;
        }
        ListElement current_state = NULL;
        int i = 0;
        LOG5
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
    LOG5
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
    LOG5
            List winnersList = makeSortedListByMapData(winnersMap);
    if (winnersList == NULL){
        return NULL;
    }
    LOG5
            printResult(winnersList);
}

List makeSortedListByMapData(Map map)
{
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
        printf( "%s\n", next_winner);
    }
    next_winner = listGetNext(list);
    while (next_winner != NULL) {
        printf( "%s\n", next_winner);
        next_winner = listGetNext(list);
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
    Map friendlyMap = mapCreate(copyVoteDataElement, copyKeyElement, freeVoteDataElement, freeKeyElement,
                                compareKeyElements);      // we count the score here
    if (friendlyMap == NULL) {
        return NULL;
    }
    int initData = 0;
    KeyElement keyElement = mapGetFirst(eurovision->state);
    MapResult result = mapPut(friendlyMap, keyElement, &initData); //we set first state
    if (result == MAP_OUT_OF_MEMORY) {
        return NULL;
    }
    while (mapGetNext(eurovision->state) != NULL) {
        keyElement = mapGetFirst(eurovision->state);
        result = mapPut(friendlyMap, keyElement, &initData); //we set rest of the states
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
        result = mapPut(friendlyMap, iterator, listGetFirst(current_list));
        if (result == MAP_OUT_OF_MEMORY) {
            return NULL;
        }
    }
    List friendlyList = listCreate(copyFriendly, freeFriendly);
    if (friendlyList == NULL) {
        return NULL;
    }
    MAP_FOREACH(int *, iterator, friendlyMap) {
        if (mapGet(friendlyMap, mapGet(friendlyMap, iterator)) == iterator) {  //the countries voted for each other
            StateData stateData1 = mapGet(eurovision->state, iterator);
            StateData stateData2 = mapGet(eurovision->state, mapGet(friendlyMap, iterator));
            char *friendlyStr = createStr(stateData1->stateName, stateData2->stateName);
            listInsertFirst(friendlyList, friendlyStr);
        }
    }
    ListResult listResult = listSort(friendlyList, compareFriendly);
    if (listResult == LIST_OUT_OF_MEMORY || listResult == LIST_NULL_ARGUMENT){
        return NULL;
    }
}

char* createStr(char* str1, char* str2) {
    if (strcmp(str1, str2) > 0){
        char* str_to_return = malloc(sizeof(str1) + sizeof(str2) + 1 + 1);
        if (str_to_return == NULL)
            return NULL;
        return str_to_return;
    }
}

ListElement copyFriendly(ListElement str){
    return stringCopy(str);
}
void freeFriendly(ListElement str) {
    free(str);
}
int compareFriendly(ListElement str1, ListElement str2){
    if (strcmp(str1, str2) == 0){
        return 0;
    }
    return ((strcmp(str1, str2) > 0) ? -1 : 1 );
}




