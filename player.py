from curses import wrapper
from time import sleep
from subprocess import Popen
def play(window):
	for l in open("./badapple.txt"):
		if l[0] == 'R': window.move(0,0)
		else:
			window.addstr(l)
			window.refresh()
i = input("Would you like to use vlc to see the original video with sound in the meanwhile? (y\\n)\nIf you do please install it (sudo apt-get install vlc -y)\n").lower()
while True:
	if i in ["y", "n"]: break
	i = input("Please write either \"y\" or \"n\" ")
if i == "y": pid = Popen(["cvlc", "video.mp4"], stdout=-1, stderr=-1)
sleep(2)
try: wrapper(play)
except Exception as e:
	if i== "y": pid.terminate()
	print(e)
