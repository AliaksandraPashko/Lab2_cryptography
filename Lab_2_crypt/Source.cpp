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

string division(string coefs1, string coefs2) {
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
	string result = std::bitset<16>(res).to_string();
	return result;
}


string multiplication(string coefs1, string coefs2,string poly) {
	int res = 0;
	int c1 = strtoul(coefs1.c_str(), NULL, 2);
	int c2 = strtoul(coefs2.c_str(), NULL, 2);
	for (int i = 0; i < coefs1.length(); i++) {
		if (coefs1[i] == '1')
			res = res ^ (c2 << (coefs1.length() - 1 - i));
	}
	string result = std::bitset<16>(res).to_string();
	result = division(result, poly);
	return result;
}


string exponentiation(string coefs, string operation,string poly) {
	int power = atoi((operation.erase(0, 1)).c_str());
	string result = coefs;
	for (int i = 1; i < power; i++ ) {
		result = multiplication(result, coefs,poly);
	}
	return result;
}

string exponentiation_1(string coefs,string poly) {
	// a*x = 1(mod m)
	// x = (a % m + m) % m;
	string res = division(addition(division(coefs, poly), poly), poly);
//	return res;
	return multiplication(res,coefs,poly);
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
				fout << division(input_vector[0], input_vector[2]);
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

