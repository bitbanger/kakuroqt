/**
  * PuzzleWindow.cpp
  *
  * Author: Lane Lawley
  * Date: May 11th, 2014
  *
  * Description: This is an implementation of PuzzleWindow.h.
  * 		 For an explanation of the class, please consult that file.
  */

#include "Cell.h"
#include "KakuroConfig.h"
#include "PuzzleWindow.h"
#include "Solver.h"

#include <iostream>

#include <QApplication>
#include <QComboBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSizePolicy>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QWidget>

#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

PuzzleWindow::PuzzleWindow() : comboModel(new QStringListModel(this)) {
	// Set up main layout
	mainLayout = new QVBoxLayout;
	
	this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	
	// Set up the combobox model
	comboList = new QStringList;
	*comboList << QString("-");
	for(int i = 1; i <= 9; ++i) {
		*comboList << QString(to_string(i).c_str());
	}
	
	comboModel->setStringList(*comboList);
	
	// Set up a horizontal layout for the buttons
	buttonBox = new QGroupBox;
	
	QHBoxLayout* buttonLayout = new QHBoxLayout;
	
	// Initialize the buttons and add them to their box
	loadButton = new QPushButton("Load", this);
	checkButton = new QPushButton("Check", this);
	hintButton = new QPushButton("Hint", this);
	solveButton = new QPushButton("Solve", this);
	resetButton = new QPushButton("Reset", this);
	helpButton = new QPushButton("Help", this);
	
	connect(loadButton, SIGNAL(clicked()), this, SLOT(loadSlot()));
	connect(checkButton, SIGNAL(clicked()), this, SLOT(checkSlot()));
	connect(hintButton, SIGNAL(clicked()), this, SLOT(hintSlot()));
	connect(solveButton, SIGNAL(clicked()), this, SLOT(solveSlot()));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(resetSlot()));
	connect(helpButton, SIGNAL(clicked()), this, SLOT(helpSlot()));
	
	buttonLayout->addWidget(loadButton);
	buttonLayout->addWidget(checkButton);
	buttonLayout->addWidget(hintButton);
	buttonLayout->addWidget(solveButton);
	buttonLayout->addWidget(resetButton);
	buttonLayout->addWidget(helpButton);
	
	buttonBox->setLayout(buttonLayout);
	
	// Initialize the grid
	gridBox = new QGroupBox;
	
	gridLayout = new QGridLayout;
	
	gridBox->setLayout(gridLayout);
	
	// Add everything to the main layout
	mainLayout->addWidget(buttonBox);
	mainLayout->addWidget(gridBox);
	
	setLayout(mainLayout);
	
	setWindowTitle("Kakuro - Lane Lawley");
}

void PuzzleWindow::displayKakuroConfig(const KakuroConfig& c) {
	while(gridLayout->count() > 0) {
		QWidget* widget = gridLayout->itemAt(0)->widget();
		gridLayout->removeWidget(widget);
		delete widget;
	}
	
	delete gridLayout;
	gridLayout = new QGridLayout;
	gridBox->setLayout(gridLayout);
	
	vector<vector<Cell>> board = c.getBoard();
	for(unsigned int i = 0; i < board.size(); ++i) {
		for(unsigned int j = 0; j < board.at(0).size(); ++j) {
			Cell& c = board.at(i).at(j);
			
			if(c.isValueCell()) {
				QComboBox* combo = new QComboBox;
				combo->setModel(comboModel);
				combo->setCurrentIndex(c.value());
				
				if(c.isFixed()) {
					combo->setEnabled(false);
				}
				
				connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxUpdatedSlot()));
				gridLayout->addWidget(combo, i, j);
			} else {
				ostringstream oss;
				oss << c;
				gridLayout->addWidget(new QLabel(QString(oss.str().c_str())), i, j);
			}
		}
	}
	
	QApplication::processEvents();
	
	this->resize(0, 0);
}

