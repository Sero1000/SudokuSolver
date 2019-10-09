#ifndef SUDOKUWIDGET_H
#define SUDOKUWIDGET_H

#include <QWidget>

class QLineEdit;

class SudokuWidget : public QWidget {
public:
	SudokuWidget(QWidget* parent = nullptr);

	QVector<QVector<short>> Inputs() const;

	void SetNumbers(const std::vector<std::vector<short>>& solution);
	void ClearInputs();
protected:
	void paintEvent(QPaintEvent* e) override;
private:
	QVector<QVector<QLineEdit*>> m_lineEdits;
};

#endif