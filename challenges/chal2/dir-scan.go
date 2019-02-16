package main

import (
	"fmt"
	"io/ioutil"
	"os"
)

func printData(dirPath string, total, dir, denied, sLink, other int) {
	//For some reason in some cases walk reads more files than ioutil.ReadDir
	fmt.Printf("| Path: |  %s  |\n-----------------------------------\n", dirPath)
	fmt.Printf("| Directories             | %d\n", dir)
	//fmt.Printf("| Permission denied files | %d\n", denied)
	fmt.Printf("| Symbolic Links          | %d\n", sLink)
	fmt.Printf("| Other files             | %d\n", other)
}

// scanDir stands for the directory scanning implementation
func scanDir(dirPath string) error {
	//var fileData []os.FileInfo

	var dirFiles, _ = ioutil.ReadDir(dirPath)
	var total = len(dirFiles)
	var dir = 0
	var sLink = 0
	var other = 0

	//Iterate over fileData to count files
	for _, file := range dirFiles {
		if file.Mode()&os.ModeDir != 0 {
			dir++
		} else if file.Mode()&os.ModeSymlink != 0 {
			sLink++
		} else {
			other++
		}
	}
	printData(dirPath, total, dir, total-dir-sLink-other, sLink, other)
	return nil
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}

	scanDir(os.Args[1])
}
