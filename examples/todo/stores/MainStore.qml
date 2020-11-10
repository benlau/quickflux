pragma Singleton
import QuickFlux 1.1

RootStore {
    /// Set the source of actions from AppDispatcher
    bindSource: AppDispatcher
}
