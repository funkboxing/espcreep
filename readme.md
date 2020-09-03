# espcreep

## ESP32CAM Based Robot Type Thing
http://funkboxing.com

![espcreep](/images/espcreep.jpg)

A cheap and simple robot thing made for whatever purposes a cheap and simple robot might be useful for.
It's not that much fun as it is but the idea is that you could make a 3-4 of these for under $100 and 
it would be a start for a sort of robot programming sandbox. It needs mechanical work for sure, it's barely mobile on a flat surface, but it drives and turns so it's a start.

If you haven't messed with an ESP32CAM before you'll probably want to do a webcam tutorial first to make sure you've got the hang of uploading with the FTDI and all that.

### FEATURES:
* WiFi Camera Stream
* Front Wheel Steering
* Onboard LED
* 3d Printed Body
* Inexpensive, Low Part Count, Simple Assembly
* Simple UDP Controls and Code

### PARTS:
* ESP32CAM
* 9g Hobby Servo (180deg)
* 9g Hobby Servo (Continous)
* 16340 Module and Battery
** https://www.diymore.cc/products/micro-usb-16340-lithium-battery-shield-mobile-power-holder-adapter-for-arduino
* 3D printed parts
* M2 screws and nuts
* Wire
* OPTIONAL - Breakout PCB
** https://oshpark.com/shared_projects/QNPIpnbK
* OPTIONAL - Capacitor 1000uF 6.3V
* OPTIONAL - Silicone or Rubber Bands

### BUILD NOTES:
This is still a work-in-progress all around.
But if you print the parts you should probably be able to figure it out.
The only trick is the 3d printed wheels are worthless unless you coat them with silicone or strap some rubber bands on.
Also there's no front axle or wheel connections yet. I just used coathangar wire and a couple of nuts glued on.
The breakout helps keep things simple, but soldering the connections would work too. Just wire power and ground to the battery 5V pads and the steering servo to 13 and the drive to 14. If you do have the breakout use a 6x female dupont header on the cam side and male pins on the back for the servos and battery. The capacitor is probably redundant to the battery module circuitry, but I wasn't sure so I just added it.

### CODE NOTES:
I've only tested on Ubuntu 18.04 but the python script is simple enough so seems like it should run anywhere you can get the python OpenCV module loaded.
The SSID\PASS is hardcoded in the arduino sketch for now.
The UDP 'protocol' is barely that. You send a string in the format n,x,y,a,b
The arduino sketch parses that out to motor or LED controls
n-command code | x,y-motor\led vals | a,b-for later
command codes are 5 for motors, 8 for LED
"5,4700,4900,0,0"	Sets motor PWMs to 4700 and 4900
"8,128,0,0,0"		Sets flash brightness to 1/2 brightness

The idea is that all the calibration and motor states will be handled by control software on another machine.
These aren't meant to do much onboard processing, just basic mobility and camera stream. My goal is a system that allows
controlling several of these maybe on a little soccer field or in a maze or something.


### FILES:
* ESP32CAM Arduino Sketch
* Python OpenCV Control Script
* STL Files
* Stencil for Breakout PCB
** I can't find them right now but I've shared the OSH Park Project so you can order them there if you want. It just breaks out the power and servo pins and has 2 more power-ground breakouts that could be routed to any other pin. 

### LIBRARIES:
Uh... I kind of forgot, I'll work on it but for now you can just run it, then google the errors until you find what you need.

### FUTURE PLANS:
* Improve 3d parts
* Wire management
* Sort of a 'trolly' charging system with some wires sticking up that can make contact with 5V rails.
* 3rd servo for gripper\lift action
* RGB LED
* Get help... I'm not great at any part of this, just good enough to glue things together. I'd love to have a skilled 3d designer or coder make this better.


