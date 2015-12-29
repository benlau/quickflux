# Message Dispatcher/Queue for Qt/QML

[![Build Status](https://travis-ci.org/benlau/quickflux.svg?branch=master)](https://travis-ci.org/benlau/quickflux)

QuickFlux is a Message Dispatcher / Message Queue solution for Qt/QML.
It is also an implementation of Flux Application Architecture (from Facebook).
By using this library, users may write their QML application in a Flux way.
Their code could be more readable and reusable.

Features

 1. Singleton Message Dispatcher / Message Queue per QML Engine.
 2. Write QML application in a Flux way.
 3. Helper function for C++ code to listen on QML event / obtain QML singleton instance
 4. Helper components for handling asynchronous sequential workflow.

Concept and Motivation
======================

Generally speaking, you should avoid writing a big QML file. 
Break down into smaller piece of files is more readable and reusable. 
But sometimes it is not true due to event/signal propagation. 
Managing code dependence is very troublesome.

Writing QML in a Flux way is a solution for this problem. 
Using a global AppDispatcher for communication between components. 
It breaks the dependence and simplify your code by taking out unnecessary signal binding. 

This project provides an implementation of Flux application architecture in QML for user to get started easily.

What is Flux and How to use in QML Application? 
===============================================

Please refer to this article for detail : 
[Writing QML Application in a Flux way | Qt Forum](http://forum.qt.io/topic/55213/writing-qml-application-in-a-flux-way)

or

[A cartoon guide to Flux — Code Cartoons — Medium](https://medium.com/code-cartoons/a-cartoon-guide-to-flux-6157355ab207)

An example program is available for demonstrate how to write QML application in a Flux way
[quickflux/examples/todo](https://github.com/benlau/quickflux/tree/master/examples/todo)

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

Example Projects
================

There are few example programs available at examples folder:

[Examples](https://github.com/benlau/quickflux/tree/master/examples)

Class Reference
---------------

[QuickFlux 1.0 Class Reference](http://benlau.github.io/quickflux/)

Related Projects
----------------
 1. [benlau/quickpromise](https://github.com/benlau/quickpromise) - Promise library for QML

