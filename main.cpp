#include <QApplication>

#include "PuzzleWindow.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	PuzzleWindow pw;
	pw.show();
	
	return app.exec();
}

