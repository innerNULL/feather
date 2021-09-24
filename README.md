# feather
FEATure HashER 


## Build & Install
* **CPP**  
```bash
cd  PATH/TO/FEATHER
mkdir build && cd build
cmake ../ -DBUILD_TESTS=ON # build with test 
#or 
cmake ../

make -j12
```

* **Python with pip**    
```bash
cd feather && python3 -m pip install ./ -vvv
# or
python -m pip install https://github.com/innerNULL/feather/archive/refs/heads/main.zip -vvv
# or
python -m pip install pyfeather
```
here is what you may see:  
```
Processing /Path/To/feather                                                                                        
Building wheels for collected packages: pyfeather
Building wheel for pyfeather (setup.py) ... done  
Created wheel for pyfeather:
filename=pyfeather-0.0.1-cp37-cp37m-macosx_10_15_x86_64.whl size=1284474 sha256=e3f9d0be1e7578274f3fcecb854c1e66336a24985b8e6ff4213375d76463299e
Stored in directory: /private/var/folders/4q/50_2647d1yb47jt9j6plwx2r0000gq/T/pip-ephem-wheel-cache-996awbes/wheels/0f/bd/93/b6936ec0c1169201de264147e21ae7e2bb894720b34bcdce79
Successfully built pyfeather
Installing collected packages: pyfeather
Successfully installed pyfeather-0.0.1  
```

* **Python with poetry**  
```bash
cd feather && poetry run python -m pip install ./ -vvv
# or
poetry run python -m pip install https://github.com/innerNULL/feather/archive/refs/heads/main.zip -vvv
```

## How to Use
### Feature-Hash
Here is an simple example:
```python
import pyfeather
from typing import List

# Loading the pre-defined feature schema.
feahash = pyfeather.FeaHash("./conf/feather.conf")

# Getting hash of value 2 and '2' of 'fea1', which is an discrete 
# feature, and both hash results should be same.
fea1_hash_str2: List[int] = feahash.GetFeaHash("fea1", "2")
fea1_hash_int2: List[int] = feahash.GetFeaHash("fea1", 2)
# [10100070] [10100070]
print(fea1_hash_str2, fea1_hash_int2)

# Getting hash of value 3.14 and '3.14' and 5.12 of "fea10", which 
# is an continuous feature, and the hash-bucket of any value of 
# this feature should always be 0, so all value has same feature 
# hash result.
fea10_hash_float3p14: List[int] = feahash.GetFeaHash("fea10", 3.14)
fea10_hash_str3p14: List[int] = feahash.GetFeaHash("fea10", '3.14')
fea10_hash_float5p12: List[int] = feahash.GetFeaHash("fea10", 5.12)
# [11000000] [11000000] [11000000] 
print(fea10_hash_float3p14, fea10_hash_str3p14, fea10_hash_float5p12)

# Getting hash of value of [4.0, 3.0, 2.0, 1.0] and [1.0, 2.0, 3.0, 4.0] 
# of "fea11", which is an vector feature with dimension as 4, so all 
# 4-dim vectors' feature-hash-bucket of this feature should always 
# be [0, 1, 2, 3], and like continusous-feature, all feature-hash of 
# any value of this feature should be same.
fea11_hash_4to1: List[int] = feahash.GetFeaHash("fea11", [4.0, 3.0, 2.0, 1.0])
fea11_hash_1to4: List[int] = feahash.GetFeaHash("fea11", [1.0, 2.0, 3.0, 4.0])
# [11100000, 11100001, 11100002, 11100003]
# [11100000, 11100001, 11100002, 11100003]
print(fea11_hash_4to1, fea11_hash_1to4)
```


