# Writing QML application in a Flux way
[![Build Status](https://travis-ci.org/benlau/quickflux.svg?branch=master)](https://travis-ci.org/benlau/quickflux)

Installation Instruction
========================

 1) Clone this repository or download release to a folder within your source tree.

(You may use `git submodule` to embed this repository)

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

*AppDispatcher.dispatch(string name,object message)*

Dispatch a message with name via the AppDispatcher.

AppListener
-----------

It is a helper class to listen dispatched message from AppDispatcher:

Example

```

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

*AppListener.on(string name,function callback)*
Add a listener to the end of the listeners array for the specified message.  Multiple calls passing the same combination of event and listener will result in the listener being added multiple times.

*AppListener.removeListener(string name,function callback)*
Remove a listener from the listener array for the specified message.

*AppListener.removeAllListener(string name)*
Remove all the listeners for a message with name. If name is empty, it will remove all the listeners.


*AppListener.received(string name,object message) [Signal]*
