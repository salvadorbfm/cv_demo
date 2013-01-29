#ifndef _APP_MANAGER_H
#define _APP_MANAGER_H
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <fstream>
#include <fftw3.h>
#include "opencv2/highgui/highgui.hpp"
#include "../elements/image_creater.h"
#include "../elements/image.h"
#include "../elements/fftw_manager.h"
#include "../elements/util.h"
#include "ui_handler.h"
#include "../elements/image_analyser.h"

using namespace elements;
class AppMngr {
public:
    AppMngr();
    AppMngr(const AppMngr& orig);
    virtual ~AppMngr();

    static void demo(int type, Image *myImg, Image *myImg2, Image *myImg3, int algorithm);
    static void exec_image(Image *myImg, Image *myImg2, Image *myImg3, int algorithm);
    static void exec_image(Image *myImg, Image *myImg2, Image *myImg3, Image *myImg4);  // This is for automatic selection
    static void exec_image_with_algorithm(Image *myImg, Image *myImg2, Image *myImg3, int algorithm);
    void manage_keyboard_input(int c, int* algorithm, bool *prev_image, bool *next_image, int *numberAlbum);
    void manage_format_image(int numberAlbum, char *format_image);

private:

};
#endif