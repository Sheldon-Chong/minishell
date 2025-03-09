# Minishell
Developing a unix shell in C, which has to support command execution, pipes, redirections, environment variables, and built-in commands. This project deepened my understanding of process management, system calls, and shell behavior, mirroring features of bash.

## 📖 Concepts covered in minishell
- forking, waiting, and killing processes
- execution of commands using relative and absolute paths (path resolution)
- redirections, communicating-between processes
- lexing, tokenization, Abstract syntax tree
- environment variables
- builtin commands like `cd`, `echo`, `env`
- signals

## ⬇️ Clone

```
git clone git@github.com:Sheldon-Chong/minishell.git
```

## ⏯️ Compile and run

(cd into repo)

```zsh
make
./minishell
```


## My experience with this project

Minishell is arguably regarded as the most difficult team project in the 42 Core Program. Personally I, (Sheldon), found that this difficulty stems from 3 main aspects:
1. Edge-cases: As minishell's aim is to imitate the shell, it means there are many edge cases that need to be handled. Fixing one case often causes another case to break, and my coding experience involved juggling and picking between different solutions. 
2. Integration: Minishell consists of two distinct parts, parsing the user input, and executing the user input. Due to C’s rigidity and lack of high-level abstractions, integrating these parts seamlessly was complex and led to a number of bugs.
3. Team Communication: Parsing and execution are two components that function very differently, and tranferring parsed input to execution portion posed a notable challenge for me. Clear communication was essential. Those working on parsing needed to structure the data correctly for execution, while those handling execution had to anticipate how the parsed input would be formatted and delivered.

To counter the third issue, I had already planned my project and data structures in advance, which I summarized using a flowchart (see below):

The behaviour of a shell is very intricate, and I found it difficult to cover each in detail. To ensure we had scraped every corner, we got others to test out our program, and compiled numerous test cases to ensure that all parts of our program worked, and followed bash's shell as closely as possible.

A lot of cadets attempt minishell by storing data in a structure known as Abstract-syntax-tree, which is a recursive and hierarchical method for storing parsed data. For me however, I utilized my own method for parsing and executing data, using a linear linkedlist, as I found it was a more straightforward to navigate tokens more easily.

