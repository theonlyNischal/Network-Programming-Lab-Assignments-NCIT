TCP Quiz
===========


Implement TCP quiz for the users. Users can create question/answer pairs and submit those to server. Server saves
submitted pairs to a buffer where it randomly selects next one to be asked. All clients, except the one who have
entered the current question try to answer it. In every questions there should be 2 minutes limit after which the
correct answers is shown to users and new questions will be drawn from the the buffer, or if the buffer is empty it will
be asked from the clients
The program gets it's parameters from the command line.
