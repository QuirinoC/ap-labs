package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"strings"
	"sync"
	"time"
)

func main() {
	var wg sync.WaitGroup
	var timeData []string
	//for _, arg := range os.Args[1:] {
	//	data = append(data, arg)
	//}
	//data = append(data)

	for i, arg := range os.Args[1:] {
		data := strings.Split(arg, "=")
		city, port := data[0], data[1]

		conn, err := net.Dial("tcp", port)
		if err != nil {
			log.Fatal(err)
		}
		timeData = append(timeData, "")
		wg.Add(1)
		go connHandler(conn, city, &wg, &timeData, i)

	}
	//go addData(&data, 0, "hola")
	go printData(&timeData)
	wg.Wait()
}

func addData(data *[]string, i int, str string) {
	(*data)[i] = str
}

func printData(data *[]string) {
	var r = ""
	time.Sleep(1 * time.Second)
	for {
		r = ""
		for _, s := range *data {
			r += s
			if s != "" {
				r += "--------------------------\n"
			}
		}
		os.Stdout.WriteString("\x1b[3;J\x1b[H\x1b[2J")
		fmt.Printf("%s", r)
		time.Sleep(1 * time.Second)
	}
}

func connHandler(c net.Conn, city string, wg *sync.WaitGroup, data *[]string, i int) {
	defer c.Close()
	defer wg.Done()
	defer addData(data, i, "")

	tmp := make([]byte, 1024)
	//data := make([]byte, 0)

	for {
		n, err := c.Read(tmp)
		if err != nil {
			// log if not normal error
			if err != io.EOF {
				fmt.Printf("Read error - %s\n", err)
			}
			break
		}
		if n == 0 {
			break
		}
		(*data)[i] = string(tmp)
	}

}
