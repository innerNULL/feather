# How Feature Hashing Works
# Overview
## Why Feature Hashing
In NLP or recommender-system fields, we always meet a lot of descete/categorical features, one way to handle this kind of feature is by building a token dictionary, which helps mapping each token to an integer ID, and with this ID we can to embedding or one-hot encoding.

The above method can fit NLP case well since we can roughly see language tokens as a closed-form set, which means new words are not created very frequently. But for recommender system, here is a different story.

In recommenrder system (or other sparse tabular data case), we always meet open-set tokens, for example, how about using the articles' key-words as feature. For this kind of open-set tokens, we can of course building a dictionary and map each token to corresponding id, but here are some drawbacks:
* Since the new token happens in high frequency, we need update dictionary, in a corresponding high frequency.
* Updating token-ID dictionary in high frequency means we also need update model in a high frequency.
* Since we are working on open-set tokens, if we map all token to a distinct ID, the ID set size will be too huge, which will make both model training and online inference hard.

Feature hashing can solve above problems more or less. Briefly speaking, feature hashing just bucketing tokens by first calculating token's hash and then moding hash value to determine which bucket it belongs to. Of course this will cause hash-collisions if the mod of two tokens' hash are same. The more bucket we have, the less hash-collision will happens.

Hash-collisions are bad, but we do not to worry a lot, although several token are collisions into one bucket, but this bucket will be dominated by the token with highest frequency in training dataset, so there are two cases:
* **Two High Frequency Tokens Happened Collision**: In this case, we should increase the **bucket-number** parameter.
* **High Frequency Token Collided with Some Low Frequency Tokens**: Do not need to worry about this since high frequency token will donominate this bucket during training, this can also understood as automated cutting-off "stop-words/tokens" which happened very rarely.


# About Feature Hash Implement
## Targets
* Each feature value could be map into a feature hash and a feature index.
* Feature hashs are inconsecutive integers, and feature index must be consecutive integers, but not necessary starts from 1 or 0.
* Each feature's slot shoud has same digits number.
* All feature-values' feature hash should has same digits number.
* Since each feature has a unified **slot ID**, so feature-hash for different values under same feature should have same prefix.
  
## Notions/Parameters
* **Hash Function**: The function helps convert `string` to `int64`, default using murmurhash.
* **Feature Slot ID**: Each feature should has a slot-ID, and all features slot-IDs should have same digits number. 
* ***Feature Slot Digits Number**: For example, a feature slot "101", which slot digits number is 3, and for "2345", the slot digits number is 4. **Note, for now all feature's slot ID should have same slot digits number**, this is helpful for decoding feature-slot from feature-hash and inverse mapping feature-hash to feature-slot/feature-name.
* **Bucket Size**:
  * **Categorical Feature**: You should careful decide this value based on EDA results, to avoid or decrease hash-collision impact.
  * **Numerical Feature**: Do not need actually bucketing, so set bucket size will be forced set to 1, and the feature value is original value.
  * **Vector Feature**: Since for a numerical vector feature, we do not need actually bucketing, but we need to convert it to a simulated "bucketed form" to keep results consistance, so we align each element with a bucket, so for vector features, the bucket-size should be same with vector dimension.
* **Unified Bucket Size Digits Number**: Suppose we have three feature, corresponding bucket-sizes are 10, 100, 1000. Than the unified bucket size digit number shoud be the max bucket size digit number, which means .By doing this, for all features, we have same length of slot-ID, 


## Feature Hash Pipeline
### Mapping Feature Value to Feature Hash
### Mapping Feature Hash to Feature Index
### Decode Feature Hash From Feature Index
### Decode Feature Slot/Name from Feature Hash