#include "image.h"
using namespace elements;

Image::Image() {
    this->rows = 0.0;
    this->cols = 0.0;
    this->Xn = 0.0;
    this->Yn = 0.0;
    this->Zn = 0.0;
    this->fx = 0.0;
    this->fy = 0.0;
    this->fz = 0.0;

}


Image::~Image() {
}


//Image::Image(int rows, int cols)
//{
//  Xn=0; Yn=0; Zn=0; fx=0;fy=0; fz=0;
//        this->Type = 0;
//  //this->SetDims(rows,cols);
//  /*this->cX = NULL;
//  this->cY = NULL;*/
//
//  //this->create_custom_image(0.0,0.0,0.0);
//}
void Image::initialize(int rows, int cols, int type)
{
        /*
         type:
         * 1: For Video
         * 0: For Normal rgb Vector with one channel
         * -1: For Normal rgb Vector with three channels
            */
        this->rows = rows;
        this->cols = cols;
        this->Type = type;
        this->rgb.set_size(rows, cols,type);

}


double Image::f(double t)
{
    //double alpha = (6.0)/(29.0);

    if(t>0.008856)//pow(alpha,3))
        return pow(t,1.0/3.0);
    else
        return 7.787*t + 16.0/116.0;//(t/(3.0*pow(alpha,2))) + 16.0/116.0;


}


void Image::rgb_to_xyz(Image *XYZ)
{
    //Image XYZ;
    //XYZ = Image(rgb.rows,rgb.cols);
    double valueR =0.0, valueG = 0.0, valueB = 0.0;
    register int index = 0;
    for(register int i=0; i< this->rows ; i++)
    {
        this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + (i)*this->rgb.widthstep );
        //XYZ->rgb.ptr_double_data = (double*)(XYZ->rgb.ptr_orig + (i)*XYZ->rgb.widthstep );
        for(register int j=0; j<this->cols; j++)
        {
            index = 3*j;
            valueR = this->rgb.ptr_data[index + 2]/255.0;
            valueG = this->rgb.ptr_data[index + 1]/255.0;
            valueB = this->rgb.ptr_data[index + 0]/255.0;
            index = i*3*this->cols + 3*j;
            XYZ->rgb.ptr_double_data[index + 2]= (0.6070*valueR)+(0.1734*valueG)+(0.2000*valueB);
            XYZ->rgb.ptr_double_data[index + 1]= (0.2990*valueR)+(0.5864*valueG)+(0.1146*valueB);
            XYZ->rgb.ptr_double_data[index + 0]= (0.0000*valueR)+(0.0661*valueG)+(1.1175*valueB);
            //index = index + 3;
        }
     }

}
void Image::xyz_to_rgb(Image *XYZ)
{
    //Image rgb;
    //register int i,j;
    //rgb = Image(this->rows,this->cols);
    double valueR=0.0,valueB=0.0, valueG=0.0;
    register int index=0;
    int N3 = XYZ->rows*XYZ->cols*3;
    while(index < N3)
    {
        valueR =(1.9100  *XYZ->rgb.ptr_double_data[index+2])+(-0.53230*XYZ->rgb.ptr_double_data[index+1])+(-0.28720 *XYZ->rgb.ptr_double_data[index+0]);
        valueG =(-0.98510*XYZ->rgb.ptr_double_data[index+2])+(2.0000  *XYZ->rgb.ptr_double_data[index+1])+(-0.028780*XYZ->rgb.ptr_double_data[index+0]);
        valueB =(0.05827 *XYZ->rgb.ptr_double_data[index+2])+(-0.11830*XYZ->rgb.ptr_double_data[index+1])+(0.89660  *XYZ->rgb.ptr_double_data[index+0]);
        //this->rgb.ptr_data[3*j+2]=(uchar)(valueR*255.0);
        //this->rgb.ptr_data[3*j+1]=(uchar)(valueG*255.0);
        //this->rgb.ptr_data[3*j+0]=(uchar)(valueB*255.0);
        this->rgb.ptr_double_data[index  +0] = valueB*255.0;
        this->rgb.ptr_double_data[index  +1] = valueG*255.0;
        this->rgb.ptr_double_data[index  +2] = valueR*255.0;
        index = index + 3;
    }

}
void Image::rgb_to_hsi(Image* HSI)
{
    /*
     *  H Index 2
     *  S Index 1
     *  I Index 0
     * */
    double valueR =0.0, valueG = 0.0, valueB = 0.0, alfa=0,denom=0;
    register int index = 0;
    for(register int i=0; i< this->rows ; i++)
    {
        this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + (i)*this->rgb.widthstep );
        //XYZ->rgb.ptr_double_data = (double*)(XYZ->rgb.ptr_orig + (i)*XYZ->rgb.widthstep );
        for(register int j=0; j<this->cols; j++)
        {
            index = 3*j;
            valueR = this->rgb.ptr_data[index + 2]/255.0;
            valueG = this->rgb.ptr_data[index + 1]/255.0;
            valueB = this->rgb.ptr_data[index + 0]/255.0;
            index = i*3*this->cols + 3*j;
             if(valueR==0 && valueG==0 && valueB==0)
            {
                 HSI->rgb.ptr_double_data[index + 0] = 0.0; //(0.333333)*(valueR+valueG+valueB);  // I
                 HSI->rgb.ptr_double_data[index + 1] = 1.0; //1.0 - (min( min(valueR,valueG), valueB )); // S
                 HSI->rgb.ptr_double_data[index + 2] = 0.0;
            }else if(valueR==valueG && valueR==valueB)
            {
                HSI->rgb.ptr_double_data[index + 0] = (0.333333)*(valueR+valueG+valueB);  // I
                HSI->rgb.ptr_double_data[index + 1] = 1.0 - (min( min(valueR,valueG), valueB )); // S
                HSI->rgb.ptr_double_data[index + 2] = 0.0; // H
            }
            else
            {
                denom = sqrt(valueR*valueR + valueG*valueG + valueB*valueB - valueR*valueG - valueG*valueB - valueR*valueB);
                alfa = acos((valueR-0.5*valueG-0.5*valueB)/(denom));
                alfa = alfa* 180.0/3.141592;
                HSI->rgb.ptr_double_data[index + 0] = (0.333333)*(valueR+valueG+valueB);  // I
                HSI->rgb.ptr_double_data[index + 1] = 1.0 - (min( min(valueR,valueG), valueB )); // S
                if(valueB>valueG)
                    HSI->rgb.ptr_double_data[index + 2] = (360.0 - alfa)/(360.0);  //H
                else
                    HSI->rgb.ptr_double_data[index + 2] = (alfa)/(360.0);  // H

            }


            cout << endl << HSI->rgb.ptr_double_data[index + 2] << " "<< HSI->rgb.ptr_double_data[index + 1] << " " << HSI->rgb.ptr_double_data[index + 0];
        }
    }

}
void Image::xyz_to_lab(Image *XYZ, Image *LAB)
{
  //Image LAB;
  //LAB = Image(this->rows,this->cols);
  //int cols =  this->cols;
  //register int i,j;
  register int index=0;
  int N3 = XYZ->cols*XYZ->rows*3;
  LAB->Xn = 1;
  LAB->Yn = 0.98072;
  LAB->Zn = 1.18225;

  while(index < N3)
  {
      LAB->rgb.ptr_double_data[index+2] = 116.0*f(XYZ->rgb.ptr_double_data[index+1]/LAB->Yn)-16;
      //else
      //    LAB.R[i*cols+j] = 903.3 * f(this->G[i*cols+j]/this->Yn);
      //.........................0..................para a
      LAB->rgb.ptr_double_data[index+1] = 500.0*(f(XYZ->rgb.ptr_double_data[index+2]/LAB->Xn)-f(XYZ->rgb.ptr_double_data[index+1]/LAB->Yn));
      //...........................................para b
      LAB->rgb.ptr_double_data[index+0] = 200.0*(f(XYZ->rgb.ptr_double_data[index+1]/LAB->Yn)-f(XYZ->rgb.ptr_double_data[index+0]/LAB->Zn));

      index = index + 3;
  }

}

void Image::lab_to_xyz(Image *LAB, Image *XYZ)
{
    //Image XYZ(LAB.rows, LAB.cols);
    double alpha = (6.0/29.0);
    register int index=0;
        int N3 = LAB->cols*LAB->rows*3;
    XYZ->Xn = 1;
    XYZ->Yn = 0.98072;
    XYZ->Zn = 1.18225;
        while(index < N3)
        {
            XYZ->fy = (LAB->rgb.ptr_double_data[index+2]+16.0)/116.0;
            XYZ->fx = XYZ->fy + (LAB->rgb.ptr_double_data[index+1]/500.0);
            XYZ->fz = XYZ->fy - (LAB->rgb.ptr_double_data[index+0]/200.0);

            if(XYZ->fy>alpha)
                    XYZ->rgb.ptr_double_data[index+1] = XYZ->Yn * pow(XYZ->fy,3.0);
            else
                    XYZ->rgb.ptr_double_data[index+1] = (XYZ->fy - 16.0/116.0) * (3.0*pow(alpha,2.0)) * (XYZ->Yn);

            if(XYZ->fx>alpha)
                    XYZ->rgb.ptr_double_data[index+2] = XYZ->Xn*pow(XYZ->fx,3.0);
            else
                    XYZ->rgb.ptr_double_data[index+2] = (XYZ->fx - 16.0/116.0)*(3.0*pow(alpha,2.0))*(XYZ->Xn);

            if(XYZ->fz>alpha)
                    XYZ->rgb.ptr_double_data[index+0] = XYZ->Zn*pow(XYZ->fz,3.0);
            else
                    XYZ->rgb.ptr_double_data[index+0] = (XYZ->fz - 16.0/116.0)*(3.0*pow(alpha,2.0))*(XYZ->Zn);
            index = index + 3;
        }
}

