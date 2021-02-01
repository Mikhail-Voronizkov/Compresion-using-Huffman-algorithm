// C program for Huffman Coding 
#include <stdio.h> 
#include <stdlib.h>
#include"boost/filesystem.hpp" 
#include"Huffman.h"
#include"Hash.h"
#include "Compress.h"
#include "Decompress.h"


// This constant can be avoided by explicitly 
// calculating height of Huffman Tree 
#define MAX_TREE_HT 256

using namespace std;

int main()
{
	/*string str = getStringFromFile("text.txt");
	vector<HuffNode*> freqTable = initFreqTable(str);

	compressToFile("compress", freqTable, str);
	decompressFromFile("compress", "extract.txt");*/

	char c;
	cout << "1. Compress\n2. Decompress\n>> ";
	cin >> c;

	if (c == '1') {
		char type;
		bool isLZW;

		cout << "Ban muon file text duoc nen theo kieu nao (file binary se su dung huffman): \n";
		cout << "1. Huffman\n2. LZW\n>> ";
		cin >> type;
		cin.ignore(1);

		if (type == '2') isLZW = 1;
		else isLZW = 0;

		string s;
		cout << "Nhap duong dan: " << endl;
		getline(cin, s);

		if (boost::filesystem::is_directory(s)) {
			compressFolder(s, true, "", isLZW);
		}
		else {
			string output;
			output = s + "_compress";

			regex textCheck(".+\\.txt$");

			if (isLZW && regex_match(s, textCheck)) {
				compressFileLZW(s, output);
			}
			else compressFile(s, output);
		}
	}
	else {
		cin.ignore(1);
		string s;
		cout << "-----Decompress-----" << endl;
		cout << "Nhap duong dan: " << endl;
		getline(cin, s);

		if (boost::filesystem::is_directory(s)) {
			decompressFolder(s, true, "");
		}
		else {
			string output(s);
			output.resize(s.size() - strlen("_compress"));
			decompressFile(s, output);
		}
	}


	return 0;
}