## Feature Hashing
### Notions
* **Feature Value**:  
Defined by `FeaValue`, which is a unified wrapping of feature data. Feature-data could be classified into three types: discrete-feature, continuous-feature, vector-feature.  
    * **Discrete Feature**:  
    The input could be `std::string`, `int32_t`, `float`, `double`, no matter what input type it is, the input will be casted into `std::string` and saved as `FeaValue::discrete_val`.  
    * **Continuous Feature**:  
    The input could be `std::string`, `int32_t`, `float`, `double`, no matter what input type it is, the input will be casted into `float` and saved as `FeaValue::continuous_val`.  
    * **Vector Feature**:  
    The input could be `std::vector<std::string>`, `std::vector<int>`, `std::vector<float>`, no matter what input type it is, the input will be casted into `std::vector<float>` and saved as `FeaValue::vec_val`.  

`FeaValue` instance will also record some feature meta-data, such as feature-type, 0 for discrete-feature, 1 for continuous-feature, 2 for vector-feature.  

Besides, `FeaValue` support transform feature-value to feature-value's hash id by `FeaValue::GetHash`, according feature-type:  
* **Discrete Feature Hash**:  
Just the result of calling `std::hash` on `FeaValue::discrete_val`.  
* **Continuous Feature Hash**:  
Always returns 1 as feature-hash. This is because continuous-feature actually don't needs feature-hashing, so its **Feature-Slot**'s bucket-size should always be 1, so by assign its feather hash-id to 1, when using mod function to hash-id on slot-bucket-size, we can always assign continuous-feature value on 0-bucket of its slot.   
* **Vector Feature Hash**:  
Similiar to continuous-feature case, vector-feature also doesn't need feature-hashing operation. So we can assign each element in vector to slot-bucket which id corresponding to element's index.  
To do this, we can let each element's hash-id as `element-index + slot-bucket-size`, when using mod function to hash-id on slot-bucket-size, we can always assign each element's slot-bucket to `element-index`.   
For example, a 3-dim vector [3.14, 5.21, 6.79], its slot-bucket-size must be 3 (same with its dimension), we assign each element's hash-id as [3 + 0, 3 + 1, 3 + 2], and so each element's slot-bucket-id is [3 % 3, 4 % 3, 5 % 3].   

* **Feature Slot**  
Defined in `FeaSlot`. Each feature corresponing to a "slot", the feature-slot takes responsibility to map each feature value's hash to a bucket by executing mod operation on feature value's hash with slot's bucket-size.
* **Feature Hash**  
Define in `FeaHash`. There is a config which defines schema of target features, the 1st column is feature-name, 2nd column is feature-slot id, 3rd column is feature-slot hash-bucket number, 4th column is feature-type (0 for discrete-feathe, 1 for continuous-feature, 2 for vector-feature). The `FeaHash` will record/register all slots at construction function. 

* **Feature Indexer**(TODO)


* **Feature Extractor**  
The base class is `FeaExtractor`, but the mainly using case is mapping feature's hash-id/index and value (in continuous and vector feature case) to libsvm format, which can be done by `LibSVMExtractor`.

* **Bucket-ID and Bucket-Code**  
Briefly, **Bucket-ID** is `int32_t`, **Bucket-Code** is `std::string` which digits/length should be fixed. Here are some example about mapping bucket-id to bucket-code, which all bucket-code has 5 digits:  
    * bucket-id: 5 -> bucket-code: '00005'  
    * bucket-id: 54234 -> bucket-code: '54234'  
    * bucket-id: 568 -> bucket-code: '00568'   

### Algorithm
Each feature has a slot and hash-bucket size, the finally hash of this feature is a `int64` in the format as `${SLOT}${HASH-BUCKET-CODE}`, since the top digit of this int64 is controled by slot, so each feature's finally hash value will far away with each other, the second part is '**part of** hash value of feature value according feature hash-bucket size' because, hash value of feature value is a int64, finally-hash is also a int64, if we just concat slot and hash value of feature value, then the finally result number will have posibillity to overflow from int64 range.

by the way, in case we want adjust each feature-slot's hash-bucket size, we can maintain a hash-ring/consistant hash for each slot.

## TODO
* `FeaHash::Hash2IndexDictBuild` should supports rebuild mode.
