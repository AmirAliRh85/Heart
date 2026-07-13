compiler = gcc
file = main.c
target = main.exe

external_c_linker = .\external\glad\src\glad.c .\external\ark\src\ark_memory.c .\external\ark\src\ark_log.c .\external\ark\src\ark_algorithm.c
c_linker =  .\src\Window.c .\src\Renderer.c .\src\Mouse.c .\src\Keyboard.c .\src\BatchDraw.c
lib_linker = -llibglfw3 -lopengl32 -lgdi32 -llibfreetype
# warning_tags = -Wall -Wextra
# debug_tags = -ggdb

run : compile
	.\$(target)

compile :
	$(compiler) $(file) -o $(target) $(external_c_linker) $(c_linker) $(lib_linker)