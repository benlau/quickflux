pragma Singleton
import QtQuick 2.0

QtObject {

    property string todo_add;
    property string todo_setDone;
    property string todo_showCompleted;

    Component.onCompleted:  {
        for (var prop in this) {
            if (typeof this[prop] === "string" &&
                prop !== "objectName") {
                this[prop] = prop;
            }
        }
    }
}

