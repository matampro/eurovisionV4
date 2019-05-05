//
// Created by DONTO on 5/5/2019.
//

#ifndef EUROVISIONV4_JUDGE_H
#define EUROVISIONV4_JUDGE_H

#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "list.h"

#define NUMBER_OF_RESULTS 10

typedef struct {
    char* judgeName;
    int* judgeResults;
} *JudgeData;

typedef void *JudgeDataMap;


void freeJudgeDataElement(JudgeDataMap judgeDataToFree);


JudgeDataMap copyJudgeDataElement(JudgeDataMap judgeDataToCopy);


char *stringCopy(const char *str);

#endif //EUROVISIONV4_JUDGE_H
