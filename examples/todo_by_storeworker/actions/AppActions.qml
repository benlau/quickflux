pragma Singleton
import QtQuick 2.0
import QuickFlux 1.0
import "./"

ActionCreator {

    signal addTask(string title)

    // Set/unset done on a task
    signal setTaskDone(int uid,bool done)

    // Show/hide completed task
    // @Remarks: Unlike other actions, TodoStore do not listen on this
    // message.
    signal setShowCompletedTasks(bool value)

}

