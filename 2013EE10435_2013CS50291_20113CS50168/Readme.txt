ScreenSaver Version 1.0  25/01/2015

GENERAL INFORMATION
------------------------------

-	ScreenSaver is an application which simulates ball collisions in an 
	environment which looks like that of a screensaver.
-	It comes along with a GUI which allows the user to okay around with 
	various effects like increasing individual ball speeds, adding/ 
	removing balls, changing the background, etc.
-	A full screen mode has also been incorporated to allow the user to
	enjoy just the visual effects.


INSTALLATION INSTRUCTIONS
-------------------------------

-	The application has been built for Linux and Mac systems containing the
	following c++ libraries : GLUT, GL, GLUI, SDL and the standard 
	libraries. Open the 'ScreenSaver' folder containing all the files 
	in the terminal and run make to run the makefile to compile program. 
	Run using "./ScreenSaver".

GUI Option
————————————————————
- ID of Selected Ball: Wire Cage appears around the selected ball.

- Change Speed Of Selected Ball : To increase/decrease velocity by 10% or stop the ball.		 

- Rotate : Only Available in 3D Mode. Rotates The Box.

- Select Your View : Switch to 2D/3D mode.

- Zoom In/Zoom out : Slider to zoom in/zoom out of the of view.

- Background : Choose different background from 4 pre-defined ones.

- Glass :  Choose different glass textures from 4 pre-defined ones.

- FullScreen Mode : Hide GUI Option to move to full screen mode.
			Press any key to escape.

- Motion : To pause/Unpause motion.

- Exit : Exits the Program
 

DEBUGGING MODE
-------------------------------

-	There is a debugging mode to remove any bugs which might be present in the 
	software. The following Debugging modes/symbols are present:

-	a)	DEBUG_BALL_COL: Displays the final and initial velocity of the colliding 
		balls after each collision.
-	b)	DEBUG_WALL_COL: Whenever a wall collision occurs, displays the ID of the
		colliding ball and that of the wall it collided with.
-	c)	DEBUG_ITER: Displays the Iteration value after every 500 iterations to
		check the continuity of the program.
-	d)	DEBUG_PQUEUE: Displays the top of the Priority Queue, ie, the next
		collision.
-	e)	DEBUG_ALL: Displays the outputs of all the debugging modes together.

-	These can be run using make DEBUG_<symbol>

CONTACT INFORMATION
--------------------------------

For updates and other details contact the following:
-	Akshay Singla: ee1130425@iitd.ac.in
-	Nikhil Chaturvedi: cs5130291@iitd.ac.in
-	Rishabh Sanghi: cs5130168@iitd.ac.in


PRECAUTIONS
————————————————————————
Do not add new images or delete images from the resource folder.
You must replace the file with the same filename and image must be .jpg.
References
--------------------------------
-	http://talkera.org/opengl/opengl-transparency-and-blending/
-	http://www.glprogramming.com/red/index.html
-	For Backgrounds and textures:
	https://images.google.com/?gws_rd=ssl


