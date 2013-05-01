#include "app_manager.h"

using namespace elements;
using namespace std;
AppMngr::AppMngr() {
}

AppMngr::AppMngr(const AppMngr& orig) {
}

AppMngr::~AppMngr() {
}

void AppMngr::demo(int type, Image *myImg, Image *myImg2, Image *myImg3, int algorithm)
{
    CvCapture* capture = NULL;
    IplImage* frame, *preOutImage=NULL, *outImage= NULL;
    //IplImage* frame1 = NULL, *frame2 = NULL, *frame3 = NULL;
    bool first_time = true, is_available = true;
    long long int cont = 0;
    double *vec = NULL, vecTemp[3], *filter = NULL, *filter2=NULL,*vector_image = NULL, *vec_max = NULL, *vec_min = NULL;
    Data g = Data();
    FFTWManager FFTWmngr, FFTWmngrFilter;
    UIHandler ui_handler;

    char c = 'a';

    if(type == 0)
    {
        capture = cvCaptureFromCAM(0);
    }

    vec_max = new double[3];
    vec_min = new double[3];
    vec_max[2] = vec_max[1] = vec_max[0] = -9999;
    vec_min[2] = vec_min[1] = vec_min[0] =  9999;

    ui_handler.set_algorithm(algorithm);

    frame = cvQueryFrame(capture);

    while (ui_handler.is_unable_to_continue() == false)
    {
        frame = cvQueryFrame(capture);

        if (!frame) break;
        cvShowImage("IN", frame);
        cvMoveWindow( "IN", 100, 10 );

        if (first_time == true) {
            myImg->initialize( frame->height,  frame->width, 1);
            myImg2->initialize(frame->height, frame->width, -1); //XYZ Image
            myImg3->initialize(frame->height, frame->width, -1); //CIELab Image
            filter = Util::create_filter(0,frame->height, frame->width);
            Util::create_filter_data(&g,3,2);
            FFTWmngr.initialize(frame->height * frame->width);
            FFTWmngr.Plan_2D(frame->height,frame->width);
            FFTWmngrFilter.InitializeAndExecuteFromAVector(filter,frame->height,frame->width,1);
            vector_image = new double[frame->height*frame->width*3];
            first_time = false;
        }
        ImageCreater::opencv_image_to_image(frame, myImg);
        myImg->remove_zeros(false);
        if (cont % 7 == 0) {
            is_available = true;
        } else {
            is_available = false;
        }
        //GRIS, ROJO, VERDE, AZUL, AMARILLO,COLORES, UMBRAL, BORDES, DIFUSO, EXIT_DEMO=20
        switch(ui_handler.algorithm)
        {
            case 0:  //................GRIS
            {
                myImg->to_grayscale();
                myImg->full_range_adjust(1,255.0,NOTHING, NULL, NULL, true);
            }break;
            case 1:  //................ROJO
            {
                myImg->to_color(0);
                myImg->full_range_adjust(0,255.0,NOTHING, NULL, NULL, true);
            }break;
            case 2:  //................VERDE
            {
                myImg->to_color(1);
                myImg->full_range_adjust(0,255.0,NOTHING, NULL, NULL, true);
            }break;
            case 3:  //................AZUL
            {
                myImg->to_color(2);
                myImg->full_range_adjust(0,255.0,NOTHING, NULL, NULL, true);
            }break;
            case 4:  //................AMARILLO
            {
                myImg->to_color(3);
                myImg->full_range_adjust(0,255.0,NOTHING, NULL, NULL, true);
            }break;
            case 5: //............... MOSAICO
            {
                myImg->to_mosaic(3, cont);
                myImg->full_range_adjust(0.0,0.0,NOTHING, NULL, NULL, true);
            }break;
            case 6: //............... UMBRAL
            {
                myImg->to_grayscale();
                //myImg->grayscale_filter_by_fft(&FFTWmngr, &FFTWmngrFilter, vector_image);
                int t = myImg->get_threshold_by_otsu(RED);
                myImg->threshold_umbralization(t);
                myImg->full_range_adjust(0,255.0,NOTHING, NULL, NULL, true);
            }break;
            case 7: // ............. BORDES
            {
                myImg->to_grayscale();
                printf("%s\n","SEM After 1");
                //myImg->full_range_adjust(0,0,NOTHING,NULL,NULL, true);
                printf("%s\n","SEM After 2");

                //myImg->to_double_vector(1, vector_image);
                //myImg->grayscale_filter_by_fft(&FFTWmngr, &FFTWmngrFilter, vector_image);

                //myImg->convolution(&g,0,true);
                printf("%s\n","SEM After 3");
                //int t = myImg->get_threshold_by_otsu(RED);
                myImg->threshold_umbralization(80);
                printf("%s\n","SEM After 4");
                myImg->full_range_adjust(0,255.0,NOTHING, NULL, NULL, true);
                printf("%s\n","SEM After 5");

            }break;
            case 8: // ............. DIFUSO
            {
                myImg->to_grayscale();
                //myImg->full_range_adjust(0,0,NOTHING,NULL,NULL, true);
                myImg->to_double_vector(1, vector_image);
                myImg->grayscale_filter_by_fft(&FFTWmngr, &FFTWmngrFilter, vector_image);
                //myImg->Convolution(&g,0,true);
                //int t = myImg->get_threshold_by_otsu(RED);
                //myImg->threshold_umbralization(80);
                myImg->full_range_adjust(0,255.0,DOUBLE_DATA_2_UCHAR_DATA, NULL, NULL, true);

            }break;
            case 9:  //................INVERSE
            {
                myImg->to_color(4);
                myImg->full_range_adjust(0,255.0,NOTHING, NULL, NULL, true);
            }break;
            case 20: //................... EXIT_DEMO
            {

            }break;
        }
        cvShowImage("OUT", frame);
        cvMoveWindow( "OUT", 10+frame->width, 10 );

        cont++;

        cvShowImage("OUT", outImage);
        cvMoveWindow( "OUT", 10+frame->width, 10 );
        c = cvWaitKey(50);
        ui_handler.parse_key_for_demo(c);
    }


    if (frame != NULL)
        cvReleaseImage(&frame);
    if(preOutImage != NULL)
        cvReleaseImage(&preOutImage);
    if(outImage != NULL)
        cvReleaseImage(&outImage);
    if(vec_max!=NULL)
        delete []vec_max;
    if(vec_min!=NULL)
        delete []vec_min;
    if(vector_image!=NULL)
        delete [] vector_image;
    if(filter != NULL)
        delete [] filter;
    if(filter2!=NULL)
        delete [] filter2;

    FFTWmngr.Free();
    FFTWmngrFilter.Free();

    if (capture != NULL)
        cvReleaseCapture(&capture);

    if (frame != NULL)
        cvReleaseImage(&frame);
    if(preOutImage != NULL)
        cvReleaseImage(&preOutImage);

    frame =NULL;
    preOutImage=NULL;
    outImage =NULL;
    vec_max = NULL;
    vec_min = NULL;
    vector_image = NULL;
    filter = NULL;
    filter2 = NULL;
    capture = NULL;
}