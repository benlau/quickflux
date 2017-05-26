Todo List Example
=================

Purpose: Demonstrate how to write a QML application in a Flux way.

File Structure
--------------

```
.
├── actions
│   ├── ActionTypes.qml
│   ├── AppActions.qml
│   └── qmldir
├── adapters
├── main.qml
├── mainWindow.qml
├── middlewares
│   └── DialogMiddleware.qml
├── qml.qrc
├── stores
│   ├── MainStore.qml
│   ├── RootStore.qml
│   ├── TodoStore.qml
│   ├── UserPrefsStore.qml
│   └── qmldir
├── todo.pro
└── views
    ├── Footer.qml
    ├── Header.qml
    ├── TodoItem.qml
    ├── TodoList.qml
    └── TodoVisualModel.qml

```


**ActionTypes.qml**

ActionTypes is a constant table (singleton component) to store all the available action types.

Example:

```
pragma Singleton
import QtQuick 2.0
import QuickFlux 1.0

KeyTable {
    // KeyTable is an object with properties equal to its key name

    property string addTask;

    property string setTaskDone;

    property string setShowCompletedTasks;

}
```

It is not recommended to name an action by combing sender and event like removeItemButtonClicked.
It is suggested to tell what users do (e.g. askToRemoveItem) or what it should actually do (e.g. removeItem).
You may add a prefix of scope to its name if needed. (e.g. itemRemove)

**AppActions.qml**

AppActions is an action creator, a helper component to create and dispatch actions via the central dispatcher. It has no knowledge about the data model and who will use it. As it only depends on AppDispatcher, it could be used anywhere.

AppDispatcher is the central dispatcher. It is also a singleton object. Actions sent by dispatch() function call will be placed on a queue. Then, the Dispatcher will emit a“dispatched” signal.

Moreover, there has a side benefit in using ActionTypes and AppActions. Since they stores all the actions, when a new developer joins the project. He/she may open theses two files and know the entire API.

**Stores**

Stores contain application data, state and logic.
Somehow it is similar to MVVM's View Model.
However, Stores are read-only to Views.
Views could only query data from Stores.
"Update" should be done via Actions.
Therefore, the "queries" and "updates" operations are in fact separated.

UserPrefsStore.qml

```
import QtQuick 2.0
import QuickFlux 1.1
import "../actions"

Store {

    property bool showCompletedTasks: false

    Filter {
        // Views do not write to showCompeletedTasks directly.
        // It asks AppActions.setShowCompletedTasks() to do so.
        type: ActionTypes.setShowCompletedTasks
        onDispatched: {
            showCompletedTasks = message.value;
        }
    }

}

Design Principles
-----------------

**1. Avoid writing a big QML file. Break down into smaller pieces**

**2. Avoid signal propagation between custom view components.**

main.qml
```
Window {
    width: 480
    height: 640
    visible: true

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16

        Header {
            Layout.fillWidth: true
            Layout.fillHeight: false
        }

        TodoList {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Footer {
            Layout.fillWidth: true
            Layout.fillHeight: false
        }
    }
}
```

This example program is divided into 3 parts : Header, TodoList and Footer. 
Each part contains at least one button that may change the status to other components.
However, they are not connected by any signal/data binding between each others as shown in the above code. 
They have no knowledge of others. 
The result is a loose coupled design.

**3. Tell, Don't ask**

Inline code for handling user event can be hard to trace and test. They are not centralized in one source file. The dependence is not obvious and therefore it is easy to be broken during refactoring.

To simplify your inline code, don’t ask for the state to make any decision, just tell what do you want to do to Action class:

Example:

header.qml
```
onCheckedChanged: {
    AppActions.setTaskDone(uid,checked);
}
```
