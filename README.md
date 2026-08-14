# Project for *Influence Minimization: Approximation Hardness and Efficient Algorithms at Billion Scale*

   This project implements the <code>ReversedFlow</code> and <code>ReversedFlow+</code> algorithms.

   We also provide the baselines (<code>Random</code>, <code>Degree</code>, <code>PageRank</code>; <code>AdvancedGreedy</code> and <code>SandIMIN</code>) for comparison.

## Files

- <code>README.md</code> (this file)
- <code>ReversedFlow+.cpp</code>: implementations of the <code>ReversedFlow</code> and <code>ReversedFlow+</code> algorithms
- <code>utils.h</code>: implementation of some auxiliary code
- <code>/datasets</code>: dataset for <code>ReversedFlow+.cpp</code>
- <code>/Baselines</code>: baseline algorithms
    - <code>0-Trivial_Baselines.cpp</code>: implementation of <code>Random</code>, <code>Degree</code> and <code>PageRank</code> algorithms for IMin
    - <code>1-AdvancedGreedy.cpp</code>: implementation of <code>AdvancedGreedy</code> algorithm for IMin
    - <code>/2-Sandwich</code>: codes of <code>SandIMIN</code> algorithm for IMin

## Dataset
   
Due to the file size limit for uploads, we have provided 8 datasets in total. Among them, gg, sf, tw, and yt are compressed and need to be unzipped into .txt format. The remaining datasets in this project can be downloaded from the [SNAP website](https://snap.stanford.edu/).

We provide the <code>EmailEuropeCore</code> (<code>eec.txt</code>) as an example.

The format of the dataset file is:

$\quad n\quad m$

$\quad u_1\quad v_1$

$\quad \dots $

$\quad  u_m\quad v_m$


---

## ReversedFlow and ReversedFlow+

We provide the implementation of the <code>RF</code>/<code>RF+</code> algorithms with fixed $|S|=10$ and you can adjust the seed set in the cpp file.

### Compile 

Compile the <code>ReversedFlow+.cpp</code> with O3 optimization.

e.g., <code>g++ -O3 -o rf.out ReversedFlow_changeBlockers.cpp</code>.


### Run 

1. Run <code>./rf.out</code>.
2. You will input some necessary parameters including the algorithm you choose, the budget $b$, the edge activation probability model and parameters in <code>RF+</code>.

---

## Trivial Baselines and AdvancedGreedy

We implement the trivial baseline algorithms and the <code>AdvancedGreedy</code> algorithms in <code>0-Trivial_Baselines.cpp</code> and <code>1-AdvancedGreedy.cpp</code>, respectively.

The compiling and running of the codes are similar with the <code>ReversedFlow+.cpp</code>, and you also need to input some necessary parameters to choose the algorithm and parameters.

---

## Sandwich and Sandwich-

This part is from the following paper:

<li>Jinghao Wang, Yanping Wu, Xiaoyang Wang, Ying Zhang, Lu Qin, Wenjie Zhang, Xuemin Lin, "Efficient Influence Minimization via Node Blocking", 2024.</li>

### Dataset

- There are six txt files in the <code>datasets/EmailCore</code> folder: 
(i) <code>attribute.txt</code>; (ii) <code>graph.txt</code>; (iii) <code>graph-rangEdge.txt</code>; (iv) <code>rumorSet_1.txt</code>; (v) <code>rumorSet_10.txt</code>; (vi) <code>rumorSet_100.txt</code>.

    - <code>attribute.txt</code> includes the $n$ and $m$.
    - <code>graph.txt</code> and <code>graph-rangEdge.txt</code> is the dataset of the network with each line recording $u\ v\ p_{u,v}$.
    - <code>rumorSet_x.txt</code> includes the seed set with size $x$.


### Compile 

<code>g++ -O3 -o IMIN.out Sandwich.cpp sfmt/SFMT.c</code>





### Run 

<ul> 
    <li>Execute the Program</li>
    <code>./IMIN.out -dataset datasets/EmailCore -k 10 -rumorNum 1 -algo SandIMIN -epsilon 0.2 -gamma 0.1 -beta 0.1</code>
    <li>Arguments</li>
    <ul>
    <li>dataset: path to the dataset directory</li>
    <li>epsilon, gamma, beta: the parameter</li>
    <li>rumorNum: number of seed nodes of misinformation</li>
    <li>algo: algorithm (SandIMIN/SandIMIN-)</li>
    </ul>
</ul>





