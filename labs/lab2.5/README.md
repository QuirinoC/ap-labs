# crawl3 - A simple web crawler

## Links library needed

    go get gopl.io/ch5/links

## Run with
    
    go run crawl3.go -depth=[depthLimit] [url]

Where depthLimit is the limit for the crawling recursion and url is the url to crawl

The program scrapes the web until the recursion limit is reached, consider that the number of links opened gets big really fast due to external links to other websites

Each link will be printed with its depth relative to the link given.


Tested with:

    http://quirinocode.com/lab2.5

Note:

thislinks.Extract(url) might return weird urls such as http://quirinocode.com?C=M;O=D: from http://quirinocode.com which is and invalid url not found in this site