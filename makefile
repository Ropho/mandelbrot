all:
	g++ main.cpp -O2 -mavx -mavx2 -I"C:\Program Files\MinGW64\SFML-2.5.1\include" \
                      -L"C:\Program Files\MinGW64\SFML-2.5.1\lib" \
                      -lsfml-graphics -lsfml-window -lsfml-system