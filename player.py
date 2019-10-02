import threading
import time
import sys
import msvcrt


sys.path.append(".\\x64\\Release")
import videoplayer

# media_player = videoplayer.init()

valid_commands = ('P', 'A', 'R', 'Q')

print("The following commands are available:")
print("P: Play or Pause the video")
print("A: Fast forward the video (x2). Press again to go back to normal rate")
print("R: Rewinds the video to the beginning and play the video in respect to reading status")
print("Q: Quit the program\n")

while True:
	if msvcrt.kbhit():
		key = (msvcrt.getch()).decode("utf-8").upper()
		if key in valid_commands:
			videoplayer.vp_module(key)	
			if key == 'Q':
				print("Quitting...")
				sys.exit()
