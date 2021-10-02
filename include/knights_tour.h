#ifndef __KNIGHTS_TOUR_H__
#define __KNIGHTS_TOUR_H__

#include <stddef.h>  /* size_t */

#define SIZE 8

void FindKnightsPath(unsigned char arr[SIZE * SIZE], int first);
void FindKnightsHeuristic(unsigned char arr[SIZE *SIZE], int first);


#endif /* __KNIGHTS_TOUR_H__ */
