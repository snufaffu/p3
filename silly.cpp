// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include <getopt.h>

#include <vector>
#include <unordered_map>
#include <iostream>


#include "Field.h"
using namespace std;

class Table {
 public:
  string name;
  int cols;

  Table(string n, int c) : name(n), cols(c) {} 

  private:
  void generate_table(int columns, string tablename){
    name = tablename;
    cols = columns;
  }
};


//global variables
bool quietOutput = false;
vector<string> coltypes;
vector<string> colnames;
unordered_map<string, Table> tables;
//


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
  // -----------------
  string tableName;
  uint32_t numCols;
  string colName;
  string colType;

  string command;
  do {
    if (cin.fail()) {
      cerr << "Reading from cin has failed! \n";
      exit(1);
    }
    cout << "% ";
    cin >> command;
    switch(command[0]) {

      case 'C' :
        cin >> tableName >> numCols;
        cout << "table name: " << tableName << " number of columns: " << numCols << '\n';
        for(size_t i = 0; i < numCols; ++i){
          cin >> colType;
          coltypes.push_back(colType);
        }
        for(size_t i = 0; i < numCols; ++i){
          cin >> colName;
          colnames.push_back(colName);
        }
        // for(size_t i = 0; i < numCols; ++i){
        //   tables.insert({colnames[i]}, coltypes[i]);
        // }
        break;
      // CREATE, error 1 is here

      case '#' :
        getline(cin, command);
        cout << "comment hit\n";
        break;
      //comments 

      case 'R' :

        break;
      //remove

      case 'I' :

        break;
      //insert

      case 'P' :

        break;
      //print

      case 'D' :

        break;
      //delete

      case 'J' :

        break;
      //join
      
      case 'G' :

        break;
      //generate

      default:
      cout << "Error: unrecognized command\n";
    } //Error 4

  } while (command != "QUIT");
  cout << "Thanks for being silly!\n";

  
  return 0;
}
