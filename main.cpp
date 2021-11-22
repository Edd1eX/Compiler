#include "include/lexicalAnalysis.h"
#include "include/syntaxAnalysis.h"
#include "include/codeGeneration.h"
int main()
{
    vector<word> lexical = lexicalAnalysis();
    syntaxAnalysis(lexical);
    generation();
    return 0;
}