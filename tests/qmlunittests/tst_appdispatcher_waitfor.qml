import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {
    name : "Dispatcher_waitFor"

    function test_waitFor() {
        var count1 = 0, count2 = 0;
        var id1,id2;
        var realSeq = [], seq = [];

        function callback1() {
            realSeq.push("callback1");
            AppDispatcher.waitFor([id2]);
            seq.push("callback1");
            count1++;
        }

        function callback2() {
            realSeq.push("callback2");
            seq.push("callback2");
            count2++;
        }

        //Nothing happen
        AppDispatcher.waitFor([]);

        id1 = AppDispatcher.addListener(callback1);
        compare(id1 > 0,true);

        id2 = AppDispatcher.addListener(callback2);
        compare(id2 > 0,true);
        compare(id1 !== id2,true);

        AppDispatcher.dispatch("a-message");

        compare(count1,1);
        compare(count2,1);
        compare(realSeq,["callback1","callback2"]);
        compare(seq,["callback2","callback1"]);

        AppDispatcher.removeListener(id1);
        AppDispatcher.removeListener(id2);

        AppDispatcher.dispatch("a-message");
        compare(count1,1);
        compare(count2,1);
    }

}

