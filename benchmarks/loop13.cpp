
// loop13 generated by makeloops.py Tue Jun 28 15:54:20 2011

#include <blitz/vector2.h>
#include <blitz/array.h>
#include <random/uniform.h>
#include <blitz/benchext.h>

#ifdef BZ_HAVE_VALARRAY
 #define BENCHMARK_VALARRAY
#endif

#ifdef BENCHMARK_VALARRAY
#include <valarray>
#endif

BZ_NAMESPACE(blitz)
extern void sink();
BZ_NAMESPACE_END

BZ_USING_NAMESPACE(blitz)
BZ_USING_NAMESPACE(std)

#if defined(BZ_FORTRAN_SYMBOLS_WITH_TRAILING_UNDERSCORES)
 #define loop13_f77 loop13_f77_
 #define loop13_f77overhead loop13_f77overhead_
 #define loop13_f90 loop13_f90_
 #define loop13_f90overhead loop13_f90overhead_
#elif defined(BZ_FORTRAN_SYMBOLS_WITH_DOUBLE_TRAILING_UNDERSCORES)
 #define loop13_f77 loop13_f77__
 #define loop13_f77overhead loop13_f77overhead__
 #define loop13_f90 loop13_f90__
 #define loop13_f90overhead loop13_f90overhead__
#elif defined(BZ_FORTRAN_SYMBOLS_CAPS)
 #define loop13_f77 LOOP13_F77
 #define loop13_f77overhead LOOP13_F77OVERHEAD
 #define loop13_f90 LOOP13_F90
 #define loop13_f90overhead LOOP13_F90OVERHEAD
#endif

extern "C" {
  void loop13_f77(const int& N, double* y, double* x, double* a, double* b, double* c, double* d, const double& u);
  void loop13_f77overhead(const int& N, double* y, double* x, double* a, double* b, double* c, double* d, const double& u);
  void loop13_f90(const int& N, double* y, double* x, double* a, double* b, double* c, double* d, const double& u);
  void loop13_f90overhead(const int& N, double* y, double* x, double* a, double* b, double* c, double* d, const double& u);

}

void VectorVersion(BenchmarkExt<int>& bench, double u);
void ArrayVersion(BenchmarkExt<int>& bench, double u);
void ArrayVersion_unaligned(BenchmarkExt<int>& bench, double u);
void ArrayVersion_misaligned(BenchmarkExt<int>& bench, double u);
void ArrayVersion_index(BenchmarkExt<int>& bench, double u);
void doTinyVectorVersion(BenchmarkExt<int>& bench, double u);
void F77Version(BenchmarkExt<int>& bench, double u);
#ifdef FORTRAN_90
void F90Version(BenchmarkExt<int>& bench, double u);
#endif
#ifdef BENCHMARK_VALARRAY
void ValarrayVersion(BenchmarkExt<int>& bench, double u);
#endif

const int numSizes = 20;
const int Nmax=1<<(numSizes-1);
const int tvNmax=7;
const bool runvector=false; // no point as long as Vector is Array<1>

int main()
{
    int numBenchmarks = 6;
    if (runvector) numBenchmarks++;
#ifdef BENCHMARK_VALARRAY
    numBenchmarks++;
#endif
#ifdef FORTRAN_90
    numBenchmarks++;
#endif

    BenchmarkExt<int> bench("loop13: $x = $a+$b+$c+$d; $y = u+$d", numBenchmarks);

    bench.setNumParameters(numSizes);

    Array<int,1> parameters(numSizes);
    Array<long,1> iters(numSizes);
    Array<double,1> flops(numSizes);

    for (int i=0; i < numSizes; ++i)
    {
      parameters(i) = Nmax>>i;
      iters(i) = 50000000L / parameters(i);
	
        if (iters(i) < 2)
            iters(i) = 2;
        flops(i) = 4 * parameters(i);
    }

    bench.setParameterVector(parameters);
    bench.setIterations(iters);
    bench.setOpsPerIteration(flops);
    bench.setDependentVariable("flops");
    bench.beginBenchmarking();

    double u = 0.39123982498157938742;


    ArrayVersion(bench, u);
    ArrayVersion_unaligned(bench, u);
    ArrayVersion_misaligned(bench, u);
    ArrayVersion_index(bench, u);
    doTinyVectorVersion(bench, u);
    F77Version(bench, u);
#ifdef FORTRAN_90
    F90Version(bench, u);
#endif
#ifdef BENCHMARK_VALARRAY
    ValarrayVersion(bench, u);
#endif

    if(runvector)
      VectorVersion(bench, u);

    bench.endBenchmarking();

    bench.saveMatlabGraph("loop13.m");
    return 0;
}

