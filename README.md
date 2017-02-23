# dds-example

Vortex Lite pub-sub examples

## // What is a dds entities

Entities is an abstract base class in DCPS, you can learn more from this [slides](http://www.slideshare.net/cmwang/entities-in-dcps-dds).

## // Procedures

### publish side

1. Create one (and only one) domain participant entity.
1. Create a publisher entity
1. Create a DDS topic entity
1. Create a typed writer entity
1. Publish data from typed writer

### subscriber side

1. Create one (and only one) domain participant entity.
1. Create a subscriber entity
1. Create a DDS topic entity
1. Create a typed reader entity
1. Receive data from typed reader

---

## // File Descriptions

- mb.idl : sample IDL for DDS topics
- pub.c  : publish sample every 100 ms
- sub1.c : subscriber with listener (async)
- sub2.c : subscriber with waitset (blocking)
- sub3.c : subscriber with waitset (blocking) and [thread pool](https://github.com/mbrossard/threadpool)
- sub4.c : subscriber with waitset (blocking) and [lock-free thread pool](https://github.com/xhjcehust/LFTPool)


## // Build

```bash
mkdir build && cd build
cmake ..
make 
./sub2
./pub
```
