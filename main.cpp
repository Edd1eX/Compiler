#include "include/lexicalAnalysis.h"
#include "include/syntaxAnalysis.h"

int main()
{
    vector<word> lexical = lexicalAnalysis();
    syntaxAnalysis(lexical);
    return 0;
}