void Image::lab_to_lch(Image* LAB, Image* lch)
{
    register int index=0;
    int N3 = LAB->cols*LAB->rows*3;
    double L=0.0, A=0.0, B=0.0;
    while(index < N3)
    {
        L = LAB->rgb.ptr_double_data[index + 2];
        A = LAB->rgb.ptr_double_data[index + 1];
        B = LAB->rgb.ptr_double_data[index + 0];
        lch->rgb.ptr_double_data[index + 2] = L;
        lch->rgb.ptr_double_data[index + 1] = sqrt(A*A + B*B);


        if(A<0.00001 && A>-0.00001){
            if(B > 0)
                lch->rgb.ptr_double_data[index + 0] = 90;
            else
                lch->rgb.ptr_double_data[index + 0] = 270;
        }
        else{
            if(A < 0){
                lch->rgb.ptr_double_data[index + 0] = 180.0  + atan(B/A) * 180/3.14159265;
            }else if(A > 0){
                if(B > 0)
                    lch->rgb.ptr_double_data[index + 0] = atan(B/A) * 180/3.14159265;
                else
                    lch->rgb.ptr_double_data[index + 0] = 360.0 - atan(abs(B)/abs(A)) * 180/3.14159265;
            }

        }
        //cout << endl << lch->rgb.ptr_double_data[index + 0];
        index = index + 3;
    }
}
void Image::xyz_to_lms(Image* XYZ, Image* LMS)
{
    //double L=0,M=0,S=0;
    register int index=0;
    int N3 = LMS->rows*LMS->cols*3;

    //LMS->rgb.ptr_double_data[index + 2] = 0.8951*XYZ->rgb.ptr_double_data[index+2] + 0.2664*XYZ->rgb.ptr_double_data[index+1] - 0.1614*XYZ->rgb.ptr_double_data[index+0];
    //LMS->rgb.ptr_double_data[index + 1] = -0.7502*XYZ->rgb.ptr_double_data[index+2] + 1.7135*XYZ->rgb.ptr_double_data[index+1] + 0.0367*XYZ->rgb.ptr_double_data[index+0];
    //LMS->rgb.ptr_double_data[index + 0] = 0.0389*XYZ->rgb.ptr_double_data[index+2] - 0.0685*XYZ->rgb.ptr_double_data[index+1] + 1.0296*XYZ->rgb.ptr_double_data[index+0];

    while(index < (N3))
    {
        LMS->rgb.ptr_double_data[index + 2] =  0.4002*XYZ->rgb.ptr_double_data[index+2] + 0.7076*XYZ->rgb.ptr_double_data[index+1] - 0.0808*XYZ->rgb.ptr_double_data[index+0];
        LMS->rgb.ptr_double_data[index + 1] = -0.2263*XYZ->rgb.ptr_double_data[index+2] + 1.1653*XYZ->rgb.ptr_double_data[index+1] + 0.0457*XYZ->rgb.ptr_double_data[index+0];
        LMS->rgb.ptr_double_data[index + 0] =  0.9182*XYZ->rgb.ptr_double_data[index+0];
        index = index + 3;
    }


}

void Image::lms_to_xyz(Image* LMS, Image* XYZ)
{
    //double L=0,M=0,S=0;
    register int index=0;
    int N3 = LMS->rows*LMS->cols*3;
    while(index < (N3))
    {
        XYZ->rgb.ptr_double_data[index+2] =  1.8600*LMS->rgb.ptr_double_data[index+2] - 1.1295*LMS->rgb.ptr_double_data[index+1] + 0.2199*LMS->rgb.ptr_double_data[index+0];
        XYZ->rgb.ptr_double_data[index+1] =  0.3612*LMS->rgb.ptr_double_data[index+2] + 0.6388*LMS->rgb.ptr_double_data[index+1];
        XYZ->rgb.ptr_double_data[index+0] =  1.0890*LMS->rgb.ptr_double_data[index+0];
        index = index + 3;
    }

}

int Image::get_rows()
{
    return rows;
}
int Image::get_cols()
{
    return cols;
}
//bool Image::Save(const char *fileName)
//{
//        ofstream FILE;
//        int nPoints = 0,indexImag=0;
//  char cad[16];
//
//        FILE.open(fileName, ios::out);
//  if ( !FILE )
//        {
//            //cout << "\n\n FILE couldnt";
//            return false;
//  }
//  FILE << "Red Component" << endl;
//  //FILE << this->x << this->y << this->z;
//  do
//  {
//      for(register int i=0; i<this->get_rows(); i++)
//      {for(register int j=0; j<this->get_cols(); j++)
//          {
//              this->R.get(i*cols+j);
//              if(indexImag==0){
//                  sprintf(cad, "%.3f ",this->R[i*cols+j]);
//                  FILE << cad;
//              }
//              if(indexImag==1)
//              {
//                  sprintf(cad, "%.3f ",this->G[i*cols+j]);
//                  FILE << cad;
//              }
//              if(indexImag == 2){
//                  sprintf(cad, "%.3f ",this->B[i*cols+j]);
//                  FILE << cad;
//              }
//              if(indexImag == 3){
//                  sprintf(cad, "%.3f ",this->colorIndex[i*cols+j]);
//                  FILE << cad;
//              }
//          }
//          FILE << endl;
//      }
//      indexImag++;
//      if(indexImag==1)
//                  FILE << "Green Component" << endl;
//      if(indexImag == 2)
//                  FILE << "Blue Component"  << endl;
//      if(indexImag == 3)
//                  FILE << "Blue Component"  << endl;
//      if(indexImag == 3)
//                  FILE << "Class"  << endl;
//  }while(indexImag<4);
//
//  FILE.close();
//
//  return true;
//
//
//}

//void Image::create_custom_image(double r, double g, double b)
//{
//  for(register int i=0; i<this->rows; i++)
//      for(register int j=0; j<this->cols; j++)
//      {
//          this->R[i*cols+j] = r;
//          this->G[i*cols+j] = g;
//          this->B[i*cols+j] = b;
//      }
//
//
//}


//double Image::AverageValue(int component)
//{
//  double sum=0.0, avg=0.0, min=0.0, avgUp =0.0;
//  for(int i=0; i<this->rows;i++)
//      for(int j=0; j<this->cols;j++)
//      {
//          switch(component)
//          {
//          case 0:
//              sum += this->R[i*cols+j];
//              break;
//          case 1:
//              sum += this->G[i*cols+j];
//              break;
//
//          case 2:
//              sum += this->B[i*cols+j];
//              break;
//
//          default:
//
//              //cout <<endl<<endl<< "Component not allowed";
//              break;
//          }
//      }
//      avg = sum/(rows*cols);
//      min = 9999999;
//      for(int i=0; i<11; i++)
//      {
//          if( sqrt((i*10 - avg)*(i*10 - avg)) < min)
//          {
//              min = sqrt((i*10 - avg)*(i*10 - avg));
//              avgUp= i*10;
//          }
//      }
//      return avgUp;
//}


void Image::transformation_tp()
{
    //Only for ptr_double_data
    register int i,j, index=0;
    double valueR, valueG, valueB;
    for(i=0; i<this->rows; i++)
    {
        //this->rgb.ptr_double_data = (double*)(this->rgb.ptr_orig + (i)*this->rgb.widthstep );
        for(j=0; j<this->cols; j++)
        {
            index = 3*i*this->cols + 3*j;
            valueR=this->rgb.ptr_double_data[index+2];
            valueG=this->rgb.ptr_double_data[index+1];
            valueB=this->rgb.ptr_double_data[index+0];
            this->rgb.ptr_double_data[index+2] = 0.612*valueR+valueG+valueB;
            this->rgb.ptr_double_data[index+1] = 0.369*valueR-valueG;
            this->rgb.ptr_double_data[index+0] = 0.019*valueR-valueB;

        }

    }
}

void Image::transformation_tp_inverse()
{
    //Only for ptr_double_data
    register int i,j, index=0;
    double valueR, valueG, valueB;
    for(i=0; i<this->rows; i++)
    {
        //this->rgb.ptr_double_data = (double*)(this->rgb.ptr_orig + (i)*this->rgb.widthstep );
        for(j=0; j<this->cols; j++)
        {
            index = 3*i*this->cols + 3*j;
            valueR=this->rgb.ptr_double_data[index+2];
            valueG=this->rgb.ptr_double_data[index+1];
            valueB=this->rgb.ptr_double_data[index+0];
            this->rgb.ptr_double_data[index+2] = valueR+valueG+valueB;
            this->rgb.ptr_double_data[index+1] = 0.369 * valueR - 0.631 * valueG + 0.369 * valueB;
            this->rgb.ptr_double_data[index+0] = 0.019 * valueR + 0.019 * valueG - 0.981 * valueB;

        }

    }
}
void Image::to_log()
{
    register int i=0;
    int N3 = this->rows*this->cols*3;

    while(i<N3)
    {
        this->rgb.ptr_double_data[i+2] = log(this->rgb.ptr_double_data[i+2]);
        this->rgb.ptr_double_data[i+1] = log(this->rgb.ptr_double_data[i+1]);
        this->rgb.ptr_double_data[i+0] = log(this->rgb.ptr_double_data[i+0]);
        i=i+3;
    }

}

void Image::to_exp()
{
    register int i=0;
    int N3 = this->rows*this->cols*3;

    while(i<N3)
    {
        this->rgb.ptr_double_data[i+2] = exp(this->rgb.ptr_double_data[i+2]);
        this->rgb.ptr_double_data[i+1] = exp(this->rgb.ptr_double_data[i+1]);
        this->rgb.ptr_double_data[i+0] = exp(this->rgb.ptr_double_data[i+0]);
        i=i+3;
    }
}

