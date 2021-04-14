#include <iostream>
#include <cmath>
#include <conio.h>
#include <cstdlib>
#include <chrono>
#include <windows.h>
using namespace std;
constexpr int WIDTH = 1000, HEIGHT = 600;
void drawPoint(char platno[HEIGHT / 16][WIDTH / 8 + 1], int A, int B, char c) {if(A >= 0 && B >= 0 && A < WIDTH / 8 && B < HEIGHT / 16) platno[B][A] = c;}
void plotLineLow(char platno[HEIGHT / 16][WIDTH / 8 + 1], int x0, int y0, int x1, int y1, char c) {
    int dx = x1 - x0, dy = abs(y1 - y0), yi = (y1 < y0) ? -1 : 1, D = 2 * dy - dx, y = y0;
    for (int x = x0; x <= x1; x++) {
        drawPoint(platno, x, y, c);
        if(D > 0) {
            y += yi;
            D -= 2 * dx;
		}
        D += 2 * dy;
	}
}
void plotLineHigh(char platno[HEIGHT / 16][WIDTH / 8 + 1], int x0, int y0, int x1, int y1, char c) {
    int dx = abs(x1 - x0), dy = y1 - y0, xi = (x1 < x0) ? -1 : 1, D = 2*dx - dy, x = x0;
    for (int y = y0; y <= y1; y++) {
        drawPoint(platno, x, y, c);
		if(D > 0) {
            x += xi;
            D -= 2 * dy;
		}
        D += 2 * dx;
	}
}
void drawLine(char platno[HEIGHT / 16][WIDTH / 8 + 1], int A, int B, int C, int D, char c) {
	if(A > C) {
		int t;
		t = A;
		A = C;
		C = t;
		t = B;
		B = D;
		D = t;
	}
	if(B == D) {
		for(int i = A; i <= C; i++) drawPoint(platno, i, B, c);
		return;
	}
	if(A == C) {
		int min = B, max = D;
		if(D < B) {
			min = D;
			max = B;
		}
		for(int i = min; i <= max; i++) drawPoint(platno, A, i, c);
		return;
	}
	if(abs(D - B) < abs(C - A)) plotLineLow(platno, A, B, C, D, c);
	else if(B > D) plotLineHigh(platno, C, D, A, B, c);
	else plotLineHigh(platno, A, B, C, D, c);
}
class Timer {
	public:
		Timer(): beg_(clock_::now()){}
		void reset(){beg_ = clock_::now();}
		float elapsed() const {return std::chrono::duration_cast<second_> (clock_::now() - beg_).count();}
	private:
		typedef std::chrono::high_resolution_clock clock_;
		typedef std::chrono::duration<float, std::ratio<1>> second_;
		std::chrono::time_point<clock_> beg_;
};
int main(int argc, char** argv) {
	int n = (argc - 1) ? atoi(argv[1]) : 1;
	Timer tmr;
	float length = 150, mass = 10, O1[n], O2[n], w1[n] = {0}, w2[n] = {0}, g = 9.81f, accumulator = 0.0f, frameStart = tmr.elapsed();
	std::fill_n(O2, n, 3.14159265358979323846);
	//for(int i = 0; i < n; i++) O1[i] = O2[i] * (1 + 2 * i / (float) n); // REALY symmetric
	for(int i = 0; i < n; i++) O1[i] = O2[i] * (1 + 2 * i / (float) n) + 0.5f / O2[i]; // more chaotic
	char platno[HEIGHT / 16][WIDTH / 8 + 1];
	for(int i = 0; i < HEIGHT / 16 - 1; i++) platno[i][WIDTH / 8] = '\n';
	platno[HEIGHT / 16 - 1][WIDTH / 8] = '\0';
	for(int i = 0; i < HEIGHT / 16; i++) for(int j = 0; j < WIDTH / 8; j++) platno[i][j] = ' ';
	ShowWindow(GetConsoleWindow(), 3);
	while(true) {
		SetConsoleCursorPosition(GetStdHandle(-11), {0, 0});
		float currentTime = tmr.elapsed();
		accumulator += currentTime - frameStart;
		frameStart = currentTime;
		if(accumulator >= 0.03f) accumulator = 0.03f;
		while(accumulator > 0.01f) {
			for(int i = 0; i < n; i++) {
				w1[i] += (-mass * (3 * g * sin(O1[i]) + g * sin(O1[i] - 2 * O2[i]) + 2 * sin(O1[i] - O2[i]) * (w2[i] * w2[i] * length + w1[i] * w1[i] * length * cos(O1[i] - O2[i]))) / (length * mass * 10 * (3 - cos(2 * O1[i] - 2 * O2[i]))));
				w2[i] += (2 * sin(O1[i] - O2[i]) * (w1[i] * w1[i] * length + g * cos(O1[i]) + w2[i] * w2[i] * length * cos(O1[i] - O2[i]) * 0.5f) / length / (3 - cos(2 * O1[i] - 2 * O2[i]))) / 5;
				O1[i] += w1[i] / 10.0f;
				O2[i] += w2[i] / 10.0f;
			}
			accumulator -= 0.01f;
		}
		for(int i = 0; i < HEIGHT / 16; i++) for(int j = 0; j < WIDTH / 8; j++) platno[i][j] = ' ';
		for(int i = 0; i < n; i++) {
			int x1 = (WIDTH / 2 + sin(O1[i]) * length) / 8 + 0.5f;
			int y1 = cos(O1[i]) * length / 16 + HEIGHT / 32 + 0.5f;
			int x2 = x1 + sin(O2[i]) * length / 8 + 0.5f;
			int y2 = y1 + cos(O2[i]) * length / 16 + 0.5f;
			drawLine(platno, WIDTH / 16, HEIGHT / 32, x1, y1, '.');
			drawLine(platno, x1, y1, x2, y2, '.');
			drawPoint(platno, WIDTH / 16, HEIGHT / 32, 'O');
			drawPoint(platno, x1, y1, 'O');
			drawPoint(platno, x2, y2, 'O');
		}
		puts(platno[0]);
	}
	return 0;
}
