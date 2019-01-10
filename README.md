# Understanding a [PLEXIL](http://plexil.sourceforge.net/wiki/index.php/Main_Page) based application using a barebones app

## About the app functionality
TickTock is a barebones PLEXIL (version 4.5.0RC2) based app that attempts to throw some light on the bells and whistles of a PLEXIL based app.
The TickTock app has no functionality whatsoever. It is an app that prints out strings "Tick" and "Tock" 25 times and exits.


## Why this app?
There are several example apps present under the "examples" directory of PLEXIL distribution. These apps are a great resource,
but I personally found them to be too complex.
More importantly, the example apps only cover the "pure PLEXIL" side of things. That is, if you're going by the examples, you
will have to use the PLEXIL executive that comes with standard PLEXIL distribution.
The SampleDrive app in the examples folder has some interfacing with C++ code, but the "simulator" part of the code is still pure PLEXIL.
(More on this below)
A standalone binary can be built by statically or dynamically linking to PLEXIL standard libraries,
and that is what I wanted to demonstrate.

## Concept
Conceptually, any PLEXIL app will have three parts:

1. The Plan:
This is the .ple (or .plx file after compilation by PLEXIL compiler). The plan is nothing but a sequence of stimuli to some code that is interfaced with PLEXIL.
In our case, the plan file is TickTock.ple. Look into this file and you will see that, this plan has only one action. This action has a while loop that sends Tick and Tock commands to an external system for 25 times.
There is also an end condition to this action, which means, this action will end after the end condition is met.

2. The "External" system:
The word external here, is with respect to PLEXIL. PLEXIL is responsible for doing a few things like reading the plan, loading it and executing it. The plan calls things or looks up things from this "external" system.
In our case, this "external" system is nothing but a few meaningless APIs. Init(), Tick() and Tock(). These are all coded in a .cpp file: TickTockController.cpp.
This file also has some "get" methods, for PLEXIL to look up internal properties of this TickTockController.

3. The "mediation" or the Adapter:
This is surprisingly the most involved part of the application, where the developer is responsible for making sure that the PLEXIL side can talk to the "external" system. This is coded in TickTockAdapter.cpp.
There are two requirements for adapter to interface PLEXIL and the "external" system.
- Specify the name of the adapter in the interface-config.xml file. With this file present there, PLEXIL will know to use it.
- Implement InterfaceAdapter interface. This is a standard interface that comes with PLEXIL distribution and PLEXIL will call the APIs that developer will override in TickTockAdapter class.

## Writing your own application:
As the PLEXIL documentation mentions, we need to write our own application, if we prefer an executable which has all interfaces pre-loaded and or executive needs to run in a particular threading environment.
To demonstrate how this works, I have written main.cpp. This main.cpp has boilerplate code that is present in the standard UniversalExec.cpp that comes with PLEXIL distribution.
The only thing that is changed in here, is the name of the adapter.
The code to handle command line arguments etc. is in fact and not changed from UniversalExec.cpp

In the CMakeLists.txt, the executable TickTockController is created from main.cpp and TickTockAdapter.cpp.


## How things work together:
- As mentioned earlier, main.cpp is the entry point of TickTockController PLEXIL app. This is the file that is responsible for reading command line arguments, reading interface-config.xml etc. and starting the execution of the PLEXIL plan.
- In the plan, I have used two PLEXIL concepts to demonstrate how things are wired together.
  1. Lookups
  2. Commands

1. Lookups:
With Lookups, you can inspect the internal state of an external system. Think about it as calling getter function on a class instance. In fact, with the help of the adapter, that is exactly what is done in this example.
When Lookup(TickCounter)  or Lookup(TockCounter) etc. is called in the PLEXIL plan, PLEXIL will call the standard interface function, lookupNow, on the adapter.
It is developer's responsibility to provide an implementation of this function that returns the correct value of the state PLEXIL is looking for. For example, in our case, we call ```Fetch```, which eventually calls ```GetTickCounter()``` etc. This mapping of what lookup should result in what getter function call, is entirely up to the developer.

2. Commands:
Commands are like remote procedure calls. A PLEXIL plan can call an arbitrary command on an external system. The adapter will take care of calling an appropriate function on the external system, if it is supported.
In order to execute a command, PLEXIL will call a standard adapter interface method ```executeCommand```. It is up to the developer to map this command to a function call to the external system.
In our case, for example, when a ```Tick()``` or ```Tock()``` command is called, PLEXIL will call ```executeCommand(...)``` on TickTockAdapter. Based on the name of the command, the logic in the ```executeCommand``` calls one of the methods on TickTockController.cpp


## Steps to run the application:
#### Prerequisites:
- CMake
- g++ compiler (standard distribution that comes with ubuntu will suffice)
- PLEXIL installed and configured (See Plexil documentation for this)

#### Running the application:
1. Go to src/plans and generate the .plx file from the .ple plan

```
plexilc TickTockPlan.ple
```

2. Compile the application:

```
mkdir ticktock-plexil-ticktock/build
cd build
cmake ../src
make
```

3. Copy the generated TickTockController and TickTockPlan.plx to convenient locations and Run the PLEXIL plan. e.g.

```
./TickTockController -p plans/TickTockPlan.plx
```


## Afterthoughts:
PLEXIL is maybe a choice when you really really want to decouple the procedure from the functionality. In this case, the functionality is the ability to print tick-tock and the ability to look-up how many times tick-tock calls were made. The procedure is calling tick-tock in sequence for 25 times and stopping the execution when its all done.
That being said, the learning curve, the increased footprint due to inclusion of PLEXIL library, the hassle of writing a PLEXIL plan and converting it to an XML file and writing an Adapter is a bit too much to pay for just decoupling.
We may very well separate out the functionality part in a shared library and write an executable state machine all in C++ and get better performance and reduced learning curve.
