#include<stdio.h>
#include "Discret.h"
#include "Reader.h"
#include<time.h>

/*
 * Sort the set of instances of an attribute, according to its values
 * @param vet vector of instances to be sort
 * @param imin minimum position to be sort
 * @param imax maximum position to be sort
 *
 */
void quicksort(Instance * vet, int low, int high) {
	if (low >= high)
		return;
	int cattemp;
	float pivo = vet[(low + high) / 2].val, lowold = low, highold = high, temp;
	while (low <= high) {
		while (vet[low].val < pivo)
			low++;
		while (vet[high].val > pivo)
			high--;
		if (low <= high) {
			temp = vet[low].val;
			cattemp = vet[low].cat;
			vet[low].val = vet[high].val;
			vet[low].cat = vet[high].cat;
			vet[high].val = temp;
			vet[high].cat = cattemp;
			low++;
			high--;
		}
	}
	if (lowold < high)
		quicksort(vet, lowold, high);
	if (highold > low)
		quicksort(vet, low, highold);
}

/*
 * Rank the vector of attributes
 * @param vet vector of instances to be sort
 * @param imin minimum position to be sort
 * @param imax maximum position to be sort
 *
 */
void rank(Attribute * vet, int low, int high) {
	if (low >= high)
		return;
	int cattemp;
	float pivo = vet[(low + high) / 2].incrate, lowold = low, highold = high,
			temp;
	while (low <= high) {
		while (vet[low].incrate < pivo)
			low++;
		while (vet[high].incrate > pivo)
			high--;
		if (low <= high) {
			temp = vet[low].incrate;
			cattemp = vet[low].id;
			vet[low].incrate = vet[high].incrate;
			vet[low].id = vet[high].id;
			vet[high].incrate = temp;
			vet[high].id = cattemp;
			low++;
			high--;
		}
	}
	if (lowold < high)
		rank(vet, lowold, high);
	if (highold > low)
		rank(vet, low, highold);
}

/*
 * Prints the values of the instances of each attribute
 * @param attrib attribute whose instances is printed
 * @param nbrlines number of instances of the dataset
 **/
void printAttribute(Attribute attrib, int nbrlines) {
	printf("\nNumber of Edges = %d", attrib.nbredges);
	printf("\nEdges List");
	for (int i = 0; i < attrib.nbredges; i++) {
		printf("[%d]=%.4f; \t", i, attrib.edges[i]);
	}
}

/*
 * Prints the values of the instances of each attribute
 * @param f  file whose output will be written
 * @param attrib attribute whose instances is printed
 * @param nbrlines number of instances of the dataset
 **/
void printAttribute(FILE * f, Attribute attrib, int nbrlines) {
	for (int i = 0; i < attrib.nbredges; i++) {
		fprintf(f, "[%d]=%.4f; \t", i, attrib.edges[i]);
	}
}

/**
 *  Gets information about an Interval [left_close, right_open[
 *  if the right interval is closed, Add a constant +0.01 before calling this function
 *  @param nbrlines number of instance of the dataset
 *  @param left_close left interval edge
 *  @param right_close right interval edge
 *  @param nbrcats number of input categories
 *  @param &nbrinst returns the number of instances in the interval
 *  @param &nbrmaj returns the number of instances of majority class in the interval
 *  @returns the majority class of the interval
 **/

int getMajority(int nbrlines, Attribute &attrib, float left_close,
		float right_open, int nbrcats, int &nbrinst, int &nbrmaj) {
	int inst = 0;
	int * occur_cat = new int[nbrcats];
	for (int i = 0; i < nbrcats; i++)
		occur_cat[i] = 0;
	while (attrib.instances[inst].val < left_close)
		inst++;  //enter in the interval

	nbrinst = 0;
	while ((inst < nbrlines) && (attrib.instances[inst].val >= left_close)
			&& (attrib.instances[inst].val < right_open)) {
		occur_cat[attrib.instances[inst].cat]++;
		nbrinst++;
		inst++;
	}
	int maj = 0;
	for (int i = 1; i < nbrcats; i++) {
		if (occur_cat[maj] < occur_cat[i])
			maj = i;
	}
	nbrmaj = occur_cat[maj];
	return maj;
}

