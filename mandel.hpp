#ifndef RMZHP_HPP
#define RMZHP_HPP


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <math.h>
#include <immintrin.h>
#include <assert.h>

typedef struct Mandel {

    const float RAD_MAX   = 4;
    const int COLOR_MAX   = 255;
    
    const int WIDTH       = 2 * 600;
    const int HEIGHT      = 2 * 400; 
    
    const float SHIFT_MULTYPLIER = 100;
    const float SCALE_MULTYPLIER = 1.5;

    float x_shift = 0;
    float y_shift = 0;
    float scale   = 100;


    float fps              = 0;
    int character_size     = WIDTH * HEIGHT / 10000;
    const int FPS_BUF_SIZE = 1024;
        
    sf::Uint8* pixels = new sf::Uint8 [WIDTH * HEIGHT * 4];

}Mandel;


void printing_madnelbrot (sf::RenderWindow *window, Mandel *mandel);
void print_fps (sf::Clock *clock, sf::Time *previousTime, sf::RenderWindow *window, sf::Font *font, Mandel* mandel);


#endif