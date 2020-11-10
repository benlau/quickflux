import org.kde.kirigami 2.2 as Kirigami
import "./views"

Kirigami.ScrollablePage {
    id: page

    // space for title will be reserved anyway,
    // so why not just fill it with something meaningful
    title: "Todo list"

    header: Header {}
    footer: Footer {}

    // page content item goes inline
    TodoList {}

    // Kirigami.ScrollablePage has undocumented feature:
    // it looks differently when the child item is ListModel
    Kirigami.Theme.colorSet: Kirigami.Theme.Window
}
