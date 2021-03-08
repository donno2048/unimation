#include <string.h>
#include <math.h>
#include <stdint.h>
char** color_map = NULL;
#define BUFSZ 1760
double freq = 0.01;
int pos_mod = 0;
void write_color_by_offset(int fd, char* data, int size);
uint32_t color_by_offset(int i) {
	return 
		((uint8_t)(sin(freq * i)                 * 127 + 128) << 24) |
		((uint8_t)(sin(freq * i + 3.141 / 3 * 2) * 127 + 128) << 16) |
		((uint8_t)(sin(freq * i + 3.141 / 3 * 4) * 127 + 128) <<  8) |
		((0x00)                                               <<  0);
}
int main() {
  for(int i = 1; ; i++) {
    double d = fmod(freq * i + freq / 10, 3.141 * 2);
    if(d < freq / 5) {
      pos_mod = i;	
      break;
    }
  } 
  color_map = (char**)malloc(pos_mod * sizeof(char*));
  for(int i = 0; i < pos_mod; i++) {
    color_map[i] = (char*)malloc(21);
    uint32_t color = color_by_offset(i);
    int esc_len;
      sprintf(color_map[i],
        "\033[38;2;%u;%u;%um%n",
      (color & 0xff000000) >> 24,
      (color & 0x00ff0000) >> 16,
      (color & 0x0000ff00) >>  8,
      &esc_len);
  }
  float A=0;
  float B=0;
  float i;
  float j;
  float z[1760];
  char  b[1760];
  int color_idx = 0; 
  int color_start = 0;
  printf("\x1b[2J");
  for (;;) {
    memset(b, 32, 1760);
    memset(z, 0, 7040);
    for (j = 0; 6.28 > j; j += 0.07) {
      for (i = 0; 6.28 > i; i += 0.02) {
        float c = sin(i);
        float d = cos(j);
        float e = sin(A);
        float f = sin(j);
        float g = cos(A);
        float h = d + 2;
        float D = 1 / (c * h * e + f * g + 5);
        float l = cos(i);
        float m = cos(B);
        float n = sin(B);
        float t = c * h * g - f * e;
        int x = 40 + 30 * D * (l * h * m - t * n);
        int y = 12 + 15 * D * (l * h * n + t * m);
        int o = x + 80 * y;
        int N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
        if (22 > y && y > 0 && x > 0 && 80 > x && D > z[o]) {
          z[o] = D;
          b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
        }
      }
    }
    printf("\x1b[H");
    color_start = (color_start >= pos_mod - 1 ) ? 0 : color_start + 1;
    color_idx = color_start;
    for (int k = 0; k < 1761; k++) {
      printf("%s", color_map[color_idx]);
      color_idx = (color_idx >= pos_mod - 1) ? 0 : color_idx + 1;
      putchar(k % 80 != 0 ? b[k]: '\n');
    }
 
    A += 0.04;
    B += 0.02;
  }
}