KakuroConfig PuzzleWindow::configFromDisplay() {
	vector<vector<Cell>> currentBoard = currentConfig->getBoard();	
	int cols = currentBoard.size();
	int rows = currentBoard.at(0).size();
	
	vector<vector<Cell>> newBoard(cols, vector<Cell>());
	
	for(int i = 0; i < cols; ++i) {
		for(int j = 0; j < rows; ++j) {
			if(currentBoard.at(i).at(j).isValueCell()) {
				QComboBox* combo = (QComboBox*)(gridLayout->itemAtPosition(i, j)->widget());
				int gridVal = combo->currentIndex();
				newBoard.at(i).push_back(Cell(gridVal, !combo->isEnabled()));
			} else {
				newBoard.at(i).push_back(currentBoard.at(i).at(j));
			}
		}
	}
	
	return KakuroConfig(newBoard, false);
}

void PuzzleWindow::loadSlot() {
	string filename = QFileDialog::getOpenFileName(this, "Open File", "", "Input Files (*.*)").toStdString();
	
	fstream file;
	file.open(filename);
	
	if(!file.good()) {
		file.close();
		return;
	}
	
	file.close();
	
	currentConfig = make_shared<KakuroConfig>(filename);
	
	initialConfig = make_shared<KakuroConfig>(*currentConfig);
	
	this->displayKakuroConfig(*currentConfig);
}

void PuzzleWindow::checkSlot() {
	if(currentConfig == nullptr) {
		return;
	}
	
	Solver<KakuroConfig> solver(currentConfig);
	
	if(solver.isFailure()) {
		QMessageBox::information(this, "Solvable?", "The puzzle is not solvable from its current state.");
	} else {
		if(currentConfig->isGoal()) {
			QMessageBox::information(this, "Solvable?", "The puzzle is solved!");
		} else {
			QMessageBox::information(this, "Solvable?", "The puzzle is solvable from its current state!");
		}
	}
}

void PuzzleWindow::hintSlot() {
	if(currentConfig == nullptr) {
		return;
	}
	
	Solver<KakuroConfig> solver(currentConfig);
	
	if(solver.isFailure()) {
		QMessageBox::information(this, "Invalid", "The current puzzle cannot reach a solution. Why not try another approach?");
	} else {
		vector<shared_ptr<KakuroConfig>> path = solver.getSolutionPath();
		
		if(path.size() == 1) {
			QMessageBox::information(this, "Invalid", "No hints available; you already won!");
			return;
		}
		
		currentConfig = path.at(path.size() - 2);
		
		currentConfig->setParent(nullptr);
		
		displayKakuroConfig(*currentConfig);
	}
}

void PuzzleWindow::solveSlot() {
	if(currentConfig == nullptr) {
		return;
	}
	
	Solver<KakuroConfig> solver(currentConfig);
	
	if(solver.isFailure()) {
		QMessageBox::information(this, "Invalid", "The current puzzle cannot reach a solution. Why not try another approach, or reset and click solve again?");
	} else {
		currentConfig = solver.getSolutionPath().front();
		displayKakuroConfig(*currentConfig);
		currentConfig->setParent(nullptr);
	}
}

void PuzzleWindow::resetSlot() {
	if(currentConfig == nullptr) {
		return;
	}
	
	currentConfig = make_shared<KakuroConfig>(*initialConfig);
	displayKakuroConfig(*currentConfig);
}

void PuzzleWindow::helpSlot() {
	string helpString = R"raw(Kakuro has two kinds of cells: sum cells and value cells.

Sum cells are non-selectable and have backslashes in them.
The number before the backslash is a down sum: all value cells directly below the sum cell must sum to that.
The number after the backslash is a right sum, and works the same as the down sum, except with value cells to the right.

To solve a Kakuro puzzle, make sure all right sums and all down sums are fulfilled.
You must also fill in all selectable boxes such that any straight line of adjacent value cells contains no duplicates and no dashes.

Good luck!)raw";
	
	QMessageBox::information(this, "Help", QString(helpString.c_str()));
}

void PuzzleWindow::comboBoxUpdatedSlot() {
	currentConfig = make_shared<KakuroConfig>(configFromDisplay());
}

