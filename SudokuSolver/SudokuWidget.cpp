#include "SudokuWidget.h"

#include <QPainter>
#include <QGridLayout>
#include <QLineEdit>
#include <QRegularExpressionValidator>

SudokuWidget::SudokuWidget(QWidget* parent) 
	:QWidget(parent) , m_lineEdits(QVector<QVector<QLineEdit*>>(9,QVector<QLineEdit*>(9))){
	
	setMinimumSize(QSize(460, 460));
	setMaximumSize(QSize(460, 460));

	QGridLayout* gridLayout = new QGridLayout();
	gridLayout->setSpacing(0);
	gridLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(gridLayout);

	QRegularExpressionValidator* validator = new QRegularExpressionValidator(QRegularExpression("[0-9]"));

	for (int row = 0; row < 9; ++row) {
		for (int column = 0; column < 9; ++column) {
			QLineEdit* lineEdit = new QLineEdit();
			m_lineEdits[row][column] = lineEdit;

			lineEdit->setAlignment(Qt::AlignHCenter);
			lineEdit->setValidator(validator);
			lineEdit->setFixedSize(QSize(50, 50));

			QFont newFont(lineEdit->font());
			newFont.setPointSize(newFont.pointSize() * 3);
			lineEdit->setFont(newFont);

			gridLayout->addWidget(lineEdit, row, column);
		}
	}
}

void SudokuWidget::SetNumbers(const std::vector<std::vector<short>>& solution) {
	for (int row = 0; row < solution.size(); ++row) {
		for (int column = 0; column < solution[row].size(); ++column) {
			m_lineEdits[row][column]->setText(QString::number(solution[row][column]));
		}
	}
}

QVector<QVector<short>> SudokuWidget::Inputs() const {
	QVector<QVector<short>> result(9,QVector<short>(9));

	for (int row = 0; row < 9; ++row) {
		for (int column = 0; column < 9; ++column) {
			if (!m_lineEdits[row][column]->text().isEmpty())
				result[row][column] = m_lineEdits[row][column]->text().toShort();
			else
				result[row][column] = -1;
		}
	}

	return result;
}

void SudokuWidget::paintEvent(QPaintEvent* e) {
	QPainter painter(this);
	painter.setPen(Qt::blue);

	double height = rect().height();
	double width = rect().width();

	painter.drawLine(rect().topLeft(), rect().topRight());
	painter.drawLine(QPointF(0, height / 3), QPointF(width, height / 3));
	painter.drawLine(QPointF(0, (2 * height) / 3), QPointF(width, (2 * height) / 3));
	painter.drawLine(rect().bottomLeft(), rect().bottomRight());

	painter.drawLine(rect().topLeft(), rect().bottomLeft());
	painter.drawLine(QPointF(width / 3, 0), QPointF(width / 3, height));
	painter.drawLine(QPointF((2 * width) / 3, 0), QPointF((2 * width) / 3, height));
	painter.drawLine(rect().topRight(), rect().bottomRight());
}

void SudokuWidget::ClearInputs() {
	for (int row = 0; row < m_lineEdits.size(); ++row) {
		for (int column = 0; column < m_lineEdits[row].size(); ++column) {
			m_lineEdits[row][column]->setText(QString());
		}
	}
}