/*****************************************************
Created Oct 2017: Find Political Donors 
***********************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>

using namespace std;

/***************************************************************************
 All Functions:
***************************************************************************/
//Find median of all the contributions, median computed using 2 heaps
int get_median( deque<float>* max_heap, deque<float>* min_heap );

//Function to maintain heap property for a given queue
void heapify(deque<float> *heap, int index);
void heapify_min (deque<float> *heap, int min);

//Function to insert running transaction amount into either heap and then build heap if needed
void insert_heap ( deque<float> *max_heap, deque<float> *min_heap, float amount );
