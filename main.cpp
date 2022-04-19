#include "mandel.hpp"


// #define SSE


int main (void) {


    Mandel mandel = {};

    sf::RenderWindow window (sf::VideoMode (mandel.WIDTH, mandel.HEIGHT), "Mandel32-www.ded.com");
    
    sf::Clock clock;
    sf::Time previousTime = clock.getElapsedTime ();

    sf::Font font;
    font.loadFromFile (".\\font.ttf");

    while (window.isOpen ()) {

        sf::Event event;

        while (window.pollEvent (event)) {

            if (event.type == sf::Event::Closed)
                window.close ();

            if (event.type == sf::Event::KeyPressed) {

                switch (event.key.code) {

                    case (sf::Keyboard::Left):
                        mandel.x_shift -= mandel.SHIFT_MULTYPLIER / mandel.scale;
                    break;

                    case (sf::Keyboard::Right):
                        mandel.x_shift += mandel.SHIFT_MULTYPLIER / mandel.scale;
                    break;

                    case (sf::Keyboard::Up):
                        mandel.y_shift -= mandel.SHIFT_MULTYPLIER / mandel.scale;
                    break;

                    case (sf::Keyboard::Down):
                        mandel.y_shift += mandel.SHIFT_MULTYPLIER / mandel.scale;
                    break;

                    case (sf::Keyboard::PageUp):
                        mandel.scale *= mandel.SCALE_MULTYPLIER;
                    break;

                    case (sf::Keyboard::PageDown):
                        mandel.scale /= mandel.SCALE_MULTYPLIER;
                    break;

                    default:
                        break;
                }
            }
        }

        window.clear (sf::Color::Black);

        printing_madnelbrot (&window, &mandel);
        print_fps (&clock, &previousTime, &window, &font, &mandel);

        window.display ();
    }

    delete (mandel.pixels);
}


