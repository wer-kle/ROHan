# ROHan: inference of heterozygosity rates and runs of homozygosity for modern and ancient samples
==========================================================

QUESTIONS :
   gabriel [dot] reno [ at sign ] gmail [dot] com


About
----------------------

ROHan is a Bayesian framework to estimate local rates of heterozygosity, infer runs of homozygosity (ROH) and compute global rates of heterozygosity. ROHan can work on modern and ancient samples with signs of ancient DNA damage.

Description
----------------------

Diploid organisms have minor differences between corresponding pairs of autosomes. Differences of a single nucleotide create heterozygous sites. Very recent inbreeding can lead to large stretches of genomic deserts of heterozygosous sites. Such deserts are called runs of homozygosity (ROH). 

Due to the lack of heterozygosous sites, ROHs can cause an underestimate of global estimates of heterozygosity. Such global estimates of rates of heterozygous sites is useful to infer population genetics parameters such as effective population size (Ne). 

Analysis of ancient samples obtained using ancient DNA (aDNA), in addition to being plagued by potential ROHs due to inbreeding, often have presence of post-mortem molecular damage and can often have low coverage due to a paucity of endogenous material.

ROHan aims at:
1) providing an estimate of local rates of heterozygosity that is robust to low coverage and post-mortem damage. 
2) identifying ROHs using the results from 1). This is done via an hidden markov model whose parameters are optimized using a Markov Chain Monte Carlo
3) using both 1) and 2), provide a global rate of heterozygosity


Downloading:
----------------------

Go to https://github.com/grenaud/rohan and either:

1) Download the ZIP 

or

2) Do a "git clone --depth 1 https://github.com/grenaud/rohan.git"

Installation
----------------------

1) make sure you have "cmake", "libtool", "libpng" and "git" installed, check for it by typing " git --version" and "cmake --version".  

For Ubuntu:

     sudo apt-get install git
     sudo apt-get install cmake
     sudo apt-get install libtool
     sudo apt-get install libpng-dev

For MacOS, if you have Homebrew (https://brew.sh/) installed: 

     brew install git
     brew install cmake
     brew install libtool
     brew install libpng

2) make sure you have gcc that supports -std=c++11, gcc version 4.7 or above. Type "gcc -v" and check the version. For both Ubuntu and MacOS, 

3) As the makefile uses "git clone" to download subpackages, please make sure that the computer on which you are installing ROHan has access to the internet. Once done, simply type :
     cd ROHan
     make

For MacOS, if you get the problem: fatal error: 'lzma.h' file not found, this is a problem building htslib with homebrew, please refer to the following htslib page: https://github.com/samtools/htslib/issues/493


4) (optional) Either put the executable in the overall path or add the path to your $PATH environment or add an alias to be able to run "rohan" from any directory.


Quick start
----------------------


Preparing the BAM file
-----------------

1) Make sure you know the transition/transversion ratio for the species/population you are working with. 

rohan --tstv [TSTV ratio] 

2) Do not apply any filter as mapping quality and base quality are all informative. Duplicate removal is very recommended. Simply use the fail QC flag to remove reads/fragments that have failed basic quality control (e.g. for duplicates). Simply sort and index and provide ROHan the same reference used for mapping. 



Documentation
-----------------



FAQ
----------------------



### Why do I have such huge confidence intervals for potential ROH regions?

This is normal, as we use the second derivative for the confidence intervals, the shape of the likelihood function is somewhat flat around 0 which leads to overestimated confidence intervals. 



### I have regions that have clear signs of being runs of homozygosity however they do not get flagged as such, why?

It is possible that you have genuine ROH but if you have a slight overestimate of the rate of local heterozygosity do to him properly calibrated weapon qualities or  base qualities. Therefore such regions will not be flagged as being ROHs. It is advisable to rerun with --rohmu XXX where XXX is the "heterozygosity rate" in ROH regions you are willing to tolerate

### What are the 3 different lines on the HMM plot?

-Green is output using the point estimates. Margenta are the estimates using upper bounds for local het. rates whereas red were computed using lower bounds of het. rates.




