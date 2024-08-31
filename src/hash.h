/**
 * @file hash.h
 *
 * Hash table's header.
 *
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
 * @date 1998 - 2023
=======
 * @date 1998 - 2017
>>>>>>> b3f048d (copyright changes)
=======
 * @date 1998 - 2020
>>>>>>> a556e46 (HashData and HashStoreData rearranged, TYPE_PUNING now uses union)
=======
 * @date 1998 - 2021
>>>>>>> 34a2291 (4.5.0: Use CRC32c for board hash)
 * @author Richard Delorme
 * @version 4.5
 */

#ifndef EDAX_HASH_H
#define EDAX_HASH_H

#include "board.h"
#include "settings.h"
#include "util.h"
#include "stats.h"

#include <stdbool.h>
#include <stdio.h>

/** HashData : data stored in the hash table */
typedef struct HashData {
	union {
#ifdef __BIG_ENDIAN__
		struct {
			unsigned char date;       /*!< dating technique */
			unsigned char cost;       /*!< search cost */
			unsigned char selectivity;/*!< selectivity */
			unsigned char depth;      /*!< depth */
		} c;
		struct {
			unsigned short date_cost;
			unsigned short selectivity_depth;
		} us;
#else
		struct {
			unsigned char depth;      /*!< depth */
			unsigned char selectivity;/*!< selectivity */
			unsigned char cost;       /*!< search cost */
			unsigned char date;       /*!< dating technique */
		} c;
		struct {
			unsigned short selectivity_depth;
			unsigned short date_cost;
		} us;
#endif
		unsigned int	ui;      /*!< as writable level */
	} wl;
	signed char lower;        /*!< lower bound of the position score */
	signed char upper;        /*!< upper bound of the position score */
	unsigned char move[2];    /*!< best moves */
} HashData;

/** Hash  : item stored in the hash table */
typedef struct Hash {
	HASH_COLLISIONS(unsigned long long key;)
	Board board;
	HashData data;
} Hash;

/** HashLock : lock for table entries */
typedef struct HashLock {
	SpinLock spin;
} HashLock;

/** HashTable: position storage */
typedef struct HashTable {
	void *memory;                 /*!< allocated memory */
	Hash *hash;                   /*!< hash table */
	HashLock *lock;               /*!< table with locks */
	unsigned long long hash_mask; /*!< a bit mask for hash entries */
	unsigned int lock_mask;       /*!< a bit mask for lock entries */
	int n_hash;                   /*!< hash table size */
	int n_lock;                   /*!< number of locks */
	unsigned char date;           /*!< date */
} HashTable;

/** HashStoreData : data to store */
typedef struct HashStoreData {
	HashData data;
	HASH_COLLISIONS(unsigned long long hash_code;)
	int alpha;
	int beta;
	int score;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	unsigned char move;
>>>>>>> d1c50ef (Structured hash_store parameters; AVXLASTFLIP changed to opt-in)
=======
>>>>>>> a556e46 (HashData and HashStoreData rearranged, TYPE_PUNING now uses union)
} HashStoreData;

/* declaration */
<<<<<<< HEAD

=======
>>>>>>> 34a2291 (4.5.0: Use CRC32c for board hash)
void hash_move_init(void);
void hash_init(HashTable*, const unsigned long long);
void hash_cleanup(HashTable*);
void hash_clear(HashTable*);
void hash_free(HashTable*);
<<<<<<< HEAD
void hash_feed(HashTable*, const Board *, const unsigned long long, HashStoreData *);
void hash_store(HashTable*, const Board *, const unsigned long long, HashStoreData *);
void hash_force(HashTable*, const Board *, const unsigned long long, HashStoreData *);
bool hash_get(HashTable*, const Board *, const unsigned long long, HashData *);
bool hash_get_from_board(HashTable*, const Board *, HashData *);
void hash_exclude_move(HashTable*, const Board *, const unsigned long long, const int);
=======
void hash_feed(HashTable*, const Board*, const unsigned long long, HashStoreData *);
void hash_store(HashTable*, const Board*, const unsigned long long, HashStoreData *);
void hash_force(HashTable*, const Board*, const unsigned long long, HashStoreData *);
bool hash_get_from_board(HashTable*, const Board*, HashData *);
void hash_exclude_move(HashTable*, const Board*, const unsigned long long, const int);
>>>>>>> d1c50ef (Structured hash_store parameters; AVXLASTFLIP changed to opt-in)
void hash_copy(const HashTable*, HashTable*);
void hash_print(const HashData*, FILE*);
extern unsigned int writeable_level(HashData *data);

