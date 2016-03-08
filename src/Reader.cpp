#include "Reader.h"

/**
*  This class is reponsible for scanning the database.
*  @param str pointer to current line reader position
*  @return current word and places the pointer (str) to next word
**/
char * getNextWord(char * &str){
   char * begin = str;
   while(*str!=' ' && *str!='\n' && *str!='\0')
      str++;
   *str='\0';
   str++;
   while(*str==' '||*str=='\n')
      str++;
   return begin;
}

/**
 * Constructor, it opens the file
 * @param filename
**/
Reader::Reader(char * filename){
        ctind = 1; //second atributed is set as categorical attribute by default
	f = fopen(filename,"r");
	char * ptr;
	if(f==NULL){
		printf("\nError in file open.");
		printf("\nPress any key to continue...");
//		getch();
		exit(0);
	}
        //conting the number of attributes
        firstLine();
        nbrattr=-2;
        ptr = line;
        while(*ptr != '\0'){
              nbrattr++;
              getNextWord(ptr);
        }
        //conting the number of lines
        firstLine();
        nbrlines=1;
        while(nextLine()!=-1)
             nbrlines++;

}

/**
* Move the current file reader to next transaction.
* @return  retunrs -1 if next line doesn't exist.
**/
int Reader::nextLine(){
	if(fgets(line,LINETAM,f)==NULL)
		return -1;
       	return 1;
}

/**
* Move the current file reader to the first transaction.
**/
void Reader::firstLine(){
	fseek(f,0,0);
        nextLine();
}


int Reader::getIDValue(){
	char * ptr = line;
	int id=-1;
        id = atoi( getNextWord(ptr) );
	return id;
}

/**
* Gets the categorical attribute value of the current transaction.
* Considers line format id(pos=0) cat(pos=1) at1 (pos=2) at2 (pos=3) at3(pos=4) ... atn(pos=n-1)
* @return returns the categorical value as an integer or -1 if it doesn´t exist.
*/
int Reader::getCatValue(){
	char * ptr = line;
	int cat=-1;
        for(int i =0; i<ctind;i++)
	   getNextWord(ptr);
	cat = atoi( getNextWord(ptr) );
	return cat;
}

/**
* Gets the quantitative attribute value of the current transaction.
* Returns -1 if it doesn´t exist.
*/
float * Reader::getQuanValue(){
        float * qt = new float[nbrattr];
	char * ptr = line;
	getNextWord(ptr); //id
        getNextWord(ptr); //class
        int i = 0;
    	for(i =0; i<nbrattr;i++)
	   qt[i] = atof( getNextWord(ptr) );
        return qt;
}

/*
* Gets the ith value of qt. Return -1 if it does not exists.
*/
float Reader::getValueOfIndex(float * qt, int index){
       if(index<nbrattr)
	  return(qt[index]);
       else
          return -1;
}

/**
* Prints the values of categorical and quantitative atributes of the current
* transaction.
**/
void Reader::show(){
	//printf("\nLine:[%s]",line);
        //printf("\nComputed");
        printf("\ncat[%d]=",getCatValue());
        float * qt = new float[nbrattr];
        qt=getQuanValue();
        for(int i=0; i < nbrattr; i++)
            printf("%3f;",*(qt+i));
}

/**
* Gets the number of attributes or features
* @returns the number of features or atributes of the file
**/
int Reader::getNbrAttribs(){
    return nbrattr;
}

/**
* Gets the number of lines of the database
* @returns the number of lines of the database
**/
int Reader::getNbrLines(){
    return nbrlines;
}
