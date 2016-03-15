import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {
    name : "ActionCreatorTests"

    ActionCreator {
        id: actionCreator
        signal test1();
        signal test2(int v1, string v2, real v3, var v4)
    }

    AppListener {
        id: listener
        property var payload: new Array
        onDispatched: {
            payload.push({
               type: type,
               message: message
              });
        }
    }

    function test_emission() {
        compare(listener.payload.length, 0);
        actionCreator.test1();
        compare(listener.payload.length, 1);
        compare(listener.payload[0].type, "test1");

        var data = {
            f1 : "f1"
        }

        actionCreator.test2(1,"v2",3.0,data);
        compare(listener.payload.length, 2);
        compare(listener.payload[1].type, "test2");

        var message = listener.payload[1].message;
        compare(message.v1, 1);
        compare(message.v2, "v2");
        compare(message.v3, 3.0);
        compare(message.v4, data);
    }

    ActionCreator {
        // No signals
        id: dummy
    }
}

