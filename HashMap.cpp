/*
 * HashMap.cpp
 *
 *  Created on: May 4, 2020
 *      Author: 13027
 */

#include "HashMap.hpp"
#include "HashNode.hpp"
#include <iostream>
#include <math.h>
using namespace std;

hashMap::hashMap(bool hash1, bool coll1)
/* Parameter: boolean hash1, boolean coll1
 *
 * Return: None
 *
 * when creating the map, make sure you
 * initialize the values to NULL so you
 * know whether that index has a key
 * in it or is set to NULL.
 */
{
	first = "";
	numKeys = 0;
	mapSize = 7;
	collisions = 0;
	hashcoll = 0;
	hashfn = hash1;
	collfn = coll1;

	map = new hashNode *[mapSize];

	for (int i = 0; i < mapSize; i++)
	{
		map[i] = NULL;
	}
}
void hashMap::addKeyValue(string k, string v)
/* Parameter: string k, string v
 *
 * Return: None.
 *
 * adds a node  to the map at the correct index
 * based on the key string, and then inserts the
 * value into the value field of the hashNode
 * Must check to see whether there's already a
 * node at that location.  If there's nothing
 * there(it's NULL), add the hashNode with the
 * keyword and value. If the node has the same keyword, just add
 * the value to the list of values.
 * If the node has a different keyword, keep calculating a new hash index until either the
 * keyword matches the node at that index's
 * keyword, or until the map at that index is
 * NULL, in which case you'll add the node there.
 * This method also checks for load, and if the
 * load is over 70%, it calls the reHash method
 * to create a new longer map array and rehash the values.
 */

{
	int index = getIndex(k);
	hashNode *nodeAtIndex = map[index];
	if (nodeAtIndex == NULL)
	{
		map[index] = new hashNode(k);
		map[index]->addValue(v);
	}
	else if (nodeAtIndex->keyword == k)
	{
		map[index]->addValue(v);
	}

	numKeys++;
	if (((float)numKeys / (float)mapSize) > 0.7)
		reHash();
}
int hashMap::getIndex(string k)
/* Parameter: string k
 *
 * Return: int
 *
 * This function is used to uses calcHash and reHash to
 * calculate and return the index of where
 * the keyword k should be inserted into the map array.
 */
{
	int index = hashfn ? calcHash1(k) : calcHash2(k);
	hashNode *nodeAtIndex = map[index];
	int ct = 0;
	while (true)
	{
		if (nodeAtIndex == NULL)
		{
			return index;
		}
		else if (nodeAtIndex->keyword == k)
		{
			return index;
		}
		if (ct == 0)
			hashcoll++;
		index = collfn ? coll1(index, ct, k) : coll2(index, ct, k);
		nodeAtIndex = map[index];
		collisions++;
		ct++;
	}
}

int hashMap::calcHash1(string k)
/* Parameter: string k
 *
 * Return: int
 *
 * This function is used to calculate the hash.
 *
 */
{
	int L = k.length();
	if(L == 1) return (int)k[0]% mapSize;
	else if (L == 2) return ((int)k[0]% + (int)k[1])%mapSize;
	else
	return ((int)k[0] + (int)k[1] * 27 + (int)k[2] * 27 * 27) % mapSize;
}

int hashMap::calcHash2(string k)
/* Parameter: string k
 *
 * Return: int
 *
 * This function is used to calculate the hash function 2.
 */
{
	int h;
	int L = k.length();
	for (int j = 0; j < L; j++)
	{
		h += (int)k[j];
	}
	return h % mapSize;
}

bool isPrime(int n)
{
	if (n == 1)
		return false;

	for (int i = 2; i < n; i++)
		if (n % i == 0)
			return false;

	return true;
}
void hashMap::getClosestPrime()
/* Parameter: None
 *
 * Return: None
 *
 * This function is using a binary search and an
 * array of primes to find the closest prime to
 * double the map Size, and then set mapSize to
 * that new prime - you can find the prime in
 * another way if you choose.
 */
{
	mapSize *= 2;
	while (true)
	{
		mapSize++;
		if (isPrime(mapSize))
			break;
	}
}
void hashMap::reHash()
/* Parameter: None
 *
 * Return: None
 *
 * This function is use to resize the Hash, if the
 * size of array is at 70%, then it will double array size
 * and rehash keys.
 */
{
	int oldMapSize = mapSize;
	hashNode **oldMap = map;
	getClosestPrime();
	numKeys = 0;
	hashNode **newMap = new hashNode *[mapSize];
	map = newMap;
	for (int i = 0; i < mapSize; i++)
	{
		map[i] = NULL;
	}

	for (int i = 0; i < oldMapSize; i++)
	{
		if (oldMap[i] != NULL)
		{
			for (int j = 0; j < oldMap[i]->currSize; j++)
			{
				addKeyValue(oldMap[i]->keyword, oldMap[i]->values[j]);
			}
		}
	}
}
int hashMap::coll1(int h, int i, string k)
/* Parameter: int h, int i, string k
 *
 * Return: int
 *
 * This function is use for a probing method
 * for collisions (when index is already full)
 *
 */
{
	return (h + i) % mapSize;
}
int hashMap::coll2(int h, int i, string k)
/* Parameter: int h, int i, string k
 *
 * Return: int
 *
 * This function is use for a different method
 * for collisions (when index is already full)
 *
 */
{
	return (h + i * i) % mapSize;
}
int hashMap::findKey(string k)
/* Parameter: string k
 *
 * Return: int
 *
 * This function is use for finds the key in the array and
 * returns its index.  If it's not in the array, returns -1.
 *
 */
{
	//NOTE: THIS METHOD CANNOT LOOP from index 0 to end of hash array looking for the key.  That destroys any efficiency in run-time.
	int index = hashfn ? calcHash1(k) : calcHash2(k);
	int ct = 0;
	while (map[index]->keyword != k)
	{
		if (map[index] == NULL)
			return -1;
		index = collfn ? coll1(index, ct, k) : coll2(index, ct, k);
	}
	return index;
}

void hashMap::printMap()
{
	cout << "In printMap()" << endl;
	for (int i = 0; i < mapSize; i++)
	{
		//cout << "In loop" << endl;
		if (map[i] != NULL)
		{
			cout << map[i]->keyword << ": ";
			for (int j = 0; j < map[i]->currSize; j++)
			{
				cout << map[i]->values[j] << ", ";
			}
			cout << endl;
		}
	}
}
