# -*- coding: utf-8 -*-
# file: feahash_test.py


import unittest
import pyfeather
from typing import Dict, List, Union, Any


CONF: str = "../conf/feather.conf"


class Test4FeaHash(unittest.TestCase):
    @classmethod
    def setUpClass(self):
        self.msg_temp = "\n\tTarget: {0} \n\tOutput: {1}"
        self.feahash = pyfeather.FeaHash(CONF)
        self.blank_feahash = pyfeather.FeaHash()

    def test_GetFeaHash_GetSlotID(self):
        fea3val1_id: int = self.feahash.GetFeaHash("fea3", "val1")
        fea3val1_id_str: str = str(fea3val1_id[0])
        slot_id: int = self.feahash.GetSlotID("fea3")
        fea3val1_id_prefix: str = fea3val1_id_str[:len(str(slot_id))]
        self.assertTrue(slot_id == int(fea3val1_id_prefix), 
                msg=self.msg_temp.format(slot_id, fea3val1_id_str))

        fea10val1_id: int = self.feahash.GetFeaHash("fea10", 3.14)
        fea10val1_id_str: str = str(fea10val1_id[0])
        slot_id: int = self.feahash.GetSlotID("fea10")
        fea10val1_id_prefix: str = fea10val1_id_str[:len(str(slot_id))]
        self.assertTrue(slot_id == int(fea10val1_id_prefix), 
                msg=self.msg_temp.format(slot_id, fea10val1_id_str))

        fea11val1_id: int = self.feahash.GetFeaHash("fea11", [1,2,3,4])
        fea11val1_id_str: str = str(fea11val1_id[0])
        slot_id: int = self.feahash.GetSlotID("fea11")
        fea11val1_id_prefix: str = fea11val1_id_str[:len(str(slot_id))]
        self.assertTrue(slot_id == int(fea11val1_id_prefix), 
                msg=self.msg_temp.format(slot_id, fea11val1_id_str))

    def test_GetFeaHash(self):
        fea1val1_id: int = self.feahash.GetFeaHash("fea1", "2")[0]
        target: str = "10100025"
        self.assertTrue(str(fea1val1_id) == target, 
                msg=self.msg_temp.format(fea1val1_id, target))

        fea1val1_id: int = self.feahash.GetFeaHash("fea1", 2)[0]
