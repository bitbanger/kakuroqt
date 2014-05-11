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
	
	connect(loadButton, SIGNAL(clicked()), this, SLOT(loadSlot()));
	connect(checkButton, SIGNAL(clicked()), this, SLOT(checkSlot()));
	connect(hintButton, SIGNAL(clicked()), this, SLOT(hintSlot()));
	connect(solveButton, SIGNAL(clicked()), this, SLOT(solveSlot()));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(resetSlot()));
	
	buttonLayout->addWidget(loadButton);
	buttonLayout->addWidget(checkButton);
	buttonLayout->addWidget(hintButton);
	buttonLayout->addWidget(solveButton);
	buttonLayout->addWidget(resetButton);
	
	buttonBox->setLayout(buttonLayout);
	
	// Initialize the grid
	gridBox = new QGroupBox;
	
	gridLayout = new QGridLayout;
	
	gridBox->setLayout(gridLayout);
	
	// Add everything to the main layout
	mainLayout->addWidget(buttonBox);
	mainLayout->addWidget(gridBox);
	
	setLayout(mainLayout);
	
	setWindowTitle("Kakuro");
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
				newBoard.at(i).push_back(gridVal);
			} else {
				newBoard.at(i).push_back(currentBoard.at(i).at(j));
			}
		}
	}
	
	return KakuroConfig(newBoard, false);
}

bool PuzzleWindow::isSolvable() {
	Solver<KakuroConfig> solver(currentConfig);
	
	return !solver.isFailure();
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
	string msg = (isSolvable() ? "Solvable!" : "Not solvable :(");
	
	QMessageBox::information(this, "Solvable?", QString(msg.c_str()));
}

void PuzzleWindow::hintSlot() {
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
	currentConfig = make_shared<KakuroConfig>(*initialConfig);
	displayKakuroConfig(*currentConfig);
}

void PuzzleWindow::comboBoxUpdatedSlot() {
	currentConfig = make_shared<KakuroConfig>(configFromDisplay());
}

