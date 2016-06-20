/** Todo Item Store

  A centralized data store of Todo list.

 */
pragma Singleton
import QtQuick 2.0
import QuickFlux 1.0
import "../actions"

AppListener {

    property alias model: model

    property int nextUid: 4;

    ListModel {
        // Initial data
        id: model

        ListElement {
            uid: 0
            title: "Task Zero"
            done: true
        }

        ListElement {
            uid: 1
            title: "Task A"
            done: false
        }

        ListElement {
            uid: 2
            title: "Task B"
            done: false
        }

        ListElement {
            uid: 3
            title: "Task C"
            done: false
        }
    }



}



