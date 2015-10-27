import QtQuick 2.0
import QuickFlux 1.0
import QtQuick.Dialogs 1.0
import "../actions"
import "../views"
import "../stores"

Item {

    FileDialog {
        id: dialog
        title: qsTr("Pick Image")
        nameFilters: [ "Image files (*.jpg *.png)"]
    }

    Component {
        id: imagePreview

        ImagePreview {

        }
    }

    // Use AppScript to run asynchronous sequence task
    // Benefit of using AppScript
    // 1. Centralize your workflow code in one place
    // 2. Highly integrated with AppDispatcher. The order of callback execution is guaranteed in sequence order.
    // 3. Only one script can be executed at a time.
    // 4. exit() will remove all the registered callbacks.

    // Why not just use Promise?
    // 1. You need another library. (e.g QuickPromise)
    // 2. AppScript.exit() will remove all the registered callback completely. You can write less code.

    AppScript {
        // Run this script if "Pick Image" is clicked.
        runWhen: ActionTypes.askToPickPhoto

        script: {
            // Open dialog and response for its result
            dialog.open();

            // Register a callback
            once(dialog.onAccepted,function() {

                // Registered callback will be executed once only per script execution
                AppActions.navigateTo(imagePreview,
                                      {source: dialog.fileUrl});

            }).then(ActionTypes.pickPhoto, function(message) {
                // Callback in then() will not be enabled until
                // its parent is resolved.

                PhotoStore.add(String(message.url));
                AppActions.navigateBack();
            }); // You may chain then() function again

            // Force to terminate if dialog is rejected / or navigateBack is dispatched
            once(dialog.onRejected,exit.bind(0));

            once(ActionTypes.navigateBack,exit.bind(0));
        }
    }

}

