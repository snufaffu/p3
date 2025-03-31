// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include <getopt.h>

#include <vector>
#include <unordered_map>
#include <iostream>
#include <queue>
#include "Field.h"
#include <algorithm>

using namespace std;

class Table {
 public:
  string name;
  int cols;
  int numData = 0;
  vector<ColumnType> coltypes;
  unordered_map<string, int> colIndex;
  vector<vector<Field>> rows;
  int totalRows = 0;
};

class Database {
  public: 
  unordered_map <string, Table> database;
};


// class lessThan {
//   bool operator()(const vector<vector<Field>> & vec, Field val) const {
//     for (auto& v : vec) {
//       if(v < val){
//         return true;
//       }
//     }
//     return false;
//   }
// };


// class equalTo {
//   bool operator()(const vector<vector<Field>> & vec, Field val) const {
//     for (auto& v : vec) {
//       if(v == val){
//         return true;
//       }
//     }
//     return false;
//   }
// };



// class equalTo {
//   Field m_val;
// public:
//   explicit equalTo(Field val) : m_val(val) {}
  
//   bool operator()(const vector<Field>& row) const {
//       return any_of(row.begin(), row.end(),
//           [this](const Field& f) { return f == m_val; });
//   }
// };


// class greaterThan {
//   bool operator()(const vector<vector<Field>> & vec, Field val) const {
//     for (auto& v : vec) {
//       if(v > val){
//         return true;
//       }
//     }
//     return false;
//   }
// };


//global variables
Database db;
bool quietOutput = false;
//

bool doesTableExist(string tableName){
  if(db.database.count(tableName) == 0){
    return false;
  }
  return true;
}

bool doesColExist(string tableName, string columnName){
  if(db.database[tableName].colIndex.count(columnName) == 0){
    return false;
  }
  return true;
}


