import QtQuick 2.0
import QuickFlux 1.0
import "../stores"
import "../actions"

StoreWorker {

    Filter {

        type: ActionTypes.addTask
        onDispatched: {
            var item = {
                uid: TodoStore.nextUid++,
                title: message.title,
                done: false
            }
            TodoStore.model.append(item);
        }
    }

    Filter {
        type: ActionTypes.setTaskDone
        onDispatched: {
            for (var i = 0 ; i < TodoStore.model.count ; i++) {
                var item  = TodoStore.model.get(i);
                if (item.uid === message.uid) {
                    TodoStore.model.setProperty(i,"done",message.done);
                    break;
                }
            }
        }
    }


}
