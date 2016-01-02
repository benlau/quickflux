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
        // use AppListener. Instead it should call AppDispatcher.addListener
        // to register a callback.

        // In production environment for non-singleton object, you should disconnect the signal during destruction.
        AppDispatcher.addListener(function(type,message) {
            if (type !== ActionTypes.showCompletedTasks) {
                return;
            }
            filterOnGroup = message.value ? "" : "nonCompleted"
        });
    }
}

