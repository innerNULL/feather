# -*- coding: utf-8 -*-
# file: example.py

import pyfeather


def main():
    #fea_slot = pyfeather.FeaSlot("fea0", 1, 128)
    #fea_slot.Info()

    fea_hash = pyfeather.FeaHash("../conf/feather.conf")
    
    fea1val1_id = fea_hash.FeaRegister("fea1", "val1")
    print("fea1#val1", fea1val1_id)
    fea1val2_id = fea_hash.FeaRegister("fea1", "val2")
    print("fea1#val2", fea1val2_id)

    fea2val1_id = fea_hash.FeaRegister("fea2", "val1")
    print("fea2#val1", fea2val1_id)
    fea2val2_id = fea_hash.FeaRegister("fea2", "val2")
    print("fea2#val2", fea2val2_id)

    fea8val1_id = fea_hash.FeaRegister("fea8", "val1")
    print("fea8#val1", fea8val1_id)
    fea8val2_id = fea_hash.FeaRegister("fea8", "val2")
    print("fea8#val2", fea8val2_id)

    fea10val1_id = fea_hash.FeaRegister("fea10", 3.14)
    print("fea10#val1", fea10val1_id)
    fea10val2_id = fea_hash.FeaRegister("fea10", 128)
    print("fea10#val2", fea10val2_id)

    fea11val1_id = fea_hash.FeaRegister("fea11", [1,2,3,4])
    print("fea11#val1", fea11val1_id)
    fea11val2_id = fea_hash.FeaRegister("fea11", [4.0, 3.0, 2.0, 1.0])
    print("fea11#val2", fea11val2_id)


    libsvm_extractor = pyfeather.LibsvmExtractor("../conf/feather.conf", "ctr", False)
    record1 = "{\"fea1\":2, \"fea2\": \"a\", \"fea8\": \"123\", \"fea10\": 3.14, \"fea11\": [2.3, 1.4, 3.5, 6.8], \"ctr\": 1}"
    print(record1)
    target = \
        "1.000000 10100025:1 11000000:3.140000 11100000:2.300000 11100001:1.400000 11100002:3.500000 11100003:6.800000 10200026:1 10805285:1"
    output = libsvm_extractor.Extract(record1)
    print(target)
    print(output)
    print(output.strip(" ") == target)


if __name__ == "__main__":
    main()
