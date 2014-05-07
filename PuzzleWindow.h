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
		
		QPushButton* button1;
		QPushButton* button2;
	
	private slots:
		void slotButton1();
		void slotButton2();
	
	public:
		PuzzleWindow();
};

#endif

