import QtQuick 2.0
import QuickFlux 1.0

Item {

    objectName: "DispatcherTests";

    property var messages: new Array

    AppListener {
        onDispatched: {
            messages.push([type, message]);
        }
    }
}
