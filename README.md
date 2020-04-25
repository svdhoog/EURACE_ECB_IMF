# The Eurace@ECB-IMF Model

This is the source code of the Eurace@ECB-IMF Model.

**Version history:**

1.0.0 (April 2020)

**Dependencies**

GCC 4.4 or higher (or other C compiler)

GSL 2.4 or higher (GNU  Scientific  Library)

FLAME XParser 0.17.1 (https://github.com/FLAME-HPC/xparser/archive/0.17.1.tar.gz)

FLAME Libmboard 0.3.1 (https://github.com/FLAME-HPC/libmboard/archive/0.3.1.zip)

# Installation of FLAME libraries on stand-alone systems and building the model

Step 2-4 (installation of FLAME libraries) can also be run via a script
`./scripts/install_flame.sh`.

## 1. Prepare system requirements

Make sure to have `cunit` and `gsl` installed.
- On Debian-based systems: `sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev libgsl-dev`.
- On macOS: `brew install cunit gsl`.
- On Windows (TODO: test on appveyor): `TODO`.

## 2. Install libmboard

On top of the repo root directory, download libmboard 0.3.1
(https://github.com/FLAME-HPC/libmboard/archive/0.3.1.tar.gz) and extract the tarball

```bash
mkdir libmboard
cd libmboard-0.3.1
chmod +x autogen.sh
./autogen.sh
# Remove --disable-parallel if you have mpi installed
# Remove --disable-tests if you have cunit installed
./configure --prefix=$(cd ../libmboard; pwd) --disable-parallel --disable-tests
make
make install
# back to the repo root directory
cd ..
```

## 3. Install xparser
## 3a.
Download xparser 0.17.1 (https://github.com/FLAME-HPC/xparser/archive/0.17.1.tar.gz) and
extract to xparser/ directory on top of the repo root directory.
```bash
wget https://github.com/FLAME-HPC/xparser/archive/0.17.1.tar.gz xparser-0.17.1.tar.gz
tar xfv xparser-0.17.1.tar.gz xparser
```
## 3b.
Build the xparser executable by running make inside the xparser folder:
```bash
cd xparser
make
```
## 3c.
In Makefile.tmpl (template file used by xparser to create your model's Makefile),
replace LIBMBOARD_DIR from `/usr/local` to your local installation folder of Libmboard.

```bash
# LIBMBOARD_DIR="/usr/local"
LIBMBOARD_DIR=$(PWD)/libmboard
```

## 4. XParse your model XML file to generate the makefile for the model
```bash
./xparser ../model_folder/eurace_model.xml
```

## 5. Build the model

Enter your model folder, run `make`, which should compile everything if the previous steps are
executed correctly.
```bash
cd model_folder
make
```

## 6. Common errors

* Xparser errors
* Compilation errors
* Libmboard not installed correctly
* GSL not installed correctly

## Further Links:

1. [http://www.wiwi.uni-bielefeld.de/lehrbereiche/vwl/etace/Eurace_Unibi/](http://www.wiwi.uni-bielefeld.de/lehrbereiche/vwl/etace/Eurace_Unibi/)
2. [https://pub.uni-bielefeld.de/data/2900767](https://pub.uni-bielefeld.de/data/2900767)
3. [http://www.flame.ac.uk](http://www.flame.ac.uk)


Contributing
------------

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Added some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

How to cite
--
The Eurace@ECB-IMF Source Code is a Modified Version of the Eurace@Unibi Source Code (Standard Version, 2014).

Following the EULA license of the Eurace@Unibi Source Code, any publication that contains results derived from the Eurace@Unibi Source Code must contain the following (or similar) acknowledgement:

This work uses (a modified version of) the Eurace@Unibi model source code, developed by Herbert Dawid, Simon Gemkow, Philipp Harting, Sander van der Hoog and Michael Neugart. More details on the Eurace@Unibi model can be found in these publications:

Dawid, H., Harting, P., van der Hoog, S., & Neugart, M. (2018a). Macroeconomics with heterogeneous agent models: Fostering transparency, reproducibility and replication. Journal of Evolutionary Economics. DOI:10.4119/unibi/2902981

Dawid, H., Gemkow, S., Harting, P., van der Hoog, S., & Neugart, M. (2018b). Agent-based macroeconomic modeling and policy analysis: The Eurace@ECB-IMF Model. In S.-H. Chen, M. Kaboudan, & Y.-R. Du (Eds.), The Oxford Handbook of Computational Economics and Finance (Ch. 17, pp. 490–519). Oxford: Oxford University Press. Available online as Working Paper: urn:nbn:de:0070-pub-26224984

BiBTeX:

@INCOLLECTION{Dawid_2018,
	Author = {H. Dawid and S. Gemkow and P. Harting and S. van der Hoog and M. Neugart},
	Title = {{Agent-Based Macroeconomic Modeling and Policy Analysis: {The Eurace@ECB-IMF} Model}},
	Editor = {Chen, S-H and Kaboudan M. and Du, Y.-R.},
	Booktitle = {The Oxford Handbook of Computational Economics and Finance},
	Publisher = {Oxford University Press},
	Year = {2018},
	Chapter = {17},
	Pages = {490-519},
}

@ARTICLE{Dawid_2019,
	author       = {Dawid, Herbert and Harting, Philipp and {van der Hoog}, Sander and Neugart, Michael},
	title        = {{A Heterogeneous Agent Macroeconomic Model for Policy Evaluation: Improving Transparency and Reproducibility}},
	year         = {2019},
	journal 	  = {Journal of Evolutionary Economics},
}


Copyright
---------

Copyright (c) 2014-2019 Herbert Dawid, Simon Gemkow, Philipp Harting, Sander van der Hoog, Michael Neugart.

Copyright (c) 2020 Sander van der Hoog, Dirk Kohlweyer, Marco Gross, Björn Hilberg.

License
--

See the LICENSE file.

Disclaimer
--

The package is provided by the owner and contributors "as is' and without any express or implied warranties. The implied warranties of merchantability, fitness for a particular purpose, or non-infringement are disclaimed to the extent permitted by your local law. unless required by law, no owner or contributor will be liable for any direct, indirect, incidental, or consequential damages arising in any way out of the use of the package, even if advised of the possibility of such damage.
