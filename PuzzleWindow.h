/**
  * PuzzleWindow.h
  *
  * Author: Lane Lawley
  * Date: May 11th, 2014
  *
  * Description: This class represents the main and solve window of the Kakuro puzzle game.
  * 		 It contains UI elements to represent an interactive game board.
  *		 It also contains utility functions for converting UI to config models and vice-versa.
  */

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
		// String model information for comboboxes
		QStringListModel* comboModel;
		QStringList* comboList;
		
		// The window's main layout
		QVBoxLayout* mainLayout;
		
		// Boxes for UI components
		QGroupBox* buttonBox;
		QGroupBox* gridBox;
		
		// Grid for the game's comboboxes
		QGridLayout* gridLayout;
		
		// All the window's buttons
		QPushButton* loadButton;
		QPushButton* checkButton;
		QPushButton* hintButton;
		QPushButton* solveButton;
		QPushButton* resetButton;
		QPushButton* helpButton;
	
	private:
		// Configuration pointers for keeping track of game state
		std::shared_ptr<KakuroConfig> initialConfig;
		std::shared_ptr<KakuroConfig> currentConfig;
	
	private:
		// Functions to convert between view/controller and model game boards
		void displayKakuroConfig(const KakuroConfig& c);
		KakuroConfig configFromDisplay();
	
	private slots:
		// Slots to hook into the buttons
		void loadSlot();
		void checkSlot();
		void hintSlot();
		void solveSlot();
		void resetSlot();
		void helpSlot();
		
		// A slot for combo box updating
		void comboBoxUpdatedSlot();
		
	public:
		// Our simple yet effective constructor
		PuzzleWindow();
};

#endif

