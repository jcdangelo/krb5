/*
 * $Source$
 * $Author$
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * For copying and distribution information, please see the file
 * <krb5/copyright.h>.
 *
 * Routine to automatically generate a subsession key based on an input key.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_gen_subkey_c[] =
"$Id$";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

krb5_error_code
krb5_generate_subkey(key, subkey)
const krb5_keyblock *key;
krb5_keyblock **subkey;
{
    krb5_pointer random_state;
    krb5_encrypt_block eblock;
    krb5_error_code retval;

    if (!valid_keytype(key->keytype))
	return KRB5_PROG_KEYTYPE_NOSUPP;

    krb5_use_keytype(&eblock, key->keytype);

    if (retval = krb5_init_random_key(&eblock, key, &random_state))
	return(retval);
    *subkey = (krb5_keyblock *)malloc(sizeof(**subkey));
    if (!*subkey) {
	(void) krb5_finish_random_key(&eblock, random_state);
	return ENOMEM;
    }
	
    if (retval = krb5_random_key(&eblock, random_state, subkey)) {
	(void) krb5_finish_random_key(&eblock, random_state);
	xfree(*subkey);
	return retval;
    }	
    /* ignore the error if any, since we've already gotten the key out */
    (void) krb5_finish_random_key(&eblock, &random_state);
    return 0;
}

