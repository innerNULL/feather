# -*- coding: utf-8 -*-
# file: dev_test.py

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

    libsvm_extractor = pyfeather.LibsvmExtractor("../conf/feather.conf", False)
    record1 = "{\"fea1\":2}"
    #print(record1)
    print(libsvm_extractor.Extract("{\"fea1\":2, \"fea2\": \"a\", \"fea8\": \"123\"}"))


if __name__ == "__main__":
    main()
