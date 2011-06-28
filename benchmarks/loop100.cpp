
// loop100 generated by makeloops.py Tue Jun 28 15:54:20 2011

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
 #define loop100_f77 loop100_f77_
 #define loop100_f77overhead loop100_f77overhead_
 #define loop100_f90 loop100_f90_
 #define loop100_f90overhead loop100_f90overhead_
#elif defined(BZ_FORTRAN_SYMBOLS_WITH_DOUBLE_TRAILING_UNDERSCORES)
 #define loop100_f77 loop100_f77__
 #define loop100_f77overhead loop100_f77overhead__
 #define loop100_f90 loop100_f90__
 #define loop100_f90overhead loop100_f90overhead__
#elif defined(BZ_FORTRAN_SYMBOLS_CAPS)
 #define loop100_f77 LOOP100_F77
 #define loop100_f77overhead LOOP100_F77OVERHEAD
 #define loop100_f90 LOOP100_F90
 #define loop100_f90overhead LOOP100_F90OVERHEAD
#endif

extern "C" {
  void loop100_f77(const int& N, double* a, double* b, double* c, double* d, double* x, const double& u, const double& v, const double& w, const double& z);
  void loop100_f77overhead(const int& N, double* a, double* b, double* c, double* d, double* x, const double& u, const double& v, const double& w, const double& z);
  void loop100_f90(const int& N, double* a, double* b, double* c, double* d, double* x, const double& u, const double& v, const double& w, const double& z);
  void loop100_f90overhead(const int& N, double* a, double* b, double* c, double* d, double* x, const double& u, const double& v, const double& w, const double& z);

}

void VectorVersion(BenchmarkExt<int>& bench, double u, double v, double w, double z);
void ArrayVersion(BenchmarkExt<int>& bench, double u, double v, double w, double z);
void ArrayVersion_unaligned(BenchmarkExt<int>& bench, double u, double v, double w, double z);
void ArrayVersion_misaligned(BenchmarkExt<int>& bench, double u, double v, double w, double z);
void ArrayVersion_index(BenchmarkExt<int>& bench, double u, double v, double w, double z);
void doTinyVectorVersion(BenchmarkExt<int>& bench, double u, double v, double w, double z);
void F77Version(BenchmarkExt<int>& bench, double u, double v, double w, double z);
#ifdef FORTRAN_90
void F90Version(BenchmarkExt<int>& bench, double u, double v, double w, double z);
#endif
#ifdef BENCHMARK_VALARRAY
void ValarrayVersion(BenchmarkExt<int>& bench, double u, double v, double w, double z);
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

    BenchmarkExt<int> bench("loop100: $x=(1.0-$c*$c)/((4*w)*sin(1.0+$c*$c-2*v*$c))*$a*$b*u*exp(-z*$d)", numBenchmarks);

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
        flops(i) = 18 * parameters(i);
    }

    bench.setParameterVector(parameters);
    bench.setIterations(iters);
    bench.setOpsPerIteration(flops);
    bench.setDependentVariable("flops");
    bench.beginBenchmarking();

    double u = 0.39123982498157938742;
    double v = 0.39123982498157938742;
    double w = 0.39123982498157938742;
    double z = 0.39123982498157938742;


    ArrayVersion(bench, u, v, w, z);
    ArrayVersion_unaligned(bench, u, v, w, z);
    ArrayVersion_misaligned(bench, u, v, w, z);
    ArrayVersion_index(bench, u, v, w, z);
    doTinyVectorVersion(bench, u, v, w, z);
    F77Version(bench, u, v, w, z);
#ifdef FORTRAN_90
    F90Version(bench, u, v, w, z);
#endif
#ifdef BENCHMARK_VALARRAY
    ValarrayVersion(bench, u, v, w, z);
#endif

    if(runvector)
      VectorVersion(bench, u, v, w, z);

    bench.endBenchmarking();

    bench.saveMatlabGraph("loop100.m");
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

