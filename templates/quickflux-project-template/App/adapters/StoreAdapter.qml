import QtQuick 2.0
import QuickFlux 1.0
import "../storeworkers"

Item {
    id: adapter

    Item {
        id: model

        MainStoreWorker {

        }

    }

    AppListener {
        onDispatched: {
            var workers = model.data;
            for (var i in workers) {
                workers[i].dispatched(type, message);
            }
        }
    }

}
