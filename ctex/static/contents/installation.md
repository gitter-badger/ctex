The latest version of `CTex` can be found on ![github](static/images/octocat-icon.png)[GitHub](https://github.com/galarius/ctex.git)

If you have GIT installed, you should do the following to get the initial copy of the repository:

    git clone https://github.com/galarius/ctex.git
    cd ctex

After that you can use

    mkdir build  
    cd build
    cmake -G "Unix Makefiles" ..
    make 

To force a fresh build after an earlier check-out simple remove the build directory and redo the steps above.
After the binary has been built, you can use

    make install

to install it.