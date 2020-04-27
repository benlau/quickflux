import QtQuick 2.3
import QuickFlux 1.1
import "./middlewares"
import "./actions"

// Non-visual container for application-wide components
Item {

    MiddlewareList {
        applyTarget: AppActions

        SystemMiddleware {
            mainWindow: mainWindow
        }

        DialogMiddleware {
        }
    }

    MainWindow {
        id: mainWindow
    }
}
