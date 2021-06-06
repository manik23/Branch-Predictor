/*
Author	Manik Mahajan
		H20201030039G
		h20200039@goa.bits-pilani.ac.in


preditiorIntialize(n-bits,case)
	n-bits is n-bits Predictor
	case can be intialised to all zeros, all ones or random.

predict()
	return Taken or Not Taken.


updateFSM()
	updates the FSM depending upon the miss-prediction or correct-predictions.


getPrediction()
	It match predictions will true outcomes. Depending upon the miss-prediction
	or correct-predictions it will update FSM of the corresponding branch.




*/


#include<iostream>
#include<stdio.h>
#include<fstream>
#include<math.h>
#include<string.h>

using namespace std;

#define file1 "branchOutcome1.txt"
#define file2 "branchOutcome2.txt"
#define file3 "branchOutcome3.txt"

#define hitFile1AllZero "./Part2_Data_Colletected/file1HitZeros.txt"
#define hitFile2AllZero "./Part2_Data_Colletected/file2HitZeros.txt"
#define hitFile3AllZero "./Part2_Data_Colletected/file3HitZeros.txt"

#define hitFile1AllOnes "./Part2_Data_Colletected/file1HitOnes.txt"
#define hitFile2AllOnes "./Part2_Data_Colletected/file2HitOnes.txt"
#define hitFile3AllOnes "./Part2_Data_Colletected/file3HitOnes.txt"


#define hitFile1AllRandom "./Part2_Data_Colletected/file1HitRandom.txt"
#define hitFile2AllRandom "./Part2_Data_Colletected/file2HitRandom.txt"
#define hitFile3AllRandom "./Part2_Data_Colletected/file3HitRandom.txt"







long long branchCounter[10];
long long lastrandom = -1;
long long low;
long long high;
long long missCount;
long long hitCount;
long long totalCount;
long long spc;

bool makeLog;

char predict(long long index) {
	if (branchCounter[index] <= high / 2) {

		return 'N';
	}

	return 'T';
}

void preditiorIntialize(int branchPredictor, int CASE = 2) {

	low  = 0;
	high = pow(2, branchPredictor) - 1;

	missCount = 0;
	hitCount = 0;
	totalCount = 0;


	if (CASE == 0 ) {
		for (int i = 0; i < sizeof(branchCounter) / sizeof(branchCounter[0]); i++) {
			branchCounter[i] = low;
		}
	}
	else if (CASE == 1) {

		for (int i = 0; i < sizeof(branchCounter) / sizeof(branchCounter[0]); i++) {
			branchCounter[i] = high;
		}
	}
	else {
		if (lastrandom == -1) {
			long long temp =  ((rand()) % ((high) - (low) + 1)) + (low);

			lastrandom = temp;
			for (int i = 0; i < sizeof(branchCounter) / sizeof(branchCounter[0]); i++) {
				branchCounter[i] = lastrandom;
			}
		}
		else {
			for (int i = 0; i < sizeof(branchCounter) / sizeof(branchCounter[0]); i++) {
				branchCounter[i] = lastrandom;
			}
		}
	}

	// cout << "starting index " << branchCounter[0] << endl;

}



void updateFSM(char x, bool miss, long long branchIndex) {
	if (!miss) missCount++;
	if (miss) hitCount++;
	totalCount++;
	if (x == 'N') {
		if (branchCounter[branchIndex] != low)
			branchCounter[branchIndex]--;
	}
	else {
		if (branchCounter [branchIndex] != high)
			branchCounter[branchIndex]++;
	}
}


void moveString(int *pointer, int CASE) {
	if (!CASE) { *pointer += 2;}
	else { *pointer += 3;}
}


