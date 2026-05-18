#ifndef TABLE_H
#define TABLE_H

#include <map>
#include <string>
#include <vector>
using namespace std;

using Row = map<string, string>;

class table {
    private:
        string name;
        vector<string> columns;
        vector<Row> rows;
    public:
        table(const string &n, const vector<string> &col) {
            name = n;
            columns = col;
        }

        void insert(const vector<string> &row_values);
        table where(const vector<string> &where_statement);
        void select(const vector<string> &col_names);
};



#endif //TABLE_H
