#include "SudokuSolver.h"
#include "SudokuWidget.h"
#include "DLX/DLX.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>

SudokuSolver::SudokuSolver(QWidget *parent)
	: QWidget(parent), m_sudokuWidget(new SudokuWidget(this))
{
	QPushButton* solveButton = new QPushButton("Solve");
	QPushButton* clearButton = new QPushButton("Clear");
	
	connect(solveButton, &QPushButton::pressed, this, &SudokuSolver::onSolve);
	connect(clearButton, &QPushButton::pressed, this, &SudokuSolver::onClear);

	setFixedWidth(480);
	QVBoxLayout* vBoxLayout = new QVBoxLayout();
	setLayout(vBoxLayout);
	vBoxLayout->addWidget(m_sudokuWidget);
	vBoxLayout->addWidget(solveButton);
	vBoxLayout->addWidget(clearButton);
}

void SudokuSolver::onSolve() {
	QVector<QVector<short>> input = m_sudokuWidget->Inputs();

	std::string inputLine;

	for (int row = 0; row < input.size(); ++row) {
		for (int column = 0; column < input[row].size(); ++column) {
			if (input[row][column] == -1) {
				inputLine.push_back('.');
			}
			else {
				inputLine.push_back(input[row][column] + 48);
			}
		}
	}

	DLX dlx(inputLine);

	std::vector<std::vector<short>> solution = dlx.Solutions();
	
	if (solutionFound(solution)) {
		m_sudokuWidget->SetNumbers(solution);
	}
	else {
		QMessageBox::information(this, "No Solution", "There is no solution for this sudoku");
		m_sudokuWidget->ClearInputs();
	}
}

void SudokuSolver::onClear() {
	m_sudokuWidget->ClearInputs();
}

bool SudokuSolver::solutionFound(const std::vector<std::vector<short>>& solution) const {
	if (solution[0][0] == 0 && solution[0][1] == 0 && solution[0][2] == 0) {
		return false;
	}

	return true;
}