# Vault Code Guessing Game

A command-line number guessing game written in C, built for GSU's CSC 3320 (Systems Programming). The program generates a random 3-digit code, and the player has 8 tries to guess it, getting feedback after each attempt on how close they are.

## How it works

Each guess gets three pieces of feedback:

- Whether the guess is too high or too low
- How many digits are correct and in the right position
- How many digits are correct but in the wrong position

Type `q` at any time to quit early and reveal the code.

 Example run

```
8 tries remaining. What is the code? 123
Too low, 0 digits are correct and in the right place, 1 digits are correct and in the wrong place.
7 tries remaining. What is the code? 456
Too high, 0 digits are correct and in the right place, 2 digits are correct and in the wrong place.
6 tries remaining. What is the code? 405
You opened the vault!
```

 How to build and run

```
gcc cardenas_mario_minigame.c -o vault_game -std=gnu89 -Wall
./vault_game
```

 Techniques used

- **Modular design** — the program is split into focused functions (`generateSecretCode`, `parseGuessDigits`, `compareGuess`, etc.) rather than one large `main()`, each with a single responsibility
- **Input validation** — guesses are checked for correct length and confirmed to be all digits before being processed, rejecting malformed input without crashing
- **Pointer-based output parameters** — `compareGuess()` returns two values through pointers (`*correctPosition`, `*correctDigitWrongPosition`) since C functions can only return one value directly
- **Counting-array algorithm** — the trickiest part of the logic. A naive comparison would double-count repeated digits (e.g. a guess of `112` against a code of `211`). This uses two frequency arrays to track how many of each digit value remain unmatched after exact-position matches are removed, so repeated digits are counted correctly on the second pass
- **Safe string handling** — uses `fgets()` with a bounded buffer instead of `gets()` or unchecked input, and manually strips the trailing newline
- **Compiled under `-std=gnu89 -Wall`** — written to an older C standard with warnings enabled, catching implicit type issues and unused variables at compile time rather than at runtime

 Background

Written as part of coursework toward a B.S. in Computer Science at Georgia State University, alongside a prior background as a U.S. Air Force aircraft electrical and environmental systems technician.
