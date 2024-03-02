#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

void AddingSpace(int NumberOfSpaces, string NewLine, int CountOfCurCharInLine) {
	if (NumberOfSpaces > 1) {
		while (NumberOfSpaces > 0) {
			NewLine.append(" ");
			NumberOfSpaces--;
			CountOfCurCharInLine++;
		}
	}
}

int main(int argc, char* argv[])
{
	string FileName = argv[1];
	int NewLengthOfLine = int(argv[2]);
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
			NewLine.append(RemainsOfPrevLine);
			RemainsOfPrevLine = "";
			LengthOfCurrentLine += NewLine.length() + 1;
		}
		istringstream Line(baseline);
		istream_iterator<string> begin(Line);
		istream_iterator<string> end;
		vector<string> words(begin, end);
		LengthOfCurrentLine += baseline.length();
		NumberOfSpaces = LengthOfCurrentLine - NewLengthOfLine;
		AddingSpace(NumberOfSpaces, NewLine, CountOfCurCharInLine);
		for (const string& word : words) {
			if ((CountOfCurCharInLine + word.length()) > NewLengthOfLine) break;
			NewLine.append(word);
			NewLine.append(" ");
		}
		AddingSpace(NumberOfSpaces, NewLine, CountOfCurCharInLine);
		NewLines.push_back(NewLine);
	}
	ofstream file(argv[2]);
	for (const string& line : NewLines) {
		file<< line <<endl;
	}
	return 0;
}