/**
 * Generate Cut points by Condition 1 (changes in categories creates cut points)
 * @param attrib set of sorted instances of attribute attrib
 * @param nbrlines number of instances of the dataset
 **/
void getEdgesCondition1(Attribute &attrib, int nbrlines) {
	attrib.edges[0] = attrib.instances[0].val; //first cut point is the minimal value
	int nbred = 1;
	for (int i = 1; i < nbrlines - 1; i++)
		if (attrib.instances[i - 1].cat != attrib.instances[i].cat)
			if (attrib.instances[i - 1].val + 0.000000009
					< attrib.instances[i].val)
				attrib.edges[nbred++] = attrib.instances[i].val;
	attrib.edges[nbred++] = attrib.instances[nbrlines - 1].val;
	attrib.nbredges = nbred;
}

/**
 * Generate Cut points by Condition 2 (eliminates cut points of intervals smaller than binsize)
 * @param attrib set of sorted instances of attribute attrib
 * @param nbrlines number of instances of the dataset
 * @param binsize minimal number of instances per interval
 **/
void getEdgesCondition2(Attribute &attrib, int nbrlines, int binsize) {
	float * tedges = new float[attrib.nbredges];
	int nbredges, bincount, inst, iedg;
	inst = 1;
	iedg = 1;
	nbredges = 1;
	tedges[0] = attrib.edges[0]; //first edge mantained
	bincount = 1;
	for (iedg = 1; iedg < attrib.nbredges - 1; iedg++) {
		while ((attrib.instances[inst].val < attrib.edges[iedg])
				&& (inst < nbrlines - 1)) { //last edge mantained
			bincount++;
			inst++;
		}
		if (bincount >= binsize) {
			tedges[nbredges++] = attrib.edges[iedg];
			bincount = 1;
			inst++;
		}
	}
	tedges[nbredges++] = attrib.edges[attrib.nbredges - 1];
	//replacing old bigger list of cut point
	attrib.nbredges = nbredges;
	for (int i = 1; i < nbredges; i++)
		attrib.edges[i] = tedges[i];
}

/**
 * Generate Cut points by Condition 3
 * @param attrib set of sorted instances of attribute attrib
 * @param nbrlines number of instances of the dataset
 * @param nbrcats number of categories
 * @param incrate maximal inconsistence rate to fuse two consecutive intervals
 * = (nbroccur instance of not-majority class) / (total number of instances of intervals)
 **/
void getEdgesCondition3(Attribute &attrib, int nbrlines, int nbrcats,
		float incrate) {

	float * tedges = new float[attrib.nbredges];
	int maj, majold, iedg, nbredges;
	int nbrmaj, nbrmajold = 0;
	int nbrinst, nbrinstold = 0;
	iedg = 1;
	nbredges = 1;
	tedges[0] = attrib.edges[0]; //first edge mantained
	bool fused;
	float left_close, right_open;
	for (iedg = 1; iedg < attrib.nbredges; iedg++) {
		left_close = attrib.edges[iedg - 1];
		right_open = attrib.edges[iedg];
		if (iedg + 1 == attrib.nbredges)
			right_open += 0.01; //closed interval, last cut point
		maj = getMajority(nbrlines, attrib, left_close, right_open, nbrcats,
				nbrinst, nbrmaj);
		fused = false;
		if ((nbrinstold != 0) && (maj == majold)) {
			float rateold = (float) (nbrinstold - nbrmajold) / nbrinstold;
			float rate = (float) (nbrinst - nbrmaj) / nbrinst;
			if ((rateold < incrate) && (rate < incrate)) {
				nbrmajold += nbrmaj;
				nbrinstold += nbrinst;
				fused = true;
			}
		}
		if (fused == false) {     //&& ( (iedg-1)!=0 )
			nbrmajold = nbrmaj;
			nbrinstold = nbrinst;
			majold = maj;
			if ((iedg - 1) != 0)
				tedges[nbredges++] = attrib.edges[iedg - 1];
		}

	}
	if (fused == true || iedg == attrib.nbredges)
		tedges[nbredges++] = right_open; //last cut point

	//replacing old bigger list of cut point
	attrib.nbredges = nbredges;
	for (int i = 1; i < nbredges; i++)
		attrib.edges[i] = tedges[i];

}

/**
 * Update Inconsistence Value of each attribute
 * @param attrib set of sorted instances of attribute attrib
 * @param nbrlines number of instances of the dataset
 * @param nbrcats number of categories
 **/
