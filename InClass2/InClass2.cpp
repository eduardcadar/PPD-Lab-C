#include <iostream>
#include <thread>
#include <functional>
#include <cmath>
using namespace std;

#define NMAX 100000

double a[NMAX];
double b[NMAX];
double cSeq[NMAX];
double cParLin[NMAX];

void add(double a[], double b[], double c[],
    int start, int stop, function<double(double, double)> f)
{
    for (int i = start; i < stop; i++)
        c[i] = f(a[i], b[i]);
}

bool areEqual(double* a, double* b, int size)
{
    for (int i = 0; i < size; i++)
        if (a[i] != b[i])
            return false;
    return true;
}

void par_lin(double a[], double b[], double c[],
    int start, int stop, int no_threads,
    function<double(double, double)> f)
{
    thread* threads = new thread[no_threads];

    int first, last, r, size;
    first = start, last = stop / no_threads, size = stop - start;
    r = size % no_threads;

    for (int i = 0; i < no_threads; i++) {
        if (r > 0) {
            last++;
            r--;
        }
        threads[i] = thread(add, a, b, c, first, last, f);
        first = last;
        last += size / no_threads;
    }
    for (int i = 0; i < no_threads; i++)
        threads[i].join();
}

int main2()
{
    const int no_threads = 4;
    function<double(double, double)> myFunc = [=](double xx, double yy) {
        return sqrt(pow(xx, 3) + pow(yy, 3));
    };

    for (int i = 0; i < NMAX; i++) {
        a[i] = i;
        b[i] = i;
    }

    auto start = chrono::steady_clock::now();
    add(a, b, cSeq, 0, NMAX, myFunc);
    auto finish = chrono::steady_clock::now();
    auto tSeq = chrono::duration <double, milli>(finish - start).count();
    cout << "Time for seq: " << tSeq << " ms\n";

    start = chrono::steady_clock::now();
    par_lin(a, b, cParLin, 0, NMAX, no_threads, myFunc);
    finish = chrono::steady_clock::now();
    auto tPar = chrono::duration <double, milli>(finish - start).count();
    cout << "Time for par lin: " << tPar << " ms\n";

    cout << "Tseq / Tpar: " << tSeq / tPar << '\n';

    if (areEqual(cSeq, cParLin, NMAX))
        cout << "Same elements";
    else
        cout << "Not same elements";

    return 0;
}


/*

#include <iostream>
#include <thread>
#include <functional>
#include <cmath>
#include <vector>
using namespace std;

#define NMAX 100000

vector<double> a(NMAX);
vector<double> b(NMAX);
vector<double> cSeq(NMAX);
vector<double> cParLin(NMAX);

void add(vector<double>& a, vector<double>& b, vector<double>& c,
    int start, int stop, function<double(double, double)> f)
{
    for (int i = start; i < stop; i++)
        c[i] = f(a[i], b[i]);
}

bool areEqual(vector<double>& a, vector<double>& b, int size)
{
    for (int i = 0; i < size; i++)
        if (a[i] != b[i])
            return false;
    return true;
}

void par_lin(vector<double>& a, vector<double>& b, vector<double>& c,
    int start, int stop, int no_threads,
    function<double(double, double)> f)
{
    thread* threads = new thread[no_threads];

    int first, last, r, size;
    first = start, last = stop / no_threads, size = stop - start;
    r = size % no_threads;

    for (int i = 0; i < no_threads; i++) {
        if (r > 0) {
            last++;
            r--;
        }
        threads[i] = thread(add, ref(a), ref(b), ref(c), first, last, f);
        first = last;
        last += size / no_threads;
    }
    for (int i = 0; i < no_threads; i++)
        threads[i].join();
}

int main()
{
    const int no_threads = 4;
    function<double(double, double)> myFunc = [=](double xx, double yy) {
        return sqrt(pow(xx, 3) + pow(yy, 3));
    };

    for (int i = 0; i < NMAX; i++) {
        a[i] = i;
        b[i] = i;
    }

    auto start = chrono::steady_clock::now();
    add(a, b, cSeq, 0, NMAX, myFunc);
    auto finish = chrono::steady_clock::now();
    auto tSeq = chrono::duration <double, milli>(finish - start).count();
    cout << "Time for seq: " << tSeq << " ms\n";

    start = chrono::steady_clock::now();
    par_lin(a, b, cParLin, 0, NMAX, no_threads, myFunc);
    finish = chrono::steady_clock::now();
    auto tPar = chrono::duration <double, milli>(finish - start).count();
    cout << "Time for par lin: " << tPar << " ms\n";

    cout << "Tseq / Tpar: " << tSeq / tPar << '\n';

    if (areEqual(cSeq, cParLin, NMAX))
        cout << "Same elements";
    else
        cout << "Not same elements";
}

*/