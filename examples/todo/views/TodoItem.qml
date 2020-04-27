import QtQuick 2.0
import QtQuick.Controls 2.0
import org.kde.kirigami 2.2 as Kirigami
import "../actions"

Item {
    id: item

    width: parent.width
    implicitHeight: 48

    property int uid;
    property alias title: checkBox.text
    property alias checked: checkBox.checked

    CheckBox {
        id: checkBox
        anchors.fill: parent
    }

    onCheckedChanged: {
        AppActions.setTaskDone(uid, checked);
    }

    // QFlux is compatible with default QML states and transitions.
    // However, use them with care. In particular, avoid any complex
    // logic in PropertyChanges updates.
    Item {
        // this is just a layout container for the animated component
        id: container
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Kirigami.Units.smallSpacing
        anchors.verticalCenter: parent.verticalCenter
        height: 2

        // animated component
        Rectangle {
            id: strikethrough
            width: 0
            height: parent.height

            color: Kirigami.Theme.disabledTextColor
            z: 1
        }

        // animation states & transitions
        states: State {
            name: "checked"
            when: item.checked
            PropertyChanges {
                target: strikethrough
                width: container.width
            }
        }

        transitions: Transition {
            to: "checked"
            reversible: true
            NumberAnimation {
                properties: "width"
                duration: Kirigami.Units.shortDuration
                easing.type: Easing.OutQuad
            }
        }
    }

    // add background to avoid overlapping during animations of surrounding items
    Rectangle {
        anchors.fill: parent
        color: Kirigami.Theme.backgroundColor
        z: -1
    }
}
