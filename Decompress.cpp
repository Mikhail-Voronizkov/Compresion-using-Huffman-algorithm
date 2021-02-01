#include "Decompress.h"
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


// Chuyen ki tu thanh day Bit
string charToBits(char c) {
	int dec = (int)c;
	// Kiem tra extended ascii
	if (dec < 0) dec = dec + 129 + 127;
	string bits = "";

	// Doc tung bit cua dec roi them vao chuoi bit
	while (dec) {
		int bit = dec & 1;
		if (bit == 1) bits = "1" + bits;
		else bits = "0" + bits;
		dec >>= 1;
	}

	// padding start with 0 for 8-length bits
	while (bits.length() < 8) {
		bits = "0" + bits;
	}

	return bits;
}

// Ham doc bang tan so tu file nen
// Input: Doi tuong file input
// Output: Bang Tan So
vector<HuffNode*> getFreqTableFromFile(ifstream& fi) {
	// no-used, just for bypass first byte
	char isLZW; // 0 is huffman
	fi.read(&isLZW, sizeof(char));

	int size;
	vector<HuffNode*> freqTable;
	// Read Size of Freq Table
	fi.read((char*)& size, sizeof(int));

	// Read Freq Table
	freqTable.resize(size);
	for (int i = 0; i < size; i++)
	{
		freqTable[i] = new HuffNode;
		fi.read((char*)freqTable[i], sizeof(HuffNode));
	}

	return freqTable;
}

// Ham doc gia tri noi dung nen tu file
// Input: doi tuong file input
// Output: Noi dung du lieu nen
string getCompressTextFromFile(ifstream& fi) {
	// Get file size
	int readSize = fi.tellg();
	fi.seekg(0, ios::end);
	int fsize = fi.tellg();
	fi.seekg(readSize);
	// Get text size
	int textSize = fsize - readSize - sizeof(int);

	// Read Compress Text
	char* c = new char[textSize];
	fi.read(c, textSize);
	string compressText(c, textSize);

	delete[]c;

	return compressText;
}

// Ham giai nen chinh cua Huffman
void decompress(string compressText, string des, HuffNode* root, int addedBit) {
	HuffNode* temp = root;
	int length = compressText.length();

	ofstream fo;
	fo.open(des, ios::binary);

	for (int i = 0; i < length; i++)
	{
		// Lower Bound => define added bit
		int lowerBound = 0;
		if (i == length - 1) lowerBound = addedBit;

		// Get 1 byte 
		unsigned int byte = (unsigned int)compressText[i];

		// Search char using bit from huffman tree
		for (int j = 7; j >= lowerBound; j--)
		{
			int bit = (byte >> j) & 1;
			// Neu bit la 0 lay nut trai
			if (bit == 0) {
				temp = temp->left;
			}
			// Neu bit la 1 lay nut phai
			else {
				temp = temp->right;
			}
			// Neu la la' thi ghi du lieu cua la' do vao file
			if (isLeaf(temp)) {
				fo.write(&temp->data, 1);
				temp = root;
			}
		}
	}

	fo.close();
}

// Thuc hien cac cong viec giai nen
void decompressFromFile(string src, string des) {
	ifstream fi;
	fi.open(src, ios::binary);

	// Read freq table from file and build huffman tree
	vector<HuffNode*> freqTable = getFreqTableFromFile(fi);
	HuffNode* root = buildHuffmanTree(freqTable);

	// Read compress text from file
	string compressText = getCompressTextFromFile(fi);

	// Read numbers of added bits
	int addedBit;
	fi.read((char*)& addedBit, sizeof(int));

	// Decompress
	decompress(compressText, des, root, addedBit);

	// Delete freq table
	for (int i = 0; i < freqTable.size(); i++)
	{
		delete freqTable[i];
	}

	fi.close();
}

