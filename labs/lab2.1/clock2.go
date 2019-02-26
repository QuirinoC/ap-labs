// Clock2 is a concurrent TCP server that periodically writes the time.
package main

import (
	"flag"
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"strings"
	"time"
)

func handleConn(c net.Conn) {
	defer c.Close()
	tz := os.Getenv("TZ")

	loc, err := time.LoadLocation(tz)

	if err != nil {
		fmt.Println("Invalid timezone")
		os.Exit(1)
	}

	for {

		_, err := io.WriteString(c, tz+strings.Repeat(" ", 15-len(tz))+":  "+time.Now().In(loc).Format("15:04:05\n"))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {
	//var tz = os.Getenv("TZ")
	portFlag := flag.Bool("port", false, "port flag")
	flag.Parse()
	//Default port value
	port := "9090"
	if *portFlag {
		if len(os.Args) >= 3 {
			port = os.Args[2]
		} else {
			fmt.Println("No port given")
			os.Exit(1)
		}
	}
	listener, err := net.Listen("tcp", "localhost:"+port)
	if err != nil {
		log.Fatal(err)
	}

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn) // handle connections concurrently
	}
}
