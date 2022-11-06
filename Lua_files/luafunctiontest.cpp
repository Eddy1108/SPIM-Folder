#include "luafunctiontest.h"
#include "Lua_files/funksjoner.h"

LuaFunctionTest::LuaFunctionTest()
{

}

void LuaFunctionTest::RunLua()
{
    std::cout << "\n - Lua Start! - \n\n";

    //Starter Lua:
    lua_State* lua_vm = luaL_newstate();

    //tester om det gikk bra; stopper hvis ikke...
    if (lua_vm == NULL)
    {
        std::cout << "Lua state not generated!\n";
        return;
    }

    //Open baselibraries:
    luaL_openlibs(lua_vm);

    /***** Lua should now be ready to run *******/

    // 0. testing sending and recieving parameters:***************

    //sending variables:
    lua_pushnumber(lua_vm, 5);
    //set variable name:
    lua_setglobal(lua_vm, "index"); //blir lagt på stacken

    //run ea scriptfile:
    luaL_dofile(lua_vm, "../SPIM-Folder/Lua_files/test0.lua");

    //get the variable:
    lua_getglobal(lua_vm, "fin");	//blir lagt på stacken
    int i = lua_gettop(lua_vm);			//henter toppindexen
    const char* p = lua_tostring(lua_vm, i);	//henter den ut

    //Print result:
    std::cout << "0. - " << p << std::endl << std::endl;

    // 1. - å hente en varabel fra Lua gjøres altså slik:

    //henter ut variabelen (må ha kjørt script-fila først):
    lua_getglobal(lua_vm, "pi");	//blir lagt på stacken
    //bruker -1 som parameter 2. Det gir også toppen av stacken:
    p = lua_tostring(lua_vm, -1);	//henter den ut

    std::cout << "1. - " << p << std::endl << std::endl;

    //2. simpelt script gitt i en string:
    std::cout << "2. - ";
    std::string strScript = "a = 2 + 5;\n print(a);\n";
    luaL_dostring(lua_vm, strScript.c_str());
    std::cout << std::endl;

    //3. kjør en scriptfil:
    std::cout << "3. - ";
    std::cout << "ren script-fil: ";
    luaL_dofile(lua_vm, "../SPIM-Folder/Lua_files/test1.lua");
    std::cout << std::endl;

    //kjører den samme scriptfila, men denne er kompilert med luac.exe
    /*std::cout << "Kompilert version:\n";
    luaL_dofile(lua_vm, "../SPIM-Folder/Lua_files/test1.out");
    std::cout << "Ferdig med kompilert\n";
    std::cout << std::endl;*/

    //4. kjører en annen scriptfil som inneholder en funksjon vi vil kalle:
    //   (se i fila funksjoner.h for besvergelser for å få dette til)
    std::cout << "4. - ";
    luaL_dofile(lua_vm, "../SPIM-Folder/Lua_files/test2.lua");

    //kaller C++funksjonen som kaller lua-funksjonen
    double res = luaFunction(lua_vm, 6.0, 2.0);

    std::cout << res << std::endl;
    std::cout << std::endl;

      //5. kjører en c-funksjon fra lua:
      //   (c-funksjonen ligger i funksjoner.h)

    std::cout << "5. - ";
    //må registrere funksjonen i Lua:
    //pusher selve funksjonen
    lua_pushcfunction(lua_vm, test_Lua);
    //gir den et navn i Lua
    lua_setglobal(lua_vm, "c_funksjon");

    luaL_dofile(lua_vm, "../SPIM-Folder/Lua_files/test3.lua");

    lua_getglobal(lua_vm, "Res1");
    int j = lua_gettop(lua_vm);
    double s = lua_tonumber(lua_vm, j); //igjen, vi kunne brukt -1 her
    std::cout << "C-programmet sier at funksjonen kalt fra Lua gav resultatet: " << s << std::endl << std::endl;


    //Script file test4: // for compulsory 1
    std::cout << std::endl;
    luaL_dofile(lua_vm, "../SPIM-Folder/Lua_files/test4.lua");
    std::cout << std::endl;


    //Lua must be closed at the end:
    lua_close(lua_vm);
}

