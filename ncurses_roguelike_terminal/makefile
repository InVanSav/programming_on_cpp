CXX = clang++
CFLAGS = -Wall\
		-Weverything\
		-Wextra\
		-Wno-poison-system-directories\
		-Wno-padded\
		-Wno-implicit-int-conversion\
		-Wno-newline-eof\
		-Wno-sign-conversion\
		-Wno-unused-parameter\
		-Wno-missing-prototypes\
		-Wno-conditional-uninitialized\
		-Wno-constant-conversion\
		-Wno-shorten-64-to-32\
		-Wno-c++11-extensions\
		-fsanitize=undefined\
		-lncurses\
		-g

SOURCES = ./*.cpp

all: roguelike run clean

roguelike:
	$(CXX) $(SOURCES) $(CFLAGS) -o $@

run:
	./roguelike

clean:
	rm -rf roguelike roguelike.dSYM