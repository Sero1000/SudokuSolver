#include "DLX.h"

#include <iostream>
#include <limits>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include <vector>

using std::cout;

DLX::DLX(const std::vector<std::vector<bool>>& matrix, const std::vector<std::string>& headerNames) :m_root(new Header("Root")), m_rows(matrix.size()), m_columns(matrix[0].size()) {
	createHeaders(headerNames);
	createNodes(matrix);
}

DLX::DLX(const std::string& line) : m_root(new Header("Root")), m_columns(324) {
	assert(line.size() == 81);
	std::vector<std::string> headerNames(324);

	createHeaders(headerNames);

	for (int i = 0; i < line.size(); ++i) {
		char token = line[i];

		if (token != '.') {
			int number = token - 48;
			addRow(number, i);
		}
		else {
			for (int j = 1; j < 10; ++j) {
				addRow(j, i);
			}
		}
		writeInFile();
	}
}

void DLX::writeInFile() {
	std::ofstream file("DLX_Headers.txt");

	for (Header* currentHeader = m_root->right_->column_; currentHeader != m_root; currentHeader = currentHeader->right_->column_) {
		file << currentHeader->name_ << "/" << currentHeader->size_ << " ";
	}
}

std::vector<std::vector<short>> DLX::Solutions() {
	search(0);

	std::vector<std::vector<short>> result(9,std::vector<short>(9));

	for (int i = 0; i < m_solutions.size(); ++i) {
		Node* solutionNode = m_solutions[i];

		short row = solutionNode->row_ / 81;
		short column = (solutionNode->row_ % 81) / 9;
		short number = (solutionNode->row_ % 81) % 9 + 1;
		result[row][column] = number;

		result[row][column] = number;
	}

	return result;
}

void DLX::addRow(int number, int i) {
	int columnInSudoku = i % 9;
	int rowInSudoku = i / 9;

	int currentRow = 81 * rowInSudoku + 9 * columnInSudoku + number - 1;
	int boxNumber = getBoxNumber(rowInSudoku, columnInSudoku);

	Node* cellNode = new Node(currentRow);
	Node* rowNode = new Node(currentRow);
	Node* columnNode = new Node(currentRow);
	Node* boxNode = new Node(currentRow);

	cellNode->right_ = rowNode;
	rowNode->left_ = cellNode;

	rowNode->right_ = columnNode;
	columnNode->left_ = rowNode;

	columnNode->right_ = boxNode;
	boxNode->left_ = columnNode;

	boxNode->right_ = cellNode;
	cellNode->left_ = boxNode;

	//need to choose 4 Headers

	int cellHeaderIndex = cellHeaderStart + 9 * rowInSudoku + columnInSudoku;
	int rowHeaderIndex = rowHeaderStart + 9 * rowInSudoku + number - 1;
	int columnHeaderIndex = columnHeaderStart + 9 * columnInSudoku + number - 1;
	int boxHeaderIndex = boxHeaderStart + 9 * boxNumber + number - 1;

	if (cellHeaderIndex >= m_headers.size() ||
		rowHeaderIndex >= m_headers.size() ||
		columnHeaderIndex >= m_headers.size() ||
		boxHeaderIndex >= m_headers.size()) {
		int h = 4;
		++i;
	}

	Header* cellHeader = m_headers[cellHeaderIndex];
	Header* rowHeader = m_headers[rowHeaderIndex];
	Header* columnHeader = m_headers[columnHeaderIndex];
	Header* boxHeader = m_headers[boxHeaderIndex];

	cellNode->up_ = cellHeader->up_;
	cellNode->up_->down_ = cellNode;
	cellHeader->up_ = cellNode;
	cellNode->down_ = cellHeader;
	cellNode->column_ = cellHeader;

	rowNode->up_ = rowHeader->up_;
	rowNode->up_->down_ = rowNode;
	rowHeader->up_ = rowNode;
	rowNode->down_ = rowHeader;
	rowNode->column_ = rowHeader;

	columnNode->up_ = columnHeader->up_;
	columnNode->up_->down_ = columnNode;
	columnHeader->up_ = columnNode;
	columnNode->down_ = columnHeader;
	columnNode->column_ = columnHeader;

	boxNode->up_ = boxHeader->up_;
	boxNode->up_->down_ = boxNode;
	boxHeader->up_ = boxNode;
	boxNode->down_ = boxHeader;
	boxNode->column_ = boxHeader;

	++cellHeader->size_;
	++rowHeader->size_;
	++columnHeader->size_;
	++boxHeader->size_;
}

