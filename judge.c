//
// Created by DONTO on 5/5/2019.
//
#include "judge.h"

JudgeDataMap copyJudgeDataElement(JudgeDataMap judgeDataToCopy) {
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

void freeJudgeDataElement(JudgeDataMap judgeDataToFree) {
    JudgeData ptr = (JudgeData) judgeDataToFree;
    free(ptr->judgeName);
    free(ptr->judgeResults);
    free(ptr);
}