SVector Image::histogram_vector(Component comp, int jump, bool is_double)
{
    SVector Hist = SVector(256);
    double value=0.0;
    int channel_index = 0;
    jump++;
    switch(comp)
    {
        case GRAY:
        {
            channel_index = 1;
        }break;
        case RED:
        {
            channel_index = 2;
        }break;
        case GREEN:
        {
            channel_index = 1;
        }break;
        case BLUE:
        {
            channel_index = 0;
        }break;
    }
    for(register int i=0; i<this->rows; i = i + jump)
    {
        if(is_double == false)
        {
            this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
        }
        for(register int j=0; j<this->cols; j = j + jump)
        {
            if(is_double)
            {
                value = this->rgb.ptr_double_data[i*3*this->cols + 3*j + channel_index];
            }else
            {
                value = this->rgb.ptr_data[3*j + channel_index];
            }
            Hist.datos[(int)value]++;
            //Hist[(int)value]++;
        }

    }
    return Hist;
}



void Image::white_patch_retinex()
{

    double maxR =0.0, maxG = 0.0, maxB = 0.0, value=0.0;

    maxR = this->rgb.get_max(RED, false);
    maxG = this->rgb.get_max(GREEN, false);
    maxB = this->rgb.get_max(BLUE, false);
    ////cout <<endl<< "maxR = "<<maxR<<"maxG = "<< maxG << "maxB" << maxB;
    ////cout << "*******************************";

    for(int i=0; i<this->rows; i++)
    {
        this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
        for(int j=0; j<this->cols; j++)
        {
            value = (double)this->rgb.ptr_data[3*j+0]/maxB * 255.0;
            if(value>255)
            {
                value = 255;
            }
            this->rgb.ptr_data[3*j+0] = value;
            value = (double)this->rgb.ptr_data[3*j+1]/maxG * 255.0;
            if(value>255)
            {
                value = 255;
            }
            this->rgb.ptr_data[3*j+1] = value;
            value = (double)this->rgb.ptr_data[3*j+2]/maxR * 255.0;
            if(value>255)
            {
                value = 255;
            }
            this->rgb.ptr_data[3*j+2] = value;

        }

    }
    // //cout << "*******************************";
    //return void;
}

void Image::white_patch_retinex_reloaded(double p, int jump, bool doWPR)
{
    //Image newImg  = Image(this->rows,this->cols);
    // p is for porcentage
    double pn = 0.0;
    double value = 0.0;
    SVector HistR, HistG, HistB;
    //int jiR=0, jiG=0, jiB=0;
    bool jiR_Finded = false, jiG_Finded = false, jiB_Finded = false;

    if(doWPR)
    {
        this->Xn = 0.0;
        this->Yn = 0.0;
        this->Zn = 0.0;
        HistR = this->histogram_vector(RED,jump,false);
        HistG = this->histogram_vector(GREEN,jump,false);
        HistB = this->histogram_vector(BLUE,jump,false);
        pn = p*(double)((this->rows * this->cols)/(jump+1));

        for(int ji=0; ji<255; ji++)
        {
            if(!jiR_Finded);
            {   if(pn< HistR.sum(ji,256) && pn> HistR.sum(ji+1,256))
                {
                    this->Xn = (double)ji;//jiR = ji;
                    jiR_Finded = true;
                }
            }

            if(!jiG_Finded)
            {
                if(pn<HistG.sum(ji,256) && pn>HistG.sum(ji+1,256))
                {
                    this->Yn = (double)ji;//jiG = ji;
                    jiG_Finded=true;
                }
            }

            if(!jiB_Finded)
            {
                if(pn<HistB.sum(ji,256)&& pn>HistB.sum(ji+1,256))
                {
                    this->Zn = (double)ji;//jiB = ji;
                    jiG_Finded=true;
                }
            }


        }

        if(this->Xn < 0.00001 && this->Xn > -0.00001)
            this->Xn = this->rgb.get_max(RED,false);
        if(this->Yn < 0.00001 && this->Yn > -0.00001)
            this->Yn = this->rgb.get_max(GREEN, false);
        if(this->Zn < 0.00001 && this->Zn > -0.00001)
            this->Zn = this->rgb.get_max(BLUE, false);
    }

    ////cout <<endl<< "jiR = "<< this->Xn << " jiG= "<< this->Yn << " jiB= " << this->Zn;


    ////cout << "*******************************";
    for(int i=0; i<this->rows; i++)
    {
        this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
        for(int j=0; j<this->cols; j++)
        {

            value = (double)this->rgb.ptr_data[3*j+0]/this->Zn * 255.0;
            if(value>255)
            {
                value = 255;
            }
            this->rgb.ptr_data[3*j+0] = value;

            value = (double)this->rgb.ptr_data[3*j+1]/this->Yn * 255.0;
            if(value>255)
            {
                value = 255;
            }
            this->rgb.ptr_data[3*j+1] = value;

            value = (double)this->rgb.ptr_data[3*j+2]/this->Xn * 255.0;
            if(value>255)
            {
                value = 255;
            }
            this->rgb.ptr_data[3*j+2] = value;

        }

    }

}
void Image::white_patch_retinex_v()
{
//    //Image newImg  = Image(this->rows,this->cols);
//    double a = 0.0, value = 0.0;
//    double maxR = 0.0, maxG = 0.0, maxB = 0.0;
//    maxR = this->rgb.get_mean(RED,0, false);
//    maxG = this->rgb.get_mean(GREEN,0, false);
//    maxB = this->rgb.get_mean(BLUE,0, false);
//    ////cout <<endl<< "jiR = "<< this->Xn << " jiG= "<< this->Yn << " jiB= " << this->Zn;
//    ////cout << "*******************************";
//    for(int i=0; i<this->rows; i++)
//    {
//        this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
//        for(int j=0; j<this->cols; j++)
//        {
//
//            value = (double)this->rgb.ptr_data[3*j+0]/maxB * 255.0;
//            if(value>255)
//            {
//                value = 255;
//            }
//            this->rgb.ptr_data[3*j+0] = value;
//
//            value = (double)this->rgb.ptr_data[3*j+1]/maxG * 255.0;
//            if(value>255)
//            {
//                value = 255;
//            }
//            this->rgb.ptr_data[3*j+1] = value;
//
//            value = (double)this->rgb.ptr_data[3*j+2]/maxR * 255.0;
//            if(value>255)
//            {
//                value = 255;
//            }
//            this->rgb.ptr_data[3*j+2] = value;
//
//        }
//
//    }

}
// Optimized GreyWorld
void Image::gray_world(int jump)
{
    //Image newImg  = Image(this->rows,this->cols);
    double a = 0.0, value = 0.0;
    double meanR = 0.0, meanG = 0.0, meanB = 0.0, meanGrey = 0.0;
    meanR = this->rgb.get_mean(RED,0, false);
    meanG = this->rgb.get_mean(GREEN,0, false);
    meanB = this->rgb.get_mean(BLUE,0, false);
    meanGrey = (meanR+meanG+meanB)/3.0;
    a = 2.0*meanGrey;
    ////cout <<endl<< "jiR = "<< this->Xn << " jiG= "<< this->Yn << " jiB= " << this->Zn;
    ////cout << "*******************************";
    for(int i=0; i<this->rows; i++)
    {
        this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
        for(int j=0; j<this->cols; j++)
        {

            value = (double)this->rgb.ptr_data[3*j+0]/a * 255.0;
            if(value>255)
            {
                value = 255;
            }
            this->rgb.ptr_data[3*j+0] = value;

            value = (double)this->rgb.ptr_data[3*j+1]/a * 255.0;
            if(value>255)
            {
                value = 255;
            }
            this->rgb.ptr_data[3*j+1] = value;

            value = (double)this->rgb.ptr_data[3*j+2]/a * 255.0;
            if(value>255)
            {
                value = 255;
            }
            this->rgb.ptr_data[3*j+2] = value;

        }

    }

}

double* Image::variant_of_horn(int assumption, bool is_available, double *last_vec)
{
    /*
     * assumption variable:
     *  0 is for White Patch Retinex Assumption
     *  1 is for Grey World Assumption
     *
     */
    double *vec = new double[3];
    double logValueR=0.0, logValueG=0.0, logValueB=0.0;
    register int i=0, j=0;
    int index = 0;

    if(is_available == true)
    {
        if(assumption == 0)   // WHite assumption
        {
            vec[0] = -9999; vec[1] = -9999;  vec[2] = -9999;
            i=0; j=0;
            while(i<this->rows)
            {
                this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
                while(j<this->cols)
                {
                    logValueR = (log((double)this->rgb.ptr_data[3*j+2]));
                    logValueG = (log((double)this->rgb.ptr_data[3*j+1]));
                    logValueB = (log((double)this->rgb.ptr_data[3*j+0]));
                    if( logValueR > vec[0])
                        vec[0] = logValueR;
                    if(logValueG > vec[1])
                        vec[1] = logValueG;
                    if(logValueB > vec[2])
                        vec[2] = logValueB;
                    j++;
                }
                j=0;
                i++;
            }

        }else if(assumption == 1)
        {
            vec[0] = 0.0; vec[1] = 0.0;  vec[2] = 0.0;
            i=0; j=0;
            while(i<this->rows)
            {
                this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
                while(j<this->cols)
                {
                    logValueR = (log((double)this->rgb.ptr_data[3*j+2]));
                    logValueG = (log((double)this->rgb.ptr_data[3*j+1]));
                    logValueB = (log((double)this->rgb.ptr_data[3*j+0]));

                    if( logValueR > 0.0)
                        vec[0] += logValueR;
                    if(logValueG >  0.0)
                        vec[1] += logValueG;
                    if(logValueB >  0.0)
                        vec[2] += logValueB;
                    j++;
                }
                j=0;
                i++;
            }

           int n = this->rows*this->cols;
           vec[0] /= n;
           vec[1] /= n;
           vec[2] /= n;
        }
    }else    // Getting the last value of vector;
    {
        vec[0] = 0.0; vec[1] = 0.0;  vec[2] = 0.0;
        vec[0] = *last_vec;
        *last_vec++;
        vec[1] = *last_vec;
        *last_vec++;
        vec[2] = *last_vec;

    }
    //cout << endl << "\t v = ( " << vec[0] << ", "<<vec[1] << ", "<<vec[2] << " )";
    i=0; j=0;
    while(i<this->rows)
    {
        this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
        while(j<this->cols)
        {
            index = i*3*this->cols + 3*j;
            logValueR = (log((double)this->rgb.ptr_data[3*j+2]));
            logValueG = (log((double)this->rgb.ptr_data[3*j+1]));
            logValueB = (log((double)this->rgb.ptr_data[3*j+0]));

            if(assumption == 0)
            {
                this->rgb.ptr_double_data[index + 2] = exp(logValueR-vec[0]);
                this->rgb.ptr_double_data[index + 1] = exp(logValueG-vec[1]);
                this->rgb.ptr_double_data[index + 0] = exp(logValueB-vec[2]);
            }else if(assumption == 1)
            {
                this->rgb.ptr_double_data[index + 2] = exp(logValueR-vec[0] - 1) ;
                this->rgb.ptr_double_data[index + 1] = exp(logValueG-vec[1]  - 1) ;
                this->rgb.ptr_double_data[index + 0] = exp(logValueB-vec[2]  - 1) ;
            }
            j++;
        }
        j=0;
        i++;
    }


    return vec;
}


