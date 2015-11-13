pragma Singleton
import QtQuick 2.0

QtObject {

    property string addTask;
    property string setTaskDone;
    property string showCompletedTasks;

    Component.onCompleted:  {
        for (var prop in this) {
            if (typeof this[prop] === "string" &&
                prop !== "objectName") {
                // It is suggested to give a prefix to your ActionTypes
                this[prop] = "AppActions." + prop;
            }
        }
    }
}

