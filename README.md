### Summary
Design and implement a simple chat system that works for one pair of users, i.e., create two programs: a chat server and a chat client.

### Requirements
- [x] chatserve must be implemented in Java or Python
- [x] chatclient must be implemented in C/C++
- [x] Must be well-modularized and well-documented
- [x] Must run on an OSU flip server (ex: flip1.engr.oregonstate.edu), specify your testing machine in the program documentation.
- [x] Should be able to send messages of up to 500 characters.
- [x] Use the directories in which the programs are running. Don't hard-code any directories, since they might be inaccessible to the graders.
- [x] Be sure to cite any references, and credit any collaborators.
- [x] Provide a README.txt with detailed instructions on how to compile, execute, and control your program.

### How to run
1. run `make` to compile the C file
2. start the server by running `python3 chatserve.py [host] [port]`. This is the server.
3. in a separate window, run `chatclient [host] [port]`. This is the client.
4. When starting, the client will send an initial message to the server initializing the conversation. In order to speak, you must wait until you are prompted by the console.
5. To quit the chat, either the server or the client can type the phrase `\quit`. This will send a message to disconnect.

### Known Issues
- Unable to both send messages at the same time without throwing off the stdin. I attempted to do some research and was unable to find a simple solution without loading any additional libraries.

### Testing
I tested the client-server files both locally on my mac, as well as flip3.engr.oregonstate.edu.

### References
- http://beej.us/guide/bgnet/html/single/bgnet.html
- https://docs.python.org/3/library/socket.html
- https://stackoverflow.com/questions/2328182/prepending-to-a-string

    
