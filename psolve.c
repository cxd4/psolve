#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <time.h>

static int number_of_coefficients;
static long * coefficients;

static double intpow(double base, int power);
static int is_integer(double x);
static int solve_polynomial(long a_n, long a_0);
static int count_factors(long product);
static int enum_ratios(double * ratios, long * num, long * den, int nN, int nD);
static int enum_factors(long * factors, long product);
static int root_test(double x);
static void wall_clock_status(const char * title);

static int solve_polynomial(long a_n, long a_0)
{
    double * possible_roots;
    double * roots_neg;
    long * constant_factors;
    long * leading_factors;
    int number_of_factors[2];
    int rational_number_solutions;
    register int i, j;

    puts("Factors of a0:");
    number_of_factors[0] = count_factors(a_0);
    constant_factors = malloc(number_of_factors[0] * sizeof(long));
    enum_factors(constant_factors, a_0);
    putchar('\n');

    puts("Factors of an:");
    number_of_factors[1] = count_factors(a_n);
    leading_factors = malloc(number_of_factors[1] * sizeof(long));
    enum_factors(leading_factors, a_n);
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
        roots_neg[i] *= (double)root_test(roots_neg[i]);
    for (i = 0; i < rational_number_solutions; i++)
        possible_roots[i] *= (double)root_test(possible_roots[i]);

    putchar('\n');
    wall_clock_status("Solve time");
    j = 0;
    for (i = 0; i < rational_number_solutions; i++)
    {
        if (roots_neg[i] != 0.)
        {
            printf("x = %f\n", roots_neg[i]);
            ++j;
        }
        if (possible_roots[i] != 0.)
        {
            printf("x = %f\n", possible_roots[i]);
            ++j;
        }
    }
    return (j);
}

int main(int argc, char ** argv)
{
    register int i;

    number_of_coefficients = argc - 1; /* arg[0] always command path */
    if (number_of_coefficients < 2)
    {
        fputs("Not a solvable polynomial.\n", stderr);
        return 1;
    }

    coefficients = malloc(sizeof(long) * number_of_coefficients);
    for (i = number_of_coefficients; i > 0; i--)
        coefficients[i - 1] = strtol(argv[i], NULL, 0);

    if (coefficients[0] == 0)
    {
        fprintf(
            stderr,
            "Not a standard polynomial:  %s\n"\
            "Try again without the \"%s\" at the beginning.\n",
            "Lead coefficient must be a real, nonzero number.",
            argv[0 + 1]
        );
        return 2;
    }
    if (coefficients[number_of_coefficients - 1] == 0)
    {
        fprintf(
            stderr,
            "Not a standard polynomial:  %s\n"\
            "Try again without the \"%s\" at the end.\n",
            "Constant term must be a real, nonzero number.",
            argv[number_of_coefficients - 1 + 1]
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
    if (coefficients[0] < 0)
        for (i = 0; i < number_of_coefficients; i++)
            coefficients[i] = -coefficients[i];

    puts("Now solving polynomial function for x, when f(x) = 0:");
    if (coefficients[0] == 1)
        { /* branch unlikely */ }
    else
        printf("%lu", coefficients[0]);
    putchar('x');
    if (number_of_coefficients - 1 > 1)
        printf("^%u", number_of_coefficients - 1);
    putchar(' ');
    for (i = 1; i < number_of_coefficients - 1; i++)
    {
        const unsigned int current_power = number_of_coefficients - i - 1;

        if (coefficients[i] == 0)
            continue;

        if (coefficients[i] == -1)
            fputs("+ -x", stdout);
        else if (coefficients[i] == +1)
            fputs("+ x", stdout);
        else
            printf("+ %ldx", coefficients[i]);

        if (current_power > 1)
            printf("^%u", current_power);
        putchar(' ');
    }
    printf("+ %ld = 0", coefficients[i]);
    putchar('\n');
    wall_clock_status("Time spent decoding polynomial");
    putchar('\n');

    i = solve_polynomial(
        coefficients[0],
        coefficients[number_of_coefficients - 1]
    );
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

    putchar('{');
    for (i = 0, j = 0; i < k; i++)
    {
        if (ratios[i] == 0.)
            continue;
        if (j > 0)
            fputs(", ", stdout);
        j = 1;
        printf("+/-%f", ratios[i]);
    }
    putchar('}');
    putchar('\n');

#if 1
    return (k - m);
#else
    return (k);
#endif
}

static int root_test(double x)
{
    double result;
    register int i;

    result = 0.;
    for (i = 0; i < number_of_coefficients; i++)
        result += coefficients[i] * intpow(x, number_of_coefficients - 1 - i);
    printf("f(x = %f) = %f\n", x, result);
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
