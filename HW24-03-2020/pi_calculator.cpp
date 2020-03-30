#include <stdio.h>
#include <winsock.h>

static long num_steps = 100000;
static double total_sum = 0;
const int numberOfThread = 8;
const int jump_step = (num_steps / numberOfThread);

double pi;
double step = 1 / ((double)num_steps);

CRITICAL_SECTION cs;

DWORD WINAPI solve(LPVOID arg) {
	double x, sum = 0.0;
	int index = *(int*)arg;
	
	for (int i = index - jump_step; i < index; i++) {
		x = (i + 0.5)*step;
		sum = sum + 4.0/(1.0 + x*x);
	}

	EnterCriticalSection(&cs);
	total_sum += sum;
	LeaveCriticalSection(&cs);
	return 0;
}

main() {
	HANDLE hThread[numberOfThread];
	InitializeCriticalSection(&cs);
	int params[numberOfThread];
	for (int i = 0; i < numberOfThread; i++) {
		params[i] = (jump_step) * (i + 1);
		// = { 12500, 25000, 37500, 50000, 62500, 75000 , 87500, 100000 };
	}

	for (int i = 0; i < numberOfThread; i++) {
		hThread[i] = CreateThread(0, 0, solve, &params[i], 0, 0);
	}

	WaitForMultipleObjects(numberOfThread, hThread, TRUE, INFINITE);

	pi = step * total_sum;
	printf("Pi = %f\n", pi);

	DeleteCriticalSection(&cs);
}