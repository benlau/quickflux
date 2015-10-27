pragma Singleton
import QtQuick 2.0
import QuickFlux 1.0
import "./"

QtObject {

    function askToPickPhoto() {
        AppDispatcher.dispatch(ActionTypes.askToPickPhoto);
    }

    function pickPhoto(url) {
        AppDispatcher.dispatch(ActionTypes.pickPhoto, {url: url});
    }

    function previewPhoto(url) {
        AppDispatcher.dispatch(ActionTypes.previewPhoto,{url: url});
    }

    function navigateTo(item,properties) {
        AppDispatcher.dispatch(ActionTypes.navigateTo,
                               {item: item,
                                properties: properties});
    }

    function navigateBack() {
        AppDispatcher.dispatch(ActionTypes.navigateBack);
    }
}

