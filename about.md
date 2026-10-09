# <c-FF9494>s</c><c-FFDB94>i</c><c-DBFF94>l</c><c-94FF94>l</c><c-94FFDB>y</c><c-94DBFF>e</c><c-9494FF>d</c><c-DB94FF>i</c><c-FF94DB>t</c>
editor mod that adds **over 2 dozen** features all throughout the editor, my best geode project forever <cr>\<3</c>

this isnt a sweeping editor overhaul mod, most features are pretty small, but should still make ur life a lil easier

## Disclaimer(s)
> *i make* ***literally no promises as to when this mod gets updated or what gets added/removed***, *i develop this mod completely for fun and with my own needs in mind first*

> ***betteredit is unsupported***, *it might still kinda work, but still keep in mind i am not trying to support betteredit at all and anything regarding that will be ignored, sillyedit already adds a few betteredit features and tinker (which is supported) adds pretty much the rest of them so*

## Features
### Settings
> ![ℹ️](frame:GJ_infoIcon_001.png?scale=0.5) <cl>no settings search yet but that will come soon !</c>

press `ctrl+shift+p` or the **sillyedit button** in editor pause to open settings for all features

all features are listed in a panel on the left, settings for the selected feature are on the right
- if the current feature has any keybinds, the **keybinds button** in the top right will be highlighted
- options to open save/config dir are in the top menu

all features are togglable and pretty much everything about them is customizable, every setting cell may have these decorations on the top right of them
- ![🔄](frame:geode.loader/update.png?scale=0.35) requires some kind of reload to (fully) take affect, click or hover for more info
- ![ℹ️](frame:GJ_infoIcon_001.png?scale=0.45) setting description, click or hover to read
- ![↪️](frame:geode.loader/reload.png?scale=0.35) reset setting to default value, click to activate

general category has settings that affect the settings menu or multiple features, aswell as letting you see all keybinds for the mod

### Tools
#### Ruler
select 2 or more objects and see the center/size of the space between them

press `ctrl+m`, or the **ruler button** in the edit menu with 2 or more objects selected to activate
press `ctrl+shift+m`, or the button with no objects selected, to remove last measurement

#### Flood Fill
> ![ℹ️](frame:GJ_infoIcon_001.png?scale=0.5) <cl>this behaviour is subject to change and feedback is appreciated as this feature is unfinished</c>

quickly fill a rect of objs with a keybind or grid fill any shape

press `k`, or the **floodfill button** in the edit menu with 2 or more objects selected to fill

when fill is triggered, the following conditions are checked, and objects are filled accordingly
- if less than 2 objects are selected, do nothing
- if exactly 2 objects (of the same id) are selected, fills the area as a rectangle, copying the first object selected's properties
- if all selected objects but 1 are of the same id, that different id object will be the center and area will be flood filled, copying the center object's properties
- if all selected objects are the same id, area will be flood filled at the center of the selection, copying the first object selected's properties

#### Zoom Input
> ![⚠️](frame:geode.loader/info-warning.png?scale=0.5) <cs>might overlap with tinker grid controls at low zoom levels</c>

have your current zoom displayed at the top of the screen, and also input whatever zoom you want

this feature also provides keybinds for zooming in and out and a constrain editor position bypass (bypass off by default)

#### Context Menu
> ![🖥️](nwo5.sillyedit/server-icon.png?scale=0.15) <cd>desktop only</c>

right click on objects or the background to open a context menu !

options currently available are:
- Edit Group
- Edit Object
- Edit Extras
- Edit Special
- Create Mode
- Edit Mode
- Delete Mode
- Flip X
- Flip Y
- Scale
- Transform
- Select All
- Deselect All
- Delete
- Copy
- Paste
- Paste State
- Duplicate
- Toggle Invisible
- Go To Layer

#### Better Select All
> ![ℹ️](frame:GJ_infoIcon_001.png?scale=0.5) <cl>ui will be revamped as its kinda unclear rn</c>

select all button now opens a popup letting you select objects in any direction

on the bottom right, green button toggles using select filter, blue button toggles the center being the center of the screen or center of selected objects

### Interface
#### Better Scale
> ![⚠️](frame:geode.loader/info-warning.png?scale=0.5) <cs>overrides tinkers scale *input*, all other tinker scale modifications work fine tho</c>

scale input, but also some other cool stuff like scale/positioning customization and more colorful textures :3

above inputs there are **scale shortcuts** that scale objects to some preset number

#### Better Layers
> ![⚠️](frame:geode.loader/info-warning.png?scale=0.5) <cs>overrides tinkers z layer input</c>

my attempt at z layer input, with a twist :3

every layer can now:
- be hidden
- be focused
- have a custom opacity

click the settings cog in the layer menu to change settings for the current layer, config saves per level !

#### Object Tab Icons
makes object tab icons prettier :3

#### Better Edit Menu
> ![ℹ️](frame:GJ_infoIcon_001.png?scale=0.5) <cl>open to feedback, design might not be final</c>
my take on the edit menu

movement and rotation are unified into one set of buttons, amount can be inputted or pasted with shortcut buttons

additionally, scale can be locked, and flip x/y is also under movement input

#### Hide UI
> ![⚠️](frame:geode.loader/info-warning.png?scale=0.5) <cs>overlapping functionality with tinker</c>

> ![ℹ️](frame:GJ_infoIcon_001.png?scale=0.5) <cl>will be removed in the next gd update</c>

fucking guess

### Utility
#### Default Object Options
configure objects to have certain properties when placed

for most people, enabling nofade/noenter/noglow might be enough, but there is far more advanced customization also available in the form of json :3

