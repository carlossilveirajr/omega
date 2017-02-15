#ifndef READER_H
#define READER_H

#define ATRIBTAM 800
#define LINETAM 8000

#include<stdlib.h>
#include<stdio.h>

char * getNextWord(char * &str);

class Reader {
private:
	FILE * f;
	int ctind; /*!<Index of the choosen categorical attribute. First atribute have index 0.*/
	int qtind; /*!<Index of the choosen quantitative attribute. First atribute have index 1.*/
	int nbrlines; /*!<Number of file transations.*/
	int nbrattr; /*!<Number of features (attributes).*/
	int actualline; /*!<Transaction actual*/
	char * ctname; /*!<Name of the choosen categorical item*/
	char * qtname; /*!<Name of the choosen quantitative item*/
	char line[LINETAM];

public:
	/**
	 * Constructor, It opens the file and initializes almost all private
	 * attributes. Only nrblines is not initialized.
	 **/
	Reader(char * filename);

	/**
	 * Move the current file reader to next transaction.
	 * Return -1 if next line doesn't exist.
	 **/
	int nextLine();

	/**
	 * Move the current file reader to the first transaction.
	 **/
	void firstLine();

	/**
	 * Gets the categorical attribute value of the current transaction.
	 * Returns -1 if it doesn�t exist.
	 */
	int getCatValue();

	/**
	 * Gets the quantitative attribute value of the current transaction.
	 * Returns -1 if it doesn�t exist.
	 */
	float * getQuanValue();

	/*
	 * Gets the ith value of qt. Return -1 if it does not exists.
	 */
	float getValueOfIndex(float * qt, int index);

	/**
	 * Gets the number of attributes or features
	 * @returns the number of features or atributes of the file
	 **/
	int getNbrAttribs();
	/**
	 * Gets the number of lines of the database
	 * @returns the number of lines of the database
	 **/
	int getNbrLines();

	/**
	 * Prints the values of categorical and quantitative atributes of the current
	 * transaction.
	 **/
	void show();

	void closef() {
		fclose(f);
	}

	int getIDValue();
};

#endif
