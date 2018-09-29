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

void updateCell_Number(Cell* pred, Cell* curr, int squareCost, int iteration){
	int p_mH = min(min(pred->minHealth[0], pred->minHealth[1]), pred->minHealth[2]);
	// health and flag associaited w/ minHealth (from pred)
	int p_cH; 
	bool p_dflag, p_pflag;
	// decides what health value to give it 
	if (p_mH == pred->minHealth[0]) {
		p_cH = pred->currHealth[0];
		p_dflag = pred->dflag[0];
		p_pflag = pred->pflag[0];
	}
	else if (p_mH == pred->minHealth[1]) {
		p_cH = pred->currHealth[1];
		p_dflag = pred->dflag[1];
		p_pflag = pred->pflag[1];
	}
	else if (p_mH == pred->minHealth[2]) {
		p_cH = pred->currHealth[2];
		p_dflag = pred->dflag[2];
		p_pflag = pred->pflag[2];
	}
	// no attributes used (even if they exist)
	if (iteration == 0){

	}
	else if (iteration == 1){
		// dflag is available and will be applied
		if (p_dflag && squareCost > 0){
			squareCost *= 2;
			//usedD = true;
		}
	}
	else if (iteration == 2){
		// pflag is available and will be applied
		if (p_pflag && squareCost < 0){
			squareCost = 0;
			//usedP = true;
		}
	}


	// positive or zero
	if (squareCost >= 0){
		// if new minHealth is smaller than old one
		if ((0 + p_mH) < curr->minHealth[iteration]){
			curr->minHealth[iteration] = 0 + p_mH;
			curr->currHealth[iteration] = squareCost + p_cH;
		}
	}
	// negative
	else {
		// negative cost is same or greater than health available
		if (abs(squareCost) >= p_cH){
			// if new minHealth is smaller than old one
			if ((abs(squareCost + p_cH) + 1) + p_mH < curr->minHealth[iteration]){
				curr->minHealth[iteration] = (abs(squareCost + p_cH) + 1) + p_mH;
				curr->currHealth[iteration] = 1;
			}
		}
		else {
			// if new minHealth is smaller than old one
			if (0 + p_mH < curr->minHealth[iteration]){
				curr->minHealth[iteration] = 0 + p_mH;
				curr->currHealth[iteration] = squareCost + p_cH;
			}
		}

	}
	// set attribute flags
	curr->dflag[iteration] = p_dflag;
	curr->pflag[iteration] = p_pflag;
}

void updateCell_Letter(Cell* pred, Cell* curr, string letter){
	// minHealth needed to get to square
	int currMH = min(min(pred->minHealth[0], pred->minHealth[1]), pred->minHealth[2]);
	// health and flag associaited w/ minHealth
	int currH; 
	bool flag;
	// decides what health value to give it 
	if (currMH == pred->minHealth[0]) {
		currH = pred->currHealth[0];
		// if letter is P we need D's values
		if (letter == "P") flag = pred->dflag[0];
		// if letter is D we need P's values
		else if (letter == "D") flag = pred->pflag[0];
	}
	else if (currMH == pred->minHealth[1]) {
		currH = pred->currHealth[1];
		if (letter == "P") flag = pred->dflag[1];
		else if (letter == "D") flag = pred->pflag[1];
	}
	else if (currMH == pred->minHealth[2]) {
		currH = pred->currHealth[2];
		if (letter == "P") flag = pred->dflag[2];
		else if (letter == "D") flag = pred->pflag[2];
	}
	// sets curr's new attributes (depending on if new mH is smaller or not)
	for (int i=0; i<3; i++){
		if (currMH < curr->minHealth[i]){
			curr->minHealth[i] = currMH;
			curr->currHealth[i] = currH;
			if (letter == "D"){
				curr->dflag[i] = true;
				curr->pflag[i] = flag;
			}
			else if (letter == "P"){
				curr->pflag[i] = true;
				curr->dflag[i] = flag;
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
		if (i==0 && j==1) cout << "i: 0 j: 1 " << "G: " << G[calc_i][calc_j] << endl;
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
			updateCell_Number(cellInfo[i][j], cellInfo[calc_i][calc_j], stoi(G[calc_i][calc_j]), k);
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
			for (int k=0; k<N; k++){
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

	cout << endl << "no attributes" << endl;
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			cout << cellInfo[i][j]->minHealth[0] << " ";
		}
		cout << endl;
	}
	
	cout << endl << "allows P" << endl;
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			cout << cellInfo[i][j]->minHealth[1] << " ";
		}
		cout << endl;
	}
	

	cout << endl << "allows D" << endl;
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			cout << cellInfo[i][j]->minHealth[2] << " ";
		}
		cout << endl;
	}
	cout << endl;

	Cell* final = cellInfo[N-1][N-1];
	return min(min(final->minHealth[0], final->minHealth[1]), final->minHealth[2]);

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
