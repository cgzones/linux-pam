/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, and the entire permission notice in its entirety,
 *    including the disclaimer of warranties.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * ALTERNATIVELY, this product may be distributed under the terms of
 * the GNU Public License, in which case the provisions of the GPL are
 * required INSTEAD OF the above restrictions.  (This clause is
 * necessary due to a potential bad interaction between the GPL and
 * the restrictions contained in a BSD-style copyright.)
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <config.h>

#include <stdio.h>
#include <string.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>

static struct pam_conv conv = {
    misc_conv,
    NULL
};

static int debug = 0;

/*
  https://bugzilla.redhat.com/bugzilla/show_bug.cgi?id=196859

  This stack should not return PAM_IGNORE to the application:
  auth [default=bad] pam_debug.so auth=ignore
*/
static int
test1 (void)
{
  pam_handle_t *pamh=NULL;
  const char *user="nobody";
  int retval;

  retval = pam_start("tst-pam_dispatch1", user, &conv, &pamh);
  if (retval != PAM_SUCCESS)
    {
      if (debug)
	fprintf (stderr, "test1: pam_start returned %d\n", retval);
      return 1;
    }

  retval = pam_authenticate(pamh, 0);
  if (retval != PAM_PERM_DENIED)
    {
      if (debug)
	fprintf (stderr, "test1: pam_authenticate returned %d\n", retval);
      return 1;
    }

  retval = pam_end(pamh,retval);
  if (retval != PAM_SUCCESS)
    {
      if (debug)
	fprintf (stderr, "test1: pam_end returned %d\n", retval);
      return 1;
    }
  return 0;
}


int main(int argc, char *argv[])
{
    if (argc > 1 && strcmp (argv[1], "-d") == 0)
      debug = 1;

    if (test1 ())
      return 1;

    return 0;
}
