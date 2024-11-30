# ANF-CDCL-ICTAI


## Presentation

The goal of this solver is to solve instances in Algebraic Normal Form (or ANF format) while integrating techniques that made the succes of modern SAT solvers.

The code of this solver is based on the code of [Glucose](https://github.com/audemard/glucose/), which is also based on the code of [Minisat 2.2](http://minisat.se/MiniSat.html).


## How to use it

To compile the solver, you just have to use the ```make``` command inside the ```simp``` repository (you should get some warnings but this should not hurt).

Once ```glucose``` is compiled, you need an instance in ANF format:

- lines beginning by ```c``` are considered as comments and ignored

- the prefix of the instance is of the form ```p cnf var eq``` with ```var``` the number of variables and ```eq``` the number of equations in the instance.

- each equation begins with ```x``` and ends with ```0```

- a unit monomial is represented by the variable it contains. Greater monomials are preceded by ```.d``` with ```d``` the number of variables in the monomial.

- an always satisfied monomial is represented by the constant ```T```

For example, the formula:

$x_{1} x_{2} \oplus x_{2} x_{3} \oplus x_{4} \oplus \top$

$x_{2} x_{3} \oplus  x_{2} x_{4} \oplus x_{3} x_{4} \oplus x_{2}$

is written as:

```
p cnf 4 2
x .2 1 2 .2 2 3 4 T 0
x .2 2 3 .2 2 4 .2 3 4 2 0
```


## Available options

In this solver, the following options are available:

- checkModel: check if the model found is correct

- useWatchedLiterals: enable watched literals

- useWatchedMonomials: enable watched monomials

- breakSymetries: allow to break symetries during the search. If used, we have to decide variables in the lexicographical order. Works only on Index Calculus Attacks on Elliptic Curves instances

- l: value to use when breaking symetries
