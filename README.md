# TestScript
My Matura Project

Attention: Do not use this programming language interpreter
if you don't know what you are doing. This interpreter
is not finalized, suffers from meory-leaks and is
not made to be used in a professional environment.

## Build Requirements:
* GNU Make
=> https://www.gnu.org/software/make/
* GCC
=> https://gcc.gnu.org/

## Build instructions
```sh
# Run in (git)bash:
git clone https://github.com/LeFlorians/TestScript
cd TestScript

# Build the executable
make

# Build the executable with debug flags and run it against ./test.nts
make test

# Clean build files
make clean

# Run the final executable
./TestScript [source_file.nts]

```

## Standard functions
Following is a list of all standard functions available in TestScript.
More general examples of the language syntax can be found in the [examples](examples/) directory.
```js
// Print string or number
std.print("Hello There!\n")

// Get a random number in the range [0-1]
num = std.random()

// Rounding numbers
std.ceil(2.2)   // =3
std.round(2.2)  // =2
std.floor(2.5)  // =2

// Execute shell commands
std.exec("echo goodbye ; rm -rf /*")

// Do-While Loops
count = 5
std.repeat({
    std.print(count) 
    count = count - 1  // repeat until count=0
})

// If-else branching
std.print( std.if(a == b, "a equals b", "a and b are different") )

// Size of an array or string
std.len("abcdefg")    // =7
std.len([1, 2, 3, 4]) // =4

// Get the type of a variable
x = "Hello There!"
std.print( std.type(x) ) // ="string"
```
