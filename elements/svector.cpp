/*
      ### Computer Vision Demo ###

    - github.com/salvadorbfm/cv_demo
    - Author: Salvador Elizarrarás Montenegro
    - Licensed under the MIT license
*/
#include "svector.h"
using namespace elements;
SVector::SVector(void)
{
    datos=NULL;
    longitud=0;
}

SVector::SVector(int longitud)
{
    this -> datos =NULL;
    this -> longitud=0;

    this->set_longitud(longitud);
}
SVector::SVector(const SVector &init)
{
    this->datos=NULL;
    this->longitud=0;

    datos=new double [init.longitud];
    ::memcpy(this->datos,init.datos,sizeof(double)*init.longitud);

    this->longitud = init.longitud;
}
SVector::~SVector(void)
{
    if(datos)
        delete[]datos;
}
bool SVector::set_longitud(int longitud)
{
    if(longitud <= 0)
        return false;
    if (datos)
    {
        delete[]datos;
        datos=NULL;
        this->longitud=0;
    }
    datos = new double[longitud];
    if(datos == NULL) return false;

    this->longitud = longitud;
    for(register int i=0; i<longitud; i++)
        datos[i] = 0;
    //for(int i=0; i < longitud; i++)
    //  datos[i] = 0.0;
    //::ZeroMemory(datos,longitud*sizeof(double));
    return true;
}
int SVector::get_longitud()
{
    return this->longitud;
}
void SVector::set(int posicion, double valor)
{
#ifdef _DEBUG
    if(posicion <= 0 || posicion < longitud)
        datos[posicion] = valor;
    else
        cout << "Error: Indice Invalido!!";
        //::MessageBox(NULL,_T("indice inv�lido"),_T("SVector::set"),MB_OK|MB_ICONERROR);
#else
    datos[posicion] = valor;
#endif
}
double SVector::get(int posicion)
{
#ifdef _DEBUG
    if(posicion <= 0 || posicion < longitud)
        return datos[posicion];
    else
        cout << "Error: Indice Invalido!!";
        //::MessageBox(NULL,_T("indice inv�lido"),_T("SVector::set"),MB_OK|MB_ICONERROR);
#else
    return datos[posicion];
#endif
}

//bool SVector::GuardarComoCsv(const TCHAR* nombreArchivo)
//{
//  Win::DataFile archivo;
//  if(archivo.CreateForWritting(nombreArchivo) == false)
//      return false;
//  TCHAR texto[64];
//  for(int i=0; i < longitud-1; i++)
//  {
//      _sntprintf(texto,64,_T("%f, "),datos[i]);
//      archivo.Write(texto);
//  }
//  _sntprintf(texto,64,_T("%f"),datos[longitud-1]);
//  archivo.Write(texto);
//  return true;
//}
double& SVector::operator[](long posicion)
{
#ifdef _DEBUG
    if(posicion <= 0 || posicion < longitud)
        return datos[posicion];
    else
        cout << "Error: Indice Invalido!!";
//  ::MessageBox(NULL,_T("indice inv�lido"),_T("SVector::set"),MB_OK|MB_ICONERROR);
#else
    return datos[posicion];
#endif
}

//bool SVector::Guardar(const char *nombreArchivo)
//{
//  int nPoints = 0,i=0;
//  ofstream FILE("points.txt", ios::in );
//  if ( !FILE )
//     {
//        cout << "\n\n FILE couldnt";
//        return false;
//  }
//  FILE >> nPoints;
//  while(i<nPoints)
//  {
//      Geometry::XYZ xyz;
//      FILE >> xyz.x >> xyz.y >> xyz.z;
//      this->Add(xyz);
//      i++;
//  }
//
//  FILE.close();
//
//  return true;
//
//
//}

void SVector::release(void)
{
    if(datos) delete [] datos;
    datos=NULL;
    longitud=0;
}

//bool SVector::Cargar(const TCHAR *nombreArchivo)
//{
//  this ->delete();
//  Win::DataFile archivo;
//
//  if(archivo.CreateForReading(nombreArchivo) == false) return false;
//
//  int noDatos=archivo.get_size()/sizeof(double);
//
//  if(noDatos <= 0) return false;
//
//  datos = new double[noDatos];
//      if(datos==NULL) return false;
//
//  if(archivo.Read(datos,noDatos*sizeof(double)) == false )
//  {
//      this -> delete();
//      return false;
//  }
//  this ->longitud = noDatos;
//
//  return true;
//}


double SVector::get_max(void)
{
    double maximo = datos[0];

    for(int i=1; i<longitud ; i++)
        if(datos[i] > maximo) maximo = datos[i];

    return maximo;
}


SVector& SVector::operator =(const SVector& init)
{
    this ->set_longitud(init.longitud);


    ::memcpy(this->datos,init.datos,sizeof(double)*init.longitud);

    return *this;


}

bool SVector::operator ==(const SVector &init) const
{
    if(this -> longitud != init.longitud) return false;

    for( int i=0; i<init.longitud;i++)
    {
        if(this ->datos[i] != init.datos[i]) return false;
    }
    return true;
}

SVector SVector::operator+(SVector init)
{
    SVector resultado;

    int minimalongitud = min(this -> longitud, init.longitud );

    resultado.set_longitud(minimalongitud);

    for(int i=0; i<minimalongitud ; i++)
        resultado[i]= this->datos[i] + init.datos[i];

    return resultado;
}


int SVector::get_size()
{
 return this->longitud;

}

double SVector::sum(int indexA, int indexB)
{
    double sum = 0;
    for(int i=indexA; i<indexB; i++ )
    {
        sum+=this->datos[i];
    }
    return sum;
}

double SVector::mean()
{
    return this->sum(0,this->get_longitud())/this->get_longitud();
}

double SVector::normalized(double value)
{

    for(register int i=0; i< this->longitud ; i++)
        this->datos[i]/=value;

}
