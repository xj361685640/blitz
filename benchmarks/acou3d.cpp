#include <blitz/array.h>
#include <blitz/timer.h>
#include <fstream>

BZ_USING_NAMESPACE(blitz)

#ifdef BZ_FORTRAN_SYMBOLS_WITH_TRAILING_UNDERSCORES
 #define acoustic3d_f90 acoustic3d_f90_
 #define acoustic3d_f77 acoustic3d_f77_
 #define acoustic3d_f90tuned acoustic3d_f90tuned_
 #define acoustic3d_f77tuned acoustic3d_f77tuned_
#endif

#ifdef BZ_FORTRAN_SYMBOLS_CAPS
 #define acoustic3d_f90       ACOUSTIC3D_F90
 #define acoustic3d_f77       ACOUSTIC3D_F77
 #define acoustic3d_f90tuned  ACOUSTIC3D_F90TUNED
 #define acoustic3d_f77tuned  ACOUSTIC3D_F77TUNED
#endif

extern "C" {
void acoustic3d_f90(int& N, int& niters, float& check);
void acoustic3d_f77(int& N, int& niters, float& check);
void acoustic3d_f90tuned(int& N, int& niters, float& check);
void acoustic3d_f77tuned(int& N, int& niters, float& check);
}

float acoustic3D_BlitzRaw(int N, int niters);
float acoustic3D_BlitzInterlacedCycled(int N, int niters);
float acoustic3D_BlitzCycled(int N, int niters);
float acoustic3D_BlitzStencil(int N, int niters);

int main()
{
    Timer timer;
    int N = 112;
    int niters = 210;    // Must be divisible by 3 for tuned Fortran versions
    float check;

    generateFastTraversalOrder(TinyVector<int,2>(N-2,N-2));

    timer.start();
    acoustic3d_f90(N, niters, check);
    timer.stop();
    cout << "Fortran 90: " << timer.elapsedSeconds() << " s  check = " 
         << check << endl;

    timer.start();
    acoustic3d_f77(N, niters, check);
    timer.stop();
    cout << "Fortran 77: " << timer.elapsedSeconds() << " s  check = " 
         << check << endl;

    timer.start();
    acoustic3d_f90tuned(N, niters, check);
    timer.stop();
    cout << "Fortran 90 (tuned): " << timer.elapsedSeconds() << " s  check = "
         << check << endl;

    timer.start();
    acoustic3d_f77tuned(N, niters, check);
    timer.stop();
    cout << "Fortran 77 (tuned): " << timer.elapsedSeconds() << " s  check = "
         << check << endl;

    timer.start();
    check = acoustic3D_BlitzRaw(N, niters);
    timer.stop();
    cout << "Blitz++ (raw):    " << timer.elapsedSeconds() << " s  check = " 
         << check << endl;

#if 0
    timer.start();
    check = acoustic3D_BlitzInterlaced(N, niters, c);
    timer.stop();
    cout << "Blitz++ (interlaced):    " << timer.elapsedSeconds() << " s  check = "
         << check << endl;
#endif

    timer.start();
    check = acoustic3D_BlitzCycled(N, niters);
    timer.stop();
    cout << "Blitz++ (cycled): " << timer.elapsedSeconds() << " s check = "
         << check << endl;
 
    timer.start();
    check = acoustic3D_BlitzInterlacedCycled(N, niters);
    timer.stop();
    cout << "Blitz++ (interlaced & cycled): " << timer.elapsedSeconds()
         << " s check = " << check << endl;

    timer.start();
    check = acoustic3D_BlitzStencil(N, niters);
    timer.stop();
    cout << "Blitz++ (stencil): " << timer.elapsedSeconds()
         << " s check = " << check << endl;

    return 0;
}

void setupInitialConditions(Array<float,3>& P1, Array<float,3>& P2,
    Array<float,3>& P3, Array<float,3>& c, int N);

void snapshot(const Array<float,3>& P, const Array<float,3>& c);

void checkArray(const Array<float,3>& A, int N);

void setupInitialConditions(Array<float,3>& P1, Array<float,3>& P2,
    Array<float,3>& P3, Array<float,3>& c, int N)
{
    // Set the velocity field
    c(Range(0,N/2-1), Range::all(), Range::all()) = 0.05;
    c(Range(N/2,N-1), Range::all(), Range::all()) = 0.3;

    int cavityLeft = 3*N/7.0-1;
    int cavityRight = 4*N/7.0-1;
    int cavityFront = 3*N/7.0-1;
    int cavityBack = 4*N/7.0-1;
    int cavityTop = 5*N/7.0-1;
    int cavityBottom = 6*N/7.0-1;

    c(Range(cavityTop,cavityBottom),Range(cavityLeft,cavityRight), 
        Range(cavityFront,cavityBack)) = 0.02;

    int cavityTop2 = 1*N/7.0-1;
    int cavityBottom2 = 2*N/7.0-1;
    c(Range(cavityTop2,cavityBottom2),Range(cavityLeft,cavityRight),
        Range(cavityFront,cavityBack)) = 0.001;

    // Initial pressure distribution
    BZ_USING_NAMESPACE(blitz::tensor)
    float ci = N/2-1;
    float cj = N/2-1;
    float ck = N/2-1;
    float s2 = 64.0 * 9.0 / pow2(N/2.0);
    P1 = 0.0;
    P2 = exp(-(pow2(i-ci)+pow2(j-cj)+pow2(k-ck)) * s2);
    P3 = 0.0;

    checkArray(P2, N);
    checkArray(c, N);
}

void checkArray(const Array<float,3>& A, int N)
{
    double check = 0.0;

    for (int i=0; i < N; ++i)
      for (int j=0; j < N; ++j)
        for (int k=0; k < N; ++k)
          check += A(i,j,k) * ((i+1)+N*(j+1)+N*N*(k+1));

    cout << "Array check: " << check << endl;
}

void snapshot(const Array<float,3>& P, const Array<float,3>& c)
{
    static int count = 0, snapshotNum = 0;
    if (++count < 5)
        return;

    count = 0;
    ++snapshotNum;
    char filename[128];
    sprintf(filename, "snapshot%03d.m", snapshotNum);

    ofstream ofs(filename);
    int N = P.length(firstDim);

    int k = N/2;
    float Pmin = -0.2;
    float PScale = 1.0/0.4;
    float VScale = 0.5;

    ofs << "P" << snapshotNum << " = [ ";
    for (int i=0; i < N; ++i)
    {
        for (int j=0; j < N; ++j)
        {
            float value = (P(i,j,k)-Pmin)*PScale + c(i,j,k)*VScale;
            int r = value * 4096;
            ofs << r << " ";
        }
        if (i < N-1)
            ofs << ";" << endl;
    }
    ofs << "];" << endl;
}
