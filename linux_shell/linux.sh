#!/bin/sh
echo copy the libmemenv.a and libleveldb.a to leveldb/
echo copy autogen-koala-man.sh to topdir
cp -rf ./libmemenv.a ../src/leveldb
cp -rf ./libleveldb.a ../src/leveldb
cp -rf ./autogen-koala-man.sh ../
cp -rf ./genbuild.sh ../share/