#if defined(hasSSE2) && (defined(_MSC_VER) || defined(__linux__))
	bool vectorcall hash_get_sse(HashTable*, __m128i, const unsigned long long, HashData *);
	#define	hash_get(hash_table,board,hash_code,data)	hash_get_sse((hash_table), _mm_loadu_si128((__m128i *) (board)), (hash_code), (data))
	#define	vhash_get(hash_table,vboard,hash_code,data)	hash_get_sse((hash_table), (vboard).v2, (hash_code), (data))
#else
	bool hash_get(HashTable*, const Board*, const unsigned long long, HashData *);
	#define	vhash_get(hash_table,vboard,hash_code,data)	hash_get((hash_table), &(vboard).board, (hash_code), (data))
#endif

extern const HashData HASH_DATA_INIT;
<<<<<<< HEAD

inline void hash_prefetch(HashTable *hashtable, unsigned long long hashcode) {
	Hash *p = hashtable->hash + (hashcode & hashtable->hash_mask);
  #ifdef hasSSE2
	_mm_prefetch((char const *) p, _MM_HINT_T0);
	_mm_prefetch((char const *)(p + HASH_N_WAY - 1), _MM_HINT_T0);
  #elif defined(__ARM_ACLE)
	__pld(p);
	__pld(p + HASH_N_WAY - 1);
  #elif defined(__GNUC__)
	__builtin_prefetch(p);
	__builtin_prefetch(p + HASH_N_WAY - 1);
  #endif
}
=======
>>>>>>> ff1c5db (skip hash access if n_moves <= 1 in NWS_endgame)

<<<<<<< HEAD
#ifdef hasSSE2
	#define	hash_prefetch(hashtable, hashcode)	_mm_prefetch((char const*)((hashtable)->hash + ((hashcode) & (hashtable)->hash_mask)), _MM_HINT_T0)
#elif defined(__ARM_ACLE)
	#define	hash_prefetch(hashtable, hashcode)	__pld((hashtable)->hash + ((hashcode) & (hashtable)->hash_mask))
#elif defined(__GNUC__)
	#define	hash_prefetch(hashtable, hashcode)	__builtin_prefetch((hashtable)->hash + ((hashcode) & (hashtable)->hash_mask))
#else
	#define	hash_prefetch(hashtable, hashcode)
#endif
<<<<<<< HEAD
=======
=======
inline void hash_prefetch(HashTable *hashtable, unsigned long long hashcode) {
	Hash *p = hashtable->hash + (hashcode & hashtable->hash_mask);
  #ifdef hasSSE2
	_mm_prefetch((char const *) p, _MM_HINT_T0);
	_mm_prefetch((char const *)(p + HASH_N_WAY - 1), _MM_HINT_T0);
  #elif defined(__ARM_ACLE)
	__pld(p);
	__pld(p + HASH_N_WAY - 1);
  #elif defined(__GNUC__)
	__builtin_prefetch(p);
	__builtin_prefetch(p + HASH_N_WAY - 1);
  #endif
}
>>>>>>> 30464b5 (add hash_prefetch to NWS_endgame)

#endif
>>>>>>> dd57cbd (add hash_prefetch; revise AVX flip & full_lines)
