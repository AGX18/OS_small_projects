## Steps
- first create the file 
- get the file descriptor
- get the stat of the file
- check the file size
- write to the file
- check the file size again
- go to step 6 and repeat until the file size is greater than 1 MB
- close the file descriptor

## main
takes 1 argument the name of the file that will be created.
