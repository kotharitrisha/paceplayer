import os 
from time import sleep 
import signal 
import math
import soundcloud
import getpass
from random import choice

#Add credentials
#username_input = raw_input("Type in your SoundCloud username: ")
#password_input = getpass.getpass()


#Create SoundCloud client
client = soundcloud.Client(client_id='8b66c70b1ab4134dae1405e08f390562',
                           client_secret='8dd0a0cc4cfc1cb60d71e6b9e31329a3',
                           username = "kotharitrisha@gmail.com",
                           password = "teamsonic"
#                           username = username_input,
#                           password = password_input
                           )


#Open text file and read it                                                                                                                                      
file = open ("/dev/ttyO2", "r")

#mode = int (raw_input("Enter mode (1 for program, 0 for automatic: "))
pace = ""

#count = 0
prev = 0
v = 0
mode = 0
volume = 100
cmd = ""
step_count = 0
flag = 0
#noise = int (open ("/sys/devices/platform/omap/tsc/ain1", "r").read())
#print noise

print mode
#if mode == 1:
#	target = int(raw_input("Enter your target speed: "))
#	print target

os.system("echo 1 > /sys/class/gpio/gpio46/value")
os.system("echo 1 > /sys/class/gpio/gpio26/value")
os.system("echo 1 > /sys/class/gpio/gpio47/value")

while (1):
        x = file.readline()
        print x
	if (x == "P\n"):
		mode = int (file.readline())
		print "You have chosen mode ", mode
        else:
		a = x.split(' ')
		step_count = int(a[1])
		v = int(a[0])
	# mplayer options filename < /dev/null &

#	v = int(raw_input("Put v: "))
	if (mode > 0):
		if (mode == 1):
			target = 110
		elif (mode == 2):
			target = 150
		else:
			target = 180

#	print "current velocoity is ", v
	
#	if (noise > 4000):
#		volume = 100
#	elif (noise < 3900):
#		volume = 80
#	else:
#		 volume = 50
#	print volume
	
	os.system("echo 1 > /sys/class/gpio/gpio46/value")
	os.system("echo 1 > /sys/class/gpio/gpio26/value")
	os.system("echo 1 > /sys/class/gpio/gpio47/value")
        
	#TOO FAST: PIN 24

# If it is a mode and the speed is greater than the target mode 
	if ((mode > 0) and ((v - target) > 20)):
                check = open ("yo.txt", "w")
                check.write( "Running too fast \n")
	       	os.system("echo 0 > /sys/class/gpio/gpio46/value")
		os.system("echo 1 > /sys/class/gpio/gpio47/value")
		os.system("echo 1 > /sys/class/gpio/gpio26/value")
	        print "You are running too fast"
	
	#TOO SLOW: PIN 22
	if ((mode > 0) and ((target - v) > 20)):
                check = open ("yo.txt", "w")
                check.write( "Running too slow \n")
                os.system("echo 0 > /sys/class/gpio/gpio47/value")
		os.system("echo 1 > /sys/class/gpio/gpio46/value")
		os.system("echo 1 > /sys/class/gpio/gpio26/value")
                print "You are running too slow"
	
	#JUST RIGHT: PIN 23
	if ((mode > 0) and (math.fabs(target - v) <= 20)):
                check = open ("yo.txt", "w")  
                check.write( "Running just right \n")
                os.system("echo 0 > /sys/class/gpio/gpio26/value")
		os.system("echo 1 > /sys/class/gpio/gpio47/value")
		os.system("echo 1 > /sys/class/gpio/gpio46/value")
                print "You are running just right"

#	track = "Something"
#	track.title = "Anything" 
#	track.stream_url = "https://ec-media.soundcloud.com/bn8MMyi7isdp.128.mp3?ff61182e3c2ecefa438cd02102d0e385713f0c1faf3b0339595665fc0603ea17f31dd911d214577888faae0c55acfe354ba1318e189421cef283c876993c10d8fa4d71abd6&AWSAccessKeyId=AKIAJ4IAZE5EOI7PA7VQ&Expires=1366961364&Signature=i47qcF6xYas82AJcbAZcfWMEKjM%3D"
 

	if (v - prev > 15):
 		if (v < 110):
			print "V is less than 110" 
			tracks = client.get('/tracks', bpm={'to': 110}, limit = 5)
		elif (v >=110 and v < 130):
			print "V is 110 - 130"
			tracks = client.get('/tracks', bpm={'to': 120, 'from': 110}, limit = 5)
		elif (v >= 130 and v < 145):
			print "V is 130 - 145"
			tracks = client.get('/tracks', bpm={'to': 145, 'from': 130}, limit = 5)
                elif (v >= 145 and v < 160):
                        print "V is 145 - 160"
			tracks = client.get('/tracks', bpm={'to': 160, 'from': 145}, limit = 5)
                elif (v >= 160 and v < 175):
                        print "V is 160 - 175"
			tracks = client.get('/tracks', bpm={'to': 175, 'from': 160}, limit = 5)
		elif (v >=170):
			print "V is greater than 170"
			tracks = client.get('/tracks', bpm={'from': 170}, limit = 5)

#		if (len(tracks) >0):
		track = choice(tracks)
#		else:
#			continue
 		print "recognized faster pace"
		song = track.title
		url = client.get(track.stream_url, allow_redirects =False).location
		if not (flag == 0):
			os.system("rm x.mp3")
		else:
			flag = 1
		cmd = "wget -q --output-document=x.mp3 " + url 
		os.system("ps -C mplayer -o pid=|xargs kill -9")
		os.system(cmd)
		os.system("mplayer x.mp3 < /dev/null &")
		prev = v
		speed = .06*v * .6
		final_string = "sudo BB_LCD/stlcd " + str(speed) + " " + str(step_count) + " "
                total = final_string + song
#                print total
                os.system(total)
	
	if (prev - v> 15):
                print "recognized slower pace"
		song = "slower"
                if (v < 110):
                        print "V is less than 110"
                	tracks = client.get('/tracks', bpm={'to': 110}, limit = 5)        
                elif (v >=110 and v < 130):
                        print "V is 110 - 130"
			tracks = client.get('/tracks', bpm={'to': 130, 'from' : 110}, limit = 5)
		elif (v>=130 and v < 145):
			print "V is 130 - 145"
			tracks = client.get('/tracks', bpm={'to': 145, 'from': 130}, limit = 5)
                elif (v >= 145 and v < 160):
                        print "V is 145 - 160"
			tracks = client.get('/tracks', bpm={'to': 160, 'from': 145}, limit = 5)
                elif (v >= 160 and v < 175):
                        print "V is 160 - 175"
			tracks = client.get('/tracks', bpm={'to': 175, 'from': 160}, limit = 5)
                elif (v >=170):
                        print "V is greater than 170"
                        tracks = client.get('/tracks', bpm={'from' : 170}, limit = 5)

#		if (len(tracks) >0):
                track = choice(tracks)
#                else:
#                        continue 
		song = track.title
                url = client.get(track.stream_url, allow_redirects =False).location
		os.system("rm x.mp3")
                cmd = "wget -q --output-document=x.mp3 " + url  
                os.system("ps -C mplayer -o pid=|xargs kill -9")
                os.system(cmd)
		os.system("mplayer x.mp3 < /dev/null &")
		prev = v
		speed = .06*v*.6
		final_string = "sudo BB_LCD/stlcd "+ str(prev)+ " "+ str(step_count) + " "
                total = final_string + song
#                print total
                os.system(total)


