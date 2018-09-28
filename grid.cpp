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

struct Cell {
	int minHealth;
	int currHealth;
	bool dflag;
	bool pflag;
};

void printCell(Cell* cell){
	cout << "minHealth: " << cell->minHealth << endl;
	cout << "currHealth: " << cell->currHealth << endl;
	cout << "dflag: " << cell->dflag << endl;
	cout << "pflag: " << cell->pflag << endl;
}

// OUTPUT: true = inBounds, false = outOfBounds
bool inBounds(int index, int N){
	if (index > (N-1) || index < 0) return false;
	else return true;
}

// initializes the values of G[0][0]
void baseCase(Cell*** cellInfo, string originValue){
	if (originValue == "D"){
		cellInfo[0][0]->minHealth = 1; // you need health of at least one (P = cost 0)
		cellInfo[0][0]->currHealth = 1; 
		cellInfo[0][0]->dflag = true;
	}
	else if (originValue == "P"){
		cellInfo[0][0]->minHealth = 1; // you need health of at least one (P = cost 0)
		cellInfo[0][0]->currHealth = 1; 
		cellInfo[0][0]->pflag = true;
	}
	// number
	else {
		// G[0][0] is negative or zero
		if (stoi(originValue) <= 0){
			cellInfo[0][0]->minHealth = abs(stoi(originValue)) + 1;
			cellInfo[0][0]->currHealth = 1;
		}
		// pos
		else {
			cellInfo[0][0]->minHealth = 0;
			cellInfo[0][0]->currHealth = stoi(originValue);
		}
	}
}

// applies attributes to cost
int* applyAttributes(Cell* cell, int cost){
	int* afterAttributes = new int[2];
	// pos 
	if (cost > 0){
		// use D attribute
		if (cell->dflag) {
			afterAttributes[0] = cost * 2;
			afterAttributes[1] = 1;
		}
		// use no attributes
		else {
			afterAttributes[0] = cost;
			afterAttributes[1] = 3;
		}
	}
	// neg
	else if (cost < 0){
		// use P attribute
		if (cell->pflag) {
			afterAttributes[0] = 0;
			afterAttributes[1] = 2;
		}
		// use no attributes
		else {
			afterAttributes[0] = cost;
			afterAttributes[1] = 3;
		}
	}
	else {
		// use no attributes
		afterAttributes[0] = cost;
		afterAttributes[1] = 3;
	}
	return afterAttributes;
}

void updateCellValues(Cell* pred, Cell* curr, int* afterAttributes){
	// updates dflag data member
	if (pred->dflag){
		// checks if doubling was used on curr 
		if (afterAttributes[1] != 1) curr->dflag = true;
	}
	// updates pflag data member
	if (pred->pflag){
		// checks if prevent was used on curr
		if (afterAttributes[1] != 2) curr->pflag = true;
	}
	// updates currHealth data member
	// afterAttributes[0] is square cost
	// pos
	if (afterAttributes[0] >= 0){
		curr->currHealth = afterAttributes[0] + pred->currHealth;
	}
	// neg
	else {
		// smaller neg than pred->currHealth
		if (abs(afterAttributes[0]) < pred->currHealth) {
			curr->currHealth = afterAttributes[0] + pred->currHealth;
		}
		// larger neg than pred->currHealth
		else {
			curr->currHealth = 1;
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
	if (i==0 && j==0) {
		cout << endl;
		cout << calc_i << ", " << calc_j << endl;
		cout << "G[calc_i][calc_j] " << G[calc_i][calc_j] << endl;	
	}

	// calculate the minHealth
	int minHealthIJ;
	// used for number case
	int* afterAttributes;
	// has double heal attribute
	if (G[calc_i][calc_j] == "D"){
		minHealthIJ = 0 + cellInfo[i][j]->minHealth;
		cellInfo[calc_i][calc_j]->dflag = true;
		afterAttributes = new int[2];
		afterAttributes[0] = 0;
		afterAttributes[1] = 3;
	}
	// has prevent attribute
	else if (G[calc_i][calc_j] == "P"){
		minHealthIJ = 0 + cellInfo[i][j]->minHealth;
		cellInfo[calc_i][calc_j]->pflag = true;
		afterAttributes = new int[2];
		afterAttributes[0] = 0;
		afterAttributes[1] = 3;
	}
	// is a number
	else {
		afterAttributes = applyAttributes(cellInfo[i][j], stoi(G[calc_i][calc_j]));
		if (i==0 && j==0) {
			cout << "afterAttributes[0] " << afterAttributes[0] << endl;
			cout << "afterAttributes[1] " << afterAttributes[1] << endl;
		}
		// pos
		if (afterAttributes[0] > 0){
			minHealthIJ = 0 + cellInfo[i][j]->minHealth;
		}
		// neg
		else if (afterAttributes[0] < 0){
			// square cost is greater than currHealth
			if (abs(afterAttributes[0]) >= cellInfo[i][j]->currHealth){
				if (i==0 && j==0) cout << "Entered negative (1)" << endl;
				minHealthIJ = (abs(afterAttributes[0] + cellInfo[i][j]->currHealth) + 1 + cellInfo[i][j]->minHealth);
			}
			else minHealthIJ = 0 + cellInfo[i][j]->minHealth;
			
		}
		// 0
		else {
			minHealthIJ = 0 + cellInfo[i][j]->minHealth;
		}
	}
	// IF minHealthIJ < current minHealth for cellInfo[i][j+1]
	// THEN replace minHealth for [i][j+1] and other data members
	if (minHealthIJ < cellInfo[calc_i][calc_j]->minHealth){
		if (i==0 && j==0) cout << "Changed that shit" << endl;
		cellInfo[calc_i][calc_j]->minHealth = minHealthIJ;
		updateCellValues(cellInfo[i][j], cellInfo[calc_i][calc_j], afterAttributes);
	}
	delete [] afterAttributes;
}

int solve(int N, vector<vector<string> > G) {
	Cell*** cellInfo = new Cell**[N];
	for (int i=0; i<N; i++){
		cellInfo[i] = new Cell*[N];
		for (int j=0; j<N; j++){
			Cell* newCell = new Cell;
			newCell->minHealth = INT_MAX;
			newCell->currHealth = 0;
			newCell->dflag = false; 
			newCell->pflag = false;
			cellInfo[i][j] = newCell;
		}
	}
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

	cout << endl << "minHealth" << endl;
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			cout << cellInfo[i][j]->minHealth << " ";
		}
		cout << endl;
	}

	cout << endl << "health" << endl;
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			cout << cellInfo[i][j]->currHealth << " ";
		}
		cout << endl;
	}

	cout << endl << "attributes" << endl;
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			cout << cellInfo[i][j]->dflag; 
			cout << cellInfo[i][j]->pflag << " ";
		}
		cout << endl;
	}
	cout << endl;

	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			delete cellInfo[i][j];
		}
		delete [] cellInfo[i];
	}
	delete [] cellInfo;

	return cellInfo[N-1][N-1]->minHealth;
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
