#include"Hash.h"

using namespace std;


//Init Frequence Table
//Input: text, output: HuffNode Table
vector<HuffNode*> initFreqTable(string str) {
	HashTable<int> hash("int");
	vector<HuffNode*> freqTable(0);

	for (int i = 0; i < str.length(); i++) {
		hash.insert(str[i]);
	}
	vector<int> hashTable = hash.getTable();

	for (int i = 0; i < MAX; i++) {
		if (hashTable[i] != 0) {
			freqTable.push_back(newNode((char)i, hashTable[i]));
		}
	}

	return freqTable;
}
//Get content from a file
//Input: file name, output: content in file
string getStringFromFile(string filename) {
	ifstream fi;
	fi.open(filename, ios::binary);
	if (!fi) return "";

	// Di chuyen con tro file de xac dinh kich thuoc file
	fi.seekg(0, ios::end);
	int fsize = fi.tellg();
	fi.seekg(0, ios::beg);

	// read text
	char* c = new char[fsize];
	fi.read(c, fsize);
	string str(c, fsize);

	delete[]c;

	return str;
}