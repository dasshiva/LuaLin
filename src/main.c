/*
* min.c
* a minimal Lua interpreter. loads stdin only.
* no standard library, only a "print" function.
*/

#include <stdio.h>
#include <stdlib.h>
#include "lua.h"

/* a simple "print". based on the code in lbaselib.c */
static int print(lua_State *L)
{
 int n=lua_gettop(L);
 int i;
 for (i=1; i<=n; i++)
 {
  if (i>1) printf("\t");
  if (lua_isstring(L,i))
   printf("%s",lua_tostring(L,i));
  else
   printf("%s:%p",lua_typename(L,lua_type(L,i)),lua_topointer(L,i));
 }
 printf("\n");
 return 0;
}

static int donothing(lua_State *L) {
	printf("Hehe");
	lua_error(L, "\nFailure");
	return 0;
}

static char** args;
static int argn;

static int init(lua_State *L) {
	lua_getglobal(L, "sys"); // table 1 
	lua_pushstring(L, "argv");
	lua_gettable(L, -2); // table 2
        for (int i = 0; i <= argn; i++) {
                lua_pushnumber(L, i + 1);
                lua_pushstring(L, *(args + i));
                lua_settable(L, -3);
        }
	lua_pushstring(L, "argv");
	lua_insert(L, -2);
	lua_settable(L, -3);
	lua_setglobal(L, "sys");
	return 0;
}

static int require(lua_State* L) {
	char* file = lua_tostring(L, -1);
	char* new = malloc(strlen(file) + 5);
	strcat(new, file);
	strcat(new, ".lua");
	new[strlen(file) + 4] = '\0';
	int s = lua_dofile(L, new);
	if (s != 0)
		lua_error(L, "Failed to execute required file");
	free(new);
	return 0;
}

int main(int argc, const char** argv)
{
 args = argv;
 argn = argc;
 lua_State *L = lua_open(0);
 lua_register(L,"print",print);
 lua_register(L,"__init_sys__", init);
 lua_register(L,"require", require);
 if (argc < 2) 
   return lua_dofile(L, NULL);
 else 
   return lua_dofile(L, *(argv + 1));
}
