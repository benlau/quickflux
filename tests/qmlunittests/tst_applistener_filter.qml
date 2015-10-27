import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {

    name : "AppListener"

    property int count1 : 0
    property int count2 : 0

    AppListener {
        id: listener1
        onDispatched: count1++;
    }

    AppListener {
        id: listener2
        onDispatched: count2++;
    }

    function test_filter() {
        var name = "test_filter"
        listener1.filter = name;
        listener1.filters = [];
        listener2.filter = "";
        listener2.filters = [];
        count1 = count2 = 0;

        AppDispatcher.dispatch("Non-related message");
        compare(count1,0);
        compare(count2,1);

        AppDispatcher.dispatch(name);
        compare(count1,1);
        compare(count2,2);

        compare(listener1.filters.length,0);
    }

    function test_filters() {
        var name1 = "test_filter1";
        var name2 = "test_filter2";

        listener1.filter = "";
        listener1.filters = [name1,name2];
        listener2.filter = "";
        listener2.filters = [];
        count1 = count2 = 0;

        AppDispatcher.dispatch("Non-related message");
        compare(count1,0);
        compare(count2,1);

        AppDispatcher.dispatch(name1);
        compare(count1,1);
        compare(count2,2);

        AppDispatcher.dispatch(name2);
        compare(count1,2);
        compare(count2,3);

        compare(listener1.filters.length,2);

    }

}

