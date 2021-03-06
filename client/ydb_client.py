#!/usr/bin/env python

import sys
import socket

TCP_IP = '127.0.0.1'
TCP_PORT = 8888
BUFFER_SIZE = 1024

def read_msg():
    msg = ""
    while True:
        line = sys.stdin.readline()
        if not line:            # if EOF, hmm
            break;
        msg = msg + line
        if line.strip().endswith("]]>]]>"):
            break
    return msg

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

data = s.recv(BUFFER_SIZE)
print "received data:", data


while True:
    msg = read_msg()
    print "sending message: \n" + msg
    s.send(msg)
    data = s.recv(BUFFER_SIZE)
    print "received data:", data

s.close()
