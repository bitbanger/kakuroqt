#ifndef PUZZLE_H
#define PUZZLE_H

#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QWidget>

class PuzzleWindow : public QWidget {
	Q_OBJECT
	
	private:
		QVBoxLayout* mainLayout;
		
		QGridLayout* gridLayout;
		
		QPushButton* loadButton;
		QPushButton* checkButton;
		QPushButton* hintButton;
		QPushButton* solveButton;
		QPushButton* resetButton;
		QPushButton* undoButton;
	
	private slots:
		void slotButton1();
		void slotButton2();
	
	public:
		PuzzleWindow();
};

#endif

