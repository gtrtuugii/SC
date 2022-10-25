#ifndef CURDLE_H
#define CURDLE_H

///////////////////////////////////
// DO NOT SUBMIT OR AMEND THIS FILE
///////////////////////////////////

/** \file
 * \brief The \ref score_record struct definition, plus
 * some useful macro definitions.
 *
 * Contains the \ref score_record struct, plus macros
 * which allow the program to emit a useful error message
 * before aborting execution.
 *
 */


/**
 * \cond internal
 */
#define STRINGIFY(s) #s
/**
 * \endcond
 */

/**
 * This allows execution to be aborted if an error has occured in a C library or POSIX function.
 * 
 * It can be called like this:
 * 
 * ```C
 *     die_perror(__FILE__, __LINE__, "my_func: couldn't read the frobnicate input file");
 * ```
 *
 * Good practice is to include the name of the function calling `die_perror` in the error message.
 */
#define die_perror(file, line, mesg)  { perror(file ":" STRINGIFY(line) ": " mesg); exit(1); }
// perror prints the error associated with the *last* failing C library
// function. If you call any other C library function in between, the error message
// emitted may not be accurate. See `man perror` for details.

/**
 * This allows execution to be aborted if an error has occured in a C library or POSIX function,
 * and the caller has a saved `errno` value describing the error.
 * 
 * It can be called like this:
 * 
 * ```C
 *     die_errno(__FILE__, __LINE__, "my_func: couldn't read the frobnicate input file", saved_errno);
 * ```
 *
 * Good practice is to include the name of the function calling `die_errno` in the error message.
 */
#define die_errno(file, line, mesg, num)  { errno=num; perror(file ":" STRINGIFY(line) ": " mesg); exit(1); }

/**
 * This allows execution to be aborted using some custom error message, specified
 * by the caller.
 * 
 * It can be called like this:
 * 
 * ```C
 *     die(__FILE__, __LINE__, "my_func: couldn't read the frobnicate input file");
 * ```
 *
 * Good practice is to include the name of the function calling `die` in the error message.
 */
#define die(file, line, mesg)  { fprintf(stderr, file ":" STRINGIFY(line) ": " mesg "\n"); exit(1); }

/**
 * Size of a name or score field in a scores file line.
 *
 * This macro (together with \ref REC_SIZE_) may be used
 * in contexts (such as struct declarations) where a `const size_t`
 * is not permissible.
 *
 * For all other purposes, it's better to use the \ref FIELD_SIZE
 * `const size_t` value instead.
 * instead.
 */
#define FIELD_SIZE_   10

/**
 * Length of a full line in the scores file: it contains
 * two fields plus a terminating newline character.
 *
 * This macro (together with \ref FIELD_SIZE_) may be used
 * in contexts (such as struct declarations) where a `const size_t`
 * is not permissible.
 *
 * For all other purposes, it's better to use the \ref REC_SIZE
 * `const size_t` value instead.
 * instead.
 */
#define REC_SIZE_     (FIELD_SIZE_*2 + 1)

/** Stores the name and current score of a
  * *curdle* player.
  *
  * Initialize a `struct score_record` with \ref score_record_init().
  *
  * e.g.:
  *
  * ```C
  *   struct score_record myrec;
  *   score_record_init(&myrec, "carol", 72);
  * ```
  *
  * A `score_record` struct has fixed size and contains no
  * pointers. It may therefore be safely passed to and returned
  * from functions by value.
  */
struct score_record {
  /** The name of the player a score is being recorded for. */
  char name[FIELD_SIZE_];
  /** The current score for the player. */
  int  score;
};



#endif // CURDLE_H

