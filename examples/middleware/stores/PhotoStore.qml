pragma Singleton
import QtQuick 2.0
import QuickFlux 1.0
import "../actions"

Item {
    property ListModel photoModel : ListModel {}

    function add(url) {
        photoModel.append({ url: url });
    }

    AppListener {
        filter: ActionTypes.pickPhoto
        onDispatched: {
            add(message.url);
        }
    }
}

