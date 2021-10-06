#include "include/lexicalAnalysis.h"
#include "include/grammaAnalysis.h"

int main()
{
    vector<word> lexical = lexicalAnalysis();
    grammaAnalysis(lexical);
    return 0;
}