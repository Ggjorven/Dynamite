# Dynamite Programming Language

Dynamite is a new programming language based on C++, designed for creating powerful and efficient applications. This is an ongoing project that currently only supports Linux systems.

## Features

- **C++ Syntax**: Leverages familiar C++ syntax and semantics.

## Syntax

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

## Installation

### Prerequisites

- A Linux operating system
- C++20 compatible compiler (e.g., g++ 10 or later)
- NASM & ld (gdb)

### Build Instructions

1. Clone the repository:
    ```bash
    git clone https://github.com/Ggjorven/Dynamite.git
    cd Dynamite
    ```

2. Navigate to the scripts folder:
    ```sh
    cd scripts/linux
    ```

3. (Optional) If you haven't already installed the premake5 build system you can install it like this:
    ```sh
    chmod +x install-premake5.sh
    ./install-premake5.sh
    ```

4. Generate make files:
    ```sh
    chmod +x gen-make.sh
    ./gen-make.sh
    ```

## License
This project is licensed under the Apache 2.0 License. See [LICENSE](LICENSE.txt) for details.

## Contributing
Contributions are welcome! Please fork the repository and create a pull request with your changes.

## Acknowledgments
This project is inspired by various programming languages and aims to create a simple yet powerful tool for developers. Special thanks to the tutorial series on [YouTube](https://youtube.com/playlist?list=PLUDlas_Zy_qC7c5tCgTMYq2idyyT241qs&si=w_2uPn_QRp0VL7LL) that guided the development of this language.

## Third-Party Libraries
  - `spdlog` [https://github.com/gabime/spdlog/](https://github.com/gabime/spdlog.git)
