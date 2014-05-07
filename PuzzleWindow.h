#ifndef PUZZLE_H
#define PUZZLE_H


#include <QPushButton>
#include <QGridLayout>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QWidget>

class PuzzleWindow : public QWidget {
	Q_OBJECT
	
	private:
		QStringListModel* model;
		
		QVBoxLayout* mainLayout;
		
		QGridLayout* gridLayout;
		
		QPushButton* loadButton;
		QPushButton* checkButton;
		QPushButton* hintButton;
		QPushButton* solveButton;
		QPushButton* resetButton;
		QPushButton* undoButton;
	
	private slots:
		void loadSlot();
		void checkSlot();
		void hintSlot();
		void solveSlot();
		void resetSlot();
		void undoSlot();
		
	public:
		PuzzleWindow();
};

#endif