void Image::to_grayscale()
{
    double valueR=0.0, valueG=0.0, valueB=0.0;
    double value=0;
    register int index =0;
    for(int i=0; i<this->rows; i++)
    {
        this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
        for(int j=0; j<this->cols; j++)
        {
            valueB = (double)this->rgb.ptr_data[3*j + 0];
            valueG = (double)this->rgb.ptr_data[3*j + 1];
            valueR = (double)this->rgb.ptr_data[3*j + 2];
            value = 0.3*valueR + 0.59*valueG + 0.11*valueB;
            //this->rgb.ptr_data[3*j+2] = 0;
            //this->rgb.ptr_data[3*j+1] = 255;
            //this->rgb.ptr_data[3*j+0] = 0;
            index = i*3*this->cols + 3*j;
            this->rgb.ptr_double_data[index + 2] =value;
            this->rgb.ptr_double_data[index + 1] = value;
            this->rgb.ptr_double_data[index + 0] = value;
        }
    }

}
void Image::to_mosaic()
{
    double valueR=0.0, valueG=0.0, valueB=0.0;
    //double valuesR[32], valuesG[32], valuesB[32];
    register int index =0;
    int channel_index=0;
    int times = 0, iFirstValue=0, iLastValue=0, jFirstValue=0, jLastValue=0, i_inc=0, j_inc=0;
    i_inc = this->rows/5;
    j_inc = this->cols/5;
    iLastValue = i_inc;
    jLastValue = j_inc;
    srand ( time(NULL) );
    do
    {

        switch(channel_index){
        case 0:
            valueB = rand()%255 + 1;
            break;
        case 1:
            valueG = rand()%255 + 1;
            break;
        case 2:
            valueR = rand()%255 + 1;
            break;
        }
        for(int i=iFirstValue; i<iLastValue; i++)
        {
            this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
            for(int j=jFirstValue; j<jLastValue; j++)
            {
                if(channel_index !=0)
                    valueB = (double)this->rgb.ptr_data[3*j + 0];
                if(channel_index !=1)
                    valueG = (double)this->rgb.ptr_data[3*j + 1];
                if(channel_index !=2)
                    valueR = (double)this->rgb.ptr_data[3*j + 2];

                index = i*3*this->cols + 3*j;
                this->rgb.ptr_double_data[index + 2] = valueR;
                this->rgb.ptr_double_data[index + 1] = valueG;
                this->rgb.ptr_double_data[index + 0] = valueB;
            }
        }
        if(channel_index==2)
            channel_index=0;
        else
            channel_index++;
        jFirstValue+= j_inc;
        jLastValue += j_inc;
        if(times%5 == 0 && times > 0)
        {
            iFirstValue += i_inc;
            iLastValue += i_inc;
            jFirstValue = 0;
            jLastValue = j_inc;
            srand ( time(NULL) );
        }
        times ++;
    }while(times<26);

}
void Image::to_color(int color)
{
    double valueR=0.0, valueG=0.0, valueB=0.0;
    double value=0;
    register int index =0;
    for(int i=0; i<this->rows; i++)
    {
        this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
        for(int j=0; j<this->cols; j++)
        {
            valueB = (double)this->rgb.ptr_data[3*j + 0];
            valueG = (double)this->rgb.ptr_data[3*j + 1];
            valueR = (double)this->rgb.ptr_data[3*j + 2];
            switch(color){
                case 0: // For Red
                {
                    valueB = 1.0;
                    valueG = 1.0;
                }break;
                case 1: // For Green
                {
                    valueB = 1.0;
                    valueR = 1.0;
                }break;
                case 2: // For Blue
                {
                    valueR = 1.0;
                    valueG = 1.0;
                }break;
                case 3: // For Yellow
                {
                    valueB = 1.0;
                }break;
                case 4: // Inverse
                {
                    valueR = 255.0 - valueR;
                    valueG = 255.0 - valueG;
                    valueB = 255.0 - valueB;
                }
            }
            index = i*3*this->cols + 3*j;
            this->rgb.ptr_double_data[index + 2] = valueR;
            this->rgb.ptr_double_data[index + 1] = valueG;
            this->rgb.ptr_double_data[index + 0] = valueB;
            valueG=valueR=valueB=0.0;
        }
    }

}
void Image::to_double_vector(int is_double, double *vector)
{
    int N3 = this->rows*this->cols*3;
    register int index = 0;
    double valueR=0.0, valueG=0.0, valueB=0.0;

    if(is_double)
    {
        while(index < N3)
        {
            valueB = (this->rgb.ptr_double_data[index + 0]);
            valueG = (this->rgb.ptr_double_data[index + 1]);
            valueR = (this->rgb.ptr_double_data[index + 2]);
            vector[index + 0] = valueB;
            vector[index + 1] = valueG;
            vector[index + 2] = valueR;
            index = index + 3;
        }
    }else
    {
        for(register int i=0; i<this->rows; i++)
        {
            this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
            for(register int j=0; j<this->cols; j++)
            {
                index = 3*j;
                valueB = (double)(this->rgb.ptr_data[index + 0]);
                valueG = (double)(this->rgb.ptr_data[index + 1]);
                valueR = (double)(this->rgb.ptr_data[index + 2]);
                index = 3*(i*this->cols + j);
                vector[index + 0] = valueB;
                vector[index + 1] = valueG;
                vector[index + 2] = valueR;
            }
        }
    }
}
void Image::double_vector_to_image(double* vector, int channel_index, int is_double)
{
    /*
     *  vector -> A data channel Vector
     *  channel_index > [2,1,0] -> [R,G,B]
     * */
    register int i=0, j=0;

    while(i<this->rows)
    {
        if(is_double == 1)
           this->rgb.ptr_double_data = (double*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
        else
           this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
        while(j<this->cols)
        {
            if(is_double == 1)
            {
                this->rgb.ptr_double_data[3*j + channel_index] = vector[i*this->cols + j];
            }
            else if(is_double == -1)
            {
                this->rgb.ptr_double_data[i*3*this->cols+ 3*j + channel_index] = vector[i*this->cols + j];
            }else if(is_double == 2)
            {
                this->rgb.ptr_double_data[i*3*this->cols+ 3*j + 0] = vector[i*this->cols + j];
                this->rgb.ptr_double_data[i*3*this->cols+ 3*j + 1] = vector[i*this->cols + j];
                this->rgb.ptr_double_data[i*3*this->cols+ 3*j + 2] = vector[i*this->cols + j];
            }
            else
            {
               this->rgb.ptr_data[3*j + channel_index] = (vector[i*this->cols + j]);
            }
            j++;
        }
        j=0;
        i++;
    }


}
//Image Image::gray_world()
//{
//    Image newImg = Image(this->rows, this->cols);
//    double meanR = 0.0, meanG = 0.0, meanB = 0.0, meanGray=0.0;
//    double maxCh[3], max;
//    meanR = this->R.mean()/255;
//    meanG = this->G.mean()/255;
//    meanB = this->B.mean()/255;
//    meanGray = (meanR+meanG+meanB)/3;
//
//
//    for(int i=0; i<this->rows*this->cols;i++)
//    {
//        newImg.R[i] = this->R.normalized(i,255)/(2*meanR);
//        newImg.G[i] = this->G.normalized(i,255)/(2*meanG);
//        newImg.B[i] = this->B.normalized(i,255)/(2*meanB);
//
//    }
//    maxCh[0] = newImg.R.get_max();
//    maxCh[1] = newImg.G.get_max();
//    maxCh[2] = newImg.B.get_max();
//
//    max = Util::max(maxCh,3);
//    for(int i=0; i<this->rows*this->cols;i++)
//    {
//        newImg.R[i]=newImg.R[i] * 255;
//        newImg.G[i]=newImg.G[i] * 255;
//        newImg.B[i]=newImg.B[i] * 255;
//        if(newImg.R[i]>255.0) newImg.R[i]=255;
//        if(newImg.G[i]>255.0) newImg.G[i]=255;
//        if(newImg.B[i]>255.0) newImg.B[i]=255;
//    }
//    return newImg;
//}

void Image::homomorphic_filter(int type,FFTWManager *fftw_f, Image *XYZ, Image *LAB, double *vector_image, double *filter)
{
    /* type:
     * 0: For Normal Homomorphic Filter
     * 1: For HSV Homomorphic Filter
     * */
    int rows = this->rows;
    int cols = this->cols;
    int N = rows*cols;
    double * tempVec = NULL;
    tempVec = new double[N];

    if(type==0)
    {
        int counter = 0;
        Util::create_filter_sigmoid(0.5,1.0,0.8,10.0,0.5,rows, cols, filter);    // Regular Homomorphic Filter
        do
        {
            for(int i=0; i<N ; i++){
                fftw_f->in[i][0]  = log(vector_image[3*i + counter]);
                fftw_f->in[i][1]  = 0;
            }
            fftw_f->FFT_ToCenter(rows, cols);
            fftw_f->Execute();
            fftw_f->FFT_Filter(filter,rows,cols);
            fftw_f->Execute_Inverse();
            fftw_f->FFT_ToCenter_Inverse(rows, cols);
            fftw_f->to_double_vector(2, tempVec);

            this->double_vector_to_image(tempVec,counter, -1);
            counter ++;
        }while(counter < 3);
        this->to_exp();
    }else
    {
        Util::create_filter_sigmoid(1.5,2,0.17,0,0,rows, cols, filter);    // with HVS
        //this->remove_zeros(false);
        this->rgb_to_xyz(XYZ);
        Image::lms_to_xyz(XYZ,LAB);
        //Image::xyz_to_lab(XYZ,LAB);
        LAB->to_log();
        LAB->transformation_tp();

        LAB->to_double_vector(1, vector_image);   // For HSV Algorithm
        int counter = 0;
        fftw_f->initialize(N);
        fftw_f->Plan_2D(rows,cols);
        do
        {
            for(int i=0; i<N ; i++)
            {
                fftw_f->in[i][0]  = vector_image[3*i + counter];
                fftw_f->in[i][1]  = 0;
            }

            fftw_f->FFT_ToCenter(rows, cols);
            fftw_f->Execute();
            fftw_f->FFT_Filter(filter,rows,cols);
            fftw_f->Execute_Inverse();
            fftw_f->FFT_ToCenter_Inverse(rows, cols);

            fftw_f->to_double_vector(2, tempVec);

            LAB->double_vector_to_image(tempVec,counter, -1);
            counter ++;
        }while(counter < 3);

        LAB->transformation_tp_inverse();
        LAB->to_exp();
        Image::lms_to_xyz(LAB,XYZ);
        //Image::lab_to_xyz(LAB,XYZ);
        this->xyz_to_rgb(XYZ);
    }
      if(tempVec!=NULL)
        delete[] tempVec;
}

void Image::grayscale_filter_by_fft(FFTWManager *fftw_f, FFTWManager *fftw_g, double *vector_image)
{
    int index=0, rows = this->rows, cols = this->cols, N = rows*cols;
    double * tempVec = new double[N];

    while(index<N)
    {
        fftw_f->in[index][0]  = vector_image[3*index];
        fftw_f->in[index][1]  = 0;
        index++;
    }
    index = 0;

    fftw_f->FFT_ToCenter(rows, cols);
    fftw_f->Execute();
    fftw_f->FFT_Filter(fftw_g,rows,cols);
    fftw_f->Execute_Inverse();
    fftw_f->FFT_ToCenter_Inverse(rows, cols);
    fftw_f->to_double_vector(2, tempVec);
    this->double_vector_to_image(tempVec,0,2);



    delete []tempVec;


}

void Image::local_average_color_by_fft(FFTWManager *fftw_f, FFTWManager *fftw_g, double *vector_image, Image* Z, bool is_available)
{
    int counter =0, index=0, rows = this->rows, cols = this->cols, N = rows*cols;

    if(is_available == true)
    {
        double * tempVec = new double[N];
        do
        {
            while(index<N)
            {
                fftw_f->in[index][0]  = vector_image[3*index + counter];
                fftw_f->in[index][1]  = 0;
                index++;
            }
            index = 0;

            fftw_f->FFT_ToCenter(rows, cols);
            fftw_f->Execute();
            fftw_f->FFT_Filter(fftw_g,rows,cols);
            fftw_f->Execute_Inverse();
            fftw_f->FFT_ToCenter_Inverse(rows, cols);
            fftw_f->to_double_vector(2, tempVec);
            Z->double_vector_to_image(tempVec,counter,-1);
            counter ++;

        }while(counter < 3);
        delete []tempVec;
    }

}

Image Image::convolution(Data *g)
{
    /*
     * Convolution between a f (type -1) Image with gray values and g mask (type 0)
     * */
    printf("SEM convolution() Begin\n");
    int max_m=0, max_n=0, jump_i=0, jump_j=0,  index_f=0;
    bool isJumpEnabled_j = false, isJumpEnabled_i = false;
    enum MODE{NONE, ASC, DSC, ALL};
    printf("SEM convolution() 0\n");
    MODE Mode_i, Mode_j;
    Mode_i = ASC;
    Mode_j = ASC;
    Image Output;
     if (g == NULL)
     {
        Output.initialize((this->rows), (this->cols),-1);
        printf("%s\n","Error! Data *g is NULL");
        return Output;
     }else
     {
        Output.initialize((this->rows+g->rows-1), (this->cols+g->cols-1),-1);
    }
    int OutputRows = Output.get_rows();
    int OutputCols = Output.get_cols();
    printf("SEM convolution() 1\n");
    for(int i=0; i<OutputRows; i++)
    {
        // Análisis para i, similar al de j
        // i==g->rows means that mask should be computed in all rows.
        if( i == g->rows)
        {
            Mode_i = ALL;
            isJumpEnabled_i = true;
        }else if(i == this->rows)
        {
            max_m--;
            Mode_i = DSC;
            isJumpEnabled_i = true;
        }
        else if(Mode_i == ASC)
        {
            max_m ++;
            Mode_i = ASC;
            isJumpEnabled_i = false;
        }

        if(isJumpEnabled_i == true)
        {
            jump_i++;
        }

        for(int j=0; j<OutputCols; j++)
        {
            double value = 0.0;
            if( j == g->cols)
            {
                    Mode_j = ALL;
                    isJumpEnabled_j = true;
            }else if(j == this->cols)
            {
                    max_n--;
                    Mode_j = DSC;
                    isJumpEnabled_j = true;
            }else if(Mode_j == ASC)
            {
                    max_n ++;
                    Mode_j = ASC;
                    isJumpEnabled_j = false;
            }

            if(isJumpEnabled_j == true)
            {
                    jump_j++;
            }
            for(int m=0; m< max_m ; m++)
            {
                this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + (m+jump_i)*this->rgb.widthstep );
                for(int n=0; n<max_n; n++)
                {
                    //index_f= (m+jump_i)*this->cols+(n+jump_j);   //Supposing that f as a vector representing a matrix
                    index_f = 3*(n+jump_j);
                    ////cout << endl << (double)this->rgb.ptr_data[index_f];
                    if(Mode_j == ASC || Mode_j == ALL)
                    {
                        // ALL/ALL es subconjunto de ASC/ASC
                        //index_g= (g->rows - max_m + m)*this->cols+(g->cols - max_n + n);  // Supposing that g is a vector Image
                        g->ptr_data = (uchar*)(g->ptr_orig + (g->rows - max_m + m)*g->widthstep + (g->cols - max_n + n)*sizeof(double));
                        value += (double)this->rgb.ptr_data[index_f]* (*((double*)g->ptr_data)); //g->ptr_data[index_g];
                        //Output.rgb.ptr_data[i*3*OutputCols+ 3*j + 1] = (double)Output.rgb.ptr_data[i*3*OutputCols+ 3*j + 1] + value; // 3*i*OutputCols+j+1     REVISAR

                    }else if(Mode_j == DSC)
                    {
                        g->ptr_data = (uchar*)(g->ptr_orig + m*g->widthstep + n*sizeof(double));
                        value += (double)this->rgb.ptr_data[index_f]* (*((double*)g->ptr_data));//*g->ptr_data[m*g->cols+n];
                        //Output.rgb.ptr_data[i*3*OutputCols+ 3*j + 1] = (double)Output.rgb.ptr_data[i*3*OutputCols+ 3*j + 1] + value;
                    }

                }
            }

//            //cout << " " << value;
           if(value>255) value = 255;
           Output.rgb.ptr_data[i*3*Output.cols+ 3*j + 0] = value;
           ////cout << " " <<(double)Output.rgb.ptr_double_data[i*3*Output.cols+ 3*j + 0];
           Output.rgb.ptr_data[i*3*Output.cols+ 3*j + 1] = value;
           Output.rgb.ptr_data[i*3*Output.cols+ 3*j + 2] = value;
        }
        printf("SEM convolution() 2\n");
        max_n = 0;
        jump_j = 0;
        Mode_j = ASC;
        ////cout << endl;

    }

    return Output;
}

