#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include "rollingHash.h"

using namespace std;

/**
 * @brief Prints a banner with the title of the program written with ASCII art.
 */
void printBanner() {
	cout << R"(
 __   __       ___  __             __   __   __   ___          __   __   __   __   __   __       
|__) /  \ \ / |__  |__)  __  |\/| /  \ /  \ |__) |__  __ |__| /  \ |__) /__` |__) /  \ /  \ |    
|__) \__/  |  |___ |  \      |  | \__/ \__/ |  \ |___    |  | \__/ |  \ .__/ |    \__/ \__/ |___ 
                                                                                                
                                                __                                              
                                          \  / /__`                                             
                                           \/  .__/                                             
                                                                                                
                          __        __                       __   __                             
                         |__)  /\  |__) | |\ | __ |__/  /\  |__) |__)                            
                         |  \ /~~\ |__) | | \|    |  \ /~~\ |  \ |                               
                                                                                                
)";
	cout << "                                                                                 by Vasco Pinto" << endl << endl << endl;
}


/**
 * @brief Presents a menu with the different tasks this program can do.
 *
 * @return 0,1,2,3 or 4, depending on what the user wants to do.
 */
unsigned short printMenu() {

	printBanner();

	cout << " ###################### MENU #####################" << endl;
	cout << " #                                               #" << endl;
	cout << " # [1] Search Pattern with Boyer-Moore-Horspool. #" << endl;
	cout << " # [2] Search Pattern with Rabin-Karp.           #" << endl;
	cout << " # [3] Compare the Performance.                  #" << endl;
	cout << " # [4] Export data for Statistical Analysis.     #" << endl;
	cout << " #                                               #" << endl;
	cout << " # [0] Quit.                                     #" << endl;
	cout << " #                                               #" << endl;
	cout << " #################################################" << endl << endl;

	unsigned short option = 0;
	do {
		cout << " Option> ";
		cin >> option;

		// prevents bad input to mess with cin
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			option = 10;
		}

	} while (option < 0 || option > 4);

	cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Remove the '\n' in the cin buffer so it doesn't mess with getline

	return option;
}

/**
 * @brief Scans a directory and returns a vector<string> with the paths of all the files in that directory and sub-directories.
 *
 * This function makes use of the new C++17 library Filesystem.
 *
 * @param directory The directory with the files that we want to search for the pattern.
 *
 * @return vector<string> with the paths of all files in the directory.
 */
vector<string> getFiles(const string& directory) {

	vector<string> files;

	//Recursively scan for files
	for (const auto& file : filesystem::recursive_directory_iterator(directory)) {

		if (!file.is_directory()) //If it's a directory, don't add it to the vector
			files.push_back(file.path().string()); //Add the file path to the vector
	}

	return files;
}

/**
 * @brief Print the current test of the Boyer-Moore-Horspool algorithm.
 *
 * This function prints to screen the pattern, a section of the line being tested and the position the charater being tested.
 *
 * @param pattern The pattern to be searched for.
 * @param line The line currently being analysed.
 * @param pos The beginning of the portion of the line currently being tested.
 * @param fix When the last char of the patt is the same as the last char of the portion of the line that's being tested (same size as patt),
 *         the algorithm will then check the other chars starting from the beginning. This parameter indicates that the line is fix (i.e. not "moving").
 */
void showCurrentTest(const string& pattern, const string& line, const unsigned short& pos, const short fix = -1) {

	cout << "Now testing:" << endl;

	cout << pattern << endl;

	if (fix != -1) { //Last char matched and now we're testing from the beginning of the word
		if (line.substr(pos).length() > 100) //If line is to big to fit the screen, just show a bit 
			cout << line.substr(pos, 100) << endl;
		else
			cout << line.substr(pos) << endl;

		cout << string(fix, ' ') << '^' << endl << endl;
	}
	else {
		if (line.substr(pos).length() > 100) //If line is to big to fit the screen, just show a bit 
			cout << line.substr(pos, 100) << endl;
		else
			cout << line.substr(pos) << endl;

		cout << string(pattern.length() - 1, ' ') << '^' << endl << endl;
	}
}

/**
 * @brief Implementation of the Boyer-Moore-Horspool algorithm for string searching.
 *
 * This function goes through a file and performs the Boyer-Moore-Horspool algorithm to each line.
 *
 * @param patt The pattern to be searched for.
 * @param file The file to search the pattern.
 * @param verbose whether the program is supposed to be verbose or not.
 *
 * @return string with all the lines where the pattern is present.
 */
