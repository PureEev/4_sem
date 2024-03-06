#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

string AddingSpace(int NumberOfSpaces, string NewLine, int CountOfCurCharInLine) {
	if (NumberOfSpaces >= 1) {
		while (NumberOfSpaces > 0) {
			NewLine.append(" ");
			NumberOfSpaces--;
			CountOfCurCharInLine++;
		}
	}
	return NewLine;
}

int main(int argc, char* argv[])
{
	string FileName = argv[0];
	int NewLengthOfLine = int(argv[1]) ;
	vector<string> NewLines;
	string baseline;
	string RemainsOfPrevLine;
	string NewLine;
	int NumberOfSpaces = 0;
	int LengthOfCurrentLine = 0;
	int CountOfCurCharInLine = 0;
	ifstream FILE(FileName);
	while (getline(FILE, baseline)) {
		if (RemainsOfPrevLine.length() != 0) {
			string tmp = baseline;
			baseline.clear();
			baseline.append(RemainsOfPrevLine);
			baseline.append(tmp);
			RemainsOfPrevLine.clear();
		}
		istringstream Line(baseline);
		istream_iterator<string> begin(Line);
		istream_iterator<string> end;
		vector<string> words(begin, end);
		LengthOfCurrentLine += baseline.length();
		NumberOfSpaces = NewLengthOfLine - LengthOfCurrentLine;
		NewLine = AddingSpace(NumberOfSpaces, NewLine, CountOfCurCharInLine);
		CountOfCurCharInLine = NewLine.length();
		for (const string& word : words) {
			if ((CountOfCurCharInLine + word.length()) > NewLengthOfLine) {
				RemainsOfPrevLine.append(word);
				RemainsOfPrevLine.append(" ");
			}
			else {
				NewLine.append(word);
				NewLine.append(" ");
				CountOfCurCharInLine += word.length();
			}
		}
		NewLine = AddingSpace(NumberOfSpaces, NewLine, CountOfCurCharInLine);
		NewLines.push_back(NewLine);
		NewLine.clear();
		NumberOfSpaces = 0;
		LengthOfCurrentLine = 0;
	}
	ofstream file(argv[2]);
	for (const string& line : NewLines) {
		file << line << endl;
	}
	return 0;
}
