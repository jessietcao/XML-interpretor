// tokenize_intput(len) reads all input, breaks it into tokens of markup or
//   content, and returns an array of tokens. The length of the returned array
//   is stored in *len.
// effects:  allocates heap memory [client must free the array and each token]
//           mutates *len
char **tokenize_input(int *len);
