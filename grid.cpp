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

using namespace std;

// i: rows, j: cols
// OUTPUT: the value of minHealth table for i, j
int calcMinHealth(int maxHP, int squareCost, int i, int j, int** squareHealth, int** minHealth){
	bool leftOut = false;
	bool topOut = false;
	if ((j-1) < 0) leftOut = true;
	if ((i-1) < 0) topOut = true;


	// squareCost is positive
	// OR squareCost is negative, but not bigger than maxHP
	if (squareCost >= 0 || abs(squareCost) < maxHP){
		// G[0][0] case
		if (i==0 && j==0) {
			if (squareCost == 0) return 1;
		}
		// normal case
		if (leftOut) return minHealth[i-1][j];
		else if (topOut) return minHealth[i][j-1];
		else return ( min(minHealth[i-1][j], minHealth[i][j-1]) );
	}
	// squareCost and maxHP cancel
	else if (abs(squareCost) == maxHP) {
		if (leftOut) return ( minHealth[i-1][j] + 1 );
		else if (topOut) return ( minHealth[i][j-1] + 1 );
		else return ( min(minHealth[i-1][j], minHealth[i][j-1]) + 1 );
	}
	// squareCost is larger than maxHP
	else {
		if (i==0 && j==0){
			cout << "Entered the if statement" << endl;
			return (abs(squareCost) + 1);
		}
		if (leftOut) return ( minHealth[i-1][j] + abs(squareCost + maxHP) + 1 );
		else if (topOut) return ( minHealth[i][j-1] + abs(squareCost + maxHP) + 1 );
		else return ( min(minHealth[i-1][j], minHealth[i][j-1]) + abs(squareCost + squareHealth[i][j]) + 1 );
	}
}

int calcSquareHealth(int maxHP, int squareCost, int i, int j, int** squareHealth){
	bool leftOut = false;
	bool topOut = false;
	if ((j-1) < 0) leftOut = true;
	if ((i-1) < 0) topOut = true;

	// squareCost is positive
	// OR squareCost is negative, but not bigger than maxHP
	if (squareCost >= 0 || abs(squareCost) < maxHP){
		return (maxHP + squareCost);
	}
	// squareCost >= maxHP
	else {
		return 1;
	}
}

// OUTPUT: HP for the lowest minHealth needed for a square
int startHealth(int i, int j, int** squareHealth, int** minHealth){
	int leftHP, topHP, finalHP;
	int minHealthLeft, minHealthTop;
	bool leftOut = false;
	bool topOut = false;
	// checks if left of curr square is out of bounds
	if (j-1 < 0) {
		leftOut = true;
		leftHP = 0;
		minHealthLeft = INT_MAX;
	}
	// left of curr not out of bounds
	else {
		leftHP = squareHealth[i][j-1];
		minHealthLeft = minHealth[i][j-1];
	}
	// checks if top of curr square is out of bounds
	if (i-1 < 0) {
		topOut = true;
		topHP = 0;
		minHealthTop = INT_MAX;

	}
	// top of curr not out of bounds
	else {
		topHP = squareHealth[i-1][j];
		minHealthTop = minHealth[i-1][j];
	}

	// left requires less min health
	if (minHealthLeft < minHealthTop){
		return leftHP;
	}
	// they're same
	else if (minHealthLeft == minHealthTop){
		return (max(leftHP, topHP));
	}
	// topHP requires less minHealth
	else {
		return topHP;
	}
}


int solve(int N, vector<vector<string> > G) {
	// declares squareHealth and minHealth array
	 int** squareHealth = new int*[N]; // max amount of health when standing on particular square
	 int** minHealth = new int*[N]; // min amount of health to get to particular square
	 for (int i=0; i<N; i++){
		 squareHealth[i] = new int[N];
		 minHealth[i] = new int[N];
	 }
	 // initializes base case
	 // G[0][0] is positive
	 if (stoi(G[0][0]) > 0) {
		 squareHealth[0][0] = stoi(G[0][0]);
		 minHealth[0][0] = 0;
	 }
	 // G[0][0] is negative or zero
	 else {
		 squareHealth[0][0] = 1;
		 minHealth[0][0] = calcMinHealth(0, stoi(G[0][0]), 0, 0, squareHealth, minHealth);
	 }
	 // dyanmic programming
	 for (int i=0; i<N; i++){
		 for (int j=0; j<N; j++){
			 // G[0][0] has already been initalized
			 if (i == 0 && j == 0) continue;
			 // reached the bottom right case
			 else if (i == (N-1) && j == (N-1)){
			 	int health = startHealth(i, j, squareHealth, minHealth);
			 	squareHealth[i][j] = calcSquareHealth(health, stoi(G[i][j]), i, j, squareHealth);
			 	minHealth[i][j] = calcMinHealth(health, stoi(G[i][j]), i, j, squareHealth, minHealth);



			 	cout << endl << "squareHealth" << endl;
			 	for (int i=0; i<N; i++){
	 	for (int j=0; j<N; j++){
	 		cout << squareHealth[i][j] << " ";
	 	}
	 	cout << endl;
	 }

	 cout << endl << "minHealth" << endl;

	 for (int i=0; i<N; i++){
	 	for (int j=0; j<N; j++){
	 		cout << minHealth[i][j] << " ";
	 	}
	 	cout << endl;
	 }
	 cout << endl;





				return minHealth[N-1][N-1];
			 }
			 // haven't reached the end yet
			 else {
				int health = startHealth(i, j, squareHealth, minHealth);
				squareHealth[i][j] = calcSquareHealth(health, stoi(G[i][j]), i, j, squareHealth);
				minHealth[i][j] = calcMinHealth(health, stoi(G[i][j]), i, j, squareHealth, minHealth);
			 }
		 }
	 }

	 for (int i=0; i<N; i++){
	 	for (int j=0; j<N; j++){
	 		cout << squareHealth[i][j] << " ";
	 	}
	 	cout << endl;
	 }

	 cout << endl << endl;

	 for (int i=0; i<N; i++){
	 	for (int j=0; j<N; j++){
	 		cout << minHealth[i][j] << " ";
	 	}
	 	cout << endl;
	 }


	 // frees dynamically allocated memory
	 for (int i=0; i<N; i++){
		 delete [] squareHealth[i];
		 delete [] minHealth[i];
	 }
	 delete [] squareHealth;
	 delete [] minHealth;

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
