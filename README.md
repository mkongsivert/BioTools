# BioTools
Implementation of some existing bioinformatics algorithms.

## BBHash
Implementation of the BBHash algorithm as described in "Fast and scalable minimal perfect hashing for massive key sets" by Limasset, et al. (2017)

I timed this implementation with various values of gamma (1, 1.25, 1.5, 1.75, and 2) and numbers of keys ranging from 100 to 1 |000 |000. In particular , I tested by loading in n words from this list of English words: <https://github.com/dwyl/english-words/blob/master/words.txt>. From my limited data, construction seems to take linear time.
