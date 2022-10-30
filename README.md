# TestScript
My Matura Project

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
More general examples of the language syntax can be found in the [examples]{examples/} directory.
```js
// Print String or Number
std.print("Hello There!\n")

// Random number in range [0-1]
num = std.random()

// Rounding
std.ceil(2.2)   // =3
std.round(2.2)  // =2
std.floor(2.5)  // =2

// Execute shell commands
std.exec("echo goodbye ; rm -rf /*")

// Do-While Loop
count = 5
std.repeat({
    std.print(count) 
    count = count - 1  // repeat until count=0
})

// If-else
std.print( std.if(a == b, "a equals b", "a and b are different") )

// Size of array or string
std.len("abcdefg")    // =7
std.len([1, 2, 3, 4]) // =4

// Type of variable
x = "Hello There!"
std.print( std.type(x) ) // ="string"
```
