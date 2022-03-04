#INFO 6019 & 6025 Exam

Build Steps:
	1. Unzip the folder
	2. Open the Visual Studio Solution
	3. Build in either release or debug x64 version
	4. Have fun
	
Deployment Folder Instructions:
	1. Build Project in Release x64
	2. Go to the Deployment folder in the project files
	3. Compile included NSI file in Deployment Fodler
	4. Run installer to make a folder on your desktop called 6019Exam

Controls:
	w - forward
	s - backward
	a - left
	d - right
	q - up
	e - down
	1 - Warp the the Start
	Left Arrow - Move Cannon Left
	Right Arrow - Move Cannon Right
	Up Arrow - Move Cannon Up
	Down Arrow - Move Cannon Down
	Click GUI Buttons - Spawn in Various boats
	
Major mark groups
	Physics:
		For the physics portion i have implamented sphere plane and sphere sphere collisions.
		I have implamented the rod constraints to make the "boats", and the buoyancy force to make the boats float
		The cannon allows the user to interact with the boats by shooting them and watching the connon balls bounce 
		off the boats and eachother
	Configeration:
		For the configuration part i have all static objects in the scene, lighting, and camera position in a xml loader class
		I also have the water height, cannon pitch and yaw, as well as movement speed in a seperate xml loader class
		I have a deployment folder being made only on release mode that includes an nsi script to create the installer
		The installer creates a folder on the desktop with all the necessary files to run the program as well as an uninstaller
		that will remove all traces of the program from your desktop
		
