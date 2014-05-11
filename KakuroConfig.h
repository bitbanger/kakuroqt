/**
  * KakuroConfig.h
  *
  * Author: Lane Lawley
  * Date: May 11th, 2014
  *
  * Description: This class represents a configuration of the Kakuro puzzle.
  *		 This configuration is used by the backtracking solver.
  *		 A configuration is either a goal or generates child configurations.
  *		 The cell with the least possible values is picked to update for all successors.
		 Only methods used by the backtracker and constructors are made public.
		 No inheritance is used because the solver is templated.
  */

#ifndef KAKURO_H
#define KAKURO_H

#include "Cell.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class KakuroConfig {
	private:
		unsigned m_deltaI, m_deltaJ;
		bool m_isGoal;
		bool m_shouldDelta;
		
		std::vector<std::vector<Cell>> m_board;
		std::shared_ptr<KakuroConfig> m_parent;
	
	public:
		// Constructor for the root config read in from a file
		// shouldDelta is set to true for path mode to clarify what cell was updated at each step
		KakuroConfig(std::vector<std::vector<Cell>> board, bool shouldDelta);
		
		// Constructor for creating a board from a filename
		KakuroConfig(std::string filename);
		
		// Copy constructor used to generate successors
		KakuroConfig(const KakuroConfig& other) = default;
	
	private:
		// Whether or not the config is the goal config (re-calculates; fairly slow)
		bool slowIsGoal() const;
		
	public:
		// Whether or not the config is the goal config (represents a solved puzzle)
		bool isGoal() const;
		
		// The successors of the config, which are all possible values for the cell with the fewest possible values
		std::vector<std::shared_ptr<KakuroConfig>> getSuccessors();
		
		// Sets the parent config of the config for path mode enumeration
		void setParent(const std::shared_ptr<KakuroConfig>& parent);
		
		// The parent config of the config
		std::shared_ptr<KakuroConfig> getParent();
		
		// The internal representation of the configuration's board
		std::vector<std::vector<Cell>> getBoard() const;
	
	public:
		// Output operator
		friend std::ostream& operator<<(std::ostream& os, const KakuroConfig& c);
};

#endif