void getPredictions(string file, long long branchIndex, int CASE = 2) {

	fstream myfile;
	myfile.open(file);
	string s;
	getline(myfile, s);
	myfile.close();
	int index = 0;

	int filepointer = 0;

	while (filepointer < s.length()) {
		// cout << s[filepointer] << " " <<   " " << predict(index) << " " ;
		if (s[filepointer] == 'T') {

			if (predict(index) == s[filepointer]) {
				// cout << "Yes" << " ";
				updateFSM(s[filepointer], 1, index);

			}
			else {
				// cout << "No" << " ";
				updateFSM(s[filepointer], 0, index);
			}
			// cout << " " << branchCounter[index] << " \n"; // cout << " " << branchCounter << " \n";
			moveString(&filepointer, 0);
		}
		else {
			if (predict(index) == 'N') {
				// cout << "Yes" << " ";
				updateFSM(s[filepointer], 1, index);

			}
			else {
				// cout << "No" << " ";
				updateFSM(s[filepointer], 0, index);
			}
			moveString(&filepointer, 1);
			// cout << branchCounter[index] << " \n";
		}
		// cout << "PC " << index << " \n";
		index = (index + 1) % branchIndex;
	}
	float accuracy = ( float(hitCount) / float(totalCount)) * 100;


	if (makeLog) {
		if (CASE == 0) {

			fstream data;
			if (file == file1) {
				data.open(hitFile1AllZero, ios::out  | ios::app);
			}
			else if (file == file2) {
				data.open(hitFile2AllZero, ios::out  | ios::app);
			}
			else {
				data.open(hitFile3AllZero, ios::out  | ios::app);
			}

			data << accuracy << ",";
			data.close();

		}
		else if (CASE == 1) {
			fstream data;
			if (file == file1) {
				data.open(hitFile1AllOnes, ios::out  | ios::app);
			}
			else if (file == file2) {
				data.open(hitFile2AllOnes, ios::out  | ios::app);
			}
			else {
				data.open(hitFile3AllOnes, ios::out  | ios::app);
			}

			data << accuracy << ",";
			data.close();

		}
		else {

			fstream data;
			if (file == file1) {
				data.open(hitFile1AllRandom, ios::out  | ios::app);
			}
			else if (file == file2) {
				data.open(hitFile2AllRandom, ios::out |  ios::app);
			}
			else {
				data.open(hitFile3AllRandom, ios::out  | ios::app);
			}

			data << accuracy << ",";
			data.close();
		}
	}



	string c;
	if (CASE == 0) {
		c = " ALL Zeros ";
	}
	else if (CASE == 1) {
		c = " ALL Ones ";
	}
	else {
		c = " ALL Random";
	}
	cout << file << c <<  "accuracy " << accuracy << endl;

// cout << missCount << " " << hitCount << " " << totalCount << endl;



}




int main() {
// #ifndef ONLINE_JUDGE
// 	freopen("input.txt", "r", stdin);
// 	freopen("output.txt", "w", stdout);
// #endif
	srand(time(0));
	int branchPredictor;
	cout << "Enter bits of predictor\n";
	cin >> branchPredictor;



	// cout << "Branch bits " << branchPredictor << endl << endl;

	/* Case 1  when - All 0s.*/
	// for (int i = 1; i <= 15; i++) {
	// 	srand(time(0));
	// 	cout << i << " BIT" << endl;
	// 	int branchPredictor = i;
	// 	makeLog = 1;

	/* Case 0  when - All 0's.*/
	// cout << "All Ones ";
	preditiorIntialize(branchPredictor, 0);
	getPredictions(file1, 2, 0);
	preditiorIntialize(branchPredictor, 0);
	getPredictions(file2, 3, 0);
	preditiorIntialize(branchPredictor, 0);
	getPredictions(file3, 4, 0);
	cout << endl;

	/* Case 1  when - All 1's.*/
	// cout << "All Ones ";
	preditiorIntialize(branchPredictor, 1);
	getPredictions(file1, 2, 1);
	preditiorIntialize(branchPredictor, 1);
	getPredictions(file2, 3, 1);
	preditiorIntialize(branchPredictor, 1);
	getPredictions(file3, 4, 1);
	cout << endl;



	/* Case 2 when - random */
	preditiorIntialize(branchPredictor);
	getPredictions(file1, 2);
	preditiorIntialize(branchPredictor);
	getPredictions(file2, 3);
	preditiorIntialize(branchPredictor);
	getPredictions(file3, 4);
	cout << endl;

	lastrandom = -1;

// }clear




}