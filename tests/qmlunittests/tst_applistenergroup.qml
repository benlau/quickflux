import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {
    id: testCase
    name : "AppListenerGroup"

    property var seq : new Array;

    Component {
        id: creator
        AppListenerGroup {
            id: group
            property string prefix;
            property int count: 0
            property alias listener1: listener1
            property alias listener2: listener2
            property alias listener3: listener3

            AppListener {
                id: listener1;
                onDispatched: {
                    count++;
                    seq.push(prefix + "Listener1");
                }
            }

            AppListener {
                id: listener2;

                onDispatched: {
                    count++;
                    seq.push(prefix + "Listener2");
                }
            }

            AppListener {
                id: listener3;

                onDispatched: {
                    count++;
                    seq.push(prefix + "Listener3");
                }
            }
        }
    }

    function test_grouping() {
        var group = creator.createObject(testCase);

        compare(group.listenerIds.length,3);
        compare(group.listener1.waitFor.length,1);
        compare(group.listener2.waitFor.length,1);
        compare(group.listener3.waitFor.length,1);
        compare(group.listener1.waitFor[0],group.listener2.waitFor[0]);
        compare(group.listener3.waitFor[0],group.listener2.waitFor[0]);

        group.destroy();
    }

    function test_waitFor() {
        seq = new Array;
        var group1 = creator.createObject(testCase);
        group1.prefix = "group1";
        var group2 = creator.createObject(testCase);
        group2.prefix = "group2";

        // group1 should receive message first, but now it depend on group2
        group1.waitFor = group2.listenerIds;

        AppDispatcher.dispatch("example message");

        compare(seq,["group2Listener1",
                     "group2Listener2",
                     "group2Listener3",
                     "group1Listener3",
                     "group1Listener2",
                     "group1Listener1"]);
    }

}

