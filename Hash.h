#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include"Huffman.h"

using namespace std;
#define MAX 256

//Hash Table
template<class T>
class HashTable {
private:
	vector<T> table;
public:
	HashTable(string type) {
		if (type == "int")
			table.resize(MAX, 0);
		else
			table.resize(MAX);
	}
	void setAllValue(T value) {
		for (int i = 0; i < MAX; i++)
		{
			table[i] = value;
		}
	}
	//Get hash key for character
	//Input: char, output: hashKey
	int getHash(char c) {
		/*	cout << (int)c;
			int temp = (int)c;
			if (temp < 0) temp = abs(temp) + 255;*/
		int dec = (int)c;
		if (dec < 0) dec = dec + 129 + 127;
		return dec % MAX;
	}

	//Insert freq to table using hashKey
	void insert(char c) {
		int key = getHash(c);
		table[key]++;
	}
	// Insert with value
	void insert(char c, T code) {
		int key = getHash(c);
		table[key] = code;
	}

	// Get value from char key
	T* get(char c) {
		int key = getHash(c);
		return &table[key];
	}

	//Return table
	const vector<int>& getTable() {
		return this->table;
	}
};

//Init Frequence Table
//Input: text, output: HuffNode Table
vector<HuffNode*> initFreqTable(string str);
//Get content from a file
//Input: file name, output: content in file
string getStringFromFile(string filename);
