/**
  * Solver.h
  *
  * Author: Lane Lawley
  * Date: April 9th, 2014
  *
  * Description: This is the generalized backtracking solver.
  *		 It's initialized with a configuration and solved during construction.
  *		 One may check its success status and solution path via accessor functions.
  */


#include <memory>
#include <vector>

using namespace std;

template <class T>
class Solver {
	private:
		bool m_failure;
		int m_deadEnds;
		int m_maxDepth;
		vector<shared_ptr<T>> m_path;
	
	private:
		shared_ptr<T> solve(shared_ptr<T> config, int depth = 1) {
			if(depth > m_maxDepth) {
				m_maxDepth = depth;
			}
			
			if(config->isGoal()) {
				return config;
			}
			
			vector<shared_ptr<T>> succ = config->getSuccessors();
			
			if(succ.size() == 0) {
				++m_deadEnds;
				return nullptr;
			}
			
			for(shared_ptr<T>& child : succ) {
				if(depth > 0) {
					child->setParent(config);
				}
				
				shared_ptr<T> solution = solve(child, depth + 1);
				if(solution != nullptr) {
					return solution;
				}
			}
			
			return nullptr;
		}
	
	public:
		Solver(shared_ptr<T> initialConfig) : m_failure(false), m_deadEnds(0), m_maxDepth(0), m_path() {
			shared_ptr<T> cursor = solve(initialConfig);
			
			if(cursor != nullptr) {
				do {
					m_path.push_back(cursor);
					cursor = cursor->getParent();
				} while(cursor != nullptr);
			} else {
				m_failure = true;
			}
		}
	
	public:
		bool isFailure() const {
			return m_failure;
		}
		
		int numDeadEnds() const {
			return m_deadEnds;
		}
		
		int maxDepth() const {
			return m_maxDepth;
		}
		
		const vector<shared_ptr<T>>& getSolutionPath() const {
			return m_path;
		}
		
};
