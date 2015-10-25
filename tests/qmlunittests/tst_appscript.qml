import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {
    name : "AppScriptTests"


    AppScript {
        id: script1
        property bool started: false
        property int step1 : 0;
        property int step2 : 0;
        property int step3 : 0;

        script: {
            started = true;

            wait("step1",function() {
                step1++;
            });

            wait("step2",function() {
                step2++;
                exit();
            });

            wait("step3",function() {
                step3++;
            });
        }
    }

    function test_parallel() {
        script1.run();
        compare(script1.started,true);

        AppDispatcher.dispatch("step1");
        compare(script1.step1,1);
        AppDispatcher.dispatch("step1");
        compare(script1.step1,1);

        AppDispatcher.dispatch("step2");
        compare(script1.step2,1);

        // It is already quit.
        AppDispatcher.dispatch("step3");
        compare(script1.step3,0);

        /* Run again */
        script1.run();
        AppDispatcher.dispatch("step1");
        compare(script1.step1,2);
        AppDispatcher.dispatch("step1");
        compare(script1.step1,2);
    }
}

