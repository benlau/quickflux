import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {
    name : "Dispatcher_dispatch_reentrant"

    property var messages : new Array

    Connections {
        target : AppDispatcher
        onDispatched: {
            messages.push([type,message]);
            if (type === "ping") {
                AppDispatcher.dispatch("pong",{})
            }
        }
    }

    Connections {
        target : AppDispatcher
        onDispatched: {
            messages.push([type,message]);
            if (type === "ping") {
                AppDispatcher.dispatch("pong",{})
            }
        }
    }

    function test_dispatch_reentrant() {
        var i;
        compare(messages.length,0);

        AppDispatcher.dispatch("ping",{});

        compare(messages.length,6); // ping x 2 , pong x 4

        compare(messages[0][0],"ping");
        compare(messages[1][0],"ping");
        compare(messages[2][0],"pong");
        compare(messages[3][0],"pong");
        compare(messages[4][0],"pong");
        compare(messages[5][0],"pong");
    }

}

