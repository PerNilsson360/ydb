#!/usr/bin/env python

"""
Netconf test cases for YDB
"""

import unittest

import util

class TestContainer(util.TestBase):
    """
    Tests the YANG containers
    """
    def __init__(self, test_case):
        super(TestContainer, self).__init__(
            test_case,
            '/home/per/prg/git/ydb/test_models',
            'several-containers.yang',
            '127.0.0.1',
            8888)

    def test_several_containers(self):
        """
        Tests several containers
        """
        self.expect_hello()

def suite():
    """
    Test suite for YDB netconf tests
    """
    test_suite = unittest.TestSuite()
    test_suite.addTest(TestContainer("test_several_containers"))
    return test_suite

if __name__ == '__main__':
    RUNNER = unittest.TextTestRunner()
    RUNNER.run(suite())
