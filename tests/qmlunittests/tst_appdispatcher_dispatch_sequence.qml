import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {
    name : "Dispatcher_dispatch_sequence"

    property var messages : new Array

    Connections {
        target : ActionDispatcher
        onReceived: {
            messages.push([name,message]);
            if (name === "ping") {
                ActionDispatcher.dispatch("pong",{})
            }
        }
    }

    Connections {
        target : ActionDispatcher
        onReceived: {
            messages.push([name,message]);
            if (name === "ping") {
                ActionDispatcher.dispatch("pong",{})
            }
        }
    }

    function test_dispatch_sequence() {
        var i;
        compare(messages.length,0);

        ActionDispatcher.dispatch("ping",{});

//        for (i = 0 ; i < messages.length ; i++) {
//            console.log(messages[i][0]);
//        }

        compare(messages.length,6); // ping x 2 , pong x 4

        compare(messages[0][0],"ping");
        compare(messages[1][0],"ping");
        compare(messages[2][0],"pong");
        compare(messages[3][0],"pong");
        compare(messages[4][0],"pong");
        compare(messages[5][0],"pong");
    }

}

