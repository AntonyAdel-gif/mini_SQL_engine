#ifndef TYPES_H
#define TYPES_H

#include <string>
using namespace std;

enum ValidationState {
    EXPECT_WORD,
    EXPECT_COMMA
};

enum comparisonOp{
    EQUAL,
    NOT_EQUAL,
    GREATER,
    LESS,
    GREATER_EQUAL,
    LESS_EQUAL,
};

enum LogicalOp {
    NONE,
    AND,
    OR
};

struct Condition {
    string column;
    string value;
    comparisonOp comparison_op;
    LogicalOp Logical_op;

};



#endif //TYPES_H
