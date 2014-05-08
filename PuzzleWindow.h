#ifndef PUZZLE_H
#define PUZZLE_H

#include "KakuroConfig.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QWidget>

#include <memory>

class PuzzleWindow : public QWidget {
	Q_OBJECT
	
	private:
		QStringListModel* comboModel;
		QStringList* comboList;
		
		QVBoxLayout* mainLayout;
		
		QGroupBox* buttonBox;
		QGroupBox* gridBox;
		
		QGridLayout* gridLayout;
		
		QPushButton* loadButton;
		QPushButton* checkButton;
		QPushButton* hintButton;
		QPushButton* solveButton;
		QPushButton* resetButton;
		QPushButton* undoButton;
	
	private:
		std::shared_ptr<KakuroConfig> currentConfig;
	
	private:
		void displayKakuroConfig(const KakuroConfig& c);
		KakuroConfig configFromDisplay();
		
		bool isSolvable();
	
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