void printing_madnelbrot (sf::RenderWindow* window, Mandel* mandel) {

    sf::Texture layer;

    layer.create (mandel->WIDTH,  mandel->HEIGHT);
    layer.update (mandel->pixels, mandel->WIDTH, mandel->HEIGHT, 0, 0);

    sf::Sprite sprite;
    sprite.setTexture (layer);

    sf::Uint8* pixels = mandel->pixels;


#ifndef SSE


    for (int y_int = 0; y_int < mandel->HEIGHT; y_int++) {

        float y0 = (((float)y_int -  mandel->HEIGHT / 2) / mandel->scale + mandel->y_shift);

        for (int x_int = 0; x_int < 4 * mandel->WIDTH; x_int += 4) {

            float x0 = (((float)x_int  / 4 - mandel->WIDTH / 2) / mandel->scale + mandel->x_shift);

            float x_cur = x0;
            float y_cur = y0;

            int  color = 0;
            for (color = 0; color < mandel->COLOR_MAX; ++color) {

                float sqr_x = x_cur * x_cur; 
                float sqr_y = y_cur * y_cur; 
                float xy    = x_cur * y_cur;
                
                float sqr_sum = sqr_x + sqr_y;

                if (sqr_sum >= mandel->RAD_MAX)
                    break;

                x_cur = sqr_x - sqr_y + x0;
                y_cur =  xy   +  xy   + y0;
            }

 
            float color_base = sqrtf (sqrtf (float (color) / float (mandel->COLOR_MAX))) * 255.f;

            for (int num = 0; num < 4; ++num) {
                // pixels [x_int + mandel->WIDTH * 4 * y_int + num] = color_base;            
                if (num == 0)
                    pixels [x_int + mandel->WIDTH * 4 * y_int + num] = color_base * 25;
                else if (num == 1 || num == 2) 
                    pixels [x_int + mandel->WIDTH * 4 * y_int + num] = 0;
                else
                    pixels [x_int + mandel->WIDTH * 4 * y_int + num] = 255;
            }

        }
    }
#else


    const __m256  _RAD_MAX      = _mm256_set1_ps (mandel->RAD_MAX);
    const __m256  _255          = _mm256_set1_ps (255.f);
    const __m256i _COLOR_MAX    = _mm256_set1_epi32 (mandel->COLOR_MAX);
    
   
    for (int y_int = 0; y_int < mandel->HEIGHT; y_int++) {
        

        float y = (((float)y_int -  mandel->HEIGHT / 2) / mandel->scale + mandel->y_shift);

        for (int x_int = 0; x_int < 4 * mandel->WIDTH; x_int += 4 * 8) {

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
            float x_conv = (((float)(x_int)  / 4 - mandel->WIDTH / 2) / mandel->scale + mandel->x_shift);
            
            float x_conv_all [8] = {x_conv};
            for (int index = 1; index < 8; ++index) {
                x_conv_all [index] = x_conv_all [index - 1] + 1.f / mandel->scale;
            }

            __m256 x0 = _mm256_set_ps  (x_conv_all[0], x_conv_all[1],
                                        x_conv_all[2], x_conv_all[3],
                                        x_conv_all[4], x_conv_all[5],
                                        x_conv_all[6], x_conv_all[7]);

            __m256 y0 = _mm256_set1_ps (y);

            __m256 x_cur = x0;
            __m256 y_cur = y0;
            
            __m256i _color = _mm256_setzero_si256 ();

            int  color = 0;
            for (color = 0; color < mandel->COLOR_MAX; ++color) {

                __m256 sqr_x = _mm256_mul_ps   (x_cur, x_cur);
                __m256 sqr_y = _mm256_mul_ps   (y_cur, y_cur);
                __m256 xy    = _mm256_mul_ps   (x_cur, y_cur);
                
                __m256 sqr_sum = _mm256_add_ps (sqr_x, sqr_y);

/////////////////////////
                __m256 _cmp = _mm256_cmp_ps (sqr_sum, _RAD_MAX, _CMP_LE_OQ);

                int adder   = _mm256_movemask_ps (_cmp);
                if (!adder) {
                    break;
                }
                
                _color = _mm256_sub_epi32 (_color, _mm256_castps_si256 (_cmp));

/////////////////////////
                
                x_cur = _mm256_add_ps (_mm256_sub_ps (sqr_x, sqr_y), x0);
                y_cur = _mm256_add_ps (_mm256_add_ps (xy, xy), y0);
            }
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

            __m256 _color_base = _mm256_mul_ps (_mm256_sqrt_ps (_mm256_div_ps(_mm256_cvtepi32_ps (_color),  _mm256_cvtepi32_ps (_COLOR_MAX) ) ), _255);

        for (int index = 0; index < 8; ++index) {

                // int *_color_ptr = (int*) &_color;               //easier to get colors
                
                float* color_base = (float*) &_color_base;      //more vivid colors
                
                unsigned char intensive = _color_base [7 - index];
                            
                
            for (int num = 0; num < 4; ++num) {
                    if (num != 3)
                        pixels [mandel->WIDTH * 4 * y_int + x_int + 4 * index + num] = 255 - intensive * 25;
                    else
                        pixels [mandel->WIDTH * 4 * y_int + x_int + 4 * index + num] = 255;

            }
        }         
      }
    }



#endif


    layer.update (mandel->pixels, mandel->WIDTH, mandel->HEIGHT, 0, 0);

    (*window).draw (sprite);
}


void print_fps (sf::Clock* clock, sf::Time* previousTime, sf::RenderWindow* window, sf::Font* font, Mandel* mandel) {
    
    sf::Time currentTime;
    sf::Text fps_text;
    
    char fps_buf [mandel->FPS_BUF_SIZE] = {0};

    currentTime = (*clock).getElapsedTime();
    mandel->fps = 1.0f / (currentTime.asSeconds () - (*previousTime).asSeconds ()); // the asSeconds returns a float
    (*previousTime) = currentTime;
    sprintf (fps_buf, "%04.2f", mandel->fps);

    fps_text.setCharacterSize (mandel->character_size);
    fps_text.setFont ((*font));
    fps_text.setString (fps_buf);
    fps_text.setPosition (0, 0);
    fps_text.setColor (sf::Color::Green);

    (*window).draw (fps_text);
}
