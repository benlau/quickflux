import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {
    name : "AppScriptTests"

    AppScript {
        id: script1
        property var runMessage;
        property bool started: false
        property int step1 : 0;
        property var step1Message;
        property int step2 : 0;
        property int step3 : 0;

        script: {
            started = true;
            runMessage = message;

            once("step1",function(message) {
                step1++;
                step1Message = message;
            });

            once("step2",function() {
                step2++;
                exit();
            });

            once("step3",function() {
                step3++;
            });
        }
    }

    function test_parallel() {
        compare(script1.listenerId > 0, true);
        script1.run(["a","b","c"]);
        compare(script1.started,true);
        compare(script1.runMessage,["a","b","c"]);

        AppDispatcher.dispatch("step1",13);
        compare(script1.step1,1);
        compare(script1.step1Message,13);
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
            once("step1",function() {
                step1++;
            }).then("step2",function() {
                step2++;
            });

            once("step3",function() {
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

            once("step1",function() {
            });

            once("step2",function() {
               run();
            });
        }

        onStarted: startedCount++;
        onFinished: finishedCount++;
    }

    function test_nested_run() {
        script3.run();
        compare(script3.running,true);
        AppDispatcher.dispatch("step1");
        compare(script3.running,true);
        AppDispatcher.dispatch("step2");
        compare(script3.running,false);
        compare(script3.startedCount,1);
        compare(script3.finishedCount,1);
    }

    AppScript {
        id: script4

        property int finishedCount: 0;
        property int startedCount: 0;

        script: {
        }

        onStarted: startedCount++;
        onFinished: finishedCount++;
    }

    function test_auto_exit() {
        script4.run();
        compare(script4.startedCount,1);
        compare(script4.finishedCount,1);
    }

    AppScript {
        id: script5
        property var runMessage;
        runWhen: "start";
        script: {
            runMessage = message;
            once("step1",function() {
            });
        }
    }

    function test_runWhen() {
        AppDispatcher.dispatch("start", {
                                value1: 2,
                                value2: function() {
                              }});
        compare(script5.running,true);
        compare(script5.runMessage.value1, 2);
        compare(typeof(script5.runMessage.value2) === "function", true);

        script5.exit();
    }

    Timer {
        id: timer6
        interval: 200
        repeat: true
        property int count : 0;
        onTriggered: {
            count++;
        }
    }

    AppScript {
        id: script6
        property int timerTriggered: 0;
        script: {
            timer6.start();

            once(timer6.onTriggered,function() {
                timerTriggered++;
            });
        }
    }

    function test_wait_for_signal() {
        var tmpType;
        var dispatchedCount = 0;
        function listener(type, message) {
            dispatchedCount++;
            tmpType = type;
        }

        AppDispatcher.onDispatched.connect(listener);
        script6.run();

        compare(script6.timerTriggered,0);
        compare(timer6.count,0);
        compare(dispatchedCount,0);
        wait(50);
        compare(script6.timerTriggered,0);
        compare(timer6.count,0);
        compare(dispatchedCount,0);

        tryCompare(script6,"timerTriggered",1);
        compare(timer6.count,1);
        compare(dispatchedCount,1);
        gc();
        wait(200);
        compare(timer6.count,2);
        compare(script6.timerTriggered,1);
        compare(dispatchedCount,1);

        AppDispatcher.dispatch(tmpType);
    }

    AppScript {
        id: script7

        signal trigger(int value1, string value2);
        property int value1 : 0;
        property string value2: "";

        script: {
            once(onTrigger,function(a,b) {
                value1 = a;
                value2 = b;
            });
        }

    }

    function test_wait_for_signal_with_arguments() {
        script7.run();
        compare(script7.running,true);
        script7.trigger(13,"value");
        compare(script7.running,false);
        compare(script7.value1,13);
        compare(script7.value2,"value");
    }

    AppScript {
        id: script8

        script: {
            once("step",function() {
               var c = undefinedVariable;
            });

            var t = undefinedVariable;
        }
    }

    function test_error_handling() {
        script8.run();
        AppDispatcher.dispatch("step");
    }

    AppScript {
        id: script9
        runWhen: "run"

        property int startedCount: 0
        property int finishedCount: 0

        script: {
            startedCount++;
            once("message-never-dispatch",function() {});
        }

        onFinished: finishedCount++
    }

    function test_rerun() {
        AppDispatcher.dispatch("run");
        compare(script9.running,true);
        compare(script9.startedCount,1);
        compare(script9.finishedCount,0);

        AppDispatcher.dispatch("run");
        compare(script9.running,true);
        compare(script9.startedCount,2);
        compare(script9.finishedCount,1);
    }

    AppScript {
        id: script10
        property int step1 : 0;
        property int step2 : 0;
        property int step3 : 0;

        script: {
            once("step1",function() {
                step1++;
            });

            on("step2",function() {
                step2++;
            });

        }
    }

    function test_on() {
        script10.run();
        compare(script10.step1,0);
        compare(script10.step2,0);

        AppDispatcher.dispatch("step2");
        compare(script10.step1,0);
        compare(script10.step2,1);

        AppDispatcher.dispatch("step2");
        compare(script10.step1,0);
        compare(script10.step2,2);

        AppDispatcher.dispatch("step1");
        compare(script10.step1,1);
        compare(script10.step2,2);

        AppDispatcher.dispatch("step1");
        compare(script10.step1,1);
        compare(script10.step2,2);
    }

    AppScript {
        id: script11
        autoExit: false;
        property int step1 : 0;
        property int step2 : 0;

        script: {
            once("step1",function() {
                step1++;
            });

            once("step2",function() {
                step2++;
            });
        }
    }

    function test_autoExit_false() {
        script11.run();
        compare(script11.running,true);
        compare(script11.step1,0);
        AppDispatcher.dispatch("step1");
        compare(script11.step1,1);

        compare(script11.step2,0);
        AppDispatcher.dispatch("step2");
        compare(script11.step2,1);

        // It will be still running.
        compare(script11.running,true);

        script11.exit();
    }

}

