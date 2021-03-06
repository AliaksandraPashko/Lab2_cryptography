#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <bitset>

using namespace std;

string addition(string coefs1, string coefs2) {
	string result = "";
	if (coefs1.length() != coefs2.length()) {
		if (coefs1.length() < coefs2.length()) {
			coefs1.insert( coefs1.begin(),coefs2.length() - coefs1.length(),'0');
		}
		else
			coefs2.insert(coefs2.begin(),coefs1.length() - coefs2.length(), '0');
	} 
	for (int i = 0; i < coefs1.length(); i++) {
		result+= to_string((coefs1[i] ^ coefs2[i]));
	}
	return result;
}

string division(string coefs1, string coefs2,bool need_ost) {
	int temp;
	int res = 0;
	int c1 = strtoul(coefs1.c_str(), NULL, 2);
	int c2 = strtoul(coefs2.c_str(), NULL, 2);
	int lg1 = (int)log2(c1);
	int lg2 = (int)log2(c2);
	while (lg1 - lg2 >= 0 && c1!=0) {
	/*	temp = c2;
		temp = temp << (lg1 - lg2);
		res += 1 << (lg1 - lg2);
		c1 = c1 ^ temp;
		lg1 = (int)log2(c1);
		*/
		c1 = c1 ^ ( c2 << lg1 - lg2 );
		res += 1 << lg1 - lg2;
		lg1 = (int)log2(c1);
	}
	string ost = coefs1;
	if (need_ost){
		return std::bitset<16>(c1).to_string();
	}
	else {
		string result = std::bitset<32>(res).to_string();
		return result;
	}
}


string multiplication(string coefs1, string coefs2,string poly) {
	int res = 0;
	int c1 = strtoul(coefs1.c_str(), NULL, 2);
	int c2 = strtoul(coefs2.c_str(), NULL, 2);
	for (int i = 0; i < coefs1.length(); i++) {
		if (coefs1[i] == '1')
			res = res ^ (c2 << (coefs1.length())-1 -i);
	}
	string result = std::bitset<32>(res).to_string();
	return division(result, poly,1);
	//return result;
}


string exponentiation(string coefs, string operation,string poly) {
	int power = atoi((operation.erase(0, 1)).c_str());
	string result = coefs;
	for (int i = 1; i < power; i++ ) {
		result = multiplication(result, coefs,poly);
	}
	return result;
}

int gcd(int a, int b, int& x, int& y, string poly) {
	if (a == 0) {
		x = 0;
		y = 1;
		return b;
	}
	int x1, y1;
	string a_ = std::bitset<16>(a).to_string();
	string b_ = std::bitset<16>(b).to_string();
	if (strtoul(multiplication(a_, b_, poly).c_str(), NULL, 2) == 1)
		return a;
 	int d = gcd(strtoul(division(b_,a_,0).c_str(), NULL, 2), a, x1, y1, poly);
	string div = division(b_, a_, 0);
	int mul = strtoul(multiplication(div, std::bitset<16>(x1).to_string(), poly).c_str(), NULL, 2);
	x = y1 ^ mul;
	y = x1;
	return d;
}

string exponentiation_1(string coefs,string poly) {
	int x, y;
	int a = strtoul(coefs.c_str(), NULL, 2);
	int m = strtoul(poly.c_str(), NULL, 2);
	x = gcd(a, m, x, y, poly);
	//return multiplication(std::bitset<16>(x).to_string(), coefs, poly);
	return std::bitset<16>(x).to_string();
}


int main() {
	ifstream fin("input.txt");
	vector <string> input_vector;
	while (!fin.eof()) {
		string str;
		fin >> str;
		input_vector.push_back(str);
	}
	fin.close();

	ifstream fpoly("polynom.txt");
	string poly;
	getline(fpoly, poly);
	fpoly.close();

	ofstream fout("output.txt");
	if (input_vector.size() == 3) {
		switch ((input_vector[1])[0])
		{
		case '+':
				fout << addition(input_vector[0], input_vector[2]);
			break;
		case '*':
				fout << multiplication(input_vector[0], input_vector[2],poly);
			break;
		case '/':
				fout << division(input_vector[0], input_vector[2], 0);
			break;
		default:
			break;
		}
	}
	else {
		if ((input_vector[1])[1] == '-')
			fout << exponentiation_1(input_vector[0], poly);
		else
			fout << exponentiation(input_vector[0], input_vector[1], poly);
	}
	return 0;
} 

