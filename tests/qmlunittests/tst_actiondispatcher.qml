import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {
    name : "ActionDispatcher"

    property var messages : new Array

    Connections {
        target : ActionDispatcher
        onReceived: {
            messages.push([name,message]);
        }
    }

    function test_actionDispatcher() {
        compare(messages.length,0);

        ActionDispatcher.dispatch("test1",{});
        compare(messages.length,1);

        // Verify can it pass function over the message.

        ActionDispatcher.dispatch("test2",{
                                      func : function(i) {
                                          return i+i;
                                      }
                                  });
        compare(messages.length,2);
        var message = messages[messages.length - 1][1];
        compare(message.func(2),4);
    }

}

