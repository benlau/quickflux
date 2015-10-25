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

    AppScript {
        id: script2
        property int step1 : 0;
        property int step2 : 0;
        property int step3 : 0;
        property int returnCode: -100;

        script: {
            wait("step1",function() {
                step1++;
            }).wait("step2",function() {
                step2++;
            });

            wait("step3",function() {
                step3++;
            });
        }

        onFinished: {
            script2.returnCode = returnCode;
        }
    }

    function test_chained() {
        script2.run();

        AppDispatcher.dispatch("step2");
        compare(script2.step2,0); // It is not ready yet

        AppDispatcher.dispatch("step3");
        compare(script2.step3,1);

        AppDispatcher.dispatch("step1");
        compare(script2.step3,1);

        AppDispatcher.dispatch("step2");
        compare(script2.step2,1);

        compare(script2.returnCode,0);
    }

    AppScript {
        id: script3

        property int finishedCount: 0;
        property int startedCount: 0;

        script: {
            run();

            wait("step1",function() {
            });

            wait("step2",function() {
               run();
            });
        }

        onStarted: startedCount++;
        onFinished: finishedCount++;
    }

    function test_nested_run() {
        script3.run();
        AppDispatcher.dispatch("step1");
        AppDispatcher.dispatch("step2");
        compare(script3.startedCount,1);
        compare(script3.finishedCount,1);
    }
}

