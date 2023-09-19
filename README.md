# SpellChecker

This is a SpellChecker program written in C language. Given a text file, the program checks if the words are correctly spelled and outputs words that were incorrect, or not in the system(this may include specific names locations, and such).

In the first version of the spell checking program I loaded the dictionary of words into memory by reading all of the strings in the dictionary file. I constructed an array of pointers to strings to store all of those words, and then implemented our binary search in that array.

In the second version of the spell checking program I used a slightly different strategy. This time around I kept the dictionary words in the original file. To make it easier to locate the words in the file I constructed an index that can tell us the location of each word in the text file. Only the index will be loaded into memory when the spell checker starts, and I used the index to locate words the user want to look up in the text file and read just one word at a time from the text file when the user does searches.

In the third version of the program I loaded both the index file and the full list of words into memory when the spell checker starts up. I worked with an alternative set of functions for reading data from a file. I used open(), read(), and close() functions in place of fopen(), fread(), and fclose().

The dictionary file I am using was from GitHub at https://github.com/dwyl/english-words.
