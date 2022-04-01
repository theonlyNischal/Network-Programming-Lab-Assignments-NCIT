Network Programming Practical Assignment, BESE VIII, NCIT
==============================================================
Implement TCP quiz for the users
*************************************
A Users can create question/answer pairs and submit those to server. Server saves submitted pairs to a buffer where it randomly selects next one to be asked. All clients, except the one who have entered the current question try to answer it.

In every questions there should be 2 minutes limit after which the correct answers is shown to users and new questions will be drawn from the the buffer, or if the buffer is empty it will be asked from the clients. The program gets it's parameters from the command line

Server is started with: tcpquizclientserver –p port
Client is started with: tcpquizclientserver –h serveraddress -p port -n nickname

- The client sends the nickname as the first message to the server. The server remembers the nickname, and TCP file descriptor so it knows later on which nickname belongs to which client.

- The server should be able to keep track of 10 simultaneous users. Length of the nickname should be limited to 10 characters.

- Use select() statement to check if input is coming from the keyboard or from which of the clients.

MESSAGE RULES
------------------
Messages between client and server must follow these rules.
These messages also must trigger the actions listed
**Information messages from server to client**
Must begin with 0 e.g. 0Welcome to the game
**Server asks for new questions from client**
Must begin with 1 e.g. 1Enter new question
This should be done only if there is no questions in the
buffer
**Client sends new question to server**
Must begin with 2
Question part must end with ?
Must contain correct answer after the ? inside ( )
e.g. 2Who is your network programming instructor? (Madan)
Client should be able to enter new questions at any time. If
the current questions is still open, this newly submitted
question should be stored into a buffer and used later on
**Server sends question to other clients**
Must begin with 3
First question from the buffer should be used. If the buffer
is empty --> ask for new questions from the clients
**Client answers to the received question**
Must begin with 4
If correct answer is not found until timer expires server
should tell the correct answer to clients and take a new
question from the buffer
