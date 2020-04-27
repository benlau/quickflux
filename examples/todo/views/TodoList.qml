import QtQuick 2.0
import org.kde.kirigami 2.0 as Kirigami

ListView {
    id: listView
    model: TodoVisualModel {}

    // In theory, animations could be handled with actions, stored etc.,
    // but it's better to let QtQuick manage it natively.
    //
    // Cascade effect is taken from Qt docs:
    // https://doc.qt.io/qt-5/qml-qtquick-viewtransition.html#animating-items-to-intermediate-positions

    // Slides in right from the above.
    add: Transition {
        id: addTransition
        NumberAnimation {
            property: "y"
            from: _getPrevItemPosition(addTransition.ViewTransition.index).y
            duration: Kirigami.Units.veryLongDuration
            easing.type: Easing.OutBounce
        }
    }

    // Just moves to the new place with animation.
    displaced: Transition {
        NumberAnimation {
            property: "y"
            duration: Kirigami.Units.veryLongDuration
            easing.type: Easing.OutBounce
        }
    }

    // Swipes out to the right, cascading timing.
    remove: Transition {
        id: removeTransition
        SequentialAnimation {
            PauseAnimation {
                duration: {
                    const offset = (removeTransition.ViewTransition.index
                                    - Math.min.apply(null, removeTransition.ViewTransition.targetIndexes));
                    const duration = Kirigami.Units.shortDuration / 2;
                    return offset * duration;
                }
            }
            ParallelAnimation {
                NumberAnimation {
                    property: "x"
                    to: removeTransition.ViewTransition.item.width
                    duration: Kirigami.Units.shortDuration
                    easing.type: Easing.InQuad
                }
                NumberAnimation {
                    property: "opacity"
                    to: 0
                    duration: Kirigami.Units.shortDuration
                    easing.type: Easing.InQuad
                }
            }
        }
    }

    function _getPrevItemPosition(index) {
        const prevIdx = index - 1;
        const prevItem = listView.itemAtIndex(prevIdx);
        return listView.mapFromItem(prevItem, 0, 0);
    }
}
