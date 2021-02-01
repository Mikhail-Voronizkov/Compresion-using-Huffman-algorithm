#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <regex>
#include <boost/filesystem.hpp>
#include "Huffman.h"
#include "Hash.h"

using namespace std;

// ChuyêÒn daÞy bit sang kiì týò 
char bitsToChar(string code);
// Xây dýòng baÒng tra cýìu neìn týÌ Huffman Codes
HashTable<string>* buildCompressTable(vector<HuffmanCode*>& huffCodes);
// Taòo daÞy bit týÌ text dýòa vaÌo baÒn tra cýìu neìn
string getSequenceOfBit(string text, HashTable<string>* table);
// Neìn vaÌo file
void compressToFile(string filename, vector<HuffNode*>& freqTable, string text);

//Input: Đường dẫn đến tập tin cần nén, tập tin output sau khi nén
void compressFile(const boost::filesystem::path& relative_path, string output);
void compressFileLZW(const boost::filesystem::path& relative_path, string output);

//Input: Đường dẫn thư mục cần nén, đây có phải là thư mục gốc hay sub Folder, đường dẫn sub Folder
//Nếu là thư mục gốc thì tham số cuối là chuỗi rỗng
void compressFolder(const boost::filesystem::path& relative_path, bool root, string rname, bool isLZW);


