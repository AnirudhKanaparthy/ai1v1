# ai1v1
AI algorithms for 1 vs 1 player games (2 player adverserial games). Currently only has [Tic-tac-toe](https://en.wikipedia.org/wiki/Tic-tac-toe) implemented. More to come soon...

## Requirement
C++ compiler supporting at least C++23.

## Quick Start

```shell
# Substitute the compiler with whatever you are using (g++/clang/msvc/...)
c++ -O3 -std=c++23 ./main.cpp -o tictactoe
./tictactoe
```

You may try to change the template parameter in the `main` function to play a **4x4** board.

```diff
> auto b{T3Board<3>::create()};
< auto b{T3Board<4>::create()};
```
