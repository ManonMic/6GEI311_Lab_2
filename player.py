import threading
import time
import sys
import msvcrt


sys.path.append(".\\x64\\Release")
import videoplayer

media_player = videoplayer.init()

def render_video():
	while True:
		time.sleep(0.001) # this should be set to 0.001 when not testing


print("The following commands are available:")
print("P: Play or Pause the video")
print("A: Fast forward the video (x2). Press again to go back to normal rate")
print("R: Rewinds the video to the beginning and play the video in respect to reading status")
print("Q: Quit the program\n")

threading1 = threading.Thread(target=render_video)
threading1.daemon = True
threading1.start()

while True:
	valid = False
	if msvcrt.kbhit():
		key = (msvcrt.getch()).decode("utf-8").upper()
		if key == 'P':
			print("Pressed P")
			valid = videoplayer.play_video(media_player)
			print("valid " + str(valid))
			pass
		elif key == 'A':
			# player.fastforward()
			print("Pressed A")
			pass
		elif key == 'R':
			# player.rewind()
			print("Pressed R")
			pass
		elif key == 'Q':
			print("Quitting...")
			sys.exit()