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
  vector<ColumnType> coltypes;
  unordered_map<string, int> colIndex;
  vector<ColumnType> rows;
};

class Database {
  public: 
  unordered_map <string, Table> database;
};


//global variables
Database db;
bool quietOutput = false;
vector<Table> tablesVector;
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

      case 'C' : {
        cout << "New table ";
        cin >> tableName >> numCols;
        cout << tableName << " with column(s) ";
        // cout << "table name: " << tableName << " number of columns: " << numCols << '\n';
        if(db.database.count(tableName) > 1){
          cout << "Error during CREATE: Cannot create already existing table " << tableName << '\n';
          break;
        }

        Table newTable;
        newTable.name = tableName;
        newTable.cols = numCols;

        for(size_t i = 0; i < numCols; ++i){
          cin >> colType;
          switch (colType[0]) {
            case 'd' : 
              newTable.coltypes.push_back(ColumnType::Double);
            break;

            case 'i' :
              newTable.coltypes.push_back(ColumnType::Int);
            break;

            case 'b' :
              newTable.coltypes.push_back(ColumnType::Bool);
            break;

            case 's' :
              newTable.coltypes.push_back(ColumnType::String);
            break;
          }
        }

        for(size_t i = 0; i < numCols; ++i){
          cin >> colName;
          newTable.colIndex.insert({colName, i});
          cout << colName << ' ';
        }
        db.database.insert({tableName, newTable});
        cout << "created\n";
        break;
      }
      // CREATE, error 1 is done

      case '#' :
        getline(cin, command);
        cout << "comment hit\n";
        break;
      //comments 

      case 'R' : 
      cin >> tableName;
      if(db.database.count(tableName) == 0){
        cout << "Error during REMOVE: " << tableName << "does not name a table in the database\n";
      }
      db.database.erase(tableName);
      cout << "Table " << tableName << " removed\n";
        break;
      //REMOVE, error 2 is done

      case 'I' :{
        std::string junk;
        int numRows;
        cin >> junk >> tableName >> numRows >> junk;
        db.database[tableName].rows.resize(numRows);
        cin >> junk;
        db.database[tableName].rows.emplace_back(junk);
        // for(int i = 0; i < numRows; ++i){
        //   for(int j = 0; i < db.database[tableName].cols; ++j){
        //     switch(db.database[tableName].coltypes[j]){

        //       case  ColumnType::Bool : {
        //         // cin >> junk;
        //         // db.database[tableName].rows.emplace_back(junk == "true");
        //       break;
        //       }

        //       case ColumnType::Double : {
        //         // cin >> junk;
        //         // db.database[tableName].rows.emplace_back(stod(junk));
        //       break;
        //       }

        //       case ColumnType::String : {
        //         cin >> junk;
        //         db.database[tableName].rows.emplace_back(Field(junk));
        //       break;
        //       }

        //       case ColumnType::Int : {
        //         // cin >> junk;
        //         // db.database[tableName].rows.emplace_back(stoi(junk));
        //       break;
        //       }
            
          

        break;
      }
      
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

      case 'Q' :
        break;


      default:
      cout << "Error: unrecognized command\n";
    } //Error 4

  } while (command != "QUIT");
  cout << "Thanks for being silly!\n";

  
  return 0;
}
