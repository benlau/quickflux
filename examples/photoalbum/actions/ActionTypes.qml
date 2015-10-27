pragma Singleton
import QtQuick 2.0

QtObject {

    property string askToPickPhoto

    property string previewPhoto

    property string pickPhoto

    property string navigateTo
    property string navigateBack

    Component.onCompleted: {
        for (var prop in this) {
            if (prop.match(/Changed$/) || prop === "objectName") {
                continue
            }
            if (typeof prop === "string") {
                this[prop] = prop;
            }
        }
    }
}
