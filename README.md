# A Flux implementation for QML

[![Build Status](https://travis-ci.org/benlau/quickflux.svg?branch=master)](https://travis-ci.org/benlau/quickflux)
[![Build status](https://ci.appveyor.com/api/projects/status/hnwf8vg4o778hifq?svg=true)](https://ci.appveyor.com/project/benlau/quickflux)

QuickFlux is an implementation of Flux Application Architecture Framework from Facebook.
It turns your QML application into a more modern and structured way.

Features
========

 1. Dispatcher
  1. A singleton Message Dispatcher / Message Queue per QML Engine.
  2. Avoid out-of-order message processing - It guarantees the order of messages are arrived in sequence to listeners (First come first served)
 2. Middleware - May inject and modify action before dispatching to the Store components
 3. Hydration - Serialize or Deserialize Store component into / from a JSON

Concept and Motivation
======================

Generally speaking, you should avoid creating a big QML file. 
Break down into smaller piece of files is more readable and reusable. 
But it may not true due to event/signal propagation.
Managing code dependence is very troublesome.
( See [reason](https://medium.com/@benlaud/action-dispatcher-design-pattern-for-qml-c350b1d2a7e7#.2dpjhcpt1) )

Using Action and Dispatcher is a solution for this problem
With a global AppDispatcher for communication between components. 
It breaks the dependence and simplify your code by taking out unnecessary signal binding. 

This project provides an implementation of dispatcher in QML for user to get started.

What is Flux and How to use in QML Application? 
===============================================

What is Flux?

 1. [A cartoon guide to Flux — Code Cartoons — Medium](https://medium.com/code-cartoons/a-cartoon-guide-to-flux-6157355ab207)

How to use it in a QML Application?

 1. [Revised QML Application Architecture Guide with Flux](https://medium.com/e-fever/revised-qml-application-architecture-guide-with-flux-a1de143fe13e)
 2. [Action-Dispatcher Design Pattern for QML — Medium](https://medium.com/@benlaud/action-dispatcher-design-pattern-for-qml-c350b1d2a7e7#.2dpjhcpt1)
 3. [What is new in Quick Flux 1.1?](https://medium.com/@benlaud/what-is-new-in-quick-flux-1-1-ad090a31fa10)

An example program is available at: [quickflux/examples/todo](https://github.com/benlau/quickflux/tree/master/examples/todo)

![Quick Flux Application Architecture 1.png (1261×446)](https://raw.githubusercontent.com/benlau/junkcode/master/docs/Quick%20Flux%20Application%20Architecture%201.png)

Installation Instruction (with qpm)
===================================

For user who are already using qpm from [qpm.io](https://qpm.io)

 1) Run `qpm install`

```
$ qpm install com.github.benlau.quickflux
```

 2) Include vendor/vendor.pri in your .pro file

You may skip this step if you are already using qpm.

```
include(vendor/vendor.pri)
```

 3) Add import statement in your QML file

```
import QuickFlux 1.0
```

Installation Instruction (without qpm)
======================================

 1) Clone this repository or download release to a folder within your source tree.

 2) Add this line to your profile file(.pro):

    include(quickflux/quickflux.pri) # You should modify the path by yourself

 3) Add import statement in your QML file

```
import QuickFlux 1.0
```

Installation Instruction (with CMake)
=====================================

Add QuickFlux as an external project in your CMakeLists.txt:
 
```
include(ExternalProject)

ExternalProject_Add(QuickFlux
	PREFIX "${PROJECT_BINARY_DIR}/QuickFlux-build"
	GIT_REPOSITORY "https://github.com/benlau/quickflux.git"
	CMAKE_ARGS "-DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}"
                    "-DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/QuickFlux"
                    "-DCMAKE_INSTALL_LIBDIR=${PROJECT_BINARY_DIR}/QuickFlux/lib"
                    "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
)

link_directories("${PROJECT_BINARY_DIR}/QuickFlux/lib")
include_directories("${PROJECT_BINARY_DIR}/QuickFlux/include/quickflux")

...

add_dependencies(<your target> QuickFlux)
target_link_libraries(<your target>	debug quickfluxd optimized quickflux)
```

Instead of using GIT_REPOSITORY which clones a remote repository, you can use SOURCE_DIR with a local path which contains the project sources.

You may need to add ```"-DCMAKE_PREFIX_PATH=<your Qt install path>"``` to the CMAKE_ARGS on some platforms.

Also make sure that your cpp file actually calls ```registerQuickFluxQmlTypes();```

If you are facing linking problems with QML, make sure to link the Qt5:: modules after linking quickflux.

Reference: [examples/todo/CMakeLists.txt](https://github.com/benlau/quickflux/blob/master/examples/todo/CMakeLists.txt)

Example Projects
================

There are few example programs available at examples folder:

[Examples](https://github.com/benlau/quickflux/tree/master/examples)

Qt Creator Project Wizard
=========================

If you think that it is very troublesome to have lots of copy & paste of source code in creating a new Flux project, you may install the following project wizard in your Qt Creator. That is displayed in the New Dialog that opens when you choose File > New File or Project.

 1. [Flux Application (Full)](https://github.com/e-fever/e-fever-codebase/tree/master/Flux-Application-Full)
 2. [Flux Application (Minimal)](https://github.com/e-fever/e-fever-codebase/tree/master/Flux-Application-Minimal)

Class Reference
---------------

[QuickFlux 1.1 Class Reference](http://benlau.github.io/quickflux/)

Wish List / Under Development
---------

 1. More example code
 2. Better C++ integration
  1. Code generator to convert KeyTable to C++ friendly class.
  
FAQ
===

Q1
---
Why use AppDispatcher instead of listening from AppActions directly?

Example:
```
import QtQuick 2.2
pragma Singleton

QtObject {

  signal openItem(string id);

  signal removeItem(string id);

}
```

ANS: Of course you could implement in this way. However, AppDispatcher offer two advantages over that approach:

1) Avoid out-of-order message processing.

Signal emission in QML is , in fact, on last come, first served basis. If you emit another signal while in a callback, it will process the latest signal immediately. That means the sequence of message processing will be out-of-order.
However, AppDispatcher will not dispatch a new message while in a callback. It will be placed in a queue until all of the listeners received the current message. So that the order of message is guaranteed to arrive in sequence.

2) Setup dependence between stores.

Quoted from [Why We Need a Dispatcher](https://facebook.github.io/react/blog/2014/07/30/flux-actions-and-the-dispatcher.html#why-we-need-a-dispatcher):

    As an application grows, dependencies across different stores are a near certainty. 
    Store A will inevitably need Store B to update itself first, so that Store A can know 
    how to update itself. We need the dispatcher to be able to invoke the callback for Store B, 
    and finish that callback, before moving forward with Store A. To declaratively assert this 
    dependency, a store needs to be able to say to the dispatcher, "I need to wait for Store B 
    to finish processing this action." The dispatcher provides this functionality through its 
    waitFor() method."

AppListener supports “waitFor” property to setup dependencies on another AppListener. 
It won't process the emitted signal and wait until the other listener received the message.
Therefore, you could control the order of message delivery among stores.

Application Gallery
===================

A collection of application using Quick Flux

 1. [Lanto - An Intuitive batch Photo Resizer](http://e-fever.net/lanto/)

Related Projects
=================

**Libaries**

 1. [benlau/quickpromise](https://github.com/benlau/quickpromise) - Promise library for QML
 1. [benlau/quickcross](https://github.com/benlau/quickcross) - QML Cross Platform Utility Library
 1. [benlau/qsyncable](https://github.com/benlau/qsyncable) - Synchronize data between models
 1. [benlau/testable](https://github.com/benlau/testable) - QML Unit Test Utilities
 1. [benlau/biginteger](https://github.com/benlau/biginteger) - QML BigInteger library
 1. [benlau/qtci](https://github.com/benlau/qtci) -  A set of scripts to install Qt in Linux command line environment (e.g travis)
 1. [benlau/quickfuture](https://github.com/benlau/quickfuture) - Using QFuture in QML
 1. [benlau/fontawesome.pri](https://github.com/benlau/fontawesome.pri) - Using FontAwesome in QML
 1. [benlau/quickandroid](https://github.com/benlau/quickandroid) - QML Material Design Component and Support Library for Android

**Tools**

 1. [SparkQML](https://github.com/benlau/sparkqml) - QML Document Viewer for State and Transition Preview
