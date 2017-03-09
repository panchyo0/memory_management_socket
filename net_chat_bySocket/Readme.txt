CMPUT379

ASSIGNMENT 2

GROUP 16
Pengda Xu & Qi Pang

Use Makefile:
type make to generate 2 executable files namely: chat379 and server379

Server:
type ./server379 portnumber

the program run in the backgroup 
the server terminate when killed, and then close the log file



Client:
type ./chat379 hostname portnumber usename

when running on the same machine as the server, hostname = localhost
when running on a different machine, hostname = IPv4 address of the server
e.g. ./server379 4444
     ./chat379 129.128.28.93 4444 Mike
 ./server379 4444
     ./chat379 localhost 4444 Mike

the client quit by entering Ctrl + C or simply closing the terminal

Once a new client comes in, it will receive the total number of the clients
and a list of their names. And all the other clients will notice the newclient 
coming.
(If the new client is using the name that already exist, it'll be shut down by 
the server. The event will be recorded in the log file)

Once a client leaves the chat, other clients will notice that.

the log file is called server379procid.log
