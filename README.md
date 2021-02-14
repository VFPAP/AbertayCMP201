# AbertayCMP201
BSc (Hons) Ethical Hacking - 2nd Year - Data Structures and Algorithms 1

## Software Project
During the first term of the second year, I've built a program in C++ that implements two String-Searching algorithms (Boyer-Moore-Horspool & Rabin-Karp) and compares their performance.
This was achieved by varying the input of the program and statistically analysing the results.
<br>Grade: A

### Input 
| Input | Text File                          | Pattern        |
|-------|------------------------------------|----------------|
| Small | 741975 Characters<br>1000 Lines    | 8 Characters   |
| Big   | 15128189 Characters<br>20000 Lines | 154 Characters |

### Usage

The program can be run from a terminal using the following syntax:
```console
BMvsRK.exe "<pattern>" "<directory with folders/files>"
```
Alternatively, the program can also be run without any arguments, and the required values will be requested as needed.
Then, the program will search for the pattern inside the text files present in the specified directory (and subdirectories) using both algorithms.
Finally, the program will display in which line(s) the pattern is present in the text files (or if it's not present at all) and present the average of time in milliseconds that each algorithm took to perform the search.
There's also an option in the program that allows the user to run the same search for a specific amount of times and to export the performance values to a text file, that can later be used in statistical analysis.

#### Example
Using the provided [test directory](testDir), a search for the string "password" can be done as follows:
```console
BMvsRK.exe "password" "/path/to/directory/testDir"
```
This directory contains two different folders (big and small), each containing a big text file and a small text file, respectively.

### Presentation
The Presentation Slides can be found [here](BMvsRK.pdf).

### Source Code
This program was developed using Microsoft Visual Studio 2019 and the complete source code of the program can be found [here](BMvsRK).

## Author
**Vasco Pinto**
<br>Twitter: [@0xVFPAP](https://twitter.com/0xVFPAP)
<br>LinkedIn: [Vasco Pinto](https://linkedin.com/in/vascopinto97)
<br>OpenBugBounty: [VFPAP](https://www.openbugbounty.org/researchers/VFPAP)
