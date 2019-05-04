#include <stdio.h>
#include "map.h"
#include <stdlib.h>
#include <memory.h>

#include "eurovision.h"

typedef struct node_t{
    MapDataElement mapDataElement;
    MapKeyElement mapKeyElement;
    struct node_t* next;
}*Node;

struct Map_t{
        int counter;
        Node head;
        Node tail;
        copyMapDataElements data_copy;
        copyMapKeyElements key_copy;
        compareMapKeyElements compair_key;
        freeMapDataElements free_data;
        freeMapKeyElements free_key;
};

Map mapCreate(copyMapDataElements copyDataElement,copyMapKeyElements copyKeyElement,
                freeMapDataElements freeDataElement,freeMapKeyElements freeKeyElement,
                compareMapKeyElements compareKeyElements){
    if((copyDataElement == NULL) || (copyKeyElement == NULL)|| (freeDataElement == NULL) || (freeKeyElement== NULL)
                                                                                   || (compareKeyElements == NULL)){
        return NULL;
    }
    Map map = malloc(sizeof(*map));
    if (map == NULL){
        return NULL;
    }
    map->data_copy = copyDataElement;
    map->compair_key = compareKeyElements;
    map->free_data = freeDataElement;
    map->key_copy = copyKeyElement;
    map->free_key = freeKeyElement;
    map->counter = 0;
    map->head = NULL;
    map->tail = NULL;

    return map;
}
void mapPrint(Map map) {

   printf("PRINTING ...%d \n\n",map->counter);
    if(map->head != NULL) {
         Node curr = map->head;
           MapKeyElement test;
        while (curr->next != NULL) {
            Node tmp = curr->next;
          test= curr->mapKeyElement;

            StateData newStateData = (StateData)curr->mapDataElement;
            printf(" addr head %p elem %d  data  %p \n",curr ,*(int*)test,newStateData->citizenVote );
            curr = tmp;
        }
         test= curr->mapKeyElement;
        printf(" addr head %p elem %d  data  %s \n",curr ,*(int*)test,curr->mapDataElement );
    }

}
void mapDestroy(Map map) {
    LOG
   if(map!=NULL){ //protections
        LOG
    if(map->head != NULL) {
        printf("Destroying  ...%d \n\n",map->counter);
        MapKeyElement test;
        test=  map->head->mapKeyElement;

        while (map->head->next != NULL) {
            Node tmp = map->head->next;
            test=  map->head->mapKeyElement;



            map->free_key(map->head->mapKeyElement);

            map->free_data(map->head->mapDataElement);
            free(map->head);
            map->head = tmp;
        }
LOG

        if(map->head->mapDataElement!=NULL)
            map->free_data(map->head->mapDataElement);          ///free last node
        if(map->head->mapKeyElement!=NULL)
            map->free_key(map->head->mapKeyElement);
        free(map->head);
    }
    LOG
    free(map);
    }
    LOG
}

Map mapCopy(Map map){
    LOGC

    if(map == NULL){
        return NULL;
    }
    if(map == NULL){
        return NULL;
    }
    LOGC
    Map new_map = malloc(sizeof(*map));
    if (new_map == NULL){
        return NULL;
    }
    LOGC
//  *new_map = *map;
//    new_map->head = malloc(sizeof(*new_map->head ));
//    if (new_map->head == NULL){
//        mapDestroy(new_map);
//        return NULL;
//    }

            /// must copy fuctions as well
       new_map->compair_key = map->compair_key;
        new_map->data_copy= map->data_copy;
        new_map->counter=map->counter;
        new_map->free_data=map->free_data;
        new_map->free_key=map->free_key;
        new_map->key_copy=map->key_copy;

    new_map->head = NULL;
    LOGC
    new_map->tail = new_map->head;
    Node iter;
iter = map->head;
if(map->head == NULL)
{
    LOGC
    return new_map;
}
    LOGC
//    new_map->head->mapKeyElement = new_map->head->mapDataElement = NULL;
    LOGC
    //// error with tail
    /// new_map->tail = new_map->head->next
    /// new_map->head = malloc(sizeof(*new_map->head ));
    /// new_map-> head is created but the next is never assinget to anthing
    /// new_map->head->next is garbage
    for (;
          iter->next != NULL
        ; iter = iter->next){
        LOGC
        Node next_node = malloc(sizeof(*next_node));
        if (next_node == NULL){
            mapDestroy(new_map);
            return NULL;
        }
        LOGC
        new_map->tail = next_node;
        new_map->tail->mapKeyElement = map->key_copy(iter->mapKeyElement);
        new_map->tail->mapDataElement = map->data_copy(iter->mapDataElement);
        new_map->tail->next = NULL;
    }
    LOGC
    Node next_node = malloc(sizeof(*next_node)); //*  copy last node
    if (next_node == NULL){
        mapDestroy(new_map);
        return NULL;
    }
    LOGC
    new_map->tail = next_node;
    new_map->tail->mapKeyElement = map->key_copy(iter->mapKeyElement);
    new_map->tail->mapDataElement = map->data_copy(iter->mapDataElement);
    new_map->tail->next = NULL;
    LOGC
    return new_map;
}

