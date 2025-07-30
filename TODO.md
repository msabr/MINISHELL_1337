minishell$ qwerty
minishell: qwerty: command not found
minishell$ $Space
minishell$ $?
minishell: 127: command not found
`---------------------------------------``
minishell$ $vertical_tab
minishell$ $?
minishell: 0: command not found
minishell$ 
---------------------------------------
minishell$ echo $""

minishell$ echo $''
$
minishell$ 
---------------------------------------
minishell$ echo $Tab$Space | cat -e
        $
bash$ echo $Tab$Space | cat -e
$
---------------------------------------
minishell$ < Libft
minishell$ echo $?
1
---------------------------------------
minishell$ echo 'a'$S'b'
ab
---------------------------------------
minishell$ exit ''
exit
➜  mini4 git:(master) ✗ $?
zsh: command not found: 0
---------------------------------------
minishell$ <<a
minishell: -302734508heredoc: No such file or directory
---------------------------------------
