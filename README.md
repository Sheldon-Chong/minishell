# QUOTATIONS

## Spaces
echo "hello "
echo " hello"
echo " hello "
echo "hello hi"
echo " hello


"
echo 
echo 	 

## Indents
echo "	hello hi	"
echo "hello	hi	"
echo "	hello	hi "

## -n
echo -n
echo -n ""

## Mixing quotations
echo "hi"
echo 'hi'
echo "h"i
echo 'h'i
echo 'h'"i"
echo "h"'i'
echo "h""i"
echo 'h''i'

## quotes within quotes

echo "hi'"
echo 'hi"'
echo "''"
echo '""'
echo "'"'"'

## Empty
echo ""''
""
''
""''

## Piping in quotes
echo "|"
echo "hi|hello"
echo " | "
echo " |"
echo "| "

## Opened quotations
"
'
echo "
echo ""'
echo ''"'

## Invalid characteres

echo (
echo "("(
echo "("

# ENVIRONMENT VARIABLES

echo $VAR
echo $
echo "$"
echo '$'

## Single quotes
echo '$VAR'
echo '$VAR'NICE
echo NICE'$VAR'NICE
echo NICE'$VAR 'NICE

## Double quotes
echo "$VAR"
echo "$VAR $VAR"
echo "$VAR hello $VAR"
echo "$VAR hello$VAR"
echo "$VAR hello$VA!"

## No quotes
echo $VAR$VAR
echo $VAR$VAR hello
echo $VAR $VAR
echo $VAR $VAR hello
echo $VAR hello $VAR hello

## Mixed
echo "$VAR"'$VAR'
echo "$VAR"$VAR'$VAR'
echo '$VAR'"$VAR"
echo '$VAR' "$VAR"
echo '$VAR' "$VAR"$VAR
echo $VAR'$VAR' "$VAR"$VAR
echo $VAR'$VAR'$VAR $VAR"$VAR"$VAR

## Ending env variable
echo $VAR!

# EXPORTING ENVIRONMENT VARIABLES

## Exporting with quotes
export "hello"'hi'
export "hi"'hello'
export "hi"hello'hello'

## Invalid cases
export !
export "hello "
export " hello"
export hi==
export hi =meme
export $HOME

## Valid cases
export _hi
export _1hello
export H1_
export __
export _

## Assigning to another env
export NICE=$HOME
export NICE=$HOME$PATH
export NICE="$HOME!$PATH"

## piping envs
export test=lol | echo $test

# REDIRECTION

echo << EOF
cat << EOF
cat << "END OF LINE"
cat << "END "OF'LINE'
cat << ""
cat << EOF << EOF2
echo hi >> lol ok

## Not allowed
cat << \ #should prbly print nothing since it specifies that it shouldn't be handled in pdf
echo >>;
echo hi <> helo
echo test>>">"
echo >>>
echo >>""
echo >>

## Multiple redirections
echo hi > file1 > file2
echo hello       >> output.txt
echo hello, >>, output.txt

# Piping

echo "hello"|"hello"
echo |"hello"
echo |"hello"
echo | echo "hi"				# NOT HANDLED PIPING
echo "hello"'hi'|'hi'|"hello"	# NOT THE SAME ERROR PRINTING ORDER

# Directories

# Executables


! HANDLE this:

echo hello >> output.txt hi  

echo hello > test.txt | echo test > hello.txt




unset path, so everything else shouldn't work anymore
export HI"="'meme'


CONSIDER ALLOWING "" as a NULL input, rather than not appending
This is because of commands like the following:
touch ""


!! HANDLE THIS:
minishell$ (
bash: syntax error near unexpected token `('



Weired behavior in bash when performing the following operation:
echo "$va"






export|cat
the order export and env is different (but according to another's minishell, not nescessary )
! "$HELO"$helo'$helo'
bash tester.sh 
echo $1 (shouldn't print anything)




Converting $$ into Env

env command doesn't print env

Change ft_strcmp to ft_strcmp
*/

//no closing brace despite all braces being closed, when copying a line with a \n
/* 
* someone elses's version of << handling
static void	redirect_input_until(t_statement *node)
{
	char	*buff;
	int		fd[2];

	pipe(fd);
	while (1)
	{
		buff = readline("> ");
		if (streq(buff, node->next->argv[0]))
			break ;
		ft_putendl_fd(buff, fd[1]);
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	free(buff);
}

! commands that need to be fixed
<
>
writing this single character to console should print the following:
"bash: syntax error near unexpected token `newline'"

! HANDLE THIS!!!!!!

"''"
'""'


get_brace status must be updated
// "echo hello" ""







## NEWEST PROBLEMS
echo hi || |
echo hi |||
echo ||
