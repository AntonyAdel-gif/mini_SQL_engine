#ifndef OPERATIONS_H
#define OPERATIONS_H
using namespace std;
#include "table.h"
#include "types.h"
#include <string>

class operations {
    public:
    table where(table Table, vector<Condition> where_stat) {
        table new_table(Table.get_name(), Table.get_columns());
        vector<string> rows_to_insert;
        string row_string;
        bool result = true;


        for (const auto &row : Table.get_rows()) {
            result = true;
            for (const auto &condition : where_stat) {
                if (condition.Logical_op == AND) {
                    result = result && evaluate(row, condition);
                }
                else if (condition.Logical_op == OR) {
                    result = result || evaluate(row, condition);
                }
                else if (condition.Logical_op == NONE) {
                    result = evaluate(row, condition);
                }
            }

            if (result) {
                for (const auto &column : Table.get_columns()) {
                    row_string = row.at(column); //row is a map <string, string>
                    rows_to_insert.push_back(row_string);
                }
                new_table.insert(rows_to_insert);
                rows_to_insert.clear();
            }
        }
        return new_table;
    }

    private:
    bool evaluate(map<string, string> &single_row, const Condition &con) {
        if (con.comparison_op == EQUAL && stoi(single_row[con.column]) == con.value) {
            return true;
        }
        if (con.comparison_op == NOT_EQUAL && stoi(single_row[con.column]) != con.value) {
            return true;
        }
        if (con.comparison_op == GREATER && stoi(single_row[con.column]) > con.value) {
            return true;
        }
        if (con.comparison_op == LESS && stoi(single_row[con.column]) < con.value) {
            return true;
        }
        if (con.comparison_op == GREATER_EQUAL && stoi(single_row[con.column]) >= con.value) {
            return true;
        }
        if (con.comparison_op == LESS_EQUAL && stoi(single_row[con.column]) <= con.value) {
            return true;
        }

        else return false;
    }
};



#endif //OPERATIONS_H
