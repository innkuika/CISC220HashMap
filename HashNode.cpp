/*
 * HashNode.cpp
 *
 *  Created on: May 4, 2020
 *      Author: 13027
 */




#include "HashMap.hpp"
#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;



hashNode::hashNode(string s){
	keyword = s;
	values = new string[100];
	valuesSize = 100;
	currSize = 0;
	srand(time(NULL));
}
hashNode::hashNode(){
	keyword = "";
	values = new string[100];
	valuesSize = 100;
	currSize = 0;
	srand(time(NULL));
}
hashNode::hashNode(string s, string v){
	keyword = s;
	values = new string[100];
	values[0] = v;
	valuesSize = 100;
	currSize = 1;
}
void hashNode::addValue(string v) {
	/* Parameter: a string
	 *
	 * Return: None.
	 *
	 * This method is used to add a value to the end of the
	 * value array associated with a key.
	 */
	currSize++;
	values[currSize-1] = v;

	if(currSize > valuesSize/2)dblArray();
}
void hashNode::dblArray() {
	/* Parameter: None
	 *
	 * Return: None
	 *
	 * This function is for when the value array gets full, you need to make a new
	 * array twice the size of the old one (just double, no
	 * going to next prime) and then copy over the old values
	 * to the new values, then de-allocate the old array.
	 * Again, just copying over, no hash function involved here.
	 */
	valuesSize *= 2;
	string *newValues = new string[valuesSize];
	for(int i = 0; i < currSize; i++){
		newValues[i] = values[i];
	}
	delete []values;
	values = newValues;
}

string hashNode::getRandValue() {
	/* Parameter: None
	 *
	 * Return: a function.
	 *
	 * Every key has a values array - an array of words that
	 * follow that key in the text document.  You're going to
	 * randomly select one of those words and return it.  That
	 * will be the word that follows your key in your output
	 * function, and it will also be the next key.
	 */

	return values[rand() % currSize];
}
