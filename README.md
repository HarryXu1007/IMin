# Influence Minimization

   This project implements the ReversedFlow and ReversedFlow+ algorithms.

## Dataset
   
All the datasets in this project can be downloaded in [SNAP](https://snap.stanford.edu/).

We provide the <code>EmailEuropeCore</code> as an example.

## Compile 

Compile the <code>ReversedFlow_changeBlockers.cpp</code> / <code>ReversedFlow_changeSeeds.cpp</code> / <code>ReversedFlow+_changeBlockers.cpp</code> / <code>ReversedFlow+_changeSeeds.cpp</code> with O3 optimization.

e.g., <code>g++ -O3 -o rf.out ReversedFlow_changeBlockers.cpp</code>.

## Format of Datasets
<ul>
<li>In the folder <code>/dataset</code>, <code>'file_name'.txt</code> is stored. The following is an example format.</li>

        1 2
        1 3
        2 3
        2 4

<li>
In the folder <code>/dataset-Seeds</code>, <code>/'file_name'</code> is stored, in which <code>rumorSet_'number'.txt</code> lists the seeds. The following is an example format.</li>

    0
    100
    200
    300
    400
    500
    600
    700
    800
    900
</ul>


## Run 

Run <code>./rf.out</code>.

## Setting the Parameters
<li> <code>RF/RF+_changeBlockers.cpp</code> need to initialize the seeds in the file and input the file name and budget.</li>
<li> <code>RF/RF+_changeSeeds.cpp</code> need to assign the seeds in <code>/dataset-Seed/'file_name'/rumorSet_'number'.txt</code> and input the file name and number of seeds.</li>


