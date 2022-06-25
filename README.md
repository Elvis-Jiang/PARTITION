# PARTITION
## Introduction

A graph stream refers to an unbounded, time involving edge stream, in which, each weighted edge has two endpoints and a timestamp, forming a dynamic graph changing rapidly over time. Graph streams have been widely generated in social networks, cyber security, and data center. Due to its huge volume and high update speed, 
recent research mainly focus on graph stream summarization to achieve linear memory usage and constant time cost per edge insertion with slight accuracy sacrifice. However, these summarization structures all sllocate
the counters the same size of bits while in real-world applications, the weights are always highly skewed. Therefore, when we allocate the size of the counters, we have to 
guarantee the maximum weight will not overflow, the higher bits of the small counters are wasted. PARTITION proposes a novel small counters unioning strategy with little memory 
and time overhead. With the same memory cost, our desing reduce the average relative error by 67% on average.


## About the source code and data sets

We have implemented the basic idea TCM and our desing PARTITION in C++. We complete the code on Linux 5.4.0-99-generic and compile successfully using gcc 7.5.0. The hash function we use is in the file querysupportstruct.h.

All the data sets we use can be downloaded from the below website:  
```
    https://catalog.caida.org/details/dataset/passive_2015_pcap  
    https://networkrepository.com  
    http://dl.acm.org/citation.cfm?id=2488173
```

The data sets should be preprocessed to the format as below:  
* rawdata.txt -- The adjacency matrix of the network in whitespace-separated values format, with one edge per line  
    The meaning of the columns are:  
    ```
        First column: ID of from node  
        Second column: ID of to node  
        Third column: edge weight  
        Fourth column: timestamp of the edge  
     ```

* edgeQueryTest.txt -- Test data for edge query
    The meaning of the columns are:
    ```
        First column: ID of from node  
        First column: ID of to node  
        Third column: Accumulated weight of the edge  
    ```


* nodeOutQueryTest.txt & nodeInQueryTest.txt -- Test data for node out (in)-flow query
    The meaning of the columns are:  
    ```
        First column: ID of from (to) node  
        Second column: Accumulated weight of the node  
    ``` 
