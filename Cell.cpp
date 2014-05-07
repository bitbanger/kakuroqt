/**
  * Cell.cpp
  *
  * Author: Lane Lawley
  * Date: April 9th, 2014
  *
  * Description: This is an implementation of Cell.h.
  * 		 For an explanation of the class, please consult that file.
  */

#include "Cell.h"

#include <vector>

using namespace std;

Cell::Cell(int value) : 
	m_isValueCell(true), m_value(value), m_rightSum(-1), 
	m_downSum(-1), m_numPossibleValues(-1) {}

Cell::Cell(int downSum, int rightSum) : 
	m_isValueCell(false), m_value(-1), m_rightSum(rightSum), 
	m_downSum(downSum), m_numPossibleValues(-1) {}

bool Cell::isValueCell() const {
	return m_isValueCell;
}

// For value cells
int Cell::value() const {
	return m_value;
}

int Cell::numPossibleValues() {
	if(false || m_numPossibleValues != -1) {
		return m_numPossibleValues;
	} else {
		int num = 0;
		for(bool b : m_possibleValues) {
			if(b) ++num;
		}
		
		// Cache the result
		m_numPossibleValues = num;
		
		return num;
	}
}

array<bool, 9> Cell::possibleValues() const {
	return m_possibleValues;
}

void Cell::setValue(int value) {
	m_value = value;
}

void Cell::setPossibleValues(array<bool, 9> possibleValues) {
	m_possibleValues = possibleValues;
	
	// Invalidate our numPossibleValues cache
	m_numPossibleValues = -1;
}

// For sum cells
int Cell::rightSum() const {
	return m_rightSum;
}

int Cell::downSum() const {
	return m_downSum;
}

ostream& operator<<(ostream& os, const Cell& c) {
	if(c.m_isValueCell) {
		os << c.m_value;
	} else {
		if(c.m_downSum != 0 || c.m_rightSum != 0) {
			os << c.m_downSum << "\\" << c.m_rightSum;
		}
	}
	
	return os;
}

