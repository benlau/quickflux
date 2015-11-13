import QtQuick 2.0
import QuickFlux 1.0
import "../stores"
import "../actions"

VisualDataModel {

    model: TodoStore.model

    filterOnGroup: "nonCompleted"

    groups: [
        VisualDataGroup {
            name: "nonCompleted"
            includeByDefault: true
        }
    ]

    delegate: TodoItem {
        id:item
        uid: model.uid
        title: model.title
        checked: model.done

        Component.onCompleted: {
            item.VisualDataModel.inNonCompleted = Qt.binding(function() { return !model.done})
        }
    }

    Component.onCompleted: {
        // It can not place any child component within VisualDataModel,
        // it will be considered as delegate. Therefore, it can not
        // use AppListener. Instead it could connect AppDispatcher.onDispatched
        // signal directly and filter messages by ourself

        // In production environment, you should disconnect the signal during destruction.
        AppDispatcher.onDispatched.connect(function(type,message) {
            if (type !== ActionTypes.showCompletedTasks) {
                return;
            }
            filterOnGroup = message.value ? "" : "nonCompleted"
        });
    }
}

