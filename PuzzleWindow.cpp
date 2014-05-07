#include "PuzzleWindow.h"

#include <iostream>

#include <QComboBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QWidget>

#include <string>

using namespace std;

PuzzleWindow::PuzzleWindow() : model(new QStringListModel(this)) {
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
	
	connect(loadButton, SIGNAL(clicked()), this, SLOT(loadSlot()));
	connect(checkButton, SIGNAL(clicked()), this, SLOT(checkSlot()));
	connect(hintButton, SIGNAL(clicked()), this, SLOT(hintSlot()));
	connect(solveButton, SIGNAL(clicked()), this, SLOT(solveSlot()));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(resetSlot()));
	connect(undoButton, SIGNAL(clicked()), this, SLOT(undoSlot()));
	
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

void PuzzleWindow::loadSlot() {
	QMessageBox::information(this, "title", "info");
	
	QStringList list;
	list << "2";
	
	model->setStringList(list);
	
	for(int i = 0; i < 3; ++i) {
		for(int j = 0; j < 3; ++j) {
			QComboBox* box = new QComboBox;
			box->setModel(model);
			gridLayout->addWidget(box, i, j);
		}
	}
}
void PuzzleWindow::checkSlot() {
	QString filename = QFileDialog::getOpenFileName(this, "Open File", "", "Files (*.*)");
	QMessageBox::information(this, "title", filename);
}
void PuzzleWindow::hintSlot() {
	QMessageBox::information(this, "title", "info");
}

void PuzzleWindow::solveSlot() {
	QMessageBox::information(this, "title", "info");
}

void PuzzleWindow::resetSlot() {
	QMessageBox::information(this, "title", "info");
}

void PuzzleWindow::undoSlot() {
	QMessageBox::information(this, "title", "info");
}

