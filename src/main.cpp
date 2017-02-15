/**
 *  Generate a precision/recall graphic script
 *  argv[1] = input filename
 *  argv[2] = minimum bin size
 *  argv[3] = maximum inconsistence rate to fuse two consective intervals
 *  argv[4] = number of categories
 *  argv[5] = maximum inconsistence rate per attribute to the same be selected
 *  example:
 *       discretizer input.txt 2 0.2 2 0.2
 */

#include "Reader.h"
#include "Discret.h"
#include "Reader.cpp"
#include "Discret.cpp"

int main(int argc, char *argv[]) {
	Reader reader(argv[1]);

	int binsize = atoi(argv[2]);
	float incratetofuse = atof(argv[3]);
	int nbrcats = atoi(argv[4]);
	float incratetoselect = atof(argv[5]);

	printf("\nChecking if the database is read correctly");
	discretize(&reader, binsize, incratetofuse, nbrcats, incratetoselect);
	printf("\nProgram ended, press any key to continue...");

	reader.close();

	return 0;
}

