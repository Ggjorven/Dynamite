# Syntax
Below the desired syntax is portrayed in the form of an example.

```rust
// Enums
enum MyEnum(u8)
{
    None = 0,
    First, // 1
    Second // 2
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
    pub fn Member(x: i32, y: i32) -> i32
    {
        return ((x * this.MultiplicationFactor) + (y * this.MultiplicationFactor));
    }
    
    // No operator overloading (yet)

    // Variables
    pub let MultiplicationFactor = 10i32; // Type deducing
};
 
// Typedefs
type CoolInt = i32;
 
// Global variables
str global = "My Global String";
bool global2 = true;
 
// Global functions
fn Func() -> void
{
    std::print("Global function called.\n");
}
 
// Main method
fn main(args: std::vec<str>) -> void
{
  for (let arg: str& in args)
  {
    std::printf("Argument passed in {}", arg);
  }
 
  return 0i32;
}
```