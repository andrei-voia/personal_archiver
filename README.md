# Personal Archiver

A simple Archiver that can be used to group an entire folder in a single file, that can be unarchived later safely.

## Unarchiving Size

The size of the extracted folder will be the exact same size as the one that has been archived.
![alt text](https://github.com/andrei-voia/personal_archiver/blob/master/sizes.png "Sizes")

## How it works

The archiving and unarchiving process at run time.
![alt text](https://github.com/andrei-voia/personal_archiver/blob/master/working.png "Sizes")

## Error / Corruption management

The code is able to detect if certain parts from the archived folder have been altered / modified 
and it will print a specific Warning message.
After the Warning, the program will try to delete the extracted files and will print the 
status in the terminal.
![alt text](https://github.com/andrei-voia/personal_archiver/blob/master/corrupted.png "Sizes")
