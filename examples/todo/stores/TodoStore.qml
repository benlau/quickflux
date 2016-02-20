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

    Filter {
        // Filter - Add a filter rule to AppListenr

        // Filter item listens on parent's dispatched signal,
        // if a dispatched signal match with its type, it will
        // emit its own "dispatched" signal. Otherwise, it will
        // simply ignore the signal.

        // It is suggested to use AppListener as the type of a Store item,
        // and use Filter item to process actions.

        // Because you may use AppListener.waitFor property to control
        // the order of message recipient. It is difficult to setup
        // the depedence with nested AppListener.

        type: ActionTypes.addTask
        onDispatched: {
            var item = {
                uid: nextUid++,
                title: message.title,
                done: false
            }
            model.append(item);
        }
    }

    Filter {
        type: ActionTypes.setTaskDone
        onDispatched: {
            for (var i = 0 ; i < model.count ; i++) {
                var item  = model.get(i);
                if (item.uid === message.uid) {
                    model.setProperty(i,"done",message.done);
                    break;
                }
            }
        }
    }

}



