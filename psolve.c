#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <time.h>

static double intpow(double base, int power);
static double P_evaluate(double x, long * coefficients, int degree);
static int P_rational_roots(long * coefficients, int degree);
static int root_test(double x, long * coefficients, int degree);
static int is_integer(double x);
static int count_factors(long product);
static int enum_factors(long * factors, long product);
static int enum_ratios(double * ratios, long * num, long * den, int nN, int nD);
static void wall_clock_status(const char * title);

static int P_rational_roots(long * coefficients, int degree)
{
    double * possible_roots;
    double * roots_neg;
    long * constant_factors;
    long * leading_factors;
    int number_of_factors[2];
    int rational_number_solutions;
    register int i, j;
    const int n = degree;
    const long a0 = coefficients[n - 0];
    const long an = coefficients[n - n];

    puts("Factors of a0:");
    number_of_factors[0] = count_factors(a0);
    constant_factors = malloc(number_of_factors[0] * sizeof(long));
    enum_factors(constant_factors, a0);
    putchar('\n');

    puts("Factors of an:");
    number_of_factors[1] = count_factors(an);
    leading_factors = malloc(number_of_factors[1] * sizeof(long));
    enum_factors(leading_factors, an);
    putchar('\n');

    puts("Possible solutions:");
    rational_number_solutions = number_of_factors[0] * number_of_factors[1];
    possible_roots = malloc(rational_number_solutions * sizeof(double));
    enum_ratios(
        possible_roots,
        constant_factors, leading_factors,
        number_of_factors[0], number_of_factors[1]
    );
    putchar('\n');

    free(constant_factors);
    free(leading_factors);
    roots_neg = malloc(rational_number_solutions * sizeof(double));

/*
 * Because even positive integers can be factored into either both positive
 * or both negative factors, each potential root has a negative counterpart.
 */
    for (i = 0; i < rational_number_solutions; i++)
        roots_neg[i] = -possible_roots[i];

/*
 * Multiply the candidate by 1 (true) if it is, indeed, a polynomial root.
 * Multiply the candidate by 0 if it is not, as 0 is never a possible root
 * for polynomials adhering to the restrictions of the rational root theorem.
 */
    for (i = 0; i < rational_number_solutions; i++)
        roots_neg[i]
         *= (double)root_test(roots_neg[i], coefficients, degree);
    for (i = 0; i < rational_number_solutions; i++)
        possible_roots[i]
         *= (double)root_test(possible_roots[i], coefficients, degree);

    putchar('\n');
    wall_clock_status("Solve time");
    j = 0;
    for (i = 0; i < rational_number_solutions; i++)
    {
        if (roots_neg[i] != 0.)
        {
            printf("x = %g\n", roots_neg[i]);
            ++j;
        }
        if (possible_roots[i] != 0.)
        {
            printf("x = %g\n", possible_roots[i]);
            ++j;
        }
    }
    return (j);
}

int main(int argc, char ** argv)
{
    long * coefficients;
    int n, terms;
    register int i;

    terms = argc - 1; /* e.g., `psolve 1 0 3`:  x^2 + 0x + 3 = 0, so 3 terms. */
    n = terms - 1; /* e.g., `x^2 + 0x^1 + 3 = 0, so a_n is a_2 or degree = 2. */
    if (n < 1)
    {
        fputs("Not a solvable polynomial.\n", stderr);
        return 1;
    }

    coefficients = malloc(sizeof(long) * terms);
    for (i = 0; i < terms; i++)
        coefficients[i] = strtol(argv[i + 1], NULL, 0);

    if (coefficients[n - n] == 0)
    {
        fprintf(
            stderr,
            "Not a standard polynomial:  %s\n"\
            "Try again without the \"%s\" at the beginning.\n",
            "Lead coefficient must be a real, nonzero number.",
            argv[0 + 1]
        );
        return -1;
    }
    if (coefficients[n - 0] == 0)
    {
        fprintf(
            stderr,
            "Not a standard polynomial:  %s\n"\
            "Try again without the \"%s\" at the end.\n",
            "Constant term must be a real, nonzero number.",
            argv[n - 1 + 1]
        );
        return 3;
    }

/*
 * Prefer to print "3x^2 + 4x - 1 = 0"
 * than to print   "-3x^2 - 4x + 1 = 0".
 *
 * Besides, negative fractions expressed as ratios, generally have the
 * numerator written as the negative, not the divisor, and the rational root
 * theorem requires dividing the constant term by the leading coefficient.
 */
    if (coefficients[n - n] < 0)
        for (i = 0; i < terms; i++)
            coefficients[i] = -coefficients[i];

    puts("Now solving polynomial function for x, when f(x) = 0:");
    if (coefficients[n - n] == 1)
        { /* branch unlikely */ }
    else
        printf("%lu", coefficients[n - n]);
    putchar('x');
    if (n > 1)
        printf("^%u", n);
    putchar(' ');
    for (i = 0 + 1; i < terms - 1; i++)
    {
        if (coefficients[i] == 0)
            continue;

        if (coefficients[i] == -1)
            fputs("+ -x", stdout);
        else if (coefficients[i] == +1)
            fputs("+ x", stdout);
        else
            printf("+ %ldx", coefficients[i]);

        if (n - i > 1)
            printf("^%u", n - i);
        putchar(' ');
    }
    printf("+ %ld = 0", coefficients[n]);
    putchar('\n');
    wall_clock_status("Time spent decoding polynomial");
    putchar('\n');

    i = P_rational_roots(coefficients, n);
    printf("Total solutions found:  %i\n", i);
    return 0;
}

