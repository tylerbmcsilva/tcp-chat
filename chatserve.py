## PYTHON 3.6
"""
Project Name:   Chat Server
Author:         Tyler McSilva
Summary:        Opens a TCP connection on a specified host and port, then waits
                for incoming connections. 
"""

import sys
import socket

BUFF_SIZE = 512

class TCPSocket:
    """
    TCPSocket Class
    Basic handler for TCP socket. 
    """
    def __init__(self, host="localhost", port=9547, s=None):
        """
        Initializes the socket
        Has default of localhost:9547
        """
        try:
            self.sock = s if s else socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.bind_sock(host, port)
            self.listen_sock()
        except Exception as e:
            print(sys.stderr, "ERROR: Setting up TCPSocket\n{}".format(e))
    
    def __del__(self):
        self.sock.close()

    def bind_sock(self, host, port):
        """
        Binds the host and port to the socket
        Throws error if unable to bind
        """
        try:
            self.sock.bind((host, port))
        except Exception as e:
            print(sys.stderr, "ERROR: Unable to bind port\n{}".format(e))
            exit(1)

    def listen_sock(self, num_conns=10):
        """
        Starts listening, default to 10 connection
        """
        self.sock.listen(num_conns)

class ChatHandler:
    """
    Manages the chat back and forth between client and server
    """
    def __init__(self, connection, exit_msg="\quit", server_name="SERVER"):
        """
        Sets initial variables based on arguments and starts the chat
        """
        self.conn = connection
        self.exit_msg = exit_msg
        self.server_name = server_name
        self.start_chat()
        
    def __del__(self):
        self.conn.close()
            
    def start_chat(self):
        """
        Initializes the chat function, first by receiving data from the client
        then sends data to the client. Repeats until break.
        """
        # Keep loop running
        while True:
            if not self.receive_message(BUFF_SIZE): break
            if not self.send_message(BUFF_SIZE): break
        self.conn.close()

    def send_message(self, size=512):
        """
        Sends message through connection
        Prints error and quits if something happens
        """
        server_msg = self.get_server_msg()
    
        try:
            self.conn.sendall(str.encode(server_msg[:size]))
        except Exception as e:
            print(sys.stderr, "ERROR SENDING MESSAGE: {}".format(e))
            exit(1)

        if self.should_chat_end(str.encode(server_msg)):
            print("Server closed connection.")
            return False
        else:
            return True

    def receive_message(self, size=512):
        """
        Receives and returns data as a string from the connection
        Defaults to bufize of 512 (power of 2)
        """
        try:
            data = self.conn.recv(size)
        except Exception as e:
            print(sys.stderr, "ERROR RECEIVING MESSAGE: {}".format(e))
            exit(1)

        # Else return data formated as a string
        if self.should_chat_end(data):
            print("Client closed connection.")
            return False
        else:
            print(str(data, 'utf-8'))
            return True
    
    def should_chat_end(self, data):
        """
        Returns true or false whether chat should end or not
        """
        return True if (self.exit_msg in str(data, 'utf-8')) else False
        
    def get_server_msg(self):
        """
        Prompts the user for a message and returns a string
        """
        message = input("{}> ".format(self.server_name))
        return "{}> {}".format(self.server_name, message.rstrip())



if __name__ == "__main__":
    # Verify number of arguments
    if len(sys.argv) != 3:
        print("Please use:\npython3 chatserve.py [host] [port]")
        
        exit(2)

    # Set variables
    HOST = sys.argv[1]
    PORT = int(sys.argv[2])
    
    try:
        # Create the server, binding to host to the port
        server = TCPSocket(HOST, PORT)
        print("Server listening on {}:{}".format(HOST, PORT))
        # Continuously run until a connection is made
        while True:
            # Accept connections
            conn, c_addr = server.sock.accept()
            try:
                # Announce that something has connected
                print("{} has connected".format(c_addr))
                # Start chat
                ChatHandler(conn)
            finally:
                # At the end, close the connection
                conn.close()
    except KeyboardInterrupt:
        print("\nGoodbye.")
        quit(0)
    except Exception as e:
        print(sys.stderr, "ERROR: ".format(e))
        exit(1)

