// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include <getopt.h>

#include <vector>
#include <unordered_map>
#include <iostream>
#include <queue>
#include "Field.h"
#include <algorithm>
#include <map>

using namespace std;

class Table {
 public:
  string name;
  int cols;
  vector<ColumnType> coltypes;
  unordered_map<string, int> colIndex;
  vector<vector<Field>> rows;
  int totalRows = 0;
  char indexType = 'n';
  int generateIndex;
  map<Field, vector<int>> b;
  unordered_map<Field, vector<int>> h;
  bool hasGen = false;
  string nameGenerated;
  size_t numKeys;

};

class Database {
  public: 
  unordered_map <string, Table> database;
};


//global variables
bool EXITNOW = false;
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

class equalTo{
  public:
  Field compareData;
  size_t colIndex;
  equalTo(Field &comp, size_t colInd) : compareData(comp), colIndex(colInd) {}

  bool operator()(vector<Field> &row){
    return(row[colIndex] == compareData);
  }
};

class lessThan{
  public:
  Field compareData;
  size_t colIndex;
  lessThan(Field &comp, size_t colInd) : compareData(comp), colIndex(colInd) {}

  bool operator()(vector<Field> &row){
    return(row[colIndex] < compareData);
  }
};

class greaterThan{
  public:
  Field compareData;
  size_t colIndex;
  greaterThan(Field &comp, size_t colInd) : compareData(comp), colIndex(colInd) {}

  bool operator()(vector<Field> &row){
    return(row[colIndex] > compareData);
  }
};

void generate(string tableName, string colname, char indextype){

        db.database[tableName].hasGen = true;
        int columnIndex = db.database[tableName].colIndex[colname];
        size_t numkeys;
        db.database[tableName].generateIndex = columnIndex;
        string outType;

        db.database[tableName].h.clear();
        db.database[tableName].b.clear();

        if(indextype == 'h'){
          outType = "hash";
          db.database[tableName].indexType = 'h';
          for(int i = 0; i < db.database[tableName].totalRows; ++i){
            db.database[tableName].h[db.database[tableName].rows[i][columnIndex]].push_back(i);
          }
          numkeys = db.database[tableName].h.size();
        }

        if(indextype == 'b'){
          outType = "bst";
          db.database[tableName].indexType = 'b';
          for(int i = 0; i < db.database[tableName].totalRows; ++i){
            db.database[tableName].b[db.database[tableName].rows[i][columnIndex]].push_back(i);
          }
          numkeys = db.database[tableName].b.size();
        }
        db.database[tableName].numKeys = numkeys;
        
}

void deleteHelp(char op, string tablename, string colname, Field compdata){
  auto &rows = db.database[tablename].rows;
  size_t checking = db.database[tablename].colIndex[colname];
  switch(op) {
    case '<' :
      rows.erase(remove_if(rows.begin(),rows.end(), lessThan(compdata, checking)),rows.end());
      break;
    
    case '=' :
      rows.erase(remove_if(rows.begin(),rows.end(), equalTo(compdata, checking)),rows.end());
      break;
    case '>' :
      rows.erase(remove_if(rows.begin(),rows.end(), greaterThan(compdata, checking)),rows.end());
    break;
  }
}

void printWhereB(char op, string tableName, Field compdata, size_t N, map<Field, vector<int>> h, vector<int> rowvec, 
  vector<int> colvec){
  int numHits = 0;
  auto &rows = db.database[tableName].rows;
  switch(op) {
    case '=' :
    for(auto & hash : h){
      if (hash.first == compdata){
        ++numHits;
        rowvec = hash.second;
      }
    }
    for(size_t i = 0; i < rowvec.size(); ++i){
      for(size_t j = 0; j < N; ++j){
        cout << rows[rowvec[i]][colvec[j]] << ' ';
      }
      cout << '\n';
    }
    cout << "Printed " << numHits << " matching rows from " << tableName << '\n';
    break;

    case '<' :
    for(auto & hash : h){
      if (hash.first < compdata){
        ++numHits;
        rowvec.insert(rowvec.end(), hash.second.begin(), hash.second.end());
      }
    }
    for(size_t i = 0; i < rowvec.size(); ++i){
      for(size_t j = 0; j < N; ++j){
        cout << rows[rowvec[i]][colvec[j]];
      }
    }
    cout << '\n' << "Printed " << numHits << " matching rows from " << tableName << '\n';
    break;

    case '>' :
    for(auto & hash : h){
      if (hash.first > compdata){
        ++numHits;
        rowvec.insert(rowvec.end(), hash.second.begin(), hash.second.end());
      }
    }
    for(size_t i = 0; i < rowvec.size(); ++i){
      for(size_t j = 0; j < N; ++j){
        cout << rows[rowvec[i]][colvec[j]];
      }
    }
    cout << '\n' << "Printed " << numHits << " matching rows from " << tableName << '\n';
    break;

  }
}

