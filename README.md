# SillyEdit
my best geode project forever \<3

## Disclaimer(s)
**i make literally no promises as to when this mod gets updated or what gets added/removed, i develop this mod completely for fun and with my own needs in mind first**

***betteredit is unsupported***, **it might still kinda work, but still keep in mind i am not trying to support betteredit at all and anything regarding that will be ignored**

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