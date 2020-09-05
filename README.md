# Koi
## this is not a release,  this is just for testing features that may/may not be added to koi via pull requests if you want to use something daily use 

## [Koi](https://github.com/baduhai/Koi) 

the original author of koi would back in september as he/she has other stuff to to  mean while here is just for testing stuff until he/she is back 

**Theme scheduling for the KDE Plasma Desktop**

**use the all in one branch 
<p>
  <img alt="profile-dialog" title="Yin & Yang" src="doc/profile_dialog.png">
</p>

This is a TODO list 

- [x] add cursors style 
- [x]  add window decoration // i am having a hard time figuring out how to get all the window decoration on the system 
- [x] most of the methods in utils should be static as they have no relation to the actual class Utils and should be movedinto other classes that it is related to like plasma-style, icons etc.
   // i found a way but not sure if it is optimal tho.
- [ ] some of the classes are now useless it would either need to be deleted or used for other thing like moving some methods from the utils class into there if they are related as the utils class it too large for my liking.
- [x] include button for running scripts
- [ ] make methods modular this will assist in the ability to add more than just light and dark if it is not hard-coded.  //almost done
- [ ] add ability to switch konsole terminal themes 
- [ ] The ui/ux is currently a mess i am no ux/ui guy but i don't really know how to make it any better .... for now
- [ ] clean up the cmakelists directory 
- [ ] add install directory for icons in the cmakelists 
- [x] remove redundant code 
- [x] add ability to run scripts 
- [ ] make the UI better and remove redundant check boxes
- [ ] fix icons not added in the desktop file and in the system
- [ ] choose a formatting standard (currently digging the Microsoft one because is very easy to read
- [ ] separating logic from UI
- [ ] add switching browser styles for (for chrome, chromium and firefox)
- [ ] add support for more languages other than English with i18n/i10n 




## **How to Build** 

NB: the app is built for the KDE desktop environment .

### **Ubuntu Based ** 

##### Dependencies 

`sudo apt update` 

//qtbase5-dev is optional for kubuntu / kde neon 20.04  if it does not build add it back 



`sudo apt install git cmake ninja-build g++ extra-cmake-modules qtbase5-dev`





`sudo apt install  libkf5coreaddons-dev libkf5widgetsaddons-dev libkf5config-dev libkf5package-dev libkf5service-dev`



### Arch Based  

`sudo pacman -Syu` 

`sudo pacman -S cmake gcc extra-cmake-modules ninja` 



// I have not gotten the dependencies for other distros currently will be added later 



### All Distros

`git clone https://github.com/Da-Viper/Koi.git`

`git checkout allInOne` 

`cd Koi && mkdir build && cd build` 

`cmake .. -DCMAKE_BUILD_TYPE=Release -G Ninja`



###### To run the app enter 

from the build folder 

`cd src  && ./koi` 



if you insist on installing the app   go the build folder and run 

`sudo ninja install` 

to uninstall run 

`sudo ninja uninstall`  



then you can delete the directory 

