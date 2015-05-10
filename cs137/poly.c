//
//  poly.c
//  CS 137 - Assignment 9
//
//  Created by Dweep Shah on 11/21/2013.
//  Copyright (c) 2013 Dweep Shah. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "poly.h"

struct poly *polyCreate() {
    struct poly *p = calloc(1, sizeof(struct poly));
    p->coeff = calloc(1, sizeof(double));
    p->length = 1;
    return p;
}

struct poly *polyDelete(struct poly *p) {
    if (p) {
        free(p->coeff);
        free(p);
    }
    return (struct poly *)0;
}

struct poly *polySetCoefficient(struct poly *p, int i, double value) {
    if (p == (struct poly *)0)
        return (struct poly *) 0;
    else if (i < 0)
        return  p;
    else if (i >= p->length) {
        if (fabs(value) < 0.000001)
            return p;
        p->coeff = realloc(p->coeff, (i+1)*sizeof(double));
        for (int j = p->length; j < i+1; j++)
            p->coeff[j] = 0;
        p->length = i+1;
    } else if (i == p->length-1 && fabs(value) < 0.000001) {
        p->coeff = realloc(p->coeff, (p->length-1)*sizeof(double));
        p->length--;
        return p;
    }
        
    p->coeff[i] = value;
    return p;
}

double polyGetCoefficient (struct poly *p, int i) {
    if (p == (struct poly *)0 || i < 0 || i >= p->length)
        return 0;
    return p->coeff[i];
}

int polyDegree(struct poly *p) {
    if (p == (struct poly *)0)
        return 0;
    return p->length - 1;
}

void polyPrint (struct poly *p) {
    int check = 0;
    double epsilon = 0.000001;

    for (int i = p->length-1; i >= 0; i--) {
        if(fabs(p->coeff[i]) > epsilon) {
            if (check == 0 && p->coeff[i] < 0.0) printf("-");
            if (i == 0) {
                if (p->coeff[i] > 0.0 && check == 1) printf(" + ");
                else if (p->coeff[i] < 0.0 && check == 1) printf(" - ");
                printf("%g", fabs(p->coeff[i]));
            }
            else if (i == 1) {
                if (p->coeff[i] > 0.0 && check == 1) printf(" + ");
                else if (p->coeff[i] < 0.0 && check == 1) printf(" - ");
                if (!(fabs(p->coeff[i]) < epsilon + 1 && fabs(p->coeff[i]) > 1 - epsilon)) printf("%gx", fabs(p->coeff[i]));
                else printf("x");
            } else {
                if (p->coeff[i] > 0.0 && check == 1) printf(" + ");
                else if (p->coeff[i] < 0.0 && check == 1) printf(" - ");
                if (!(fabs(p->coeff[i]) < epsilon + 1 && fabs(p->coeff[i]) > 1 - epsilon)) printf("%gx^%d", fabs(p->coeff[i]), i);
                else printf("x^%d", i);
            }
            check = 1;
        }
    }
    
    if (check == 0) printf("0");
    printf("\n");
}

struct poly *polyCopy(struct poly *p){
    if (p == (struct poly *)0)
        return (struct poly *) 0;
    
    struct poly *s = polyCreate();
    s->coeff = realloc(s->coeff, p->length*sizeof(double));
    for (int i = 0; i < p->length; i++)
        s->coeff[i] = p->coeff[i];
    s->length = p->length;
    
    return s;
}

struct poly *polyAdd(struct poly *p0, struct poly *p1) {
    if (p0 == (struct poly *)0 && p1 == (struct poly *)0)
        return (struct poly *) 0;
    else if (p0 == (struct poly *)0)
        return p1;
    else if (p1 == (struct poly *)0)
        return p0;
    
    struct poly *s = polyCreate();
    int minlen = (p0->length <= p1->length) ? p0->length : p1->length;
    int maxlen = (p0->length >= p1->length) ? p0->length : p1->length;
    s->coeff = realloc(s->coeff, maxlen*sizeof(double));
    s->length = maxlen;
    
    for (int i = 0; i < minlen; i++)
        s->coeff[i] = p0->coeff[i] + p1->coeff[i];
    if (minlen != maxlen) {
        if (p0->length >= p1->length)
            for (int j = minlen; j < maxlen; j++)
                s->coeff[j] = p0->coeff[j];
        else
            for (int j = minlen; j < maxlen; j++)
                s->coeff[j] = p1->coeff[j];
    }
    
    // Reduces the length of the array
    int index = 0;
    for (int k = s->length-1; k >= 0; k--)
        if (fabs(s->coeff[k]) > 0.000001) {
            index = k;
            break;
        }

    if (index != s->length-1) {
        s->coeff = realloc(s->coeff, (index+1)*sizeof(double));
        s->length = index + 1;
    }
    
    return s;
}

struct poly *polyMultiply(struct poly *p0, struct poly *p1) {
    if (p0 == (struct poly *)0 || p1 == (struct poly *)0)
        return (struct poly *) 0;
    
    struct poly *s = polyCreate();
    s->coeff = realloc(s->coeff, (polyDegree(p0)+polyDegree(p1)+1)*sizeof(double));
    s->length = polyDegree(p0)+polyDegree(p1)+1;
    
    for (int i = 0; i < p0->length; i++) 
        for (int j = 0; j < p1->length; j++)
            s->coeff[i+j] += p0->coeff[i]*p1->coeff[j];
    
    // Reduces the length of the array
    int index = 0;
    for (int k = s->length-1; k >= 0; k--)
        if (fabs(s->coeff[k]) > 0.000001) {
            index = k;
            break;
        }
    
    if (index != s->length-1) {
        s->coeff = realloc(s->coeff, (index+1)*sizeof(double));
        s->length = index + 1;
    }
    
    return s;
}

struct poly *polyPrime(struct poly *p){
    if(p == (struct poly *)0)
        return (struct poly *) 0;
    else if (p->length == 1) {
        struct poly *s = polyCreate();
        return s;
    }
    struct poly *s = polyCreate();
    
    for (int i = 0; i < p->length-1; i++)
        s->coeff[i] = p->coeff[i+1]*(i+1);
    s->length = p->length-1;
    
    return s;
}

double polyEval(struct poly *p, double x){
    if(p == (struct poly *)0)
        return 0.0;
    double value = 0.0;
    
    for (int i = 0; i < p->length; i++)
        value += p->coeff[i]*pow(x,i);
    return value;
}
