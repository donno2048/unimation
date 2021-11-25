#include <cmath>
#include <iostream>
#include <windows.h>
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))
const int HEIGHT = 40, WIDTH = 80;
class Plot {
	public:
		Plot() {
			for (int i = 0; i < HEIGHT - 1; i++) {
				platno[i][WIDTH] = '\n';
				for (int j = 0; j < WIDTH; j++) platno[i][j] = ' ';
			}
		}
		void zero() {
			for (int i = 0; i < HEIGHT; i++) for (int j = 0; j < WIDTH; j++) platno[i][j] = ' ';
		  	SetConsoleCursorPosition(GetStdHandle(-11), {0, 0});
		}
		void print() {puts(platno[0]);}
		void addPoint(int x, int y, char c) {if (min(x, y) >= 0 && x < WIDTH && y < HEIGHT) platno[y][x] = c;}
		void addLine(int A, int B, int C, int D, char c) {
			if (A > C) {
				int t = A;
				A = C;
				C = t;
				t = B;
				B = D;
				D = t;
			}
			if (B == D) {
				for (int i = A; i <= C; i++) addPoint(i, B, c);
				return;
			}
			if (A == C) {
				int min = B, max = D;
				if (D < B) {
					min = D;
					max = B;
				}
				for (int i = min; i <= max; i++) addPoint(A, i, c);
				return;
			}
			if (abs(D - B) < abs(C - A)) {
				int dx = C - A, dy = abs(D - B), yi = (D < B) ? -1 : 1, D = 2 * dy - dx, y = B;
				for (int x = A; x <= C; x++) {
					addPoint(x, y, c);
					if(D > 0) {
						y += yi;
						D -= 2 * dx;
					}
					D += 2 * dy;
				}
			}
			else {
				int dx = abs(A - C), dy = abs(B - D), xi = B > D ? ((A < C) ? -1 : 1) : ((A < C) ? 1 : -1), di = 2 * dx - dy, x = B > D ? C : A;
				for (int y = min(D, B); y <= max(D, B); y++) {
					addPoint(x, y, c);
					if (di > 0) {
						x += xi;
						di -= 2 * dy;
					}
					di += 2 * dx;
				}
			}
		}
	private:
		char platno[HEIGHT][WIDTH + 1];
};
int main(int argc, char** argv) {
	Plot plt;
	int n = (argc - 1) ? atoi(argv[1]) : 1;
	float length = 150, mass = 10, *O1 = new float[n], *O2 = new float[n], *w1 = new float[n](), *w2 = new float[n](), g = 9.81f;
	std::fill_n(O2, n, 3.14159265358979323846);
	if (n % 2) for (int i = 0; i < n; i++) O1[i] = O2[i] * (0.5f + 2 * i / (float) n); // symmetrical horizontally (initially)
	else for (int i = 0; i < n; i++) O1[i] = O2[i] * (1 + (2 * i + 1) / (float) n); // Symmetric vertically (initially)
	ShowWindow(GetConsoleWindow(), 3);
	while (true) {
		int i = 0;
		while (i++ < 1 << (3 << 3)); // wait
		for (int i = 0; i < n; i++) {
			w1[i] += (-mass * (3 * g * sin(O1[i]) + g * sin(O1[i] - 2 * O2[i]) + 2 * sin(O1[i] - O2[i]) * (w2[i] * w2[i] * length + w1[i] * w1[i] * length * cos(O1[i] - O2[i]))) / (length * mass * 10 * (3 - cos(2 * O1[i] - 2 * O2[i]))));
			w2[i] += (2 * sin(O1[i] - O2[i]) * (w1[i] * w1[i] * length + g * cos(O1[i]) + w2[i] * w2[i] * length * cos(O1[i] - O2[i]) * 0.5f) / length / (3 - cos(2 * O1[i] - 2 * O2[i]))) / 5;
			O1[i] += w1[i] / 10.0f;
			O2[i] += w2[i] / 10.0f;
		}
		plt.zero();
		for (int i = 0; i < n; i++) {
			int x1 = WIDTH / 2 + sin(O1[i]) * length / 8 + 0.5f;
			int y1 = cos(O1[i]) * length / 16 + HEIGHT / 2 + 0.5f;
			int x2 = x1 + sin(O2[i]) * length / 8 + 0.5f;
			int y2 = y1 + cos(O2[i]) * length / 16 + 0.5f;
			plt.addLine(WIDTH / 2, HEIGHT / 2, x1, y1, '.');
			plt.addLine(x1, y1, x2, y2, '.');
			plt.addPoint(x2, y2, 'O');
		}
		plt.print();
	}
	return 0;
}
