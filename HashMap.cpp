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
{
	cout << "inside addKeyValue" << endl;
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
{
	int L = k.length();
	if(L == 1) return (int)k[0]% mapSize;
	else if (L == 2) return ((int)k[0]% + (int)k[1])%mapSize;
	else
	return ((int)k[0] + (int)k[1] * 27 + (int)k[2] * 27 * 27) % mapSize;
}

int hashMap::calcHash2(string k)
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
{
	return (h + i) % mapSize;
}
int hashMap::coll2(int h, int i, string k)
{
	return (h + i * i) % mapSize;
}
int hashMap::findKey(string k)
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
