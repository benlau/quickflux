import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {
    name : "AppDispatcher"

    property var messages : new Array

    Connections {
        target : AppDispatcher
        onDispatched: {
            messages.push([type,message]);
        }
    }

    function test_actionDispatcher() {
        compare(messages.length,0);

        AppDispatcher.dispatch("test1",{});
        compare(messages.length,1);

        // Verify can it pass function over the message.

        AppDispatcher.dispatch("test2",{
                                      func : function(i) {
                                          return i+i;
                                      }
                                  });
        compare(messages.length,2);
        var message = messages[messages.length - 1][1];
        compare(message.func(2),4);
    }

}

