#include "lua.hpp"  //lua include that has the extern "C" thing internally
#include <iostream>

//for å kunne kalle en funksjon definert i Lua som "div_numbers"
double luaFunction (lua_State *LS, double x, double y) {

    double result;	//svarvariabel

	/* push functions and arguments */
    lua_getglobal(LS, "div_numbers");  /* get function to be called */
    lua_pushnumber(LS, x);   /* push 1st argument */
    lua_pushnumber(LS, y);   /* push 2nd argument */

	/* do the call (2 arguments, 1 result)
    med litt feilhåndtering
    Når fjerde parameter er 0 vil stacken få feilmeldingen ved feil
    (lua_call() gjør det samme, men uten feiltesting)
    lua_call(LS, 2, 1)
    */

    if (lua_pcall(LS, 2, 1, 0) != 0)
    {
        std::cout << "error running function f \n";
        //Print error message that Lua gives
        std::cout << lua_tostring(LS, -1) << std::endl;
    }
	/* retrieve result 
    tester først om vi får et tall */
    if (!lua_isnumber(LS, -1)){
        std::cout << "function 'div_numbers' must return a number \n";
		return 0;
	}

    result = lua_tonumber(LS, -1);   //-1 gir toppen av stacken
    lua_pop(LS, 1);  /* pop returned value */

    return result;
}

//En funksjon som kan kalles fra Lua:
static int test_Lua (lua_State *LS)
{
    //henter indexen paa toppen av stacken
    int arg = lua_gettop(LS);
	
	//henter ut nummer:
    double d = lua_tonumber(LS, arg);

    //Den fantastiske funksjonen som C++ skal utføre!
	d *= 2;

	//sender resultatet til Lua:
    lua_pushnumber(LS, d);

	//indikerer til Lua at 1 resultat er returnert:
	return 1;
}
