/**
  * KakuroConfig.cpp
  *
  * Author: Lane Lawley
  * Date: April 9th, 2014
  *
  * Description: This is an implementation of KakuroConfig.h.
  * 		 For an explanation of the class, please consult that file.
  */

#include "Cell.h"
#include "KakuroConfig.h"
#include "Partitioner.h"

#include <memory>
#include <string>
#include <vector>

using namespace std;

array<bool, 9> availableValues(const vector<vector<Cell>>& board, int ver, int hor, bool vertical) {
	int ptr(vertical ? ver : hor);
	
	while(board.at(vertical ? --ptr : ver).at(vertical ? hor : --ptr).isValueCell());
	
	int sum = (vertical ? board.at(ptr).at(hor).downSum() : board.at(ver).at(ptr).rightSum());
	
	int avail(0);
	int totalUsed(0);
	int numNeighbors(0);
	array<bool, 9> used{};
	
	auto bound = vertical ? board.size() : board.at(0).size();
	while(ptr+1 < bound) {
		const Cell& c = board[vertical ? ++ptr : ver][vertical ? hor : ++ptr];
		if(!c.isValueCell()) break;
		++numNeighbors;
		if(c.value() > 0) {
			totalUsed += c.value();
			used[c.value() - 1] = true;
		} else if(c.value() == 0) {
			++avail;
		} else {
			cout << c << endl;
		}
	}
	
	return Partitioner::getInstance().possibleValues(sum - totalUsed, avail, used);
}

bool KakuroConfig::slowIsGoal() const {
	for(int i = 0; i < m_board.size(); ++i) {
		for(int j = 0; j < m_board.at(0).size(); ++j) {
			const Cell& c = m_board.at(i).at(j);
			
			if(!c.isValueCell()) {
				if(c.downSum() > 0) {
					int verPtr(i);
					int sum(0);
					array<bool, 9> used{};
					
					while(verPtr+1 < m_board.size()) {
						const Cell& vc = m_board.at(++verPtr).at(j);
						
						if(!vc.isValueCell()) break;
						
						int val = vc.value();
						
						if(val == 0) return false;
						
						sum += val;
						
						if(used[val - 1]) return false;
						
						used[val - 1] = true;
					}
					
					if(sum != c.downSum()) return false;
				}
				
				if(c.rightSum() > 0) {
					int horPtr(j);
					int sum(0);
					array<bool, 9> used{};
					
					while(horPtr+1 < m_board.at(0).size()) {
						const Cell& vc = m_board.at(i).at(++horPtr);
						
						if(!vc.isValueCell()) break;
						
						int val = vc.value();
						
						if(val == 0) return false;
						
						sum += val;
						
						if(used[val - 1]) return false;
						
						used[val - 1] = true;
					}
					
					if(sum != c.rightSum()) return false;
				}
			}
		}
	}
	
	return true;
}

KakuroConfig::KakuroConfig(vector<vector<Cell>> board, bool shouldDelta) : 
	m_isGoal(false), m_board(board), m_parent(nullptr), m_deltaI(-1), m_deltaJ(-1), m_shouldDelta(shouldDelta) {
	// If our root config is the goal, don't do any more work
	if(slowIsGoal()) {
		m_isGoal = true;
		return;
	}
	
	// Compute initial possible values for all cells
	
	// The possible values for a cell are 
	// the intersection of the integer partition sets 
	// of each dimensional neighbor group with target sums 
	// determined by the sum cells
	for(int i = 0; i < m_board.size(); ++i) {
		for(int j = 0; j < m_board.size(); ++j) {
			Cell& cursor = m_board[i][j];
			
			if(cursor.isValueCell()) {
				// Do two walks, horizontally and vertically
				// Count the number of peers and establish target sums
				// Then intersect possible values
				
				int horPtr(j), verPtr(i);
				
				array<bool, 9> horPossibles = availableValues(m_board, i, j, false);
				array<bool, 9> verPossibles = availableValues(m_board, i, j, false);
				
				array<bool, 9> cellPossibles = Partitioner::getInstance().intersection(horPossibles, verPossibles);
				
				cursor.setPossibleValues(cellPossibles);
			}
		}
	}
}