void Image::convolution(Data *g, int channel, bool isCutted)
{
    /*
     * Convolution between a f (type -1) Image with gray values and g mask (type 0)
     * */
    int  max_m=0, max_n=0, jump_i=0, jump_j=0, index_f=0, index=0;
    bool isJumpEnabled_j = false, isJumpEnabled_i = false;
    enum MODE{NONE, ASC, DSC, ALL};
    double value = 0.0, value_F = 0.0;
    MODE Mode_i, Mode_j;
    Mode_i = ASC;
    Mode_j = ASC;
    //Image Output = Image((this->rows+g->rows-1), (this->cols+g->cols-1),-1);
    int OutputRows = this->rows;
    int OutputCols = this->cols;
    int borderRows = (g->rows-1)/2;
    int borderCols = (g->cols-1)/2;
    double max = -9999, min = 9999;
    if(isCutted == true)
    {
        OutputRows += g->rows-1;
        OutputCols += g->cols-1;
    }
    for(register int i=0; i<OutputRows; i++)
    {
        // Análisis para i, similar al de j
        // i==g->rows means that mask should be computed in all rows.
        if( i == g->rows)
        {
            Mode_i = ALL;
            isJumpEnabled_i = true;
        }else if(i == this->rows)
        {
            max_m--;
            Mode_i = DSC;
            isJumpEnabled_i = true;
        }
        else if(Mode_i == ASC)
        {
            max_m ++;
            Mode_i = ASC;
            isJumpEnabled_i = false;
        }

        if(isJumpEnabled_i == true)
        {
            jump_i++;
        }


        for(register int j=0; j<OutputCols; j++)
        {
            value = 0.0;
            if( j == g->cols)
            {
                    Mode_j = ALL;
                    isJumpEnabled_j = true;
            }else if(j == this->cols)
            {
                    max_n--;
                    Mode_j = DSC;
                    isJumpEnabled_j = true;
            }else if(Mode_j == ASC)
            {
                    max_n ++;
                    Mode_j = ASC;
                    isJumpEnabled_j = false;
            }

            if(isJumpEnabled_j == true)
            {
                    jump_j++;
            }
            for(int m=0; m< max_m ; m++)
            {
                this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + (m+jump_i)*this->rgb.widthstep );
                for(int n=0; n<max_n; n++)
                {
                    //index_f= (m+jump_i)*this->cols+(n+jump_j);   //Supposing that f as a vector representing a matrix
                    index_f = 3*(n+jump_j) + channel;
                    value_F = (double)this->rgb.ptr_data[index_f];
                    //value_F /= 255.0;
                    ////cout << endl << (double)this->rgb.ptr_data[index_f];
                    if(Mode_j == ASC || Mode_j == ALL)
                    {
                        g->ptr_data = (uchar*)(g->ptr_orig + (g->rows - max_m + m)*g->widthstep + (g->cols - max_n + n)*sizeof(double));
                        value += value_F * (*((double*)g->ptr_data)); //g->ptr_data[index_g];

                    }else if(Mode_j == DSC)
                    {
                        g->ptr_data = (uchar*)(g->ptr_orig + m*g->widthstep + n*sizeof(double));
                        value += value_F * (*((double*)g->ptr_data));//*g->ptr_data[m*g->cols+n];

                    }

                }
            }

            if(value > max)
            {
                max = value;
            }
            if(value < min)
            {
                min = value;

            }

            if(isCutted == true)
            {
                if(i >= borderRows && i < OutputRows-borderRows && j >= borderCols && j < OutputCols-borderCols)
                {
                    index = (i-borderRows)*3*this->cols+ 3*(j-borderCols);
                    this->rgb.ptr_double_data[index + 0] = value;
                    this->rgb.ptr_double_data[index + 1] = value;
                    this->rgb.ptr_double_data[index + 2] = value;

                }

            }else
            {
                this->rgb.ptr_double_data[i*3*this->cols+ 3*j + channel] = value;

            }

           ////cout << " " <<(double)Output->rgb.ptr_double_data[i*3*Output->cols+ 3*j + 0];


        }
        max_n = 0;
        jump_j = 0;
        Mode_j = ASC;
        ////cout << endl;

    }
    //cout << endl << "Convolution: ";
    //cout << endl << "Max: " << max;
    //cout << endl << "Min: " << min;


}

