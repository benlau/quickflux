# QuickFlux - Inter-Process Communication / Message Queue for Qt/QML

[![Build Status](https://travis-ci.org/benlau/quickflux.svg?branch=master)](https://travis-ci.org/benlau/quickflux)

QuickFlux is an Inter-Process Communication / Message Queue solution for Qt/QML.
It is also an implementation of Flux Application Architecture which is invented by Facebook.
By using this library, users may write their QML application in a Flux way.

Features

 1. Singleton Message Queue per QML Engine.
 2. Write QML application in a Flux way.
 3. Utility function for C++ code to listen on QML event.

Concept and Motivation
======================

Generally speaking, you should avoid writing a big QML file. 
Break down into smaller piece of files is more readable and reusable. 
But sometimes it is not true due to event/signal propagation. 
Managing code dependence is very troublesome.

Writing QML in a Flux way is a solution for this problem. 
It breaks the dependence and simplify your code by taking out event/signal propagation. 
This project provides an implementation of Flux application architecture in QML for user to get started easily.

What is Flux and How to use in QML Application? 
===============================================

Please refer to this article for detail : 
[Writing QML Application in a Flux way | Qt Forum](http://forum.qt.io/topic/55213/writing-qml-application-in-a-flux-way)

Installation Instruction
========================

 1) Clone this repository or download release to a folder within your source tree.

 2) Add this line to your profile file(.pro):

    include(quickflux/quickflux.pri) # You should modify the path by yourself

 3) Add import statement in your QML file

```
import QuickFlux 1.0
```

Class Reference
===============

AppDispatcher
-------------

AppDispatcher is a singleton object in QML scope for delivering message.

**AppDispatcher.dispatch(string name,object message)**

Dispatch a message with name via the AppDispatcher.
Listeners should listen on the "dispatched" signal to be notified.

Usually, it will emit "dispatched" signal immediately after calling dispatch().
However, if AppDispatcher is still dispatching messages,
the new messages will be placed on a queue,
and wait until the it is finished.
It guarantees the order of messages are arrived in sequence to listeners

**AppDispatcher.dispatched(string name,object message)[Signal]**

Listeners should listen on this signal to get the latest dispatched message from AppDispatcher.

Example code:

```
import QuickFlux 1.0

Connections {
    target: AppDispatcher
    onDispatched: {
        if (name === "ItemOpen") {
            ...
        } else if (name ==="ItemDelete") {
            ...
        }
    }
}
```

AppListener
-----------

It is a helper class to listen dispatched messages from AppDispatcher. It provides an alternative method other then using Connections component. 

Example

```

// Only listen for specific message
AppListener {
    filter: "messageType1"; // filter property will be added in v1.0.1
    onDispatched: {
      // Your code here
    }
}

// Listen for multiple messages.
AppListener {
    onDispatched: {
        switch (name) {
            case "messageType1":
                // ...
                break;
            case "messageType2":
                // ...
                break;
        }
    }
}

// Alternative method to listen for multiple messages

AppListener {

  Component.onComponented: {
    on("messageType1",function() {
       /// Your code here.
    });
    on("messageType2",function() {
       /// Your code here.
    });
  }
}


```

**AppListener.on(string name,function callback)**

Add a listener to the end of the listeners array for the specified message.  Multiple calls passing the same combination of event and listener will result in the listener being added multiple times.

**AppListener.removeListener(string name,function callback)**

Remove a listener from the listener array for the specified message.

**AppListener.removeAllListener(string name)**

Remove all the listeners for a message with name. If name is empty, it will remove all the listeners.

**AppListener.dispatched(string name,object message)[Signal]**

It is a proxy of AppDispatcher.dispatched signal.
If the enabled property is set to false, this signal will not be emitted.

**enabled[Property]**

If this property is set to false, all the signal and callback will not be invokved.
This include the "dispatched" signal and callback registered via the "on()" function.

**filter[Property]**

Set a filter to incoming message. Only message with name matched with the filter will emit "dispatched" signal.
If it is not set, it will dispatch every message.

**filters[Property]**
Set a list of filter to incoming message. Only message with name matched with the filters will emit "dispatched" signal.
If it is not set, it will dispatch every message.