void deleteHelp(char op){
  switch(op) {
    case '<' :
      cout << "less\n";
      break;
    
    case '=' :
      cout << "equal\n";
      break;

    case '>' :
    cout << "more\n";
    break;
  }
  cout << "helping!\n";
}


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
        cin >> tableName >> numCols;
        if(db.database.count(tableName) == 1){
          cout << "Error during CREATE: Cannot create already existing table " << tableName << '\n';
          getline(cin, command);
          break;
        }
        cout << "New table ";
        cout << tableName << " with column(s) ";
        // cout << "table name: " << tableName << " number of columns: " << numCols << '\n';

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
        cerr << "comment hit\n";
        break;
      //comments 

      case 'R' : 
      cin >> tableName;

      if(doesTableExist(tableName) == false){
        cout << "Error during REMOVE: " << tableName << " does not name a table in the database\n";
        break;
      } // error checking

      if(db.database.count(tableName) == 0){
        cout << "Error during REMOVE: " << tableName << " does not name a table in the database\n";
      }
      db.database.erase(tableName);
      cout << "Table " << tableName << " removed\n";
        break;
      //REMOVE, error 2 is done

      case 'I' :{
        string junk;
        int numRows;
        cin >> junk >> tableName >> numRows >> junk;

        if(doesTableExist(tableName) == false){
          cout << "Error during INSERT: " << tableName << " does not name a table in the database\n";
          break;
        } // error checking


        cout << "Added " << numRows << " rows to " << tableName <<
        " from position " << db.database[tableName].totalRows << " to " 
        << numRows + db.database[tableName].totalRows - 1 << "\n";



        db.database[tableName].totalRows = db.database[tableName].totalRows + numRows;

        db.database[tableName].rows.reserve(numRows + db.database[tableName].totalRows);


        // cout << db.database[tableName].cols;
        for(int i = 0; i < numRows; ++i){
          db.database[tableName].rows.emplace_back();
          db.database[tableName].rows.back().reserve(db.database[tableName].cols);
          for(int j = 0; j < db.database[tableName].cols; ++j){
            

            switch(db.database[tableName].coltypes[j]){

              case  ColumnType::Bool : 
                cin >> junk;
                db.database[tableName].rows[i].emplace_back(junk == "true");
                ++db.database[tableName].numData;
                cerr << db.database[tableName].rows[i].back() << '\n';
              break;
              

              case ColumnType::Double : 
                cin >> junk;
                db.database[tableName].rows[i].emplace_back(stod(junk));
                ++db.database[tableName].numData;
                cerr << db.database[tableName].rows[i].back() << '\n';
              break;
              

              case ColumnType::String : 
                cin >> junk;
                db.database[tableName].rows[i].emplace_back(junk);
                ++db.database[tableName].numData;
                cerr << db.database[tableName].rows[i].back() << '\n';
              break;
              

              case ColumnType::Int : 
                cin >> junk;
                db.database[tableName].rows[i].emplace_back(stoi(junk));
                ++db.database[tableName].numData;
                cerr << db.database[tableName].rows[i].back() << '\n';
              break;
              
            }
          }
        }
            
          

        break;
      }
      
      //insert

      case 'P' : {
        int N;
        int index;
        string junk;
        queue<string> printNames;
        vector<int> indexList;
        string name;

        cin >> junk >> tableName >> N;
        if(doesTableExist(tableName) == false){
          cout << "Error during PRINT: " << tableName << " does not name a table in the database\n";
          getline(cin, command);
          break;
        } 
        //error checking
        for(int i = 0; i < N; ++i){
          cin >> junk;
          printNames.push(junk);
        }

        cin >> junk;

        if(junk[0] == 'W'){
          cout << "come back later";
          break;
        } // not this far yet

        if(junk[0] == 'A'){
          for(int i = 0; i < N; ++i){
            name = printNames.front();
            cout << printNames.front() << ' ';
            indexList.push_back(db.database[tableName].colIndex[name]);
            printNames.pop();
          }
          
          cout << '\n';
            for(int i = 0; i < db.database[tableName].totalRows; ++i){
              for(int j = 0; j < N; ++j){
                index = indexList[j];
                  cout << db.database[tableName].rows[i][index];
                  cout << ' ';
              }
              cout << '\n';
        }
          
        cout << "Printed " << db.database[tableName].totalRows << " matching rows from " << tableName << '\n';

        } //print all

        break;
      }
      //print

      case 'D' : {
        string junk;
        string inputname;
        char op;
        string val;
        int index;

      
        cin >> junk >> tableName >> junk >> inputname >> op;


        if(doesTableExist(tableName) == false){
          cout << "Error during INSERT: " << tableName << " does not name a table in the database\n";
          break;
        } // error checking
        
        index = db.database[tableName].colIndex[inputname];

        switch(db.database[tableName].coltypes[index]){
          case ColumnType::String : {
            cin >> val;
            cerr << "string\n";
          break;
        }


          case ColumnType::Double : {
            cin >> val;
            cerr << "double\n";
          break;
          }

          case ColumnType::Int : {
            cin >> val;
            cerr << "int\n";
          break;
          }

          case ColumnType::Bool : {
            cin >> val;
            bool boolVal = (val == "true");
            cerr << boolVal << '\n';
          break;
          }

        }

        switch(op) {
          case '=' :

          // auto& rows = db.database[tableName].rows;
          // rows.erase(remove_if(rows.begin(), rows.end(), equalTo(rows.back(), val)), rows.end());  
          // db.database[tableName].rows.erase(remove_if(db.database[tableName].rows.begin(), db.database[tableName].rows.end(),
          //  equalTo(db.database[tableName].rows, val)));
          break;
        }

        break;
      }
      //delete WTF
      
      case 'J' : {
        string junk;
        string tablename1;
        string tablename2;
        string colname1;
        string colname2;
        int N;
        int tableIndex;
        vector<string> columnCheck;
        vector<int> tableCheck;
        char op;


        cin >> tablename1 >> junk >> tablename2 >> junk >> colname1 >> op >> colname2
        >> junk >> junk >> N;

        for(int i = 0; i < N; ++i){
          cin >> junk >> tableIndex;
          columnCheck.push_back(junk);
          tableCheck.push_back(tableIndex);
        }
        
        auto& rows = db.database[tablename1].rows;
        int index = db.database[tablename1].colIndex[colname1];
        auto& rows2 = db.database[tablename2].rows;
        int index2 = db.database[tablename2].colIndex[colname2];
        for(size_t i = 0; i < min(rows.size(), rows2.size()); ++i){
          if(rows[i][index] == rows2[i][index2]){
            cout << "hit\n";
          }

        } 


        break;
      }
      //join
      
      case 'G' :
        cout << "generating\n";
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
