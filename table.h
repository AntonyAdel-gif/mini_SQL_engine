#ifndef TABLE_H
#define TABLE_H

#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include <algorithm>
using namespace std;

enum nodeType {
    Logic_OR,
    Logic_AND,
    Condition,
    Operand
};

struct condition {
    string column;
    string value;
    string op;

};

struct node {
    nodeType type;
     string value;
    condition con;

    node *left, *right;

};


using Row = map<string, string>;

class table {
    private:
        string name;
        vector<string> columns;
        vector<Row> rows;
        node *root;
    public:
        table(const string &n, const vector<string> &col) {
            name = n;
            columns = col;
        }

        void insert(const vector<string> &row_values) {
            if (row_values.size() != columns.size()) {
                cout << "❌ Column count mismatch" << endl;
                return;
            }
            Row row;
            for (int i = 0; i < columns.size(); i++) {
                row[columns[i]] = row_values[i];
            }
            rows.push_back(row);
        }


        void select(const vector<string> &col_names);

        vector<string> &get_columns() {
            return columns;
        }

        vector<Row> &get_rows() {
            return rows;
        }

        string &get_name() {
            return name;
        }
};



#endif //TABLE_H
