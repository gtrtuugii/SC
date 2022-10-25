
<!--
##########################
## DO NOT SUBMIT THIS FILE
##########################
-->

\mainpage curdle score-tracking

This project contains a C file with functions for safely updating
a scores file for the [Curdle](https://gumbo.systems/curdle/)
game, plus tests for the code using the [Check][libcheck]
unit-testing framework.

[libcheck]: https://libcheck.github.io/check/index.html

The primary function for updating scores
is \ref adjust_score, contained in the \ref adjust_score.c file.
At runtime, Curdle stores scores internally in
a \ref score_record struct. The definition for this struct,
plus some macro definitions useful for development,
are contained in the \ref curdle.h file.




