# CPU Acrhitecture

## General information

<!-- TODO: Clean up awkward wording. Explain the relationship between the static array and the stack pointer more thoroughly. -->
The main grunt of the work done on the CPU is done through the stack.
The stack consists of two elements: A static array of 256 16-bit values, accompanied by a
stack pointer, used to keep track of where the top-most value on the stack lies.
The value of the stack pointer cannot be made known to the user by means of the usage of and valid
opcodes, it is instead only important to note the existence of the stack pointer when attempting to
emulate the functionality of this CPU.


## Instruction set

### Meta operations

| Instruction | Opcode | Signature      | Description |
|-------------|--------|----------------|-------------|
| nop         | 0x00   | --             | No-op. Does nothing. |

### Stack operations

<!-- TODO: Use signature labels instead of stack values of relative positions in descriptions -->
| Instruction | Opcode | Signature      | Description |
|-------------|--------|----------------|-------------|
| push8       | 0x10   | -- a           | Pushes an 8-bit value onto the stack. |
| push16      | 0x11   | -- a           | Pushes a 16-bit value onto the stack. |
| drop        | 0x12   | a --           | Pops the top-most value off of the stack. |
| dup         | 0x13   | a -- a a       | Duplicates the top-most value on the stack. |
| swap        | 0x14   | a b -- b a     | Swaps the positions of the two top-most values on the stack. |
| over        | 0x15   | a b -- a b a   | Copies the value directly below the top-most value on the stack, pushing it to the top of the stack. |
| rot         | 0x16   | a b c -- b c a | Rotates the order of the three top-most values clockwise. |
| store8      | 0x17   | a mem --       | Stores 8-bit value `a` at memory address `mem`. |
| store16     | 0x18   | a mem --       | Stores 16-bit value `a` at memory address `mem`. |
| load8       | 0x19   | mem -- a       | Loads 8-bit value from memory address `mem`, pushing it to the top of the stack. |
| load16      | 0x1A   | mem -- a       | Loads 16-bit value from memory address `mem`, pushing it to the top of the stack. |

### Arithmetic operations

<!-- TODO: Use signature labels instead of stack values of relative positions in descriptions -->
| Instruction | Opcode | Signature      | Description |
|-------------|--------|----------------|-------------|
| add         | 0x20   | a b -- c       | Pushes the sum of the two top-most values on the stack onto the stack. |
| sub         | 0x21   | a b -- c       | Pushes the result of subtracting the top-most value on the stack from the value below it onto the stack. |
| mult        | 0x22   | a b -- c       | Pushes the result of multiplying the two top-most values on the stack together onto the stack. |
| div         | 0x23   | a b -- c       | Pushes the result of dividing the value directly below top-most value on the stack by the value above it onto the stack. |
| mod         | 0x24   | a b -- c       | Pushed the remainder of the two top-most values on the stack onto the stack. *(See div instruction)* |

### Bitwise operations

| Instruction | Opcode | Signature      | Description |
|-------------|--------|----------------|-------------|
| and         | 0x30   | a b -- c       | Performs a bitwise-and operation on values `a` and `b`, pushing the result of the operation to the top of the stack. |
| or          | 0x31   | a b -- c       | Performs a bitwise-or operation on values `a` and `b`, pushing the result of the operation to the top of the stack. |
| not         | 0x32   | a b -- c       | Performs a bitwise-not operation on values `a` and `b`, pushing the result of the operation to the top of the stack. |
| xor         | 0x33   | a b -- c       | Performs a bitwise-xor operation on values `a` and `b`, pushing the result of the operation to the top of the stack. |
| shiftl      | 0x34   | a n -- c       | Shifts bits in value `a` to the left `n` times, pushing the result of the operation to the top of the stack. |
| shiftr      | 0x35   | a n -- c       | Shifts bits in value `a` to the right `n` times, pushing the result of the operation to the top of the stack. |

### Logical operations

| Instruction | Opcode | Signature      | Description |
|-------------|--------|----------------|-------------|
| and         | 0x40   | a b -- c       | Performs a logical and operation on values `a` and `b`, pushing the result of the operation to the top of the stack. |
| or          | 0x41   | a b -- c       | Performs a logical or operation on values `a` and `b`, pushing the result of the operation to the top of the stack. |
| not         | 0x42   | a b -- c       | Performs a logical not operation on values `a` and `b`, pushing the result of the operation to the top of the stack. |

### Comparison/Branching operations

#### Result of comparison operation `cmp`

- Bit 0: Equal to
- Bit 1: Greater than
- Bit 2: Less than

| Instruction | Opcode | Signature      | Description |
|-------------|--------|----------------|-------------|
| cmp         | 0x50   | a b -- c       | Compare values `a` and `b`, pushing the resulting information to the top of the stack. |
| jmp         | 0x51   | --             | Jumps to constant location. |
| jeq         | 0x52   | a --           | Jumps to constant location if `a`'s equal bit is high. |
| jne         | 0x53   | a --           | Jumps to constant location if `a`'s equal bit is low. |
| jlt         | 0x54   | a --           | Jumps to constant location if `a`'s less-than bit is high. |
| jle         | 0x55   | a --           | Jumps to constant location if `a`'s less-than or `a`'s equal bit are high. |
| jgt         | 0x56   | a --           | Jumps to constant location if `a`'s greater-than bit is high. |
| jge         | 0x57   | a --           | Jumps to constant location if `a`'s greater-than bit or `a`'s equal bit are high. |