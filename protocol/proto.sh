#!/bin/bash
protoc -I=./ --cpp_out=./ $1
