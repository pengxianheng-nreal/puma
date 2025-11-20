#!/bin/bash
g++ -shared cap_dshow.cpp -I . -O2 -fomit-frame-pointer -msse -msse2 -ffunction-sections -fPIC -luuid -lole32 -loleaut32 -o cap_dshow.dll