bool contains(const string& str, char search) {
	return str.find(search) != string::npos;
}

vector<int> splitRuleCell(const string& str) {
	vector<int> result;
	
	stringstream ss(str);
	
	string chunk;
	while(getline(ss, chunk, '\\')) {
		result.push_back(atoi(chunk.c_str()));
	}
	
	return result;
}

KakuroConfig::KakuroConfig(string filename) : m_isGoal(false), m_parent(nullptr), m_deltaI(-1), m_deltaJ(-1), m_shouldDelta(false) {
	fstream file;
	file.open(filename);
	
	if(!file.good()) {
		file.close();
		cerr << "FATAL ERROR: bad file given to config when it should have been checked first" << endl;
		return;
	}
	
	int x, y;
	file >> x;
	file >> y;
	
	if(x <= 0 || y <= 0) {
                cerr << "INVALID INPUT: " << endl;
                cerr << "\tThe first two tokens must be positive integers representing the dimensions of the board." << endl;
                cerr << "\tThe first dimension is vertical and the second horizontal." << endl;
                return;
        }
	
	vector<vector<Cell>> board(x, vector<Cell>());
	
	for(int i = 0; i < x; ++i) {
		for(int j = 0; j < y; ++j) {
			string cell;
			if(!(file >> cell)) {
				cerr << "INVALID INPUT: " << endl;
				cerr << "\tNot enough cells to read given the " << x << " by " << y << " dimensions from the input file!" << endl;
				return ;
			}
			if(contains(cell, '\\')) {
				vector<int> rules = splitRuleCell(cell);
				Cell ruleCell(rules[0], rules[1]);
				board[i].push_back(ruleCell);
			} else {
				Cell valCell(atoi(cell.c_str()));
				board[i].push_back(valCell);
			}

		}
	}

	file.close();
	
	m_board = board;
}

bool KakuroConfig::isGoal() const {
	return m_isGoal;
}

