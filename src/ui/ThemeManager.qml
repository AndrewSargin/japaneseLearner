pragma Singleton

import QtQuick
import JapaneseLearner

QtObject {
    id: manager

    property QtObject dark: DarkTheme {}
    property QtObject light: LightTheme {}

    Component.onCompleted: console.log("ready")

    property QtObject current: dark

    function setTheme(themeName) {
        if (themeName === "dark") current = dark
        else if (themeName === "light") current = light
    }

    function toggle() {
        current = (current === dark) ? light : dark
    }
}