static double intpow(double base, int power)
{
    register double answer;

    if (power < 0)
    {
        power = -power;
        base = 1 / base;
    }
    answer = 1.;
    while (power-- > 0)
        answer *= base;
    return (answer);
}

static int count_factors(long product)
{
    register int number_of_factors;
    register long factor;

    number_of_factors = 0;
    if (product < 0)
        product = -product;
    for (factor = 1; factor - 1 < product; factor++)
        number_of_factors += is_integer((double)product / (double)factor);
    return (number_of_factors);
}

static int enum_factors(long * factors, long product)
{
    register int number_of_factors;
    register double factor, product_f;

    number_of_factors = 0;
    if (product < 0)
        product = -product;
    product_f = (double)product;
    putchar('{');
    for (factor = 1.; factor <= product_f; factor += 1.)
    {
        const double ratio = product_f / factor;
        const int whole_number = is_integer(ratio);

        if (whole_number == 0)
            continue;
        if (number_of_factors > 0)
            fputs(", ", stdout);
        factors[number_of_factors] = (long)ratio;
        printf("%ld", factors[number_of_factors]);
        ++number_of_factors;
    }
    putchar('}');
    putchar('\n');
    return (number_of_factors);
}

static int enum_ratios(double * ratios, long * num, long * den, int nN, int nD)
{
    register int i, j, k, l, m;

    m = k = 0;
    for (j = 0; j < nD; j++)
        for (i = 0; i < nN; i++)
        {
            ratios[k] = (double)num[i] / (double)den[j];
#if 1
/*
 * Take the time to eliminate duplicate entries of the same ratio.
 * If the search finds that the possible root was already stored earlier,
 * it overwrites this value with the special value of 0, as a polynomial with
 * a nonzero constant term cannot have 0 as a root solution.
 */
            for (l = 0; l < k; l++)
            {
                if (ratios[k] == ratios[l])
                    ratios[k] = 0.;
                ++m;
            }
#endif
            ++k;
        }

    fputs("+/-", stdout);
    putchar('{');
    for (i = 0, j = 0; i < k; i++)
    {
        if (ratios[i] == 0.)
            continue;
        if (j > 0)
            fputs(", ", stdout);
        j = 1;
        printf("%g", ratios[i]);
    }
    putchar('}');
    putchar('\n');

#if 1
    return (k - m);
#else
    return (k);
#endif
}

/*
 * Evalute polynomial function P(x).
 *
 * Example:  P(x) = x^2 + 3x + 9
 *     coefficients[0] =  1;
 *     coefficients[1] =  3;
 *     coefficients[2] =  9;
 *     call:  P_evalute(x, coefficients, 2);
 */
static double P_evaluate(double x, long * coefficients, int degree)
{
    double result;
    register int i;

    result = 0.;
    for (i = 0; i < degree + 1; i++)
        result += (double)coefficients[i] * intpow(x, degree - i);
    return (result);
}

static int root_test(double x, long * coefficients, int degree)
{
    double result;
    register int i;

    if (x == 0.)
        return 0; /* 0 is never a possible root when the a_0 term is nonzero. */
#ifdef TODO_TRY_TESTING_WITH_SYNTHETIC_DIVISION_TABLES_INSTEAD
 /* ??? */

/* Synthetic division is a less definitive bypass but computes much faster
 * than squaring x, cubing x, and having to do other higher-power exponents
 * when solving incredibly complex polynomials which the user might try to
 * task this program with.  I will see about implementing this performance
 * saver shortcut later on if the solving process starts to make us wait.
 */
#else
    result = P_evaluate(x, coefficients, degree);
#endif
    i = printf("f(x = %.4g)", x);
    if (i < 16)
        putchar('\t');
    printf("= %.8g\n", result);
    return (result == 0.);
}

static void wall_clock_status(const char * title)
{
    static clock_t t1;
    clock_t t2;
    double seconds;

    t2 = clock();
    seconds = (double)(t2 - t1) / (double)CLOCKS_PER_SEC;
    printf("%s:  %f s (%lu cycles)\n", title, seconds, t2 - t1);
    t1 = t2;
    return;
}

static int is_integer(double x)
{
    const long as_integer = (long)x;
    const double as_fp = (double)as_integer;

    return (x == as_fp);
}
