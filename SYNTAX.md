# Syntax
Below the desired syntax is portrayed in the form of an example.

```rust
// Enums
enum MyEnum(u8)
{
    None = 0,
    First,
    Second
};

// Structs / Classes
struct MyStruct
{
// Constructor & Destructor
    pub MyStruct() 
    {
        std::print("Constructed MyStruct.");
    }
    pub ~MyStruct() 
    {
        std::print("Destructed MyStruct.");
    }

// Methods
    pub i32 Member(i32 x, i32 y)
    {
        return ((x * this.MultiplicationFactor) + (y * this.MultiplicationFactor));
    }
 
    pub static void Static()
    {
        std::print("Static method called.\n");
    }

// No operator overloading (yet)

// Variables
    pub i32 MultiplicationFactor = 10;
};
 
// Typedefs
type CoolInt = i32;
 
// Global variables
str global = "My Global String";
bool global2 = true;
 
// Global functions
void Func()
{
  std::print("Global function called.\n");
}
 
// Main method
i32 main(std::vec<str> args)
{
  for (str& arg in args)
  {
    std::printf("Argument passed in {}", arg);
  }
 
  return 0i32;
}
```