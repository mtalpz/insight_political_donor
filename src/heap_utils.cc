/*****************************************************
Created Oct 2017: Find Political Donors 
This file includes functions related to inserting/balancing new value in
min and max heap, which is eventually used to calculate median value of 
all values.

***********************************************/

#include <heap_utils.h>
#include <misc_utils.h>

using namespace std;

extern fstream log_file;
extern int log_level;

/************************************************************
   Function to obtain the median for multiple records
   returns rounded result as requested
************************************************************/

int get_median( deque<float> *max_heap, deque<float> *min_heap )
{
	stringstream msg;
	int median;

	if ( (*max_heap).size() > (*min_heap).size() )
		median = (int) ( (*max_heap)[0] + 0.5 );
	else if ( (*max_heap).size() < (*min_heap).size() )
		median = (int) ( (*min_heap)[0] + 0.5 );
	else {
		median = (int) (( (*max_heap)[0] + (*min_heap)[0] ) /2 ) +0.5;
	}

	msg << "Returning median = " << median;
    logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), DEBUG);
    msg.str("");

    return median;

}
		
/************************************************************
   Function to maintain heap property after insertion
	heap is min or max based on bool provided
************************************************************/
void heapify (deque<float> *heap, bool max)
{
    int parent = 0;
    stringstream msg;

    msg << "Started heapify with max = " << (max ? "TRUE" : "FALSE");
    logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), DEBUG);
    msg.str("");

    while (parent < heap->size()) {
        int key = parent;
        int left = 2*parent + 1;
        int right = 2*parent + 2;

        // check the left child
        if (left < heap->size()) {
            if ((max == true && (*heap)[left] > (*heap)[key]) ||
                (max == false && (*heap)[left] < (*heap)[key])) {
                key = left;
            }
        }

        // check the right child
        if (right < heap->size()) {
            if ((max == true && (*heap)[right] > (*heap)[key]) ||
                (max == false && (*heap)[right] < (*heap)[key])) {
                key = right;
            }
        }

        // check if we need to swap and continue below
        if (key != parent) {
            float num = (*heap)[parent];
            (*heap)[parent] = (*heap)[key];
            (*heap)[key] = num;
            parent = key;
        } else {
            break;
        }
    }
   
    msg << "Finished heapify with max = " << (max ? "TRUE" : "FALSE");
    logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), DEBUG);
    msg.str("");
    return;
}


void balance_max_heap( deque<float> *max_heap)
{
	int child_index  = max_heap->size() - 1;
	int parent_index = (child_index-1)/2;
	while ( child_index > 0) {
		if ( (*max_heap)[parent_index] < (*max_heap)[child_index] ) {
			float tmp = (*max_heap)[child_index];
			(*max_heap)[child_index] = (*max_heap)[parent_index];
			(*max_heap)[parent_index] = tmp;
		}
		child_index = parent_index;
		parent_index = (child_index-1)/2;
	}
	return;
}


void balance_min_heap( deque<float> *min_heap )
{
	int child_index  = min_heap->size() - 1;
	int parent_index = (child_index-1)/2;
	while ( child_index > 0) {
		cout<<"IN balance min heap"<<endl;
		if ( (*min_heap)[parent_index] > (*min_heap)[child_index] ) {
			float tmp = (*min_heap)[child_index];
			(*min_heap)[child_index] = (*min_heap)[parent_index];
				(*min_heap)[parent_index] = tmp;
		}
		child_index = parent_index;
		parent_index = (child_index-1)/2;
	}
	return;
}

/************************************************************
	Function to insert running transaction amount into either
	min or max heap based on bool and then maintian heap property.
************************************************************/

void insert_heap ( deque<float> *heap, float num, bool max )
{
    stringstream msg;

    msg << "Started heap insertion with max = " << (max ? "TRUE" : "FALSE");
    logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), DEBUG);
    msg.str("");

    heap->push_back(num);
    int child = heap->size() - 1;
    int parent = (child - 1) / 2;
    while (child > 0) {
        if ((max == true && (*heap)[parent] < (*heap)[child]) ||
            (max == false && (*heap)[parent] > (*heap)[child])) {
            int tmp = (*heap)[child];
            (*heap)[child] = (*heap)[parent];
            (*heap)[parent] = tmp;
            child = parent;
            parent = (child - 1) / 2;
        } else {
            break;
        }
    }

    msg << "Finished heap insertion with max = " << (max ? "TRUE" : "FALSE");
    logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), DEBUG);
    msg.str("");
    return;
}

//For debug
void print_heaps (deque<float> *max_heap, deque<float> *min_heap)
{
    cout << "max_heap[" << max_heap->size() << "]: ";
    for (int i = 0; i < max_heap->size(); i++)
        cout << (*max_heap)[i] << ", ";
    cout << endl;
    cout << "min_heap[" << min_heap->size() << "]: ";
    for (int i = 0; i < min_heap->size(); i++)
        cout << (*min_heap)[i] << ", ";
    cout << endl;
}
