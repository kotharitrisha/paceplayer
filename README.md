paceplayer
==========

The Automated Running Experience


This project aims to create a system to provide the automated running experience. Below are some key feature of the project:

Pace detection and change of music as per the BPM detected

Introduction of a program mode component for track selection and feedback generation.

Feedback on an LCD screen.

Integration with Sound Cloud

Volume modulation



Below are the files that contain most of the changes implemented:

play_pro_2.py ==> Primary Python file which reads data from the serial input TTYO2, chooses music on the basis of the SoundCloud tracks received, changes volume on the basis of the data received from the Firefly sensor, and send the information to the LCD screen. Determines the feedback in program mode, and switches the back light on/off.

stlcd.c; glcd.c ==> Displays the data on the LCD screen.

gpio.sh ==> Shell script to enable all the respective GPIO pins

command_sudo.sh ==> Shell script to enable serial communication
