import threading
import time
import sys
import msvcrt


sys.path.append(".\\x64\\Release")
import videoplayer

media_player = videoplayer.init()

print("The following commands are available:")
print("P: Play or Pause the video")
print("A: Fast forward the video (x2). Press again to go back to normal rate")
print("R: Rewinds the video to the beginning and play the video in respect to reading status")
print("Q: Quit the program\n")

while True:
	if msvcrt.kbhit():
		key = (msvcrt.getch()).decode("utf-8").upper()
		if key == 'P':
			print("Pressed P")
			media_player = videoplayer.play_video(media_player)
		elif key == 'A':
			# player.fastforward()
			print("Pressed A")
			videoplayer.fastforward_video(media_player)
		elif key == 'R':
			# player.rewind()
			print("Pressed R")
		elif key == 'Q':
			print("Quitting...")
			sys.exit()
