#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <algorithm>
using namespace std;
#include <string>
#include "types.h"
#include <sstream>

struct ParsedQuery {
    string command;
    string table_name;
    vector<string> columns;
    vector<string> values;
    vector<Condition> where_statement;
    bool isValid;
    string errorMessage;
};

class parser {
private:
    string toUpper(string &s) {
        transform(s.begin(), s.end(), s.begin(), ::toupper);
        return s;
    }

    string trim(string &s) {
        int start = s.find_first_not_of(" \t");
        int end = s.find_last_not_of(" \t");
        return (start == string::npos) ? "" : s.substr(start, end - start + 1);
    }

    string normalizeSpaces(string &s) {
        string result;
        bool previousWasSpace = false;
        for (char c : s) {
            if (isspace(c)) {  // same as    if (c == ' '){}
                if (!previousWasSpace) {
                    result +=c;
                    previousWasSpace = true;
                }
            }
            else {
                result +=c;
                previousWasSpace = false;
            }
        }
        return result;
    }




    bool validateColumnNames(const string& s, string& errorMessage) {
        ValidationState state = EXPECT_WORD;
        string current_token = "";

        for (char c : s) {

            if (isalpha(c) || c == '_') {
                current_token += c;
                if (state == EXPECT_COMMA && current_token.size() == 1) {
                    errorMessage = "Expected comma between column names";
                    return false;
                }
            }

            else if (c == ' ' || c == '\t') {
                if (!current_token.empty()) {
                    if (state == EXPECT_WORD) {
                        state = EXPECT_COMMA;
                    }
                }
            }

            else if (c == ',') {
                if (state == EXPECT_WORD) {
                    errorMessage = current_token.empty()
                        ? "Expected column name, got comma" : "Expected comma between column names";
                    return false;
                }
                current_token = "";
                state = EXPECT_WORD;
            }

            else {
                errorMessage = "Invalid character in column list: ";
                errorMessage += c;
                return false;
            }
        }
        if (state == EXPECT_WORD && current_token.empty()) {
            errorMessage = "Trailing comma in column list";
            return false;
        }

        return true;
    }


    bool isOnlyText(const string &s) {
        return all_of(s.begin(), s.end(), [](unsigned char c) { return isalpha(c) || c=='_'; });
    }

    vector<string> splitBy(const string& s, char delim) {
        vector<string> result;
        stringstream ss(s);
        string token;
        while (getline(ss, token, delim))
            result.push_back(trim(token));
        return result;
    }

public:
    ParsedQuery Parser(string &sql) {
        sql = trim(sql);
        sql = normalizeSpaces(sql);
        stringstream ss(sql);
        vector<string> result;
        ParsedQuery q;
        string word;
        ss >> word;
        q.command = toUpper(word);
        q.isValid = true;

        if (q.command == "CREATE") {
            ss >> word;
            if (word != "TABLE") {
                q.isValid = false;
                q.errorMessage = "Expected TABLE after CREATE, got: " + word;
                return q;
            }
            ss >> word;
            if (isOnlyText(word)) {
                q.table_name = word;
            }
            else {
                q.isValid = false;
                q.errorMessage = "invalid table name";
                return q;
            }
            string rest;
            string err_msg;
            getline(ss, rest);
            int start = rest.find('(');
            int end = rest.find(')');
            int end_query = rest.find(';');
            if (start != string::npos && end != string::npos && start < end) {
                for (int i = end +1 ; i < end_query; i++) {
                    if (rest[i] != ' ') {
                        q.isValid = false;
                        q.errorMessage = "CREATE TABLE statment need to have ';' at the end";
                        return q;
                    }
                }
                rest = rest.substr(start + 1, end - start - 1);
                if(validateColumnNames(rest, err_msg)) {
                    q.columns = splitBy(rest, ',');
                }
                else {
                    q.isValid = false;
                    q.errorMessage = err_msg;
                    return q;
                }
            }
            else {
                q.isValid = false;
                q.errorMessage = "Invalid '(' or ')' position";
                return q;
            }
        }
        if (q.command == "INSERT") {
            ss>>word;
            if (word == "INTO") {
                ss >> word; // table_name validation
                if (word == q.table_name) {
                    vector<string> INSERT_query;
                    for (string s : ss >> word) {
                        if (word == ")") {
                            break;
                        }
                        INSERT_query.push_back(s);
                    }
                    //validate Brackets content
                    for (int i = 1 ; i < INSERT_query.size(); i = i+2) {
                        for (int j = 0 ; j < q.columns.size(); j++) {
                            if (q.columns[j] == INSERT_query[i]) {
                                break;
                            }
                        }
                        else {
                            q.isValid = false;
                            //
                            return q;
                        }
                    }
                    vector<string>().swap(INSERT_query); // clear the vector
                    ss >> word;
                    if (word == "VALUES") {
                        for (string s : ss >> word) {
                            if (word == ")") {
                                break;
                            }
                            INSERT_query.push_back(s);
                        }
                        //validate Brackets content
                        for (int i = 1 ; i < INSERT_query.size(); i = i++) {
                            q.values[i] = INSERT_query[i];
                        }
                    }
                    else {
                        q.isValid = false;
                        q.errorMessage = "Expect 'VALUES' instead of " + word;
                        return q;
                    }
                }
                else {
                    q.isValid = false;
                    //
                    //
                }
            }
            else {
                q.isValid = false;
                q.errorMessage = "Invalid 'INSERT' position";
                return q;
            }
        }


        return q;
    }





};



#endif //PARSER_H
