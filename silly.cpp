// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include <getopt.h>

#include <iostream>

#include "Field.h"
using namespace std;

class Table {
 public:
  int rows;
  int cols;
};

bool quietOutput = false;

void printHelp() {
  cout << "SQL thingy\n";
  cout << "only command that works is quiet!\n";
}  // printHelp()

void getOptions(int argc, char** argv) {
  // These are used with getopt_long()
  opterr = static_cast<int>(
      false);  // Let us handle all error output for command line options
  int choice;
  int index = 0;

  struct option longOpts[] = {{"quiet", no_argument, nullptr, 'q'},
                              {"help", no_argument, nullptr, 'h'},
                              {nullptr, 0, nullptr, '\0'},
                              {}};

  while ((choice = getopt_long(argc, argv, "mhvs:", longOpts, &index)) != -1) {
    switch (choice) {
      case 'h':
        printHelp();
        exit(0);  // case 'h'

      case 'q':
        quietOutput = true;
        cout << "quiet mode is on! \n";
        break;
    }
  }
}

int main(int argc, char** argv) {
  ios_base::sync_with_stdio(false); 
  cin >> std::boolalpha;            
  cout << std::boolalpha;           
  getOptions(argc, argv);
  string command;
  do {
    if (cin.fail()) {
      cerr << "Reading from cin has failed! \n";
      exit(1);
    }
    cout << "% ";
    cin >> command;
  } while (command != "QUIT");
  cout << "Thanks for being silly!\n";
  return 0;
}
