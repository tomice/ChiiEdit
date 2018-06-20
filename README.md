# ChiiEdit
ChiiEdit is a basic text editor written in C without relying on any libraries.

![demo](https://user-images.githubusercontent.com/8818630/41635712-3f6ba3c2-7418-11e8-991e-061f13b14764.gif)

## Installation
To install locally into ~/usr/local/bin:

```bash
./build.sh
```

This will place ChiiEdit in ~/usr/local/bin. You can then add it to your PATH:

```bash
export PATH="${PATH}":/home/"${USER}"/usr/local/bin
```

To install the manual autotools way:

```bash
autoreconf -iv
./configure
make
make install
```

## Usage
ChiiEdit can be executed by itself to start creating a file from scratch.

To modify a file, or create a new one, simply pass it a filename:

```bash
ChiiEdit foo.txt
```

## Author
Written by Tom Ice.

## License
This is under the BSD-2 License. See actual license file for more information.
