#include "image_creater.h"

using namespace elements;
ImageCreater::ImageCreater() {
}

ImageCreater::~ImageCreater() {
}

void  ImageCreater::opencv_image_to_image(IplImage* img, Image* myImg)
{
   myImg->rgb.ptr_data = (uchar*) ( img->imageData );//+ y * img->widthstep );
   myImg->rgb.ptr_orig = (uchar*) ( img->imageData );
   myImg->rgb.widthstep = img->widthStep;//3*sizeof(double)*img->width;//img->widthstep;

}

IplImage* ImageCreater::image_to_opencv_image(Image* sImag, bool is_double_data)
{
    int height = sImag->get_rows();
    int width = sImag->get_cols();   //IPL_DEPTH_32F
    register int index = 0;
    IplImage* img =  cvCreateImage(cvSize(width,height),IPL_DEPTH_8U ,3);

    if(is_double_data == true)
    {
        sImag->full_range_adjust(0,255,DOUBLE_DATA_2_DOUBLE_DATA,NULL,NULL,true);
        //sImag->DeNormalize(true);
        for(int i=0;i<height; i++){
            uchar* ptr = (uchar*) ( img->imageData + i * img->widthStep );
            for(int j=0; j<width; j++){
                index = i*3*width +3*j;
                ptr[3*j+0] = (uchar)sImag->rgb.ptr_double_data[index + 0];
                ptr[3*j+1] = (uchar)sImag->rgb.ptr_double_data[index + 1];
                ptr[3*j+2] = (uchar)sImag->rgb.ptr_double_data[index + 2];
            }
        }
    }else
    {
        if(sImag->Type == 1 || sImag->Type == -1)
        {
            sImag->full_range_adjust(0,255,UCHAR_DATA_2_UCHAR_DATA,NULL, NULL,true);

            for(int i=0;i<height; i++){
                uchar* ptr = (uchar*) ( img->imageData + i * img->widthStep );
                sImag->rgb.ptr_data = (uchar *)(sImag->rgb.ptr_orig + i*sImag->rgb.widthstep);
                for(int j=0; j<width; j++){
                    ptr[3*j+0] = sImag->rgb.ptr_data[3*j + 0];//[i*3*width +3*j  +0];
                    ptr[3*j+1] = sImag->rgb.ptr_data[3*j + 0];//[i*3*width +3*j  +1];
                    ptr[3*j+2] = sImag->rgb.ptr_data[3*j + 0];//[i*3*width +3*j  +2];
                }
            }


        }
    }

    return img;

}


void ImageCreater::create_mask(Data* matrix, mask_type maskType)
{
    double value;
   int nMax = Util::max(matrix->rows, matrix->cols);

    double sigma  = nMax*0.087;
    //ptrVoid = matrix->ptr_double_data;
    for(int i=0; i<matrix->rows; i++)
    {
        //matrix->ptr_data = (uchar*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
        //matrix->ptr_data = (uchar*)(matrix->ptr_orig + i*matrix->cols*sizeof(double));

        for(int j=0; j<matrix->cols; j++)
        {
            switch(maskType)
            {
                case AVG3x3:
                    value = (1.0/3.0);
                    matrix->ptr_double_data[i*matrix->cols+j] = value;
                    break;
                case AVG5x5:
                    //if(j%2 == 0)
                        value = (1.0)/(5.0);
                    //else
                    //    value = (1.0)/(3.0);
                    matrix->ptr_double_data[i*matrix->cols+j] = value;
                    //matrix->ptr_data += j*sizeof(double);
                    matrix->ptr_data = (uchar *) (matrix->ptr_orig + i*matrix->widthstep + j*sizeof(double));

                    //cout << endl << matrix->ptr_double_data[i*matrix->cols+j] << " -- "<< *((double*)matrix->ptr_data);//<<" -- " << (double)((matrix->ptr_data[j]));
                    break;
                case EXP:
                    value = exp(-(sqrt((i)*(i)+(j)*(j)))/(sigma));
                    matrix->ptr_double_data[i*matrix->cols+j] = value;
                    //matrix->ptr_data += j*sizeof(double);
                    matrix->ptr_data = (uchar *) (matrix->ptr_orig + i*matrix->widthstep + j*sizeof(double));

                    break;
                case GAUSSIAN:
                    break;

            }

        }
    }
//    for(int i=0; i<(matrix->rows*matrix->cols)*3 ; i++)
//        {
//            matrix->ptr_data = (uchar*)(matrix->ptr_orig + i*sizeof(double));
//            cout << endl << (double)matrix->ptr_data[0];
//            cout << endl << (double)matrix->ptr_data[1];
//            cout << endl << (double)matrix->ptr_data[2];
//            cout << endl;        }

}