vector<shared_ptr<KakuroConfig>> KakuroConfig::getSuccessors() {
	vector<shared_ptr<KakuroConfig>> successors;
	
	// PHASE 1: 
	// Find the cell with the fewest possible values; this will be the mutated cell for our next branch
	int competitors(0);
	int fewestHor(0), fewestVer(0);
	int fewestNum(-1);
	array<bool, 9> values{};
	
	for(int i = 0; i < m_board.size(); ++i) {
		for(int j = 0; j < m_board[0].size(); ++j) {
			Cell& candidate = m_board.at(i).at(j);
			if(candidate.value() == 0) {
				++competitors;
				
				if(candidate.numPossibleValues() >= fewestNum && fewestNum != -1) continue;
				
				if(candidate.numPossibleValues() == 0) continue;
				
				fewestNum = candidate.numPossibleValues();
				fewestHor = j;
				fewestVer = i;
				values = candidate.possibleValues();
			}
		}
	}
	
	// PHASE 2:
	// Begin generating successors, updating the values of neighbors as we try new candidate values
	
	// Put candidate values into a vector
	vector<int> candVals;
	for(int i = 0; i < 9; ++i) if(values[i]) candVals.push_back(i + 1);
	
	for(int candVal : candVals) {
		shared_ptr<KakuroConfig> succ = make_shared<KakuroConfig>(*this);
			
		// Change the candidate cell to the candidate value
		succ->m_board[fewestVer][fewestHor] = candVal;
		
		// Update delta metadata for better printouts
		succ->m_deltaI = fewestVer;
		succ->m_deltaJ = fewestHor;
		
		// Now update the possible values of all neighbors
		
		// ...unless we won, in which case we're okay!
		// Make sure we actually won, though, in case the initial values were bad.
		if(competitors == 1 && succ->slowIsGoal()) {
			succ->m_isGoal = true;
			successors.push_back(succ);
			return successors;
		}
		
		// First, find the sum cells that initiate the group
		int horPtr(fewestHor), verPtr(fewestVer);
		
		while(succ->m_board[fewestVer][--horPtr].isValueCell());
		while(succ->m_board[--verPtr][fewestHor].isValueCell());
		
		int downSum = succ->m_board[verPtr][fewestHor].downSum();
		int rightSum = m_board[fewestVer][horPtr].rightSum();
		
		// Now count what's already used in each peer group
		int horPtr2(horPtr), verPtr2(verPtr);
		int numHorPeers(0), numVerPeers(0);
		array<bool, 9> usedHor{};
		int curHorSum(0), curVerSum(0);
		array<bool, 9> usedVer{};
		while(horPtr2+1 < succ->m_board[0].size() && succ->m_board[fewestVer][++horPtr2].isValueCell()) {
			int val = succ->m_board[fewestVer][horPtr2].value();
			if(val > 0) {
				usedHor[val - 1] = true;
				curHorSum += val;
			} else ++numHorPeers;
		}
		while(verPtr2+1 < succ->m_board.size() && succ->m_board[++verPtr2][fewestHor].isValueCell()) {
			int val = succ->m_board[verPtr2][fewestHor].value();
			if(val > 0) {
				usedVer[val - 1] = true;
				curVerSum += val;
			} else ++numVerPeers;
			
		}
		
		// Get new possible values with this info
		array<bool, 9> possibleHor = Partitioner::getInstance().possibleValues(rightSum - curHorSum, numHorPeers, usedHor);
		array<bool, 9> possibleVer = Partitioner::getInstance().possibleValues(downSum - curVerSum, numVerPeers, usedVer);
		
		// Set new possible vals for all horizontal and vertical peers
		while(horPtr+1 < succ->m_board[0].size()) {
			Cell& c = succ->m_board[fewestVer][++horPtr];
			if(c.isValueCell()) {
				if(c.value() == 0) {
					array<bool, 9> possibleVer = availableValues(succ->m_board, fewestVer, horPtr, true);
					array<bool, 9> possibleVals = Partitioner::getInstance().intersection(possibleHor, possibleVer);
					c.setPossibleValues(possibleVals);
				}
			} else break;
		}
		
		while(verPtr + 1 < succ->m_board.size()) {
			Cell& c = succ->m_board[++verPtr][fewestHor];
			if(c.isValueCell()) {
				if(c.value() == 0) {
					array<bool, 9> possibleHor = availableValues(succ->m_board, verPtr, fewestHor, false);
					array<bool, 9> possibleVals = Partitioner::getInstance().intersection(possibleHor, possibleVer);
					c.setPossibleValues(possibleVals);
				}
			} else break;
		}
		
		// All good; add the candidate to the list!
		successors.push_back(succ);
	}
	
	
	return successors;
}

void KakuroConfig::setParent(const shared_ptr<KakuroConfig>& parent) {
	m_parent = parent;
}

shared_ptr<KakuroConfig> KakuroConfig::getParent() {
	return m_parent;
}

vector<vector<Cell>> KakuroConfig::getBoard() const {
	return m_board;
}

ostream& operator<<(ostream& os, const KakuroConfig& c) {
	for(int i = 0; i < c.m_board.size(); ++i) {
		for(int j = 0; j < c.m_board[0].size(); ++j) {
			if(c.m_shouldDelta && i == c.m_deltaI && j == c.m_deltaJ) {
				os << "[" << c.m_board[i][j] << "]" << "\t";
			} else {
				os << c.m_board[i][j] << "\t";
			}
		}
		
		if(i != c.m_board.size() - 1) os << endl;
	}
	
	return os;
}

