Hey Linds,


  I just wanted to put a little info here to get you started.


First, you might want to check out my GesturePasswords repo, because a lot of the code that I am going to use in the first parts of this are from that project. The code is not too well documented, but with the readme and project writeup you should be able to make sense of what is going on.

To get everything set up right:

1. Install opencv:
	I tried a bunch of different install methods but found port was the most reliable:
	$  sudo port install opencv @2.4.4_1+universal

2. Install QT Creator:
	-Go to: http://qt-project.org/downloads
	-I am using QT 4.8, so make sure that you install the 4.8 binary
	-Then install QT Creator from a binary

After that everything should be set up right, and you shoud be able to open the project by simply opening the *.pro file in the GestureTrainer directory.

Once you get everything rolling, we should meet up and I will give you a brief overview of what is going in my code, and give you a good opencv book as a reference.

You also probably want to bookmark these two APIs that are fairly in depth:

OPENCV:	http://docs.opencv.org/
QT:		http://qt-project.org/doc/  (their search feature is good)