void ImageCreater::compute_images(IplImage* img, Image *myImg)
{
    char bufferIN[256], buffer[256];
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            //printf(bufferIN, "images/CC/TEST/yo%i.jpg", i);
            img = cvLoadImage(bufferIN);

            if (img == NULL)
            {
                cout << endl << "Error at loading image";
                return ;
            }
            //cvShowImage( "IN", img );
            myImg->initialize(img->height, img->width, 1);

            ImageCreater::opencv_image_to_image(img, myImg);
            switch (j)
            {
                case 0:
                   //printf(buffer, "OUT/TEST2/wpr_%i_in.bmp", i);
                    cvSaveImage(buffer, img);
                    //printf(buffer, "OUT/TEST2/wpr_%i_out.bmp", i);
                    myImg->white_patch_retinex();
                    //cvShowImage("preOUT",img);
                    cvSaveImage(buffer, img);
                    break;

                case 1:
                    printf(buffer, "OUT/TEST2/wpr2_%i_in.bmp", i);
                    cvSaveImage(buffer, img);
                    printf(buffer, "OUT/TEST2/wpr2_%i_out.bmp", i);
                    myImg->white_patch_retinex_reloaded(0.001, 1, true);
                    //cvShowImage("preOUT",img);-
                    cvSaveImage(buffer, img);
                    break;

                case 2:
                    printf(buffer, "OUT/TEST2/gw_%i_in.bmp", i);
                    cvSaveImage(buffer, img);
                    printf(buffer, "OUT/TEST2/gw_%i_out.bmp", i);
                    myImg->gray_world(0);
                    cvSaveImage(buffer, img);
                    //cvShowImage("preOut",img);
                    break;

                case 3:
                    printf(buffer, "OUT/TEST2/hornWPR_%i_in.bmp", i);
                    cvSaveImage(buffer, img);
                    printf(buffer, "OUT/TEST2/hornWPR_%i_out.bmp", i);
                    myImg->variant_of_horn(0, true, NULL);
                    cvSaveImage(buffer, img);
                    //cvShowImage("preOut",img);
                    break;

                case 4:
                    printf(buffer, "OUT/TEST2/hornGW_%i_in.bmp", i);
                    cvSaveImage(buffer, img);
                    printf(buffer, "OUT/TEST2/hornGW_%i_out.bmp", i);
                    myImg->variant_of_horn(1, true, NULL);
                    cvSaveImage(buffer, img);
                    //cvShowImage("preOut",img);
                    break;

            }

        }
    }

}


int ImageCreater::is_null(IplImage* img)
{
    if (img == NULL) {
        cout << endl << "Error at loading image";
        return (EXIT_FAILURE);
    }
    return 0;
}


