##insight_political_donor

*To run the case, please use ./run.sh*
( The first time we run using ./run.sh, additional folders - "output","src/bin", "src/obj", "src/logs" will be created. On following runs, these folders already exist, so prompt will be displayed accordingly. output is overwritten )


The directory structure is as follows:







**APPROACH:**

The entire functionality to print the two outout files has been split into primarily 3 source files and 3 header files.

1- **find_political_donors.cc**
  It is the top level file which parses the input txt file and then populates record structure for evaluating median values by using zip and median values by using date. The output for medianvals_by_zip.txt is written as we parse each line as here, there can be duplicates and the median and total are the running median and total and printed accordingly for duplicate cases. The output for medianvals_by_date.txt is written after the entire input file is parsed as we need all the records pertaining to a particular recipient for a particular date and then provide just a unique output record with requested details.
  
  
2- **misc_utils.cc**
  This file is used for all helper functions required by functions in other files. It includes functions to get log information for error, warning, debug as required. It also has the function to validate the date provided. The function written to tokenize each field is also included. It provides the pipe separates tokens as specified in the dictionary. If the fields are empty with consecutive pipe separation, we accept that as an empty string field. The function returns NULL when entire string is read.

3- **heap_utils.cc**
  This file is used for all function related to heap - insertion, balance. In order to evaluate the median for running amounts obtained from the FEC, the idea is to use to binary heaps- MIN_HEAP and MAX_HEAP. At every time, both the heaps are sorted and the difference in size in the heaps is atmost 1. When evaluating median, if the two heaps as equal in size, the median value is the sum of value at root od MAX_HEAP and value at root of MIN_HEAP divided by two. If they are not of same size, the median is evaluated as the value at the root of the heap which is bigger in size. Since the algorith for most function to insert and balance heaps is similar and only the comparator changes, we have used only one function for both heaps. A bool value is provided as argument to the function which can switch the flow depending on using it for MIN_HEAP or MAX_HEAP.
  
All src files have corresponding header files located at src/include/* . The function declarations are provided in header files.


  
