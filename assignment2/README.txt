Author: Yavuz Selim Suzen

Description of the program: The program will prompt the user to indicate whether they wish to encrypt a readable message (the plaintext), or decrypt an already encrypted sequence of numbers (the ciphertext) back to its readable form.

List of files: a2-posted.c and README.txt

Instructions: For compiling the program, write "gcc -o a2-posted a2-posted.c" in the terminal. After that, write "./a2-posted" for running the program. The user will select '1' for encrypt a message or '2' for decrypt decimals. Plaintext string and space-separated sequence of decimal, numeric values will be read from the command line. Plaintext has multiple space-delimited words, so scanf() cannot be used for this;instead,  will be used fgets() as shown in the provided code. scanf() function reads a single value up to a delimiter, which can be either a space or a newline character function for space-separated sequence of decimal, numeric values. 

For compiling the program: gcc -o a2-posted a2-posted.c
For running the program: ./a2-posted
