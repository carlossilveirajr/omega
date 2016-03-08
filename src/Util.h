/**
*  This is function for future purpose
**/
#ifndef UtilH
#define UtilH

#include <string.h>
/*
* Read the integer values of a list in this format {1,32,3}
*/
char * getNextIndex (char *& str);

/*
* Transfer the integer values of a list in this format {1,32,3}
* to the vector vet
* @param str input list
* @param vet vector returned with the integers of the input list
*/
void transfer(char * str, int * vet);

/**
*  Function used to test and show the optimized work of other funcions
**/
void testCaseTransfer();

//---------------------------------------------------------------------------
#endif
