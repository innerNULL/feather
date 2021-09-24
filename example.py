# -*- coding: utf-8 -*-
# file: example.py

import pyfeather


def main():
    #fea_slot = pyfeather.FeaSlot("fea0", 1, 128)
    #fea_slot.Info()

    fea_hash = pyfeather.FeaHash("../conf/feather.conf")
    
    fea1val1_id = fea_hash.GetFeaHash("fea1", "val1")
    print("fea1#val1", fea1val1_id)
    fea1val2_id = fea_hash.GetFeaHash("fea1", "val2")
    print("fea1#val2", fea1val2_id)
    fea1val1val2_id = fea_hash.GetFeaHash("fea1", ["val1", "val2"])
    print("fea1#val1_val2", fea1val1val2_id) 

    fea2val1_id = fea_hash.GetFeaHash("fea2", "val1")
    print("fea2#val1", fea2val1_id)
    fea2val2_id = fea_hash.GetFeaHash("fea2", "val2")
    print("fea2#val2", fea2val2_id)

    fea8val1_id = fea_hash.GetFeaHash("fea8", "val1")
    print("fea8#val1", fea8val1_id)
    fea8val2_id = fea_hash.GetFeaHash("fea8", "val2")
    print("fea8#val2", fea8val2_id)

    fea10val1_id = fea_hash.GetFeaHash("fea10", 3.14)
    print("fea10#val1", fea10val1_id)
    fea10val2_id = fea_hash.GetFeaHash("fea10", 128)
    print("fea10#val2", fea10val2_id)

    fea11val1_id = fea_hash.GetFeaHash("fea11", [1,2,3,4])
    print("fea11#val1", fea11val1_id)
    fea11val2_id = fea_hash.GetFeaHash("fea11", [4.0, 3.0, 2.0, 1.0])
    print("fea11#val2", fea11val2_id)

    fea12val1_1_id = fea_hash.GetFeaHash("fea12", "1")
    fea12val1_2_id = fea_hash.GetFeaHash("fea12", 1)
    print("fea12='1' vs fea12=1: ", fea12val1_1_id, fea12val1_2_id)


    libsvm_extractor = pyfeather.LibsvmExtractor("../conf/feather.conf", "ctr", False)
    record1 = "{\"fea1\":2, \"fea2\": \"a\", \"fea8\": \"123\", \"fea10\": 3.14, \"fea11\": [2.3, 1.4, 3.5, 6.8], \"ctr\": 1}"
    record2 = "{\"fea1\":\"2\", \"fea2\": \"a\", \"fea8\": \"123\", \"fea10\": 3.14, \"fea11\": [2.3, 1.4, 3.5, 6.8], \"ctr\": 1}"
    print("record1: \n", record1)
    print("record2: \n", record2)
    target = \
        "1 10100025:1 11000000:3.140000 11100000:2.300000 11100001:1.400000 11100002:3.500000 11100003:6.800000 10200026:1 10805285:1"
    target = target.split(" ")
    target = [target[0]] + sorted(target[1:], key=lambda x: x.split(":")[0])
    target = " ".join(target)
    output1 = libsvm_extractor.Extract(record1, True)
    output2 = libsvm_extractor.Extract(record2, True)  
    print("target:\n" + target)
    print("output1:\n" + output1)
    print("output2:\n" + output2)
    print(output1.strip(" ") == target)
    print(output2.strip(" ") == target) 


    # Map fea-hash back to fea-group
    hash2name_1 = fea_hash.FeaHash2FeaName(10100025)
    print("10100025 is fea-hash of %s" % hash2name_1)
    hash2name_11_0 = fea_hash.FeaHash2FeaName(11100000)
    print("11100000 is fea-hash of %s" % hash2name_11_0)
    hash2name_11_3 = fea_hash.FeaHash2FeaName(11100003)
    print("11100003 is fea-hash of %s" % hash2name_11_3)
    hash2name_10 = fea_hash.FeaHash2FeaName(11000000)
    print("11000000 is fea-hash of %s" % hash2name_10)


    libsvm_extractor = pyfeather.LibsvmExtractor("../conf/feather.conf", "ctr", True)
    output1_idx = libsvm_extractor.Extract(record1, True) 
    print("output1_idx:\n" + output1_idx)

if __name__ == "__main__":
    main()
