// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"bytes"
	"flag"
	"fmt"
	"log"
	"net"
	"strings"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

var users map[string]net.Conn //stores users currently online

var (
	entering = make(chan client)
	leaving  = make(chan client)
	messages = make(chan string) // all incoming client messages
)

func getUsers() string {
	//Display info of connected users
	var buf bytes.Buffer
	buf.WriteString("server > Connected users:\n")
	buf.WriteString("--------------\n")
	for k, _ := range users {
		buf.WriteString(k + " ")
	}
	buf.WriteString("\n--------------\n")
	return buf.String()
}

func getUserInfo(user string) string {
	if _, ok := users[user]; ok { //checks if user is already connected
		return ("server > " + user + " addr: " + users[user].RemoteAddr().String())
	} else {
		return "There's no user with that name"
	}

}

func getTime() string {
	var buf bytes.Buffer
	buf.WriteString("server > Local time: ")
	buf.WriteString(time.Now().Location().String())
	buf.WriteString(" ")
	buf.WriteString(time.Now().Format("15:04:05"))

	return buf.String()
}

func sendPrivateMessage(originUser, destinyUser string, text []string) {
	if _, ok := users[destinyUser]; ok { //checks if user is already connected
		fmt.Fprintln(users[destinyUser], originUser+"> "+strings.Join(text, " "))
	} else {
		fmt.Fprintln(users[originUser], "Invalid username")
	}

}

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn, user string) {

	users[user] = conn

	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch, user)

	ch <- "server > Welcome user " + user + "."
	entering <- ch

	input := bufio.NewScanner(conn)
	for input.Scan() {
		data := input.Text()
		data_len := len(data)

		//Skip if empty message
		if data_len <= 0 {
			messages <- user + "> " + input.Text()
			continue
		}
		//Skip if not a command
		if strings.Compare(string(data[0]), "/") != 0 {
			messages <- user + "> " + input.Text()
			continue
		}

		//Test for command
		command := strings.Split(
			strings.Trim(data, " "),
			" ")

		if len(command) == 1 {
			switch command[0] {
			case "/users":
				ch <- getUsers()
			case "/time":
				ch <- getTime()
			default:
				messages <- user + "> " + data
			}
		} else if len(command) == 2 {
			if strings.Compare(command[0], "/user") == 0 {
				ch <- getUserInfo(command[1])
			} else {
				messages <- user + "> " + data
			}
		} else if len(command) >= 3 {
			if strings.Compare(command[0], "/msg") == 0 {
				sendPrivateMessage(user, command[1], command[2:])
			}
		}

	}
	//----------------------------------------------------

	delete(users, user)

	fmt.Println("server >", user, "left")

	leaving <- ch
	messages <- user + " has left"

	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string, user string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg)
	}
}

//!-handleConn

//!+main
func main() {
	var host string
	var port string
	flag.StringVar(&host, "host", "localhost", "server address")
	flag.StringVar(&port, "port", "8000", "server port")

	flag.Parse()

	users = make(map[string]net.Conn)

	listener, err := net.Listen("tcp", (host + ":" + port))
	if err != nil {
		log.Fatal(err)
	} else {
		fmt.Println("server > Server running on: ", host, port)
	}

	go broadcaster()
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}

		user, _ := bufio.NewReader(conn).ReadString('\n')
		user = user[:len(user)-1]
		//Test is user exists
		if _, exists := users[user]; exists {
			fmt.Fprintf(conn, "server > Username already taken")
			conn.Close()
		} else {
			go handleConn(conn, user)
		}
	}
}
