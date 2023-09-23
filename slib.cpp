#include "slib.h"
#include "lodepng.h"
#include "smath.hpp"

namespace slib
{

    texture DecodePng(const char* filename) 
    {
        std::vector<unsigned char> buffer;
        std::vector<unsigned char> image; //the raw pixels
        lodepng::load_file(buffer, filename);
        unsigned width, height;

        lodepng::State state;
    
        //decode
        unsigned error = lodepng::decode(image, width, height, state, buffer);
        std::cout << width << ", " << height << "\n";
        const LodePNGColorMode& color = state.info_png.color;
        auto bpp = lodepng_get_bpp(&color);
    
        //if there's an error, display it
        if(error) 
        {
            std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
            exit(1);
        }
    
        //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
        return { static_cast<int>(width), static_cast<int>(height), image,  4 };
    }


void Camera::Rotate(float x, float y)
    {
        const float sensitivity = 0.075f;
        rotation.y -= x * sensitivity;
        rotation.x -= y * sensitivity;
    }
}