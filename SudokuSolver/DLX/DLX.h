#ifndef DLX_H
#define DLX_H

#include <string>
#include <vector>

struct Header;

struct Node {
	Node* left_, * right_, * up_, * down_;
	Header* column_;
	size_t row_;

	Node(size_t row, Node* left = nullptr, Node* right = nullptr, Node* up = nullptr, Node* down = nullptr, Header* column = nullptr)
		:row_(row), left_(left), right_(right), up_(up), down_(down), column_(column) {}
};

struct Header : public Node {
	int size_;
	std::string name_;

	Header(const std::string& name = "Default Name"/*, Header* left = nullptr, Header* right = nullptr, Node* up = nullptr, Node* down = nullptr*/) :
		size_(0), name_(name), Node(0, this, this, this, this, this) {}
};

class DLX {
public:
	DLX(const std::string&);
	DLX(const std::vector<std::vector<bool>>&, const std::vector<std::string>&);

	void print() const;

	std::vector<std::vector<short>> Solutions();


private:
	void createHeaders(const std::vector<std::string>&);
	void createNodes(const std::vector<std::vector<bool>>&);
	Header* chooseColumn();

	void printOutput(const std::vector<Node*>& outputs);
	void addRow(int number, int i);
	void writeInFile();
	int getBoxNumber(int row, int column) const;
	
	void search(int k, std::vector<Node*> solution = std::vector<Node*>());

	void cover(Header*);
	void unCover(Header*);
private:
	static const int cellHeaderStart = 0;
	static const int rowHeaderStart = 81;
	static const int columnHeaderStart = 162;
	static const int boxHeaderStart = 243;
private:
	std::vector<Header*> m_headers;

	Header* m_root;

	size_t m_rows;
	size_t m_columns;

	std::vector<Node*> m_solutions;
};

#endif
