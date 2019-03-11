// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 241.

// Crawl2 crawls web links starting with the command-line arguments.
//
// This version uses a buffered channel as a counting semaphore
// to limit the number of concurrent calls to links.Extract.
//
// Crawl3 adds support for depth limiting.
//
package main

import (
	"flag"
	"fmt"
	"log"
	"os"

	"gopl.io/ch5/links"
)

//!+sema
// tokens is a counting semaphore used to
// enforce a limit of 20 concurrent requests.
var tokens = make(chan struct{}, 50)

// Avoid multiple goroutines modifying the map
var mapToken = make(chan struct{}, 1)

func crawl(url string, depthMap map[string]int, depthLimit int) []string {
	tokens <- struct{}{} // acquire a token
	list, err := links.Extract(url)
	<-tokens // release the token

	if err != nil {
		log.Print(err)
	}

	mapToken <- struct{}{}

	if depthMap[url] <= depthLimit {
		fmt.Println(url, depthMap[url], depthLimit)
	}
	if depthMap[url] > depthLimit {
		list = []string{}
	}

	//if depthMap[url] > depthLimit {
	//	fmt.Println(url, depthMap[url])
	//}
	for _, link := range list {
		depthMap[link] = depthMap[url] + 1
	}
	<-mapToken

	return list
}

//!-sema

//!+
func main() {
	worklist := make(chan []string)
	depthLimit := flag.Int("depth", 1, "Depth limit")
	flag.Parse()
	var n int // number of pending sends to worklist

	// Start with the command-line arguments.
	n++
	go func() { worklist <- os.Args[2:] }()

	// Keep track of depth for each link
	depthMap := make(map[string]int)
	// Crawl the web concurrently.
	seen := make(map[string]bool)
	for ; n > 0; n-- {
		list := <-worklist
		for _, link := range list {
			if !seen[link] {
				seen[link] = true
				n++
				go func(link string) {
					tmp := crawl(link, depthMap, *depthLimit)
					worklist <- tmp
				}(link)
			}
		}
	}

}