string searchBoyerMooreHorspool(const string& patt, ifstream& file, const char& verbose) {

	string linesWithPatt;
	string line;
	unsigned int lineNr = 1; //Starting from line 1

	unsigned short lookupTable[256];

	for (unsigned short i = 0; i < 256; i++)
		lookupTable[i] = (unsigned short)patt.length(); //Fill every value with patt.length()

	for (unsigned short i = 0; i < patt.length(); i++)
		lookupTable[int(patt[i])] = ((unsigned short)patt.length() - 1) - i; //Fill the values of the chars in the pattern with their distance from the last char


	while (getline(file, line)) { //Using getline because it's supposed to keep track of the lines where the pattern is present using the var lineNr

		if (line.empty() || line.length() < patt.length()) continue; //Skip empty lines and small lines

		for (unsigned short i = 0; i <= line.length() - patt.length(); i++) {

			if (verbose == 'y')
				showCurrentTest(patt, line, i); //Debuging

			unsigned short distanceEnd = lookupTable[int(line[i + patt.length() - 1])];

			if (distanceEnd) { //If not 0
				i += distanceEnd - 1;
				continue;
			}
			else {
				unsigned short j;
				for (j = 0; j < patt.length() - 1; j++) { //Only testing until j < patt.length() - 1 cause we already know the last char matches

					if (verbose == 'y')
						showCurrentTest(patt, line, i, j); //Debuging

					if (line[static_cast<basic_string<char, char_traits<char>, allocator<char>>::size_type>(i) + j] != patt[j]) //static_cast suggested by VS due to possible arithmetic overflow 
						break;
				}
				if (j == patt.length() - 1) { //Match found
					if (linesWithPatt.empty())
						linesWithPatt = "Line: " + to_string(lineNr) + " Char: " + to_string(i + 1);
					else
						linesWithPatt += ", Line: " + to_string(lineNr) + " Char: " + to_string(i + 1);
					break;
				}
			}
		}
		lineNr++;
	}
	return linesWithPatt;
}

/**
 * @brief Implementation of the Rabin-Karp algorithm for string searching.
 *
 * This function goes through a file and performs the Rabin-Karp algorithm to each line.
 *
 * @param patt The pattern to be searched for.
 * @param file The file to search the pattern.
 * @param verbose whether the program is supposed to be verbose or not.
 *
 * @return string with all the lines where the pattern is present.
 */
string searchRabinKarp(const string& patt, ifstream& file, const char& verbose) {

	string linesWithPatt;
	string line;
	unsigned int lineNr = 1; //Starting from line #1

	rollingHash pattern(patt, (unsigned short)patt.length());

	while (getline(file, line)) { //Using getline beacuse it's supposed to keep track of the lines where the pattern is present

		if (line.empty() || line.length() < patt.length()) continue; //Skip empty lines and small lines


		rollingHash text(line, (unsigned short)patt.length());

		for (unsigned short i = 0; i <= line.length() - patt.length(); i++) {

			if (verbose == 'y') {
				//////////////////// DEBUG ////////////////////
				cout << "Now testing:" << endl;
				cout << patt << endl;
				cout << text.textValue() << endl;
				cout << string(patt.length(), '^') << endl << endl;
				///////////////////////////////////////////////
			}

			if (text.hashValue() == pattern.hashValue()) {
				if (text.textValue() == patt) {
					if (linesWithPatt.empty())
						linesWithPatt = "Line: " + to_string(lineNr) + " Char: " + to_string(text.getStart() + 1);
					else
						linesWithPatt += ", Line: " + to_string(lineNr) + " Char: " + to_string(text.getStart() + 1);
					break;
				}
			}
			text.update();
		}
		lineNr++;
	}
	return linesWithPatt;
}

/**
 * @brief Displays where the pattern is present on a file.
 *
 * This function takes a vector<string> with the location of the pattern in a file and prints it out.
 *
 * @param matches The location the pattern in a file.
 * @param nrFiles The number of files that were searched.
 */
void printMatches(const vector<string>& matches, const unsigned short& nrFiles) {

	//If nrFiles == 0 (i.e. if(true)) print the all the matches in all files
	if (!nrFiles) {
		for (const auto& match : matches)
			cout << match << endl << endl;
	}
	else { //Print just one time the matches in the files (this prevents printing duplicates when using the loopSearches() to test the performance)
		for (unsigned short i = 0; i < nrFiles; i++)
			cout << matches[i] << endl << endl;
	}
}

