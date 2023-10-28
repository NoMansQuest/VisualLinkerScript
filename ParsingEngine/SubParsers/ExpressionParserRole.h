#ifndef EXPRESSIONPARSERROLE_H
#define EXPRESSIONPARSERROLE_H

enum class ExpressionParserRole
{
    // This applies to the R-Value of an assignment.
    // Example: ... = (X) + Z + P - (Q);
    // It will not complete by the first valid parentheiss closure encountered,
    // and will continue until either a bracket-open is observed, an invalid parenthesis
    // closure is encountered or when semicolon or coma is encountered. It will be tolerant of misplaced
    // ternary operators (i.e. colon and question-mark).
    RValueOfAssignmentParser,

    // This applies to expression passed as parameter to a function
    // Example: ... = ALIGN((ALPHA + 0x200) + BETA + 0x100);
    // It will complete upon detection of first valid parentheiss closure
    // or when a semicolon or coma is encountered. It will be tolerant of misplaced
    // ternary operators (i.e. colon and question-mark).
    FunctionParameterParser,

    // This applies to expression passed as parameter to an assignment procedure such as 'PROVIDE'.
    // Example: PROVIDE (GAMMA = ALIGN((ALPHA + 0x200) + BETA + 0x100));
    // It will complete upon detection of first valid parentheiss closure
    // or when a semicolon or coma is encountered. It will be tolerant of misplaced
    // ternary operators (i.e. colon and question-mark).
    RValueOfAssignmentProcedureCall,

};

#endif // EXPRESSIONPARSERROLE_H