void Image::normalize(bool is_double)
{
    // No funciona
    int index =0;
    for(register int i=0; i<this->rows; i++){
        this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
        for(register int j=0; j<this->cols; j++){
            index = i*3*this->cols + 3*j;
            if(is_double == true){
                this->rgb.ptr_double_data[index + 0] = this->rgb.ptr_double_data[index + 0]/255.0;
                this->rgb.ptr_double_data[index + 1] = this->rgb.ptr_double_data[index + 1]/255.0;
                this->rgb.ptr_double_data[index + 2] = this->rgb.ptr_double_data[index + 2]/255.0;
            }else{
                this->rgb.ptr_double_data[index + 0] = (double)this->rgb.ptr_data[3*j + 0]/255.0;
                this->rgb.ptr_double_data[index + 1] = (double)this->rgb.ptr_data[3*j + 1]/255.0;
                this->rgb.ptr_double_data[index + 2] = (double)this->rgb.ptr_data[3*j + 2]/255.0;
            }
        }
     }


}
void Image::denormalize(bool is_double)
{
    // No funciona
    int index =0;
    for(register int i=0; i<this->rows; i++){
        this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
        for(register int j=0; j<this->cols; j++){
            index = i*3*this->cols + 3*j;
            if(is_double == true){
                this->rgb.ptr_double_data[index + 0] = this->rgb.ptr_double_data[index + 0]*255.0;
                this->rgb.ptr_double_data[index + 1] = this->rgb.ptr_double_data[index + 1]*255.0;
                this->rgb.ptr_double_data[index + 2] = this->rgb.ptr_double_data[index + 2]*255.0;
            }else{
                this->rgb.ptr_double_data[index + 0] = (double)this->rgb.ptr_data[3*j + 0]*255.0;
                this->rgb.ptr_double_data[index + 1] = (double)this->rgb.ptr_data[3*j + 1]*255.0;
                this->rgb.ptr_double_data[index + 2] = (double)this->rgb.ptr_data[3*j + 2]*255.0;
            }
        }
     }


}
void Image::remove_zeros(bool is_double)
{
    int index =0;
    for(register int i=0; i<this->rows; i++){
        this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
        for(register int j=0; j<this->cols; j++){
            index = i*3*this->cols + 3*j;
            if(is_double == true){
                if(this->rgb.ptr_double_data[index + 0] < 1)
                    this->rgb.ptr_double_data[index + 0] = 1.0;
                if(this->rgb.ptr_double_data[index + 1] < 1)
                    this->rgb.ptr_double_data[index + 1] = 1.0;
                if(this->rgb.ptr_double_data[index + 2] < 1)
                    this->rgb.ptr_double_data[index + 2] = 1.0;
            }else{
                if(this->rgb.ptr_data[3*j + 0] < 1)
                    this->rgb.ptr_data[3*j + 0] = 1;
                if(this->rgb.ptr_data[3*j + 1] < 1)
                    this->rgb.ptr_data[3*j + 1] = 1;
                if(this->rgb.ptr_data[3*j + 2] < 1)
                    this->rgb.ptr_data[3*j + 2] = 1;
            }
        }
    }
}

