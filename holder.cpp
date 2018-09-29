/*
	CSCI 270 Fall 2018
	Programming Assignment
	Name   : David Tupper
	Email  : dtupper@usc.edu
	USC ID : 9255772152
*/

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <climits>
#include <stdexcept>

using namespace std;

// all data members may need to become int arrays of size 3 (considering all cases of flags)
struct Cell {
	int* minHealth;
	int* currHealth;
	bool* dflag;
	bool* pflag;
};

// OUTPUT: true = inBounds, false = outOfBounds
bool inBounds(int index, int N){
	if (index > (N-1) || index < 0) return false;
	else return true;
}

// initializes the values of G[0][0]
void baseCase(Cell*** cellInfo, string originValue){
	int mH, cH;
	bool d, p;
	// G[0][0] is a D
	if (originValue == "D"){
		mH = 1; // you need health of at least one (P = cost 0)
		cH = 1; 
		d = true;
		p = false;
	}
	// G[0][0] is a P
	else if (originValue == "P"){
		mH = 1; // you need health of at least one (P = cost 0)
		cH = 1; 
		p = true;
		d = false;
	}
	// number
	else {
		// G[0][0] is negative or zero
		if (stoi(originValue) <= 0){
			mH = abs(stoi(originValue)) + 1;
			cH = 1;
			d = false;
			p = false;
		}
		// pos
		else {
			mH = 0;
			cH = stoi(originValue);
			d = false;
			p = false;
		}
	}
	for (int i=0; i<3; i++){
		cellInfo[0][0]->minHealth[i] = mH;
		cellInfo[0][0]->currHealth[i] = cH;
		cellInfo[0][0]->dflag[i] = d;
		cellInfo[0][0]->pflag[i] = p;
	}

}

void updateCell_Number(Cell* pred, Cell* curr, int calc_i, int calc_j, int N, int squareCost, int iteration){
	int p_mH = pred->minHealth[iteration];
	// health and flag associaited w/ minHealth (from pred)
	int p_cH = pred->currHealth[iteration]; 
	bool p_dflag = pred->dflag[iteration];
	bool p_pflag = pred->pflag[iteration];

	bool usedD, usedP = false;
	// no attributes used (even if they exist)
	if (iteration == 0){

	}
	else if (iteration == 1){
		// dflag is available and will be applied
		if (p_dflag && squareCost > 0){
			squareCost *= 2;
			usedD = true;
		}
	}
	else if (iteration == 2){
		// pflag is available and will be applied
		if (p_pflag && squareCost < 0){
			squareCost = 0;
			usedP = true;
		}
	}

	bool changeIt = true;
	// positive or zero
	if (squareCost >= 0){
		if (0 + p_mH > curr->minHealth[iteration]) changeIt = false;
		if (changeIt){
			curr->minHealth[iteration] = 0 + p_mH;
			curr->currHealth[iteration] = squareCost + p_cH;
		}
	}
	// negative
	else {
		// negative cost is same or greater than health available
		if (abs(squareCost) >= p_cH){
			if ((abs(squareCost + p_cH) + 1) + p_mH > curr->minHealth[iteration]) changeIt = false;
			if (changeIt){
				curr->minHealth[iteration] = (abs(squareCost + p_cH) + 1) + p_mH;
				curr->currHealth[iteration] = 1;
			}
		}
		else {
			if (0 + p_mH > curr->minHealth[iteration]) changeIt = false;
			if (changeIt){
				curr->minHealth[iteration] = 0 + p_mH;
				curr->currHealth[iteration] = squareCost + p_cH;
			}
		}

	}
	// set attribute flags
	if (changeIt) {
		if (usedD) curr->dflag[iteration] = false;
		else curr->dflag[iteration] = p_dflag;
		if (usedP) curr->pflag[iteration] = false;
		else curr->pflag[iteration] = p_pflag;
	}
}

