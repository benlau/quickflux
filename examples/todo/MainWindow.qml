import org.kde.kirigami 2.0 as Kirigami

// Wraps single-page application in a platform window
Kirigami.ApplicationWindow {
    width: 18 * Kirigami.Units.gridUnit
    height: 20 * Kirigami.Units.gridUnit

    pageStack.initialPage: MainPage {}
}
