compiler = gcc
file = main.c
target = main.exe

c_linker = .\external\glad\src\glad.c .\src\Window.c .\src\Renderer.c .\src\Mouse.c .\src\Keyboard.c .\src\BatchDraw.c .\src\Utils.c
lib_linker = -llibglfw3 -lopengl32 -lgdi32 -llibfreetype
# warning_tags = -Wall -Wextra
# debug_tags = -ggdb

run : compile
	.\$(target)

compile :
	$(compiler) $(file) -o $(target) $(c_linker) $(lib_linker)