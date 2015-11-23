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

/*
 * All complex numbers can be written in the form:  (a + bi)
 * For example, sqrt(-9) = 0 + 3i.  (i is always sqrt(-1).)
 */
typedef struct {
    double a; /* real component */
    double b; /* imaginary component */
} complex;
typedef complex* vector;

extern double cbrt(double x);

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

/*
 * Just a dummy placeholder function for solving for x:
 * ax + b = 0, when a is 0.  (Doesn't really do anything.)
 */
static vector constant_inverse(double a)
{
    vector x;

    x = malloc(0 * sizeof(complex));
    return (x);
}

static vector linear_inverse(double a, double b)
{
    vector x;

    if (a == 0.)
        return constant_inverse(b);
    x = malloc(1 * sizeof(complex));

    x[0].a = -b / a; /* ax + b = 0, so x = -b/a. */
    x[0].b = 0.; /* Such a simple solution for x has no imaginary unit. */
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
/*
 * Optional:  Divide all coefficients by (a).
 * x^2 + bx + c = 0
 * x = -b/2 +/- sqrt(b^2 - 4c)/2
 * If (b*b - 4*c < 0), then x is a complex number:  (a +/- b*i).
 */
    c /= a;
    b /= a;
    a /= a;
#endif
    discriminant = pow(b, 2) - 4*a*c;
    if (discriminant < 0)
    {
        x[0].a
      = x[1].a = -b / (2 * a);

        x[0].b = -sqrt(-discriminant) / (2 * a);
        x[1].b = +sqrt(-discriminant) / (2 * a);
    }
    else
    {
        x[0].a = -b/(2*a) - sqrt(+discriminant)/(2*a);
        x[0].b = 0.;
        x[1].a = -b/(2*a) + sqrt(+discriminant)/(2*a);
        x[1].b = 0.;
    }
    return (x);
}

static vector cubic_inverse(double a, double b, double c, double d)
{
    vector x;
    double p, q, r;
    double Q, R;
    double discriminant;

    if (a == 0.)
        return quadratic_inverse(b, c, d);
    x = malloc(3 * sizeof(complex));

    p = b / a;
    q = c / a;
    r = d / a;

/*
 * Cardano's derivation of the cubic solutions in ca. 1545:
 * Rewrite "y^3 + py^2 + qy + r" as the reduced cubic "x^3 + ax + b = 0".
 */
    a = q - pow(p, 2)/3;
    b = r - p*q/3 + 2*pow(p, 3)/27;

    Q =  a / 3;
    R = -b / 2;
#if 0
    discriminant = (4*q*q*q - p*p*q*q + 4*p*p*p*r - 18*p*q*r + 27*r*r)/108.;
#elif 0
    discriminant = (4*a*a*a + 27*b*b) / (27. * 4.);
#else
    discriminant = pow(Q, 3) + pow(R, 2);
#endif

/*
 * If the discriminant is negative, then the cubic formula ultimately yields
 * the problem of adding cube roots of complex or imaginary numbers, even
 * though this also means that all solutions for x are real numbers.
 *     Example, if x^3 + 2x^2 - x - 2 = 0:
 *         x0 = cbrt(10/27 + sqrt(-1/3)) + cbrt(10/27 - sqrt(-1/3)) = 1
 * Every old book so far suggests to use a trigonometric alternative instead.
 */
    if (discriminant < 0)
    { /* three real and unequal roots */
        double cosine_theta, theta;
        const double pi = 3.1415926535897932384626433832795;

        cosine_theta = R / sqrt(pow(-Q, 3));
        theta = acos(cosine_theta);
        x[0].a = 2 * sqrt(-Q) * cos(theta/3 +   0*pi/180) - p/3;
        x[1].a = 2 * sqrt(-Q) * cos(theta/3 + 120*pi/180) - p/3;
        x[2].a = 2 * sqrt(-Q) * cos(theta/3 + 240*pi/180) - p/3;

        x[0].b
      = x[1].b
      = x[2].b = 0.;
    }
    else if (discriminant == 0)
    { /* three real roots of which two at least are equal */
        double A, B;

/*
 * b^2/4 + a^3/27 = 0 # And we know that (a <= 0) must also be true.
 * b^2/4          = -a^3/27
 * b/2            = sqrt(-27a^3)/27
 * cbrt(b/2)      = cbrt(sqrt(-27a^3))/3
 *                = (-3^3 * a^3)^(1/6) / 3 = ((-3a)^3)^(1/6) / 3
 *                = sqrt(-a / 3) = sqrt(-Q) (defined above)
 */
        A = B = sqrt(-Q);

        x[0].a = A + B;
        x[0].b = 0;

        x[1].a = -(A + B)/2;
        x[1].b = 0; /* = (A - B)/2 * sqrt(-3) = 0 * sqrt(-3) */

        x[2].a = -(A + B)/2;
        x[2].b = 0;
    }
    else
    { /* one real root and two conjugate imaginary roots */
        const double A = cbrt(R + sqrt(discriminant));
        const double B = cbrt(R - sqrt(discriminant));

        x[0].a = A + B;
        x[0].b = 0;

        x[1].a = -(A + B) / 2;
        x[1].b = +sqrt(3) * (A - B)/2;

        x[2].a = -(A + B) / 2;
        x[2].b = -sqrt(3) * (A - B)/2;
    }
    return (x);
}

static vector quartic_inverse(double a, double b, double c, double d, double e)
{
    vector x;

    if (a == 0.)
        return cubic_inverse(b, c, d, e);
    x = malloc(4 * sizeof(complex));

 /* ??? */

    fputs("Not yet implemented:  Quartic function inversion.\n", stderr);
    return (x);
}
