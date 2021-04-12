Those are some unicode animations, donut2.c is for Windows and donut.c is for Linux, they are not the same

When compiling the c files use the `-lm` flag

The python file is compatible with python3 and python2 but you have to maximize to terminal when running it.

If you get `_curses.error: addwstr() returned ERR` try decreasing the font size.

To create the _badapple.txt_ for _player.py_ I used this script:

```py
try: from PIL.Image import open
except: from Image import open
from sys import stdout
from cv2 import VideoCapture, imwrite, resize
th = 127
def I2T(File):
	im = open(File)
	(w, h) = im.size
	mim = im.convert("1")
	data = list(mim.getdata())
	counter = 0
	field = True
	for pixel in data:
		if field:
			if pixel > th: stdout.write("*")
			else: stdout.write(" ")
		counter = counter + 1
		if counter >= w:
			counter = 0
			if field: stdout.write("\n")
			field = not field
vidcap = VideoCapture('./video.mp4') # This is the video shown
success, image = vidcap.read()
while success:
	imwrite(".jpg", resize(image, (144, 108), interpolation = 3))
	I2T(".jpg")
	success, image = vidcap.read()
	print("R")
```

(You need to `pip3 install opencv-python Pillow` in order to do that)

I used this script like so:

```sh
python3 main.py > badapple.txt
```