template<class T>
void initializeRandomDouble(T* data, int numElements, int stride = 1)
{
    ranlib::Uniform<T> rnd;

    for (int i=0; i < numElements; ++i)
        data[size_t(i*stride)] = rnd.random();
}

template<class T>
void initializeRandomDouble(valarray<T>& data, int numElements, int stride = 1)
{
    ranlib::Uniform<T> rnd;

    for (int i=0; i < numElements; ++i)
        data[size_t(i*stride)] = rnd.random();
}

void VectorVersion(BenchmarkExt<int>& bench, double u)
{
    bench.beginImplementation("Vector<T>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;

        Vector<double> y(N);
        initializeRandomDouble(y.data(), N);
        Vector<double> x(N);
        initializeRandomDouble(x.data(), N);
        Vector<double> a(N);
        initializeRandomDouble(a.data(), N);
        Vector<double> b(N);
        initializeRandomDouble(b.data(), N);
        Vector<double> c(N);
        initializeRandomDouble(c.data(), N);
        Vector<double> d(N);
        initializeRandomDouble(d.data(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = a+b+c+d; y = u+d;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}


  void ArrayVersion(BenchmarkExt<int>& bench, double u)
{
    bench.beginImplementation("Array<T,1>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;

        Array<double,1> y(N);
        initializeRandomDouble(y.dataFirst(), N);
        Array<double,1> x(N);
        initializeRandomDouble(x.dataFirst(), N);
        Array<double,1> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        Array<double,1> b(N);
        initializeRandomDouble(b.dataFirst(), N);
        Array<double,1> c(N);
        initializeRandomDouble(c.dataFirst(), N);
        Array<double,1> d(N);
        initializeRandomDouble(d.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = a+b+c+d; y = u+d;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}


  void ArrayVersion_index(BenchmarkExt<int>& bench, double u)
{
    bench.beginImplementation("Array<T,1> (indexexpr.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;

        Array<double,1> y(N);
        initializeRandomDouble(y.dataFirst(), N);
        Array<double,1> x(N);
        initializeRandomDouble(x.dataFirst(), N);
        Array<double,1> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        Array<double,1> b(N);
        initializeRandomDouble(b.dataFirst(), N);
        Array<double,1> c(N);
        initializeRandomDouble(c.dataFirst(), N);
        Array<double,1> d(N);
        initializeRandomDouble(d.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = a(tensor::i)+b(tensor::i)+c(tensor::i)+d(tensor::i); y(tensor::i) = u+d(tensor::i);
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}

  void ArrayVersion_unaligned(BenchmarkExt<int>& bench, double u)
{
    bench.beginImplementation("Array<T,1> (unal.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;


    Array<double,1> yfill(N+1);
    Array<double,1> y(yfill(Range(1,N)));
    initializeRandomDouble(y.dataFirst(), N);

    Array<double,1> xfill(N+1);
    Array<double,1> x(xfill(Range(1,N)));
    initializeRandomDouble(x.dataFirst(), N);

    Array<double,1> afill(N+1);
    Array<double,1> a(afill(Range(1,N)));
    initializeRandomDouble(a.dataFirst(), N);

    Array<double,1> bfill(N+1);
    Array<double,1> b(bfill(Range(1,N)));
    initializeRandomDouble(b.dataFirst(), N);

    Array<double,1> cfill(N+1);
    Array<double,1> c(cfill(Range(1,N)));
    initializeRandomDouble(c.dataFirst(), N);

    Array<double,1> dfill(N+1);
    Array<double,1> d(dfill(Range(1,N)));
    initializeRandomDouble(d.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = a+b+c+d; y = u+d;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}

  void ArrayVersion_misaligned(BenchmarkExt<int>& bench, double u)
{
    bench.beginImplementation("Array<T,1> (misal.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;


    Array<double,1> yfill(N+6);
    Array<double,1> y(yfill(Range(0,N+0-1)));
    initializeRandomDouble(y.dataFirst(), N);

    Array<double,1> xfill(N+6);
    Array<double,1> x(xfill(Range(1,N+1-1)));
    initializeRandomDouble(x.dataFirst(), N);

    Array<double,1> afill(N+6);
    Array<double,1> a(afill(Range(2,N+2-1)));
    initializeRandomDouble(a.dataFirst(), N);

    Array<double,1> bfill(N+6);
    Array<double,1> b(bfill(Range(3,N+3-1)));
    initializeRandomDouble(b.dataFirst(), N);

    Array<double,1> cfill(N+6);
    Array<double,1> c(cfill(Range(4,N+4-1)));
    initializeRandomDouble(c.dataFirst(), N);

    Array<double,1> dfill(N+6);
    Array<double,1> d(dfill(Range(5,N+5-1)));
    initializeRandomDouble(d.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = a+b+c+d; y = u+d;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}


template<int N>
void TinyVectorVersion(BenchmarkExt<int>& bench, double u)
{
        cout << bench.currentImplementation() << ": N = " << N << endl;

        const int sz = bench.getParameter();
        assert(N==sz);
                           
        long iters = bench.getIterations();

        TinyVector<double,N> y(N);
        initializeRandomDouble(y.dataFirst(), N);
        TinyVector<double,N> x(N);
        initializeRandomDouble(x.dataFirst(), N);
        TinyVector<double,N> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        TinyVector<double,N> b(N);
        initializeRandomDouble(b.dataFirst(), N);
        TinyVector<double,N> c(N);
        initializeRandomDouble(c.dataFirst(), N);
        TinyVector<double,N> d(N);
        initializeRandomDouble(d.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = a+b+c+d; y = u+d;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}
        bench.stopOverhead();

	TinyVectorVersion<N>>1>(bench, u);
}

// end recursion
template<>
void TinyVectorVersion<0>(BenchmarkExt<int>& bench, double u)
{}

void doTinyVectorVersion(BenchmarkExt<int>& bench, double u)
{
  bench.beginImplementation("TinyVector<T>");
  // can't run tinyvector with full length because meta-unrolling
  // kills compiler...
  int N=Nmax;
  while(N> 1<<tvNmax) {
   bench.getParameter();
   bench.getIterations();
   bench.skip();
   N>>=1;
  }

  TinyVectorVersion< 1<<tvNmax >(bench, u);
  bench.endImplementation();
}


#ifdef BENCHMARK_VALARRAY
void ValarrayVersion(BenchmarkExt<int>& bench, double u)
{
    bench.beginImplementation("valarray<T>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << bench.currentImplementation() << ": N = " << N << endl;

        long iters = bench.getIterations();

        valarray<double> y(N);
        initializeRandomDouble(y, N);
        valarray<double> x(N);
        initializeRandomDouble(x, N);
        valarray<double> a(N);
        initializeRandomDouble(a, N);
        valarray<double> b(N);
        initializeRandomDouble(b, N);
        valarray<double> c(N);
        initializeRandomDouble(c, N);
        valarray<double> d(N);
        initializeRandomDouble(d, N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = a+b+c+d; y = u+d;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
	  sink();
	}
        bench.stopOverhead();
    }

    bench.endImplementation();
}
#endif

void F77Version(BenchmarkExt<int>& bench, double u)
{
    bench.beginImplementation("Fortran 77");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << bench.currentImplementation() << ": N = " << N << endl;

        int iters = bench.getIterations();

        double* y = new double[N];
        initializeRandomDouble(y, N);
        double* x = new double[N];
        initializeRandomDouble(x, N);
        double* a = new double[N];
        initializeRandomDouble(a, N);
        double* b = new double[N];
        initializeRandomDouble(b, N);
        double* c = new double[N];
        initializeRandomDouble(c, N);
        double* d = new double[N];
        initializeRandomDouble(d, N);
        

        bench.start();
        for (int iter=0; iter < iters; ++iter)
            loop13_f77(N, y, x, a, b, c, d, u);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            loop13_f77overhead(N, y, x, a, b, c, d, u);

        bench.stopOverhead();

        delete [] y;
        delete [] x;
        delete [] a;
        delete [] b;
        delete [] c;
        delete [] d;

    }

    bench.endImplementation();
}

#ifdef FORTRAN_90
void F90Version(BenchmarkExt<int>& bench, double u)
{
    bench.beginImplementation("Fortran 90");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << bench.currentImplementation() << ": N = " << N << endl;

        int iters = bench.getIterations();

        double* y = new double[N];
        initializeRandomDouble(y, N);
        double* x = new double[N];
        initializeRandomDouble(x, N);
        double* a = new double[N];
        initializeRandomDouble(a, N);
        double* b = new double[N];
        initializeRandomDouble(b, N);
        double* c = new double[N];
        initializeRandomDouble(c, N);
        double* d = new double[N];
        initializeRandomDouble(d, N);


        bench.start();
        for (int iter=0; iter < iters; ++iter)
            loop13_f90(N, y, x, a, b, c, d, u);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            loop13_f90overhead(N, y, x, a, b, c, d, u);

        bench.stopOverhead();
        delete [] y;
        delete [] x;
        delete [] a;
        delete [] b;
        delete [] c;
        delete [] d;

    }

    bench.endImplementation();
}
#endif

