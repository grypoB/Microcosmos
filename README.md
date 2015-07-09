#Microcosmos
2D particules simulation with basic physics, black holes and particle generators using OpenGL (GLUI)

Written in C and C++

##Resources
### Course
Programing project for BA2 EPFL : PROGRAMMATION I ET II (MT & EL)
Professor Ronan Boulic
Spring 2015

### Links (hopefully no dead ones :relaxed:)
* Source : http://moodle.epfl.ch/course/view.php?id=6831

* Specs :
  * mirror 1: http://moodle.epfl.ch/pluginfile.php/1555228/mod_resource/content/8/Specifications_2014_15_V1.00.pdf

  * mirror 2: https://drive.google.com/open?id=0BxNo5k5IQRfKZGkxS3RvNkJlbWs

##Compiling
Go to the source folder and use
```
make depend
make
```

If it doesn't work, good luck finding the appropriate librairies :crying_cat_face:.

If you are on MacOS X use the "apple" version :
```
make appleDepend
make apple
```

If you want to tinker arround you might find useful the debug mode:
```bash
make clean      # so all files are recompiled using the DEBUG define
make debug      # linux
make appleDebug # macos-x
```
##Usage
`make` will create an executable which will accept parameters :
```bash
./exec.x [mode, file]
```
There are 5 modes :

1. Error
2. Force
3. Integration
4. Graphic
5. Simulation

The 3 first are command line only, the last two open an OpenGl window. The only interesting mode is Simulation.
If you don't enter anything, the mode will be Simulation and you can then load a file within the GUI.

There are numerous files included in the folders `r1`,`r2`,`r3` and `Otest`. The most interesting ones are the `Otest`, our personal favorite being `O12.txt` (you can find screenshot of it in the folder `report`).