int mapGetSize(Map map){
    if(map == NULL){
        return -1;
    }
    return map->counter;
}

bool mapContains(Map map, MapKeyElement element){
    LOGJ
    if((map == NULL) || (element == NULL) || (map->head == NULL)){  // check also that map->head is not NULL
        return false;
    }
    LOGJ
    Node iter;
    iter = map->head;                                          // iter needs to start from head
    LOGJ
    while (iter->next != NULL){
        if(map->compair_key(iter->mapKeyElement , element)== 0 ){ /// you dont know the tyme of key element so == makes no sence
           LOGJ
            return true;
        }
        iter = iter->next;
    }
    LOGJ
     if(map->compair_key(iter->mapKeyElement , element)== 0 ){    //check last node
        return true;
    }
    LOGJ
        return false;
}



MapResult createNewNode(Node *new_node,MapKeyElement keyElement, MapDataElement dataElement,Map map){
 LOGJ
    if((new_node == NULL) || (keyElement == NULL) || (dataElement == NULL) ||  (map == NULL)){
        return MAP_OUT_OF_MEMORY;
    }
    *new_node = malloc(sizeof(struct node_t));
    if (*new_node == NULL) {
        return MAP_OUT_OF_MEMORY;
    }else{
         LOG4
//         printf("function copy data %p  %d\n",map->data_copy,*(int*)dataElement);
        (*new_node)->mapDataElement = map->data_copy (dataElement);
          LOG4
        (*new_node)->mapKeyElement = map->key_copy(keyElement);
           LOG4
        (*new_node)->next = NULL;
    }
    return MAP_SUCCESS;
}
void addNewNodeAfterNode(Node new_node ,Node previousNode) {
    Node temp = previousNode->next;
    previousNode->next =new_node;
    new_node->next =  temp;
}
MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement) {
//printf("has function %p\n", map->data_copy);
    Node iter;
    printf("put %d in %d\n", *(int *)dataElement,*(int *)keyElement);

    if((map == NULL) || (keyElement == NULL)|| (dataElement == NULL)){
        return MAP_OUT_OF_MEMORY;
    }
//    if (mapContains(map, keyElement)){
//        return MAP_ITEM_ALREADY_EXISTS;
//    }

    Node new_node=NULL;
    if(map->head == NULL){                    //first node in the list
        LOG4
        if (createNewNode(&new_node, keyElement, dataElement,map) == MAP_OUT_OF_MEMORY) {
            return MAP_OUT_OF_MEMORY;
        } else {
            LOG4
            map->counter++;
            map->head=new_node;
            iter=map->head;
            return MAP_SUCCESS;
        }
    }
 printf("traverse \n");
    Node prevNode=map->head;
    for (iter = map->head; iter != NULL ; iter = iter->next) {
        LOG4
        if (map->compair_key(iter->mapKeyElement, keyElement) == 0) {//swap data
            printf("swap \n");
            iter->mapDataElement = map->data_copy(dataElement);
            iter->mapKeyElement = map->key_copy(keyElement);
            return MAP_SUCCESS;
        }
        if (map->compair_key(keyElement ,iter->mapKeyElement ) < 0) {//middle before this object
            LOG4
            if (createNewNode(&new_node, keyElement, dataElement,map) == MAP_OUT_OF_MEMORY) {
                return MAP_OUT_OF_MEMORY;
            } else {
                map->counter++;
                addNewNodeAfterNode(new_node,prevNode); //in the middle
                return MAP_SUCCESS;
            }
        }
        prevNode = iter;
      }
     printf("last node  \n");
        iter=prevNode;
        LOG4
        if (map->compair_key( keyElement,iter->mapKeyElement) > 0){
            LOG4
            if (createNewNode(&new_node, keyElement, dataElement,map) == MAP_OUT_OF_MEMORY) { /// end of list
                return MAP_OUT_OF_MEMORY;
            } else {
                map->counter++;
                addNewNodeAfterNode(new_node, iter);
                return MAP_SUCCESS;
            }
        }
        LOG4
        return MAP_OUT_OF_MEMORY;  //should not get here
}

