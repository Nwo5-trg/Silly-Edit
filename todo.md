# things i need to do before index release
- soggy and eri setting button

- fix ui camera mode thing
- a btn to copy particle string to clipboard

- kerning label/input
- test multi text edit
- make sure empty text edit sets label to a
- separate kerning input update function from slider changed

- rewrite that one flood fill feature to actually drag 

- trigger group scrolling

- selection box customization for selection utils

- make all keybinds togglable

- add compat feature in core that lets you open ui scale/better trail settings

# things i need to do before full release
- add drag + modifier to fill rect thing to flood fill (holy fuck this will b so good)
- finish ui feature
- make sure everything disables in the best way possible
- add better move menu
- make keybinds button scroll u to the correct category
- add the custom filters system in whatever way i eventually decide
- group label shenanigans
- get off my ass and reverse engineer so i dont have to create objects twice for default object options

# chores i should get around to sometimes but arent rly important
- trans theme and mayb some other pride themes for settings popup (this also means properly scaling sidebar)
- refactor ui code to use new layout stuff i added so i can leverage them more (especially replacing deprecated horizontal/verticaldistriblayout)
- refactor ui code to be more nested (i finally realised my old philosophy of not nesting ui code was actually bitting readability and my overall experience writing ui in the ass)
- refactor whatever the fuck is going on in ruler
- execute magic numbers from ui feature code as much as possible (ie balance my perfectionism with my sanity)
- add proper this-> access to all called member functions (and accessed members in lambdas)
- mayb some uniform system for select filters throughout the mod cuz a few features use that alrdy
- add searching settings mayb
- whenever 2.209 comes around impl some of sillysetting into savedsetting (like platform specific and enable if schemes maybbb)
- mayb actually subnamespace shared idk