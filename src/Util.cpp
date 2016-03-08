//---------------------------------------------------------------------------

#pragma hdrstop

#include "Util.h"
#include <stdlib.h>
 #include <stdio.h>
/*
* Read the integer values of a list in this format {1,32,3}
*/
char * getNextIndex (char *& str){
    char * begin = str;
    if(*str=='{')
      begin = ++str;
    while(*str!=',' && *str!='}'&& *str!='\0')
		str++;
	*str='\0';
    str++;
    if(*begin=='\0')
       return NULL;
    return begin;
}

/*
* Transfer the integer values of a list in this format {1,32,3}
* to the vector vet
* @param str input list
* @param vet vector returned with the integers of the input list
*/
void transfer(char * str, int * vet){
     char * nbr;
     int i=0;
     while( (nbr=getNextIndex(str))!=NULL ){
         vet[i++]= atoi(nbr);
     }
}

/**
*  Function used to test and show the optimized work of other funcions
**/
void testCaseTransfer(){
   char * list = "{1,2,34,35}";
   int sizeind = strlen(list)/2;
   int * indexes = new int(sizeind);
   transfer(list,indexes);
   for(int i =0; i < sizeind;i++)
       printf("\n%d",indexes[i]);

}
//---------------------------------------------------------------------------
#pragma package(smart_init)
