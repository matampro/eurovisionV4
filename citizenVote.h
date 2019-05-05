//
// Created by DONTO on 5/5/2019.
//

#ifndef EUROVISIONV4_CITIZENVOTE_H
#define EUROVISIONV4_CITIZENVOTE_H

#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

/**
*   copyVoteDataElement      - Copies vote data element
*   freeVoteDataElement      - frees an existing vote data element
 **/


/** Data element data type for map citizenVote */
typedef void *VoteDataElement;


VoteDataElement copyVoteDataElement(VoteDataElement voteToCopy);


void freeVoteDataElement(VoteDataElement voteToFree);

#endif //EUROVISIONV4_CITIZENVOTE_H
