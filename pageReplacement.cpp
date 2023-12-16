#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

struct Frames {
	int frame1;
	int frame2;
	int frame3;
};

void fifo(vector<int>, Frames);
void optimal(vector<int>, Frames);
int findFutureRef(vector<int>, int, int);
void lru(vector<int>, Frames);
void printFrames(string, Frames);


int main() {

	// vector to store the values
	vector<int> pageVector;
	ifstream inFile("pageStrings");
	
	// check for file error
	if(!inFile.is_open()) {
		cout << "File error" << endl;
		exit(EXIT_FAILURE);
	}
  
  	// fill vector with values from file
  	string str;
	while(getline(inFile, str))
		pageVector.push_back(stoi(str));
		
	inFile.close();
		
	Frames frames;
	frames.frame1 = -1;
	frames.frame2 = -1;
	frames.frame3 = -1;
		
	cout << "fifo" << endl;
	fifo(pageVector, frames);
	cout << "optimal" << endl;
	optimal(pageVector, frames);
	cout << "lru" << endl;
	lru(pageVector, frames);
	
	return 0;
}


void fifo(vector<int> pageVector, Frames frames) {
	
	int oldestPI = 1;		// keep track of the oldest
	int secondOldestPI = 2;	// 
	int thirdOldestPI = 3;	//
	int temp = -1; 			// used for swapping values
	int numFaults = 0; 		// track number of page faults
	
	for(int n = 0; n < pageVector.size(); n++) {
		// check if value is already in frames
		if(frames.frame1 == pageVector[n] || frames.frame2 == pageVector[n] || frames.frame3 == pageVector[n]) {
			cout << to_string(pageVector[n]) << " |" << endl;
			continue;
		}
		// check for empty frames
		else if(oldestPI == 1)
			frames.frame1 = pageVector[n];
		else if(oldestPI == 2)
			frames.frame2 = pageVector[n];
		else if(oldestPI == 3)
			frames.frame3 = pageVector[n];
		else
			exit(EXIT_FAILURE);
			
		printFrames(to_string(pageVector[n]), frames); // function to diplay each line of algorithm
		// swap values
		temp = oldestPI;
		oldestPI = secondOldestPI;
		secondOldestPI = thirdOldestPI;
		thirdOldestPI = temp;
		numFaults++; // increment number of page faults
	}
	
	cout << numFaults << " faults" << endl << endl;
	
	return;
}


void optimal(vector<int> pageVector, Frames frames) {

	int futureIndex1 = -1; 	// keep track of futre index of values
	int futureIndex2 = -1; 	//
	int futureIndex3 = -1; 	//
	int numFaults = 0; 		// track number of page faults
	
	for(int n = 0; n < pageVector.size(); n++) {
		// check if value is already in frames
		if(frames.frame1 == pageVector[n] || frames.frame2 == pageVector[n] || frames.frame3 == pageVector[n]) {
			cout << to_string(pageVector[n]) << " |" << endl;
			continue;
		}
		// check for empty frames
		else if(frames.frame1 == -1)
			frames.frame1 = pageVector[n];
		else if(frames.frame2 == -1)
			frames.frame2 = pageVector[n];
		else if(frames.frame3 == -1)
			frames.frame3 = pageVector[n];
		else {
			futureIndex1 = findFutureRef(pageVector, n, frames.frame1); // find future undex using helper function
			futureIndex2 = findFutureRef(pageVector, n, frames.frame2); //
			futureIndex3 = findFutureRef(pageVector, n, frames.frame3); //
			
			// go through possible outsomes
			if(futureIndex1 == -1)
				frames.frame1 = pageVector[n];
			else if(futureIndex2 == -1) 
				frames.frame2 = pageVector[n];
			else if(futureIndex3 == -1)
				frames.frame3 = pageVector[n];
			else if(futureIndex1 >= futureIndex2 && futureIndex1 >= futureIndex3)
				frames.frame1 = pageVector[n];
			else if(futureIndex2 >= futureIndex1 && futureIndex2 >= futureIndex3)
				frames.frame2 = pageVector[n];
			else if(futureIndex3 >= futureIndex1 && futureIndex3 >= futureIndex2)
				frames.frame3 = pageVector[n];
			else
				exit(EXIT_FAILURE);
		}
		
		printFrames(to_string(pageVector[n]), frames); // function to diplay each line of algorithm
		numFaults++; // increment number of page faults
	}
	
	cout << numFaults << " faults" << endl << endl;

	return;
}

// helper function to find the index of future reference of that value
int findFutureRef(vector<int> pageVector, int curIndex, int page) {
	int futureIndex = -1;
	for(int n = pageVector.size()-1; n > curIndex; n--) {
		if(page == pageVector[n])
			futureIndex = n;
	}
	return futureIndex;
}


void lru(vector<int> pageVector, Frames frames) {

	unordered_map<int, int> map; // map to track indexes
	int f1Index = -1; // keep track of least recently used
	int f2Index = -1; //
	int f3Index = -1; // 
	int numFaults = 0; // track number of page faults
	
	for(int n = 0; n < pageVector.size(); n++) {
		// check if value is already in frames
		if(frames.frame1 == pageVector[n] || frames.frame2 == pageVector[n] || frames.frame3 == pageVector[n]) {
			cout << to_string(pageVector[n]) << " |" << endl;
			map[pageVector[n]] = n;
			continue;
		}
		// check for empty frames
		else if(frames.frame1 == -1)
			frames.frame1 = pageVector[n];
		else if(frames.frame2 == -1)
			frames.frame2 = pageVector[n];
		else if(frames.frame3 == -1)
			frames.frame3 = pageVector[n];
		else {
			// get indexes
			f1Index = map.at(frames.frame1);
			f2Index = map.at(frames.frame2);
			f3Index = map.at(frames.frame3);
			// find least recently used and swap
			if(f1Index <= f2Index && f1Index <= f3Index)
				frames.frame1 = pageVector[n];
			else if(f2Index <= f1Index && f2Index <= f3Index)
				frames.frame2 = pageVector[n];
			else if(f3Index <= f1Index && f3Index <= f2Index)
				frames.frame3 = pageVector[n];
		}
		
		printFrames(to_string(pageVector[n]), frames); // function to diplay each line of algorithm
		numFaults++; // increment number of page faults
		map[pageVector[n]] = n; // save index in map
	}
		
	cout << numFaults << " faults" << endl << endl;

	return;
}

// function to diplay each line of algorithm
void printFrames(string newPage, Frames frames) {
	cout << newPage << " | " << frames.frame1 << " " << frames.frame2 << " " << frames.frame3 << endl;
	return;
}


