# Message Dispatcher/Queue for Qt/QML

[![Build Status](https://travis-ci.org/benlau/quickflux.svg?branch=master)](https://travis-ci.org/benlau/quickflux)

QuickFlux is a Message Dispatcher / Message Queue solution for Qt/QML.
It is also an implementation of Flux Application Architecture (from Facebook).
By using this library, users may write their QML application in a Flux way.
Their code could be more readable and reusable.

Features

 1. AppDispatcher
  1. A singleton Message Dispatcher / Message Queue per QML Engine.
  2. Avoid out-of-order message processing - It guarantees the order of messages are arrived in sequence to listeners (First come first served)
  3. Support to setup dependence between listeners / stores
 2. Write QML application in a Flux way.
 3. Helper function for C++ code to listen on QML event / obtain QML singleton instance
 4. Helper components for handling asynchronous sequential workflow.

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

 1. [QML Application Architecture Guide with Flux — Medium](https://medium.com/@benlaud/qml-application-architecture-guide-with-flux-b4e970374635#.e4g0tzo78)
 2. [Action-Dispatcher Design Pattern for QML — Medium](https://medium.com/@benlaud/action-dispatcher-design-pattern-for-qml-c350b1d2a7e7#.2dpjhcpt1)

An example program is available at: [quickflux/examples/todo](https://github.com/benlau/quickflux/tree/master/examples/todo)

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

Related Projects
----------------
 1. [benlau/quickpromise](https://github.com/benlau/quickpromise) - Promise library for QML
 2. [benlau/quickcross](https://github.com/benlau/quickcross) - QML Cross Platform Utility Library
 3. [benlau/qsyncable](https://github.com/benlau/qsyncable) - Synchronize data between models
 4. [benlau/testable](https://github.com/benlau/testable) - QML Unit Test Utilities
 5. [benlau/qtci](https://github.com/benlau/qtci) -  A set of scripts to install Qt in Linux command line environment (e.g travis)