void updateCell_Letter(Cell* pred, Cell* curr, string letter){
	for (int l=0; l<3; l++){
		// minHealth needed to get to square
		int currMH = pred->minHealth[l];
		// health and flag associaited w/ minHealth
		int currH = pred->currHealth[l];
		bool flag;
		// if letter is P we need D's values
		if (letter == "P") flag = pred->dflag[l];
		// if letter is D we need P's values
		else if (letter == "D") flag = pred->pflag[l];

		if (currMH < curr->minHealth[l]){
			curr->minHealth[l] = currMH;
			curr->currHealth[l] = currH;
			if (letter == "D"){
				curr->dflag[l] = true;
				curr->pflag[l] = flag;
			}
			else if (letter == "P"){
				curr->pflag[l] = true;
				curr->dflag[l] = flag;
			}

		}
	}
}

// calculates & saves the minHealth of G[i][j+1] or G[i+1][j]
void calcMin(vector<vector<string> > G, Cell*** cellInfo, int i, int j, bool calcRight){
	// determine if calculating right or down
	int calc_i, calc_j;
	if (calcRight) {
		calc_i = i;
		calc_j = j + 1;
	}
	else {
		calc_i = i + 1;
		calc_j = j;
	}
	// has double heal attribute
	if (G[calc_i][calc_j] == "D"){
		updateCell_Letter(cellInfo[i][j], cellInfo[calc_i][calc_j], "D");
	}
	// has prevent attribute
	else if (G[calc_i][calc_j] == "P"){
		updateCell_Letter(cellInfo[i][j], cellInfo[calc_i][calc_j], "P");
	}
	// is a number
	else {
		// iterates through three possible scenarios 
		// 1: don't use attributes, 2: P available, 3: D available
		for (int k=0; k<3; k++){
			updateCell_Number(cellInfo[i][j], cellInfo[calc_i][calc_j], calc_i, calc_j, G[0].size(), stoi(G[calc_i][calc_j]), k);
		}
	}
}

int solve(int N, vector<vector<string> > G) {
	Cell*** cellInfo = new Cell**[N];
	for (int i=0; i<N; i++){
		cellInfo[i] = new Cell*[N];
		for (int j=0; j<N; j++){
			Cell* newCell = new Cell;
			newCell->minHealth = new int[3];
			newCell->currHealth = new int[3];
			newCell->dflag = new bool[3];
			newCell->pflag = new bool[3];
			for (int k=0; k<3; k++){
				newCell->minHealth[k] = INT_MAX;
				newCell->currHealth[k] = 0;
				newCell->dflag[k] = false; 
				newCell->pflag[k] = false;
			}
			cellInfo[i][j] = newCell;
		}
	}
	cout << endl;
	// dynamic programming
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			if (i==0 && j==0){
				baseCase(cellInfo, G[0][0]);
			}
			if (inBounds(j+1, N)) calcMin(G, cellInfo, i, j, true);
			if (inBounds(i+1, N)) calcMin(G, cellInfo, i, j, false);
		}
	}

	Cell* final = cellInfo[N-1][N-1];
	int rValue = final->minHealth[0];
	cout << "Final Considerations" << endl;
	for (int i=0; i<3; i++){
		cout << i << ": " << final->minHealth[i] << " ";
		if (final->minHealth[i] < rValue) rValue = final->minHealth[i];
	}
	cout << endl;

	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			delete [] cellInfo[i][j]->minHealth;
			delete [] cellInfo[i][j]->currHealth;
			delete [] cellInfo[i][j]->dflag;
			delete [] cellInfo[i][j]->pflag;
			delete cellInfo[i][j];
		}
		delete [] cellInfo[i];
	}
	delete [] cellInfo;	
	return rValue;
}

//	The main function reads the input and outputs your answer.
//	Please do not modify it. (Assume the input is well-formed;
//	You don't need to do any error-check)
int main(int argc, char **argv) {
	int N;
	vector<vector<string> > G;
	cin >> N;
	G.resize(N);
	for (int i=0; i<N; i++) {
		for (int j=0; j<N; j++) {
			string tmp;
			cin >> tmp;
			G[i].push_back(tmp);
		}
	}
	cout << solve(N, G) << endl;
	return 0;
}
