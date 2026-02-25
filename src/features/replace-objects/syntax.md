# very dumb extra language for query (and replace)
oki so this is like my reimpl of more select filters and instead of doing the (probably better) ui approach i did last time, i decided to make a silly little langauge for ur queries !

## operators
if uve programmed in like literally anything ever (meaning in a c based language bcs no other programming languages exist totally) thisll prolly look familiar 

- `!` : not operator : `![condition]`
- `&&` : and operator : `[condition] && [condition]`
- `||` : or operator : `[condition] || [condition]`

operator precedence is the same two (and if u dont know it js look at the order the operators r listed in ;3)

## conditions
conditions r how u actually check for stuff

syntax is `[data][identifier]`, so if u were checking for gid 2, it would be `2g` (g being the identifier for group)
if ur data needs to b some string instead of a number (i dont think if impl anything that would require string data yet but oh well) wrap the data in `{}`

currently conditions r
- `g` : group : gid -1 will be true only if obj has no groups, gid 0 will be true if the obj has any groups
- `o` : obj id
- `t` : target group : gid 0 will select any trigger
- `c` : center group : gid 0 will select any trigger
- `i` : item id : item 0 will select any trigger
- `b` : base/primary color channel
- `d` : detail/secondary color channel

## other stuffs
parentheses r supported

to input obj ids js click the button to the left of either input and it adds the selected obj id(s) to the end

whitespace gets ignored almost everywhere

## replace
i dont wanna write better replace docs rn but uhh

replace syntax is basically `[lhs][action operator][rhs],...`

replace actions r
- `=>` : set : *lhs* becomes *rhs*
- `>>` : add : *lhs* becomes *lhs* + *rhs*
- `<>` : sub : *lhs* becomes *lhs* - *rhs*

the lhs is data/identifiers just like in queries (most of the same identifiers are supported), but rhs shouldnt include a identifier

lhs data can also be `.` to apply to all objs that the identifier can support (u can figure out however i decided to impl this per identifier by urself)

## obj string shenanigans
replace also supports special syntax for obj strings

they use the `s` identifier and takes string data on the rhs

replace actions r implemented kinda differently here so js to clarify
- `=>` : set : completely overrides the obj string with *rhs* (unless determined to not be a valid obj string then does nothing)
- `>>` : add : appends *rhs* to the end of the obj string (automatically adds a comma so u dont need to do that)
- `<>` : sub : implemented as literally "str replace `,[rhs]` with ` `"

btw no guarantees u dont crash ur game fw these im not responsible for that

## examples
explaining my dumb syntax with visuals !

### query
`2g || (1o && -1g)`

any objs with group 2 or objs of obj id 1 with no groups
### replace
`.g >> 5, 11o => 12`

increase all groups (other than parent groups) by 5 and replace all objs of obj id 11 with objs of obj id 12
i *might* do parent groups l8r but it feels like those would be annoying so not now

`.s >> {134,1}`

add `134,1` to the end of the obj string of all objs

(obj strings are special little girls who always interpret lhs as `.` :3)