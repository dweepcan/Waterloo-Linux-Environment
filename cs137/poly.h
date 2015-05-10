//
//  poly.h
//  CS 137 - Assignment 9
//
//  Created by Dweep Shah on 11/21/2013.
//  Copyright (c) 2013 Dweep Shah. All rights reserved.

struct poly {
    double *coeff;
    int length;
};

struct poly *polyCreate();
struct poly *polyDelete(struct poly *p);
struct poly *polySetCoefficient (struct poly *p, int i, double value);
double polyGetCoefficient (struct poly *p, int i);
int polyDegree (struct poly *p);
void polyPrint (struct poly *p);
struct poly *polyCopy (struct poly *p);
struct poly *polyAdd (struct poly *p0, struct poly *p1);
struct poly *polyMultiply (struct poly *p0, struct poly *p1);
struct poly *polyPrime (struct poly *p);
double polyEval(struct poly *p, double x);