int DLX::getBoxNumber(int row, int column) const {
	return 3 * (row / 3) + column / 3;
}

void DLX::createHeaders(const std::vector<std::string>& headerNames) {
	Header* current = m_root;
	std::string name;
	for (size_t i = 0; i < m_columns; ++i) {
		name = headerNames[i];
		Header* newHeader = new Header(name);

		m_headers.push_back(newHeader);

		newHeader->left_ = current;
		current->right_ = newHeader;
		current = current->right_->column_;
	}

	m_root->left_ = current;
	current->right_ = m_root;
}

void DLX::createNodes(const std::vector<std::vector<bool>>& matrix) {
	Header* currentHeader;

	for (size_t row = 0; row < m_rows; ++row) {
		currentHeader = m_root;

		Node rowNode(row);
		rowNode.left_ = &rowNode;
		rowNode.right_ = &rowNode;
		rowNode.down_ = &rowNode;
		rowNode.up_ = &rowNode;

		for (size_t column = 0; column < m_columns; ++column) {
			currentHeader = currentHeader->right_->column_;
			if (matrix[row][column] == true) {
				Node* newNode = new Node(row);

				newNode->up_ = currentHeader->up_;
				currentHeader->up_->down_ = newNode;
				newNode->down_ = currentHeader;
				currentHeader->up_ = newNode;
				newNode->column_ = currentHeader;

				++currentHeader->size_;

				newNode->right_ = &rowNode;
				rowNode.left_->right_ = newNode;
				newNode->left_ = rowNode.left_;
				rowNode.left_ = newNode;
			}
		}

		rowNode.left_->right_ = rowNode.right_;
		rowNode.right_->left_ = rowNode.left_;
	}
}

void DLX::print() const {
	Header* current = m_root->right_->column_;
	while (current != m_root) {
		cout << current->name_ << "/" << current->size_ << " ";
		current = current->right_->column_;
	}
	cout << std::endl;

}

void DLX::cover(Header* c) {
	c->left_->right_ = c->right_;
	c->right_->left_ = c->left_;

	for (Node* i = c->down_; i != c; i = i->down_) {
		for (Node* j = i->right_; j != i; j = j->right_) {
			j->down_->up_ = j->up_;
			j->up_->down_ = j->down_;

			--j->column_->size_;
		}
	}
}

void DLX::unCover(Header* c) {
	for (Node* i = c->up_; i != c; i = i->up_) {
		for (Node* j = i->left_; j != i; j = j->left_) {
			++j->column_->size_;

			j->down_->up_ = j;
			j->up_->down_ = j;
		}
	}

	c->right_->left_ = c;
	c->left_->right_ = c;
}

void DLX::search(int k,std::vector<Node*>& solutions) {
	//cout << "search(" << k << ")" << std::endl;
	if (m_root->right_ == m_root) {
		m_solutions = solutions;
		return;
	}
	Header* c = chooseColumn();
	cover(c);
	for (Node* r = c->down_; r != c; r = r->down_) {
		solutions.push_back(r);

		for (Node* j = r->right_; j != r; j = j->right_) {
			cover(j->column_);
		}

		search(k + 1,solutions);

		solutions.pop_back();
		c = r->column_;

		for (Node* j = r->left_; r != j; j = j->left_) {
			unCover(j->column_);
		}
	}

	unCover(c);
	return;
}

void DLX::printOutput(const std::vector<Node*>& O) {

	std::vector < std::vector<int>> sudoku(9, std::vector<int>(9));

	for (int i = 0; i < O.size(); ++i) {
		Node* r = O[i];
		size_t row = r->row_ / 81;
		size_t column = (r->row_ % 81) / 9;
		size_t number = (r->row_ % 81) % 9 + 1;
		sudoku[row][column] = number;
	}

	for (int row = 0; row < sudoku.size(); ++row) {
		for (int column = 0; column < sudoku[row].size(); ++column) {
			cout << sudoku[row][column] << "  ";
		}
		cout << "\n\n";
	}
}

Header* DLX::chooseColumn() {
	int s = std::numeric_limits<int>::max();
	Header* res = m_root;

	for (Header* j = m_root->right_->column_; j != m_root; j = j->right_->column_) {
		if (j->size_ < s) {
			s = j->size_;
			res = j;
		}
	}

	return res;
}

