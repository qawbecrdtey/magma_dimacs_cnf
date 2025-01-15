# DIMACS CNF generator for Magmas

Unlike the title, this repository currently contains two files `677/search.cpp` and `README.md`.

There will be updates for improvements or new experiments.

## How to use the generator

To obtain the `.cnf` file in most linux environment,

```sh
$ g++ --std=c++14 path/to/code.cpp -o generator
$ ./generator N > path/to/dimacs.cnf
```

Replace `N` with any number not smaller than 5.

Using the file `path/to/dimacs.cnf`, pass it to any solver like [CaDiCaL](https://github.com/arminbiere/cadical) or [Kissat](https://github.com/arminbiere/kissat).

```sh
$ ./cadical path/to/dimacs.cnf
```

## Contribute

If you'd like to suggest a new experiment or give improvements in code, please make a github issue, or make a pull request from the forked repo.

## Files

### [`677`](./677)

#### [search.cpp](./677/search.cpp)

* Generates a `.cnf` file which can be satisfied if and only if there is some magma with `N` elements which satisfies [equation 677](https://teorth.github.io/equational_theories/implications/?677&finite).
