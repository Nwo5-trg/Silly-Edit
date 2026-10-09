# sillyedit
editor mod that adds over 2 dozen features all throughout the editor, my best geode project forever <cr>\<3</c>

this isnt a sweeping editor overhaul mod, most features are pretty small, but should still make ur life a lil easier

## Disclaimer(s)
**i make literally no promises as to when this mod gets updated or what gets added/removed, i develop this mod completely for fun and with my own needs in mind first**

***betteredit is unsupported***, **it might still kinda work, but still keep in mind i am not trying to support betteredit at all and anything regarding that will be ignored, sillyedit already adds a few betteredit features and tinker (which is supported) adds pretty much the rest of them so*

## Features
feature list in about.md

## API
sillyedit has an api for enabling/disabling features

to use just include

```cpp
#include <nwo5.sillyedit/include/include.hpp>
```

all function are in

```cpp
namespace nwo5::sillyedit {}
```

lets say u wanna disable selection utils, you can do

```cpp
void someFunctionRawr() {
    nwo5::sillyedit::toggleFeature("Selection Utils", false);
}
```

you can also check if a feature is enabled

```cpp
void someFunctionRawr() {
    const auto enabled = nwo5::sillyedit::featureEnabled("Selection Utils");
}
```

or if its force disabled with this api

```cpp
void someFunctionRawr() {
    const auto forceDisabled = nwo5::sillyedit::featureForceDisabled("Selection Utils");
}
```
all sillyedit settings are geode savedvalues, if u rly wanna access them they are always named `[category-name]-[setting-name]`

so getting this setting

```cpp
inline SillySetting<bool> snapIndicator{"Snap Indicator", feature, true};
```

would be

```cpp
void someFunctionRawr() {
    const auto snapIndicatorEnabled = Loader::get()->getLoadedMod("nwo5.sillyedit")
        ->getSavedValue<bool>("selection-utils-snap-indicator");
}
```