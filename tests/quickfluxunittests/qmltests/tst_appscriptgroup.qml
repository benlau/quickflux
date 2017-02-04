import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {
    name : "AppScriptGroupTests"

    AppScript {
        id: script1
        script: {
            once("dummy-message", function() {

            });
        }
    }

    AppScript {
        id: script2
        script: {
            once("dummy-message", function() {

            });
        }
    }

    AppScriptGroup {
        id: group1
        scripts: [script1,script2];
    }

    function test_scripgroup() {
        script1.run();
        compare(script1.running, true);
        compare(script2.running, false);

        script2.run();
        compare(script1.running, false);
        compare(script2.running, true);

        group1.exitAll();

        compare(script1.running, false);
        compare(script2.running, false);
    }

    AppScriptGroup {
        id: group2;
    }

    function test_error_handling() {
        group2.scripts = 3;
        group2.scripts = [group1];
    }

}

