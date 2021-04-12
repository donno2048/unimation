from curses import wrapper
def play(window):
	for l in open("./badapple.txt"):
		if l[0] == 'R': window.move(0,0)
		else:
			window.addstr(l)
			window.refresh()
wrapper(play)
