/*
Author	Manik Mahajan
		H20201030039G
		h20200039@goa.bits-pilani.ac.in

This code generates true outcomes for following -

Code Segment-1, Code Segment-2, Code Segment-3

The true outcomes are stored in following files -
branchOutcome1.txt, branchOutcome2.txt, branchOutcome3.txt

*/


#include<iostream>
#include<fstream>

using namespace std;

#define file1 "branchOutcome1.txt"
#define file2 "branchOutcome2.txt"
#define file3 "branchOutcome3.txt"

void codeSegment1() {
	fstream myfile;
	myfile.open(file1, ios::trunc);
	int a, b;
	a = 10000;
	if (a) { myfile << "T,";}
	while (a > 0) { //PC:0b0100_0100

		b = 50;
		if (b > 0) { myfile << "T,"; }
		while (b > 0) { //PC: 0b0110_1100

			b = b - 2;
			if (b > 0) { myfile << "T,"; }
			else { myfile << "NT,"; }
		}

		a = a - 1;
		if (a > 0) { myfile << "T,"; }
		else { myfile << "NT,"; }
	}
	myfile.close();
}



void codeSegment2() {
	fstream myfile(file2);
	int a, b = 5;
	a = 0;
	if (a < 10000) { myfile << "T,";}
	while (a < 10000) { //PC:0b0100_0100


		if (a % 2 == 0) { myfile << "T,"; }
		else { myfile << "NT,"; }
		if (a % 2 == 0) { //PC: 0b0110_1100
		}



		a = a + 1;


		if (b == 0) { myfile << "T,"; }
		else { myfile << "NT,"; }
		if (b == 0) {
		}


		if ((a) < 100000) { myfile << "T,"; }
		else { myfile << "NT,"; }
	}
}

void codeSegment3() {
	ofstream myfile (file3);
	int a, b, m;
	m = 10000;
	if (m > 0) { myfile << "T,";}
	while (m > 0) { //PC:0b0100_0100


		a = m % 2;
		if (a == 2) { myfile << "T,"; }
		else { myfile << "NT,"; }
		if (a == 2) { //PC: 0b0110_1100
			a = 0;
		}


		b = (int) m / 2;
		if (b == 2) { myfile << "T,"; }
		else { myfile << "NT,"; }
		if (b == 2) { //PC: 0b0111_0100
			b = 0;
		}


		if (a != b) { myfile << "T,"; }
		else { myfile << "NT,"; }
		if (a != b) { //PC: 0b1000_1000
			m = m - 2;
		}



		m = m - 1;
		if (m) { myfile << "T,"; }
		else { myfile << "NT,"; }
	}

	myfile.close();
}

int main() {

	codeSegment1();
	codeSegment2();
	codeSegment3();

}