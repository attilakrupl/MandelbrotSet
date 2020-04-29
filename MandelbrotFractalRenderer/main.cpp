#include <Windows.h>
#include <cmath>
#include <SDL.h>

constexpr const int         kWindowWidth       = 1440;
constexpr const int         kWindowHeight      = 900;
constexpr const int         kRenderingWidth    = 800;
constexpr const int         kRenderingHeight   = 800;

                int         gMaximumIterations = 200;
                long double gMinimumValue      = -2.84;
                long double gMaximumValue      = 1.0;
                long double gRangeValueFactor  = 1;


long double MapValue( long double aValueToMap, long double aInMin, long double aInMax, long double aOutMin, long double aOutMax )
{
    return ( aValueToMap - aInMin ) * ( aOutMax - aOutMin ) / ( aInMax - aInMin ) + aOutMin; 
}

int main( int argc, char** argv )
{
    SDL_Init( SDL_INIT_EVERYTHING );

    SDL_Window*   lWindow;
    SDL_Renderer* lRenderer;
    SDL_Event     lEvent;

    SDL_CreateWindowAndRenderer( kWindowWidth, kWindowHeight, 0, &lWindow, &lRenderer );
    SDL_RenderSetLogicalSize( lRenderer, kRenderingWidth, kRenderingHeight );

    int lZoomCount = 0;

    while( true )
    {
        gMaximumValue -= 0.1 * gRangeValueFactor;
        gMinimumValue += 0.15 * gRangeValueFactor;
        gRangeValueFactor *= 0.9349;
        gMaximumIterations += 5;

        if ( lZoomCount > 30 )
        {
            gMaximumIterations *= 1.02;
        }

        SDL_RenderPresent( lRenderer );

        if ( SDL_PollEvent( &lEvent ) && lEvent.type == SDL_QUIT )
        {
            return 0;
        }

        if ( GetKeyState('Q') & 0x8000 )
        {
            return 0;
        }

        for ( int x = 0; x < kRenderingWidth; ++x )
        {

            for( int y = 0; y < kRenderingHeight; ++y )
            {
                long double lA = MapValue( x, 0, kRenderingWidth, gMinimumValue, gMaximumValue );
                long double lB = MapValue( y, 0, kRenderingHeight, gMinimumValue, gMaximumValue );

                long double lAInitial = lA;
                long double lBInitial = lB;

                int lNumberOfIterations = 0;

                for ( int i = 0; i < gMaximumIterations; ++i )
                {
                    ++lNumberOfIterations;

                    long double lAN = lA * lA - lB * lB;
                    long double lBN = 2 * lA * lB;

                    lA = lAN + lAInitial;
                    lB = lBN + lBInitial;

                    if ( abs(lA) > 2 || abs(lB) > 2 )
                    {
                        break;
                    }

                }

                int lBrightness = MapValue( lNumberOfIterations, 0, gMaximumIterations, 0, 255 );

                if ( ( lNumberOfIterations == gMaximumIterations ) || ( lBrightness < 20 ) )
                {
                    lBrightness = 0;
                }

                int red   = MapValue( lBrightness * lBrightness, 0, 6502, 0, 255 );
                int green = lBrightness;
                int blue  = MapValue( sqrt(lBrightness), 0, sqrt(255), 0, 255 );

                SDL_SetRenderDrawColor( lRenderer, red, green, blue, 255 );
                SDL_RenderDrawPoint( lRenderer, x, y );
            }
        }

        ++lZoomCount;
    }


    return 0;
}