package main

import (
	"io"
	"log"
	"net"
	"os"
	"strings"
	"sync"
)

func main() {
	var wg sync.WaitGroup

	for _, arg := range os.Args[1:] {
		data := strings.Split(arg, "=")
		city, port := data[0], data[1]

		conn, err := net.Dial("tcp", port)
		if err != nil {
			log.Fatal(err)
		}
		wg.Add(1)
		go connHandler(conn, city, &wg)

	}
	wg.Wait()
}

func connHandler(c net.Conn, city string, wg *sync.WaitGroup) {
	defer c.Close()
	defer wg.Done()

	_, err := io.Copy(os.Stdout, c)
	if err != nil {
		log.Fatal(err)
	}

}
