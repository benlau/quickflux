That is a template created for application with using Quick Flux. This template provides:

1. qpm.json - Install QuickFlux via qpm
2. Template of ActionTypes and AppActions
3. Templates of Store / StoreWorker and Adapter
4. AppView C++ - Initialize QML environment and listen message from AppDispatcher

File Structre:

```
.
├── App
│   ├── actions
│   │   ├── ActionTypes.qml
│   │   ├── AppActions.qml
│   │   └── qmldir
│   ├── adapters
│   │   ├── StoreAdapter.qml
│   │   └── qmldir
│   ├── constants
│   │   ├── Constants.qml
│   │   └── qmldir
│   ├── stores
│   │   ├── MainStore.qml
│   │   └── qmldir
│   └── storeworkers
│       ├── MainStoreWorker.qml
│       ├── StoreWorker.qml
│       └── qmldir
├── app.pri
├── app.pro
├── app.qrc
├── appview.cpp
├── appview.h
├── deployment.pri
├── main.cpp
├── main.qml
└── qpm.json

```

To use this project template, just copy this folder to your workspace. Rename the app.pro to your project name. Then you could start coding.
