# Minishell - 42 Network

<div align="center">
<img src="minishell.png" with="600"/>
</div>

A minimal shell implementation in C that recreates basic bash functionality as part of the 42 School curriculum.



## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Implementation Details](#implementation-details)
- [Built-in Commands](#built-in-commands)
- [Testing](#testing)
- [Known Issues](#known-issues)
- [Authors](#authors)

## 🎯 Overview

Minishell is a simplified shell implementation that provides a command-line interface similar to bash. This project demonstrates understanding of:

- Process management and system calls
- Lexical analysis and parsing
- Signal handling
- File descriptor manipulation
- Memory management
- Inter-process communication

## ✨ Features

### Core Functionality
- **Interactive command prompt** with custom banner
- **Command execution** with PATH resolution
- **Pipe support** for command chaining (`|`)
- **Redirections**: 
  - Input redirection (`<`)
  - Output redirection (`>`)
  - Append redirection (`>>`)
  - Here-documents (`<<`)
- **Variable expansion** (`$VAR`, `$?`)
- **Quote handling** (single and double quotes)
- **Signal handling** (Ctrl+C, Ctrl+\, Ctrl+D)

### Built-in Commands
- `echo` (with `-n` flag)
- `cd` (with relative/absolute paths)
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

### Advanced Features
- **Environment variable management**
- **Exit status tracking** (`$?`)
- **Syntax error detection**
- **Memory leak prevention**
- **History support** (via readline)

## 🔧 Prerequisites

- **GCC** or compatible C compiler
- **GNU Readline library**
- **Make**
- **macOS** (for brew-based readline installation)

## 🚀 Installation

1. **Clone the repository:**
   ```bash
   git clone git@github.com:msabr/minishell.git
   cd minishell
   ```

2. **Install readline (if not already installed):**
   ```bash
   make install-readline
   ```

3. **Compile the project:**
   ```bash
   make
   ```

4. **Run minishell:**
   ```bash
   make run
   # or
   ./minishell
   ```

## 💻 Usage

### Basic Commands
```bash
minishell$ echo "Hello, World!"
Hello, World!

minishell$ pwd
/path/to/current/directory

minishell$ ls -la | grep .c
# Lists all .c files in current directory
```

### Redirections
```bash
minishell$ echo "content" > file.txt
minishell$ cat < file.txt
content

minishell$ echo "more content" >> file.txt
minishell$ cat file.txt
content
more content
```

### Here-documents
```bash
minishell$ cat << EOF
> line 1
> line 2
> EOF
line 1
line 2
```

### Variable Operations
```bash
minishell$ export MY_VAR="hello"
minishell$ echo $MY_VAR
hello

minishell$ echo $?
0
```

## 📁 Project Structure

```
minishell/
├── Libft
│   ├── Makefile
│   ├── ft_atoi.c
│   ├── ft_bzero.c
│   ├── ft_calloc.c
│   ├── ft_free.c
│   ├── ft_is_number.c
│   ├── ft_isalnum.c
│   ├── ft_isalpha.c
│   ├── ft_isascii.c
│   ├── ft_isdigit.c
│   ├── ft_isprint.c
│   ├── ft_isspace.c
│   ├── ft_itoa.c
│   ├── ft_malloc.c
│   ├── ft_memchr.c
│   ├── ft_memcmp.c
│   ├── ft_memcpy.c
│   ├── ft_memmove.c
│   ├── ft_memset.c
│   ├── ft_putchar_fd.c
│   ├── ft_putendl_fd.c
│   ├── ft_putnbr_fd.c
│   ├── ft_putstr_fd.c
│   ├── ft_split.c
│   ├── ft_split_space.c
│   ├── ft_strcat.c
│   ├── ft_strchr.c
│   ├── ft_strcmp.c
│   ├── ft_strcpy.c
│   ├── ft_strdup.c
│   ├── ft_striteri.c
│   ├── ft_strjoin.c
│   ├── ft_strlcat.c
│   ├── ft_strlcpy.c
│   ├── ft_strlen.c
│   ├── ft_strmapi.c
│   ├── ft_strncmp.c
│   ├── ft_strndup.c
│   ├── ft_strnstr.c
│   ├── ft_strrchr.c
│   ├── ft_strrev.c
│   ├── ft_strstr.c
│   ├── ft_strtrim.c
│   ├── ft_substr.c
│   ├── ft_tolower.c
│   ├── ft_toupper.c
│   └── libft.h
├── Makefile
├── execution
│   ├── banner.c
│   ├── builtins
│   │   ├── builtins.c
│   │   ├── builtins.h
│   │   ├── cd.c
│   │   ├── echo.c
│   │   ├── env.c
│   │   ├── exit.c
│   │   ├── export.c
│   │   ├── export_files.c
│   │   ├── pwd.c
│   │   └── unset.c
│   ├── environment
│   │   ├── file1.c
│   │   ├── file2.c
│   │   └── file3.c
│   ├── execution.h
│   ├── exit_status.c
│   ├── file_descriptor.c
│   ├── main_loop.c
│   ├── path_functions.c
│   ├── path_functions_1.c
│   ├── pipe
│   │   ├── handel_pipe1.c
│   │   ├── handel_pipe2.c
│   │   ├── handel_pipe3.c
│   │   ├── handel_pipe4.c
│   │   └── pipe.h
│   ├── print_errors.c
│   ├── redirection
│   │   ├── redirect_append.c
│   │   ├── redirect_heredoc.c
│   │   ├── redirect_heredoc_1.c
│   │   ├── redirect_overwrite.c
│   │   ├── redirect_stdin.c
│   │   ├── redirection.h
│   │   └── redirection_files.c
│   ├── signals.c
│   └── simple_cmd.c
├── main.c
├── minishell.h
└── parsing
    ├── check_syntax
    │   ├── syntax_error.c
    │   └── syntax_tool.c
    ├── expension2
    │   ├── expend_helper.c
    │   ├── expend_helper2.c
    │   ├── expend_helper3.c
    │   ├── expend_helper4.c
    │   ├── expend_herdoc.c
    │   ├── expend_utils.c
    │   ├── expend_utils2.c
    │   ├── expend_utils3.c
    │   ├── expend_utils4.c
    │   ├── expend_utils5.c
    │   └── expension.c
    ├── lexing
    │   ├── lexer.c
    │   ├── lexer_handler2.c
    │   ├── lexer_handlers.c
    │   ├── lexer_utils.c
    │   └── token_utils.c
    ├── parse_cmd
    │   ├── export_parse.c
    │   ├── parse_utils.c
    │   ├── parse_utils2.c
    │   ├── parse_utils3.c
    │   ├── parser.c
    │   └── parser_helper.c
    ├── parse_input.c
    └── parsing.h

12 directories, 111 files
```

## 🔍 Implementation Details

### Parsing Pipeline
1. **Lexical Analysis**: Tokenizes input into meaningful units
2. **Expansion**: Handles variable substitution and quote removal
3. **Syntax Checking**: Validates command structure
4. **Command Building**: Constructs executable command structures

### Key Data Structures

```c
typedef struct s_cmd {
    char         **args;     // Command arguments
    t_redir      *redirs;    // Redirections list
    bool         in_pipe;    // Pipe context flag
    struct s_cmd *next;      // Next command in pipeline
} t_cmd;

typedef struct s_env {
    char         *key;              // Variable name
    char         *value;            // Variable value
    bool         export_variable;   // Export flag
    struct s_env *next;             // Next environment variable
} t_env;
```

### Memory Management
- Custom memory allocation tracking via `ft_malloc`/`ft_free`
- Automatic cleanup on exit
- Prevention of memory leaks through proper deallocation

## 🔨 Built-in Commands

| Command | Description | Usage |
|---------|-------------|-------|
| `echo` | Display text | `echo [-n] [text...]` |
| `cd` | Change directory | `cd [path]` |
| `pwd` | Print working directory | `pwd` |
| `export` | Set environment variables | `export [VAR=value]` |
| `unset` | Remove environment variables | `unset [VAR]` |
| `env` | Display environment | `env` |
| `exit` | Exit the shell | `exit [status]` |

## 🧪 Testing

### Basic Testing
```bash
# Test basic commands
minishell$ echo hello world
minishell$ pwd
minishell$ cd /tmp
minishell$ pwd

# Test pipes
minishell$ echo "test" | cat | cat

# Test redirections
minishell$ echo "test" > file && cat file

# Test variables
minishell$ export TEST=value
minishell$ echo $TEST
```

### Error Handling
```bash
# Test syntax errors
minishell$ echo |
minishell$ cat <

# Test command not found
minishell$ nonexistent_command

# Test directory execution
minishell$ ./directory_name
```

## 🏗️ Build Options

- **Debug mode**: Uncomment `-g3 -fsanitize=address,undefined` in Makefile
- **Clean build**: `make fclean && make`
- **Rebuild**: `make re`

## ⚠️ Known Issues

- Limited error handling for edge cases
- Some advanced bash features not implemented
- Platform-specific (macOS-oriented due to brew readline)

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📄 License

This project is part of the 42 School curriculum. Please respect academic integrity guidelines.

## 👥 Authors

- **msabr** - Main developer
- **kabouelf** - Contributor
  
```
	███╗   ███╗██╗███╗   ██╗██╗ ██████╗██╗   ╔██╗╔████████╗██╗
	██╔████╔██║██║██╔██╗ ██║██║██╔════╝██╝   ╚██╝║██══════╝██║
	██║╚██╔╝██║██║██║╚██╗██║██║╚█████╗ █████████╗║████████╗██║
	██║ ╚═╝ ██║██║██║  ╚███║██║ ╚═══██╗██╔═══╗██║║██╔═════╝██║
	██║     ██║██║██║   ╚██╝██║██████╔╝██║   ║██║║████████╗████████╗
	╚═╝     ╚═╝╚═╝╚═╝    ╚═╝═╝╚══════╝ ╚═╝   ╚══╝╚═══════╝╚═══════╝
```
