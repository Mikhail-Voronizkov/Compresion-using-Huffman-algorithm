#include"Huffman.h"

using namespace std;

HuffNode* newNode(char data, unsigned freq)
{
	HuffNode* temp = new HuffNode;

	temp->left = temp->right = NULL;
	temp->data = data;
	temp->freq = freq;

	return temp;
}
//Khởi tạo heap
MinHeap* createMinHeap(unsigned capacity)
{
	MinHeap* minHeap = new MinHeap;

	// current size is 0 
	minHeap->size = 0;

	minHeap->capacity = capacity;

	minHeap->array = new HuffNode * [capacity * sizeof(HuffNode*)];
	return minHeap;
}
//Sắp xếp heap thành min heap
void buildMinHeap(MinHeap* minHeap)
{
	int n = minHeap->size - 1;
	int i;

	for (i = (n - 1) / 2; i >= 0; i--)
		minHeapify(minHeap, i);
}

//Tạo ra một min heap có size Node.
//Mỗi Node chứa một ký tự từ mảng data và trọng số tương ứng trong mảng freq
MinHeap* createAndBuildMinHeap(vector<HuffNode*> freqTable)
{
	//Tạo cây có dung lượng size
	MinHeap* minHeap = createMinHeap(freqTable.size());

	for (int i = 0; i < freqTable.size(); ++i)
		minHeap->array[i] = newNode(freqTable[i]->data, freqTable[i]->freq);
	//Cập nhập số phần tử hiện tại trong cây
	minHeap->size = freqTable.size();
	//Sắp xếp cây thành min heap
	buildMinHeap(minHeap);

	return minHeap;
}

// swap 2 min heap nodes 
void swapHuffNode(HuffNode** a, HuffNode** b)
{

	HuffNode* t = *a;
	*a = *b;
	*b = t;
}

// minHeapify
void minHeapify(MinHeap* minHeap, int idx)
{
	int smallest = idx;
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;

	if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
		smallest = left;

	if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
		smallest = right;

	if (smallest != idx) {
		swapHuffNode(&minHeap->array[smallest], &minHeap->array[idx]);
		minHeapify(minHeap, smallest);
	}
}

//Kiểm tra số phần tử hiện tại trong min heap
int isSizeOne(MinHeap* minHeap)
{
	return (minHeap->size == 1);
}

//Trích xuất nút có trọng số nhỏ nhất(Xóa nút gốc ra khỏi min heap)
HuffNode* extractMin(struct MinHeap* minHeap)
{

	HuffNode* temp = minHeap->array[0];
	minHeap->array[0] = minHeap->array[minHeap->size - 1];

	minHeap->size = minHeap->size - 1;
	minHeapify(minHeap, 0);

	return temp;
}

//Thêm nút mới vào min heap
void insertMinHeap(MinHeap* minHeap, HuffNode* NewNode)
{

	minHeap->size++;
	int i = minHeap->size - 1;

	while (i && NewNode->freq < minHeap->array[(i - 1) / 2]->freq) {

		minHeap->array[i] = minHeap->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}

	minHeap->array[i] = NewNode;
}

// Kiểm tra nút có phải là lá trong cây hay không?
bool isLeaf(HuffNode* root)
{
	if ((root->left == NULL) && (root->right == NULL))
		return true;
	return false;
}

void printArr(int arr[], int n)
{
	int i;
	for (i = 0; i < n; ++i)
		cout << arr[i];

	cout << endl;
}

// Xây dựng cây Huffman
HuffNode* buildHuffmanTree(vector<HuffNode*> freqTable)
{
	HuffNode* left, * right, * top;

	//Bước 1: 
	// Tạo ra một min heap có size Node.
	// Mỗi Node chứa một ký tự từ mảng data và trọng số tương ứng trong mảng freq. 
	MinHeap* minHeap = createAndBuildMinHeap(freqTable);

	// Nếu số Node không phải là 1 thì lặp
	while (!isSizeOne(minHeap)) {

		//Bước 2:
		//Lấy ra 2 phần tử có trọng số nhỏ nhất từ min heap
		left = extractMin(minHeap);
		right = extractMin(minHeap);

		//Bước 3:
		//Tạo một Node mới có trọng số là tổng của 2 Node vừa lấy ra.
		//2 Node vừa lấy ra trở thành con của Node vừa tạo
		//Thêm Node vừa tạo vào lại min heap

		// '$' là giá trị đánh dấu nút tổng
		top = newNode('$', left->freq + right->freq);

		top->left = left;
		top->right = right;

		insertMinHeap(minHeap, top);
	}

	//Bước 4:
	//Trả về Node duy nhất còn lại trong heap
	//Đó chính là gốc của cây Huffman
	return extractMin(minHeap);
}

// Chuyển bit array sang dạng string
string bitArrToString(int arr[], int n) {
	string str;

	for (int i = 0; i < n; i++)
	{
		if (arr[i] == 1) str += '1';
		else str += '0';
	}

	return str;
}

// In bảng mã ra màn hình
// Dùng mảng arr để lưu mã
void printCodes(HuffNode* root, int arr[], int top, vector<HuffmanCode*>& out)

{
	// Gán 0 nếu là trái
	if (root->left) {

		arr[top] = 0;
		printCodes(root->left, arr, top + 1, out);
	}

	// Gán 1 nếu là phải
	if (root->right) {

		arr[top] = 1;
		printCodes(root->right, arr, top + 1, out);
	}

	//Nếu là lá thì xuất ra giá trị của lá và mã bit sau khi nén của nó
	if (isLeaf(root)) {

		HuffmanCode* huffcode = new HuffmanCode;
		huffcode->c = root->data;
		huffcode->code = bitArrToString(arr, top);
		out.push_back(huffcode);
	}
}

// Hàm tạo cây Huffman và bảng mã từ bảng tần số
vector<HuffmanCode*> HuffmanCodes(vector<HuffNode*>& freqTable)
{
	// Tạo cây Huffman 
	HuffNode* root = buildHuffmanTree(freqTable);

	//Mảng lưu mã của ký tự
	int arr[MAX_TREE_HT], top = 0;
	vector<HuffmanCode*> out(0); //Bảng mã sau khi mã hóa

	printCodes(root, arr, top, out);
	return out;
}