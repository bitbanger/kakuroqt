/**
  * Partitioner.h
  *
  * Author: Lane Lawley
  * Date: May 11th, 2014
  *
  * Description: This class is a singleton that's primarily responsible for integer partitioning.
  * 		 Kakuro primarily relies on the ability to find a unique, arbitrary-length combination of numbers that sum to a given value.
  *		 Through a naive recursive algorithm and caching, we achieve good partitioning performance.
  */

#ifndef KPART_H
#define KPART_H

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Partitioner {
	private:
		int m_cacheHits;
		int m_cacheMisses;
		
		unordered_map<string, array<bool, 9>> m_cache;
	
	public:
		// Singleton accessor for the partitioner class
		static Partitioner& getInstance() {
			static Partitioner instance;
			return instance;
		}
		
	private:
		// Default constructor (to be run once)
		Partitioner() : m_cacheHits(0), m_cacheMisses(0), m_cache() {}
		
		// Disable copy construction
		Partitioner(const Partitioner& other) = delete;
		
		// Disable assignment
		void operator=(const Partitioner& other) = delete;
	
		// Converts an array of 9 booleans to a readable string
		string arrToStr(array<bool, 9> arr) {
			string str;
			
			for(int i = 0; i < 9; ++i) {
				str.append(to_string(arr[i]) + " ");
			}
			
			return str;
		}
		
		// Determines if a vector contains any contiguous duplicates
		// Given the construction of the partitioning algorithm, duplicates will always be contiguous
		bool isUnique(const vector<int>& vec) {
			for(unsigned i = 1; i < vec.size(); ++i) {
				if(vec[i] == vec[i - 1]) return false;
			}
			
			return true;
		}
		
		// Determines if a vector contains a given number
		bool vecContains(vector<int>& vec, int num) {
			return find(vec.begin(), vec.end(), num) != vec.end();
		}
		
		// Recursive algorithm to compute the integer partitions of a given number, sum
		// The number is partitioned into num separate integers between minVal and maxVal
		// All possible partitions are returned
		vector<vector<int>> partHelper(int sum, int num, int minVal, int maxVal) {
			vector<vector<int>> result;
			
			if(sum <= 1 || num == 1) {
				if(sum >= minVal && sum <= maxVal) {
					result.push_back(vector<int> {sum});
					return result;
				}
			}
			
			int upperBound = (sum < maxVal) ? sum : maxVal;
			
			for(int i = upperBound; i >= minVal; --i) {
				vector<vector<int>> recParts = partHelper(sum - i, num - 1, minVal, i);
				for(vector<int>& recPart : recParts) {
					recPart.push_back(i);
					
					int s1 = recPart.size();
					
					if(isUnique(recPart) && s1 == num) {
						result.push_back(recPart);
					}
				}
			}
			
			return result;
		}
		
		
	public:
		// Number of cache hits during partitioning requests
		int cacheHits() const {
			return m_cacheHits;
		}
		
		// Number of cache misses during partitioning requests
		int cacheMisses() const {
			return m_cacheMisses;
		}
		
		// Returns the intersection of two boolean arrays
		array<bool, 9> intersection(array<bool, 9> a, array<bool, 9> b) {
			array<bool, 9> result{};
			for(int i = 0; i < 9; ++i) {
				result[i] = a[i] && b[i];
			}
			
			return result;
		}
		
		// Returns the possible values of a cell in a cell group of size num which group needs to add up to sum
		// Will not use any values marked in the used array
		array<bool, 9> possibleValues(int sum, int num, array<bool, 9> used = array<bool, 9>()) {
			string key = to_string(sum) + " " + to_string(num) + " " + arrToStr(used);
			
			if(m_cache.count(key) > 0) {
				++m_cacheHits;
				return m_cache[key];
			}
			
			++m_cacheMisses;
			
			array<bool, 9> vals{};
			
			if(sum <= 0) {
				// If the sum is less than or equal to 0, we have no restriction other than what's used
				for(int i = 0; i < 9; ++i) vals[i] = !used[i];
				return vals;
			}
			
			vector<vector<int>> parts = partHelper(sum, num, 1, 9);
			
			for(auto& v : parts) {
				bool cont = true;
				array<bool, 9> tmp{};
				for(int val : v) {
					if(used[val - 1]) {
						cont = false;
						break;
					}
					tmp[val - 1] = true;
				}
				
				if(cont) {
					for(int i = 0; i < 9; ++i) {
						vals[i] |= tmp[i];
					}
				}
			}
			
			m_cache[key] = vals;
			
			return vals;
		}
};

#endif

