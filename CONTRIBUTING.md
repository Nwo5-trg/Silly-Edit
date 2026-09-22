# Contributing
this isnt exactlyyyy a contributing md its more like "how to do anything in the project guide"

## AI
dont use ai for anything whatsoever oki thx \<3

## Conventions
- *variables/constants* `camelCase`
- *constexpr constants* `UPPER_SNAKE_CASE`
- *parameters* `pPascalCase`
- *types* `PascalCase`
- *class member* `m_camelCase`
- *macros* `UPPER_SNAKE_CASE`
- *enumerations* `PascalCase`
- *files/folders* `kebab-case`
- *template args* `T, U, V` or `PascalCase`
- *concepts* `PascalCase`
- *globals* `s_camelCase`
- *namespaces* `PascalCase`
- *library/api namespaces* `nocase`

look at the code for the rest idk

## Making a feature
there are pretty much only 3 parts to a feature, their registry, their `include.hpp` and their main.cpp

(feature templates are in `examples/feature`)

## Registering
just go into `features/registry.hpp` and add ur feature to the `SILLYEDIT_FEATURE_LIST` macro in `PascalCase`, features are organized in settings popup as they appear in the macro

after that just make your feature directory, the path should be something like `features/title/silly-feature` (title being tools, utility, etc..)

## include.hpp
this the core of your feature where everything is defined, heres an example one

```cpp
#pragma once

#include <Geode/modify/GJRawr.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>

namespace SillyFeature {
    class $feature(SillyFeature, /*optional*/SettingReload::Editor, /*optional*/SettingCondition::DesktopOnly) {
        void onEditor() override; // override necessary functions (check feature/base.hpp), if no functions are overriden make the enitre feature declaration one line
    } feature;

    inline constexpr float SOME_CONSTANT = 10.0f; // if u need constexpr variables for ui stuff *across the whole feature* (for example in onEditor) define here (or in utils, js do whatever makes the most sense)
        
    class $feature_modify(GJRawr) {
        struct Fields { // if any fields are present
            int silliness = 100;
        };

        static constexpr float SOME_OTHER_CONSTANT = 5.0f; // if u need constexpr variables for ui stuff *in a class* define here

        void someCustomMethod();
        void onSomeCallback(CCObject* /*optional*/pSender); // always use function callbacks when it comes to buttons if u can help it
    
        static void onModify(auto& pSelf) {
            (void)pSelf.setHookPriorityPost("GJRawr::someHookedMethod", geode::Priority::Late);
        } // if necessary

        void someHookedMethod();
        void someOtherHookedMethod(bool whateverParamNameBindingsHasNoPrefix);

        // tl;dr order is (this applies to main.cpp as well)
        // CUSTOMMETHODS
        //
        // ONMODIFY
        //
        // HOOKEDMETHODS
    }; 

    class $feature_modify(EditorUI) {
        void someHookedMethod();
    };

    class $setting_category("silly-feature-logo.png"_spr, "Description without capitalization or grammar, only capitalize first letter, make it silly \!\!");

    inline SillySetting<bool> coolSetting{
        "Cool Setting\nName", feature, true // yes newlines are hardcoded into the names sue me
    };
    inline SillySetting<int> sillySetting{
        "Silly Setting", feature, 5, "Same as cateogry description"
    };
}
```

## main.cpp
this is where ur main feature impl is, where u impl all ur hooks anyway

```cpp
#include <utils/include.hpp> // youll almost certainly need this
#include "include.hpp" // or shared.hpp if u have that

using namespace geode::prelude;
using namespace nwo5::ui::prelude; // if needed

static int sillyHelper() { // any static helpers can go above the main block
    return (long signed)"6" + 7;
}

namespace SillyFeature {
    GJRawr::someCustomMethod() { // u can js hook like this
        auto rawr = SillyFeature::coolSetting && true; // setting access should still be qualified
    }; // one space gap

    GJRawr::onSomeCallback(CCObject* pSender) {
        this->someCustomMethod(); // use this to call the hooked version of any function
    }; // three space gap



    void GJRawr::someHookedMethod() {
        GD::GJRawr::someHookedMethod(); // to call original, prefix with the gd namespace
    }

    void GJRawr::someOtherHookedMethod(bool whateverParamNameBindingsHasNoPrefix) {
        auto callback = menu_selector(SillyFeature::GJRawr::onSomeCallback); // callbacks should still be qualified
        
        GD::GJRawr::someOtherHookedMethod(whateverParamNameBindingsHasNoPrefix);
    } // five space gap





    void GJSoggy::someHookedMethod() {
        if (SillyFeature::enabled()) {
            GD::GJSoggy::someHookedMethod();
        }
    } // five space gap





    void Feature::onEditor() {
        auto self = editor::ui<SillyFeature::EditorUI>(); // still qualified, can also be editor::layer if that would work better ig
    }
```

## extra files
rawr

### shared.hpp
if u want to make a function a different feature can use, make a `shared.hpp` file

```cpp
#pragma once

#include "include.hpp"

namespace sillyedit::shared {
    void someFunctionRawr();
}
```

and impl inside a `shared.cpp` file (which doesnt have strict formatting rules just look at the other `shared.cpp` files in the project ig)

### utils.hpp
if u declare any structs or classes or smth, make a `utils.hpp`

```cpp
#pragma once

namespace SillyFeature {
    struct Rawr {
        int rawr = 12345;
        bool isRawr = true;
    };
}
```

include `utils.hpp` in `feature.hpp`

u can impl stuff in `utils.hpp` wherever other than `main.cpp` preferably, like u can impl in `utils.cpp` or just split impl across multiple files (hell if ur feature is primarily ui most of it will be defined outside of `main.cpp` anyway as thats mainly for hooks)

impl for utils also doesnt have strict formatting rules