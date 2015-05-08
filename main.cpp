#include <iostream>
#include <string>
#include "LuaWrapper.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

class Human {
private:
	int age;
	const char* name;

public:
	Human(const char* name, int age);
	int getAge() { return age; };
	void setAge(int a);
	void say(const char* speech);
};

Human::Human(const char* n, int a) {
	name = n;
	age = a;
}

void Human::setAge(int a) {
	age = a;
}

void Human::say(const char* speech) {
	std::cout << speech << std::endl;
}

Human* Human_new(lua_State *L) {
	const char* name = luaL_checkstring(L, 1);
	int age = luaL_checknumber(L, 2);
	return new Human(name, age);
}

int Human_getAge(lua_State *L) {
	Human* homoNova = luaW_check<Human>(L, 1);
	lua_pushnumber(L, homoNova->getAge());
	return 1;
}

static luaL_Reg Human_table[]{
	{NULL,NULL}
};

static luaL_Reg Human_metatable[]{
	{"getAge",Human_getAge},
	{NULL,NULL}
};

static int luaopen_Human(lua_State *L) {
	luaW_register<Human>(L, "Human", Human_table, Human_metatable,Human_new);
	return 1;
}


int main() {
	
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	luaopen_Human(L);
	
	if (luaL_loadfile(L, "test.lua")) {
		std::cout << "Error, can't open script" << std::endl;
	}

	lua_call(L, 0, LUA_MULTRET, 0);
	
	system("PAUSE");
	return 0;
}