"""
Utility functions for testing YDB
"""

import os
import subprocess
import tempfile
import unittest
import socket
import time

import xml.etree.ElementTree as et

BUFFER_SIZE = 8 * 1024

def create_hello():
    """
    Creates a netconf hello message
    """
    hello = et.Element('hello')
    caps = et.SubElement(hello, 'capabilities')
    cap = et.SubElement(caps, 'capability')
    cap.text = "urn:ietf:params:ns:netconf:capability:startup:1.0"
    msg = et.tostring(hello)
    return msg + "]]>]]>"

class TestBase(unittest.TestCase):
    """
    A class contaning setup, teardown and utilities for netconf tests

    """
    _schema_path = None
    _schema_name = None
    _ydb_proc = None
    _ip = None
    _port = None
    _socket = None
    def __init__(self, test_case, schema_path, schema_name, ip, port):
        super(TestBase, self).__init__(test_case)
        self._schema_path = schema_path
        self._schema_name = schema_name
        self._ip = ip
        self._port = port

    def setUp(self):
        config = et.Element('ydb-config')
        schema_path = et.SubElement(config, 'schema-path')
        schema_path.text = self._schema_path
        schema = et.SubElement(config, 'schema')
        schema.text = self._schema_name
        et.dump(config)
        tmp_file = tempfile.NamedTemporaryFile(mode="w", delete=False)
        tree = et.ElementTree(config)
        tree.write(tmp_file)
        print 'creating config file:' + tmp_file.name
        tmp_file.close()
        os.environ['YDB_CONFIG_FILE'] = tmp_file.name
        self._ydb_proc = subprocess.Popen("../ydb", stdout=subprocess.PIPE)
        self.try_connect()
        self.send_hello()

    def try_connect(self):
        """
        Tries to connect to ydb
        """
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        count = 0
        connected = False
        while count < 10:
            try:
                print "trying to connect: " + self._ip + ":" + str(self._port)
                self._socket.connect((self._ip, self._port))
                connected = True
                break
            except:
                count += 1
                time.sleep(0.1)
        self.assertTrue(connected)

    def send_hello(self):
        """
        Sends a netconf hello message
        """
        hello = create_hello()
        self._socket.send(hello)

    def expect_hello(self):
        """
        Verifies that the first message is a netconf hello message
        """
        data = self.recv_xml()
        root = et.fromstring(data)
        hello_found = False
        for child in root:
            if child.tag == 'hello':
                hello_found = True
        self.assertTrue(hello_found)

    def recv_xml():
        """
        Receivis a Netconf message from socket
        and stripps away "]]>]]>"
        """
        data = self._socket.recv(BUFFER_SIZE)
        result = data.split("]]>]]>")
        return result[0]
        
    def tearDown(self):
        self._socket.close()
        self._ydb_proc.kill()
