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

Installation Instruction
========================

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
===============

AppDispatcher
-------------

AppDispatcher is a singleton object in QML scope for message delivery.

**AppDispatcher.dispatch(string type,object message)**

Dispatch a message with type via the AppDispatcher.
The message will be placed on a queue and delivery via the "dispatched" signal.
Listeners may listen to the "dispatched" signal directly, 
or using helper components like AppListener / AppScript to capture signal.

```
MouseArea {
    anchors.fill: parent
    onClicked: {
        AppDispatcher.dispatch(ActionTypes.askToRemoveItem, { uid: uid });
    }
}
```

Usually, it will emit "dispatched" signal immediately after calling dispatch().
However, if AppDispatcher is still dispatching messages,
the new messages will be placed on a queue,
and wait until it is finished.
It guarantees the order of messages are arrived in sequence to listeners

``` 

AppListener {
    filter: ActionTypes.askToRemoveItem
    onDispatched: {
        if (options.skipRemoveConfirmation) {
            AppDispatcher.dispatch(ActionTypes.removeItem,message);
            // Because AppDispatcher is still dispatching ActionTypes.askToRemoveItem,
            // ActionTypes.removeItem will be placed in a queue and will dispatch when
            // all the listeners received current message.
        }
    }
}

```

**AppDispatcher.addListener(callback)**

Registers a callback to be invoked with every dispatched message. Returns a listener ID that can be used with waitFor().

**AppDispatcher.removeLister(listenerId)**

Remove a callback by the listenerId returned by addListener

**AppDispatcher.waitFor(array of ids)**

Waits for the callbacks specified to be invoked before continuing execution of the current callback. This method should only be used by a callback in response to a dispatched message.

**AppDispatcher.dispatched(string type,object message)[Signal]**

Listeners may listen on this signal to get the latest dispatched message from AppDispatcher.

Example code:

```
import QuickFlux 1.0

// Method 1 - Using Connections component
Connections {
    target: AppDispatcher
    onDispatched: {
        switch (type) {
            case "OpenItem";
                // ...
                break;
            case "DeleteItem";
                // ...
                break;
        }
    }
}

// Method 2 - Using helper component, AppListener
AppListener {
    filter: "ItemOpen";
    onDispatched: {
      /// ...
    }
}

// Method 3 - Using addListener

Component.onCompleted: {
   AppDispatcher.addListener(function() {
     switch (type) {
        case "OpenItem";
          // ...
            break;
        case "DeleteItem";
          // ...
            break;
     }
   });
}

```


AppListener
-----------

It is a helper class to listen dispatched messages from AppDispatcher. It provides an alternative method other then using Connections component. 

Example

```

// Only listen for specific message
AppListener {
    filter: "messageType1";
    onDispatched: {
      // Your code here
    }
}

// Listen for multiple messages.
AppListener {
    onDispatched: {
        switch (type) {
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

**AppListener.on(string type,function callback)**

Add a listener to the end of the listeners array for the specified message.  Multiple calls passing the same combination of event and listener will result in the listener being added multiple times.

**AppListener.removeListener(string type,function callback)**

Remove a listener from the listener array for the specified message.

**AppListener.removeAllListener(string type)**

Remove all the listeners for a message with type. If type is empty, it will remove all the listeners.

**AppListener.dispatched(string type,object message)[Signal]**

It is a proxy of AppDispatcher.dispatched signal.
If the enabled property is set to false, this signal will not be emitted.

**enabled[Property]**

This property holds whether the listener receives message.
If it is false, it won't emit "dispatched" signal and trigger callback registered via "on" function.
By default this is true.

The value can be controlled by parent component.
Setting this property directly affects the enabled value of child items.
When set to false, the enabled values of all child items also become false.
When set to true,
the enabled values of child items are returned to true,
unless they have explicitly been set to false.

**filter[Property]**

Set a filter to incoming message. Only message with type matched with the filter will emit "dispatched" signal.
If it is not set, it will dispatch every message.

**filters[Property]**
Set a list of filter to incoming message. Only message with type matched with the filters will emit "dispatched" signal.
If it is not set, it will dispatch every message.

**alwaysOn[Property]**

This property holds a value to indicate if the listener should remain listening message when it is not enabled.

**listenerId[Property]**

The listener ID of this component.
It could be used with AppListener.waitFor/AppDispatcher.waitFor to control the order of message delivery.

**waitFor[Property]**

If it is set, it will block the emission of dispatched signal until all the specificed listeners has been invoked.

Example code:

```
AppListener {
  id: listener1
}

