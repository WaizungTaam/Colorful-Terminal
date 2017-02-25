Colorful Terminal
=================

This is C++ library for colorful terminal output, supporting Linux and Windows.

Example:
```
std::cout << terminal::bold << terminal::bg::green << terminalfg::red 
          << "Hello World"
          << terminal::reset
          << std::endl;
```