void calcInconsistence(Attribute &attrib, int nbrlines, int nbrcats) {
	int iedg = 1;
	int nbrinst = 0, incount = 0, nbrmaj;
	float left_close, right_open;
	for (iedg = 1; iedg < attrib.nbredges; iedg++) {
		left_close = attrib.edges[iedg - 1];
		right_open = attrib.edges[iedg];
		if (iedg + 1 == attrib.nbredges)
			right_open += 0.01; //closed interval, last cut point
		getMajority(nbrlines, attrib, left_close, right_open, nbrcats, nbrinst,
				nbrmaj);
		incount += nbrinst - nbrmaj;
	}
	attrib.incrate = (float) incount / nbrlines;
}

/*
 * Discretize the data
 * @param reader
 * @param binsize  - minimal number of instance per interval
 * @param incratetofuse - maximal inconsistence rate of intervals to fuse two intervals
 * @param nbrcat - number of categories
 * @param incratetoselect - maximal inconsistence rate of an attribute
 *
 */
void discretize(Reader * reader, int binsize, float incratetofuse, int nbrcats,
		float incratetoselect) {

	FILE * f = fopen("presage_log.txt", "w");
	fprintf(f,
			"binsize=%d; max_inc_to_fuse=%.2f; nbrcats=%d; max_globalinc_to_select=%.3f",
			binsize, incratetofuse, nbrcats, incratetoselect);
	int nbrattribs = reader->getNbrAttribs();
	int nbrlines = reader->getNbrLines();
	int nbredges = nbrlines;   //max number of edges

	//it stores instances value and class info for each attribute.
	Attribute * attributes = new Attribute[nbrattribs];

	//time count start;
	clock_t start, end;
	start = clock();

	//for each attribute
	for (int iat = 0; iat < nbrattribs; iat++) { //begin
		printf("\n\n******Processing attribute %d********", iat + 1);
		//initializing data structure for each attribute
		attributes[iat].edges = new float[nbredges];
		attributes[iat].nbredges = 0;
		attributes[iat].instances = new Instance[nbrlines];

		int i = 0; //i used to vary line in line scan
		reader->firstLine();
		do {
			attributes[iat].instances[i].cat = reader->getCatValue();
			attributes[iat].instances[i].val = reader->getValueOfIndex(
					reader->getQuanValue(), iat);
			i++;
		} while (reader->nextLine() != -1);

		quicksort(attributes[iat].instances, 0, nbrlines - 1);

		printf("\nPrinting sorted attribute %d", iat + 1);
		for (int i = 0; i < nbrlines; i++)
			printf("\ncat=%d val=%.6f", attributes->instances[i].cat,
					attributes->instances[i].val);

		//Condition 1: create a cut point for every class change
		printf("\nValidating condition 1");
		getEdgesCondition1(attributes[iat], nbrlines);
		//printAttribute(attributes[iat], nbrlines);

		//Condition 2: eliminate intervals with less than binsize instance
		printf("\nValidating condition 2");
		getEdgesCondition2(attributes[iat], nbrlines, binsize);
		//printAttribute(attributes[iat], nbrlines);

		//Condition 3: Fuse Consecutive Intervals
		printf("\nValidating condition 3");
		getEdgesCondition3(attributes[iat], nbrlines, nbrcats, incratetofuse);
		//printAttribute(attributes[iat], nbrlines);

		//prints the attribute cut points in the output f
		printf("\nPrinting the cut points in the file");
		fprintf(f, "\n[%d]=> ", iat + 1);
		printAttribute(f, attributes[iat], nbrlines);

	} //end for attrib

	transformDISCRETE(reader, attributes, nbrcats);
	transformNOMINAL(reader, attributes, nbrcats);

	//calc inconsistent rate by attribute
	for (int iat = 0; iat < nbrattribs; iat++) {
		attributes[iat].id = iat + 1; //ATTRIBUTES NAME START BY 0NE->1
		calcInconsistence(attributes[iat], nbrlines, nbrcats);
	}

	printf("\n\nALL Attributes:");
	fprintf(f, "\nAttributes Selected Ordered By Indice::\n");
	int nbr_a_sel = 0;
	for (int iat = 0; iat < nbrattribs; iat++) {
		printf("\n%d(%.3f)", attributes[iat].id, attributes[iat].incrate);
		if (attributes[iat].incrate < incratetoselect) {
			fprintf(f, "%d,", attributes[iat].id);
			nbr_a_sel++;
		}
	}

	fprintf(f, "\nNumber of attributes selected: \%d", nbr_a_sel);
	printf("\nNumber of attributes selected: \%d", nbr_a_sel);

	rank(attributes, 0, nbrattribs - 1);

	//STOP MEASURING MININIG TIME
	end = clock();

	printf("\n\nALL Attributes Ranked Inconsistence Ascending:");
	fprintf(f, "\nAttributes Ranked Inconsistence Ascending:\n");
	for (int iat = 0; iat < nbrattribs; iat++) {
		printf("\n%d(%.3f)", attributes[iat].id, attributes[iat].incrate);
		if (attributes[iat].incrate < incratetoselect) {
			fprintf(f, "%d,", attributes[iat].id);
		}
	}

	fclose(f);
} //end function