void printWhereH(char op, string tableName, Field compdata, size_t N, unordered_map<Field, vector<int>> h, vector<int> rowvec, 
  vector<int> colvec){
  int numHits = 0;
  auto &rows = db.database[tableName].rows;
  switch(op) {
    case '=' :
    for(auto & hash : h){
      if (hash.first == compdata){
        ++numHits;
        rowvec = hash.second;
      }
    }
    for(size_t i = 0; i < rowvec.size(); ++i){
      for(size_t j = 0; j < N; ++j){
        cout << rows[rowvec[i]][colvec[j]] << ' ';
      }
      cout << '\n';
    }
    cout << "Printed " << numHits << " matching rows from " << tableName << '\n';
    break;

    case '<' :
    for(auto & hash : h){
      if (hash.first < compdata){
        ++numHits;
        rowvec.insert(rowvec.end(), hash.second.begin(), hash.second.end());
      }
    }
    for(size_t i = 0; i < rowvec.size(); ++i){
      for(size_t j = 0; j < N; ++j){
        cout << rows[rowvec[i]][colvec[j]] << ' ';
      }
      cout << '\n';
    }
    cout << "Printed " << numHits << " matching rows from " << tableName << '\n';
    break;

    case '>' :
    for(auto & hash : h){
      if (hash.first > compdata){
        ++numHits;
        rowvec.insert(rowvec.end(), hash.second.begin(), hash.second.end());
      }
    }
    for(size_t i = 0; i < rowvec.size(); ++i){
      for(size_t j = 0; j < N; ++j){
        cout << rows[rowvec[i]][colvec[j]] << ' ';
      }
      cout << '\n';
    }
    cout << "Printed " << numHits << " matching rows from " << tableName << '\n';
    break;

  }
}

