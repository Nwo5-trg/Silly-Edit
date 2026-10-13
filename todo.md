# things i need to do before full release
- finish trigger ui feature
- make a startpos disabled extra in group label shenanigans
- make paste button clickable with copy obj strings if u have an obj string in ur clipboard
- add option to copy obj strings to interpret a single integer string as object id thats placed in center
- add compat feature in core that lets you open ui scale/better trail settings
- a btn to copy particle string to clipboard
- make sure everything disables in the best way possible
- double click to edit object (shift to edit group)
- duration and rate sliders/inputs in easing preview
- file path settings button
- add compat warnings in game not just in about
- make group label shenanigans atleast support p1/p2/c for advanced follow
- add the custom filters system in whatever way i eventually decide
- create gradient with selection button (or if nothings selected create a square of objects)

# things i can do whenever
- popup to show all z layers (or editor layers) in selection and let you change them individually (refer to shadys dms)
- port over everything to geode button lol (keep separate functions js call them in the lambda callback)
- translations mayb ??????? (no)
- clean up better select all code (this also means renaming the arrow textures to their actual names and not js numbers)
- stop using the dumb chroma node system
- reorganize mod.json keybinds to reflect feature order
- soggy and eri setting button texture
- refactor whatever the fuck is going on in ruler
- make easing buttons inheret from geode button set their id inside easing popup not in init
- mayb some uniform system for select filters throughout the mod cuz a few features use that alrdy
- mayb cache keybind enabled settings
- mayb actually subnamespace shared idk
- mayb a better system for "if key down" other than a listener + fields
- update more stuff to properly use layout anchors

# porting to 2.209
in my hacks to make keybinds popup work, i should change cclabelbmfont to geode label cuz it will be changed in geode ui byy then