package main

import (
	"strings"

	"golang.org/x/tour/wc"
)

func WordCount(s string) map[string]int {
	var arr = strings.Fields(s)
	wc := make(map[string]int)

	for _, word := range arr {
		wc[word] = wc[word] + 1
	}
	return wc
}

func main() {
	wc.Test(WordCount)
}