void printWhere(char op, string tableName, Field compdata, size_t colIndex, size_t N, vector<int> ind){

  int numHits = 0;
  auto &rows = db.database[tableName].rows;
  switch(op) {
    case '<' :
      for(int i = 0; i < db.database[tableName].totalRows; ++i){
        if(rows[i][colIndex] < compdata){
          ++numHits;
          for(size_t j = 0; j < N; ++j){
            cout << rows[i][ind[j]];
          }
          cout << '\n';
        }
      }
    if(numHits == 0){
    cout << "Printed 0 matching rows from " << tableName << '\n';
    } else{
      cout << "Printed " << numHits << " matching rows from " << tableName << '\n';
    }

    break;

    case '=' :
    for(int i = 0; i < db.database[tableName].totalRows; ++i){
      if(rows[i][colIndex] == compdata){
        ++numHits;
        for(size_t j = 0; j < N; ++j){
          cout << rows[i][ind[j]] << ' ';
        }
        cout << '\n';
      }
    }
    if(numHits == 0){
      cout << "Printed 0 matching rows from " << tableName << '\n';
      } else{
        cout << "Printed " << numHits << " matching rows from " << tableName << '\n';
      }

    break;

    case '>' :
    for(int i = 0; i < db.database[tableName].totalRows; ++i){
      if(rows[i][colIndex] > compdata){
        ++numHits;
        for(size_t j = 0; j < N; ++i){
          cout << rows[i][ind[j]] << ' ';
        }
        cout << '\n';
      }
    }
    if(numHits == 0){
      cout << "Printed 0 matching rows from " << tableName << '\n';
      } else{
        cout << "Printed " << numHits << " matching rows from " << tableName << '\n';
      }

    break;
  }
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
    EXITNOW = false;
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

        int beginningRows = db.database[tableName].totalRows;

        db.database[tableName].totalRows = db.database[tableName].totalRows + numRows;

        db.database[tableName].rows.reserve(db.database[tableName].totalRows);


        // cout << db.database[tableName].cols;
        for(int i = beginningRows; i < numRows + beginningRows; ++i){
          db.database[tableName].rows.emplace_back();
          db.database[tableName].rows.back().reserve(db.database[tableName].cols);
          for(int j = 0; j < db.database[tableName].cols; ++j){
            

            switch(db.database[tableName].coltypes[j]){

              case  ColumnType::Bool : 
                cin >> junk;
                db.database[tableName].rows[i].emplace_back(junk == "true");
                // cerr << db.database[tableName].rows[i].back() << '\n';
              break;
              

              case ColumnType::Double : 
                cin >> junk;
                db.database[tableName].rows[i].emplace_back(stod(junk));
                // cerr << db.database[tableName].rows[i].back() << '\n';
              break;
              

              case ColumnType::String : 
                cin >> junk;
                db.database[tableName].rows[i].emplace_back(junk);
                // cerr << db.database[tableName].rows[i].back() << '\n';
              break;
              

              case ColumnType::Int : 
                cin >> junk;
                db.database[tableName].rows[i].emplace_back(stoi(junk));
                // cerr << db.database[tableName].rows[i].back() << '\n';
              break;
              
            }
          }
        }
        
            
        if(db.database[tableName].indexType == 'b' || db.database[tableName].indexType == 'h'){
          generate(tableName, db.database[tableName].nameGenerated, db.database[tableName].indexType);
        }

        break;
      }
      
      //insert

      case 'P' : {
        bool flag = false;
        int N;
        int index;
        string junk;
        queue<string> printNames;
        vector<string> printNamesVec;
        vector<int> indexList;
        vector<Field> copyVec;
        string name;
        char op;
        string whereCol;


        cin >> junk >> tableName >> N;
        printNamesVec.reserve(N);
        if(doesTableExist(tableName) == false){
          cout << "Error during PRINT: " << tableName << " does not name a table in the database\n";
          getline(cin, command);
          break;
        } 
        //error checking
        for(int i = 0; i < N; ++i){
          cin >> junk;
          printNames.push(junk);
          if(doesColExist(tableName, junk) == false){
            EXITNOW = true;
            cout << "Error during PRINT: " << junk << " does not name a column in " << tableName << '\n';
            getline(cin, junk);
            break;
          }
        }
        
        if(EXITNOW) break;
        // 
        cin >> junk;

        if(junk[0] == 'W'){
          cin >> whereCol >> op;
          int test = db.database[tableName].colIndex[whereCol];
          vector<int> R;
          if((db.database[tableName].indexType == 'h' || db.database[tableName].indexType == 'b') && test == db.database[tableName].generateIndex){

            if(db.database[tableName].indexType == 'h'){
            flag = true;
            for(int i = 0; i < N; ++i){
              cout << printNames.front() << ' ';
              indexList.push_back(db.database[tableName].colIndex[printNames.front()]);
              printNames.pop();
            }

            cout << '\n';

            switch(db.database[tableName].coltypes[test]) {
              case ColumnType::Bool : {
                cin >> junk;
                Field temp(junk == "true");
                printWhereH(op, tableName, temp, N, db.database[tableName].h, R, indexList);
              break;
              }
  
              case ColumnType::Double : {
                cin >> junk;
                double val = stod(junk);
                Field temp(val);
                printWhereH(op, tableName, temp, N, db.database[tableName].h, R, indexList);
              break;
              }
  
              case ColumnType::Int : {
                cin >> junk;
                int val = stoi(junk);
                Field temp(val);
                printWhereH(op, tableName, temp, N, db.database[tableName].h, R, indexList);
              break;
              }
              
              case ColumnType::String : 
                cin >> junk;
                Field temp(junk);
                printWhereH(op, tableName, temp, N, db.database[tableName].h, R, indexList);
              break;
            }
            }
            if(db.database[tableName].indexType == 'b'){
              //bst
              flag = true;
              for(int i = 0; i < N; ++i){
                cout << printNames.front() << ' ';
                indexList.push_back(db.database[tableName].colIndex[printNames.front()]);
                printNames.pop();
              }
              cout << '\n';

              switch(db.database[tableName].coltypes[test]) {
                case ColumnType::Bool : {
                  cin >> junk;
                  Field temp(junk == "true");
                  printWhereB(op, tableName, temp, N, db.database[tableName].b, R, indexList);
                break;
                }
    
                case ColumnType::Double : {
                  cin >> junk;
                  double val = stod(junk);
                  Field temp(val);
                  printWhereB(op, tableName, temp, N, db.database[tableName].b, R, indexList);
                break;
                }
    
                case ColumnType::Int : {
                  cin >> junk;
                  int val = stoi(junk);
                  Field temp(val);
                  printWhereB(op, tableName, temp, N, db.database[tableName].b, R, indexList);
                break;
                }
                
                case ColumnType::String : 
                  cin >> junk;
                  Field temp(junk);
                  printWhereB(op, tableName, temp, N, db.database[tableName].b, R, indexList);
                break;
              }
            }
          }
          

          if((db.database[tableName].indexType == 'n' || db.database[tableName].indexType == 'h' || db.database[tableName].indexType == 'b') && flag == false){ 
            int test = db.database[tableName].colIndex[whereCol];
            for(int i = 0; i < N; ++i){
              cout << printNames.front() << ' ';
              indexList.push_back(db.database[tableName].colIndex[printNames.front()]);
              printNames.pop();
            }
            
            cout << '\n';

            switch(db.database[tableName].coltypes[test]) {
              case ColumnType::Bool : {
                cin >> junk;
                Field temp(junk == "true");
                printWhere(op, tableName, temp, test, N, indexList);
              break;
              }
  
              case ColumnType::Double : {
                cin >> junk;
                double val = stod(junk);
                Field temp(val);
                printWhere(op, tableName, temp, test, N, indexList);
              break;
              }
  
              case ColumnType::Int : {
                cin >> junk;
                int val = stoi(junk);
                Field temp(val);
                printWhere(op, tableName, temp, test, N, indexList);
              break;
              }
              
              case ColumnType::String : 
                cin >> junk;
                Field temp(junk);
                printWhere(op, tableName, temp, test, N, indexList);
            }
          }




          break;
        } // PRINT WHERE

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
        size_t sizeBeforeDelete;
        size_t sizeAfterDelete;

        

      
        cin >> junk >> tableName >> junk >> inputname >> op;
      
        sizeBeforeDelete = db.database[tableName].rows.size();

        if(doesTableExist(tableName) == false){
          cout << "Error during INSERT: " << tableName << " does not name a table in the database\n";
          break;
        } // error checking

        if(doesColExist(tableName, inputname) == false) {
          cout << "Error during DELETE: " << inputname << " does not name a column in " << tableName << '\n';
        }
        
        index = db.database[tableName].colIndex[inputname];

        switch(db.database[tableName].coltypes[index]){
          case ColumnType::String : {
            cin >> val;
            Field deleteInput(val);
            deleteHelp(op, tableName, inputname, deleteInput);
          break;
        }


          case ColumnType::Double : {
            cin >> val;
            double input = stod(val);
            Field deleteInput(input);
            // cerr << "double\n";
            deleteHelp(op, tableName, inputname, deleteInput);
          break;
          }

          case ColumnType::Int : {
            cin >> val;
            int input = stoi(val);
            Field deleteInput(input);
            // cerr << "int\n";
            deleteHelp(op, tableName, inputname, deleteInput);
          break;
          }

          case ColumnType::Bool : {
            cin >> val;
            Field deleteInput(val == "true");
            // cerr << "bool\n";
            deleteHelp(op, tableName, inputname, deleteInput);
          break;
          }
        }
        sizeAfterDelete = db.database[tableName].rows.size();
        db.database[tableName].totalRows = int(sizeAfterDelete);
        cout << "Deleted " << sizeBeforeDelete - sizeAfterDelete << " rows from " << tableName << '\n';

        if(db.database[tableName].indexType == 'b' || db.database[tableName].indexType == 'h'){
          generate(tableName, db.database[tableName].nameGenerated, db.database[tableName].indexType);
        }
        break;
      }
    
      //delete, error checking done
      
      case 'J' : {
        string junk;
        cout << "join\n";
        getline(cin, junk);
        // string tablename1;
        // string tablename2;
        // string colname1;
        // string colname2;
        // int N;
        // int tableIndex;
        // vector<string> columnCheck;
        // vector<int> tableCheck;
        // char op;

        // cin >> tablename1 >> junk >> tablename2 >> junk >> colname1 >> op >> colname2
        // >> junk >> junk >> N;

        // for(int i = 0; i < N; ++i){
        //   cin >> junk >> tableIndex;
        //   columnCheck.push_back(junk);
        //   tableCheck.push_back(tableIndex);
        // }
        
        // auto& rows = db.database[tablename1].rows;
        // int index = db.database[tablename1].colIndex[colname1];
        // auto& rows2 = db.database[tablename2].rows;
        // int index2 = db.database[tablename2].colIndex[colname2];
        // for(size_t i = 0; i < min(rows.size(), rows2.size()); ++i){
        //   if(rows[i][index] == rows2[i][index2]){
        //     cout << "hit\n";
        //   }
        // } 


        break;
      }
      //join
      
      case 'G' :{
        string junk;
        string indexType;
        cin >> junk >> tableName >> indexType >> junk >> junk >> colName;
        if(doesTableExist(tableName) == false) {
          cout << "Error during GENERATE: " << tableName << " does not name a table in the database\n";
          break;
        }

        if (doesColExist(tableName, colName) == false) {
          cout << "Error during GENERATE: " << colName << " does not name a column in " << tableName << '\n';
          break;
        }
        db.database[tableName].nameGenerated = colName;
        generate(tableName, colName, indexType[0]);

        cout << "Generated " << indexType << " index for table " << tableName << " on column " << colName << " with "
         << db.database[tableName].numKeys << " distinct keys\n";
  
      break;
      }
      //generate done

      case 'Q' :
        break;


      default:
      string junk;
      cout << "Error: unrecognized command\n";
      getline(cin, junk);
    } //Error 4

  } while (command != "QUIT");
  cout << "Thanks for being silly!\n";

  
  return 0;
}