/*Transforms the database in a NOMINAL database
 * @param reader
 * @param attributes, array of attribute information
 * @param attributes vector of attributes
 * @param nbrcats number of categories
 */

void transformNOMINAL(Reader * reader, Attribute * attributes, int nbrcats) {
	FILE * fd = fopen("entradaApriori.txt", "w");
	reader->firstLine();
	int cat, bin = 0, upper = 0, iat = 0, line = 0;
	float val;
	int nbrattribs = reader->getNbrAttribs();

	line = 0;
	do {
		fprintf(fd, "%i ", line + 1);
		for (iat = 0; iat < nbrattribs; iat++) {
			val = reader->getValueOfIndex(reader->getQuanValue(), iat);
			upper = 1;
			while ((upper < attributes[iat].nbredges)
					&& (attributes[iat].edges[upper] < val + TOLERATE))
				upper++;
			fprintf(fd, "%i[%.8f-%.8f[ ", iat + 1,
					attributes[iat].edges[upper - 1],
					attributes[iat].edges[upper]);
		}
		fprintf(fd, "%d", reader->getCatValue());
		fprintf(fd, "\n");
		line++;
	} while (reader->nextLine() != -1);
	fclose(fd);

}

/*Transforms the database in a DISCRETE database
 * @param reader
 * @param attributes, array of attribute information
 * @param attributes vector of attributes
 * @param nbrcats number of categories
 */

void transformDISCRETE(Reader * reader, Attribute * attributes, int nbrcats) {
	FILE * fd = fopen("Out_Discret.arff", "w");
	FILE * fpr = fopen("Out_PrecRec.txt", "w");

	reader->firstLine();
	int cat, bin = 0, upper = 0, iat = 0, line = 0;
	float val;
	int nbrattribs = reader->getNbrAttribs();

	//cabecalho do arquivo do weka
	fprintf(fd, "@relation discweka\n");
	fprintf(fd, "@attribute id NUMERIC \n");
	//atributos
	for (iat = 0; iat < nbrattribs; iat++)
		fprintf(fd, "@attribute a%i NUMERIC\n", iat + 1);
	//categoria
	fprintf(fd, "@attribute cat {");
	for (cat = 0; cat < nbrcats - 1; cat++)
		fprintf(fd, "%d,", cat);
	fprintf(fd, "%d}", cat);

	fprintf(fd, "\n@data\n");
	//data
	line = 0;
	do {
		fprintf(fd, "%i,", line + 1);
		fprintf(fpr, "%i", line + 1);  //file prec rec
		fprintf(fpr, " %d", reader->getCatValue()); //file prec rec
		for (iat = 0; iat < nbrattribs; iat++) {
			val = reader->getValueOfIndex(reader->getQuanValue(), iat);
			upper = 1;
			while ((upper < attributes[iat].nbredges)
					&& (attributes[iat].edges[upper] < val + TOLERATE))
				upper++;
			fprintf(fd, "%i,", upper - 1);
			fprintf(fpr, " %i", upper - 1);
		}

		fprintf(fd, "%d", reader->getCatValue());
		fprintf(fd, "\n");
		fprintf(fpr, "\n");
		line++;
	} while (reader->nextLine() != -1);
	fclose(fd);
	fclose(fpr);
}
