#ifndef LUAFUNCTIONTEST_H
#define LUAFUNCTIONTEST_H

#include <string>

class LuaFunctionTest
{
public:
    LuaFunctionTest();
    static void RunLua();
    static std::string GetName();
};

#endif // LUAFUNCTIONTEST_H
