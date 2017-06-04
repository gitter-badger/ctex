# CTex  

Generate `LaTeX` math equations from `C` source code.

`CTex` is the tool to simplify documentation generation for source code with a lot of math expressions.  

`CTex` can help you in the following ways:  

* Quickly generates `LaTeX` equations for huge amount of math expressions in a source code.  
* Helps with representation of math formula in `LaTeX` format if you don't familiar with syntax.
* Generates `Doxygen`-friendly output

`CTex` is developed under macOS, but is set-up to be highly portable. As a result, it runs on Windows and  on a variety of Unix flavors as well.

## Installation

```
git clone https://github.com/galarius/ctex.git
cd ctex
mkdir build  && cd build
cmake -G "Unix Makefiles" ..
make && make install
```

## Usage

* `ctex.exe <input.c> <output.c>`  

* `ctex.exe -i` - interactive mode

## Contributing

There are plenty of possible improvements ([Check for open issues](https://github.com/galarius/ctex/issues)):

* Expansion of support for `LaTeX` syntax.  

* Generation of prettier output.  

* Test coverage.  

Here's a quick guide on `pull requests`:

1. [Check for open issues](https://github.com/galarius/ctex/issues), or
   open a fresh issue to start a discussion around a feature idea or a bug.
   Opening a separate issue to discuss the change is less important for smaller
   changes, as the discussion can be done in the pull request.  
2. [Fork](https://github.com/galarius/ctex.git) this repository on GitHub, and start making your changes.
3. Check out the README for information about the project setup and usage.
3. Push the change (it's recommended to use a separate branch for your feature).
4. Open a pull request.
5. I will try to merge and deploy changes as soon as possible, or at least leave
   some feedback, but if you haven't heard back from me after a couple of days,
   feel free to leave a comment on the pull request.

## Documentation 

See [Documentation](https://galarius.github.io/ctex/static/doc/index.html)

## License

Copyright (c) 2017 by Shoshin Ilya.
Permission to use, copy, modify, and distribute this software and its documentation under the terms of the GNU General Public License is hereby granted. No representations are made about the suitability of this software for any purpose. It is provided "as is" without express or implied warranty. See the [GNU General Public License](http://www.gnu.org/licenses/gpl.html) for more details.