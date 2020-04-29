#include <fstream>
#include <iostream>
#include <chrono>

using namespace std;

double mapToReal( int aX, int aImageWidth, double aMinR, double aMaxR )
{
    double lRange = aMaxR - aMinR;

    return aX * ( lRange / aImageWidth ) + aMinR;
}

double mapToImaginary( int aY, int aImageHeight, double aMinI, double aMaxI)
{
    double lRange = aMaxI - aMinI;

    return aY * ( lRange / aImageHeight ) + aMinI;
}

int findMandelbrot( double aCReal, double aCImaginary, int aMaxIterations )
{
    int lI = 0;
    double lZr = 0.0, lZi = 0.0;
    while ( ( lI < aMaxIterations       )
         && ( lZr * lZr + lZi * lZi < 4.0  ) )
    {
        double lTemp = lZr * lZr - lZi * lZi + aCReal;
        lZi = 2.0 * lZr * lZi + aCImaginary;
        lZr = lTemp;
        ++lI;
    }

    return lI;
}

int main()
{
    ifstream lInputFile( "input.txt" );
    int lImageWidth, lImageHeight, lMaxN;
    double lMinR, lMaxR, lMinI, lMaxI;
    if ( !lInputFile )
    {
        cout << "Could not open file!" << endl;
        cin.ignore();
        return 0;
    }
    lInputFile >> lImageWidth >>lImageHeight >> lMaxN;
    lInputFile >> lMinR >> lMaxR >> lMinI >> lMaxI;
    lInputFile.close();

    //std::chrono::system_clock::time_point lNow   = std::chrono::system_clock::now();
    //long                                  lNowMS = std::chrono::time_point_cast<std::chrono::milliseconds>( lNow ).time_since_epoch().count();

    ofstream lOutputFile( "output_image.ppm" );
    lOutputFile << "P3" << endl;
    lOutputFile << lImageWidth << " " << lImageHeight << endl;
    lOutputFile << "256" << endl;


    for ( int y = 0; y < lImageHeight; ++y )
    {
        for ( int x = 0; x < lImageWidth; ++x )
        {
            double lCReal = mapToReal( x, lImageHeight, lMinR, lMaxR );
            double lCImaginary = mapToImaginary( y, lImageHeight, lMinI, lMaxI );

            int lMandelbrot = findMandelbrot( lCReal, lCImaginary, lMaxN );

            int lColor = ( 255 - lMandelbrot ) % 255;

            int lGreen = ( (int)(lColor * sinf( (float)lColor) ) % 256 );
            int lRed = ( (lColor * lColor) % 256 );
            int lBlue  = ( lColor % 256 );

            lOutputFile << lRed << " " << lGreen << " " << lBlue << " ";
        }
        lOutputFile << endl;
    }
    lOutputFile.close();
    cout << "Finished!" << endl;

    return 0;
}