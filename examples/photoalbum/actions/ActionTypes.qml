pragma Singleton
import QtQuick 2.0

QtObject {
    id: actionTypes;

    property string askToPickPhoto

    property string previewPhoto

    property string pickPhoto

    property string navigateTo

    property string navigateBack

    Component.onCompleted: {
        for (var prop in this) {
            if (typeof this[prop] === "string" &&
                prop !== "objectName") {
                // It is suggested to give a prefix to your ActionTypes
                this[prop] = "AppActions." + prop;
            }
        }
    }
}
