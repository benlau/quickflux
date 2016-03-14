import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {
    name : "AppListener_waitFor"

    property var seq : new Array;

    AppListener {
        id: listener1
        property int count : 0;
        waitFor: []
        onDispatched: {
            count++;
            seq.push("listener1");
        }
    }

    AppListener {
        id: listener2
        property int count : 0;
        onDispatched: {
            count++;
            seq.push("listener2");
        }
    }

    AppListener {
        id: listener3
        property int count : 0;
        onDispatched: {
            count++;
            seq.push("listener3");
        }
    }

    function test_waitFor() {
        AppDispatcher.dispatch("test-message","test-message");
        listener2.waitFor = [listener1.listenerId,listener3.listenerId];
        listener3.waitFor = [listener1.listenerId];
        seq = new Array;
        AppDispatcher.dispatch("test-message","test-message");
        compare(listener1.count,2);
        compare(listener2.count,2);
        compare(listener3.count,2);
        compare(seq,["listener1","listener3","listener2"]);
    }


}