void ImageCreater::load_image_by_opencv(IplImage** img, const char * pathname)
{
    if(*img != NULL)
        cvReleaseImage(img);
    *img = cvLoadImage(pathname);

}
//switch(comp)
//{
//  case GRAY:
//      channelIndex = 1;
//      break;
//  case RED:
//      channelIndex = 2;
//      break;
//  case GREEN:
//      channelIndex = 1;
//      break;
//  case BLUE:
//      channelIndex = 0;
//      break;
//}
//if( (height-1-i) < (Rhistogram.datos[j]) )
//{
//  matrix[i][j] = 255.0;
//}else
//{
//  matrix[i][j] = 0.0;
//}
IplImage* ImageCreater::histogram_image_by_opencv(Image *myImg)
{
    int height = 200;
    int width  = 256;   //IPL_DEPTH_32F
    register int index = 0;
    double max=0.0;
    int pix = 0;
    unsigned char RMatrix[200][256], GMatrix[200][256], BMatrix[200][256],VMatrix[200][256];
    ulint N = (ulint)myImg->get_rows()*myImg->get_cols();
    SVector Rhistogram = myImg->histogram_vector(RED,0,false);
    SVector Ghistogram = myImg->histogram_vector(GREEN,0,false);
    SVector Bhistogram = myImg->histogram_vector(BLUE,0,false);
    myImg->to_grayscale();
    SVector Vhistogram = myImg->histogram_vector(GRAY,0,true);
    IplImage* img =  cvCreateImage(cvSize(width,height),IPL_DEPTH_8U ,3);

    Rhistogram.normalized((double)N);
    Ghistogram.normalized((double)N);
    Bhistogram.normalized((double)N);
    Vhistogram.normalized((double)N);

    max = Rhistogram.get_max();
    cout << " \nRMax:" << max;
    max = Ghistogram.get_max();
    cout << " \nGMax:" << max;
    max = Bhistogram.get_max();
    cout << " \nBMax:" << max;
    cout << endl << "   RHist[0] = " << Rhistogram.datos[0] << "    RHist[255] = " << Rhistogram.datos[255];
    cout << endl << "   GHist[0] = " << Ghistogram.datos[0] << "    GHist[255] = " << Ghistogram.datos[255];
    cout << endl << "   BHist[0] = " << Bhistogram.datos[0] << "    BHist[255] = " << Bhistogram.datos[255];
    max = Vhistogram.get_max();
    //cout << " \nVMax:" << max;
    max = Util::max( Util::max(Rhistogram.get_max(), Ghistogram.get_max()), Util::max(Bhistogram.get_max(),Vhistogram.get_max())); //Util::max(Bhistogram.get_max(),Vhistogram.get_max()) );

    //max = max/2;
    cout << " \nMax:" << max;
    cout << " ";
    for(register int i=0; i< height; i++)
        for(register int j=0; j< width; j++)
        {
            RMatrix[i][j] = 0;
            GMatrix[i][j] = 0;
            BMatrix[i][j] = 0;
            VMatrix[i][j] = 0;
        }

    for(int j=0; j<width ; j++)
    {
        pix = (int)((double)(height-1)*( (double)(Rhistogram.datos[j]) / (max) ));
        pix = height - pix - 1;
        RMatrix[pix][j]=255;
        pix = (int)((double)(height-1)*( (double)(Ghistogram.datos[j]) / (max) ));
        pix = height - pix - 1;
        GMatrix[pix][j]=200;
        pix = (int)((double)(height-1)*( (double)(Bhistogram.datos[j]) / (max) ));
        pix = height - pix - 1;
        BMatrix[pix][j]=240;
        pix = (int)((double)(height-1)*( (double)(Vhistogram.datos[j]) / (max) ));
        pix = height - pix - 1;
        VMatrix[pix][j]=240;

    }

    for(int i=0;i<height; i++)
    {
        uchar* ptr = (uchar*) ( img->imageData + i * img->widthStep );

        for(int j=0; j<width; j++)
        {
            //index = i*3*width +3*j;
            index = 3*j;
            ptr[index+0] = 250;
            ptr[index+1] = 250;
            ptr[index+2] = 250;

            if(RMatrix[i][j] > 0 || GMatrix[i][j] > 0 || BMatrix[i][j] > 0)
            {
                ptr[index+2] = 0;
                ptr[index+1] = 0;
                ptr[index+0] = 0;
            }
            if(RMatrix[i][j] > 0)
                ptr[index+2] = RMatrix[i][j];
            if(GMatrix[i][j] > 0){
                //ptr[index+2] = GMatrix[i][j];
                ptr[index+1] = GMatrix[i][j];
                //ptr[index+0] = GMatrix[i][j];
            }
            if(BMatrix[i][j] > 0)
                ptr[index+0] = BMatrix[i][j];

            if(VMatrix[i][j] > 0){
                ptr[index+2] = 0;
                ptr[index+1] = 0;
                ptr[index+0] = 0;
            }

        }
    }
    return img;
}