void Image::scaling_input_values_with_image(Image* scale_image, double factor)
{
    double valueR = 0.0, valueG = 0.0, valueB = 0.0, scaleValueR=0.0, scaleValueG=0.0, scaleValueB=0.0;
    int index=0;
    if(this->rows != scale_image->rows ||this->cols != scale_image->cols)
    {
        //cout << endl<< "Image sizes must be equal" << endl;
        return ;
    }
    for(int i=0; i<this->rows; i++)
    {
        this->rgb.ptr_data = (uchar*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
        //scale_image->rgb.ptr_data = (uchar*)(scale_image->rgb.ptr_orig + i* scale_image->rgb.widthstep);
        //this->rgb.ptr_double_data = (double*)(this->rgb.ptr_orig + i* this->rgb.widthstep);
        for(int j=0; j<this->cols; j++)
        {
            index = i*3*scale_image->cols + 3*j;
            scaleValueR = (double)scale_image->rgb.ptr_double_data[index + 2];
            scaleValueG = (double)scale_image->rgb.ptr_double_data[index + 1];
            scaleValueB = (double)scale_image->rgb.ptr_double_data[index + 0];

            if(scaleValueR == 0)
            {
                valueR = 0.0;
            }else
            {
                valueR = (double)(this->rgb.ptr_data[3*j + 2])/(factor*(scaleValueR));
            }

            if(scaleValueG == 0)
            {
                valueG = 0.0;
            }else
            {
                valueG = (double)(this->rgb.ptr_data[3*j + 1])/(factor*(scaleValueG));
            }

            if(scaleValueB == 0)
            {
                valueB = 0.0;
            }else
            {
                valueB = (double)(this->rgb.ptr_data[3*j + 0])/(factor*(scaleValueB));
            }

            this->rgb.ptr_double_data[i*3*this->cols + 3*j + 2] = valueR;
            this->rgb.ptr_double_data[i*3*this->cols + 3*j + 1] = valueG;
            this->rgb.ptr_double_data[i*3*this->cols + 3*j + 0] = valueB;
//            this->rgb.ptr_double_data[i*3*this->cols + 3*j + 2] = scaleValueR;
//            this->rgb.ptr_double_data[i*3*this->cols + 3*j + 1] = scaleValueG;
//            this->rgb.ptr_double_data[i*3*this->cols + 3*j + 0] = scaleValueB;
        }
     }



}

void Image::color_shifts(int type, Image* a)
{
    /*
     *  type:
     *      0:  Normal color_shifts
     *      1:  normalized Color Shifts
     * */
    register int i=0, j=0;
    int rows = this->rows, cols = this->cols, index = 0;
    double aConstant = 0.0, cConstant = 0.0;

    if(type == 0)
    {
        while(i<rows)
        {
            this->rgb.ptr_data = (uchar *)(this->rgb.ptr_orig + i*this->rgb.widthstep);
            while(j<cols)
            {
                index = i*3*cols + 3*j;
                aConstant = (1.0)/(3.0) *(a->rgb.ptr_double_data[index + 2] + a->rgb.ptr_double_data[index + 1] + a->rgb.ptr_double_data[index + 0]);
                this->rgb.ptr_double_data[index + 2] = this->rgb.ptr_data[3*j + 2] - a->rgb.ptr_double_data[index + 2] + aConstant;

                this->rgb.ptr_double_data[index + 1] = this->rgb.ptr_data[3*j + 1] - a->rgb.ptr_double_data[index + 1] + aConstant;

                this->rgb.ptr_double_data[index + 0] = this->rgb.ptr_data[3*j + 0] - a->rgb.ptr_double_data[index + 0] + aConstant;
                j++;
            }
            j=0;
            i++;
        }
    }else if(type == 1)
    {
        while(i<rows)
        {
            this->rgb.ptr_data = (uchar *)(this->rgb.ptr_orig + i*this->rgb.widthstep);
            while(j<cols)
            {
                index = i*3*cols + 3*j;
                // aConstant is different of Normal Color Shifts
                aConstant = (a->rgb.ptr_double_data[index + 2] + a->rgb.ptr_double_data[index + 1] + a->rgb.ptr_double_data[index + 0]);
                cConstant = this->rgb.ptr_data[3*j + 2] + this->rgb.ptr_data[3*j + 1] + this->rgb.ptr_data[3*j + 0];

                this->rgb.ptr_double_data[index + 2] = this->rgb.ptr_data[3*j + 2] -(cConstant)/(aConstant)*(a->rgb.ptr_double_data[index + 2] - 1.0/3.0*(aConstant));
                this->rgb.ptr_double_data[index + 1] = this->rgb.ptr_data[3*j + 1] -(cConstant)/(aConstant)*(a->rgb.ptr_double_data[index + 1] - 1.0/3.0*(aConstant));
                this->rgb.ptr_double_data[index + 0] = this->rgb.ptr_data[3*j + 0] -(cConstant)/(aConstant)*(a->rgb.ptr_double_data[index + 0] - 1.0/3.0*(aConstant));

                j++;
            }
            j=0;
            i++;
        }
    }


}

void Image::full_range_adjust(double rgb_min, double rgb_max, data_type_conversion data_type_conv,double *vec_max, double *vec_min, bool is_available)
{
    double maxR, minR, maxG, minG, maxB, minB, valueR=0.0, valueG=0.0, valueB=0.0;
    if(is_available == true)
    {
        maxR = maxG = maxB = -999999;
        minR = minG = minB =  999999;
    }else
    {
        maxR = vec_max[0];
        maxG = vec_max[1];
        maxB = vec_max[2];
        minR = vec_min[0];
        minG = vec_min[1];
        minB = vec_min[2];
    }


    int height = this->rows;
    int width = this->cols;
    int N = width*height;
    int index = 0;
    register int i=0, j=0;

    double rgbDif= rgb_max-rgb_min;

    switch(data_type_conv)
    {
        case DOUBLE_DATA_2_DOUBLE_DATA:
        {
            if(is_available == true)
            {
                index = 0;
                while(index<(N*3))
                {
                    //index = i*3*width + 3*j;
                    valueB = this->rgb.ptr_double_data[index +0];
                    valueG = this->rgb.ptr_double_data[index +1];
                    valueR = this->rgb.ptr_double_data[index +2];

                    if(valueB > maxB)
                        maxB = valueB;
                    if(valueG > maxG)
                        maxG = valueG;
                    if(valueR > maxR)
                        maxR = valueR;

                    if(valueB < minB)
                        minB = valueB;
                    if(valueG < minG)
                        minG = valueG;
                    if(valueR < minR)
                        minR = valueR;
                    index = index + 3;
                }


            }
            index = 0;
            while(index<(N*3))
            {
                valueB = (rgbDif)/(maxB-minB)*(this->rgb.ptr_double_data[index  +0] - minB) + rgb_min;
                valueG = (rgbDif)/(maxG-minG)*(this->rgb.ptr_double_data[index  +1] - minG) + rgb_min;
                valueR = (rgbDif)/(maxR-minR)*(this->rgb.ptr_double_data[index  +2] - minR) + rgb_min;
                this->rgb.ptr_double_data[index  +0] = valueB;
                this->rgb.ptr_double_data[index  +1] = valueG;
                this->rgb.ptr_double_data[index  +2] = valueR;
                index = index + 3;
            }



        }break;
        case DOUBLE_DATA_2_UCHAR_DATA:
        {
            if(is_available == true)
            {
                index = 0;
                while(index<(N*3))
                {
                    valueB = this->rgb.ptr_double_data[index  +0];
                    valueG = this->rgb.ptr_double_data[index  +1];
                    valueR = this->rgb.ptr_double_data[index  +2];


                    if(valueB > maxB)
                        maxB = valueB;
                    if(valueG > maxG)
                        maxG = valueG;
                    if(valueR > maxR)
                        maxR = valueR;

                    if(valueB < minB)
                        minB = valueB;
                    if(valueG < minG)
                        minG = valueG;
                    if(valueR < minR)
                        minR = valueR;
                    index = index + 3;
                }

            }
            //cout << endl << "Max:( " << maxR << ", " << maxG << ", " << maxB << ")";
            //cout << endl << "Min:( " << minR << ", " << minG << ", " << minB << ")";


            i=0; j=0;
            while(i<height)
            {
                this->rgb.ptr_data = (uchar *)(this->rgb.ptr_orig + i*this->rgb.widthstep);
                while(j<width)
                {
                    if(maxB && maxG && maxR)
                    {
                        index = i*3*width + 3*j;
                        valueB = (rgbDif)/(maxB-minB)*(this->rgb.ptr_double_data[index  +0] - minB);
                        valueG = (rgbDif)/(maxG-minG)*(this->rgb.ptr_double_data[index  +1] - minG);
                        valueR = (rgbDif)/(maxR-minR)*(this->rgb.ptr_double_data[index  +2] - minR);
                        if(valueB>255.0)
                            valueB = 255.0;
                        if(valueG>255.0)
                            valueG = 255.0;
                        if(valueR>255.0)
                            valueR = 255.0;

                        index = 3*j;
                        this->rgb.ptr_data[index  +0] = (uchar)valueB;
                        this->rgb.ptr_data[index  +1] = (uchar)valueG;
                        this->rgb.ptr_data[index  +2] = (uchar)valueR;
                        j++;
                    }else
                    {
                        index = i*3*width + 3*j;
                        valueB = (rgbDif)/(200)*(this->rgb.ptr_double_data[index  +0] );
                        valueG = (rgbDif)/(200)*(this->rgb.ptr_double_data[index  +1] );
                        valueR = (rgbDif)/(200)*(this->rgb.ptr_double_data[index  +2] );

                        index = 3*j;
                        this->rgb.ptr_data[index  +0] = (uchar)valueB;
                        this->rgb.ptr_data[index  +1] = (uchar)valueG;
                        this->rgb.ptr_data[index  +2] = (uchar)valueR;
                        j++;
                    }
                }
                j=0;
                i++;
            }

        }break;
        case NOTHING:
        {
            i=0; j=0;
            while(i<height)
            {
                this->rgb.ptr_data = (uchar *)(this->rgb.ptr_orig + i*this->rgb.widthstep);
                while(j<width)
                {


                        index = i*3*width + 3*j;
                        valueB = this->rgb.ptr_double_data[index  +0];
                        valueG = this->rgb.ptr_double_data[index  +1];
                        valueR = this->rgb.ptr_double_data[index  +2];
                        if(valueB>255.0)
                            valueB = 255.0;
                        if(valueG>255.0)
                            valueG = 255.0;
                        if(valueR>255.0)
                            valueR = 255.0;

                        index = 3*j;
                        this->rgb.ptr_data[index  +0] = (uchar)valueB;
                        this->rgb.ptr_data[index  +1] = (uchar)valueG;
                        this->rgb.ptr_data[index  +2] = (uchar)valueR;
                        j++;

                }
                j=0;
                i++;
            }

        }break;
        case UCHAR_DATA_2_UCHAR_DATA:
        {
            if(is_available == true)
            {
                for(int i=0;i<height; i++)
                {
                    this->rgb.ptr_data = (uchar *)(this->rgb.ptr_orig + i*this->rgb.widthstep);
                    for(int j=0; j<width; j++)
                    {
                        index = 3*j;

                        valueB = (double)this->rgb.ptr_data[index  +0];
                        valueG = (double)this->rgb.ptr_data[index  +1];
                        valueR = (double)this->rgb.ptr_data[index  +2];

                        if(valueB > maxB)
                            maxB = valueB;
                        if(valueG > maxG)
                            maxG = valueG;
                        if(valueR > maxR)
                            maxR = valueR;

                        if(valueB < minB)
                            minB = valueB;
                        if(valueG < minG)
                            minG = valueG;
                        if(valueR < minR)
                            minR = valueR;

                    }
                }
            }


            for(int i=0;i<height; i++)
            {
                this->rgb.ptr_data = (uchar *)(this->rgb.ptr_orig + i*this->rgb.widthstep);
                for(int j=0; j<width; j++)
                {
                    index  = 3*j;
                    valueB = (rgbDif)/(maxB-minB)*((double)this->rgb.ptr_data[index  +0]  - minB) + rgb_min;
                    valueG = (rgbDif)/(maxG-minG)*((double)this->rgb.ptr_data[index  +1] - minG) + rgb_min;
                    valueR = (rgbDif)/(maxR-minR)*((double)this->rgb.ptr_data[index  +2] - minR) + rgb_min;
                    this->rgb.ptr_data[index  +0] = (uchar)valueB;
                    this->rgb.ptr_data[index  +1] = (uchar)valueG;
                    this->rgb.ptr_data[index  +2] = (uchar)valueR;

                }
            }
        }break;

    }
    if(vec_max != NULL && vec_min != NULL)
    {
        vec_max[0] = maxR;
        vec_max[1] = maxG;
        vec_max[2] = maxB;
        vec_min[0] = minR;
        vec_min[1] = minG;
        vec_min[2] = minB;
    }




}
void Image::copy_to_image(Image *out, data_type_conversion data_type_conv)
{
    double valueR=0.0, valueG=0.0, valueB=0.0;



    int height = this->rows;
    int width = this->cols;
    int N = width*height;
    int index = 0;
    register int i=0, j=0;



    switch(data_type_conv)
    {

        case NOTHING:
        {
            i=0; j=0;
            while(i<height)
            {
                out->rgb.ptr_data = (uchar *)(this->rgb.ptr_orig + i*this->rgb.widthstep);
                while(j<width)
                {


                        index = i*3*width + 3*j;
                        valueB = this->rgb.ptr_double_data[index  +0];
                        valueG = this->rgb.ptr_double_data[index  +1];
                        valueR = this->rgb.ptr_double_data[index  +2];
                        if(valueB>255.0)
                            valueB = 255.0;
                        if(valueG>255.0)
                            valueG = 255.0;
                        if(valueR>255.0)
                            valueR = 255.0;

                        index = 3*j;
                        out->rgb.ptr_data[index  +0] = (uchar)valueB;
                        out->rgb.ptr_data[index  +1] = (uchar)valueG;
                        out->rgb.ptr_data[index  +2] = (uchar)valueR;
                        j++;

                }
                j=0;
                i++;
            }

        }break;


    }





}

int Image::get_threshold_by_otsu(Component comp)
{
    double mean_b=0.0, mean_f=0.0,  wb=0.0, wf=0.0, sum=0.0,max=-9999;
    double sumb = 0.0, betweenVarianceClass=0.0;
    int t=0, optimalThreshold = 0,Hist_b=0, Hist_f=0 ;
    int N = this->rows*this->cols;
    SVector Hist;
    SVector PVector = SVector(256);
    SVector nVector = SVector(256);
    Hist = this->histogram_vector(comp,0,true);

    for(int i=0;i<256;i++)
    {
        PVector.datos[i] = Hist.datos[i]/N;
        sum+= i*Hist.datos[i];

    }

    for(t=0; t<256;t++)
    {
        Hist_b += Hist.datos[t];//PVector[t];
        //if(Hist_b==0) continue;

        Hist_f = N-Hist_b;
        //if(Hist_f==0) break;

        wb += PVector.datos[t];
        if(wb == 0.0)
            continue;
        wf = 1.0 - wb;
        if(wf < 0.0001 && wf > -0.001)
            break;
        sumb += t*Hist.datos[t];
        mean_b = (sumb)/((double)Hist_b);
        mean_f = (sum-sumb)/((double)Hist_f);

        //betweenVarianceClass = (double(Hist_b*Hist_f))*(mean_b-mean_f)*(mean_b-mean_f);//wb*wf*(mean_b-mean_f)*(mean_b-mean_f);
        betweenVarianceClass = wb*wf*(mean_b-mean_f)*(mean_b-mean_f);
        if(betweenVarianceClass >  max)
        {
            max = betweenVarianceClass;
            optimalThreshold = t;
        }
    }

    return optimalThreshold;
}
int* Image::get_multi_threshold_by_otsu(Component component, int numberOfThresholds)
{
    double **P=NULL, **S=NULL, **H=NULL;
    int *thresholds = NULL, *indexes = NULL;
    bool *enableIndex = NULL;
    int N = this->rows*this->cols, L = 255;
    double sumS = 0.0, sumP = 0.0;
    double sigma=0.0, max = -9999;
    SVector Hist;
    SVector PVector = SVector(256);
    SVector P_SumVector = SVector(256);
    SVector S_SumVector = SVector(256);
    Hist = this->histogram_vector(component,0,true);
    if(numberOfThresholds>0){
        indexes = new int [numberOfThresholds];
        thresholds = new int [numberOfThresholds];
        enableIndex = new bool [numberOfThresholds];
    }
    else{
        return NULL;
    }
    P = new double*[256];
    S = new double*[256];
    H = new double*[256];
    for(int i = 0 ; i < 256 ; i++){
        P[i] = new double[256]; S[i] = new double[256];     H[i] = new double[256];
    }
    for(register int i=0;i<256;i++)
            for(register int j=0;j<256;j++){
                P[i][j] = 0.0;  S[i][j] = 0.0;      H[i][j] = 0.0;
            }
    for(int i=0;i<numberOfThresholds;i++){
        indexes[i] = 1;
        enableIndex[i] = false;
        thresholds[i] = 0;
    }
//*****************************************************************
    for(int i=1;i<256;i++){
        PVector.datos[i] = Hist.datos[i]/N;
        P_SumVector.datos[i] = PVector.datos[i] + sumP;
        S_SumVector.datos[i] = i*PVector.datos[i] + sumS;
        sumP += PVector.datos[i];
        sumS += i*PVector.datos[i];
    }
    for(register int i=1;i<256;i++){
        for(register int j=i;j<256;j++){
            if(i==1){
                P[i][j] = P_SumVector[j];
                S[i][j] = S_SumVector[j];
            }else{
                P[i][j] = P_SumVector[j] - P_SumVector[i-1];
                S[i][j] = S_SumVector[j] - S_SumVector[i-1];
            }
            if(P[i][j]>0.0001)
                H[i][j] = (S[i][j]*S[i][j])/(P[i][j]);
//          if(i==79 && j == 79){
//              cout << endl << "P["<<i<<", "<<j<<"] = " << P[i][j];
//              cout << "  S["<<i<<", "<<j<<"] = " << S[i][j];
//              cout << "  H["<<i<<", "<<j<<"] = " << H[i][j];
//          }

        }
    }
    enableIndex[numberOfThresholds-1] = true;  //Last Index must be enabled.
    int sub = 0;
    bool isValid = true;  //isValid is for the valid sets.
    do
    {
        isValid = true;
        for(int i=0;i<numberOfThresholds;i++){
            sub = numberOfThresholds-i;
            //Last Index with last valor only appears one time {1,2,3,4,1,2,3,4}, enables the before index and leaves the loop.
            if(indexes[i] == (L + 1 - sub) && i == numberOfThresholds-1){
                indexes[i] = 1;
                if(i != 0)
                    enableIndex[i-1] = true;
                isValid = false;
                continue;
            }
            //This is for the rest of indexes. They are constant in a period and only when increases by one its last value back to 1.
            if(indexes[i] == (L + 2 - sub) && i != numberOfThresholds-1){
                indexes[i] = 1;
                if(i != 0)
                    enableIndex[i-1] = true;
                isValid = false;
                continue;
            }
            // For last index {1,2,3,4} assuming that L is 4.
            if(indexes[i] < (L + 1 - sub) && i ==  numberOfThresholds-1){
                indexes[i]++;
            }
            // For the rest of idexes {1,2,3,4,5} assuming that L is 4
            if(indexes[i] < (L + 2 - sub) && enableIndex[i]==true && i !=  numberOfThresholds-1){
                indexes[i]++;
                enableIndex[i] = false;
                if(indexes[i] == (L + 2 - sub)){
                    isValid = false;
                    continue;
                }
                isValid = true;
            }
        }
        double lastValueIndex = 0;
        //This a filter of sets with no necessary numbers to compute.
        if(isValid == true){
            for(int i=0;i<numberOfThresholds;i++){
                if(indexes[i]<=lastValueIndex){
                    isValid=false;
                    break;
                }
                lastValueIndex = indexes[i];
            }
        }
        //Here we obtain a sigma with the selected thresholds
        if(isValid == true){
            lastValueIndex = 0;
            sigma = 0;
            for(int i=0;i<numberOfThresholds;i++){
                if(i==0){
                    sigma += H[1][indexes[0]];
                }else if(i==numberOfThresholds-1){
                    sigma += H[indexes[i-1]+1][indexes[i]];
                    if(indexes[i]<L){
                        sigma += H[indexes[i]+1][L];}
                    else{
                        sigma += H[indexes[i]][L]; }
                }else{
                    if(indexes[i] == lastValueIndex)
                        sigma += H[indexes[i]][indexes[i]];
                    else
                        sigma += H[indexes[i-1]+1][indexes[i]];
                }
                lastValueIndex = indexes[i];
                //cout << endl << "Sigma: "<<sigma;
            }
            if(sigma>max)
            {
                max = sigma;
                //cout << endl << "Max: "<<max;
                for(int i=0;i<numberOfThresholds;i++){
                    thresholds[i] = indexes[i];
                }
            }
        }
    }while(indexes[0] < (L - (numberOfThresholds-2) ));

//*****************************************************************
    for(int i=0;i<256;i++){
        delete [] P[i];
        delete [] S[i];
    }
    delete P;
    delete S;
    delete indexes;
    delete enableIndex;
    return thresholds;
}
void Image::threshold_umbralization(int threshold)
{
    /*
     * Input Image must be a greyscale Image.
     *
     */
    int N3 = this->rows*this->cols*3;
    register int index =0;
    double value;
    while(index < N3)
    {
        value = this->rgb.ptr_double_data[index + 2];
        if((int)value>threshold)
        {
            this->rgb.ptr_double_data[index + 2] = 255.0;
            this->rgb.ptr_double_data[index + 1] = 255.0;
            this->rgb.ptr_double_data[index + 0] = 255.0;
        }else
        {
            this->rgb.ptr_double_data[index + 2] = 0.0;
            this->rgb.ptr_double_data[index + 1] = 0.0;
            this->rgb.ptr_double_data[index + 0] = 0.0;
        }
        index = index +3;
    }
}
void Image::multi_threshold_umbralization(const int *thresholds, int nThresholds)
{
    /*
     * Input Image must be a grayscale Image.
     *
     */
    int N3 = this->rows*this->cols*3, lastThreshold=0;
    register int index =0;
    double value=0.0, grayValue=0.0;
    while(index < N3){
        value = this->rgb.ptr_double_data[index + 1];
        grayValue = 0.0;
        lastThreshold = 0;
        for(int i=0;i<nThresholds;i++){
            if((int)value >= lastThreshold && (int)value < thresholds[i]){
                this->rgb.ptr_double_data[index + 2] = grayValue;
                this->rgb.ptr_double_data[index + 1] = grayValue;
                this->rgb.ptr_double_data[index + 0] = grayValue;
            }
            lastThreshold = thresholds[i];
            grayValue+= 255.0/(nThresholds-1);
        }

        index = index +3;
    }
}
