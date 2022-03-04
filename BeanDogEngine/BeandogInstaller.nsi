#Configuration Exam

#Compile this script in the Deployment Folder
#This program will create a BeandogInstaller.exe file
#Running the installer will copy the deployment folder contents to a desktop folder

#Running the included uninstaller will remove the folder on the desktop

# the name of the installer
OutFile "BeandogInstaller.exe"

# directory of the installer
InstallDir "$DESKTOP\6019Exam"

# first and default section
Section

# the path of our installer
SetOutPath $INSTDIR

# note for future: use /r flag on a folder for a recursive copy on folder
# the file we wish to copy pt1
File BeanDogEngine.exe

File /nonfatal /a /r "*.lib"

File /nonfatal /a /r "*.dll"

File /nonfatal /a /r "assets"

# declare the uninstaller
WriteUninstaller "$INSTDIR\BeandogUninstaller.exe"

# end the default section
SectionEnd


# start the uninstall section
Section "Uninstall"

# remove the directory
RMDir /r $INSTDIR

# end the uninstaller section
SectionEnd