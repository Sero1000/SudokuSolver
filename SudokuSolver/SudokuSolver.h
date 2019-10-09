#ifndef SUDOKUSOLVER_H
#define SUDOKUSOLVER_H

#include <QWidget>

class SudokuWidget;

class SudokuSolver : public QWidget
{
	Q_OBJECT

public:
	SudokuSolver(QWidget *parent = Q_NULLPTR);
private slots:
	void onSolve();
	void onClear();
private:
	SudokuWidget* m_sudokuWidget;
};

#endif