// giai nen du lieu su dung LZW
void decompressFromFileLZW(string src, string des) {
	ifstream fi2;
	fi2.open(src, ios::binary);
	ofstream fo2;
	fo2.open(des, ios::binary);

	// no-used, just for bypass first byte
	char isLZW; // 0 is huffman
	fi2.read(&isLZW, sizeof(char));

	unsigned short d;
	vector<unsigned short> compressed(0);

	while (fi2.read((char*)& d, 2)) {
		compressed.push_back(d);
	}
	cout << compressed.size() << endl;
	int dictSize2 = 256;
	// Xay dung tu dien ban dau gom 256 tu don
	map<unsigned short, string> dict2;
	for (unsigned short i = 0; i < 256; i++) {
		dict2[i] = string(1, i);
	}

	cout << "size: " << dict2.size() << endl;

	string word, entry;
	word = dict2[compressed[0]];
	string result = word;

	// Duyen tuan tu cac dau hieu them vao tu dien dong thoi ghi vao result
	for (int i = 1; i < compressed.size(); i++) {
		unsigned short value = compressed[i];
		if (dict2.count(value)) {
			entry = dict2[value];
		}
		else if (value == dictSize2) {
			entry = word + word[0];
		}
		else {
			cout << "Loi roi";
			break;
		}
		// Bat dau viec xu ly them gia tri cho tu dien
		word += entry[0];
		if (dictSize2 <= 65535) dict2[dictSize2++] = word;
		result += entry;
		word = entry;
	}
	cout << result.length();
	fo2.write(result.c_str(), result.length());

	fi2.close();
	fo2.close();
}

// Ham nhan vao duong dan file nen va giai nen su dung thuat toan phu hop
void decompressFile(const boost::filesystem::path& source_path, string output) {
	ifstream fi;
	fi.open(source_path.generic_path().generic_string(), ios::binary);

	// Khong can giai nen neu file khong co noi dung
	fi.seekg(0, ios::end);
	if (!fi.tellg()) {
		ofstream fo(output, ios::binary);
		fo.close();
		fi.close();
		return;
	}
	fi.seekg(0, ios::beg);

	// Kiem tra thuat toan LZW hay huffman
	char isLZW;
	fi.read(&isLZW, sizeof(char));
	fi.close();
	// Tien hanh giai nen thuat toan phu hop
	if (isLZW) decompressFromFileLZW(source_path.generic_path().generic_string(), output);
	else decompressFromFile(source_path.generic_path().generic_string(), output);
}

// Ham nen 1 folder
void decompressFolder(const boost::filesystem::path& source_path, bool root, string rname) {
	string s;
	using namespace boost::filesystem;

	if (!boost::filesystem::exists(source_path)) {
		cerr << source_path << " does not exist!" << endl;
		return;
	}

	string name;
	if (root && is_directory(source_path)) {
		_chdir(source_path.generic_path().generic_string().c_str());
		name = current_path().generic_path().generic_string();
		name.resize(name.size() - strlen("_compress"));
		create_directories(name);
		cout << "Root: " << endl;
		cout << source_path.generic_path().generic_string().c_str() << endl;
		cout << name << endl;
		cout << "----------------------------" << endl;
	}
	else {
		name = rname;
		_chdir(rname.c_str()); //Cho lá
	}

	//Duyệt thư mục
	boost::filesystem::directory_iterator end_itr;

	for (boost::filesystem::directory_iterator itr(source_path); itr != end_itr; itr++) {
		if (boost::filesystem::is_directory(*itr)) {
			path Sub(*itr);
			_chdir(name.c_str());
			string subname = Sub.filename().generic_string();
			subname.resize(subname.size() - strlen("_compress"));
			subname = current_path().generic_path().generic_string() + "\\" + subname;
			cout << "cwd: " << current_path() << endl;
			cout << "Directory: " << endl;
			cout << subname << endl;

			create_directories(subname);
			decompressFolder(*itr, false, subname);
		}
		else {
			_chdir(name.c_str());
			cout << "cwd: " << current_path() << endl;
			cout << "**** FILE ***" << endl;
			path File(*itr);
			string fname = File.filename().generic_string();
			fname.resize(fname.size() - strlen("_compress"));
			cout << "File: " << fname << endl;
			decompressFile(*itr, fname);
		}
	}
}