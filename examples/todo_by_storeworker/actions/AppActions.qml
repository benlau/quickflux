pragma Singleton
import QtQuick 2.0
import QuickFlux 1.0
import "./"

QtObject {

    // Add a new task
    function add(title) {
        AppDispatcher.dispatch(ActionTypes.addTask,{title: title });
    }

    // Set/unset done on a task
    function setTaskDone(uid,done) {
        AppDispatcher.dispatch(ActionTypes.setTaskDone,{uid: uid,done: done })
    }

    // Show/hide completed task
    // @Remarks: Unlike other actions, TodoStore do not listen on this
    // message.
    function setShowCompletedTasks(value) {
        AppDispatcher.dispatch(ActionTypes.setShowCompletedTasks, {value: value })
    }

}

