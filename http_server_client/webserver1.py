#   Auther      : Heinz Samuelsson
#   Date        : 2015-10-04
#   File        : webserver1.py
#   Reference   : -
#   Description : Simple web server
#                 Use 'telnet localhost 8888' to communicate with.
#   Python ver  : 2.7.3 (gcc 4.6.3)

import socket

HOST, PORT = '', 8888

listen_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
listen_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
listen_socket.bind((HOST, PORT))
listen_socket.listen(1)
print 'Serving HTTP on port %s ...' % PORT

while True:
    client_connection, client_address = listen_socket.accept()
    request = client_connection.recv(1024)
    print '->', request
    msg = request.split()

    print 'msg[0]:', msg[0]

    if msg[1] == "hello":
            http_response = """\
        HTTP/1.1 200 OK
        Hello, World!
        """
    else:
            http_response = """\
        HTTP/1.1 200 OK
        Hello, There!
        """

    client_connection.sendall(http_response)
    client_connection.close()
