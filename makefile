gol: gol.cpp
	$(CXX) -std=c++20 gol.cpp -o gol -lgraphics64 -luuid -lmsimg32 -lgdi32 -limm32 -lole32 -loleaut32 -lwinmm -lgdiplus

clean: gol.exe
	rm gol.exe
