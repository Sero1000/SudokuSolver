#include "SudokuSolver.h"
#include "SudokuWidget.h"
#include "DLX/DLX.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QMenuBar>
#include <QFileDialog>
#include <QTextStream>

SudokuSolver::SudokuSolver(QWidget* parent)
	: QWidget(parent), m_sudokuWidget(new SudokuWidget(this))
{
	QPushButton* solveButton = new QPushButton("Solve");
	QPushButton* clearButton = new QPushButton("Clear");
	QPushButton* openPushButton = new QPushButton("Open");

	QMenuBar* menuBar = new QMenuBar();

	connect(solveButton, &QPushButton::pressed, this, &SudokuSolver::onSolve);
	connect(clearButton, &QPushButton::pressed, this, &SudokuSolver::onClear);
	connect(openPushButton, &QPushButton::pressed, this, &SudokuSolver::onOpen);

	setFixedWidth(480);
	QVBoxLayout* vBoxLayout = new QVBoxLayout();
	setLayout(vBoxLayout);
	vBoxLayout->addWidget(m_sudokuWidget);
	vBoxLayout->addWidget(solveButton);
	vBoxLayout->addWidget(clearButton);
	vBoxLayout->addWidget(openPushButton);
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

void SudokuSolver::onOpen() {
	std::vector<std::vector<short>> input(9,std::vector<short>(9));

	QString fileDir = QFileDialog::getOpenFileName(this,QString("Open File"));
	if (!fileDir.isNull()) {
		QFile file(fileDir);
		file.open(QIODevice::ReadOnly);

		QTextStream stream(&file);

		for (int i = 0; i < input.size(); ++i) {
			for (int j = 0; j < input[i].size(); ++j) {
				QChar value;
				stream >> value;
				input[i][j] = QString(value).toShort();
			}
		}
	}

	m_sudokuWidget->SetNumbers(input);
}