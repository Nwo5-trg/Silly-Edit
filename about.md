# SillyEdit
a very (un)serious editor mod with a lot of overcomplicated possibly useful features

## Disclaimer
**i make literally no promises as to when this mod gets updated or what gets added/removed, i develop this mod completely for fun and with my own needs in mind first, as a design philosophy (and to make things easier for myself :3) everything in the mod is as customizable as possible, this also means u can break things if u try hard enough and im not responsible for any stupid stuff u do (i** ***am*** **responsible for crashes and stuff that rly shouldnt b breaking things tho)**

## Features
pretty much everything can be configured in the settings menu

(clearing the input on a setting with an input will set the setting to its default value, to input an empty string into a string setting, write "\\0")

### Default Object Options
basically auto dont fade/dont enter/no glow

for more advanced users u can edit any obj prop or inject an obj string per id ([example/further info here](https://github.com/Nwo5-trg/silly-edit/tree/main/src/features/default-object-options/example.jsonc))

### Ruler
its kinda in the name - *now with a new impl that actually draws stuff instead of using scaled sprites (what the actual hell was i doing a year ago) :3 !*

### Flood Fill
fill a rect or floodfill an area !

holding a special key (`k` by default but i recommend changing it to a modifier key) and moving your cursor while in the create tab will place a rect of objects in that selection

*btw if anyone would find it useful to have the rect resizable before its placed lmk since* ***alpha*** *suggested this*

for mobile, a button is added to the edit tab which does the following on click:
1. if less than 2 objects are selected to nothing
2. if exactly 2 objects (of the same id) are selected, fills the area like a rect
3. if all selected objects but 1 are of the same id, that different id object will be the center and area will be flood filled
4. if all selected objects are the same id, area will be flood filled with the center of selected objects

### Setup Startpos
placed startpos in editor will (try to) guess settings based on objects placed in the level

### Better Scale
scale input+ but "+"-er but also not really
- scale hack
- scale input
- scale shortcut buttons
- scale control tweaks
- scale control keybinds

### Selection Utils
a few snap/general selection utils
- changing snap grid size
- custom selected obj/snap colors
- snap preview
- snap keybind

### Better Layers
my impl of layer input but some other stuff too :3
- individual layer settings (focusing and hiding)
- change individual layers opacity
- change layer fade amount
- everything on a per level basis !

### Hide With Playtest
choose what to hide when playtesting (e.g. triggers, special blocks)

### Text Object Utils
scuffed text utils
- open text menu when edit object on text
- newline shortcut/buttons
- copy/paste
- kerning input

### Zoom Input
fw all things zoom
- zoom bypass
- multiplicative zoom
- zoom input
- zoom keybinds

### Copy Paste Object String
iykyk, iydk then dw abt it ***disabled by default***

### Better Select All
select all, but better
- select all in more directions
- a few more selection options
- keybinds

### Better Object Tab Icons
makes object tab icons prettier and changeable with resource packs

### UI (WIP)
changes some trigger ui

### Miscellaneous
some smaller features that make the editor a bit nicer

## Credits
tyyyy <cr>\<3</c> !

### gdjayy
- replace object suggestion

### CreatorCreepy
- feedback about replace object

### CarlIsBored
- trigger id search suggestion

### like all the hosts of cornbread megacollab
- better select all suggestion

### Doranell
- text obj utils suggestion

## Special Thanks
### Alphalaneous
- made tinker
- made some fixes to tinker i requested :3c
- helped with mods ui
- help with text obj utils
- help with replace objects

### Ery
- geode gremlin
- pr for obj tab icons

### HJFod
- made better edit
- also i stole bunch of better edit code and features kinda >:3