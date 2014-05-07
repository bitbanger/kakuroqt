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
	QGroupBox* buttonBox = new QGroupBox("Buttons");
	
	QHBoxLayout* buttonLayout = new QHBoxLayout;
	
	// Initialize the buttons and add them to their box
	button1 = new QPushButton("Button #1", this);
	button2 = new QPushButton("Button #2", this);
	
	buttonLayout->addWidget(button1);
	buttonLayout->addWidget(button2);
	
	buttonBox->setLayout(buttonLayout);
	
	// Initialize the grid
	QGroupBox* gridBox = new QGroupBox("Grid");
	
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
