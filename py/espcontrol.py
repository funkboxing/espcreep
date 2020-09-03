###################################################
######### OPENCV UDP ESP32CAM BOT CONTROL #########
###################################################
#Simple stream viewer and UDP controls for ESP32 bot(s)
#EXAMPLE UDP COMMANDS:
#"5,4700,4900,0,0"	SET MOTORS WITH RAW PWM
#"8,128,0,0,0"		SET FLASH BRIGHTNESS 0-999999
###################################################
import socket		#---UDP
import time		#---UDP
import cv2		#---CAM
import urllib2		#---CAM\URL
import numpy as np
import sys

#======VARIABLES======
hostip = "192.168.1.66"
udpport = 1234
timer_now = int(round(time.time() * 1000))	#---UDP SEND FREQUENCY
timer_then = int(round(time.time() * 1000))	#---UDP SEND FREQUENCY
timer_limit = 150				#---UDP SEND FREQUENCY
servoRAW = [4700,4700] 				#---Servo States  

smin = 100
smax = 8888
smid = 4700
sstep = 100
stupidLED = [0]

#======UDP OBJECT======
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def checktime():
	global timer_now
	global timer_then
	timer_now = int(round(time.time() * 1000))
	if (timer_now - timer_then) > timer_limit:
		timer_then = int(round(time.time() * 1000))
		return True
	else: return False

def sendudp_cmd(d):
	if d == 0:	#UP\FORWARD
		servoRAW[1] = servoRAW[1] + sstep
		udpstr = "5," + str(servoRAW[0]) + "," + str(servoRAW[1]) + ",0,0"
	if d == 1:	#DOWN\BACK
		servoRAW[1] = servoRAW[1] - sstep
		udpstr = "5," + str(servoRAW[0]) + "," + str(servoRAW[1]) + ",0,0"
	if d == 2:	#LEFT
		servoRAW[0] = servoRAW[0] - sstep
		udpstr = "5," + str(servoRAW[0]) + "," + str(servoRAW[1]) + ",0,0"
	if d == 3:	#RIGHT
		servoRAW[0] = servoRAW[0] + sstep
		udpstr = "5," + str(servoRAW[0]) + "," + str(servoRAW[1]) + ",0,0"
	if d == 4:	#STOP
		servoRAW[0] = smid
		servoRAW[1] = smid
		udpstr = "5," + str(servoRAW[0]) + "," + str(servoRAW[1]) + ",0,0"
	if d == 5:	#LED UP
		stupidLED[0] = stupidLED[0] + 1
		udpstr = "8," + str(stupidLED[0]) + ",0,0,0"
	if d == 6:	#LED DOWN
		stupidLED[0] = stupidLED[0] - 1
		udpstr = "8," + str(stupidLED[0]) + ",0,0,0"
	if checktime:
		sock.sendto(udpstr, (hostip, udpport))
		print("   SENDING UDP: " + udpstr)

#======MAIN======
cv2.CV_LOAD_IMAGE_COLOR = 1
hoststr = 'http://' + hostip
print('Streaming ' + hoststr)

stream=urllib2.urlopen(hoststr)

bytes=''
while True:
	bytes+=stream.read(1024)
	a = bytes.find('\xff\xd8')
	b = bytes.find('\xff\xd9')
	if a!=-1 and b!=-1:
		jpg = bytes[a:b+2]
		bytes= bytes[b+2:]
		i = cv2.imdecode(np.fromstring(jpg, dtype=np.uint8),cv2.CV_LOAD_IMAGE_COLOR)
		i90 = cv2.rotate(i, cv2.ROTATE_90_CLOCKWISE)
		cv2.imshow(hoststr,i90)		
	key = cv2.waitKey(1)
	if key == 27:
		exit(0)
	elif key == ord('w'): sendudp_cmd(0) #UP\FORWARD
	elif key == ord('s'): sendudp_cmd(1) #DOWN\BACK
	elif key == ord('a'): sendudp_cmd(2) #LEFT		
	elif key == ord('d'): sendudp_cmd(3) #RIGHT
	elif key == ord('c'): sendudp_cmd(4) #STOP
	elif key == ord('q'): sendudp_cmd(5) #LED UP
	elif key == ord('e'): sendudp_cmd(6) #LED DOWN

