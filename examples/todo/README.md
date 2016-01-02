Todo List Example
=================

Purpose: Demonstrate the use of Action, Store and AppDispatcher

**Design Principles**

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





