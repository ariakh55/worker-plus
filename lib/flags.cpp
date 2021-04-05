#include "flags.h"

double math(double __initiative, double __addition, char *__operation)
{
    double result = 0;

    cout<<__initiative << " " << __addition << " " << __operation << endl;

    if (string(__operation, 0, 3) == "add")
        result = __initiative + __addition;

    if(string(__operation, 0, 3) == "sub")
        result = __initiative - __addition;

    if(string(__operation, 0, 3) == "div")
        result = __initiative / __addition;

    if(string(__operation, 0, 4) == "mult")
        result = __initiative * __addition;
    
    cout<<result<<endl;

    return result;
}