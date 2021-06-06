/*
Author	Manik Mahajan
		H20201030039G
		h20200039@goa.bits-pilani.ac.in


preditiorIntialize(value,CASE)
	It will intialize the predictors for different values of p 3,4, and 5
	CASE can be all Zeros or all Ones.

globalorLocal()
	It will return the selection choice to go for local or global prediction depending
	upon the correspoing CPT value.

predict(branch,PC)
	branch can be either local or global.
	PC is used while making prediction from local.

updateTablesPrediction()
	This function will update the values in LHT, GPT and fsm of 2-bit in LPT.
	This function will update the values in CPT and GHR

updateGPT()
	It will update the value of GPT depending upon the outcome

updateLHT()
	It will update the value of LHT depending upon the outcome

updateGHR()
	It will update the value of GHR depending upon the outcome


predict()
	This function will generate prediction and match them with the actual outcomes
	and measure the accuracy for the prediction.



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


int LHT[8];
int lht_lower = 0;
int lht_higher = 7;


int LPT[8][8];


int GPT[64];
int CPT[64];


int GHR;
int ghr_lower;
int ghr_upper;

int low = 0;
int high = 3;

int missCount;
int hitCount;
int totalCount;
int spc;

long long localCount;
long long globalCount;


void preditiorIntialize(int value, int CASE) {

	missCount = 0;
	hitCount = 0;
	totalCount = 0;
	localCount = 0;
	globalCount = 0;

	ghr_lower = 0;
	ghr_upper = pow(2, value) - 1;
	spc = value;

	if (CASE == 0) {
		memset(LHT, low, sizeof(LHT));
		memset(LPT, low, sizeof(LPT));
		memset(GPT, low, sizeof(GPT));
		memset(CPT, low , sizeof(CPT));


	}
	if (CASE == 1) {

		for (int i = 0; i < 8; i++) {
			LHT[i] = lht_higher;
		}
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				LPT[i][j] = high;
			}
		}

		for (int i = 0; i <= ghr_upper; i++) {
			// cout << ghr_upper  << endl;
			GPT[i] = high;
			CPT[i] = high;

		}
	}

	GHR = 0;
}



void moveString(int *pointer, int CASE) {
	if (!CASE) { *pointer += 2;}
	else { *pointer += 3;}
}

char globalOrLocal() {

	if (CPT[GHR] <= 1) {
		// localCount++;
		return 'L';
	}
	// globalCount++;
	return 'G';
}


char predict(char branch, int PC) {
	if (branch == 'G') {
		if (GPT[GHR] <= high / 2) {
			return 'N';
		}
		return 'T';
	}
	else {
		int temp = LHT[PC];
		if (LPT[PC][temp] <= high / 2) {
			return 'N';
		}
		return 'T';
	}
}


void updateGPT(char outcome, int index) {
	bool bits[2];
	int count = 0;
	while (count < 2) {
		bits[count++] = GPT[index] & 1;
		GPT[index] >>= 1;
	}

	if (outcome == 'T') {
		GPT[index] = 1;
	}
	else {
		GPT[index]  = 0;
	}
	count = 1;
	while (count > 0) {
		GPT[index] <<= 1;
		GPT[index] = GPT[index] | bits[count--];
	}

	// cout << " GPT " << GPT[index] << endl;
}




void updateLHT(char outcome, int PC) {
	bool bits[3];
	int count = 0;
	while (count < 3) {
		bits[count++] = LHT[PC] & 1;
		LHT[PC] >>= 1;
	}

	if (outcome == 'T') {
		LHT[PC] = 1;
	}
	else {
		LHT[PC]  = 0;
	}
	count = 2;
	while (count > 0) {
		LHT[PC] <<= 1;
		LHT[PC] = LHT[PC] | bits[count--];
	}

	// cout << " LHT " << LHT[PC] << endl;

}


void updateGHR(char outcome) {
	bool bits[spc];
	int count = 0;
	while (count < spc) {
		bits[count++] = GHR & 1;
		GHR >>= 1;
	}

	if (outcome == 'T') {
		GHR = 1;
	}
	else {
		GHR  = 0;
	}
	count = spc - 1;
	while (count > 0) {
		GHR <<= 1;
		GHR = GHR | bits[count--];
	}

}



void updateTablesPrediction(char outcome, char branch, int PC, bool hit) {
	if (hit) hitCount++;
	if (!hit) missCount++;
	totalCount++;
	// cout << branch <<  " " << hit << endl;
	if (branch == 'G') {
		globalCount++;
		if (outcome == 'T') {
			if (GPT[GHR] != high)
				GPT[GHR]++;
		}
		else {

			if (GPT[GHR] != low)
				GPT[GHR]--;

		}

		if (hit) {
			if (CPT[GHR] != high)
				CPT[GHR]++;
		}
		else {
			if ( CPT[GHR] != low)
				CPT[GHR]--;
		}

	}
	else {
		localCount++;
		int temp = LHT[PC];
		// cout << PC << " ";
		// cout << temp << " ";
		if (outcome == 'T') {
			if (LPT[PC][temp] != high)
				LPT[PC][temp]++;
		}
		else {

			if (LPT[PC][temp] != low)
				LPT[PC][temp]--;
		}

		if (hit) {
			if ( CPT[GHR] != low)
				CPT[GHR]--;
		}
		else {
			if (CPT[GHR] != high)
				CPT[GHR]++;
		}
	}



	updateGPT(outcome, GHR);
	updateLHT(outcome, PC);
	updateGHR(outcome);


}








void predict(string file, int PC[], int sz, int CASE) {
	fstream myfile;
	myfile.open(file);
	string s;
	getline(myfile, s);
	myfile.close();
	// cout << sz << endl;
	int index = 0;

	int filepointer = 0;


	while (filepointer < s.length()) {

		if (s[filepointer] == 'T') {
			if (predict(globalOrLocal(), PC[index]) == 'T') {
				updateTablesPrediction(s[filepointer], globalOrLocal(), PC[index], 1);
			}
			else {
				updateTablesPrediction(s[filepointer], globalOrLocal(), PC[index], 0);
			}
			moveString(&filepointer, 0);
		}
		else {
			if (predict(globalOrLocal(), PC[index]) == 'N') {
				updateTablesPrediction(s[filepointer], globalOrLocal(), PC[index], 1);
			}
			else {
				updateTablesPrediction(s[filepointer], globalOrLocal(), PC[index], 0);
			}
			moveString(&filepointer, 1);
		}
		// cout << index << " ";
		index = (index + 1) % sz;
	}

	string c;
	if (CASE == 0) {
		c = " ALL Zeros ";
	}
	else if (CASE == 1) {
		c = " ALL Ones ";
	}
	// cout << hitCount << "  " << totalCount << endl;
	cout << file << " p = " << spc << " " << c << endl;
	// cout << "Local branch selection  = " << localCount << endl;
	// cout << "Gloabal branch selection = " << " " << globalCount << endl;
	cout <<  "accuracy " <<  (float(hitCount) / float(totalCount) * 100.0f) << endl;
}





int main() {
// #ifndef ONLINE_JUDGE
// 	freopen("input.txt", "r", stdin);
// 	freopen("output.txt", "w", stdout);
// #endif
	// All Zeros and p = 3,4,5
	int pc_codeSegment1[2] = {4, 4};
	int pc_codeSegment2[3] = {4, 4, 4};
	int pc_codeSegment3[4] = {4, 4, 4, 0};



	preditiorIntialize(3, 0);
	predict(file1, pc_codeSegment1, 2, 0);
	cout << endl;
	preditiorIntialize(3, 0);
	predict(file2, pc_codeSegment2, 3, 0);
	cout << endl;
	preditiorIntialize(3, 0);
	predict(file3, pc_codeSegment3, 4, 0);
	cout << endl;

	preditiorIntialize(4, 0);
	predict(file1, pc_codeSegment1, 2, 0);
	cout << endl;
	preditiorIntialize(4, 0);
	predict(file2, pc_codeSegment2, 3, 0);
	cout << endl;
	preditiorIntialize(4, 0);
	predict(file3, pc_codeSegment3, 4, 0);
	cout << endl;

	preditiorIntialize(5, 0);
	predict(file1, pc_codeSegment1, 2, 0);
	cout << endl;
	preditiorIntialize(5, 0);
	predict(file2, pc_codeSegment2, 3, 0);
	cout << endl;
	preditiorIntialize(5, 0);
	predict(file3, pc_codeSegment3, 4, 0);
	cout << endl;


	// // All Zeros and p = 3,4,5

	preditiorIntialize(3, 1);
	predict(file1, pc_codeSegment1, 2, 1);
	cout << endl;
	preditiorIntialize(3, 1);
	predict(file2, pc_codeSegment2, 3, 1);
	cout << endl;
	preditiorIntialize(3, 1);
	predict(file3, pc_codeSegment3, 4, 1);
	cout << endl;

	preditiorIntialize(4, 1);
	predict(file1, pc_codeSegment1, 2, 1);
	cout << endl;
	preditiorIntialize(4, 1);
	predict(file2, pc_codeSegment2, 3, 1);
	cout << endl;
	preditiorIntialize(4, 1);
	predict(file3, pc_codeSegment3, 4, 1);
	cout << endl;

	preditiorIntialize(5, 1);
	predict(file1, pc_codeSegment1, 2, 1);
	cout << endl;
	preditiorIntialize(5, 1);
	predict(file2, pc_codeSegment2, 3, 1);
	cout << endl;
	preditiorIntialize(5, 1);
	predict(file3, pc_codeSegment3, 4, 1);
	cout << endl;






}




