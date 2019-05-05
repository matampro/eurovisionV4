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

/**
* Copies vote data element
*
* @return
* NULL if there is an allocation problem
* pointer to the data element otherwise
**/
VoteDataElement copyVoteDataElement(VoteDataElement voteToCopy);

/**
* Removes the data element of the map
**/
void freeVoteDataElement(VoteDataElement voteToFree);

#endif //EUROVISIONV4_CITIZENVOTE_H
