import threading
import time
import sys
import msvcrt


sys.path.append(".\\x64\\Release")
import videoplayer

def render_video():
	while True:
		time.sleep(1) # this should be set to 0.001 when not testing
		# play the video with py_object
		print("Video playing")


print("The following commands are available:")
print("P: Play or Pause the video")
print("A: Fast forward the video (x2). Press again to go back to normal rate")
print("R: Rewinds the video to the beginning and play the video in respect to reading status")
print("Q: Quit the program\n")
	
player = '' #= videoplayer.init()

threading1 = threading.Thread(target=render_video)
threading1.daemon = True
threading1.start()

while True:
	valid = False
	if msvcrt.kbhit():
		key = (msvcrt.getch()).decode("utf-8").upper()
		if key == 'P':
			# player.play()
			print("Pressed P")
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