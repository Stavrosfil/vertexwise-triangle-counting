## How to fix cilk on Arch linux/Manjaro

Make sure you have installed `ncurses`

Then you need to install `ncurses5-compat-libs` to avoid messages like: `error while loading shared libraries: libtinfo.so.5: cannot open shared object file: No such file or directory`

```bash
> gpg --keyserver keys.gnupg.net --recv-keys 702353E0F7E48EDB

> yay -S ncurses5-compat-libs
```

Profit.