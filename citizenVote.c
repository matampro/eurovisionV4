//
// Created by DONTO on 5/5/2019.
//
#include "citizenVote.h"

VoteDataElement copyVoteDataElement(VoteDataElement voteToCopy) {

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

void freeVoteDataElement(VoteDataElement voteToFree) {
    free(voteToFree);
}