void VectorVersion(BenchmarkExt<int>& bench, double u, double v, double w, double z)
{
    bench.beginImplementation("Vector<T>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;

        Vector<double> a(N);
        initializeRandomDouble(a.data(), N);
        Vector<double> b(N);
        initializeRandomDouble(b.data(), N);
        Vector<double> c(N);
        initializeRandomDouble(c.data(), N);
        Vector<double> d(N);
        initializeRandomDouble(d.data(), N);
        Vector<double> x(N);
        initializeRandomDouble(x.data(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x=(1.0-c*c)/((4*w)*sin(1.0+c*c-2*v*c))*a*b*u*exp(-z*d);
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


  void ArrayVersion(BenchmarkExt<int>& bench, double u, double v, double w, double z)
{
    bench.beginImplementation("Array<T,1>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;

        Array<double,1> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        Array<double,1> b(N);
        initializeRandomDouble(b.dataFirst(), N);
        Array<double,1> c(N);
        initializeRandomDouble(c.dataFirst(), N);
        Array<double,1> d(N);
        initializeRandomDouble(d.dataFirst(), N);
        Array<double,1> x(N);
        initializeRandomDouble(x.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x=(1.0-c*c)/((4*w)*sin(1.0+c*c-2*v*c))*a*b*u*exp(-z*d);
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


  void ArrayVersion_index(BenchmarkExt<int>& bench, double u, double v, double w, double z)
{
    bench.beginImplementation("Array<T,1> (indexexpr.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;

        Array<double,1> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        Array<double,1> b(N);
        initializeRandomDouble(b.dataFirst(), N);
        Array<double,1> c(N);
        initializeRandomDouble(c.dataFirst(), N);
        Array<double,1> d(N);
        initializeRandomDouble(d.dataFirst(), N);
        Array<double,1> x(N);
        initializeRandomDouble(x.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x=(1.0-c(tensor::i)*c(tensor::i))/((4*w)*sin(1.0+c(tensor::i)*c(tensor::i)-2*v*c(tensor::i)))*a(tensor::i)*b(tensor::i)*u*exp(-z*d(tensor::i));
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

  void ArrayVersion_unaligned(BenchmarkExt<int>& bench, double u, double v, double w, double z)
{
    bench.beginImplementation("Array<T,1> (unal.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;


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

    Array<double,1> xfill(N+1);
    Array<double,1> x(xfill(Range(1,N)));
    initializeRandomDouble(x.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x=(1.0-c*c)/((4*w)*sin(1.0+c*c-2*v*c))*a*b*u*exp(-z*d);
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

  void ArrayVersion_misaligned(BenchmarkExt<int>& bench, double u, double v, double w, double z)
{
    bench.beginImplementation("Array<T,1> (misal.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;


    Array<double,1> afill(N+5);
    Array<double,1> a(afill(Range(0,N+0-1)));
    initializeRandomDouble(a.dataFirst(), N);

    Array<double,1> bfill(N+5);
    Array<double,1> b(bfill(Range(1,N+1-1)));
    initializeRandomDouble(b.dataFirst(), N);

    Array<double,1> cfill(N+5);
    Array<double,1> c(cfill(Range(2,N+2-1)));
    initializeRandomDouble(c.dataFirst(), N);

    Array<double,1> dfill(N+5);
    Array<double,1> d(dfill(Range(3,N+3-1)));
    initializeRandomDouble(d.dataFirst(), N);

    Array<double,1> xfill(N+5);
    Array<double,1> x(xfill(Range(4,N+4-1)));
    initializeRandomDouble(x.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x=(1.0-c*c)/((4*w)*sin(1.0+c*c-2*v*c))*a*b*u*exp(-z*d);
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
void TinyVectorVersion(BenchmarkExt<int>& bench, double u, double v, double w, double z)
{
        cout << bench.currentImplementation() << ": N = " << N << endl;

        const int sz = bench.getParameter();
        assert(N==sz);
                           
        long iters = bench.getIterations();

        TinyVector<double,N> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        TinyVector<double,N> b(N);
        initializeRandomDouble(b.dataFirst(), N);
        TinyVector<double,N> c(N);
        initializeRandomDouble(c.dataFirst(), N);
        TinyVector<double,N> d(N);
        initializeRandomDouble(d.dataFirst(), N);
        TinyVector<double,N> x(N);
        initializeRandomDouble(x.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x=(1.0-c*c)/((4*w)*sin(1.0+c*c-2*v*c))*a*b*u*exp(-z*d);
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}
        bench.stopOverhead();

	TinyVectorVersion<N>>1>(bench, u, v, w, z);
}

// end recursion
template<>
void TinyVectorVersion<0>(BenchmarkExt<int>& bench, double u, double v, double w, double z)
{}

void doTinyVectorVersion(BenchmarkExt<int>& bench, double u, double v, double w, double z)
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

  TinyVectorVersion< 1<<tvNmax >(bench, u, v, w, z);
  bench.endImplementation();
}


#ifdef BENCHMARK_VALARRAY
void ValarrayVersion(BenchmarkExt<int>& bench, double u, double v, double w, double z)
{
    bench.beginImplementation("valarray<T>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << bench.currentImplementation() << ": N = " << N << endl;

        long iters = bench.getIterations();

        valarray<double> a(N);
        initializeRandomDouble(a, N);
        valarray<double> b(N);
        initializeRandomDouble(b, N);
        valarray<double> c(N);
        initializeRandomDouble(c, N);
        valarray<double> d(N);
        initializeRandomDouble(d, N);
        valarray<double> x(N);
        initializeRandomDouble(x, N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x=(1.0-c*c)/((4*w)*sin(1.0+c*c-2*v*c))*a*b*u*exp(-z*d);
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

void F77Version(BenchmarkExt<int>& bench, double u, double v, double w, double z)
{
    bench.beginImplementation("Fortran 77");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << bench.currentImplementation() << ": N = " << N << endl;

        int iters = bench.getIterations();

        double* a = new double[N];
        initializeRandomDouble(a, N);
        double* b = new double[N];
        initializeRandomDouble(b, N);
        double* c = new double[N];
        initializeRandomDouble(c, N);
        double* d = new double[N];
        initializeRandomDouble(d, N);
        double* x = new double[N];
        initializeRandomDouble(x, N);
        

        bench.start();
        for (int iter=0; iter < iters; ++iter)
            loop100_f77(N, a, b, c, d, x, u, v, w, z);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            loop100_f77overhead(N, a, b, c, d, x, u, v, w, z);

        bench.stopOverhead();

        delete [] a;
        delete [] b;
        delete [] c;
        delete [] d;
        delete [] x;

    }

    bench.endImplementation();
}

#ifdef FORTRAN_90
void F90Version(BenchmarkExt<int>& bench, double u, double v, double w, double z)
{
    bench.beginImplementation("Fortran 90");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << bench.currentImplementation() << ": N = " << N << endl;

        int iters = bench.getIterations();

        double* a = new double[N];
        initializeRandomDouble(a, N);
        double* b = new double[N];
        initializeRandomDouble(b, N);
        double* c = new double[N];
        initializeRandomDouble(c, N);
        double* d = new double[N];
        initializeRandomDouble(d, N);
        double* x = new double[N];
        initializeRandomDouble(x, N);


        bench.start();
        for (int iter=0; iter < iters; ++iter)
            loop100_f90(N, a, b, c, d, x, u, v, w, z);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            loop100_f90overhead(N, a, b, c, d, x, u, v, w, z);

        bench.stopOverhead();
        delete [] a;
        delete [] b;
        delete [] c;
        delete [] d;
        delete [] x;

    }

    bench.endImplementation();
}
#endif

