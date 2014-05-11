/**
  * Cell.h
  *
  * Author: Lane Lawley
  * Date: April 9th, 2014
  *
  * Description: This class represents a single cell on the Kakuro puzzle board.
  * 		 A cell is either a sum cell, which dictates the sum of all cells below and/or to the right of it, or a value cell, which contains a value to be summed.
  *		 Each cell caches its possible values, and these are updated lazily by the configuration when generating successors.
  */

#ifndef KCELL_H
#define KCELL_H

#include <array>
#include <iostream>
#include <vector>

class Cell {
	private:
		bool m_isValueCell;
		
		// For value cells
		int m_value;
		int m_numPossibleValues;
		std::array<bool, 9> m_possibleValues;
		bool m_isFixed;
		
		// For sum cells
		int m_rightSum;
		int m_downSum;
	
	public:
		// Constructor for value cells
		Cell(int value, bool isFixed);
		
		// Constructor for sum cells
		// 0 values mean no restriction
		Cell(int downSum, int rightSum);
	
	public:
		bool isValueCell() const;
		
		// For value cells
		
		// Current value of the cell
		int value() const;
		
		// Whether or not the cell is pre-set on loading
		bool isFixed() const;
		
		// The number of possible values for the cell
		int numPossibleValues();
		
		// The possible values for the cell
		std::array<bool, 9> possibleValues() const;
		
		// Sets the value for the cell
		void setValue(int value);
		
		// Sets the possible values for the cell
		void setPossibleValues(std::array<bool, 9> possibleValues);
		
		// For sum cells
		
		// The sum constraint for all value cells in the group to the right of the cell
		int rightSum() const;
		
		// The sum constraint for all value cells in the group below the cell
		int downSum() const;
	
	public:
		// Output operator
		friend std::ostream& operator<<(std::ostream& os, const Cell& c);
		bool operator==(const Cell& other) const;
};

#endif