MapDataElement mapGet(Map map, MapKeyElement keyElement){
    if((map == NULL) || (keyElement == NULL)){
        return NULL;
    }
    /// again you cant use several function with tail !
    Node iter;
    for (iter = map->head; iter != NULL ; iter = iter->next) {
        if (map->compair_key(iter->mapKeyElement, keyElement) == 0){
            return iter->mapDataElement;
        }
    }
    return NULL;
}

MapResult mapRemove(Map map, MapKeyElement keyElement) {
    if((map == NULL) || (keyElement == NULL)){
        return MAP_NULL_ARGUMENT;
    }
    LOGJ
    Node iter;
    ///protection againt empty case
    if(map->head ==NULL){
        LOGJ
        return MAP_SUCCESS;
    }
    int flag = 0; // We check if the item is found//
    Node prev = map->head;
    for (iter = map->head; iter->next != NULL ; iter = iter->next) {
        LOGJ
        if (map->compair_key(iter->mapKeyElement, keyElement) == 0) {
            flag = 1;

            Node tmp = iter->next;
            if( map->head == iter)
            {
               map->head = tmp;
            }
            if(iter==map->tail) // protecteion from losing tail locaiton
                map->tail =tmp;
            prev->next = tmp;
            map->free_data(iter->mapDataElement);
            map->free_key(iter->mapKeyElement);
            free(iter);
            iter = tmp;

            map->counter--;
            break;
        }
    }
    if (map->compair_key(iter->mapKeyElement, keyElement) == 0) {
        flag = 1;
        Node tmp = iter->next;
        if(iter==map->tail) // protecteion from losing tail locaiton
            map->tail =tmp;
        map->free_data(iter->mapDataElement);
        map->free_key(iter->mapKeyElement);
        free(iter);
        iter = tmp;
        map->counter--;

    }

    if(flag == 1){
        LOGJ
        return MAP_SUCCESS;   //  how can we know that it went well?
    }else{
        LOGJ
        return MAP_ITEM_DOES_NOT_EXIST;
    }

}

MapKeyElement mapGetFirst(Map map) {
    if(map == NULL){
        return NULL;
    }
    if (map->head == NULL){
        return NULL;
    }
    map->tail = map->head;
    /// two function are colliding here map get next
    ///points to next and mapGetFirst return to status
    return  map->tail->mapKeyElement;
}

MapKeyElement mapGetNext(Map map){
    if(map == NULL){
        return NULL;
    }
    map->tail = map->tail->next;
    if(map->tail !=NULL)
        return map->tail->mapKeyElement;
    else
        return NULL;
}

MapResult mapClear(Map map){
    if(map == NULL){
        return MAP_NULL_ARGUMENT;
    }
    while(map->head != NULL)
    {
        Node tmp = map->head;
        map->head = map->head->next;
        map->free_data(tmp->mapDataElement);
        map->free_key(tmp->mapKeyElement);
    }
    map->free_data(map->head->mapDataElement);
    map->free_key(map->head->mapKeyElement);

    return MAP_SUCCESS;
}
