Mandelbrot optimizaton with SIMD instructions
=============================================

Usage
-----------
1.  Proceccor
    > AMD Ryzen 5 4500U
2.  Instructions
    > AVX, AVX2
3.  Language
    > C, C++
4.  Graphic library
    > SFML

Compile
-------
+       < g++ main.cpp -O2 -mavx -mavx2 -I"C:\Program Files\MinGW64\SFML-2.5.1\include" \
                      -L"C:\Program Files\MinGW64\SFML-2.5.1\lib" \
                      -lsfml-graphics -lsfml-window -lsfml-system >
