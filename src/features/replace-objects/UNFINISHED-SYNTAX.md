# new syntax test
## constants
num
`1`
var
`[identifier]`
auto var (i.e. if obj id action then obj id)
`.`
random (random int 1,2 or 3)
`1~3`
string
`{rawr}`

## basic action
syntax
`[identifier][op][arg],[...]`
binary op syntax (no chaining only up to 1 binary operation)
`[identifier][op][lhs][binary op][rhs]`
operators
- `=` set
- `+` add
- `-` sub
- `*` mul
- `/` div
- `%` mod

## groups
set objects groups to 1
`g={1}`
adding group 1 to object
`g+1`
removing group 1 from object
`g-1`
changing group 2 to group 1
`2g=1`
changing group 2 by 1
`2g+1`
changing group 2 by -1
`2g-1`
adding 1 to all groups
`.g+1`
subtracting 1 from all groups
`.g-1`
all other operators are errors but does support binary op

## strings
concatenates object str
`s+{1,10}`
all other operators are errors and does not support binary op

