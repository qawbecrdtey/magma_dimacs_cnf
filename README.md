# DIMACS CNF generator for Magmas

There will be updates for improvements or new experiments.

## How to use the generator

To obtain the `.cnf` file in most linux environment,

```sh
$ g++ --std=c++14 path/to/code.cpp -o generator
$ ./generator N > path/to/dimacs.cnf
```

Replace `N` with any number not smaller than 5.

Using the file `path/to/dimacs.cnf`, pass it to any solver like [CaDiCaL](https://github.com/arminbiere/cadical)
or [Kissat](https://github.com/arminbiere/kissat).

```sh
$ ./cadical path/to/dimacs.cnf
```

## Contribute

If you'd like to suggest a new experiment or give improvements in code, please make a github issue, or make a pull request from the forked repo.

## Files

### [`255`](./255)

#### [a255.hpp](255/a255.hpp)

Defines a function `a255`, which generates clauses to find a magma which all variables satisfy [equation 255](https://teorth.github.io/equational_theories/implications/?255&finite).

#### [e255.hpp](255/e255.hpp)

Defines a function `e255`, which generates clauses to find a magma with at least one variable which satisfies [equation 255](https://teorth.github.io/equational_theories/implications/?255&finite).

#### [na255.hpp](./255/na255.hpp)

Defines a function `na255`, which generates clauses to find a magma with at least one variable which doesn't satisfy [equation 255](https://teorth.github.io/equational_theories/implications/?255&finite).

#### [ne255.hpp](255/ne255.hpp)

Defines a function `ne255`, which generates clauses to find a magma which no variables satisfy [equation 255](https://teorth.github.io/equational_theories/implications/?255&finite).

### [`677`](./677)

#### [a677.hpp](677/a677.hpp)

Defines a function `a677`, which generates clauses to find a magma which all variables satisfy [equation 677](https://teorth.github.io/equational_theories/implications/?677&finite).

#### [search.cpp](./677/search.cpp)

Generates a `.cnf` file which can be satisfied if and only if there is some magma with `N` elements which satisfies [equation 677](https://teorth.github.io/equational_theories/implications/?677&finite).

#### [search_a2504.cpp](677/search_a2504.cpp)

Generates a `.cnf` file which can be satisfied if and only if there is some magma with `N` elements which both [equation 677](https://teorth.github.io/equational_theories/implications/?677&finite) and [equation 2504](https://teorth.github.io/equational_theories/implications/?2504&finite) are satisfied.

#### [search_na255.cpp](677/search_na255.cpp)

Generates a `.cnf` file which can be satisfied if and only if there is some magma with `N` elements which satisfies  [equation 677](https://teorth.github.io/equational_theories/implications/?677&finite) but not [equation 255](https://teorth.github.io/equational_theories/implications/?255&finite).

### [`2504`](2504)

#### [a2504.cpp](2504/a2504.hpp)

Defines a function `a2504` which generates clauses to find a magma which all variables satisfy [equation 2504](https://teorth.github.io/equational_theories/implications/?2504&finite).
