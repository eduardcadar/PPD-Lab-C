#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <barrier>
#include <mutex>
using namespace std;

class my_barrier {
public:
    my_barrier(int count) : thread_count(count), counter(0), waiting(0) {}
    void wait() {
        //fence mechanism
        std::unique_lock<std::mutex> lk(m);
        ++counter;
        ++waiting;
        cv.wait(lk, [&]{return counter >= thread_count;});
        cv.notify_one();
        --waiting;
        if (waiting == 0)
        { //reset barrier
            counter = 0;
        }
        lk.unlock();
    }
private:
    std::mutex m;
    std::condition_variable cv;
    int counter;
    int waiting;
    int thread_count;
};

class Matrix {
public:
    int n, m;
    double** values;

    Matrix() {
        n = m = 0;
        values = 0;
    }
};

Matrix ReadFileAsMatrix(string& filePath) {
    ifstream fin(filePath);

    int n, m;
    fin >> n >> m;
    Matrix matrix = Matrix();
    matrix.n = n;
    matrix.m = m;

    double** matrixValues = new double* [n];
    for (int i = 0; i < n; i++)
        matrixValues[i] = new double[m];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            fin >> matrixValues[i][j];

    matrix.values = matrixValues;
    return matrix;
}

void runThread(Matrix& matrix, Matrix& windowMatrix, int start, int end, my_barrier& thread_barrier) {
    int first = start, last = end - 1, N = matrix.n, M = matrix.m;
    int firstLine = first / M, firstCol = first % M;
    int lastLine = last / M, lastCol = last % M;
    int allocSize = windowMatrix.n / 2;
    int i, j, allocLinesBefore = 0, allocLinesAfter = 0;
    i = firstLine;
    while (i > 0 && allocLinesBefore < allocSize) {
        allocLinesBefore++;
        i--;
    }
    i = lastLine;
    while (i < N - 1 && allocLinesAfter < allocSize) {
        allocLinesAfter++;
        i++;
    }

    // copyMatrixLines - how many lines the copy matrix will have
    // the line number of the last element minus the line number of the first element + 1
    int copyMatrixLines = (last / M) - (first / M) + 1;
    // copyMatrixColumns - how many columns the copy matrix will have
    // max of the column number of the elements between start and end
    int copyMatrixColumns;
    int maxCol = max(last % M + 1, first % M + 1);
    if (copyMatrixLines > 1 || maxCol + allocSize > M)
        copyMatrixColumns = M;
    else
        copyMatrixColumns = maxCol + allocSize;

    copyMatrixLines += allocLinesBefore + allocLinesAfter;
    double** matrixCopy = new double*[copyMatrixLines];
    for (i = 0; i < copyMatrixLines; i++)
        matrixCopy[i] = new double[copyMatrixColumns];
    int lineDifference = first / M - allocLinesBefore;

    int a = firstLine - lineDifference - allocLinesBefore;
    int b = lastLine - lineDifference + allocLinesAfter;
    j = firstCol - allocSize;
    int k = lastCol + allocSize;
    while (j < 0) j++;
    int firstCopyCol = j;
    while (k > M - 1) k--;
    if (firstLine == lastLine) {
        for (i = a; i <= b; i++)
            for (j = firstCopyCol; j <= k; j++)
                matrixCopy[i][j] = matrix.values[i + lineDifference][j];
    }
    else {
        int firstCopy;
        if (allocLinesBefore < allocSize)
            firstCopy = 0;
        else
            firstCopy = a * M + j;
        int lastCopy;
        if (allocLinesAfter < allocSize)
            lastCopy = copyMatrixLines * copyMatrixColumns - 1;
        else
            lastCopy = b * M + k;

        for (i = firstCopy; i <= lastCopy; i++) {
            matrixCopy[i / M][i % M] = matrix.values[i / M + lineDifference][i % M];
        }
    }

    thread_barrier.wait();

    for (i = first; i <= last; i++) {
        double output = 0;
        for (k = -windowMatrix.n / 2; k <= windowMatrix.n / 2; k++) {
            for (int l = -windowMatrix.m / 2; l <= windowMatrix.m / 2; l++) {
                a = (i / M) + k;
                a -= lineDifference;
                b = (i % M) + l;
                if (a < 0) a = 0;
                if (b < 0) b = 0;
                if (a >= copyMatrixLines) a = copyMatrixLines - 1;
                if (b >= copyMatrixColumns) b = copyMatrixColumns - 1;

                output +=
                    matrixCopy[a][b] * windowMatrix.values[k + windowMatrix.n / 2][l + windowMatrix.m / 2];
            }
        }
        matrix.values[i / M][i % M] = output;
    }
}

void SolvePar(Matrix& matrix, Matrix& windowMatrix, int no_threads, my_barrier& thread_barrier) {
    thread* threads = new thread[no_threads];
    int start = 0;
    int size = matrix.n * matrix.m;
    int end = size / no_threads;
    int r = size % no_threads;
    for (int i = 0; i < no_threads; i++) {
        if (r > 0) end++, r--;
        threads[i] = thread(runThread, ref(matrix), ref(windowMatrix), start, end, ref(thread_barrier));
        start = end;
        end += size / no_threads;
    }
    for (int i = 0; i < no_threads; i++)
        threads[i].join();
}

bool SameElements(Matrix& matrix1, Matrix& matrix2)
{
    if (matrix1.n != matrix2.n)
        return false;
    if (matrix1.m != matrix2.m)
        return false;
    for (int i = 0; i < matrix1.n; i++)
        for (int j = 0; j < matrix1.m; j++)
            if (matrix1.values[i][j] != matrix2.values[i][j])
                return false;
    return true;
}

int main(int argc, char* argv[])
{
	int no_threads = 4;
    string matrixPath = "C:\\facultate\\Semestrul 5\\PPD\\PPD_LAB\\Lab1\\src\\matrix";
    string windowMatrixPath = "C:\\facultate\\Semestrul 5\\PPD\\PPD_LAB\\Lab1\\src\\windowMatrix";
    string outputMatrixPath = "C:\\facultate\\Semestrul 5\\PPD\\PPD_LAB\\Lab1\\src\\outputMatrix";
    string matrixNumber = "1";
    if (argc > 1) {
        if (argc != 3) {
            cout << "Give matrix number, then number of threads";
            return 0;
        }
        matrixNumber = argv[1];
        no_threads = stoi(argv[2]);
    }
    my_barrier thread_barrier(no_threads);
    matrixPath += matrixNumber + ".txt";
    windowMatrixPath += matrixNumber + ".txt";
    outputMatrixPath += matrixNumber + ".txt";
    Matrix matrix = ReadFileAsMatrix(matrixPath);
    Matrix windowMatrix = ReadFileAsMatrix(windowMatrixPath);
    Matrix answerMatrix = ReadFileAsMatrix(outputMatrixPath);

    auto start = chrono::steady_clock::now();
    SolvePar(matrix, windowMatrix, no_threads, thread_barrier);
    auto finish = chrono::steady_clock::now();
    auto time = chrono::duration <double, milli>(finish - start).count();

    if (!SameElements(matrix, answerMatrix))
        cout << "Wrong answer";
    else 
        cout << time;
}