AppListener {
   id: listener2
   waitFor: [listener1.listenerId]
}
```

AppListenerGroup
----------------

AppListenerGroup collects listener ID from all of its child AppListener and initialize their waitFor property.
It could be used as the base type of a Store component for setup dependence between them.

MyStore1.qml

```
AppListenerGroup {
    AppListener {
    }
    AppListener {
    }
}
```

MyStore2.qml

```
AppListenerGroup {
   waitFor: MyStore1.listenerIds
}
```


QFAppDispatcher
---------------

QFAppDispatcher is the C++ implementation of AppDispatcher in QML scope. 
You may need this class to setup communication between C++ and QML.

```
#include "qfappdispatcher.h"
```

**static QFAppDispatcher* QFAppDispatcher.instance(QQmlEngine* engine)**

Obtain the singleton instance of AppDispatcher for specific QQmlEngine

**static QObject* singletonObject(QQmlEngine* engine,QString package,
                                    int versionMajor,
                                    int versionMinor,
                                    QString typeName)**


Obtain a singleton object from package for specific QQmlEngine. 
It is useful when you need to get singleton Actions component.


AppScript
---------

AppScript is a helper component to handle asynchronous sequential workflow.
The immediate benefit of using AppScript  is the centralisation of code in a place.
Instead of placing them within onXXXX code block in several components in several places.

AppScript also manage the life cycle of registered callbacks. You could remove them by a single function. Nothing will leave in memory.

Example Code:
[quickflux/ImagePickerScript.qml at master · benlau/quickflux](https://github.com/benlau/quickflux/blob/master/examples/photoalbum/scripts/ImagePickerScript.qml)

```

    AppScript {
        // Run this script if "Pick Image" button is clicked.
        runWhen: ActionTypes.askToPickPhoto

        script: {
            // Step 1. Open file dialog
            dialog.open();

            // Register a set of callbacks as workflow
            // Registered callbacks will be executed once only per script execution
            once(dialog.onAccepted, function() {

                // Step 2. Once user picked an image, launch preview window and ask for confirmation.
                AppActions.navigateTo(imagePreview,
                                      {source: dialog.fileUrl});

            }).then(ActionTypes.pickPhoto, function(message) {
                // The function of then() is same as once() but it won't
                // trigger the callback until once() is triggered.

                // Step 3. Add picked image to store and go back to previous page.

                PhotoStore.add(String(message.url));

                AppActions.navigateBack();

            }); // <-- You may chain then() function again.

            // Condition to terminate the workflow:
            // Force to terminate if dialog is rejected / or navigateBack is dispatched
            // That will remove all the registered callbacks

            once(dialog.onRejected,exit.bind(this,0));

            once(ActionTypes.navigateBack,exit.bind(this,0));
        }
    }
```

**Benefit of using AppScript**

1. Centralize your workflow code in one place
2. Highly integrated with AppDispatcher. The order of callback execution is guaranteed in sequence order.
3. Only one script can be executed at a time. Registered callbacks by previous script will be removed before starting.
4. exit() will remove all the registered callbacks. No callback leave after termination.

**Why not just use Promise?**

1. You need another library. (e.g QuickPromise)
2. You need to set reject condition per callback/promise

Explanation: Coding in a promise way requires you to handle every reject condition correctly. Otherwise,
promise will leave in memory and their behaviour will be unexpected.
AppScript.run() / exit() clear all the registered callback completely. You can write less code.

**AppScript.script[Property]**
The code to be executed at run()

**AppScript.run()**
Execute the code within the script property.
If the previous script is still running.
AppScript will terminate it by removing all the registered callback.

*AppScript.runWhen[Property]*
Whatever a message with type same as runWhen will trigger run() immediately. 

**AppScript.once(type,callback)**

Register a callback to be triggered when the required message type is dispatched.
It will be triggered once only. 
Users should call this function within the script code block.

**AppScript.once().then(type,callback)**

Similar to once() function, it will register a callback to be executed once only.
But its registration is deferred until the previous callback triggered.
then() is a chain able function, users may place as many as possible as they like.

**AppScript.on(type,callback)**

Register a callback to be triggered whatever the required message type is dispatched.
Users should call this function within the script code block.

**AppScript.exit()**
Terming current executing script by removing all the registered callbacks.

**AppScript.running[Property]**

It is true when the script is running or there still has registered callback leave.



Related Projects
----------------
 1. [benlau/quickpromise](https://github.com/benlau/quickpromise) - Promise library for QML

