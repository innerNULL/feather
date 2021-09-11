# -*- coding: utf-8 -*-

import pyfeather


def dynamical_init_feahash():
    print("=" * 35 + " dynamical_init_feahash " + "=" * 35)
    
    feahash = pyfeather.FeaHash()
    feahash.SchemaLineRegister(["discrete_fea", "100", "128", "0"])
    feahash.SchemaLineRegister(["continuous_fea", "101", "1", "1"]) 
    feahash.SchemaLineRegister(["vec_fea", "102", "32", "2"])

    discrete_fea_val2 = feahash.FeaRegister("discrete_fea", "2")
    print("discrete_fea's fea-hash with value '2': {}".format(discrete_fea_val2[0]))
    discrete_fea_val2 = feahash.FeaRegister("discrete_fea", 2)
    print("discrete_fea's fea-hash with value 2: {}".format(discrete_fea_val2[0]))
    
    continuous_fea_val3p14 = feahash.FeaRegister("continuous_fea", "3.14")
    print("continuous_fea's fea-hash with value '3.14': {}".format(continuous_fea_val3p14[0]))
    continuous_fea_val3p14 = feahash.FeaRegister("continuous_fea", 3.14)
    print("continuous_fea's fea-hash with value 3.14: {}".format(continuous_fea_val3p14[0]))
    continuous_fea_val5p12 = feahash.FeaRegister("continuous_fea", 5.12) 
    print("continuous_fea's fea-hash with value 5.12: {}".format(continuous_fea_val5p12[0]))

    vec_fea_1to32 = feahash.FeaRegister("vec_fea", list(range(32, 64)))
    print("vec_fea's fea-hash with value 32 to 63: {}".format(vec_fea_1to32))


def init_feahash_with_schema_conf():
    print("=" * 35 + " init_feahash_with_schema_conf " + "=" * 35)
    feahash = pyfeather.FeaHash("./conf/feather.conf") 




if __name__ == "__main__":
    init_feahash_with_schema_conf()
    dynamical_init_feahash()
