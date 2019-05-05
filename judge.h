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

/** Type for defining the judgeData struct */
typedef struct {
    char* judgeName;
    int* judgeResults;
} *JudgeData;

/** Element judge data map type  */
typedef void *JudgeDataMap;


/**
* Removes the judge data element of the judge map
**/
void freeJudgeDataElement(JudgeDataMap judgeDataToFree);


/**
* Copies judge data element
*
 * returns
* NULL if there is an allocation problem
* pointer to the data element otherwise
**/
JudgeDataMap copyJudgeDataElement(JudgeDataMap judgeDataToCopy);


/**
* function to use from eurovision
**/
char *stringCopy(const char *str);

#endif //EUROVISIONV4_JUDGE_H
