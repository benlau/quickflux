import QtQuick 2.0

Item {
    property var next;

    property var stack;

    function navigateTo(message) {
        stack.push(message.item,message.properties);
    }

    function navigateBack(message) {
        stack.pop();
    }
}
