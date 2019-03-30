# IRC CHAT

Build

    make

Remove binaries 

    make clean

Run client with

    ./client -user username -host ip -port port

Run server with

    ./server -host ip -port port

| Sub-command         | description                                             |
|---------------------|---------------------------------------------------------|
| `/users`            | To list all connected users                             |
| `/msg <user> <msg>` | To send a direct message to the specified `user`        |
| `/time`             | Get IRC Server's localtime                              |
| `/user <user>`      | Get more details about `user` (Username and IP address) |
