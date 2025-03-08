## My experience with this project

Minishell is arguably regarded as the most difficult team project in the 42 Core Program. Personally I, (Sheldon), found that this difficulty stems from 3 main aspects:
1. Edge-cases: As minishell's aim is to imitate the shell, it means there are many edge cases that need to be handled. Fixing one case often causes another case to break, and my coding experience involved juggling and picking between different solutions. 
2. Integration: Minishell consists of two distinct parts, parsing the user input, and executing the user input. Due to C’s rigidity and lack of high-level abstractions, integrating these parts seamlessly was complex and led to a number of bugs.
3. Team Communication: Parsing and execution are two components that function very differently, and tranferring parsed input to execution portion posed a notable challenge for me. Clear communication was essential. Those working on parsing needed to structure the data correctly for execution, while those handling execution had to anticipate how the parsed input would be formatted and delivered.
To counter the third issue, I had already planned my project and data structures in advanced, which I summarized using a graph
