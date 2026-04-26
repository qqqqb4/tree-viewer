# tree-viewer
Cli tree-style directory viewer for linux with .gitignore support

## Requirements
- C++23

## Installation
```
git clone https://github.com/qqqqb4/tree-viewer 
cd tree-viewer
make
```
Then you can add executable to your $PATH.

## Usage
### With no arguments
```
tree-viewer
```
Shows current directory
### With arguments
```
tree-viewer arg0 arg1 arg2 ... 
```
arg0 - directory to show (. for current), arg1+ - files and dirs to exclude besides .gitignore ones

You need to press ENTER for update, and type quit or exit for shutdown.
