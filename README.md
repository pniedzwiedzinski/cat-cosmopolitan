# cat

This is a simple C program that outputs file content. The magic is that it works
on Linux, *BSD, macOs, Windows thanks to [cosmopolitan](https://github.com/jart/cosmopolitan).

I created this repo as a project template for simple ape programs. `/ape` folder
contains https://justine.lol/cosmopolitan/cosmopolitan-amalgamation-0.3.zip and
`/stdio` is copied from cosmopolitan@7ed524ca31bca83fe6700045d346cad3f97b9567

## Running

```sh
wget https://niedzwiedzinski.cyou/cat.com
chmod +x cat.com
sha256sum cat.com
# 495b3bb169813b07d4c4c43568c6c395e72a952fdf7fe3e5b46bf130cc415af0

# For zsh or fish
bash -c "./cat.com"

# For everything else
./cat.com
```

## Compiling

Requirements:

- make
- gcc
- objcopy (from pkg `binutils`)

```
make
```
