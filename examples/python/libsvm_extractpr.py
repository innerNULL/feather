# -*- coding: utf-8 -*-


import pyfeather


def libsvm_extractor():
    print("=" * 35 + " libsvm_extractor " + "=" * 35)
    libsvm_hash = pyfeather.LibsvmExtractor("./conf/feather.conf", "ctr", False)
    libsvm_index = pyfeather.LibsvmExtractor("./conf/feather.conf", "ctr", True)

    record1 = "{\"fea1\":2, \"fea10\":3.14, \"fea11\":[2.3, 1.4, 3.5, 6.8], \"ctr\":1}"
    output1_hash = libsvm_hash.Extract(record1)
    output1_index = libsvm_index.Extract(record1)  
    print("output1_hash: {}".format(output1_hash))
    print("output1_index: {}".format(output1_index))


if __name__ == "__main__":
    libsvm_extractor()
