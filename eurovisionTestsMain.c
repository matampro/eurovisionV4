#include <stdio.h>
#include <stdlib.h>
#include "eurovisionTests.h"

#define TEST(t)                                                                \
  do {                                                                         \
    printf("Testing %s ...\n", #t);                                            \
    if ((t()))                                                                 \
      printf("Test %s: SUCCESS\n", #t);                                        \
    else                                                                       \
      printf("Test %s: FAIL\n", #t);                                           \
  } while (0);

int main(int argc, char *argv[]) {
  TEST(testAddState)
    //printf("after %d\n",__LINE__);
   TEST(testRemoveState)
      //      printf("after %d\n",__LINE__);
    TEST(testAddJudge)
        //    printf("after %d\n",__LINE__);
    TEST(testRemoveJudge)
//            printf("after %d\n",__LINE__);
    TEST(testAddVote)
//            printf("after %d\n",__LINE__);
  TEST(testRemoveVote)
//            printf("after %d\n",__LINE__);
   TEST(testRunContest)
//            printf("after %d\n",__LINE__);

   TEST(testRunAudienceFavorite)
//            printf("after %d\n",__LINE__);
  TEST(testRunGetFriendlyStates)
//            printf("after %d\n",__LINE__);
    return 0;
}
