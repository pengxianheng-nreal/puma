## 编译Android arm64, arm32 libusb的动态链接库
### 环境依赖
- cmake 3.13 及以上
- Android NDK 21.4.7075529
### 编译方式
- 进入项目根目录

```bash
export ANDROID_NDK_DIR=""
export BUILD_DIR=""

cmake -H. -B$BUILD_DIR -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_DIR/build/cmake/android.toolchain.cmake -DANDROID_STL=c++_static -DANDROID_NATIVE_API_LEVEL=26 -DANDROID_ABI=arm64-v8a -DCMAKE_BUILD_TYPE=Release

cmake --build $BUILD_DIR
```
 