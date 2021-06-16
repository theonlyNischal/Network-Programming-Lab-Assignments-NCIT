Design and implement TCP Client and TCP Server applications for providing registration numbers to the users.
Requirement: 

1. The server contains a text file having one line of information about users (total 10 lines user information in the filename as users.txt). File content Example:  1. Welcome <<Yourname>>, your unique identification key is <<Registration No.>>
1.1. Replace <<Yourname>> and <<Registration No>> by any sample value for sample  file and output. 
2. The client establishes a connection to the server with its unique ID number (use your roll no) send via command-line argument during sending a connection request.
3. The Server sends user information on the basis of provided ID number.  Use division operation by 11 (i,e: ID no %11) to find the line number to find the contents. 
4. Use the line number calculated as above to find the information and send that information to the respective client.
5. Server also send asking information immediately after sending registration no.
6. Your server must support multiple simultaneous connections. The server must also ask the client after the first information if he wishes to see the same information again. 
7. If a client wants to quit, take input ‘quit’ from the console, and the client terminates. If a client wants to see the same information again, input ‘Again’ from the console and send a request to the server.
8. Your server must not support more than 10 simultaneous clients.

Use select() statement to check if the input is coming from the keyboard or from which of the clients.

    • The server is started with:
tcpInfoServer –p port
    • The client is started with:
tcpInfoClient –h serveraddress -p port -n IDNumber
 
Use the following messaging rules:
Information messages from the server to client
1Welcome <<Yourname>>, your unique identification key is <<Registration No.>>

3Do you want to see your information again?

Message from client to sever to see information repetitively
2Again