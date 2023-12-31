bin/lua: $(wildcard src/*.c) bin/liblua.a
	$(CC) $^ -Iinclude/ -o bin/lua
