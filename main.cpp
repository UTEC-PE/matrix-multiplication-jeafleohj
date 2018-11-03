/*
 * file: main.cpp
 * author: Jhony Angulo
 * date: 03-11-2018
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <thread>

using namespace std;
int n,m,p;
double **A,**B,**C;
int ncore = 0;
int nthreads;
void mult();
void multN();
void printM(double** M, int n, int m);
int main(){

	default_random_engine generator;
	uniform_int_distribution<int> distribution(0,1000);

	cin>>n>>m>>p;

	A=new double*[n];//n*m
	B=new double*[m];//m*p
	C=new double*[n];//n*p

	for(int i = 0; i<n; ++i) {
		A[i] = new double[m];
		C[i] = new double[p];
		for(int j = 0; j<m; ++j) {
			A[i][j] = distribution(generator);
		}
	}
	for(int i = 0; i<m;i++){
		B[i] = new double[p];
		for(int j = 0; j<p; ++j) {
			B[i][j] = distribution(generator);
		}
	}
	cout << "Mat A" << "\n";
	//printM(A,n,m);
	cout << "Mat B" << "\n";
	//printM(B,m,p);
	auto start = std::chrono::system_clock::now();
	multN();
	auto end = std::chrono::system_clock::now();
	cout << "Mat C(sin hilos)"<< chrono::duration_cast<std::chrono::duration<double> >(end-start).count()<<"seg\n";
	//printM(C,n,p);
	//Clean matriz
	for(int i = 0; i<n; ++i) {
		for(int j = 0; j<p; ++j) {
			C[i][j] = 0;
		}
	}
	//Hilos que soporta el computador
	nthreads = thread::hardware_concurrency();
	vector<thread> threads;

	for(int i = 0; i<nthreads; ++i) {
		threads.emplace_back(mult);

	}
	start = std::chrono::system_clock::now();
	for(int i = 0; i<nthreads; ++i) {
		threads[i].join();
	}
	end = std::chrono::system_clock::now();	
	cout << "Mat C( con "<<nthreads<<" hilos)" << chrono::duration_cast<std::chrono::duration<double> >(end-start).count()<<"seg\n";
	//printM(C,n,p);


	return 0;
}

void mult(){
	//static int core = -1;
	int core = ncore++;
	for (int i = core * n / nthreads; i < (core + 1) * n / nthreads; i++){

        for (int j = 0; j < p; j++){  
            for (int k = 0; k < m; k++){
                C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

void multN(){
	for (int i = 0; i < n ; i++){
        for (int j = 0; j < p; j++){  
            for (int k = 0; k < m; k++){
                C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

void printM(double** M, int n, int m){
	for(int i=0; i<n; i++){
		for(int j=0; j<m; j++){
			cout << M[i][j] << " \n"[j == m-1];
		}
	}putchar(10);
}