/**
 * @brief Displays the performance of a given Algorithm while searching for a pattern in a text file.
 *
 * This function takes a time interval and calculates the duration of that interval in milliseconds.
 *
 * @param start The time when the Algorithm started.
 * @param end The time when the Algorithm ended.
 * @param algo The name of the Algorithm.
 */
void printPerformance(const chrono::steady_clock::time_point& start, const chrono::steady_clock::time_point& finish, const string& algo) {
	auto duration = chrono::duration_cast<chrono::milliseconds>(finish - start).count();

	cout << algo << " took " << duration << " milliseconds to find pattern." << endl << endl;
}

/**
 * @brief Runs both Algorithms a certain amount of times to test performance.
 *
 * This function takes a pattern and a directory and searches for the pattern in the files of that directory with both string-searching Algorithms over and over again to analyse their performance.
 *
 * @param patt The pattern to be searched for.
 * @param directory The directory with the files that we want to search for the pattern.
 * @param times The amount of times the function will repeat the search.
 * @param save Whether the function will export the performace of the Algorithms to a file or not.
 */
void loopSearches(const string& pattern, const string& directory, const unsigned short& times, const string& save = "") {
	vector<string> files = getFiles(directory);

	string linesWithPatt;

	vector<string> matchesBM;
	vector<string> matchesRK;

	vector<unsigned int> valuesBM;
	vector<unsigned int> valuesRK;

	for (unsigned short i = 0; i < times; i++) {

		////////////////////////////////// Boyer-Moore-Horspool //////////////////////////////////////
		auto startBM = chrono::steady_clock::now();
		for (const auto& filePath : files) {

			ifstream file(filePath);

			if (!file.good()) {
				cerr << "Error loading file: " << filePath << endl << endl;
				continue;
			}
			linesWithPatt = searchBoyerMooreHorspool(pattern, file, false);

			if (linesWithPatt.empty())
				linesWithPatt = "Pattern not Found!";

			matchesBM.push_back(filePath + " => " + linesWithPatt + " (BM)");
		}
		auto finishBM = chrono::steady_clock::now();

		valuesBM.push_back((unsigned int)chrono::duration_cast<chrono::milliseconds>(finishBM - startBM).count());
		/////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////// Rabin-Karp //////////////////////////////////////////
		auto startRK = chrono::steady_clock::now();
		for (const auto& filePath : files) {

			ifstream file(filePath);

			if (!file.good()) {
				cerr << "Error loading file: " << filePath << endl << endl;
				continue;
			}
			linesWithPatt = searchRabinKarp(pattern, file, false);

			if (linesWithPatt.empty())
				linesWithPatt = "Pattern not Found!";

			matchesRK.push_back(filePath + " => " + linesWithPatt + " (RK)");
		}
		auto finishRK = chrono::steady_clock::now();

		valuesRK.push_back((unsigned int)chrono::duration_cast<chrono::milliseconds>(finishRK - startRK).count());
		/////////////////////////////////////////////////////////////////////////////////////////////

		if (matchesBM.size() != matchesRK.size()) {
			cerr << "Something went wrong." << endl;
			break;
		}
	}
	unsigned int avgDurationBM = 0;
	unsigned int avgDurationRK = 0;

	//If a name for the files was specified
	if (!save.empty()) {

		ofstream outBM(save + "_BM.txt");

		for (const auto& value : valuesBM) {
			avgDurationBM += value;
			outBM << value << endl;
		}
		outBM.close();

		ofstream outRK(save + "_RK.txt");

		for (const auto& value : valuesRK) {
			avgDurationRK += value;
			outRK << value << endl;
		}
		outRK.close();

		cout << endl << "Values saved in folder: " << filesystem::current_path() << endl << endl;
	}
	else {

		for (const auto& value : valuesBM)
			avgDurationBM += value;

		for (const auto& value : valuesRK)
			avgDurationRK += value;
	}

	cout << endl << endl;

	printMatches(matchesBM, (unsigned short)files.size());
	printMatches(matchesRK, (unsigned short)files.size());

	cout << "Average of Boyer-Moore-Horspool: " << avgDurationBM / valuesBM.size() << " milliseconds." << endl;
	cout << "Average of Rabin-Karp: " << avgDurationRK / valuesRK.size() << " milliseconds." << endl << endl;
}

