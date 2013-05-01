/*
    Computer Vision Demo

    - github.com/salvadorbfm/cv_demo
    - Author: Salvador Elizarrarás Montenegro
    - Licensed under the MIT license

    main.cpp
*/
#include <stdlib.h>
#include <ctime>
#include <opencv2/highgui/highgui.hpp>
#include <fftw3.h>
#include "elements/image_creater.h"
#include "elements/image.h"
#include "elements/fftw_manager.h"
#include "elements/util.h"
#include "elements/image_analyser.h"
#include "app_ui/ui_handler.h"
#include "app_ui/app_manager.h"

using namespace elements;
using namespace std;

int main(int argc, char** argv) {
    char cad[30];
    Image myImg, myImg2, myImg3, myImg4;

    enum Option {
        image_opt, auto_opt, video_opt, camera_opt, test_opt, algorithms_opt, demo_opt, exit_opt=20
    };
    enum Algorithm{
        WPR, WPR_I, GW, HORN_W, HORN_GW, HOMOMORPHIC, HOMOMORPHIC_HSV, LOCALCOLOR_1, EXIT_ALG = 20
    };
    enum DemoAlgorithm{
        GRIS, ROJO, VERDE, AZUL, AMARILLO, COLORES, UMBRAL, BORDES, DIFUSO,INVERSO, EXIT_DEMO=20
    };
    Option option = video_opt;
    srand ( time(NULL) );
    cvNamedWindow("IN", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("OUT", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Histograms", CV_WINDOW_AUTOSIZE);

    IplImage* img = NULL, *img2 = NULL, *lplOut = NULL, *histRimg=NULL, *histGimg=NULL, *histBimg = NULL, *histLimg=NULL;
    strcpy(cad, "demo");                                                // For option
    do
    {
        DemoAlgorithm demo_algorithm;
        option = (Option)Util::get_option_enum(cad);
        switch(option)
        {
            case exit_opt:
            {}break;
            case demo_opt:
            {
                demo_algorithm = (DemoAlgorithm)Util::get_demo_algorithm_enum(cad);
                do
                {
                    strcpy(cad, "colores");
                    demo_algorithm = (DemoAlgorithm)Util::get_demo_algorithm_enum(cad);
                    AppMngr::demo(0,&myImg,&myImg2,&myImg3,(int)demo_algorithm);
                    Util::format_a3("Escribe el algoritmo que deseas");
                    scanf("%s",cad);
                }while(demo_algorithm != EXIT_DEMO);
                Util::format_a2("Has entrado al modo demo_opt");
                Util::format_a3("Escribe el algoritmo que deseas");
                scanf("%s",cad);                                        // For option
                break;
            }
        }
        Util::format_a3("Elige un modo");
        scanf("%s",cad);
    }while(option != exit_opt);

    cvDestroyAllWindows();
    if (img != NULL) {
        cvReleaseImage(&img);
    }
    if (img2 != NULL) {
        cvReleaseImage(&img2);
    }
    if (lplOut != NULL) {
        cvReleaseImage(&lplOut);
    }
    if(histRimg != NULL) {
        cvReleaseImage(&histRimg);
    }
    if(histGimg != NULL) {
        cvReleaseImage(&histGimg);
    }
    if(histBimg != NULL) {
        cvReleaseImage(&histBimg);
    }
    if(histLimg != NULL) {
        cvReleaseImage(&histLimg);
    }
    return (EXIT_SUCCESS);

}