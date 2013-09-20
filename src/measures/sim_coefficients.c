/*
 * Harry - A Tool for Measuring String Similarity
 * Copyright (C) 2013 Konrad Rieck (konrad@mlsec.org)
 * --
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.  This program is distributed without any
 * warranty. See the GNU General Public License for more details. 
 */
#include "config.h"
#include "common.h"
#include "harry.h"
#include "util.h"
#include "uthash.h"

#include "sim_coefficients.h"

/**
 * @addtogroup measures
 * <hr>
 * <em>sim_jaccard</em>: Jaccard coefficient
 * @{
 */

typedef struct {
    sym_t sym; 		/**< Symbol or character */
    float cnt; 		/**< Count of symbol */
    UT_hash_handle hh;  /**< uthash handle */
} bag_t;

/* External variables */
extern config_t cfg;

/**
 * Computes a histogram of symbols or characters
 * @param x string
 * @return histogram
 */
static bag_t *bag_create(hstring_t x)
{
    bag_t *xh = NULL, *bag = NULL;

    for (int i = 0; i < x.len; i++) {
        sym_t s = hstring_get(x, i);
        HASH_FIND(hh, xh, &s, sizeof(sym_t), bag);
        
        if (!bag) {
            bag = malloc(sizeof(bag_t));
            bag->sym = s;
            bag->cnt = 0;
            HASH_ADD(hh, xh, sym, sizeof(sym_t), bag);
        }
        
        bag->cnt++;
    }
    
    return xh;
} 

/** 
 * Free the memory of histogram
 * @param xh Histogram
 */
static void bag_destroy(bag_t *xh)
{
    /* Clear hash table */
    while (xh) {
        bag_t *bag = xh;
        HASH_DEL(xh, bag);
        free(bag);
    }
}

/**
 * Computes the matches and mismatches
 * @param x first string 
 * @param y second string
 * @return matches
 */
match_t match(hstring_t x, hstring_t y)
{
    bag_t *xh, *yh, *xb, *yb;
    match_t match;
    
    xh = bag_create(x);
    yh = bag_create(y);
    
    int missing = y.len;
    for(xb = xh; xb != NULL; xb = xb->hh.next) {
        HASH_FIND(hh, yh, &(xb->sym), sizeof(sym_t), yb);
        if (!yb) {
            match.b += xb->cnt;
        } else {
            match.a = fabs(xb->cnt - yb->cnt);
            missing -= yb->cnt;
        }
    }
    match.c += missing;
    
    bag_destroy(xh);
    bag_destroy(yh);
    
    return match;
}

/**
 * Computes the Jaccard coefficient 
 * @param x String x
 * @param y String y
 * @return coefficient
 */
float sim_jaccard(hstring_t x, hstring_t y)
{
    match_t m = match(x, y);
    return m.a / (m.a + m.b + m.c);
}

/**
 * Computes the Simpson coefficient 
 * @param x String x
 * @param y String y
 * @return coefficient
 */
float sim_simpson(hstring_t x, hstring_t y)
{
    match_t m = match(x, y);
    return m.a / fmin(m.a + m.b, m.a + m.c);
}

/**
 * Computes the Braun-Blanquet coefficient 
 * @param x String x
 * @param y String y
 * @return coefficient
 */
float sim_braunblanquet(hstring_t x, hstring_t y)
{
    match_t m = match(x, y);
    return m.a / fmax(m.a + m.b, m.a + m.c);
}

/**
 * Computes the Czekanowski efficient 
 * @param x String x
 * @param y String y
 * @return coefficient
 */
float sim_czekanowski(hstring_t x, hstring_t y)
{
    match_t m = match(x, y);
    return 2 * m.a / (2 * m.a + m.b + m.c);
}

/**
 * Computes the Sokal-Sneath efficient 
 * @param x String x
 * @param y String y
 * @return coefficient
 */
float sim_sokalsneath(hstring_t x, hstring_t y)
{
    match_t m = match(x, y);
    return m.a / (m.a + 2 * (m.b + m.c));
}

/**
 * Computes the Kulczynski (1st) efficient 
 * @param x String x
 * @param y String y
 * @return coefficient
 */
float sim_kulczynski1(hstring_t x, hstring_t y)
{
    match_t m = match(x, y);
    return m.a / (m.b + m.c);
}

/**
 * Computes the Kulczynski (2nd) efficient 
 * @param x String x
 * @param y String y
 * @return coefficient
 */
float sim_kulczynski2(hstring_t x, hstring_t y)
{
    match_t m = match(x, y);
    return 0.5 * (m.a / (m.a + m.b) + m.a / (m.a + m.c));
}

/**
 * Computes the Otsuka efficient 
 * @param x String x
 * @param y String y
 * @return coefficient
 */
float sim_otsuka(hstring_t x, hstring_t y)
{
    match_t m = match(x, y);
    return m.a / sqrt((m.a + m.b) * (m.a + m.c));
}

/** @} */