create a json according to [this template](https://github.com/Nwo5-trg/Silly-Edit/blob/main/examples/default-object-options.jsonc) and place it in `[config-dir]/object-options.json`, json allows you to edit object props individually and append object strings

#### Selection Utils
> ![⚠️](frame:geode.loader/info-warning.png?scale=0.5) <cs>colors might have some quirks with tinker, but nothing too intrusive</c>

a handful of little selection tweaks !
- custom grid snap size with a preview of snap position
- custom select color and selection box (with chroma :3)
- clicking an object while other objects are selected, deselects all other than the object clicked (disabled by default)
- clicking on empty space deselects all objects (disabled by default)

#### Easing Preview
> ![ℹ️](frame:GJ_infoIcon_001.png?scale=0.5) <cl>will **not** be removed in the next gd update, instead, it will be reworked slightly</c>

show animated previews of easing by clicking the **easing label** in a trigger popup (stolen by robtop >:3)

the popup has 2 modes, animated and graph
- animated shows an eased animation of block being transformed
- graph mode draws easing as a curve

#### Setup Startpos
> ![ℹ️](frame:GJ_infoIcon_001.png?scale=0.5) <cl>arrow triggers dont exist, what are those ? never heard of them</c>

place start positions try and guess speed/gamemode/etc... on place

#### Text Object Utils
hacky utils to make adding newlines to text easier

also includes kerning input and copy/paste buttons for text

#### Scroll Groups
> ![🖥️](nwo5.sillyedit/server-icon.png?scale=0.15) <cd>desktop only</c>

hold `shift` and scroll your mouse over a trigger to change its target group

hold `shift+ctrl` to change center group instead

### Overlay
#### Trigger Indicators
> ![ℹ️](frame:GJ_infoIcon_001.png?scale=0.5) <cl>might be laggy on high object labels, *dont complain*</c>

draw lines from a trigger to all of its targets

30+ settings to customize to your hearts content

#### Group Label Shenanigans
> ![ℹ️](frame:GJ_infoIcon_001.png?scale=0.5) <cl>might be laggy on high object labels, *dont complain*</c>

adds group labels to all the objects robtop forgot to add group labels too, in quite the silly way

some triggers are now capable of displaying **2+ groups** on their labels, like rotate trigger showing `3/4` for target and center

some triggers also come with **little dots** on them to show things like if static camera trigger is set to follow mode :3

all of this is data-driven btw so tinker to your hearts content in `[config-dir]/group-labels.json`, the template can be found [here](https://github.com/Nwo5-trg/Silly-Edit/blob/main/examples/group-label-shenanigans.jsonc)

#### Trigger Type Boxes
have you ever wondered why touch triggered objects have an outline around them but spawn triggered objects dont ? no ? well i have

this feature adds circles around spawn triggered objects, and a secondary faded outline around multi triggered objects

also u can make them chroma !

#### Place Object Preview
> ![🖥️](nwo5.sillyedit/server-icon.png?scale=0.15) <cd>desktop only</c>

> ![⚠️](frame:geode.loader/info-warning.png?scale=0.5) <cs>overlapping functionality with tinker</c>

> ![ℹ️](frame:GJ_infoIcon_001.png?scale=0.5) <cl>will crash 100% guarantee :3 /halfjoking</c>

faded preview of object at your cursor before you place it

*this fucking feature took me so long because of so many dumb bugs so idegaf anymore*

### Miscellaneous
#### Silly Keybinds
random keybinds for the editor that dont really have anything to do with other features

right now these are the keybinds included:
- `n` - toggle ignore damage
- `j` - toggle object invisible (if you use alpha group, check out general setting "invisible with group")
- `ctrl+r` - restart playtest

#### Editor Time
show a label with how much time youve spent in the editor (kinda like the fps label)

#### Fixes
> ![⚠️](frame:geode.loader/info-warning.png?scale=0.5) <cs>some of these are also functionality in tinker but they should work fine together</c>

some misc fixes (credit to alpha for most of these)
- fix area corruption
- fix object info label (make it show properly on single select)
- fix ignore damage wave
- allow shift change modes (this ones dumb)

#### Hide With Playtest
editor only objects get in your way during playtesting ? no more !

#### Copy Paste Object Strings
> ![⚠️](frame:geode.loader/info-warning.png?scale=0.5) <cs>disables tinkers hooks >w<</c>

> ![ℹ️](frame:GJ_infoIcon_001.png?scale=0.5) <cl>disabled by default</c>

iykykiydktdtc

#### Template
> ![ℹ️](frame:GJ_infoIcon_001.png?scale=0.5) <cl>this feature could be expanded upon prolly, not sure if i will tho</c>

select a level to be a **template**, all newly created levels will copy this template

the ***template button** is located in `level settings > level options` (the popup with spawn group input), just click the button and the current level will be saved as the template

## Credits
### Special Thanks
#### Alpha
- made tinker
- replace obj impl
- setting popup inspo
- reverse engineering most of editor ui
- help with their api
- answering some dumb questions
- some code >:3

#### Ery
- geode gremlin
- pr for obj tab icons
- permission to use them as assets
- prolly accepting this mod

#### HJFod
- made better edit
- let me steal a bunch of stuff
- let me have a bunch of other stuff

### Credits

#### gdjayy
- replace object suggestion

#### CreatorCreepy
- feedback for replace object
- feedback for floodfill

#### CarlIsBored
- trigger id search suggestion

#### like all the hosts of cornbread megacollab
- better select all suggestion

#### Doranell
- text obj utils suggestion

#### DasshuDev
- copy particle string idea