#include "PuzzleWindow.h"

#include <iostream>

#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

using namespace std;

PuzzleWindow::PuzzleWindow() {
	// Set up main layout
	mainLayout = new QVBoxLayout;
	
	// Set up a horizontal layout for the buttons
	QGroupBox* buttonBox = new QGroupBox;
	
	QHBoxLayout* buttonLayout = new QHBoxLayout;
	
	// Initialize the buttons and add them to their box
	loadButton = new QPushButton("Load", this);
	checkButton = new QPushButton("Check", this);
	hintButton = new QPushButton("Hint", this);
	solveButton = new QPushButton("Solve", this);
	resetButton = new QPushButton("Reset", this);
	undoButton = new QPushButton("Undo", this);
	
	buttonLayout->addWidget(loadButton);
	buttonLayout->addWidget(checkButton);
	buttonLayout->addWidget(hintButton);
	buttonLayout->addWidget(solveButton);
	buttonLayout->addWidget(resetButton);
	buttonLayout->addWidget(undoButton);
	
	buttonBox->setLayout(buttonLayout);
	
	// Initialize the grid
	QGroupBox* gridBox = new QGroupBox;
	
	gridLayout = new QGridLayout;
	
	gridBox->setLayout(gridLayout);
	
	// Add everything to the main layout
	mainLayout->addWidget(buttonBox);
	mainLayout->addWidget(gridBox);
	
	setLayout(mainLayout);
	
	setWindowTitle("Kakuro");
}

void PuzzleWindow::slotButton1() {
	cout << "Button 1 hit!" << endl;
}

void PuzzleWindow::slotButton2() {
	cout << "Button 2 hit!" << endl;
}
