#ifndef DISCRET_H
#define DISCRET_H

#include "Reader.h"
#include <math.h>

//define maxima tolerancia para a variacao de um intervalo
#define TOLERATE 0.0001

/**
 * An attribute value linked with its category
 * val - value of an attribute
 * cat - category of an attribute
 **/
typedef struct {
	float val;
	int cat;
} Instance;

/**
 * Info about an Attribute
 * edges - intervals' limits
 * nbredges - number of edges
 * instance - set of attributes values linked with its category
 * incrate - rate of inconsistences
 **/
typedef struct {
	int id;
	float * edges;
	int nbredges;
	Instance * instances;
	float incrate;
} Attribute;

/*
 * Sort the set of instances of an attribute, according to its values
 * @param vet vector of instances to be sort
 * @param imin minimum position to be sort
 * @param imax maximum position to be sort
 *
 */
void quicksort(Instance * vet, int low, int high);
void rank(Attribute * vet, int low, int high);

/*
 * Prints the values of the instances of each attribute
 * @param attrib attribute whose instances is printed
 * @param nbrlines number of instances of the dataset
 **/
void printAttribute(Attribute attrib, int nbrlines);

/*
 * Prints the values of the instances of each attribute
 * @param f  file whose output will be written
 * @param attrib attribute whose instances is printed
 * @param nbrlines number of instances of the dataset
 **/
void printAttribute(FILE * F, Attribute attrib, int nbrlines);

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
		float incratetoselect);

/**
 *  Gets information about an Interval [left_close, right_open[
 *  if the right interval is closed, Add a constant +0.01 before calling this function
 *  @param nbrlines number of instances
 *  @param left_close left interval edge
 *  @param right_close right interval edge
 *  @param nbrcats number of input categories
 *  @param &nbrinst returns the number of instances in the interval
 *  @param &nbrmaj returns the number of instances of majority class in the interval
 *  @returns the majority class of the interval
 **/
int getMajority(int nbrlines, Attribute &attrib, float left_close,
		float right_open, int nbrcats, int &nbrinst, int &nbrmaj);

/**
 * Generate Cut points by Condition 1
 * @param attrib set of sorted instances of attribute attrib
 * @param nbrlines number of instances of the dataset
 **/
void getEdgesCondition1(Attribute &attrib, int nbrlines);

/**
 * Generate Cut points by Condition 2 (eliminates cut points of intervals smaller than binsize)
 * @param attrib set of sorted instances of attribute attrib
 * @param nbrlines number of instances of the dataset
 * @param binsize minimal number of instances per interval
 **/
void getEdgesCondition2(Attribute &attrib, int nbrlines, int binsize);

/**
 * Generate Cut points by Condition 3
 * @param attrib set of sorted instances of attribute attrib
 * @param nbrlines number of instances of the dataset
 * @param nbrcats number of categories
 * @param incrate maximal inconsistence rate to fuse two consecutive intervals
 * = (nbroccur instance of not-majority class) / (total number of instances of intervals)
 **/
void getEdgesCondition3(Attribute &attrib, int nbrlines, int nbrcats,
		float incrate);

/*Transforms the database in a NOMINAL DATABASE
 * @param reader
 * @param attributes, array of attribute information
 * @param attributes vector of attributes
 * @param nbrcats number of categories
 * @param attribrank initialize attribute information about inconsistences
 */

void transformNOMINAL(Reader * reader, Attribute * attributes, int nbrcats);

/*Transforms to DISCRETE, not nominal values
 * @param reader
 * @param attributes, array of attribute information
 * @param attributes vector of attributes
 * @param nbrcats number of categories
 * @param attribrank initialize attribute information about inconsistences
 */

void transformDISCRETE(Reader * reader, Attribute * attributes, int nbrcats);

#endif
