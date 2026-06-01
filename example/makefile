compiler = g++
file = main
target = $(file).exe

cpp_linker = ..\external\glad\src\glad.c

lib_linker = -llibglfw3 -lopengl32 -lgdi32

run : compile
	.\$(target)

compile :
	$(compiler) $(file).cpp -o $(target) $(cpp_linker) $(lib_linker)