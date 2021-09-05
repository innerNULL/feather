# -*- coding: utf-8 -*-


import pyfeather


if __name__ == "__main__":
    libsvm_extractor = pyfeather.LibsvmExtractor("./conf/feather.conf", "ctr", False)
    
    record1 = "{\"fea1\":2, \"fea10\":3.14, \"fea11\":[2.3, 1.4, 3.5, 6.8], \"ctr\":1}"
    output1 = libsvm_extractor.Extract(record1)
    print("output1: ", output1)
