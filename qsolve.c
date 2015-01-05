/*
 * qsolve.c:  direct approach at solving some polynomial equations
 * See `psolve.c' for the slow approach, needed for equations too complex.
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

/*
 * Since the Renaissance, it was proven impossible to direct solve
 * beyond degree MAX_POLYNOMIAL_DEPTH -- understood to be quartic complexity.
 *     degree 0:  f(x) = a
 *     degree 1:  f(x) = ax + b
 *     degree 2:  f(x) = ax^2 + bx + c
 *     degree 3:  f(x) = ax^3 + bx^2 + cx + d
 *     degree 4:  f(x) = ax^4 + bx^3 + cx^2 + dx + e
 */
#define MAX_POLYNOMIAL_DEPTH    4

typedef struct {
    double a; /* real component */
    double b; /* imaginary component */
} complex;
typedef complex* vector;

static vector linear_inverse(
    double a, double b);
static vector quadratic_inverse(
    double a, double b, double c);
static vector cubic_inverse(
    double a, double b, double c, double d);
static vector quartic_inverse(
    double a, double b, double c, double d, double e);

int main(int argc, char ** argv)
{
    double a[MAX_POLYNOMIAL_DEPTH + 1];
    vector result;
    int degree;
    register unsigned int i;

    degree = argc - 1 - 1; /* excluding argv[0] and the constant term */
    if (degree < 1)
    { /* to-do:  use scanf or something to ask the user at run-time? */
        fputs("Nothing to solve for from a constant monomial.\n", stderr);
        return 1;
    }
    if (degree > MAX_POLYNOMIAL_DEPTH)
    {
        degree = MAX_POLYNOMIAL_DEPTH;
        fputs("Warning:  Polynomial too complex.  Truncating.\n", stderr);
    }

    for (i = 0; i < MAX_POLYNOMIAL_DEPTH + 1; i++)
        a[i] = 0.;
    for (i = 0; i < degree + 1; i++)
        a[i] = strtod(argv[degree + 1 - i], NULL);
    printf("Evaluating all solutions for x when P(x) = 0...\n");

    printf(
        "P(x) = %gx^4 + %gx^3 + %gx^2 + %gx + %g\n",
        a[MAX_POLYNOMIAL_DEPTH - 0],
        a[MAX_POLYNOMIAL_DEPTH - 1],
        a[MAX_POLYNOMIAL_DEPTH - 2],
        a[MAX_POLYNOMIAL_DEPTH - 3],
        a[MAX_POLYNOMIAL_DEPTH - 4]
    );
    result = quartic_inverse(a[4], a[3], a[2], a[1], a[0]);
    for (i = 0; i < degree; i++)
        printf("x%u = %g + %gi\n", i, result[i].a, result[i].b);
    return 0;
}

static vector constant_inverse(double a)
{
    a = a / 1.;

    return NULL;
}

static vector linear_inverse(double a, double b)
{
    vector x;

    if (a == 0.)
        return constant_inverse(b);
    x = malloc(1 * sizeof(complex));

    x[0].a = -b / a;
    x[0].b = 0.;
    return (x);
}

static vector quadratic_inverse(double a, double b, double c)
{
    vector x;
    double discriminant;

    if (a == 0.)
        return linear_inverse(b, c);
    x = malloc(2 * sizeof(complex));

#if 0
    a /= a;
    b /= a;
    c /= a;
#endif
    discriminant = b*b - 4*a*c;
    if (discriminant < 0.)
    {
        x[0].a = x[1].a = -b / (2 * a);

        x[0].b = -sqrt(-discriminant) / (2 * a);
        x[1].b = +sqrt(-discriminant) / (2 * a);
    }
    else
    {
        x[0].a = -b/(2.*a) - sqrt(+discriminant)/(2.*a);
        x[0].b = 0.;
        x[1].a = -b/(2.*a) + sqrt(+discriminant)/(2.*a);
        x[1].b = 0.;
    }
    return (x);
}

static vector cubic_inverse(double a, double b, double c, double d)
{
    vector x;

    if (a == 0.)
        return quadratic_inverse(b, c, d);
    x = malloc(3 * sizeof(complex));

/*
 * Since ax^3 + bx^2 + cx + d = 0, and a != 0, we know that:
 * ax^3/a + bx^2/a + cx/a + d/a = 0/a, so we can permanently divide by a.
 *
 * Eliminating the use of `a` from the solve algorithm is not so much an
 * attempt at [premature] optimization, so much as it is to keep the size of
 * the algorithm smaller, as there is no official, standard cubic formula.
 * The cubic formula, in plain sight, is pages long.  Any attempt at showing
 * the formula in any programming language will either be the same way or use
 * plenty of higher-level variables.
 */
    a /= a;
    b /= a;
    c /= a;
    d /= a;

/*
 * sadly I forget what the cubic formula is atm... :P
 * There were two or three algebraic/geometric methods of defining it I saw.
 */
    fputs("Not yet implemented:  Cubic function inverses.\n", stderr);
    return (x);
}

static vector quartic_inverse(double a, double b, double c, double d, double e)
{
    vector x;

    if (a == 0.)
        return cubic_inverse(b, c, d, e);
    x = malloc(4 * sizeof(complex));

 /* ??? */

    fputs("Not yet implemented:  Quartic function inverses.\n", stderr);
    return (x);
}
