/**
 * @file empty.h
 *
 * List of empty squares.
 *
 * @date 1998 - 2020
 * @author Richard Delorme
 * @version 4.4
 */

#ifndef EDAX_EMPTY_H
#define EDAX_EMPTY_H

/** double linked list of squares */
typedef struct SquareList {
	unsigned char previous;	/*!< link to previous square */
	unsigned char next;	/*!< link to next square */
} SquareList;

/**
 * @brief remove an empty square from the list.
 *
 * @param empty  empty square.
 */
static inline void empty_remove(SquareList *empty, int index)
{
	int	next = empty[index].next;
	int	prev = empty[index].previous;
	empty[prev].next = next;
	empty[next].previous = prev;
}

/**
 * @brief restore the list of empty squares
 *
 * @param empty  empty square.
 */
static inline void empty_restore(SquareList *empty, int index)
{
	empty[empty[index].previous].next = index;
	empty[empty[index].next].previous = index;
}

/** Loop over all empty squares */
<<<<<<< HEAD
<<<<<<< HEAD
#define foreach_empty(index, empty)\
	for ((index) = (empty)[NOMOVE].next; index != NOMOVE; (index) = (empty)[index].next)
=======
#define foreach_empty(empty, list)\
	for ((empty) = (list)->next; (empty)->next; (empty) = (empty)->next)
>>>>>>> 4b9f204 (minor optimize in search_eval_1/2 and search_shallow)
=======
#define foreach_empty(index, empty)\
	for ((index) = (empty)[NOMOVE].next; index != NOMOVE; (index) = (empty)[index].next)
>>>>>>> 5e86fd6 (Change pointer-linked empty list to index-linked)

#endif

