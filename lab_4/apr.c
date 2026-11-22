#include <stdio.h>
#include <gsl/gsl_chebyshev.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>

#define SAMPLE_COUNT 4001

typedef struct
{
  double amplitude;
  double frequency;
} oscillatory_params;

static double
exp_cos (double x, void *params)
{
  (void) params;
  return exp (x) * cos (0.5 * x * x);
}

static double
rational (double x, void *params)
{
  (void) params;
  return 1.0 / (12.0 * x * x + 1.0);
}

static double
oscillatory (double x, void *params)
{
  oscillatory_params *p = params;
  return cos (x) + p->amplitude * sin (p->frequency * x);
}

static int
write_data (const char *filename, gsl_function *function,
            double lower, double upper, const size_t *degrees,
            size_t degree_count, size_t max_degree)
{
  FILE *output = fopen (filename, "w");
  if (output == NULL)
    {
      perror (filename);
      return 1;
    }

  gsl_cheb_series *series = gsl_cheb_alloc (max_degree);
  if (series == NULL)
    {
      fprintf (stderr, "Nie mozna zaalokowac szeregu Czebyszewa.\n");
      fclose (output);
      return 1;
    }

  int status = gsl_cheb_init (series, function, lower, upper);
  if (status != GSL_SUCCESS)
    {
      fprintf (stderr, "Blad inicjalizacji aproksymacji: %s\n",
               gsl_strerror (status));
      gsl_cheb_free (series);
      fclose (output);
      return 1;
    }

  for (size_t i = 0; i < SAMPLE_COUNT; ++i)
    {
      double x = lower + (upper - lower) * i / (SAMPLE_COUNT - 1.0);
      if (fprintf (output, "%.17g %.17g", x, GSL_FN_EVAL (function, x)) < 0)
        {
          perror (filename);
          gsl_cheb_free (series);
          fclose (output);
          return 1;
        }

      for (size_t j = 0; j < degree_count; ++j)
        if (fprintf (output, " %.17g",
                     gsl_cheb_eval_n (series, degrees[j], x)) < 0)
          {
            perror (filename);
            gsl_cheb_free (series);
            fclose (output);
            return 1;
          }

      if (fputc ('\n', output) == EOF)
        {
          perror (filename);
          gsl_cheb_free (series);
          fclose (output);
          return 1;
        }
    }

  gsl_cheb_free (series);
  if (fclose (output) != 0)
    {
      perror (filename);
      return 1;
    }
  return 0;
}

int
main (void)
{
  const size_t standard_degrees[] = { 3, 10, 40 };
  const size_t oscillatory_degrees[] = { 40, 256 };
  oscillatory_params params = { 0.05, 25.0 };

  gsl_function function;

  function.function = exp_cos;
  function.params = NULL;
  if (write_data ("exp_cos.dat", &function, 0.0, 3.0,
                  standard_degrees, 3, 40) != 0)
    return 1;

  function.function = rational;
  if (write_data ("rational.dat", &function, -2.0, 2.0,
                  standard_degrees, 3, 40) != 0)
    return 1;

  function.function = oscillatory;
  function.params = &params;
  if (write_data ("oscillatory.dat", &function, 0.0, 10.0,
                  oscillatory_degrees, 2, 256) != 0)
    return 1;

  printf ("Wygenerowano pliki exp_cos.dat, rational.dat i oscillatory.dat.\n");
  printf ("Dla funkcji dodatkowej: A = %.2f, B = %.1f.\n",
          params.amplitude, params.frequency);
  return 0;
}
