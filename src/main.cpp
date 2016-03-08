//---------------------------------------------------------------------------

#pragma hdrstop

//---------------------------------------------------------------------------
/*
*  Generate a precision/recall graphic script
*  argv[1] = input filename
*  argv[2] = minimum bin size
*  argv[3] = maximum inconsistence rate to fuse two consective intervals
*  argv[4] = number of categories
*  argv[5] = maximum inconsistence rate per attribute to the same be selected
*  example:
*       discretizer input.txt 2 0.2 2 0.2
*  ps: the output file is default presage_log.txt ***@TO DO, CHANGE THIS**
*  id classid q1 q2 q3 ... qn
*  ***the categories should be integer consecutives starting by zero***CAUTION
*  fist atribute index = 1
**/

#include <string.h>
#include "Reader.h"
#include "Discret.h"
#include "Reader.cpp"
#include "Discret.cpp"

/*THE INDEX OF THE FIRST ATTRIBUTE IS ONE*/


int main(int argc, char * argv[]){
       char * filename = argv[1];
       int binsize= atoi(argv[2]);
       float incratetofuse =  atof(argv[3]);
       int nbrcats =  atoi(argv[4]);
       float incratetoselect =  atof(argv[5]);
       Reader * reader = new Reader(filename);
      	//See if database was read correctly
       printf("\nChecking if the database is read correctly");
      /* reader->firstLine();
       do{
       	reader->show();
       }while(reader->nextLine()!=-1);*/

       discretize(reader,binsize, incratetofuse,nbrcats,incratetoselect);
       printf("\nProgram ended, press any key to continue...");
       reader->closef();
//       getch();
}




