/**
 * @brief Pauses the screen until the ENTER key is pressed.
 *
 * Safer and platform-independent alternative to system("pause").
 */
void pauseScreen() {

	cout << endl << "Press ENTER to continue . . ." << endl;

	cin.ignore();
	cin.get();
}

int main(int argc, char** argv) {

	if (argc == 1) {
		//Interactive Mode
		string pattern;
		string directory;

		vector<string> files;

		char verbose = false;

		unsigned int option = 0;

		do {
			option = printMenu();

			switch (option)
			{

			case 0: break;

			case 1: //BM
			{
				cout << endl << "Enter the Pattern to search for: ";
				getline(cin, pattern);

				cout << endl << "Enter the Directory with the file(s) to be searched: ";
				getline(cin, directory);

				do {
					cout << endl << "Enable Verbose Mode (every single line of the file will be shown in the screen)? (y/n): ";
					cin >> verbose;

					// prevents bad input to mess with cin
					if (cin.fail()) {
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
					}
				} while (verbose != 'y' && verbose != 'n');

				files = getFiles(directory);

				string linesWithPatt;
				vector<string> matches;

				for (const auto& filePath : files) {

					ifstream file(filePath);

					if (!file.good()) {
						cerr << "Error loading file: " << filePath << endl << endl;
						continue;
					}

					linesWithPatt = searchBoyerMooreHorspool(pattern, file, verbose);

					if (linesWithPatt.empty())
						linesWithPatt = "Pattern not Found!";

					matches.push_back(filePath + " => " + linesWithPatt);
				}

				cout << endl << endl;

				//Matches with BM
				printMatches(matches, 0);

				pauseScreen();

				break;
			}
			case 2: //RK
			{

				cout << endl << "Enter the Pattern to search for: ";
				getline(cin, pattern);

				cout << endl << "Enter the Directory with the file(s) to be searched: ";
				getline(cin, directory);

				do {
					cout << endl << "Enable Verbose Mode (every single line of the file will be shown in the screen)? (y/n): ";
					cin >> verbose;

					// prevents bad input to mess with cin
					if (cin.fail()) {
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
					}
				} while (verbose != 'y' && verbose != 'n');

				files = getFiles(directory);

				string linesWithPatt;
				vector<string> matches;

				for (const auto& filePath : files) {

					ifstream file(filePath);

					if (!file.good()) {
						cerr << "Error loading file: " << filePath << endl << endl;
						continue;
					}

					linesWithPatt = searchRabinKarp(pattern, file, verbose);

					if (linesWithPatt.empty())
						linesWithPatt = "Pattern not Found!";

					matches.push_back(filePath + " => " + linesWithPatt);
				}

				cout << endl << endl;

				//Matches with RK
				printMatches(matches, 0);

				pauseScreen();

				break;
			}
			case 3: case 4: //3: Compare, 4: Compare & export
			{
				unsigned int times = 0;

				cout << endl << "Enter the Pattern to search for: ";
				getline(cin, pattern);

				cout << endl << "Enter the Directory with the file(s) to be searched: ";
				getline(cin, directory);

				do {
					cout << endl << "Enter how many Times the algorithms will search for the Pattern: ";
					cin >> times;

					// prevents bad input to mess with cin
					if (cin.fail()) {
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
					}

				} while (times < 1);

				if (option == 3)
					loopSearches(pattern, directory, times);
				else if (option == 4) {
					string save;

					cout << endl << "Enter the Name of the files to export: ";
					getline(cin >> ws, save);

					loopSearches(pattern, directory, times, save);
				}
				else cerr << "Something went wrong." << endl;

				pauseScreen();

				break;
			}
			default:
				cerr << "Invalid Option!" << endl;
				break;
			}
		} while (option != 0);
	}

	else if (argc == 3) {
		//non-Interactive Mode

		printBanner();

		loopSearches(argv[1], argv[2], 1);
	}

	//If the program was run from the Command Prompt in non-Interactive Mode, i.e. with arguments, show usage if there was a mistake
	else {
		cout << "Usage: " << argv[0] << " <pattern> <directory>" << endl;
		cout << "Example (search for the word \"password\" in every file in the current directory): " << argv[0] << " password ." << endl;
		return 1;
	}
	return 0;
}