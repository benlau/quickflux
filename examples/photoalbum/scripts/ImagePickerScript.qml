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

    AppScript {
        // Run this script if "Pick Image" is clicked.
        runWhen: ActionTypes.askToPickPhoto

        script: {
            // Open dialog and wait for its result
            dialog.open();

            wait(dialog.onAccepted,function() {

                // waited task will be executed once only per script execution
                AppActions.navigateTo(imagePreview,
                                      {source: dialog.fileUrl});

            }).wait(ActionTypes.pickPhoto, function(message) {
                // Chained wait() will not be enabled until
                // its parent wait() is resolved.

                PhotoStore.add(String(message.url));
                AppActions.navigateBack();
            });

            // Force to terminate if dialog is rejected / or navigateBack is triggered
            wait(dialog.onRejected,exit.bind(0));

            wait(ActionTypes.navigateBack,exit.bind(0));
        }
    }

}

